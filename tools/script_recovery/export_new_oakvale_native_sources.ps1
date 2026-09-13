param(
    [string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot "..\..")),
    [string]$GhidraHome = "D:\Subuwu\tools\ghidra-public"
)

$ErrorActionPreference = "Stop"
$entityDirectory = Join-Path $Workspace "refs\script_recovery\new_oakvale_intro\entities"
$addresses = Get-ChildItem -LiteralPath $entityDirectory -Filter "*.json" -File |
    ForEach-Object {
        $inventory = Get-Content -LiteralPath $_.FullName -Raw | ConvertFrom-Json
        $inventory.functions.PSObject.Properties.Value.address
    } |
    Sort-Object { [Convert]::ToUInt32($_.Substring(2), 16) } -Unique

$headless = Join-Path $GhidraHome "support\analyzeHeadless.bat"
$scriptDirectory = Join-Path $Workspace "tools\ghidra_scripts"
$outputDirectory = Join-Path $Workspace "ghidra_out\script_recovery\native_decompiles"
$arguments = @(
    (Join-Path $Workspace "ghidra_proj"), "FableTLC",
    "-process", "Fable.exe", "-readOnly", "-noanalysis",
    "-scriptPath", $scriptDirectory,
    "-postScript", "DecompFuncsToDirectory.java", $outputDirectory
) + $addresses

& $headless @arguments
if ($LASTEXITCODE -ne 0) {
    throw "Ghidra export failed with exit code $LASTEXITCODE"
}

python (Join-Path $Workspace "tools\script_recovery\audit_new_oakvale_native_sources.py")
if ($LASTEXITCODE -ne 0) {
    throw "New Oakvale native-source audit failed"
}
