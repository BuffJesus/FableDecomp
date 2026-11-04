# Texture runtime follow-up, 2026-10-21

User resumed after PNG/DDS and AI stress comparisons. Focus: runtime mip requests/residency, keeping mesh LOD, shadow/bump shading, and file encoding separate. No game launched in this continuation yet.

## Read current evidence before new claims

`NEAR_RANGE_POPIN_2026-10-19.md` records later static analysis: Oakvale rock bump flags occur at LOD0, and the static shader selection depends on shadow receiver volume. These are code mechanisms, not proof of the cause of each reported visual transition. The texture checkout's `docs/TEXTURES_NEXT_SESSION_20261018.md` begins with a later video review superseding its older unreviewed-video instructions. Recording `C:/Users/Cornelio/Videos/Radeon ReLive/unknown/unknown_2026.10.19-20.40.mp4` was reviewed previously; 88.4s, AV1, 1024x768/60Hz. That run showed no clear rock mip/silhouette pop, without proving a fix. Reuse review evidence in `build/texture-user-video-review-20261019`; do not repeat broad video scans or infer native1440p from the filename of unrelated fixtures.

Additional October19 visual trials exist beyond the old texture-transitions journal. Six completed baseline/LOD0/shadow4096/range64 reports have normal exit0 and restoration receipts, but no new near-range visual efficacy assertion was made here. Viewed baseline-b and range64 wide images: shadow coverage differs; single stills and moving NPCs do not establish a streaming fix.

## Interrupted October19 scratch trial recovered

Shared `.agents/runtime-lease.json` still held token `9e73c0f6ad054076b0d9929f5b1a8c2f`, owner Texture Archive4GB, dead PID13664, trial `build/visual-quality-d-candidate-20261019a`. No final report and only initial capture. Read-only audit verified no owner/game/helper process, all original backup hashes, exact staged candidate/definition/DLL identities, and 563 original saves unchanged. Four extra `0aa` profile files exist. Twelve tracked scratch paths were still staged.

Added narrow `tools/ingame/recover_interrupted_texture_trial.py` in texture checkout. Requires pinned intake audit, same lease token/trial, absent original PID/game, original scratch EXE, exact current/backup hashes and unchanged original saves. Preserves all displaced files, restores only tracked private scratch paths, never writes saves, releases only after double verification. Preflight matching/refusal/path-escape controls passed. Executed recovery with audit SHA256 `a8a96dfbe0db2fcf919ba86ad483cf2cea13fef2d8775e96ef13622e7b20a44c`.

Receipt: `build/visual-quality-d-candidate-20261019a/interrupted-recovery/verification.json`: twelve scratch files verified, 563 originals and all567 save files unchanged, zero save writes, displaced files retained. Lease release `released-runtime-lease-interrupted-recovery.json`. No successful runtime outcome inferred for interrupted trial. Read-only audit at `build/texture-runtime-intake-20261021/audit.json`.

## Prepared live comparison

Fresh commands in texture `build/texture-streaming-plan-20261021/`:

- `control-command.json`: ordinary corrected AI bank, existing 2K patch, stock streaming, same pinned profile,150s, fresh trial `texture-streaming-control-20261021a`.
- `request-pool-command.json`: same bank/profile, request200%, stock retention3/30, nonzero HiresTextureMemory floor16384, fresh trial `texture-streaming-request-pool-20261021a`.

Both actual wrapper dry runs exit0, all fixture pins/baselines accepted, desktop blockers empty and no shared lease after recovery. They have not launched. These change request timing and pool budget together and can establish only a combined treatment effect; isolate each if useful. They do not change mesh LOD/draw distance/LAA. Expected comparison uses existing residency probe plus identical bounded W/S route after verified gameplay. Atomically claimed lease and fresh backups remain mandatory.

Scoped probe/analyzer improvements: Windows private/pagefile/RSS/VMS sampling (optional available psutil fields); normalized pool observed ranges keyed by resource-offset/index; private-byte peak; transition records include bank identity. Synthetic coherent-promotion/memory/pool-counter checks and Python compilation pass. Existing historical reports not overwritten. CPU residency/counts are not GPU sampling, frame times, confirmed exhaustion, or a visual quality score.

User desktop availability was asked asynchronously. User subsequently explicitly authorized launches: "You can launch if needed." That authorization persists; no further launch permission needed within these scratch trials. Results below supersede the pending checkpoint. Original public release/source-review branch and local quality candidate unchanged.

## Three matched live trials completed

The first stock attempt `texture-streaming-control-20261021a` stopped before launch because the recovered existing0aa profile remained present. Wrapper restored its staged scratch files, verified567saves and released its lease. It is not a gameplay result.

