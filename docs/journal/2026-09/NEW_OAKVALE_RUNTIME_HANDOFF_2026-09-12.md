# New Oakvale runtime session — September 12

## Current overnight state ? supersedes the historical entries below

User stopped for sleep; no automatic continuation or launch. Read
[the overnight handoff and resume checklist](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md)
first. Installed baseline is v23, with all25 files verified unchanged. The user
completed childhood on original FSE plus the isolated add-on. Father Escape and
Bully retail skip behavior are accepted. Audio remains open: live DirectSound
returns unsupported config8, causing the native six-channel fallback and disabled
relative-position updates. V5 captured the raw result; no audio fix is installed.

The v15?v21 status and unknown-actor/device statements below are historical.
Current actor is Father, headphones, authored locked cutscene camera.


Latest: **v21 Bully fallback removal installed, game closed**, backed by v20 command traces.
426 tests and 120 fixtures pass. Awaiting fixed-build skipped visual test; audio remains open.
See the final entry below; earlier hypotheses are historical.

Runtime ownership: New Oakvale reconstructed scripts, fixtures, evidence, and
`D:/Code/ForgeFSE-retail-shadow`. Converter files and generated lifted packages are read-only
for this session. No resets, stashes, commits, or changes to the shared handoff/SDK manifests.

## Installed state verified, including drift omitted from the incoming handoff

At session start the game and launcher were closed. Both installed DLLs matched v15:
`8102215BF082508052FDF451B1FE2BAB61E2FD8FC736DD93F4389EA69C6DF80E`.
The installed registry was empty, the native allocator override enabled, and
`FinalAlbion.qst` contained no `AddQuest("NewOakValeIntro", TRUE)`.
The saved log reports `NOVI_AUTHORITY ... native=true legacy=false`.

**The installed Lua was not the canonical v15 package.** Only the Bully entity differed
among the package scripts (registry/override intentionally differ from disabled source
defaults). Its hash was `B5B9310F5E23A1DC5DAF34B07B2CF2D9520734DFA48949025E8B15638448F79F`.
That experimental version kept Bully drawable if `IsCameraPosOnScreen` returned true and
conditionally omitted `RemoveThing`. It had no later retry for that skipped removal.
Backups named v16/v17/v18 exist after the handoff's last recorded v15 install.
Do not attribute this behavior to canonical v15 or reintroduce it as a retail-order fix.

Exact installed files, hashes, original runtime source files, and both dirty-tree status
snapshots were preserved under
`refs/script_recovery/new_oakvale_intro/runtime_evidence/session-start-20260912/`.
The original installed log (last write September 11, 20:27:50) has SHA-256
`C6A2F666517B51D7446416530FA47674FF929352B8AB19567C566861BEAF0E18`.
It contains no DLL build hash; current installed DLL hashes do not prove which DLL made it.

## Bully evidence and remaining diagnosis

Decoded the installed macros into `runtime_evidence/bully-installed-macros-20260912.json`.
Installed `script.bin` matches retail SHA-256
`3D6D09939B612478F70C6AA1EB45A5BE0E850B27D534F78D996A00466501C1D9`.
RUN1 still teleports to BULLY1, speaks, waits, runs to BULLY2, changes to the BRAT camera,
speaks, teleports Hero, and hides Bully. RUN2 performs the subsequent BRATHERO camera cut.
No macro asset modification explains the reported behavior.

The newly preserved log records:

- RUN1 ends at BULLY1 `(3604.479,860.742,13.707)`, with Bully's task false.
- Compatibility reaches BULLY2 `(3596.740,851.895,13.375)` in 46 frames.
- Experimental visibility query is true there and removal is deferred after RUN2.
- BULLY resource is `0x48cfa68`. RUN1 has no movement issue and no task poll matching
  this resource. Its logged polls belong to other controllers. His speech does reach
  the native Speak hook with `soundIn2D=1`.

`analyze_cutscene_task_capture.py` now separates macro intervals, actor/resource identities,
movement issues, poll transitions, and speech operands. Its report is
`runtime_evidence/task-capture-unattributed-20260911-202750.json`.
Missing issue/poll records are an instrumentation/dispatch question, **not proof that the
engine rejected or cancelled Bully's task**. Global macro bracketing also observes unrelated
entity tasks during yielded frames. Polls may represent speech, animation, or movement.

`analyze_bully_run_variance.py` schema 0.2 retains `moved` for compatibility but explicitly
labels its inclusion of the pre-run teleport. New `runStartDisplacement` and
`endedAtPreRunTeleport` fields prevent the old 2.36 m interpretation. Endpoint displacement
is not sampled path length. Updated report: `analysis-bully-run-variance-20260912.json`.

Read-only PE inspection confirms retail vtable `0x0128D86C` has MoveToPosition at
`0x00903BC0`, MoveToThing at `0x00903E20`, Speak at `0x00904780`.
Fresh capture must verify the live actor slots and whether RUN1 reaches these boundaries.
The native RunTo wait and the successful v13 control-depth behavior remain unchanged.

## Audio investigation

The user reports camera-facing dialogue becoming quiet/distant. No exact actor/line,
playback device, or controlled orientation comparison has been supplied yet.
Keep this independent from Bully movement until evidence connects them.

Read the direct Speak wrapper, conversation creation/line wrapper, native macro Speak
probe, and reconstructed speech operands. Direct Speak forwards the supplied selection,
listen, `soundIn2D`, and fade flags. Conversations use a separate AddNewConversation path.
The preserved Bully native macro speech reports `soundIn2D=1`; switching all dialogue to
2D would therefore not establish a cause or fix. Installed `user.ini` contains
`SetPlatform2DGain(0.6)`; it was archived and left unchanged. A global fixed gain by itself
does not explain orientation dependence.

v19 adds passive timestamps and identities for direct speech, conversation creation/queued
lines, and existing native speech/movement probes. A queued conversation line is not a
playback-start timestamp. Audio listener transforms, actual attenuation, and output-channel
routing are **not measured by these probes** and remain investigation leads.
Do not claim the flags prove how the downstream audio renderer routes a line.

Live capture procedure: identify the affected NPC/key; keep Hero and NPC positions fixed;
compare forward, side, and away camera orientations during a repeatable line; distinguish
free-camera rotation from an authored cut changing camera distance. Record video timing,
headphones/speakers, the active playback endpoint, and whether other dialogue routes behave
the same way. If needed use a separately archived native-only configuration with the same
scene/line. No global volume, speech-flag, or device-setting workaround has been applied.

## v19 diagnostic build installed; awaiting coordinated live test

User selected **keep investigating; they will tell us when ready**. Do not launch early.

Release|x86 build succeeds. DLL SHA-256:
`227C6E78FF000D787D486E1F240039B1D0B2676B91E0A28998DF8138D97557A8`.
Bundle: `work/new_oakvale_test_bundle_20260912_v19_diagnostics/`.
It contains canonical Lua, the empty custom registry, and enabled identity override.
Installed all 25 destination files with pre/post process checks and exact hash verification;
both installed DLLs match. The game remained closed.

Rollback: game `FSE/backups/new-oakvale-v19-diagnostics-20260912-091303/`;
deployment manifest also archived as `runtime_evidence/deployment-v19-20260912.json`.
This preserves the previous experimental installed Bully file. v19 restores canonical
unconditional hide/removal ordering; it is a baseline correction, **not a verified fix for
the original native run-off or audio defect**.

Runtime changes add actor vtable slot addresses and ticks; direct speech speaker/controller
identity and ticks; conversation identity/flags/queued keys; and ticks on native boundary logs.
They do not alter call operands, return values, control acquisition, camera ordering, or volume.

Baseline gates: recovery suite 397/397, package 119/119 (zero warnings/errors), authority OK,
control ABI OK, canonical tooling SDK PASS (`--skip-mirrors`). Three focused analyzer tests
pass after the new capture analyzer and corrected displacement fields. Build log and gate
outputs are in `work/runtime-20260912-*`. Full final gate results are appended below.

Final gates: **406/406 recovery tests**, v19 control ABI OK, Release|x86 build successful.
The installed 21 package scripts were copied byte-for-byte into a disposable validation
directory, with canonical `quests.lua` supplied there solely as static registration metadata:
**119/119 fixtures**, zero syntax/API/callsite errors or warnings. Report:
`runtime_evidence/installed-v19-validation-20260912.json`.
Directly pointing the package validator at the live empty registry is unsupported: its
missing-registration early return lacks `summary`, causing a CLI `KeyError`. No live registry
change was made to accommodate that tool. The isolated validation copy contains 22 Lua files
(21 package scripts plus test registration); canonical source validation includes the separate
override file as its 23rd Lua file.

Next live run: launch via FSE_Launcher only when the user is ready; confirm startup authority
and diagnostic installation; capture Bully run/disappearance/camera pan and specific audio
lines; preserve exact DLL/Lua hashes and log/video correlation. Keep Wife dialogue/travel,
Man walk-home, single Guard lecture, warehouse reward/outcome, Theresa/chocolates, raid,
post-raid, and green-filled/red-empty health bar as regression requirements.

## Next port preparation

Selected **QS_MeetSister** for the next review after New Oakvale live fixes. It has an
anchor-checked native cluster, two existing executable seed scripts plus quest Main, and
quest/messenger trace fixtures. Its bounded quest lifecycle has no persistent state in
the exported Init/OnPersist bodies. Native Main `0x00E28DE0` registers Messenger then Sister
(native binding IDs 1 then 0), finalizes entities, and sets the summary objective to
`TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY`, `HauntedHouse`, `BarrowFields`.
Slots 0x100/0x4A0/0xA3C resolve to PostAddScriptedEntities/SetQuestCardObjective/GetActiveQuestName.

Sources: `native_clusters/QS_MeetSister.json`, `seed_corpus/sources/MeetSister/`,
`trace_fixtures/meet_sister_main.json`, `trace_fixtures/meet_sister_messenger.json` under
`refs/script_recovery/`. See `docs/scripts/SCRIPT_RECOVERY_PIPELINE.md` and the shared roadmap.
ScytheInfo lacks an evidence anchor; HerosOldHouse adds three entities, persistence, and a
worker thread. MeetSister therefore offers the smaller anchored review boundary.

Do not enable it yet: the cluster exports only the quest lifecycle; `native_threads/QS_MeetSister.json`
has zero functions and does not establish the two entity bodies. Review those native
allocators/Init/Main before trusting the seed's self-lookups, ArenaExterior wait, cutscene
actor casing, removals, completion operands, deactivation, and fade. Add termination and
Sister completion fixtures, then test isolated authority/save/region cleanup. Converter
output remains untouched and is not accepted as entity behavioral evidence.

