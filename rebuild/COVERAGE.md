# Full-decompilation coverage dashboard

Generated: `2026-07-26T16:01:58-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,552 | 49,552 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,161 | 49,552 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,510 | 49,552 | 99.915% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,491 | 49,552 | 77.678% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,218 | 49,552 | 69.055% | Known CC, return, and parameter types |
| Auto-RE candidate source | 632 | 49,552 | 1.275% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 619 | 49,552 | 1.249% | Structural checker grade only |
| Auto-RE source integrity | 573 | 573 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 251 | 573 | 43.805% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 506 | 573 | 88.307% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 4,917 | 49,552 | 9.923% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 4,917 | 49,552 | 9.923% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 2,687 | 49,552 | 5.423% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 1,899 | 49,552 | 3.832% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 4,604 | 49,552 | 9.291% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 2,703 | 49,552 | 5.455% | Exact `.text` match |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,553 functions. The dashboard does not silently extrapolate the missing 471 measurements.

## Current gates

- 15,334 functions still lack a complete non-undefined prototype.
- 11,061 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 57 of 573 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 67 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 331 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
