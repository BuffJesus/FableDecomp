# Curated C++ promotion queue

Generated: `2026-07-27T23:18:16-06:00`

Uncompiled auto-RE candidates: **523**. Showing: **100**. Semantic-review quarantine: **130**.

Ranking favors checker/integrity/signature PASS and candidates without known source-level hazards, then the smallest declaration, dependency, VC7.1, and source-size repair surface. Structural fidelity does not by itself make an unsafe C++ expression promotable; every promotion still needs semantic review, a focused behavior oracle, and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Hazards | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---|---:|---:|---:|---|
| 1 | `0x0088F660` | `CGameScriptInterface::CGameScriptInterface::StopSound` | `compile-now` | `PASS` |  | 0 | 0 | 269 |  |
| 2 | `0x00891810` | `CGameScriptInterface::CGameScriptInterface::TellHeroQuestObjectiveFailed` | `compile-now` | `PASS` |  | 0 | 0 | 418 |  |
| 3 | `0x00891800` | `CGameScriptInterface::CGameScriptInterface::TellHeroQuestObjectiveCompleted` | `compile-now` | `PASS` |  | 0 | 0 | 429 |  |
| 4 | `0x00892F00` | `CGameScriptInterface::CGameScriptInterface::DeactivateQuest` | `compile-now` | `PASS` |  | 0 | 0 | 481 |  |
| 5 | `0x008914E0` | `CGameScriptInterface::CGameScriptInterface::RemoveQuestCardFromHero` | `compile-now` | `PASS` |  | 0 | 0 | 505 |  |
| 6 | `0x008917F0` | `CGameScriptInterface::CGameScriptInterface::GiveHeroNewQuestObjective` | `compile-now` | `PASS` |  | 0 | 0 | 505 |  |
| 7 | `0x0089CD50` | `CGameScriptInterface::CGameScriptInterface::CanThingBe_Smelled_ByOtherThing` | `vc71-port` | `PASS` |  | 0 | 1 | 601 |  |
| 8 | `0x00891C10` | `CGameScriptInterface::CGameScriptInterface::RemoveQuestInfoElement` | `vc71-port` | `PASS` |  | 0 | 2 | 626 |  |
| 9 | `0x00891BF0` | `CGameScriptInterface::CGameScriptInterface::UpdateQuestInfoTick` | `vc71-port` | `PASS` |  | 0 | 2 | 714 |  |
| 10 | `0x00891BB0` | `CGameScriptInterface::CGameScriptInterface::UpdateQuestInfoTimer` | `vc71-port` | `PASS` |  | 0 | 2 | 729 |  |
| 11 | `0x008A9E60` | `CGameScriptInterface::CGameScriptInterface::AddGossipFactionToCategory` | `vc71-port` | `PASS` |  | 0 | 2 | 765 |  |
| 12 | `0x00891BD0` | `CGameScriptInterface::CGameScriptInterface::UpdateQuestInfoCounter` | `vc71-port` | `PASS` |  | 0 | 2 | 770 |  |
| 13 | `0x00891BE0` | `CGameScriptInterface::CGameScriptInterface::UpdateQuestInfoCounterList` | `vc71-port` | `PASS` |  | 0 | 2 | 799 |  |
| 14 | `0x0088F400` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled` | `vc71-port` | `PASS` |  | 0 | 2 | 849 |  |
| 15 | `0x00891B50` | `CGameScriptInterface::CGameScriptInterface::AddQuestInfoCounterList` | `vc71-port` | `PASS` |  | 0 | 2 | 859 |  |
| 16 | `0x00891B80` | `CGameScriptInterface::CGameScriptInterface::UpdateQuestInfoBar` | `vc71-port` | `PASS` |  | 0 | 2 | 871 |  |
| 17 | `0x00891B20` | `CGameScriptInterface::CGameScriptInterface::AddQuestInfoTimer` | `vc71-port` | `PASS` |  | 0 | 2 | 888 |  |
| 18 | `0x00891BA0` | `CGameScriptInterface::CGameScriptInterface::ChangeQuestInfoBarColour` | `vc71-port` | `PASS` |  | 0 | 2 | 980 |  |
| 19 | `0x0088F910` | `CGameScriptInterface::CGameScriptInterface::AutoSaveCheckPoint` | `vc71-port` | `PASS` |  | 0 | 3 | 558 |  |
| 20 | `0x0088F920` | `CGameScriptInterface::CGameScriptInterface::AutoSaveQuestStart` | `vc71-port` | `PASS` |  | 0 | 3 | 559 |  |
| 21 | `0x0088F970` | `CGameScriptInterface::CGameScriptInterface::ResetToFrontEnd` | `vc71-port` | `PASS` |  | 0 | 3 | 649 |  |
| 22 | `0x008CFE30` | `CGameScriptThing::CGameScriptThing::GetHomePos` | `vc71-port` | `PASS` |  | 0 | 4 | 619 |  |
| 23 | `0x0088F610` | `CGameScriptInterface::CGameScriptInterface::IsSoundPlaying` | `vc71-port` | `PASS` |  | 0 | 4 | 787 |  |
| 24 | `0x0088E130` | `CGameScriptInterface::CGameScriptInterface::GiveHeroWeapon` | `vc71-port` | `PASS` |  | 0 | 4 | 858 |  |
| 25 | `0x0088DE10` | `CGameScriptInterface::CGameScriptInterface::GiveThingItemInHand` | `vc71-port` | `PASS` |  | 0 | 4 | 1289 |  |
| 26 | `0x0088F710` | `CGameScriptInterface::CGameScriptInterface::EntitySetCombatEnabled` | `vc71-port` | `PASS` |  | 0 | 5 | 1715 |  |
| 27 | `0x00891820` | `CGameScriptInterface::CGameScriptInterface::AddQuestRegion` | `vc71-port` | `PASS` |  | 0 | 5 | 1872 |  |
| 28 | `0x00891330` | `CGameScriptInterface::CGameScriptInterface::IsDangerMusicEnabled` | `vc71-port` | `PASS` |  | 0 | 6 | 1114 |  |
| 29 | `0x0088F8D0` | `CGameScriptInterface::CGameScriptInterface::StartCountdownTimer` | `vc71-port` | `PASS` |  | 0 | 6 | 1136 |  |
| 30 | `0x0088FDC0` | `CGameScriptInterface::CGameScriptInterface::SetTimeOfDay` | `vc71-port` | `PASS` |  | 0 | 6 | 1376 |  |
| 31 | `0x0088FE90` | `CGameScriptInterface::CGameScriptInterface::FastForwardTimeTo` | `vc71-port` | `PASS` |  | 0 | 6 | 1528 |  |
| 32 | `0x0088E300` | `CGameScriptInterface::CGameScriptInterface::IsRegionLoaded` | `vc71-port` | `PASS` |  | 0 | 6 | 1560 |  |
| 33 | `0x0088F9B0` | `CGameScriptInterface::CGameScriptInterface::GetGuildSealRecallPos` | `vc71-port` | `PASS` |  | 0 | 7 | 888 |  |
| 34 | `0x0088E380` | `CGameScriptInterface::CGameScriptInterface::DontPopulateNextLoadedRegion` | `vc71-port` | `PASS` |  | 0 | 7 | 905 |  |
| 35 | `0x0088DF60` | `CGameScriptInterface::CGameScriptInterface::GetWaterHeightAtPosition` | `vc71-port` | `PASS` |  | 0 | 7 | 1152 |  |
| 36 | `0x004AAAF0` | `CScriptThing::CScriptThing::MsgIsKilledBy` | `vc71-port` | `PASS` |  | 0 | 7 | 1192 |  |
| 37 | `0x008907D0` | `CGameScriptInterface::CGameScriptInterface::IsConversationActive` | `vc71-port` | `PASS` |  | 0 | 7 | 1197 |  |
| 38 | `0x0088F480` | `CGameScriptInterface::CGameScriptInterface::ApplyScriptBrush` | `vc71-port` | `PASS` |  | 0 | 7 | 1510 |  |
| 39 | `0x0088E6F0` | `CGameScriptInterface::CGameScriptInterface::AreEntitiesEnemies` | `vc71-port` | `PASS` |  | 0 | 7 | 1610 |  |
| 40 | `0x0088FF30` | `CGameScriptInterface::CGameScriptInterface::TransitionToThemeAllInternals` | `vc71-port` | `PASS` |  | 0 | 7 | 1906 |  |
| 41 | `0x004AAC10` | `CScriptThing::CScriptThing::MsgHowLongWasExpressionPerformed` | `vc71-port` | `PASS` |  | 0 | 8 | 1122 |  |
| 42 | `0x00891310` | `CGameScriptInterface::CGameScriptInterface::CacheMusicSet` | `vc71-port` | `PASS` |  | 0 | 8 | 1168 |  |
| 43 | `0x0088E200` | `CGameScriptInterface::CGameScriptInterface::SetGuildMasterMessages` | `vc71-port` | `PASS` |  | 0 | 9 | 933 |  |
| 44 | `0x006E7690` | `CGameScriptInterface::CGameScriptInterface::DeregisterTimer` | `vc71-port` | `PASS` |  | 0 | 9 | 1559 |  |
| 45 | `0x0088E0B0` | `CGameScriptInterface::CGameScriptInterface::SetSoundThemesAsEnabledForRegion` | `vc71-port` | `PASS` |  | 0 | 9 | 2272 |  |
| 46 | `0x008907E0` | `CGameScriptInterface::CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn` | `vc71-port` | `PASS` |  | 0 | 10 | 1199 |  |
| 47 | `0x00894490` | `CGameScriptInterface::CGameScriptInterface::RemoveDeadCreature` | `vc71-port` | `PASS` |  | 0 | 10 | 2398 |  |
| 48 | `0x00890200` | `CGameScriptInterface::CGameScriptInterface::ScreenFilterFadeOut` | `vc71-port` | `PASS` |  | 0 | 11 | 2093 |  |
| 49 | `0x0088FC90` | `CGameScriptInterface::CGameScriptInterface::HeroHasExpression` | `vc71-port` | `PASS` |  | 0 | 12 | 1943 |  |
| 50 | `0x0088FD00` | `CGameScriptInterface::CGameScriptInterface::EntitySetInLimbo` | `vc71-port` | `PASS` |  | 0 | 13 | 1625 |  |
| 51 | `0x00890B90` | `CGameScriptInterface::CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `vc71-port` | `PASS` |  | 0 | 13 | 2519 |  |
| 52 | `0x0088ECE0` | `CGameScriptInterface::CGameScriptInterface::CameraShake` | `vc71-port` | `PASS` |  | 0 | 14 | 2350 |  |
| 53 | `0x008A9320` | `CGameScriptInterface::CGameScriptInterface::EntityCastForcePush` | `vc71-port` | `PASS` |  | 0 | 14 | 2708 |  |
| 54 | `0x006E79E0` | `CGameScriptInterface::CGameScriptInterface::RegisterTimer` | `vc71-port` | `PASS` |  | 0 | 14 | 2736 |  |
| 55 | `0x00893060` | `CGameScriptInterface::CGameScriptInterface::AddBoast` | `vc71-port` | `PASS` |  | 0 | 16 | 2844 |  |
| 56 | `0x00893240` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelUnloaded` | `vc71-port` | `PASS` |  | 0 | 16 | 3236 |  |
| 57 | `0x008A8E40` | `CGameScriptInterface::CGameScriptInterface::EntityDecapitate` | `vc71-port` | `PASS` |  | 0 | 18 | 2947 |  |
| 58 | `0x00891070` | `CGameScriptInterface::CGameScriptInterface::CameraUseCameraPoint` | `vc71-port` | `PASS` |  | 0 | 18 | 3093 |  |
| 59 | `0x008A8EB0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAttackThingImmediately` | `vc71-port` | `PASS` |  | 0 | 18 | 3229 |  |
| 60 | `0x00891EF0` | `CGameScriptInterface::CGameScriptInterface::IsHeroAllowedHenchmenInRegion` | `vc71-port` | `PASS` |  | 0 | 19 | 2856 |  |
| 61 | `0x00891EA0` | `CGameScriptInterface::CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion` | `vc71-port` | `PASS` |  | 0 | 20 | 2600 |  |
| 62 | `0x008A9610` | `CGameScriptInterface::CGameScriptInterface::SetThingAsConscious` | `vc71-port` | `PASS` |  | 0 | 20 | 3648 |  |
| 63 | `0x008931B0` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelLoaded` | `vc71-port` | `PASS` |  | 0 | 20 | 3672 |  |
| 64 | `0x0089AF30` | `CGameScriptInterface::CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `vc71-port` | `PASS` |  | 0 | 20 | 4247 |  |
| 65 | `0x00894DD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsMarryable` | `vc71-port` | `PASS` |  | 0 | 22 | 2481 |  |
| 66 | `0x0089B3F0` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsWearing` | `vc71-port` | `PASS` |  | 0 | 24 | 2715 |  |
| 67 | `0x0089E130` | `CGameScriptInterface::CGameScriptInterface::EntitySetMaxNumberOfAttackers` | `vc71-port` | `PASS` |  | 0 | 24 | 2810 |  |
| 68 | `0x008984E0` | `CGameScriptInterface::CGameScriptInterface::SetWeaponOutCrimeEnabled` | `vc71-port` | `PASS` |  | 0 | 25 | 3309 |  |
| 69 | `0x00892410` | `CGameScriptInterface::CGameScriptInterface::CameraMoveToPosAndLookAtPos` | `vc71-port` | `PASS` |  | 0 | 26 | 3451 |  |
| 70 | `0x0088F570` | `CGameScriptInterface::CGameScriptInterface::Play2DSound` | `vc71-port` | `PASS` |  | 0 | 29 | 3029 |  |
| 71 | `0x00891220` | `CGameScriptInterface::CGameScriptInterface::OverrideMusic` | `vc71-port` | `PASS` |  | 0 | 31 | 3466 |  |
| 72 | `0x00891720` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHealthBy` | `vc71-port` | `PASS` |  | 0 | 31 | 5204 |  |
| 73 | `0x006E7920` | `CGameScriptInterface::CGameScriptInterface::CheckForCameraMessage` | `vc71-port` | `PASS` |  | 0 | 31 | 5901 |  |
| 74 | `0x00893030` | `CGameScriptInterface::CGameScriptInterface::GetExclusiveQuestScriptName` | `declaration-fix` | `PASS` |  | 0 | 1 | 528 | 'reinterpret_cast' from integer to pointer |
| 75 | `0x00892D70` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript` | `declaration-fix` | `PASS` |  | 0 | 1 | 679 | 'reinterpret_cast' from integer to pointer |
| 76 | `0x00892F40` | `CGameScriptInterface::CGameScriptInterface::IsQuestActive` | `declaration-fix` | `PASS` |  | 0 | 2 | 570 | 'reinterpret_cast' from integer to pointer |
| 77 | `0x00892F70` | `CGameScriptInterface::CGameScriptInterface::IsQuestFailed` | `declaration-fix` | `PASS` |  | 0 | 2 | 570 | 'reinterpret_cast' from integer to pointer |
| 78 | `0x00891920` | `CGameScriptInterface::CGameScriptInterface::DisplayQuestInfo` | `declaration-fix` | `PASS` |  | 0 | 3 | 760 | no matching function for call to 'NPlayerGui::CDrawQuestInfo::DisplayQuestInfo(NPlayerGui::CDrawQuestInfo*&, bool) const' |
| 79 | `0x00891AC0` | `CGameScriptInterface::CGameScriptInterface::AddQuestInfoBar` | `declaration-fix` | `PASS` |  | 0 | 3 | 1499 | no matching function for call to 'NPlayerGui::CDrawQuestInfo::AddBar(NPlayerGui::CDrawQuestInfo*&, float&, float&, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float&) const' |
| 80 | `0x008AA010` | `CGameScriptInterface::CGameScriptInterface::ClearGossip` | `declaration-fix` | `PASS` |  | 0 | 5 | 1275 | static assertion failed |
| 81 | `0x008907C0` | `CGameScriptInterface::CGameScriptInterface::RemoveConversation` | `declaration-fix` | `PASS` |  | 0 | 7 | 1270 | 'reinterpret_cast' from integer to pointer |
| 82 | `0x008915B0` | `CGameScriptInterface::CGameScriptInterface::SetBoastAsFailed` | `declaration-fix` | `PASS` |  | 0 | 7 | 1128 | 'reinterpret_cast' from integer to pointer |
| 83 | `0x0088E2A0` | `CGameScriptInterface::CGameScriptInterface::IsLevelLoaded` | `declaration-fix` | `PASS` |  | 0 | 11 | 2394 | static assertion failed |
| 84 | `0x00892CF0` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabled` | `declaration-fix` | `PASS` |  | 0 | 14 | 3296 | jump to label 'SetWorldFlag' |
| 85 | `0x00894C90` | `CGameScriptInterface::CGameScriptInterface::IsGiftFriendly` | `declaration-fix` | `PASS` |  | 0 | 16 | 5128 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 86 | `0x00894BF0` | `CGameScriptInterface::CGameScriptInterface::IsGiftRomantic` | `declaration-fix` | `PASS` |  | 0 | 16 | 5210 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 87 | `0x00894D30` | `CGameScriptInterface::CGameScriptInterface::IsGiftOffensive` | `declaration-fix` | `PASS` |  | 0 | 16 | 5212 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 88 | `0x0089EC60` | `CGameScriptInterface::CGameScriptInterface::CreatureGeneratorAddTriggerer` | `declaration-fix` | `PASS` |  | 0 | 20 | 3701 | comparison between distinct pointer types '{anonymous}::TCInterfaceEntryOverlay*' and 'CVectorMap<int, CTCBase*, CKeyPairCompareLess<int, CTCBase*> >::Entry*' lacks a cast |
| 89 | `0x00899220` | `CGameScriptInterface::CGameScriptInterface::IncreaseHeroRenownLevel` | `declaration-fix` | `PASS` |  | 0 | 26 | 3065 | 'reinterpret_cast' from integer to pointer |
| 90 | `0x0089C330` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionDeedTypeEnabled` | `declaration-fix` | `PASS` |  | 0 | 27 | 3325 | incomplete type '{anonymous}::OpinionDeedInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 91 | `0x0089C3D0` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionReactionEnabled` | `declaration-fix` | `PASS` |  | 0 | 29 | 3454 | incomplete type '{anonymous}::OpinionReactionInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 92 | `0x00896EE0` | `CGameScriptInterface::CGameScriptInterface::GetDeathRecoveryMarkerName` | `declaration-fix` | `PASS` |  | 0 | 30 | 3294 | static assertion failed |
| 93 | `0x00891170` | `CGameScriptInterface::CGameScriptInterface::PlaySoundAtPos` | `declaration-fix` | `PASS` |  | 0 | 33 | 3318 | 'reinterpret_cast' from integer to pointer |
| 94 | `0x008981C0` | `CGameScriptInterface::CGameScriptInterface::AddTattooToHero` | `declaration-fix` | `PASS` |  | 0 | 48 | 9170 | static assertion failed |
| 95 | `0x0089FEB0` | `CGameScriptInterface::CGameScriptInterface::AddCrimeCommitted` | `declaration-fix` | `PASS` |  | 0 | 69 | 12255 | 'reinterpret_cast' from integer to pointer |
| 96 | `0x00449B60` | `CPlayerManager::GetMultiplayerColour` | `dependency-stub` | `PASS` |  | 1 | 0 | 436 | 'CRGBColour' does not name a type |
| 97 | `0x008A9E30` | `CGameScriptInterface::CGameScriptInterface::AddGossipVillage` | `dependency-stub` | `PASS` |  | 1 | 0 | 475 | invalid use of incomplete type 'class CGameScriptInterface' |
| 98 | `0x008A9DD0` | `CGameScriptInterface::CGameScriptInterface::AddNewRumourToCategory` | `dependency-stub` | `PASS` |  | 1 | 0 | 640 | invalid use of incomplete type 'class CGameScriptInterface' |
| 99 | `0x00892F10` | `CGameScriptInterface::CGameScriptInterface::DeactivateQuestLater` | `dependency-stub` | `PASS` |  | 1 | 0 | 380 | expected '(' before '{' token |
| 100 | `0x00891540` | `CGameScriptInterface::CGameScriptInterface::AddFeatCard` | `dependency-stub` | `PASS` |  | 1 | 1 | 427 | invalid use of incomplete type 'class CGameScriptInterface' |

