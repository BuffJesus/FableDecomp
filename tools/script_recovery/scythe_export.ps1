param(
    [string]$Workspace = (Resolve-Path (Join-Path $PSScriptRoot '..\..')),
    [string]$GhidraHome = 'D:\Subuwu\tools\ghidra-public'
)
$ErrorActionPreference = 'Stop'
$taskEvidence = Join-Path $Workspace 'work\scythe_converter\evidence'
New-Item -ItemType Directory -Force -Path $taskEvidence | Out-Null
$arguments = @(
    (Join-Path $Workspace 'ghidra_proj'), 'FableTLC',
    '-process', 'Fable.exe', '-readOnly', '-noanalysis',
    '-scriptPath', (Join-Path $Workspace 'tools\ghidra_scripts'),
    '-postScript', 'DecompFuncsToDirectory.java', (Join-Path $taskEvidence 'decompiles'),
    '0x00E29F50', '0x00E2A0F0', '0x00E2A320', '0x00E2A900', '0x00E29E80', '0x00E2A020'
)
& (Join-Path $GhidraHome 'support\analyzeHeadless.bat') @arguments
if ($LASTEXITCODE -ne 0) { throw 'Read-only Scythe native export failed.' }
& (Join-Path $Workspace 'tools\query_pdb_oracle.ps1') -Name 'NScript::CQS_ScytheInfoScript' -Mode Type |
    Set-Content -Encoding utf8 (Join-Path $taskEvidence 'Ego_r-class-layout.txt')
