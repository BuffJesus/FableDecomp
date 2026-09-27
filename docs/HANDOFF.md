# Lua recovery handoff - 2026-09-26

**Crash resume (2026-09-27):** BookOwned's retail +0xAC byte-vector offset now comes from its named
OnPersist transfer, so it is no longer persisted as a scalar bool (honest TODO; binding missing). Only
BookCollecting changes. All 55 unit JSONs since regenerated; Lua changes only in BookCollecting (order), Bordello/TourGuide
(unnamed workers now carry their spawn-site names, WatchForHeroLeavingRegionWithBeer / WatchForNoFollowers).
**Candidate v33 staged** (v32 + 8 files: BookCollecting, Bordello, 3 scalar-copy fixes; pinned adult-chain
packages untouched; 210 Lua parse, 215 hashes match; NOT installed or run). Next: run v33 in-game when the
install is free. See [marathon checkpoints 3-5 + v33](journal/2026-09/CODEX_LUA_MARATHON_2026-09-27.md).

**Marathon checkpoint (2026-09-27):** saved scalar/handle copies now survive dead-store
cleanup. BookCollecting conversation handles are preserved across animation branches.
144 tests / 28 subtests pass; 29-unit A/B reviewed; affected smoke has no new failing
callbacks; 369-file audit has zero syntax failures. V32 has not yet been repackaged.
See [marathon evidence and next fixes](journal/2026-09/CODEX_LUA_MARATHON_2026-09-27.md).

**Codex worker dispatch follow-up (2026-09-27):** BookCollecting's teacher now
queues `BookReaction` with its captured index; the registered worker name is recovered
from retail instructions. v32 is staged, not installed or run. 129 tests / 28 subtests
pass; 29-unit A/B changes only BookCollecting; 369-file audit has zero syntax failures.
Three existing BookCollecting smoke faults remain. See [evidence, validation and next work](journal/2026-09/CODEX_LUA_SPAWN_2026-09-27.md).

**Codex continuation (2026-09-27):** workflow skills validated; inherited readability fixes
and BookCollecting's worker key, indexed marker reads and by-value actors are promoted.
Candidate **v31** is staged (210 Lua files parse; 215 manifest hashes match), NOT installed
or run in-game. Focused suite: 177 passed / 30 subtests. Full suite: 2,414 passed,
30 failed, 12 errored; replay of those 42 cases matches baseline (36 pass, 6 fail),
with no current-only failure reproduced. See journal for subtests and context limits.
The 29-unit A/B against inherited sources changes only BookCollecting. Combined source
improvements affect eight units; unresolved Bordello smoke faults and BookCollecting
teacher dispatch remain. See [continuation and next gates](journal/2026-09/CODEX_LUA_CONTINUATION_2026-09-27.md).

**Playtesting is allowed again (user, 2026-09-24).** Another session may share the install: check before launching.
Branch: `feat/novi-script-recovery`. Task priorities live in [ROADMAP.md](ROADMAP.md).

**Twelfth pass (2026-09-27): script-member resources.** Syntax failures 16 → 10. Retail
`seh_*` resource members and the `csargs` string map now lower to a persistent sidecar store
(`resources:MemberResource / MemberStringMap / AssignResource / ClearStringMap`, patch
`novi-zzzzzzzzzzzzzz-member-resources.patch`, candidate DLL builds; NOT installed or in-game
validated). **Afternoon: syntax failures 10 → 0** (369 files; 24 superseded first-generation lifts
listed in `SUPERSEDED.json`). OakValeRevisited and BookCollecting recovered (flag maps, member thing lists,
`quest:GlobalConversations`); the six old clusters are registered units (`register_unit.py`). 23-unit A/B: only
the five intended units change. Two more sidecar patches (state-list resize, global conversations), candidate
builds, nothing installed or run in-game. **Playtest bundle v30** (`local-candidate-v30` = v29 + the round-12 sidecar
candidate + the 11 recovered/registered units as retail overrides; offline checks: 210 Lua files parse, manifest
hashes match) is staged but NOT run -- the user deferred in-game testing (machine busy). Its sidecar also carries
the three round-10 patches never run in-game (resource-vtable-methods, entity-event-data, arena-rounds), and its
ambient village overrides (Bordello, SickChild, BeardyBaldy, ...) are live from the first frame, so compare any
regression in the adult chain against v29 first. Next: run v30 in-game; open items (BeardyBaldy `extraout_EAX`, SummoningTheShip Init, BookCollecting
DoConversation) in [member resources](journal/2026-09/MEMBER_RESOURCES_2026-09-27.md).

