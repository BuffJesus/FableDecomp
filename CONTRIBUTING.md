# Contributing to FableDecomp

FableDecomp reconstructs the native PC `Fable.exe` (Fable: The Lost Chapters, VC7.1 / MSVC 2003)
as C++ that compiles back to the retail instruction bytes. Read `docs/ARCHITECTURE.md` first,
then `docs/BUILDING.md`. This page covers how work is accepted.

## What counts as done

Every reconstructed function is graded. Only the last two grades are reported as coverage.

| Grade | Meaning |
|---|---|
| `candidate` | Generated or hand-written source exists; nothing verified |
| `checker-pass` | Structural checker accepts it; still not compiled |
| `compiled` | Compiles under VC7.1 to an object file |
| `functional` | Behaviour test passes; `.text` differs from retail only in relocation slots (`RELOCATION_MATCH`) |
| `matching` | Behaviour test passes and `.text` is byte-identical to retail (`MATCH`) |
| `asm_bake` | Hand-written `__asm` / `_emit` body. Byte-exact oracle, **not** reconstruction. Lives in `rebuild/src/asm_bake/` and is excluded from the C++ coverage numbers |

"Compiles" is never coverage. "Looks right" is never coverage.

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
   Add `--land` only when it reports `MATCH` or `RELOCATION_MATCH` plus behaviour `PASS`.
   Functions with an embedded jump table use `verify_land_jumptable.py` instead.
5. Genuine C++ only. `__declspec(naked)`, `__asm`, and `_emit` are not accepted as reconstruction
   (see `docs/pipeline/PARITY_TRIAGE.md` for the de-bake lane if you are replacing one).

## Branches and commits

- `main` is always what the public sees and always builds. Work on a short-lived branch
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