Added explicit `--park-existing-comparison-profile` to the existing wrapper. Default still refuses existing0aa. Opt-in requires pinned profile fixture; validates original profile against the fresh complete save snapshot and its backup, moves it into the trial evidence directory, stages the known comparison profile, and relies on exact snapshot restoration afterward. Existing profile and trial-generated saves are preserved in evidence. Permanent `tools/test_texture_profile_preservation.py`:5 private-fixture tests pass (round trip/unrelated files, changed-source refusal, changed-backup refusal, destination collision, overlapping destination). Python compile and scoped whitespace checks pass.

Stock retry `texture-streaming-control-20261021b`, combined experiment `texture-streaming-request-pool-20261021a`, and definitions-only pool experiment `texture-streaming-pool-only-20261021a` all PASS driver, capture helper,17-step W/S route and residency sampler. Each has140samples over70s, approximately49s route,2560x1440 captures, graceful exit0, tracked scratch restoration and all567 preexisting saves hash-verified. Each original0aa was preserved/restored. Final independent cross-trial readback matched all snapshot save hashes and every tracked scratch baseline hash. No Fable process or shared lease remains.

All use the same corrected bank SHA733e4af... and pinned profile. Mesh LOD error2 and draw factor1 remain unchanged. Stock delays3/30 remain; native temporary1/1 overrides are observed in stock/pool-only and preserved. Pool-only fixture references only the same two definitions used by combined experiment; no probe DLL/config is installed in pool-only. Pool floor16384 is the stock Highest preset value applied to nonzero lower presets, not a measured VRAM budget. Actual relevant preallocation pool count rises35->95; both size descriptors remain11184810bytes under the upstream2K construction patch. Do not call `maxSize` a slot count: it is a byte-size descriptor here.

| Treatment | Request scale | Pool entries | Nonzero failure snapshots /140 | Route sampled jumps >=2 mips | Coherent route transitions | Peak private MiB |
| --- | --- | --- | --- | --- | --- | --- |
| Stock |1x|35|7|2|70|301.84375|
| Pool only |1x|95|0|0|26|328.37109375|
| Requests + pool |2x|95|0|0|31|354.83203125|

One stock failure snapshot falls inside route time; the other six fall in the later stationary observation window. Stock maximum sampled FailedAllocations value3. These counters reset; seven snapshots are not seven unique failure events. Native CTextureManager::AllocateTexture atA69B70 increments FailedAllocations after either exhausted preallocation or texture creation failure. These runs support pool capacity as a contributor; they do not distinguish every lower allocation failure reason or prove process/GPU exhaustion.

Tracked ordinary entries3223 (OV_FIREPLACE) and3864 (SILVERBIRCHTRUNK_24), matching the pinned disk bank's native-ID contract, provide concrete residency witnesses. Stock fireplace drops1024->256 at route+12.70s, returns1024 at+13.20s; stock birch is256 at initial sample and becomes1024 at+33.73s. Both stay1024 in every observed sample in both larger-pool treatments. CPU allocation observations do not prove these objects were visible or prove which GPU mip was sampled. No global pop-in removal, frame-time, long-duration stability or rock-shadow/LOD fix claim.

Supervision/commands/receipts: `build/texture-streaming-plan-20261021/`; each trial's `trial-report.json`, `released-runtime-lease.json`, `launch/launch-report.json`, `residency/{samples.jsonl,summary.json,analysis.json}`, `route/route.json`. `compare_trials.py` independently verifies restorations and identical bank/recipe, emits `comparison.json`. First control runner uses a legacy log directory; later runners are trial-name-scoped. No active process remains.

User artifacts: Desktop `FableTextures Testing 2026-10-21/Streaming comparisons/Streaming-report.html` is self-contained interactive residency table/timeline, with source measurements and limitations. `Verification.json` includes all three trial paths/counters. Raw route06 frames retained for each case; `Route-comparison.png` shows equally reduced stills. Stills reviewed: comparable street/cottage route but small pose/timing differences, no dramatic visual improvement asserted. No external hosting, Nexus upload, source-review update, public DLL/app replacement, or installed Steam game change. The local PNG-quality ZIP remains the prior unmodified candidate.

Next evidence requirement before promoting a user-facing memory option: a wider repeatable route covering reported rock/foliage transitions, repeated runs/frame-time/GPU measurements as warranted, mod-preserving definition edits rather than shipping a replacement game.bin, and clear separation from mesh/shadow behavior. Current bounded result favors the pool-only setting over extra request scale for this route; request-only and comprehensive release acceptance remain untested.

## Bedtime stop requested

