# Lua recovery handoff - 2026-09-25

**Playtesting is allowed again (user, 2026-09-24).** Another session may share the install: check before launching.
Branch: `feat/novi-script-recovery`. Task priorities live in [ROADMAP.md](ROADMAP.md).

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

Next, in order:
1. Grow the runner into every quest (one `runner_quests/<quest>.json` each; Trader Escort's is the model:
   legs per region, `party` expression, done/failed), aiming at a run from New Game. Replay Trader Escort with
   `sh work/trader_escort/to_post_intro.sh` + `python tools/script_recovery/ingame_runner.py
   tools/script_recovery/runner_quests/trader_escort.json --bundle v15 --tag <tag> --minutes 35` (about 10
   minutes). Do NOT hand-drive the game: fix the runner instead. The launch stages a save into profile 1234234;
   restore it from that launch's `scratchpad/save_backup_1234234_<timestamp>`, NOT `_orig` (it sorts last).
2. Re-export the other units with the fixed exporter (9507b0d: thing-vector elements, pushes kept across
   zero-parameter calls; 3811caa: `push reg` / `lea reg,[reg]` keep a register's thing tag) and with `work/ebp_fix/<unit>_typed.json`, one at a time: regenerate into work/,
   review the diff, then promote. Playtested units (Wasp, Guild, Guardian, Trader Conflict) need extra care.
3. Pin the SCRIPT_DEF middle zone (a live dump of the CScriptDef object settles it).
4. Still open: a won boast's payout line (Orchard, and Trader Escort's boast 9), Orchard Evil success, the
   WatchForMissionRules "DarkwoodTrader" name-slot pairing, and DarkwoodAssassinSpawn's trigger distance
   (`f_CVar3 = xStack_20` stays TODO: the readable smoke's one free global).

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
