[CmdletBinding(SupportsShouldProcess)]
param(
    [ValidateRange(0, 10080)]
    [int]$MinimumAgeMinutes = 30
)

$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$workRoot = [IO.Path]::GetFullPath((Join-Path $root 'work'))
$snapshotRoot = [IO.Path]::GetFullPath((Join-Path $root 'snapshots'))

function Assert-ChildPath([string]$Path, [string]$Parent) {
    $prefix = $Parent.TrimEnd([IO.Path]::DirectorySeparatorChar) +
        [IO.Path]::DirectorySeparatorChar
    if (-not $Path.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing destination outside $Parent`: $Path"
    }
}

$categories = @(
    [pscustomobject]@{
        Name = 'root object'
        Match = { param($File) $File.Extension -ieq '.obj' }
        Destination = [IO.Path]::GetFullPath((Join-Path $workRoot 'scratch\root\objects'))
    },
    [pscustomobject]@{
        Name = 'root scratch source'
        Match = {
            param($File)
            $File.Name -like 'scratchpad*.cpp' -or $File.Name -eq 'rebuild_abi.h'
        }
        Destination = [IO.Path]::GetFullPath((Join-Path $workRoot 'scratch\root\sources'))
    },
    [pscustomobject]@{
        Name = 'local documentation archive'
        Match = { param($File) $File.Name -eq 'FableTLC_RE_docs.zip' }
        Destination = [IO.Path]::GetFullPath((Join-Path $snapshotRoot 'local-archives'))
    }
)

foreach ($category in $categories) {
    $parent = if ($category.Destination.StartsWith(
        $workRoot + [IO.Path]::DirectorySeparatorChar,
        [StringComparison]::OrdinalIgnoreCase
    )) {
        $workRoot
    } else {
        $snapshotRoot
    }
    Assert-ChildPath $category.Destination $parent
}

$activeCommandLines = @(
    Get-CimInstance Win32_Process -ErrorAction SilentlyContinue |
        Where-Object CommandLine |
        ForEach-Object { [string]$_.CommandLine }
)
$cutoff = (Get-Date).AddMinutes(-$MinimumAgeMinutes)
$planned = 0
$moved = 0
$bytesMoved = [int64]0
$protected = 0
$collisions = 0

foreach ($file in Get-ChildItem -LiteralPath $root -File -Force) {
    $category = $categories |
        Where-Object { & $_.Match $file } |
        Select-Object -First 1
    if ($null -eq $category) { continue }
    if ($file.LastWriteTime -gt $cutoff) {
        ++$protected
        continue
    }
    if ($activeCommandLines | Where-Object {
        $_.IndexOf($file.FullName, [StringComparison]::OrdinalIgnoreCase) -ge 0
    }) {
        ++$protected
        continue
    }

    $destination = Join-Path $category.Destination $file.Name
    if (Test-Path -LiteralPath $destination) {
        ++$collisions
        Write-Warning "Leaving source in place because destination exists: $destination"
        continue
    }

    ++$planned
    if ($PSCmdlet.ShouldProcess($file.FullName, "Move to $destination")) {
        New-Item -ItemType Directory -Path $category.Destination -Force | Out-Null
        Move-Item -LiteralPath $file.FullName -Destination $destination
        ++$moved
        $bytesMoved += $file.Length
    }
}

$liftOrganizer = Join-Path $root 'lift\scripts\organize_lift.ps1'
if (Test-Path -LiteralPath $liftOrganizer) {
    & $liftOrganizer -MinimumAgeMinutes $MinimumAgeMinutes -WhatIf:$WhatIfPreference
}

$artifactOrganizer = Join-Path $root 'tools\organize_decomp_artifacts.py'
if (Test-Path -LiteralPath $artifactOrganizer) {
    $python = (Get-Command python -ErrorAction Stop).Source
    $artifactArguments = @(
        $artifactOrganizer
        '--root'
        $root
        '--minimum-age-minutes'
        [string]$MinimumAgeMinutes
        '--json'
    )
    if (-not $WhatIfPreference) {
        $artifactArguments += '--apply'
    }
    & $python @artifactArguments
    if ($LASTEXITCODE -ne 0) {
        throw "Decomp artifact organizer failed with exit code $LASTEXITCODE"
    }
}

[pscustomobject]@{
    Root = $root
    Cutoff = $cutoff
    PlannedFiles = $planned
    MovedFiles = $moved
    BytesMoved = $bytesMoved
    ProtectedFiles = $protected
    Collisions = $collisions
} | Format-List
