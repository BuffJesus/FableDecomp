# Reconstruction backlog

Generated: `2026-07-22T19:48:17-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `_global` | 11370 | 9376 | 43 | 0 |
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
| 1 | `00c19f90` | `_global` | 0 | 338 |  |
| 2 | `00c2d7b0` | `_global` | 0 | 493 |  |
| 3 | `00c20ef0` | `_global` | 0 | 582 |  |
| 4 | `008702d0` | `_global` | 0 | 728 |  |
| 5 | `00a7a5e0` | `CNavQuadTree` | 0 | 1309 | 'C2DLineF' was not declared in this scope |
| 6 | `007266c0` | `CTCPhysicsBase` | 0 | 1401 | static assertion failed |
| 7 | `008368f0` | `_global` | 0 | 1421 |  |
| 8 | `00c37000` | `_global` | 0 | 1482 |  |
| 9 | `00c36e00` | `_global` | 0 | 1495 |  |
| 10 | `00c36fb0` | `_global` | 0 | 1687 | expected '(' before '{' token |
| 11 | `00c362f0` | `_global` | 0 | 1706 |  |
| 12 | `0086fa00` | `_global` | 0 | 1753 |  |
| 13 | `0086c5c0` | `_global` | 0 | 1754 |  |
| 14 | `00c336f0` | `_global` | 0 | 1818 |  |
| 15 | `00c43f80` | `_global` | 0 | 1845 | expected '(' before '{' token |

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
| 12 | `00c20ef0` | `_global` | signature-audit |  |
| 13 | `00c2d7b0` | `_global` | signature-audit |  |
| 14 | `00c336f0` | `_global` | signature-audit |  |
| 15 | `00c362f0` | `_global` | signature-audit |  |
| 16 | `00c36e00` | `_global` | signature-audit |  |
| 17 | `00c36fb0` | `_global` | signature-audit | expected '(' before '{' token |
| 18 | `00c37000` | `_global` | signature-audit |  |
| 19 | `00c43de0` | `_global` | signature-audit |  |
| 20 | `00c43f80` | `_global` | signature-audit | expected '(' before '{' token |
| 21 | `008702d0` | `_global` | signature-audit |  |
| 22 | `00c19f90` | `_global` | signature-audit |  |
| 23 | `00ab5d00` | `CMouseDX` | signature-audit |  |
| 24 | `00a65ff0` | `CMemoryAllocatorFixedSize` | signature-audit | 'CMemoryAllocatorFixedSize' does not name a type |
| 25 | `00a793d0` | `CNavQuadTree` | signature-audit | 'CNavConnectorNode' does not name a type |
| 26 | `00a79680` | `CNavQuadTree` | signature-audit | 'CNavConnectorNode' does not name a type |
| 27 | `00caf580` | `CEngineLandscapePatch` | signature-audit | invalid use of incomplete type 'class CCharString' |
| 28 | `00caf680` | `CEngineLandscapePatch` | signature-audit | 'CEngineLandscapePatch' has not been declared |
| 29 | `00bfd420` | `CLandscapeLayerMesh` | signature-audit | 'CLandscapeLayerMesh' does not name a type |
| 30 | `00bfddd0` | `CLandscapeLayerMesh` | signature-audit | 'CGraphicInfoFrame' does not name a type |
| 31 | `00c3b900` | `CLandscapeLayerMesh` | signature-audit | 'CEngineLandscapeMap' does not name a type |
| 32 | `006b0730` | `CTCPhysicsBase` | signature-audit | 'C3DVector' does not name a type |
| 33 | `007266c0` | `CTCPhysicsBase` | signature-audit | static assertion failed |
| 34 | `00713140` | `CActionDoCreatureAction` | signature-audit | invalid use of incomplete type 'class NTCScriptedControl::CActionDoCreatureAction' |
| 35 | `00401296` | `_global` | compiled-needs-parity |  |
| 36 | `004012ce` | `_global` | compiled-needs-parity |  |
| 37 | `0040135c` | `_global` | compiled-needs-parity |  |
| 38 | `00403c60` | `_global` | compiled-needs-parity |  |
| 39 | `00404280` | `_global` | compiled-needs-parity |  |
| 40 | `00405ba0` | `_global` | compiled-needs-parity |  |
