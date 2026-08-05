param(
    [string]$Executable = '',

    [int]$TimeoutSeconds = 10,

    [switch]$RetailVideo,

    [switch]$VerifyBootSequence,

    [switch]$VerifyEscapeSkip,

    [switch]$VerifyBootToFrontend,

    [switch]$VerifyMaximizedScale,

    [switch]$VerifyRetailReferenceSize,

    [switch]$VerifyFrontendAnimation,

    [switch]$VerifyMainMenu,

    [switch]$VerifySubscreens,

    [string]$ReferenceProfileName = '',

    [switch]$BuffJesus,

    [switch]$VerifyUpscaled,

    [switch]$OriginalVideo,

    [ValidateSet('boot', 'microsoft', 'lionhead', 'attract', 'intro')]
    [string]$Movie = 'boot'
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $Executable) {
    $Executable = Join-Path $rebuildRoot `
        'build\bootstrap-Release\FableTLC-Reconstruction-VisualCheckpoint.exe'
}
$Executable = (Resolve-Path -LiteralPath $Executable).Path

if (
    $RetailVideo -or
    $VerifyMaximizedScale -or
    $VerifyRetailReferenceSize -or
    $VerifyFrontendAnimation -or
    $VerifyMainMenu -or
    $VerifySubscreens
) {
    Add-Type -AssemblyName System.Drawing
    if (-not ('VisualSmokeNativeMethods' -as [type])) {
        Add-Type @'
using System;
using System.Runtime.InteropServices;

public static class VisualSmokeNativeMethods
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Rect
    {
        public int Left;
        public int Top;
        public int Right;
        public int Bottom;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Point
    {
        public int X;
        public int Y;
    }

    [DllImport("user32.dll")]
    public static extern bool GetWindowRect(IntPtr window, out Rect rectangle);

    [DllImport("user32.dll")]
    public static extern bool GetClientRect(IntPtr window, out Rect rectangle);

    [DllImport("user32.dll")]
    public static extern bool ClientToScreen(IntPtr window, ref Point point);

    [DllImport("user32.dll")]
    public static extern IntPtr GetWindow(IntPtr window, uint command);

    [DllImport("user32.dll")]
    public static extern bool ShowWindow(IntPtr window, int command);

    [DllImport("user32.dll")]
    public static extern bool BringWindowToTop(IntPtr window);

    [DllImport("user32.dll")]
    public static extern bool SetForegroundWindow(IntPtr window);

    [DllImport("user32.dll")]
    public static extern bool PrintWindow(
        IntPtr window,
        IntPtr deviceContext,
        uint flags
    );

    [DllImport("user32.dll")]
    public static extern bool PostMessage(
        IntPtr window,
        uint message,
        UIntPtr wordParameter,
        IntPtr longParameter
    );

    [DllImport("user32.dll")]
    public static extern IntPtr SendMessage(
        IntPtr window,
        uint message,
        UIntPtr wordParameter,
        IntPtr longParameter
    );
}
'@
    }
}

$arguments = @()
if ($BuffJesus) {
    $arguments += '--buff-jesus'
    if (-not $RetailVideo) {
        # The public BuffJesus launch includes the retail boot sequence. Keep
        # direct UI smoke runs fast unless they explicitly request that gate.
        $arguments += '--skip-boot-videos'
    }
}
if ($VerifyMaximizedScale -or $VerifySubscreens) {
    # Keep the existing decoded-backdrop pixel oracle deterministic while the
    # default frontend runs its independent forest/sunbeam animation.
    $arguments += '--retail-frontend-static'
}
if ($VerifyRetailReferenceSize) {
    $arguments += '--retail-frontend-reference-size'
    if (-not $VerifyFrontendAnimation) {
        # Keep the reference-size screenshot deterministic for pixel inspection.
        $arguments += '--retail-frontend-static'
    }
}
if ($RetailVideo) {
    $arguments += if ($Movie -eq 'boot') {
        '--retail-video'
    } else {
        "--retail-video=$Movie"
    }
    if ($OriginalVideo) {
        $arguments += '--retail-video-original'
    }
    if ($VerifyUpscaled) {
        $arguments += '--retail-video-upscaled'
    }
}
$startOptions = @{
    FilePath = $Executable
    PassThru = $true
}
if ($arguments.Count -ne 0) {
    $startOptions.ArgumentList = $arguments
}
$process = Start-Process @startOptions
$title = ''
$quitViaRetailAction = $false
$deadline = [DateTime]::UtcNow.AddSeconds($TimeoutSeconds)
try {
    while ([DateTime]::UtcNow -lt $deadline) {
        Start-Sleep -Milliseconds 100
        $process.Refresh()
        if ($process.HasExited) {
            break
        }
        $title = $process.MainWindowTitle
        if (
            ($RetailVideo -and $title -like '*Retail WMV Playing*') -or
            (-not $RetailVideo -and $title -like '*D3D9 Presented via Render2D*')
        ) {
            break
        }
    }

    $expectedTitle = if ($RetailVideo) {
        '*Retail WMV Playing*'
    } else {
        '*D3D9 Presented via Render2D*'
    }
    if ($title -notlike $expectedTitle) {
        throw "Expected presentation was not observed; last title was '$title'."
    }
    if ($BuffJesus -and -not $RetailVideo -and $title -notlike '*BuffJesus*') {
        throw "The BuffJesus text variant was not activated; title was '$title'."
    }
    if ($VerifyUpscaled) {
        if (-not $RetailVideo -or $OriginalVideo) {
            throw (
                '-VerifyUpscaled requires -RetailVideo without ' +
                '-OriginalVideo.'
            )
        }
        if ($title -notlike '*AI 2x*') {
            throw "The enhanced movie was not selected; title was '$title'."
        }
    }
    if ($OriginalVideo -and $title -like '*AI 2x*') {
        throw "Original-video override selected an enhanced movie: '$title'."
    }

    if ($VerifyBootToFrontend) {
        if (-not $RetailVideo -or $Movie -ne 'boot') {
            throw '-VerifyBootToFrontend requires -RetailVideo -Movie boot.'
        }
    }
    if ($VerifyFrontendAnimation -and $RetailVideo) {
        throw (
            '-VerifyFrontendAnimation targets the directly revealed ' +
            'frontend; combine boot-to-frontend and animation as separate gates.'
        )
    }
    if (
        ($VerifyMainMenu -or $VerifySubscreens) -and
        $RetailVideo -and
        -not $VerifyBootToFrontend
    ) {
        throw (
            'Frontend interaction verification targets the directly revealed press-start ' +
            'screen unless -VerifyBootToFrontend is also selected.'
        )
    }

    if ($VerifyBootSequence) {
        if (-not $RetailVideo -or $Movie -ne 'boot') {
            throw '-VerifyBootSequence requires -RetailVideo -Movie boot.'
        }

        while ([DateTime]::UtcNow -lt $deadline) {
            Start-Sleep -Milliseconds 100
            $process.Refresh()
            if ($process.HasExited) {
                break
            }
            $title = $process.MainWindowTitle
            if ($title -like '*Retail WMV Playing 3/3 - Intro*') {
                break
            }
        }
        if ($title -notlike '*Retail WMV Playing 3/3 - Intro*') {
            throw (
                'The retail boot chain did not advance through Lionhead and ' +
                "Microsoft into the intro; last title was '$title'."
            )
        }
    }

    if ($VerifyEscapeSkip -or $VerifyBootToFrontend) {
        if (-not $RetailVideo -or $Movie -ne 'boot') {
            throw (
                '-VerifyEscapeSkip and -VerifyBootToFrontend require ' +
                '-RetailVideo -Movie boot.'
            )
        }

        function Send-EscapeKey {
            $escapeKey = [UIntPtr]::new(0x1B)
            if (
                -not [VisualSmokeNativeMethods]::PostMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    $escapeKey,
                    [IntPtr]::Zero
                ) -or
                -not [VisualSmokeNativeMethods]::PostMessage(
                    $process.MainWindowHandle,
                    0x0101,
                    $escapeKey,
                    [IntPtr]::Zero
                )
            ) {
                throw 'Could not send Escape to the retail-video window.'
            }
        }

        function Wait-ForMovieTitle([string]$ExpectedTitle) {
            while ([DateTime]::UtcNow -lt $deadline) {
                Start-Sleep -Milliseconds 50
                $process.Refresh()
                if ($process.HasExited) {
                    break
                }
                $script:title = $process.MainWindowTitle
                if ($script:title -like $ExpectedTitle) {
                    return
                }
            }
            throw (
                "Escape did not advance to '$ExpectedTitle'; " +
                "last title was '$script:title'."
            )
        }

        # -VerifyBootSequence has already observed the natural chain at 3/3.
        # Do not rewind the assertion state and wait for the now-finished
        # Microsoft movie when it is combined with -VerifyBootToFrontend.
        if (-not $VerifyBootSequence) {
            Send-EscapeKey
            Wait-ForMovieTitle '*Retail WMV Playing 2/3 - Microsoft*'
            Send-EscapeKey
            Wait-ForMovieTitle '*Retail WMV Playing 3/3 - Intro*'
        }

        if ($VerifyBootToFrontend) {
            Send-EscapeKey
            Wait-ForMovieTitle (
                '*Retail Boot Movies Complete - ' +
                'Frontend Checkpoint Ready - ' +
                'Post-Movie Startup Ordered*'
            )
            if (
                [VisualSmokeNativeMethods]::GetWindow(
                    $process.MainWindowHandle,
                    5
                ) -ne [IntPtr]::Zero
            ) {
                throw (
                    'The boot sequence reached the frontend checkpoint, ' +
                    'but its DirectShow child window is still attached.'
                )
            }
        }
    }

    $maximizedScaleProof = ''
    $retailReferenceSizeProof = ''
    if ($VerifyRetailReferenceSize) {
        $client = New-Object VisualSmokeNativeMethods+Rect
        if (-not [VisualSmokeNativeMethods]::GetClientRect(
            $process.MainWindowHandle,
            [ref]$client
        )) {
            throw 'Could not resolve the retail reference client rectangle.'
        }
        $clientWidth = $client.Right - $client.Left
        $clientHeight = $client.Bottom - $client.Top
        if ($clientWidth -ne 1024 -or $clientHeight -ne 768) {
            throw (
                'The retail reference client is not 1024x768: ' +
                "${clientWidth}x${clientHeight}."
            )
        }
        $retailReferenceSizeProof =
            " reference-size=${clientWidth}x${clientHeight}"
    }
    if ($VerifyMaximizedScale) {
        if (
            $RetailVideo -and
            -not $VerifyBootToFrontend
        ) {
            throw (
                '-VerifyMaximizedScale with retail video requires ' +
                '-VerifyBootToFrontend.'
            )
        }
        # ShowWindow reports the previous visibility state, not whether the
        # request succeeded. Explicitly activate the checkpoint as well:
        # CopyFromScreen otherwise samples whichever application obscures it.
        [void][VisualSmokeNativeMethods]::ShowWindow(
            $process.MainWindowHandle,
            3
        )
        [void][VisualSmokeNativeMethods]::BringWindowToTop(
            $process.MainWindowHandle
        )
        [void][VisualSmokeNativeMethods]::SetForegroundWindow(
            $process.MainWindowHandle
        )
        Start-Sleep -Milliseconds 900
        $process.Refresh()

        $client = New-Object VisualSmokeNativeMethods+Rect
        $clientOrigin = New-Object VisualSmokeNativeMethods+Point
        if (
            -not [VisualSmokeNativeMethods]::GetClientRect(
                $process.MainWindowHandle,
                [ref]$client
            ) -or
            -not [VisualSmokeNativeMethods]::ClientToScreen(
                $process.MainWindowHandle,
                [ref]$clientOrigin
            )
        ) {
            throw 'Could not resolve the maximized client rectangle.'
        }
        $clientWidth = $client.Right - $client.Left
        $clientHeight = $client.Bottom - $client.Top
        if ($clientWidth -le 0 -or $clientHeight -le 0) {
            throw 'The maximized client rectangle is empty.'
        }

        $maximizedBitmap =
            New-Object System.Drawing.Bitmap $clientWidth, $clientHeight
        $maximizedGraphics =
            [System.Drawing.Graphics]::FromImage($maximizedBitmap)
        try {
            [void][VisualSmokeNativeMethods]::SetForegroundWindow(
                $process.MainWindowHandle
            )
            Start-Sleep -Milliseconds 100
            $maximizedGraphics.CopyFromScreen(
                $clientOrigin.X,
                $clientOrigin.Y,
                0,
                0,
                $maximizedBitmap.Size
            )
            $maximizedScreenshot = Join-Path (
                Split-Path -Parent $Executable
            ) 'maximized-scale-smoke.png'
            $maximizedBitmap.Save(
                $maximizedScreenshot,
                [System.Drawing.Imaging.ImageFormat]::Png
            )

            $retailBackdrop = Join-Path (
                Split-Path -Parent $Executable
            ) 'frontend_backdrop_01.png'
            if (-not (Test-Path -LiteralPath $retailBackdrop)) {
                throw (
                    'The maximized-scale gate requires the decoded retail ' +
                    "backdrop: $retailBackdrop"
                )
            }
            $expectedBitmap =
                [System.Drawing.Bitmap]::FromFile($retailBackdrop)
            try {
                $drawWidth = $clientWidth
                $drawHeight = $clientHeight
                $drawLeft = 0
                $drawTop = 0
                $totalError = 0
                $channelCount = 0
                foreach ($sampleY in @(190, 250, 320, 390, 450)) {
                    foreach ($sampleX in @(60, 160, 280, 400, 520, 600)) {
                        $actualX = [Math]::Min(
                            $clientWidth - 1,
                            $drawLeft + [int](
                                ($sampleX + 0.5) *
                                $drawWidth /
                                $expectedBitmap.Width
                            )
                        )
                        $actualY = [Math]::Min(
                            $clientHeight - 1,
                            $drawTop + [int](
                                ($sampleY + 0.5) *
                                $drawHeight /
                                $expectedBitmap.Height
                            )
                        )
                        $actual =
                            $maximizedBitmap.GetPixel($actualX, $actualY)
                        $expected =
                            $expectedBitmap.GetPixel($sampleX, $sampleY)
                        $totalError +=
                            [Math]::Abs($actual.R - $expected.R) +
                            [Math]::Abs($actual.G - $expected.G) +
                            [Math]::Abs($actual.B - $expected.B)
                        $channelCount += 3
                    }
                }
                $meanChannelError =
                    [Math]::Round($totalError / $channelCount, 2)
                if ($meanChannelError -gt 32) {
                    throw (
                        'The maximized frontend does not match the expected ' +
                        "retail full-view scale (mean channel error " +
                        "$meanChannelError)."
                    )
                }
                $maximizedScaleProof = (
                    " maximized=${clientWidth}x${clientHeight}" +
                    " scale-error=$meanChannelError"
                )
            } finally {
                $expectedBitmap.Dispose()
            }
        } finally {
            $maximizedGraphics.Dispose()
            $maximizedBitmap.Dispose()
        }
    }

    $frameProof = ''
    $script:redefineScrollHash = ''
    $script:redefineScrollEndHash = ''
    $script:redefineOffPageRemap = $false
    if (
        $RetailVideo -or
        $VerifyFrontendAnimation -or
        $VerifyMainMenu -or
        $VerifySubscreens
    ) {
        $process.Refresh()
        $bounds = New-Object VisualSmokeNativeMethods+Rect
        if (
            -not [VisualSmokeNativeMethods]::GetWindowRect(
                $process.MainWindowHandle,
                [ref]$bounds
            )
        ) {
            throw 'Could not resolve the retail-video window bounds.'
        }
        $width = $bounds.Right - $bounds.Left
        $height = $bounds.Bottom - $bounds.Top
        if ($width -le 0 -or $height -le 0) {
            throw 'The retail-video window has invalid bounds.'
        }

        function Get-WindowFrameHash {
            [void][VisualSmokeNativeMethods]::BringWindowToTop(
                $process.MainWindowHandle)
            [void][VisualSmokeNativeMethods]::SetForegroundWindow(
                $process.MainWindowHandle)
            Start-Sleep -Milliseconds 40
            $bitmap = New-Object System.Drawing.Bitmap $width, $height
            $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
            $stream = New-Object System.IO.MemoryStream
            try {
                $graphics.CopyFromScreen(
                    $bounds.Left,
                    $bounds.Top,
                    0,
                    0,
                    $bitmap.Size
                )
                $bitmap.Save(
                    $stream,
                    [System.Drawing.Imaging.ImageFormat]::Bmp
                )
                $sha = [System.Security.Cryptography.SHA256]::Create()
                try {
                    return (
                        [BitConverter]::ToString(
                            $sha.ComputeHash($stream.ToArray())
                        ) -replace '-', ''
                    )
                } finally {
                    $sha.Dispose()
                }
            } finally {
                $stream.Dispose()
                $graphics.Dispose()
                $bitmap.Dispose()
            }
        }

        function Get-WindowSampleColorCount {
            $bitmap = New-Object System.Drawing.Bitmap $width, $height
            $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
            try {
                $graphics.CopyFromScreen(
                    $bounds.Left,
                    $bounds.Top,
                    0,
                    0,
                    $bitmap.Size
                )
                $colours = [System.Collections.Generic.HashSet[int]]::new()
                for ($sampleY = 1; $sampleY -lt 12; ++$sampleY) {
                    for ($sampleX = 1; $sampleX -lt 16; ++$sampleX) {
                        $x = [Math]::Min(
                            $width - 1,
                            [int](($sampleX * $width) / 16)
                        )
                        $y = [Math]::Min(
                            $height - 1,
                            [int](($sampleY * $height) / 12)
                        )
                        $null = $colours.Add($bitmap.GetPixel($x, $y).ToArgb())
                    }
                }
                return $colours.Count
            } finally {
                $graphics.Dispose()
                $bitmap.Dispose()
            }
        }

        if ($VerifyBootToFrontend -or $VerifySubscreens) {
            Start-Sleep -Milliseconds 600
        }

        function Get-ClientDesignPixelArgb {
            param(
                [int]$DesignX,
                [int]$DesignY
            )
            [void][VisualSmokeNativeMethods]::BringWindowToTop(
                $process.MainWindowHandle)
            [void][VisualSmokeNativeMethods]::SetForegroundWindow(
                $process.MainWindowHandle)
            Start-Sleep -Milliseconds 40
            $client = New-Object VisualSmokeNativeMethods+Rect
            $origin = New-Object VisualSmokeNativeMethods+Point
            if (
                -not [VisualSmokeNativeMethods]::GetClientRect(
                    $process.MainWindowHandle,
                    [ref]$client
                ) -or
                -not [VisualSmokeNativeMethods]::ClientToScreen(
                    $process.MainWindowHandle,
                    [ref]$origin
                )
            ) {
                throw 'Could not resolve the frontend client sample point.'
            }
            $clientWidth = $client.Right - $client.Left
            $clientHeight = $client.Bottom - $client.Top
            $sampleX = $origin.X + [int](
                $DesignX * $clientWidth / 640)
            $sampleY = $origin.Y + [int](
                $DesignY * $clientHeight / 480)
            $sample = New-Object System.Drawing.Bitmap 1, 1
            $graphics = [System.Drawing.Graphics]::FromImage($sample)
            try {
                $graphics.CopyFromScreen(
                    $sampleX,
                    $sampleY,
                    0,
                    0,
                    $sample.Size
                )
                return $sample.GetPixel(0, 0).ToArgb()
            } finally {
                $graphics.Dispose()
                $sample.Dispose()
            }
        }
        $firstFrameHash = Get-WindowFrameHash
        if ($VerifyBootToFrontend -or $VerifySubscreens) {
            $sampleColorCount = Get-WindowSampleColorCount
            if ($sampleColorCount -lt 12) {
                throw (
                    'The revealed frontend checkpoint was visually flat ' +
                    "($sampleColorCount colours)."
                )
            }
            Start-Sleep -Milliseconds 300
            $secondFrameHash = Get-WindowFrameHash
            $frameState = if ($firstFrameHash -eq $secondFrameHash) {
                'stable'
            } else {
                'presenting'
            }
            $frameProof = (
                " frontend=$frameState" +
                " colours=$sampleColorCount" +
                " hash=$($firstFrameHash.Substring(0, 12))" +
                $(if ($VerifyBootToFrontend) {
                    ' directshow-child=closed'
                } else {
                    ' static-interaction-baseline=verified'
                })
            )
        } else {
            $secondFrameHash = $firstFrameHash
            while (
                $secondFrameHash -eq $firstFrameHash -and
                [DateTime]::UtcNow -lt $deadline
            ) {
                Start-Sleep -Milliseconds 300
                $secondFrameHash = Get-WindowFrameHash
            }
            if ($firstFrameHash -eq $secondFrameHash) {
                $changingSurface = if ($VerifyFrontendAnimation) {
                    'frontend forest/sunbeam animation'
                } else {
                    'retail WMV'
                }
                throw (
                    "The $changingSurface clock ran, but no changing frame was " +
                    'observed before the smoke deadline.'
                )
            }
            $frameProof = (
                " frames=changed first=$($firstFrameHash.Substring(0, 12))" +
                " second=$($secondFrameHash.Substring(0, 12))"
            )
        }

        function Get-ClientFrameHash {
            [void][VisualSmokeNativeMethods]::BringWindowToTop(
                $process.MainWindowHandle)
            [void][VisualSmokeNativeMethods]::SetForegroundWindow(
                $process.MainWindowHandle)
            Start-Sleep -Milliseconds 40
            $client = New-Object VisualSmokeNativeMethods+Rect
            $origin = New-Object VisualSmokeNativeMethods+Point
            if (
                -not [VisualSmokeNativeMethods]::GetClientRect(
                    $process.MainWindowHandle,
                    [ref]$client
                ) -or
                -not [VisualSmokeNativeMethods]::ClientToScreen(
                    $process.MainWindowHandle,
                    [ref]$origin
                )
            ) {
                throw 'Could not resolve the frontend client capture area.'
            }
            $clientWidth = $client.Right - $client.Left
            $clientHeight = $client.Bottom - $client.Top
            $windowBitmap =
                New-Object System.Drawing.Bitmap $width, $height
            $windowGraphics =
                [System.Drawing.Graphics]::FromImage($windowBitmap)
            $deviceContext = $windowGraphics.GetHdc()
            try {
                if (-not [VisualSmokeNativeMethods]::PrintWindow(
                    $process.MainWindowHandle,
                    $deviceContext,
                    2
                )) {
                    throw 'PrintWindow could not capture the frontend.'
                }
            } finally {
                $windowGraphics.ReleaseHdc($deviceContext)
                $windowGraphics.Dispose()
            }
            $sourceRectangle = New-Object System.Drawing.Rectangle `
                ($origin.X - $bounds.Left), `
                ($origin.Y - $bounds.Top), `
                $clientWidth, `
                $clientHeight
            $bitmap = $windowBitmap.Clone(
                $sourceRectangle,
                $windowBitmap.PixelFormat)
            $stream = New-Object System.IO.MemoryStream
            try {
                $bitmap.Save(
                    $stream,
                    [System.Drawing.Imaging.ImageFormat]::Bmp
                )
                $sha = [System.Security.Cryptography.SHA256]::Create()
                try {
                    return (
                        [BitConverter]::ToString(
                            $sha.ComputeHash($stream.ToArray())
                        ) -replace '-', ''
                    )
                } finally {
                    $sha.Dispose()
                }
            } finally {
                $stream.Dispose()
                $bitmap.Dispose()
                $windowBitmap.Dispose()
            }
        }

        function Get-ClientFrameBitmap {
            [void][VisualSmokeNativeMethods]::BringWindowToTop(
                $process.MainWindowHandle)
            [void][VisualSmokeNativeMethods]::SetForegroundWindow(
                $process.MainWindowHandle)
            Start-Sleep -Milliseconds 40
            $client = New-Object VisualSmokeNativeMethods+Rect
            $origin = New-Object VisualSmokeNativeMethods+Point
            if (
                -not [VisualSmokeNativeMethods]::GetClientRect(
                    $process.MainWindowHandle,
                    [ref]$client
                ) -or
                -not [VisualSmokeNativeMethods]::ClientToScreen(
                    $process.MainWindowHandle,
                    [ref]$origin
                )
            ) {
                throw 'Could not resolve the frontend client capture area.'
            }
            $clientWidth = $client.Right - $client.Left
            $clientHeight = $client.Bottom - $client.Top
            $windowBitmap =
                New-Object System.Drawing.Bitmap $width, $height
            $windowGraphics =
                [System.Drawing.Graphics]::FromImage($windowBitmap)
            $deviceContext = $windowGraphics.GetHdc()
            try {
                if (-not [VisualSmokeNativeMethods]::PrintWindow(
                    $process.MainWindowHandle,
                    $deviceContext,
                    2
                )) {
                    throw 'PrintWindow could not capture the frontend.'
                }
            } finally {
                $windowGraphics.ReleaseHdc($deviceContext)
                $windowGraphics.Dispose()
            }
            $sourceRectangle = New-Object System.Drawing.Rectangle `
                ($origin.X - $bounds.Left), `
                ($origin.Y - $bounds.Top), `
                $clientWidth, `
                $clientHeight
            try {
                return $windowBitmap.Clone(
                    $sourceRectangle,
                    $windowBitmap.PixelFormat)
            } finally {
                $windowBitmap.Dispose()
            }
        }

        function Get-ClientFrameHash {
            $bitmap = Get-ClientFrameBitmap
            $stream = New-Object System.IO.MemoryStream
            try {
                $bitmap.Save(
                    $stream,
                    [System.Drawing.Imaging.ImageFormat]::Bmp
                )
                $sha = [System.Security.Cryptography.SHA256]::Create()
                try {
                    return (
                        [BitConverter]::ToString(
                            $sha.ComputeHash($stream.ToArray())
                        ) -replace '-', ''
                    )
                } finally {
                    $sha.Dispose()
                }
            } finally {
                $stream.Dispose()
                $bitmap.Dispose()
            }
        }

        function Get-ClientDesignRegionMeanDifference {
            param(
                [System.Drawing.Bitmap]$Left,
                [System.Drawing.Bitmap]$Right,
                [int]$DesignX,
                [int]$DesignY,
                [int]$DesignWidth,
                [int]$DesignHeight
            )
            $leftX = [int]($DesignX * $Left.Width / 640)
            $leftY = [int]($DesignY * $Left.Height / 480)
            $rightX = [int](($DesignX + $DesignWidth) * $Left.Width / 640)
            $rightY = [int](($DesignY + $DesignHeight) * $Left.Height / 480)
            $sampleStep = 4
            $total = 0
            $count = 0
            for ($y = $leftY; $y -lt $rightY; $y += $sampleStep) {
                for ($x = $leftX; $x -lt $rightX; $x += $sampleStep) {
                    $leftPixel = $Left.GetPixel($x, $y)
                    $rightPixel = $Right.GetPixel($x, $y)
                    $total +=
                        [Math]::Abs($leftPixel.R - $rightPixel.R) +
                        [Math]::Abs($leftPixel.G - $rightPixel.G) +
                        [Math]::Abs($leftPixel.B - $rightPixel.B)
                    $count += 3
                }
            }
            if ($count -eq 0) {
                return 0.0
            }
            return [double]$total / $count
        }

        if ($VerifyMainMenu -or $VerifySubscreens) {
            $pressStartHash = Get-WindowFrameHash
            [void][VisualSmokeNativeMethods]::SendMessage(
                $process.MainWindowHandle,
                0x0100,
                [UIntPtr]::new(0x0D),
                [IntPtr]::Zero
            )
            Start-Sleep -Milliseconds 500
            $mainMenuHash = Get-WindowFrameHash
            if ($mainMenuHash -eq $pressStartHash) {
                throw (
                    'Keyboard Enter did not dispatch retail press-start ' +
                    'action 229 and replace the ' +
                    'press-start frontend with the main-menu frontend.'
                )
            }
            [void][VisualSmokeNativeMethods]::SendMessage(
                $process.MainWindowHandle,
                0x0100,
                [UIntPtr]::new(0x26),
                [IntPtr]::Zero
            )
            Start-Sleep -Milliseconds 100
            $keyboardUpHash = Get-WindowFrameHash
            if ($keyboardUpHash -eq $mainMenuHash) {
                throw (
                    'Recovered CFrontEndList ScrollUp did not wrap ' +
                    'Continue Game to Quit.'
                )
            }
            [void][VisualSmokeNativeMethods]::SendMessage(
                $process.MainWindowHandle,
                0x0100,
                [UIntPtr]::new(0x28),
                [IntPtr]::Zero
            )
            Start-Sleep -Milliseconds 100
            $keyboardDownHash = Get-WindowFrameHash
            if ($keyboardDownHash -eq $keyboardUpHash) {
                throw (
                    'Recovered CFrontEndList ScrollDown did not return ' +
                    'Quit to Continue Game.'
                )
            }
            # Sample the translucent middle of the Quit ornament, away from
            # its text and decorative end caps. This catches a valid generated
            # selection plan whose TS_BUTTON_M texture is nevertheless dropped
            # by the live Render2D texture-binding bridge.
            $quitHighlightBeforeArgb =
                Get-ClientDesignPixelArgb 260 450
            $menuClient = New-Object VisualSmokeNativeMethods+Rect
            if (
                -not [VisualSmokeNativeMethods]::GetClientRect(
                    $process.MainWindowHandle,
                    [ref]$menuClient
                )
            ) {
                throw 'Could not resolve the main-menu client rectangle.'
            }
            $hoverX = [int](320 * $menuClient.Right / 640)
            $hoverY = [int](440 * $menuClient.Bottom / 480)
            $hoverPosition = [IntPtr]::new(
                (($hoverY -band 0xFFFF) -shl 16) -bor
                ($hoverX -band 0xFFFF)
            )
            if (
                -not [VisualSmokeNativeMethods]::PostMessage(
                    $process.MainWindowHandle,
                    0x0200,
                    [UIntPtr]::Zero,
                    $hoverPosition
                )
            ) {
                throw 'Could not hover the retail Quit row.'
            }
            Start-Sleep -Milliseconds 150
            $hoverMenuHash = Get-WindowFrameHash
            if ($hoverMenuHash -eq $mainMenuHash) {
                throw (
                    'The compiled main-menu mouse geometry did not move ' +
                    'selection from Continue Game to Quit.'
                )
            }
            $quitHighlightAfterArgb =
                Get-ClientDesignPixelArgb 260 450
            $quitHighlightBefore =
                [System.Drawing.Color]::FromArgb(
                    $quitHighlightBeforeArgb)
            $quitHighlightAfter =
                [System.Drawing.Color]::FromArgb(
                    $quitHighlightAfterArgb)
            $quitHighlightDelta =
                [Math]::Abs(
                    [int]$quitHighlightAfter.R -
                    [int]$quitHighlightBefore.R) +
                [Math]::Abs(
                    [int]$quitHighlightAfter.G -
                    [int]$quitHighlightBefore.G) +
                [Math]::Abs(
                    [int]$quitHighlightAfter.B -
                    [int]$quitHighlightBefore.B)
            if ($quitHighlightDelta -lt 24) {
                throw (
                    'The Quit row selection moved, but its retail ' +
                    'TS_BUTTON_M highlight texture was not visibly bound ' +
                    "(sample delta $quitHighlightDelta)."
                )
            }
            $menuBitmap = New-Object System.Drawing.Bitmap $width, $height
            $menuGraphics =
                [System.Drawing.Graphics]::FromImage($menuBitmap)
            try {
                $menuGraphics.CopyFromScreen(
                    $bounds.Left,
                    $bounds.Top,
                    0,
                    0,
                    $menuBitmap.Size
                )
                $menuScreenshot = Join-Path (
                    Split-Path -Parent $Executable
                ) $(if ($BuffJesus) {
                    'buff-jesus-main-menu-smoke.png'
                } else {
                    'main-menu-transition-smoke.png'
                })
                $menuBitmap.Save(
                    $menuScreenshot,
                    [System.Drawing.Imaging.ImageFormat]::Png
                )
            } finally {
                $menuGraphics.Dispose()
                $menuBitmap.Dispose()
            }
            $frameProof += (
                " menu=transitioned" +
                " press=$($pressStartHash.Substring(0, 12))" +
                " main=$($mainMenuHash.Substring(0, 12))" +
                " hover=$($hoverMenuHash.Substring(0, 12))" +
                " highlight-delta=$quitHighlightDelta" +
                " keys=enter-up-wrap-down"
            )

            if ($VerifySubscreens) {
                function Send-DesignMouse {
                    param(
                        [int]$Message,
                        [int]$DesignX,
                        [int]$DesignY
                    )
                    $client = New-Object VisualSmokeNativeMethods+Rect
                    if (
                        -not [VisualSmokeNativeMethods]::GetClientRect(
                            $process.MainWindowHandle,
                            [ref]$client
                        )
                    ) {
                        throw 'Could not resolve the frontend client rectangle.'
                    }
                    $x = [int]($DesignX * $client.Right / 640)
                    $y = [int]($DesignY * $client.Bottom / 480)
                    $position = [IntPtr]::new(
                        (($y -band 0xFFFF) -shl 16) -bor
                        ($x -band 0xFFFF)
                    )
                    # SendMessage makes the state transition and its immediate
                    # render complete before the smoke captures the result.
                    # PostMessage made this gate scheduler-dependent under a
                    # concurrent VC7.1/retail-oracle refresh.
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        [uint32]$Message,
                        [UIntPtr]::Zero,
                        $position
                    )
                }

                function Send-DesignMouseWheel {
                    param(
                        [int]$Delta,
                        [int]$DesignX = 200,
                        [int]$DesignY = 200
                    )
                    $client = New-Object VisualSmokeNativeMethods+Rect
                    if (
                        -not [VisualSmokeNativeMethods]::GetClientRect(
                            $process.MainWindowHandle,
                            [ref]$client
                        )
                    ) {
                        throw 'Could not resolve the frontend client rectangle.'
                    }
                    $point = New-Object VisualSmokeNativeMethods+Point
                    $point.X = [int]($DesignX * $client.Right / 640)
                    $point.Y = [int]($DesignY * $client.Bottom / 480)
                    if (
                        -not [VisualSmokeNativeMethods]::ClientToScreen(
                            $process.MainWindowHandle,
                            [ref]$point
                        )
                    ) {
                        throw 'Could not translate the wheel point to screen coordinates.'
                    }
                    $wheelWord = [uint64]($Delta -band 0xFFFF) * 65536
                    $screenPoint = [IntPtr]::new(
                        (($point.Y -band 0xFFFF) -shl 16) -bor
                        ($point.X -band 0xFFFF)
                    )
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        0x020A,
                        [UIntPtr]::new($wheelWord),
                        $screenPoint
                    )
                }

                if ($ReferenceProfileName) {
                    $profileRoot = Join-Path $env:USERPROFILE `
                        'Documents\My Games\Fable\Saves'
                    if (-not (Test-Path -LiteralPath $profileRoot)) {
                        throw (
                            "Reference profile root does not exist: " +
                            $profileRoot
                        )
                    }
                    $profileNames = @(
                        Get-ChildItem -LiteralPath $profileRoot -Directory |
                            Where-Object { $_.Name -notmatch '^\.\.?$' } |
                            Select-Object -ExpandProperty Name |
                            Sort-Object { $_.ToLowerInvariant() }
                    )
                    $profileIndex = -1
                    for ($i = 0; $i -lt $profileNames.Count; ++$i) {
                        if (
                            [string]::Equals(
                                $profileNames[$i],
                                $ReferenceProfileName,
                                [System.StringComparison]::OrdinalIgnoreCase
                            )
                        ) {
                            $profileIndex = $i + 1
                            break
                        }
                    }
                    if ($profileIndex -lt 1) {
                        throw (
                            "Reference profile was not found: " +
                            $ReferenceProfileName
                        )
                    }
                    # Main-menu action 16 -> profile list.  Normal profile
                    # selection reserves item 0 for New Profile, hence the
                    # existing-directory ordinal is offset by one.
                    Send-DesignMouse 0x0200 320 230
                    Start-Sleep -Milliseconds 100
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        0x0100,
                        [UIntPtr]::new(0x0D),
                        [IntPtr]::Zero
                    )
                    Start-Sleep -Milliseconds 250
                    for ($i = 0; $i -lt $profileIndex; ++$i) {
                        Send-DesignMouseWheel -Delta -120
                    }
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        0x0100,
                        [UIntPtr]::new(0x0D),
                        [IntPtr]::Zero
                    )
                    Start-Sleep -Milliseconds 300
                    $frameProof +=
                        " profile-loaded=$ReferenceProfileName"
                }

                # Continue Game dispatches action 66, refreshes the recovered
                # autosave/manual ordering, and enters used key 0x08.
                Send-DesignMouse 0x0200 320 205
                # Let the profile-list hover settle before taking the
                # pre-transition sample; otherwise the first D3D9 present can
                # race the CopyFromScreen probe on slower runs.
                Start-Sleep -Milliseconds 300
                # The main-menu Continue hover occupies the old (100,100)
                # probe location and can match the first save-row highlight.
                # Sample the title band instead; it is stable and distinct
                # across the two screens.
                $saveSelectionBeforeArgb =
                    Get-ClientDesignPixelArgb 100 60
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x0D),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 250
                $saveMenuHash = Get-WindowFrameHash
                $saveSelectionAfterArgb =
                    Get-ClientDesignPixelArgb 100 60
                $saveSelectionBefore =
                    [System.Drawing.Color]::FromArgb(
                        $saveSelectionBeforeArgb)
                $saveSelectionAfter =
                    [System.Drawing.Color]::FromArgb(
                        $saveSelectionAfterArgb)
                $saveSelectionDelta =
                    [Math]::Abs(
                        [int]$saveSelectionAfter.R -
                        [int]$saveSelectionBefore.R) +
                    [Math]::Abs(
                        [int]$saveSelectionAfter.G -
                        [int]$saveSelectionBefore.G) +
                    [Math]::Abs(
                        [int]$saveSelectionAfter.B -
                        [int]$saveSelectionBefore.B)
                $saveDiagnosticBitmap = Get-ClientFrameBitmap
                try {
                    $saveDiagnosticBitmap.Save(
                        (Join-Path (
                            Split-Path -Parent $Executable
                        ) 'frontend-saved-games-initial-smoke.png'),
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $saveDiagnosticBitmap.Dispose()
                }
                if (
                    $saveMenuHash -eq $hoverMenuHash -or
                    $saveMenuHash -eq $mainMenuHash
                ) {
                    throw (
                        'Retail action 66 did not activate used key 0x08, ' +
                        'UI_FRONTEND_PROFILE_SAVED_GAMES_MENU ' +
                        "(selection sample delta $saveSelectionDelta)."
                    )
                }
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x28),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 100
                $saveMenuDownHash = Get-WindowFrameHash
                if ($saveMenuDownHash -eq $saveMenuHash) {
                    throw (
                        'Recovered saved-games list ScrollDown did not ' +
                        'advance from AutoSave to Manual - Save1.'
                    )
                }
                $saveBitmap = Get-ClientFrameBitmap
                try {
                    $saveScreenshot = Join-Path (
                        Split-Path -Parent $Executable
                    ) 'frontend-saved-games-smoke.png'
                    $saveBitmap.Save(
                        $saveScreenshot,
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $saveBitmap.Dispose()
                }
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x1B),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 150

                # Move from Quit to Options and dispatch retail action 297
                # through the selected child rather than a mouse release.
                Send-DesignMouse 0x0200 320 260
                Start-Sleep -Milliseconds 100
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x0D),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 350
                $optionsHash = Get-WindowFrameHash
                if (
                    $optionsHash -eq $hoverMenuHash -or
                    $optionsHash -eq $mainMenuHash
                ) {
                    throw (
                        'Retail action 297 did not activate ' +
                        'UI_FRONTEND_OPTIONS_SUB_MENU.'
                    )
                }
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x28),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 100
                $optionsKeyboardDownHash = Get-WindowFrameHash
                if ($optionsKeyboardDownHash -eq $optionsHash) {
                    throw (
                        'Recovered Options ScrollDown did not change ' +
                        'the selected child.'
                    )
                }
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x26),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 100
                $optionsKeyboardUpHash = Get-WindowFrameHash
                if ($optionsKeyboardUpHash -eq $optionsKeyboardDownHash) {
                    throw (
                        'Recovered Options ScrollUp did not restore ' +
                        'the selected child.'
                    )
                }
                Send-DesignMouse 0x0200 320 205
                Start-Sleep -Milliseconds 150
                $optionsHoverHash = Get-WindowFrameHash
                if ($optionsHoverHash -eq $optionsHash) {
                    throw (
                        'The compiled Options list geometry did not move ' +
                        'selection to Audio Options.'
                    )
                }

                function Send-FrontendEscape {
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        0x0100,
                        [UIntPtr]::new(0x1B),
                        [IntPtr]::Zero
                    )
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        0x0101,
                        [UIntPtr]::new(0x1B),
                        [IntPtr]::Zero
                    )
                }

                function Test-FrontendDetailScreen {
                    param(
                        [string]$Name,
                        [int]$RowY,
                        [string]$PreviousHash,
                        [string]$ScreenshotName = '',
                        [bool]$UseCancel = $false,
                        [int]$FirstArrowY = 95
                    )
                    Send-DesignMouse 0x0200 320 $RowY
                    Start-Sleep -Milliseconds 80
                    [void][VisualSmokeNativeMethods]::SendMessage(
                        $process.MainWindowHandle,
                        0x0100,
                        [UIntPtr]::new(0x0D),
                        [IntPtr]::Zero
                    )
                    Start-Sleep -Milliseconds 300
                    $detailHash = Get-WindowFrameHash
                    if ($detailHash -eq $PreviousHash) {
                        throw (
                            "Recovered Options action did not activate " +
                            "the $Name detail screen."
                        )
                    }
                    if ($Name -eq 'Redefine Keys') {
                        # Preserve the untouched entry state for visual parity
                        # work.  The normal screenshot below is intentionally
                        # taken after the hover/capture/reset interaction
                        # proof, so it cannot be compared directly with the
                        # retail RedefineKeys1 capture.
                        $initialBitmap = Get-ClientFrameBitmap
                        try {
                            $initialBitmap.Save(
                                (Join-Path (
                                    Split-Path -Parent $Executable
                                ) 'frontend-redefine-keys-initial-smoke.png'),
                                [System.Drawing.Imaging.ImageFormat]::Png
                            )
                        } finally {
                            $initialBitmap.Dispose()
                        }
                        # One Down event selects the next logical ActionOrder
                        # child and exercises the native 44-row scroll-page
                        # atlas. Return Up before the capture proof so the
                        # existing first-page interaction remains unchanged.
                        [void][VisualSmokeNativeMethods]::SendMessage(
                            $process.MainWindowHandle,
                            0x0100,
                            [UIntPtr]::new(0x28),
                            [IntPtr]::Zero
                        )
                        Start-Sleep -Milliseconds 150
                        $redefineScrollHash = Get-WindowFrameHash
                        $script:redefineScrollHash = $redefineScrollHash
                        if ($redefineScrollHash -eq $detailHash) {
                            throw (
                                'Redefine Down did not present the recovered ' +
                                'native scrolled-page atlas.'
                            )
                        }
                        $scrollBitmap = Get-ClientFrameBitmap
                        try {
                            $scrollBitmap.Save(
                                (Join-Path (
                                    Split-Path -Parent $Executable
                                ) 'frontend-redefine-keys-scroll-smoke.png'),
                                [System.Drawing.Imaging.ImageFormat]::Png
                            )
                        } finally {
                            $scrollBitmap.Dispose()
                        }
                        Send-DesignMouse 0x0200 120 127
                        # Allow the live row hover state and its present to
                        # settle before hashing the scrolled page.
                        Start-Sleep -Milliseconds 300
                        $redefineScrollHoverHash = Get-WindowFrameHash
                        if ($redefineScrollHoverHash -eq $redefineScrollHash) {
                            throw (
                                'Redefine scrolled row hover did not enter ' +
                                'the live hover strip state.'
                            )
                        }
                        Send-DesignMouseWheel -Delta 120
                        Start-Sleep -Milliseconds 150
                        $redefineScrollReturnHash = Get-WindowFrameHash
                        if ($redefineScrollReturnHash -eq $redefineScrollHash) {
                            throw (
                                'Redefine Up did not restore the initial ' +
                                'native Redefine viewport.'
                            )
                        }
                        # Capture the intermediate logical selections that
                        # correspond to the supplied retail RedefineKeys2-4
                        # references.  ActionOrder children 6, 15, and 22
                        # materialize at expanded offsets 9, 18, and 27.
                        $referencePages = @(
                            @{ Steps = 6; Name = 'frontend-redefine-keys-reference-2-smoke.png' },
                            @{ Steps = 15; Name = 'frontend-redefine-keys-reference-3-smoke.png' },
                            @{ Steps = 22; Name = 'frontend-redefine-keys-reference-4-smoke.png' }
                        )
                        foreach ($referencePage in $referencePages) {
                            for ($referenceStep = 0;
                                 $referenceStep -lt [int]$referencePage.Steps;
                                 $referenceStep++) {
                                [void][VisualSmokeNativeMethods]::SendMessage(
                                    $process.MainWindowHandle,
                                    0x0100,
                                    [UIntPtr]::new(0x28),
                                 [IntPtr]::Zero
                                )
                                Start-Sleep -Milliseconds 25
                            }
                            Start-Sleep -Milliseconds 150
                            $referenceBitmap = Get-ClientFrameBitmap
                            try {
                                $referenceBitmap.Save(
                                    (Join-Path (
                                        Split-Path -Parent $Executable
                                    ) $referencePage.Name),
                                    [System.Drawing.Imaging.ImageFormat]::Png
                                )
                            } finally {
                                $referenceBitmap.Dispose()
                            }
                            # A changed hash alone is not sufficient here:
                            # the page-atlas failure used to leave the forest
                            # backdrop on screen and still changed the frame.
                            # The authored title-rule pixel proves the live
                            # Redefine UI actually survived the present.
                            $referenceHeaderPixel =
                                [System.Drawing.Color]::FromArgb(
                                    (Get-ClientDesignPixelArgb 100 60)
                                )
                            if (
                                $referenceHeaderPixel.R -lt 120 -or
                                $referenceHeaderPixel.G -lt 100 -or
                                $referenceHeaderPixel.B -lt 90
                            ) {
                                throw (
                                    'Redefine page-atlas capture lost the ' +
                                    'live UI and retained only the backdrop.'
                                )
                            }
                            for ($referenceStep = 0;
                                 $referenceStep -lt [int]$referencePage.Steps;
                                 $referenceStep++) {
                                [void][VisualSmokeNativeMethods]::SendMessage(
                                    $process.MainWindowHandle,
                                    0x0100,
                                    [UIntPtr]::new(0x26),
                                    [IntPtr]::Zero
                                )
                                Start-Sleep -Milliseconds 25
                            }
                        }
                        # Walk to the non-wrapping list end as well. This
                        # exercises every logical-to-expanded page offset and
                        # verifies that the final viewport clamps at offset
                        # 35, where the Down arrow is suppressed.
                        Send-DesignMouse 0x0200 620 200
                        Start-Sleep -Milliseconds 80
                        for ($scrollStep = 0; $scrollStep -lt 30; $scrollStep++) {
                            [void][VisualSmokeNativeMethods]::SendMessage(
                                $process.MainWindowHandle,
                                0x0100,
                                [UIntPtr]::new(0x28),
                                [IntPtr]::Zero
                            )
                            Start-Sleep -Milliseconds 25
                        }
                        $redefineScrollEndHash = Get-WindowFrameHash
                        $redefineScrollEndClientHash = Get-ClientFrameHash
                        $script:redefineScrollEndHash = $redefineScrollEndHash
                        if ($redefineScrollEndHash -eq $detailHash) {
                            throw (
                                'Redefine Down navigation did not reach the ' +
                                'recovered final scroll page.'
                            )
                        }
                        $scrollEndBitmap = Get-ClientFrameBitmap
                        try {
                            $scrollEndBitmap.Save(
                                (Join-Path (
                                    Split-Path -Parent $Executable
                                ) 'frontend-redefine-keys-scroll-end-smoke.png'),
                                [System.Drawing.Imaging.ImageFormat]::Png
                            )
                        } finally {
                            $scrollEndBitmap.Dispose()
                        }
                        Send-DesignMouse 0x0200 120 142
                        Start-Sleep -Milliseconds 120
                        $redefineScrollEndBlankClientHash = Get-ClientFrameHash
                        if ($redefineScrollEndBlankClientHash -ne $redefineScrollEndClientHash) {
                            throw (
                                'Redefine final-page blank rows accepted a ' +
                                'hover state outside the materialized list.'
                            )
                        }
                        # A final-page row is an expanded row, not one of the
                        # nine first-page capture slots.  Click it and assign
                        # Z to prove the native capture/remap bridge writes
                        # the expanded state and overlays the changed tile
                        # without replacing unresolved static labels.
                        Send-DesignMouse 0x0202 120 116
                        Start-Sleep -Milliseconds 100
                        $redefineOffPageCaptureHash = Get-WindowFrameHash
                        [void][VisualSmokeNativeMethods]::SendMessage(
                            $process.MainWindowHandle,
                            0x0100,
                            [UIntPtr]::new(0x5A),
                            [IntPtr]::Zero
                        )
                        Start-Sleep -Milliseconds 100
                        $redefineOffPageChangedHash = Get-WindowFrameHash
                        if (
                            $redefineOffPageChangedHash -eq
                            $redefineOffPageCaptureHash)
                        {
                            throw (
                                'Off-page Redefine capture did not apply ' +
                                'the Z keyboard control.'
                            )
                        }
                        $script:redefineOffPageRemap = $true
                        for ($scrollStep = 0; $scrollStep -lt 30; $scrollStep++) {
                            [void][VisualSmokeNativeMethods]::SendMessage(
                                $process.MainWindowHandle,
                                0x0100,
                                [UIntPtr]::new(0x26),
                                [IntPtr]::Zero
                            )
                            Start-Sleep -Milliseconds 25
                        }
                        $redefineScrollEndReturnHash = Get-WindowFrameHash
                        if ($redefineScrollEndReturnHash -eq $redefineScrollEndHash) {
                            throw (
                                'Redefine Up navigation did not leave the ' +
                                'recovered final scroll page.'
                            )
                        }
                    }
                    if ($Name -eq 'Redefine Keys') {
                        # CKeyRedefiner::OnHovered enters retail visual state
                        # 3.  Moving onto the first compiled row must replace
                        # its two OFF slots with their ON counterparts.
                        Send-DesignMouse 0x0200 120 116
                        Start-Sleep -Milliseconds 150
                        $redefineHoverHash = Get-WindowFrameHash
                        if ($redefineHoverHash -eq $detailHash) {
                            throw (
                                'Redefine Keys hover did not enter recovered ' +
                                'CKeyRedefiner visual state 3.'
                            )
                        }
                        Send-DesignMouse 0x0202 120 116
                        Start-Sleep -Milliseconds 100
                        $redefineCaptureHash = Get-WindowFrameHash
                        if ($redefineCaptureHash -eq $redefineHoverHash) {
                            throw (
                                'CKeyRedefiner::OnLeftUnclicked did not enter ' +
                                'the recovered PRESS CONTROL capture state.'
                            )
                        }
                        [void][VisualSmokeNativeMethods]::SendMessage(
                            $process.MainWindowHandle,
                            0x0100,
                            [UIntPtr]::new(0x5A),
                            [IntPtr]::Zero
                        )
                        Start-Sleep -Milliseconds 100
                        $redefineChangedHash = Get-WindowFrameHash
                        if ($redefineChangedHash -eq $redefineCaptureHash) {
                            throw (
                                'The recovered CKeyRedefiner capture state ' +
                                'did not apply the Z keyboard control.'
                            )
                        }
                        # UI_RESET action 284 restores the shipped arrow
                        # scheme; UI_RESET_WASD action 311 restores W/S/A/D.
                        Send-DesignMouse 0x0202 480 410
                        Start-Sleep -Milliseconds 100
                        $redefineArrowsHash = Get-WindowFrameHash
                        if ($redefineArrowsHash -eq $redefineChangedHash) {
                            throw (
                                'Retail action 284 did not reset movement ' +
                                'to the four arrow-key bindings.'
                            )
                        }
                        Send-DesignMouse 0x0202 160 410
                        Start-Sleep -Milliseconds 100
                        $redefineWasdHash = Get-WindowFrameHash
                        if ($redefineWasdHash -eq $redefineArrowsHash) {
                            throw (
                                'Retail action 311 did not reset movement ' +
                                'to the four W/S/A/D bindings.'
                            )
                        }
                        Send-DesignMouse 0x0200 160 410
                        Start-Sleep -Milliseconds 100
                        $redefineResetHoverHash = Get-WindowFrameHash
                        if ($redefineResetHoverHash -eq $redefineWasdHash) {
                            throw (
                                'UI_RESET_WASD did not enter its recovered ' +
                                'retail ON hover state.'
                            )
                        }
                        $detailHash = $redefineResetHoverHash
                    }
                    else {
                        # Detail-screen (Gameplay/Video/Audio) hover proofs.
                        # The footer buttons (Cancel/Defaults/Apply) and the
                        # per-row value arrows must now swap their baked OFF art
                        # for the ON/HOVERED overlay while the cursor is over
                        # them, exactly like the Options/About Back helper.
                        # Move onto Cancel (design 120,450), assert the frame
                        # changed, then onto Defaults (320,405), Apply
                        # (490,450), and finally a left value arrow
                        # (design x 320 in [300,340), the first row band), each
                        # producing a distinct frame hash from the last.
                        Send-DesignMouse 0x0200 120 450
                        Start-Sleep -Milliseconds 150
                        $detailCancelHoverHash = Get-WindowFrameHash
                        if ($detailCancelHoverHash -eq $detailHash) {
                            throw (
                                "The $Name Cancel footer button did not " +
                                'enter its recovered ON hover state.'
                            )
                        }
                        Send-DesignMouse 0x0200 320 405
                        Start-Sleep -Milliseconds 150
                        $detailDefaultsHoverHash = Get-WindowFrameHash
                        if ($detailDefaultsHoverHash -eq $detailCancelHoverHash) {
                            throw (
                                "The $Name Defaults footer button did not " +
                                'enter its recovered ON hover state.'
                            )
                        }
                        Send-DesignMouse 0x0200 490 450
                        Start-Sleep -Milliseconds 150
                        $detailApplyHoverHash = Get-WindowFrameHash
                        if ($detailApplyHoverHash -eq $detailDefaultsHoverHash) {
                            throw (
                                "The $Name Apply footer button did not " +
                                'enter its recovered ON hover state.'
                            )
                        }
                        # A value arrow: hover the first row's left arrow
                        # (design x 420 in [400,440), y $RowY-ish in the first
                        # row band), then move off it and assert both
                        # transitions change the frame hash.
                        Send-DesignMouse 0x0200 420 $FirstArrowY
                        Start-Sleep -Milliseconds 150
                        $detailArrowHoverHash = Get-WindowFrameHash
                        if ($detailArrowHoverHash -eq $detailApplyHoverHash) {
                            throw (
                                "The $Name row value arrow did not enter " +
                                'its recovered HOVERED overlay state.'
                            )
                        }
                        # Move the cursor into dead space; the arrow overlay
                        # must clear (frame changes back away from the hovered
                        # frame), proving the hover is not sticky.
                        Send-DesignMouse 0x0200 320 205
                        Start-Sleep -Milliseconds 150
                        $detailArrowClearHash = Get-WindowFrameHash
                        if ($detailArrowClearHash -eq $detailArrowHoverHash) {
                            throw (
                                "The $Name row value arrow hover did not " +
                                'clear when the cursor left it.'
                            )
                        }
                        $script:detailHoverProof = (
                            " detail-cancel-hover=" +
                            "$($detailCancelHoverHash.Substring(0, 12))" +
                            " detail-defaults-hover=" +
                            "$($detailDefaultsHoverHash.Substring(0, 12))" +
                            " detail-apply-hover=" +
                            "$($detailApplyHoverHash.Substring(0, 12))" +
                            " detail-arrow-hover=" +
                            "$($detailArrowHoverHash.Substring(0, 12))"
                        )
                        $detailHash = $detailArrowClearHash
                    }
                    if ($ScreenshotName) {
                        # Save the same client-area rectangle used by the
                        # reference-size gate and the Redefine initial proof.
                        # A full window capture includes the host title bar and
                        # makes the resulting detail reference 1040x807 rather
                        # than the retail 1024x768 client surface.
                        $detailBitmap = Get-ClientFrameBitmap
                        try {
                            $detailBitmap.Save(
                                (Join-Path (
                                    Split-Path -Parent $Executable
                                ) $ScreenshotName),
                                [System.Drawing.Imaging.ImageFormat]::Png
                            )
                        } finally {
                            $detailBitmap.Dispose()
                        }
                    }
                    if ($UseCancel) {
                        Send-DesignMouse 0x0202 120 450
                    } else {
                        Send-FrontendEscape
                    }
                    Start-Sleep -Milliseconds 250
                    $returnHash = Get-WindowFrameHash
                    if ($returnHash -eq $detailHash) {
                        throw (
                            "Retail action 86 did not return from the " +
                            "$Name detail screen."
                        )
                    }
                    return $detailHash
                }

                # Exercise the retail profile transaction around one text
                # slider: mutation is live, Cancel reverts it, Apply keeps it,
                # and Defaults restores the recovered profile default.
                Send-DesignMouse 0x0200 420 155
                Send-DesignMouse 0x0202 420 155
                Start-Sleep -Milliseconds 250
                $gameplayInitialHash = Get-ClientFrameHash
                $gameplayInitialFrame = Get-ClientFrameBitmap
                # The WinMM controller bridge emits these same virtual-key
                # routes. Prove detail focus and Left/Right mutation without
                # requiring physical controller hardware in automation.
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x27),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 100
                $gameplayControllerFirstHash = Get-ClientFrameHash
                $gameplayControllerFirstFrame = Get-ClientFrameBitmap
                $firstControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayControllerFirstFrame 450 75 100 70
                $firstBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayControllerFirstFrame 600 75 40 70
                $gameplayControllerFirstFrame.Dispose()
                if ($firstControlDelta -le ($firstBackdropDelta * 1.25 + 2)) {
                    throw (
                        'Controller-compatible Right input did not mutate ' +
                        'the focused Gameplay detail row ' +
                        "(control-delta=$([Math]::Round($firstControlDelta, 2)) " +
                        "backdrop-delta=$([Math]::Round($firstBackdropDelta, 2)))."
                    )
                }
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x25),
                    [IntPtr]::Zero
                )
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x28),
                    [IntPtr]::Zero
                )
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x27),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 100
                $gameplayControllerSecondHash = Get-ClientFrameHash
                $gameplayControllerSecondFrame = Get-ClientFrameBitmap
                $secondControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayControllerSecondFrame 450 75 100 70
                $secondBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayControllerSecondFrame 600 75 40 70
                $gameplayControllerSecondFrame.Dispose()
                if ($secondControlDelta -le ($secondBackdropDelta * 1.25 + 2)) {
                    throw (
                        'Controller-compatible Down/Right input did not ' +
                        'move focus and mutate the second Gameplay row ' +
                        "(control-delta=$([Math]::Round($secondControlDelta, 2)) " +
                        "backdrop-delta=$([Math]::Round($secondBackdropDelta, 2)))."
                    )
                }
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x25),
                    [IntPtr]::Zero
                )
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x26),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 100
                $gameplayControllerRestoredHash = Get-ClientFrameHash
                $gameplayControllerRestoredFrame = Get-ClientFrameBitmap
                $restoredControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayControllerRestoredFrame 450 75 100 70
                $restoredBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayControllerRestoredFrame 600 75 40 70
                $gameplayControllerRestoredFrame.Dispose()
                if ($restoredControlDelta -gt ($restoredBackdropDelta * 1.75 + 4)) {
                    throw (
                        'Controller-compatible detail mutation did not ' +
                        'round-trip to the activation values ' +
                        "(control-delta=$([Math]::Round($restoredControlDelta, 2)) " +
                        "backdrop-delta=$([Math]::Round($restoredBackdropDelta, 2)))."
                    )
                }
                Send-DesignMouse 0x0202 580 90
                Start-Sleep -Milliseconds 150
                $gameplayMutatedHash = Get-ClientFrameHash
                $gameplayMutatedFrame = Get-ClientFrameBitmap
                $mutatedControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayMutatedFrame 450 75 100 70
                $mutatedBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayMutatedFrame 600 75 40 70
                if ($mutatedControlDelta -le ($mutatedBackdropDelta * 1.25 + 2)) {
                    throw 'Gameplay text-slider action did not change its live value.'
                }
                Send-DesignMouse 0x0202 120 450
                Start-Sleep -Milliseconds 200
                Send-DesignMouse 0x0202 420 155
                Start-Sleep -Milliseconds 200
                $gameplayCancelledHash = Get-ClientFrameHash
                $gameplayCancelledFrame = Get-ClientFrameBitmap
                $cancelledControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayCancelledFrame 450 75 100 70
                $cancelledBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayCancelledFrame 600 75 40 70
                $gameplayCancelledFrame.Dispose()
                if ($cancelledControlDelta -gt ($cancelledBackdropDelta * 1.75 + 4)) {
                    throw (
                        'Gameplay Cancel did not restore the activation value ' +
                        "(control-delta=$([Math]::Round($cancelledControlDelta, 2)) " +
                        "backdrop-delta=$([Math]::Round($cancelledBackdropDelta, 2)))."
                    )
                }
                Send-DesignMouse 0x0202 580 90
                Start-Sleep -Milliseconds 150
                Send-DesignMouse 0x0202 490 450
                Start-Sleep -Milliseconds 200
                Send-DesignMouse 0x0202 420 155
                Start-Sleep -Milliseconds 200
                $gameplayAppliedHash = Get-ClientFrameHash
                $gameplayAppliedFrame = Get-ClientFrameBitmap
                $appliedControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayMutatedFrame `
                        $gameplayAppliedFrame 450 75 100 70
                $appliedBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayMutatedFrame `
                        $gameplayAppliedFrame 600 75 40 70
                $gameplayAppliedFrame.Dispose()
                $gameplayMutatedFrame.Dispose()
                if ($appliedControlDelta -gt ($appliedBackdropDelta * 1.75 + 4)) {
                    throw 'Gameplay Apply did not preserve the changed profile value.'
                }
                Send-DesignMouse 0x0202 320 405
                Start-Sleep -Milliseconds 150
                $gameplayDefaultsHash = Get-ClientFrameHash
                $gameplayDefaultsFrame = Get-ClientFrameBitmap
                $defaultsControlDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayDefaultsFrame 450 75 100 70
                $defaultsBackdropDelta =
                    Get-ClientDesignRegionMeanDifference `
                        $gameplayInitialFrame `
                        $gameplayDefaultsFrame 600 75 40 70
                $gameplayDefaultsFrame.Dispose()
                $gameplayInitialFrame.Dispose()
                if ($defaultsControlDelta -gt ($defaultsBackdropDelta * 1.75 + 4)) {
                    throw (
                        'Gameplay Defaults did not restore the recovered values ' +
                        "(control-delta=$([Math]::Round($defaultsControlDelta, 2)) " +
                        "backdrop-delta=$([Math]::Round($defaultsBackdropDelta, 2)))."
                    )
                }
                Send-DesignMouse 0x0202 490 450
                Start-Sleep -Milliseconds 200

                $script:detailHoverProof = ''
                $gameplayHash = Test-FrontendDetailScreen `
                    'Gameplay Options' 155 $optionsHoverHash `
                    'frontend-gameplay-options-smoke.png'
                $videoHash = Test-FrontendDetailScreen `
                    'Video Options' 185 $gameplayHash `
                    'frontend-video-options-smoke.png' $false 131
                $audioHash = Test-FrontendDetailScreen `
                    'Audio Options' 215 $videoHash `
                    'frontend-audio-options-smoke.png' $false 191
                $redefineHash = Test-FrontendDetailScreen `
                    'Redefine Keys' 245 $audioHash `
                    'frontend-redefine-keys-hover-smoke.png' $true

                Send-DesignMouse 0x0200 120 430
                Start-Sleep -Milliseconds 100
                $optionsBackHoverHash = Get-WindowFrameHash
                if ($optionsBackHoverHash -eq $optionsHoverHash) {
                    throw 'The retail Back helper did not enter its hovered state.'
                }
                Send-DesignMouse 0x0202 120 430
                Start-Sleep -Milliseconds 350
                $returnedMenuHash = Get-WindowFrameHash
                if ($returnedMenuHash -eq $optionsBackHoverHash) {
                    throw 'Retail action 86 did not return from Options.'
                }

                # Dispatch retail action 314, hover No, then return via action 86.
                Send-DesignMouse 0x0200 320 440
                Start-Sleep -Milliseconds 100
                Send-DesignMouse 0x0202 320 440
                Start-Sleep -Milliseconds 350
                $quitHash = Get-WindowFrameHash
                if ($quitHash -eq $returnedMenuHash) {
                    throw (
                        'Retail action 314 did not activate ' +
                        'UI_FRONTEND_QUIT_PROMPT.'
                    )
                }
                # The quit prompt clears to black and the helper bars are
                # opaque at their centres, so this point is a stable,
                # animation-independent proof of the OFF -> ON state change.
                $quitNoOffPixel = Get-ClientDesignPixelArgb 120 420
                Send-DesignMouse 0x0200 120 420
                $hoverDeadline = [DateTime]::UtcNow.AddSeconds(1)
                do {
                    Start-Sleep -Milliseconds 80
                    $quitNoOnPixel = Get-ClientDesignPixelArgb 120 420
                    if ($quitNoOnPixel -ne $quitNoOffPixel) {
                        break
                    }
                    # PostMessage is asynchronous. Re-post while a loaded
                    # build host is late presenting the first hover frame.
                    Send-DesignMouse 0x0200 120 420
                } while ([DateTime]::UtcNow -lt $hoverDeadline)
                $quitNoHoverHash = Get-WindowFrameHash
                if (
                    $quitNoOnPixel -eq $quitNoOffPixel -or
                    $quitNoHoverHash -eq $quitHash
                ) {
                    throw 'The retail Quit No helper did not enter its hovered state.'
                }

                $subscreenBitmap =
                    New-Object System.Drawing.Bitmap $width, $height
                $subscreenGraphics =
                    [System.Drawing.Graphics]::FromImage($subscreenBitmap)
                try {
                    $subscreenGraphics.CopyFromScreen(
                        $bounds.Left,
                        $bounds.Top,
                        0,
                        0,
                        $subscreenBitmap.Size
                    )
                    $subscreenScreenshot = Join-Path (
                        Split-Path -Parent $Executable
                    ) 'frontend-quit-prompt-smoke.png'
                    $subscreenBitmap.Save(
                        $subscreenScreenshot,
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $subscreenGraphics.Dispose()
                    $subscreenBitmap.Dispose()
                }

                Send-DesignMouse 0x0202 120 420
                Start-Sleep -Milliseconds 350
                $quitReturnHash = Get-WindowFrameHash
                if ($quitReturnHash -eq $quitNoHoverHash) {
                    throw 'Quit No action 86 did not return to the main menu.'
                }
                $frameProof += (
                    " options=$($optionsHash.Substring(0, 12))" +
                    " saves=$($saveMenuHash.Substring(0, 12))" +
                    " save-scroll=$($saveMenuDownHash.Substring(0, 12))" +
                    " save-highlight-delta=$saveSelectionDelta" +
                    " option-hover=$($optionsHoverHash.Substring(0, 12))" +
                    " gameplay=$($gameplayHash.Substring(0, 12))" +
                    " controls=cancel-apply-defaults" +
                    " video=$($videoHash.Substring(0, 12))" +
                    " audio=$($audioHash.Substring(0, 12))" +
                    " redefine=$($redefineHash.Substring(0, 12))" +
                    " redefine-wheel=verified" +
                    " redefine-scroll=$($script:redefineScrollHash.Substring(0, 12))" +
                    " redefine-scroll-end=$($script:redefineScrollEndHash.Substring(0, 12))" +
                    " redefine-scroll-hover=state3" +
                    " redefine-scroll-end-blank=verified" +
                    " redefine-offpage-remap=$($script:redefineOffPageRemap)" +
                    " redefine-hover=state3" +
                    $script:detailHoverProof +
                    " detail-hover=cancel-defaults-apply-arrow" +
                    " back-hover=$($optionsBackHoverHash.Substring(0, 12))" +
                    " quit=$($quitHash.Substring(0, 12))" +
                    " no-hover=$($quitNoHoverHash.Substring(0, 12))" +
                    " subscreens=verified"
                )

                # Enter the About screen: main-menu row 5 dispatches retail
                # action 321 -> used key 0x1c -> UI_FRONTEND_ABOUT_MENU, whose
                # recovered g_AboutTexture panel ("About Fable" title rule +
                # legal-notice message + Back) is a single 640x480 overlay.
                Send-DesignMouse 0x0200 320 410
                Start-Sleep -Milliseconds 100
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x0D),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 350
                $aboutHash = Get-WindowFrameHash
                if (
                    $aboutHash -eq $mainMenuHash -or
                    $aboutHash -eq $optionsHash -or
                    $aboutHash -eq $quitHash
                ) {
                    throw (
                        'Retail action 321 did not activate the recovered ' +
                        'UI_FRONTEND_ABOUT_MENU panel.'
                    )
                }
                $aboutBitmap =
                    New-Object System.Drawing.Bitmap $width, $height
                $aboutGraphics =
                    [System.Drawing.Graphics]::FromImage($aboutBitmap)
                try {
                    $aboutGraphics.CopyFromScreen(
                        $bounds.Left,
                        $bounds.Top,
                        0,
                        0,
                        $aboutBitmap.Size
                    )
                    $aboutScreenshot = Join-Path (
                        Split-Path -Parent $Executable
                    ) 'frontend-about-smoke.png'
                    $aboutBitmap.Save(
                        $aboutScreenshot,
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $aboutGraphics.Dispose()
                    $aboutBitmap.Dispose()
                }
                # About Back hover: moving the cursor into the shared UI_HELPERS
                # Back rect (design 20-270 x 420-450) must swap the live helper
                # glyph OFF->ON and change the frame hash.  Proves the hover
                # highlight the previously baked-only About panel lacked.
                Send-DesignMouse 0x0200 120 435
                Start-Sleep -Milliseconds 200
                $aboutBackHoverHash = Get-WindowFrameHash
                if ($aboutBackHoverHash -eq $aboutHash) {
                    throw 'The About Back helper did not enter its hovered state.'
                }
                # Move the cursor off the button before the keyboard Back.
                Send-DesignMouse 0x0200 320 205
                Start-Sleep -Milliseconds 200
                # UI_HELPERS/UI_BACK action 86 returns to the main menu.
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x1B),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 300
                $aboutReturnHash = Get-WindowFrameHash
                if ($aboutReturnHash -eq $aboutHash) {
                    throw 'About Back action 86 did not return to the menu.'
                }
                $frameProof += " about=$($aboutHash.Substring(0, 12))"
                $frameProof +=
                    " about-hover=$($aboutBackHoverHash.Substring(0, 12))"

                # Enter Credits: main-menu row 4 dispatches action 67 to the
                # compiled UI_FRONTEND_CREDITS_MENU.  Its initial scrolling
                # state is intentionally title + widescreen bars only; the
                # credit text container starts at y=480 in frontend.bin.
                Send-DesignMouse 0x0200 320 380
                Start-Sleep -Milliseconds 100
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x0D),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 350
                $creditsHash = Get-WindowFrameHash
                if (
                    $creditsHash -eq $mainMenuHash -or
                    $creditsHash -eq $optionsHash -or
                    $creditsHash -eq $aboutHash
                ) {
                    throw (
                        'Retail action 67 did not activate the recovered ' +
                        'UI_FRONTEND_CREDITS_MENU frame.'
                    )
                }
                $creditsBitmap =
                    New-Object System.Drawing.Bitmap $width, $height
                $creditsGraphics =
                    [System.Drawing.Graphics]::FromImage($creditsBitmap)
                try {
                    $creditsGraphics.CopyFromScreen(
                        $bounds.Left,
                        $bounds.Top,
                        0,
                        0,
                        $creditsBitmap.Size
                    )
                    $creditsBitmap.Save(
                        (Join-Path (
                            Split-Path -Parent $Executable
                        ) 'frontend-credits-smoke.png'),
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $creditsGraphics.Dispose()
                    $creditsBitmap.Dispose()
                }
                Send-DesignMouse 0x0200 120 435
                Start-Sleep -Milliseconds 200
                $creditsBackHoverHash = Get-WindowFrameHash
                if ($creditsBackHoverHash -eq $creditsHash) {
                    throw 'The Credits Back helper did not enter its hovered state.'
                }
                Send-FrontendEscape
                Start-Sleep -Milliseconds 300
                $creditsReturnHash = Get-WindowFrameHash
                if ($creditsReturnHash -eq $creditsBackHoverHash) {
                    throw 'Credits Back action 86 did not return to the menu.'
                }
                $frameProof +=
                    " credits=$($creditsHash.Substring(0, 12))"
                $frameProof +=
                    " credits-hover=$($creditsBackHoverHash.Substring(0, 12))"

                # Change Profile: action 16 refreshes the runtime profile
                # directory and enters the authored Type-43 list surface.
                # The row names are data-driven; the smoke proves the route
                # and shared Back helper without assuming a fixed profile set.
                Send-DesignMouse 0x0200 320 230
                Start-Sleep -Milliseconds 100
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x0D),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 350
                $profilesHash = Get-WindowFrameHash
                if (
                    $profilesHash -eq $mainMenuHash -or
                    $profilesHash -eq $creditsHash
                ) {
                    throw (
                        'Retail action 16 did not activate the recovered ' +
                        'UI_FRONTEND_PROFILES_MENU surface.'
                    )
                }
                Send-DesignMouse 0x0200 120 435
                Start-Sleep -Milliseconds 200
                $profilesBackHoverHash = Get-WindowFrameHash
                if ($profilesBackHoverHash -eq $profilesHash) {
                    throw 'The Profile Back helper did not enter its hovered state.'
                }
                Send-FrontendEscape
                Start-Sleep -Milliseconds 300
                $profilesReturnHash = Get-WindowFrameHash
                if ($profilesReturnHash -eq $profilesBackHoverHash) {
                    throw 'Profile Back action 86 did not return to the menu.'
                }
                $frameProof +=
                    " profiles=$($profilesHash.Substring(0, 12))"
                $frameProof +=
                    " profiles-hover=$($profilesBackHoverHash.Substring(0, 12))"

                # Re-enter the prompt and validate action 296 last, because
                # its retail meaning is to end the application.
                Send-DesignMouse 0x0200 320 440
                Start-Sleep -Milliseconds 100
                Send-DesignMouse 0x0202 320 440
                Start-Sleep -Milliseconds 250
                Send-DesignMouse 0x0200 490 420
                Start-Sleep -Milliseconds 100
                Send-DesignMouse 0x0202 490 420
                $quitViaRetailAction = $true
            }
        }
    }

    if (-not $quitViaRetailAction) {
        if (-not $process.CloseMainWindow()) {
            throw 'The visual checkpoint did not accept WM_CLOSE.'
        }
    }
    if (-not $process.WaitForExit($TimeoutSeconds * 1000)) {
        $exitTrigger = if ($quitViaRetailAction) {
            'retail Quit Yes action 296'
        } else {
            'WM_CLOSE'
        }
        throw "The visual checkpoint did not exit after $exitTrigger."
    }
    if ($process.ExitCode -ne 0) {
        throw "The visual checkpoint exited with code $($process.ExitCode)."
    }

    if ($quitViaRetailAction) {
        $frameProof += ' yes-action=296-exited'
    }
    if ($BuffJesus) {
        $frameProof += ' text=buff-jesus'
    }
    Write-Output (
        "VISUAL_WINDOW_SMOKE PASS title=$title" +
        "$frameProof$maximizedScaleProof$retailReferenceSizeProof movie=$Movie " +
        "exit=$($process.ExitCode)"
    )
} finally {
    if (-not $process.HasExited) {
        Stop-Process -Id $process.Id
    }
}
