param(
    [int]$BatchSize = 500,
    [int]$MaximumFunctionBytes = 48,
    [int]$MaxBatchesPerRun = 4
)

$ErrorActionPreference = 'Stop'
$root = 'D:\Documents\FableTLC'
$python = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\python.exe'
$pipeline = Join-Path $root 'tools\decomp_pipeline'
$pending = Join-Path $root 'rebuild\oracles\pending'
$work = Join-Path $root 'rebuild\build\local-parity'
$state = Join-Path $root 'rebuild\local-parity'
$pidPath = Join-Path $state 'queue.pid'
$logPath = Join-Path $state 'queue.log'
$refreshPidPath = Join-Path $root 'rebuild\refresh.pid'

New-Item -ItemType Directory -Path $work, $state, $pending -Force | Out-Null

function Write-QueueLog([string]$Message) {
    Add-Content -LiteralPath $logPath -Value (
        "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] $Message"
    ) -Encoding UTF8
}

function Test-ProcessCommand([int]$ProcessId, [string]$Pattern) {
    $process = Get-CimInstance Win32_Process -Filter "ProcessId = $ProcessId" -ErrorAction SilentlyContinue
    return $null -ne $process -and $process.CommandLine -like "*$Pattern*"
}

function Invoke-Checked([string]$Description, [scriptblock]$Action) {
    Write-QueueLog "START $Description"
    & $Action
    if ($LASTEXITCODE -ne 0) {
        throw "$Description failed with exit code $LASTEXITCODE"
    }
    Write-QueueLog "DONE $Description"
}

if (Test-Path -LiteralPath $pidPath) {
    try {
        $existingPid = [int](Get-Content -LiteralPath $pidPath -Raw)
        if ($existingPid -ne $PID -and (Test-ProcessCommand $existingPid 'run_local_parity_queue.ps1')) {
            Write-QueueLog "SKIP already running pid=$existingPid"
            exit 0
        }
    } catch {
        # A stale or malformed lock is replaced below.
    }
}

# The local lane does not need Ghidra and is intentionally allowed to run beside
# Wave 3. It must not, however, mutate the candidate catalog while the canonical
# rebuild refresh is compiling that same catalog.
if (Test-Path -LiteralPath $refreshPidPath) {
    try {
        $refreshPid = [int](Get-Content -LiteralPath $refreshPidPath -Raw)
        if (Test-ProcessCommand $refreshPid 'run_rebuild_refresh.ps1') {
            Write-QueueLog "DEFER canonical rebuild refresh is active pid=$refreshPid"
            exit 0
        }
    } catch {
        Write-QueueLog 'DEFER canonical rebuild refresh PID cannot be validated'
        exit 0
    }
}

$catalogBuilder = Get-CimInstance Win32_Process -ErrorAction SilentlyContinue |
    Where-Object {
        $_.ProcessId -ne $PID -and
        $_.CommandLine -like '*build_candidates.ps1*'
    } |
    Select-Object -First 1
