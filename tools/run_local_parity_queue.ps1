param(
    [int]$BatchSize = 500,
    [int]$MaximumFunctionBytes = 32,
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
    for ($index = 1; $index -le $MaxBatchesPerRun; ++$index) {
        $batch = "auto-$(Get-Date -Format 'yyyyMMdd-HHmmss')-$index"
        $oracle = Join-Path $pending "$batch`_oracle.tsv"
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
        Invoke-Checked 'artifact organization' {
            & $python (Join-Path $root 'tools\organize_decomp_artifacts.py') `
                --root $root --apply --allow-active
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
