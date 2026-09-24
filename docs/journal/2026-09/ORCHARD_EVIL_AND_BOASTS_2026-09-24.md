# Orchard Farm Evil route and the boast system — 2026-09-24

First live run of the Evil route (Attack Orchard Farm) and of the boast system. The user lifted the
offline-only instruction this morning ("You can playtest if needed"). A parallel session (fabletlc-89)
owned converter / Trader Escort work; this session owned the exporter and the playtest.

## Exporter fix (committed fa56828)

`ExportTypedTranslationUnit.java` read EBP as a frame base in every function. VC7.1 keeps `this` in EBP in
many script bodies (`mov ebp, ecx`), so `[ebp+0x40]` lost its script-interface tag and no prototype
override reached those vtable calls (Trader Escort: 309 of 687 printed with no arguments; MakeTraderComment
38/44). EBP is now a frame base only after `mov ebp, esp`. Two depth faults surfaced once those calls typed
(epilogue scan stopped at `xor al,al` between the pops; flow pass never excluded prologue saves) and an
in-place CScriptThing (`mov [esp+X], 0x1238C8C`) now tags its slot. Argument-less vtable calls fell in every
unit (bordello 617 -> 38, trader_conflict 177 -> 55, beggar_and_child 148 -> 9). Per-unit report:
`work/ebp_fix/report.json`. The depth fix also dissolved the Orchard Evil `addQuestInfoCounter = f_stk_14_2`
line (two adjacent slots had merged); fabletlc-89 regenerated Orchard (d74e5f4).

## Boasts: verified live (v12)

Card table -> Attack Orchard Farm -> **Take Quest and Boast** -> the engine moves the hero onto the
boasting podium outside the Guild. All five boasts the converted `OrchardFarmRaidEvil` Init registers with
`AddBoast` are listed, with retail wagers/rewards:

| Boast | id | Wager | Reward |
|---|---|---|---|
| No Protection (naked) | 1 | 80 | 160 |
| Without A Scratch | 3 | 100 | 400 |
| Fist Fighter | 6 | 100 | 300 |
| No Healing | 8 | 80 | 160 |
| Protect Bandits | 18 | 100 | 275 |

Took 3, 8, 18. `IsBoastTaken` = true for exactly those (1, 6 false); the Quest Start screen lists the three
with their rewards; boasts survive save/reload. The quest-specific boasts are resolved by the engine from
master states the Lua sets (`OrchardFarmBanditKilled` from `CrateTeamMember` on a bandit death — observed
flipping true live). Boast *outcomes* at completion are not yet observed (no Evil completion yet).

Checkpoint: `Saves/adult_orchard_evil_accepted_2026-09-24` (autosave right after Take Quest and Boast;
boasts 3/8/18 taken).

## SCRIPT_DEF offset table

The live boast UI values proved `refs/script_recovery/script_def_offsets.json` was 0x3C low for the boast
block (retail reads Evil Naked at 0x168; the table had its values at 0x12c). Readable names only; draft
offsets were right. fabletlc-89 corrected the model (PDB-4 up to +0x254) and regenerated readables (0b55994).

## Evil route run 1 (v12): failure path and a binding defect

Evil crossing = map slot 10 (OrchardFarmEast), arrival 3 m from BanditTeamSpawn (3301.48, 3292.28, 45.16).
`CS_ORCHARD_EVIL_INTRO` played in full, Quest Start (Enter), tutorial pages. Combat driver:
`work/orchard_evil/evil_combat.py`, then `evil_combat2.py` (targets guards, leads bandits to the barn crates at
~(3267,3194); the hero heals via ChangeHeroHealthBy below a threshold — logged, and this voids No Healing).
The hero died once (phial revive; not a quest failure: MissionFailed stayed 0). The guards then killed every
bandit -> MissionFailed = 3 (team killed). 0 Lua runtime errors in the whole run
(`work/ab_runs/v12-20260924-082321`).

