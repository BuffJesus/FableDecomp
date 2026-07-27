param(
    [string]$Executable = '',

    [int]$TimeoutSeconds = 10,

    [switch]$RetailVideo,

    [ValidateSet('microsoft', 'lionhead', 'attract', 'intro')]
    [string]$Movie = 'microsoft'
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
}
'@
    }
}

$arguments = @()
if ($RetailVideo) {
    $arguments += if ($Movie -eq 'microsoft') {
        '--retail-video'
    } else {
        "--retail-video=$Movie"
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

        $firstFrameHash = Get-WindowFrameHash
        Start-Sleep -Milliseconds 600
        $secondFrameHash = Get-WindowFrameHash
        if ($firstFrameHash -eq $secondFrameHash) {
            throw 'The retail WMV clock ran, but two captured frames were identical.'
        }
        $frameProof = (
            " frames=changed first=$($firstFrameHash.Substring(0, 12))" +
            " second=$($secondFrameHash.Substring(0, 12))"
        )
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
