param(
    [string]$Executable = '',

    [int]$TimeoutSeconds = 10,

    [switch]$RetailVideo,

    [switch]$VerifyBootSequence,

    [switch]$VerifyEscapeSkip,

    [switch]$VerifyBootToFrontend,

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

if ($RetailVideo) {
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

    [DllImport("user32.dll")]
    public static extern bool GetWindowRect(IntPtr window, out Rect rectangle);

    [DllImport("user32.dll")]
    public static extern IntPtr GetWindow(IntPtr window, uint command);

    [DllImport("user32.dll")]
    public static extern bool PostMessage(
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

        Send-EscapeKey
        Wait-ForMovieTitle '*Retail WMV Playing 2/3 - Microsoft*'
        Send-EscapeKey
        Wait-ForMovieTitle '*Retail WMV Playing 3/3 - Intro*'

        if ($VerifyBootToFrontend) {
            Send-EscapeKey
            Wait-ForMovieTitle (
                '*Retail Boot Movies Complete - ' +
                'Frontend Checkpoint Ready*'
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

    $frameProof = ''
    if ($RetailVideo) {
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

        if ($VerifyBootToFrontend) {
            Start-Sleep -Milliseconds 600
        }
        $firstFrameHash = Get-WindowFrameHash
        if ($VerifyBootToFrontend) {
            $sampleColorCount = Get-WindowSampleColorCount
            if ($sampleColorCount -lt 12) {
                throw (
                    'The DirectShow child closed, but the revealed frontend ' +
                    "checkpoint was visually flat ($sampleColorCount colours)."
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
                ' directshow-child=closed'
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
                throw (
                    'The retail WMV clock ran, but no changing frame was ' +
                    'observed before the smoke deadline.'
                )
            }
            $frameProof = (
                " frames=changed first=$($firstFrameHash.Substring(0, 12))" +
                " second=$($secondFrameHash.Substring(0, 12))"
            )
        }
    }

    if (-not $process.CloseMainWindow()) {
        throw 'The visual checkpoint did not accept WM_CLOSE.'
    }
    if (-not $process.WaitForExit($TimeoutSeconds * 1000)) {
        throw 'The visual checkpoint did not exit after WM_CLOSE.'
    }
    if ($process.ExitCode -ne 0) {
        throw "The visual checkpoint exited with code $($process.ExitCode)."
    }

    Write-Output (
        "VISUAL_WINDOW_SMOKE PASS title=$title" +
        "$frameProof movie=$Movie exit=$($process.ExitCode)"
    )
} finally {
    if (-not $process.HasExited) {
        Stop-Process -Id $process.Id
    }
}
