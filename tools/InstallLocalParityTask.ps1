param(
    [int]$Minutes = 60
)

$ErrorActionPreference = 'Stop'
$taskName = 'FableTLC Local Parity Queue'
$script = 'D:\Documents\FableTLC\tools\run_local_parity_queue.ps1'
$powershell = "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe"
$action = New-ScheduledTaskAction -Execute $powershell -Argument (
    "-NoProfile -WindowStyle Hidden -ExecutionPolicy Bypass -File `"$script`""
)
$trigger = New-ScheduledTaskTrigger -Once -At (Get-Date).AddMinutes(5) `
    -RepetitionInterval (New-TimeSpan -Minutes $Minutes) `
    -RepetitionDuration (New-TimeSpan -Days 3650)
$settings = New-ScheduledTaskSettingsSet -Hidden -MultipleInstances IgnoreNew `
    -RestartCount 3 -RestartInterval (New-TimeSpan -Minutes 2) `
    -ExecutionTimeLimit (New-TimeSpan -Hours 2) -StartWhenAvailable
$principal = New-ScheduledTaskPrincipal -UserId $env:USERNAME -LogonType Interactive -RunLevel Limited

Register-ScheduledTask -TaskName $taskName -Action $action -Trigger $trigger `
    -Settings $settings -Principal $principal -Description (
        'Selects short retail functions, authors deterministic VC7.1 candidates, lands exact byte-and-behavior matches, and refreshes public progress without using Ghidra.'
    ) -Force | Out-Null
Write-Output "LOCAL_PARITY_TASK installed name=$taskName intervalMinutes=$Minutes"
