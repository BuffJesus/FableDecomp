# The VC7.1 QFE-4035 codegen gate (and how to close it)

Some behaviour-correct, fully-reversed functions cannot be made byte-exact with our
compiler no matter the source spelling or flags. This documents *why* (hard evidence),
*which* functions, and the *workaround*.

## Evidence: retail Fable.exe was built with two compiler backends
`tools/decomp_pipeline/rich_header.py` parses the PE Rich header (link.exe's per-object
build fingerprint). On retail Fable.exe:

| prodID | tool                    | build       | objects |
|--------|-------------------------|-------------|---------|
| 0x60   | Utc1310_CPP             | 3077 (RTM)  | 1369    |
| 0x5f   | Utc1310_CPP             | 3077 (RTM)  | 51      |
| 0x0f   | Linker                  | 3077 (RTM)  | 12      |
| **0x5f** | **Utc1310_CPP**       | **4035 (QFE)** | **4**  |
| **0x60** | **Utc1310_CPP (QFE)** | **4035 (QFE)** | **1**  |
| **0x5d** | **Utc1310_C (QFE)**   | **4035 (QFE)** | **6**  |
| …      | (older 2067/2179/3052 from linked static libs) | | |

**11 objects were compiled with build 13.10.4035**; everything else with **13.10.3077
(RTM)**. Our toolchain (`D:\Tools\vc71`, `cl.exe` banner = 13.10.3077) is the RTM build,
so it reproduces the 3077 objects byte-exact but *cannot* reproduce the codegen of the 11
QFE objects. This matches memory `retail-compiler-build` ("+QFE 4035 for ~11 objs").

## Proof the gate is real (not a source/flag problem)
`tools/permuter/anneal.py` (full-regalloc SA) + an exhaustive flag×pragma×source-structure
sweep were run on the three known same-length/idiom deferrals. None reach parity:

- **`CDrawMiniMap::IsActive` @ 0x00643e09** — retail shares ONE `xor eax,eax` and reuses
  `al`=0 as the operand of every `cmp byte[mem], al` AND as the false return value, with a
  single shared false-exit (`je`) and `jbe`. RTM 3077 emits `mov al,[mem]; test`/`setne`/
  local returns instead — under *every* flag (`/O1 /Os /Ox /Ot /Og`), pragma
  (`s/t/g/a/y/gs`), and source restructure (multi-return, single-exit `&&`, default-false).
  Closest: 38 bytes vs 37. The "`cmp mem, zeroed-reg`" size idiom is 4035-only.
- **`CDrawMiniMap::SetAsActive` @ 0x00643e2e** — same class (length-gated, 41→best 40 vs 36).
- **`CNewFrontendGameComponent::Init` @ 0x0042f75e** — same *length* (47v47), score 19:
  RTM tail-jmps the final void member call (`pop esi; jmp`) and load-hoists a `push [esi+10]`
  arg; QFE keeps `mov ecx,eax; call; pop esi; ret` and pushes the memory operand directly.

The annealer bottoms out honestly on all three (no false-claim) — exactly the QFE-4035
class. Do not keep re-attacking these with RTM; the ceiling is the compiler build.

## Workaround: acquire the 13.10.4035 backend
Build **13.10.4035** shipped in the **Windows Server 2003 SP1 DDK (build 3790.1830)** — the
free driver kit carried a full VC7.1 C/C++ compiler at that QFE level (the plain 3790 no-SP
DDK is 13.10.2190; the *SP1* 3790.1830 DDK is 4035). Community mirrors:
`github.com/9176324/WinDDK` (tree `3790.1830/bin`) and the archive.org Server 2003 SP1 ISO.

**The harness is already wired (2026-08-16).** The `--qfe` switch exists end-to-end; it just
needs the binaries dropped in place:
- `tools/permuter/permuter_score.py --qfe`, `tools/permuter/anneal.py --qfe`,
  `tools/decomp_pipeline/verify_and_land.py --qfe` — all route the *parity* compile to
  `$VC71_QFE\bin\cl.exe` (default `D:\Tools\vc71-qfe4035\bin`), keeping RTM INCLUDE/LIB.
  They fail loudly if the toolset is absent (no silent RTM fallback).
- `verify_and_land.py --qfe` stamps each landed entry `Compiler = 'qfe4035'`; the build
  driver `rebuild/build_candidates.ps1` honors that tag and compiles those TUs with
  `$vcQfeRoot` (`$env:VC71_QFE`) instead of `$vcRoot`. So a QFE win reproduces on the real build.

Remaining steps (need the binaries):
1. Obtain `cl.exe` + `c1xx.dll` (C++ frontend) + `c2.dll` (codegen backend) from WinDDK
   3790.1830's x86 `bin` tree. Rich shows the QFE objects used the 4035 **frontend** (prodID
   0x5d/0x5f/0x60) *and* backend, so take BOTH c1xx.dll and c2.dll.
2. Drop them in `D:\Tools\vc71-qfe4035\bin` (or set `$env:VC71_QFE`); verify
   `(Get-Item …\c2.dll).VersionInfo.FileVersion` == `13.10.4035.0`.
3. Confirm the loop: `python tools/permuter/permuter_score.py 00643e09 <isactive.cpp>
   --name IsActive --qfe` → expect score 0 (the exact 37-byte match `33c03881eb…40c3`).
   Then re-author/verify the deferred frontend targets under `--qfe` and land them.

## Fallback if 4035 cannot be sourced
For the confirmed-gated functions ONLY, a documented `__declspec(naked)` byte-stub is
policy-defensible: `faithful-decomp-policy` permits naked when a function is *proven
irreducible* under the available toolchain, and the Rich-header fingerprint + exhaustive
anneal/flag/source sweep IS that proof. Mark each such stub
`// QFE-4035 codegen, irreducible under RTM 3077 — see docs/QFE4035_COMPILER_GATE.md`.
Prefer sourcing the compiler; the stub is a last resort and must never be used for a
function that merely *hasn't been reversed yet*.