The native Quest Failed screen came up **blank** (no quest name, no reason) and its **Reload button was dead**
(click, Enter, Esc all ignored; the game had to be killed). Cause, from retail and the binding source:

- Retail `ProcessGameRulesEvil` passes `SetQuestAsFailed(GetActiveQuestName(), true, FailReasons[n], true)`
  where `FailReasons[]` are CWideStrings built from ASCII keys (Init 0x00DCC39D, 0x0099B800).
- `GetActiveQuestName` (0x00891880) returns `CQuestManager::PQuestCurrentlyRunning`'s name. Measured inside a
  thread created in the OrchardFarmRaid host: `Q_OrchardFarmRaid`, as retail. (Called from a channel eval it
  returns `Gameflow` — the eval runs in Gameflow's frame; don't use eval results for this API.)
- The ForgeFSE binding `LuaQuestState::SetQuestAsFailed` ignored the message and passed a CWideString with a
  **null data pointer** and a hard-coded `true`.

Fix: `tools/script_recovery/sidecar_patches/novi-zzzz-quest-failed-message.patch` builds the message with the
retail CWideString ctor (0x0099B6B0/dtor 0x0099B510, the helper DisplayGameInfoText already uses) and passes
the caller's flag. Built on a copy of the exact v12 source (`sidecar-abi-v13`, baseline commit = v12 tree;
reverse-check clean). **v13** = v12 + that DLL + the regenerated Orchard packages (d74e5f4); every other file
byte-identical; manifest updated. Orchard readable smoke: 11 files, 0 problems.

## Evil route on v13 (runs 2 and 3)

Launched from `adult_orchard_evil_accepted_2026-09-24`; entry checklist `work/orchard_evil/06_evil_entry.json`
4/4 both times; intro, Quest Start (all three boasts listed), tutorials cleared with `clear_all`.
Driver `work/orchard_evil/evil_combat2.py` (final form): fetch the farthest bandit when >20 m away, fight
guards within 10 m of the hero or a bandit with sword clicks + a lightning cast, lead the bandits to the barn
in 6 m hops, heal the hero below 50% (logged), click a detected game-info box on a channel timeout.

Observed, all from the converted Lua + engine, no outcome or NPC-health writes:
- **Boast failure notice live:** the first scripted heal raised "Boast Failed. You restored your health."
  (No Healing, id 8) — the engine tracks the boast and counts ChangeHeroHealthBy as healing.
- **Crate theft works:** bandits switch to fetching within 10 m of a crate (Teams_1_StateCounter_2), carry it
  to the drop point, and CrateCount falls 3 -> 2 -> 1 (run 2 and run 3 both reached 1).
- **Waves:** bandit reinforcements arrive in three waves at BanditTeamSpawn; guards respawn indefinitely.
- **Failure path, fixed:** both runs ended with the third wave dead -> MissionFailed = 3. The Quest Failed
  screen now shows "Your team were all killed." and **Reload works** (it loaded the live profile's save —
  see the staging gotcha; Fable was killed and the original save re-restored and hash-checked).

Not reached: last crate, Whisper, `CS_ORCHARD_EVIL_OUTRO`, Evil completion and the boast payout screen. The
limit is the driver's combat output (guards camp the bandit spawn; each 20 HP bandit wave dies fast), not a
script fault. Archives (0 Lua runtime errors each): `work/ab_runs/v12-20260924-082321`,
`v12-20260924-085955`, `v13-20260924-091014`, `v13-20260924-095638`.

## Open

- Evil success path + boast payouts: needs a stronger driver (hold position at the drop point until the
  wave is clear of guards; spells from range) or a hand-played run from `adult_orchard_evil_accepted_2026-09-24`.
- A cheaper boast-*success* check: Good route with the No Crates Stolen boast (id 17); last Good run ended
  with `OFBR_NoCratesWereStolen = true`, so the payout line should appear on its completion screen.
- The Quest Failed title bar stays empty (retail passes `Q_OrchardFarmRaid`, which owns no card) —
  unverified against retail; low priority.
