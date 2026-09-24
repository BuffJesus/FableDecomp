# Lua recovery handoff - 2026-09-23

**Continue offline. No playtesting until the user changes that instruction.**
Branch: `feat/novi-script-recovery`. This is the bedtime checkpoint; resume
implementation next session. Task priorities live in [ROADMAP.md](ROADMAP.md).

Trader Escort is an unfinished, disabled draft: 98 native bodies, 14 entity
bindings, all four quest workers preserved, and 65 generated functions.
Current result: **298 TODOs; draft 14/16 files compile; readable 15/17;
5 smoke problems**. It has not been packaged or tested in game.

**2026-09-24 update:** converter now uses RET-proven callee purges and pairs
code-pointer calls; with the peer session's EBP exporter fix (uncommitted
re-export) Trader Escort is at **185 TODOs, 16/16 files, 3 smoke problems**.
Open: exporter depth bug at 0xE019F2 (reported to the exporter owner),
inlined CScriptThing copy of `speaker`, DarkwoodTrader Init `auVar5`,
TraderComment vector loop. See the journal's 2026-09-24 section.

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

The last verified live baseline remains **v12**, save
**adult_orchard_completed_2026-09-23**, stage **500** (Trader Escort waiting).
Wasp, Maze, and Orchard progression/reloads were checked before the offline-only
instruction. Original profile saves were restored; the offline work did not
launch Fable or touch saves. See [Orchard evidence](journal/2026-09/ORCHARD_GUARDED_REPLAY_2026-09-23.md).

Unrelated reconstruction outputs and root scratch remain outside this Lua
checkpoint. Preserve them. Earlier handoff history is retained in
[HANDOFF_ARCHIVE.md](journal/HANDOFF_ARCHIVE.md).
