param(
    [string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot "..\..")),
    [string]$GhidraHome = "D:\Subuwu\tools\ghidra-public"
)

$ErrorActionPreference = "Stop"
$clusterPath = Join-Path $Workspace "refs\script_recovery\native_clusters\Q_NewOakValeIntro.json"
$cluster = Get-Content -Raw -LiteralPath $clusterPath | ConvertFrom-Json
$addresses = @(
    $cluster.allocatorAddress,
    $cluster.constructorAddress,
    $cluster.baseConstructorAddress,
    $cluster.destructorImplementationAddress,
    $cluster.baseDestructorAddress
) + @($cluster.lifecycle.address) |
    Sort-Object { [Convert]::ToUInt32($_.Substring(2), 16) } -Unique
$arguments = @(
    (Join-Path $Workspace "ghidra_proj"), "FableTLC",
    "-process", "Fable.exe", "-readOnly", "-noanalysis",
    "-scriptPath", (Join-Path $Workspace "tools\ghidra_scripts"),
    "-postScript", "DecompFuncsToDirectory.java",
    (Join-Path $Workspace "ghidra_out\script_recovery\native_quest_construction")
) + $addresses
& (Join-Path $GhidraHome "support\analyzeHeadless.bat") @arguments
if ($LASTEXITCODE -ne 0) { throw "Ghidra quest-construction export failed with exit code $LASTEXITCODE" }

python (Join-Path $Workspace "tools\script_recovery\audit_new_oakvale_quest_construction.py")
if ($LASTEXITCODE -ne 0) { throw "New Oakvale quest-construction audit failed" }