**Current user priority (2026-09-26):** review all current Lua ports for Aeon-style
readability, including older Arena output. Eleven passes across 357 files updated 126
current Lua files; 16 syntax failures remain. Net goto reduction is 898; the Arena round-data
pass restored seven previously omitted countdown/interruption jumps (counts below are as of pass eleven; pass twelve: 10 syntax failures). Boolean-array
layout, persistence keys, integer parsing and BordelloLady's hidden string return are recovered.
Her Name initialization, five prices and all 15 dialogue suffixes are recovered; Init has no LAB labels.
Resource handling and Main's missing operands remain unresolved. All-unit validation is recorded
in the [readability review](journal/2026-09/ALL_SCRIPT_READABILITY_REVIEW_2026-09-26.md).
Arena creature counters, local GUI counter arrays, dynamic spawn flags and its 4x5 crowd-tag table
are recovered. PlayWave parses; all twenty crowd tags initialize and indexed crowd reactions use strings.
Arena's root now loads: a shared primitive-state snapshot replaces its round-vector
copy, and PlayWave reads named round/wave/creature fields. Nineteen unreachable
container helpers are omitted with call-graph evidence; 139 diagnostics are resolved.
Both creation APIs and countdown branches reach HUD setup in 24 generated-script traces.
The Release x86 sidecar candidate with all six later patches builds/links in
`work/readability_marathon_20260926_round10/sidecar_candidate`; it is not installed or
in-game validated. Arena-pass validation: 527 tests and 28 subtests; all 23 units
have no new smoke regressions. Multi-point spawning/combat/reward flow remains unverified.
The older named-cluster roots for MazeResearch, MeetSister, ScytheInfo, Fisherman,
RockTrollFirstEncounter and GuardianTrophyDealerInfo now use the readable pass.
Their 80 callback traces agree; 109 machine-temporary references are removed.
A suffix-scope fix preserves undeclared globals (including two unresolved helper
files). Phase eleven: 225 tests plus 101 subtests; 220 unit/legacy script smoke
comparisons have no new regressions. Reproduce older outputs with
`python -m tools.script_recovery.build_readable_cluster --script <cluster> --out <new-directory>`.
Next: missing operands/field names and old entity stubs; 16 files still fail syntax.
Do not call the remaining generated output fully readable.

**Expressions continuation (2026-09-26):** guild-seal teleport now preserves its zero
recall vector and follower expression loop; draft/readable behavioral tests pass.
All 23 units compared: changes limited to Expressions, White Balverine WW resource
calls, and Chicken Kicking's epsilon. Picklock/Steal still need packed-flag/numeric
recovery before an Expressions live bundle. See [Expressions recovery](journal/2026-09/EXPRESSIONS_RECOVERY_2026-09-26.md).

**After Bandit Camp (2026-09-26, converted scripts, v28 + runner)**: QS_GuardianTrophyDealerInfo (Maze in the
Guild), V_TrophyDealer (Witchwood, Demon Door, the dealer's cave) and White Balverine (Knothole Glade + Witchwood,
both halves in one run) are COMPLETE in-game, 0 Lua errors; the Singing Stones puzzle is assisted (retail-native).
Checkpoint `adult_white_balverine_completed_2026-09-26` (Gameflow 875, next: Q_Arena). Units, converter fixes and
resume steps: [chain after Bandit Camp](journal/2026-09/CHAIN_AFTER_BANDIT_CAMP_2026-09-26.md).

