# Lua recovery handoff - 2026-09-24

**Playtesting is allowed again (user, 2026-09-24).** Another session may share the install: check before launching.
Branch: `feat/novi-script-recovery`. Task priorities live in [ROADMAP.md](ROADMAP.md).

Trader Escort is an unfinished, disabled draft (98 native bodies, 14 entity
bindings, 4 quest workers, 65 generated functions). As of 2026-09-24:
**175 TODOs; 65/65 functions and 16/16 files compile; smoke 1 problem**
(draft and readable). Not packaged or tested in game.

Done 2026-09-24 (see the journal's 2026-09-24 sections): RET-proven callee
purges, code-pointer call pairing, vector register aliases, parent-worker
spawns as `CreateThread(name, {args = {me}})`, the depth-fixed export
(fa56828) promoted for Trader Escort and Orchard (diff-reviewed), and the
SCRIPT_DEF table corrected (leading block is PDB - 4; the middle zone
0x258..0xd60 is unproven and stays numeric in readables).

Next, in order:
1. DarkwoodTrader's remaining smoke problem: dead byte-merge noise
   (`CONCAT31((int3)(extraout_EAX >> 8), ...)`) needs a liveness pass.
2. MakeTraderComment: inlined CScriptThing copy/assign of `speaker`
   (vtable store + refcount), then `AddQuestInfoBarHealth`'s colour operand.
3. Promote `work/ebp_fix/<unit>_typed.json` for the other 13 units, one at a
   time: regenerate into work/, review the diff, then promote. Playtested
   units (Wasp, Guild, Guardian, Trader Conflict) need extra care.
4. Pin the SCRIPT_DEF middle zone (a live dump of the CScriptDef object
   settles it).

Next bundle (v14, not built): v13 + `novi-zzzz-quest-failed-message.patch` in the sidecar + the
Orchard output from b270c0f (constructor zero-inits in Init). Sidecar-side namespace clearing on host creation
is deliberately NOT done: check first whether persisted state is loaded into the global map before a host exists.

Change generators and evidence, never generated Lua by hand.

From the repository root:

```powershell
python -m tools.script_recovery.convert_quest_unit --unit trader_escort
python -m tools.script_recovery.build_readable_unit --unit trader_escort
python -m tools.script_recovery.smoke_run_unit --unit trader_escort --stage draft --json work/trader_escort_draft_smoke.json
```

Read the smoke JSON: the command can exit successfully with reported problems.
Focused offline tests: **168 passed** (2026-09-24); the base command is in the
conversion journal, plus the `test_callee_purge_pairing`, `test_vector_register_aliases`,
`test_spawn_capture`, `test_script_def_offsets` and `test_readable_*` modules. Established-unit regeneration checks are recorded there.
The broad suite was cancelled; no new full-suite pass is claimed.

Live baseline: **v13** (= v12 + `SetQuestAsFailed` binding fix + regenerated Orchard,
`work/new-oakvale-original-fse-20260912/local-candidate-v13`). Stage-500 save
**adult_orchard_completed_2026-09-23** (Good route, Trader Escort waiting) still stands.
2026-09-24: boasts verified live (podium UI, `AddBoast` list, `IsBoastTaken`, Quest Start
lines, live "Boast Failed" notice); Orchard **Evil** route played three times to its
team-killed failure (intro, crate theft 3->1, waves, failure screen + Reload fixed in v13;
0 Lua errors). Evil success/Whisper/outro and boast payouts still unseen. Checkpoint
`adult_orchard_evil_accepted_2026-09-24`. See [Evil + boasts](journal/2026-09/ORCHARD_EVIL_AND_BOASTS_2026-09-24.md).

Unrelated reconstruction outputs and root scratch remain outside this Lua
checkpoint. Preserve them. Earlier handoff history is retained in
[HANDOFF_ARCHIVE.md](journal/HANDOFF_ARCHIVE.md).
