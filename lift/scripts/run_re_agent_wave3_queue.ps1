param(
    [int]$MaxAttemptsPerTarget = 2,
    [int]$MaxTargetsPerRun = 16,
    [int]$TargetTimeoutMinutes = 35,
    [int]$RefreshWaitMinutes = 5,
    [switch]$Preview
)

$ErrorActionPreference = 'Stop'
$root = 'D:\Documents\FableTLC'
$liftRoot = Join-Path $root 'lift'
$legacyPidPath = Join-Path $liftRoot 're-agent-wave3-queue.pid'
if (Test-Path -LiteralPath $legacyPidPath) {
    try {
        $legacyPid = [int](Get-Content -LiteralPath $legacyPidPath -Raw)
        $legacyProcess = Get-CimInstance Win32_Process -Filter "ProcessId = $legacyPid" -ErrorAction SilentlyContinue
        if ($legacyPid -ne $PID -and $null -ne $legacyProcess -and
            $legacyProcess.CommandLine -like '*run_re_agent_wave3_queue.ps1*') {
            exit 0
        }
    } catch {
        # Stale or malformed legacy state is handled by the migration script.
    }
}
& (Join-Path $liftRoot 'scripts\migrate_wave3_layout.ps1') | Out-Null
$stateRoot = Join-Path $liftRoot 'state'
$configPath = Join-Path $liftRoot 'config\re-agent-wave3.yaml'
$sessionPath = Join-Path $stateRoot 're-agent-wave3-progress.json'
$queueLog = Join-Path $stateRoot 're-agent-wave3-queue.log'
$pidPath = Join-Path $stateRoot 're-agent-wave3-queue.pid'
$stopPath = Join-Path $stateRoot 're-agent-wave3-queue.stop'
$completePath = Join-Path $stateRoot 're-agent-wave3-queue.complete'
$refreshPidPath = Join-Path $root 'rebuild\refresh.pid'
$reAgent = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\Scripts\re-agent.exe'

$env:RE_AGENT_CODEX_BIN = 'C:\Users\Cornelio\AppData\Local\JetBrains\Rider2026.2\acp-agents\.runtimes\node\24.13.0\node.exe'
$env:RE_AGENT_CODEX_JS = 'C:\Users\Cornelio\AppData\Local\JetBrains\Rider2026.2\acp-agents\.runtimes\node\24.13.0\npm-cache\_npx\a758dee5a93640a8\node_modules\@openai\codex\bin\codex.js'
$env:RE_AGENT_CODEX_MAX_ATTEMPTS = '3'
$env:RE_AGENT_DECOMPILE_CACHE_DIR = 'D:\Documents\FableTLC\lift\.cache\re-agent-decompile'

# === Co-op / multiplayer cluster (priority probe, added 2026-07-23) =========
# Fable retains a disabled co-op subsystem (docs/FINDINGS.md). Decompile the GATE,
# the CNetworkClient client/host lifecycle, the CGameEventPackage replication +
# WIRE FORMAT (Compress/InitFromCompressedBuffer), and the CTCCoopSpirit entity so
# we can judge how complete-vs-gutted the path is and plan a revival. These run
# first (already-recorded addresses are skipped by the shared ledgers).
$coopTargets = @(
    [pscustomobject]@{ Address = '0x00449D20'; Slug = 'mp-playermgr-ismultiplayergameactive' },
    [pscustomobject]@{ Address = '0x00449B60'; Slug = 'mp-playermgr-getmultiplayercolour' },
    [pscustomobject]@{ Address = '0x004AE940'; Slug = 'mp-networkclient-initialiseaslocal' },
    [pscustomobject]@{ Address = '0x004AE9D0'; Slug = 'mp-networkclient-update' },
    [pscustomobject]@{ Address = '0x004AEA70'; Slug = 'mp-networkclient-isfreetorender' },
    [pscustomobject]@{ Address = '0x004AEAA0'; Slug = 'mp-networkclient-getlocaleventpackageset' },
    [pscustomobject]@{ Address = '0x004AEBA0'; Slug = 'mp-networkclient-geteventpackageset' },
    [pscustomobject]@{ Address = '0x00416670'; Slug = 'mp-processeventpackage' },
    [pscustomobject]@{ Address = '0x004165E8'; Slug = 'mp-checksync' },
    [pscustomobject]@{ Address = '0x0041726D'; Slug = 'mp-updatefromeventpackageset' },
    [pscustomobject]@{ Address = '0x00416148'; Slug = 'mp-geteventpackagesetfromsave' },
    [pscustomobject]@{ Address = '0x004161A7'; Slug = 'mp-addeventpackagesettosave' },
    [pscustomobject]@{ Address = '0x009F1810'; Slug = 'mp-gameevent-compressintobuffer' },
    [pscustomobject]@{ Address = '0x009F1870'; Slug = 'mp-gameevent-initfromcompressedbuffer' },
    [pscustomobject]@{ Address = '0x009F19A0'; Slug = 'mp-packageset-compressintobuffer' },
    [pscustomobject]@{ Address = '0x009F1AC0'; Slug = 'mp-packageset-initfromcompressedbuffer' },
    [pscustomobject]@{ Address = '0x009F16F0'; Slug = 'mp-packageset-addpackage' },
    [pscustomobject]@{ Address = '0x00A0D340'; Slug = 'mp-processedinput-addgameevent' },
    [pscustomobject]@{ Address = '0x004D55D0'; Slug = 'mp-coopspirit-construct' },
    [pscustomobject]@{ Address = '0x006700F0'; Slug = 'mp-coopspirit-oncreate' },
    [pscustomobject]@{ Address = '0x006701A0'; Slug = 'mp-coopspirit-updateattractiontomaster' },
    [pscustomobject]@{ Address = '0x0066FF20'; Slug = 'mp-coopspirit-swaptohero' },
    [pscustomobject]@{ Address = '0x00670710'; Slug = 'mp-coopspirit-updatescore' },
    [pscustomobject]@{ Address = '0x0062C0E0'; Slug = 'mp-world-eamovespirit' }
)

