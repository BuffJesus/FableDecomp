# ForgeFSE runtime binding audit

- registeredQuestBindings: **881**
- registeredEntityBindings: **91**
- requiredCalls: **84**
- callable: **79**
- missing: **5**
- wrongScope: **0**

## Conversion blockers

| Status | Scope | API | Calls | Consumers |
|---|---|---|---:|---|
| missing | Quest | `RunCutscene` | 11 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `EndCutscene` | 8 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `FadeOutAndKillEntity` | 6 | GuardianSisterInfo2:FSE/GuardianSisterInfo2/Entities/MazeAtTavern.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `StartCutscene` | 5 | HerosOldHouse:FSE/HerosOldHouse/Entities/FishermansWife.lua<br>HerosOldHouse:FSE/HerosOldHouse/Entities/GhostFisherman.lua |
| missing | Quest | `AddLogbookStoryEntry` | 1 | GuardianTrophyDealerInfo:FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua |

This report reflects names registered in `LuaManager.cpp`; it does not prove native ABI or runtime behavior.
