# Auto-RE candidate compile gate

Generated: `2026-08-01T23:19:42-06:00`

| Gate | Passing | Total |
|---|---:|---:|
| Source integrity | 850 | 850 |
| 32-bit host C++20 syntax | 401 | 850 |
| No detected VC7.1 language blockers | 69 | 850 |
| Ready for VC7.1 module compilation | 41 | 850 |

A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.

## Most common missing declarations

| Declaration | Candidates |
|---|---:|
| `offsetof` | 61 |
| `class CGameScriptInterface` | 40 |
| `CCharString` | 30 |
| `CGameScriptInterface` | 23 |
| `mov` | 15 |
| `std` | 13 |
| `CTCBase` | 13 |
| `C2DVector` | 13 |
| `CKeyPairCompareLess` | 12 |
| `ETCInterfaceType` | 12 |
| `CMouseDX` | 12 |
| `self` | 11 |
| `CMemoryAllocatorFixedSize` | 11 |
| `CScriptThing` | 10 |
| `CNavQuadTree` | 10 |
| `CWideString` | 9 |
| `CDefinitionManager` | 8 |
| `C3DVector` | 8 |
| `EThemePaintType` | 8 |
| `CVectorMap` | 7 |
| `class CScriptThing` | 7 |
| `CEngineLandscapeMap` | 7 |
| `CGraphicInfoFrame` | 7 |
| `CGameEvent` | 6 |
| `next` | 6 |
| `CTCCoopSpirit` | 6 |
| `CQuestManager` | 6 |
| `CANavQuadTreeNode` | 6 |
| `CEngineLandscapePatch` | 6 |
| `CMemoryAllocatorVariableSize` | 5 |

Full per-candidate results are in `candidates.tsv`.
