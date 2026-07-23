param(
    [string]$FseRoot = "D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\FableScriptExtender",
    [string]$ReferenceOut = "refs\fse_engine_addresses.tsv",
    [string]$LabelOut = "ghidra_out\labels_fse_addresses.tsv"
)

$ErrorActionPreference = "Stop"

$Root = "D:\Documents\FableTLC"
Set-Location $Root

if (-not (Test-Path $FseRoot)) {
    throw "Missing FSE source root: $FseRoot"
}

function Clean-Name($Name) {
    $n = $Name.Trim()
    $n = $n -replace "^(auto|DWORD|void\*|void\*\*|\w+)\s+", ""
    $n = $n -replace "^\*+", ""
    $n = $n -replace "[^A-Za-z0-9_:]+", "_"
    $n = $n.Trim("_")
    if ($n -eq "hA") { return "FSE_NativeScriptRegistrationHook" }
    return $n
}

$rows = New-Object System.Collections.Generic.List[object]

Get-ChildItem $FseRoot -Recurse -File -Include *.cpp,*.h,*.hpp | ForEach-Object {
    $file = $_.FullName
    $rel = Resolve-Path -Path $file -Relative
    $lineNo = 0
    Get-Content $file | ForEach-Object {
        $lineNo++
        $line = $_
        $m = [regex]::Match($line, "^\s*(?<lhs>[^=;]+?)\s*=\s*ASLR\s*<(?<type>[^>]+)>\s*\(\s*(?<addr>0x[0-9A-Fa-f]+)\s*\)")
        if (-not $m.Success) { return }

        $name = Clean-Name $m.Groups["lhs"].Value
        if ([string]::IsNullOrWhiteSpace($name)) { return }

        $type = $m.Groups["type"].Value.Trim()
        $kind = if ($type -match "^\s*t[A-Za-z0-9_]+$") { "function" } else { "data" }
        $addr = $m.Groups["addr"].Value.ToLowerInvariant()
        $comment = "FSE ASLR source: ${rel}:${lineNo} type=$type"

        $rows.Add([pscustomobject]@{
            Address = $addr
            Name = $name
            Kind = $kind
            Type = $type
            Source = "${rel}:${lineNo}"
            Comment = $comment
        })
    }
}

$rows = $rows | Sort-Object Address,Name -Unique

$refLines = New-Object System.Collections.Generic.List[string]
$refLines.Add("#address`tname`tkind`ttype`tsource`tcomment")
foreach ($r in $rows) {
    $refLines.Add("$($r.Address)`t$($r.Name)`t$($r.Kind)`t$($r.Type)`t$($r.Source)`t$($r.Comment)")
}
$refLines | Set-Content -Path $ReferenceOut -Encoding UTF8

$labelLines = New-Object System.Collections.Generic.List[string]
$labelLines.Add("#addr`tname`tcomment")
foreach ($r in ($rows | Where-Object { $_.Kind -eq "function" })) {
    $labelLines.Add("$($r.Address)`t$($r.Name)`t$($r.Comment)")
}
$labelLines | Set-Content -Path $LabelOut -Encoding UTF8

Write-Host "Extracted FSE ASLR addresses: $($rows.Count)"
Write-Host "Function labels: $(($rows | Where-Object { $_.Kind -eq 'function' } | Measure-Object).Count)"
Write-Host "Reference TSV: $ReferenceOut"
Write-Host "Label TSV: $LabelOut"
