# Wasp combat converter recovery — 2026-09-23

Later follow-up: `WASP_CARD_HANDOFF_2026-09-23.md` resolves the handoff boundary
described below by accepting the actual guild quest card. The same v11 quest
Lua/DLL reaches persistent completion and stage 300 with Guardian Sister Info
active. This report retains the earlier direct-activation observations.

Continuation of `WASP_MOVIE_SCOPE_2026-09-23.md`, requested as a marathon.
All quest Lua changes come from the converter. Live bundle: **v11**; source save:
**adult_graduated**. Only `WaspBoss/WaspBoss.lua` differs from v10's 98 Lua files.

## Recovered native behavior

- Wrapped spawned-thread handler assignments now preserve `WatchForCutscene`
  (Main 0x00E0EA40) and `GuildmasterHelp` (DoMission 0x00E12580), including the
  native ownership-transfer removal already used for single-line assignments.
- CDefString conversion calls targeting **0x00415D70**, with a receiver in the
  global definition table, become `ReadGlobalGameDataString(offset)`. Offset
  **0xE3C** supplies drone definitions; **0xE40** supplies the queen definition.
  The new sidecar binding converts the current native token, copies its string,
  and destroys the native temporary. No creature definition is hardcoded in Lua.
- GSI pointers mistyped as CCharString pointers at the known script interface
  fields are normalized. A second receiver-annotation pass resolves thing calls
  after cached GSI calls receive names; reassignment ends a receiver's lifetime.
  DoMission now creates five drones at `QueenDepositPos1` through `5` using each
  marker's position, and the queen at `MK_WQ_STARTING`.
- A proven local CScriptThing's canonicalized Data/Info copy and retain become
  one Lua handle assignment. The queen health bar and kill predicate retain the
  queen instead of nil. Address aliases of filled thing return slots preserve
  the queen target for the hero's facing call after the queen cutscene.
- Consecutive script-name queries append to their output vector. Native
  **0x008A8570** appends at `[out+4]`, advances the end by 12, and does not clear
  the vector; **0x008ACD30** reserves capacity while preserving existing elements.
  The generated wave gate now includes HornetDrone, WaspChaser, and WaspAttacker.
  The recovery refuses to merge across branches, resets, or intervening uses.
- Empty bitmask cleanup guards left after native destructors are removed no
  longer evaluate an uninitialized temporary in the draft output.

Evidence input: `refs/script_recovery/wasp_boss/translation_unit_typed.json`.
Native instruction excerpts: `wasp_native_combat_evidence_2026-09-23.json`.
Sidecar source: `work/new-oakvale-original-fse-20260912/sidecar-abi-v2`.
Reproducible new binding patch:
`tools/script_recovery/sidecar_patches/novi-zz-definition-strings.patch`.
MSBuild Release/x86 succeeded. DLL SHA-256:
`70fcd204bb7dee232df5a6054160e9e4d16f3bdf332d4d0a352d09282d06cc2a`.

## Tests and first live run

Focused checks: **96 tests passed** (`work/wasp_marathon_focused_final.log`). The
behavioral tests execute draft and readable DoMission with each enemy group as
the sole survivor. They verify all five markers, dynamic definition reads, no
early queen, queen spawn after the last survivor dies, and entry to the health
phase. Another executable test verifies the cutscene's hero/queen facing pair.
Negative tests cover unproven string callees, mismatched copied fields, unknown
interface offsets, receiver reassignment, and vector lifetime boundaries.
Wasp draft and readable each smoke **10 files, zero problems**; **46/46 functions
and 10/10 script files compile**. Converter TODO count: **23 before this batch,
5 after**. These counts are not a claim of complete behavioral parity.

First v11 live entry: **9/9 passed** in
`autopilot_wasp_v11_entry_2026-09-23.json`. Runtime query observed
`HornetDrone=5/5,WaspChaser=1/1,WaspAttacker=1/1,QueenHornet=0/0` and definition
values `CREATURE_HORNET_PICNIC,CREATURE_HORNET_QUEEN_01`.

The sword-assisted initial wave passed **2/2 steps** in
`autopilot_wasp_v11_sword_2026-09-23.json`. The test teleports the hero near a
living target and sends actual attack input; it does not set quest success or
queen-phase flags. An earlier aborted attempt used E (bow) instead of Q (sword)
and did not clear the wave. The corrected run spawned the queen naturally,
started GuildmasterHelp, completed the queen cutscene, and reached native boss
phase 3 after its next minion wave. The hero then died; **this run does not prove
quest completion**. No Lua runtime errors were recorded. There was a separate
native FollowPreCalculatedRoute no-control diagnostic, and the queen entity
repeatedly retries priority-zero acquisition.

First run archive: `work/ab_runs/v11-20260923-122048/FableScriptExtender.log`.
Screenshots and diagnostic combat drivers remain in `scratchpad/` and `work/`.
The input driver now accepts F1–F12 for key/hold actions (the first F2 attempt
exposed missing function-key names). The first staged save was restored; all
three AutoSave files match the backup taken before the second replay.

## Controlled queen defeat and remaining handoff boundary

The second v11 replay passed **11/11 entry/wave steps** in
`autopilot_wasp_v11_controlled_2026-09-23.json`. Real sword/bow inputs defeated
the queen through her minion phases. This was an **assisted combat test**:
the driver teleported the hero near targets and replenished hero health.
It never assigned queen phases, enemy health, or quest success flags.
Queen health reached 24, 13, 2, then 0 in the final phase; the outro ran,
the quest itself set `MissionSucceeded`, and native `SetQuestAsCompleted`
entered and returned. The completion-call checklist passed **1/1** in
`autopilot_wasp_v11_completion_2026-09-23.json`.

**Persistent completion and the next quest are not verified.** After the native
call returned, `IsQuestCompleted('Q_WaspBoss')` remained false and gameflow's
`PostSavePosition` remained 200. One guarded crossing back to Lookout Point and
clearing the subsequent inventory tutorial did not change those observations.
The harness activates the quest directly; the next investigation should compare
that with accepting the actual guild quest card, then trace native completion
delivery and the renown tutorial gate. Do not force stage 300 or claim that
Guardian Sister Info was handed off. No causal diagnosis is established yet.

Controlled-run log: `work/ab_runs/v11-20260923-123820/FableScriptExtender.log`.
There were **zero Lua runtime errors**. Diagnostic exec-channel errors (an
unavailable Lua `debug` global and input commands mistakenly sent to the Lua
channel) are separate and retained in the log. The game is closed. All three
staged AutoSave files were restored from
`scratchpad/save_backup_1234234_20260923-122117` and hash-verified.

## Cross-unit validation

Final isolated generation covered **16 units**. Comparing all available draft
and readable baseline smoke reports found **no new error identities**; existing
village-unit failures remain. Guild, Wasp, Guardian Sister Info, Orchard Farm,
and Trader Conflict draft/readable smoke checks report zero problems. New
Oakvale separately compiled **51/51 functions and 18/18 files**; only two empty
native cleanup guards changed in its isolated draft. Other units' generated
outputs were not promoted.

Machine-readable evidence, combat observations, and restored save hashes:
`wasp_marathon_validation_2026-09-23.json`. Full regression suite: **1,689 tests
in 2,266.572 seconds, 403 failures / 45 errors**. All 448 failing/error test
identities exactly match the 1,677-test baseline; there are no new identities.
The suite remains red on those existing failures. It started before the final draft-only
empty-guard cleanup; the 96 focused checks and final isolated generation include
that cleanup. The v11 bundled readable Wasp script matches the final generated
readable script byte for byte.
