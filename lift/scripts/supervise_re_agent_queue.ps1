param(
    [int]$PollSeconds = 30,
    [int]$ProviderBackoffMinutes = 15
)

$ErrorActionPreference = 'Stop'

$liftRoot = 'D:\Documents\FableTLC\lift'
$stateRoot = Join-Path $liftRoot 'state'
$primaryQueueScript = Join-Path $liftRoot 'scripts\run_re_agent_queue.ps1'
$primaryQueuePidPath = Join-Path $stateRoot 're-agent-queue.pid'
$primaryQueueStopPath = Join-Path $stateRoot 're-agent-queue.stop'
$primaryQueueLogPath = Join-Path $stateRoot 're-agent-queue.log'
$primarySessionPath = Join-Path $stateRoot 're-agent-progress.json'
$retryQueueScript = Join-Path $liftRoot 'scripts\run_re_agent_failure_queue.ps1'
$retryQueuePidPath = Join-Path $stateRoot 're-agent-retry-queue.pid'
$retryQueueStopPath = Join-Path $stateRoot 're-agent-retry-queue.stop'
$retryQueueLogPath = Join-Path $stateRoot 're-agent-retry-queue.log'
$retrySessionPath = Join-Path $stateRoot 're-agent-retry-progress.json'
$supervisorPidPath = Join-Path $stateRoot 're-agent-supervisor.pid'
$supervisorStopPath = Join-Path $stateRoot 're-agent-supervisor.stop'
$supervisorLogPath = Join-Path $stateRoot 're-agent-supervisor.log'
$completionMarkerPath = Join-Path $stateRoot 're-agent-queue.complete'
$completionSummaryPath = Join-Path $stateRoot 're-agent-completion-summary.md'
$primaryCodeDir = Join-Path $liftRoot 'reports\primary\code'
$retryCodeDir = Join-Path $liftRoot 'reports\retry\code'

$targets = @(
    [pscustomobject]@{ Key = '00bfe050'; Address = '0x00BFE050'; Name = 'CLandscapeLayerMesh::LoadForeground' },
    [pscustomobject]@{ Key = '00bf3b60'; Address = '0x00BF3B60'; Name = 'CEngineLandscapePatch::LoadForegroundPatch' },
    [pscustomobject]@{ Key = '00be7d70'; Address = '0x00BE7D70'; Name = 'CLandscapeBackgroundPatch::Load' },
    [pscustomobject]@{ Key = '00be8920'; Address = '0x00BE8920'; Name = 'CLandscapeBackgroundPatch::LoadCompressed' },
    [pscustomobject]@{ Key = '00bf3a90'; Address = '0x00BF3A90'; Name = 'CEngineLandscapePatch::LoadHeader' },
    [pscustomobject]@{ Key = '00bebed0'; Address = '0x00BEBED0'; Name = 'CEnginePrimitiveManagerMeshBase::GetComposedTexture' },
    [pscustomobject]@{ Key = '00bec000'; Address = '0x00BEC000'; Name = 'CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture' },
    [pscustomobject]@{ Key = '00bec470'; Address = '0x00BEC470'; Name = 'CEnginePrimitiveManagerMeshBase::PreloadBumpTexture' },
    [pscustomobject]@{ Key = '00bec950'; Address = '0x00BEC950'; Name = 'CEnginePrimitiveManagerMeshBase::GetOverlaidTextures' },
    [pscustomobject]@{ Key = '00beaaa0'; Address = '0x00BEAAA0'; Name = 'CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures [A]' },
    [pscustomobject]@{ Key = '00beb4d0'; Address = '0x00BEB4D0'; Name = 'CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures [B]' },
    [pscustomobject]@{ Key = '00bf5c20'; Address = '0x00BF5C20'; Name = 'CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader [A]' },
    [pscustomobject]@{ Key = '00bf5c60'; Address = '0x00BF5C60'; Name = 'CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader [B]' },
    [pscustomobject]@{ Key = '00bf5ca0'; Address = '0x00BF5CA0'; Name = 'CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader [A]' },
    [pscustomobject]@{ Key = '00bf5cd0'; Address = '0x00BF5CD0'; Name = 'CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader [B]' },
    [pscustomobject]@{ Key = '00bf5d00'; Address = '0x00BF5D00'; Name = 'CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader [C]' }
)

