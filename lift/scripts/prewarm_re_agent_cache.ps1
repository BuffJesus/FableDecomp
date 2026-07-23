$ErrorActionPreference = 'Stop'

$liftRoot = 'D:\Documents\FableTLC\lift'
$stateRoot = Join-Path $liftRoot 'state'
$cli = Join-Path $liftRoot 'bin\ghidra_cli.cmd'
$cacheRoot = Join-Path $liftRoot '.cache\re-agent-decompile'
$logPath = Join-Path $stateRoot 're-agent-cache-prewarm.log'
$pidPath = Join-Path $stateRoot 're-agent-cache-prewarm.pid'

$targets = @(
    '0x00BFE050', '0x00BF3B60', '0x00BE7D70', '0x00BE8920',
    '0x00BF3A90', '0x00BEBED0', '0x00BEC000', '0x00BEC470',
    '0x00BEC950', '0x00BEAAA0', '0x00BEB4D0', '0x00BF5C20',
    '0x00BF5C60', '0x00BF5CA0', '0x00BF5CD0', '0x00BF5D00'
)

function Write-PrewarmLog([string]$Message) {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $logPath -Value "[$stamp] $Message" -Encoding UTF8
}

New-Item -ItemType Directory -Path $cacheRoot -Force | Out-Null
Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
Write-PrewarmLog "START pid=$PID targets=$($targets.Count)"

try {
    foreach ($target in $targets) {
        $cacheName = $target.ToLowerInvariant() + '.txt'
        $cachePath = Join-Path $cacheRoot $cacheName
        if ((Test-Path -LiteralPath $cachePath) -and (Get-Item $cachePath).Length -gt 0) {
            Write-PrewarmLog "SKIP $target cached"
            continue
        }

        Write-PrewarmLog "RUN $target"
        $output = & $cli decompile $target
        if ($LASTEXITCODE -ne 0) {
            throw "Ghidra decompile failed for $target with exit $LASTEXITCODE"
        }
        Set-Content -LiteralPath $cachePath -Value $output -Encoding UTF8
        Write-PrewarmLog "DONE $target bytes=$((Get-Item $cachePath).Length)"
    }
} catch {
    Write-PrewarmLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Write-PrewarmLog "END pid=$PID"
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
}