User explicitly stopped work for tonight and requested documentation for the next session. No more launches/imports/builds/AI/publication/cleanup or queued jobs. Main resume entry is now the first section of texture checkout `docs/TEXTURES_NEXT_SESSION_20261018.md`, linked from `docs/HANDOFF_TEXTURE_PROJECT_2026-10-14.md`. Includes results, limitations, exact artifacts, safe profile preservation, original source-review branch and local codec status. Final read-only state/source pins are recorded in `build/texture-session-handoff-20261021-bedtime/state.json`.

Next experiments should counterbalance treatment order or repeat a stock control to check OS cache, timing and actor variation before attributing a repeatable improvement. Three single runs support the bounded pool finding, not global visual efficacy. All source/evidence/private snapshots and public review artifacts remain protected. No artifacts released for cleanup. Resume only on the next user work instruction.


## Resumed 2026-10-22: repetition and harness diagnostics

User requested continuing FableTextures until asked to stop. Active texture checkout unchanged; all saved source pins matched before work. Runtime ownership/STOP/desktop checks clear. New evidence directory `build/texture-streaming-repeat-20261022` in that checkout.

Fresh stock repeat `texture-streaming-repeat-stock-a-20261022` completed: driver/helper/route/sampler exit0,140 samples,35 pool entries,15 nonzero failure snapshots (9 moving),69 ordinary coherent route transitions,1 promotion >=2mips and0 demotions >=2mips, peak private301.0625MiB. Scratch restoration and567 original save restorations verified by wrapper. Independent matched report `stock-repeat-comparison.json` accepts October21 and October22 stock pins/renderer/recipe/sample count/1440p dimensions. This demonstrates variation versus the original stock7/1 snapshot counts; no unique event or visual efficacy claim.

Three pool attempts did not reach accepted gameplay: pool-a no visible window; pool-a2 timed-out capture at title left truncated direct-write PNG; pool-b complete captures but title unresponsive after three verified owned click calls. All wrappers report scratch/saves restored; no residency comparison inferred. Bounded title diagnostic trial is in progress at this checkpoint, not a completed experiment.

New reusable comparison tool checks sampler/capture/released lease identity, run/restoration success, sample continuity/coverage and matched bank/frontend/executable/size-data/size-DLL/profile/renderer/route/count/resolution. Reports large promotions and demotions separately, grouped ranges by observed pool entry count, and retains treatment fixture hashes. Eight focused tests pass. Prior report's jumps>=2 label was promotions only; original stock also had one large demotion, pool-only zero.

Captured PNGs now publish atomically through `tools/ingame/capture_file.py`; gamewin uses temporary PNG encoding plus replace. Three tests pass including forced encoder termination after partial output (old published image preserved, interrupted temp retained). This fixes partial publication, not the underlying timeout. Existing startup observer gains opt-in visible-window bounded stacks. No C++ app/package/source-review/public artifacts changed. Source/dirty work preserved.

Independent reader rechecked all14764 definition entries against frozen baseline: only Normal HiresTextureMemory6144->16384 and High10240->16384 changed; all other payload/identity bytes and names.bin identical. Receipt `pool-definition-recheck.json`. Main texture resume doc updated with current continuation and limitations.


### Runtime continuation finalized; encoder work active

Accepted pool-c completed with140samples,95entries,0nonzero failure snapshots,0large promotions/demotions,24ordinary route transitions,330.1484375MiB peak private. Fresh stock-a has15/9failure snapshots,1/0large promotions/demotions,69transitions,301.0625MiB. Combined accepted-pair receipt `completed-comparison.json` and Desktop `FableTextures Testing 2026-10-22/Repeated streaming comparisons/Streaming-repeat-report.html` include timed ordinary-entry witnesses and explicit limitations. Counterbalanced repetitions remain incomplete: pool-d again failed title acknowledgement. Five unsuccessful startup/diagnostic attempts retained; no further blind launch repeats.

Visible-window diagnostic attached/detached CDB successfully; main thread sampled running game code, not proof of an allocator deadlock. Diagnostic reached profile selection then failed menu-glyph validation, exited0 with helper1, so is not gameplay proof. Inventory exposed full-size D3DProxyWindow sharing PID. gamewin now filters exact native title/class and requires main foreground HWND;2identity tests pass. This repairs an ownership-selection gap but did not eliminate intermittent ignored title clicks (pool-d still failed). Comparison tool now also requires helper exit0 and returns per-entry timed witness observations;9tests pass.

Independent final readback verified all567current save hashes and every current tracked scratch baseline for all7trials; runtime lease absent and no game process. `final-restoration-readback.json`, also copied to Desktop report folder. Canonical codec and old private quality ZIP matched their original pins at this readback. No installed Steam writes/publication.

