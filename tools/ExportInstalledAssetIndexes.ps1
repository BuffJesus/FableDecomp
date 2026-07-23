param(
    [string]$GameRoot = "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters",
    [string]$EgoCoreExe = "C:/Users/Cornelio/Documents/EgoCoreInspect/EgoCore-master/x64/Release/EgoCore.exe",
    [string]$WadBridgeExe = "D:/Code/Fable/ChocolateBox/ChocolateBox/src/SilverChest.WadBridge/bin/Debug/net48/SilverChest.WadBridge.exe",
    [string]$OutDir = "ghidra_out/installed_game"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if (!(Test-Path -LiteralPath $GameRoot)) { throw "Game root not found: $GameRoot" }
if (!(Test-Path -LiteralPath $EgoCoreExe)) { throw "EgoCore not found: $EgoCoreExe" }
if (!(Test-Path -LiteralPath $WadBridgeExe)) { throw "WAD bridge not found: $WadBridgeExe" }

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$dataRoot = Join-Path $GameRoot "data"
$bankFiles = Get-ChildItem -LiteralPath $dataRoot -Recurse -File |
    Where-Object { $_.Extension.Equals(".big", [StringComparison]::OrdinalIgnoreCase) } |
    Sort-Object FullName
$allEntries = New-Object System.Collections.Generic.List[object]

foreach ($bank in $bankFiles) {
    $rel = $bank.FullName.Substring((Resolve-Path -LiteralPath $GameRoot).Path.Length + 1)
    $safe = ($rel -replace '[:\\\/ ]+', '_')
    $listPath = Join-Path $OutDir ("egocore_" + $safe + ".tsv")

    & $EgoCoreExe --list $bank.FullName | Set-Content -LiteralPath $listPath -Encoding UTF8
    if ($LASTEXITCODE -ne 0) {
        throw "EgoCore --list failed for $($bank.FullName) with exit $LASTEXITCODE"
    }

    Get-Content -LiteralPath $listPath | ForEach-Object {
        if ($_ -match '^\d+\t') {
            $parts = $_ -split "`t"
            if ($parts.Count -ge 6) {
                $allEntries.Add([pscustomobject]@{
                    bank = $rel
                    idx = [int]$parts[0]
                    type = [int]$parts[1]
                    size = [int64]$parts[2]
                    id = [int64]$parts[3]
                    name = $parts[4]
                    friendly = $parts[5]
                })
            }
        }
    }
}

$allEntries |
    Sort-Object bank, idx |
    Export-Csv -LiteralPath (Join-Path $OutDir "big_entries.csv") -NoTypeInformation

$allEntries |
    Group-Object bank |
    Sort-Object Name |
    ForEach-Object {
        [pscustomobject]@{
            bank = $_.Name
            entries = $_.Count
            bytes = ($_.Group | Measure-Object size -Sum).Sum
        }
    } |
    Export-Csv -LiteralPath (Join-Path $OutDir "big_banks.csv") -NoTypeInformation

$allEntries |
    Group-Object type |
    Sort-Object Count -Descending |
    ForEach-Object {
        [pscustomobject]@{
            type = $_.Name
            entries = $_.Count
            bytes = ($_.Group | Measure-Object size -Sum).Sum
        }
    } |
    Export-Csv -LiteralPath (Join-Path $OutDir "big_entry_types.csv") -NoTypeInformation

$wadPath = Join-Path $GameRoot "data/Levels/FinalAlbion.wad"
if (Test-Path -LiteralPath $wadPath) {
    $wadJson = Join-Path $OutDir "finalalbion_wad_entries.json"
    & $WadBridgeExe --list $wadPath | Set-Content -LiteralPath $wadJson -Encoding UTF8
    if ($LASTEXITCODE -ne 0) {
        throw "WAD bridge --list failed with exit $LASTEXITCODE"
    }

    $wadEntries = Get-Content -LiteralPath $wadJson -Raw | ConvertFrom-Json
    $wadEntries |
        ForEach-Object {
            [pscustomobject]@{
                index = $_.index
                name = $_.name
                extension = [IO.Path]::GetExtension([string]$_.name).ToLowerInvariant()
                size = $_.size
            }
        } |
        Export-Csv -LiteralPath (Join-Path $OutDir "finalalbion_wad_entries.csv") -NoTypeInformation
}

$tngRoot = Join-Path $GameRoot "data/Levels/FinalAlbion"
if (Test-Path -LiteralPath $tngRoot) {
    Get-ChildItem -LiteralPath $tngRoot -File -Filter "*.tng" |
        Sort-Object Name |
        ForEach-Object {
            [pscustomobject]@{
                file = $_.Name
                size = $_.Length
                new_things = @(Select-String -LiteralPath $_.FullName -Pattern '^\s*NewThing\s').Count
                script_names = @(Select-String -LiteralPath $_.FullName -Pattern '^\s*ScriptName\s').Count
                definition_types = @(Select-String -LiteralPath $_.FullName -Pattern '^\s*DefinitionType\s').Count
            }
        } |
        Export-Csv -LiteralPath (Join-Path $OutDir "loose_tng_summary.csv") -NoTypeInformation
}

"big_entries=$(Join-Path $OutDir "big_entries.csv")"
"wad_entries=$(Join-Path $OutDir "finalalbion_wad_entries.csv")"
"loose_tng_summary=$(Join-Path $OutDir "loose_tng_summary.csv")"