# Prototype-closure seed: quick ABI/accessor wins first, then moderate bodies.
$seedTargets = @() + $coopTargets + @(
    [pscustomobject]@{ Address = '0x00A66550'; Slug = 'fixedallocator-getfragmentation' },
    [pscustomobject]@{ Address = '0x00BDC130'; Slug = 'landscapemap-relocatedata' },
    [pscustomobject]@{ Address = '0x00B6CA10'; Slug = 'landscaperenderer-peekscenefilterflags' },
    [pscustomobject]@{ Address = '0x00A65DE0'; Slug = 'fixedallocator-getallocationsizeincludingheader' },
    [pscustomobject]@{ Address = '0x00BF3840'; Slug = 'landscapepatch-canrelocatedata' },
    [pscustomobject]@{ Address = '0x00A65E50'; Slug = 'fixedallocator-getnoallocatedareas' },
    [pscustomobject]@{ Address = '0x00A65D70'; Slug = 'fixedallocator-getheaderoverhead' },
    [pscustomobject]@{ Address = '0x00A65E20'; Slug = 'fixedallocator-getnofreeareas' },
    [pscustomobject]@{ Address = '0x00A65DF0'; Slug = 'fixedallocator-getlargestfreeareasize' },
    [pscustomobject]@{ Address = '0x00A65DA0'; Slug = 'fixedallocator-gettotalmemoryallocated' },
    [pscustomobject]@{ Address = '0x00B676A0'; Slug = 'landscaperenderer-releasedefaultpool' },
    [pscustomobject]@{ Address = '0x00BF3980'; Slug = 'landscapepatch-relocatedata' },
    [pscustomobject]@{ Address = '0x00A65FF0'; Slug = 'fixedallocator-constructor' },
    [pscustomobject]@{ Address = '0x00AB5D00'; Slug = 'mousedx-constructor' },
    [pscustomobject]@{ Address = '0x00CAF680'; Slug = 'landscapepatch-allocatetask' },
    [pscustomobject]@{ Address = '0x00CAF580'; Slug = 'landscapepatch-destructor' },
    # Landscape-paint closure: LEV theme slots/blends through the world/map/
    # scripted-brush layers. These are curated even though their prototypes are
    # complete because their bodies define the visual paint/bake contract.
    [pscustomobject]@{ Address = '0x0063A970'; Slug = 'worldmap-setthemeatblock-a' },
    [pscustomobject]@{ Address = '0x0063A9E0'; Slug = 'worldmap-setthemeblendatblock' },
    [pscustomobject]@{ Address = '0x0063AA50'; Slug = 'worldmap-getthemeatblock' },
    [pscustomobject]@{ Address = '0x0063AAB0'; Slug = 'worldmap-getthemeblendatblock' },
    [pscustomobject]@{ Address = '0x0081EE60'; Slug = 'map-setenginethemeat' },
    [pscustomobject]@{ Address = '0x0081EFC0'; Slug = 'map-setengineblendat' },
    [pscustomobject]@{ Address = '0x0081F090'; Slug = 'map-getenginethemeat' },
    [pscustomobject]@{ Address = '0x0081F170'; Slug = 'map-getengineblendat' },
    [pscustomobject]@{ Address = '0x0082E330'; Slug = 'scriptedbrush-gettheme' },
    [pscustomobject]@{ Address = '0x0082E3D0'; Slug = 'scriptedbrush-getthemeblend' },
    [pscustomobject]@{ Address = '0x0082E420'; Slug = 'scriptedbrush-setthemeblend' },
    # Native-nav closure beyond the terrain-only generator: placed-object line
    # blocking, detailed inputs, stacked layers, transitions, and map-edge links.
    [pscustomobject]@{ Address = '0x0050A650'; Slug = 'worldmap-getnavigationareainit' },
    [pscustomobject]@{ Address = '0x00A76F30'; Slug = 'nav-isareablockedbylines' },
    [pscustomobject]@{ Address = '0x00A781A0'; Slug = 'nav-updatelines' },
    [pscustomobject]@{ Address = '0x00A79B30'; Slug = 'nav-node-initialise' },
    [pscustomobject]@{ Address = '0x00A7A5E0'; Slug = 'nav-initialiselines' },
    [pscustomobject]@{ Address = '0x00A7A8D0'; Slug = 'nav-tree-initialise' },
    [pscustomobject]@{ Address = '0x00A77480'; Slug = 'nav-joinlayers' },
    [pscustomobject]@{ Address = '0x00A77500'; Slug = 'nav-verifytransitions' },
    [pscustomobject]@{ Address = '0x00A7A650'; Slug = 'nav-linkneighbouringmaps' },
    [pscustomobject]@{ Address = '0x00A793D0'; Slug = 'nav-connectverticaledge-a' },
    [pscustomobject]@{ Address = '0x00A79680'; Slug = 'nav-connectverticaledge-b' }
)

