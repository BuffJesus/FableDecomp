# ForgeFSE seed-script compatibility report

- Scripts with trace templates: **71**
- API call sites: **3672** across **221** scope/name pairs
- Missing API names: **20**

## Missing capabilities

| Priority | API | Occurrences | Consumers |
|---:|---|---:|---|
| 60 | `Speak` | 30 | BeardyBaldy:FSE/BeardyBaldy/Entities/BB_BeardyBaldyMan.lua<br>SingingStones:FSE/SingingStones/Entities/ManWithDoorName.lua |
| 24 | `MsgIsHitBy` | 6 | SummoningTheShip:FSE/SummoningTheShip/Entities/FireHeart.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua<br>WaspBoss:FSE/WaspBoss/Entities/HornetDrone.lua |
| 15 | `MsgIsHitByAnySpecialAbilityFrom` | 5 | SummoningTheShip:FSE/SummoningTheShip/Entities/FireHeart.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua |
| 15 | `MsgIsHitBySpecialAbilityFrom` | 5 | SummoningTheShip:FSE/SummoningTheShip/Entities/FireHeart.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua<br>SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua |
| 6 | `AddLogbookTutorialEntry` | 3 | BowerstoneTownLifeIntro:FSE/BowerstoneTownLifeIntro/Entities/BowerstoneIntroGuard.lua<br>Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| 4 | `ActivateQuestWithoutLoadingResources` | 2 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua<br>TrophyDealer:FSE/TrophyDealer/TrophyDealer.lua |
| 4 | `IsXbox` | 2 | Fisherman:FSE/Fisherman/Entities/Fisherman.lua<br>Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| 4 | `MsgOnRegionLoaded` | 2 | BeardyBaldy:FSE/BeardyBaldy/Entities/BB_BeardyBaldyMan.lua<br>RockTrollFirstEncounter:FSE/RockTrollFirstEncounter/RockTrollFirstEncounter.lua |
| 3 | `GetValidTextEntryNameWithAttitude` | 3 | Fisherman:FSE/Fisherman/Entities/Fisherman.lua |
| 2 | `ActivateMultipleQuestsWithoutLoadingResources` | 2 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| 2 | `FollowPreCalculatedRoute` | 2 | WaspBoss:FSE/WaspBoss/Entities/WaspChaseWoman.lua |
| 2 | `IsFishingSpotEnabled` | 2 | Fisherman:FSE/Fisherman/Entities/Fisherman.lua |
| 1 | `ClearAllActions` | 1 | WaspBoss:FSE/WaspBoss/Entities/WaspHelper.lua |
| 1 | `ClearAllActionsIncludingLoopingAnimations` | 1 | WaspBoss:FSE/WaspBoss/Entities/HornetDrone.lua |
| 1 | `CloseDoor` | 1 | Fisherman:FSE/Fisherman/Entities/Fisherman.lua |
| 1 | `GetCurrentStateGroupType` | 1 | SummoningTheShip:FSE/SummoningTheShip/Entities/STS_BriarRose.lua |
| 1 | `PrepareQuestsWhenFinalQuestIsActivated` | 1 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| 1 | `PrepareQuestsWhenFinalQuestIsCompleted` | 1 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| 1 | `SetHeroGuideToShowQuestCardsWhenSpokenTo` | 1 | Gameflow:FSE/LUAGameflow/LUAGameflow.lua |
| 1 | `SummonerLightningOrbAttackTarget` | 1 | SummoningTheShip:FSE/SummoningTheShip/Entities/SummonerAttacker.lua |

## Interpretation

A missing capability means the reconstructed scripts call a name that is absent from the current
ForgeFSE/tutorial API manifest. It is a conversion blocker, not proof that the proposed signature or
behavior is correct. Each capability still needs native address, ABI, wrapper, and runtime validation.

Trace templates are ordered lexical call skeletons. Arguments and branch outcomes remain unresolved
until the native decompiler or a runtime fixture supplies them.
