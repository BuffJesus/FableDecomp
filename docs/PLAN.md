# Fable: The Lost Chapters — Decompilation Plan

*Created 2026-07-17. Project root: `D:\Documents\FableTLC`. Target binary:
`C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe`.*

This plan adapts the reverse-engineering methodology proven on the **Fable2RE** project
(`D:\Documents\Fable2RE`) to Fable: The Lost Chapters — but the two jobs are fundamentally
different, and that difference sets the whole strategy.

---

## 0. The key insight: TLC is *not* Fable 2

| | Fable 2 (Fable2RE) | Fable: TLC (this project) |
|---|---|---|
| Platform | Xbox 360 | **Native PC** |
| CPU | PowerPC (big-endian) | **x86-32 (little-endian)** |
| Binary | Encrypted `default.xex` (XEX2) | **Clean `Fable.exe` — PE32, plain MSVC sections, NO packer/SteamStub** |
| Image base | guest 0x82000000 | **0x400000** (verified in the FSE log) |
| To even *read* the code | XEXLoaderWV to decrypt + static **recompilation** (ReXGlue) to run | **Ghidra loads it directly.** No decrypt, no recomp, no runtime. |
| Existing RE community | ~none (bleeding edge) | **~20 years mature** (fabletlcmod.com, Fable Explorer, FSE, unofficial patch) |
| Scripting hook | had to be reversed from scratch | **FableScriptExtender already did it** — 931-function API map ships in `refs/fse_api_manifest.json` |

Consequences:

- **Drop the entire recompilation track.** There is nothing to recompile — the game is
  already native x86 and runs on this machine. Fable2RE's ReXGlue / codegen / dangling-goto
  fixers / heap glue are irrelevant here.
- **This is a pure static-analysis + live-instrumentation RE project**, aimed at *understanding*
  and *extending* the engine (modding), not porting it.
- **We start with an enormous head start** that Fable2RE never had: a clean binary, a working
  script-injection DLL (FSE) already hooked into the engine, ~2300 named API entries, and two
  decades of community format documentation. The work is *filling in the C++ engine internals
  between the known edges*, not bootstrapping from zero.

Verified facts (this session):
- `Fable.exe`: PE32, machine `0x14c` (x86-32), ImageBase `0x400000`, 6 sections
  (`.text` 14.8 MB, `.rdata` 1.3 MB, `.data` 0.8 MB, `.idata`, `_PDATA`, `.rsrc`). No packer signature.
- `_PDATA` section + Xbox `titleid 4D530937` (in `Fable.exe.cfg`) confirm the PC build descends
  from the **Xbox codebase** — so structures/naming rhyme with the Fable2 Xbox findings.
- FSE runtime log confirms: module base `0x400000`, native-script-registration hook, a Lua entity
  host vtable, and a working Sol2 Lua bridge over `CWorld`/`CThingManager`/`CHero`/`CScriptThing`.

---

## 1. Goals (in priority order)

**★ Primary goal (user directive, 2026-07-17): a FULL decompilation of `Fable.exe`** — a complete,
readable, buildable C/C++ source reconstruction of the game, not just a modding surface. See
`docs/FULL_DECOMP.md` for the dedicated strategy; this is the destination the phases below climb toward.

The remaining goals are **milestones on the way to** (and dividends of) the full decomp:

1. **A fully-labelled Ghidra database of `Fable.exe`** — every function named/typed and decompiling
   to readable pseudo-C. This is the substrate the source reconstruction is lifted from.
2. **A native modding surface** — the C++ functions/structs a mod (or FSE-style DLL) can call/hook,
   beyond what FSE exposes. Falls out naturally once subsystems are decompiled.
3. **Deeper modding capability than the current toolchain** — new engine behaviours, not just data
   edits (EgoCore/Fable Explorer) or Lua quests (FSE).
4. **A buildable, parity-checked source tree** — the end state of the full decomp: recovered C that
   compiles (with the era MSVC 2003 / VC7.1) into a functionally-equivalent binary, verified against
   the original.

