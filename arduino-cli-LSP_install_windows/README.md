# Arduino Tools Installer for Windows 11

`install-arduino-tools.ps1` installs and configures the tools required to use Arduino sketches and the Arduino Language Server on Windows 11.

## What It Installs

The script downloads these pinned official releases:

- Arduino CLI 1.5.1
- Arduino Language Server 0.7.7
- clangd 22.1.6

The executables are installed in:

```text
%LOCALAPPDATA%\Programs\ArduinoTools\bin
```

The script adds this directory to the current user's `PATH`. Open a new terminal after installation to load the updated `PATH`.

It also configures Arduino CLI by:

- Creating `%LOCALAPPDATA%\Arduino15\arduino-cli.yaml` when it does not exist.
- Adding the official Espressif ESP32 board index.
- Updating the Arduino package indexes.
- Installing the Arduino AVR Boards core.
- Installing the Espressif ESP32 core.
- Installing the Stepper, Servo, and ESP32Servo libraries.
- Generating and enabling `arduino-cli` completion for Windows PowerShell and PowerShell 7.
- Verifying both supported board FQBNs and all installed executables.

Downloaded Arduino CLI and Arduino Language Server archives are checked against their official SHA-256 manifests. The clangd archive is checked against the SHA-256 digest published with its GitHub release.

## Requirements

- 64-bit Windows 11
- Windows PowerShell 5.1 or PowerShell 7+
- An internet connection
- No administrator privileges are required for the default installation directory

## Run the Installer

From the root of this Neovim configuration, use Windows PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\install-arduino-tools.ps1
```

Or use PowerShell 7:

```powershell
pwsh -File .\scripts\install-arduino-tools.ps1
```

The script is idempotent. Running it again updates the pinned executables and confirms that the requested cores and libraries are installed.

## PowerShell Completion

The installer generates the completion script at:

```text
%LOCALAPPDATA%\Programs\ArduinoTools\completions\arduino-cli.ps1
```

It adds an idempotent, marked loader block to both supported profile locations:

```text
%USERPROFILE%\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1
%USERPROFILE%\Documents\PowerShell\Microsoft.PowerShell_profile.ps1
```

This enables argument completion for Arduino CLI commands, subcommands, and flags. Open a new PowerShell window after installation, type a partial command such as `arduino-cli bo`, and press `Tab` to use it.

## Options

Skip installation of board cores and Arduino libraries while still installing and verifying the three executables:

```powershell
pwsh -File .\scripts\install-arduino-tools.ps1 -SkipArduinoPackages
```

Override the pinned tool versions:

```powershell
pwsh -File .\scripts\install-arduino-tools.ps1 `
    -ArduinoCliVersion "1.5.1" `
    -ArduinoLanguageServerVersion "0.7.7" `
    -ClangdVersion "22.1.6"
```

Override the installation directory:

```powershell
pwsh -File .\scripts\install-arduino-tools.ps1 `
    -InstallRoot "$env:LOCALAPPDATA\Programs\ArduinoTools"
```

## Board Selection

The Neovim configuration uses `default_fqbn` from the sketch's `sketch.yaml` file. For an Arduino Mega 2560:

```yaml
default_fqbn: arduino:avr:mega
```

For an ESP32 Dev Module:

```yaml
default_fqbn: esp32:esp32:esp32
```

If `sketch.yaml` does not define a board, Neovim uses the `ARDUINO_FQBN` environment variable and then falls back to `esp32:esp32:esp32`.

After changing the board, restart the Arduino language server or reopen Neovim so it starts with the new FQBN.
