# Project layout and workspace care

Start with [HANDOFF](HANDOFF.md) for the public checkpoint, [ROADMAP](ROADMAP.md)
for current priorities, and [INDEX](INDEX.md) for the full documentation catalog.

The September Discord concerns about layout, typed objects, AI maintainability,
debug symbols, and engine integration are recorded in
[community feedback](pipeline/COMMUNITY_FEEDBACK.md).

## Source and evidence locations

| Location | Purpose |
|---|---|
| `rebuild/src/compiled/` | Curated VC7.1 reconstruction, organized by native address |
| `rebuild/include/` | Shared engine layouts and subsystem declarations |
| `rebuild/tests/` | Behavior fixtures and integration tests |
| `rebuild/modern/` | Separate C++23/x64 subsystem work |
| `tools/decomp_pipeline/` | Native reconstruction and verification tools |
| `tools/modern/` | Modernization preparation and validation tools |
| `tools/script_recovery/` | Lua recovery tools and tests |
| `docs/engine/`, `docs/formats/`, `docs/pipeline/`, `docs/scripts/`, `docs/modern/` | Reference documentation by topic |
| `docs/journal/YYYY-MM/` | Dated results, failed attempts, and historical checkpoints |
| `refs/`, `ghidra_out/`, `lift/` | Curated analysis inputs, exports, and intake records |
| `work/`, `build/`, `rebuild/build/` | Local scratch, builds, and evidence; retained locally |
| `scratchpad/`, `runtime_dumps/`, `.scratch_tmp/` | Local captures and temporary experiments |

New experiments belong under `work/<topic>/<date>/`. Keep native source at its
existing address-based path until dependency and parity checks justify a move.
Keep related receipts beside the experiment; a failed attempt can still explain
a current restriction. Ignoring a file does not make it safe to delete.

## Parallel work

Before cleanup, identify active sessions and registered worktrees with
`git worktree list` and coordinate exact path ownership with their owners.
Record local ownership in `.agents/CLEANUP_COORDINATION.md` when using that
workspace convention. Keep local session identifiers out of public documents.

Use an isolated checkout for public updates. Do not switch, stash, reset, stage,
or commit the shared checkout while another session is writing there. Build
outputs, source snapshots, failed attempts, and prior evidence may be live test
dependencies even when old. Relocation, deletion, or compression needs an exact
reviewed scope and a coordinated timing window.

The local maintenance catalog and its saved project shortcuts provide navigation
without relocating projects. Saved inventory dates describe past observations;
they do not establish that a folder is currently unused.

## Public updates

Publish an explicit file list from a clean checkout and inspect the staged diff.
Do not merge a development branch solely to make the public activity look
current. A status update can describe qualified local work while source remains
under review; state that distinction in [NATIVE_STATUS](NATIVE_STATUS.md).

Never publish retail binaries, proprietary symbols, game assets, decoded
payloads, local captures, or raw byte-oracle evidence. Keep user-supplied inputs
and their generated products local. See [CONTRIBUTING](../CONTRIBUTING.md).

Before publication, run:

```powershell
python tools/docs_reorg.py --index
python tools/docs_reorg.py --check-links --check-findings --check-root
python tools/update_readme_progress.py --check
git diff --check
```

Regenerate coverage only from a settled, reviewed reconstruction checkpoint.
Composition-test counts and CTest totals are not whole-executable coverage.