**Bandit Camp is COMPLETE in-game (2026-09-26, converted scripts, v22 + in-game runner)**: from
`adult_maze2_completed_2026-09-25` through both gates, the Forger, the hostage guard walking off on
his own patrol (a converter fix, 3b9602b), the freed hostages, the boss (assisted by health drain),
the Theresa flashback and `SetQuestAsCompleted('Q_BanditCamp')`. The sidecar is sidecar-abi-v13 +
the things-killed + cancel-using-ability patches, built in a scratch copy (see GOTCHAS). Status and
resume steps: [Bandit Camp journal](journal/2026-09/BANDIT_CAMP_CONVERSION_2026-09-25.md).

**Trader Escort is COMPLETE in-game (2026-09-25)**: v15 + the in-game runner played it hands-free from
`adult_trader_escort_accepted_v15_2026-09-24` to `SetQuestAsCompleted` (all three traders, troll, end trader),
0 Lua errors, no quest state set by hand. Three converter fixes landed on the way (exporter tag loss, comparison
flags, merged vector begin/end), each A/B-checked against every other unit. Checkpoint
`adult_trader_escort_completed_v15_2026-09-25`. Details:
[2026-09-25](journal/2026-09/TRADER_ESCORT_PLAYTEST_2026-09-25.md), [2026-09-24](journal/2026-09/TRADER_ESCORT_PLAYTEST_2026-09-24.md).

