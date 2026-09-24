# Trader Escort: first in-game runs (v15), 2026-09-24

Bundle `work/new-oakvale-original-fse-20260912/local-candidate-v15` = v14's unit set + Trader Escort
(`--unit guild_training wasp_boss guardian_sister_info orchard_farm trader_conflict trader_escort`, v14's
NoviCompatibility as `--oakvale`, its LUAGameflow as `--gameflow`) + a DLL with sidecar patches
novi-zzzz / zzzzz / zzzzzz / **zzzzzzz** (below). Diffed against v14 before use: only Trader Escort, its override
entry and the reviewed regenerations differ.

## What ran

From `adult_orchard_completed_v14_2026-09-24` (PostSavePosition 500): travel to the Guild (map slot 70), the
card table lists Trader Escort (2000 gold, 500 renown, 3 boasts). **Take Quest and Boast** -> podium lists the
three boasts the converted Init registers, with the retail wagers read through SCRIPT_DEF:

| Boast | id | Wager | Reward |
|---|---|---|---|
| No Protection | 1 | 200 | 400 |
| Without A Scratch | 3 | 200 | 1000 |
| Protect Traders | 9 | 100 | 400 |

Took 9. Checkpoint `Saves/adult_trader_escort_accepted_v15_2026-09-24` (quest active, boast 9 taken).

Darkwood1 (map slot 18 at 3056,2720,63.5): the intro macro `CS_DARKWOOD_TRADER_INTRO` plays in full (cameras,
teleports, speech, the hero's InteractiveSpeak), then `IntroFinished`, the Quest Start screen (lists the Protect
Traders boast, +400), the PC follow/wait info boxes, `FollowInfoGiven`; two traders follow the hero
(`TradersStillAliveCounter` 2). Darkwood2 (slot 35 at 3136,2592,78.5): the flesh-eating balverine scene plays and
the balverine attacks. **0 Lua errors** on the final run.

## Four defects found and fixed on the way

1. `WatchForPickpocketing` got an un-indexable userdata: entity scripts have their own `sol::state`, and the
   `{args = {me}}` of a parent-quest thread named a registry slot of the wrong state. Sidecar patch
   `novi-zzzzzzz-thread-args-cross-state.patch` rebuilds each argument in the quest's state (9495329).
2. The intro parked forever: `GSI->StartMovieSequence(&name, (CScriptThing *)M)` escaped the double-start drop
   rule because of the cast (0293d08).
3. DarkwoodTrader died on a nil flag word: the temp-destruction flags were relayed through a register mid-phase;
   `fold_flag_relays` (1a6c0ef).
4. The game crashed on the surprise balverines: `CreateCreature(nil, nil, ...)`. Exporter: a
   `std::vector<CScriptThing>` element (`lea r,[i+i*2]; lea r,[begin+r*4]`) is now a thing, and a resolved call
   with no stack parameters keeps the pending pushes; lowering: `V[i * 3]` element vcalls and filled
   `CScriptThing` stack locals (9507b0d). Only Trader Escort was re-exported.

## Not yet tested

- The escort itself: a map-slot teleport does NOT bring followers along (the traders stayed in Darkwood1).
  Retail needs the traders walked across each boundary: Darkwood1 -> 2 (slots 35/36) -> 3 (37/38) -> 4 (39) ->
  5 (40/41) -> 6 (42/43) -> BarrowFields (44). A walking driver (hold W toward a waypoint, re-issue Follow) is
  the next harness piece.
- Combat in Darkwood2 needs driving (the idle hero lost a resurrection phial to the balverine).
- The other units' exports predate the exporter fix; re-export and diff them one at a time.
