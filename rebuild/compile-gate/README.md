# Auto-RE candidate compile gate

Generated: `2026-07-25T15:26:08-06:00`

| Gate | Passing | Total |
|---|---:|---:|
| Source integrity | 534 | 534 |
| 32-bit host C++20 syntax | 227 | 534 |
| No detected VC7.1 language blockers | 53 | 534 |
| Ready for VC7.1 module compilation | 34 | 534 |

A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.

## Most common missing declarations

| Declaration | Candidates |
|---|---:|
| `offsetof` | 46 |
| `class CGameScriptInterface` | 33 |
| `mov` | 14 |
| `CMouseDX` | 12 |
| `CMemoryAllocatorFixedSize` | 11 |
| `C2DVector` | 11 |
| `CNavQuadTree` | 10 |
| `EThemePaintType` | 8 |
| `class CScriptThing` | 7 |
| `CEngineLandscapeMap` | 7 |
| `CGraphicInfoFrame` | 7 |
| `CGameEvent` | 6 |
| `CTCCoopSpirit` | 6 |
| `std` | 6 |
| `CANavQuadTreeNode` | 6 |
| `CEngineLandscapePatch` | 6 |
| `CNetworkClient` | 5 |
| `CGameEventPackageSet` | 5 |
| `CWorldMap` | 5 |
| `CEngineLandscapeRenderer` | 5 |
| `LS_LOADING_STATUS` | 5 |
| `CDataInputStream` | 5 |
| `CMainGameComponent` | 4 |
| `C2DCoordI` | 4 |
| `GetMapNumberAt` | 4 |
| `localCoord` | 4 |
| `CMap` | 4 |
| `C2DLineF` | 4 |
| `::operator_new` | 4 |
| `CFrameData` | 4 |

Full per-candidate results are in `candidates.tsv`.
