# Reconstruction backlog

Generated: `2026-07-22T18:26:38-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `_global` | 11370 | 9377 | 26 | 0 |
| 2 | candidate-cluster | `CMouseDX` | 24 | 0 | 14 | 10 |
| 3 | candidate-cluster | `CMemoryAllocatorFixedSize` | 21 | 0 | 11 | 7 |
| 4 | candidate-cluster | `CNavQuadTree` | 22 | 1 | 9 | 0 |
| 5 | candidate-cluster | `CEngineLandscapePatch` | 29 | 2 | 9 | 0 |
| 6 | candidate-cluster | `CEngineLandscapeRenderer` | 33 | 0 | 8 | 0 |
| 7 | candidate-cluster | `CWorldMap` | 185 | 3 | 5 | 0 |
| 8 | candidate-cluster | `CEngineLandscapeMap` | 19 | 0 | 4 | 0 |
| 9 | candidate-cluster | `CEnginePrimitiveManagerMeshBase` | 4 | 0 | 4 | 0 |
| 10 | candidate-cluster | `CLandscapeLayerMesh` | 9 | 3 | 4 | 0 |
| 11 | candidate-cluster | `CMap` | 89 | 3 | 4 | 0 |
| 12 | candidate-cluster | `CLandscapeBackgroundPatch` | 3019 | 0 | 3 | 0 |
| 13 | candidate-cluster | `CLandscapeBackgroundTreeNode` | 29 | 0 | 3 | 0 |
| 14 | candidate-cluster | `CScriptedMapBrush` | 8 | 1 | 3 | 0 |
| 15 | candidate-cluster | `CTCPhysicsBase` | 51 | 2 | 2 | 1 |
| 16 | candidate-cluster | `CEngineScreenEffectOutlineGlow` | 31 | 0 | 2 | 0 |
| 17 | candidate-cluster | `CLocalDetailCacheMap` | 8 | 0 | 1 | 0 |
| 18 | candidate-cluster | `CObjectFamilyDef` | 4 | 0 | 1 | 0 |
| 19 | candidate-cluster | `CActionDoCreatureAction` | 751 | 1 | 1 | 0 |
| 20 | candidate-cluster | `CNavQuadTreeNode` | 26 | 2 | 1 | 0 |
| 21 | quick-closure | `CTCInventoryAbilities` | 100 | 1 | 0 | 0 |
| 22 | quick-closure | `C3DMeshFileSubMeshChunk` | 99 | 1 | 0 | 0 |
| 23 | quick-closure | `CPatchTesselationEdgeStrip` | 98 | 1 | 0 | 0 |
| 24 | quick-closure | `CRegion` | 72 | 1 | 0 | 0 |
| 25 | quick-closure | `CDefClassBase` | 168 | 2 | 0 | 0 |

## Small candidate quick wins

Uncompiled auto-RE candidates ordered by authoritative retail function size, then candidate source size.

| Rank | Address | Module | Retail bytes | Candidate bytes | Blocker |
|---:|---|---|---:|---:|---|
| 1 | `00a7a5e0` | `CNavQuadTree` | 0 | 1309 | 'C2DLineF' was not declared in this scope |
| 2 | `007266c0` | `CTCPhysicsBase` | 0 | 1401 | static assertion failed |
| 3 | `00403480` | `_global` | 0 | 1865 |  |
| 4 | `006b0730` | `CTCPhysicsBase` | 0 | 1971 | 'C3DVector' does not name a type |
| 5 | `00c3b900` | `CLandscapeLayerMesh` | 0 | 1978 | 'CEngineLandscapeMap' does not name a type |
| 6 | `00409090` | `_global` | 0 | 2191 |  |
| 7 | `00caf680` | `CEngineLandscapePatch` | 0 | 2195 | 'CEngineLandscapePatch' has not been declared |
| 8 | `00713140` | `CActionDoCreatureAction` | 0 | 2734 | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 9 | `00a65ff0` | `CMemoryAllocatorFixedSize` | 0 | 2986 | 'CMemoryAllocatorFixedSize' does not name a type |
| 10 | `00bf3980` | `CEngineLandscapePatch` | 0 | 3473 | 'CEngineLandscapePatch' has not been declared |
| 11 | `00ab5d00` | `CMouseDX` | 0 | 4084 |  |
| 12 | `00bfd420` | `CLandscapeLayerMesh` | 0 | 4210 | 'CLandscapeLayerMesh' does not name a type |
| 13 | `00caf580` | `CEngineLandscapePatch` | 0 | 4306 | invalid use of incomplete type 'class CCharString' |
| 14 | `00bfddd0` | `CLandscapeLayerMesh` | 0 | 4322 | 'CGraphicInfoFrame' does not name a type |
| 15 | `004022b0` | `_global` | 0 | 4991 | invalid use of incomplete type 'struct CDisplayManager' |

## First function queue

| Rank | Address | Module | Gap | Compiler blocker |
|---:|---|---|---|---|
| 1 | `00401c00` | `_global` | signature-audit |  |
| 2 | `00402130` | `_global` | signature-audit |  |
| 3 | `004022b0` | `_global` | signature-audit | invalid use of incomplete type 'struct CDisplayManager' |
| 4 | `00403480` | `_global` | signature-audit |  |
| 5 | `00409090` | `_global` | signature-audit |  |
| 6 | `0040cec0` | `_global` | signature-audit |  |
| 7 | `00ab5d00` | `CMouseDX` | signature-audit |  |
| 8 | `00a65ff0` | `CMemoryAllocatorFixedSize` | signature-audit | 'CMemoryAllocatorFixedSize' does not name a type |
| 9 | `00a793d0` | `CNavQuadTree` | signature-audit | 'CNavConnectorNode' does not name a type |
| 10 | `00a79680` | `CNavQuadTree` | signature-audit | 'CNavConnectorNode' does not name a type |
| 11 | `00caf580` | `CEngineLandscapePatch` | signature-audit | invalid use of incomplete type 'class CCharString' |
| 12 | `00caf680` | `CEngineLandscapePatch` | signature-audit | 'CEngineLandscapePatch' has not been declared |
| 13 | `00bfd420` | `CLandscapeLayerMesh` | signature-audit | 'CLandscapeLayerMesh' does not name a type |
| 14 | `00bfddd0` | `CLandscapeLayerMesh` | signature-audit | 'CGraphicInfoFrame' does not name a type |
| 15 | `00c3b900` | `CLandscapeLayerMesh` | signature-audit | 'CEngineLandscapeMap' does not name a type |
| 16 | `006b0730` | `CTCPhysicsBase` | signature-audit | 'C3DVector' does not name a type |
| 17 | `007266c0` | `CTCPhysicsBase` | signature-audit | static assertion failed |
| 18 | `00713140` | `CActionDoCreatureAction` | signature-audit | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 19 | `00401296` | `_global` | compiled-needs-parity |  |
| 20 | `004012ce` | `_global` | compiled-needs-parity |  |
| 21 | `00401356` | `_global` | compiled-needs-parity |  |
| 22 | `0040135c` | `_global` | compiled-needs-parity |  |
| 23 | `0040138c` | `_global` | compiled-needs-parity |  |
| 24 | `004013c7` | `_global` | compiled-needs-parity |  |
| 25 | `004013de` | `_global` | compiled-needs-parity |  |
| 26 | `00401f30` | `_global` | compiled-needs-parity |  |
| 27 | `00403c60` | `_global` | compiled-needs-parity |  |
| 28 | `00404280` | `_global` | compiled-needs-parity |  |
| 29 | `00404310` | `_global` | compiled-needs-parity |  |
| 30 | `00404690` | `_global` | compiled-needs-parity |  |
| 31 | `004046f0` | `_global` | compiled-needs-parity |  |
| 32 | `00404a90` | `_global` | compiled-needs-parity |  |
| 33 | `00405ba0` | `_global` | compiled-needs-parity |  |
| 34 | `00406770` | `_global` | compiled-needs-parity |  |
| 35 | `0040bb90` | `_global` | compiled-needs-parity |  |
| 36 | `0040e130` | `_global` | compiled-needs-parity |  |
| 37 | `0040e170` | `_global` | compiled-needs-parity |  |
| 38 | `00ab4700` | `CMouseDX` | compiled-needs-parity |  |
| 39 | `00a65d70` | `CMemoryAllocatorFixedSize` | compiled-needs-parity |  |
| 40 | `00a65da0` | `CMemoryAllocatorFixedSize` | compiled-needs-parity |  |
