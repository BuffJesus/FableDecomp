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

## Scope & roadmap
- **Now (manual mode):** you supply the 2-3 uncertain spellings as `PERM(...)`; the search finds
  the byte-matching combination. Ideal when you know *where* the ambiguity is (idiom, pragma,
  signedness, operand order).
- **Next (random/AST mode):** the piece that cracks pure regalloc coin-flips — automatic
  AST mutations (introduce/inline temps, reassociate, retype, reorder independent statements)
  à la upstream decomp-permuter. Needs a C/C++ AST layer (pycparser or libclang); the scorer
  here is already the objective function it would hill-climb on.
- **Integration:** pairs with [reccmp](https://github.com/isledecomp/reccmp) (the LEGO Island
  MSVC-x86 matching-decomp toolchain) for source annotations + per-function match tracking; our
  relocation-masked compare already reimplements reccmp's core comparison.

Oracle bytes come from `rebuild/oracles/auto-re-candidates.tsv` by default (`--oracle` to override,
e.g. a freshly Ghidra-extracted batch).
