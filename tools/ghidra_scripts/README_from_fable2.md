# Self-contained headless function labeler (Fable2_TU1)

Batch-label `sub_`/`Function_` functions in the Ghidra project with no external
services or API key. The "LLM" step is a human/Claude reading a worklist and
writing back names — the same loop `auto-re-agent` runs, using only the Ghidra
headless scripts we already have working.

Two scripts, driven by `analyzeHeadless`:

## 1. Gather — `LabelDump.java`
Emits a labeling worklist (decompiled C + callers + callees + referenced
strings) for a set of functions.

```
analyzeHeadless <proj> Fable2_TU1 -process default_tu1.xex -noanalysis \
  -scriptPath tools/ghidra_label -postScript LabelDump.java <selector> [outfile]
```
`<selector>`:
- `auto:strings`   — functions that reference string literals (default, cap 60)
- `auto:calls:N`   — the N most-called unnamed functions
- `<path>`         — a file of hex addresses (one per line)

Output → `ghidra_out/label_batch.txt` by default.

## 2. Apply — `LabelApply.java`
Reads a TSV (`<addr>\t<name>\t<comment>`, `#` comments allowed) and applies
`setName` + a plate comment to each function (creating it if missing).

```
analyzeHeadless <proj> Fable2_TU1 -process default_tu1.xex -noanalysis \
  -scriptPath tools/ghidra_label -postScript LabelApply.java <tsv-path>
```
Default input: `ghidra_out/labels_in.tsv`.

## Workflow
1. Run `LabelDump` to produce `label_batch.txt`.
2. Read it, decide names, write `labels_in.tsv` lines (`addr \t name \t comment`).
3. Run `LabelApply`.
4. Repeat; changes persist in the Ghidra project.

`ghidra_out/labels_in.tsv` seeds the first batch: 21 boot/CRT/thread functions
reverse-engineered while debugging the boot hang (CRT thread startup, `_getptd`,
the Lionhead debug-log init, etc.).

Note: `analyzeHeadless` may print `exit code 255` even on success — check for the
script's own `LabelDump/LabelApply: ...` summary line to confirm it ran.
