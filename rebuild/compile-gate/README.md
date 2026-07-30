# Auto-RE candidate compile gate

Generated: `2026-07-30T14:42:47-06:00`

| Gate | Passing | Total |
|---|---:|---:|
| Source integrity | 845 | 845 |
| 32-bit host C++20 syntax | 397 | 845 |
| No detected VC7.1 language blockers | 68 | 845 |
| Ready for VC7.1 module compilation | 41 | 845 |

A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.

## Most common missing declarations

| Declaration | Candidates |
|---|---:|
| `offsetof` | 61 |
| `class CGameScriptInterface` | 40 |
| `CCharString` | 29 |
| `CGameScriptInterface` | 23 |
| `mov` | 15 |
| `std` | 13 |
| `CTCBase` | 13 |
| `C2DVector` | 13 |
| `ETCInterfaceType` | 12 |
| `CMouseDX` | 12 |
| `self` | 11 |
| `CKeyPairCompareLess` | 11 |
| `CMemoryAllocatorFixedSize` | 11 |
| `CNavQuadTree` | 10 |
| `CWideString` | 9 |
| `CScriptThing` | 9 |
| `CDefinitionManager` | 8 |
| `EThemePaintType` | 8 |
| `CVectorMap` | 7 |
| `class CScriptThing` | 7 |
| `C3DVector` | 7 |
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
