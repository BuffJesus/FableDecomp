# ForgeFSE runtime binding audit

- registeredQuestBindings: **908**
- registeredEntityBindings: **92**
- requiredCalls: **221**
- callable: **209**
- missing: **12**
- wrongScope: **0**

## Conversion blockers

| Status | Scope | API | Calls | Consumers |
|---|---|---|---:|---|
| missing | Entity | `MsgIsHitBy` | 6 | SummoningTheShip:FSE/SummoningTheShip/Entities/FireHeart.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua<br>WaspBoss:FSE/WaspBoss/Entities/HornetDrone.lua |
| missing | Entity | `MsgIsHitByAnySpecialAbilityFrom` | 5 | SummoningTheShip:FSE/SummoningTheShip/Entities/FireHeart.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua |
| missing | Entity | `MsgIsHitBySpecialAbilityFrom` | 5 | SummoningTheShip:FSE/SummoningTheShip/Entities/FireHeart.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua |
| missing | Quest | `AddLogbookTutorialEntry` | 3 | BowerstoneTownLifeIntro:FSE/BowerstoneTownLifeIntro/Entities/BowerstoneIntroGuard.lua<br>Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| missing | Quest | `GetValidTextEntryNameWithAttitude` | 3 | Fisherman:FSE/Fisherman/Entities/Fisherman.lua |
| missing | Quest | `ActivateMultipleQuestsWithoutLoadingResources` | 2 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| missing | Quest | `ActivateQuestWithoutLoadingResources` | 2 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua<br>TrophyDealer:FSE/TrophyDealer/TrophyDealer.lua |
| missing | Quest | `IsXbox` | 2 | Fisherman:FSE/Fisherman/Entities/Fisherman.lua<br>Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| missing | Entity | `GetCurrentStateGroupType` | 1 | SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua |
| missing | Entity | `MoveToPosition_NonBlocking` | 1 | BeardyBaldy:FSE/BeardyBaldy/Entities/BB_BeardyBaldyMan.lua |
| missing | Quest | `PrepareQuestsWhenFinalQuestIsActivated` | 1 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| missing | Quest | `PrepareQuestsWhenFinalQuestIsCompleted` | 1 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |

This report reflects names registered in `LuaManager.cpp`; it does not prove native ABI or runtime behavior.
