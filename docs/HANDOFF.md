# Lua recovery handoff - 2026-09-23

**Playtesting is allowed again (user, 2026-09-24).** Another session may share the install: check before launching.
Branch: `feat/novi-script-recovery`. This is the bedtime checkpoint; resume
implementation next session. Task priorities live in [ROADMAP.md](ROADMAP.md).

Trader Escort is an unfinished, disabled draft: 98 native bodies, 14 entity
bindings, all four quest workers preserved, and 65 generated functions.
Current result: **298 TODOs; draft 14/16 files compile; readable 15/17;
5 smoke problems**. It has not been packaged or tested in game.

**2026-09-24 update:** converter uses RET-proven callee purges, pairs
code-pointer calls, folds vector register aliases; the peer session's
depth-fixed export is promoted for Trader Escort. Trader Escort: **186 -> 175 TODOs,
16/16 files, smoke 1 problem** after parent-worker spawns lowered to
`CreateThread(name, {args = {me}})` (175 TODOs). Open: dead byte-merge
`extraout_EAX` noise in DarkwoodTrader (liveness), inlined CScriptThing copy of
`speaker` in MakeTraderComment, `AddQuestInfoBarHealth` colour operand. New exports for
the other 15 units wait in `work/ebp_fix/` (promote after diff review; the
exporter fix is committed as fa56828). See the
journal's 2026-09-24 sections.

Resume with [the conversion journal](journal/2026-09/TRADER_ESCORT_CONVERSION_2026-09-23.md)
and native/PDB evidence in `refs/script_recovery/trader_escort/`.
Repair MakeTraderComment operand/type recovery first (numeric receiver for
GetDataString), then DarkwoodTrader resource/vector operands, TraderComment
local-vector iteration, and captured thing arguments in parent worker spawns.
Change generators and evidence, never generated Lua by hand.

From the repository root:

```powershell
python -m tools.script_recovery.convert_quest_unit --unit trader_escort
python -m tools.script_recovery.build_readable_unit --unit trader_escort
python -m tools.script_recovery.smoke_run_unit --unit trader_escort --stage draft --json work/trader_escort_draft_smoke.json
```

Read the smoke JSON: the command can exit successfully with reported problems.
Final checkpoint: **137 focused offline tests passed**; exact command is in the
conversion journal. Established-unit regeneration checks are recorded there.
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