if ($null -ne $catalogBuilder) {
    Write-QueueLog "DEFER candidate catalog build is active pid=$($catalogBuilder.ProcessId)"
    exit 0
}

Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
$totalAuthored = 0
$totalWins = 0
Write-QueueLog (
    "START local parity pid=$PID batch_size=$BatchSize max_bytes=$MaximumFunctionBytes " +
    "max_batches=$MaxBatchesPerRun"
)
try {
    $replayBatch = "replay-$(Get-Date -Format 'yyyyMMdd-HHmmss')"
    $replayOracle = Join-Path $work "$replayBatch-oracle.tsv"
    $replayAuthored = Join-Path $work "$replayBatch-authored.json"
    $replayVerificationLog = Join-Path $work "$replayBatch-verification.log"
    Invoke-Checked "pending-pattern replay $replayBatch" {
        & $python (Join-Path $pipeline 'replay_pending_tiny.py') `
            $replayOracle $replayAuthored $BatchSize
    }
    $replayData = Get-Content -LiteralPath $replayAuthored -Raw | ConvertFrom-Json
    $replayCount = @($replayData.result.authored).Count
    $totalAuthored += $replayCount
    if ($replayCount -gt 0) {
        Invoke-Checked "pending replay verification $replayBatch" {
            & $python (Join-Path $pipeline 'verify_and_land.py') `
                $replayAuthored $replayOracle --land |
                Tee-Object -FilePath $replayVerificationLog
        }
        $replayVerification = Get-Content -LiteralPath $replayVerificationLog -Raw
        $replayWinMatch = [regex]::Match(
            $replayVerification,
            '(?m)^WINS:\s+(\d+)\s*$'
        )
        $replayWins = if ($replayWinMatch.Success) {
            [int]$replayWinMatch.Groups[1].Value
        } else {
            0
        }
        $totalWins += $replayWins
        Write-QueueLog (
            "REPLAY authored=$replayCount verified_wins=$replayWins " +
            "batch=$replayBatch"
        )
    } else {
        Write-QueueLog "REPLAY no newly recognized pending candidates"
    }

    for ($index = 1; $index -le $MaxBatchesPerRun; ++$index) {
        $batch = "auto-$(Get-Date -Format 'yyyyMMdd-HHmmss')-$index"
        $oracle = Join-Path $pending "$batch`_oracle.tsv"
        $targets = Join-Path $pending "$batch`_targets.json"
        $authored = Join-Path $work "$batch-authored.json"
        $selectionLog = Join-Path $work "$batch-addresses.json"
        $verificationLog = Join-Path $work "$batch-verification.log"

        Invoke-Checked "candidate selection $batch" {
            & $python (Join-Path $pipeline 'next_batch.py') `
                $batch $BatchSize $MaximumFunctionBytes > $selectionLog
        }

        $selectedCount = @(
            Import-Csv -LiteralPath $oracle -Delimiter "`t"
        ).Count
        if ($selectedCount -eq 0) {
            Remove-Item -LiteralPath $oracle, $targets -Force -ErrorAction SilentlyContinue
            Write-QueueLog "COMPLETE eligible short-function backlog exhausted batch=$batch"
            break
        }

        Invoke-Checked "deterministic authoring $batch" {
            & $python (Join-Path $pipeline 'auto_author_tiny.py') $oracle $authored
        }
        $authoredData = Get-Content -LiteralPath $authored -Raw | ConvertFrom-Json
        $authoredCount = @($authoredData.result.authored).Count
        $totalAuthored += $authoredCount
        Write-QueueLog "BATCH selected=$selectedCount authored=$authoredCount batch=$batch"
        if ($authoredCount -eq 0) {
            continue
        }

        Invoke-Checked "byte and behavior verification $batch" {
            & $python (Join-Path $pipeline 'verify_and_land.py') `
                $authored $oracle --land | Tee-Object -FilePath $verificationLog
        }
        $verification = Get-Content -LiteralPath $verificationLog -Raw
        $winMatch = [regex]::Match($verification, '(?m)^WINS:\s+(\d+)\s*$')
        $wins = if ($winMatch.Success) { [int]$winMatch.Groups[1].Value } else { 0 }
        $totalWins += $wins
        Write-QueueLog "BATCH verified_wins=$wins batch=$batch"
    }

    if ($totalWins -gt 0) {
        Invoke-Checked 'VC7.1 catalog compile and behavior gate' {
            & powershell.exe -NoProfile -ExecutionPolicy Bypass `
                -File (Join-Path $root 'rebuild\build_candidates.ps1')
        }
        Invoke-Checked 'retail object parity comparison' {
            & $python (Join-Path $root 'tools\compare_candidate_objects.py') --root $root
        }
        Invoke-Checked 'rebuild manifest refresh' {
            & $python (Join-Path $root 'tools\bootstrap_rebuild_tree.py') --root $root
        }
        Invoke-Checked 'reconstruction backlog refresh' {
            & $python (Join-Path $root 'tools\build_reconstruction_backlog.py') --root $root
        }
        Invoke-Checked 'promotion queue refresh' {
            & $python (Join-Path $root 'tools\build_promotion_queue.py') --root $root
        }
        Invoke-Checked 'coverage dashboard refresh' {
            & $python (Join-Path $root 'tools\write_decomp_dashboard.py') --root $root
        }
        Invoke-Checked 'README progress refresh' {
            & $python (Join-Path $root 'tools\update_readme_progress.py') --root $root
        }
        Write-QueueLog 'START artifact organization'
        & $python (Join-Path $root 'tools\organize_decomp_artifacts.py') `
            --root $root --apply --allow-active
        $organizerExit = $LASTEXITCODE
        if ($organizerExit -eq 0) {
            Write-QueueLog 'DONE artifact organization'
        } elseif ($organizerExit -eq 2) {
            # Exit 2 means collision-safe refusal: every source is preserved and
            # the index is still refreshed. Live Wave3 can legitimately have a
            # root copy and a sharded copy during its current target boundary.
            Write-QueueLog 'WARN artifact organization preserved live collisions'
        } else {
            throw "artifact organization failed with exit code $organizerExit"
        }
    }
    Write-QueueLog "COMPLETE authored=$totalAuthored verified_wins=$totalWins"
} catch {
    Write-QueueLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
    Write-QueueLog "END local parity pid=$PID"
}
