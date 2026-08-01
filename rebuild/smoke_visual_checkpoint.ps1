param(
    [string]$Executable = '',

    [int]$TimeoutSeconds = 10,

    [switch]$RetailVideo,

    [switch]$VerifyBootSequence,

    [switch]$VerifyEscapeSkip,

    [switch]$VerifyBootToFrontend,

    [switch]$VerifyMaximizedScale,

    [switch]$VerifyFrontendAnimation,

    [switch]$VerifyMainMenu,

    [switch]$VerifySubscreens,

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

                # Continue Game dispatches action 66, refreshes the recovered
                # autosave/manual ordering, and enters used key 0x08.
                Send-DesignMouse 0x0200 320 205
                Start-Sleep -Milliseconds 100
                $saveSelectionBeforeArgb =
                    Get-ClientDesignPixelArgb 100 100
                [void][VisualSmokeNativeMethods]::SendMessage(
                    $process.MainWindowHandle,
                    0x0100,
                    [UIntPtr]::new(0x0D),
                    [IntPtr]::Zero
                )
                Start-Sleep -Milliseconds 250
                $saveMenuHash = Get-WindowFrameHash
                $saveSelectionAfterArgb =
                    Get-ClientDesignPixelArgb 100 100
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
                $saveDiagnosticBitmap =
                    New-Object System.Drawing.Bitmap $width, $height
                $saveDiagnosticGraphics =
                    [System.Drawing.Graphics]::FromImage(
                        $saveDiagnosticBitmap)
                try {
                    $saveDiagnosticGraphics.CopyFromScreen(
                        $bounds.Left,
                        $bounds.Top,
                        0,
                        0,
                        $saveDiagnosticBitmap.Size
                    )
                    $saveDiagnosticBitmap.Save(
                        (Join-Path (
                            Split-Path -Parent $Executable
                        ) 'frontend-saved-games-smoke.png'),
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $saveDiagnosticGraphics.Dispose()
                    $saveDiagnosticBitmap.Dispose()
                }
                if (
                    $saveMenuHash -eq $hoverMenuHash -or
                    $saveMenuHash -eq $mainMenuHash -or
                    $saveSelectionDelta -lt 40
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
                $saveBitmap =
                    New-Object System.Drawing.Bitmap $width, $height
                $saveGraphics =
                    [System.Drawing.Graphics]::FromImage($saveBitmap)
                try {
                    $saveGraphics.CopyFromScreen(
                        $bounds.Left,
                        $bounds.Top,
                        0,
                        0,
                        $saveBitmap.Size
                    )
                    $saveScreenshot = Join-Path (
                        Split-Path -Parent $Executable
                    ) 'frontend-saved-games-smoke.png'
                    $saveBitmap.Save(
                        $saveScreenshot,
                        [System.Drawing.Imaging.ImageFormat]::Png
                    )
                } finally {
                    $saveGraphics.Dispose()
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
                        [bool]$UseCancel = $false
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
                    if ($ScreenshotName) {
                        $detailBitmap =
                            New-Object System.Drawing.Bitmap $width, $height
                        $detailGraphics =
                            [System.Drawing.Graphics]::FromImage(
                                $detailBitmap)
                        try {
                            $detailGraphics.CopyFromScreen(
                                $bounds.Left,
                                $bounds.Top,
                                0,
                                0,
                                $detailBitmap.Size
                            )
                            $detailBitmap.Save(
                                (Join-Path (
                                    Split-Path -Parent $Executable
                                ) $ScreenshotName),
                                [System.Drawing.Imaging.ImageFormat]::Png
                            )
                        } finally {
                            $detailGraphics.Dispose()
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
                Send-DesignMouse 0x0200 320 155
                Send-DesignMouse 0x0202 320 155
                Start-Sleep -Milliseconds 250
                $gameplayInitialHash = Get-ClientFrameHash
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
                if ($gameplayControllerFirstHash -eq $gameplayInitialHash) {
                    throw (
                        'Controller-compatible Right input did not mutate ' +
                        'the focused Gameplay detail row.'
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
                if ($gameplayControllerSecondHash -eq $gameplayInitialHash) {
                    throw (
                        'Controller-compatible Down/Right input did not ' +
                        'move focus and mutate the second Gameplay row.'
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
                if ($gameplayControllerRestoredHash -ne $gameplayInitialHash) {
                    throw (
                        'Controller-compatible detail mutation did not ' +
                        'round-trip to the activation values.'
                    )
                }
                Send-DesignMouse 0x0202 480 90
                Start-Sleep -Milliseconds 150
                $gameplayMutatedHash = Get-ClientFrameHash
                if ($gameplayMutatedHash -eq $gameplayInitialHash) {
                    throw 'Gameplay text-slider action did not change its live value.'
                }
                Send-DesignMouse 0x0202 120 450
                Start-Sleep -Milliseconds 200
                Send-DesignMouse 0x0202 320 155
                Start-Sleep -Milliseconds 200
                $gameplayCancelledHash = Get-ClientFrameHash
                if ($gameplayCancelledHash -ne $gameplayInitialHash) {
                    throw (
                        'Gameplay Cancel did not restore the activation value ' +
                        "(initial=$($gameplayInitialHash.Substring(0, 12)) " +
                        "cancelled=$($gameplayCancelledHash.Substring(0, 12)))."
                    )
                }
                Send-DesignMouse 0x0202 480 90
                Start-Sleep -Milliseconds 150
                Send-DesignMouse 0x0202 490 450
                Start-Sleep -Milliseconds 200
                Send-DesignMouse 0x0202 320 155
                Start-Sleep -Milliseconds 200
                $gameplayAppliedHash = Get-ClientFrameHash
                if ($gameplayAppliedHash -ne $gameplayMutatedHash) {
                    throw 'Gameplay Apply did not preserve the changed profile value.'
                }
                Send-DesignMouse 0x0202 320 405
                Start-Sleep -Milliseconds 150
                $gameplayDefaultsHash = Get-ClientFrameHash
                if ($gameplayDefaultsHash -ne $gameplayInitialHash) {
                    throw (
                        'Gameplay Defaults did not restore the recovered values ' +
                        "(initial=$($gameplayInitialHash.Substring(0, 12)) " +
                        "defaults=$($gameplayDefaultsHash.Substring(0, 12)))."
                    )
                }
                Send-DesignMouse 0x0202 490 450
                Start-Sleep -Milliseconds 200

                $gameplayHash = Test-FrontendDetailScreen `
                    'Gameplay Options' 155 $optionsHoverHash `
                    'frontend-gameplay-options-smoke.png'
                $videoHash = Test-FrontendDetailScreen `
                    'Video Options' 185 $gameplayHash
                $audioHash = Test-FrontendDetailScreen `
                    'Audio Options' 215 $videoHash
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
                    " redefine-hover=state3" +
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
        "$frameProof$maximizedScaleProof movie=$Movie " +
        "exit=$($process.ExitCode)"
    )
} finally {
    if (-not $process.HasExited) {
        Stop-Process -Id $process.Id
    }
}