## Live launch — 09:16:56 MDT

User requested launch. Rechecked all 25 installed files against the v19 deployment manifest and confirmed the empty registry/enabled native override/no duplicate FinalAlbion activation. Launched through FSE_Launcher.exe; Fable PID 9208. Fresh startup confirms movement, speech, and cutscene diagnostic hooks installed. Awaiting gameplay capture; do not overwrite the live DLL or Lua files.

## v19 video comparison and v20 — 09:26 MDT

User supplied local `Fable - The Lost Chapters_2026.09.12-09.17.mp4` (Bully run ~0:38),
compared with https://www.youtube.com/watch?v=Wc-6dgTt4YY around 1:02.
Archived the closed game's log as `runtime_evidence/interactive-20260912-v19-bully-video.log`:
SHA-256 `4EBE3808E2D2BE2B2868AD78631E3CB6AA761C87C533E121B8F6FB42DDB403FF`.
Video hash/path and build correlation: `video-comparison-v19-20260912.json`.
Authority is native=true/legacy=false; zero error markers or Lua runtime errors.

Downloaded the user-linked reference interval 00:52–01:22 with yt-dlp after the web page
fetch failed. Contact sheets and reference clip: `work/runtime-20260912-video-comparison/`.
Local sheet covers 00:33–00:50; reference sheet covers original 00:52–01:21, both at 1 fps.

**The missing retail sequence is larger than premature disappearance.** The reference
shows the run, Hero turning, the victim close-up with his initial thank-you, then the teddy
exchange. The user's video goes directly to the teddy exchange after the compatibility run,
omitting the victim close-up/initial line. Investigate interpreter progression before path
or camera geometry changes.

v19 proves live BULLY resource `0x48e8400` uses retail vtable `0x0128D86C` with expected
movement/task-poll targets. RUN1 has **zero matching movement issues and zero matching polls**.
Its only native speech record is `TEXT_QST_048_BULLY_GOING_AWAY` at tick 5176593; RUN1 returns
with Bully at BULLY1. Compatibility reaches BULLY2 in 46 frames. RUN2 then speaks
`TEXT_QST_048_VICTIM_THANKS_HAVE_ROSIE`; the earlier RUN1 victim line is absent.
Report: `task-capture-v19-20260912.json`.

Retail RUN1 SkipCond is FadeOut, GamePause 0.5, Hero/Brat teleports, look adjustments,
Bully Drawable FALSE, FadeIn. It omits RunTo and the initial victim line. This is a candidate
explanation, **not yet proven**. Outside-macro `skipped=0` does not establish intermediate
message results. Asked whether the user pressed skip; no answer received before this entry.

Read-only disassembly: IsCutsceneSkipped `0x00CBEB7E` selects CGSI slot 0xA8
(MsgIsActionModeButtonPressed) or 0xB0 (MsgIsCutSceneSkipped) using byte `0x0143E8F4`.
The interpreter loop can replace its command vector with SkipCond on a true result.
CGSI MsgIsCutSceneSkipped `0x00893BA0` queries message type 0x32.

v20 adds passive, byte-validated hooks:

- FindCharPos `0x0099E5A0`, five bytes `56 8B F1 8B 06`, logs complete current command only
  for interpreter return address `0x00CC023C`, then forwards once.
- IsCutsceneSkipped `0x00CBEB7E`, seven-byte absolute-memory compare with its embedded
  address checked against ASLR(0x0143E8F4). Calls original exactly once, logs true results
  with caller/tick, preserves result. No renderer-eligibility hook is reapplied.

No skip suppression, forced commands, movement/camera changes, or volume changes.
Analyzer now preserves command order/skip results; four focused tests pass. Control ABI
passes and Release|x86 builds. Lua bytes remain identical to the 119/119-tested v19 package.
Session-only source delta: `runtime_evidence/v20-command-diagnostics.patch`.

Installed v20 DLL SHA-256:
`8CBEB1943CCD3D1106E10BE291A24FCB1D03F928660E7A0376592454DFC0E698`.
Bundle: `work/new_oakvale_test_bundle_20260912_v20_command_trace/`.
Rollback: `FSE/backups/new-oakvale-v20-command-trace-20260912-092610/`.
Deployment: `runtime_evidence/deployment-v20-20260912.json`, all 25 file hashes verified;
only two DLLs changed. Game/launcher closed before backup and installation.
**No relaunch occurred. Next run requires coordinated timing and command/skip trace review.**


## Evidence correction and v20 proof launch ? 09:31 MDT

User confirms they intentionally pressed skip, as they do in retail. This explains why
missing RunTo/first victim speech cannot be called a native failure without observing
SkipCond dispatch. Earlier claims of premature native return were overstated. v19 proves
only no matching movement issue/poll, Bully still at BULLY1, and subsequent Lua fallback.
A candidate removing that fallback was prepared, then moved to
`work/runtime-20260912-video-comparison/unverified-fallback-removal/` pending command traces.
Active Bully source was restored byte-for-byte from v20; original fixture semantics were
restored and its three byte-audit tests pass. Candidate was never installed.

User authorized launch for skipped versus unskipped proof captures. Rechecked all 25 files
against deployment-v20 and the absent duplicate activation; launched FSE_Launcher at
09:31:27 MDT. No behavior change is included. Compare actual command/skip traces before
accepting or deploying a candidate fix.

## First v20 proof snapshot

Archived `interactive-20260912-v20-first-proof-snapshot.log`, SHA-256 `6048602271AEBF4B3B004FADEDF624A51E5161E6F44A114409651847B3F35049`; parsed `task-capture-v20-first-proof.json`. RUN1 executes native RunTo at tick 6046906 on BULLY controller 0x47e4500, then the victim camera/first thank-you. Skip query becomes true later at tick 6052562 (caller 0x00CC0133), followed by the exact eight SkipCond commands. Bully ends at BULLY2 and no compatibility move occurs. This capture is a **late skip**, not the earlier skip-during-apology case. Requested an early-skip capture to establish the missing causal link. The short interval between RunTo issue and the next command does not prove visually smooth movement; retain video comparison as a separate gate. No candidate installed.

## User correction: unskipped success; second v20 launch ? 09:34 MDT

User: "I didn't skip this time and everything worked like it should". Therefore the prior
entry's label "late skip" must not imply user input. The log proves the engine returned
true from its skip query late in RUN1 and selected SkipCond; it does not prove why.
Retain the user's no-skip observation and successful visual playback as the baseline.
Final log archived as `interactive-20260912-v20-unskipped-success.log`, with hash and user
statement in `v20-unskipped-observation.json`. Native RunTo executed and ended at BULLY2;
no fallback move occurred. Candidate remains outside active source/install.

Relaunched unchanged v20 at 09:34:00 for the remaining early-skip comparison in the
already-authorized two-run test. Archived previous log before launch and verified all 25
installed hashes. User instructed to skip while Bully apologizes. Awaiting actual trace.

## Early-skip proof and v21 supported fix ? 09:39 MDT

Second v20 capture establishes the actual early-skip dispatch. RUN1's skip query returns
true at tick 6192375, before BULLY.WaitTask/RunTo. The interpreter then executes all eight
retail SkipCond commands in exact order, including BULLY.Drawable FALSE at tick 6193109.
No native Bully movement command is dispatched. After RUN1 returns at BULLY1, Lua logs
its compatibility branch, re-shows Bully, runs him to BULLY2, and hides him again after
46 frames. User observes: "The skip seems to be the issue with him disappearing while
visible on the screen".

Complete evidence: `runtime_evidence/interactive-20260912-v20-early-skip-complete.log`.
`tools/script_recovery/audit_bully_skip_runtime.py` compares that capture with the user's
successful unskipped baseline and decoded retail macros. All 11 checks pass; hashes,
inputs, and exact executed skip tail are in `runtime_evidence/bully-skip-runtime-proof-20260912.json`.
This establishes that the fallback overrides legitimate skip cleanup. It does not prove
all historical short-endpoint captures had identical causes or resolve the audio issue.

Applied the previously staged candidate only after obtaining that command evidence.
Removed the post-RUN1 distance check, re-show, compatibility movement/wait, second hide,
and their three private constants. Native RUN1 and RUN2/dummy calls, flags/actor maps,
movie/control cleanup, deed state, and unconditional final removal remain unchanged.
No native movement, camera, skip-input, or global control implementation changed.

Added a fixture with the observed native post-skip BULLY1 position alongside the normal
BULLY2-position case. Both prohibit Lua re-showing/run movement and preserve macro/removal
order. Mock fixtures supply native endpoints; they do not execute the native interpreter.
The Bully lifecycle test also checks the post-macro event interval directly. Regenerated
only the 15 Bully traces (including the new fixture). Initial tests caught that forbidding
all MoveToPosition calls also excluded the pre-encounter walk-home path; corrected the
assertion to forbid run-type movement and all post-RUN1 movement, preserving walk-home.

Updated the obsolete wrapper audit that demanded the fallback. It now recomputes the
captured skip proof and forbids Lua movement/visibility overrides in run_off. The first
full suite failed solely on that obsolete requirement; final suite **426/426 PASS**.
Package **120/120**, zero warnings/errors. Authority and canonical SDK PASS; v20 runtime
control ABI/build remain unchanged. Reports under `work/runtime-20260912-video-comparison/v21-*`
and `runtime_evidence/v21-package-validation.json`.

v21 bundle: `work/new_oakvale_test_bundle_20260912_v21_retail_skip/`.
Only installed `FSE/NewOakValeIntro/Entities/NOVI_Bully.lua` changed:
SHA-256 `FB2DFAD25DA53EE4006FBE9A53B1EFA4EC9BFAE7948ACC444BA5BE2CA06675AA`.
DLL remains v20 `8CBEB1943CCD3D1106E10BE291A24FCB1D03F928660E7A0376592454DFC0E698`.
Rollback `FSE/backups/new-oakvale-v21-retail-skip-20260912-093903/`.
All 25 installed hashes verified; game and launcher closed before backup/install.
Manifest: `runtime_evidence/deployment-v21-20260912.json`.

**Supported fix installed, visual outcome awaiting live test.** Asked user for timing to
launch the fixed-build early-skip check. No launch yet. Preserve successful unskipped
behavior and childhood regressions; audio investigation remains a separate open task.

## v21 live verification launch ? 09:40:26 MDT

