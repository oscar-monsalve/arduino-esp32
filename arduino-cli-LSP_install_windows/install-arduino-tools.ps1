<#
.SYNOPSIS
Installs the Arduino CLI, Arduino Language Server, clangd, board cores, and libraries on Windows 11.

.EXAMPLE
powershell -ExecutionPolicy Bypass -File .\scripts\install-arduino-tools.ps1

.EXAMPLE
pwsh -File .\scripts\install-arduino-tools.ps1 -SkipArduinoPackages
#>

[CmdletBinding()]
param(
    [string]$ArduinoCliVersion = "1.5.1",
    [string]$ArduinoLanguageServerVersion = "0.7.7",
    [string]$ClangdVersion = "22.1.6",
    [string]$InstallRoot = (Join-Path $env:LOCALAPPDATA "Programs\ArduinoTools"),
    [switch]$SkipArduinoPackages
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

if (-not [Environment]::Is64BitOperatingSystem) {
    throw "This installer supports 64-bit Windows only."
}

$Esp32IndexUrl = "https://espressif.github.io/arduino-esp32/package_esp32_index.json"
$BinDir = Join-Path $InstallRoot "bin"
$TempDir = Join-Path ([IO.Path]::GetTempPath()) ("arduino-tools-" + [guid]::NewGuid())
$CliPath = Join-Path $BinDir "arduino-cli.exe"
$LanguageServerPath = Join-Path $BinDir "arduino-language-server.exe"
$ClangdPath = Join-Path $BinDir "clangd.exe"
$CompletionPath = Join-Path $InstallRoot "completions\arduino-cli.ps1"

function Write-Step([string]$Message) {
    Write-Host "`n==> $Message"
}

function Get-Download([string]$Uri, [string]$Destination) {
    Invoke-WebRequest -UseBasicParsing -Uri $Uri -OutFile $Destination
}

function Assert-FileHash(
    [string]$Path,
    [string]$ExpectedHash
) {
    $actualHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $Path).Hash
    if ($actualHash -ne $ExpectedHash) {
        throw "SHA-256 mismatch for $(Split-Path -Leaf $Path). Expected $ExpectedHash, got $actualHash."
    }
}

function Get-ManifestHash(
    [string]$ManifestPath,
    [string]$ArchiveName
) {
    $escapedName = [regex]::Escape($ArchiveName)
    $line = Get-Content -LiteralPath $ManifestPath | Where-Object { $_ -match "^[0-9a-fA-F]{64}\s+\*?$escapedName$" }
    if (-not $line) {
        throw "No checksum found for $ArchiveName."
    }

    return ($line -split "\s+")[0]
}

function Install-CheckedArchive(
    [string]$Name,
    [string]$ArchiveUrl,
    [string]$ChecksumUrl,
    [string]$ExecutableName,
    [string]$Destination
) {
    $archiveName = Split-Path -Leaf $ArchiveUrl
    $archivePath = Join-Path $TempDir $archiveName
    $manifestPath = Join-Path $TempDir "$Name-checksums.txt"
    $extractPath = Join-Path $TempDir "$Name-extracted"

    Write-Step "Downloading $Name"
    Get-Download $ArchiveUrl $archivePath
    Get-Download $ChecksumUrl $manifestPath
    Assert-FileHash $archivePath (Get-ManifestHash $manifestPath $archiveName)

    Expand-Archive -LiteralPath $archivePath -DestinationPath $extractPath -Force
    $executable = Get-ChildItem -LiteralPath $extractPath -Filter $ExecutableName -File -Recurse | Select-Object -First 1
    if (-not $executable) {
        throw "$ExecutableName was not found in $archiveName."
    }
    Copy-Item -LiteralPath $executable.FullName -Destination $Destination -Force
}