# ForgeFSE binding-verification lane. The generated rows are retail candidates,
# never executable hooks. This runner only reconstructs and structurally checks
# the candidate implementation; explicit hook approval remains a later build-ID,
# ABI, owner/callsite, and runtime-probe gate.
$bindingQueuePath = Join-Path $root 'rebuild\backlog\forgefse-binding-queue.tsv'
if (Test-Path -LiteralPath $bindingQueuePath) {
    $seedTargets += @(
        Import-Csv -LiteralPath $bindingQueuePath -Delimiter "`t" |
            Where-Object { $_.hook_approved -ne '1' } |
            ForEach-Object {
                [pscustomobject]@{
                    Address = $_.address
                    Slug = "forgefse-binding-$($_.address.ToLowerInvariant().Replace('0x', ''))"
                    Lane = 'forgefse-binding'
                    ApiNames = $_.api_names
                }
            }
    )
}

# The donor resolver has saturated at two unique signatures, but 40 small
# semantic/accessor bodies still need evidence-backed review. Feed them through
# the reconstruction agent after the capability-focused seeds. Successful rows
# are skipped forever by the shared ledgers; newly triaged rows join later runs.
$qualityTailPath = Join-Path $root 'ghidra_out\naming_stragglers\quality_triage.tsv'
if (Test-Path -LiteralPath $qualityTailPath) {
    $seedTargets += @(
        Import-Csv -LiteralPath $qualityTailPath -Delimiter "`t" |
            Where-Object { $_.disposition -eq 'semantic-review' } |
            ForEach-Object {
                [pscustomobject]@{
                    Address = "0x$($_.address.ToUpperInvariant().Replace('0X', ''))"
                    Slug = "naming-$($_.address.ToLowerInvariant().Replace('0x', ''))"
                }
            }
    )
}

function Write-QueueLog([string]$Message) {
    Add-Content -LiteralPath $queueLog -Value "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] $Message" -Encoding UTF8
}

try {
    & (Join-Path $liftRoot 'scripts\organize_lift.ps1') -MinimumAgeMinutes 30 | Out-Null
} catch {
    Write-QueueLog "WARN transcript housekeeping failed: $($_.Exception.Message)"
}

function Test-ProcessCommand([int]$ProcessId, [string]$Pattern) {
    $process = Get-CimInstance Win32_Process -Filter "ProcessId = $ProcessId" -ErrorAction SilentlyContinue
    return $null -ne $process -and $process.CommandLine -like "*$Pattern*"
}

