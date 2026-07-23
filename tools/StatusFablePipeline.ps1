param(
    [int]$ImportPid = 15816,
    [int]$WatcherPid = 4852,
    [int]$SampleSeconds = 5
)

$ErrorActionPreference = "SilentlyContinue"

$Root = "D:\Documents\FableTLC"
$Out = Join-Path $Root "ghidra_out"
$AnalyzeLog = Join-Path $Out "analyze_fablewin.log"
$DriverLog = Join-Path $Out "continue_fablewin_pipeline.log"

function Format-GB($Bytes) {
    if ($null -eq $Bytes) { return "" }
    return ("{0:n2} GB" -f ($Bytes / 1GB))
}

Set-Location $Root

Write-Host "FableTLC pipeline status"
Write-Host "Root: $Root"
Write-Host ""

$import = Get-Process -Id $ImportPid
if ($import) {
    $cpu1 = $import.CPU
    Start-Sleep -Seconds $SampleSeconds
    $import.Refresh()
    $cpu2 = $import.CPU
    Write-Host "Import PID ${ImportPid}: alive"
    Write-Host "  CPU delta over ${SampleSeconds}s: $([math]::Round($cpu2 - $cpu1, 1))s"
    Write-Host "  Total CPU: $([math]::Round($import.CPU, 1))s"
    Write-Host "  RAM: $(Format-GB $import.WorkingSet64)"
    Write-Host "  Started: $($import.StartTime)"
}
else {
    Write-Host "Import PID ${ImportPid}: not running"
}

$watcher = Get-Process -Id $WatcherPid
if ($watcher) {
    Write-Host "Watcher PID ${WatcherPid}: alive"
}
else {
    Write-Host "Watcher PID ${WatcherPid}: not running"
}

Write-Host ""
if (Test-Path $AnalyzeLog) {
    $saved = Select-String -Path $AnalyzeLog -Pattern "Save succeeded" -Quiet
    $lastWrite = (Get-Item $AnalyzeLog).LastWriteTime
    Write-Host "FableWin analysis log: $AnalyzeLog"
    Write-Host "  Last write: $lastWrite"
    Write-Host "  Save succeeded: $saved"
}
else {
    Write-Host "FableWin analysis log missing: $AnalyzeLog"
}

Write-Host ""
if (Test-Path $DriverLog) {
    Write-Host "Driver log tail:"
    Get-Content $DriverLog -Tail 12

    $lines = Get-Content $DriverLog
    $lastStart = $lines | Where-Object { $_ -match " START " } | Select-Object -Last 1
    $lastDone = $lines | Where-Object { $_ -match " DONE " } | Select-Object -Last 1
    if ($lastStart -and $lastStart -ne $lastDone) {
        Write-Host ""
        Write-Host "Last started step: $lastStart"
        if ($lastStart -match "-> (.+)$") {
            $stepLog = $Matches[1]
            if (Test-Path $stepLog) {
                Write-Host "Current step log tail:"
                Get-Content $stepLog -Tail 20
            }
        }
    }
}
else {
    Write-Host "Driver log missing: $DriverLog"
}

Write-Host ""
Write-Host "Newest ghidra_out files:"
Get-ChildItem $Out -File | Sort-Object LastWriteTime -Descending |
    Select-Object -First 10 LastWriteTime,Length,Name |
    Format-Table -AutoSize
