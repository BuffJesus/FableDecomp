param(
    [int]$MaxAttemptsPerTarget = 2,
    [switch]$DryRun
)

$ErrorActionPreference = 'Stop'

$liftRoot = 'D:\Documents\FableTLC\lift'
$stateRoot = Join-Path $liftRoot 'state'
$configPath = Join-Path $liftRoot 'config\re-agent-retry.yaml'
$primarySessionPath = Join-Path $stateRoot 're-agent-progress.json'
$retrySessionPath = Join-Path $stateRoot 're-agent-retry-progress.json'
$queueLog = Join-Path $stateRoot 're-agent-retry-queue.log'
$pidPath = Join-Path $stateRoot 're-agent-retry-queue.pid'
$stopPath = Join-Path $stateRoot 're-agent-retry-queue.stop'
$reAgent = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\Scripts\re-agent.exe'

# Detached shells do not reliably inherit Rider's Node/Codex PATH entries.
$env:RE_AGENT_CODEX_BIN = 'C:\Users\Cornelio\AppData\Local\JetBrains\Rider2026.2\acp-agents\.runtimes\node\24.13.0\node.exe'
$env:RE_AGENT_CODEX_JS = 'C:\Users\Cornelio\AppData\Local\JetBrains\Rider2026.2\acp-agents\.runtimes\node\24.13.0\npm-cache\_npx\a758dee5a93640a8\node_modules\@openai\codex\bin\codex.js'
$env:RE_AGENT_CODEX_MAX_ATTEMPTS = '3'
$decompileCacheRoot = 'D:\Documents\FableTLC\lift\.cache\re-agent-decompile'

$targets = @(
    [pscustomobject]@{ Address = '0x00BFE050'; Slug = 'landscapelayer-loadforeground' },
    [pscustomobject]@{ Address = '0x00BF3B60'; Slug = 'landscapepatch-loadforegroundpatch' },
    [pscustomobject]@{ Address = '0x00BE7D70'; Slug = 'backgroundpatch-load' },
    [pscustomobject]@{ Address = '0x00BE8920'; Slug = 'backgroundpatch-loadcompressed' },
    [pscustomobject]@{ Address = '0x00BF3A90'; Slug = 'landscapepatch-loadheader' },
    [pscustomobject]@{ Address = '0x00BEBED0'; Slug = 'landscape-getcomposedtexture' },
    [pscustomobject]@{ Address = '0x00BEC000'; Slug = 'landscape-preloaddiffusetexture' },
    [pscustomobject]@{ Address = '0x00BEC470'; Slug = 'landscape-preloadbumptexture' },
    [pscustomobject]@{ Address = '0x00BEC950'; Slug = 'landscape-getoverlaidtextures' },
    [pscustomobject]@{ Address = '0x00BEAAA0'; Slug = 'landscape-finishdynamictextures-a' },
    [pscustomobject]@{ Address = '0x00BEB4D0'; Slug = 'landscape-finishdynamictextures-b' },
    [pscustomobject]@{ Address = '0x00BF5C20'; Slug = 'foreground-spotlight-vs-a' },
    [pscustomobject]@{ Address = '0x00BF5C60'; Slug = 'foreground-spotlight-vs-b' },
    [pscustomobject]@{ Address = '0x00BF5CA0'; Slug = 'foreground-bumpshadow-vs-a' },
    [pscustomobject]@{ Address = '0x00BF5CD0'; Slug = 'foreground-bumpshadow-vs-b' },
    [pscustomobject]@{ Address = '0x00BF5D00'; Slug = 'foreground-bumpshadow-vs-c' }
)

function Write-QueueLog([string]$Message) {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $queueLog -Value "[$stamp] $Message" -Encoding UTF8
}

try {
    & (Join-Path $liftRoot 'scripts\organize_lift.ps1') -MinimumAgeMinutes 30 | Out-Null
} catch {
    Write-QueueLog "WARN transcript housekeeping failed: $($_.Exception.Message)"
}

function Get-Session([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path)) { return $null }
    return Get-Content -LiteralPath $Path -Raw | ConvertFrom-Json
}

function Test-TargetRecorded([string]$Address) {
    try {
        $session = Get-Session $retrySessionPath
        if ($null -eq $session) { return $false }
        $key = $Address.ToLowerInvariant().Replace('0x', '')
        return $null -ne $session.functions.PSObject.Properties[$key]
    } catch {
        Write-QueueLog "WARN could not read retry ledger: $($_.Exception.Message)"
        return $false
    }
}