Encoder investigation moved offline: scratch convergence detection stops a quantizer fit when its endpoint pair repeats, since subsequent indices/refinement recur.204fullDDS cases (40real PNGs+28synthetic inputs, eachDXT1/3/5) byte-identical to frozen quality candidate. Six alternating2048face timing pairs: reference median0.5605267s, scratch0.46923705s (~16.3%less); not whole-import timing. Source/scratch code text verified equal after narrowly promoting the loop optimization to canonical texturewrite.cpp. Preserved yesterday's app-source/build/package; new isolated app-source copied and censused with only codec differing. App build/regression/standalone import validation underway in `build/texture-codec-convergence-20261022`. No repackaging/publication yet.


### Offline definition authoring preservation, 2026-10-22

Hardened texture checkout tools/prepare_texture_distance_trial.py without exposing an app memory setting. A read-only four-preset plan requires exactly one correctly typed preset each, one complete signed HiresTextureMemory field, nonnegative budgets, and integer floor0..16384. It keeps zero disabled and preserves custom budgets above the floor. Active lease/source-hash drift refuses fixture construction; only explicitly requested fields are visited, and byte/name/source preservation uses explicit runtime checks rather than removable assert statements. Existing default draw scale remains1.5, so memory-only recipes explicitly pass --scale1 --mesh-distance-scale1.

Seven focused tests pass normally and under python-O (stock selection, custom higher/zero preservation, noop floor, invalid floor, missing/duplicate/wrong-class presets, negative budget, malformed fields). Fresh original-bank memory fixture matches the prior pool-only game.bin hashded27f... and namesac5c... exactly, verifying14764entries. Independent modified fixture includes Normal24576,Highest65536,High8192,Low0 and unrelated Normal draw multiplier1.25; only High changes to16384. All14764entry identities/payloads, names and source hashes verified; active fake lease refused before output creation. Selected-fields final output byte-identical to the earlier verified modified fixture.

Evidence: build/texture-memory-preservation-20261022/{verification.json,selected-fields-verification.json}, build/texture-memory-floor-offline-20261022, build/texture-memory-modded-floor-selected-20261022. No installation write/new game/publication. Counterbalanced runtime repeat and wider rock/foliage route remain incomplete; helper main-window filtering closes a gap but does not explain all ignored title clicks. Asked user asynchronously about mouse/keyboard use during trials to clarify that remaining input uncertainty, without requesting launch permission again.


The final fixture-builder selection change also passes full byte regressions against both October19 draw-only and draw+mesh output banks/names (distance-regressions.json). User answered that the desktop was idle during all failed trials, ruling out competing human input. Added optional per-action JSONL state tracing to gamewin (cursor/clip rectangle/exact foreground HWND/left-button down state). Button-up remains guaranteed even if trace logging fails. Fresh bounded stock-budget title-only diagnostic is running under build/texture-title-input-plan-20261022, with trial build/texture-title-input-diagnostic-20261022: one click only after fresh title-glyph verification, no profile fixture or save navigation, existing guardian handles normal close/restoration. Prior final-restoration-readback covers only the earlier7trials, not this active diagnostic. No new streaming result inferred.


### Title input/capture diagnostics completed

First stock-budget title-only diagnostic passed: one click after freshly verified title, traced exact main foreground HWND and cursor512/400, button observed up->down->up, resulting profile layout verified. No save navigation/profile fixture. Second diagnostic required both GDI and Desktop Duplication title verification before clicking; ffmpeg ddagrab produced no frame within15s and observer refused before click. Thus stale-GDI hypothesis remains untested, not established. Existing visual route documents stale GDI capture in another context; default capture backend was not switched. gamewin optional ddagrab branch is bounded and refuses failed/incorrect-viewport output; capture publication remains atomic and foreground checked.

Both games exited normally0 and wrappers restored scratch paths/all567saves unchanged. Independent current readback in build/texture-title-capture-plan-20261022/restoration-readback.json verifies both runs, no lease/game, unchanged dev2/dev3 ZIP pins. User confirmed desktop idle. Retain diagnostics; no streaming/performance result or cause inferred. Work returned offline.

Offline capture control subsequently reproduced a Desktop Duplication timeout without a game: ffmpeg8.1 opens DXGI output0 at2560x1440 but the64x64PNG capture produces no frame within10s. GDI control through the same ffmpeg/PNG pipe returns0 and a valid1652byte PNG in0.81s. Only the top-left64x64region was requested; image bytes were not published or retained. No input/window manipulation. This shows the DDA timeout can occur outside the game, not its root cause or the cause of ignored clicks; retain default GDI and do not infer stale-frame confirmation. Receipts in texture checkout `build/texture-lzo-deduplicate-20261022/{desktop-capture-probe.json,desktop-gdi-control.json}`.

