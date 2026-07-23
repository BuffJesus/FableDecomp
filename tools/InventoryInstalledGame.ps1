param(
    [string]$GameRoot = "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters",
    [string]$OutDir = "ghidra_out/installed_game"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if (!(Test-Path -LiteralPath $GameRoot)) {
    throw "Game root not found: $GameRoot"
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$files = Get-ChildItem -LiteralPath $GameRoot -Recurse -File | ForEach-Object {
    [pscustomobject]@{
        relative_path = $_.FullName.Substring((Resolve-Path -LiteralPath $GameRoot).Path.Length + 1)
        extension = $_.Extension.ToLowerInvariant()
        size = $_.Length
        last_write = $_.LastWriteTime.ToString("s")
    }
}

$files |
    Sort-Object relative_path |
    Export-Csv -LiteralPath (Join-Path $OutDir "files.csv") -NoTypeInformation

$files |
    Group-Object extension |
    Sort-Object Count -Descending |
    ForEach-Object {
        [pscustomobject]@{
            extension = if ($_.Name) { $_.Name } else { "(none)" }
            count = $_.Count
            bytes = ($_.Group | Measure-Object size -Sum).Sum
        }
    } |
    Export-Csv -LiteralPath (Join-Path $OutDir "extensions.csv") -NoTypeInformation

$levelRoot = Join-Path $GameRoot "data/Levels"
$finalAlbion = Join-Path $levelRoot "FinalAlbion"
$summary = [ordered]@{
    game_root = (Resolve-Path -LiteralPath $GameRoot).Path
    fable_exe = Test-Path -LiteralPath (Join-Path $GameRoot "Fable.exe")
    fse_dll = Test-Path -LiteralPath (Join-Path $GameRoot "FableScriptExtender.dll")
    fse_folder = Test-Path -LiteralPath (Join-Path $GameRoot "FSE")
    finalalbion_wad = Test-Path -LiteralPath (Join-Path $levelRoot "FinalAlbion.wad")
    finalalbion_folder = Test-Path -LiteralPath $finalAlbion
    loose_tng_count = if (Test-Path -LiteralPath $finalAlbion) { @(Get-ChildItem -LiteralPath $finalAlbion -Filter "*.tng" -File).Count } else { 0 }
    loose_lev_count = if (Test-Path -LiteralPath $finalAlbion) { @(Get-ChildItem -LiteralPath $finalAlbion -Filter "*.lev" -File).Count } else { 0 }
    generated = (Get-Date).ToString("s")
}

($summary.GetEnumerator() | ForEach-Object { "$($_.Key)=$($_.Value)" }) |
    Set-Content -LiteralPath (Join-Path $OutDir "summary.txt") -Encoding UTF8

"summary=$(Join-Path $OutDir "summary.txt")"
"files=$(Join-Path $OutDir "files.csv")"
"extensions=$(Join-Path $OutDir "extensions.csv")"
