# Integrity audit — exact-decomp honesty pass (2026-07-31)

## Current reconciliation addendum (2026-08-02)

The historical audit below is retained as evidence and is not silently
rewritten. Current canonical metrics are 5,588 compiled/behavior-gated
sources, 3,033 exact plus 2,389 relocation matches, 38 `DIFFER`, and 128
`ORACLE_MISSING`; the whole-project verified functional/matching total is
5,440/49,568, with 3,049 byte-identical functions. The focused frontend
Python suite is 35/35.

Frontend progress remains explicitly bounded: wheel and arrow input use the
recovered retail action/event IDs and generic manager dispatch ordering at a
callback boundary; delete confirmation is composed from retail
`frontend.bin`/`frontend.big`/`text.big`/`fonts.big` data; and new-profile text
uses the recovered 128-byte/127-character/low-byte/backspace contract. The
visual adapter does not claim native manager/list ownership and does not write
profiles or delete data. The full bootstrap is still blocked by the strict
`CThreadedFile::Open @ 0x0098E1E0` non-relocation-byte mismatch. No parity gate,
oracle, or asset-purity rule was weakened.

Read-only fan-out audit + adversarial verification across 7 dimensions, triggered by
"do a pass over the rest to ensure we didn't hack our way through." 37 candidate findings,
**32 confirmed/plausible, 5 refuted**. This doc is the durable ledger + live remediation state.

## Verdict

**Broadly honest headline, shortcuts one grade down.** The north-star number is trustworthy:
`Byte-identical C++` (3,052 = 16 lift_matching + 3,036 candidate MATCH) is strictly an exact
`.text` match, held separate from behavior-tested (5,548). `retail-parity.tsv` reconciles cleanly
(3,036 MATCH + 2,343 RELOCATION_MATCH + 128 ORACLE_MISSING + 41 DIFFER = 5,548; the 128+41 are
correctly excluded from the 5,397 "verified"). Nothing fake is counted as byte-identical. The
"hacking through" is concentrated in (a) behavioral slices sitting in the `compiled/` tree that
implies decomp, (b) two dashboard accounting defects, (c) the frontend `PIXEL_IDENTICAL` metric
meaning self-consistency with an authored bake, not retail pixels.

## Corrections to the audit itself (honesty about the audit)

- **Finding #2 (167 NO_ORACLE folded into "checked against retail") — FALSE POSITIVE.** Verified
  against `status.json` + `write_decomp_dashboard.py`: `verified`=5,397 = 3,036 MATCH + 2,343 RELOC
  + 18 lift, all genuinely matched. The 167 `NO_ORACLE` exist only in the *stale* `parity_audit.tsv`,
  which the dashboard generator never reads. No count inflation in the headline. Not remediated
  because there is no defect.

- **Findings #5, #13, #17, #18, #19 (RELOC "abi-stub-shortcut" slices) — OVER-FLAGGED.** Verified
  2026-07-31 against the regenerated `parity_audit.tsv` + reading the files. `LaunchVirtualKeyboard`
  (00596917), `AddSuccessfulBlockParry` (006585ee), `CreatureNav::IsCurrentlyNavigating` (006cb8f0),
  `PhysicsStandard::ResetInterpolation` (007236e0), `CIME::FinaliseString` (00404b80) all grade
  **RELOC** — a *legitimate* relocation-masked byte match. A RELOC pass masks only the 4-byte call
  **targets**; every other byte equals retail, so retail genuinely CALLS those helpers at those exact
  offsets (inlined logic cannot RELOC-match a `call`). These are thin wrappers over real engine calls
  — which is what retail is — NOT hidden logic. AddSuccessfulBlockParry's helpers ARE declared
  (`extern "C" __cdecl`), contradicting the "undeclared / work not present" claim. **Real residual
  gap = placeholder callee NAMES (semantic), not byte fidelity.** Downgrade these to low (naming gap).

## Backlog-wide over-capture recovery (2026-07-31, ultracode fan-out + full re-audit)

The 6-slice re-bound technique was scaled across the whole `DIFFER` backlog. Triage of
`parity_audit.tsv` showed **179 of 208 DIFFER were over-capture** (built length N > retail M =
fused adjacent code), only 24 under-capture (N<M, genuinely incomplete), 5 same-length. A
self-checking workflow (181 agents) re-bounded each over-capture to its real function, keeping only
tool-verified EXACT/RELOC and `git checkout`-reverting the rest. **Authoritative full re-audit
before → after:**

