# Contributing to FableDecomp

FableDecomp reconstructs the native PC `Fable.exe` (Fable: The Lost Chapters, VC7.1 / MSVC 2003)
as genuine C++ with retail behavior and close VC7.1 instruction output. Read `docs/ARCHITECTURE.md` first,
then `docs/BUILDING.md`. This page covers how work is accepted.

## What counts as done

Every reconstructed function is graded. `near_match` and matching grades count as
reconstruction; exact byte totals remain separate. See the 2026-09-26 standard in
`docs/modern/MODERNIZATION.md`.

| Grade | Meaning |
|---|---|
| `candidate` | Generated or hand-written source exists; nothing verified |
| `checker-pass` | Structural checker accepts it; still not compiled |
| `compiled` | Compiles under VC7.1 to an object file |
| `functional` | Behaviour test passes, but output differs beyond the near-match margin; source-completeness evidence only |
| `near_match` | Genuine decompiled C++, candidate-required behaviour test passes, instruction count within 10% and normalized instruction similarity at least 90% |
| `matching` | Behaviour test passes and `.text` matches exactly (`MATCH`) or after masking COFF relocations (`RELOCATION_MATCH`) |
| `asm_bake` | Hand-written `__asm` / `_emit` body. Byte-exact oracle, **not** reconstruction. May remain under `rebuild/src/compiled/`; identify by source and grade, not directory. Existing aggregate coverage can include bakes and must not be read as a genuine-C++ count |

"Compiles" is never reconstruction coverage. "Looks right" is never coverage.

These grades describe the intended acceptance contract. Historical `PASS` records are not blanket behavioral certification: some fixtures only print success, exact/relocation admission can run the test without its candidate, and some retype paths permit a missing test. Require candidate-exercising assertions and relevant negative controls before claiming tested behavior. Relocation masking checks byte layout outside masked fields; it does not establish relocation target identity. Published aggregate metrics also need purity filtering and address deduplication.

## Authoring one function

1. Pick a target from `rebuild/manifest/functions.tsv` that has no `compiled_source`. The oracle
   bytes come from your own retail install via `tools/decomp_pipeline/pe_oracle.py`.
2. Write the source as a single translation unit under `rebuild/src/compiled/<aa>/<bb>/`
   (`<aa><bb>` are the first four hex digits of the address) and a paired test under
   `rebuild/tests/<aa>/<bb>/`. File name: `<Class>_<Method>_<8-hex-address>.cpp`.
3. **Use the shared class headers.** If `rebuild/include/engine/<Class>.h` exists, include it and
   access members by name. Do not redeclare the class locally, and do not write
   `struct T { char pad[0x1a8]; ... }` when a header covers the class. The verifier warns on both.
   If no header exists yet, a local declaration with real member names and offset comments is
   acceptable; the header generator will pick it up.
4. Verify with `python tools/decomp_pipeline/verify_and_land.py <payload.json> <oracle.tsv>`.
   Add `--land` only when it reports `MATCH`, `RELOCATION_MATCH`, or `NEAR_MATCH` plus behaviour `PASS`.
   Stop tuning bytes at a passing near match unless the remaining difference changes behavior.
   Similarity masks register allocation, relocation operands, branch targets and stack offsets;
   constants, member offsets, operand widths and instruction order remain significant.
   The score is not a proof of behavior: fixtures must check outputs, side effects and call order.
   Functions with an embedded jump table use `verify_land_jumptable.py` instead.
5. Genuine C++ only. `__declspec(naked)`, `__asm`, and `_emit` are not accepted as reconstruction
   (see `docs/pipeline/PARITY_TRIAGE.md` for the de-bake lane if you are replacing one).

## Branches and commits

- `main` is what the public sees; keep it buildable. Validate the affected build and checks before publication; documentation/Python CI alone does not prove a native build. Work on a short-lived branch
  (`wip/<topic>`), merge when the parity audit is clean, delete the branch.
- Commit subjects are for humans. Prefix with one of `land:` `docs:` `tools:` `fix:` `hygiene:`
  and say what changed and how much, for example
  `land: CGameScriptInterface accessors (41 fns, byte-exact)`.
- Never hand-edit coverage numbers. `tools/write_decomp_dashboard.py` and
  `tools/update_readme_progress.py` are the only writers; CI fails on drift.

## What not to commit

No retail bytes: no executable, PDB, asset, decoded texture, model, or byte-oracle extract.
Oracle TSVs under `rebuild/oracles/` hold only the short instruction spans needed for parity
comparison. Root-level scratch (`*.obj`, `*.bin`) is ignored; keep working files under `work/`.

## Reporting a problem

Open an issue with the function address, the verifier output, and the compiler flags used.
For readability complaints about landed code, name the file; a retype pass exists exactly for that.
