param(
    [datetime]$ResumeAt = [datetime]'2026-07-28T22:35:00'
)

$ErrorActionPreference = 'Stop'

$liftRoot = 'D:\Documents\FableTLC\lift'
$stateRoot = Join-Path $liftRoot 'state'
$watchdogPidPath = Join-Path $stateRoot 're-agent-resume-watchdog.pid'
$queuePidPath = Join-Path $stateRoot 're-agent-queue.pid'
$stopPath = Join-Path $stateRoot 're-agent-queue.stop'
$queueScript = Join-Path $liftRoot 'scripts\run_re_agent_queue.ps1'
$queueLog = Join-Path $stateRoot 're-agent-queue.log'

function Write-WatchdogLog([string]$Message) {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $queueLog -Value "[$stamp] WATCHDOG $Message" -Encoding UTF8
}

Set-Content -LiteralPath $watchdogPidPath -Value $PID -Encoding ASCII
Write-WatchdogLog "START pid=$PID resumeAt=$($ResumeAt.ToString('s'))"

try {
    while ((Get-Date) -lt $ResumeAt) {
        Start-Sleep -Seconds 60
    }

    $queueAlive = $false
    if (Test-Path -LiteralPath $queuePidPath) {
        try {
            $queuePid = [int](Get-Content -LiteralPath $queuePidPath -Raw)
            $queueAlive = $null -ne (Get-Process -Id $queuePid -ErrorAction SilentlyContinue)
        } catch {
            $queueAlive = $false
        }
    }

    if ($queueAlive) {
        Write-WatchdogLog "SKIP queue already active pid=$queuePid"
    } else {
        Remove-Item -LiteralPath $stopPath -Force -ErrorAction SilentlyContinue
        $queue = Start-Process -FilePath 'powershell.exe' -ArgumentList @(
            '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', $queueScript
        ) -WorkingDirectory $liftRoot -WindowStyle Hidden -PassThru
        Write-WatchdogLog "LAUNCH queue pid=$($queue.Id)"
    }
} catch {
    Write-WatchdogLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Remove-Item -LiteralPath $watchdogPidPath -Force -ErrorAction SilentlyContinue
}