Non-goals: porting to another platform (unnecessary — it's already native PC); shipping a specific
mod (downstream of the RE); Fable Anniversary (standing "TLC only" directive).

### Current integration priority (2026-07-25)

The next product milestone is a reconstructed process that reaches the game loop, not a polished
port shell. Work proceeds through the staged gates in `rebuild/RUNNABLE.md`: owned startup,
GFMain/GFInitialise, data bootstrap, then a minimal update/render loop. Installer work, replacement
settings menus, x64 migration, and broad C++23 refactoring stay behind that runnable milestone.
Small modern proofs may continue when they lock down a recovered contract, but they do not displace
the VC7.1 boot-spine dependency closure.

---

## 2. What we inherit from Fable2RE (the transferable methodology)

Fable2RE's RE track is directly reusable; only the loader changes (no XEX). Already copied into
this project:

- **`tools/ghidra_scripts/`** — the full Ghidra scripting suite (30 scripts): `LabelApply.java` /
  `LabelDump.java` (TSV-driven mass (re)naming — the backbone of the labelling pipeline),
  `FindLuaNatives{,2,3,4,5}.java` (locate `luaL_Reg` tables / native bindings — TLC uses Lua too),
  `FindStrXref.java` / `FindRefs.java` (string→function pinning, the primary way to name
  functions), `DecompFuncs.java` / `DisasmRange.java` / `DumpFuncsInRange.java` (batch decompile
  dumps for offline reading), `FindAccessors.java`, `FindCodePointers.java`, `ParsePdataInBlobs.java`
  (relevant — TLC has a `_PDATA` section), `WhatIs.java` / `NameAt.java` / `ReadStr.java` (probes).
- **`tools/lua_mod/`** — Lua disassembler (`luadis.py` / `luadis51.py`), bytecode tooling, and
  script-injection patterns. Fable2's Lua work (`Shaw_Johnathan_LuaScripting_LuaInFableII.pdf`,
  copied to `refs/`) documents Lionhead's Lua-in-Fable design that TLC shares.
- **The documentation discipline**: `HANDOFF / PROGRESS / FINDINGS / TOOLCHAIN / ROADMAP /
  SYSTEMS_ANALYSIS` doc set, updated with evidence (log/cdb/Ghidra), never assumption.
- **The labelling loop** (§4 below) — pick a subsystem → find an anchor (string/import/xref) →
  name outward → decompile → document → feed names back via `LabelApply`.
- **AI-driven RE via GhidraMCP** — the Ghidra MCP HTTP server (`REPlugins/GhidraMCP`, port 8089,
  ~249 RE-automation tools) lets Claude drive decompile/rename/xref directly. Same setup as Fable2RE.
- **`auto-re-agent`** (`D:\Documents\Fable2RE\auto-re-agent`) — the AI+Ghidra parity-checked
  function-reversing agent, for producing compileable C for chosen functions.

What we **do not** carry over: XEXLoaderWV (Xbox-only), ReXGlue SDK, codegen, `fix_dangling_gotos`,
`add_call_targets`, the recomp `src/` glue, cdb guest↔host membase math. None apply to a native exe
(ordinary x86 debugging with x64dbg/WinDbg applies instead).

---

## 3. The head start — integrating the existing ecosystem

Researched this session. **We build on these rather than re-deriving them.**

### 3a. FableScriptExtender (FSE) — the single biggest asset
- Repo: `github.com/eeeeeAeoN/FableScriptExtender` (v0.75, Jan 2026, Sol2-based). Already installed
  and running in the user's game (`.../Fable The Lost Chapters/FSE/`, enabled via `Mods.ini`).
- It **already reverse-engineered the game's C++ scripting API**: `CScriptThing`, `CWorld`,
  `CThingManager`, `CHero`, plus VMTables — and injects a Lua VM via a JMP hook at `0xCDB355`
  (native script-registration site).
- **`refs/fse_api_manifest.json` (895 KB, 931 functions)** is a ready-made symbol/type catalog.
  `tools/fse_import/fse_manifest_to_ghidra.py` converts it into:
    - **`fse_api.h`** → parse into Ghidra (`File > Parse C Source` → *Apply Function Datatypes*)
      to seed real names/signatures/types instead of blank `sub_XXXXXX`.
    - **`fse_api_index.md`** → the per-category RE roadmap (Entity API 88 / Quest API 843).
