[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$ForgeExe,

    [Parameter(Mandatory = $true)]
    [string]$Workspace,

    [Parameter(Mandatory = $true)]
    [string]$SourceStb,

    [Parameter(Mandatory = $true)]
    [string]$OutputDir
)

$ErrorActionPreference = 'Stop'

function Resolve-RequiredFile([string]$Path, [string]$Label) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "$Label not found: $Path"
    }
    return (Resolve-Path -LiteralPath $Path).Path
}

function Invoke-Forge([object[]]$Arguments) {
    & $script:ResolvedForge @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "forge failed ($LASTEXITCODE): $($Arguments -join ' ')"
    }
}

$script:ResolvedForge = Resolve-RequiredFile $ForgeExe 'forge executable'
$resolvedWorkspace = (Resolve-Path -LiteralPath $Workspace).Path
$resolvedSourceStb = Resolve-RequiredFile $SourceStb 'source STB'
$levelsDir = Join-Path $resolvedWorkspace 'preview_material_levs'
$background = Resolve-RequiredFile `
    (Join-Path $resolvedWorkspace 'background_green_dxt1.bin') `
    'background inline texture'
$manifestTemplate = Resolve-RequiredFile `
    (Join-Path $resolvedWorkspace 'relocated_6144_coverage_candidate\manifest.json') `
    'relocated manifest template'

if (Test-Path -LiteralPath $OutputDir) {
    $existing = @(Get-ChildItem -LiteralPath $OutputDir -Force)
    if ($existing.Count -ne 0) {
        throw "output directory must be absent or empty: $OutputDir"
    }
} else {
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}
$resolvedOutput = (Resolve-Path -LiteralPath $OutputDir).Path
$chunkDir = New-Item -ItemType Directory -Path `
    (Join-Path $resolvedOutput 'material_chunks')
$infoDir = New-Item -ItemType Directory -Path `
    (Join-Path $resolvedOutput 'material_info')
$commonDir = New-Item -ItemType Directory -Path `
    (Join-Path $resolvedOutput 'material_common')
$extractDir = New-Item -ItemType Directory -Path `
    (Join-Path $resolvedOutput 'extracted_static_maps')

$themeArguments = @(
    '--theme-material', '20', '4200', '4200', '0', '4200', '4200', '0',
    '--theme-material', '21', '4241', '4241', '0', '4241', '4241', '0',
    '--theme-material', '22', '4263', '4263', '0', '4263', '4263', '0',
    '--theme-material', '23', '4277', '4277', '0', '4277', '4277', '0',
    '--theme-material', '24', '4287', '4287', '0', '4287', '4287', '0',
    '--theme-material', '25', '4289', '4289', '0', '4289', '4289', '0',
    '--theme-material', '26', '4315', '4315', '0', '4315', '4315', '0'
)