function Install-Clangd {
    $archiveName = "clangd-windows-$ClangdVersion.zip"
    $releaseUrl = "https://api.github.com/repos/clangd/clangd/releases/tags/$ClangdVersion"
    $release = Invoke-RestMethod -UseBasicParsing -Uri $releaseUrl
    $asset = $release.assets | Where-Object { $_.name -eq $archiveName } | Select-Object -First 1
    if (-not $asset -or -not $asset.digest -or $asset.digest -notmatch "^sha256:(.+)$") {
        throw "The GitHub release does not provide a SHA-256 digest for $archiveName."
    }

    $archivePath = Join-Path $TempDir $archiveName
    $extractPath = Join-Path $TempDir "clangd-extracted"
    Write-Step "Downloading clangd $ClangdVersion"
    Get-Download $asset.browser_download_url $archivePath
    Assert-FileHash $archivePath $Matches[1]

    Expand-Archive -LiteralPath $archivePath -DestinationPath $extractPath -Force
    $executable = Get-ChildItem -LiteralPath $extractPath -Filter "clangd.exe" -File -Recurse | Select-Object -First 1
    if (-not $executable) {
        throw "clangd.exe was not found in $archiveName."
    }
    Copy-Item -LiteralPath $executable.FullName -Destination $ClangdPath -Force
}

