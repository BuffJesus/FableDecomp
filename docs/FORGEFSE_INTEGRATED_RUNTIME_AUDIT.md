# ForgeFSE runtime binding audit

- registeredQuestBindings: **885**
- registeredEntityBindings: **91**
- requiredCalls: **84**
- callable: **83**
- missing: **1**
- wrongScope: **0**

## Conversion blockers

| Status | Scope | API | Calls | Consumers |
|---|---|---|---:|---|
| missing | Quest | `AddLogbookStoryEntry` | 1 | GuardianTrophyDealerInfo:FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua |

This report reflects names registered in `LuaManager.cpp`; it does not prove native ABI or runtime behavior.
