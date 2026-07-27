[CmdletBinding()]
param(
    [ValidateSet('boot', 'lionhead', 'microsoft', 'intro', 'attract')]
    [string[]]$Movie = @('boot'),

    [ValidateSet(2, 3, 4)]
    [int]$Scale = 2,

    [ValidateSet(
        'realesr-animevideov3',
        'realesrgan-plus-anime',
        'realesrgan-plus'
    )]
    [string]$Model = 'realesr-animevideov3',

    [int]$Gpu = 0,

    [int]$VideoBitRate = 8000000,

    [switch]$InstallVideo2X,

    [switch]$Force,

    [string]$OutputDirectory
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $PSCommandPath
$buildRoot = Join-Path $rebuildRoot 'build'
$video2XVersion = '6.4.0'
$video2XArchiveName = 'video2x-windows-amd64-6.4.0.zip'
$video2XArchiveHash =
    '0337B6DCAE2BAD2FD13E43FBDB73EF728D2F6289F37CC3617E87AC3A1A5CF793'
$video2XUrl =
    'https://github.com/k4yt3x/video2x/releases/download/6.4.0/' +
    'video2x-windows-amd64.zip'
$toolsRoot = Join-Path $buildRoot 'tools'
$video2XRoot = Join-Path $toolsRoot "video2x-$video2XVersion"
$video2XExe = Join-Path $video2XRoot 'video2x.exe'

if ([string]::IsNullOrWhiteSpace($OutputDirectory)) {
    $OutputDirectory = Join-Path (
        Join-Path $buildRoot 'bootstrap-Release'
    ) 'upscaled-video'
}
if ($Model -ne 'realesr-animevideov3' -and $Scale -ne 4) {
    throw (
        "$Model ships only a 4x model in Video2X. Use Scale 4 or select " +
        'realesr-animevideov3 for true 2x/3x processing.'
    )
}

function Install-PortableVideo2X {
    New-Item -ItemType Directory -Force -Path $toolsRoot | Out-Null
    $archivePath = Join-Path $toolsRoot $video2XArchiveName
    if (-not (Test-Path -LiteralPath $archivePath)) {
        Write-Host "Downloading Video2X $video2XVersion..."
        Invoke-WebRequest -Uri $video2XUrl -OutFile $archivePath
    }

    $actualHash = (
        Get-FileHash -Algorithm SHA256 -LiteralPath $archivePath
    ).Hash
    if ($actualHash -ne $video2XArchiveHash) {
        throw (
            "Video2X archive hash mismatch. Expected $video2XArchiveHash, " +
            "received $actualHash."
        )
    }

    if (-not (Test-Path -LiteralPath $video2XExe)) {
        Expand-Archive -LiteralPath $archivePath -DestinationPath $video2XRoot
    }
    if (-not (Test-Path -LiteralPath $video2XExe)) {
        throw "Video2X executable was not found after extraction."
    }
}

if (-not (Test-Path -LiteralPath $video2XExe)) {
    if (-not $InstallVideo2X) {
        throw (
            "Portable Video2X $video2XVersion is not installed. Re-run with " +
            '-InstallVideo2X to download the pinned, hash-verified release.'
        )
    }
    Install-PortableVideo2X
}

$ffprobe = Get-Command ffprobe -ErrorAction SilentlyContinue
if (-not $ffprobe) {
    throw 'ffprobe is required to validate source and enhanced movies.'
}

$uninstallKey =
    'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' +
    'Steam App 204030'
$installLocation = (
    Get-ItemProperty -LiteralPath $uninstallKey -ErrorAction Stop
).InstallLocation
$retailVideoDirectory = Join-Path $installLocation 'data\Video'
if (-not (Test-Path -LiteralPath $retailVideoDirectory)) {
    throw "Retail video directory was not found: $retailVideoDirectory"
}

$movieNames = [System.Collections.Generic.List[string]]::new()
foreach ($selection in $Movie) {
    $expanded = switch ($selection) {
        'boot' {
            @(
                'lionhead_logo.wmv',
                'microsoft_logo.wmv',
                'intro_comp.wmv'
            )
        }
        'lionhead' { @('lionhead_logo.wmv') }
        'microsoft' { @('microsoft_logo.wmv') }
        'intro' { @('intro_comp.wmv') }
        'attract' { @('fable_attract_english.wmv') }
    }
    foreach ($name in $expanded) {
        if (-not $movieNames.Contains($name)) {
            $movieNames.Add($name)
        }
    }
}

function Get-VideoProbe([string]$Path) {
    $json = & $ffprobe.Source `
        -v error `
        -count_frames `
        -show_entries `
        'format=duration:stream=codec_name,codec_type,width,height,channels,nb_read_frames' `
        -of json `
        $Path
    if ($LASTEXITCODE -ne 0) {
        throw "ffprobe failed for $Path"
    }
    return $json | ConvertFrom-Json
}

function Assert-EnhancedMovie(
    [string]$SourcePath,
    [string]$EnhancedPath
) {
    $sourceProbe = Get-VideoProbe $SourcePath
    $enhancedProbe = Get-VideoProbe $EnhancedPath
    $sourceVideo = $sourceProbe.streams |
        Where-Object codec_type -eq 'video' |
        Select-Object -First 1
    $enhancedVideo = $enhancedProbe.streams |
        Where-Object codec_type -eq 'video' |
        Select-Object -First 1
    $sourceAudio = $sourceProbe.streams |
        Where-Object codec_type -eq 'audio' |
        Select-Object -First 1
    $enhancedAudio = $enhancedProbe.streams |
        Where-Object codec_type -eq 'audio' |
        Select-Object -First 1

    if (
        $enhancedVideo.width -ne ($sourceVideo.width * $Scale) -or
        $enhancedVideo.height -ne ($sourceVideo.height * $Scale) -or
        $enhancedVideo.codec_name -ne 'wmv2'
    ) {
        throw "Enhanced dimensions are incorrect for $EnhancedPath"
    }
    if (
        $enhancedVideo.nb_read_frames -ne $sourceVideo.nb_read_frames
    ) {
        throw "Enhanced frame count differs for $EnhancedPath"
    }
    if (
        -not $sourceAudio -or
        -not $enhancedAudio -or
        $enhancedAudio.channels -ne $sourceAudio.channels -or
        $enhancedAudio.codec_name -ne $sourceAudio.codec_name -or
        $enhancedAudio.nb_read_frames -ne $sourceAudio.nb_read_frames
    ) {
        throw "Enhanced audio stream differs for $EnhancedPath"
    }

    [pscustomobject]@{
        SourceWidth = [int]$sourceVideo.width
        SourceHeight = [int]$sourceVideo.height
        Width = [int]$enhancedVideo.width
        Height = [int]$enhancedVideo.height
        Frames = [int]$enhancedVideo.nb_read_frames
        AudioPackets = [int]$enhancedAudio.nb_read_frames
        Duration = [double]$enhancedProbe.format.duration
        AudioCodec = [string]$enhancedAudio.codec_name
        AudioChannels = [int]$enhancedAudio.channels
    }
}

New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null

foreach ($movieName in $movieNames) {
    $sourcePath = Join-Path $retailVideoDirectory $movieName
    $enhancedPath = Join-Path $OutputDirectory $movieName
    $partialPath = "$enhancedPath.partial.wmv"
    $manifestPath = "$enhancedPath.upscale.json"
    if (-not (Test-Path -LiteralPath $sourcePath)) {
        throw "Retail movie was not found: $sourcePath"
    }

    $sourceHash = (
        Get-FileHash -Algorithm SHA256 -LiteralPath $sourcePath
    ).Hash
    if (
        -not $Force -and
        (Test-Path -LiteralPath $enhancedPath) -and
        (Test-Path -LiteralPath $manifestPath)
    ) {
        $manifest = Get-Content -Raw -LiteralPath $manifestPath |
            ConvertFrom-Json
        if (
            $manifest.source_sha256 -eq $sourceHash -and
            $manifest.scale -eq $Scale -and
            $manifest.model -eq $Model -and
            $manifest.video2x_version -eq $video2XVersion -and
            $manifest.video_bitrate -eq $VideoBitRate
        ) {
            $validation = Assert-EnhancedMovie $sourcePath $enhancedPath
            Write-Host (
                "AI_UPSCALE SKIP name=$movieName " +
                "size=$($validation.Width)x$($validation.Height) " +
                "frames=$($validation.Frames)"
            )
            continue
        }
    }

    if (Test-Path -LiteralPath $partialPath) {
        Remove-Item -LiteralPath $partialPath
    }
    Write-Host (
        "AI_UPSCALE START name=$movieName scale=${Scale}x model=$Model"
    )
    & $video2XExe `
        -i $sourcePath `
        -o $partialPath `
        -p realesrgan `
        -s $Scale `
        --realesrgan-model $Model `
        -d $Gpu `
        -c wmv2 `
        --bit-rate $VideoBitRate `
        --max-b-frames 0 `
        --qmin 2 `
        --qmax 31 `
        --no-progress `
        --log-level info
    if ($LASTEXITCODE -ne 0) {
        throw "Video2X failed for $movieName with exit code $LASTEXITCODE"
    }

    $validation = Assert-EnhancedMovie $sourcePath $partialPath
    Move-Item -Force -LiteralPath $partialPath -Destination $enhancedPath
    $manifest = [ordered]@{
        movie = $movieName
        source_path = $sourcePath
        source_sha256 = $sourceHash
        output_sha256 = (
            Get-FileHash -Algorithm SHA256 -LiteralPath $enhancedPath
        ).Hash
        scale = $Scale
        model = $Model
        video2x_version = $video2XVersion
        video_bitrate = $VideoBitRate
        width = $validation.Width
        height = $validation.Height
        frames = $validation.Frames
        audio_packets = $validation.AudioPackets
        duration = $validation.Duration
        audio_codec = $validation.AudioCodec
        audio_channels = $validation.AudioChannels
    }
    $manifest | ConvertTo-Json |
        Set-Content -Encoding UTF8 -LiteralPath $manifestPath
    Write-Host (
        "AI_UPSCALE PASS name=$movieName " +
        "size=$($validation.Width)x$($validation.Height) " +
        "frames=$($validation.Frames) " +
        "audio=$($validation.AudioCodec)/$($validation.AudioChannels)ch"
    )
}

Write-Host "AI_UPSCALE_COMPLETE output=$OutputDirectory"
