# Auto-RE candidate compile gate

Generated: `2026-09-08T11:21:24-06:00`

| Gate | Passing | Total |
|---|---:|---:|
| Source integrity | 1066 | 1066 |
| 32-bit host C++20 syntax | 575 | 1066 |
| No detected VC7.1 language blockers | 71 | 1066 |
| Ready for VC7.1 module compilation | 43 | 1066 |

A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.

## Most common missing declarations

| Declaration | Candidates |
|---|---:|
| `offsetof` | 61 |
| `class CGameScriptInterface` | 41 |
| `CCharString` | 32 |
| `CGameScriptInterface` | 24 |
| `mov` | 15 |
| `self` | 13 |
| `CTCBase` | 13 |
| `CDataOutputStream` | 13 |
| `std` | 12 |
| `CKeyPairCompareLess` | 12 |
| `ETCInterfaceType` | 12 |
| `C2DVector` | 12 |
| `stream` | 12 |
| `CMouseDX` | 12 |
| `CMemoryAllocatorFixedSize` | 11 |
| `CScriptThing` | 10 |
| `CWideString` | 9 |
| `CNavQuadTree` | 9 |
| `C3DVector` | 8 |
| `EThemePaintType` | 8 |
| `CDefinitionManager` | 7 |
| `CVectorMap` | 7 |
| `class CScriptThing` | 7 |
| `CEngineLandscapeMap` | 7 |
| `CGraphicInfoFrame` | 7 |
| `CGameEvent` | 6 |
| `next` | 6 |
| `CDataInputStream` | 6 |
| `CTCCoopSpirit` | 6 |
| `CQuestManager` | 6 |

Full per-candidate results are in `candidates.tsv`.