function Get-FailedPrimaryTargets {
    $session = Get-Session $primarySessionPath
    if ($null -eq $session) { throw "Primary session is missing: $primarySessionPath" }
    return @($targets | Where-Object {
        $key = $_.Address.ToLowerInvariant().Replace('0x', '')
        $property = $session.functions.PSObject.Properties[$key]
        $null -ne $property -and $property.Value.success -ne $true
    })
}

$failedTargets = @(Get-FailedPrimaryTargets)
if ($DryRun) {
    $pending = @($failedTargets | Where-Object { -not (Test-TargetRecorded $_.Address) })
    $pending | ForEach-Object { "$($_.Address) $($_.Slug)" }
    exit 0
}

Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
Write-QueueLog "START retry queue pid=$PID failedTargets=$($failedTargets.Count)"
$providerBlocked = $false

try {
    foreach ($target in $failedTargets) {
        if (Test-Path -LiteralPath $stopPath) {
            Write-QueueLog "STOP marker found at $stopPath"
            break
        }

        if (Test-TargetRecorded $target.Address) {
            Write-QueueLog "SKIP recorded retry $($target.Address) $($target.Slug)"
            continue
        }

        $targetStartedAt = Get-Date
        for ($attempt = 1; $attempt -le $MaxAttemptsPerTarget; $attempt++) {
            $stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
            $addressKey = $target.Address.ToLowerInvariant().Replace('0x', '')
            $addressRelative = Join-Path $addressKey.Substring(0, 2) (
                Join-Path $addressKey.Substring(2, 2) $addressKey
            )
            $runLogRoot = Join-Path $liftRoot (
                Join-Path "logs\retry\$(Get-Date -Format 'yyyy-MM-dd')" $addressRelative
            )
            $env:RE_AGENT_DECOMPILE_CACHE_DIR = Join-Path $decompileCacheRoot (
                Join-Path $addressKey.Substring(0, 2) $addressKey.Substring(2, 2)
            )
            New-Item -ItemType Directory -Path $runLogRoot -Force | Out-Null
            New-Item -ItemType Directory -Path $env:RE_AGENT_DECOMPILE_CACHE_DIR -Force | Out-Null
            $stdoutPath = Join-Path $runLogRoot "re-agent-retry-$($target.Slug)-$stamp.stdout.log"
            $stderrPath = Join-Path $runLogRoot "re-agent-retry-$($target.Slug)-$stamp.stderr.log"
            Write-QueueLog "RUN retry=$($target.Address) slug=$($target.Slug) attempt=$attempt"

            $process = Start-Process -FilePath $reAgent -ArgumentList @(
                '--config', $configPath,
                'reverse', '--address', $target.Address,
                '--max-rounds', '4'
            ) -WorkingDirectory $liftRoot -NoNewWindow -Wait -PassThru `
              -RedirectStandardOutput $stdoutPath -RedirectStandardError $stderrPath

            Write-QueueLog "EXIT retry=$($target.Address) attempt=$attempt code=$($process.ExitCode)"
            if (Test-TargetRecorded $target.Address) { break }

            if (Select-String -LiteralPath $stderrPath -Pattern 'usage limit' -Quiet) {
                Write-QueueLog 'BLOCKED Codex usage limit; preserving remaining retries'
                $providerBlocked = $true
                break
            }

            if ($attempt -lt $MaxAttemptsPerTarget) {
                Write-QueueLog "RETRY unrecorded target=$($target.Address) after wrapper failure"
                Start-Sleep -Seconds 10
            }
        }
        try {
            & (Join-Path $liftRoot 'scripts\organize_lift.ps1') `
                -MinimumAgeMinutes 0 -Wave retry -Address $target.Address `
                -StartedAt $targetStartedAt | Out-Null
        } catch {
            Write-QueueLog "WARN target artifact organization failed target=$($target.Address): $($_.Exception.Message)"
        }

        if (-not (Test-TargetRecorded $target.Address)) {
            Write-QueueLog "ABANDON retry=$($target.Address) after $MaxAttemptsPerTarget wrapper attempts"
        }
        if ($providerBlocked) { break }
    }
} catch {
    Write-QueueLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Write-QueueLog "END retry queue pid=$PID"
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
}
