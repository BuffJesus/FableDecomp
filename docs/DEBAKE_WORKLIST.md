# De-bake worklist — replacing naked `_emit` bakes with genuine C++

**North star:** byte-pure reconstruction from *genuine compiler-emitted C++*, not
`__declspec(naked)` `_emit` byte-bakes. Bakes are byte-exact oracles at best; they
are not faithful reconstructions. See memory `byte-purity-policy` / `faithful-decomp-policy`.

As of 2026-08-19 the landed set is ~94% genuine: **599 baked** functions remain
(was 696 on 2026-08-14). A family shares (length, call-masked skeleton), so
authoring ONE genuine head byte-exact lets `harvest_skeleton.py` / `debake_family.py`
clone-sweep the rest as genuine RELOCATION_MATCH.

## 2026-08-19 — all 8 staged templates LANDED (91 bakes replaced, +16 new)
`tools/decomp_pipeline/crawl/debake_family.py <template> <genuine.cpp> <prefix> --apply`
does the whole de-bake in one shot: computes the template's call-masked skeleton, finds
every manifest fn sharing it, un-lands the members whose landed source is an `_emit` bake
(catalog block + oracle row + src/test files), and emits a verify_and_land payload that
re-lands them all from the genuine source. Bootstrap-fixture addresses (referenced by
`rebuild/build_bootstrap.ps1`) are never touched; members already landed genuine are left alone.

Result (each re-verified byte-exact by the harness, MATCH or RELOCATION_MATCH):

| template | family | de-baked | +new | total landed |
|----------|--------|---------:|-----:|-------------:|
| 004477c0 | `_Uninit_copy` 8B stride (50B) | 31 | 0 | 31 (MATCH) |
| 00412b00 | `vector_deleting_destructor` (30B) | 22 | 0 | 22 |
| 0042ab1c | `OnReadFinished` (23B) | 8 | 11 | 19 |
| 004190e2 | `ShareData` helper-release (38B) | 8 | 4 | 12 |
| 00448280 | `_Uninit_copy` 12B stride (56B) | 7 | 0 | 7 (MATCH) |
| 0042abe1 | `OnKill` CTCCarryable (41B) | 6 | 0 | 6 |
| 0042a1b6 | `BuildTreeArray` (45B) | 5 | 0 | 5 |
| 00419463 | `operator=` → ShareData (23B) | 4 | 1 | 5 |
| **total** | | **91** | **16** | **107** |

Gates re-run after landing: `build_candidates.ps1 -Address <107>` → CANDIDATE_BUILD PASS
objects=107; `compare_candidate_objects.py` → all 107 MATCH/RELOCATION_MATCH (0 differing);
`build_bootstrap.ps1` → VISUAL_BOOT_CHECKPOINT PASS.

## Method (proven this session)
1. Disassemble the head; model as genuine C++ (real members, `__fastcall(self,…)` for
   this-in-ecx, address-of-extern for data relocs, declared-not-defined helpers for
   `call rel32` so they stay masked relocs).
2. Verify byte-exact standalone (`cl /O2 /Oy` + pragma/flag sweep) or via
   `verify_and_land.py` (dry-run) against the retail oracle → want MATCH/RELOCATION_MATCH.
   - **⚠ Verify ONLY with `tools/decomp_pipeline/vchk.py` (Python subprocess).** Manual `cl` in Git
     Bash mangles `/O2`-style args via MSYS path conversion (`/O2` → `C:/Program Files/
     Git/O2`), silently producing bogus framed/spilled output. Several families first judged
     "regalloc-hard" (`_Uninit_copy`) or "permuter-near-miss" (`ShareData-38`) were in fact
     clean MATCHes — the "misses" were MSYS artifacts. Trust vchk (or verify_and_land) only.
   - **Pragma idiom:** most families match on base `/O2 /Oy`; some need `optimize("s",on)`
     when retail cleans a single cdecl arg with `pop ecx` (size peephole) instead of
     `add esp,4`. `vchk.py` sweeps ["", g, s, t, gs, gt, ga, gsy]. `verify_and_land.py`
     PRAGMAS lacks `s`-combos beyond bare `s` — put the winning `#pragma` in-source (base
     compile honors it).