$totalFrames = 0
$totalTopologies = 0
for ($tile = 0; $tile -lt 30; ++$tile) {
    $name = 'BWSMarket_{0:D2}' -f $tile
    $lev = Resolve-RequiredFile (Join-Path $levelsDir "$name.lev") "$name LEV"
    $worldX = 6144 + (($tile % 5) * 64)
    $worldY = 6144 + ([math]::Floor($tile / 5) * 64)
    $bankIndex = 427 + $tile
    $chunk = Join-Path $chunkDir "$name.chunk.bin"
    $info = Join-Path $infoDir "$name.info.bin"
    $common = Join-Path $commonDir "$name.common.bin"

    $createArguments = @(
        'stb', 'create-terrain', $lev, $background,
        $worldX, $worldY, $bankIndex,
        '4200', '4200', '0', $chunk, $info
    ) + $themeArguments
    Invoke-Forge $createArguments
    Invoke-Forge @('stb', 'terrainrecord', $info, $chunk, $common)

    $gateOutput = (& $script:ResolvedForge stb foregroundinfo $chunk `
        --verify-topology --verify-roundtrip --verify-base-coverage 2>&1) -join "`n"
    if ($LASTEXITCODE -ne 0) {
        throw "$name foreground gate failed:`n$gateOutput"
    }
    $frames = [regex]::Match($gateOutput, 'foreground frames: (\d+)')
    $roundtrip = [regex]::Match(
        $gateOutput, 'foreground roundtrip exact: (\d+)/(\d+)')
    $coverage = [regex]::Match(
        $gateOutput, 'base coverage complete: (\d+)/(\d+)')
    $topology = [regex]::Match($gateOutput, 'topology exact: (\d+)/(\d+)')
    if (-not ($frames.Success -and $roundtrip.Success -and
              $coverage.Success -and $topology.Success)) {
        throw "$name foreground gate summary is incomplete"
    }
    $frameCount = [int]$frames.Groups[1].Value
    if ([int]$roundtrip.Groups[1].Value -ne $frameCount -or
        [int]$roundtrip.Groups[2].Value -ne $frameCount -or
        [int]$coverage.Groups[1].Value -ne $frameCount -or
        [int]$coverage.Groups[2].Value -ne $frameCount -or
        $topology.Groups[1].Value -ne $topology.Groups[2].Value) {
        throw "$name foreground gate is not exact"
    }
    $totalFrames += $frameCount
    $totalTopologies += [int]$topology.Groups[1].Value
    Write-Host "${name}: PASS ($frameCount frames)"
}

$manifest = Join-Path $resolvedOutput 'manifest.json'
Copy-Item -LiteralPath $manifestTemplate -Destination $manifest
$outStb = Join-Path $resolvedOutput 'FinalAlbion_RT.opaque_base.stb'
Invoke-Forge @(
    'stb', 'replace-static-map-batch-relayout',
    $resolvedSourceStb, $outStb, $manifest
)
$diffOutput = (& $script:ResolvedForge stb diff $resolvedSourceStb $outStb 2>&1) -join "`n"
if ($LASTEXITCODE -ne 0) {
    throw "STB payload diff failed:`n$diffOutput"
}
$changedNames = @([regex]::Matches($diffOutput, '(?m)^CHANGED\t([^\t\r\n]+)') |
    ForEach-Object { $_.Groups[1].Value })
$expectedChanged = @('__STATIC_MAP_COMMON_HEADER__') + @(
    0..29 | ForEach-Object {
        '__ENGINE_SEA_STATIC_MAP_BANK_FILE__BWSMarket_{0:D2}' -f $_
    })
$unexpected = @($changedNames | Where-Object { $_ -notin $expectedChanged })
$missing = @($expectedChanged | Where-Object { $_ -notin $changedNames })
if ($unexpected.Count -ne 0 -or $missing.Count -ne 0 -or
    $diffOutput -notmatch 'stb diff: unchanged=424 changed=31 added=0 removed=0') {
    throw "STB changed-entry scope mismatch; unexpected=$($unexpected -join ',') missing=$($missing -join ',')"
}
Invoke-Forge @(
    'stb', 'extract', $outStb, $extractDir,
    '__ENGINE_SEA_STATIC_MAP_BANK_FILE__BWSMarket'
)

$sourceChunks = @(Get-ChildItem -LiteralPath $chunkDir -Filter '*.chunk.bin' |
    Sort-Object Name)
$extractedChunks = @(Get-ChildItem -LiteralPath $extractDir -Recurse -File |
    Where-Object Name -Like '__ENGINE_SEA_STATIC_MAP_BANK_FILE__BWSMarket*' |
    Sort-Object Name)
if ($sourceChunks.Count -ne 30 -or $extractedChunks.Count -ne 30) {
    throw "expected 30 source and extracted chunks; got $($sourceChunks.Count) and $($extractedChunks.Count)"
}
for ($index = 0; $index -lt 30; ++$index) {
    $sourceHash = (Get-FileHash -LiteralPath $sourceChunks[$index].FullName `
        -Algorithm SHA256).Hash
    $extractedHash = (Get-FileHash -LiteralPath $extractedChunks[$index].FullName `
        -Algorithm SHA256).Hash
    if ($sourceHash -ne $extractedHash) {
        throw "relayout payload mismatch: $($sourceChunks[$index].Name)"
    }
}

$stbItem = Get-Item -LiteralPath $outStb
$stbHash = (Get-FileHash -LiteralPath $outStb -Algorithm SHA256).Hash
Write-Host "PASS: 30 tiles, $totalFrames frames, $totalTopologies topologies"
Write-Host "PASS: 30/30 relayout payload hashes"
Write-Host "PASS: 424 untouched payloads; exactly 30 maps + common header changed"
Write-Host "STB: $($stbItem.Length) bytes SHA-256 $stbHash"
