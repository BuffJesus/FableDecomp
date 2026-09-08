# Full-decompilation coverage dashboard

Generated: `2026-09-07T19:25:27-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,568 | 49,568 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,177 | 49,568 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,525 | 49,568 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,537 | 49,568 | 77.746% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,288 | 49,568 | 69.174% | Known CC, return, and parameter types |
| Auto-RE candidate source | 1,066 | 49,568 | 2.151% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 1,053 | 49,568 | 2.124% | Structural checker grade only |
| Auto-RE source integrity | 1,066 | 1,066 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 575 | 1,066 | 53.940% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 542 | 1,066 | 50.844% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 18,604 | 49,568 | 37.532% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 18,604 | 49,568 | 37.532% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 8,066 | 49,568 | 16.273% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 10,495 | 49,568 | 21.173% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 18,579 | 49,568 | 37.482% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 8,082 | 49,568 | 16.305% | Exact `.text` match (includes asm bakes below) |
| Byte-identical **genuine** C++ | 7,790 | 49,568 | 15.716% | Exact match from real compiler input; no `__asm`, `naked`, `_emit` |
| Hand-written asm bakes (grade `asm_bake`) | 292 | 600 | — | Byte-exact oracles, **not** reconstruction; de-bake worklist |
| Landed sources typed onto shared engine headers | 573 | 14,431 | 3.971% | `#include "engine/<Class>.h"` with PDB member names (962 headers) |
| Generic local struct declarations remaining | 4,427 | — | — | `struct T/Sub/Owner...` throwaways in 3,288 files; retype worklist |
| Matched retail bytes (genuine C++) | 385,690 | — | — | Bytes of retail `.text` reproduced by genuine landings |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,568 functions. The dashboard does not silently extrapolate the missing 486 measurements.

## Current gates

- 15,280 functions still lack a complete non-undefined prototype.
- 11,031 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 71 of 1,066 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 524 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 43 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
