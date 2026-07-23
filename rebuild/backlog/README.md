# Reconstruction backlog

Generated: `2026-07-22T19:11:18-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `_global` | 11370 | 9376 | 32 | 0 |
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
| 3 | `008368f0` | `_global` | 0 | 1421 |  |
| 4 | `0086fa00` | `_global` | 0 | 1753 |  |
| 5 | `0086c5c0` | `_global` | 0 | 1754 |  |
| 6 | `00403480` | `_global` | 0 | 1865 |  |
| 7 | `006b0730` | `CTCPhysicsBase` | 0 | 1971 | 'C3DVector' does not name a type |
| 8 | `00c3b900` | `CLandscapeLayerMesh` | 0 | 1978 | 'CEngineLandscapeMap' does not name a type |
| 9 | `00409090` | `_global` | 0 | 2191 |  |
| 10 | `00caf680` | `CEngineLandscapePatch` | 0 | 2195 | 'CEngineLandscapePatch' has not been declared |
| 11 | `00713140` | `CActionDoCreatureAction` | 0 | 2734 | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 12 | `00a65ff0` | `CMemoryAllocatorFixedSize` | 0 | 2986 | 'CMemoryAllocatorFixedSize' does not name a type |
| 13 | `00ab5d00` | `CMouseDX` | 0 | 4084 |  |
| 14 | `00bfd420` | `CLandscapeLayerMesh` | 0 | 4210 | 'CLandscapeLayerMesh' does not name a type |
| 15 | `00caf580` | `CEngineLandscapePatch` | 0 | 4306 | invalid use of incomplete type 'class CCharString' |

## First function queue

| Rank | Address | Module | Gap | Compiler blocker |
|---:|---|---|---|---|
| 1 | `00401c00` | `_global` | signature-audit |  |
| 2 | `00402130` | `_global` | signature-audit |  |
| 3 | `004022b0` | `_global` | signature-audit | invalid use of incomplete type 'struct CDisplayManager' |
| 4 | `00403480` | `_global` | signature-audit |  |
| 5 | `00409090` | `_global` | signature-audit |  |
| 6 | `0040cec0` | `_global` | signature-audit |  |
| 7 | `004fe7f0` | `_global` | signature-audit | static assertion failed |
| 8 | `007602f0` | `_global` | signature-audit | expected '(' before '{' token |
| 9 | `008368f0` | `_global` | signature-audit |  |
| 10 | `0086c5c0` | `_global` | signature-audit |  |
| 11 | `0086fa00` | `_global` | signature-audit |  |
| 12 | `00ab5d00` | `CMouseDX` | signature-audit |  |
| 13 | `00a65ff0` | `CMemoryAllocatorFixedSize` | signature-audit | 'CMemoryAllocatorFixedSize' does not name a type |
| 14 | `00a793d0` | `CNavQuadTree` | signature-audit | 'CNavConnectorNode' does not name a type |
| 15 | `00a79680` | `CNavQuadTree` | signature-audit | 'CNavConnectorNode' does not name a type |
| 16 | `00caf580` | `CEngineLandscapePatch` | signature-audit | invalid use of incomplete type 'class CCharString' |
| 17 | `00caf680` | `CEngineLandscapePatch` | signature-audit | 'CEngineLandscapePatch' has not been declared |
| 18 | `00bfd420` | `CLandscapeLayerMesh` | signature-audit | 'CLandscapeLayerMesh' does not name a type |
| 19 | `00bfddd0` | `CLandscapeLayerMesh` | signature-audit | 'CGraphicInfoFrame' does not name a type |
| 20 | `00c3b900` | `CLandscapeLayerMesh` | signature-audit | 'CEngineLandscapeMap' does not name a type |
| 21 | `006b0730` | `CTCPhysicsBase` | signature-audit | 'C3DVector' does not name a type |
| 22 | `007266c0` | `CTCPhysicsBase` | signature-audit | static assertion failed |
| 23 | `00713140` | `CActionDoCreatureAction` | signature-audit | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 24 | `00401296` | `_global` | compiled-needs-parity |  |
| 25 | `004012ce` | `_global` | compiled-needs-parity |  |
| 26 | `0040135c` | `_global` | compiled-needs-parity |  |
| 27 | `00403c60` | `_global` | compiled-needs-parity |  |
| 28 | `00404280` | `_global` | compiled-needs-parity |  |
| 29 | `00405ba0` | `_global` | compiled-needs-parity |  |
| 30 | `0040bb90` | `_global` | compiled-needs-parity |  |
| 31 | `0040e170` | `_global` | compiled-needs-parity |  |
| 32 | `00ab4700` | `CMouseDX` | compiled-needs-parity |  |
| 33 | `00a65d70` | `CMemoryAllocatorFixedSize` | compiled-needs-parity |  |
| 34 | `00a65da0` | `CMemoryAllocatorFixedSize` | compiled-needs-parity |  |
| 35 | `00a65e20` | `CMemoryAllocatorFixedSize` | compiled-needs-parity |  |
| 36 | `00a7a650` | `CNavQuadTree` | compiled-needs-parity |  |
| 37 | `00bf3a90` | `CEngineLandscapePatch` | compiled-needs-parity |  |
| 38 | `00bf3b60` | `CEngineLandscapePatch` | compiled-needs-parity |  |
| 39 | `00bf4130` | `CEngineLandscapePatch` | compiled-needs-parity |  |
| 40 | `00bf4220` | `CEngineLandscapePatch` | compiled-needs-parity |  |
