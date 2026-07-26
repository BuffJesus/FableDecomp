param([int]$MaxAttemptsPerTarget = 2)

$ErrorActionPreference = 'Stop'
$root = 'D:\Documents\FableTLC'
$liftRoot = Join-Path $root 'lift'
$stateRoot = Join-Path $liftRoot 'state'
$configPath = Join-Path $liftRoot 'config\re-agent-wave2.yaml'
$sessionPath = Join-Path $stateRoot 're-agent-wave2-progress.json'
$queueLog = Join-Path $stateRoot 're-agent-wave2-queue.log'
$pidPath = Join-Path $stateRoot 're-agent-wave2-queue.pid'
$stopPath = Join-Path $stateRoot 're-agent-wave2-queue.stop'
$completePath = Join-Path $stateRoot 're-agent-wave2-queue.complete'
$refreshPidPath = Join-Path $root 'rebuild\refresh.pid'
$reAgent = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\Scripts\re-agent.exe'

$env:RE_AGENT_CODEX_BIN = 'C:\Users\Cornelio\AppData\Local\JetBrains\Rider2026.2\acp-agents\.runtimes\node\24.13.0\node.exe'
$env:RE_AGENT_CODEX_JS = 'C:\Users\Cornelio\AppData\Local\JetBrains\Rider2026.2\acp-agents\.runtimes\node\24.13.0\npm-cache\_npx\a758dee5a93640a8\node_modules\@openai\codex\bin\codex.js'
$env:RE_AGENT_CODEX_MAX_ATTEMPTS = '3'
$decompileCacheRoot = 'D:\Documents\FableTLC\lift\.cache\re-agent-decompile'

$targets = @(
    [pscustomobject]@{ Address = '0x00AB46E0'; Slug = 'mousedx-getpos' },
    [pscustomobject]@{ Address = '0x00AB4700'; Slug = 'mousedx-setpos' },
    [pscustomobject]@{ Address = '0x00AB5390'; Slug = 'mousedx-setmousecursor' },
    [pscustomobject]@{ Address = '0x00AB5F00'; Slug = 'mousedx-peekpos' },
    [pscustomobject]@{ Address = '0x00AB5F10'; Slug = 'mousedx-setinputon' },
    [pscustomobject]@{ Address = '0x00AB5F20'; Slug = 'mousedx-setrecentringon' },
    [pscustomobject]@{ Address = '0x00AB5F30'; Slug = 'mousedx-isrecentringon' },
    [pscustomobject]@{ Address = '0x00AB5F40'; Slug = 'mousedx-setdrawcursor' },
    [pscustomobject]@{ Address = '0x00AB5F50'; Slug = 'mousedx-isdrawingcursor' },
    [pscustomobject]@{ Address = '0x00AB5F60'; Slug = 'mousedx-setmousefiltering' },
    [pscustomobject]@{ Address = '0x00AB5F70'; Slug = 'mousedx-islmbdown' },
    [pscustomobject]@{ Address = '0x00AB5F80'; Slug = 'mousedx-ismmbdown' },
    [pscustomobject]@{ Address = '0x00AB5F90'; Slug = 'mousedx-isrmbdown' },
    [pscustomobject]@{ Address = '0x00A65D30'; Slug = 'fixedallocator-consolidate' },
    [pscustomobject]@{ Address = '0x00A65D40'; Slug = 'fixedallocator-getallocationsize' },
    [pscustomobject]@{ Address = '0x00A65D50'; Slug = 'fixedallocator-getcontrolledmemory' }
)

function Write-QueueLog([string]$Message) {
    Add-Content -LiteralPath $queueLog -Value "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] $Message" -Encoding UTF8
}

try {
    & (Join-Path $liftRoot 'scripts\organize_lift.ps1') -MinimumAgeMinutes 30 | Out-Null
} catch {
    Write-QueueLog "WARN transcript housekeeping failed: $($_.Exception.Message)"
}

function Test-ProcessCommand([int]$ProcessId, [string]$Pattern) {
    $process = Get-CimInstance Win32_Process -Filter "ProcessId = $ProcessId" -ErrorAction SilentlyContinue
    return $null -ne $process -and $process.CommandLine -like "*$Pattern*"
}

function Test-Recorded([string]$Address) {
    if (-not (Test-Path -LiteralPath $sessionPath)) { return $false }
    try {
        $session = Get-Content -LiteralPath $sessionPath -Raw | ConvertFrom-Json
        $key = $Address.ToLowerInvariant().Replace('0x', '')
        return $null -ne $session.functions.PSObject.Properties[$key]
    } catch {
        Write-QueueLog "WARN unreadable progress ledger: $($_.Exception.Message)"
        return $false
    }
}

