# De-bake worklist — replacing naked `_emit` bakes with genuine C++

**North star:** byte-pure reconstruction from *genuine compiler-emitted C++*, not
`__declspec(naked)` `_emit` byte-bakes. Bakes are byte-exact oracles at best; they
are not faithful reconstructions. See memory `byte-purity-policy` / `faithful-decomp-policy`.

As of 2026-08-19 (fifth pass) the landed set is ~98.3% genuine: **238 baked** functions
remain (696 on 2026-08-14 → 599 → 401 → 342 → 254 → 238). A family shares (length, call-masked skeleton), so
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

## 2026-08-19 (fifth pass) — whole-manifest family sweep + the last DEFER falls

New `tools/decomp_pipeline/crawl/harvest_all.py`: instead of harvesting one template at a
time, it groups **every** manifest row by (trimmed length, call-masked skeleton), and for each
group that already has a landed GENUINE source it emits the un-landed members as candidates
with that source. It never propagates a `_emit` bake (the template source must be genuine).
One pass landed **858** functions across 178 families; a second pass came back dry (2 DIFFERs),
so the family veins are now drained at the current template set.

Hand-authored this pass (all MATCH or RELOCATION_MATCH):

| addr | function | model |
|------|----------|-------|
| 00436a20 | `_Dest_val` + vector-deleting-destructor (73B) | release slot, clear both words, `if(flags&1) Free1(this)`; +43 family |
| 0043fcf0 | list `Destroy` (62B) | Clear loop, re-point sentinel, free sentinel |
| 00431020 | **`UpdateShadowScene` (65B)** | profiled two-case switch with a zero-init local handle |
| 004406d0 | `~vector<T>` (50B) | destroy each element via vtable slot 0 (stride 8), free block |
| 00415d70 | `CCharString` assign-through-pool (25B) | `g_pool.Assign(dst, this->data); return dst;` |
| 0041bd80 | `OnDie` (23B) | `if (p) { p->Cleanup(); Free1(p); }` |

**`UpdateShadowScene` was the last of the original DEFER list** — "per-instance immediate
divergence" was, like the `_Dest_val` class, a modelling gap: it needed `#pragma optimize("s")`
plus caching the loop bound in a local (`~vector` likewise: cache `last`, or VC re-reads it).

Round total 924 (921 landed + 3 that stayed unlanded as DIFFER). Gates: CANDIDATE_BUILD PASS
objects=921; comparer 93 MATCH + 828 RELOCATION_MATCH, 0 differing; VISUAL_BOOT_CHECKPOINT PASS.

### Still parked
- `0042bf35` CopyBackBufferToTexture (25B ×7) — retail recomputes `lea eax,[ebp-1]` for both
  out-params; VC7.1 CSEs it. Permuter.
- `00429418` `_Find` (34B ×3) — retail re-reads `last` from the stack each iteration and keeps
  the sret pointer in eax; hand models cache it. Permuter.
- The 4 IAT thunks; the two `call;ret` void sub-object forwarders; the sret wrapper at 0042b6dc.

## 2026-08-19 (fourth pass) — ROW TRIMMING unlocks 1,513 landings

The biggest remaining blocker was not authoring difficulty, it was **over-captured manifest
rows**: 125 of the 342 remaining bakes (and thousands of never-landed rows) spanned their own
body *plus* one or more unlisted neighbours, because VC7.1 packs tiny functions with no `0xCC`
padding between them. `trim_overcapture.py` only handles the padded case.

New `tools/decomp_pipeline/crawl/rowtrim.py` handles the unpadded case by control-flow
analysis: decode the row, track the furthest forward branch target that lands *inside* the row,
and cut at the first `ret`/`ret N`/`jmp` that nothing branches past. Branches out of the row are
tail calls, not internal flow. It refuses to cut when the decode does not cover the row, so
`true_body()` is safe to call unconditionally — `debake_family.py`, `shape_author.py` and
`bake_families.py` all use it now.

With trimming on, previously-unreachable rows became landable and the existing genuine
templates swept them up:

| step | landed |
|------|-------:|
| trim-aware re-run of the 6 matching genuine templates (vecdel 30B alone: 667) | 903 |
| trim-aware re-run of all 19 staged templates (`_Dest_val` 53B: 267, OnReadFinished-25: 60, list Clear 47B: 49) | 474 |
| `shape_author` with trimming (54 pair-`_Dest_val`, 20 forwarders, …) | 111 |
| hand-authored this pass (Deactivate, operator[], operator new, flag-selected float getter ×2, ProcessEvent, IsSystemEvent, GetBankHandle, AddToInterface, ScaleToInt, DeleteData ×4, PtrEqual, FreeIfSet, HasPhysicsMesh, SubPtrConst, iter ++/--, char/sub-ptr setters, DeleteIfSet) | 25 |
| **total** | **1513** |

256 MATCH + 1257 RELOCATION_MATCH, 0 differing. Gates: CANDIDATE_BUILD PASS objects=1513;
build_bootstrap VISUAL_BOOT_CHECKPOINT PASS.

### Known near-misses (parked)
- `0042bf35` CopyBackBufferToTexture (25B ×7): retail recomputes `lea eax,[ebp-1]` for BOTH
  out-params; VC7.1 CSEs the address into `push eax; push eax` (22B) whatever the source shape
  (one local, two locals, struct+member aliasing all tried). Needs the permuter.
- `00434a00` / `004175da` (`add ecx,imm; call; ret`, ×2): retail keeps `call;ret` where VC7.1
  tail-jumps a void sub-object forwarder — the known non-recoverable idiom.
- `0042bef2` sret struct-return forwarder: VC7.1 tail-jumps the whole thing.