Done 2026-09-24 (see the journal's 2026-09-24 sections): RET-proven callee
purges, code-pointer call pairing, vector register aliases, parent-worker
spawns as `CreateThread(name, {args = {me}})`, the depth-fixed export
(fa56828) promoted for Trader Escort and Orchard (diff-reviewed), and the
SCRIPT_DEF table corrected (leading block is PDB - 4; the middle zone
0x258..0xd60 is unproven and stays numeric in readables).

Runner continuation (2026-09-25): the adult campaign reached Wasp -> Maze ->
Orchard Good -> Trader Escort. Final checkpoint
`adult_chain_20260925d_trader_escort` passed a fresh-process reload: all four
quests complete, Gameflow stage 600, and the next story quest active. This
was a resumed c/d run: Wasp needed a corrected handoff predicate, and Orchard
received a live runner targeting fix. No quest outcome or pause flag was
set by hand. The final escort needed no intervention and brought all three
traders to the end marker on the bridge. The three d stages and final reload
have zero Lua runtime/resource errors; the original AutoSave files are restored.
Runner fixes cover Maze OCR, save ownership/restoration, arrival UI settling,
combat near boundaries, and synchronous log preservation. Converter fixes
recover isolated float locals, entity receivers hidden by casts, and presented
item outputs (8f4faae, d9fbbc1, 031fa58); the live bundles are unchanged.
Details and limits: [runner continuation](journal/2026-09/RUNNER_CONTINUATION_2026-09-25.md).

Next, in order:
1. Run the adult campaign from graduation with a new tag:
   `python tools/script_recovery/run_campaign.py tools/script_recovery/runner_campaigns/adult_good.json --bundle v16 --tag <tag>`.
   A clean uninterrupted four-stage replay is still open; then extend the runner
   toward New Game/Guild training and subsequent quests. Do NOT hand-drive the
   game: fix the runner instead. The driver closes staged games before restoring
   the protected profile and archives each stage's log. See
   [runner usage](scripts/INGAME_RUNNER.md) for checkpoint reload verification.
2. Re-export the other units with the fixed exporter (9507b0d: thing-vector elements, pushes kept across
   zero-parameter calls; 3811caa: `push reg` / `lea reg,[reg]` keep a register's thing tag) and with `work/ebp_fix/<unit>_typed.json`, one at a time: regenerate into work/,
   review the diff, then promote. Playtested units (Wasp, Guild, Guardian, Trader Conflict) need extra care.
3. Pin the SCRIPT_DEF middle zone (a live dump of the CScriptDef object settles it).
4. Still open: a won boast's payout line (Orchard, and Trader Escort's boast 9), Orchard Evil success,
   WatchForMissionRules "DarkwoodTrader" name-slot pairing, and live DarkwoodAssassinSpawn coverage.
   Its trigger distance and entity-position receivers are fixed in generated source (8f4faae, d9fbbc1),
   with readable smoke 16 files / 0 problems. These fixes are not deployed to v15/v16.

Sidecar-side namespace clearing on host creation is still NOT done: check first whether persisted state is
loaded into the global map before a host exists.

Change generators and evidence, never generated Lua by hand.

**Reconstruction lane (2026-09-25):** round 3 completes the boot-leaf de-bake:
117/118 verified leaves are genuine C++ (the remaining Exit is an import thunk), and
both game-component constructors pass parity and behavior. The unmodified bootstrap,
including WinMain, passes; OpenRetailBank's real-bank runtime probe passes too.
Branch `land/boot-debake` (round 15, 2026-09-26): 123/123 boot leaves, CBaseClass
root class, and the **trap-linked CGame::Play seam**: `python tools/decomp_pipeline/play_seam.py`
links Play over every landed genuine source, traps the rest and prints the next missing
callee in execution order. Round 15 verifies ten functions (2,551 retail bytes),
including four near matches. Three scaffolds graduated; only unsigned-long vector
assignment at 0045BC09 remains. The --entry system diagnostic now passes profiler
construction and math tables, then traps at GFInitVectorMath (00A5B850: CPUID;
VC7.1 has no __cpuid intrinsic). No asm substitute or bypass was added.
Play still faults at GetFont+8 with no font manager; connect real startup and font
inputs after resolving its dependencies. Default system flags request no fonts/display.
The seam reads Data\CompiledDefs read-only. Before every commit run
`python tools/decomp_pipeline/gate_all.py --bootstrap`.
Commit with a temp index (the checkout is shared with `feat/novi-script-recovery`). See
[PLAY_SEAM_DRIVER_2026-09-26](journal/2026-09/PLAY_SEAM_DRIVER_2026-09-26.md),
[BOOT_DEBAKE_2026-09-25](journal/2026-09/BOOT_DEBAKE_2026-09-25.md).

From the repository root:

```powershell
python -m tools.script_recovery.convert_quest_unit --unit trader_escort
python -m tools.script_recovery.build_readable_unit --unit trader_escort
python -m tools.script_recovery.smoke_run_unit --unit trader_escort --stage draft --json work/trader_escort_draft_smoke.json
```

Read the smoke JSON: the command can exit successfully with reported problems.
Focused offline tests: **168 passed** (2026-09-24; plus the 2026-09-25 cases in `test_lift_native_lua`, `test_vector_register_aliases`, `test_thing_release_and_flags`); the base command is in the
conversion journal, plus the `test_callee_purge_pairing`, `test_vector_register_aliases`,
`test_spawn_capture`, `test_script_def_offsets` and `test_readable_*` modules. Established-unit regeneration checks are recorded there.
The broad suite was cancelled; no new full-suite pass is claimed.

Live baseline: **v15** (`local-candidate-v15`) = v14's units + Trader Escort + sidecar patches up to
`novi-zzzzzzz` (thread arguments across Lua states). Build it with `build_unit_playtest_package.py` (`--bundle` is
now required; its old default overwrote local-candidate-v5). v14, for the Orchard history below:
**v14** (`work/new-oakvale-original-fse-20260912/local-candidate-v14`) = v12 +
`novi-zzzz` (SetQuestAsFailed message) + `novi-zzzzz` (IsEqualTo operand) sidecar patches + Orchard Lua with
ctor defaults (b270c0f). Orchard **Good** completed end to end on v14 (handoff 3/3, 0 Lua errors; checkpoint
`adult_orchard_completed_v14_2026-09-24`); **Evil** played to its failure path only. Boasts verified live
(podium UI, AddBoast, IsBoastTaken, Quest Start lines, "Boast Failed" notice); a WON boast's payout line is
still unseen. See [Evil + boasts](journal/2026-09/ORCHARD_EVIL_AND_BOASTS_2026-09-24.md).

Unrelated reconstruction outputs and root scratch remain outside this Lua
checkpoint. Preserve them. Earlier handoff history is retained in
[HANDOFF_ARCHIVE.md](journal/HANDOFF_ARCHIVE.md).

**Native round 16:** two rendering fixtures now link their candidates and pass NEAR_MATCH; seven verified near matches in total. Runtime stops remain GFInitVectorMath/CPUID and missing Play font startup. See the [round 16 journal](journal/2026-09/PLAY_SEAM_DRIVER_2026-09-26.md).

**Native round 17:** eleven font lookup/acquisition/ownership functions verified (616 retail bytes); one assembly bake replaced. Nine verified near matches. Next font dependencies are the static/streaming constructors; runtime still requires real font-manager startup and the CPUID gap. See the [round 17 journal](journal/2026-09/PLAY_SEAM_DRIVER_2026-09-26.md).

## Native frontend priority - 2026-09-26

User clarification: the visible program must run reconstructed retail C++, not
rely on authored frontend adapters. The visual checkpoint is reference/debugging
material, not completion of this goal. No further adapter expansion is the default.

1. Recover retail GFMain (00402510) and its startup dependencies, starting from
   recovered WinMain. The startup probe now executes genuine MicroThread::SetStack
   (009D86B0) and stops at @GFMain@12. Command:
   `python tools/decomp_pipeline/play_seam.py --entry startup --out work/native-startup`.
2. Follow retail startup/configuration to construct display, input and font managers;
   then GFInitialise -> CGame initialisation -> Play. The isolated system probe's
   CPUID gap and the font constructors remain open. Do not bypass them with adapters.
3. Run the recovered frontend state, layout, rendering and input code against assets
   read from installed retail containers at runtime. Extracted/pre-rendered menu BMPs
   and embedded artwork do not satisfy this milestone.
4. Validate a visible frontend and its actions against retail, with provenance for
   the linked engine code and runtime asset reads. Keep LIVE omitted as requested.
   Traps, unresolved dependencies, and unverified startup data remain explicit blockers.

Reconstruction still means genuine decompilation-derived C++, passing behavior
fixtures and byte-close VC7.1 output. This is not modernization/fork work.

## Shutdown handoff - 2026-09-26

User is shutting the computer down. Native implementation checkpoint: 74b2a78.
All work is saved; no native reconstruction build, probe, or game window remains
running from this session. No task depends on keeping the computer on.

Resume on land/boot-debake without switching the shared Lua checkout. Use a separate
Git index for native commits. Start with the native startup journal:
`docs/journal/2026-09/NATIVE_FRONTEND_STARTUP_2026-09-26.md`.

Next implementation: genuine retail GFMain at 00402510. Its fresh decompilation is
`work/codex-native-startup/GFMain_00402510_decomp.c`; cross-check the call addresses
against `rebuild/integration/gfmain_calls.tsv` and retail disassembly. Existing
GFMain phase adapters are reference material, not the implementation to link.
Reproduce the frontier with:
`python tools/decomp_pipeline/play_seam.py --entry startup --out work/codex-native-startup`.
Expected: WinMain -> recovered SetStack -> trap @GFMain@12 (exit 86).

Requirement: the visible frontend must run recovered retail C++ and load real
containers at runtime. Do not extend/relaunch the authored visual checkpoint as a
substitute. LIVE removal is committed and visually verified. The native frontend
is not yet visible; the later system CPUID and font startup gaps remain open.
Latest checks: all gates PASS, 123 boot leaves, 488 header dependents, zero
regressions, full bootstrap; SetStack parity/behavior and mutation checks passed.