function Test-Recorded([string]$Address) {
    if (-not (Test-Path -LiteralPath $sessionPath)) { return $false }
    try {
        $session = Get-Content -LiteralPath $sessionPath -Raw | ConvertFrom-Json
        $key = $Address.ToLowerInvariant().Replace('0x', '')
        $property = $session.functions.PSObject.Properties[$key]
        return $null -ne $property -and $property.Value.success -eq $true
    } catch {
        Write-QueueLog "WARN unreadable progress ledger: $($_.Exception.Message)"
        return $false
    }
}

function Get-AllRecordedAddresses {
    $recorded = @{}
    @(
        're-agent-progress.json',
        're-agent-retry-progress.json',
        're-agent-wave2-progress.json',
        're-agent-wave3-progress.json'
    ) | ForEach-Object {
        $ledgerPath = Join-Path $stateRoot $_
        if (Test-Path -LiteralPath $ledgerPath) {
            try {
                $ledger = Get-Content -LiteralPath $ledgerPath -Raw | ConvertFrom-Json
                foreach ($property in $ledger.functions.PSObject.Properties) {
                    if ($property.Value.success -eq $true) {
                        $recorded[$property.Name.ToLowerInvariant().Replace('0x', '')] = $true
                    }
                }
            } catch {
                Write-QueueLog "WARN unreadable ledger $ledgerPath`: $($_.Exception.Message)"
            }
        }
    }

    # The compiled reconstruction lane is a stronger terminal gate than the
    # LLM checker. Do not spend later agent batches rediscovering a candidate
    # that already compiles under VC7.1 and passes its focused behavior oracle.
    # This also lets a manual byte-parity promotion supersede a conservative or
    # mistaken checker FAIL without rewriting the append-only agent ledger.
    $compiledPath = Join-Path $root 'rebuild\compile-gate\vc71-compiled.tsv'
    if (Test-Path -LiteralPath $compiledPath) {
        try {
            Import-Csv -LiteralPath $compiledPath -Delimiter "`t" |
                Where-Object { $_.status -eq 'PASS' -and $_.behavior_test -eq 'PASS' } |
                ForEach-Object {
                    $recorded[$_.address.ToLowerInvariant().Replace('0x', '')] = $true
                }
        } catch {
            Write-QueueLog "WARN unreadable compiled-candidate gate $compiledPath`: $($_.Exception.Message)"
        }
    }
    return $recorded
}

$allRecorded = Get-AllRecordedAddresses
$pendingSeeds = @($seedTargets | Where-Object {
    -not $allRecorded.ContainsKey($_.Address.ToLowerInvariant().Replace('0x', ''))
})
$targets = @($pendingSeeds | Select-Object -First $MaxTargetsPerRun)
if ($targets.Count -eq 0) {
    $backlogPath = Join-Path $root 'rebuild\backlog\functions.tsv'
    if (Test-Path -LiteralPath $backlogPath) {
        $targets = @(
            Import-Csv -LiteralPath $backlogPath -Delimiter "`t" |
                Where-Object {
                    $_.gap -match 'prototype' -and
                    -not $allRecorded.ContainsKey($_.address.ToLowerInvariant().Replace('0x', ''))
                } |
                Select-Object -First $MaxTargetsPerRun |
                ForEach-Object {
                    [pscustomobject]@{
                        Address = "0x$($_.address.ToUpperInvariant().Replace('0X', ''))"
                        Slug = "backlog-$($_.address.ToLowerInvariant().Replace('0x', ''))"
                    }
                }
        )
    }
}
if ($targets.Count -eq 0) {
    $completedAt = Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz'
    Set-Content -LiteralPath $completePath -Value "backlog-exhausted $completedAt" -Encoding UTF8
    Write-QueueLog "COMPLETE reconstruction and ForgeFSE binding queues exhausted at=$completedAt"
    exit 0
}
if ($Preview) {
    $targets | Select-Object Address, Slug, Lane, ApiNames
    exit 0
}
Remove-Item -LiteralPath $completePath -Force -ErrorAction SilentlyContinue
if (Test-Path -LiteralPath $pidPath) {
    try {
        $existingPid = [int](Get-Content -LiteralPath $pidPath -Raw)
        if ($existingPid -ne $PID -and (Test-ProcessCommand $existingPid 'run_re_agent_wave3_queue.ps1')) {
            Write-QueueLog "SKIP already running pid=$existingPid"
            exit 0
        }
    } catch {}
}
if (Test-Path -LiteralPath $refreshPidPath) {
    try {
        $refreshPid = [int](Get-Content -LiteralPath $refreshPidPath -Raw)
        if (Test-ProcessCommand $refreshPid 'run_rebuild_refresh.ps1') {
            $refreshDeadline = (Get-Date).AddMinutes($RefreshWaitMinutes)
            Write-QueueLog "WAIT rebuild refresh owns Ghidra pid=$refreshPid deadline=$($refreshDeadline.ToString('HH:mm:ss'))"
            do {
                Start-Sleep -Seconds 15
            } while ((Get-Date) -lt $refreshDeadline -and
                (Test-ProcessCommand $refreshPid 'run_rebuild_refresh.ps1'))
            if (Test-ProcessCommand $refreshPid 'run_rebuild_refresh.ps1') {
                Write-QueueLog "DEFER rebuild refresh still active pid=$refreshPid after=${RefreshWaitMinutes}m"
                exit 0
            }
            Write-QueueLog "RESUME rebuild refresh released Ghidra pid=$refreshPid"
        }
    } catch {}
}