User requested launch. All 25 installed hashes match deployment-v21, duplicate FinalAlbion activation absent, and game/launcher were closed. Launched through FSE_Launcher for early-skip verification. Awaiting command trace and user visual result.

## v21 visual test FAILED; fade diagnosis continues

User reports Bully disappears without running. v21 is not a completed visual fix.
The trace executes the retail eight-command SkipCond before RunTo, with no Lua
resurrection/move. FadeOut at tick 6588046, hide and FadeIn at 6588703; command
logging alone does not establish accepted fade or screen opacity. Log and hashed
observation are archived as interactive-20260912-v21-visible-hide.log and
v21-visible-hide-observation.json. No new video is available for this run.

Preparing v22 passive fade-state snapshots at interpreter command boundaries.
No new control, movement, visibility, input or fade behavior changes. Native
call-site bytes still confirm setup=false/skippable=true matches wrapper.
Original retail commands outside Lua brackets will also be logged (macro=<none>)
to support a reversible native-only comparison. This is diagnostic, not a fix.

## v22 fade diagnostic build installed

Release x86 build PASS; control ABI PASS; four focused capture/wrapper tests PASS.
Both installed DLLs SHA256 6FD946ADE7507AB67CDB09CD61CB470EC17D005C3E0F7613D90B4EDB4457D424.
All 25 installed files checked against v21 before deployment; only the two DLLs
changed. Game was closed. Rollback: `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\FSE\backups\new-oakvale-v22-fade-diag-20260912-095143`.
Deployment manifest: runtime_evidence/deployment-v22-20260912.json.
Lua and successful childhood control behavior unchanged. Awaiting coordinated
early-skip capture; this build does not claim to fix the visual defect.

User confirms Escape in both retail and Forge. User asks to keep investigating
and will signal when ready; do not launch yet. Prepared (not deployed) a native-only
comparison config under work/runtime-20260912-video-comparison/native-only-comparison.
Only override enabled changes; runtime loader returns before installing allocator
overrides when disabled. Use fresh disposable test and restore exact config afterward.

Older v19 video automated blackdetect (30s offset, 18s window, pix_th=0.05,
pic_th=0.97,d=0.03) finds a 0.05s black interval at absolute 35.448893-35.498893s
and no later interval. This supports further fade inspection, not proof of v21
renderer state (UI pixels/threshold can affect detection). Output: work/runtime-20260912-video-comparison/v19-blackdetect.txt.

Independent next-port preparation: recovered both QS_MeetSister entity allocators,
vtables and Main bodies directly from installed retail bytes. Findings and remaining
review are in refs/script_recovery/meet_sister/runtime_evidence/PORT_REVIEW_2026-09-12.md.
A concrete seed discrepancy is movie lifetime around RemoveThing: Messenger removes
inside its paused movie scope; Sister removes after scope teardown. No port activated.

## v23: independently confirmed RUN1 termination bug fixed offline

User supplied independent review work/bully-independent-review-20260912.md.
Native gates at 0x00DBCBA4/0x00DBCBF7 call entity termination helper 0x00F35B30
and take cleanup-only 0x00DBCC00. The Lua port omitted both and continued success.

Fixed IsActiveThreadTerminating Lua binding to capture the actual pEntityHost
provided by RegisterBindingsInState for each entity VM; quest VMs retain their
parent-quest query. No Lua signature/manifest/overlay change, no extra frame, no
cast from arbitrary thing pointer. Bully tests that query immediately after RUN1
and unwinds camera/movie then victim, Hero, Bully controls without second macro,
teddy state, deed, ran-off flag or normal-path RemoveThing. Ordinary skip remains
separate and continues the normal completion path.

Actual run_off closure regression raises entity termination inside RUN1 with
both teddy states and rejects any later macro or position access. Both PASS.
Two full-Main cancellation fixtures also verify exact post-RUN1 cleanup order
and forbid success writes. Before/after traces are archived in
runtime_evidence/bully-run1-cancellation-before-after-20260912.json.
Five focused tests PASS; full recovery suite 437/437 PASS; package 122/122 PASS
with zero errors/warnings. Only 17 Bully traces regenerated. Release x86 build,
control ABI, authority and canonical SDK PASS. Native live termination not yet
reproduced with v23; the regression proof is offline Lua execution plus runtime
host-dispatch code review/build.

Installed v23 after verifying all 25 v22 hashes and closed game. Only two DLLs
and Bully Lua changed; manifest deployment-v23-20260912.json. DLL SHA256 C3900B3DB85423E1A7E4035825B265225E4892EB26522B92580AA2A055539D31; Bully Lua F64C66264D456455C63DF8A2E6F07D5518E942DF4E75DA0A439D9C2523ECDB54.
Rollback: `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\FSE\backups\new-oakvale-v23-run1-cancellation-20260912-100639`.

Visible Escape disappearance and audio are STILL UNRESOLVED. v23 retains all
v22 fade snapshots. User asked to wait for their launch signal; no launch.

## v23 launch 2026-09-12T10:13:51

User requested launch. Verified all 25 installed v23 hashes, absent duplicate
FinalAlbion activation and closed game/launcher. Archived previous log and
launched via FSE_Launcher.exe for Escape during Bully apology. Awaiting capture.

## v23 capture: accepted and completed fade, visible scene

User video 2026.09.12-10.14.mp4 (8-second clip) shows Bully disappearing while
scene remains visible. Archived full log interactive-20260912-v23-visible-hide.log
and hashes v23-video-provenance.json. This run differs from v21: native RunTo
dispatched at 8574437, then skip query true 8574640. Skip FadeOut 8574703 is
accepted (next snapshot active=1,outElapsed=0,outTime=0.5). Before hide at
8575375: active=1,fadingOut=0,fadingIn=0,holdUntilIn=1,outElapsed=0.516086,
outTime=0.5,colour=0xff000000,freezeCount=0. Fade rejection/failure to advance
is ruled out for this capture. Visible presentation remains wrong. The drawing
path CDrawScreenFade::Draw at 0x006495A8 uses display alpha through 0x004364C0,
which derives it from the same fade state. Need compare original retail before
altering rendering or inventing movement.

TEMPORARY NATIVE-ONLY PROFILE NOW INSTALLED, not launched. Only
FSE/retail_override.lua enabled flag changed false, after checking all 25 v23
hashes and closed game. DLLs, Lua scripts and assets unchanged. Backup and
restoreSource recorded in deployment-native-only-v23-20260912.json. Restore
exact override bytes after the comparison, game closed. Expect disabled allocator
log and no Lua NOVI activation. Await user timing for original retail Escape test.

Additional v23 distinction: after RUN1 Bully is (3601.102,856.481,13.607),
with bullyTask=true. He has moved from BULLY1 when skip cleanup hides him.
Do not classify this as the earlier skip-before-RunTo case or zero movement.
Structured command/snapshot analysis: runtime_evidence/v23-fade-capture-analysis.json.
Video blackdetect found no fully black interval across the 8.35-second recording.

Native-only v23 comparison launch authorized by user at 2026-09-12T10:19:55. All 25 profile hashes verified, override disabled, game closed. Launching FSE_Launcher. Restore override after capture.

## Original retail quest comparison reproduces visible hide

Native-only v23 launch completed; game closed. Loader confirms allocator override
disabled. Original retail Bully runs SkipCond before RunTo in this capture.
At hide tick 8948531 the sampled fade is held, outElapsed=0.516286/outTime=0.5,
active=1, fadingIn/out=0, black colour, freezeCount=0. Video
2026.09.12-10.20.mp4 shows visible Bully disappearance with no black transition.
Log interactive-20260912-native-only-v23.log and provenance hashes archived.
This reproduces with the original quest and rules out missing reconstructed
Bully logic as the cause of the absent rendered fade. It does NOT rule out
the extender: v23 DLL and its hooks were still loaded. Next comparison should
launch stock Fable.exe directly and verify the extender module is absent.

Restored exact v23 retail_override.lua bytes from backup after capture and
verified all 25 v23 installed hashes. Native-only comparison is no longer active.
No behavior fix inferred from this result; presentation bug remains unresolved.

## Stock executable comparison launch 2026-09-12T10:33:17

User explicitly authorized direct Fable.exe launch. PID 15444. Module enumeration
verified FableScriptExtender.dll absent; evidence stock-launch-no-extender-20260912.json.
No installed files changed. v23 profile remains installed for subsequent FSE launches.
Awaiting stock Escape/video result; no extender trace is expected for this run.

## Stock launch result and verification limitation

2026.09.12-10.33.mp4 reproduces visible disappearance, followed by victim/teddy
scene. Direct executable launch generated no new extender log (still 10:20).
Correction: the PowerShell module list contains only seven native/WOW64 modules,
so it is not exhaustive evidence of 32-bit DLL absence. Direct launch plus the
unchanged extender log support stock behavior; do not cite the module list alone
as conclusive verification. Video hash and limitations in stock-video-provenance-20260912.json.
User suggests stock TLC may lack visible fades and supplied a second reference:
https://www.youtube.com/watch?v=IpF4n02QJhw&t=292s. Reviewing that recording next.

Reference IpF4n02QJhw at 292s remains UNVERIFIED: web fetch throttled;
yt-dlp metadata/formats succeeded but selected media-range downloads stalled
(including lower-resolution video-only retry). Stopped only our matching ffmpeg
download processes. Do not claim to have watched this reference. Existing local
three-way captures still support a stock-installation fade presentation issue;
they do not prove all retail builds/hardware are affected. No further changes.

## v24: passive fade draw probe installed, awaiting live capture

Static trace rules out GUI mode 1 alone as a missing-fade cause. Predicate
0x004374F0 returns mode(+0x294) != 1. Caller 0x0069EAFB routes mode 1 to
0x0069EBC9, which explicitly calls CPlayerGui::DrawScreenFade at 0x0069EC09.
Other modes use regular GUI drawing. Both paths call 0x006495A8.

v24 FableAPI.cpp adds a pass-through __thiscall/fastcall draw hook at 0x006495A8.
Verified installed retail prologue is five complete relocatable bytes
55 8d 6c 24 8c (push ebp; lea ebp,[esp-0x74]); function returns ret 4.
It calls the original exactly once, counts entry/return, and records receiver/tick.
No logging, native queries, or engine state writes inside the rendering hook.
Existing command snapshots read GUI native +0x168 fade object, +0x294 mode,
fade object +0x10 main-game pointer -> +0x28 render display, compare to GSI
fade display, and report counters. drawFlag(+0x18) and opaqueSticky(+0x28)
are historical flags, NOT proof that the current frame reached submission.
Counters establish draw entry/return, NOT successful primitive presentation.
A zero counter must be interpreted alongside hook installation status.

