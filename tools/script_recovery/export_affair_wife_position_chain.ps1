param(
    [string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot "..\..")),
    [string]$GhidraHome = "D:\Subuwu\tools\ghidra-public"
)

$ErrorActionPreference = "Stop"
$rows = Import-Csv -Delimiter "`t" -LiteralPath (Join-Path $Workspace "rebuild\manifest\functions.tsv")
$wanted = @(
    @{ module = "CScriptThing"; name = "?GetPos@CScriptThing@@UBEABVC3DVector@@XZ" },
    @{ module = "CGameScriptThing"; name = "CGameScriptThing::GetPos" },
    @{ module = "_global"; name = "IsDistanceBetweenThingsUnder" }
)
$addresses = foreach ($target in $wanted) {
    $matches = @($rows | Where-Object { $_.module -eq $target.module -and $_.name -eq $target.name })
    if ($matches.Count -ne 1) { throw "Expected one manifest row for $($target.module)::$($target.name)" }
    "0x$($matches[0].address)"
}

$arguments = @(
    (Join-Path $Workspace "ghidra_proj"), "FableTLC",
    "-process", "Fable.exe", "-readOnly", "-noanalysis",
    "-scriptPath", (Join-Path $Workspace "tools\ghidra_scripts"),
    "-postScript", "DecompFuncsToDirectory.java",
    (Join-Path $Workspace "ghidra_out\script_recovery\native_support")
) + $addresses
& (Join-Path $GhidraHome "support\analyzeHeadless.bat") @arguments
if ($LASTEXITCODE -ne 0) { throw "Ghidra position-chain export failed with exit code $LASTEXITCODE" }

python (Join-Path $Workspace "tools\script_recovery\audit_affair_wife_position_chain.py")
if ($LASTEXITCODE -ne 0) { throw "Affair Wife position-chain audit failed" }
