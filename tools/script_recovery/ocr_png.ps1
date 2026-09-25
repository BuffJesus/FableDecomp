# Windows' built-in OCR (Windows.Media.Ocr) over a PNG: one line per recognised text line, "x y w h<TAB>text"
# (pixel box of the line in the image). Used by ingame_runner.py to find a quest's row on the card-table screen.
param([Parameter(Mandatory = $true)][string]$Path)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Runtime.WindowsRuntime
$null = [Windows.Storage.StorageFile, Windows.Storage, ContentType = WindowsRuntime]
$null = [Windows.Media.Ocr.OcrEngine, Windows.Foundation, ContentType = WindowsRuntime]
$null = [Windows.Graphics.Imaging.BitmapDecoder, Windows.Foundation, ContentType = WindowsRuntime]
$asTask = ([System.WindowsRuntimeSystemExtensions].GetMethods() | Where-Object {
    $_.Name -eq 'AsTask' -and $_.GetParameters().Count -eq 1 -and $_.GetParameters()[0].ParameterType.Name -eq 'IAsyncOperation`1' })[0]
function Await($op, [Type]$t) {
    $task = $asTask.MakeGenericMethod($t).Invoke($null, @($op))
    $task.Wait() | Out-Null
    $task.Result
}
$file = Await ([Windows.Storage.StorageFile]::GetFileFromPathAsync((Resolve-Path $Path).Path)) ([Windows.Storage.StorageFile])
$stream = Await ($file.OpenAsync([Windows.Storage.FileAccessMode]::Read)) ([Windows.Storage.Streams.IRandomAccessStream])
$decoder = Await ([Windows.Graphics.Imaging.BitmapDecoder]::CreateAsync($stream)) ([Windows.Graphics.Imaging.BitmapDecoder])
$bitmap = Await ($decoder.GetSoftwareBitmapAsync()) ([Windows.Graphics.Imaging.SoftwareBitmap])
$engine = [Windows.Media.Ocr.OcrEngine]::TryCreateFromUserProfileLanguages()
$result = Await ($engine.RecognizeAsync($bitmap)) ([Windows.Media.Ocr.OcrResult])
foreach ($line in $result.Lines) {
    $xs = $line.Words | ForEach-Object { $_.BoundingRect }
    $x = ($xs | Measure-Object -Property X -Minimum).Minimum
    $y = ($xs | Measure-Object -Property Y -Minimum).Minimum
    $r = ($xs | ForEach-Object { $_.X + $_.Width } | Measure-Object -Maximum).Maximum
    $b = ($xs | ForEach-Object { $_.Y + $_.Height } | Measure-Object -Maximum).Maximum
    "{0:0} {1:0} {2:0} {3:0}`t{4}" -f $x, $y, ($r - $x), ($b - $y), $line.Text
}
$stream.Dispose()
