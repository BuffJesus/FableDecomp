# Curated C++ promotion queue

Generated: `2026-07-30T13:33:07-06:00`

Uncompiled auto-RE candidates: **461**. Showing: **100**. Semantic-review quarantine: **90**.

Ranking favors checker/integrity/signature PASS and candidates without known source-level hazards, then the smallest declaration, dependency, VC7.1, and source-size repair surface. Structural fidelity does not by itself make an unsafe C++ expression promotable; every promotion still needs semantic review, a focused behavior oracle, and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Hazards | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---|---:|---:|---:|---|
| 1 | `0x008A9E60` | `CGameScriptInterface::CGameScriptInterface::AddGossipFactionToCategory` | `vc71-port` | `PASS` |  | 0 | 2 | 765 |  |
| 2 | `0x0088F400` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled` | `vc71-port` | `PASS` |  | 0 | 2 | 849 |  |
| 3 | `0x0088F910` | `CGameScriptInterface::CGameScriptInterface::AutoSaveCheckPoint` | `vc71-port` | `PASS` |  | 0 | 3 | 558 |  |
| 4 | `0x0088F920` | `CGameScriptInterface::CGameScriptInterface::AutoSaveQuestStart` | `vc71-port` | `PASS` |  | 0 | 3 | 559 |  |
| 5 | `0x0088F970` | `CGameScriptInterface::CGameScriptInterface::ResetToFrontEnd` | `vc71-port` | `PASS` |  | 0 | 3 | 649 |  |
| 6 | `0x008CFE30` | `CGameScriptThing::CGameScriptThing::GetHomePos` | `vc71-port` | `PASS` |  | 0 | 4 | 619 |  |
| 7 | `0x0088F610` | `CGameScriptInterface::CGameScriptInterface::IsSoundPlaying` | `vc71-port` | `PASS` |  | 0 | 4 | 787 |  |
| 8 | `0x0088E130` | `CGameScriptInterface::CGameScriptInterface::GiveHeroWeapon` | `vc71-port` | `PASS` |  | 0 | 4 | 858 |  |
| 9 | `0x0088DE10` | `CGameScriptInterface::CGameScriptInterface::GiveThingItemInHand` | `vc71-port` | `PASS` |  | 0 | 4 | 1289 |  |
| 10 | `0x0088F710` | `CGameScriptInterface::CGameScriptInterface::EntitySetCombatEnabled` | `vc71-port` | `PASS` |  | 0 | 5 | 1715 |  |
| 11 | `0x00401C00` | `GFHandleSystemInitError` | `vc71-port` | `PASS` |  | 0 | 5 | 9567 |  |
| 12 | `0x00891330` | `CGameScriptInterface::CGameScriptInterface::IsDangerMusicEnabled` | `vc71-port` | `PASS` |  | 0 | 6 | 1114 |  |
| 13 | `0x0088F8D0` | `CGameScriptInterface::CGameScriptInterface::StartCountdownTimer` | `vc71-port` | `PASS` |  | 0 | 6 | 1136 |  |
| 14 | `0x0088FDC0` | `CGameScriptInterface::CGameScriptInterface::SetTimeOfDay` | `vc71-port` | `PASS` |  | 0 | 6 | 1376 |  |
| 15 | `0x0088FE90` | `CGameScriptInterface::CGameScriptInterface::FastForwardTimeTo` | `vc71-port` | `PASS` |  | 0 | 6 | 1528 |  |
| 16 | `0x0088E300` | `CGameScriptInterface::CGameScriptInterface::IsRegionLoaded` | `vc71-port` | `PASS` |  | 0 | 6 | 1560 |  |
| 17 | `0x0088F9B0` | `CGameScriptInterface::CGameScriptInterface::GetGuildSealRecallPos` | `vc71-port` | `PASS` |  | 0 | 7 | 888 |  |
| 18 | `0x0088E380` | `CGameScriptInterface::CGameScriptInterface::DontPopulateNextLoadedRegion` | `vc71-port` | `PASS` |  | 0 | 7 | 905 |  |
| 19 | `0x0088DF60` | `CGameScriptInterface::CGameScriptInterface::GetWaterHeightAtPosition` | `vc71-port` | `PASS` |  | 0 | 7 | 1152 |  |
| 20 | `0x004AAAF0` | `CScriptThing::CScriptThing::MsgIsKilledBy` | `vc71-port` | `PASS` |  | 0 | 7 | 1192 |  |
| 21 | `0x008907D0` | `CGameScriptInterface::CGameScriptInterface::IsConversationActive` | `vc71-port` | `PASS` |  | 0 | 7 | 1197 |  |
| 22 | `0x0088F480` | `CGameScriptInterface::CGameScriptInterface::ApplyScriptBrush` | `vc71-port` | `PASS` |  | 0 | 7 | 1510 |  |
| 23 | `0x0088E6F0` | `CGameScriptInterface::CGameScriptInterface::AreEntitiesEnemies` | `vc71-port` | `PASS` |  | 0 | 7 | 1610 |  |
| 24 | `0x0088FF30` | `CGameScriptInterface::CGameScriptInterface::TransitionToThemeAllInternals` | `vc71-port` | `PASS` |  | 0 | 7 | 1906 |  |
| 25 | `0x004AAC10` | `CScriptThing::CScriptThing::MsgHowLongWasExpressionPerformed` | `vc71-port` | `PASS` |  | 0 | 8 | 1122 |  |
| 26 | `0x00891310` | `CGameScriptInterface::CGameScriptInterface::CacheMusicSet` | `vc71-port` | `PASS` |  | 0 | 8 | 1168 |  |
| 27 | `0x0088E200` | `CGameScriptInterface::CGameScriptInterface::SetGuildMasterMessages` | `vc71-port` | `PASS` |  | 0 | 9 | 933 |  |
| 28 | `0x006E7690` | `CGameScriptInterface::CGameScriptInterface::DeregisterTimer` | `vc71-port` | `PASS` |  | 0 | 9 | 1559 |  |
| 29 | `0x0088E0B0` | `CGameScriptInterface::CGameScriptInterface::SetSoundThemesAsEnabledForRegion` | `vc71-port` | `PASS` |  | 0 | 9 | 2272 |  |
| 30 | `0x008907E0` | `CGameScriptInterface::CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn` | `vc71-port` | `PASS` |  | 0 | 10 | 1199 |  |
| 31 | `0x00890200` | `CGameScriptInterface::CGameScriptInterface::ScreenFilterFadeOut` | `vc71-port` | `PASS` |  | 0 | 11 | 2093 |  |
| 32 | `0x0088FC90` | `CGameScriptInterface::CGameScriptInterface::HeroHasExpression` | `vc71-port` | `PASS` |  | 0 | 12 | 1943 |  |
| 33 | `0x0088FD00` | `CGameScriptInterface::CGameScriptInterface::EntitySetInLimbo` | `vc71-port` | `PASS` |  | 0 | 13 | 1625 |  |
| 34 | `0x00890B90` | `CGameScriptInterface::CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `vc71-port` | `PASS` |  | 0 | 13 | 2519 |  |
| 35 | `0x0088ECE0` | `CGameScriptInterface::CGameScriptInterface::CameraShake` | `vc71-port` | `PASS` |  | 0 | 14 | 2350 |  |
| 36 | `0x008A9320` | `CGameScriptInterface::CGameScriptInterface::EntityCastForcePush` | `vc71-port` | `PASS` |  | 0 | 14 | 2708 |  |
| 37 | `0x006E79E0` | `CGameScriptInterface::CGameScriptInterface::RegisterTimer` | `vc71-port` | `PASS` |  | 0 | 14 | 2736 |  |
| 38 | `0x00893060` | `CGameScriptInterface::CGameScriptInterface::AddBoast` | `vc71-port` | `PASS` |  | 0 | 16 | 2844 |  |
| 39 | `0x00893240` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelUnloaded` | `vc71-port` | `PASS` |  | 0 | 16 | 3236 |  |
| 40 | `0x00891070` | `CGameScriptInterface::CGameScriptInterface::CameraUseCameraPoint` | `vc71-port` | `PASS` |  | 0 | 18 | 3093 |  |
| 41 | `0x008A9610` | `CGameScriptInterface::CGameScriptInterface::SetThingAsConscious` | `vc71-port` | `PASS` |  | 0 | 20 | 3648 |  |
| 42 | `0x008931B0` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelLoaded` | `vc71-port` | `PASS` |  | 0 | 20 | 3672 |  |
| 43 | `0x0089AF30` | `CGameScriptInterface::CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `vc71-port` | `PASS` |  | 0 | 20 | 4247 |  |
| 44 | `0x00894DD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsMarryable` | `vc71-port` | `PASS` |  | 0 | 22 | 2481 |  |
| 45 | `0x0089B3F0` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsWearing` | `vc71-port` | `PASS` |  | 0 | 24 | 2715 |  |
| 46 | `0x0089E130` | `CGameScriptInterface::CGameScriptInterface::EntitySetMaxNumberOfAttackers` | `vc71-port` | `PASS` |  | 0 | 24 | 2810 |  |
| 47 | `0x008984E0` | `CGameScriptInterface::CGameScriptInterface::SetWeaponOutCrimeEnabled` | `vc71-port` | `PASS` |  | 0 | 25 | 3309 |  |
| 48 | `0x0088F570` | `CGameScriptInterface::CGameScriptInterface::Play2DSound` | `vc71-port` | `PASS` |  | 0 | 29 | 3029 |  |
| 49 | `0x00891720` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHealthBy` | `vc71-port` | `PASS` |  | 0 | 31 | 5204 |  |
| 50 | `0x006E7920` | `CGameScriptInterface::CGameScriptInterface::CheckForCameraMessage` | `vc71-port` | `PASS` |  | 0 | 31 | 5901 |  |
| 51 | `0x00892D70` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript` | `declaration-fix` | `PASS` |  | 0 | 1 | 679 | 'reinterpret_cast' from integer to pointer |
| 52 | `0x008AA010` | `CGameScriptInterface::CGameScriptInterface::ClearGossip` | `declaration-fix` | `PASS` |  | 0 | 5 | 1275 | static assertion failed |
| 53 | `0x008907C0` | `CGameScriptInterface::CGameScriptInterface::RemoveConversation` | `declaration-fix` | `PASS` |  | 0 | 7 | 1270 | 'reinterpret_cast' from integer to pointer |
| 54 | `0x008915B0` | `CGameScriptInterface::CGameScriptInterface::SetBoastAsFailed` | `declaration-fix` | `PASS` |  | 0 | 7 | 1128 | 'reinterpret_cast' from integer to pointer |
| 55 | `0x0088E2A0` | `CGameScriptInterface::CGameScriptInterface::IsLevelLoaded` | `declaration-fix` | `PASS` |  | 0 | 11 | 2394 | static assertion failed |
| 56 | `0x00892CF0` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabled` | `declaration-fix` | `PASS` |  | 0 | 14 | 3296 | jump to label 'SetWorldFlag' |
| 57 | `0x00894C90` | `CGameScriptInterface::CGameScriptInterface::IsGiftFriendly` | `declaration-fix` | `PASS` |  | 0 | 16 | 5128 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 58 | `0x00894BF0` | `CGameScriptInterface::CGameScriptInterface::IsGiftRomantic` | `declaration-fix` | `PASS` |  | 0 | 16 | 5210 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 59 | `0x00894D30` | `CGameScriptInterface::CGameScriptInterface::IsGiftOffensive` | `declaration-fix` | `PASS` |  | 0 | 16 | 5212 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 60 | `0x0089EC60` | `CGameScriptInterface::CGameScriptInterface::CreatureGeneratorAddTriggerer` | `declaration-fix` | `PASS` |  | 0 | 20 | 3701 | comparison between distinct pointer types '{anonymous}::TCInterfaceEntryOverlay*' and 'CVectorMap<int, CTCBase*, CKeyPairCompareLess<int, CTCBase*> >::Entry*' lacks a cast |
| 61 | `0x0089C330` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionDeedTypeEnabled` | `declaration-fix` | `PASS` |  | 0 | 27 | 3325 | incomplete type '{anonymous}::OpinionDeedInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 62 | `0x0089C3D0` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionReactionEnabled` | `declaration-fix` | `PASS` |  | 0 | 29 | 3454 | incomplete type '{anonymous}::OpinionReactionInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 63 | `0x00891170` | `CGameScriptInterface::CGameScriptInterface::PlaySoundAtPos` | `declaration-fix` | `PASS` |  | 0 | 33 | 3318 | 'reinterpret_cast' from integer to pointer |
| 64 | `0x008981C0` | `CGameScriptInterface::CGameScriptInterface::AddTattooToHero` | `declaration-fix` | `PASS` |  | 0 | 48 | 9170 | static assertion failed |
| 65 | `0x0089FEB0` | `CGameScriptInterface::CGameScriptInterface::AddCrimeCommitted` | `declaration-fix` | `PASS` |  | 0 | 69 | 12255 | 'reinterpret_cast' from integer to pointer |
| 66 | `0x00449B60` | `CPlayerManager::GetMultiplayerColour` | `dependency-stub` | `PASS` |  | 1 | 0 | 436 | 'CRGBColour' does not name a type |
| 67 | `0x008A9E30` | `CGameScriptInterface::CGameScriptInterface::AddGossipVillage` | `dependency-stub` | `PASS` |  | 1 | 0 | 475 | invalid use of incomplete type 'class CGameScriptInterface' |
| 68 | `0x008A9DD0` | `CGameScriptInterface::CGameScriptInterface::AddNewRumourToCategory` | `dependency-stub` | `PASS` |  | 1 | 0 | 640 | invalid use of incomplete type 'class CGameScriptInterface' |
| 69 | `0x0088FE30` | `CGameScriptInterface::CGameScriptInterface::GetTimeOfDay` | `dependency-stub` | `PASS` |  | 1 | 4 | 860 | expected '(' before '{' token |
| 70 | `0x00449D20` | `CPlayerManager::IsMultiplayerGameActive` | `dependency-stub` | `PASS` |  | 1 | 4 | 1695 | 'reinterpret_cast' from integer to pointer |
| 71 | `0x0088FE80` | `CGameScriptInterface::CGameScriptInterface::SetTimeAsStopped` | `dependency-stub` | `PASS` |  | 1 | 5 | 741 | 'CGameScriptInterface' has not been declared |
| 72 | `0x0088FE40` | `CGameScriptInterface::CGameScriptInterface::IsTimeOfDayBetween` | `dependency-stub` | `PASS` |  | 1 | 6 | 1079 | invalid use of incomplete type 'class CGameScriptInterface' |
| 73 | `0x0088E3D0` | `CGameScriptInterface::CGameScriptInterface::MiniMapAllowRouteBetweenRegions` | `dependency-stub` | `PASS` |  | 1 | 7 | 1391 | invalid use of incomplete type 'class CGameScriptInterface' |
| 74 | `0x004AE9D0` | `CNetworkClient::Update` | `dependency-stub` | `PASS` |  | 1 | 7 | 844 | 'CNetworkClient' has not been declared |
| 75 | `0x00893E70` | `CGameScriptInterface::CGameScriptInterface::MsgOnGameSavedManually` | `dependency-stub` | `PASS` |  | 1 | 9 | 2300 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 76 | `0x00893EC0` | `CGameScriptInterface::CGameScriptInterface::MsgOnHeroSlept` | `dependency-stub` | `PASS` |  | 1 | 9 | 2000 | 'CGameScriptInterface' has not been declared |
| 77 | `0x004AAC40` | `CScriptThing::CScriptThing::MsgReceivedMoney` | `dependency-stub` | `PASS` |  | 2 | 3 | 508 | 'CScriptThing' has not been declared |
| 78 | `0x0088FC60` | `CGameScriptInterface::CGameScriptInterface::GiveHeroExpression` | `dependency-stub` | `PASS` |  | 2 | 10 | 1389 | 'reinterpret_cast' from integer to pointer |
| 79 | `0x0088F960` | `CGameScriptInterface::CGameScriptInterface::SetSaveGameMarkerPos` | `dependency-stub` | `PASS` |  | 3 | 3 | 390 | 'CGameScriptInterface' has not been declared |
| 80 | `0x004AE940` | `CNetworkClient::InitialiseAsLocal` | `dependency-stub` | `PASS` |  | 3 | 4 | 580 | 'CNetworkClient' has not been declared |
| 81 | `0x004AAC60` | `CScriptThing::CScriptThing::MsgIsPresentedWithItem` | `dependency-stub` | `PASS` |  | 3 | 6 | 648 | 'CScriptThing' has not been declared |
| 82 | `0x004AABF0` | `CScriptThing::CScriptThing::MsgExpressionPerformedTo` | `dependency-stub` | `PASS` |  | 3 | 6 | 700 | 'CScriptThing' has not been declared |
| 83 | `0x008901A0` | `CGameScriptInterface::CGameScriptInterface::RadialBlurSetCenterWorldPos` | `dependency-stub` | `PASS` |  | 3 | 9 | 1155 | 'CEnvironmentSpellEffectInterpolator' does not name a type |
| 84 | `0x00891A00` | `CGameScriptInterface::CGameScriptInterface::SetQuestInfoText` | `manual-lift` | `PASS` |  | 0 | 39 | 5228 | static assertion failed |
| 85 | `0x0088EAD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsDamageable` | `manual-lift` | `PASS` |  | 1 | 18 | 2384 | invalid use of incomplete type 'class CGameScriptInterface' |
| 86 | `0x008943C0` | `CGameScriptInterface::CGameScriptInterface::MsgIsTutorialClickedPast` | `manual-lift` | `PASS` |  | 1 | 21 | 3529 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 87 | `0x0089C380` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionAttitudeEnabled` | `manual-lift` | `PASS` |  | 1 | 24 | 3222 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 88 | `0x008A1650` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreBlackjack` | `manual-lift` | `PASS` |  | 1 | 26 | 3934 | 'reinterpret_cast' from integer to pointer |
| 89 | `0x008A1770` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreShoveHaPenny` | `manual-lift` | `PASS` |  | 1 | 26 | 4325 | 'reinterpret_cast' from integer to pointer |
| 90 | `0x008A1710` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreCoinGolfSnowSpire` | `manual-lift` | `PASS` |  | 1 | 26 | 4445 | 'reinterpret_cast' from integer to pointer |
| 91 | `0x00890FF0` | `CGameScriptInterface::CGameScriptInterface::CameraDefault` | `manual-lift` | `PASS` |  | 1 | 27 | 3783 | 'field48' was not declared in this scope |
| 92 | `0x0089C7B0` | `CGameScriptInterface::CGameScriptInterface::EntityPostOpinionDeedKeepSearchingForWitnesses` | `manual-lift` | `PASS` |  | 1 | 27 | 4897 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 93 | `0x008A1C50` | `CGameScriptInterface::CGameScriptInterface::RepopulateVillage` | `manual-lift` | `PASS` |  | 1 | 28 | 4207 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 94 | `0x008A9AE0` | `CGameScriptInterface::CGameScriptInterface::MsgOnBoastsMade` | `manual-lift` | `PASS` |  | 1 | 35 | 8369 | field 'boastText' has incomplete type 'CCharString' |
| 95 | `0x0089AD90` | `CGameScriptInterface::CGameScriptInterface::IsQuestStartScreenActive` | `manual-lift` | `PASS` |  | 1 | 40 | 3359 | invalid use of incomplete type 'class CGameScriptInterface' |
| 96 | `0x009F19A0` | `CGameEventPackageSet::CompressIntoBuffer` | `manual-lift` | `PASS` |  | 1 | 45 | 3986 | 'CGameEventPackageSet' has not been declared; did you mean 'CGameEventPackageOverlay'? |
| 97 | `0x00A0D340` | `CProcessedInput::AddGameEvent` | `manual-lift` | `PASS` |  | 2 | 15 | 1266 | 'CProcessedInput' has not been declared |
| 98 | `0x009F16F0` | `CGameEventPackageSet::AddPackage` | `manual-lift` | `PASS` |  | 2 | 17 | 1282 | 'CGameEventPackageSet' has not been declared; did you mean 'CGameEventPackageOverlay'? |
| 99 | `0x004165E8` | `CMainGameComponent::CheckSync` | `manual-lift` | `PASS` |  | 2 | 20 | 2016 | 'reinterpret_cast' from integer to pointer |
| 100 | `0x00891CA0` | `CGameScriptInterface::CGameScriptInterface::GetHero` | `manual-lift` | `PASS` |  | 2 | 37 | 5438 | 'CScriptThing* NScript::operator&&(GetHeroPredicate5*, CIsThingAlive*)' must have an argument of class or enumerated type |

