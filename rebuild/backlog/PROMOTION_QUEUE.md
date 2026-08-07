# Curated C++ promotion queue

Generated: `2026-08-05T11:04:37-06:00`

Uncompiled auto-RE candidates: **466**. Showing: **100**. Semantic-review quarantine: **89**.

Ranking favors checker/integrity/signature PASS and candidates without known source-level hazards, then the smallest declaration, dependency, VC7.1, and source-size repair surface. Structural fidelity does not by itself make an unsafe C++ expression promotable; every promotion still needs semantic review, a focused behavior oracle, and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Hazards | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---|---:|---:|---:|---|
| 1 | `0x0088F610` | `CGameScriptInterface::CGameScriptInterface::IsSoundPlaying` | `vc71-port` | `PASS` |  | 0 | 4 | 787 |  |
| 2 | `0x0088F710` | `CGameScriptInterface::CGameScriptInterface::EntitySetCombatEnabled` | `vc71-port` | `PASS` |  | 0 | 5 | 1715 |  |
| 3 | `0x00401C00` | `GFHandleSystemInitError` | `vc71-port` | `PASS` |  | 0 | 5 | 9567 |  |
| 4 | `0x0088FDC0` | `CGameScriptInterface::CGameScriptInterface::SetTimeOfDay` | `vc71-port` | `PASS` |  | 0 | 6 | 1376 |  |
| 5 | `0x0088FE90` | `CGameScriptInterface::CGameScriptInterface::FastForwardTimeTo` | `vc71-port` | `PASS` |  | 0 | 6 | 1528 |  |
| 6 | `0x004AAAF0` | `CScriptThing::CScriptThing::MsgIsKilledBy` | `vc71-port` | `PASS` |  | 0 | 7 | 1192 |  |
| 7 | `0x0088F480` | `CGameScriptInterface::CGameScriptInterface::ApplyScriptBrush` | `vc71-port` | `PASS` |  | 0 | 7 | 1510 |  |
| 8 | `0x004AAC10` | `CScriptThing::CScriptThing::MsgHowLongWasExpressionPerformed` | `vc71-port` | `PASS` |  | 0 | 8 | 1122 |  |
| 9 | `0x006E7690` | `CGameScriptInterface::CGameScriptInterface::DeregisterTimer` | `vc71-port` | `PASS` |  | 0 | 9 | 1559 |  |
| 10 | `0x0088FC90` | `CGameScriptInterface::CGameScriptInterface::HeroHasExpression` | `vc71-port` | `PASS` |  | 0 | 12 | 1943 |  |
| 11 | `0x0088FD00` | `CGameScriptInterface::CGameScriptInterface::EntitySetInLimbo` | `vc71-port` | `PASS` |  | 0 | 13 | 1625 |  |
| 12 | `0x00890B90` | `CGameScriptInterface::CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `vc71-port` | `PASS` |  | 0 | 13 | 2519 |  |
| 13 | `0x00893240` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelUnloaded` | `vc71-port` | `PASS` |  | 0 | 16 | 3236 |  |
| 14 | `0x00891070` | `CGameScriptInterface::CGameScriptInterface::CameraUseCameraPoint` | `vc71-port` | `PASS` |  | 0 | 18 | 3093 |  |
| 15 | `0x008A9610` | `CGameScriptInterface::CGameScriptInterface::SetThingAsConscious` | `vc71-port` | `PASS` |  | 0 | 20 | 3648 |  |
| 16 | `0x008931B0` | `CGameScriptInterface::CGameScriptInterface::MsgIsLevelLoaded` | `vc71-port` | `PASS` |  | 0 | 20 | 3672 |  |
| 17 | `0x0089AF30` | `CGameScriptInterface::CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `vc71-port` | `PASS` |  | 0 | 20 | 4247 |  |
| 18 | `0x00894DD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsMarryable` | `vc71-port` | `PASS` |  | 0 | 22 | 2481 |  |
| 19 | `0x0089B3F0` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsWearing` | `vc71-port` | `PASS` |  | 0 | 24 | 2715 |  |
| 20 | `0x0089E130` | `CGameScriptInterface::CGameScriptInterface::EntitySetMaxNumberOfAttackers` | `vc71-port` | `PASS` |  | 0 | 24 | 2810 |  |
| 21 | `0x008984E0` | `CGameScriptInterface::CGameScriptInterface::SetWeaponOutCrimeEnabled` | `vc71-port` | `PASS` |  | 0 | 25 | 3309 |  |
| 22 | `0x0088F570` | `CGameScriptInterface::CGameScriptInterface::Play2DSound` | `vc71-port` | `PASS` |  | 0 | 29 | 3029 |  |
| 23 | `0x00891720` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHealthBy` | `vc71-port` | `PASS` |  | 0 | 31 | 5204 |  |
| 24 | `0x006E7920` | `CGameScriptInterface::CGameScriptInterface::CheckForCameraMessage` | `vc71-port` | `PASS` |  | 0 | 31 | 5901 |  |
| 25 | `0x008AA010` | `CGameScriptInterface::CGameScriptInterface::ClearGossip` | `declaration-fix` | `PASS` |  | 0 | 5 | 1275 | static assertion failed |
| 26 | `0x0088E2A0` | `CGameScriptInterface::CGameScriptInterface::IsLevelLoaded` | `declaration-fix` | `PASS` |  | 0 | 11 | 2394 | static assertion failed |
| 27 | `0x00892CF0` | `CGameScriptInterface::CGameScriptInterface::SetCreatureGeneratorsEnabled` | `declaration-fix` | `PASS` |  | 0 | 14 | 3296 | jump to label 'SetWorldFlag' |
| 28 | `0x00894C90` | `CGameScriptInterface::CGameScriptInterface::IsGiftFriendly` | `declaration-fix` | `PASS` |  | 0 | 16 | 5128 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 29 | `0x00894BF0` | `CGameScriptInterface::CGameScriptInterface::IsGiftRomantic` | `declaration-fix` | `PASS` |  | 0 | 16 | 5210 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 30 | `0x00894D30` | `CGameScriptInterface::CGameScriptInterface::IsGiftOffensive` | `declaration-fix` | `PASS` |  | 0 | 16 | 5212 | invalid conversion from 'const void*' to 'void*' [-fpermissive] |
| 31 | `0x0089EC60` | `CGameScriptInterface::CGameScriptInterface::CreatureGeneratorAddTriggerer` | `declaration-fix` | `PASS` |  | 0 | 20 | 3701 | comparison between distinct pointer types '{anonymous}::TCInterfaceEntryOverlay*' and 'CVectorMap<int, CTCBase*, CKeyPairCompareLess<int, CTCBase*> >::Entry*' lacks a cast |
| 32 | `0x0089C330` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionDeedTypeEnabled` | `declaration-fix` | `PASS` |  | 0 | 27 | 3325 | incomplete type '{anonymous}::OpinionDeedInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 33 | `0x0089C3D0` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionReactionEnabled` | `declaration-fix` | `PASS` |  | 0 | 29 | 3454 | incomplete type '{anonymous}::OpinionReactionInterfaceMap' {aka '{anonymous}::CVectorMap<{anonymous}::ETCInterfaceType, CTCBase*, {anonymous}::CKeyPairCompareLess<{anonymous}::ETCInterfaceType, CTCBase*> >'} used in nested name specifier |
| 34 | `0x00891170` | `CGameScriptInterface::CGameScriptInterface::PlaySoundAtPos` | `declaration-fix` | `PASS` |  | 0 | 33 | 3318 | 'reinterpret_cast' from integer to pointer |
| 35 | `0x008981C0` | `CGameScriptInterface::CGameScriptInterface::AddTattooToHero` | `declaration-fix` | `PASS` |  | 0 | 48 | 9170 | static assertion failed |
| 36 | `0x0089FEB0` | `CGameScriptInterface::CGameScriptInterface::AddCrimeCommitted` | `declaration-fix` | `PASS` |  | 0 | 69 | 12255 | 'reinterpret_cast' from integer to pointer |
| 37 | `0x00449B60` | `CPlayerManager::GetMultiplayerColour` | `dependency-stub` | `PASS` |  | 1 | 0 | 436 | 'CRGBColour' does not name a type |
| 38 | `0x00449D20` | `CPlayerManager::IsMultiplayerGameActive` | `dependency-stub` | `PASS` |  | 1 | 4 | 1695 | 'reinterpret_cast' from integer to pointer |
| 39 | `0x0088E3D0` | `CGameScriptInterface::CGameScriptInterface::MiniMapAllowRouteBetweenRegions` | `dependency-stub` | `PASS` |  | 1 | 7 | 1391 | invalid use of incomplete type 'class CGameScriptInterface' |
| 40 | `0x00893E70` | `CGameScriptInterface::CGameScriptInterface::MsgOnGameSavedManually` | `dependency-stub` | `PASS` |  | 1 | 9 | 2300 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 41 | `0x00893EC0` | `CGameScriptInterface::CGameScriptInterface::MsgOnHeroSlept` | `dependency-stub` | `PASS` |  | 1 | 9 | 2000 | 'CGameScriptInterface' has not been declared |
| 42 | `0x004AAC40` | `CScriptThing::CScriptThing::MsgReceivedMoney` | `dependency-stub` | `PASS` |  | 2 | 3 | 508 | 'CScriptThing' has not been declared |
| 43 | `0x004AE940` | `CNetworkClient::InitialiseAsLocal` | `dependency-stub` | `PASS` |  | 3 | 4 | 580 | 'CNetworkClient' has not been declared |
| 44 | `0x004AAC60` | `CScriptThing::CScriptThing::MsgIsPresentedWithItem` | `dependency-stub` | `PASS` |  | 3 | 6 | 648 | 'CScriptThing' has not been declared |
| 45 | `0x004AABF0` | `CScriptThing::CScriptThing::MsgExpressionPerformedTo` | `dependency-stub` | `PASS` |  | 3 | 6 | 700 | 'CScriptThing' has not been declared |
| 46 | `0x00891A00` | `CGameScriptInterface::CGameScriptInterface::SetQuestInfoText` | `manual-lift` | `PASS` |  | 0 | 39 | 5228 | static assertion failed |
| 47 | `0x0088EAD0` | `CGameScriptInterface::CGameScriptInterface::EntitySetAsDamageable` | `manual-lift` | `PASS` |  | 1 | 18 | 2384 | invalid use of incomplete type 'class CGameScriptInterface' |
| 48 | `0x008943C0` | `CGameScriptInterface::CGameScriptInterface::MsgIsTutorialClickedPast` | `manual-lift` | `PASS` |  | 1 | 21 | 3529 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 49 | `0x0089C380` | `CGameScriptInterface::CGameScriptInterface::EntitySetOpinionAttitudeEnabled` | `manual-lift` | `PASS` |  | 1 | 24 | 3222 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 50 | `0x008A1650` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreBlackjack` | `manual-lift` | `PASS` |  | 1 | 26 | 3934 | 'reinterpret_cast' from integer to pointer |
| 51 | `0x008A1770` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreShoveHaPenny` | `manual-lift` | `PASS` |  | 1 | 26 | 4325 | 'reinterpret_cast' from integer to pointer |
| 52 | `0x008A1710` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreCoinGolfSnowSpire` | `manual-lift` | `PASS` |  | 1 | 26 | 4445 | 'reinterpret_cast' from integer to pointer |
| 53 | `0x00890FF0` | `CGameScriptInterface::CGameScriptInterface::CameraDefault` | `manual-lift` | `PASS` |  | 1 | 27 | 3783 | 'field48' was not declared in this scope |
| 54 | `0x0089C7B0` | `CGameScriptInterface::CGameScriptInterface::EntityPostOpinionDeedKeepSearchingForWitnesses` | `manual-lift` | `PASS` |  | 1 | 27 | 4897 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 55 | `0x008A9AE0` | `CGameScriptInterface::CGameScriptInterface::MsgOnBoastsMade` | `manual-lift` | `PASS` |  | 1 | 35 | 8369 | field 'boastText' has incomplete type 'CCharString' |
| 56 | `0x009F19A0` | `CGameEventPackageSet::CompressIntoBuffer` | `manual-lift` | `PASS` |  | 1 | 45 | 3986 | 'CGameEventPackageSet' has not been declared; did you mean 'CGameEventPackageOverlay'? |
| 57 | `0x00A0D340` | `CProcessedInput::AddGameEvent` | `manual-lift` | `PASS` |  | 2 | 15 | 1266 | 'CProcessedInput' has not been declared |
| 58 | `0x004165E8` | `CMainGameComponent::CheckSync` | `manual-lift` | `PASS` |  | 2 | 20 | 2016 | 'reinterpret_cast' from integer to pointer |
| 59 | `0x00891CA0` | `CGameScriptInterface::CGameScriptInterface::GetHero` | `manual-lift` | `PASS` |  | 2 | 37 | 5438 | 'CScriptThing* NScript::operator&&(GetHeroPredicate5*, CIsThingAlive*)' must have an argument of class or enumerated type |
| 60 | `0x00896390` | `CGameScriptInterface::CGameScriptInterface::OpenChest` | `manual-lift` | `PASS` |  | 2 | 77 | 14865 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 61 | `0x004AEAA0` | `CNetworkClient::GetLocalGameEventPackageSet` | `manual-lift` | `PASS` |  | 3 | 16 | 2398 | 'CNetworkClient' has not been declared |
| 62 | `0x00896A30` | `CGameScriptInterface::CGameScriptInterface::SetQuestCardObjective` | `manual-lift` | `PASS` |  | 3 | 29 | 3566 | 'ETCInterfaceType' was not declared in this scope |
| 63 | `0x0041726D` | `CMainGameComponent::UpdateFromEventPackageSet` | `manual-lift` | `PASS` |  | 3 | 31 | 4223 | 'reinterpret_cast' from integer to pointer |
| 64 | `0x00401067` | `entry` | `manual-lift` | `PASS` |  | 3 | 50 | 6397 | static assertion failed |
| 65 | `0x00890820` | `CGameScriptInterface::CGameScriptInterface::FadeScreenOut` | `manual-lift` | `PASS` |  | 4 | 14 | 1422 | 'CRGBColour' was not declared in this scope |
| 66 | `0x009F1810` | `CGameEvent::CompressIntoBuffer` | `manual-lift` | `PASS` |  | 6 | 0 | 931 | 'CGameEvent' has not been declared |
| 67 | `0x00893B00` | `CGameScriptInterface::CGameScriptInterface::MsgOnExpressionPerformed` | `manual-lift` | `PASS` |  | 6 | 17 | 3018 | 'CDefinitionManager' does not name a type |
| 68 | `0x009F1870` | `CGameEvent::InitFromCompressedBuffer` | `manual-lift` | `PASS` |  | 7 | 7 | 1460 | 'CGameEvent' does not name a type |
| 69 | `0x00A76F30` | `CNavQuadTree::IsAreaBlockedByLines` | `manual-lift` | `PASS` |  | 9 | 24 | 6417 | 'list' in namespace 'std' does not name a template type |
| 70 | `0x0062C0E0` | `CWorld::EAMoveSpirit` | `manual-lift` | `PASS` |  | 10 | 33 | 3624 | 'CWorld' has not been declared |
| 71 | `0x00891940` | `CGameScriptInterface::CGameScriptInterface::SetQuestInfoName` | `manual-lift` | `PASS` |  | 11 | 25 | 2863 | 'CGameScriptInterface' has not been declared |
| 72 | `0x00A781A0` | `CNavQuadTree::UpdateLines` | `manual-lift` | `PASS` |  | 12 | 75 | 16017 | 'C2DVector' does not name a type |
| 73 | `0x00A7A8D0` | `CNavQuadTree::Initialise` | `manual-lift` | `PASS` |  | 13 | 142 | 26841 | 'C2DLineF' was not declared in this scope |
| 74 | `0x008A1B10` | `CGameScriptInterface::CGameScriptInterface::GetSleepingPositionAndOrientationFromBed` | `manual-lift` | `PASS` |  | 15 | 21 | 3084 | 'CGameScriptInterface' has not been declared |
| 75 | `0x008A89D0` | `CGameScriptInterface::CGameScriptInterface::GetAllCreaturesExcludingHero` | `manual-lift` | `PASS` |  | 17 | 22 | 4932 | 'CGameScriptInterface' has not been declared |
| 76 | `0x006700F0` | `CTCCoopSpirit::OnCreate` | `manual-lift` | `PASS` |  | 18 | 39 | 4196 | 'CDefClassBase' does not name a type |
| 77 | `0x00A79B30` | `CNavQuadTreeNode::Initialise` | `manual-lift` | `PASS` |  | 18 | 78 | 15716 | 'CANavQuadTreeNode' does not name a type |
| 78 | `0x0066FF20` | `CTCCoopSpirit::SwapToHero` | `manual-lift` | `PASS` |  | 20 | 40 | 5478 | 'CVertexBufferWin32' does not name a type |
| 79 | `0x008A9C40` | `CGameScriptInterface::CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom` | `manual-lift` | `PASS` |  | 23 | 33 | 6214 | 'CGameScriptInterface' has not been declared |
| 80 | `0x0050A650` | `CWorldMap::GetMapNavigationAreaInit` | `manual-lift` | `PASS` |  | 26 | 93 | 25507 | use of enum 'EHeroMorphType' without previous declaration |
| 81 | `0x00897150` | `CGameScriptInterface::CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 17 | 2298 |  |
| 82 | `0x00896060` | `CGameScriptInterface::CGameScriptInterface::SetVillageLimbo` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 17 | 2540 | incomplete type 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >' used in nested name specifier |
| 83 | `0x00897B10` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasCurrentMarriage` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 22 | 3511 | 'reinterpret_cast' from integer to pointer |
| 84 | `0x00897BD0` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasChildren` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 24 | 3699 |  |
| 85 | `0x0089B4D0` | `CGameScriptInterface::CGameScriptInterface::RemoveHeroHairstyle` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 24 | 3883 |  |
| 86 | `0x008A1590` | `CGameScriptInterface::CGameScriptInterface::GetBestTimePairs` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 24 | 2907 | 'reinterpret_cast' from integer to pointer |
| 87 | `0x008A15F0` | `CGameScriptInterface::CGameScriptInterface::GetBestTimeSorting` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 24 | 2923 | 'reinterpret_cast' from integer to pointer |
| 88 | `0x008A17D0` | `CGameScriptInterface::CGameScriptInterface::GetBestTimeGuessTheAddition` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 24 | 2925 | 'reinterpret_cast' from integer to pointer |
| 89 | `0x00897C30` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasMurderedWife` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 25 | 3866 |  |
| 90 | `0x00897B70` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasDivorcedMarriage` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 25 | 3958 |  |
| 91 | `0x00897C90` | `CGameScriptInterface::CGameScriptInterface::GetNumberOfTimesHeroHasHadSex` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 25 | 4150 | 'reinterpret_cast' from integer to pointer |
| 92 | `0x008A16B0` | `CGameScriptInterface::CGameScriptInterface::GetBestScoreCoinGolfOakVale` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 25 | 3055 | 'reinterpret_cast' from integer to pointer |
| 93 | `0x00897030` | `CGameScriptInterface::CGameScriptInterface::IsToFailQuestOnDeath` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 3608 | 'reinterpret_cast' from integer to pointer |
| 94 | `0x00897480` | `CGameScriptInterface::CGameScriptInterface::IsPlayerWieldingWeapon` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 3816 | 'reinterpret_cast' from integer to pointer |
| 95 | `0x00897AA0` | `CGameScriptInterface::CGameScriptInterface::GetHeroHasMarried` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 4053 | 'reinterpret_cast' from integer to pointer |
| 96 | `0x0089EF30` | `CGameScriptInterface::CGameScriptInterface::SetMoralityChangingAsEnabled` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 4105 | 'reinterpret_cast' from integer to pointer |
| 97 | `0x00897CF0` | `CGameScriptInterface::CGameScriptInterface::SetNumberOfTimesHeroHasHadSex` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 4233 | 'reinterpret_cast' from integer to pointer |
| 98 | `0x0089B450` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHairstyle` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 4577 | 'reinterpret_cast' from integer to pointer |
| 99 | `0x00898540` | `CGameScriptInterface::CGameScriptInterface::SetGuardsIgnoreCrimes` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 27 | 3851 | 'reinterpret_cast' from integer to pointer |
| 100 | `0x00897D50` | `CGameScriptInterface::CGameScriptInterface::SetHeroAsHavingHadSex` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 27 | 4045 | 'reinterpret_cast' from integer to pointer |

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
|  | _64 additional quarantined candidates omitted_ |  |
