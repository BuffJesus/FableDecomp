# Full-decompilation coverage dashboard

Generated: `2026-08-05T11:04:37-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,568 | 49,568 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,177 | 49,568 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,525 | 49,568 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,503 | 49,568 | 77.677% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,234 | 49,568 | 69.065% | Known CC, return, and parameter types |
| Auto-RE candidate source | 888 | 49,568 | 1.791% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 875 | 49,568 | 1.765% | Structural checker grade only |
| Auto-RE source integrity | 888 | 888 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 435 | 888 | 48.986% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 541 | 888 | 60.923% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 5,798 | 49,568 | 11.697% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 5,798 | 49,568 | 11.697% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 3,096 | 49,568 | 6.246% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 2,532 | 49,568 | 5.108% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 5,646 | 49,568 | 11.390% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 3,112 | 49,568 | 6.278% | Exact `.text` match |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,568 functions. The dashboard does not silently extrapolate the missing 486 measurements.

## Current gates

- 15,334 functions still lack a complete non-undefined prototype.
- 11,065 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 69 of 888 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 347 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 170 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