## 2026-08-19 (third pass) — shape-class authoring: 358 more landed

New tool `tools/decomp_pipeline/crawl/shape_author.py`. Where `debake_family.py` needs one
byte-identical family per template, this recognises a **parameterised instruction shape** and
emits a per-address source with the right member offset / constant / argument count, so a
whole class of tiny functions lands from one classifier. Shapes recognised today:

| shape | model |
|-------|-------|
| `ret` / `ret N` | empty member hook |
| `xor al,al` / `mov al,1` / `xor eax,eax` / `mov eax,imm` + ret | constant-return member |
| `mov/movzx/fld eax,[ecx+d]` + ret | int / char / float member getter (pack(1)) |
| `lea eax,[ecx+d]` + ret | address-of-member accessor |
| `mov eax,[esp+4]; mov [ecx+d],eax; ret 4` | member setter |
| `mov ecx,[ecx+d]; jmp` | forward to a member of a sub-object pointer (VC7.1 tail-call) |
| `jmp rel32` | free-function forwarder |
| `mov eax,[global]; ret` | global getter |
| `push esi; mov esi,ecx; lea ecx,[esi+o2]; call; …; pop esi; jmp` | std::pair `_Dest_val` (release second, tail-call first) |
| `lea eax,[esp+4]; push eax; add ecx,d; call; ret 4` | forward the ADDRESS of the stack arg to a sub-object |
| `sub esp,8; lea eax,[esp]; push eax; call; mov eax,[eax(+4)]; add esp,8; ret` | out-param temp getter |

358 landed (59 de-baked + 299 previously-unlanded manifest functions), all MATCH (125) or
RELOCATION_MATCH (233). Gates: CANDIDATE_BUILD PASS objects=358; comparer 0 differing;
build_bootstrap VISUAL_BOOT_CHECKPOINT PASS.

**`#pragma pack(push,1)` is required** on every emitted struct: retail member offsets are
byte-exact and natural alignment silently shifts an odd one (`lea eax,[ecx+0x29]` became
`+0x2c` and failed parity until packed).

**Not de-bakeable:** the 4 IAT thunks (`initterm`, `__dllonexit`, `malloc`, `strstr` at
`ff25 <iat>`) are linker-generated, not compiler output — they stay `_emit` by nature.

## 2026-08-19 (second pass) — `_Dest_val` class SOLVED, 401 bakes left

The whole DEFER class fell to plain C++ once the `this->p = 0` was placed **inside** the
release branch (retail's `je` skips the zero store when the pointer is null) and the
`o->p` reload was left un-hoisted (o may alias this). No permuter needed.

| template | family | de-baked | +new | landed | model |
|----------|--------|---------:|-----:|-------:|-------|
| 00413310 | `_Dest_val` 53B | 83 | 378 | 461 | release-block: `--rc; if(!rc){ blk->release(blk->owner); Free1(blk); }` then clear both words |
| 004190b6 | `_Dest_val` 44B | 10 | 20 | 30 | same source, `#pragma optimize("s")` (`and`/`pop ecx` peepholes) |
| 0041bcd0 | `CIVCP::Adopt` 45B | 15 | 0 | 15 | release then take raw ptr, no addref |
| 00419554 | `CIVCP::Release` 24B | 13 | 0 | 13 | same as 27B with pragma `s` |
| 0042a6c3 | `GetMeshEffect` 47B | 7 | 0 | 7 | CIVCP assign; was the "49v47 permuter near-miss" — actually one `je` displacement |
| 0041bae0 | `CIVCP::Release` 27B | 7 | 0 | 7 | `if(cur){ if(--rc==0) cur->v1(); this->p=0; }` |
| 0042a141 | list `Clear` 39B | 4 | 6 | 10 | circular list free loop (manifest calls it `SortTreeRecursively`) |
| 00431e70 | tail-jmp forwarder 5B | 5 | 3 | 8 | void member → void member, VC7.1 tail-calls it |
| 00415920 / 00415e67 / 00440ec0 | empty `ret` / vecdel-28 / OnReadFinished-25 | 6+6+6 | 0 | 18 | |
| 004197b0 / 0042dc60 / 0042da20 / 004197a0 / 0042da50 | `return false` / `return true` / empty `ret 4` / byte-flag setter / `return 0` | 3+2+3+2+2 | 0 | 12 | |
| 0042a2b9 / 0041c7c0 / 00416268 / 0042b636 / 004293f2 / 0042d9a0 | SetStaticCast / `operator!=` / BeginInputLoading / list `Count` / helper-fixup / vecdel+vptr | 3+2+2+2+2+2 | 0 | 13 | |
| **total** | | **187** | **407** | **594** | |

Gates: CANDIDATE_BUILD PASS objects=594; `compare_candidate_objects.py` → 64 MATCH +
530 RELOCATION_MATCH, 0 differing; `build_bootstrap.ps1` VISUAL_BOOT_CHECKPOINT PASS.

**Bakes remaining: 401 in 386 families — the tail is flat (largest family is 3).** Clone
leverage is spent; from here each bake is roughly one authoring job. Regenerate the live
family table any time with `scratchpad/bake_families.py` (groups landed `_emit` sources by
length + call-masked skeleton straight from the catalog).

### Still open
- `004193a0` (`DeleteData`, 36B ×3) — the manifest row is **over-captured**: it spans a
  `ret 4` at +0x14 and then a second, unlisted 13-byte function. Needs `trim_overcapture.py`
  (no `0xCC` between them) or a two-function land, not a single source.
- `00431020` / `00431242` (`UpdateShadowScene`, 65B/53B) — per-instance immediate divergence.
- Everything else: 386 families of 1–3.

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