Set-Content -LiteralPath $pidPath -Value $PID -Encoding ASCII
Write-QueueLog "START wave3 pid=$PID targets=$($targets.Count)"
$providerBlocked = $false
try {
    foreach ($target in $targets) {
        if (Test-Path -LiteralPath $stopPath) {
            Write-QueueLog "STOP marker found at $stopPath"
            exit 0
        }
        if (Test-Recorded $target.Address) {
            Write-QueueLog "SKIP recorded $($target.Address) $($target.Slug)"
            continue
        }
        for ($attempt = 1; $attempt -le $MaxAttemptsPerTarget; ++$attempt) {
            if (Test-Path -LiteralPath $stopPath) {
                Write-QueueLog "STOP marker found before target=$($target.Address) attempt=$attempt"
                exit 0
            }
            $stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
            $runLogRoot = Join-Path $liftRoot "logs\wave3\$(Get-Date -Format 'yyyy-MM-dd')"
            New-Item -ItemType Directory -Path $runLogRoot -Force | Out-Null
            $stdoutPath = Join-Path $runLogRoot "re-agent-wave3-$($target.Slug)-$stamp.stdout.log"
            $stderrPath = Join-Path $runLogRoot "re-agent-wave3-$($target.Slug)-$stamp.stderr.log"
            $lane = if ($target.Lane) { $target.Lane } else { 'reconstruction' }
            $apiNames = if ($target.ApiNames) { $target.ApiNames } else { '' }
            Write-QueueLog "RUN target=$($target.Address) slug=$($target.Slug) lane=$lane api=$apiNames attempt=$attempt"
            $process = Start-Process -FilePath $reAgent -ArgumentList @(
                '--config', $configPath, 'reverse', '--address', $target.Address, '--max-rounds', '4'
            ) -WorkingDirectory $liftRoot -NoNewWindow -PassThru `
              -RedirectStandardOutput $stdoutPath -RedirectStandardError $stderrPath
            $exited = $process.WaitForExit($TargetTimeoutMinutes * 60 * 1000)
            if (-not $exited) {
                Write-QueueLog "TIMEOUT target=$($target.Address) attempt=$attempt minutes=$TargetTimeoutMinutes; terminating process tree"
                & taskkill.exe /PID $process.Id /T /F 2>&1 | Out-Null
                $null = $process.WaitForExit(5000)
                continue
            }
            Write-QueueLog "EXIT target=$($target.Address) attempt=$attempt code=$($process.ExitCode)"
            if (Test-Recorded $target.Address) { break }
            if (Select-String -LiteralPath $stderrPath -Pattern 'usage limit' -Quiet) {
                Write-QueueLog 'BLOCKED Codex usage limit; scheduled retry will resume'
                $providerBlocked = $true
                break
            }
        }
        if ($providerBlocked) { exit 0 }
    }
    $pending = @($targets | Where-Object { -not (Test-Recorded $_.Address) })
    if ($pending.Count -eq 0) {
        $completedAt = Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz'
        Write-QueueLog "BATCH COMPLETE targets=$($targets.Count) at=$completedAt; next schedule will refill"
    } else {
        Write-QueueLog "INCOMPLETE pending=$($pending.Count); scheduled retry will resume"
    }
} catch {
    Write-QueueLog "FATAL $($_.Exception.ToString())"
    throw
} finally {
    Remove-Item -LiteralPath $pidPath -Force -ErrorAction SilentlyContinue
    Write-QueueLog "END wave3 pid=$PID"
}
