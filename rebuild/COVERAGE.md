# Full-decompilation coverage dashboard

Generated: `2026-07-30T18:49:04-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,568 | 49,568 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,177 | 49,568 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,525 | 49,568 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,503 | 49,568 | 77.677% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,234 | 49,568 | 69.065% | Known CC, return, and parameter types |
| Auto-RE candidate source | 846 | 49,568 | 1.707% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 832 | 49,568 | 1.679% | Structural checker grade only |
| Auto-RE source integrity | 846 | 846 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 398 | 846 | 47.045% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 541 | 846 | 63.948% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 5,548 | 49,568 | 11.193% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 5,548 | 49,568 | 11.193% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 3,036 | 49,568 | 6.125% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 2,343 | 49,568 | 4.727% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 5,397 | 49,568 | 10.888% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 3,052 | 49,568 | 6.157% | Exact `.text` match |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,553 functions. The dashboard does not silently extrapolate the missing 471 measurements.

## Current gates

- 15,334 functions still lack a complete non-undefined prototype.
- 11,065 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 68 of 846 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 305 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 169 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