- **FSE source is already on this machine** — `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\`
  (46 `.cpp/.h`). No GitHub clone needed. Read `dllmain.cpp` + the API wrappers: they contain the
  literal hook addresses and the `g_fableBase`-relative pointers into the engine. The manifest is
  `generated-from-header`, i.e. produced from these headers — so the headers are the authoritative
  source of the API signatures.
- **Action:** every FSE wrapper *calls a real engine function at a fixed 0x4xxxxx address* — those
  call sites (and the pointer-init tables in FSE source) are the fastest way to pin hundreds of
  engine functions in Ghidra. FSE is both a symbol source **and** the proven injection framework we
  extend for native mods (goal 3).

### 3a-bis. FQT (Fable Quest Tool) — the user's own quest editor (downstream consumer)
- `D:\Code\FQT\FQT\` — a WPF/.NET 8 visual node-graph quest editor that **generates FSE Lua**
  (repo also mirrored at `D:\Code\Fable\FQT\Projects`). It is the authoring front-end over FSE.
- Its `DOCS/GAME_DATA.md` states there is **"no automated extraction pipeline yet"** for its
  Regions/Creatures/Objects/Abilities lists — they are hand-curated. **A decompiled `Fable.exe`
  is exactly that missing pipeline**: the engine's internal name tables (entity defs, region ids,
  ability ids) can be dumped from Ghidra/`.data` and exported straight into FQT's `GameData.cs`.
  This is the concrete payoff that ties this RE project to the tool the user already ships.
- Relevant FQT docs to read: `DOCS/FSE_FQT_INTEGRATION_CONTRACT.md`, `DOCS/GAME_DATA.md`,
  `SourceFilesToReference/fse-quest-creator-prompt-v2.md`.

### 3b. fabletlcmod.com — the format bible
- Wiki + forums, ~20 years of community RE of TLC's **data formats**: `.big`/`.bin` archives,
  `.lev` (levels), `.tng` ("things"/entity placement), `.stb`, `.gtg`, `.wad`, graphics/bank formats,
  `def.xml` / `CompiledDefs`. The UE6Verse project already leans on much of this (EgoCore, `.big`
  banks) — cross-reference `memory/fable_defbank_decode.md` etc.
- **Action:** mirror the key wiki pages into `refs/formats/` (offline copy) as the data-format
  reference. The **exe RE (this project) and the data-format RE (community) are complementary**:
  Ghidra shows *how the engine parses* a `.tng`; the wiki documents *the byte layout*. Confirm one
  against the other.

### 3c. Tooling to reference (don't rebuild)
- **Fable Explorer** — BIG/BIN archive + FMP mod-package editor, script.bin editing. The de-facto
  data-mod tool. Our native RE complements it (engine behaviour vs. data).
- **FableTools 0.7** — BIZ↔BIG conversion, WAD import/export.
- **Fable TLC Toolpack** (Nexus mods/4) — the "lost & forgotten" tool collection.
- **Wyntilda/Unofficial-Fable-Patch** — a bug-fixing RE effort; its documented bugs/addresses are
  a cross-check for our own findings.
- **Keshire/Fable3LUADecompiler**, **JustSomeGuy1234/Fable2Modding** — sibling-game Lua RE, useful
  for the shared Lua lineage.

---

## 4. The core RE loop (per subsystem)

Same loop Fable2RE used, minus the recomp steps:

1. **Anchor** — find a way in: a diagnostic/UI **string** (`FindStrXref.java`), a named **import**
   (`.idata`), an **FSE call site** (§3a), or a known **wiki-documented format** whose parser we
   want (`.tng` loader, save format, etc.).
2. **Name outward** — from the anchor, follow xrefs/callees; apply names+types (`LabelApply.java`
   from a TSV, or GhidraMCP `rename`/`set_function_prototype` live).
3. **Type** — recover the relevant struct/vtable layout; apply via Ghidra Data Type Manager
   (seeded by `fse_api.h`).
4. **Decompile & read** — `DecompFuncs.java` dumps to `ghidra_out/`; read, annotate.
5. **Verify with evidence** — cross-check against: the FSE manifest, the fabletlcmod wiki byte
   layout, live behaviour (x64dbg breakpoint on the function, or an FSE Lua probe), or the
   Fable2 Xbox findings (shared codebase). Never assert from a single source.
6. **Document** — append to `docs/FINDINGS.md` + `docs/SYSTEMS_ANALYSIS.md`; commit the label TSV
   so the DB is reproducible (`git`-track `ghidra_out/labels_*.tsv`, not the `.rep`).

Loop-prevention & non-destructive rules carry over from the UE6Verse CLAUDE.md (don't repeat a
failing call >2×; verify with evidence; log every change).

---

## 5. Phased roadmap

### Phase 0 — Environment (½ day)
- [ ] Install Ghidra 12.1.2 (already at `D:\ghidra_12.1.2_PUBLIC` / `D:\Subuwu\tools\ghidra-public`
      per Fable2RE) + the `REPlugins/GhidraMCP` extension (port 8089). **No XEXLoaderWV needed.**
- [ ] `analyzeHeadless <ghidra_proj> FableTLC -import "<...>\Fable.exe"` — clean PE32 auto-analysis
      (minutes, not the hours the 21 MB XEX took). Log to `ghidra_out/analyze.log`.
- [ ] `python tools/fse_import/fse_manifest_to_ghidra.py` → parse `fse_api.h` into the DB
      (`File > Parse C Source`), apply function datatypes.
- [ ] Clone FSE source → `refs/FableScriptExtender/`; mirror key fabletlcmod wiki pages → `refs/formats/`.

### Phase 1 — Anchor & map the skeleton (1–2 days)
- [ ] Import-table + RTTI pass: MSVC RTTI is usually intact → recover C++ class names/vtables
      wholesale (Ghidra's RTTI analyzer + `FindAccessors`). This alone names a large fraction of classes.
- [ ] String-xref sweep (`FindStrXref`) → name error/log/asset-path handlers.
- [ ] FSE call-site pass: for each of the 931 manifest functions, locate the engine target it wraps
      and apply the manifest name/signature. Produces `ghidra_out/labels_fse_targets.tsv`.
- [ ] Lua-natives pass (`FindLuaNatives*`) → the game's own Lua binding tables.
- [ ] **Deliverable:** `docs/SYSTEMS_ANALYSIS.md` skeleton — every major subsystem located and named
      at the entry-point level (World, ThingManager, Hero, Quest/Script VM, save/load, renderer,
      resource/BIG loader, combat, Will/magic, UI).

### Phase 2 — Subsystem deep-dives (ongoing, pick by modding value)
Priority order (highest modding leverage first — mirrors Fable2RE's SYSTEMS_ANALYSIS):
- [ ] **Script/Quest VM & the FSE hook region** (0xCDB355 neighbourhood) — the native-mod seam.
- [ ] **CThingManager / CScriptThing / entity model** — spawning, entity defs, appearance.
- [ ] **Resource/BIG/`.tng`/`.lev` loaders** — cross-checked against the wiki; the asset-mod seam.
- [ ] **Combat / Will (magic) / items & appearance** — the "custom spells / gameplay" modding target.
- [ ] **Save format** — for save-editing + persistence of mod state.
- [ ] **Text/Babel & UI** — localisation + HUD modding.

### Phase 3 — Native modding surface (after enough of Phase 2)
- [ ] `docs/NATIVE_MODDING_API.md` — catalogue the callable/hookable native functions (beyond FSE),
      with addresses, signatures, and a `register_native`-style extension design layered on FSE.
- [ ] Prototype one native capability FSE/Fable Explorer *can't* do, to prove the surface.

### Phase 4 — FULL source-level decompilation (the primary goal — full detail in `docs/FULL_DECOMP.md`)
Not a stretch anymore: the destination. Phases 0–3 make it *possible* (a fully-labelled, fully-typed
DB); Phase 4 turns that into buildable source, module by module.
- [ ] Achieve **100% pseudo-C coverage** first: every function in `.text` decompiles cleanly in
      Ghidra (no undefined/failed functions) — driven by the `_PDATA` bounds + RTTI + auto-analysis.
- [ ] Stand up the **build harness**: an MSVC 2003 (VC7.1 — the game's own toolset) project that
      links recovered translation units against the original, so each lifted module can be
      **parity-checked** (byte/behaviour diff) against `Fable.exe`.
- [ ] Lift module-by-module (priority = the Phase-2 subsystem order) from Ghidra pseudo-C →
      compileable C, using **`auto-re-agent`** (AI+Ghidra parity loop) for bulk and hand-work for
      the hard functions. Cross-check asset/resource modules against **EgoCore** source (the format
      answer key) and gameplay/script modules against the **FSE** manifest.
- [ ] Track coverage as a first-class metric (functions lifted / total, % bytes matched).

---

## 6. Immediate next actions (start here next session)
1. Stand up Ghidra + GhidraMCP; headless-import `Fable.exe` (Phase 0).
2. Run the RTTI + FSE-target + string-xref passes (Phase 1) to get the first `SYSTEMS_ANALYSIS.md`.
3. Clone FSE source and mirror the fabletlcmod format wiki into `refs/`.
4. Keep `docs/HANDOFF.md` current — it is the resume point.

See `docs/TOOLCHAIN.md` for exact commands, `docs/ECOSYSTEM.md` for the full community-tool survey,
and `docs/METHODOLOGY.md` for the Ghidra-scripting mechanics inherited from Fable2RE.
