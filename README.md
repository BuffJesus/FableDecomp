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

The detailed strategy is in [FULL_DECOMP.md](docs/FULL_DECOMP.md). The latest
working memory and exact resume point are always at the top of
[HANDOFF.md](docs/HANDOFF.md).

<p align="right"><a href="#contents">back to contents</a></p>

<a id="current-progress"></a>
## 2. Current progress

| Snapshot | Current result |
|---|---:|
| Catalogued retail functions | **49,568** |
| Verified functional or matching reconstruction | **4,984 · 10.05%** |
| Byte-identical reconstruction | **2,751 · 5.55%** |
| Accepted analysis naming quality | **99.211%** |
| Runnable milestone | **Boot movies + interactive frontend, Saved Games, and Options screens** |

Current functional-or-matching coverage is **10.15%** of the 49,568-function catalog.
Of that verified set, **5.57%** is byte-identical C++.
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
| Analysis DB | Calling convention known | 77.665% |
| Analysis DB | Complete non-`undefined` prototype | 69.045% |
| Reconstruction | Curated sources, VC7.1-compiled **and** behaviour-gated | **5,346** |
| Reconstruction | Verified functional or matching C++ | **5,031** (10.15%) |
| Reconstruction | — of which byte-**identical** C++ | 2,762 (5.57%) |
| Reconstruction | Compiled sources still honestly `DIFFER` | 205 |
| Reconstruction | Compiled rows lacking a Ghidra function-start oracle | 128 |
| Auto-RE intake | Generated candidates / structural checker PASS | 832 / 817 |
| Boot path | GFMain direct-call sites proven | **48 / 257** (18.68%) |
| Boot path | Callable authored GFMain phases | **2 / 10** (20.00%) |
| Boot path | Current Phase 3 direct calls proven | **29 / 34** (85.29%) |

Counts above are from the 2026-07-28 canonical refresh:
`rebuild/manifest/status.json`, `rebuild/compile-gate/retail-parity.json`,
`rebuild/COVERAGE.md`, and the naming-quality reports.

Generated agent output is tracked separately and is never counted merely
because a structural checker accepted it. The 10.15% figure is intentionally the strict,
whole-executable denominator. Boot-path figures are a separate view of
the 3,952-byte `GFMain` coordinator, not an estimate of total engineering time.

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
| Function reconstruction | 5,299 VC7.1-compiled and behavior-gated sources; 4,984 verified functional/matching | Continue strict promotion without counting structural-only generated code |
| Startup | Retail-matched `WinMain`, recovered Phase 1/2 path, connected `GFInitialise`, progress-display ownership | Complete GFMain Phase 3 onward and replace authored integration seams |
| Frontend | Boot movies, interactive menus, live generated title-rule and selection components, CUIState-driven main-menu/Save/Options rows, keyboard/mouse navigation, Continue-to-Saved-Games routing, and retail list/transition sounds | Visual sign-off is still open: measure reconstructed screens against identical retail captures before calling them presentation-parity; then connect validated save action `0x11` to the reconstructed main-game load boundary |
| Controls | Interactive capture/cancel/apply, duplicate clearing, W/S/A/D movement expansion, arrow/WASD reset actions | Full 31-action scrolling, filtering, coexistence, and profile persistence |
| Video | Retail movie order, end-of-stream advancement, Escape skip, and D3D9 texture publication | Replace the compatibility presentation seam with reconstructed `CTexture`/`CMovie::Draw` submission |
| Particles and lighting | Asset reports, billboard/crossed-sprite contracts, blend state, shader assembly, environment/RSA lookup, sky inputs | Recover constant semantics, signed/special passes, shadows, and golden viewer captures |
| Modding research | Broad readers/writers for definitions, levels, terrain, meshes, animation, audio, text, quests, and saves | Consolidate them behind safe product workflows and runtime validation |
| Cut multiplayer | Player slots, event/package codecs, local initialization, and a grounded revival plan are documented | Reconstruct synchronization, seating, lifecycle, and transport before enabling anything |

This table is a landing-page summary. Detailed addresses, byte counts, evidence,
caveats, and chronological checkpoints belong in
[HANDOFF.md](docs/HANDOFF.md), [ACTIVE_TASK_LIST.md](docs/ACTIVE_TASK_LIST.md),
and the subsystem documents below.

<p align="right"><a href="#contents">back to contents</a></p>

<a id="roadmap"></a>
## 5. Roadmap

### P0 — retail frontend and renderer parity

- Finish live `CTable`, `CList`, and `CKeyRedefiner` ownership and state flow,
  including detail-row extraction, controller input, and remaining actions.
- Add deterministic retail-vs-reconstruction screenshot comparisons.
- Continue GFMain Phase 3 and replace authored rendering/media seams.
- Recover particle, RSA-lighting, sky, and shadow runtime contracts.

