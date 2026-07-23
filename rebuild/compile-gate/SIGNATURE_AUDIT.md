# Candidate signature audit

Generated: `2026-07-22T19:01:34-06:00`

Clean: **85 / 121**. Review required: **36**.

| Issue | Candidates |
|---|---:|
| `explicit-parameter-count-mismatch` | 20 |
| `method-name-mismatch` | 8 |
| `owner-mismatch` | 7 |
| `duplicate-linkage-signature` | 7 |
| `return-shape-mismatch` | 4 |
| `definition-not-parsed` | 2 |

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
| `004fe7f0` | `_global` | `CollectSoundThemeSwitchDefs` | `CollectSoundThemeSwitchDefs` | explicit-parameter-count-mismatch |
| `00662020` | `_global` | `Getter_FieldE0_FC` | `Getter_FieldE0_FC` | explicit-parameter-count-mismatch |
| `006b0730` | `CTCPhysicsBase` | `CTCPhysicsBase` | `CTCPhysicsBase` | owner-mismatch;duplicate-linkage-signature |
| `00713140` | `CActionDoCreatureAction` | `CActionDoCreatureAction` | `CActionDoCreatureAction` | owner-mismatch |
| `007266c0` | `CTCPhysicsBase` | `CTCPhysicsBase` | `CTCPhysicsBase` | owner-mismatch;duplicate-linkage-signature |
| `007602f0` | `_global` | `Math_PolygonAngleSumClosureCheck` | `Math_PolygonAngleSumClosureCheck_InvSqrtOrdered` | method-name-mismatch;explicit-parameter-count-mismatch |
| `008368f0` | `_global` | `ConditionalVirtualDispatch_OnLevelUnload` | `ConditionalVirtualDispatch_OnLevelUnload` | explicit-parameter-count-mismatch |
| `0086c5c0` | `_global` | `CPersistInfo_InsertionSortStep_0086c5c0` | `CPersistInfo_InsertionSortStep_0086c5c0` | explicit-parameter-count-mismatch |
| `0086fa00` | `_global` | `CPersistInfo_InsertionSortStep_0086fa00` | `CPersistInfo_InsertionSortStep_0086fa00` | explicit-parameter-count-mismatch |
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
| `00c3b900` | `CLandscapeLayerMesh` | `CLandscapeLayerMesh` | `` | definition-not-parsed;duplicate-linkage-signature |
| `00caf580` | `CEngineLandscapePatch` | `CEngineLandscapePatch::Destructor` | `` | definition-not-parsed |
| `00caf680` | `CEngineLandscapePatch` | `CEngineLandscapePatch::AllocateTask` | `AllocateTask` | explicit-parameter-count-mismatch |
