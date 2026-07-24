# Auto-RE candidate compile gate

Generated: `2026-07-24T00:44:00-06:00`

| Gate | Passing | Total |
|---|---:|---:|
| Source integrity | 509 | 509 |
| 32-bit host C++20 syntax | 226 | 509 |
| No detected VC7.1 language blockers | 49 | 509 |
| Ready for VC7.1 module compilation | 34 | 509 |

A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.

## Most common missing declarations

| Declaration | Candidates |
|---|---:|
| `offsetof` | 44 |
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
| `CANavQuadTreeNode` | 6 |
| `CEngineLandscapePatch` | 6 |
| `CWorldMap` | 5 |
| `std` | 5 |
| `CEngineLandscapeRenderer` | 5 |
| `LS_LOADING_STATUS` | 5 |
| `CDataInputStream` | 5 |
| `C2DCoordI` | 4 |
| `GetMapNumberAt` | 4 |
| `localCoord` | 4 |
| `CMap` | 4 |
| `C2DLineF` | 4 |
| `::operator_new` | 4 |
| `CFrameData` | 4 |
| `CResource` | 4 |
| `CVertexShader` | 4 |
| `pFrameData` | 4 |
| `pResource` | 4 |
| `CLandscapeLayerMesh` | 4 |

Full per-candidate results are in `candidates.tsv`.
