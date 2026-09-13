# Guardian Trophy Dealer Info — next-port recovery

Selected QS_GuardianTrophyDealerInfo for the next bounded recovery after Maze
Research. It has one bound entity, one named completion task, one persistent
boolean and three identifiable macro assets. Existing runtime APIs cover its
root, persistence, quest completion and actor queries. Its entity still needs
full resource/speech integration review before an executable package is staged.
This directory contains a non-registered reference, not an activated port.

Other candidates inspected: QS_GuardianSisterInfo and its BanditCamp variant
have larger root bodies in the current inventory; Q_HerosOldHouse has a longer
root and more persistence fields. This choice is based on bounded evidence
available here, not on the seed or converter declaring the script complete.

## Identity and root contract

| Component | Native address |
| --- | --- |
| Init | E27870; clears quest+48 |
| Main registration | E27880 |
| Main | E27920 |
| OnPersist | E27E60; PieceOver, default false, transfer 4045C0 |
| WaitForPieceOver task | E27AC0 |
| GTDI_Maze allocator | E27BB0 |
| GTDI_Maze vtable | 12E20A4 |
| GTDI_Maze Init | E27C60; stores initial angle at entity+1C |
| GTDI_Maze Main | E27E90 through E28C9A ret |

Main binds GTDI_Maze using allocator E27BB0, enabled byte 1 and binding flags 0.
It calls CScriptBase::PostAddScriptedEntities, then CGSI+100 (native slot
inventory: PostAddScriptedEntities at 6E7460). Forge's FinalizeEntityBindings
already performs both calls. It spawns WaitForPieceOver with an empty region
before setting the objective, using the active quest and
TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01 with both region strings
HeroGuildComplexInside. The watcher must not be folded into blocking root Main.

The allocator creates a 0x20-byte entity, copies its bound Thing to +8, stores
parent at +14 and master data at +18. Use the bound instance rather than the
seed's additional self-lookup for entity behavior. The corpus declares Lua
entity ID 69; that is registry evidence, not a numeric native vtable slot, and
must be checked against the eventual merged override configuration.

## Executed watcher contract

The task waits for PieceOver with frame/query pairs, checks termination again
after observing the flag, then looks up GTDI_Maze once. While the returned
native Thing data pointer exists and its +12C IsAlive query is true, it waits
with frame/query pairs. It checks termination again before deactivation.
On success it calls DeactivateQuestLater(activeQuest,0), then destroys its
local lookup wrapper. Cancellation after lookup also destroys that wrapper.

Ten native cases cover ready-dead, absent Maze, pre-lookup cancellation,
waiting cancellation, simultaneous flag/cancellation, cancellation immediately
after observing the flag, ordinary flag transition, alive-to-dead transition,
alive-wait cancellation and cancellation immediately before deactivation.
All ten match reference/quest.lua's watcher event sequence. APIs and storage
are doubles; native callback branches and termination helper execute.

```powershell
python work/runtime-20260912-video-comparison/prove_guardian_trophy_waiter.py
python -m unittest tools.script_recovery.test_guardian_trophy_waiter
```

## Authored assets

All three installed CCutsceneDef payloads decode without leftover bytes:
CS_MAZE_TROPHY_INFO_SETUP, CS_MAZE_TROPHY_INFO_HITME and CS_MAZE_TROPHY_INFO.
The setup and hit assets begin with KeepEntityMap TRUE and have empty skip
vectors. The final asset has explicit speed restoration, teleports and fades
in its skip vector. Preserve the retained actor-map relationship across the
macro sequence; independently reacquiring actors between macros is unproven.

Ten unique TNG records were found for all referenced MK_MTI/CAM_MTI names,
GTDI_Maze and WitchwoodTeleporter across HeroGuildComplexInside and Witchwood.
The full macro vectors, payload hashes, script.bin hash, TNG hashes and records
are archived in installed-macros and installed-placements JSON files.

## Remaining entity implementation gate

Update: first-encounter and repeat-hit/talk bodies are now implemented in the
non-registered reference and match 49 native replay cases. Full movie CFG
identity and cleanup checks pass. See ENCOUNTER_RESOURCE_REVIEW_2026-09-12.md
for the exact coverage, assets, remaining entry/outer-loop work and resource
speech adapter gate. The following paragraph records the original recovery gate.

The complete entity byte range and seven vtable slots are archived, but its
full hit/talk/normal/termination behavior has not yet been replayed. The seed
is not promoted: it uses cached control and convenience macro/speech helpers
whose movie/resource lifetimes require comparison with the native locals.

Next concrete work: recover and replay the entity's acquisition and movie
branches, hit-versus-talk macro sequence, quest card/teleporter/story effects,
PieceOver timing, and post-meeting repeat speech. Explicit RetailResources
currently has no direct resource Speak/task-poll operation; either prove an
existing safe route or add that specific native operation with actual-code
checks. Do not mix a new explicit resource with cached Speak without proof.

Also verify the quest-card/dialogue definitions and the override registry
identity before constructing an executable package. Full scheduler, live
macro playback, cancellation, save/load and streaming remain acceptance gates.
No installed configuration, seed files or converter outputs changed.
