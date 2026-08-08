#!/usr/bin/env bash

set -euo pipefail

ARDUINO_CLI_VERSION="${ARDUINO_CLI_VERSION:-1.5.1}"
ARDUINO_LS_VERSION="${ARDUINO_LS_VERSION:-0.7.7}"
ESP32_INDEX_URL="https://espressif.github.io/arduino-esp32/package_esp32_index.json"
BIN_DIR="${HOME}/.local/bin"
CLI_CONFIG="${HOME}/.arduino15/arduino-cli.yaml"

log() {
    printf '\n==> %s\n' "$1"
}

install_system_dependencies() {
    local packages=()

    command -v curl >/dev/null 2>&1 || packages+=(curl)
    command -v tar >/dev/null 2>&1 || packages+=(tar)
    command -v sha256sum >/dev/null 2>&1 || packages+=(coreutils)
    command -v clangd >/dev/null 2>&1 || packages+=(clang)

    if ((${#packages[@]} == 0)); then
        return
    fi

    if ! command -v pacman >/dev/null 2>&1; then
        printf 'Missing required commands. Install these packages first: %s\n' "${packages[*]}" >&2
        exit 1
    fi

    log "Installing required Arch Linux packages: ${packages[*]}"
    sudo pacman -S --needed --noconfirm "${packages[@]}"
}

arduino_ls_asset_for_arch() {
    case "$(uname -m)" in
        x86_64)
            printf 'Linux_64bit'
            ;;
        i386 | i486 | i586 | i686)
            printf 'Linux_32bit'
            ;;
        aarch64 | arm64)
            printf 'Linux_ARM64'
            ;;
        armv7l)
            printf 'Linux_ARMv7'
            ;;
        armv6l)
            printf 'Linux_ARMv6'
            ;;
        *)
            printf 'Unsupported architecture: %s\n' "$(uname -m)" >&2
            exit 1
            ;;
    esac
}

install_arduino_cli() {
    log "Installing Arduino CLI ${ARDUINO_CLI_VERSION} in ${BIN_DIR}"
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh \
        | BINDIR="${BIN_DIR}" sh -s "${ARDUINO_CLI_VERSION}"
}

install_arduino_language_server() (
    local platform archive base_url temp_dir

    platform="$(arduino_ls_asset_for_arch)"
    archive="arduino-language-server_${ARDUINO_LS_VERSION}_${platform}.tar.gz"
    base_url="https://github.com/arduino/arduino-language-server/releases/download/${ARDUINO_LS_VERSION}"
    temp_dir="$(mktemp -d)"
    trap 'rm -rf "${temp_dir}"' EXIT

    log "Downloading Arduino Language Server ${ARDUINO_LS_VERSION}"
    curl -fL "${base_url}/${archive}" -o "${temp_dir}/${archive}"
    curl -fL "${base_url}/${ARDUINO_LS_VERSION}-checksums.txt" \
        -o "${temp_dir}/checksums.txt"

    log "Verifying Arduino Language Server checksum"
    (
        cd "${temp_dir}"
        sha256sum --check --ignore-missing checksums.txt
    )

    tar -xzf "${temp_dir}/${archive}" -C "${BIN_DIR}" arduino-language-server
    chmod +x "${BIN_DIR}/arduino-language-server"
)

configure_arduino_cli() {
    if [[ ! -f "${CLI_CONFIG}" ]]; then
        log "Creating ${CLI_CONFIG}"
        arduino-cli config init
    fi

    if ! arduino-cli config get board_manager.additional_urls | grep -Fq "${ESP32_INDEX_URL}"; then
        log "Adding the official Espressif board index"
        arduino-cli config add board_manager.additional_urls "${ESP32_INDEX_URL}"
    fi

    log "Updating Arduino package indexes"
    arduino-cli core update-index

    log "Installing Arduino AVR Boards for the Mega 2560"
    arduino-cli core install arduino:avr

    log "Installing Espressif ESP32 Boards"
    arduino-cli core install esp32:esp32

    log "Installing Arduino libraries: Stepper, Servo, and ESP32Servo"
    arduino-cli lib install Stepper Servo ESP32Servo
}

configure_serial_permissions() {
    local groups_to_add=()

    getent group uucp >/dev/null 2>&1 && groups_to_add+=(uucp)
    getent group lock >/dev/null 2>&1 && groups_to_add+=(lock)

    if ((${#groups_to_add[@]} == 0)); then
        return
    fi

    local missing_groups=()
    local group
    for group in "${groups_to_add[@]}"; do
        if ! id -nG "${USER}" | tr ' ' '\n' | grep -Fxq "${group}"; then
            missing_groups+=("${group}")
        fi
    done

    if ((${#missing_groups[@]} == 0)); then
        return
    fi

    local group_list
    group_list="$(IFS=,; printf '%s' "${missing_groups[*]}")"
    log "Adding ${USER} to serial-port groups: ${group_list}"
    sudo usermod -aG "${group_list}" "${USER}"
    printf 'Log out and back in before uploading through a serial port.\n'
}

verify_installation() {
    log "Verifying the installation"
    arduino-cli version
    clangd --version | sed -n '1p'
    "${BIN_DIR}/arduino-language-server" -h >/dev/null 2>&1
    arduino-cli core list
    arduino-cli lib list
    arduino-cli board details -b arduino:avr:mega >/dev/null
    arduino-cli board details -b esp32:esp32:esp32 >/dev/null

    if command -v modinfo >/dev/null 2>&1 && modinfo cp210x >/dev/null 2>&1; then
        printf 'Linux CP210x USB-to-serial driver: available\n'
    else
        printf 'Note: could not verify the cp210x kernel module with modinfo.\n'
    fi
}

main() {
    if ((EUID == 0)); then
        printf 'Run this script as your regular user; it invokes sudo when needed.\n' >&2
        exit 1
    fi

    install_system_dependencies
    mkdir -p "${BIN_DIR}"
    export PATH="${BIN_DIR}:${PATH}"

    install_arduino_cli
    install_arduino_language_server
    configure_arduino_cli
    configure_serial_permissions
    verify_installation

    log "Installation complete"
    printf 'Mega 2560 FQBN: arduino:avr:mega\n'
    printf 'ESP32 DevKit32 FQBN: esp32:esp32:esp32\n'
    printf 'Connect a board and run: arduino-cli board list\n'
}

main "$@"
