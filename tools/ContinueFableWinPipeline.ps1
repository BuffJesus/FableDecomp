param(
    [int]$ImportPid = 15816
)

$ErrorActionPreference = "Stop"

$Root = "D:\Documents\FableTLC"
$Out = Join-Path $Root "ghidra_out"
$ProjDir = Join-Path $Root "ghidra_proj"
$Scripts = Join-Path $Root "tools\ghidra_scripts"
$Headless = "D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat"
$Bsim = "D:\Subuwu\tools\ghidra-public\support\bsim.bat"
$Plan = "D:/Documents/FableTLC/ghidra_out/fablewin_pdb_names.tsv"
$DbUrl = "file:/D:/Documents/FableTLC/bsim/fabletlc2"
$DriverLog = Join-Path $Out "continue_fablewin_pipeline.log"

function Log($Message) {
    $line = "$(Get-Date -Format s) $Message"
    $line | Tee-Object -FilePath $DriverLog -Append
}

function Run-Step($Name, $LogName, [scriptblock]$Body) {
    $log = Join-Path $Out $LogName
    Log "START $Name -> $log"
    $global:LASTEXITCODE = 0
    try {
        & $Body *> $log
    }
    catch {
        Log "FAIL $Name exception=$($_.Exception.Message)"
        throw
    }
    if ($LASTEXITCODE -ne 0) {
        Log "FAIL $Name exit=$LASTEXITCODE"
        throw "$Name failed with exit code $LASTEXITCODE"
    }
    Log "DONE $Name"
}

Set-Location $Root
Log "Pipeline driver starting; waiting for import pid $ImportPid"

$p = Get-Process -Id $ImportPid -ErrorAction SilentlyContinue
if ($p) {
    Wait-Process -Id $ImportPid
}

if (-not (Select-String -Path (Join-Path $Out "analyze_fablewin.log") -Pattern "Save succeeded" -Quiet)) {
    Log "STOP analyze_fablewin.log does not contain Save succeeded"
    throw "FableWin import did not save cleanly; inspect ghidra_out\analyze_fablewin.log"
}

Run-Step "Apply FableWin PDB names" "apply_pdb_fablewin.log" {
    & $Headless $ProjDir "FableTLC" -process "FableWin.exe" -noanalysis -scriptPath $Scripts -postScript "ApplyNames.java" $Plan
}

Run-Step "Create missing FableWin PDB functions" "create_missing_fablewin.log" {
    & $Headless $ProjDir "FableTLC" -process "FableWin.exe" -noanalysis -scriptPath $Scripts -postScript "CreateMissingFunctions.java" $Plan
}

Run-Step "Re-apply FableWin PDB names" "apply_pdb_fablewin2.log" {
    & $Headless $ProjDir "FableTLC" -process "FableWin.exe" -noanalysis -scriptPath $Scripts -postScript "ApplyNames.java" $Plan
}

Run-Step "Generate upgraded ego_r BSim signatures" "bsim_gensigs_egor_upgraded.log" {
    & $Bsim generatesigs "ghidra:/D:/Documents/FableTLC/ghidra_proj/FableTLC?/ego_r.exe" "D:/Documents/FableTLC/bsim/sigs_egor_upgraded" --config medium_nosize
}

Run-Step "Commit upgraded ego_r BSim signatures" "bsim_commitsigs_egor_upgraded.log" {
    & $Bsim commitsigs $DbUrl "D:/Documents/FableTLC/bsim/sigs_egor_upgraded"
}

Run-Step "Generate FableWin BSim signatures" "bsim_gensigs_fablewin.log" {
    & $Bsim generatesigs "ghidra:/D:/Documents/FableTLC/ghidra_proj/FableTLC?/FableWin.exe" "D:/Documents/FableTLC/bsim/sigs_fablewin" --config medium_nosize
}

Run-Step "Commit FableWin BSim signatures" "bsim_commitsigs_fablewin.log" {
    & $Bsim commitsigs $DbUrl "D:/Documents/FableTLC/bsim/sigs_fablewin"
}

Run-Step "BSim port to Fable.exe at 0.7" "bsim_apply_fabletlc2_07.log" {
    & $Headless $ProjDir "FableTLC" -process "Fable.exe" -noanalysis -scriptPath $Scripts -postScript "BSimApplyNames.java" $DbUrl "0.7" "0.0" "D:/Documents/FableTLC/ghidra_out/bsim_port_audit_fabletlc2_07.tsv"
}

Run-Step "BSim port to Fable.exe at 0.5" "bsim_apply_fabletlc2_05.log" {
    & $Headless $ProjDir "FableTLC" -process "Fable.exe" -noanalysis -scriptPath $Scripts -postScript "BSimApplyNames.java" $DbUrl "0.5" "0.0" "D:/Documents/FableTLC/ghidra_out/bsim_port_audit_fabletlc2_05.tsv"
}

Run-Step "Demangle Fable.exe" "demangle_fable_after_fablewin.log" {
    & $Headless $ProjDir "FableTLC" -process "Fable.exe" -noanalysis -scriptPath $Scripts -postScript "DemangleAll.java"
}

Run-Step "Dump Fable.exe stats" "stats_fable_after_fablewin.log" {
    & $Headless $ProjDir "FableTLC" -process "Fable.exe" -noanalysis -scriptPath $Scripts -postScript "DumpStats.java"
}

Log "Pipeline complete"
