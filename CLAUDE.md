# FableTLC — AI agent guide

Reverse-engineering project on the **native PC** `Fable.exe` (Fable: The Lost Chapters, Steam).
Read `docs/PLAN.md` first, then `docs/HANDOFF.md` for where things stand.

## The one thing to get right
**TLC is native x86-32 Windows — NOT an Xbox port.** There is no recompilation, no XEX, no decrypt,
no packer. Do **not** copy the Fable2RE recomp workflow (ReXGlue / codegen / dangling-goto fixers /
guest↔host membase math). This is Ghidra static analysis + ordinary native (x32dbg/WinDbg) debugging
of a clean PE32 at ImageBase `0x400000`.

## Biggest assets (use them, don't re-derive)
- **FSE manifest** `refs/fse_api_manifest.json` — 931 reversed API functions. Run
  `python tools/fse_import/fse_manifest_to_ghidra.py` → `fse_api.h` (parse into Ghidra) +
  `fse_api_index.md` (RE roadmap).
- **FSE source (local):** `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\`
  — hook `0xCDB355`, `g_fableBase`-relative engine pointers = call targets to pin in Ghidra.
- **Community formats:** fabletlcmod.com wiki (byte layouts) — complements Ghidra's parsers.
- **Fable2RE** (`D:\Documents\Fable2RE`) — tooling + methodology + sibling-engine priors.

## Working rules (from Fable2RE + UE6Verse CLAUDE.md)
- **Evidence, not assumption** — cross-check every claim against ≥2 sources (FSE manifest, wiki,
  live x32dbg/FSE-Lua probe, Fable2 findings). Verify before asserting.
- **Reproducible DB** — record names as `ghidra_out/labels_*.tsv`, apply with `LabelApply.java`;
  git-track the TSV, not the `.rep`.
- **Loop prevention** — never repeat a failing call >2× with same args; after 2 failed attempts,
  stop and document the gap in `docs/FINDINGS.md`.
- **Document as you go** — `FINDINGS.md` (cited technical truth), `SYSTEMS_ANALYSIS.md` (per-subsystem
  map + moddability verdict), `PROGRESS.md`, `HANDOFF.md` (resume point).
- **Living gotchas** — when you solve a real problem, append a one-liner here.
- Ghidra `X86FunctionPurgeAnalyzer` on huge binaries (165k fns) goes log-silent for hours in a
  quadratic progress-reporting loop — it's working, not hung; verify with jstack + CPU sampling,
  never kill (headless saves only after analysis completes).
- PDB names can contain whitespace ("dynamic initializer for 'x'") which Ghidra symbols reject —
  `ApplyNames.java` sanitizes `\s+`→`_` before `setName`.

- Preserve MSVC decorated names on the first PDB/BSim `setName` attempt so `DemangleAll.java` still
  works; only fall back to `SymbolUtilities.replaceInvalidChars(...)` after Ghidra rejects a name.
- WinLibs mingw64 g++ builds can die at startup with 0xC0000139 (entrypoint not found) from runtime
  DLL mismatches on PATH - link `-static -static-libgcc -static-libstdc++` (done in FableForge).
- **★ Active build target since 2026-07-18: FableForge (`D:\Code\FableForge`)** - the C++ rewrite of
  the modding toolchain consuming this project's RE outputs. FQT is a donor, not the target.
- RTTI vtable-slot ports beat low-confidence BSim guesses when slot counts align; preserve the
  compare TSV, then use `LabelApplyForce.java` and demangle.
- `analyzeHeadless.bat` script args: cmd.exe splits on `=`, so `name=0xaddr` arrives as TWO args —
  pass alternating `name addr` pairs instead (bit DumpVerbSlices.java mid-run; had to kill+rerun).
- Current FableForge commit: `17b7b8e` (`M2: add STB static-map reader`). Implemented WAD/TNG/WLD/
  LEV/STB/catalog + `forge validate`; retail install validates CLEAN.
- Chest facts: `CChestDef::Transfer` is retail `0x004DE204`. Key requirement IS in `CChestDef`:
  `OpenerObject` at `+0x34`, `OpenersRequired` at `+0x38`. Rewards are elsewhere:
  `CContainerRewardHeroDef::ObjectFamilies` at `+0x28`, expanded by
  `CTCContainerRewardHero::GetRewardItems`.
- Ghidra DB after RTTI force pass: 49,082 functions, 40,187 named, 8,895 default-named. Bulk RTTI
  port source is `ghidra_out/labels_rtti_port.tsv`; conflict audit is
  `ghidra_out/rtti_port_compare.tsv`.
- Quest logic is compiled C++ (161-entry name→allocator table @ 0x00CD52D0, no quest VM) — see
  docs/QUEST_VM_RE.md. Trust FSE ASLR addresses over BSim names when they clash (0x00CB8110 is
  the CScriptBase ctor, not "CHeroMorphDef"; 0x00CBFAB8 is SetScriptActiveStatus).
- 3DAF anim payload = `u32 decompSize` + ONE raw LZO1X stream (no `[u16 clen]` frames — that's
  the texture/mesh framing); decompressed chunks are plain `[fourcc][u32 size]`, one XSEQ per
  bone track. EgoCore (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) is the whole-format
  answer key (defs/crc0, meshes, anim, lipsync, banks) — went STABLE 31.7.26; check it BEFORE byte-RE.
  See `docs/BIG_ANIM_FORMAT.md` §9, `docs/DEMON_DOOR_FACE.md`, and `docs/EGOCORE_ASSESSMENT_20260731.md`
  (full cross-ref: confirms crc0/3DAF/skinning, unblocks Mario-rig via `GltfAnimImporter`, adds
  `SpeechAnalyzer` WAV→lipsync; DO NOT copy EgoCore's `CDefStringTable::GetCRC` (std-CRC+tolower, wrong)
  or its `classIndex=0` — trust our byte-proven crc0 + dense index; filed EgoCore issue #4).
- Texture payloads: only MIP 0 is chunked-LZO; mips 1..n-1 are stored RAW. Info+24 (MipSize0) =
  on-disk mip-0 region size, 0 = all-raw payload (loader-accepted). DXT3 Info tail is `02 08`,
  not `03 04`. Writer: `tools/texture_build.py`; recipe: `docs/TEXTURE_WRITER.md`.
- Compiled-mesh Info `LODSizes[]` are LOD byte SIZES; every retail 1-LOD type-1 entry appends an
  UNCOUNTED "ghost LOD" (empty mesh block) after LOD0, and material lists end with a
  `DegenerateTriangles` sentinel (STATIC meshes only — retail skinned meshes carry none).
  New-mesh composer: `mesh_rw.compose_mesh` + `big_write.rebuild(adds=)`; SKINNED type-5
  via `skeleton=mesh_rw.clone_skeleton(donor)` (bone blocks cloned raw; weight bytes sum
  exactly 255 retail-wide, max 3 influences); recipe: `docs/MESH_COMPOSE.md`.
  (EgoCore 2026-07-31: ghost LOD actually applies to types 1/2/4/5 — we retail-verified 1/5, 2/4
  to-verify; mesh types = 1 static / 2 instanced (stride-36) / 3 physics-BBM-via-PhysicsIndex /
  4 particle / 5 skinned. See `docs/MESH_COMPOSE.md` EgoCore section.)

- Fable name hash = **crc0** = reflected CRC-32 poly 0xEDB88320, **seed 0, NO final inversion**
  (`CCharString::ComputeCRC32` 0x00404310) — keys names.bin CRCs, game.bin field tags, and the
  `map<unsigned_long,CDefClassInfo>` def registry (13593/13593 retail names verified). NOT
  `0xFFFFFFFF-crc32`, NOT zlib crc32. A game.bin def APPEND is engine-resolvable only if (1) the new
  names.bin CRC is crc0 (forge `bin.cpp` fixed) AND (2) the cloned payload's self global-entry-index
  back-refs are retargeted to the new landing index (component sub-defs are SHARED, leave them). Counts
  were never the bug. In-place `setEntryData` field edits sidestep both. See docs/DEF_LOAD_CONTRACT.md.
- Save edits: any SAVED_ENTITIES cell edit must patch the 36-byte cell descriptor
  (recLen=29+clen / clen / ulen) AND sectionLen AND chunk1_ulen, then re-sign — patching only
  the section length mis-frames the engine's record walk. Use tools/save_edit.py
  (report/set-gold/set-stat/set-qty/add-item); grammar in SAVE_ENTITY_GRAPH.md §9.5.

- Dialogue join: text.big SpeechBank names only the bank; the clip index <N> is in
  data\Defs\<bank>snds.bin = sorted {crc0("SND_"+entryName), soundID} pairs; soundID ==
  .lut clip Index == dialogue.big LIPSYNC id. Pipeline: tools/dialogue_pipeline.py,
  docs/DIALOGUE_PIPELINE.md. .lut MaxEntries < retail Count => not a count cap.

- New-map registration REQUIRES a FinalAlbion_RT.stb common-header chunk: OpenRetailStaticMap
  (0xB41E50) does an UNCHECKED map<name,offset> lookup per registered map -> garbage seek ->
  CTD 0xA2428A on miss. Writer + chunk layout: work/newlevel_experiment/
  assemble_forgetest_stage2.py; post-mortem NEW_LEVEL_ASSEMBLY.md par.9. WER Application-log
  fault offsets are the fastest crash triage (offset+0x400000 = Ghidra VA).
- Adding a D3D9 frontend texture (visual_boot_d3d9.cpp) requires registering it in BOTH the
  `FableInitialiseVisualD3D9` upload chain AND the `VisualRender2DAdapter`
  `RENDER2D_ADAPTER_ATTACH_TEXTURE` pointer->selectedTexture chain (~L1024-1101). Miss the attach
  chain and the texture uploads/validates/selects fine but its quads bind nothing and draw FLAT
  WHITE. Pinned the About-screen SPOOKY bg bug this way (2026-07-30); new frontend resource ids need
  a free slot (101-120 were taken; 116-119 are WAVE, so About=120, spooky=121/122).
- Decomp authoring lane (`verify_and_land.py`): it runs `html.unescape()` on `test_cpp` before
  compiling, so any `&<word>;` adjacency (e.g. `&marker;`) expands to a Unicode glyph -> VC7.1
  C3209 "Unicode identifiers not supported". Author tests without `&name;` sequences (rename the
  var, or take the address into a `void* p = &var;` first). Same file rewrites `__thiscall`->
  `__fastcall` and strips `static_assert`; never write the literal `__thiscall` keyword (VC7.1
  rejects it, C4234) — model this-in-ecx methods as real members or free `__fastcall(self,...)`.
- Workflow `args` arrive in the script as a JSON **string**, not a parsed value — guard with
  `const items = typeof args === 'string' ? JSON.parse(args) : args` before `.map`/`.length`.
- `verify_and_land.py` CANNOT auto-verify functions with an EMBEDDED JUMP TABLE (switch >~4 dense
  cases → `jmp [eax*4+table]` with the table inline in .text). Two failure modes in `obj_text`:
  (1) objdump `-d` splits the body at every internal `$Lxxx` local label, so the leaf-named block is
  just the pre-`jmp` head → reports `DIFFER(37v120)`; (2) the relocated table dwords are all-zero in
  the fresh .obj so objdump elides them as `...`, dropping 4·N bytes. Verify these by RAW COFF
  extraction (read .text section bytes start→next-non-`$`-symbol, mask reloc slots) + behavior test,
  NOT the harness. Proven: `CKeyRedefiner::GetSubTypeForAction` 0x557CA0 (RELOCATION_MATCH 120/120,
  source in docs/REDEFINE_INPUT_SYSTEM.md) + `AreAllowedToCoexist` 0x5578A0 (147/147). USE
  `tools/decomp_pipeline/verify_land_jumptable.py <land.json> <oracle.tsv> [--land]` — same interface
  as verify_and_land, imports it and swaps ONLY obj_text for the raw-COFF extractor (shared harness
  untouched, so a background crawl keeps using verify_and_land concurrently).
- Manifest boundary OVER-CAPTURE: a function's oracle span is [addr, next_manifest_addr); when the
  real next fn ISN'T in the manifest the span swallows inter-fn `0xCC` int3 padding + the next fn's
  head, so the row is longer than the real body (e.g. 00a14e20 Clear = real 8B vs captured 19B) and
  never reaches parity. The harness's trailing-CC strip misses INTERIOR padding. Auto-fix with
  `tools/decomp_pipeline/trim_overcapture.py 0x<addr> ...` (prints trimmed len+bytes) or
  `--oracle <tsv>` (rewrites over-captured rows in place); it cuts at the first standalone int3 that
  follows a terminator (ret/tail-jmp), leaving clean rows untouched. Then author the single fn.
  TAIL-CALL CAVEAT authoring recovered forwarders: a VALUE-returning member forwarder
  (`return this->f4->M(this->f8);`) or cdecl-cleanup forwarder (`helper(a,b,-1);` w/ `add esp,N`)
  keeps retail's `call;ret` and recovers byte-exact; a VOID member forwarder (`sub->M(x);` as the whole
  body) gets tail-call-optimized to `jmp` by VC7.1 while retail kept `push;call;ret` — DIFFER(NvN)
  same-length, not recoverable. Backlog of the 84 deferred over-captures (by shape) is
  rebuild/backlog/overcapture-recovery-worklist.tsv; ~11 recovered so far this pass.
- Frontend visual QA can be DRIVEN headlessly: build via `rebuild/build_bootstrap.ps1
  -RetailFrontendBank <frontend.big>`, launch FableTLC-Reconstruction-VisualCheckpoint.exe (client
  1280x720, asset-free, loads data/frontend/*.bmp), then synth-click with SetForegroundWindow+
  SetCursorPos+mouse_event and screenshot with Graphics.CopyFromScreen (crop+3x NearestNeighbor to
  inspect glyphs). `$PID` is read-only in PS — use another var. Full recipe + menu map + current
  fixed/open state in docs/VISUAL_PARITY_STATUS.md.
- OPEN visual bug: the profile-name font (`AppendProfileNameText`, ENG_ARIAL save rows/profile names/
  File Information) renders DOUBLED/ghosted vs retail's smooth AA. NOT a double-draw (one quad/glyph)
  and NOT fixed by fixed-function SetSamplerState LINEAR (no effect). Leading theory: the frontend
  samples the atlas via a PIXEL SHADER (window title `...PixelShader...`), so API sampler state is
  bypassed — investigate the Render2D PS sampler + ENG_ARIAL atlas cell packing (half-texel inset).
  The detail-title font IS clean (LINEAR after BeginScene, commit 352a684). See VISUAL_PARITY_STATUS.md.

## Toolchain (see docs/TOOLCHAIN.md for commands)
- Mario rig gotcha (2026-07-22): `work/mario_hero/stage_bindaxis4` is format-valid and looks
  better, but its live gameplay test still stretches every disconnected SM64 body segment apart.
  Mario is smaller/differently proportioned than the hero; hero-sized parent-relative animation
  translations pull the pieces out to hero joint positions. Textures are also still visibly wrong.
  Do not iterate the same per-piece bind-axis placement or call the atlas fixed. Fable ANIM position
  keys carry parent-relative bind translations, so mesh-only rest/inverse-bind edits are insufficient.
  Resume from the timestamped Mario section at the end of `docs/HANDOFF.md`; first test animation
  translation retargeting or smooth/overlapping joint geometry on one walk clip.
- Quest/terrain smoke gotcha (2026-07-22): Wasp Menace (`OBJECT_QUEST_CARD_WASP_MENACE`) rendered
  correctly through ForgeFSE with 1337 gold, 500 renown, card art, and Guildmaster audio, so the base
  quest API works when the card asset exists. Custom blank cards are a data/text/definition binding
  problem. Do not activate a quest during ForgeTest terrain teleports; the engine raises the
  quest-region abandon/reload modal and can block/confuse manual terrain validation. Evidence:
  `work/runtime_smoke_quest_terrain_20260722/RESULTS.md`.
- Wave3 decomp gotcha (2026-07-22): attached
  `lift/reports/wave3/code/0x0089B330_global_IsHeroNaked_CGameScriptInterface_UBE_NXZ.cpp` names
  `CGameScriptInterface::IsHeroNaked`, but review before promotion. Its current miss path can
  dereference the vector-map end sentinel after `LowerBound`; verify the type-key `0x5E` interface
  method before curating it.

Ghidra **12.1** @ **`D:\Subuwu\tools\ghidra-public`** (verified path; not the stale `D:\ghidra_12.1.2_PUBLIC`)
+ GhidraMCP (port 8089). **No XEXLoaderWV.**
Era compiler = MSVC 2003 / VC7.1 (game ships `msvcr71.dll`) for Phase-4 oracle compiles.
Ghidra scripts in `tools/ghidra_scripts/`; Lua tooling in `tools/lua_mod/`.

## Payoff link
A decompiled name/def-table export can auto-feed **FQT**'s hand-curated `GameData.cs`
(`D:\Code\FQT\FQT\DOCS\GAME_DATA.md` says it has "no automated extraction pipeline yet").
