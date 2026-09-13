param(
    [string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot "..\..")),
    [string]$GhidraHome = "D:\Subuwu\tools\ghidra-public"
)

$ErrorActionPreference = "Stop"
$snapshot = Get-Content -LiteralPath (Join-Path $Workspace "ghidra_out\script_recovery\new_oakvale_entity_vtables.json") -Raw | ConvertFrom-Json
$addresses = $snapshot.entities.PSObject.Properties.Value.destructor |
    Sort-Object { [Convert]::ToUInt32($_.Substring(2), 16) } -Unique

$headless = Join-Path $GhidraHome "support\analyzeHeadless.bat"
$scriptDirectory = Join-Path $Workspace "tools\ghidra_scripts"
$outputDirectory = Join-Path $Workspace "ghidra_out\script_recovery\native_destructors"
$arguments = @(
    (Join-Path $Workspace "ghidra_proj"), "FableTLC",
    "-process", "Fable.exe", "-readOnly", "-noanalysis",
    "-scriptPath", $scriptDirectory,
    "-postScript", "DecompFuncsToDirectory.java", $outputDirectory
) + $addresses

& $headless @arguments
if ($LASTEXITCODE -ne 0) {
    throw "Ghidra destructor export failed with exit code $LASTEXITCODE"
}

$supportOutputDirectory = Join-Path $Workspace "ghidra_out\script_recovery\native_support"
$supportArguments = @(
    (Join-Path $Workspace "ghidra_proj"), "FableTLC",
    "-process", "Fable.exe", "-readOnly", "-noanalysis",
    "-scriptPath", $scriptDirectory,
    "-postScript", "DecompFuncsToDirectory.java", $supportOutputDirectory,
    $snapshot.sharedEntityCleanup.address
)
& $headless @supportArguments
if ($LASTEXITCODE -ne 0) {
    throw "Ghidra shared cleanup export failed with exit code $LASTEXITCODE"
}

python (Join-Path $Workspace "tools\script_recovery\audit_new_oakvale_destructors.py")
if ($LASTEXITCODE -ne 0) {
    throw "New Oakvale destructor audit failed"
}
