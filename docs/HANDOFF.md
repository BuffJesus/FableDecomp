# Lua recovery handoff - 2026-09-25

**Playtesting is allowed again (user, 2026-09-24).** Another session may share the install: check before launching.
Branch: `feat/novi-script-recovery`. Task priorities live in [ROADMAP.md](ROADMAP.md).

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

**Reconstruction lane (2026-09-25):** boot-path de-bake, 40 -> 25 asm leaves of 118, `CGame::Play` now genuine,
full bootstrap green except the WinMain fixture (needs Fable closed). Gates: `python tools/decomp_pipeline/gate_boot_leaves.py`
and `gate_header_dependents.py`. Next steps are in ROADMAP (c); details in
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
