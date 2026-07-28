# Full-decompilation coverage dashboard

Generated: `2026-07-27T23:18:16-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,568 | 49,568 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,177 | 49,568 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,525 | 49,568 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,497 | 49,568 | 77.665% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,224 | 49,568 | 69.045% | Known CC, return, and parameter types |
| Auto-RE candidate source | 803 | 49,568 | 1.620% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 787 | 49,568 | 1.588% | Structural checker grade only |
| Auto-RE source integrity | 803 | 803 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 367 | 803 | 45.704% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 541 | 803 | 67.372% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 5,051 | 49,568 | 10.190% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 5,051 | 49,568 | 10.190% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 2,689 | 49,568 | 5.425% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 2,029 | 49,568 | 4.093% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 4,736 | 49,568 | 9.555% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 2,705 | 49,568 | 5.457% | Exact `.text` match |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,553 functions. The dashboard does not silently extrapolate the missing 471 measurements.

## Current gates

- 15,344 functions still lack a complete non-undefined prototype.
- 11,071 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 67 of 803 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 262 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 333 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