Installed DLL SHA256 5129C4CA1B79A3AC9172C0B12C119A9C5D61A8960F2C6824BBCB67FD9BE7D565.
All 25 v23 hashes verified before deploy and all 25 v24 after; only two DLLs
changed, game closed. Exact v23 DLL backups and deployment paths are in
runtime_evidence/deployment-v24-20260912.json. Lua remains identical to v23,
including the separately proven RUN1 cancellation fix. No renderer behavior fix.
Release x86 build and control ABI audit passed. Installed byte proof and isolated
source delta: v24-draw-probe-retail-bytes.json and v24-draw-probe.patch.
Build log: work/runtime-20260912-video-comparison/v24-draw-probe-build.log.
Awaiting user launch timing for Escape during apology. Not launched automatically.

## v24 withdrawn after Father Escape failure

User reports Escape did not work while talking to Father. Game was already closed.
Archived log interactive-20260912-v24-father-escape.log before any replacement.
Verified all 25 v24 hashes, restored only both DLLs from exact v23 backup, then
verified all 25 files match the pre-v24 manifest. Installed build is v23 again.
Rollback record: v24-rollback-father-escape-20260912.json.
Removed only the three v24 insertions from working FableAPI.cpp; retained v24
source separately in work/runtime-20260912-video-comparison/FableAPI-v24-withdrawn.cpp.
Do not re-deploy the Release DLL until rebuilt: it is still the withdrawn v24.

Capture entered CS_OAKVALE_INTRO_FATHER skippable=1; no successful skip query
was recorded. End state skipped=0. Late commands all execute within 16ms with
stale draw counters, consistent with teardown/cancellation but not proof of
its trigger. Do not count that tail as normally played dialogue. Entry/return
counts matched in the sampled data; rendering receiver uses same display as GSI.
These observations do not establish the cause of failed Escape. Treat v24 as a
suspected regression pending comparison; build success was not live validation.

Withdrawal follow-up: Release x86 rebuild passed after removing v24 insertions;
Release output no longer contains the withdrawn probe. Installed DLLs remain the
exact backed-up v23 binaries, not the rebuild. Log v24-withdrawal-build.log.

## Native eligibility emulation and external capture preparation

User requested continued work, then explicitly deferred launching the restored
v23 comparison. Game remains closed; do not launch until user supplies timing.

Executed installed retail IsCutsceneSkippable bytes (0x0048DE44) using Unicorn
in an isolated work/runtime_re_tools dependency directory. All 384 combinations
of six boolean gates and six representative subtitle masks agree with the derived
predicate. Full inputs, native outputs and executable hash are archived in
retail-skip-gate-emulation-20260912.json. Reproducer:
work/runtime-20260912-video-comparison/prove_skip_gate.py.

EASkipCutScene at 0x00629F50 first requires GUI+0xF6 inCutscene, then calls that
predicate with receiver GUI+0x18. The predicate rejects nonzero
[global 0x013B86A0 -> +0x24 -> +0x104], disabled skippable GUI+0xF8,
already-skipped +0xF7, subtitle mask +0x94 & 0x48, inactive skip timer +0x100,
or nonzero timer phase +0x10D. These live values were NOT all in the failed log.
Native SetAsInCutscene 0x0049319C initializes CFadeInFadeOutBase at subtitle+0xE8
(GUI+0x100) using duration from GUI definition+0x2C4, via 0x00434840.
0x00490494 updates this timer through 0x00434870 while preparing subtitles.
Hence a skippable=1 boundary log alone cannot prove that Escape was eligible.

Added tools/script_recovery/observe_retail_cutscene.py: external RPM-only capture,
checks executable hash, uses Toolhelp32 MODULE|MODULE32 enumeration, samples
raw GUI/fade bytes, gate values, foreground PID, and Escape transitions.
Keyboard observer uses its own message-only Raw Input INPUTSINK; no NOLEGACY,
no async-key-state consumption, no input hook, no injection. Only Escape events
retained. Windows registration/poll/cleanup smoke passed; all 384 predicate cases
match actual emulation. Whole live sampler still awaits a running game.
Raw Input reference: https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
Run after authorized launch with --pid GAME_PID --output UNIQUE_JSONL --seconds 600.

Additional archived-log finding: before Father speech, draw counters advance
1->30 with drawFlag=1 and opaqueSticky=1 while fade active; later 130->278 as
speech progresses, fade inactive. It is not a blanket draw-entry failure. Flags
show the default opaque-submission branch was reached historically, not proof
that pixels appeared. No new video was present for the Father failure (latest
video remains 10.33 stock comparison).

Audio static chain: ScriptedThing Speak 0x00904780 -> talk constructor 0x00717AA0
-> base 0x00717950 stores soundIn2D at action+0x72. SetUpTextAndSpeech loads it
at 0x00716C48 and forwards to CTCTalk 0x006E3580 -> attached sample 0x0066B300
-> 0x0066B080 -> emitter dispatch 0x00836810 (+0x98 object, vslot+0xC).
No flag drop identified in this chain. Actual backend/source positioning remains
unverified; do not infer audible 2D output solely from flag propagation. Exact
native ranges saved in retail-escape-audio-path-bytes-20260912.json.
No further installed files, Lua semantics, mixer settings, or converter files changed.

Additional asset proof: installed game.bin PLAYER_GUI_DEFAULT (7074) and
PLAYER_GUI_PC (7075) both contain TextBoxCutsceneSecondsBeforeSkip = 2.0,
tag 13 37 10 27, float bytes 00 00 00 40. Null GUI definition is 0.0.
retail-gui-skip-delay-20260912.json records installed bank/payload hashes and
exact offsets/context. This establishes configured duration, NOT the actual
elapsed timer when user pressed Escape. Observer now decodes duration/elapsed
from GUI+0x104/+0x110 and samples sound-manager vtable and CreateEmitter target
so the next authorized run identifies the active audio implementation too.

External observer decoder regression test added: test_observe_retail_cutscene.py;
it compares every archived native execution result (384 cases), passed. Observer
compile and message-only Windows Raw Input registration/poll/cleanup smoke pass.
Final installed verification: all 25 files still match v23. User's launch deferral
remains in force. No live observer capture yet, no proven fix for Father Escape,
no proven root cause for missing rendered fade or directional audio. The next
run can distinguish key arrival/focus, native eligibility gates, timer progress,
fade display identity and audio implementation without modifying the game.

## Background porting: Meet Sister executable reference

User explicitly requested background script-porting work while unavailable.
Created refs/script_recovery/meet_sister/reference/quest.lua, reconstructing root
Main and both entity bodies against an explicit resource adapter contract.
This is NOT an FSE-loadable package and is NOT installed. The runtime lacks the
single unchecked Hero-acquisition and explicit actor-resource/map lifetime
operations needed for full fidelity. PORT_REVIEW_2026-09-12.md explains these
specific blockers and the required adapter, with no invented registered APIs.

Eight Lupa tests pass in tools/script_recovery/test_meet_sister_reference.py:
root setup, empty Init, region wait termination, initial termination, acquisition
retry/termination, ignored Hero failure, and both post-macro cleanup sequences.
Test host additionally checks surviving resource/map-copy lifetimes and balances
all conceptual handles. These are reference-contract tests, not native refcount
measurements or live playback proof. Native bodies have no post-macro termination
check: unlike Bully RUN1, completion cleanup continues after the macro returns.

All 19 marker/camera dependencies resolve uniquely in the installed TNGs. Both
bound actors are authored in QS_MeetSister sections. All eight text references
resolve (two groups with six members total, five direct spoken lines, one objective).
Three installed dependency artifacts record complete records and source hashes:
installed-marker-camera-dependencies-20260912.json,
installed-dialogue-dependencies-20260912.json,
installed-activation-dependencies-20260912.json.

Corrected own earlier evidence labels: E28DE0 is Main, not RegisterMain; actual
RegisterMain E28D40. Removed unsupported numeric Binding ID labels in favor of
proven Messenger-first, Sister-second registration order. Native entity callback
slots OnPersist/OnPredicateFail/OnInterrupted are empty CDEBC0/CDEBD0/CDEBE0;
GetParentScript accessors E29010/E290E0 return host+0x14. No hidden custom
entity predicate was found in those slots.
No installed files, converter files, shared manifest, or runtime source changed
by this background port work. New Oakvale remains the sole active Lua override.

Meet Sister native execution follow-up: actual retail x86 entity Main bodies
now run in Unicorn with engine-call stubs. Fourteen scenarios match the actual
Lua reference call-for-call; native return stacks balance. Results and scenarios
in meet_sister/runtime_evidence/native-lua-flow-comparison-20260912.json.
Reproducer work/runtime-20260912-video-comparison/prove_meet_sister_native_flow.py.
Do not mistake this for full-engine simulation: resource payloads are empty,
engine calls are stubbed, and scheduler/refcounts/camera/audio are not emulated.
It independently checks the script branch and cleanup order, including the
ignored Hero-acquire result and unconditional post-macro continuation.

Meet Sister further RE found an additional real adapter gap: binding+0x18 carries
active-entity Flags, not an ID. Messenger=1, Sister=0; constructor CE1110 stores
it at active-script+0x3C. Native deactivation with its trailing bool=true pauses
flag-1 scripts (FuncPaused+0x28), while flag-0 follows finish/save; reactivation
flag-1 unpauses while flag-0 reinitializes/reschedules. Existing Forge binding
registration zeroes this field for all entities. Exact bytes and detailed branch
limits in meet_sister/runtime_evidence/binding-flags-native-bytes-20260912.json
and PORT_REVIEW_2026-09-12.md. Reference bind contract updated with flags.
Native emulator expanded to root Main: 15 native/Lua scenario comparisons pass,
including actual binding stores. No installed or runtime source change yet.

Background-port checkpoint: Meet Sister now has nine reference tests, including
replay of all 15 archived native x86 scenarios. Combined with the external skip
observer test (384 native eligibility cases), 10 tests pass. Package remains
reference-only until explicit resources, one-attempt Hero acquisition and binding
Flags can be preserved by a runtime adapter. User has not authorized a new launch
time; game was not launched. No installed files changed during port preparation.

## Offline runtime implementation: optional binding flags