One targeted pool-title trace followed completed offline imports: fresh `build/texture-pool-title-trace-plan-20261022`/trial `texture-pool-title-trace-20261022`, same pinned bank and pool-only definitions, no profile fixture/save navigation. Observer would send exactly one freshly verified title click and sample foreground/cursor/button at50ms. Dry-run pins/desktop blockers and peer lease/game/STOP checks passed. Native main window never appeared within50s; no title observed, click or sampled input trace collected. Guardian terminated the owned windowless game (not a normal game exit); both helper/driver exit1. Scratch restoration and all567save hashes pass independent readback; lease/game absent. No new streaming, input-delivery or cause evidence. Retain failed trial; no repeated blind launch. Returned offline.

### Read-only session and display-state evidence

User is at work and unsure whether the monitor was asleep/off during failed trials. Keep historical display state unknown; no need to ask them to watch it. Read-only WTS/InputDesktop probes now report session unlocked, Default desktop receiving input. Active AC display timeout900s/DC180s, no setting changes. Native PowerSettingRegisterNotification initial session-display callback reports ON. An identical ffmpeg DDA64x64control then succeeds in0.2089s with full PNG verification, OS display ON before/after. Earlier timed-out captures lacked those state records; this supports collecting the missing variable, not assigning cause or claiming physical monitor visibility. No game/input in these controls.

New texture-checkout `tools/ingame/desktop_session_state.py`: read-only WTS/session, input/thread desktop, idle counter and session-display state; no WTS user/domain identity in output. Owned desktop/notification handles and WTS buffers cleaned. Native Win64 WTS header/ctypes size/offsets match. Registration callback reads initial state only; no PowerSet, wake, unlock, mouse or keyboard action. Child-process2s timeout protects guardian progress from stalled native queries. Existing archive wrapper records before trial/launch, launch guardian records at cleanup, optional input trace at ready. No automatic launch/input policy or GDI-default change. Eleven tests pass (API failure/truncation, identity redaction, ownership, Windows7 flag inversion, bounded service probe, invalid replies and display mapping/unregistration); actual native/Python queries agree. Instrumentation has not been exercised in a new live trial.

