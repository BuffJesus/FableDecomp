# Curated C++ promotion queue

Generated: `2026-07-22T20:26:41-06:00`

Uncompiled auto-RE candidates: **54**. Showing: **54**.

Ranking favors checker/integrity/signature PASS, then the smallest declaration, dependency, VC7.1, and source-size repair surface. It does not claim semantic correctness; every promotion still needs a focused behavior oracle and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---:|---:|---:|---|
| 1 | `0x00890180` | `CGameScriptInterface::CGameScriptInterface::CancelRadialBlurFade` | `vc71-port` | `PASS` | 0 | 11 | 1736 |  |
| 2 | `0x0088EC90` | `CGameScriptInterface::CGameScriptInterface::CameraSetCameraPreloadFlag` | `vc71-port` | `PASS` | 0 | 13 | 1885 |  |
| 3 | `0x0088ECE0` | `CGameScriptInterface::CGameScriptInterface::CameraShake` | `vc71-port` | `PASS` | 0 | 14 | 2424 |  |
| 4 | `0x00891720` | `CGameScriptInterface::CGameScriptInterface::ChangeHeroHealthBy` | `vc71-port` | `PASS` | 0 | 31 | 5345 |  |
| 5 | `0x008A0820` | `CGameScriptInterface::CGameScriptInterface::CancelHeroTeleportEffects` | `vc71-port` | `PASS` | 0 | 35 | 5545 |  |
| 6 | `0x004AADA0` | `CScriptThing::CScriptThing::MsgOpenedChest` | `dependency-stub` | `PASS` | 1 | 3 | 811 | invalid use of incomplete type 'class CScriptThing' |
| 7 | `0x004AACC0` | `CScriptThing::CScriptThing::MsgPerformedAnySpecialAbility` | `dependency-stub` | `PASS` | 1 | 3 | 940 | invalid use of incomplete type 'class CScriptThing' |
| 8 | `0x004AB040` | `CScriptThing::CScriptThing::SetAsUsable` | `dependency-stub` | `PASS` | 1 | 7 | 1094 | invalid use of incomplete type 'class CScriptThing' |
| 9 | `0x004AAF80` | `CScriptThing::CScriptThing::MsgIsKicked` | `dependency-stub` | `PASS` | 1 | 8 | 1075 | invalid use of incomplete type 'class CScriptThing' |
| 10 | `0x004AACE0` | `CScriptThing::CScriptThing::MsgPerformedAnyAggressiveSpecialAbility` | `dependency-stub` | `PASS` | 1 | 8 | 1382 | invalid use of incomplete type 'class CScriptThing' |
| 11 | `0x004AAD00` | `CScriptThing::CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility` | `dependency-stub` | `PASS` | 1 | 8 | 1425 | invalid use of incomplete type 'class CScriptThing' |
| 12 | `0x00890FC0` | `CGameScriptInterface::CGameScriptInterface::CameraResetToViewBehindHero` | `dependency-stub` | `PASS` | 1 | 8 | 1066 | expected '(' before '{' token |
| 13 | `0x00890FF0` | `CGameScriptInterface::CGameScriptInterface::CameraDefault` | `manual-lift` | `PASS` | 1 | 27 | 3894 | 'field48' was not declared in this scope |
| 14 | `0x008977F0` | `CGameScriptInterface::CGameScriptInterface::ApplyHeroPenaltyForDeath` | `manual-lift` | `PASS` | 1 | 30 | 4950 | invalid use of incomplete type 'class CGameScriptInterface' |
| 15 | `0x00401067` | `entry` | `manual-lift` | `PASS` | 3 | 50 | 6600 | static assertion failed |
| 16 | `0x00A76F30` | `CNavQuadTree::IsAreaBlockedByLines` | `manual-lift` | `PASS` | 9 | 24 | 6581 | 'list' in namespace 'std' does not name a template type |
| 17 | `0x00A781A0` | `CNavQuadTree::UpdateLines` | `manual-lift` | `PASS` | 12 | 75 | 16425 | 'C2DVector' does not name a type |
| 18 | `0x00A7A8D0` | `CNavQuadTree::Initialise` | `manual-lift` | `PASS` | 13 | 142 | 27473 | 'C2DLineF' was not declared in this scope |
| 19 | `0x00A79B30` | `CNavQuadTreeNode::Initialise` | `manual-lift` | `PASS` | 18 | 78 | 16120 | 'CANavQuadTreeNode' does not name a type |
| 20 | `0x0050A650` | `CWorldMap::GetMapNavigationAreaInit` | `manual-lift` | `PASS` | 26 | 93 | 26227 | use of enum 'EHeroMorphType' without previous declaration |
| 21 | `0x00C19F90` | `Getter_Field84` | `vc71-port` | `REVIEW` | 0 | 4 | 338 |  |
| 22 | `0x00C2D7B0` | `CTCInventory_CheckSlotTypeNegative` | `vc71-port` | `REVIEW` | 0 | 5 | 493 |  |
| 23 | `0x00401C00` | `GFHandleSystemInitError` | `vc71-port` | `REVIEW` | 0 | 5 | 9788 |  |
| 24 | `0x00C20EF0` | `IsStateStreaming_2to4` | `vc71-port` | `REVIEW` | 0 | 6 | 582 |  |
| 25 | `0x008702D0` | `Handle_InitInvalid_008702d0` | `vc71-port` | `REVIEW` | 0 | 8 | 728 |  |
| 26 | `0x00C37000` | `IntelligentPointer_BuildAndDispatchFloat` | `vc71-port` | `REVIEW` | 0 | 9 | 1482 |  |
| 27 | `0x00C36E00` | `CSBAgents_BuildAndDispatch_SummonerLightning` | `vc71-port` | `REVIEW` | 0 | 10 | 1495 |  |
| 28 | `0x00409090` | `Engine_FindPrimitiveInArray` | `vc71-port` | `REVIEW` | 0 | 10 | 2191 |  |
| 29 | `0x008368F0` | `ConditionalVirtualDispatch_OnLevelUnload` | `vc71-port` | `REVIEW` | 0 | 12 | 1421 |  |
| 30 | `0x0086FA00` | `CPersistInfo_InsertionSortStep_0086fa00` | `vc71-port` | `REVIEW` | 0 | 12 | 1753 |  |
| 31 | `0x0086C5C0` | `CPersistInfo_InsertionSortStep_0086c5c0` | `vc71-port` | `REVIEW` | 0 | 12 | 1754 |  |
| 32 | `0x00C362F0` | `CSBAgents_BuildAndDispatch_A` | `vc71-port` | `REVIEW` | 0 | 13 | 1706 |  |
| 33 | `0x00403480` | `_WinMain@16` | `vc71-port` | `REVIEW` | 0 | 16 | 1865 |  |
| 34 | `0x00C43DE0` | `DrawAutoPickups_SelectAndDrawPrimitive` | `vc71-port` | `REVIEW` | 0 | 19 | 2098 |  |
| 35 | `0x00C336F0` | `Constraint_NotifyGlobalManager` | `vc71-port` | `REVIEW` | 0 | 21 | 1818 |  |
| 36 | `0x00402130` | `MyDocuments_CheckWritePermissions` | `vc71-port` | `REVIEW` | 0 | 29 | 6575 |  |
| 37 | `0x00AB5D00` | `CMouseDX::CMouseDX::CMouseDX` | `vc71-port` | `REVIEW` | 0 | 35 | 4084 |  |
| 38 | `0x00C36FB0` | `CSBAgents_BuildAndDispatch_TavernGame` | `dependency-stub` | `REVIEW` | 1 | 10 | 1687 | expected '(' before '{' token |
| 39 | `0x00C43F80` | `Weapon_ProcessAnimFrameIfReady` | `manual-lift` | `REVIEW` | 1 | 13 | 1845 | expected '(' before '{' token |
| 40 | `0x007602F0` | `Math_PolygonAngleSumClosureCheck` | `manual-lift` | `REVIEW` | 1 | 16 | 5367 | expected '(' before '{' token |
| 41 | `0x007266C0` | `CTCPhysicsBase::CTCPhysicsBase` | `manual-lift` | `REVIEW` | 1 | 16 | 1401 | static assertion failed |
| 42 | `0x00713140` | `CActionDoCreatureAction::CActionDoCreatureAction` | `manual-lift` | `REVIEW` | 1 | 24 | 2734 | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 43 | `0x00A65FF0` | `CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize` | `manual-lift` | `REVIEW` | 1 | 41 | 2986 | 'CMemoryAllocatorFixedSize' does not name a type |
| 44 | `0x00BFD420` | `CLandscapeLayerMesh::CLandscapeLayerMesh` | `manual-lift` | `REVIEW` | 1 | 47 | 4210 | 'CLandscapeLayerMesh' does not name a type |
| 45 | `0x004022B0` | `GFInitialise` | `manual-lift` | `REVIEW` | 1 | 47 | 4991 | invalid use of incomplete type 'struct CDisplayManager' |
| 46 | `0x006B0730` | `CTCPhysicsBase::CTCPhysicsBase` | `manual-lift` | `REVIEW` | 2 | 14 | 1971 | 'C3DVector' does not name a type |
| 47 | `0x00CAF680` | `CEngineLandscapePatch::CEngineLandscapePatch::AllocateTask` | `manual-lift` | `REVIEW` | 2 | 28 | 2195 | 'CEngineLandscapePatch' has not been declared |
| 48 | `0x00CAF580` | `CEngineLandscapePatch::CEngineLandscapePatch::Destructor` | `manual-lift` | `REVIEW` | 2 | 30 | 4306 | invalid use of incomplete type 'class CCharString' |
| 49 | `0x00C3B900` | `CLandscapeLayerMesh::CLandscapeLayerMesh` | `manual-lift` | `REVIEW` | 3 | 15 | 1978 | 'CEngineLandscapeMap' does not name a type |
| 50 | `0x00BFDDD0` | `CLandscapeLayerMesh::CLandscapeLayerMesh` | `manual-lift` | `REVIEW` | 7 | 30 | 4322 | 'CGraphicInfoFrame' does not name a type |
| 51 | `0x00A79680` | `CNavQuadTree::ConnectVerticalMapEdge` | `manual-lift` | `REVIEW` | 9 | 58 | 13026 | 'CNavConnectorNode' does not name a type |
| 52 | `0x00A793D0` | `CNavQuadTree::ConnectVerticalMapEdge` | `manual-lift` | `REVIEW` | 11 | 60 | 12933 | 'CNavConnectorNode' does not name a type |
| 53 | `0x004FE7F0` | `CollectSoundThemeSwitchDefs` | `manual-lift` | `REVIEW` | 16 | 83 | 10371 | static assertion failed |
| 54 | `0x0040CEC0` | `CUserProfileManager_Constructor` | `vc71-port` | `REVIEW` | 0 | 99 | 14968 |  |
