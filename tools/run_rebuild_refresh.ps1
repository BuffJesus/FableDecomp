param(
    [switch]$Force,
    [switch]$ResumeAfterOracle
)

$ErrorActionPreference = 'Stop'
$root = 'D:\Documents\FableTLC'
$tools = Join-Path $root 'tools'
$rebuild = Join-Path $root 'rebuild'
$log = Join-Path $rebuild 'refresh.log'
$statePath = Join-Path $rebuild 'refresh.state.json'
$pidPath = Join-Path $rebuild 'refresh.pid'
$python = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\python.exe'

function Write-RefreshLog([string]$Message) {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $log -Value "[$stamp] $Message" -Encoding UTF8
}

function Test-ProcessCommand([int]$ProcessId, [string]$Needle) {
    $process = Get-CimInstance Win32_Process -Filter "ProcessId = $ProcessId" -ErrorAction SilentlyContinue
    return $null -ne $process -and $process.CommandLine -like "*$Needle*"
}

function Get-InputFingerprint {
    $inputs = @(
        Get-ChildItem (Join-Path $root 'lift\reports\primary\code') -Filter '*.cpp' -File -Recurse -ErrorAction SilentlyContinue
        Get-ChildItem (Join-Path $root 'lift\reports\retry\code') -Filter '*.cpp' -File -Recurse -ErrorAction SilentlyContinue
        Get-ChildItem (Join-Path $root 'lift\reports\wave2\code') -Filter '*.cpp' -File -Recurse -ErrorAction SilentlyContinue
        Get-ChildItem (Join-Path $root 'lift\reports\wave3\code') -Filter '*.cpp' -File -Recurse -ErrorAction SilentlyContinue
        Get-ChildItem (Join-Path $rebuild 'src\compiled') -Filter '*.cpp' -File -Recurse -ErrorAction SilentlyContinue
        Get-ChildItem (Join-Path $rebuild 'tests') -Filter '*.cpp' -File -Recurse -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'ghidra_out\engine_api.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'ghidra_out\naming_stragglers\manifest.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'ghidra_out\naming_stragglers\proposals.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'ghidra_out\naming_stragglers\unresolved.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'lift\state\re-agent-progress.json') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'lift\state\re-agent-retry-progress.json') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'lift\state\re-agent-wave2-progress.json') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'lift\state\re-agent-wave3-progress.json') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $rebuild 'build_candidates.ps1') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $rebuild 'corrections\function_overrides.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $rebuild 'corrections\msvc_prototype_overrides.tsv') -ErrorAction SilentlyContinue
        Get-ChildItem (Join-Path $rebuild 'include\candidates') -File -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'gate_re_agent_candidates.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'audit_candidate_signatures.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'compare_candidate_objects.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'bootstrap_rebuild_tree.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'recover_msvc_prototypes.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'gen_fable_engine_header.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'export_fse_native_overlay.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'build_forgefse_binding_queue.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'audit_forgefse_binding_slots.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'validate_tooling_sdk.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'build_reconstruction_backlog.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'build_promotion_queue.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'triage_naming_quality.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'work\frontend_re\manager_init2_used_key_manifest.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'work\frontend_re\manager_init2_resource_manifest.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'work\frontend_re\manager_init2_screen_children.tsv') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'validate_frontend_init2_manifest.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $root 'work\frontend_re\CFrontEndManager_GetProfileNames_005955ab_contract.md') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'validate_frontend_profile_names_contract.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'write_decomp_dashboard.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'artifact_layout.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'organize_decomp_artifacts.py') -ErrorAction SilentlyContinue
        Get-Item (Join-Path $tools 'update_readme_progress.py') -ErrorAction SilentlyContinue
    ) | Where-Object { $null -ne $_ } | Sort-Object FullName
    $description = ($inputs | ForEach-Object {
        "$($_.FullName)|$($_.Length)|$($_.LastWriteTimeUtc.Ticks)"
    }) -join "`n"
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try {
        $bytes = [System.Text.Encoding]::UTF8.GetBytes($description)
        return ([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-', '').ToLowerInvariant()
    } finally {
        $sha.Dispose()
    }
}

function Invoke-Checked([string]$Description, [scriptblock]$Action) {
    Write-RefreshLog "START $Description"
    & $Action
    if ($LASTEXITCODE -ne 0) {
        throw "$Description failed with exit code $LASTEXITCODE"
    }
    Write-RefreshLog "DONE $Description"
}

New-Item -ItemType Directory -Path $rebuild -Force | Out-Null
if (Test-Path -LiteralPath $pidPath) {
    try {
        $existingPid = [int](Get-Content -LiteralPath $pidPath -Raw)
        if ($existingPid -ne $PID -and (Test-ProcessCommand $existingPid 'run_rebuild_refresh.ps1')) {
            Write-RefreshLog "SKIP already running pid=$existingPid"
            exit 0
        }
    } catch {
        # A malformed or stale PID is replaced below.
    }
}

$retryPid = Join-Path $root 'lift\state\re-agent-retry-queue.pid'
$wave2Pid = Join-Path $root 'lift\state\re-agent-wave2-queue.pid'
$wave3Pids = @((Join-Path $root 'lift\state\re-agent-wave3-queue.pid'))
$namingPid = Join-Path $root 'ghidra_out\naming_stragglers\pipeline.pid'
$localParityPid = Join-Path $rebuild 'local-parity\queue.pid'
if (Test-Path -LiteralPath $localParityPid) {
    try {
        $activeLocalParityPid = [int](Get-Content -LiteralPath $localParityPid -Raw)
        if (Test-ProcessCommand $activeLocalParityPid 'run_local_parity_queue.ps1') {
            Write-RefreshLog "DEFER local parity queue is mutating the candidate catalog pid=$activeLocalParityPid"
            exit 0
        }
    } catch {
        Write-RefreshLog 'DEFER local parity queue PID cannot be validated yet'
        exit 0
    }
}
if (Test-Path -LiteralPath $retryPid) {
    Write-RefreshLog 'DEFER auto-RE retry queue is active'
    exit 0
}
if (Test-Path -LiteralPath $wave2Pid) {
    try {
        $activeWave2Pid = [int](Get-Content -LiteralPath $wave2Pid -Raw)
        if (Test-ProcessCommand $activeWave2Pid 'run_re_agent_wave2_queue.ps1') {
            Write-RefreshLog 'DEFER auto-RE wave 2 queue is active'
            exit 0
        }
    } catch {
        Write-RefreshLog 'DEFER auto-RE wave 2 PID cannot be validated yet'
        exit 0
    }
}
foreach ($wave3Pid in $wave3Pids) {
    if (Test-Path -LiteralPath $wave3Pid) {
        try {
            $activeWave3Pid = [int](Get-Content -LiteralPath $wave3Pid -Raw)
            if (Test-ProcessCommand $activeWave3Pid 'run_re_agent_wave3_queue.ps1') {
                Write-RefreshLog 'DEFER auto-RE wave 3 queue is active'
                exit 0
            }
        } catch {
            Write-RefreshLog 'DEFER auto-RE wave 3 PID cannot be validated yet'
            exit 0
        }
    }
}
if (Test-Path -LiteralPath $namingPid) {
    try {
        $activeNamingPid = [int](Get-Content -LiteralPath $namingPid -Raw)
        if (Test-ProcessCommand $activeNamingPid 'run_naming_straggler_pipeline.ps1') {
            Write-RefreshLog 'DEFER naming/signature pipeline is active'
            exit 0
        }
    } catch {
        Write-RefreshLog 'DEFER naming pipeline PID cannot be validated yet'
        exit 0
    }
}

& $python (Join-Path $tools 'organize_decomp_artifacts.py') --root $root --apply --allow-active
if ($LASTEXITCODE -ne 0) {
    throw "decomp artifact organization failed with exit code $LASTEXITCODE"
}

$fingerprint = Get-InputFingerprint
if (-not $Force -and (Test-Path -LiteralPath $statePath)) {
    try {
        $state = Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json
        if ($state.fingerprint -eq $fingerprint) {
            Write-RefreshLog "SKIP unchanged fingerprint=$fingerprint"
            exit 0
        }
    } catch {
        Write-RefreshLog 'WARN refresh state was unreadable; rebuilding'
    }
}

Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
Write-RefreshLog "START refresh pid=$PID fingerprint=$fingerprint"
try {
    if ($ResumeAfterOracle) {
        $compiledManifest = Join-Path $rebuild 'compile-gate\vc71-compiled.tsv'
        $oracleManifest = Join-Path $rebuild 'oracles\auto-re-candidates.tsv'
        if (-not (Test-Path -LiteralPath $compiledManifest) -or
            -not (Test-Path -LiteralPath $oracleManifest)) {
            throw 'ResumeAfterOracle requires compiled and retail-oracle manifests'
        }
        Write-RefreshLog 'RESUME after retail oracle export'
    } else {
        Invoke-Checked 'MSVC decorated prototype recovery' {
            & $python (Join-Path $tools 'recover_msvc_prototypes.py') --root $root
        }
        Invoke-Checked 'candidate compile gate' {
            & $python (Join-Path $tools 'gate_re_agent_candidates.py') --root $root
        }
        Invoke-Checked 'VC7.1 candidate build and behavior tests' {
            & powershell.exe -NoProfile -ExecutionPolicy Bypass -File (Join-Path $rebuild 'build_candidates.ps1')
        }
        Invoke-Checked 'retail oracle export' {
            & powershell.exe -NoProfile -ExecutionPolicy Bypass -File (Join-Path $tools 'ExportCandidateOracles.ps1')
        }
    }
    Invoke-Checked 'retail parity comparison' {
        & $python (Join-Path $tools 'compare_candidate_objects.py') --root $root
    }
    Invoke-Checked 'rebuild manifest' {
        & $python (Join-Path $tools 'bootstrap_rebuild_tree.py') --root $root
    }
    Invoke-Checked 'tooling engine ABI header' {
        & $python (Join-Path $tools 'gen_fable_engine_header.py') --root $root
    }
    Invoke-Checked 'FSE native binding overlay' {
        & $python (Join-Path $tools 'export_fse_native_overlay.py') --root $root
    }
    Invoke-Checked 'ForgeFSE binding verification queue' {
        & $python (Join-Path $tools 'build_forgefse_binding_queue.py') --root $root
    }
    Invoke-Checked 'ForgeFSE CGSI binding slot audit' {
        & $python (Join-Path $tools 'audit_forgefse_binding_slots.py') --root $root
    }
    Invoke-Checked 'tooling SDK validation' {
        & $python (Join-Path $tools 'validate_tooling_sdk.py') --root $root
    }
    Invoke-Checked 'candidate signature audit' {
        & $python (Join-Path $tools 'audit_candidate_signatures.py') --root $root
    }
    Invoke-Checked 'frontend Init2 manifest validation' {
        & $python (Join-Path $tools 'validate_frontend_init2_manifest.py') --root $root
    }
    Invoke-Checked 'frontend profile-name contract validation' {
        & $python (Join-Path $tools 'validate_frontend_profile_names_contract.py') --root $root
    }
    Invoke-Checked 'naming quality triage' {
        & $python (Join-Path $tools 'triage_naming_quality.py') --root $root
    }
    Invoke-Checked 'reconstruction backlog' {
        & $python (Join-Path $tools 'build_reconstruction_backlog.py') --root $root
    }
    Invoke-Checked 'curated promotion queue' {
        & $python (Join-Path $tools 'build_promotion_queue.py') --root $root
    }
    Invoke-Checked 'coverage dashboard' {
        & $python (Join-Path $tools 'write_decomp_dashboard.py') --root $root
    }
    Invoke-Checked 'public README progress' {
        & $python (Join-Path $tools 'update_readme_progress.py') --root $root
    }
    Invoke-Checked 'artifact layout index' {
        & $python (Join-Path $tools 'organize_decomp_artifacts.py') --root $root --apply --allow-active
    }
    $newState = [ordered]@{
        fingerprint = $fingerprint
        completed_at = (Get-Date).ToString('o')
    }
    $newState | ConvertTo-Json | Set-Content -LiteralPath "$statePath.tmp" -Encoding UTF8
    Move-Item -LiteralPath "$statePath.tmp" -Destination $statePath -Force
    Write-RefreshLog 'COMPLETE refresh'
} catch {
    Write-RefreshLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
}
