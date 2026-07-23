# Candidate signature audit

Generated: `2026-07-22T18:26:37-06:00`

Clean: **85 / 115**. Review required: **30**.

| Issue | Candidates |
|---|---:|
| `explicit-parameter-count-mismatch` | 14 |
| `method-name-mismatch` | 7 |
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
| `006b0730` | `CTCPhysicsBase` | `CTCPhysicsBase` | `CTCPhysicsBase` | owner-mismatch;duplicate-linkage-signature |
| `00713140` | `CActionDoCreatureAction` | `CActionDoCreatureAction` | `CActionDoCreatureAction` | owner-mismatch |
| `007266c0` | `CTCPhysicsBase` | `CTCPhysicsBase` | `CTCPhysicsBase` | owner-mismatch;duplicate-linkage-signature |
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
