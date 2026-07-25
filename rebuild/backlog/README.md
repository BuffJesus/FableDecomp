# Reconstruction backlog

Generated: `2026-07-25T16:44:08-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `CGameScriptInterface` | 954 | 6 | 379 | 0 |
| 2 | candidate-cluster | `_global` | 11370 | 9376 | 43 | 0 |
| 3 | candidate-cluster | `CMouseDX` | 24 | 0 | 14 | 10 |
| 4 | candidate-cluster | `CMemoryAllocatorFixedSize` | 21 | 0 | 11 | 7 |
| 5 | candidate-cluster | `CScriptThing` | 103 | 2 | 12 | 0 |
| 6 | candidate-cluster | `CNavQuadTree` | 22 | 1 | 9 | 0 |
| 7 | candidate-cluster | `CEngineLandscapePatch` | 29 | 2 | 9 | 0 |
| 8 | candidate-cluster | `CEngineLandscapeRenderer` | 33 | 0 | 8 | 0 |
| 9 | candidate-cluster | `CNetworkClient` | 6 | 0 | 5 | 0 |
| 10 | candidate-cluster | `CMainGameComponent` | 63 | 1 | 5 | 0 |
| 11 | candidate-cluster | `CTCCoopSpirit` | 24 | 2 | 5 | 0 |
| 12 | candidate-cluster | `CWorldMap` | 185 | 3 | 5 | 0 |
| 13 | candidate-cluster | `CEngineLandscapeMap` | 19 | 0 | 4 | 0 |
| 14 | candidate-cluster | `CEnginePrimitiveManagerMeshBase` | 4 | 0 | 4 | 0 |
| 15 | candidate-cluster | `CLandscapeLayerMesh` | 9 | 3 | 4 | 0 |
| 16 | candidate-cluster | `CMap` | 89 | 3 | 4 | 0 |
| 17 | candidate-cluster | `CLandscapeBackgroundPatch` | 3019 | 0 | 3 | 0 |
| 18 | candidate-cluster | `CLandscapeBackgroundTreeNode` | 29 | 0 | 3 | 0 |
| 19 | candidate-cluster | `CGameEventPackageSet` | 4 | 1 | 3 | 0 |
| 20 | candidate-cluster | `CScriptedMapBrush` | 8 | 1 | 3 | 0 |
| 21 | candidate-cluster | `CGameScriptThing` | 91 | 3 | 3 | 0 |
| 22 | candidate-cluster | `CTCPhysicsBase` | 51 | 2 | 2 | 1 |
| 23 | candidate-cluster | `CEngineScreenEffectOutlineGlow` | 31 | 0 | 2 | 0 |
| 24 | candidate-cluster | `CGameEvent` | 14 | 1 | 2 | 0 |
| 25 | candidate-cluster | `CPlayerManager` | 21 | 2 | 2 | 0 |

## Small candidate quick wins

Uncompiled auto-RE candidates ordered by authoritative retail function size, then candidate source size.

| Rank | Address | Module | Retail bytes | Candidate bytes | Blocker |
|---:|---|---|---:|---:|---|
| 1 | `004aea70` | `CNetworkClient` | 0 | 179 | 'CNetworkClient' has not been declared |
| 2 | `004aeba0` | `CNetworkClient` | 0 | 210 | 'CNetworkClient' has not been declared |
| 3 | `0088f660` | `CGameScriptInterface` | 0 | 282 |  |
| 4 | `004d55d0` | `CTCCoopSpirit` | 0 | 308 | 'CTCCoopSpirit' does not name a type |
| 5 | `00892f10` | `CGameScriptInterface` | 0 | 394 | expected '(' before '{' token |
| 6 | `00891540` | `CGameScriptInterface` | 0 | 446 | invalid use of incomplete type 'class CGameScriptInterface' |
| 7 | `00449b60` | `CPlayerManager` | 0 | 455 | 'CRGBColour' does not name a type |
| 8 | `008a9e30` | `CGameScriptInterface` | 0 | 489 | invalid use of incomplete type 'class CGameScriptInterface' |
| 9 | `008917f0` | `CGameScriptInterface` | 0 | 525 |  |
| 10 | `008914e0` | `CGameScriptInterface` | 0 | 526 |  |
| 11 | `00893030` | `CGameScriptInterface` | 0 | 553 | 'reinterpret_cast' from integer to pointer |
| 12 | `00c20ef0` | `_global` | 0 | 582 |  |
| 13 | `0088f910` | `CGameScriptInterface` | 0 | 586 |  |
| 14 | `0088f920` | `CGameScriptInterface` | 0 | 588 |  |
| 15 | `00892f40` | `CGameScriptInterface` | 0 | 593 | 'reinterpret_cast' from integer to pointer |

## First function queue

| Rank | Address | Module | Gap | Compiler blocker |
|---:|---|---|---|---|
| 1 | `00891610` | `CGameScriptInterface` | signature-audit | 'reinterpret_cast' from integer to pointer |
| 2 | `00891d50` | `CGameScriptInterface` | signature-audit | cannot call constructor 'NHeroInformationScreens::CBase::CBase' directly [-fpermissive] |
| 3 | `00892b50` | `CGameScriptInterface` | signature-audit | cannot call constructor 'NCameraModes::CScriptModuleBase::CScriptModuleBase' directly [-fpermissive] |
| 4 | `008961b0` | `CGameScriptInterface` | signature-audit | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 5 | `0089a490` | `CGameScriptInterface` | signature-audit | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 6 | `0089c510` | `CGameScriptInterface` | signature-audit |  |
| 7 | `0089faa0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 8 | `0089fbb0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 9 | `0089fc80` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 10 | `0089fd20` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 11 | `0089fda0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 12 | `008a9fc0` | `CGameScriptInterface` | compiled-needs-parity |  |
| 13 | `008aa480` | `CGameScriptInterface` | compiled-needs-parity |  |
| 14 | `006e7690` | `CGameScriptInterface` | candidate-compile |  |
| 15 | `006e7920` | `CGameScriptInterface` | candidate-compile |  |
| 16 | `006e79e0` | `CGameScriptInterface` | candidate-compile |  |
| 17 | `0088de10` | `CGameScriptInterface` | candidate-compile |  |
| 18 | `0088e0b0` | `CGameScriptInterface` | candidate-compile |  |
| 19 | `0088e130` | `CGameScriptInterface` | candidate-compile |  |
| 20 | `0088e200` | `CGameScriptInterface` | candidate-compile |  |
| 21 | `0088e2a0` | `CGameScriptInterface` | candidate-compile | static assertion failed |
| 22 | `0088e300` | `CGameScriptInterface` | candidate-compile |  |
| 23 | `0088e380` | `CGameScriptInterface` | candidate-compile |  |
| 24 | `0088e3d0` | `CGameScriptInterface` | candidate-compile | invalid use of incomplete type 'class CGameScriptInterface' |
| 25 | `0088e6f0` | `CGameScriptInterface` | candidate-compile |  |
| 26 | `0088ead0` | `CGameScriptInterface` | candidate-compile | invalid use of incomplete type 'class CGameScriptInterface' |
| 27 | `0088ece0` | `CGameScriptInterface` | candidate-compile |  |
| 28 | `0088f400` | `CGameScriptInterface` | candidate-compile |  |
| 29 | `0088f480` | `CGameScriptInterface` | candidate-compile |  |
| 30 | `0088f570` | `CGameScriptInterface` | candidate-compile |  |
| 31 | `0088f610` | `CGameScriptInterface` | candidate-compile |  |
| 32 | `0088f660` | `CGameScriptInterface` | candidate-compile |  |
| 33 | `0088f710` | `CGameScriptInterface` | candidate-compile |  |
| 34 | `0088f8d0` | `CGameScriptInterface` | candidate-compile |  |
| 35 | `0088f910` | `CGameScriptInterface` | candidate-compile |  |
| 36 | `0088f920` | `CGameScriptInterface` | candidate-compile |  |
| 37 | `0088f970` | `CGameScriptInterface` | candidate-compile |  |
| 38 | `0088f9b0` | `CGameScriptInterface` | candidate-compile |  |
| 39 | `0088fc90` | `CGameScriptInterface` | candidate-compile |  |
| 40 | `0088fd00` | `CGameScriptInterface` | candidate-compile |  |