Added `Quest:AddEntityBinding(name, path, flags)` in the runtime checkout.
The third operand is `sol::optional<int>`; omitted/nil stays zero. Finalization
copies the value into binding+0x18 without changing the native structure layout.
Existing bindings keep their previous behavior; the Meet Sister adapter can now
request Messenger=1 and Sister=0 once its remaining resource operations exist.

Verified the runtime-selected vtable 0x12EA57C uses factory 0xE7ED60. Its actual
installed bytes load +0x18 at E7EDBB and call CE1110, whose actual bytes store
Flags at +0x3C. Six Unicorn executions preserve 0, 1, 2, 3, 0x80000000 and
0xFFFFFFFF through both functions, with balanced return stacks and unchanged
UID operands. Allocator, Init, string/task and refcount helpers are stubbed;
this is forwarding/constructor proof, not a scheduler lifecycle test.
Reproducer: `work/runtime-20260912-video-comparison/prove_binding_flags_factory.py`.
Evidence: `runtime-binding-factory-emulation-20260912.json`.

A standalone C++ harness extracts the actual changed method, binding-info
structure and sol registration, then compiles with this checkout's sol and
Lua sources. Omitted, explicit nil, 1 and 0 all pass. It substitutes only the
quest holder/logger; it does not invoke in-game FinalizeEntityBindings.
Reproducer: `work/runtime-20260912-video-comparison/check_binding_flags_lua.py`;
its generated directory includes the executable, source and result.log.
Release x86 build, control ABI audit, and 10 focused reference/observer tests
also pass. Validation hashes: `binding-flags-offline-validation-20260912.json`.

No shared API manifests or overlays regenerated: coordinate that metadata update
with the converter session before regeneration. This is an offline source/build
change, not a deployment or a complete Meet Sister port. Explicit resource
identity, single-attempt acquisition, and caller-owned map lifetimes remain
required. All 25 installed v23 manifest files still match; game remains closed.

## Offline explicit resources and staged Meet Sister package

Implemented `LuaRetailResources.h` in the runtime checkout and registered
`Quest:WithRetailResources(callback)`. The callback receives explicit, stable
resource/map/movie locals. `TryAcquire` makes one native attempt; Lua owns its
retry and termination decisions. `SetActor` copies even an empty resource.
`RunMacro` borrows the caller's map and movie, passes nil flags/input plus the
explicit setup/skippable operands, and leaves cleanup to the caller. `Frame`
calls the native frame operation without an additional termination query.
Existing AcquireControl and RunCutsceneWithSetup are unchanged.

Explicit ReleaseResource/DestroyActorMap/DestroyMovie preserve the native
normal-path order. Callback exit/error closes any remaining locals in reverse
creation order, unpauses if necessary, and invalidates the scope. Retaining the
Lua scope does not defer native release to GC; calls on a closed scope fail.
This is an error fallback, not an added native cancellation branch. The reviewed
port explicitly closes all locals on its normal and cancellation branches.
Mixing this scope with legacy cached control/movie helpers is not reviewed.

`reference/runtime_adapter.lua` bridges the native-reviewed reference to actual
runtime API names. A disabled package is staged at
`refs/script_recovery/reconstructed/MeetSister/FSE`: root and both entity entry
points, reference, adapter, empty legacy quests table and disabled retail
allocator override for QS_MeetSister. Neither game files nor quest activation
were changed. The reference remains canonical; package copies currently match.

Validation:
- Release x86 build passes (retail-resources-build.log).
- Actual C++ resource header and sol registration compile against vendor Lua;
  smoke checks single failed acquisition, empty actor-map copy, map ownership
  after local release, stable addresses through 1,000 allocations, invalid-kind
  and released handles, Lua error cleanup, and retained closed-scope rejection.
  Reproducer source: work/runtime-20260912-video-comparison/retail_resources_smoke.cpp;
  build/run: binding-flags-lua-check/resources.cmd in that directory;
  result: binding-flags-lua-check/resources-result.log.
- 15 native script scenarios match through the actual Lua adapter, and again
  through staged package Init/Main entry points. Engine/Quest calls are doubles.
- Native 8ABD10 assignment and 7E74D0 destruction were executed for five actual
  reference-count cases: empty, shared-info assignment, replacement of last old
  reference, local-before-map and map-before-local release. Actual arithmetic
  selects the deleter/free calls; those two engine operations are stubbed.
  Reproducer: prove_resource_refcounts.py in the same work directory; evidence
  native-resource-refcounts-20260912.json. This is not live actor destruction.
- 12 focused unit tests, existing control ABI audit, and all 122 New Oakvale
  package fixtures pass. Build/source/package hashes are recorded in
  resource-adapter-offline-validation-20260912.json.

Remaining: shared API metadata coordination before regeneration; engine/scheduler
and in-game lifecycle testing. The common LuaEntityHost Main entry has its existing
pre-entry termination guard; native-body replay exercises entered bodies and does
not prove scheduler behavior before entry. No global lifecycle guard was changed.
The installed build remains v23; New Oakvale Escape/fade/audio capture is still
pending the user's launch timing.

## Additional audio backend evidence while live capture is deferred

Installed bytes at C565F0 resolve GetCurrentDirectoryA (IAT143FD78), append
/OpenAL32.dll, then /wrap_oal.dll, and try LoadLibraryA. A later branch uses
GetSystemDirectoryA (IAT143FEBC) and /OpenAL32.dll. At inspection, neither DLL
exists in the game directory, Windows/SysWOW64, or Windows/System32. This
weakens treating the earlier OpenAL listener path as the active-path explanation;
it does not establish which audio backend is active or why speech attenuates.
The game's current directory must also be established for a live-path conclusion.

C57460 uses the loader at 143BDB0, tests module handle+4 and invokes C565F0.
The external read-only observer now samples that loader's module handle,
+104 built-in-proc lookup flag, and +10C attempted byte. It still records the
actual audio-manager vtable/CreateEmitter target and full startup module list.
No hooks, key handling, DLLs, mixer settings or installed Lua were changed.
Byte evidence and six file-presence checks: audio-loader-path-20260912.json.
Actual backend selection and 2D emitter handling remain unverified until capture.

## Staged-package registration/API validation

Added `tools/script_recovery/validate_meet_sister_package.py`: all seven Lua files
compile, the staged native override is disabled, legacy registration is empty,
entity identities/files/IDs are consistent, reference copies match, and the
adapter's 12 Quest plus 12 RetailResources method names are present in the actual
runtime registration source. Result: staged-package-validation-20260912.json.
This is a binding-presence/structure check, not shared-metadata or playback proof.

The generic validate_reconstructed_package CLI was attempted first. It requires a
legacy quests.lua entry and returned an early report without summary, causing
KeyError('summary') when printing. Its all-non-Quest-receivers-are-entities model
also does not fit this explicit-resource adapter. It was left unchanged; do not
add a second legacy quest identity merely to satisfy that checker. Existing
New Oakvale validation still passes all 122 fixtures.

Final installed-file check after this work: all 25 v23 manifest entries match.
No runtime build or staged Meet Sister file was deployed. API coordination note:
work/meet-sister-runtime-api-coordination-20260912.md.

## Native lifecycle execution and corrected script-audio operand route

Meet Sister: executed actual existing-script activation CB8960 and deactivation
CB88B0 for flags 0/1/2/3 and both trailing deactivation booleans. Twelve cases
confirm Flags bit 0 selects pause/resume rather than save/reinitialize; actual
F359F0 writes FuncPaused. Lookup, Save, reschedule and notification calls are
stubs. This does not prove native scheduler or live region streaming behavior.
Reproducer: work/runtime-20260912-video-comparison/prove_binding_lifecycle.py;
evidence: meet_sister/runtime_evidence/native-binding-lifecycle-20260912.json.

Audio correction: an intermediate commentary incorrectly called the emitter's
THIRD boolean soundIn2D. The script action+72 actually reaches the FIFTH emitter
operand. Corrected the descriptor experiment's labels to third_boolean and
fifth_boolean. Do not use the earlier interpretation that scripted soundIn2D
clears descriptor spatial+10.

The new argument-route experiment executes the call site at 716C46, the operand
construction at 6E362C, 66B300, 66B080, 836810 and AABF60 through AAC0BE. The
prerequisite lookup portion of 6E3580 is skipped with explicit synthetic state;
actor/emitter and global RNG-object lookups are stubs. Native RNG arithmetic,
CSoundInit mask 0x8040 and descriptor helpers execute. It proves:
- action+72 -> argument 6 of 6E3580 -> argument 6 of 66B080 -> argument 5 of
  emitter PlaySample. The third emitter operand is zero on this script path.
- With emitter+18=1 (constructor default), descriptor+10 stays 1 for BOTH
  script soundIn2D values. Descriptor+24 receives script soundIn2D.
- soundIn2D=false copies the emitter position; true copies manager+74/+78/+7C.
  This proves descriptor construction, not mixer interpretation or audible output.

Two script-route cases and eight independent descriptor-operand combinations pass.
Reproducers: prove_script_speech_arguments.py and prove_speech_emitter_descriptor.py
in work/runtime-20260912-video-comparison. Evidence:
new_oakvale_intro/runtime_evidence/native-script-speech-arguments-20260912.json and
native-speech-emitter-descriptor-20260912.json (corrected numbered operands).

The installed engine construction path at 9A68A2 creates manager A3EF00/vtable
129D2C4; CreateEmitter slot+10 is A3ECB0, constructing AAC580/vtable129E6EC.
Emitter slot+C is AABF60. Manager+74 position defaults to (0,0.5,0).
The external observer now records manager bytes/that position and core identity
only after verifying this manager vtable. Construction/dispatch byte proof:
audio-manager-emitter-dispatch-20260912.json. No new game hook was added.

Read-only Windows output query used IMMDeviceEnumerator default render endpoints
and IAudioClient::GetMixFormat, without initializing a stream. At this inspection,
console, multimedia and communications all select Speakers (Realtek(R) Audio),
2 channels, 48 kHz, 32-bit mix format, mask 0x3. This is current Windows default
configuration, not proof of which endpoint Fable selected in an earlier capture,
nor of whether physical headphones are connected. Source/log:
work/runtime-20260912-video-comparison/read_audio_endpoints.cpp and
read_audio_endpoints.log. No audio/device settings were modified.

No visible or audible bug is claimed fixed by these offline investigations.
Installed v23 and New Oakvale Lua remain untouched; live capture still pending.

### Background continuation: native audio geometry and bounded sample capture

