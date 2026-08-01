# Reconstruction backlog

Generated: `2026-07-31T18:18:13-06:00`

The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.

## Highest-value modules

| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |
|---:|---|---|---:|---:|---:|---:|
| 1 | candidate-cluster | `CGameScriptInterface` | 953 | 5 | 440 | 0 |
| 2 | candidate-cluster | `_global` | 11362 | 9365 | 278 | 0 |
| 3 | candidate-cluster | `CMouseDX` | 24 | 0 | 14 | 10 |
| 4 | candidate-cluster | `CScriptThing` | 103 | 2 | 15 | 0 |
| 5 | candidate-cluster | `CMemoryAllocatorFixedSize` | 21 | 0 | 11 | 7 |
| 6 | candidate-cluster | `CNavQuadTree` | 22 | 1 | 9 | 0 |
| 7 | candidate-cluster | `CEngineLandscapePatch` | 28 | 2 | 9 | 0 |
| 8 | candidate-cluster | `CEngineLandscapeRenderer` | 33 | 0 | 8 | 0 |
| 9 | candidate-cluster | `CMainGameComponent` | 64 | 1 | 6 | 0 |
| 10 | candidate-cluster | `CNetworkClient` | 6 | 0 | 5 | 0 |
| 11 | candidate-cluster | `CTCCoopSpirit` | 24 | 2 | 5 | 0 |
| 12 | candidate-cluster | `CWorldMap` | 186 | 3 | 5 | 0 |
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
| 1 | `00434dc0` | `_global` | 0 | 53 |  |
| 2 | `00434dd0` | `_global` | 0 | 60 |  |
| 3 | `00c9cb60` | `_global` | 0 | 93 |  |
| 4 | `0041c780` | `_global` | 0 | 123 |  |
| 5 | `0042a59e` | `_global` | 0 | 133 | 'RBTree__GetMaxNode' was not declared in this scope |
| 6 | `0042a5c0` | `_global` | 0 | 133 | 'RBTree__GetMaxNode' was not declared in this scope |
| 7 | `0042b687` | `_global` | 0 | 134 | 'LinkedList_Insert_Before' was not declared in this scope |
| 8 | `0042ac52` | `_global` | 0 | 135 |  |
| 9 | `0042ac9f` | `_global` | 0 | 135 |  |
| 10 | `0042ad10` | `_global` | 0 | 139 |  |
| 11 | `0043006f` | `_global` | 0 | 144 |  |
| 12 | `0042acec` | `_global` | 0 | 172 |  |
| 13 | `00430022` | `_global` | 0 | 183 |  |
| 14 | `0043f510` | `_global` | 0 | 194 |  |
| 15 | `0043f560` | `_global` | 0 | 218 |  |

## First function queue

| Rank | Address | Module | Gap | Compiler blocker |
|---:|---|---|---|---|
| 1 | `004b1670` | `CGameScriptInterface` | signature-audit | static assertion failed |
| 2 | `00681cd0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 3 | `006e7740` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 4 | `00896120` | `CGameScriptInterface` | signature-audit | field 'interfaces_44' has incomplete type 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >' |
| 5 | `008961b0` | `CGameScriptInterface` | signature-audit | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 6 | `0089c510` | `CGameScriptInterface` | signature-audit |  |
| 7 | `0089f300` | `CGameScriptInterface` | signature-audit | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 8 | `0089faa0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 9 | `0089fbb0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 10 | `0089fc80` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 11 | `0089fd20` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 12 | `0089fda0` | `CGameScriptInterface` | signature-audit | invalid use of incomplete type 'class CGameScriptInterface' |
| 13 | `008a9100` | `CGameScriptInterface` | signature-audit | 'EHeroMorphType' was not declared in this scope |
| 14 | `008ae890` | `CGameScriptInterface` | signature-audit | static assertion failed |
| 15 | `008aeac0` | `CGameScriptInterface` | signature-audit | static assertion failed |
| 16 | `006e7690` | `CGameScriptInterface` | candidate-compile |  |
| 17 | `006e7820` | `CGameScriptInterface` | candidate-compile |  |
| 18 | `006e7920` | `CGameScriptInterface` | candidate-compile |  |
| 19 | `006e79e0` | `CGameScriptInterface` | candidate-compile |  |
| 20 | `0088de10` | `CGameScriptInterface` | candidate-compile |  |
| 21 | `0088df60` | `CGameScriptInterface` | candidate-compile |  |
| 22 | `0088e0b0` | `CGameScriptInterface` | candidate-compile |  |
| 23 | `0088e130` | `CGameScriptInterface` | candidate-compile |  |
| 24 | `0088e200` | `CGameScriptInterface` | candidate-compile |  |
| 25 | `0088e2a0` | `CGameScriptInterface` | candidate-compile | static assertion failed |
| 26 | `0088e300` | `CGameScriptInterface` | candidate-compile |  |
| 27 | `0088e380` | `CGameScriptInterface` | candidate-compile |  |
| 28 | `0088e3d0` | `CGameScriptInterface` | candidate-compile | invalid use of incomplete type 'class CGameScriptInterface' |
| 29 | `0088e6f0` | `CGameScriptInterface` | candidate-compile |  |
| 30 | `0088ead0` | `CGameScriptInterface` | candidate-compile | invalid use of incomplete type 'class CGameScriptInterface' |
| 31 | `0088ece0` | `CGameScriptInterface` | candidate-compile |  |
| 32 | `0088f400` | `CGameScriptInterface` | candidate-compile |  |
| 33 | `0088f480` | `CGameScriptInterface` | candidate-compile |  |
| 34 | `0088f570` | `CGameScriptInterface` | candidate-compile |  |
| 35 | `0088f610` | `CGameScriptInterface` | candidate-compile |  |
| 36 | `0088f710` | `CGameScriptInterface` | candidate-compile |  |
| 37 | `0088f8d0` | `CGameScriptInterface` | candidate-compile |  |
| 38 | `0088f910` | `CGameScriptInterface` | candidate-compile |  |
| 39 | `0088f920` | `CGameScriptInterface` | candidate-compile |  |
| 40 | `0088f960` | `CGameScriptInterface` | candidate-compile | 'CGameScriptInterface' has not been declared |
