# Reconstruction backlog

Generated: `2026-07-22T23:12:19-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `CGameScriptInterface` | 954 | 6 | 106 | 0 |
| 2 | candidate-cluster | `_global` | 11370 | 9376 | 43 | 0 |
| 3 | candidate-cluster | `CMouseDX` | 24 | 0 | 14 | 10 |
| 4 | candidate-cluster | `CMemoryAllocatorFixedSize` | 21 | 0 | 11 | 7 |
| 5 | candidate-cluster | `CNavQuadTree` | 22 | 1 | 9 | 0 |
| 6 | candidate-cluster | `CEngineLandscapePatch` | 29 | 2 | 9 | 0 |
| 7 | candidate-cluster | `CEngineLandscapeRenderer` | 33 | 0 | 8 | 0 |
| 8 | candidate-cluster | `CScriptThing` | 103 | 2 | 6 | 0 |
| 9 | candidate-cluster | `CWorldMap` | 185 | 3 | 5 | 0 |
| 10 | candidate-cluster | `CEngineLandscapeMap` | 19 | 0 | 4 | 0 |
| 11 | candidate-cluster | `CEnginePrimitiveManagerMeshBase` | 4 | 0 | 4 | 0 |
| 12 | candidate-cluster | `CLandscapeLayerMesh` | 9 | 3 | 4 | 0 |
| 13 | candidate-cluster | `CMap` | 89 | 3 | 4 | 0 |
| 14 | candidate-cluster | `CLandscapeBackgroundPatch` | 3019 | 0 | 3 | 0 |
| 15 | candidate-cluster | `CLandscapeBackgroundTreeNode` | 29 | 0 | 3 | 0 |
| 16 | candidate-cluster | `CScriptedMapBrush` | 8 | 1 | 3 | 0 |
| 17 | candidate-cluster | `CTCPhysicsBase` | 51 | 2 | 2 | 1 |
| 18 | candidate-cluster | `CEngineScreenEffectOutlineGlow` | 31 | 0 | 2 | 0 |
| 19 | candidate-cluster | `CLocalDetailCacheMap` | 8 | 0 | 1 | 0 |
| 20 | candidate-cluster | `CObjectFamilyDef` | 4 | 0 | 1 | 0 |
| 21 | candidate-cluster | `CActionDoCreatureAction` | 751 | 1 | 1 | 0 |
| 22 | candidate-cluster | `CNavQuadTreeNode` | 26 | 2 | 1 | 0 |
| 23 | quick-closure | `CTCInventoryAbilities` | 100 | 1 | 0 | 0 |
| 24 | quick-closure | `C3DMeshFileSubMeshChunk` | 99 | 1 | 0 | 0 |
| 25 | quick-closure | `CPatchTesselationEdgeStrip` | 98 | 1 | 0 | 0 |

## Small candidate quick wins

Uncompiled auto-RE candidates ordered by authoritative retail function size, then candidate source size.

| Rank | Address | Module | Retail bytes | Candidate bytes | Blocker |
|---:|---|---|---:|---:|---|
| 1 | `0088e210` | `CGameScriptInterface` | 0 | 212 |  |
| 2 | `0088fbb0` | `CGameScriptInterface` | 0 | 212 |  |
| 3 | `0088fb10` | `CGameScriptInterface` | 0 | 228 |  |
| 4 | `0088fb30` | `CGameScriptInterface` | 0 | 232 |  |
| 5 | `0088fc10` | `CGameScriptInterface` | 0 | 235 |  |
| 6 | `0088fbd0` | `CGameScriptInterface` | 0 | 242 |  |
| 7 | `0088fad0` | `CGameScriptInterface` | 0 | 244 |  |
| 8 | `0088fa90` | `CGameScriptInterface` | 0 | 246 |  |
| 9 | `0088fab0` | `CGameScriptInterface` | 0 | 248 |  |
| 10 | `00893050` | `CGameScriptInterface` | 0 | 248 |  |
| 11 | `0088fb50` | `CGameScriptInterface` | 0 | 262 |  |
| 12 | `0088fb70` | `CGameScriptInterface` | 0 | 262 |  |
| 13 | `0088fc30` | `CGameScriptInterface` | 0 | 266 |  |
| 14 | `0088faf0` | `CGameScriptInterface` | 0 | 268 |  |
| 15 | `0088fbf0` | `CGameScriptInterface` | 0 | 299 |  |

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
| 29 | `006e7290` | `CGameScriptInterface` | candidate-compile |  |
| 30 | `006e7340` | `CGameScriptInterface` | candidate-compile |  |
| 31 | `006e7d40` | `CGameScriptInterface` | candidate-compile |  |
| 32 | `0088e210` | `CGameScriptInterface` | candidate-compile |  |
| 33 | `0088e400` | `CGameScriptInterface` | candidate-compile |  |
| 34 | `0088e4c0` | `CGameScriptInterface` | candidate-compile |  |
| 35 | `0088e4d0` | `CGameScriptInterface` | candidate-compile |  |
| 36 | `0088e500` | `CGameScriptInterface` | candidate-compile | 'reinterpret_cast' from integer to pointer |
| 37 | `0088ec90` | `CGameScriptInterface` | candidate-compile |  |
| 38 | `0088ecc0` | `CGameScriptInterface` | candidate-compile |  |
| 39 | `0088ece0` | `CGameScriptInterface` | candidate-compile |  |
| 40 | `0088f3e0` | `CGameScriptInterface` | candidate-compile |  |