### P0 — product-facing native features

- Complete and runtime-test the secret-hunt quest/content package.
- Prioritize high-value Quest, NPC, frontend, and renderer functions.
- Keep game-install changes transactional, hashed, and reversible.

### P1 — reconstruction throughput

- Promote short deterministic candidates through VC7.1 compile, behavior, and
  retail-byte gates.
- Diagnose compact `DIFFER` residues with disassembly feedback, the permuter,
  and the optional Unicorn function-oracle pilot.
- Repair boundaries and prototypes before re-authoring repeated failures.

### Later — complete engine and game

- Connect the full engine initialization, archive/runtime asset loading,
  renderer, world, scripting, save, audio, and gameplay loops.
- Replace compatibility and authored integration layers only when the recovered
  native owners are ready.
- Treat modern C++ subsystem code as a readable consumer layer, never as an
  automatic byte-parity claim.

The ranked, acceptance-gated queue is
[ACTIVE_TASK_LIST.md](docs/ACTIVE_TASK_LIST.md). Broader sequencing is in
[PLAN.md](docs/PLAN.md).

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
- [source architecture](docs/SOURCE_ARCHITECTURE.md)
- [parity triage](docs/PARITY_TRIAGE.md)
- [methodology](docs/METHODOLOGY.md)
- [toolchain setup](docs/TOOLCHAIN.md)

<p align="right"><a href="#contents">back to contents</a></p>

<a id="documentation"></a>
## 7. Documentation

Inspired by documentation-first projects, this README is the index; deep
technical material lives in focused documents.

### Start here

| Document | Purpose |
|---|---|
| [HANDOFF.md](docs/HANDOFF.md) | Authoritative latest checkpoint and exact resume state |
| [ACTIVE_TASK_LIST.md](docs/ACTIVE_TASK_LIST.md) | Ranked work with acceptance gates |
| [rebuild/RUNNABLE.md](rebuild/RUNNABLE.md) | Build, launch, smoke, and runtime-boundary instructions |
| [TOOLCHAIN.md](docs/TOOLCHAIN.md) | Ghidra, BSim, FSE, VC7.1, and local prerequisites |
| [CAPABILITY_INDEX.md](docs/CAPABILITY_INDEX.md) | Index of recovered tooling and format capabilities |

### Reconstruction

| Document | Purpose |
|---|---|
| [FULL_DECOMP.md](docs/FULL_DECOMP.md) | Full-decomp strategy and promotion model |
| [SOURCE_ARCHITECTURE.md](docs/SOURCE_ARCHITECTURE.md) | Generated, parity, and modern-source boundaries |
| [BSIM_PORT.md](docs/BSIM_PORT.md) | Donor-symbol porting runbook |
| [DECOMP_ACCELERATORS.md](docs/DECOMP_ACCELERATORS.md) | Automation and throughput options |
| [UNICORN_EMULATION_PLAN.md](docs/UNICORN_EMULATION_PLAN.md) | Optional bounded x86 behavior-oracle pilot |

### Runtime, UI, and rendering

| Document | Purpose |
|---|---|
| [FRONTEND_FORMAT.md](docs/FRONTEND_FORMAT.md) | Recovered frontend definitions, tables, actions, and transforms |
| [VIDEO_SYSTEM_RE.md](docs/VIDEO_SYSTEM_RE.md) | `CVideoSys`/`CMovie`, DirectShow, and texture-presentation recovery |
| [PARTICLE_LIGHTING_VIEWER_HANDOFF.md](docs/PARTICLE_LIGHTING_VIEWER_HANDOFF.md) | Particles, billboards, RSA/environment lighting, shaders, sky, and shadows |
| [UI_UPSCALE_PLAN.md](docs/UI_UPSCALE_PLAN.md) | Optional high-resolution cache policy and validation |
| [COOP_REVIVAL.md](docs/COOP_REVIVAL.md) | Evidence and safe order for the cut multiplayer system |

### Content and modding

| Document | Purpose |
|---|---|
| [COMPREHENSIVE_MODDING_ENVIRONMENT.md](docs/COMPREHENSIVE_MODDING_ENVIRONMENT.md) | Overall tool/editor environment |
| [CONTENT_AUTHORING_PLAN.md](docs/CONTENT_AUTHORING_PLAN.md) | Safe content-creation workflow |
| [SYSTEMS_ANALYSIS.md](docs/SYSTEMS_ANALYSIS.md) | Per-subsystem maps and moddability verdicts |
| [FINDINGS.md](docs/FINDINGS.md) | Cross-checked technical findings |
| [FABLE_TLC_REPORTED_BUGS.md](docs/FABLE_TLC_REPORTED_BUGS.md) | Evidence template for reported game defects |

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

See [WORKSPACE_LAYOUT.md](docs/WORKSPACE_LAYOUT.md) for ownership and cleanup
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
