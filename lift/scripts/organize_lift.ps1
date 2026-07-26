[CmdletBinding(SupportsShouldProcess)]
param(
    [ValidateRange(0, 10080)]
    [int]$MinimumAgeMinutes = 30,
    [ValidateSet('', 'primary', 'retry', 'wave2', 'wave3')]
    [string]$Wave = '',
    [string]$Address = '',
    [datetime]$StartedAt = [datetime]::MinValue
)

$ErrorActionPreference = 'Stop'
$liftRoot = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$logsRoot = [IO.Path]::GetFullPath((Join-Path $liftRoot 'logs'))
$liftPrefix = $liftRoot.TrimEnd([IO.Path]::DirectorySeparatorChar) + [IO.Path]::DirectorySeparatorChar
if (-not $logsRoot.StartsWith($liftPrefix, [StringComparison]::OrdinalIgnoreCase)) {
    throw "Refusing to organize outside the lift directory: $logsRoot"
}

$categories = @(
    [pscustomobject]@{ Name = 'wave3'; Pattern = '^re-agent-wave3-.*\.(?:stdout|stderr)\.log$'; Runner = 'run_re_agent_wave3_queue.ps1' },
    [pscustomobject]@{ Name = 'wave2'; Pattern = '^re-agent-wave2-.*\.(?:stdout|stderr)\.log$'; Runner = 'run_re_agent_wave2_queue.ps1' },
    [pscustomobject]@{ Name = 'retry'; Pattern = '^re-agent-retry-.*\.(?:stdout|stderr)\.log$'; Runner = 'run_re_agent_failure_queue.ps1' },
    [pscustomobject]@{ Name = 'wave1'; Pattern = '^re-agent-(?!wave[23]-|retry-).*\.(?:stdout|stderr)\.log$'; Runner = 'run_re_agent_queue.ps1' }
)

# A runner can hold an empty stderr transcript open for longer than the age
# threshold. Protect every transcript created since that runner started, not
# merely files with a recent LastWriteTime.
$runnerStarts = @{}
Get-CimInstance Win32_Process -ErrorAction SilentlyContinue |
    Where-Object { $_.CommandLine } |
    ForEach-Object {
        foreach ($category in $categories) {
            if ($_.CommandLine -like "*$($category.Runner)*") {
                $started = if ($_.CreationDate -is [datetime]) {
                    $_.CreationDate
                } else {
                    [Management.ManagementDateTimeConverter]::ToDateTime([string]$_.CreationDate)
                }
                if (-not $runnerStarts.ContainsKey($category.Name) -or $started -lt $runnerStarts[$category.Name]) {
                    $runnerStarts[$category.Name] = $started
                }
            }
        }
    }

$cutoff = (Get-Date).AddMinutes(-$MinimumAgeMinutes)
$planned = 0
$moved = 0
$bytesMoved = [int64]0
$protected = 0
$collisions = 0

function Get-NormalizedAddress([string]$Value) {
    $normalized = $Value.ToLowerInvariant().Replace('0x', '')
    if ($normalized -notmatch '^[0-9a-f]{8}$') {
        throw "Invalid function address: $Value"
    }
    return $normalized
}

function Get-AddressFromName([string]$Name) {
    foreach ($match in [regex]::Matches($Name, '(?i)(?:0x)?([0-9a-f]{8})')) {
        $candidate = $match.Groups[1].Value.ToLowerInvariant()
        $numeric = [Convert]::ToUInt32($candidate, 16)
        if ($numeric -ge 0x00400000 -and $numeric -lt 0x02000000) {
            return $candidate
        }
    }
    return ''
}

function Get-ShardedDirectory([string]$Root, [string]$NormalizedAddress, [switch]$Leaf) {
    $directory = Join-Path $Root (
        Join-Path $NormalizedAddress.Substring(0, 2) $NormalizedAddress.Substring(2, 2)
    )
    if ($Leaf) {
        $directory = Join-Path $directory $NormalizedAddress
    }
    return [IO.Path]::GetFullPath($directory)
}

function Move-Safely([IO.FileInfo]$File, [string]$DestinationDirectory, [string]$Description) {
    $destinationDirectoryFull = [IO.Path]::GetFullPath($DestinationDirectory)
    if (-not $destinationDirectoryFull.StartsWith(
        $liftPrefix,
        [StringComparison]::OrdinalIgnoreCase
    )) {
        throw "Refusing destination outside lift: $destinationDirectoryFull"
    }
    $destination = Join-Path $destinationDirectoryFull $File.Name
    if (Test-Path -LiteralPath $destination) {
        ++$script:collisions
        Write-Warning "Leaving source in place because destination exists: $destination"
        return
    }
    ++$script:planned
    if ($PSCmdlet.ShouldProcess($File.FullName, "$Description to $destination")) {
        New-Item -ItemType Directory -Path $destinationDirectoryFull -Force | Out-Null
        Move-Item -LiteralPath $File.FullName -Destination $destination
        ++$script:moved
        $script:bytesMoved += $File.Length
    }
}

