# Fable: The Lost Chapters — Decompilation & Native Modding

A reverse-engineering project on the **native PC** `Fable.exe` (Fable: The Lost Chapters, Steam).
**End goal: a full decompilation** — a complete, readable, buildable C/C++ source reconstruction of
the game (see `docs/FULL_DECOMP.md`) — with a deep native modding surface as the natural dividend.

> **This is not a port project.** TLC is already a native x86-32 Windows game — there is nothing to
> recompile (unlike the sibling **Fable2RE**, which statically recompiles an Xbox 360 PowerPC binary).
> This is pure static analysis + live instrumentation of a **clean, unpacked PE32** (base `0x400000`).

## Why this is tractable
- `Fable.exe` is a clean MSVC PE32 — **no packer, no DRM stub** — Ghidra loads it directly.
- **FableScriptExtender (FSE)** already reversed the game's C++ scripting API: its **931-function
  manifest** (`refs/fse_api_manifest.json`) and its **local source** (`D:\Code\FQT\SourceFilesToReference\FSE`)
  seed Ghidra with real names, types, and engine call sites.
- **~20 years of community RE** (fabletlcmod.com, Fable Explorer) already solved the *data formats* —
  we do the complementary *engine internals* work.
- The **Fable2RE** sibling project provides a proven Ghidra scripting suite + RE methodology, copied in.

## Layout
| Path | What |
|---|---|
| `docs/PLAN.md` | ★ **The plan — read first.** Goals, ecosystem integration, phased roadmap. |
| `docs/FULL_DECOMP.md` | ★ The full-decompilation strategy (the primary goal). |
| `docs/ECOSYSTEM.md` | Survey of existing tools (community + the user's own FSE/FQT/**EgoCore**). |
| `docs/TOOLCHAIN.md` | Exact commands: Ghidra import, GhidraMCP, FSE import, live analysis. |
| `docs/METHODOLOGY.md` | The Ghidra labelling/RE loop inherited from Fable2RE. |
| `docs/HANDOFF.md` | ▶ Resume-here checklist. |
| `tools/ghidra_scripts/` | Ghidra RE script suite (30 scripts, from Fable2RE). |
| `tools/fse_import/` | Converts FSE's manifest → `fse_api.h` (Ghidra) + `fse_api_index.md` (roadmap). |
| `tools/lua_mod/` | Lua bytecode disassembler + archive tooling (from Fable2RE). |
| `refs/` | FSE manifest + runtime log, "Lua in Fable II" paper, (to add) FSE source + format wiki. |
| `ghidra_proj/` | The Ghidra project `FableTLC` (created on first import). |
| `ghidra_out/` | Decompile dumps + `labels_*.tsv` (the reproducible DB source). |

## Repository hygiene

This public repository preserves reconstruction source, documentation, automation, curated tests,
and compact derived analysis catalogs. It intentionally does not contain original game executables,
private PDBs, Ghidra/BSim databases, staged game archives, crash dumps, or generated build products.
Those local inputs are reproducible prerequisites rather than redistributable project source; see
`docs/TOOLCHAIN.md` for the expected local setup.

## Quick start
See `docs/TOOLCHAIN.md`. In short: install GhidraMCP into Ghidra 12.1.2 → `analyzeHeadless` import
`Fable.exe` → `python tools/fse_import/fse_manifest_to_ghidra.py` → parse `fse_api.h` → run the
RTTI + string-xref + FSE-target passes (Phase 1) → build `docs/SYSTEMS_ANALYSIS.md`.

## Relationship to other projects on this machine
- **Fable2RE** (`D:\Documents\Fable2RE`) — sibling; Xbox 360 Fable 2 recomp+decomp. Source of tooling.
- **FQT** (`D:\Code\FQT`) — the user's WPF quest editor over FSE; a downstream consumer of this
  project's extracted name/def tables.
- **UE6Verse** (`C:\Users\...\UE6Verse`) — the user's UE6 game that ingests extracted Fable assets;
  shares the `data\` format knowledge (see its memory notes).
