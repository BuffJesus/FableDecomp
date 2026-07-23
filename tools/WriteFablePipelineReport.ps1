param(
    [string]$StatsLog = "ghidra_out\stats_fable_after_fablewin.log",
    [string[]]$BsimAudits = @(
        "ghidra_out\bsim_port_audit_fabletlc2_07.tsv",
        "ghidra_out\bsim_port_audit_fabletlc2_05.tsv"
    ),
    [string]$OutPath = "ghidra_out\fablewin_pipeline_report.md"
)

$ErrorActionPreference = "Stop"

$Root = "D:\Documents\FableTLC"
Set-Location $Root

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add("# FableWin BSim pipeline report")
$lines.Add("")
$lines.Add("Generated: $(Get-Date -Format s)")
$lines.Add("")

if (Test-Path $StatsLog) {
    $stats = powershell -NoProfile -ExecutionPolicy Bypass -File tools\SummarizeStatsLog.ps1 -Path $StatsLog
    $lines.Add("## Coverage")
    $lines.Add("")
    $lines.Add('```')
    $stats | ForEach-Object { $lines.Add($_) }
    $lines.Add('```')
    $lines.Add("")
}
else {
    $lines.Add("## Coverage")
    $lines.Add("")
    $lines.Add(("Stats log not present yet: {0}" -f $StatsLog))
    $lines.Add("")
}

$lines.Add("## BSim audits")
$lines.Add("")
foreach ($audit in $BsimAudits) {
    $lines.Add("### $audit")
    $lines.Add("")
    if (Test-Path $audit) {
        $summary = powershell -NoProfile -ExecutionPolicy Bypass -File tools\SummarizeBsimAudit.ps1 -Path $audit
        $lines.Add('```')
        $summary | ForEach-Object { $lines.Add($_) }
        $lines.Add('```')
    }
    else {
        $lines.Add("Not present yet.")
    }
    $lines.Add("")
}

$lines | Set-Content -Path $OutPath -Encoding UTF8
Write-Host "Wrote $OutPath"