$artifactNames = [ordered]@{
    '0x008ED590' = 'CObjectFamilyDef_GetRandomObject'
    '0x00B687D0' = 'CEngineLandscapeRenderer_BuildThemes'
    '0x00BDC180' = 'CEngineLandscapeMap_UpdateBackgroundLOD'
    '0x00BDC510' = 'CEngineLandscapeMap_UpdateForeground'
    '0x00BDD0E0' = 'CEngineLandscapeMap_OpenStaticMap'
    '0x00BDF010' = 'CLocalDetailCacheMap_OpenStaticMap'
    '0x00BE6F70' = 'CLandscapeBackgroundPatch_Render'
    '0x00BE7D70' = 'CLandscapeBackgroundPatch_Load'
    '0x00BF3B60' = 'CEngineLandscapePatch_LoadForegroundPatch'
    '0x00BF4130' = 'CEngineLandscapePatch_LoadTextures'
    '0x00BF4220' = 'CEngineLandscapePatch_UpdateForeground'
    '0x00BF4570' = 'CEngineLandscapePatch_RenderForeground'
    '0x00BF6720' = 'CLandscapeBackgroundTreeNode_UpdateTesselation'
    '0x00BF7400' = 'CLandscapeBackgroundTreeNode_LoadHeader'
    '0x00BF8810' = 'CLandscapeBackgroundTreeNode_UpdateStaticLOD'
    '0x00BFE050' = 'CLandscapeLayerMesh_LoadForeground'
}

$legacySessionMetadata = [ordered]@{
    '00bfe050' = @('CLandscapeLayerMesh', 'LoadForeground')
    '00bf3b60' = @('CEngineLandscapePatch', 'LoadForegroundPatch')
    '00be7d70' = @('CLandscapeBackgroundPatch', 'Load')
}

function Write-SupervisorLog([string]$Message) {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $supervisorLogPath -Value "[$stamp] $Message" -Encoding UTF8
}

function Test-ProcessCommand([int]$ProcessId, [string]$Pattern) {
    $process = Get-CimInstance Win32_Process -Filter "ProcessId = $ProcessId" -ErrorAction SilentlyContinue
    return $null -ne $process -and $process.CommandLine -like "*$Pattern*"
}

function Test-QueueIsLive([string]$PidPath, [string]$ScriptName) {
    if (-not (Test-Path -LiteralPath $PidPath)) { return $false }
    try {
        $queuePid = [int](Get-Content -LiteralPath $PidPath -Raw)
        return Test-ProcessCommand $queuePid $ScriptName
    } catch {
        return $false
    }
}

function Get-Session([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path)) { return $null }
    return Get-Content -LiteralPath $Path -Raw | ConvertFrom-Json
}

function Get-PendingPrimaryTargets {
    try {
        $session = Get-Session $primarySessionPath
        if ($null -eq $session) { return $targets }
        return @($targets | Where-Object {
            $null -eq $session.functions.PSObject.Properties[$_.Key]
        })
    } catch {
        Write-SupervisorLog "WARN could not read primary ledger: $($_.Exception.Message)"
        return $targets
    }
}

function Get-FailedPrimaryTargets {
    $session = Get-Session $primarySessionPath
    if ($null -eq $session) { return @() }
    return @($targets | Where-Object {
        $property = $session.functions.PSObject.Properties[$_.Key]
        $null -ne $property -and $property.Value.success -ne $true
    })
}

function Get-PendingRetryTargets {
    $failed = @(Get-FailedPrimaryTargets)
    $retrySession = Get-Session $retrySessionPath
    if ($null -eq $retrySession) { return $failed }
    return @($failed | Where-Object {
        $null -eq $retrySession.functions.PSObject.Properties[$_.Key]
    })
}

function Repair-CodeFilenames([string]$CodeDir) {
    if (-not (Test-Path -LiteralPath $CodeDir)) { return }
    foreach ($entry in $artifactNames.GetEnumerator()) {
        $badPath = Join-Path $CodeDir "$($entry.Key)__====.cpp"
        if (-not (Test-Path -LiteralPath $badPath)) { continue }
        $goodPath = Join-Path $CodeDir "$($entry.Key)_$($entry.Value).cpp"
        if (Test-Path -LiteralPath $goodPath) {
            Write-SupervisorLog "WARN cannot repair duplicate artifact $badPath"
            continue
        }
        Move-Item -LiteralPath $badPath -Destination $goodPath
        Write-SupervisorLog "RENAMED artifact $($entry.Key) -> $($entry.Value)"
    }
}

