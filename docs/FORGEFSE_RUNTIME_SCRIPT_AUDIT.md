# ForgeFSE runtime binding audit

- registeredQuestBindings: **881**
- registeredEntityBindings: **89**
- requiredCalls: **84**
- callable: **77**
- missing: **6**
- wrongScope: **1**

## Conversion blockers

| Status | Scope | API | Calls | Consumers |
|---|---|---|---:|---|
| missing | Quest | `RunCutscene` | 11 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `EndCutscene` | 8 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `FadeOutAndKillEntity` | 6 | GuardianSisterInfo2:FSE/GuardianSisterInfo2/Entities/MazeAtTavern.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `StartCutscene` | 5 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Entity | `GetAngleXY` | 2 | GuardianTrophyDealerInfo:FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua |
| missing | Quest | `AddLogbookStoryEntry` | 1 | GuardianTrophyDealerInfo:FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua |
| wrong-scope | Entity | `SetIsPushableByHero` | 1 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua |

This report reflects names registered in `LuaManager.cpp`; it does not prove native ABI or runtime behavior.
