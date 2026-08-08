# Arduino Mega 2560 and ESP32 Development

This repository contains Arduino sketches and the setup used to develop them
with Neovim, `arduino-language-server`, and `arduino-cli`.

## Supported boards

| Board | Platform core | FQBN | Typical port |
| --- | --- | --- | --- |
| Arduino Mega 2560 | `arduino:avr` | `arduino:avr:mega` | `/dev/ttyACM0` |
| ESP32 DevKit32, ESP32-WROOM-32D | `esp32:esp32` | `esp32:esp32:esp32` | `/dev/ttyUSB0` |

The ESP32 DevKit32 uses a Silicon Labs CP210x USB-to-serial interface. Linux
already includes the `cp210x` kernel driver, so no separate driver download is
needed. Depending on the exact chip revision, `lsusb` may identify it as a
CP2101, CP2102, or CP210x device.

## Automated installation

On an Omarchy or Arch Linux machine, run:

```bash
"./arduino-cli_&_LSP_install_script/install.sh"
```

The script installs and verifies:

- Arduino CLI 1.5.1
- Arduino Language Server 0.7.7
- clangd, when it is not already installed
- Arduino AVR Boards, including the Mega 2560
- Espressif ESP32 Boards
- The official Espressif stable package index
- Serial-port group membership on Arch Linux

Log out and back in if the script adds your user to the `uucp` or `lock`
groups.

## Manual platform setup

Initialize the Arduino CLI configuration if it does not exist:

```bash
arduino-cli config init
```

Add Espressif's official stable board index:

```bash
arduino-cli config add board_manager.additional_urls \
  https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Update indexes and install both board platforms:

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli core install esp32:esp32
arduino-cli core list
```

The ESP32 platform is large because it includes toolchains and support files
for the complete ESP32 family.

## Create a sketch

`arduino-cli sketch new` is the project initialization command. For example:

```bash
mkdir -p ~/Arduino
arduino-cli sketch new ~/Arduino/mega_blink
arduino-cli sketch new ~/Arduino/esp32_blink
```

Arduino requires the sketch directory and primary `.ino` file to have the same
name:

```text
mega_blink/
`-- mega_blink.ino
```

A file such as `~/Arduino/test.ino` is not a valid standalone sketch. It must
be `~/Arduino/test/test.ino`.

The sketch may be located anywhere on the filesystem. Its parent directory is
used as the language-server and Arduino CLI project root. For example, this is
valid:

```text
/home/om/personal/coding/arduino-esp32/examples/test/
`-- test.ino
```

When opening a brand-new `.ino` path in Neovim, write the file once so it
exists on disk. The Arduino LSP configuration retries automatically after the
first write. Using `arduino-cli sketch new <path>` avoids this distinction by
creating the directory and primary file before Neovim opens them.

Open a sketch in Neovim with:

```bash
nvim ~/Arduino/mega_blink/mega_blink.ino
```

## Select the board for Neovim LSP

The recommended setup stores the board in each project's `sketch.yaml` instead
of hardcoding one board globally in Neovim. Remove or comment both `-fqbn`
entries from the `cmd` table in
`~/.config/nvim/lua/om/lazy/lsp.lua`:

```lua
-- "-fqbn", "arduino:avr:mega",
-- "-fqbn", "esp32:esp32:esp32",
```

Then attach the appropriate board to each sketch:

```bash
arduino-cli board attach -b arduino:avr:mega ~/Arduino/mega_blink
arduino-cli board attach -b esp32:esp32:esp32 ~/Arduino/esp32_blink
```

This creates a `sketch.yaml` in each project. With no hardcoded `-fqbn`,
`arduino-language-server` asks Arduino CLI to use that project metadata. Every
sketch must have an attached board for the LSP to build its compilation
database.

Restart the Arduino LSP after changing a project's attached board. Restart
Neovim or run `:LspRestart arduino_language_server`. Running `:LspInfo` in an
`.ino` buffer should show `arduino_language_server` as an attached client.

Hardcoding one FQBN in `lsp.lua` remains available as a fallback, but it
overrides `sketch.yaml` for every Arduino project opened by that Neovim
configuration.

## Check whether a board is recognized

Connect the board with a USB data cable and run:

```bash
arduino-cli board list
```

An ESP32 with a CP210x interface normally appears as `/dev/ttyUSB0`. A genuine
Mega 2560 commonly appears as `/dev/ttyACM0`; some clones appear as
`/dev/ttyUSB0`.

Generic USB-to-serial devices may be listed by Arduino CLI as `Unknown`. This
does not prevent compiling or uploading when the correct port and FQBN are
provided explicitly.

To watch Linux detect a board, run this command before connecting it:

```bash
journalctl -kf
```

Other useful checks are:

```bash
lsusb
ls -l /dev/ttyUSB* /dev/ttyACM*
groups
```

On Arch Linux, fix serial-port permission errors with:

```bash
sudo usermod -aG uucp,lock "$USER"
```

Fully log out of the desktop session and log back in, then verify that both
groups are active:

```bash
id -nG
```

Do not use `chmod 666 /dev/ttyUSB0` as a permanent fix. Its permissions reset
when the device reconnects, and it grants access to every local user.

## Find board identifiers

List installed board definitions or inspect a specific board:

```bash
arduino-cli board listall "Mega 2560"
arduino-cli board listall "ESP32 Dev Module"
arduino-cli board details -b arduino:avr:mega
arduino-cli board details -b esp32:esp32:esp32
```

