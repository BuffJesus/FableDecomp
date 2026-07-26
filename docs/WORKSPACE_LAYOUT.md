# Workspace layout and housekeeping

The checkout separates durable public reconstruction work from local analysis inputs and generated
artifacts. Keep those boundaries intact: many tools assume these paths, and moving an entire live
tree for cosmetic reasons can break reproducibility.

| Path | Purpose | Public repository |
|---|---|---|
| `docs/` | Findings, plans, runbooks, and handoff state | Yes |
| `tools/` | Reproducible extraction, audit, and promotion tooling | Yes |
| `rebuild/src/compiled/<aa>/<bb>/` | Landed one-function VC7.1 reconstruction source | Yes |
| `rebuild/tests/<aa>/<bb>/` | Focused per-function behavior gates | Yes |
| `rebuild/modern/<subsystem>/` | Cohesive human-facing C++23 reconstruction | Yes |
| `rebuild/ARTIFACT_INDEX.tsv` | Address/module map across source, tests, snapshots, and builds | Yes; generated |
| `rebuild/build/<kind>/<aa>/<bb>/<address>/` | VC7.1 objects, executables, and logs | No; generated |
| `rebuild/candidates/snapshots/<aa>/<bb>/` | Replaceable gate input snapshots | No; generated |
| `lift/reports/<wave>/code/<aa>/<bb>/` | Uncurated agent candidate source | Yes when intentionally reviewed |
| `lift/logs/<wave>/<date>/<aa>/<bb>/<address>/` | Attempt transcripts | No; local |
| `lift/.cache/re-agent-decompile/<aa>/<bb>/` | Replaceable decompiler cache | No; generated |
| `lift/state/` | Durable local queue ledgers and control files | Mixed; logs/PIDs are local |
| `work/` | Experiments, staged mods, runtime probes, and scratch | No; local |
| `snapshots/` | Local binary/document archives | No; local |
| `ghidra_proj/`, `bsim/`, `symbols/` | Large proprietary/local analysis inputs | No; local |
| `FSE/` | Local deployment backups used by staging scripts | No; local |

## Safe cleanup

Run:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/organize_workspace.ps1
```

The combined organizer is non-destructive. It:

- moves old root-level `.obj` files to `work/scratch/root/objects/`;
- moves root-level scratchpad source to `work/scratch/root/sources/`;
- moves the local documentation ZIP to `snapshots/local-archives/`;
- invokes the lift organizer, which archives and address-shards RE-agent transcripts;
- address-shards candidate source, curated source, tests, snapshots, build products,
  and land-verification products;
- rewrites catalog source/test paths and regenerates `rebuild/ARTIFACT_INDEX.tsv`;
- refuses destination paths outside the intended ignored trees, preserves collisions, and skips
  recent or actively referenced files.

Use `-WhatIf` to preview and `-MinimumAgeMinutes` to change the recent-file protection window.
An apply run refuses a full decomp migration while a queue/refresh process is active. Collisions
remain in place and make the migration fail for explicit review. The script does not delete files,
prune build products, move live state trees, or alter documented experiment paths under `work/`.

See `docs/SOURCE_ARCHITECTURE.md` for why the parity layer remains one function per translation
unit and how it feeds the subsystem-oriented C++23 layer.