function Repair-LegacySessionMetadata {
    if (-not (Test-Path -LiteralPath $primarySessionPath)) { return }
    $session = Get-Session $primarySessionPath
    $changed = $false
    foreach ($entry in $legacySessionMetadata.GetEnumerator()) {
        $property = $session.functions.PSObject.Properties[$entry.Key]
        if ($null -eq $property) { continue }
        $record = $property.Value
        if ($record.function_name -ne '====') { continue }
        $record.class_name = $entry.Value[0]
        $record.function_name = $entry.Value[1]
        $changed = $true
    }
    if (-not $changed) { return }

    $tempPath = "$primarySessionPath.supervisor.$PID.tmp"
    $backupPath = "$primarySessionPath.supervisor.$PID.bak"
    $json = $session | ConvertTo-Json -Depth 20
    [System.IO.File]::WriteAllText($tempPath, $json, [System.Text.UTF8Encoding]::new($false))
    [System.IO.File]::Replace($tempPath, $primarySessionPath, $backupPath)
    Remove-Item -LiteralPath $backupPath -Force -ErrorAction SilentlyContinue
    Write-SupervisorLog 'REPAIRED legacy session metadata for pre-fix workers'
}

function Get-ProviderBackoffSeconds([string]$LogPath) {
    if (-not (Test-Path -LiteralPath $LogPath)) { return 0 }
    $lines = @(Get-Content -LiteralPath $LogPath -Tail 40)
    $lastStart = -1
    $lastBlocked = -1
    for ($index = 0; $index -lt $lines.Count; $index++) {
        if ($lines[$index] -match '\] START ') { $lastStart = $index }
        if ($lines[$index] -match '\] BLOCKED Codex usage limit') { $lastBlocked = $index }
    }
    if ($lastBlocked -le $lastStart) { return 0 }
    if ($lines[$lastBlocked] -notmatch '^\[(?<stamp>[^\]]+)\]') { return 0 }
    $blockedAt = [datetime]::ParseExact($Matches.stamp, 'yyyy-MM-dd HH:mm:ss', $null)
    $remaining = ($blockedAt.AddMinutes($ProviderBackoffMinutes) - (Get-Date)).TotalSeconds
    return [math]::Max(0, [math]::Ceiling($remaining))
}

function Start-Queue([string]$ScriptPath, [int]$PendingCount, [string]$NextAddress, [string]$Label) {
    $queue = Start-Process -FilePath 'powershell.exe' -ArgumentList @(
        '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', $ScriptPath
    ) -WorkingDirectory $liftRoot -WindowStyle Hidden -PassThru
    Write-SupervisorLog "RELAUNCHED $Label queue pid=$($queue.Id) pending=$PendingCount next=$NextAddress"
}

function Get-Record([object]$Session, [string]$Key) {
    if ($null -eq $Session) { return $null }
    $property = $Session.functions.PSObject.Properties[$Key]
    if ($null -eq $property) { return $null }
    return $property.Value
}

function Write-CompletionSummary {
    $primarySession = Get-Session $primarySessionPath
    $retrySession = Get-Session $retrySessionPath
    $primaryPass = 0
    $primaryFail = 0
    $retryPass = 0
    $retryFail = 0
    $remaining = 0
    $rows = @()

    foreach ($target in $targets) {
        $primary = Get-Record $primarySession $target.Key
        $retry = Get-Record $retrySession $target.Key
        $displayName = $target.Name
        if ($null -ne $primary -and $primary.class_name -and $primary.function_name -and $primary.function_name -ne '====') {
            $displayName = "$($primary.class_name)::$($primary.function_name)"
        }
        if ($null -eq $primary) {
            $primaryResult = 'MISSING'
            $remaining++
        } elseif ($primary.success -eq $true) {
            $primaryResult = 'PASS'
            $primaryPass++
        } else {
            $primaryResult = 'FAIL'
            $primaryFail++
        }

        $retryResult = '-'
        if ($null -ne $retry) {
            if ($retry.success -eq $true) {
                $retryResult = 'PASS'
                $retryPass++
            } else {
                $retryResult = 'FAIL'
                $retryFail++
            }
        } elseif ($primaryResult -eq 'FAIL') {
            $retryResult = 'MISSING'
            $remaining++
        }
        $finalResult = if ($primaryResult -eq 'PASS' -or $retryResult -eq 'PASS') { 'PASS' } elseif ($retryResult -eq 'FAIL') { 'FAIL' } else { $primaryResult }
        $rows += "| $($target.Address) | $displayName | $primaryResult | $retryResult | $finalResult |"
    }

    $completedAt = Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz'
    $lines = @(
        '# Auto-RE terrain queue completion',
        '',
        "Completed: $completedAt",
        '',
        "Primary: **$primaryPass PASS / $primaryFail FAIL**. Failure-only retry: **$retryPass PASS / $retryFail FAIL**. Missing records: **$remaining**.",
        '',
        '| Address | Function | Primary | Retry | Final |',
        '|---|---|---:|---:|---:|'
    ) + $rows
    Set-Content -LiteralPath $completionSummaryPath -Value $lines -Encoding UTF8
    Set-Content -LiteralPath $completionMarkerPath -Value $completedAt -Encoding UTF8
    return [pscustomobject]@{ PrimaryPass = $primaryPass; PrimaryFail = $primaryFail; RetryPass = $retryPass; RetryFail = $retryFail; Remaining = $remaining }
}

