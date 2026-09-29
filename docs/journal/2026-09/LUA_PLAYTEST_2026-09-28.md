# Lua runtime replay - 2026-09-28

The user authorized launching/testing and explicitly redirected coverage toward the
earlier scripts still awaiting in-game tests. Resume that backlog, not just Witch.

## Launch and harness fixes

v42 failed before gameplay: launcher result 9, add-on start status 11. v35's staging
script copied the round-12 sidecar into `FableScriptExtender.dll`, leaving an older
`NoviCompatibility.dll` in the add-on slot. v36-v42 inherited that pairing. Manifest
hashes verified those bytes but did not verify their roles. Previous offline Lua
results stand; the packaging checks did not establish launch readiness.

**v43** corrects the pairing without changing any Lua:

- Stock FSE from v34, upstream b2432e8:
  `36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea`.
- Round-12 sidecar e83c39f in `NoviCompatibility.dll`:
  `f0ded2603690b8325fb0565d8aa0829b83887300f62c5553b03b828513e7df2c`.
- 210 Lua parse; 216 manifest entries match. New preflight checks reject v42 even
  with its matching manifest. Successful live startup logs both stock registration
  and the chained compatibility hook. No shared installation files changed.

The first v43 replay stopped in the frontend: the driver assumed 0atlas occupied
row three, but the current list puts it in row two. The load-header check rejected
the wrong profile before loading. The driver now locates the exact profile label
using Windows OCR, accepts its observed O/0 ambiguity, verifies the hover bar, and
retains the load-header check. Missing/ambiguous labels do not trigger a selection.
The captured shifted profile resolves to row 254. The retry loaded the intended save.

Validation: 27 tests plus five subtests pass across DLL-role regression, profile
selection, autopilot launch/driver, and runner lifecycle suites.

Stage and evidence: `work/codex_runtime_v43_20260928/`; payload
`work/new-oakvale-original-fse-20260912/local-candidate-v43/`.

## Runtime coverage

Replay command (assisted travel and normal talk input; no quest-completion state writes):

```powershell
python -u tools/script_recovery/ingame_runner.py tools/script_recovery/runner_quests/guardian_sister_info.json --bundle v43 --tag v43_guardian_bowerstone_retry_20260928 --minutes 8 --launch --save adult_wasp_completed_2026-09-23 --close
```

**Passed:** the save loaded, Bowerstone's arrival scene ran, and real TAB interaction
completed Maze's conversation. The runner verified quest completion, master
`PostSavePosition == 400`, and `CoreQuestWaiting == true` before returning success.
This is an assisted replay, not unassisted travel or a fresh save/reload proof.
The game closed and all **561 save files** match the pre-test backup byte-for-byte,
with no added files. Evidence: `work/runner/v43_guardian_bowerstone_retry_20260928.jsonl`
and its `_FableScriptExtender.log`; save comparison in
`work/codex_runtime_v43_20260928/save_integrity.json`.

**Confirmed earlier-port failure:** StatueMaster Main (retail 0x00ED3B30), generated
Lua line 30: `attempt to compare nil with number`. The angle-wrapping branch assigns
and compares unassigned `fret_0`; the same shape repeats for other angles. Investigate
the `_CIfmod` / ST0 result lowering end to end; current unit tests cover the operand
rewrite but not this generated branch. The zero/default mock positions missed it.
Do not hand-edit generated Lua or call the entire candidate error-free.

**Second confirmed failure:** SickChild entity Main (retail 0x00EC5DE0), generated
line 33, calls `EntityTeleportToPosition(me, nil, false, false)` and the live sol
binding rejects the argument count/types. This stops the child entity during the
Bowerstone load. Recover the bed position/orientation and teleport arguments from
native evidence, then test the actual generated callback against the binding
contract. Nearby animation/string operands and angle wrapping also remain suspect;
only the teleport failure is demonstrated on this path. Final archived log contains
two Lua runtime errors total, one for StatueMaster and one for SickChild.

## Remaining earlier-script coverage

| Scope | Next runtime gate |
| --- | --- |
| StatueMaster | Reproduce nonzero/negative angle wrapping in generated Lua, recover native result, then repeat startup and statue interaction |
| BeardyBaldy, Book Collecting, Sick Child | Separate Bowerstone/Quay encounters, conversation branches, state changes, save/reload; loading the region alone is insufficient |
| Chicken Kicking, Oakvale Revisited, Hero's Old House | Oakvale checkpoint and actual interactions/scene completion |
| Bordello | Reach its region, dialogue/resource ownership and encounter completion |
| Singing Stones, Summoning the Ship, Dragon Boss Fight | Appropriate later story checkpoints and completion/handoff checks |
| Previously completed Guild/adult chain | Regression coverage on the current candidate, especially changed gate/resource/persistence paths |
| Tour Guide, village GuildMaster, Arena recovery | Outside v43's current override roster; require separately reviewed staging before claiming runtime coverage |

The 11 newly added overrides since v30 are Bordello, Chicken Kicking, Sick Child,
Book Collecting, Oakvale Revisited, BeardyBaldy, Dragon Boss Fight, Hero's Old House,
Singing Stones, StatueMaster and Summoning the Ship. Earlier successful runs of
other quests do not validate these, and later source changes require their own replay.