## Semantic-review quarantine

These candidates may structurally match the retail path, but their generated C++ contains a known source-level hazard and is intentionally ranked behind ordinary manual lifts.

| Address | Owner/function | Hazards |
|---|---|---|
| `0x00899EF0` | `CGameScriptInterface::CGameScriptInterface::GiveHeroMorality` | possible-end-sentinel-dereference |
| `0x00897150` | `CGameScriptInterface::CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | possible-end-sentinel-dereference |
| `0x00896060` | `CGameScriptInterface::CGameScriptInterface::SetVillageLimbo` | possible-end-sentinel-dereference |
| `0x00897B10` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasCurrentMarriage` | possible-end-sentinel-dereference |
| `0x00897BD0` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasChildren` | possible-end-sentinel-dereference |
| `0x0089B4D0` | `CGameScriptInterface::CGameScriptInterface::RemoveHeroHairstyle` | possible-end-sentinel-dereference |
| `0x00899DB0` | `CGameScriptInterface::CGameScriptInterface::IsPlayerCreatureFlourishEnabled` | possible-end-sentinel-dereference |
| `0x008994E0` | `CGameScriptInterface::CGameScriptInterface::SetHeroAge` | possible-end-sentinel-dereference |
| `0x00899470` | `CGameScriptInterface::CGameScriptInterface::GetHeroWillLevel` | possible-end-sentinel-dereference |
| `0x00899400` | `CGameScriptInterface::CGameScriptInterface::GetHeroSkillLevel` | possible-end-sentinel-dereference |
| `0x00899390` | `CGameScriptInterface::CGameScriptInterface::GetHeroStrengthLevel` | possible-end-sentinel-dereference |
| `0x00899C90` | `CGameScriptInterface::CGameScriptInterface::GetPlayerCreatureCombatMultiplier` | possible-end-sentinel-dereference |
| `0x008A1590` | `CGameScriptInterface::CGameScriptInterface::GetBestTimePairs` | possible-end-sentinel-dereference |
| `0x008A15F0` | `CGameScriptInterface::CGameScriptInterface::GetBestTimeSorting` | possible-end-sentinel-dereference |
| `0x008A17D0` | `CGameScriptInterface::CGameScriptInterface::GetBestTimeGuessTheAddition` | possible-end-sentinel-dereference |
| `0x00897C30` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasMurderedWife` | possible-end-sentinel-dereference |
| `0x00897B70` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasDivorcedMarriage` | possible-end-sentinel-dereference |
| `0x00897C90` | `CGameScriptInterface::CGameScriptInterface::GetNumberOfTimesHeroHasHadSex` | possible-end-sentinel-dereference |
| `0x008A16B0` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreCoinGolfOakVale` | possible-end-sentinel-dereference |
| `0x00899F90` | `CGameScriptInterface::CGameScriptInterface::GetHeroMorality` | possible-end-sentinel-dereference |
| `0x008974F0` | `CGameScriptInterface::CGameScriptInterface::IsEntityWieldingWeapon` | possible-end-sentinel-dereference |
| `0x00897030` | `CGameScriptInterface::CGameScriptInterface::IsToFailQuestOnDeath` | possible-end-sentinel-dereference |
| `0x00897480` | `CGameScriptInterface::CGameScriptInterface::IsPlayerWieldingWeapon` | possible-end-sentinel-dereference |
| `0x0089A000` | `CGameScriptInterface::CGameScriptInterface::GetHeroMoralityCategory` | possible-end-sentinel-dereference |
| `0x00897AA0` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasMarried` | possible-end-sentinel-dereference |
|  | _105 additional quarantined candidates omitted_ |  |
