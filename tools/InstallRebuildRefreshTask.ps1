param(
    [int]$Minutes = 15
)

$ErrorActionPreference = 'Stop'
$taskName = 'FableTLC Rebuild Refresh'
$script = 'D:\Documents\FableTLC\tools\run_rebuild_refresh.ps1'
$powershell = "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe"
$action = New-ScheduledTaskAction -Execute $powershell -Argument (
    "-NoProfile -WindowStyle Hidden -ExecutionPolicy Bypass -File `"$script`""
)
$trigger = New-ScheduledTaskTrigger -Once -At (Get-Date).AddMinutes(1) `
    -RepetitionInterval (New-TimeSpan -Minutes $Minutes) `
    -RepetitionDuration (New-TimeSpan -Days 3650)
$settings = New-ScheduledTaskSettingsSet -Hidden -MultipleInstances IgnoreNew `
    -RestartCount 3 -RestartInterval (New-TimeSpan -Minutes 1) `
    -ExecutionTimeLimit (New-TimeSpan -Hours 1) -StartWhenAvailable
$principal = New-ScheduledTaskPrincipal -UserId $env:USERNAME -LogonType Interactive -RunLevel Limited

Register-ScheduledTask -TaskName $taskName -Action $action -Trigger $trigger `
    -Settings $settings -Principal $principal -Description (
        'Refreshes FableTLC candidate compile gates, behavior tests, retail parity, manifests, and dashboards when inputs change.'
    ) -Force | Out-Null
Write-Output "REBUILD_REFRESH_TASK installed name=$taskName intervalMinutes=$Minutes"
