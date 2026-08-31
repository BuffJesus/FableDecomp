# ForgeFSE seed-script compatibility report

- Scripts with trace templates: **16**
- API call sites: **542** across **84** scope/name pairs
- Missing API names: **6**

## Missing capabilities

| Priority | API | Occurrences | Consumers |
|---:|---|---:|---|
| 22 | `RunCutscene` | 11 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| 18 | `FadeOutAndKillEntity` | 6 | GuardianSisterInfo2:FSE/GuardianSisterInfo2/Entities/MazeAtTavern.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| 16 | `EndCutscene` | 8 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| 10 | `StartCutscene` | 5 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| 4 | `GetAngleXY` | 2 | GuardianTrophyDealerInfo:FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua |
| 1 | `AddLogbookStoryEntry` | 1 | GuardianTrophyDealerInfo:FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua |

## Interpretation

A missing capability means the reconstructed scripts call a name that is absent from the current
ForgeFSE/tutorial API manifest. It is a conversion blocker, not proof that the proposed signature or
behavior is correct. Each capability still needs native address, ABI, wrapper, and runtime validation.

Trace templates are ordered lexical call skeletons. Arguments and branch outcomes remain unresolved
until the native decompiler or a runtime fixture supplies them.
