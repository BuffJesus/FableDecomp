param(
    [string]$GameRoot = "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters",
    [string]$OutputDir = "ghidra_out/installed_game",
    [string]$BridgePath = "D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.StbBridge\bin\Debug\net48\SilverChest.StbBridge.exe"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$stbPath = Join-Path $GameRoot "data\Levels\FinalAlbion_RT.stb"
if (!(Test-Path -LiteralPath $stbPath)) {
    throw "STB not found: $stbPath"
}
if (!(Test-Path -LiteralPath $BridgePath)) {
    throw "STB bridge not found: $BridgePath"
}

$jsonPath = Join-Path $OutputDir "finalalbion_stb_entries.json"
$csvPath = Join-Path $OutputDir "finalalbion_stb_entries.csv"
$summaryPath = Join-Path $OutputDir "stb_summary.txt"

& $BridgePath --list $stbPath | Set-Content -LiteralPath $jsonPath -Encoding UTF8
if ($LASTEXITCODE -ne 0) {
    throw "SilverChest.StbBridge --list failed with exit code $LASTEXITCODE"
}

$entries = Get-Content -LiteralPath $jsonPath -Raw | ConvertFrom-Json
$entries |
    Select-Object index,name,size,offset,@{Name="extension";Expression={[IO.Path]::GetExtension($_.name).ToLowerInvariant()}} |
    Export-Csv -LiteralPath $csvPath -NoTypeInformation

$totalBytes = 0L
foreach ($entry in $entries) {
    $totalBytes += [int64]$entry.size
}

@(
    "stb_path=$stbPath"
    "entry_count=$(($entries | Measure-Object).Count)"
    "entry_bytes=$totalBytes"
) | Set-Content -LiteralPath $summaryPath -Encoding ASCII

"entries=$(($entries | Measure-Object).Count) bytes=$totalBytes"
