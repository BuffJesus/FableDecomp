# Candidate signature audit

Generated: `2026-07-25T18:35:31-06:00`

Clean: **506 / 573**. Review required: **67**.

| Issue | Candidates |
|---|---:|
| `explicit-parameter-count-mismatch` | 42 |
| `return-shape-mismatch` | 16 |
| `definition-not-parsed` | 11 |
| `method-name-mismatch` | 9 |
| `owner-mismatch` | 7 |
| `duplicate-linkage-signature` | 7 |

## Review queue

| Address | Module | Manifest | Generated | Issues |
|---|---|---|---|---|
| `00401356` | `_global` | `initterm` | `initterm` | explicit-parameter-count-mismatch |
| `004013de` | `_global` | `__dllonexit` | `__dllonexit` | explicit-parameter-count-mismatch |
| `00401c00` | `_global` | `GFHandleSystemInitError` | `GFHandleSystemInitError` | explicit-parameter-count-mismatch |
| `00402130` | `_global` | `MyDocuments_CheckWritePermissions` | `MyDocuments_CheckWritePermissions` | explicit-parameter-count-mismatch |
| `004022b0` | `_global` | `GFInitialise` | `GFInitialise` | explicit-parameter-count-mismatch |
| `00403480` | `_global` | `_WinMain@16` | `_WinMain_16` | method-name-mismatch;explicit-parameter-count-mismatch |
| `00404690` | `_global` | `std_list_node_Init` | `std_list_node_Init` | explicit-parameter-count-mismatch |
| `004046f0` | `_global` | `ListNode_Create_Circular` | `ListNode_Create_Circular` | explicit-parameter-count-mismatch |
| `00404a90` | `_global` | `$E2` | `GlobalAt` | method-name-mismatch;explicit-parameter-count-mismatch;return-shape-mismatch |
| `00409090` | `_global` | `Engine_FindPrimitiveInArray` | `Engine_FindPrimitiveInArray` | explicit-parameter-count-mismatch |
| `0040cec0` | `_global` | `CUserProfileManager_Constructor` | `CUserProfileManager_Constructor` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `004aa9e0` | `CScriptThing` | `CScriptThing::GetCurrentMapName` | `GetCurrentMapName` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `004aaa10` | `CScriptThing` | `CScriptThing::GetHomeMapName` | `GetHomeMapName` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `004abe60` | `CScriptThing` | `CScriptThing::MsgWhoKilledMe` | `` | definition-not-parsed |
| `004d55d0` | `CTCCoopSpirit` | `CTCCoopSpirit::Construct` | `Construct` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `004fe7f0` | `_global` | `CollectSoundThemeSwitchDefs` | `CollectSoundThemeSwitchDefs` | explicit-parameter-count-mismatch |
| `006b0730` | `CTCPhysicsBase` | `CTCPhysicsBase` | `CTCPhysicsBase` | owner-mismatch;duplicate-linkage-signature |
| `006e7300` | `CGameScriptInterface` | `CGameScriptInterface::IsCameraPosOnScreen` | `` | definition-not-parsed |
| `00713140` | `CActionDoCreatureAction` | `CActionDoCreatureAction` | `CActionDoCreatureAction` | owner-mismatch |
| `007266c0` | `CTCPhysicsBase` | `CTCPhysicsBase` | `CTCPhysicsBase` | owner-mismatch;duplicate-linkage-signature |
| `007602f0` | `_global` | `Math_PolygonAngleSumClosureCheck` | `Math_PolygonAngleSumClosureCheck_InvSqrtOrdered` | method-name-mismatch;explicit-parameter-count-mismatch |
| `008368f0` | `_global` | `ConditionalVirtualDispatch_OnLevelUnload` | `ConditionalVirtualDispatch_OnLevelUnload` | explicit-parameter-count-mismatch |
| `0086c5c0` | `_global` | `CPersistInfo_InsertionSortStep_0086c5c0` | `CPersistInfo_InsertionSortStep_0086c5c0` | explicit-parameter-count-mismatch |
| `0086fa00` | `_global` | `CPersistInfo_InsertionSortStep_0086fa00` | `CPersistInfo_InsertionSortStep_0086fa00` | explicit-parameter-count-mismatch |
| `008702d0` | `_global` | `Handle_InitInvalid_008702d0` | `Handle_InitInvalid_008702d0` | explicit-parameter-count-mismatch |
| `0088ed20` | `CGameScriptInterface` | `CGameScriptInterface::CameraEarthquakeIntensityAtPos` | `` | definition-not-parsed |
| `0088fd60` | `CGameScriptInterface` | `CGameScriptInterface::IsEntityInLimbo` | `` | definition-not-parsed |
| `00890090` | `CGameScriptInterface` | `CGameScriptInterface::SetEnvironmentThemeWeightExternals` | `` | definition-not-parsed |
| `00890280` | `CGameScriptInterface` | `CGameScriptInterface::IsEntityAbleToAttack` | `` | definition-not-parsed |
| `00891610` | `CGameScriptInterface` | `CGameScriptInterface::GetMostRecentValidUsedTarget` | `GetMostRecentValidUsedTarget` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `00891d50` | `CGameScriptInterface` | `CGameScriptInterface::GetThingWithUID` | `GetThingWithUID` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `00892b50` | `CGameScriptInterface` | `CGameScriptInterface::CameraCircleAroundThing` | `` | definition-not-parsed |
| `008961b0` | `CGameScriptInterface` | `CGameScriptInterface::ClearHeroEnemyOfGuards` | `` | definition-not-parsed |
| `0089a490` | `CGameScriptInterface` | `CGameScriptInterface::GetHeroTargetedThing` | `GetHeroTargetedThing` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `0089c510` | `CGameScriptInterface` | `CGameScriptInterface::OpinionSourceSetAsAttentionGrabbing` | `` | definition-not-parsed |
| `0089faa0` | `CGameScriptInterface` | `CGameScriptInterface::CreateLight` | `CreateLight` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `0089fbb0` | `CGameScriptInterface` | `CGameScriptInterface::CreateExperienceOrb` | `CreateExperienceOrb` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `0089fc80` | `CGameScriptInterface` | `CGameScriptInterface::CreateExplosion` | `CreateExplosion` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `0089fd20` | `CGameScriptInterface` | `CGameScriptInterface::CreatePhysicalBarrier` | `CreatePhysicalBarrier` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `0089fda0` | `CGameScriptInterface` | `CGameScriptInterface::CreateRumble` | `CreateRumble` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `008d1890` | `CGameScriptThing` | `CGameScriptThing::MsgWhoHitMe` | `MsgWhoHitMe` | explicit-parameter-count-mismatch;return-shape-mismatch |
| `00a65ff0` | `CMemoryAllocatorFixedSize` | `CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize` | `CMemoryAllocatorFixedSize` | owner-mismatch |
| `00a793d0` | `CNavQuadTree` | `ConnectVerticalMapEdge` | `ConnectVerticalMapEdge` | duplicate-linkage-signature |
| `00a79680` | `CNavQuadTree` | `ConnectVerticalMapEdge` | `ConnectVerticalMapEdge` | duplicate-linkage-signature |
| `00ab5d00` | `CMouseDX` | `CMouseDX::CMouseDX` | `CMouseDX` | owner-mismatch |
| `00beaaa0` | `CEngineScreenEffectOutlineGlow` | `CEngineScreenEffectOutlineGlow::~CEngineScreenEffectOutlineGlow` | `FinishWithDynamicTextures` | method-name-mismatch |
| `00beb4d0` | `CEngineScreenEffectOutlineGlow` | `CEngineScreenEffectOutlineGlow::ScalarDeletingDestructor` | `FinishWithDynamicTextures` | method-name-mismatch;return-shape-mismatch |
| `00bf3b60` | `CEngineLandscapePatch` | `LoadForegroundPatch` | `LoadForegroundPatch` | explicit-parameter-count-mismatch |
| `00bf4220` | `CEngineLandscapePatch` | `UpdateForeground` | `UpdateForeground` | explicit-parameter-count-mismatch |
| `00bf5c20` | `CEngineLandscapeRenderer` | `CEngineLandscapeRenderer::GetForegroundVertexShader` | `GetForegroundSpotLightVertexShader` | method-name-mismatch |
| `00bf5ca0` | `CEngineLandscapeRenderer` | `CEngineLandscapeRenderer::GetForegroundShadowedVertexShader` | `GetForegroundBumpShadowedVertexShader` | method-name-mismatch;return-shape-mismatch |
| `00bf5d00` | `CEngineLandscapeRenderer` | `CEngineLandscapeRenderer::GetForegroundSpotShadowedVertexShader` | `GetForegroundBumpShadowedVertexShader` | method-name-mismatch |
| `00bfd420` | `CLandscapeLayerMesh` | `CLandscapeLayerMesh` | `CLandscapeLayerMesh` | owner-mismatch;duplicate-linkage-signature |
| `00bfddd0` | `CLandscapeLayerMesh` | `CLandscapeLayerMesh` | `CLandscapeLayerMesh` | owner-mismatch;duplicate-linkage-signature |
| `00c19f90` | `_global` | `Getter_Field84` | `Getter_Field84` | explicit-parameter-count-mismatch |
| `00c20ef0` | `_global` | `IsStateStreaming_2to4` | `IsStateStreaming_2to4` | explicit-parameter-count-mismatch |
| `00c2d7b0` | `_global` | `CTCInventory_CheckSlotTypeNegative` | `CTCInventory_CheckSlotTypeNegative` | explicit-parameter-count-mismatch |
| `00c336f0` | `_global` | `Constraint_NotifyGlobalManager` | `Constraint_NotifyGlobalManager` | explicit-parameter-count-mismatch |
| `00c362f0` | `_global` | `CSBAgents_BuildAndDispatch_A` | `CSBAgents_BuildAndDispatch_A` | explicit-parameter-count-mismatch |
| `00c36e00` | `_global` | `CSBAgents_BuildAndDispatch_SummonerLightning` | `CSBAgents_BuildAndDispatch_SummonerLightning` | explicit-parameter-count-mismatch |
| `00c36fb0` | `_global` | `CSBAgents_BuildAndDispatch_TavernGame` | `CSBAgents_BuildAndDispatch_TavernGame_Impl` | method-name-mismatch;explicit-parameter-count-mismatch |
| `00c37000` | `_global` | `IntelligentPointer_BuildAndDispatchFloat` | `IntelligentPointer_BuildAndDispatchFloat` | explicit-parameter-count-mismatch |
| `00c3b900` | `CLandscapeLayerMesh` | `CLandscapeLayerMesh` | `` | definition-not-parsed;duplicate-linkage-signature |
| `00c43de0` | `_global` | `DrawAutoPickups_SelectAndDrawPrimitive` | `DrawAutoPickups_SelectAndDrawPrimitive` | explicit-parameter-count-mismatch |
| `00c43f80` | `_global` | `Weapon_ProcessAnimFrameIfReady` | `Weapon_ProcessAnimFrameIfReady` | explicit-parameter-count-mismatch |
| `00caf580` | `CEngineLandscapePatch` | `CEngineLandscapePatch::Destructor` | `` | definition-not-parsed |
| `00caf680` | `CEngineLandscapePatch` | `CEngineLandscapePatch::AllocateTask` | `AllocateTask` | explicit-parameter-count-mismatch |
