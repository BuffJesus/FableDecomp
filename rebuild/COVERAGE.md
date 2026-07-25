# Full-decompilation coverage dashboard

Generated: `2026-07-25T16:44:08-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,553 | 49,553 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,162 | 49,553 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,510 | 49,553 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,481 | 49,553 | 77.656% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,203 | 49,553 | 69.023% | Known CC, return, and parameter types |
| Auto-RE candidate source | 550 | 49,553 | 1.110% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 542 | 49,553 | 1.094% | Structural checker grade only |
| Auto-RE source integrity | 550 | 550 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 233 | 550 | 42.364% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 483 | 550 | 87.818% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 1,850 | 49,553 | 3.733% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 1,850 | 49,553 | 3.733% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 914 | 49,553 | 1.844% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 609 | 49,553 | 1.229% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 1,541 | 49,553 | 3.110% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 930 | 49,553 | 1.877% | Exact `.text` match |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,553 functions. The dashboard does not silently extrapolate the missing 471 measurements.

## Current gates

- 15,350 functions still lack a complete non-undefined prototype.
- 11,072 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- -1,300 auto-RE candidates still need a VC7.1-compatible translation unit.
- 67 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 327 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