Evidence `build/texture-desktop-state-evidence-20261022/verification.json` plus preserved failed DDA/GDI/verified-ON DDA receipts and powercfg output. API contracts: [WTS lock/session fields](https://learn.microsoft.com/en-us/windows/win32/api/wtsapi32/ns-wtsapi32-wtsinfoex_level1_w), [initial power notification](https://learn.microsoft.com/en-us/windows/win32/api/powersetting/nf-powersetting-powersettingregisternotification), [session display values](https://learn.microsoft.com/en-us/windows/win32/power/power-setting-guids). No new app package/publication; dev4 remains current. Current game/lease absent and567saves verified exact after the failed title trial.

### Known-ON control and startup observer

`texture-pool-display-control-20261022` required reported ON/unlocked/input desktop before launch and every intended observer action. It again failed main-window wait50s before any observer action; cleanup also records ON/unlocked. Thus display-off cannot explain every windowless startup. No click or independent title frames in that trial. Guardian terminated owned game, both helpers exit1; independent scratch/567save readback exact, lease/game absent. Not a streaming or root-cause result.

Startup helper previously treated any full-size visible same-process window as a main window, so D3DProxyWindow could suppress missing-main stack capture. It now uses shared exact title/class predicate, retaining separate any-large/main-large receipt fields. Two identity tests pass. Optional observation duration1..25s (default25) permits10s observation before existing bounded25s CDB capture inside unchanged50s guardian window bound. Initial proposed90s guardian wait was refused in dry-run; preserved refusal, retained guard and corrected50s. No staging on refusal.

Fresh `texture-pool-startup-stack-20261022` under reported ON/unlocked then produced a valid native main window; observer correctly skipped CDB. Guardian verified title after3Escape logo skips, held45s, closed normally0; both helpers exit0. Read-only GDI and DDA captures succeeded1024x768 and both white/outline prompt mismatches are0.0. No observer mouse/keyboard or menu navigation. This verifies current in-game DDA capture capability, not stale-GDI history or ignored-click cause. Normal startup and windowless failure both occur under the pinned pool fixture and recorded ON conditions; do not assign cause to pool budget or display power from this pair.

Independent `texture-pool-startup-stack-plan-20261022/restoration-readback.json` verifies all scratch baseline paths,567save hashes, normal exit and no remaining game/lease. Capture pair in trial `startup-diagnostic/capture-pair.json`. Initial supervisor field desktopInputSent=false ambiguously described observer only; original preserved as supervisor-result-original.json and corrected receipt explicitly records observerDesktopInputSent=false, guardian3Escape intro skips and menuInputSent=false. User-facing wording corrected. Existing observer/capture receipts remain accurately scoped. No app/public package change.

Combined native-window/dual-capture click observer in `texture-pool-adaptive-title-20261022` reached native main/title without debugger; guardian4Escape logo skips and normal exit0. Click observer refused before its first capture because the bounded desktop-state child timed out2s. No observer input, no dual-capture click comparison. Unknown state is not lock/display-off. Independent readback verifies scratch/567saves and no game/lease. Follow-up session/display query completes0.088s; its game liveness was not recorded, so the original timeout remains unexplained and this is not a validated under-game timing. Original timing artifact name says during-game but that circumstance is unverified.

Added best-effort phase markers to isolated desktop-state child and lastProbePhase on timeout/nonzero replies, retaining2s bound. Output errors cannot interrupt native notification cleanup. Eleven focused tests pass. Separate real-process failure probe deliberately sleeps after a phase marker; bounded parent kills/reaps only that owned Python child in2.010s and retains display-register marker. No Windows service stalled/altered, game/input/settings action. Evidence `build/texture-desktop-state-evidence-20261022/real-timeout-cleanup.json`. One further combined attempt prepared in fresh texture-pool-adaptive-title2 plan/trial to collect a traced click or a more specific refusal; no repetition queue.

Title2 combined trial succeeds: native main appeared, no debugger attempted, both fresh GDI/DDA title checks passed, one traced click, both after captures verify profile selection. Cursor512/400 before/down/up; button observed up/down/up, hold trace1.508s.147foreground samples all native main HWND, max sample gap0.0514s; samples do not prove continuous state between samples. Guardian3Escape logo skips, normal game exit0; both helpers0. No save navigation. Independent scratch baseline/567save hashes exact, lease/game absent. Receipt `build/texture-pool-adaptive-title2-plan-20261022/restoration-readback.json`; copy on Desktop as Dual-capture-title-click-and-restoration.json. This establishes a working independent capture/input path, not the historical intermittent cause or streaming improvement.

Next actual dev4 output validation prepared: `texture-dev4-native-bank-plan-20261022` / `texture-dev4-native-bank-20261022`, independently verified580-image bank811772265bytes/SHA844b890b..., same preserved frontend26e655... and DAT81dd81... . Stock memory definitions, existing pinned test-save/menu helper and safe0aa parking,150s hold, normal-close/restoration guardian. Fresh pins/ON-unlocked/input-desktop dry-run pass. This is a native loading/rendering check of packaged app output, not comparison against prior expanded streaming bank. Sparse580benchmark is not a finished AI campaign; full-campaign preparer is not used/relaxed and no pending manifest fields falsified. Existing generic explicit-bank leased wrapper is appropriate. Results pending.

## October22 display OFF control and bounded request

Display control update: the naturally idle, unlocked Default desktop reports
OFF. The same64x64 DDA pipeline that succeeded in0.21s under recorded ON timed
out10s under recorded OFF (`texture-lzo-deduplicate-20261022/capture-display-off-control.json`).
This association does not identify all historical startup/input failures; a
missing native window also occurred under recorded ON.

Added optional guardian `--keep-display-awake`: thread-scoped execution request
inside the exclusive lease, known-unlocked/input preflight, bounded ON check
before staging and again before launch, prior thread flags restored in finally
even if restoration raises. No input, unlock, or power-plan changes. Five focused
fake-API tests pass. Actual OFF control: Windows accepted request, stayed OFF,
request restored to0x80000000, AC900/DC180second settings unchanged. Capability
is NOT a proven wake path. No game/capture launched in this control. Guardian
integration refused before staging, request released, all567save hashes and
scratch hashes exact, no game/lease (`texture-display-guardian-refusal-20261022/
independent-verification.json`). Do not retry equivalent OFF game launches or
alter system preferences; continue offline checks until known ON is available.

API reference: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setthreadexecutionstate

Scene-witness audit completed using the already accepted matched historical pair
(validated through compare_texture_streaming_trials; no runtime repeat).32edited
IDs from the sparse580fixture appear resident in each historical run. Young Hero
IDs792face/793hair/794torso and3223fireplace are candidates for dev4 sampling.
Historical expanded-bank face: stock256? mip3 in138/140snapshots, pool2048? mip0
in139/140. Torso stock1024? mip1 in139/140, pool2048? mip0 in140/140. Remaining
snapshots are unobserved, not asserted failures. Current dev4 has2048face/torso,
ten levels each;512hair/eight levels. Exclude old3864birch witness because not
edited in sparse580. No dev4 residency or visible-quality claim. Source hashes,
all32ID distributions in texture-dev4-all-mips-20261022/scene-witness-selection.json;
Desktop Repeated streaming comparisons/Texture-residency-witnesses.html.

## October22 afternoon actual dev4 bank residency

Instrumented pool2 completed: guardian/helper/sampler/passive-observer all0.
60samples/29.55s;95pool entries,0nonzero failure snapshots. Face7922048x2048mip0
all60, torso7942048x2048mip0 in59(1unobserved), hair793512mip0 all60,
fireplace32231024mip0 all60. Actual bank844b890b... unchanged; bank/profile/exe/
sidecar/renderer andpose recipe match accepted stock run. Source-bank/scratch
andall567saves independently exact, stock definitions after restoration, no game/
lease after trial. Display request released. Native2K/size hooks verified.
New portrait/wide inspected, no obvious missing/blank textures or major sizing
fault; blink differs. Samples follow captures, so do not infer portrait GPU mip.
One matched stationary pair, not route/performance/global acceptance.

Guardian optional --trace-title-input is supported in both launcher/leased
allowlist; default unchanged. Successful pool2 first click: cursor512/400,
exact main foreground all6trace events and30passive hold samples, button up/
down/up,1.516s hold, maxgap0.05095s. Observer2279samples, no input/focus calls.
No root cause inferred for earlier ignored clicks. Source pins and input receipt
in texture-dev4-residency-pool2-plan-20261022. Desktop new Dev4 residency comparison/
Compare.html uses raw original frames with fit/native-size slider, JSON and
independent restoration receipts. Dev4/public binary and source ZIPs unchanged.

Active old-bank route completion: texture-streaming-completion-20261022/run_trial.py
pool-e; trial texture-streaming-repeat-pool-e-20261022. Both pool-e/stock-b dry-run
pins pass. Same old733e4a...bank, fixture37a014..., profile367ce8...,70s sample+
original route recipe/150s guardian hold. Optional guardian title traces added;
no display power request in these route repeats, matching the original defaults.
Read-only ON/unlocked/idle preflight. Guardian alone owns closure/restoration.
After accepted pool-e and independent readback, stock-b is next. Do not launch a
peer runtime or call four-control/counterbalanced completion before accepted.


## Interrupted input worker cleanup, October22 afternoon

Pool-e timed out its third title click worker. Trace ended before button-up; Windows reported button still down after game closure. Sent only an owned button-up/no focus/no movement, verified false. Independent scratch/source/567save readback passes. This concrete timeout gap does not explain all historical ignored clicks.

Implemented leased input journal (input_gestures.py) in gamewin mouse/key
presses. Original held inputs refuse a new press. Atomic intent before down;
released marker only after native up returns. Archive guardian supplies context
to children; standalone route helper explicitly passes the validated lease
context too. After game closure/before lease release, bounded8s recovery child
validates lease/path, retires surviving exact worker identity (PID+creation+
script+game arg), checks unlocked input desktop before state query, emits only
missing up events. Failure retains lease and records error. No default input
backend switch; mouse_event remains. Seven focused tests pass including actual
terminated Python child intent (fake input callbacks, no native input).

Windows control texture-input-recovery-control-20261022 passed: exited worker
published a pending mouse intent, native state already UP, recovery marked it
recovered without sending any input; correct lease released. Real stuck-button
up itself was separately verified in interrupted-button-cleanup.json. No claim
that new journal has exercised a real held-input recovery automatically yet.

Active protected integration: texture-streaming-completion-20261022/run_trial.py
pool-f; trial texture-streaming-repeat-pool-f-20261022. Same old bank/fixture/
profile/route70ssampler/150sguardian. Dry-run pins pass. Source hashes saved in
input-recovery-source-pins.json. This exercises worker-journal cleanup and may
also produce second accepted pool route. If no gameplay, no streaming result.
Guardian owns game/scratch/save/input cleanup. Stock-b remains unlaunched.
App binary/source packages and public snapshot unchanged.


Windows API references: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mouse_event and https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput . Modern API alternative inspected, but no unverified backend change made.

## Completed afternoon milestone

Latest completed milestone (October22 afternoon): no active game/job/lease.
Four old-bank route controls accepted in stock/pool/pool/stock order,140samples
each, same data/renderer/pose/resolution pins. Stock failure snapshots15/11,
pool0/0; during-route9/11vs0/0. Stock >=2mip promotions1/1, pool0/0; demotions0
all. Peak private stock301.06/303.80MiB, pool330.15/331.95MiB. Failed trials
intervened; later runs add title tracing/input journals. No GPU/frame-time/
global visual-fix claim. Four-control comparison complete; stop repeating this
unchanged short-route gate. Wider rock/foliage route and GPU/frame-time evidence
remain open before global promotion or a user-facing memory option.

New private Desktop artifacts: Repeated streaming comparisons/Four-control-
report.html (original two-control report now links to it); Dev4 residency
comparison/Compare.html (different actual580-image bank, stationary pair;
not merged into old-bank route stats); Input-worker-recovery.json.
Latest independent old-bank readbacks: texture-streaming-completion-20261022/
{pool-f,stock-b}-restoration-readback.json. Both25input journals allreleased,
no pending input or recovery needed on normal paths. All567saves/source/scratch
hashes exact after each, three staged autosaves changed and restored.

Real input-fault recovery PROVEN end-to-end: texture-input-interruption-plan-
20261022/verification.json. Fresh title only, original baseline bank, no save
navigation. Observer started owned2s click, confirmed real native LMB down,
terminated only that worker before its up. Pending intent/held state confirmed.
Guardian closed game normally0, restored source/scratch/567saves, bounded child
recovered exact one pending mouse record (beforeDown=true,afterDown=false),
marked recovered, released lease. Native global button-up independently checked.
Four journal records total (three completed intro ESCs plus interrupted click).
No new down/focus call from recovery. Seven focused tests and native no-input
control also passed. This fixes worker-kill cleanup; historical title-ack cause
remains unresolved. No API backend switch or app/public package change.


October22 rock-path scout: eight reviewed camera/key steps reach rock lane and
covered bridge far-exit view;916 read-only samples/0errors, owned stop marker.
Native closure access violation0xc0000005, Event1000 matches PID12564 in ntdll
+0x4e56d. Not accepted efficacy/control. Independent cleanup PASS:567 saves,
source/scratch exact,20journal records released, no game/lease, display released.
Candidate timed eight-step replay prepared with pinned inputs/source and
successful dry-run. Started bounded replay under guardian; inspect plan result
and independent readback before further launch. No application/package change.

October22 timed wider pair completed normally, independent567save/source/scratch
restoration exact,20ownedinputsreleased each. Stock143samples/125duringroute,
4failure snapshots/2duringroute,35poolentries. Pool102/85,0/0,95entries.
Same input recipe, source/data/renderer pins. Wall times62.91vs42.66s and final
positions differ, so these are preliminary observations, not accepted efficacy.
Desktop Wider route preliminary comparison/Compare.html records raw evidence.
Canonical comparison helper now settles750ms and retries profile selection once
only under positively matched original Select Profile layout; traces enabled.
Existing selected/continue/gameplay captures reject retry, compile pass, two
subsequent live runs progressed normally on first click. Not proof of old causes.
Fixed15s client-start cadence stock run ACTIVE; plan records actual starts and
fails if >1s late.300s guardianhold/150ssampler or owned marker. No peer game.

October22 WIDER fixed-cadence four-controls complete, stock/pool/pool/stock.
All238samples/221duringroute, source/data/input/renderer pins exact and15sclient
startcadence maintained. Failuresnapshots11/0/0/16; duringroute9/0/0/16.
Allchild exits0, normalclose0,567save/source/scratch hashes exact and20journal
recordsreleased each. Finalstock ends insidebridge; others reachfarrock, so
notidenticalworldpath. Joint mip/allocation readings retained, noGPU/globalfix
claim. Desktop Fixed cadence wider route/Four-control.html +12rawPNGs/JSON/pins.
Do not repeat passed residencycohort unchanged. Source app/release unchanged.
Saved initialscout minidump offlinecdb showsheapfree viaFable+0x59e9d9/FSECRT
onexit/processdetach, partialdump/unwindwarning; heapstateorigin unresolved.
PresentMon2.6.0portable officialassetdigest verified, noinstaller/adminrestart.
OwnPythonPID2spreflight0, ownETWsessionabsentafter; notgameframecoverage.
Gameframepilotstock ACTIVE under exclusiveguardian, exec14636; passivePID/
creation/leasecheckedcollector, uniquenamedETWsession/130stimedexit, inputtracking
disabled. Routewaitscollectorready, same15scadence. NeedCSVcoverage/metricreview,
normalexit/restorationreadback before accepting. No othertrace/game allowed.

NIGHT STOP, explicit user request: all work stopped; no queued background jobs.
Frame-time pilot failed before usable game window/verified gameplay. PresentMon
never started; zero routeinputs/gesturejournals. Excluded, all child exits1.
Guardian retired owned process and restored all567saves/source/scratch hashes;
independent readback confirms no game/lease, displayrequestreleased. Final
night-stop-state.json confirms own ETWsession notfound0x80300002. Prior read-only
shell sessions also finished. No retry after user stop.12Desktop wider-cohort
rawPNGs independently rehashed exact. Handoff updated; day-end recap added.
Resume only on new user request; real frame-time CSV validation remains open.