## Semantic-review quarantine

These candidates may structurally match the retail path, but their generated C++ contains a known source-level hazard and is intentionally ranked behind ordinary manual lifts.

| Address | Owner/function | Hazards |
|---|---|---|
| `0x00897150` | `CGameScriptInterface::CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | possible-end-sentinel-dereference |
| `0x00896060` | `CGameScriptInterface::CGameScriptInterface::SetVillageLimbo` | possible-end-sentinel-dereference |
| `0x00897B10` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasCurrentMarriage` | possible-end-sentinel-dereference |
| `0x00897BD0` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasChildren` | possible-end-sentinel-dereference |
| `0x0089B4D0` | `CGameScriptInterface::CGameScriptInterface::RemoveHeroHairstyle` | possible-end-sentinel-dereference |
| `0x008A1590` | `CGameScriptInterface::CGameScriptInterface::GetBestTimePairs` | possible-end-sentinel-dereference |
| `0x008A15F0` | `CGameScriptInterface::CGameScriptInterface::GetBestTimeSorting` | possible-end-sentinel-dereference |
| `0x008A17D0` | `CGameScriptInterface::CGameScriptInterface::GetBestTimeGuessTheAddition` | possible-end-sentinel-dereference |
| `0x00897C30` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasMurderedWife` | possible-end-sentinel-dereference |
| `0x00897B70` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasDivorcedMarriage` | possible-end-sentinel-dereference |
| `0x00897C90` | `CGameScriptInterface::CGameScriptInterface::GetNumberOfTimesHeroHasHadSex` | possible-end-sentinel-dereference |
| `0x008A16B0` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreCoinGolfOakVale` | possible-end-sentinel-dereference |
| `0x00897030` | `CGameScriptInterface::CGameScriptInterface::IsToFailQuestOnDeath` | possible-end-sentinel-dereference |
| `0x00897480` | `CGameScriptInterface::CGameScriptInterface::IsPlayerWieldingWeapon` | possible-end-sentinel-dereference |
| `0x00897AA0` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasMarried` | possible-end-sentinel-dereference |
| `0x0089EF30` | `CGameScriptInterface::CGameScriptInterface::SetMoralityChangingAsEnabled` | possible-end-sentinel-dereference |
| `0x00897CF0` | `CGameScriptInterface::CGameScriptInterface::SetNumberOfTimesHeroHasHadSex` | possible-end-sentinel-dereference |
| `0x0089B450` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHairstyle` | possible-end-sentinel-dereference |
| `0x00898540` | `CGameScriptInterface::CGameScriptInterface::SetGuardsIgnoreCrimes` | possible-end-sentinel-dereference |
| `0x00897D50` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsHavingHadSex` | possible-end-sentinel-dereference |
| `0x00897DB0` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsHavingHadGaySex` | possible-end-sentinel-dereference |
| `0x008A12A0` | `CGameScriptInterface::CGameScriptInterface::GetNumHousesOwned` | possible-end-sentinel-dereference |
| `0x00896E60` | `CGameScriptInterface::CGameScriptInterface::SetPreferredQuickAccessItem` | possible-end-sentinel-dereference |
| `0x00897090` | `CGameScriptInterface::CGameScriptInterface::SetWhetherToFailQuestOnDeath` | possible-end-sentinel-dereference |
| `0x00898DB0` | `CGameScriptInterface::CGameScriptInterface::TakeObjectFromHero` | possible-end-sentinel-dereference;process-termination |
|  | _65 additional quarantined candidates omitted_ |  |
