# Full-decompilation coverage dashboard

Generated: `2026-08-16T14:26:17-06:00`

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,568 | 49,568 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,177 | 49,568 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,525 | 49,568 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,503 | 49,568 | 77.677% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,234 | 49,568 | 69.065% | Known CC, return, and parameter types |
| Auto-RE candidate source | 894 | 49,568 | 1.804% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 882 | 49,568 | 1.779% | Structural checker grade only |
| Auto-RE source integrity | 894 | 894 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 440 | 894 | 49.217% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 541 | 894 | 60.515% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 11,108 | 49,568 | 22.410% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 11,108 | 49,568 | 22.410% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 4,419 | 49,568 | 8.915% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 6,505 | 49,568 | 13.123% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 10,942 | 49,568 | 22.075% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 4,435 | 49,568 | 8.947% | Exact `.text` match |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,568 functions. The dashboard does not silently extrapolate the missing 486 measurements.

## Current gates

- 15,334 functions still lack a complete non-undefined prototype.
- 11,065 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 69 of 894 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 353 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 184 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