3. Land the genuine head, then `harvest_skeleton.py <head> <N>` to clone the family.
   The harness re-verifies every clone, so a bad match simply fails to land.

**Win banked this session:** `CCountedPointer<T>::ShareData` (67-byte variant, template
`0x004a88a0`) authored genuine on the first try (RELOCATION_MATCH, only the `operator
delete` reloc masked) → **92 genuine functions** landed (1 head + 91 clones). Model:
refcount-swap `if(this->p!=np){ if(this->p){ if(--rc==0){ p->fn(p->ctx); ::operator
delete(p);} } this->info=newInfo; this->p=np; if(np) ++np->rc; }`.

## Families by size + tractability
`rebuild`-relative template addr in brackets. Tractability from this session's probes.

| len | count | family | status (vchk-verified) |
|----:|------:|--------|--------------|
| 30 | 22 | `vector_deleting_destructor` (`??_E`) | **SOLVED/staged** — base `/O2`. `self->Dtor(); if(flags&1) ::operator delete(self); return self;` |
| 50 | 31 | `_Uninit_copy<…8B stride>` | **SOLVED/staged** — base `/O2`. (Prior "regalloc-hard" was an MSYS artifact.) |
| 56 | 7 | `_Uninit_copy<…12B stride>` | **SOLVED/staged** — base `/O2`. |
| 38 | 8 | `ShareData` (helper-release variant) | **SOLVED/staged** — `optimize("s")`. `if(this->p!=np){ this->ReleaseCur(); this->info=n; this->p=np; if(np) ++*np; }` (ReleaseCur=`0x4057a0`). |
| 23 | 8 | `OnReadFinished` | **SOLVED/staged** — `optimize("s")`. `this->Base(); if(this->m0) Free1(this->m0);` (Base=`0x42a141`). |
| 45 | 5 | `BuildTreeArray` (recursive tree free) | **SOLVED/staged** — `optimize("s")`. self-recursive: `while(node){ this->BuildTreeArray(node->mc); n=node->m8; Free1(node); node=n; }` |
| 23 | 4 | `operator=` (fwd to ShareData) | **SOLVED/staged** — `optimize("s")`. `this->ShareData(o.info,o.p); return *this;` |
| 41 | 6 | `OnKill` (CTCCarryable) | **SOLVED/staged** — `optimize("s")`. `if(this->m4){ this->BuildTreeArray(this->m0->m4); this->m0->m8=this->m0; this->m0->m4=0; this->m0->mc=this->m0; this->m4=0; }` |
| 47 | 7 | `GetMeshEffect` | **PERMUTER-NEAR-MISS** — 49v47; retail keeps `cur` in ecx for the virtual call, hand-model regalloc picks eax (+1 `mov ecx,eax`). CIVCountedPointer assign; model correct otherwise. |
| 53 | 83 | `_Dest_val<…CCountedPointer<QuadricOpt…>>` | **DEFER** — reloc-mask misalign; permuter. |
| 45/24/44/27 | ~45 | `_Dest_val<…>` variants | **DEFER** (same class). |
| 65 | 3 | `UpdateShadowScene` | **DEFER** — per-instance immediate divergence (proven DIFFER). |
| 39 | 4 | `SortTreeRecursively` | Partial — some genuine, some diverge. |

All SOLVED sources: `rebuild/backlog/staged_debakes/` (+ `README.md` manifest). **91 functions
across 8 templates staged genuine**, ready to land once the concurrent session is idle.

## Priority order (remaining)
1. Land the 8 staged templates → harvest families (**91 genuine functions**).
2. `GetMeshEffect` (7) — permuter pass (49v47, one `mov ecx,eax`).
3. `_Dest_val` (~130) + `UpdateShadowScene`/`SortTreeRecursively` — DEFER class; permuter.

Source data: `rebuild/backlog/staged_debakes/debake_families.tsv` (full 30-family table).

## Guardrail (recommended tooling fix)
`harvest_skeleton.py` should refuse to clone a template whose source contains `_emit`
(so the crawl never propagates bakes — this is how 49 bakes were created in one sweep
before the byte-purity reframe). Land only genuine heads as templates.