foreach ($file in Get-ChildItem -LiteralPath $liftRoot -File -Force) {
    $category = $categories | Where-Object { $file.Name -match $_.Pattern } | Select-Object -First 1
    if ($null -eq $category) { continue }
    if ($file.LastWriteTime -gt $cutoff) {
        ++$protected
        continue
    }
    if ($runnerStarts.ContainsKey($category.Name) -and $file.LastWriteTime -ge $runnerStarts[$category.Name].AddSeconds(-2)) {
        ++$protected
        continue
    }

    $day = $file.LastWriteTime.ToString('yyyy-MM-dd')
    $dayRoot = [IO.Path]::GetFullPath((Join-Path $logsRoot (Join-Path $category.Name $day)))
    $fileAddress = Get-AddressFromName $file.Name
    $destinationDirectory = if ($fileAddress) {
        Get-ShardedDirectory $dayRoot $fileAddress -Leaf
    } else {
        Join-Path $dayRoot '_unassigned'
    }
    if (-not $destinationDirectory.StartsWith($logsRoot + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing destination outside the log archive: $destinationDirectory"
    }
    Move-Safely $file $destinationDirectory 'Archive transcript'
}

# Re-shard older transcript files that already live directly under a date
# directory. New runners write to the final address leaf immediately.
foreach ($waveDirectory in Get-ChildItem -LiteralPath $logsRoot -Directory -ErrorAction SilentlyContinue) {
    foreach ($dayDirectory in Get-ChildItem -LiteralPath $waveDirectory.FullName -Directory -ErrorAction SilentlyContinue) {
        if ($dayDirectory.Name -notmatch '^\d{4}-\d{2}-\d{2}$') { continue }
        foreach ($file in Get-ChildItem -LiteralPath $dayDirectory.FullName -File -ErrorAction SilentlyContinue) {
            if ($file.LastWriteTime -gt $cutoff) {
                ++$protected
                continue
            }
            $fileAddress = Get-AddressFromName $file.Name
            $destinationDirectory = if ($fileAddress) {
                Get-ShardedDirectory $dayDirectory.FullName $fileAddress -Leaf
            } else {
                Join-Path $dayDirectory.FullName '_unassigned'
            }
            Move-Safely $file $destinationDirectory 'Shard transcript'
        }
    }
}

# After a target finishes, move its generated source and the report-internal
# round JSON files into the same address shard. The timestamp boundary prevents
# one wave from claiming an older target's report files.
if ($Wave -and $Address) {
    $normalizedAddress = Get-NormalizedAddress $Address
    $reportRoot = Join-Path $liftRoot (Join-Path 'reports' $Wave)
    $codeRoot = Join-Path $reportRoot 'code'
    $codeDestination = Get-ShardedDirectory $codeRoot $normalizedAddress
    Get-ChildItem -LiteralPath $codeRoot -File -Filter "0x$normalizedAddress`_*.cpp" -ErrorAction SilentlyContinue |
        ForEach-Object { Move-Safely $_ $codeDestination 'Shard generated source' }

    $reportLogRoot = Join-Path $reportRoot 'logs'
    $reportBoundary = if ($StartedAt -eq [datetime]::MinValue) {
        $cutoff
    } else {
        $StartedAt.AddSeconds(-2)
    }
    Get-ChildItem -LiteralPath $reportLogRoot -File -ErrorAction SilentlyContinue |
        Where-Object { $_.LastWriteTime -ge $reportBoundary } |
        ForEach-Object {
            $dayRoot = Join-Path $reportLogRoot $_.LastWriteTime.ToString('yyyy-MM-dd')
            $destinationDirectory = Get-ShardedDirectory $dayRoot $normalizedAddress -Leaf
            Move-Safely $_ $destinationDirectory 'Shard agent round report'
        }
}

[pscustomobject]@{
    LiftRoot = $liftRoot
    Cutoff = $cutoff
    PlannedFiles = $planned
    MovedFiles = $moved
    BytesMoved = $bytesMoved
    ProtectedFiles = $protected
    Collisions = $collisions
} | Format-List