if (Test-Path -LiteralPath $completePath) { exit 0 }
if (Test-Path -LiteralPath $pidPath) {
    try {
        $existingPid = [int](Get-Content -LiteralPath $pidPath -Raw)
        if ($existingPid -ne $PID -and (Test-ProcessCommand $existingPid 'run_re_agent_wave2_queue.ps1')) {
            Write-QueueLog "SKIP already running pid=$existingPid"
            exit 0
        }
    } catch {}
}
if (Test-Path -LiteralPath $refreshPidPath) {
    try {
        $refreshPid = [int](Get-Content -LiteralPath $refreshPidPath -Raw)
        if (Test-ProcessCommand $refreshPid 'run_rebuild_refresh.ps1') {
            Write-QueueLog "DEFER rebuild refresh owns Ghidra pid=$refreshPid"
            exit 0
        }
    } catch {}
}

Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
Write-QueueLog "START wave2 pid=$PID targets=$($targets.Count)"
$providerBlocked = $false
try {
    foreach ($target in $targets) {
        if (Test-Path -LiteralPath $stopPath) {
            Write-QueueLog "STOP marker found at $stopPath"
            exit 0
        }
        if (Test-Recorded $target.Address) {
            Write-QueueLog "SKIP recorded $($target.Address) $($target.Slug)"
            continue
        }
        $targetStartedAt = Get-Date
        for ($attempt = 1; $attempt -le $MaxAttemptsPerTarget; ++$attempt) {
            $stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
            $addressKey = $target.Address.ToLowerInvariant().Replace('0x', '')
            $addressRelative = Join-Path $addressKey.Substring(0, 2) (
                Join-Path $addressKey.Substring(2, 2) $addressKey
            )
            $runLogRoot = Join-Path $liftRoot (
                Join-Path "logs\wave2\$(Get-Date -Format 'yyyy-MM-dd')" $addressRelative
            )
            $env:RE_AGENT_DECOMPILE_CACHE_DIR = Join-Path $decompileCacheRoot (
                Join-Path $addressKey.Substring(0, 2) $addressKey.Substring(2, 2)
            )
            New-Item -ItemType Directory -Path $runLogRoot -Force | Out-Null
            New-Item -ItemType Directory -Path $env:RE_AGENT_DECOMPILE_CACHE_DIR -Force | Out-Null
            $stdoutPath = Join-Path $runLogRoot "re-agent-wave2-$($target.Slug)-$stamp.stdout.log"
            $stderrPath = Join-Path $runLogRoot "re-agent-wave2-$($target.Slug)-$stamp.stderr.log"
            Write-QueueLog "RUN target=$($target.Address) slug=$($target.Slug) attempt=$attempt"
            $process = Start-Process -FilePath $reAgent -ArgumentList @(
                '--config', $configPath, 'reverse', '--address', $target.Address, '--max-rounds', '4'
            ) -WorkingDirectory $liftRoot -NoNewWindow -Wait -PassThru `
              -RedirectStandardOutput $stdoutPath -RedirectStandardError $stderrPath
            Write-QueueLog "EXIT target=$($target.Address) attempt=$attempt code=$($process.ExitCode)"
            if (Test-Recorded $target.Address) { break }
            if (Select-String -LiteralPath $stderrPath -Pattern 'usage limit' -Quiet) {
                Write-QueueLog 'BLOCKED Codex usage limit; scheduled retry will resume'
                $providerBlocked = $true
                break
            }
        }
        try {
            & (Join-Path $liftRoot 'scripts\organize_lift.ps1') `
                -MinimumAgeMinutes 0 -Wave wave2 -Address $target.Address `
                -StartedAt $targetStartedAt | Out-Null
        } catch {
            Write-QueueLog "WARN target artifact organization failed target=$($target.Address): $($_.Exception.Message)"
        }
        if ($providerBlocked) { exit 0 }
    }
    $pending = @($targets | Where-Object { -not (Test-Recorded $_.Address) })
    if ($pending.Count -eq 0) {
        $completedAt = Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz'
        Set-Content -LiteralPath $completePath -Value $completedAt -Encoding UTF8
        Write-QueueLog "COMPLETE targets=$($targets.Count) at=$completedAt"
    } else {
        Write-QueueLog "INCOMPLETE pending=$($pending.Count); scheduled retry will resume"
    }
} catch {
    Write-QueueLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
    Write-QueueLog "END wave2 pid=$PID"
}