Executed the native C1C49D-C1C5D9 position-update slice with 20 combinations of
relative flag, listener yaw and translation. Relative=true submits the source
0.5 metres in front of the listener in every tested pose. This executes native
coordinate conversion, matrix construction/inverse and position branches;
lock, voice lookup and backend submission are stubbed, and the eligibility
prefix is not executed. It does not prove live path selection or audible gain.
Reproducer: work/runtime-20260912-video-comparison/prove_audio_relative_position.py.
Evidence: new_oakvale_intro/runtime_evidence/native-audio-relative-position-20260912.json.

The external observer now takes bounded, non-atomic emitter/sample snapshots at
most twice per second: 128 emitters, 256 samples, 20ms checked between reads.
It validates manager/emitter/sample vtables, retains raw object bytes, and records
sample handle, bank pointer, sample ID, spatial byte and script soundIn2D byte.
Invalid/deleted objects, cycles and exceeded limits produce partial results;
no writes, injection or engine calls. A single OS read can exceed the budget,
and short-lived samples can fall between scans. These are observations of
registered sample records, not proof that a voice is audible at that instant.
Layout evidence: native-audio-sample-layout-20260912.json (A3ECB0/AAC580,
AACCC0/AAB850 and registration bytes). Sample payload+18 is an ID, not a pointer.
Six observer tests pass, including all 384 native skip predicate cases and
synthetic list traversal/deletion/cycle/limit/class checks; py_compile passes.
The enhanced live observer has not yet been run against the game.

Rechecked all 25 installed deployment-v23 hashes: exact match. No game launch,
installed asset change, runtime hook or converter change in this continuation.
Next live capture should correlate sample flags/IDs with the affected spoken
line and camera turn; Father Escape still needs the deferred restored-v23 run.

### Continued port preparation: staged Meet Sister through actual C++ wrapper

Added an offline integration harness that runs the staged entity Init/Main Lua
and adapter through actual vendor sol/Lua and LuaRetailResources.h, with mocked
engine boundaries and Quest operations. All 14 entity scenarios match all 255
archived native trace events. Cleanup leaves no mock maps, movies, local resource
ownership or nonzero mock reference counts. Standalone C++ error/ownership smoke
also passes. This closes the prior separation between Lua replay and C++ wrapper
tests, without claiming engine/scheduler playback proof. No port/runtime fix was
needed; no installed files or converter files changed.

Details and reproduction commands are in the Meet Sister PORT_REVIEW; provenance
is meet_sister/runtime_evidence/cpp-staged-port-integration-20260912.json.
Root bindings remain covered by their existing separate tests. Meet Sister stays
disabled/uninstalled, and live New Oakvale Escape/audio verification is pending.

### Audio listener capture and withdrawn-hook review

Extended the external observer to record the native audio listener position,
forward and up vectors in game coordinates, with raw core/mixer bytes. It checks
core vtable 12B1E24 and mixer vtable 12B2748 before interpreting their fields;
C0A590 returns core+38 and C1C49D reads mixer+8/+14/+20. Mixer+885 is recorded as
one raw update gate, not labelled proof that position submission happened.
Read failures stay inside the listener result. This is non-atomic observation;
no engine locks or writes are taken. Native class/getter/read bytes are archived
in native-audio-listener-layout-20260912.json. Eight observer tests pass, including
relocation and rejected-class/read-failure handling; Python compilation passes.
No live listener/sample capture has occurred yet.

Reviewed the withdrawn v24 draw insertion against installed executable bytes.
It copies exactly `55 8D 6C 24 8C` (push EBP; lea EBP,[ESP-74]) at 6495A8,
two complete instructions with no relative operand, then resumes at 6495AD.
The saved source uses a thiscall original with one explicit argument and a
fastcall bridge whose unused EDX parameter leaves that argument on the stack;
the native function exits with ret 4. This static review did not identify an
instruction-splitting or declared stack-cleanup mismatch. It is not proof of
binary execution safety, and does not exonerate v24 or explain the user's Escape
failure. No hook was restored. The restored-v23 input/eligibility comparison
remains the next live check; do not describe the Father issue as fixed.

### Offline queue execution: clean builds and cancellation fixes

Task 1 complete. Promoted C++ harnesses to tools/script_recovery/runtime_checks
and added run_meet_sister_runtime_checks.py. Builds vendor Lua from C and both
harnesses from C++ in a new output directory; no existing objects/response files.
MSVC discovery, exact input/compiler/output hashes, compiler version logs,
command status and source-change detection are included. Setup failures return
1 and retain reports. A deliberately wrong native event returns 1; reusing an
output directory returns 1 without overwriting the existing report. Successful
fresh build: work/meet-sister-clean-runtime-check-20260912-d/result.json, with
14 scenarios/255 events and ownership smoke passing. Earlier a/b setup failures
were retained; c passed before adding explicit exception-to-exit-1 handling.

Task 2 found and fixed two source-only cancellation mismatches:

1. Father intro lacked the termination query at DB895A (Xbox)/DB89D4 (PC)
   before tutorial display, and DB8A3D before counter creation. Added both.
   Preserve retail's earlier DadFinishedIntro write, unmute and camera reset:
   there is no immediate post-macro guard at that point. Eight native cases
   (normal/pre-card/post-click/wait cancellation across PC/Xbox) execute actual
   DB8906-DB8A83 branches and both termination helpers; engine calls are doubles.
   The actual Lua closure failed all eight comparisons before the change and
   passes after it (normal differences were missing queries). Native evidence:
   native-father-intro-gates-20260912.json; reproducer prove_father_intro_gates.py
   under work/runtime-20260912-video-comparison; focused test:
   tools/script_recovery/test_father_intro_cancellation.py.

2. Bully RUN1 cancellation included an extra FixMovieSequenceCamera(false).
   Native DBCC00 skips the normal DBCC6F camera-unfix call; removed that extra
   call from the cancellation branch. Six executed post-RUN1 cases cover both
   teddy states and no termination/RUN1 termination/RUN2-or-dummy termination.
   The native tail still awards/removes after termination inside RUN2/dummy;
   no extra gate was added there. Before the change exactly the two RUN1 cases
   failed actual-Lua event comparison; all six now pass. Native evidence:
   native-bully-completion-gates-20260912.json; reproducer
   prove_bully_completion_gates.py; test_bully_completion_cancellation.py.
   Macro/map lifetime is explicitly outside that test; engine/deed/destructor
   operations are doubles. This is not proof of rendered camera behavior.

Resolved a prior review uncertainty: native F35B30 calls entity vtable+C to get
its parent and jumps to CB7940, which reads parent+2C active thread's byte+5.
Father's native getter DAC360 returns entity+14. LuaEntityHost::GetParentScript_Stub
returns the same parent base used by Quest:IsActiveThreadTerminating. No new
termination API or global host change is justified for this forwarding path.

Reviewed trace updates: Father's normal intro adds exactly two termination
queries; Bully's two RUN1 cancellation traces lose exactly one camera-unfix call.
Other affected traces change only source hashes. All 122 package fixtures pass,
zero warnings/errors: work/father-bully-gates-validation-20260912.json.
All 25 installed v23 hashes still match. Fixes are in reconstructed source only;
no game launch, install, converter or shared metadata change. These cancellation
fixes are not claimed to fix Escape input, visible disappearance, fades or audio.
The wider task-2 audit is still in progress.

### Offline queue task 3: automatic Escape/fade report complete

Added tools/script_recovery/analyze_retail_cutscene_observer.py. It reads the
observer JSONL, recomputes the native eligibility predicate from raw GUI bytes,
decodes observed fade fields, and associates Escape transitions with samples
before/after the input timestamp (bounded to 250ms). Tick wrap and repeat presses
are handled. Missing/unreadable samples remain unknown; malformed/truncated
records produce errors and CLI exit 1. No input is explicitly not a successful
Escape test. Eligibility does not establish game input consumption, and raw fade
state does not establish visible pixels. Reports retain these limitations and
capture SHA-256; output paths cannot overwrite previous reports.

Six focused tests pass; actual CLI synthetic complete/truncated captures return
0/1 respectively. Artifacts: work/observer-report-check-20260912/.
Usage: python tools/script_recovery/analyze_retail_cutscene_observer.py CAPTURE.jsonl
--output NEW_REPORT.json. No real new capture was available or claimed analyzed.
Task 3 marked complete; tasks 4-7 remain queued and the wider task-2 audit remains
in progress. The two source cancellation fixes above remain uninstalled.

### Speech audit: proved numeric-zero boolean mismatch and added audio correlation

The source inventory contains 13 direct Speak call sites, 17 conversation creates,
27 conversation lines and 7 macro calls; Converse and StartAmbientConversation
are unused by this package. Source/hash inventory:
speech-callsite-inventory-20260912.json. This inventory does not claim every
native speech call has been matched; the broader operand audit remains open.

Found and fixed four source calls: two in NOVI_Guard and two in NOVI_Villager
passed `AddNewConversation(me, 0, 0)` into LuaQuestState's C++ bool parameters.
Actual vendor sol/Lua experiment: (0,0) arrives as (true,true), whereas
(false,false) arrives as (false,false). Native calls DAC8F4, DAD8FC, DAE4CC and
DAE69E pass zero operands. Guard's first site uses EBX, zeroed at DAC76B with no
local reassignment along the reviewed path; the other three use immediate zeros.
Changed only those four Lua calls to false,false. Do not claim this explains the
reported quiet/distant line: that line has not been identified or captured.
Evidence: conversation-bool-operands-20260912.json; exploratory C++ reproducer:
work/runtime-20260912-video-comparison/prove_sol_conversation_bools.cpp.

Promoted the check into runtime_checks/conversation_bool_operands.cpp and the
clean runner. It evaluates actual operand expressions from all 17 source call
sites through vendor sol. Fresh build in
work/runtime-clean-check-conversation-bools-20260912 passes that check, the
ownership smoke and all 14 Meet Sister cases/255 events. Deliberately replacing
one generated pair with (0,0) returns exit 1. Five fixture traces change only
conversation arguments from numeric zero to false; other affected traces only
refresh source hashes. All 122 package fixtures pass, no errors/warnings:
work/conversation-bool-package-validation-20260912.json.

Added analyze_retail_audio.py to the observer report. It correlates registered
sample identities, flags, emitter positions and listener poses, reports turns,
translation, overlaps, partial scans and gaps, and separates emitter distance
from the head-relative source distance. Partial/omitted scans do not establish
absence; missing poses do not silently bridge rotation. IDs can be reused, so
first/last observation is not called lifetime, and overlapping records are not
assigned to a spoken line. Five focused audio tests plus six existing analyzer
tests pass. No real new audio capture was available or analyzed.

