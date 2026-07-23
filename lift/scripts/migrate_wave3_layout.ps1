[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$liftRoot = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$configRoot = Join-Path $liftRoot 'config'
$stateRoot = Join-Path $liftRoot 'state'
$reportRoot = Join-Path $liftRoot 'reports'
$archiveRoot = Join-Path $stateRoot 'archive'

foreach ($directory in @($configRoot, $stateRoot, $reportRoot, $archiveRoot)) {
    $full = [IO.Path]::GetFullPath($directory)
    if (-not $full.StartsWith($liftRoot + [IO.Path]::DirectorySeparatorChar,
            [StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing migration outside lift: $full"
    }
    New-Item -ItemType Directory -Path $full -Force | Out-Null
}

$legacyPidPath = Join-Path $liftRoot 're-agent-wave3-queue.pid'
if (Test-Path -LiteralPath $legacyPidPath) {
    try {
        $legacyPid = [int](Get-Content -LiteralPath $legacyPidPath -Raw)
        $legacyProcess = Get-CimInstance Win32_Process -Filter "ProcessId = $legacyPid" -ErrorAction SilentlyContinue
        if ($null -ne $legacyProcess -and
            $legacyProcess.CommandLine -like '*run_re_agent_wave3_queue.ps1*') {
            throw "Wave 3 legacy process is still active: pid=$legacyPid"
        }
    } catch [System.FormatException] {
        # A malformed stale PID is migrated with the remaining state below.
    }
}

$legacyReport = Join-Path $liftRoot 're-agent-wave3-reports'
$canonicalReport = Join-Path $reportRoot 'wave3'
if (Test-Path -LiteralPath $legacyReport) {
    if (Test-Path -LiteralPath $canonicalReport) {
        throw "Wave 3 report destination already exists: $canonicalReport"
    }
    Move-Item -LiteralPath $legacyReport -Destination $canonicalReport
}

foreach ($name in @(
    're-agent-wave3-progress.json',
    're-agent-wave3-queue.log',
    're-agent-wave3-queue.pid',
    're-agent-wave3-queue.stop',
    're-agent-wave3-queue.complete'
)) {
    $source = Join-Path $liftRoot $name
    $destination = Join-Path $stateRoot $name
    if (Test-Path -LiteralPath $source) {
        if (Test-Path -LiteralPath $destination) {
            throw "Wave 3 state destination already exists: $destination"
        }
        Move-Item -LiteralPath $source -Destination $destination
    }
}

$legacyConfig = Join-Path $liftRoot 're-agent-wave3.yaml'
if (Test-Path -LiteralPath $legacyConfig) {
    $archivedConfig = Join-Path $archiveRoot 're-agent-wave3.legacy.yaml'
    if (Test-Path -LiteralPath $archivedConfig) {
        throw "Legacy Wave 3 config archive already exists: $archivedConfig"
    }
    Move-Item -LiteralPath $legacyConfig -Destination $archivedConfig
}

[pscustomobject]@{
    LiftRoot = $liftRoot
    Config = Join-Path $configRoot 're-agent-wave3.yaml'
    State = $stateRoot
    Reports = $canonicalReport
}
