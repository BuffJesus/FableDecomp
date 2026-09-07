# Auto-RE candidate compile gate

Generated: `2026-08-21T19:00:48-06:00`

| Gate | Passing | Total |
|---|---:|---:|
| Source integrity | 12 | 12 |
| 32-bit host C++20 syntax | 8 | 12 |
| No detected VC7.1 language blockers | 0 | 12 |
| Ready for VC7.1 module compilation | 0 | 12 |

A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.

## Most common missing declarations

| Declaration | Candidates |
|---|---:|
| `CRGBColour` | 2 |
| `CThingBuildingDef` | 1 |
| `CDataOutputStream` | 1 |
| `CMemoryDataInputStream` | 1 |
| `CPersistContext` | 1 |
| `CPhysicalPrimitiveInit` | 1 |
| `CActiveFile` | 1 |

Full per-candidate results are in `wave3-0045-new.tsv`.
