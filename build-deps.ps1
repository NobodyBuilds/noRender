$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$deps = Join-Path $root 'build\_deps'
$glmDir = Join-Path $deps 'glm-1.0.2'
$glfwDir = Join-Path $deps 'glfw-3.4.bin.WIN64'
$gladDir = Join-Path $deps 'glad'
$pythonDir = Join-Path $deps 'python'

New-Item -ItemType Directory -Force -Path $deps | Out-Null

function Get-And-ExpandZip([string]$Url, [string]$ArchivePath, [string]$Destination, [string]$ExpectedFile) {
    if (Test-Path -LiteralPath $ExpectedFile) {
        return
    }

    Write-Host "Downloading $([IO.Path]::GetFileName($ArchivePath))..."
    Invoke-WebRequest -Uri $Url -OutFile $ArchivePath
    Expand-Archive -LiteralPath $ArchivePath -DestinationPath $Destination -Force
    Remove-Item -LiteralPath $ArchivePath -Force

    if (-not (Test-Path -LiteralPath $ExpectedFile)) {
        throw "The downloaded archive did not contain the expected file: $ExpectedFile"
    }
}

Get-And-ExpandZip `
    'https://github.com/g-truc/glm/archive/refs/tags/1.0.2.zip' `
    (Join-Path $deps 'glm-1.0.2.zip') `
    $deps `
    (Join-Path $glmDir 'glm\glm.hpp')

Get-And-ExpandZip `
    'https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip' `
    (Join-Path $deps 'glfw-3.4.bin.WIN64.zip') `
    $deps `
    (Join-Path $glfwDir 'include\GLFW\glfw3.h')

$gladSource = Join-Path $gladDir 'src\glad.c'
$gladHeader = Join-Path $gladDir 'include\glad\glad.h'
$gladPlatformHeader = Join-Path $gladDir 'include\KHR\khrplatform.h'
if (-not (Test-Path -LiteralPath $gladSource) -or -not (Test-Path -LiteralPath $gladHeader) -or -not (Test-Path -LiteralPath $gladPlatformHeader)) {
    $pythonLauncher = Get-Command 'py.exe' -ErrorAction SilentlyContinue
    if ($pythonLauncher) {
        $python = $pythonLauncher.Source
        $pythonPrefix = @('-3')
    } else {
        $pythonCommand = Get-Command 'python.exe' -ErrorAction SilentlyContinue
        if (-not $pythonCommand) {
            throw 'Python 3 is required to generate GLAD. Install Python 3, then run build.bat again.'
        }
        $python = $pythonCommand.Source
        $pythonPrefix = @()
    }

    if (-not (Test-Path -LiteralPath (Join-Path $pythonDir 'glad\__main__.py'))) {
        New-Item -ItemType Directory -Force -Path $pythonDir | Out-Null
        Write-Host 'Downloading the pinned GLAD 1 generator and its Python dependencies...'
        & $python @pythonPrefix -m pip install --disable-pip-version-check --target $pythonDir 'setuptools' 'glad==0.1.36'
        if ($LASTEXITCODE -ne 0) {
            throw 'Could not download the GLAD generator. Check your network connection and Python/pip installation.'
        }
    }

    $previousPythonPath = $env:PYTHONPATH
    $env:PYTHONPATH = if ($previousPythonPath) { "$pythonDir;$previousPythonPath" } else { $pythonDir }
    try {
        Write-Host 'Generating the GLAD OpenGL 4.6 core loader...'
        & $python @pythonPrefix -m glad --profile core --out-path $gladDir --api gl=4.6 --generator c --spec gl --reproducible
        if ($LASTEXITCODE -ne 0) {
            throw 'GLAD generation failed.'
        }
    } finally {
        $env:PYTHONPATH = $previousPythonPath
    }
}

if (-not (Test-Path -LiteralPath $gladSource) -or -not (Test-Path -LiteralPath $gladHeader) -or -not (Test-Path -LiteralPath $gladPlatformHeader)) {
    throw 'GLAD setup finished without producing glad.c and glad.h.'
}

Write-Host 'Dependencies are ready under build\_deps.'