function Add-UserPath([string]$Path) {
    $userPath = [Environment]::GetEnvironmentVariable("Path", "User")
    $entries = @($userPath -split ";" | Where-Object { $_ })
    if ($entries -notcontains $Path) {
        $newPath = (@($Path) + $entries) -join ";"
        [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    }
    if (($env:Path -split ";") -notcontains $Path) {
        $env:Path = "$Path;$env:Path"
    }
}

function Invoke-ArduinoCli([string[]]$Arguments) {
    for ($attempt = 1; $attempt -le 3; $attempt++) {
        & $CliPath @Arguments
        $exitCode = $LASTEXITCODE
        if ($exitCode -eq 0) {
            return
        }
        if ($attempt -lt 3) {
            Write-Warning "arduino-cli failed (attempt $attempt of 3); retrying in 3 seconds."
            Start-Sleep -Seconds 3
        }
    }

    throw "arduino-cli $($Arguments -join ' ') failed with exit code $exitCode."
}

function Install-PowerShellCompletion {
    Write-Step "Generating Arduino CLI PowerShell completion"
    New-Item -ItemType Directory -Path (Split-Path -Parent $CompletionPath) -Force | Out-Null
    Invoke-ArduinoCli @("completion", "powershell") |
        Set-Content -LiteralPath $CompletionPath -Encoding utf8

    if (-not (Test-Path -LiteralPath $CompletionPath) -or (Get-Item -LiteralPath $CompletionPath).Length -eq 0) {
        throw "Arduino CLI generated an empty PowerShell completion script."
    }

    [Environment]::SetEnvironmentVariable("ARDUINO_TOOLS_HOME", $InstallRoot, "User")
    $env:ARDUINO_TOOLS_HOME = $InstallRoot

    $profileMarker = "# >>> Arduino CLI completion >>>"
    $profileBlock = @(
        $profileMarker
        '$arduinoToolsHome = [Environment]::GetEnvironmentVariable("ARDUINO_TOOLS_HOME", "User")'
        'if ($arduinoToolsHome) {'
        '    . (Join-Path $arduinoToolsHome "completions\arduino-cli.ps1")'
        '}'
        "# <<< Arduino CLI completion <<<"
    ) -join [Environment]::NewLine

    $documents = [Environment]::GetFolderPath([Environment+SpecialFolder]::MyDocuments)
    $profilePaths = @(
        (Join-Path $documents "WindowsPowerShell\Microsoft.PowerShell_profile.ps1")
        (Join-Path $documents "PowerShell\Microsoft.PowerShell_profile.ps1")
    ) | Sort-Object -Unique

    foreach ($profilePath in $profilePaths) {
        New-Item -ItemType Directory -Path (Split-Path -Parent $profilePath) -Force | Out-Null
        if (-not (Test-Path -LiteralPath $profilePath)) {
            New-Item -ItemType File -Path $profilePath -Force | Out-Null
        }

        $profileText = [IO.File]::ReadAllText($profilePath)
        if (-not $profileText.Contains($profileMarker)) {
            if ($profileText.Length -gt 0) {
                Add-Content -LiteralPath $profilePath -Value ""
            }
            Add-Content -LiteralPath $profilePath -Value $profileBlock
        }
    }
}

function Initialize-ArduinoCli {
    $configPath = Join-Path $env:LOCALAPPDATA "Arduino15\arduino-cli.yaml"
    if (-not (Test-Path -LiteralPath $configPath)) {
        Write-Step "Creating $configPath"
        Invoke-ArduinoCli @("config", "init")
    }

    $additionalUrls = (Invoke-ArduinoCli @("config", "get", "board_manager.additional_urls")) -join "`n"
    if ($additionalUrls -notmatch [regex]::Escape($Esp32IndexUrl)) {
        Write-Step "Adding the official Espressif board index"
        Invoke-ArduinoCli @("config", "add", "board_manager.additional_urls", $Esp32IndexUrl)
    }

    Write-Step "Updating package indexes"
    Invoke-ArduinoCli @("core", "update-index")

    Write-Step "Installing Arduino AVR and Espressif ESP32 board cores"
    Invoke-ArduinoCli @("core", "install", "arduino:avr")
    Invoke-ArduinoCli @("core", "install", "esp32:esp32")

    Write-Step "Installing Arduino libraries"
    foreach ($library in @("Stepper", "Servo", "ESP32Servo")) {
        Invoke-ArduinoCli @("lib", "install", $library)
    }
}

try {
    New-Item -ItemType Directory -Path $BinDir -Force | Out-Null
    New-Item -ItemType Directory -Path $TempDir -Force | Out-Null

    $cliArchive = "arduino-cli_${ArduinoCliVersion}_Windows_64bit.zip"
    Install-CheckedArchive `
        "Arduino CLI $ArduinoCliVersion" `
        "https://github.com/arduino/arduino-cli/releases/download/v$ArduinoCliVersion/$cliArchive" `
        "https://github.com/arduino/arduino-cli/releases/download/v$ArduinoCliVersion/$ArduinoCliVersion-checksums.txt" `
        "arduino-cli.exe" `
        $CliPath

    $languageServerArchive = "arduino-language-server_${ArduinoLanguageServerVersion}_Windows_64bit.zip"
    Install-CheckedArchive `
        "Arduino Language Server $ArduinoLanguageServerVersion" `
        "https://github.com/arduino/arduino-language-server/releases/download/$ArduinoLanguageServerVersion/$languageServerArchive" `
        "https://github.com/arduino/arduino-language-server/releases/download/$ArduinoLanguageServerVersion/$ArduinoLanguageServerVersion-checksums.txt" `
        "arduino-language-server.exe" `
        $LanguageServerPath

    Install-Clangd
    Add-UserPath $BinDir
    Install-PowerShellCompletion

    if (-not $SkipArduinoPackages) {
        Initialize-ArduinoCli
    }

    Write-Step "Verifying the installation"
    Invoke-ArduinoCli @("version")
    & $ClangdPath --version | Select-Object -First 1
    if ($LASTEXITCODE -ne 0) {
        throw "clangd verification failed."
    }
    & $LanguageServerPath -h *> $null
    if ($LASTEXITCODE -ne 0) {
        throw "Arduino Language Server verification failed."
    }

    if (-not $SkipArduinoPackages) {
        Invoke-ArduinoCli @("board", "details", "-b", "arduino:avr:mega") | Out-Null
        Invoke-ArduinoCli @("board", "details", "-b", "esp32:esp32:esp32") | Out-Null
    }

    Write-Host "`nInstallation complete."
    Write-Host "Tools: $BinDir"
    Write-Host "PowerShell completion: $CompletionPath"
    Write-Host "Mega 2560 FQBN: arduino:avr:mega"
    Write-Host "ESP32 Dev Module FQBN: esp32:esp32:esp32"
    Write-Host "Open a new terminal before using the updated user PATH."
}
finally {
    if (Test-Path -LiteralPath $TempDir) {
        Remove-Item -LiteralPath $TempDir -Recurse -Force
    }
}
