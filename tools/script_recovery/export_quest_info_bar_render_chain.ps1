param(
    [string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot "..\..")),
    [string]$GhidraHome = "D:\Subuwu\tools\ghidra-public"
)

$ErrorActionPreference = "Stop"
$outputDirectory = Join-Path $Workspace "ghidra_out\script_recovery\native_support"
$arguments = @(
    (Join-Path $Workspace "ghidra_proj"), "FableTLC",
    "-process", "Fable.exe", "-readOnly", "-noanalysis",
    "-scriptPath", (Join-Path $Workspace "tools\ghidra_scripts"),
    "-postScript", "DecompFuncsToDirectory.java", $outputDirectory,
    "0x0064F7FC", "0x00644DF8", "0x00656B1B", "0x006463FF"
)
& (Join-Path $GhidraHome "support\analyzeHeadless.bat") @arguments
if ($LASTEXITCODE -ne 0) { throw "Quest-info-bar render-chain export failed" }

python (Join-Path $Workspace "tools\script_recovery\audit_quest_info_bar_render_chain.py")
if ($LASTEXITCODE -ne 0) { throw "Quest-info-bar render-chain audit failed" }
