param(
    [string]$Executable = '',

    [int]$TimeoutSeconds = 10
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $Executable) {
    $Executable = Join-Path $rebuildRoot `
        'build\bootstrap-Release\FableTLC-Reconstruction-VisualCheckpoint.exe'
}
$Executable = (Resolve-Path -LiteralPath $Executable).Path

$process = Start-Process -FilePath $Executable -PassThru
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
        if ($title -like '*D3D9 Presented via Render2D*') {
            break
        }
    }

    if ($title -notlike '*D3D9 Presented via Render2D*') {
        throw "D3D9 presentation was not observed; last title was '$title'."
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
        "VISUAL_WINDOW_SMOKE PASS title=$title exit=$($process.ExitCode)"
    )
} finally {
    if (-not $process.HasExited) {
        Stop-Process -Id $process.Id
    }
}