All changes remain offline/source-only. No installed assets, runtime DLLs,
converter code, speech volume or device settings changed. Task 4 is in progress;
its capture-correlation component is implemented, full native operand comparison
is not yet complete.

### Offline tasks 5 and 6 completed: entry contract and release review

Native coroutine entry A446A0 filters active+5 before active virtual slot+10.
Meet Sister's active wrapper E29220 and Forge's E7EE70 both dispatch entity Main
then mark active+5 finished. Four native cases (two wrappers x initial termination
state) pass through the actual trampoline/wrapper/query chain. Already-terminated
tasks do not enter Main; admitted cases see false from the same parent/current
active-task query. CB7959 supplies the current-task association in native source;
that association is synthetic in the execution test. Actual fiber switching,
concurrent termination and region scheduling remain unexecuted. This provides no
basis for changing the common LuaEntityHost guard. Detailed bounded contract:
meet_sister/runtime_evidence/SCHEDULER_ENTRY_REVIEW_2026-09-12.md and
native-scheduler-entry-20260912.json. Reproducer:
work/runtime-20260912-video-comparison/prove_meet_sister_scheduler_entry.py.
The test uses Meet Sister's native parent getter E29010.

Prepared review-only metadata and release materials with
`tools/script_recovery/prepare_meet_sister_release.py --forge-root
D:/Code/ForgeFSE-retail-shadow --output refs/script_recovery/meet_sister/release_review_20260912`.
The output contains all seven staged Lua hashes, runtime-source/dependency hashes,
validation, an AddEntityBinding flags amendment and 13 proposed API additions,
plus resource-contract examples, deployment prerequisites and exact rollback
procedure. Hashes/identities verify. It deliberately contains no deployable DLL,
retains disabled activation, and warns against replacing the shared override file
with the standalone staging configuration. The metadata proposal was not applied;
new receiver/callback handling still requires converter-owner coordination.

Tasks 5 and 6 are complete within their offline definitions. No game launch,
installed file, shared manifest, SDK overlay or converter change occurred.
Tasks 2/4 retain broader audit work; task 7 target selection/recovery is next.


### Task 7: Maze Research native entity and UNLIMBO recovery

Selected Maze Research after comparing its two-entity surface with Guardian
Trophy Dealer and Dragon Boss Fight. Archived both native entity bodies,
Init/Persist and UnLimboSword in maze_research/runtime_evidence, with executable
hash, reachable instructions, branch edges, vtables and literal strings.
PORT_REVIEW_2026-09-12.md records addresses, dependency gaps and seed differences.

Six native-instruction Init/UNLIMBO tests pass. Retail initializes a shared
UNLIMBO flag, schedules a parent task and reveals the sword only after that flag.
The seed instead reveals it before the macro and omits Init. Native cancellation
prevents both reveal calls, including simultaneous flag and cancellation delivery.
Engine/map/frame behavior is doubled; actual macro flag production is untested.
HistoryBookcase also has post-click termination gates absent from the seed.

Runtime CreateThread exists, but the exact shared flag map is not exposed by
RetailResources.RunMacro. RunCutsceneWithSetup uses a local map distinct from the
member map read by GetCutsceneFlag. Recover macro commands and design explicit
map/task lifetime before staging a runnable replacement. Task 7 remains in
progress; no seed, converter, runtime implementation or installed files changed.


### Marathon milestone completed: Maze Research disabled port

Recovered installed SAINT/REDEEM macro flag writes and skip ordering, plus seven
unique authored placements. Executed full native HistoryBookcase (11 cases) and
EmptyGrave (20 cases), complementing six Init/UNLIMBO cases. Reference and staged
Lua/adapter match the 31 body traces and five UNLIMBO outcomes. All seven staged
Lua files parse; activation and disposable acknowledgement remain off.

Runtime source now owns transient RetailFlags/retained things per quest and passes
the same flag map through RunMacroWithFlags. No cross-VM Lua objects are passed.
Actual two-VM C++/Lua tests verify shared identity, last-owner destruction and
borrowed/owned/nil usable arguments. SetThingAsUsable's shared_ptr parameter was a
confirmed bug for raw me; changed to a synchronous raw pointer. The exact current
method body is compiled in the regression harness. Release x86 DLL build passed;
source SHA256 2CEA2E887B1646397A13C3B6ED936DFCE5C28944486B45A9392C5A91AFC5352B.

Validation: work/runtime-maze-shared-state-20260912/result.json; eight Maze Python
tests; work/maze-package-validation-final-20260912.json; all 122 New Oakvale
fixtures still pass. Installed root/FSE DLLs remain exact v23. Full review and
unapplied six-method API proposal are in maze_research/runtime_evidence.

No game launch/install, converter/seed edits or shared metadata regeneration.
Real scheduler/fibers, streaming, resource internals and macro playback remain
live acceptance boundaries. The native replay doubles those engine operations.
This completes the bounded offline task-7 port milestone, not in-game acceptance.


### Direct dialogue operand audit completed

Recovered 102 native Speak and 18 conversation-creation instructions across all
13 speaking entity inventories, including Barrel Man's four-way jump table.
Every Speak has flags false,true,false; every conversation false,false. Native
register constants are joined across control flow; six discriminating analyzer
checks pass. Eighteen native conversation instructions correspond to 17 shared
Lua call sites (Barrel Thug merges two identical creation paths).

Actual runtime Speak registration + 13 source expressions/two method overrides
compile and pass with real sol/Lua. A changed sound2D operand is rejected. All
17 conversation bool conversions also pass. No new mismatch or audio fix claimed.
Review: runtime_evidence/DIRECT_DIALOGUE_OPERAND_REVIEW_2026-09-12.md. Native
helper 7E7390 is six arguments/ret18; the archived seven-argument decompile is
incorrect. No converter artifact or installed file changed.

AddLineToConversation and macro speech remain beyond this completed direct-call
subtask; perceived loudness still needs the actual line/capture.


### Conversation-line audit completed

36 native AddLine calls all pass false for the line boolean. The analyzer handles
Villager's nested string calls and Affair Man's shared participant-prefix branch;
all 36 bound-register identities are checked. Four native Affair Man near-line
branch tests preserve the normal speaker order. Eight native API marshalling
cases prove empty listener values; a null C++ listener reproduces the 89076C fault.

Fresh C++/Lua checks compile the actual helper/body/registration and pass all 27
source line expressions, borrowed/owned participant conversion, nil listener,
explicit/default booleans and invalid nil speaker. Clean final report:
work/runtime-conversation-lines-final-20260912/result.json. No new runtime or
port mismatch was found and no installed file changed. Full review:
CONVERSATION_LINE_REVIEW_2026-09-12.md under runtime_evidence. Authored macro
speech and live affected-line correlation remain outside this completed subtask.


## Authored cutscene speech milestone

Completed the remaining inventoried speech route: nine installed CCutsceneDef
assets, 28 speech commands, 29 native scalar instruction assertions with raw
bytes and source/asset hashes. Reproducer: audit_oakvale_macro_speech.py; evidence:
new_oakvale_intro/runtime_evidence/native-macro-speech-operands-20260912.json and
MACRO_SPEECH_REVIEW_2026-09-12.md. Native macro conversation flags intentionally
differ from direct quest flags; Lua invokes the native interpreter and preserves
those defaults. No additional speech operand mismatch found. No installs or
launches. Quiet/distant output and rendered fades still need live correlation.
Offline queue task 4 complete within these limits.


## Refreshed two-port release review

Prepared refs/script_recovery/retail_port_release_review_20260912 with current
Meet Sister and Maze Research source manifests, passing package validations,
unapplied metadata proposals and runtime provenance. All 120 inputs of the
six-binary runtime proof remain current. Independently verified generated
artifact hashes, both seven-file Lua packages and both runtime source snapshots.
Reproduce with prepare_retail_port_reviews.py (new output directory required).
README includes transient shared-state limits, deployment prerequisites and
rollback. Existing offline DLL is identified, not bundled or claimed cleanly
reproduced. Both installed DLLs remain exact v23. No activation or live test.


## Father/Bully pre-macro cancellation fixes

New native/Lua comparisons confirmed missing entry gates. Father now acquires
Hero and checks cancellation before movie setup, retaining Hero through movie
cleanup. Six native entry cases and eight post-macro cases pass. Bully now
checks cancellation after each successful acquisition and in both text-selection
branches before movie setup; ten native entry cases and six completion cases
pass. See FATHER_BULLY_ENTRY_CANCELLATION_REVIEW_2026-09-12.md in New Oakvale
runtime_evidence for addresses, failure-before-fix results and limits.

All 123 package fixtures pass with zero warnings/errors. Existing Bully event
arrays are identical when termination-query events are excluded. Only related
fixtures/traces changed; no C++ changes or installation. Current clean runtime
proof directory: work/runtime-father-bully-acquired-final-20260912/. Prior release
review snapshots identify earlier Lua sources and remain historical. Live
Escape/fade/audio acceptance and legacy wrapper map-lifetime equivalence remain
separate from these proven entry/cleanup fixes.


## Legacy map cleanup milestone

Twelve native constructor/destructor cases confirm compatible headers but an
invalid actor-destructor fallback for populated boolean maps. Runtime now
requires setup-map cleanup APIs before allocation and removes that fallback.
Both APIs exist in installed v23; this is not a demonstrated live bug cause.
Release x86 build and six clean C++/Lua checks pass. Evidence/reproducer and
normal/cancellation ownership limits: LEGACY_MACRO_CLEANUP_REVIEW_2026-09-12.md.
New source DLL hash recorded in map-cleanup-build-provenance-20260912.json; both
installed DLLs still exact v23. Exception safety remains an explicit separate
limitation. Proceeding to staged-port persistence contracts.


## Persistence contract milestone

Recovered native Maze OnPersist (SwordTaken then BookRead, false defaults) and
Meet Sister's empty callback. Twenty-six native cases pass. Added a seventh
clean C++/Lua binary that loads the actual staged Maze root/adapter and compiles
the current PersistTransferBool method/registration; all 24 native Maze field
transfer cases match. Seven runtime binaries pass in
work/runtime-port-persistence-20260912/. No new persistence implementation
change. See maze_research/runtime_evidence/PERSISTENCE_REVIEW_2026-09-12.md
for transient state and real save/load lifecycle limits. Proceeding to next
supported port selection/recovery.


## Next-port recovery milestone

