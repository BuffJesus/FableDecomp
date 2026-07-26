# Curated C++ promotion queue

Generated: `2026-07-25T17:58:49-06:00`

Uncompiled auto-RE candidates: **287**. Showing: **100**. Semantic-review quarantine: **110**.

Ranking favors checker/integrity/signature PASS and candidates without known source-level hazards, then the smallest declaration, dependency, VC7.1, and source-size repair surface. Structural fidelity does not by itself make an unsafe C++ expression promotable; every promotion still needs semantic review, a focused behavior oracle, and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Hazards | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---|---:|---:|---:|---|
| 1 | `0x0088F660` | `CGameScriptInterface::CGameScriptInterface::StopSound` | `compile-now` | `PASS` |  | 0 | 0 | 282 |  |
| 2 | `0x008917F0` | `CGameScriptInterface::CGameScriptInterface::GiveHeroNewQuestObjective` | `compile-now` | `PASS` |  | 0 | 0 | 525 |  |
| 3 | `0x008914E0` | `CGameScriptInterface::CGameScriptInterface::RemoveQuestCardFromHero` | `compile-now` | `PASS` |  | 0 | 0 | 526 |  |
| 4 | `0x0089CD50` | `CGameScriptInterface::CGameScriptInterface::CanThingBe_Smelled_ByOtherThing` | `vc71-port` | `PASS` |  | 0 | 1 | 624 |  |
| 5 | `0x00891C10` | `CGameScriptInterface::CGameScriptInterface::RemoveQuestInfoElement` | `vc71-port` | `PASS` |  | 0 | 2 | 656 |  |
| 6 | `0x0088F400` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled` | `vc71-port` | `PASS` |  | 0 | 2 | 886 |  |
| 7 | `0x00891B50` | `CGameScriptInterface::CGameScriptInterface::AddQuestInfoCounterList` | `vc71-port` | `PASS` |  | 0 | 2 | 901 |  |
| 8 | `0x00891B20` | `CGameScriptInterface::CGameScriptInterface::AddQuestInfoTimer` | `vc71-port` | `PASS` |  | 0 | 2 | 931 |  |
| 9 | `0x00891BA0` | `CGameScriptInterface::CGameScriptInterface::ChangeQuestInfoBarColour` | `vc71-port` | `PASS` |  | 0 | 2 | 1024 |  |
| 10 | `0x0088F910` | `CGameScriptInterface::CGameScriptInterface::AutoSaveCheckPoint` | `vc71-port` | `PASS` |  | 0 | 3 | 586 |  |
| 11 | `0x0088F920` | `CGameScriptInterface::CGameScriptInterface::AutoSaveQuestStart` | `vc71-port` | `PASS` |  | 0 | 3 | 588 |  |
| 12 | `0x0088F970` | `CGameScriptInterface::CGameScriptInterface::ResetToFrontEnd` | `vc71-port` | `PASS` |  | 0 | 3 | 683 |  |
| 13 | `0x008CFE30` | `CGameScriptThing::CGameScriptThing::GetHomePos` | `vc71-port` | `PASS` |  | 0 | 4 | 651 |  |
| 14 | `0x0088F610` | `CGameScriptInterface::CGameScriptInterface::IsSoundPlaying` | `vc71-port` | `PASS` |  | 0 | 4 | 817 |  |
| 15 | `0x0088E130` | `CGameScriptInterface::CGameScriptInterface::GiveHeroWeapon` | `vc71-port` | `PASS` |  | 0 | 4 | 885 |  |
| 16 | `0x0088DE10` | `CGameScriptInterface::CGameScriptInterface::GiveThingItemInHand` | `vc71-port` | `PASS` |  | 0 | 4 | 1338 |  |
| 17 | `0x0088F710` | `CGameScriptInterface::CGameScriptInterface::EntitySetCombatEnabled` | `vc71-port` | `PASS` |  | 0 | 5 | 1780 |  |
| 18 | `0x00891330` | `CGameScriptInterface::CGameScriptInterface::IsDangerMusicEnabled` | `vc71-port` | `PASS` |  | 0 | 6 | 1161 |  |
| 19 | `0x0088F8D0` | `CGameScriptInterface::CGameScriptInterface::StartCountdownTimer` | `vc71-port` | `PASS` |  | 0 | 6 | 1183 |  |
| 20 | `0x0088FDC0` | `CGameScriptInterface::CGameScriptInterface::SetTimeOfDay` | `vc71-port` | `PASS` |  | 0 | 6 | 1428 |  |
| 21 | `0x0088FE90` | `CGameScriptInterface::CGameScriptInterface::FastForwardTimeTo` | `vc71-port` | `PASS` |  | 0 | 6 | 1586 |  |
| 22 | `0x0088E300` | `CGameScriptInterface::CGameScriptInterface::IsRegionLoaded` | `vc71-port` | `PASS` |  | 0 | 6 | 1619 |  |
| 23 | `0x0088F9B0` | `CGameScriptInterface::CGameScriptInterface::GetGuildSealRecallPos` | `vc71-port` | `PASS` |  | 0 | 7 | 928 |  |
| 24 | `0x0088E380` | `CGameScriptInterface::CGameScriptInterface::DontPopulateNextLoadedRegion` | `vc71-port` | `PASS` |  | 0 | 7 | 933 |  |
| 25 | `0x004AAAF0` | `CScriptThing::CScriptThing::MsgIsKilledBy` | `vc71-port` | `PASS` |  | 0 | 7 | 1235 |  |
| 26 | `0x008907D0` | `CGameScriptInterface::CGameScriptInterface::IsConversationActive` | `vc71-port` | `PASS` |  | 0 | 7 | 1243 |  |
| 27 | `0x0088F480` | `CGameScriptInterface::CGameScriptInterface::ApplyScriptBrush` | `vc71-port` | `PASS` |  | 0 | 7 | 1566 |  |
| 28 | `0x0088E6F0` | `CGameScriptInterface::CGameScriptInterface::AreEntitiesEnemies` | `vc71-port` | `PASS` |  | 0 | 7 | 1667 |  |
| 29 | `0x004AAC10` | `CScriptThing::CScriptThing::MsgHowLongWasExpressionPerformed` | `vc71-port` | `PASS` |  | 0 | 8 | 1161 |  |
| 30 | `0x00891310` | `CGameScriptInterface::CGameScriptInterface::CacheMusicSet` | `vc71-port` | `PASS` |  | 0 | 8 | 1208 |  |
| 31 | `0x0088E200` | `CGameScriptInterface::CGameScriptInterface::SetGuildMasterMessages` | `vc71-port` | `PASS` |  | 0 | 9 | 961 |  |
| 32 | `0x006E7690` | `CGameScriptInterface::CGameScriptInterface::DeregisterTimer` | `vc71-port` | `PASS` |  | 0 | 9 | 1618 |  |
| 33 | `0x0088E0B0` | `CGameScriptInterface::CGameScriptInterface::SetSoundThemesAsEnabledForRegion` | `vc71-port` | `PASS` |  | 0 | 9 | 2357 |  |
| 34 | `0x008907E0` | `CGameScriptInterface::CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn` | `vc71-port` | `PASS` |  | 0 | 10 | 1239 |  |
| 35 | `0x00894490` | `CGameScriptInterface::CGameScriptInterface::RemoveDeadCreature` | `vc71-port` | `PASS` |  | 0 | 10 | 2486 |  |
| 36 | `0x00890200` | `CGameScriptInterface::CGameScriptInterface::ScreenFilterFadeOut` | `vc71-port` | `PASS` |  | 0 | 11 | 2156 |  |
| 37 | `0x0088FC90` | `CGameScriptInterface::CGameScriptInterface::HeroHasExpression` | `vc71-port` | `PASS` |  | 0 | 12 | 2007 |  |
| 38 | `0x0088FD00` | `CGameScriptInterface::CGameScriptInterface::EntitySetInLimbo` | `vc71-port` | `PASS` |  | 0 | 13 | 1683 |  |
| 39 | `0x00890B90` | `CGameScriptInterface::CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `vc71-port` | `PASS` |  | 0 | 13 | 2606 |  |
| 40 | `0x0088ECE0` | `CGameScriptInterface::CGameScriptInterface::CameraShake` | `vc71-port` | `PASS` |  | 0 | 14 | 2424 |  |
| 41 | `0x008A9320` | `CGameScriptInterface::CGameScriptInterface::EntityCastForcePush` | `vc71-port` | `PASS` |  | 0 | 14 | 2807 |  |
| 42 | `0x006E79E0` | `CGameScriptInterface::CGameScriptInterface::RegisterTimer` | `vc71-port` | `PASS` |  | 0 | 14 | 2847 |  |
| 43 | `0x00893240` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelUnloaded` | `vc71-port` | `PASS` |  | 0 | 16 | 3344 |  |
| 44 | `0x008A8E40` | `CGameScriptInterface::CGameScriptInterface::EntityDecapitate` | `vc71-port` | `PASS` |  | 0 | 18 | 3040 |  |
| 45 | `0x008A8EB0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAttackThingImmediately` | `vc71-port` | `PASS` |  | 0 | 18 | 3339 |  |
| 46 | `0x00891EF0` | `CGameScriptInterface::CGameScriptInterface::IsHeroAllowedHenchmenInRegion` | `vc71-port` | `PASS` |  | 0 | 19 | 2961 |  |
| 47 | `0x00891EA0` | `CGameScriptInterface::CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion` | `vc71-port` | `PASS` |  | 0 | 20 | 2698 |  |
| 48 | `0x008A9610` | `CGameScriptInterface::CGameScriptInterface::SetThingAsConscious` | `vc71-port` | `PASS` |  | 0 | 20 | 3774 |  |
| 49 | `0x008931B0` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelLoaded` | `vc71-port` | `PASS` |  | 0 | 20 | 3789 |  |
| 50 | `0x0089AF30` | `CGameScriptInterface::CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `vc71-port` | `PASS` |  | 0 | 20 | 4386 |  |
| 51 | `0x00894DD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsMarryable` | `vc71-port` | `PASS` |  | 0 | 22 | 2571 |  |
| 52 | `0x0089B3F0` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsWearing` | `vc71-port` | `PASS` |  | 0 | 24 | 2796 |  |
| 53 | `0x0089E130` | `CGameScriptInterface::CGameScriptInterface::EntitySetMaxNumberOfAttackers` | `vc71-port` | `PASS` |  | 0 | 24 | 2897 |  |
| 54 | `0x008984E0` | `CGameScriptInterface::CGameScriptInterface::SetWeaponOutCrimeEnabled` | `vc71-port` | `PASS` |  | 0 | 25 | 3413 |  |
| 55 | `0x00892410` | `CGameScriptInterface::CGameScriptInterface::CameraMoveToPosAndLookAtPos` | `vc71-port` | `PASS` |  | 0 | 26 | 3577 |  |
| 56 | `0x0088F570` | `CGameScriptInterface::CGameScriptInterface::Play2DSound` | `vc71-port` | `PASS` |  | 0 | 29 | 3144 |  |
| 57 | `0x00891220` | `CGameScriptInterface::CGameScriptInterface::OverrideMusic` | `vc71-port` | `PASS` |  | 0 | 31 | 3589 |  |
| 58 | `0x00891720` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHealthBy` | `vc71-port` | `PASS` |  | 0 | 31 | 5345 |  |
| 59 | `0x006E7920` | `CGameScriptInterface::CGameScriptInterface::CheckForCameraMessage` | `vc71-port` | `PASS` |  | 0 | 31 | 6101 |  |
| 60 | `0x00893030` | `CGameScriptInterface::CGameScriptInterface::GetExclusiveQuestScriptName` | `declaration-fix` | `PASS` |  | 0 | 1 | 553 | 'reinterpret_cast' from integer to pointer |
| 61 | `0x00892D70` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript` | `declaration-fix` | `PASS` |  | 0 | 1 | 706 | 'reinterpret_cast' from integer to pointer |
| 62 | `0x00892F40` | `CGameScriptInterface::CGameScriptInterface::IsQuestActive` | `declaration-fix` | `PASS` |  | 0 | 2 | 593 | 'reinterpret_cast' from integer to pointer |
| 63 | `0x00892F70` | `CGameScriptInterface::CGameScriptInterface::IsQuestFailed` | `declaration-fix` | `PASS` |  | 0 | 2 | 593 | 'reinterpret_cast' from integer to pointer |
| 64 | `0x00891920` | `CGameScriptInterface::CGameScriptInterface::DisplayQuestInfo` | `declaration-fix` | `PASS` |  | 0 | 3 | 796 | no matching function for call to 'NPlayerGui::CDrawQuestInfo::DisplayQuestInfo(NPlayerGui::CDrawQuestInfo*&, bool) const' |
| 65 | `0x00891AC0` | `CGameScriptInterface::CGameScriptInterface::AddQuestInfoBar` | `declaration-fix` | `PASS` |  | 0 | 3 | 1565 | no matching function for call to 'NPlayerGui::CDrawQuestInfo::AddBar(NPlayerGui::CDrawQuestInfo*&, float&, float&, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float&) const' |
| 66 | `0x008907C0` | `CGameScriptInterface::CGameScriptInterface::RemoveConversation` | `declaration-fix` | `PASS` |  | 0 | 7 | 1315 | 'reinterpret_cast' from integer to pointer |
| 67 | `0x008915B0` | `CGameScriptInterface::CGameScriptInterface::SetBoastAsFailed` | `declaration-fix` | `PASS` |  | 0 | 7 | 1169 | 'reinterpret_cast' from integer to pointer |
| 68 | `0x0088E2A0` | `CGameScriptInterface::CGameScriptInterface::IsLevelLoaded` | `declaration-fix` | `PASS` |  | 0 | 11 | 2485 | static assertion failed |
| 69 | `0x00892CF0` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabled` | `declaration-fix` | `PASS` |  | 0 | 14 | 3432 | jump to label 'SetWorldFlag' |
| 70 | `0x00894C90` | `CGameScriptInterface::CGameScriptInterface::IsGiftFriendly` | `declaration-fix` | `PASS` |  | 0 | 16 | 5278 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 71 | `0x00894BF0` | `CGameScriptInterface::CGameScriptInterface::IsGiftRomantic` | `declaration-fix` | `PASS` |  | 0 | 16 | 5364 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 72 | `0x00894D30` | `CGameScriptInterface::CGameScriptInterface::IsGiftOffensive` | `declaration-fix` | `PASS` |  | 0 | 16 | 5366 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 73 | `0x0089EC60` | `CGameScriptInterface::CGameScriptInterface::CreatureGeneratorAddTriggerer` | `declaration-fix` | `PASS` |  | 0 | 20 | 3832 | comparison between distinct pointer types '{anonymous}::TCInterfaceEntryOverlay*' and 'CVectorMap<int, CTCBase*, CKeyPairCompareLess<int, CTCBase*> >::Entry*' lacks a cast |
| 74 | `0x00899220` | `CGameScriptInterface::CGameScriptInterface::IncreaseHeroRenownLevel` | `declaration-fix` | `PASS` |  | 0 | 26 | 3179 | 'reinterpret_cast' from integer to pointer |
| 75 | `0x0089C330` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionDeedTypeEnabled` | `declaration-fix` | `PASS` |  | 0 | 27 | 3439 | incomplete type '{anonymous}::OpinionDeedInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 76 | `0x0089C3D0` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionReactionEnabled` | `declaration-fix` | `PASS` |  | 0 | 29 | 3569 | incomplete type '{anonymous}::OpinionReactionInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 77 | `0x00896EE0` | `CGameScriptInterface::CGameScriptInterface::GetDeathRecoveryMarkerName` | `declaration-fix` | `PASS` |  | 0 | 30 | 3417 | static assertion failed |
| 78 | `0x00891170` | `CGameScriptInterface::CGameScriptInterface::PlaySoundAtPos` | `declaration-fix` | `PASS` |  | 0 | 33 | 3435 | 'reinterpret_cast' from integer to pointer |
| 79 | `0x008981C0` | `CGameScriptInterface::CGameScriptInterface::AddTattooToHero` | `declaration-fix` | `PASS` |  | 0 | 48 | 9458 | static assertion failed |
| 80 | `0x0089FEB0` | `CGameScriptInterface::CGameScriptInterface::AddCrimeCommitted` | `declaration-fix` | `PASS` |  | 0 | 69 | 12596 | 'reinterpret_cast' from integer to pointer |
| 81 | `0x00449B60` | `CPlayerManager::GetMultiplayerColour` | `dependency-stub` | `PASS` |  | 1 | 0 | 455 | 'CRGBColour' does not name a type |
| 82 | `0x008A9E30` | `CGameScriptInterface::CGameScriptInterface::AddGossipVillage` | `dependency-stub` | `PASS` |  | 1 | 0 | 489 | invalid use of incomplete type 'class CGameScriptInterface' |
| 83 | `0x008A9DD0` | `CGameScriptInterface::CGameScriptInterface::AddNewRumourToCategory` | `dependency-stub` | `PASS` |  | 1 | 0 | 667 | invalid use of incomplete type 'class CGameScriptInterface' |
| 84 | `0x00892F10` | `CGameScriptInterface::CGameScriptInterface::DeactivateQuestLater` | `dependency-stub` | `PASS` |  | 1 | 0 | 394 | expected '(' before '{' token |
| 85 | `0x00891540` | `CGameScriptInterface::CGameScriptInterface::AddFeatCard` | `dependency-stub` | `PASS` |  | 1 | 1 | 446 | invalid use of incomplete type 'class CGameScriptInterface' |
| 86 | `0x008913F0` | `CGameScriptInterface::CGameScriptInterface::AddQuestCard` | `dependency-stub` | `PASS` |  | 1 | 2 | 1653 | invalid use of incomplete type 'class CGameScriptInterface' |
| 87 | `0x0088FE30` | `CGameScriptInterface::CGameScriptInterface::GetTimeOfDay` | `dependency-stub` | `PASS` |  | 1 | 4 | 900 | expected '(' before '{' token |
| 88 | `0x00449D20` | `CPlayerManager::IsMultiplayerGameActive` | `dependency-stub` | `PASS` |  | 1 | 4 | 1755 | 'reinterpret_cast' from integer to pointer |
| 89 | `0x0088FE40` | `CGameScriptInterface::CGameScriptInterface::IsTimeOfDayBetween` | `dependency-stub` | `PASS` |  | 1 | 6 | 1112 | invalid use of incomplete type 'class CGameScriptInterface' |
| 90 | `0x0088E3D0` | `CGameScriptInterface::CGameScriptInterface::MiniMapAllowRouteBetweenRegions` | `dependency-stub` | `PASS` |  | 1 | 7 | 1436 | invalid use of incomplete type 'class CGameScriptInterface' |
| 91 | `0x004AE9D0` | `CNetworkClient::Update` | `dependency-stub` | `PASS` |  | 1 | 7 | 869 | 'CNetworkClient' has not been declared |
| 92 | `0x00893E70` | `CGameScriptInterface::CGameScriptInterface::MsgOnGameSavedManually` | `dependency-stub` | `PASS` |  | 1 | 9 | 2384 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 93 | `0x004AEBA0` | `CNetworkClient::GetGameEventPackageSet` | `dependency-stub` | `PASS` |  | 3 | 0 | 210 | 'CNetworkClient' has not been declared |
| 94 | `0x004AE940` | `CNetworkClient::InitialiseAsLocal` | `dependency-stub` | `PASS` |  | 3 | 4 | 599 | 'CNetworkClient' has not been declared |
| 95 | `0x00899290` | `CGameScriptInterface::CGameScriptInterface::GetHeroStatLevel` | `manual-lift` | `PASS` |  | 0 | 28 | 3908 | 'reinterpret_cast' from integer to pointer |
| 96 | `0x008916A0` | `CGameScriptInterface::CGameScriptInterface::SheatheHeroWeapons` | `manual-lift` | `PASS` |  | 0 | 28 | 3692 | 'reinterpret_cast' from integer to pointer |
| 97 | `0x00891A00` | `CGameScriptInterface::CGameScriptInterface::SetQuestInfoText` | `manual-lift` | `PASS` |  | 0 | 39 | 5436 | static assertion failed |
| 98 | `0x0088EAD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsDamageable` | `manual-lift` | `PASS` |  | 1 | 18 | 2462 | invalid use of incomplete type 'class CGameScriptInterface' |
| 99 | `0x008943C0` | `CGameScriptInterface::CGameScriptInterface::MsgIsTutorialClickedPast` | `manual-lift` | `PASS` |  | 1 | 21 | 3652 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 100 | `0x0089C380` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionAttitudeEnabled` | `manual-lift` | `PASS` |  | 1 | 24 | 3324 | macro 'offsetof' passed 3 arguments, but takes just 2 |

## Semantic-review quarantine

These candidates may structurally match the retail path, but their generated C++ contains a known source-level hazard and is intentionally ranked behind ordinary manual lifts.

| Address | Owner/function | Hazards |
|---|---|---|
| `0x00899EF0` | `CGameScriptInterface::CGameScriptInterface::GiveHeroMorality` | possible-end-sentinel-dereference |
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
| `0x0089EF30` | `CGameScriptInterface::CGameScriptInterface::SetMoralityChangingAsEnabled` | possible-end-sentinel-dereference |
| `0x00899A10` | `CGameScriptInterface::CGameScriptInterface::SetHeroWillEnergyLevel` | possible-end-sentinel-dereference |
|  | _85 additional quarantined candidates omitted_ |  |
