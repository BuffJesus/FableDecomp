# Cross-Corpus Comparison — OURS (FableTLC / FableForge) vs NIKO'S (OpenFable)

*Fable: The Lost Chapters reverse-engineering documentation. Rigorous, evidence-based,
file:line-cited. Prepared 2026-07-21 for Niko (OpenFable author).*

Corpus A ("OURS"): `D:\Documents\FableTLC\docs\` (~83 md) + `D:\Documents\FableTLC\share_openfable\`
(4 md written **for** OpenFable) + `D:\Code\FableForge\docs\` (9 md).
Corpus B ("NIKO'S"): `…\niko_docs\docs\` (91 md: `fable-exe/formats`, `fable-exe/re`,
`fable-exe/functions`, `fable-exe/pe`, `openfable/`, `playbook/`, `history/`).

---

## (a) Executive summary — the four numbers

The two corpora have **different centers of gravity**, and this dominates every count:

- **OURS** = content authoring on the **retail `Fable.exe`**: byte-exact native readers/writers
  for every on-disk format, so new content loads in the shipped game. Depth is in **formats +
  writers + gameplay-data** (save, quest, audio, anim, appearance, dialogue, effects, text).
- **NIKO'S** = a **re-implementation** (OpenFable, Vulkan/Linux) built from static RE of the
  retail exe. Depth is in the **runtime/renderer** (D3D9 vtable census, skinning palette,
  allocators, boot spine, shaders, file I/O) plus a ~200k-line **function-address manual**.

Both independently reversed the same on-disk formats, so the format layer overlaps heavily and —
importantly — **almost entirely agrees**.

| Bucket | Count | Notes |
|--------|-------|-------|
| **1. Shared topics that AGREE** | **~18 of ~21 shared** | LEV core record, mesh codec/POSPACKED3/UV/IB-endian/catalog-tag, textures BIGB/DXT1, DefBin (every field), STB container framing + LZO frame + CRangeCompressor + InfoBlock pointers, LZO1X=stock, height_scale 2048, skeletal-skinning verdict, level adjacency/STB footprints, "no native level-series", scenegraph building meshes, BIGB/BBBB containers, audio LiOnHeAd family, index-buffer runtime = native LE |
| **2. Shared topics that CONTRADICT** | **1 hard + ~3 soft/label** | Hard: the retail terrain model (STB baked-mesh vs .lev-derived heightmesh — see §c1). Soft: LEV "version" framing & theme-slot count (an *intra-OURS* discrepancy; our OpenFable-facing notes agree with Niko); DXT3-vs-"DXT5-class" naming for fmt 0x20; skin-palette bone cap 16 vs 18 |
| **3. EXCLUSIVE to OURS** | **~30 topics** | Save format (FableSave!), quest/.qst/cutscene VM, audio .lut/.lug + dialogue snds.bin join, animation 3DAF byte format, lipsync, appearance/hero-morph, demon-door lipsync, effects/particles, text.big, frontend UI tree, controller enums, navmesh (CNavQuadTree) + .bwd/.wld world file, FSE editor bridge, native writers for all of the above, custom-region-walked-live |
| **4. EXCLUSIVE to NIKO'S** | **~17 doc-clusters** | Full D3D9 vtable/slot census, per-frame draw tree, primitive topology, shader-bank permutation catalog, GPU bone-palette register layout, matrix-math hubs, memory allocators, Win32 file-I/O atlas, boot spine phase table, input stack, PE import report, subsystem xref/coupling graphs, the ~200k-line per-function address manual, STB neighbor-degree census |

**One-line answer to Niko:** *Where we both looked at the same bytes we agree ~85% of the time
and there are effectively **no hard format contradictions** — the single real disagreement is a
model/behavior one about how retail terrain is drawn (baked STB mesh vs load-time heightmesh),
and it stems from our different goals, not a mis-read offset. The rest is complementary: you own
the runtime, we own the writers + gameplay data.*

---

## (b) Shared-topic table

| # | Topic | OURS doc | NIKO doc | Verdict |
|---|-------|----------|----------|---------|
| 1 | BIGB `.big` container | `BIG_FAMILY_FORMATS.md`, MasterNotes §1 | `FORMATS.md`, `TEXTURES_BIG.md` | AGREE (magic BIGB, type 100) |
| 2 | BBBB `.wad` container | MasterNotes, `LEVEL_CONTAINER_INDEX.md` | `FORMATS.md` | AGREE |
| 3 | LZO1X = stock Lionhead LZ | `LIONHEAD_LZ.md` | `STB_FORMAT.md`, `MESH_BANK.md` | AGREE (public LZO1X-1; two framings) |
| 4 | LEV terrain record | `LEV_WRITER.md`, `share_openfable §3` | `LEV_FORMAT.md` | AGREE on core; soft label conflict (§c2) |
| 5 | Height world scale | `LEV_WRITER.md` (`×2048`) | `TERRAIN.md` (`kLevHeightScale=2048`) | AGREE |
| 6 | Cooked MESH codec | `BIG_MESH_GEOMETRY.md`, `MESH_COMPOSE.md` | `MESH_BANK.md` | AGREE (Fable-framed LZO1x) |
| 7 | POSPACKED3 (11+11+10) | `MESH_COMPOSE.md` | `MESH_BANK.md` | AGREE (verbatim) |
| 8 | Mesh UV int16/2048 @+8 | `MESH_COMPOSE.md` | `MESH_BANK.md` | AGREE (verbatim; half-float rejected both sides) |
| 9 | Mesh index endianness | `MESH_COMPOSE.md`, MasterNotes §6 | `MESH_BANK.md`, `INDEX_BUFFER_ENDIAN_RUNTIME.md` | AGREE (BE flora on disk, LE prop; runtime native LE, no swap) |
| 10 | MBANK entry tag `0xAE689191` | `MESH_COMPOSE.md` | `MESH_BANK.md` | AGREE |
| 11 | Building scenegraph mesh (`C3DMeshFile`) | `BIG_MESH_FORMAT.md §3`, `MESH_SCENEGRAPH_CONTAINER.md`* | `MESH_SCENEGRAPH_CONTAINER.md` | AGREE (hierarchical XVBD/XTST; same codec) |
| 12 | Textures BIGB/DXT | `BIG_TEXTURE_FORMAT.md`, `TEXTURE_WRITER.md` | `TEXTURES_BIG.md` | AGREE (0x1f=DXT1); soft label conflict (§c3) |
| 13 | DefBin game.bin/names.bin | `COMPILED_DEFINITION_INDEX.md` | `DEFBIN_FORMAT.md`, `CPP_LOADER_PLAN.md` | AGREE (every field: 20-B hdr, 0xa8e36c34, 12-B entry, zlib chunks, NULLDEF, component prefix) |
| 14 | DefBin data codec = zlib | `MESH_BANK_INFLATE.md`, `LIONHEAD_LZ.md` | `DEFBIN_FORMAT.md` | AGREE (zlib for defs; LZO for tex/mesh) |
| 15 | STB container (25 SEA + 398 land + COMMON) | `share_openfable §0`, MasterNotes §4 | `STB_FORMAT.md`, `FORMATS.md` | AGREE (same census, same catalog) |
| 16 | STB baked chunk framing (LZO frame / CRangeCompressor / InfoBlock ptrs) | `STB_Chunk_Layout_Spec.md`, MasterNotes §4 | (implied not-present; see §c1) | OURS deep; NIKO does not load it |
| 17 | Retail terrain draw model | `share_openfable §0`, MasterNotes §4 | `openfable/TERRAIN.md §7`, `LEV_FORMAT.md`, `STB_FORMAT.md` | **CONTRADICT** (§c1) |
| 18 | Level adjacency / STB footprints | `WORLD_AND_CHEST_INDEX.md`, MasterNotes §5 | `LEVEL_ADJACENCY.md`, `LEVEL_NEIGHBOR_CENSUS.md`, `SERIES_CATALOG.md` | AGREE (STB AABB shared-edge; Lookout/Picnic/Greatwood cluster) |
| 19 | "No native level-series/graph" | MasterNotes §5, `FINDINGS.md` | `LEVEL_ADJACENCY.md`, `MESH_SCENEGRAPH_CONTAINER.md` | AGREE (no CRegionGraph/CLevelGroup; per-quest Watch* + STB edges) |
| 20 | Skeletal skinning + anim | `BIG_ANIM_FORMAT.md`, `BIG_MESH_GEOMETRY.md`, `APPEARANCE_SYSTEM.md` | `MATRIX_SKINNING_ANIM.md`, `NPC_DRAW_CHUNK_FINDINGS.md`, `SHADER_CATALOG.md` | AGREE (skeletal+morph+cloth; TRS keyframes); soft cap 16/18 (§c4) |
| 21 | Audio LiOnHeAd family | `LUG_AUDIO_FORMAT.md`, `LUT_AUDIOBANK_FORMAT.md`, `share_openfable §4` | (mentioned in `FORMATS.md` audio note) | AGREE where they touch; OURS far deeper |

\* `MESH_SCENEGRAPH_CONTAINER.md` exists in **both** trees with near-identical content — a shared,
mutually-consistent finding rather than an independent duplicate.

---

## (c) CONTRADICTIONS — quoted evidence

### c1. HARD contradiction — how retail draws terrain (baked STB mesh vs load-time heightmesh)

This is the single substantive disagreement and it is a **behavior/model** claim, not an offset.

- **NIKO'S** (`openfable/TERRAIN.md`): OpenFable *builds its own triangle mesh from the `.lev`
  height grid at load* — `lev::build_height_mesh` "Quads → de-indexed tris; splat weights"
  (`TERRAIN.md:90`), `kLevHeightScale = 2048.f` "Raw LEV float → world Y" (`TERRAIN.md:97`), and a
  self-built `build_chunk_lods` "Pyramid of meshes, strides {1,2,4,8}" (`TERRAIN.md:94`). Niko's
  `LEV_FORMAT.md:85-94` frames the STB only as "Retail did the equivalent via STB-cached patch /
  LOD tiles" — treated as an alternative, not the actual visible ground. Niko's `STB_FORMAT.md`
  concludes the Lookout land bank is **heightmesh + detail maps**, explicitly *"not a per-tree
  RepeatedStaticMesh instance list"* and *"landscape-patch cache geometry … not a … tree instance
  table"* (`STB_FORMAT.md:117-126`).

- **OURS** (`share_openfable/FableTLC_RE_Notes_for_OpenFable.md`, written *to answer Niko's §7*):
  *"The retail engine does not tessellate terrain from the `.lev` at load — there is no load-time
  heightmesh bake. The visible ground is a **pre-baked mesh** stored per-map inside
  `FinalAlbion_RT.stb`, and the `.lev` (heights/themes) only feeds collision / nav / height
  queries at runtime."* (`FableTLC_RE_Notes_for_OpenFable.md:19-24`). Backed with addresses:
  `CEngineMap::OpenRetailStaticMap @0x00b41e50`, `UpdateBackgroundLOD @0x00bdc180` →
  `CLandscapeBackgroundTreeNode::LoadHeader`; and *"If it's 0 → …BuildDefaultSection @0x00be6880 =
  a flat default = the void."* (`:31-37`). `STB_Chunk_Layout_Spec.md` proves the baked chunk is
  LZO1X `SaveCompressed` frames carrying VB/IB meshes (`CLandscapeBackgroundPatch::Save
  @0x02ce3220`), round-tripped byte-exact.

**Why it's a real contradiction and not just emphasis:** the two docs make opposite claims about
*what geometry the retail GPU actually draws for the ground* — Niko: a mesh derived from the `.lev`
grid; OURS: a pre-baked mesh from the STB, with the `.lev` feeding only nav/collision. **Note the
partial convergence:** Niko's `STB_FORMAT.md` does now find baked landscape-patch/LOD tiles in the
land bank (agreeing the STB holds cached patch geometry), but still concludes those are
heightmesh/detail caches rather than *the* render source, and OpenFable does not load them.
For OpenFable this is a deliberate design choice (build its own mesh) — so it is a contradiction
of *retail-faithfulness*, and the most valuable single thing OURS can hand Niko if he ever wants
retail-exact LOD tiles. Both **agree** the height scale is 2048 and that the `.lev` grid is the
authoritative height source.

### c2. SOFT / label — LEV "version" and theme-slot count (mostly an intra-OURS discrepancy)

- **Version field.** NIKO `LEV_FORMAT.md:22-24`: `+0 u32 = Version 25`; `+4 u32 = 0x1904`
  (two fields). OURS `LEV_WRITER.md:30`: `u32 headerSize(=25)`, then `u16 version(=6404)`.
  `0x1904 = 6404`, so **both read the same bytes** — they only disagree on *which dword is named
  "version."* Same on-disk value; a naming difference, not a byte conflict. (Our internal
  MasterNotes §5 says "version 6404"; our OpenFable-facing note §3 says "Version 25" — matching
  Niko.)

- **Theme-slot count.** NIKO `LEV_FORMAT.md:11,36`: **512** slots × 132 B = 67584 B preface.
  OURS `LEV_WRITER.md:35`: **256** themes × 132 B = 33792 B. **But** our OpenFable-facing note is
  explicit and agrees with Niko: *"preface **512 theme slots × 132 bytes**; slot 0 = header"*
  (`FableTLC_RE_Notes_for_OpenFable.md:203`). So the 256 figure is an **internal OURS inconsistency**
  in `LEV_WRITER.md`, not a corpus-vs-corpus contradiction — where OURS speaks to Niko it says 512.
  The **132-byte slot size agrees on all three docs.** (Action item on our side: reconcile
  `LEV_WRITER.md`'s 256 against the proven 512.)

### c3. SOFT / label — texture format enum 0x20

- OURS `BIG_TEXTURE_FORMAT.md`: enum `0x20` (32) = **DXT3**, validated by alloc-byte math and a
  byte-exact write path ("Fable uses DXT1/DXT3, not DXT5").
- NIKO `TEXTURES_BIG.md:31`: "DXT5-class (fmt 32/35)".

Not a byte conflict (both agree 0x1f=DXT1, both agree it's a 4×4 block DXT). Niko's "DXT5-class" is
a loose grouping; OURS is specific and write-validated as **DXT3**. Worth Niko correcting.

### c4. SOFT — skinned bone-palette cap

- NIKO `MATRIX_SKINNING_ANIM.md:143-155`: GPU palette = **54 float4 registers ÷ 3 = 18 bones**;
  retail *"splits skinned meshes into bone groups"* per DIP.
- OURS `BIG_MESH_GEOMETRY.md`/`MESH_COMPOSE.md`: skin auto-palette *"cap 16/block → greedy
  multi-block partition."*

Both describe the same mechanism (per-block bone palette with multi-block partition). 16 vs 18 may
be a conservative authoring cap (ours) vs the observed HW register-derived cap (niko) — likely
reconcilable, but a concrete number Niko should know: **retail's palette holds 18, so ours could
raise the authoring cap from 16.**

**No other hard contradictions were found** across the ~21 shared topics. On every concrete magic
number, struct offset, field size, block order, enum, and codec that both corpora document, they
agree (frequently verbatim, suggesting the format layer was reversed to the same ground truth).

---

## (d) EXCLUSIVE to OURS (topics Niko does not document as specs)

Gameplay-data & authoring formats — verified absent from NIKO'S set (only incidental string
mentions in the function manual / history, never a format spec):

1. **Save format** — `FableSave!` 307,200-B container, dual-zlib HEADER + SAVED_ENTITIES, 6
   signature invariants, `CUserProfileManager::VerifySignature @0x00409730`, hero-stat CRC tags,
   childhood-restore gap. (`SAVE_*.md` ×8, `SAVEGAME_FORMAT.md`)
2. **Quest / .qst / cutscene VM** — 161 native `CQ_*Script` classes, `.qst` `AddQuest` grammar,
   `CCutsceneDef` 184-verb text interpreter `@0x00CBFB7D`, `CGameScriptInterface` 891 methods.
   (`QST_FORMAT.md`, `QUEST_VM_RE.md`, `QUEST_CARD_SYSTEM.md`, `SCRIPT_VM_MAP.md`)
3. **Audio .lut / .lug** — LiOnHeAd bank byte formats, 652-B sample records, criteria→CRC→CSoundMap
   routing, 20k+ clips byte-exact. (`LUT_AUDIOBANK_FORMAT.md`, `LUG_AUDIO_FORMAT.md`, `LUT_WRITER.md`)
   *(Niko has a runtime OpenAL note but no bank format.)*
4. **Dialogue join** — `snds.bin` (name→CRC→soundID→.lut/LIPSYNC) chain. (`DIALOGUE_PIPELINE.md`)
5. **Animation 3DAF byte format** — XSEQ/ANRT/AOBJ/MVEC chunk grammar, quaternion pools, byte-exact
   on 3,435. (`BIG_ANIM_FORMAT.md`, `ANIM_WRITER.md`) *(Niko has the runtime interpolators, not the
   on-disk chunk layout.)*
6. **Lipsync format** — viseme/keyframe layout, 20,505 byte-exact. (`DIALOGUE_PIPELINE.md`)
7. **Appearance / hero morph** — `CAppearanceModifierDef`, `CHeroMorphDef` 156 records, DXT3
   overlays. (`APPEARANCE_SYSTEM.md`, `HERO_MORPH.md`)
8. **Demon-door face lipsync** — phoneme-pose blend. (`DEMON_DOOR_FACE.md`)
9. **Effects / particles** — `CParticleEmitter` serialization, 10 component classes. (`EFFECTS_FORMAT.md`)
10. **text.big** — 28,913-entry UTF-16 string DB. (`TEXTBIG_FORMAT.md`, `TEXT_BIG_WRITE.md`)
11. **Frontend UI tree** — 785 `CUIDef` widgets. (`FRONTEND_FORMAT.md`)
12. **Controller enums** — DIK scancodes, EGameAction. (`CONTROLLER_ENUMS.md`)
13. **Navmesh (CNavQuadTree)** — adaptive quad-tree algorithm, TopologyWeights, oracle-validated on
    398. (`NAVMESH_RE.md`, `MULTILAYER_NAV.md`) *(Niko has no navmesh spec.)*
14. **World file .bwd / .wld** — compiled `CMapInfo`/`CRegion` records, region membership.
    (`WORLD_AND_CHEST_INDEX.md`, MasterNotes §5)
15. **STB baked-chunk emitter** — `forge::stbbake`, CRangeCompressor RAW, byte-exact InfoBlock
    writer. (`STB_Chunk_Layout_Spec.md`, `NATIVE_STB_WRITER_SCOPE.md`)
16. **Native writers, every format** — the whole point: byte-exact write + engine-append (CDefIndex
    retarget) so retail loads new content. (`*_WRITER.md`, `BIG_WRITER.md`)
17. **Teleport / cross-region** — `GoToMapSlot` chain, ResolveWorldMap vtable slot 12. (MasterNotes §12)
18. **FSE editor bridge** — Lua bindings via `ASLR<>(addr)`. (`FQT_EDITOR_*`, `EGOCORE_INTEGRATION.md`)
19. **Custom region walked live** — end-to-end new-region assembly proven in the retail game.
    (`NEW_LEVEL_CREATION.md`, `NEW_LEVEL_ASSEMBLY.md`)
20. **Definition enum-header catalog** — 63,540 constants. (`DEFINITION_HEADER_INDEX.md`)
21. **Debug-build unlock** — FableWin.pdb / Ego_d.pdb as byte-exact oracle. (MasterNotes §17)
22. **Quest-card vtable group** — `AddQuestCard` vtable[292] `@0x008913F0`. (`QUEST_CARD_SYSTEM.md`)
23. **Xbox-ADPCM encoder** — `xbadpcm.py` (ffmpeg has *no* encoder for `WAVE_FORMAT_XBOX_ADPCM`
    0x0069, blockAlign 36). (`LUG_AUDIO_FORMAT.md`)
24. **Multi-layer navmesh** — `JoinNavigationLayers @0x03291790`, retail ceiling 5 layers
    (HookCoast); no Z in nav data. (`MULTILAYER_NAV.md`)
25. **Mod-pack / .fmp field-level merge** — def_schema conflict resolution (53→39 field-merged on
    Aeon + Lost Content). (`MOD_PACKS.md`, `FMP_FORMAT.md`)
26. **Load-order model** — Fable has *no* runtime load-order arbitration; FableForge constructs the
    merged result itself (LOOT/Wrye-Bash-style masterlist). (`LOAD_ORDER.md`)
27. **Unified node scripting** — `forge::questnodes`: 104 FQT nodes + 931 FSE fns → Lua codegen.
    (`UNIFIED_SCRIPTING.md`)
28. **CGameScriptInterface API catalog** — 891 methods + 101 candidate FSE bindings, vtable base
    `0x1260F0C`. (`SCRIPT_BINDINGS_CANDIDATES.md`)
29. **FablePro `Profile.bin` registry** — 50 `SaveGameNames1..50` slots. (`SAVE_PROFILE_INDEX.md`)
30. **EgoCore community-source cross-validation** — independent confirm of seed-0 field-tag CRC.
    (`EGOCORE_INTEGRATION.md`)

**Cross-cutting canonical fact (OURS):** every on-disk name→tag hash = reflected CRC-32 poly
`0xEDB88320`, **seed 0, no final inversion** — unifies save field tags, game.bin def tags, text.big
symbols, snds.bin, and the save trailer. (names.bin uses the complemented `0xFFFFFFFF−crc32` form;
`CCharString::GetCRC` seed-`0xFFFFFFFF` is the lone contested save-stream variant.) NIKO'S DefBin
docs reference the same `0xa8e36c34` platform indicator and names.bin hash but do **not** document
this unified seed-0 CRC family across the gameplay-data formats (which NIKO'S set does not cover).

---

## (e) EXCLUSIVE to NIKO'S (topics OURS does not document)

Runtime / renderer RE — the reimplementation lane. OURS has scattered addresses but no equivalent
systematic docs for:

1. **Full D3D9 vtable/slot census** — all 20 d3d9 interfaces × every slot; `DrawIndexedPrimitive`
   slot 82 = `0x148`, `SetIndices` slot 104 = `0x1a0`. (`D3D9_FULL_VTABLE_CENSUS.md`)
2. **Per-frame draw-call logic tree** — world/character/alpha/UI passes, gold batch `@0xb8eaa0`,
   Present `[dev+0x44]`. (`D3D9_FRAME_TREE.md`)
3. **Primitive topology** — per-draw at `+0xC` (5=STRIP/4=LIST), PrimCount proof `@0xba1f50`.
   (`D3D9_PRIMITIVE_TOPOLOGY.md`)
4. **Shader-bank permutation catalog** — shaders.big 353 VS + 112 PS; PALSKIN=87, permutation axes.
   (`SHADER_CATALOG.md`)
5. **GPU bone-palette register layout** — 54 regs @c38, WVP @c92, built `@0xbdb260`.
   (`MATRIX_SKINNING_ANIM.md`)
6. **Matrix-math hubs** — SSE 4×4 multiply `@0x988a50` (fan_in 75), 3×3 inverse `@0xa11f20`.
7. **Memory allocators** — CRT→Pool→SizeClassHeap(30 classes)→adapters; `@0x9f20b0`, poison
   `0xCDCDCDCD`. (`MEMORY_ALLOCATORS.md`)
8. **Win32 file-I/O atlas** — CFileStream, `CreateFileW @0x99a773`, no mmap, async shader cache.
   (`FILE_IO.md`)
9. **Boot spine** — entry→WinMain `@0x403480`→AppInit `@0x402510` phase table. (`BOOT_SPINE.md`)
10. **Input stack** — DirectInput8 `@0xa60050`, ~65 `CInputProcess*` mode stack. (`GAMEPLAY_FRONTEND_INPUT.md`)
11. **PE import report** — 422 imports/16 DLLs, entry `0x401067`. (`pe/REPORT.md`)
12. **Subsystem xref / coupling graphs** — 23,199 funcs, 209,249 edges; graphics_d3d↔crt.
    (`SYSTEM_XREF.md`, `SYSTEM_COUPLING.md`, `CALL_FLOW_TREE.md`)
13. **~200k-line per-function address manual** — 26,563 functions across 1-MiB VA buckets,
    5 naming passes. (`functions/0x00400000.md` … `0x01200000.md`)
14. **STB neighbor-degree census** — max degree 9, modal 3–4, ~89.8% connected. (`LEVEL_NEIGHBOR_CENSUS.md`)
15. **BIG-bank asset intel** — 5,739 embedded `\DEV\…\*.TGA` source paths. (`pe/ASSET_INTEL.md`)
16. **OpenFable engine architecture / Vulkan port / build & run / loading pipeline** — the whole
    `openfable/` subtree (the reimplementation itself). (`openfable/*.md`)
17. **Reconstruction playbook / project-evolution history** — methodology + `history/`.

**Biggest gaps each way:**
- *Niko lacks from us:* everything gameplay-data (save, quest, audio banks, anim/lipsync byte
  formats, appearance, effects, text, navmesh, world .bwd) and every **native writer**.
- *We lack from Niko:* the entire **runtime/render RE** (D3D9 call surface, shader permutations,
  bone-palette registers, allocators, boot/input/file-I/O) and the exhaustive **function-address
  manual**.

---

## Method note

Format specs on both sides were opened and compared claim-by-claim (magic numbers, offsets, field
sizes, block orders, enums, codecs, addresses). The ~200k-line NIKO function manual and the OURS
gameplay-data writers were characterized by kind (they establish the exclusives, not
contradictions). A difference in emphasis or field-naming was **not** counted as a contradiction;
only a differing offset/value/behavior was. The result is an asymmetric but highly-consistent pair:
OURS = writers + gameplay data on retail; NIKO'S = runtime/render RE for a reimplementation.
