# The VC7.1 QFE-4035 codegen gate (and how to close it)

> ## ⚠ CORRECTION (2026-08-16): 13.10.4035 codegen == 13.10.3077. The "gate" is a misdiagnosis.
> We acquired the real 13.10.4035 compiler (WinDDK 3790.1830, verified `FileVersion 13.10.4035`)
> and A/B-tested it against our RTM 3077 with `tools/permuter/qfe_codegen_diff.py`:
> **128/128 functions produced byte-identical `.text`** (120 random real landed functions +
> 8 synthetic probes including the x87 `fmul [mem]`-fold shapes the gate was blamed on). The
> QFE 3077→4035 hotfix was a **non-codegen fix** (crash/ICE/security), not a codegen change —
> so the ~11 objects stamped 4035 in retail's Rich header are **not** unmatchable *because* of
> the compiler build (correlation, not causation). The `--qfe` harness works and is retained
> (harmless), but swapping to 4035 buys nothing over 3077.
>
> **Redirected conclusion:** the truly-unmatchable functions differ by **per-TU compiler FLAGS**
> or **source structure**, not compiler version. Both are attackable with `tools/permuter/anneal.py`
> (source/flag search) — this is the productive lane, not chasing a compiler. The selfcheck trio
> (IsActive/SetAsActive/Init) was *never* 4035-gated: 3077 and 4035 both emit their 42/41/47-byte
> forms; retail's 37/36/47 forms are a source/flag puzzle. Everything below is retained as the
> evidence trail that led here; read it as history, not current strategy.

> ## Case study: ApplyScriptBrush @ 0x0088f480 — the "gate" is a register-alloc coin-flip
> Reconstructed one of the canonical "4035-gated" functions (memory named it) to see what the
> gap actually is. Semantics are exact (25v25 bytes, behaviour-correct); the reloc-masked
> diff is **4 bytes** — a pure `eax↔edx` swap: retail loads `name→eax, vptr→edx` and does
> `call [edx+0x30]`; VC71 picks `name→edx, vptr→eax` / `call [eax+0x30]`. Same instructions,
> same order, opposite register assignment. We threw everything at it:
> - **1680 flag combos** (opt levels × `/G5/G6/G7/GB` CPU targets × `/Op/Oa/Ow/GS/Gy/GF` × 6 pragmas) → best still 4.
> - **4 hand structural variants** (chained call, temp-sub, ptr-name) → all 4.
> - **anneal**, 343 mutation compiles (temp-intro/inline/reorder/block/retype) → still 4.
>
> So this function is **not** compiler-version-gated (4035≡3077), **not** flag-gated, and
> **not** reachable by any source spelling we can express — it's a fixed allocator tiebreak,
> the same class as `examples/stdmovebackward` (score 4). The retail bytes were produced by
> 3077/4035 from *some* source, but the allocation is a coin-flip our reconstruction can't
> land on. Cracking these needs either the exact original source or a **register-level**
> permuter (assign specific SSA values to specific registers) — a lever beyond source
> mutation. Fixture kept at `examples/qfe/applyscriptbrush.cpp` as a regalloc dead-end test.
>
> **Bottom line for the whole thread:** the ~11 "unmatchable" functions are register-allocation
> / scheduling coin-flips, not a compiler-version problem. Neither a different compiler nor
> flags nor source search closes them; only exact-source fidelity or a register-level permuter would.
>
> ### Does this risk scale to substantial functions? No — it's inversely correlated with size.
> Measured on the current landed corpus: **113 non-clone real-logic functions >256 bytes are
> byte-exact** (InitialiseConsoleVariables 4160B, ScrollDown 3664B, CBankFileManager::OpenRetailBank
> 1568B, the Action input handler 1723B, many 1-2.5KB SuspendableProcess). Coin-flips need allocator
> *slack* — ApplyScriptBrush (25B) has 2 live values and 4 free registers, so the choice is free and
> our source can't bias it. Large functions have high register pressure + dense data deps that
> *force* the allocation, so matching source reproduces matching registers. The 25-byte leaf is the
> worst case, not a preview of big-function risk.
>
> **If a substantial function ever IS blocked** (data-driven trigger: a >256B function whose only
> residual is a same-length regalloc DIFFER that survives anneal), the escalation ladder is:
> (1) anneal source search [have it]; (2) build a register-level permuter against that concrete
> target [justified only when one appears — don't build speculatively]; (3) last resort per
> faithful-decomp-policy, an inline-asm island for *only* the divergent basic block, not the whole
> function (keeps the surrounding C++ genuine). Until such a function appears, this stays a
> tiny-leaf-only defer.

---

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