function Show-CompletionNotification([object]$Summary) {
    try {
        $message = "Terrain auto-RE is complete.`nPrimary: $($Summary.PrimaryPass) PASS / $($Summary.PrimaryFail) FAIL`nRetry recovered: $($Summary.RetryPass); retry FAIL: $($Summary.RetryFail)`nSee re-agent-completion-summary.md"
        $shell = New-Object -ComObject WScript.Shell
        [void]$shell.Popup($message, 30, 'FableTLC auto-RE complete', 64)
    } catch {
        Write-SupervisorLog "WARN desktop completion notification failed: $($_.Exception.Message)"
    }
}

if (Test-Path -LiteralPath $supervisorPidPath) {
    try {
        $existingPid = [int](Get-Content -LiteralPath $supervisorPidPath -Raw)
        if ($existingPid -ne $PID -and (Test-ProcessCommand $existingPid 'supervise_re_agent_queue.ps1')) {
            Write-SupervisorLog "ALREADY RUNNING pid=$existingPid"
            exit 0
        }
    } catch {
        # A malformed/stale PID file is replaced below.
    }
}

Set-Content -LiteralPath $supervisorPidPath -Value $PID -Encoding ASCII
Write-SupervisorLog "START supervisor pid=$PID pollSeconds=$PollSeconds"

try {
    while ($true) {
        Repair-CodeFilenames $primaryCodeDir
        Repair-CodeFilenames $retryCodeDir

        if (Test-Path -LiteralPath $supervisorStopPath) {
            Write-SupervisorLog "STOP marker found at $supervisorStopPath"
            break
        }

        $pendingPrimary = @(Get-PendingPrimaryTargets)
        if ($pendingPrimary.Count -gt 0) {
            if (-not (Test-QueueIsLive $primaryQueuePidPath 'run_re_agent_queue.ps1')) {
                if (Test-Path -LiteralPath $primaryQueueStopPath) {
                    Write-SupervisorLog "PAUSED primary queue stop marker present; pending=$($pendingPrimary.Count)"
                } else {
                    $backoff = Get-ProviderBackoffSeconds $primaryQueueLogPath
                    if ($backoff -gt 0) {
                        Write-SupervisorLog "BACKOFF primary provider limit for another $backoff seconds"
                    } else {
                        Start-Queue $primaryQueueScript $pendingPrimary.Count $pendingPrimary[0].Address 'primary'
                    }
                }
            }
            Start-Sleep -Seconds $PollSeconds
            continue
        }

        # Wait for the primary wrapper's finally block before touching its ledger.
        if (Test-QueueIsLive $primaryQueuePidPath 'run_re_agent_queue.ps1') {
            Start-Sleep -Seconds $PollSeconds
            continue
        }
        Repair-LegacySessionMetadata

        $pendingRetry = @(Get-PendingRetryTargets)
        if ($pendingRetry.Count -gt 0) {
            if (-not (Test-QueueIsLive $retryQueuePidPath 'run_re_agent_failure_queue.ps1')) {
                if (Test-Path -LiteralPath $retryQueueStopPath) {
                    Write-SupervisorLog "PAUSED retry queue stop marker present; pending=$($pendingRetry.Count)"
                } else {
                    $backoff = Get-ProviderBackoffSeconds $retryQueueLogPath
                    if ($backoff -gt 0) {
                        Write-SupervisorLog "BACKOFF retry provider limit for another $backoff seconds"
                    } else {
                        Start-Queue $retryQueueScript $pendingRetry.Count $pendingRetry[0].Address 'retry'
                    }
                }
            }
            Start-Sleep -Seconds $PollSeconds
            continue
        }

        if (Test-QueueIsLive $retryQueuePidPath 'run_re_agent_failure_queue.ps1') {
            Start-Sleep -Seconds $PollSeconds
            continue
        }

        if (-not (Test-Path -LiteralPath $completionMarkerPath)) {
            $summary = Write-CompletionSummary
            Write-SupervisorLog "COMPLETE primary=$($summary.PrimaryPass)/$($targets.Count) retryRecovered=$($summary.RetryPass) finalFailures=$($summary.RetryFail)"
            Show-CompletionNotification $summary
        } else {
            Write-SupervisorLog 'COMPLETE marker already present'
        }
        break
    }
} catch {
    Write-SupervisorLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Write-SupervisorLog "END supervisor pid=$PID"
    Remove-Item -LiteralPath $supervisorPidPath -Force -ErrorAction SilentlyContinue
}