Selected QS_GuardianTrophyDealerInfo and recovered its root/task contract,
entity allocator/vtable/full byte range, three installed macro assets and ten
unique placements. Ten native WaitForPieceOver cases match a non-registered
reference. See guardian_trophy_dealer_info/runtime_evidence/PORT_RECOVERY_2026-09-12.md.
Full GTDI_Maze entity replay and explicit resource speech routing are the next
implementation gate; no executable/activated entity port is claimed. Seed,
converter outputs and installed game remain unchanged.

## Guardian encounter/resource milestone

Used converter native_call_setup_ir and native_resource_lifetime read-only to
recover 43 resource/movie/macro records. Full Main CFG passes movie balance and
identity checks across 1,121 instruction/state pairs; five bad event mappings
are rejected. The shared movie destructor uses either ESP+68 or ESP+78, depending
on branch. Implemented first hit/talk encounters and repeat bodies in the
non-registered reference: 27 first-encounter plus 22 repeat native cases match,
alongside the existing ten watcher cases. No runtime source change or build.

Retail's post-macro check guards all quest progress; cancellation during any
macro still cleans movie/map/Hero before releasing Maze. Successful completion
does not include the seed's extra global teleport-enable call. Repeat hit
reacquires Maze, repeat talk does not; hit rechecks health between its two lines.
All 11 required text keys and three macros resolve. Quest-card OBJECT exists,
but generic schema decoding fails; raw payload/error archived, not certified.

Details and commands: guardian_trophy_dealer_info/runtime_evidence/
ENCOUNTER_RESOURCE_REVIEW_2026-09-12.md. Next: entity entry/predicate/outer-loop
replay, explicit resource Speak/task/Thing adapter proof, quest-card components,
then disabled package and actual C++/Lua integration. Both installed DLLs verified
exact v23 again; no launches, installs, seed or converter-file edits. Fade pixels,
Father Escape and perceived loudness remain live acceptance items.

## Guardian full body and explicit dispatch milestone

Composed reference Init/Main after 28 full native body cases matched all 1,377
events. Existing body/watcher comparisons still pass: 87 native/reference cases.
Repeat hit and talk may both execute before the same outer-loop frame; Heal Life
ability 14 is excluded by the native short-circuit hit predicate. Initial frame,
resource gates, property flags and HUD marker match native operands.

Added RetailResources.Speak and IsPerformingScriptTask in the runtime header.
Ten native wrapper cases and actual-header sol/C++ checks prove dispatch through
the selected explicit resource, including distinct expert identities and empty
handles. Seven runtime binaries and Release build pass. No cached speech path,
audio flags or existing script was changed. New uninstalled source DLL hash:
58fcea9de937cc1f1302d5f1d6331105aef397116c013daa55c64b9013614c20.
Both installed DLLs remain exact v23. Shared metadata proposal is unapplied.

Native Main also installs condition 12C32F0; CDF120 evaluates bound IsAlive.
Five scheduler cases prove pre/post-resume failure behavior. The required
reference register_alive_condition operation still needs an actual adapter
route; do not assume native frame yielding or post-return host fallback installs
it. Next: explicit Thing lifetime support, native condition registration proof,
then complete C++/sol adapter, quest-card components and disabled package.
Evidence: guardian_trophy_dealer_info/runtime_evidence/
ENTITY_AND_DISPATCH_REVIEW_2026-09-12.md. Runtime report:
work/runtime-guardian-dispatch-20260912/result.json. No converter file edits.

## User live acceptance after v23 launcher run

User reports: "Father escape key works again, bulley disappears when scene
skipped like retail, so I guess that's good enough. Probably supposed to be a
fade, but they don't seem to work anyways, even in retail".

Record Father Escape as restored and Bully's skipped-scene disappearance as
accepted by the user for observed retail parity. Both installed DLL hashes were
rechecked after this report and remain v23:
c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31.
The game was launched through FSE_Launcher.exe; no new source build was installed.
This is user-observed acceptance, not a new trace/video or fade-rendering proof.
Missing fades remain an unresolved retail observation; do not invent a run-off
or alter the authored skip branch to conceal it. The separate quiet/distant
dialogue report is not resolved by this acceptance. Continue offline porting.

## Guardian native condition and actual adapter milestone

Implemented scoped Thing output/lookups/query/destruction and the explicit
Quest:RegisterBoundAliveCondition binding. Native F35B10 clones the temporary
condition; the runtime helper releases its local Thing and leaves the clone
owned by the task. Guard uses actual current-task slot parent+2C, without changing
the shared legacy base layout or adding implicit scheduler behavior to scripts.
Sixteen native Thing ownership cases and eight condition registration/replacement
cases pass. Actual sol/C++ checks cover ownership, invalid handles, absent task
and synthetic registration-error cleanup.

The real Guardian reference plus runtime_adapter.lua now matches every native
Init/Main event through actual RetailResources and the condition binding:
28 cases, 1,377 events. All nine runtime binaries and Release build pass in
work/runtime-guardian-adapter-20260912 and
work/runtime-guardian-adapter-release-20260912. The initial duplicate test fixture
symbol was fixed and the clean suite passes; no runtime bug was inferred from it.

Resolved required quest-card components using documented tag/link/back-reference
layout: OBJECT 3736 -> CQuestCardDef 12321, all 18 fields decoded, localized title
"Find the Archaeologist". Generic OBJECT decoding still fails on DieOffTimer's
oversized slice; no parser/assets changed. Registry still declares GTDI_Maze 69;
final override conflict checks remain before disabled-package assembly.

Next: actual-adapter root/watcher/persistence checks and disabled package.
Detailed evidence: guardian_trophy_dealer_info/runtime_evidence/
THING_CONDITION_ADAPTER_REVIEW_2026-09-12.md. Shared API proposal remains unapplied;
converter-owned files and installed v23 were untouched. Father Escape/Bully skip
user acceptance stands; quiet speech and missing retail fades are not claimed fixed.


## Guardian disabled package completed

Completed root/watcher/PieceOver integration and staged the six-file disabled
GuardianTrophyDealerInfo package. Found and reproduced a real root adapter bug:
AddEntityBinding received 0 as its script path. The actual C++ method/sol binding
queued "0"; adapter now supplies GuardianTrophyDealerInfo/Entities/GTDI_Maze and
flags 0. Preserved failing proof: work/runtime-guardian-binding-repro-20260912.

Final clean nine-binary checks pass: packaged entity 28 cases/1,377 events,
watcher ten native cases with real scoped Thing cleanup, eight Guardian native
persistence comparisons (32 including Maze), and actual root binding operands.
Two native Init/Main cases also match the packaged root; the validator passes
all 22 checks. Registry uses QS_GuardianTrophyDealerInfo and seed entity ID 69;
no conflicts in the checked installed/reconstructed profiles. No legacy entry.

Evidence: refs/script_recovery/guardian_trophy_dealer_info/runtime_evidence/
PACKAGE_REVIEW_2026-09-12.md and package-validation-20260912.json.
Compiled report: work/runtime-guardian-package-final-20260912/result.json.
Installed v23, runtime implementation, and converter-owned files unchanged.
The harness VM-teardown failure and the root-binding defect were separately
reproduced and fixed; neither was inferred to explain a user-visible game bug.
Live Guardian scheduling, streaming, save/load and audiovisual playback remain
unverified. Father Escape/Bully skip acceptance stands; quiet speech remains open.


## Extended marathon: VM ownership, actor conversion, audio and next port

Confirmed/fixed EntityScriptData VM teardown use-after-free. All six actual
manager lifecycle scenarios failed under ASan before the fix and pass after
explicitly releasing Lua references while the VM is alive. Default move order
preserves reference-before-VM replacement. Source-only runtime change.

Actual cross-VM quest-state checks pass: entity VM and root coroutine share
parent state, other quest namespaces stay isolated, Init resets same-name
state, and simulated persistence restores it after manager reset. This is
explicitly supplied lifecycle ordering, not a native save-file round trip.

Found StartAmbientConversation rejected borrowed bound NPC userdata. Fixed
its binding/method to accept actor objects; ASan then exposed shared_ptr-probe
buffer overread in GetScriptThingFromSolObject. Final checked Thing-pointer
conversion supports both borrowed and owned userdata. All argument/default/
failure checks and the 27 existing conversation-line expressions pass under
ASan. No native speech flag, volume or device setting was changed.

All three staged roots pass actual AddEntityBinding operand checks. Final
nine-binary regression and Release build pass; new candidate DLL SHA-256:
c9eaf045ae7fc8e780e7f077b92be700c76cb149f717052821bd00e8625e4f30.
It is UNINSTALLED. All 25 installed v23 deployment files remain exact matches.

Native full audio update passes 91 cases. Confirmed rear world-space source
reflection preserves distance; default relative speech stays 0.5m forward.
No audible-volume cause is proved. Guardian Sister root/persistence/entity
lifecycle reference matches 19 native cases; macro/text inventory resolves
14 keys and 19 placements, with two unresolved fan-exit marker names. Full
MazeAtTavern Main remains the next substantial port body to reconstruct.

Detailed review: docs/journal/2026-09/RUNTIME_EXTENDED_MARATHON_2026-09-12.md.
Final manifest: work/runtime-marathon-verified-20260912.json (18/18 checks).
ASan: work/runtime-marathon-state-conversation-asan-20260912/result.json.
Regression: work/runtime-marathon-verified-regression-20260912/result.json.
Guardian package revalidation: guardian_trophy_dealer_info/runtime_evidence/
package-validation-marathon-final-20260912.json (22/22 checks).
No converter-owned files, shared metadata, installed scripts or game assets
changed. Latest converter notes were read through Wife ally recovery; no
unverified draft was promoted. Father Escape/Bully skip acceptance stands.

## Original FSE compatibility add-on: local candidate prepared

User authorized a reversible add-on while keeping original FSE DLL unchanged,
with local testing before distribution. See
NEW_OAKVALE_COMPAT_ADDON_PLAN_2026-09-12.md and
NEW_OAKVALE_COMPAT_ADDON_RESULTS_2026-09-12.md.
Candidate: work/new-oakvale-original-fse-20260912/local-test-ready.
Original source compiled unchanged; separate sidecar owns New Oakvale hosts and
chains original registration. No installed files changed. Both builds, six actual
binary hook cases, loader fixture, 123 Lua fixtures and21 Python checks pass.
Actual Fable launch/gameplay still untested. Audio remains open; launch wrapper
backs up saves and starts the external observer. No distribution occurred.
