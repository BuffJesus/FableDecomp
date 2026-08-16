# permuter — VC7.1/x86 decomp-permuter (prototype)

A [decomp-permuter](https://github.com/simonlindholm/decomp-permuter)-style search harness
adapted to this project's toolchain: it drives the **real VC7.1 `cl.exe`** and scores each
candidate's `.text` against authoritative retail bytes (relocation-masked), converging on the
source spelling whose codegen byte-matches. Built to attack the **register-allocation /
instruction-scheduling tail** — functions that are functionally correct and the right length
but differ only in which register holds a value or where a `push` lands (see
`docs/PARITY_TRIAGE.md` "class 2").

## Pieces
- `permuter_score.py` — the scorer. Compiles a `.cpp` with `cl /c /O2 /Oy /W3`, extracts the
  target function's `.text` via objdump, masks relocations (mirrors
  `tools/compare_candidate_objects.py`), and returns a numeric distance:
  **0 = exact/relocation match**; equal-length-but-different = number of differing bytes;
  wrong length = large penalty + prefix bonus. Importable (`score_source`) or CLI.
- `permuter.py` — manual-mode search driver. Annotate a candidate with choice points and it
  expands the cross-product (random-samples when large), compiles+scores each, keeps the best,
  stops early on a match, and writes `<name>.match.cpp` / `<name>.best.cpp`.

## Choice-point syntax
```c
PERM(<alt0>|<alt1>|...)      // inline: tries each alternative expression/statement
PERMPRAGMA(<p0>|<p1>|)       // a whole line; empty final option = omit the line
```
`|` splits at the top level only (parens/brackets/braces are respected), so
`PERM(a(x,y)|b)` is two options, not three.

## Demonstrated
```
# auto-rediscovers the hand-found fix (score 0):
python tools/permuter/permuter.py 0040135c examples/setdefaultprecision_0040135c.cpp
  -> MATCH FOUND (RELOCATION_MATCH); winning choice: #pragma optimize("s", on)

# honest on a genuine regalloc dead-end (converges but does not false-claim):
python tools/permuter/permuter.py 00405ba0 examples/stdmovebackward_00405ba0.cpp
  -> BEST score=4 (30-byte fn, 4 differing bytes = the mov;sub / sub;mov swap)
```

## Full-regalloc mode (`anneal.py`) — no annotation
`python anneal.py <hexaddr> <plain.cpp> [--name leaf] [--oracle t.tsv] [--iters N]
[--seed S] [--t0 4.0] [--tend 0.3] [--restart 120]`

The upstream-decomp-permuter loop: a **simulated-annealing** random walk over the whole
mutation library, biased downhill by a cooling Metropolis criterion so it can accept a
lateral/worse move to reach a spelling several mutations away that finally flips the
register allocation. Pipeline per run:
1. **seed** flag×pragma sweep on the untouched source;
2. **greedy 1-hop sweep** — every single mutation × a small flag/pragma subset
   (deterministic; guarantees a match one mutation away is found regardless of seed);
3. **anneal** — multi-hop SA with restart-from-best, memoized scoring.

### Register-level mode (`reg_mutations.py`) — and its measured ceiling
Added the register-allocator levers upstream relies on for the residuals source *shape* can't
reach: **`permute_decl`** (full permutations of each contiguous local-declaration run — the
dominant input to VC's stack-slot + first-fit register assignment) and **`pressure`** (inject a
live dummy local to raise register pressure and shift keep-in-reg vs reload/spill). The anneal
flag grid now also sweeps **`/Oy-`** (keep frame pointer) — retail TUs use both `/Oy` and `/Oy-`,
and the frame choice dominates the whole layout (this is what closed `InitWorld` and `AddStatUpdate`
reached its near-match).

**Measured ceiling (2026-08-16):** on the three canonical residual classes these levers do NOT
reach the byte-match — `ApplyScriptBrush` eax↔edx register-choice (stays score 4), `AddStatUpdate`
reload-vs-cache (list-find matches; 2-byte reload residual unmoved), `CPlayerManager::LoadGameState`
stack-slot ordering (decl-permute changes the layout but never to retail's). The reason is
fundamental: **C++ exposes no register control**, and VC7.1's allocator (proven identical across
RTM 3077 / QFE 4035 / SP1 — see QFE4035_COMPILER_GATE.md) is robust to source perturbation for
these choices. The `/Oy-` sweep and `permute_decl` genuinely help *discovery* (frame-pointer TUs,
some slot layouts), but the reload-vs-cache / exact-register / exact-slot residuals are
source-unreachable for MSVC x86. Closing them would require asm-level editing (which breaks
byte-purity — the source would no longer produce the bytes) or the exact original source. This
is a real limit of byte-pure reconstruction for this compiler, not a gap in the search.

Mutation library (all semantics-preserving by construction; byte-match is still
behaviour-re-checked at land time):
- `clang_mutations.py` — materialize-earlier levers: **temp introduction**, operand
  **reassociation**, **decl splitting** (`T x=a op b;` → `T x=a; x op= b;`).
- `regalloc_mutations.py` — regalloc/scheduling levers: **temp inlining** (fold a
  single-use local back into its use — the inverse of temp-intro, forces re-materialize/
  reload), **decl reordering**, **adjacent independent statement reordering**,
  **block-scope insertion** (`{ }` to shorten local lifetimes → stack-slot reuse), and
  **local retyping** (value-equivalent integer width/spelling).

Validated: reproduces the `optimize("s")` pragma match (seed); auto-discovers a
statement-reorder fix on a flag-immune same-length gap (greedy + SA); and stays honest
on genuine QFE-4035-gated dead-ends (`stdmovebackward` score 4, `Init` score 19) without
false-claiming. Needs `libclang` (see `requirements.txt`).

- **Manual mode (`permuter.py`):** when you know *where* the ambiguity is, annotate the
  2-3 uncertain spellings as `PERM(...)`/`PERMPRAGMA(...)` and it searches the cross-product.
- **Integration:** pairs with [reccmp](https://github.com/isledecomp/reccmp) (the LEGO Island
  MSVC-x86 matching-decomp toolchain) for source annotations + per-function match tracking; our
  relocation-masked compare already reimplements reccmp's core comparison.

Oracle bytes come from `rebuild/oracles/auto-re-candidates.tsv` by default (`--oracle` to override,
e.g. a freshly Ghidra-extracted batch).
