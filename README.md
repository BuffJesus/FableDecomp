# Fable: The Lost Chapters — Decompilation & Native Modding

An evidence-gated reconstruction of the native 32-bit Windows
`Fable.exe`, targeting readable, buildable C/C++ source and a durable native
modding platform.

> **Current state:** the reconstructed executable boots through the recovered
> startup path, plays the retail intro sequence, and presents an interactive
> frontend checkpoint. The complete engine, renderer, and game loop are not yet
> reconstructed.

[Run the checkpoint](#run-the-checkpoint) ·
[See current progress](#current-progress) ·
[Read the roadmap](#roadmap) ·
[Browse the documentation](#documentation)

<a id="contents"></a>
## Contents

1. [Project](#project)
2. [Current progress](#current-progress)
3. [Run the checkpoint](#run-the-checkpoint)
4. [What works today](#what-works-today)
5. [Roadmap](#roadmap)
6. [Verification pipeline](#verification-pipeline)
7. [Documentation](#documentation)
8. [Repository layout](#repository-layout)
9. [Project boundaries](#project-boundaries)
10. [Why this is tractable](#why-this-is-tractable)

<a id="project"></a>
## 1. Project

The goal is a full decompilation of **Fable: The Lost Chapters for PC**: a
complete source reconstruction that can be understood, built, tested, and
extended.

This is not a source port or a static recompilation of another architecture.
TLC already ships as a clean native x86-32 PE built with Microsoft Visual C++
7.1. The project combines static analysis, donor-symbol recovery, live
instrumentation, original-compiler reconstruction, and retail-byte comparison.

The work has three connected outputs:

- **Decompilation:** faithful VC7.1-compatible source, one verified function at
  a time.
- **Runnable reconstruction:** recovered startup, frontend, rendering, media,
  and eventually gameplay systems connected into executables.
- **Native tooling:** reusable format, scripting, editor, viewer, and modding
  knowledge produced by the reverse engineering.

Where to look first: [ROADMAP.md](docs/ROADMAP.md) is the single task list (done / in flight /
next), [HANDOFF.md](docs/HANDOFF.md) is the one-page resume point, [ARCHITECTURE.md](docs/ARCHITECTURE.md)
explains the source layers, [BUILDING.md](docs/BUILDING.md) the toolchain, and
[CONTRIBUTING.md](CONTRIBUTING.md) the landing rules. The full strategy is in
[FULL_DECOMP.md](docs/pipeline/FULL_DECOMP.md); every other document is listed in [docs/INDEX.md](docs/INDEX.md).

**On readability.** Early landed code modelled `this` with throwaway local structs
(`struct T { char pad[0x1a8]; ... }`). Since 2026-09-07 the class layouts recovered from the debug
PDB are emitted as compilable, PDB-named headers in `rebuild/include/engine/`, and landed functions are
retyped onto them only when the rewritten file still compiles to the identical retail bytes. The
"Readability" rows in the metrics below track that conversion; the remaining generic declarations are
a worklist, not the intended end state.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="current-progress"></a>
## 2. Current progress

| Snapshot | Current result |
|---|---:|
| Catalogued retail functions | **49,568** |
| Verified functional or matching reconstruction | **18,764 · 37.86%** |
| Byte-identical reconstruction | **8,115 · 16.37%** |
| Accepted analysis naming quality | **99.211%** |
| Runnable milestone | **Boot movies + interactive frontend, Saved Games, and Options screens** |

Current functional-or-matching coverage is **37.86%** of the 49,568-function catalog.
Of that verified set, **16.37%** is byte-identical C++.
The first 5% byte-match milestone has been passed.

<details>
<summary><strong>Full generated metrics</strong></summary>

The canonical refresh updates this table from the checked compile, behavior,
parity, coverage, and naming reports:

| Track | Metric | Status |
|---|---|---:|
| Analysis DB | Functions catalogued | **49,568** |
| Analysis DB | Mechanically named (no `FUN_*`) | 100.000% |
| Analysis DB | Accepted naming quality | 99.211% |
| Analysis DB | Usable reconstruction/navigation names | 99.913% |
| Analysis DB | Calling convention known | 77.746% |
| Analysis DB | Complete non-`undefined` prototype | 69.174% |
| Reconstruction | Curated sources, VC7.1-compiled **and** behaviour-gated | **18,842** |
| Reconstruction | Verified functional or matching C++ | **18,764** (37.86%) |
| Reconstruction | — of which byte-**identical** C++ | 8,115 (16.37%) |
| Reconstruction | — of which hand-written asm bakes (grade `asm_bake`, **not** counted as reconstruction) | 292 |
| Readability | Landed sources typed onto shared PDB-named engine headers | 667 |
| Readability | Throwaway local `struct T {...}` declarations remaining | 4,410 |
| Reconstruction | Compiled sources still honestly `DIFFER` | 43 |
| Reconstruction | Compiled rows lacking a Ghidra function-start oracle | 0 |
| Auto-RE intake | Generated candidates / structural checker PASS | 1,066 / 1,053 |
| Boot path | GFMain direct-call sites proven | **257 / 257** (100.00%) |
| Boot path | Callable authored GFMain phases | **10 / 10** (100.00%) |
| Boot path | Current Phase 10 direct calls proven | **21 / 21** (100.00%) |
| Modern C++23 | GFMain phases promoted into the modern startup lane | **0 / 10** (0.00%) |

Counts above are from the 2026-09-08 canonical refresh:
`rebuild/manifest/status.json`, `rebuild/compile-gate/retail-parity.json`,
`rebuild/COVERAGE.md`, and the naming-quality reports.

Generated agent output is tracked separately and is never counted merely
because a structural checker accepted it. The 37.86% figure is intentionally the strict,
whole-executable denominator. Boot-path figures are a separate view of
the 3,952-byte `GFMain` coordinator, not an estimate of total engineering time.
The C++23 startup figure is deliberately separate too: exact x86 parity and
readable integration work do not count as a modern port merely because their
interfaces are becoming stable.

</details>

The public dashboard is refreshed only after the compile/parity pipeline has
settled. Unreviewed background-agent output does not silently change these
numbers.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="run-the-checkpoint"></a>
## 3. Run the checkpoint

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/build_bootstrap.ps1
```

Then launch the retail-layout checkpoint with the optional BuffJesus text
variant:

```powershell
.\rebuild\build\bootstrap-Release\FableTLC-Reconstruction-VisualCheckpoint.exe --buff-jesus
```

Useful switches:

| Switch | Behavior |
|---|---|
| `--buff-jesus` | Changes main-menu strings while preserving retail geometry, actions, videos, and assets. |
| `--skip-boot-videos` | Fast developer launch directly into the frontend. |
| `--retail-video` | Exercises the recovered retail movie path. |
| `--retail-video-upscaled` | Uses an explicitly generated local 2× video cache when available. |

The normal BuffJesus path includes Lionhead, Microsoft, and `intro_comp`
movies. Escape skips the current movie. Retail banks are read from the local
game installation at build/runtime; they are not committed to this repository.

For smoke commands, prerequisites, individual movie modes, and the honest
runtime boundary, see [rebuild/RUNNABLE.md](rebuild/RUNNABLE.md).

<p align="right"><a href="#contents">back to contents</a></p>

<a id="what-works-today"></a>
## 4. What works today

| Area | Verified state | Major remaining boundary |
|---|---|---|
| Analysis database | Nearly complete navigation-quality names with recovered prototypes and calling conventions | Resolve the remaining ambiguous types, function starts, and hard naming stragglers |
| Function reconstruction | See the generated metrics above (byte-identical genuine C++, functional-or-matching, asm bakes reported separately) | Continue strict promotion; retype landed code onto the shared PDB-named headers; de-bake the `asm_bake` grade |
| Startup | Retail-matched `WinMain`, all ten authored GFMain phases callable, connected exact `GFInitialise`, progress-display ownership | Replace the remaining GFMain dependency boundaries and pursue whole-coordinator retail parity |
| Frontend | Boot movies, retail-data-driven interactive menus, CUIState-driven main-menu/Save/Options rows, keyboard/mouse/controller navigation, Credits routing, profile/delete/new-profile branches, Continue-to-Saved-Games routing, and retail list/transition sounds | Visual sign-off is still open: compare identical retail/reconstruction captures; link native profile persistence and validated save action `0x11` only at their recovered manager/game boundaries |
| Controls | Interactive capture/cancel/apply, duplicate clearing, W/S/A/D movement expansion, arrow/WASD reset actions, and recovered wheel/arrow ingress for authored scrolling lists | Full 31-action Redefine materialization/filtering/coexistence, profile persistence, and remaining controller/action ownership |
| Video | Retail movie order, end-of-stream advancement, Escape skip, and D3D9 texture publication | Replace the compatibility presentation seam with reconstructed `CTexture`/`CMovie::Draw` submission |
| Particles and lighting | Asset reports, billboard/crossed-sprite contracts, blend state, shader assembly, environment/RSA lookup, sky inputs | Recover constant semantics, signed/special passes, shadows, and golden viewer captures |
| Modding research | Broad readers/writers for definitions, levels, terrain, meshes, animation, audio, text, quests, and saves | Consolidate them behind safe product workflows and runtime validation |
| Cut multiplayer | Player slots, event/package codecs, local initialization, and a grounded revival plan are documented | Reconstruct synchronization, seating, lifecycle, and transport before enabling anything |

This table is a landing-page summary. Detailed addresses, byte counts, evidence, and caveats
live in the subsystem documents (`docs/engine/`, `docs/formats/`, `docs/pipeline/`); the
chronological record is `docs/journal/`.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="roadmap"></a>
## 5. Roadmap

The roadmap is a checklist, not prose: [docs/ROADMAP.md](docs/ROADMAP.md). It has one
"current focus" block and seven lanes (repo health, symbols and types, byte-parity
reconstruction, runnable frontend, engine subsystem RE, quest-script recovery, modding
toolchain), each with `[x]` done / `[ ]` open items that point at their owning document.
Superseded plans are archived under `docs/journal/`.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="verification-pipeline"></a>
## 6. Verification pipeline

Every promoted function passes an evidence chain:

1. **Recover** — Ghidra, donor PDB/BSim evidence, runtime traces, and retail
   bytes establish identity, boundaries, ABI, and behavior.
2. **Author** — readable C++03/VC7.1-compatible source is placed in the
   address-sharded reconstruction tree.
3. **Test** — the original VC7.1 compiler builds the function and a focused
   behavior fixture exercises its contract.
4. **Compare** — object `.text` is compared with authoritative retail bytes,
   with only real COFF relocation fields normalized.
5. **Land** — only reviewed `MATCH`, `RELOCATION_MATCH`, or explicitly
   behavior-only results enter their corresponding honest totals.

The result is intentionally conservative:

- `MATCH` means byte-identical.
- `RELOCATION_MATCH` means identical after expected relocation normalization.
- `DIFFER` remains visible even when behavior passes.
- `ORACLE_MISSING` is never guessed into parity.
- Generated/structural `PASS` is intake, not reconstruction credit.

Implementation details:

- [decomp pipeline](tools/decomp_pipeline/README.md)
- [source architecture](docs/ARCHITECTURE.md)
- [parity triage](docs/pipeline/PARITY_TRIAGE.md)
- [methodology](docs/pipeline/METHODOLOGY.md)
- [toolchain setup](docs/BUILDING.md)

<p align="right"><a href="#contents">back to contents</a></p>

<a id="documentation"></a>
## 7. Documentation

Inspired by documentation-first projects, this README is the index; deep
technical material lives in focused documents.

### Start here

| Document | Purpose |
|---|---|
| [HANDOFF.md](docs/journal/HANDOFF_ARCHIVE.md) | Authoritative latest checkpoint and exact resume state |
| [ACTIVE_TASK_LIST.md](docs/journal/2026-09/ACTIVE_TASK_LIST.md) | Ranked work with acceptance gates |
| [rebuild/RUNNABLE.md](rebuild/RUNNABLE.md) | Build, launch, smoke, and runtime-boundary instructions |
| [TOOLCHAIN.md](docs/BUILDING.md) | Ghidra, BSim, FSE, VC7.1, and local prerequisites |
| [CAPABILITY_INDEX.md](docs/engine/CAPABILITY_INDEX.md) | Index of recovered tooling and format capabilities |

### Reconstruction

| Document | Purpose |
|---|---|
| [FULL_DECOMP.md](docs/pipeline/FULL_DECOMP.md) | Full-decomp strategy and promotion model |
| [TECHNICAL_BRIEF.md](docs/journal/2026-07/TECHNICAL_BRIEF.md) | Current architecture, evidence grades, boot status, and modernization boundaries |
| [SOURCE_ARCHITECTURE.md](docs/ARCHITECTURE.md) | Generated, parity, and modern-source boundaries |
| [BSIM_PORT.md](docs/pipeline/SYMBOLS.md) | Donor-symbol porting runbook |
| [DECOMP_ACCELERATORS.md](docs/pipeline/DECOMP_ACCELERATORS.md) | Automation and throughput options |
| [UNICORN_EMULATION_PLAN.md](docs/pipeline/UNICORN_EMULATION_PLAN.md) | Optional bounded x86 behavior-oracle pilot |

### Runtime, UI, and rendering

| Document | Purpose |
|---|---|
| [FRONTEND_FORMAT.md](docs/formats/FRONTEND_FORMAT.md) | Recovered frontend definitions, tables, actions, and transforms |
| [VIDEO_SYSTEM_RE.md](docs/engine/VIDEO_SYSTEM_RE.md) | `CVideoSys`/`CMovie`, DirectShow, and texture-presentation recovery |
| [PARTICLE_LIGHTING_VIEWER_HANDOFF.md](docs/journal/2026-08/PARTICLE_LIGHTING_VIEWER_HANDOFF.md) | Particles, billboards, RSA/environment lighting, shaders, sky, and shadows |
| [UI_UPSCALE_PLAN.md](docs/modding/README.md#ui_upscale_plan) | Optional high-resolution cache policy and validation |
| [COOP_REVIVAL.md](docs/modding/README.md#coop_revival) | Evidence and safe order for the cut multiplayer system |

### Content and modding

| Document | Purpose |
|---|---|
| [COMPREHENSIVE_MODDING_ENVIRONMENT.md](docs/modding/README.md#comprehensive_modding_environment) | Overall tool/editor environment |
| [CONTENT_AUTHORING_PLAN.md](docs/modding/README.md#content_authoring_plan) | Safe content-creation workflow |
| [SYSTEMS_ANALYSIS.md](docs/engine/SYSTEMS_ANALYSIS.md) | Per-subsystem maps and moddability verdicts |
| [FINDINGS.md](docs/journal/FINDINGS_LOG.md) | Cross-checked technical findings |
| [FABLE_TLC_REPORTED_BUGS.md](docs/engine/FABLE_TLC_REPORTED_BUGS.md) | Evidence template for reported game defects |

The `docs/` directory contains focused references for definitions, BIG
archives, textures, meshes, animation, audio, terrain, navigation, quests,
scripts, saves, and editor integration.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="repository-layout"></a>
## 8. Repository layout

| Path | Contents |
|---|---|
| `rebuild/` | Buildable reconstruction, tests, manifests, oracles, integration, and parity reports |
| `lift/` | Auto-RE candidate reports, configuration, and durable worker state |
| `ghidra_out/` | Reproducible labels, APIs, decompilation exports, and naming audits |
| `tools/` | Ghidra scripts, promotion pipeline, reports, asset/format tools, and smokes |
| `docs/` | Technical references, plans, runbooks, and handoffs |
| `refs/` | FSE and other compact reference manifests |
| `work/`, `snapshots/`, `dist/` | Ignored local experiments, generated artifacts, and share packages |

See [WORKSPACE_LAYOUT.md](docs/pipeline/WORKSPACE_LAYOUT.md) for ownership and cleanup
rules.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="project-boundaries"></a>
## 9. Project boundaries

- No retail executable, artwork, video, archive, PDB, or decoded asset is
  committed.
- Retail-derived screenshots, texture decodes, model renders, Blender files,
  and byte-oracle extracts are local-only test outputs and are ignored by Git.
  Tests must generate them from a user-supplied installation outside the
  repository.
- A local retail installation is used as read-only evidence and asset input.
- Generated agent code is never treated as verified merely because it exists.
- A visually similar checkpoint is not claimed as the complete retail
  renderer.
- A behavior-equivalent function is not claimed as byte-identical.
- Experimental dependencies such as Unicorn remain optional and isolated from
  canonical builds until their licensing and technical boundaries are proven.
- Any tool capable of modifying game data must use explicit staging,
  validation, backup, and rollback paths.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="why-this-is-tractable"></a>
## 10. Why this is tractable

- `Fable.exe` is a clean MSVC PE32 image without a packer or DRM wrapper.
- FableScriptExtender contributes a large native scripting/API map.
- PDB-bearing Lionhead binaries provide rich donor names and type lineage.
- BSim and reproducible overrides transfer that evidence into retail.
- The original VC7.1 compiler is available for instruction-level comparison.
- Two decades of community research cover many data formats while this project
  concentrates on engine internals and faithful reconstruction.
- Every promotion is measurable, reviewable, and reversible.

The destination is ambitious, but the next step is always concrete: identify a
boundary, recover a contract, compile it with the original toolchain, test it,
compare it with retail, and only then count it.

<p align="right"><a href="#contents">back to contents</a></p>
