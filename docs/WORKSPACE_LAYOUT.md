# Workspace layout and housekeeping

The checkout separates durable public reconstruction work from local analysis inputs and generated
artifacts. Keep those boundaries intact: many tools assume these paths, and moving an entire live
tree for cosmetic reasons can break reproducibility.

| Path | Purpose | Public repository |
|---|---|---|
| `docs/` | Findings, plans, runbooks, and handoff state | Yes |
| `tools/` | Reproducible extraction, audit, and promotion tooling | Yes |
| `rebuild/src/compiled/` | Landed reconstruction source | Yes |
| `rebuild/tests/` | Focused behavior gates | Yes |
| `rebuild/build/` | VC7.1 objects, executables, and logs | No; generated |
| `lift/reports/<wave>/code/` | Uncurated agent candidate source | Yes when intentionally reviewed |
| `lift/logs/<wave>/<date>/` | Attempt transcripts | No; local |
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

The organizer is non-destructive. It:

- moves old root-level `.obj` files to `work/scratch/root/objects/`;
- moves root-level scratchpad source to `work/scratch/root/sources/`;
- moves the local documentation ZIP to `snapshots/local-archives/`;
- invokes the existing lift organizer, which archives loose RE-agent transcripts under
  `lift/logs/<wave>/<date>/`;
- refuses destination paths outside the intended ignored trees, preserves collisions, and skips
  recent or actively referenced files.

Use `-WhatIf` to preview and `-MinimumAgeMinutes` to change the recent-file protection window.
The script does not delete files, prune build products, move live report/state trees, or alter
documented experiment paths under `work/`.
