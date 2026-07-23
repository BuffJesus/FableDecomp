[CmdletBinding(SupportsShouldProcess)]
param(
    [ValidateRange(0, 10080)]
    [int]$MinimumAgeMinutes = 30
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
    $destinationDirectory = [IO.Path]::GetFullPath((Join-Path $logsRoot (Join-Path $category.Name $day)))
    if (-not $destinationDirectory.StartsWith($logsRoot + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing destination outside the log archive: $destinationDirectory"
    }
    $destination = Join-Path $destinationDirectory $file.Name
    if (Test-Path -LiteralPath $destination) {
        ++$collisions
        Write-Warning "Leaving source in place because destination exists: $destination"
        continue
    }

    ++$planned
    if ($PSCmdlet.ShouldProcess($file.FullName, "Move to $destination")) {
        New-Item -ItemType Directory -Path $destinationDirectory -Force | Out-Null
        Move-Item -LiteralPath $file.FullName -Destination $destination
        ++$moved
        $bytesMoved += $file.Length
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