The generic ESP32 Dev Module FQBN is appropriate for the 30-pin DevKit32 with
an ESP32-WROOM-32D module. If a board is explicitly branded as a DOIT ESP32
DevKit V1, its alternative FQBN is `esp32:esp32:doitESP32devkitV1`.

## Save a board and port in a sketch

`board attach` creates or updates `sketch.yaml` with default values for that
project.

ESP32 example:

```bash
arduino-cli board attach \
  -b esp32:esp32:esp32 \
  -p /dev/ttyUSB0 \
  ~/Arduino/esp32_blink
```

Mega 2560 example:

```bash
arduino-cli board attach \
  -b arduino:avr:mega \
  -p /dev/ttyACM0 \
  ~/Arduino/mega_blink
```

Inspect the result with:

```bash
arduino-cli board attach ~/Arduino/esp32_blink
```

Using explicit `--fqbn` and `--port` arguments is still useful in scripts
because it makes the selected target unambiguous.

## Recommended workflow

Install the board platforms once, then use this process for each project.

1. Create the sketch:

   ```bash
   arduino-cli sketch new ~/Arduino/esp32_blink
   ```

2. Attach its board and, when known, its current port:

   ```bash
   arduino-cli board attach \
     -b esp32:esp32:esp32 \
     -p /dev/ttyUSB0 \
     ~/Arduino/esp32_blink
   ```

3. Keep both `-fqbn` entries commented in `lsp.lua`, open the primary sketch,
   and confirm the client with `:LspInfo`:

   ```bash
   nvim ~/Arduino/esp32_blink/esp32_blink.ino
   ```

4. Compile using the defaults from `sketch.yaml`:

   ```bash
   arduino-cli compile --warnings all ~/Arduino/esp32_blink
   ```

5. Upload using the saved FQBN and port:

   ```bash
   arduino-cli upload ~/Arduino/esp32_blink
   ```

6. To move the project to another board, attach the new FQBN and restart its
   LSP client:

   ```bash
   arduino-cli board attach -b arduino:avr:mega ~/Arduino/esp32_blink
   ```

For projects that move between computers or USB sockets, keep the FQBN in
`sketch.yaml` but update the port with another `board attach` command after
checking `arduino-cli board list`.

## Compile

Compilation does not require a connected board.

Compile for the Mega 2560:

```bash
arduino-cli compile \
  --fqbn arduino:avr:mega \
  --warnings all \
  ~/Arduino/mega_blink
```

Compile for the ESP32 DevKit32:

```bash
arduino-cli compile \
  --fqbn esp32:esp32:esp32 \
  --warnings all \
  ~/Arduino/esp32_blink
```

To keep build artifacts in a known directory:

```bash
arduino-cli compile \
  --fqbn esp32:esp32:esp32 \
  --output-dir ~/Arduino/esp32_blink/build \
  ~/Arduino/esp32_blink
```

## Upload

The most convenient command compiles and uploads in one operation.

ESP32 example:

```bash
arduino-cli compile \
  --upload \
  --port /dev/ttyUSB0 \
  --fqbn esp32:esp32:esp32 \
  --warnings all \
  ~/Arduino/esp32_blink
```

Mega 2560 example:

```bash
arduino-cli compile \
  --upload \
  --port /dev/ttyACM0 \
  --fqbn arduino:avr:mega \
  --warnings all \
  ~/Arduino/mega_blink
```

`arduino-cli upload` by itself does not compile. To upload a previously saved
build explicitly:

```bash
arduino-cli upload \
  --port /dev/ttyUSB0 \
  --fqbn esp32:esp32:esp32 \
  --input-dir ~/Arduino/esp32_blink/build \
  ~/Arduino/esp32_blink
```

If the ESP32 remains at `Connecting...`, hold its `BOOT` button, start the
upload, and release the button when writing begins.

## Serial monitor

For a sketch that calls `Serial.begin(115200)`, open the monitor with:

```bash
arduino-cli monitor \
  --port /dev/ttyUSB0 \
  --config baudrate=115200
```

Use `/dev/ttyACM0` for the Mega when that is its detected port. Exit the monitor
before uploading if the uploader reports that the port is busy.

## Libraries

Board platforms provide the core APIs. Additional sketch libraries can be
searched, installed, and listed with:

```bash
arduino-cli lib search "library name"
arduino-cli lib install "Library Name"
arduino-cli lib list
arduino-cli lib update-index
arduino-cli lib upgrade
```

No additional Arduino library is required for basic GPIO, serial, Wi-Fi, or
Bluetooth support on the ESP32; those APIs are included with `esp32:esp32`.

## Maintenance and troubleshooting

Update indexes and inspect available upgrades:

```bash
arduino-cli core update-index
arduino-cli core list
arduino-cli outdated
```

Useful diagnostics:

```bash
arduino-cli version
arduino-cli config dump --verbose
arduino-cli core list
arduino-cli board list
```

Common problems:

| Symptom | Check |
| --- | --- |
| No serial port | Use a USB data cable; inspect `journalctl -kf` and `lsusb` |
| Port permission denied | Add the user to `uucp,lock`, then log out and back in |
| ESP32 shown as `Unknown` | Use `/dev/ttyUSB0` and the explicit ESP32 FQBN |
| ESP32 stuck at `Connecting...` | Hold `BOOT` during connection |
| Language server uses wrong APIs | Select the matching FQBN in `lsp.lua` and restart Neovim |
| Language server does not attach | Confirm the matching `folder/folder.ino` sketch layout and run `:LspInfo` |

Official references:

- [Arduino CLI documentation](https://docs.arduino.cc/arduino-cli/)
- [Arduino Language Server](https://github.com/arduino/arduino-language-server)
- [Espressif Arduino ESP32 installation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