| Verdict | Before | After | Δ |
|---|---:|---:|---:|
| EXACT | 2,899 | 3,034 | +135 |
| RELOC | 2,313 | 2,348 | +35 |
| DIFFER | 208 | 38 | −170 |
| NO_ORACLE | 226 | 226 | 0 |

170 recovered (95%); byte-level matches 5,212 → 5,382 of 5,646 landed (95.3%); remaining DIFFER 38
(0.7% of graded). Safety verified: shared `build_and_diff.ps1` untouched, all 9 reverts git-clean,
sampled keeps independently EXACT. **The DIFFER backlog was ~95% mis-bounding (a systemic naked-asm
"fuse-to-force" shortcut), now excised.** Remaining 38 DIFFER = 24 under-capture (incomplete lifts,
real decomp work — task #10) + 9 residual: 8 genuine MSVC7.1 codegen divergence + 1 oracle-boundary
defect (`004012ce` oracle truncated mid-instruction, task #9). None faked.

## NO_ORACLE grading + oracle-integrity (2026-07-31, cont.)

Two more integrity passes after the over-capture recovery:
- **Oracle-integrity sweep** (capstone, all 5,420 oracles): **99.8% clean** (fully decode + terminator
  end). 10 mis-bounded (all truncated mid-instruction) found + fixed via Ghidra re-capture; recovered
  `0098b5e0`, and caught 3 candidates the mass re-bound had shortened to match truncated oracles (restored
  to EXACT). The parity ground truth is sound.
- **NO_ORACLE grading**: batch-captured 236 addresses (107 yielded functions; 128 "NO FUNC" = candidate
  address is not a Ghidra function start — task #13). Added 97 oracles and graded: **83/97 (86%) are
  genuine byte matches** (74 RELOC + 9 EXACT), 13 DIFFER. That population was mostly fine — just missing
  from oracle intake.

**Session-long parity trajectory (verified via full re-audits):**

| | start | end | Δ |
|---|---:|---:|---:|
| EXACT | 2,899 | 3,040 | +141 |
| RELOC | 2,313 | 2,422 | +109 |
| DIFFER | 208 | 54 | −154 |
| NO_ORACLE | 226 | 130 | −96 |

Byte-level matches **5,212 → 5,462 of 5,646 landed (92.3% → 96.7%; 99.0% of graded)**. Remaining: 54
DIFFER (genuine incomplete/codegen residue, task #10) + 130 NO_ORACLE (mostly non-function addresses,
task #13). Every gain verified by the parity tool; the oracles it grades against are now swept + corrected.

## Slice-family re-grade (verified against regenerated parity_audit.tsv, 2026-07-31)

| Address | File | Fresh verdict | Honest grade |
|---|---|---|---|
| 00596917 | LaunchVirtualKeyboard | RELOC | ✅ legit byte-match; callee names are placeholders |
| 006585ee | AddSuccessfulBlockParry | RELOC | ✅ legit byte-match; helpers declared; names placeholder |
| 006cb8f0 | CreatureNav::IsCurrentlyNavigating | RELOC | ✅ legit byte-match; names placeholder |
| 007236e0 | PhysicsStandard::ResetInterpolation | RELOC | ✅ legit byte-match; names placeholder |
| 00404b80 | CIME::FinaliseString | RELOC | ✅ legit byte-match; names placeholder |
| 0059b777 | CTCInventoryTrade::ProcessButtonAReleased | DIFFER(35v25) | ⚠ genuine slice — candidate LONGER than retail = boundary over-capture (fused trailing thunks) |
| 0044be30 | CLandscapeBackgroundPatch dtor | DIFFER(73v30) | ⚠ over-capture (2 fused COMDATs) |
| 0044f90f | CLandscapeBackgroundPatch dtor | DIFFER(56v34) | ⚠ over-capture |
| 0044fe5f | CLandscapeBackgroundPatch dtor | DIFFER(70v34) | ⚠ over-capture |
| 0044c2e2 | CHeroMarriageDef::GetSizeofClass | DIFFER(31v6) | ⚠ over-capture (GetSizeofClass fused w/ factory thunk) |
| 0044c1f6 | CCarrySlotDef::GetSizeofClass | DIFFER(29v4) | ⚠ over-capture (29B blob for a ~4B fn) |
| 00596cce | RefreshAvailableProfiles | NO_ORACLE | ⚠ ungraded 824B inlined-construction slice; needs oracle |

**Corrected picture:** of the 12, **5 are legitimate byte matches** (RELOC; only naming gaps), **6 are
genuine DIFFER slices whose common root cause is boundary OVER-CAPTURE** (candidate spans MORE bytes
than the real retail function — fused adjacent thunks/COMDATs; cf. memory `pe-oracle-boundary-overcapture`),
and **1 is NO_ORACLE**. The real remediation for the 6 is RE-BOUNDING the function extent (disassemble,
find the true `ret`, drop the trailing fused bytes), NOT decomposing more logic. This is materially
different from the audit's "12 dangerous hidden-logic slices" framing.

**REMEDIATION APPLIED 2026-07-31 — ALL 6 DIFFER slices re-bounded to genuine byte matches:**
- `0044c1f6` GetSizeofClass DIFFER(29v4) → **EXACT** (faithful `return 0x38`, flag `/Os`).
- `0044c2e2` GetSizeofClass DIFFER(31v6) → **EXACT** (`return 0xC8`, flag `/Oy`).
- `0044be30` / `0044f90f` / `0044fe5f` Landscape vec-del-dtors DIFFER(73v30/56v34/70v34) → **RELOC**
  (dropped the appended padding + fused adjacent function; kept only the real 30/34-byte dtor).
- `0059b777` InventoryTrade::ProcessButtonAReleased DIFFER(35v25) → **RELOC** (dropped the two
  appended jmp thunks; kept the real 25-byte tail-calling dispatcher).
Each prior candidate was a `__declspec(naked)` blob that fused 2–3 functions to force a byte region to
line up — the "hack our way through" pattern — now replaced by the correctly-bounded real function.
Added an `/Os` size-opt variant to `catalog_parity_audit.py` (vtable GetSizeofClass thunks).

`00596cce` RefreshAvailableProfiles (NO_ORACLE): honestly **graded** as a behavior-slice in its header
(824-byte inlined construction behind 11 `extern` boundaries → cannot byte-match as-is; not counted as
byte-identical). Byte-match left as documented future work (decompose the construction; same `/Oy-`(/G7) TU).

**Final slice-family state (verified via parity tool):** 11/12 are byte-level matches (2 EXACT + 9 RELOC),
1 is an honestly-graded behavior-slice. Only low-priority residual: resolve placeholder callee NAMES on
the 5 originally-legit RELOC files (semantic, not byte fidelity).

## Remediation ledger

| # | Sev | File | Defect | Status |
|---|---|---|---|---|
| 1 | crit | `rebuild/src/compiled/00/59/CFrontEndManager_Action_0059a238.cpp` | Address-isolated 1,486-byte slice w/ 15 extern stubs; `NO_ORACLE`, in decomp tree | **oracle captured** (`...expected.tsv`, 1486B verified) + full decomp reference + header **re-graded** "behavior-slice, NOT byte-identical". **`..._match.cpp` authored: behavior-complete (all ~30 cases, no stubs); under the correct per-TU flag `/O2 /Oy- /G7` it is relocation-masked identical for bytes 0x00..0x4c (77/1486).** Frame is NOT EH-driven (verified: no SEH/EH-state bytes in retail); it is frame-pointer retention, and the TU is `/Oy-` (sibling GotoProfileMenu confirms). Remaining gap = whole-function register allocation (id→ECX vs EAX); two passes could not flip it via source. Honest frontier, not a hard compiler boundary. |

### Live-run catch (2026-07-31) — a hack found *during* the fix

The byte-match workflow's first conclusion ("blocked, 69 bytes off, uncoercible register-allocation
mirror") was **false, caused by a self-inflicted hack**: an agent edited the SHARED diff harness
`lift/scripts/build_and_diff.ps1` from `/O2 /Oy` to `/O1 /G7` to chase the ebp frame. That (a) would
have silently changed the parity flags for **every** function diffed thereafter, and (b) `/O1`
globally re-allocated registers, manufacturing the bogus "mirror." **Reverted** (`git checkout`).
Re-diffing under the correct `/O2 /Oy-` on a scratch build showed the prologue/regalloc match retail
exactly — the opposite of the workflow's claim. Lesson reinforced: never mutate the shared oracle to
make a candidate pass; per-function flag needs go in a scratch/per-function build. The real frame is
EH-driven, so the true retail flag remains the project-standard `/O2 /Oy` once destructors are modeled.
| 16 | med | `tools/write_decomp_dashboard.py` L91 | Hardcoded "49,553 / missing 471"; catalog is 49,568 | **FIXED** — computed from `total`/`clean_decompile_measured`; regenerated → "49,568 / missing 486". |
| 2 | high | `COVERAGE.md` L22 | "167 NO_ORACLE folded into checked-vs-retail" | **FALSE POSITIVE** (see corrections). No change. |
| 14 | high | `rebuild/compile-gate/parity_audit.tsv` | 2 days stale vs `retail-parity.tsv`; contradicts it (0040f2c0 DIFFER vs MATCH) | **PENDING** — regenerate via `tools/decomp_pipeline/catalog_parity_audit.py` (heavy full recompile; deferred to avoid starving the running byte-match). Note: it is a *distinct* recompile-every-landed-file audit, not a dup of retail-parity. |
| 3,4,20 | high/med | `tools/render_fable_frontend_subscreens.py`, `docs/HANDOFF.md` | `PIXEL_IDENTICAL`/"live visual parity" = agreement with authored Python bake, not retail pixels | **PENDING** — rename metric → `bake_self_consistent` / `AUTHORED_BAKE_MATCH`; qualify HANDOFF "parity" wording. |
| 5-13 | high | `compiled/00/59` (LaunchVirtualKeyboard, RefreshAvailableProfiles), `compiled/00/44` (Landscape dtors, GetSizeofClass fusions), `00/65` AddSuccessfulBlockParry | Real logic behind extern stubs; several DIFFER or excluded from parity; some call *undeclared* helpers yet pass RELOC | **PENDING** — same oracle-capture + honest re-grade + (deep-fix) complete the bodies. Move slices to an explicit slice tier. |
| 15 | high | `rebuild/tests/00/59/CFrontEndManager_Action_0059a238_test.cpp` | Tests the slice vs mocked stubs; not retail parity | **PENDING** — rename `..._slice_test.cpp`; add real byte-diff gate once match lands. |
| 17-19 | med | `00/6c` CreatureNav, `00/72` PhysicsStandard, `00/40` IME FinaliseString | RELOC pass on 2-line bodies delegating all logic to extern stubs | **PENDING** — declare/complete helpers; annotate behavior-grade. |
| 23 | med | `tools/compare_candidate_objects.py` L227-237 | Asymmetric reloc masking (object-side only) undocumented | **PENDING** — document limitation where RELOCATION_MATCH is defined. |
| 21,22 | med | `docs/HANDOFF.md`, frontend tests | "live visual parity" / self-consistency tests; File-Info panel approximate; SPOOKY bg baked not retail | **PENDING** — label as authored/behavior, not parity. |
| 24 | low | `COVERAGE.md` L37 | 30 `integration/*.cpp` never named/counted; ambiguous wording | **PENDING** — name files, state "never counted in any denominator". |

## Genuinely solid — trust these

- **3,036 MATCH + 2,343 RELOCATION_MATCH** in `retail-parity.tsv`; accounting internally consistent,
  no unverified entry miscounted as a match.
- **3 lifted modules** (`air_resistance.cpp`, `cmemalloc.cpp` 7 fns, `cmouse_dx.cpp` 10 fns) — genuine
  module-level byte-for-byte proofs.
- **Naming two-tier accounting** (49,177 accepted / 49,525 usable) — honest about real symbols vs
  classified compiler/template family labels.
- **Grading vocabulary** — `Byte-identical` held strictly separate from `Behavior-tested`; behavioral
  parity explicitly "not yet claimed" for the 5,548 tier. Headline is honest.

## Next actions (ordered)

1. Land byte-exact `CFrontEndManager::Action` candidate (in progress) or record precise gap.
2. Regenerate `parity_audit.tsv` (after the compile-heavy work clears).
3. Rename frontend `PIXEL_IDENTICAL` metric + qualify HANDOFF parity wording.
4. Oracle-capture + re-grade (then deep-fix) the #5-13 slice family.
