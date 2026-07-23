param(
    [int]$PollSeconds = 60
)

$ErrorActionPreference = 'Stop'

$root = 'D:\Documents\FableTLC'
$tools = Join-Path $root 'tools'
$outDir = Join-Path $root 'ghidra_out\naming_stragglers'
$labels = Join-Path $root 'ghidra_out\labels_forge_autoname_t6_all.tsv'
$resolver = Join-Path $tools 'resolve_naming_stragglers.py'
$bootstrap = Join-Path $tools 'bootstrap_rebuild_tree.py'
$dashboard = Join-Path $tools 'write_decomp_dashboard.py'
$compileGate = Join-Path $tools 'gate_re_agent_candidates.py'
$backlog = Join-Path $tools 'build_reconstruction_backlog.py'
$candidateBuild = Join-Path $root 'rebuild\build_candidates.ps1'
$oracleExport = Join-Path $tools 'ExportCandidateOracles.ps1'
$parityCompare = Join-Path $tools 'compare_candidate_objects.py'
$signatureAudit = Join-Path $tools 'audit_candidate_signatures.py'
$namingTriage = Join-Path $tools 'triage_naming_quality.py'
$python = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\python.exe'
$headless = 'D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat'
$projectDir = Join-Path $root 'ghidra_proj'
$scriptDir = Join-Path $tools 'ghidra_scripts'
$autoReComplete = Join-Path $root 'lift\state\re-agent-queue.complete'
$pidPath = Join-Path $outDir 'pipeline.pid'
$logPath = Join-Path $outDir 'pipeline.log'
$stopPath = Join-Path $outDir 'pipeline.stop'
$completePath = Join-Path $outDir 'pipeline.complete'
$targetSignatures = Join-Path $outDir 'Fable.exe.signatures.tsv'
$egoSignatures = Join-Path $outDir 'ego_r.exe.signatures.tsv'
$editorSignatures = Join-Path $outDir 'FableWin.exe.signatures.tsv'

New-Item -ItemType Directory -Path $outDir -Force | Out-Null

function Write-PipelineLog([string]$Message) {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $logPath -Value "[$stamp] $Message" -Encoding UTF8
}

function Test-PipelineProcess([int]$ProcessId) {
    $process = Get-CimInstance Win32_Process -Filter "ProcessId = $ProcessId" -ErrorAction SilentlyContinue
    return $null -ne $process -and $process.CommandLine -like '*run_naming_straggler_pipeline.ps1*'
}

function Invoke-Checked([string]$Description, [scriptblock]$Action) {
    Write-PipelineLog "START $Description"
    & $Action
    if ($LASTEXITCODE -ne 0) {
        throw "$Description failed with exit code $LASTEXITCODE"
    }
    Write-PipelineLog "DONE $Description"
}

function Export-Signatures([string]$Program, [string]$OutPath) {
    if (Test-Path -LiteralPath $OutPath) {
        Write-PipelineLog "SKIP existing signatures $Program"
        return
    }
    $tempPath = "$OutPath.tmp.$PID"
    Write-PipelineLog "START signature export $Program"
    & $headless $projectDir 'FableTLC' -process $Program -noanalysis `
        -scriptPath $scriptDir -postScript 'ExportInstructionSignatures.java' $tempPath
    $exitCode = $LASTEXITCODE
    if ($exitCode -ne 0) {
        throw "signature export $Program failed with exit code $exitCode"
    }
    Move-Item -LiteralPath $tempPath -Destination $OutPath
    Write-PipelineLog "DONE signature export $Program"
}

if (Test-Path -LiteralPath $pidPath) {
    try {
        $existingPid = [int](Get-Content -LiteralPath $pidPath -Raw)
        if ($existingPid -ne $PID -and (Test-PipelineProcess $existingPid)) {
            Write-PipelineLog "ALREADY RUNNING pid=$existingPid"
            exit 0
        }
    } catch {
        # Replace malformed/stale PID files below.
    }
}

Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
Write-PipelineLog "START pipeline pid=$PID"

try {
    if (Test-Path -LiteralPath $completePath) {
        Write-PipelineLog "ALREADY COMPLETE marker=$completePath"
        exit 0
    }

    Invoke-Checked 'quality-tail manifest' {
        & $python $resolver --labels $labels --out-dir $outDir --manifest-only
    }

    while (-not (Test-Path -LiteralPath $autoReComplete)) {
        if (Test-Path -LiteralPath $stopPath) {
            Write-PipelineLog "STOP marker found at $stopPath"
            exit 0
        }
        Write-PipelineLog 'WAIT active auto-RE queue has not completed'
        Start-Sleep -Seconds $PollSeconds
    }

    # The terrain loop uses the same Ghidra project. Its cached workers normally
    # never open Ghidra, but waiting for completion makes project ownership explicit.
    Export-Signatures 'Fable.exe' $targetSignatures
    Export-Signatures 'ego_r.exe' $egoSignatures
    Export-Signatures 'FableWin.exe' $editorSignatures

    Invoke-Checked 'offline donor resolution' {
        & $python $resolver --labels $labels --out-dir $outDir `
            --target-signatures $targetSignatures `
            --donor "ego_r=$egoSignatures" --donor "FableWin=$editorSignatures"
    }

    Invoke-Checked 'auto-RE candidate compile gate' {
        & $python $compileGate --root $root
    }
    Invoke-Checked 'auto-RE signature audit' {
        & $python $signatureAudit --root $root
    }
    Invoke-Checked 'VC7.1 candidate build' {
        & powershell.exe -NoProfile -ExecutionPolicy Bypass -File $candidateBuild
    }
    Invoke-Checked 'retail candidate oracle export' {
        & powershell.exe -NoProfile -ExecutionPolicy Bypass -File $oracleExport
    }
    Invoke-Checked 'candidate retail parity comparison' {
        & $python $parityCompare --root $root
    }
    Invoke-Checked 'rebuild manifest refresh' {
        & $python $bootstrap --root $root
    }
    Invoke-Checked 'naming quality triage' {
        & $python $namingTriage --root $root
    }
    Invoke-Checked 'reconstruction backlog refresh' {
        & $python $backlog --root $root
    }
    Invoke-Checked 'decomp dashboard refresh' {
        & $python $dashboard --root $root
    }

    $completedAt = Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz'
    Set-Content -LiteralPath $completePath -Value $completedAt -Encoding UTF8
    Write-PipelineLog "COMPLETE $completedAt"
    try {
        $shell = New-Object -ComObject WScript.Shell
        [void]$shell.Popup(
            "Naming straggler proposals are ready.`nNo Ghidra labels were changed.`nSee ghidra_out\naming_stragglers\summary.md",
            30,
            'FableTLC naming resolver complete',
            64
        )
    } catch {
        Write-PipelineLog "WARN desktop notification failed: $($_.Exception.Message)"
    }
} catch {
    Write-PipelineLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Write-PipelineLog "END pipeline pid=$PID"
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
}
