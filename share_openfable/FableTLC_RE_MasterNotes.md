# Fable: The Lost Chapters — Master Reverse-Engineering Notes

A consolidated dump of what we've reversed about TLC's on-disk formats and engine
pipeline, from a parallel project (**FableForge** — native mod/authoring tools for
the *retail* engine). Compiled from ~72 internal RE docs, decompiles of retail
`Fable.exe`, the FableWin 1.5 dev editor, and the Anniversary debug PDBs, all
cross-checked against real game data with round-trip proofs.

**Conventions.** Addresses are **retail `Fable.exe`** (ImageBase `0x400000`)
unless prefixed `FableWin:` (TLC 1.5 dev editor, ships a PDB) or `Ego_d:`
(Anniversary debug engine PDB). Status tags: **PROVEN** (byte-exact round-trip
and/or live in-game), **RECIPE** (grammar closed, write path coded, runtime not
yet confirmed), **OPEN** (gap remains).

> Companion files: `FableTLC_RE_Notes_for_OpenFable.md` (terrain intro) and
> `STB_Chunk_Layout_Spec.md` (deep terrain byte spec).

**One hash rules everything:** the field-tag / symbol hash used across game.bin,
text.big, dialogue `snds.bin`, and save streams is **reflected CRC-32, poly
`0xEDB88320`, seed 0, no final inversion**. There is no seed-`0xFFFFFFFF` variant
on disk. (The def-container *names.bin* uses the complemented form
`0xFFFFFFFF - crc32`; the *field* streams use plain seed-0.)

---

## 1. BIG archives (`BIGB`) — the master container

All bulk assets live in 8 `.big` files. **PROVEN: 8/8 round-trip byte-exact
(SHA-256).**

- **Header (16 B):** Magic `"BIGB"`, Version 100, FooterOffset, FooterSize.
  Payloads from 0x10 to FooterOffset; footer/directory at FooterOffset.
- **Directory:** per sub-bank {name, version, entryCount, TOC off/size/align};
  TOC = stats header (type histograms) + variable entry records {Magic 42, ID,
  Type, Size, Offset, CRC, name, Timestamp, Deps, InfoSize, Info blob}. **TOC
  chain terminates exactly at FooterOffset** (validation invariant, all 8 pass).
- **Format families by bank:** `TEXT_*` (text.big), `LIPSYNC_*` (dialogue.big),
  `GBANK_*` (textures/frontend graphics), `MBANK_*` (graphics.big meshes+anims),
  `PARTICLE_*` (effects.big), `SHADER_*` (shaders.big).
- **Gotchas:** payload alignment 2048 for GBANK/MBANK/PARTICLE/fonts (512/4/1
  elsewhere); padding is 100% zero. **The TOC `CRC` field is a per-format-class
  fingerprint, NOT a payload hash** (all 465 shaders share one CRC) — a writer
  must preserve it verbatim, never recompute. **Timestamp** is opaque
  high-entropy — preserve verbatim too.
- **Write path:** `bigb_roundtrip.py` (8/8 byte-exact), `bigb_write.py`
  (replace-payload same-size + size-changing validated; offsets re-flow by
  alignment). New-entry add: staged (`big_write adds=`).

---

## 2. Lionhead LZ = standard LZO1X (the codec under almost everything)

**PROVEN** by decompressing real textures/meshes to coherent output; ported
byte-exact on 300+ retail blocks. Retail `lzo1x_decompress @0x00c06b90`
(+ safe variant `@0x00c08170`). It's the **public LZO1X-1** family (miniLZO
compatible) — ~48 KB window, standard M1–M4 tokens, M4 distance 0 = EOS.

- **Two framings in the wild:**
  - Texture/mesh sub-blocks: `[u16 clen][clen LZO bytes]` (`0xFFFF` escape → u32
    clen; `clen==0` → stored). Used for texture mip0, mesh vertex/index buffers,
    bone names, keyframe pools.
  - STB landscape frames: `[uncompLen u32LE][compLen u32LE][lzo1x body]`
    (`SaveCompressed`). Engine writes `lzo1x_999`; you can emit `lzo1x_1` — same
    standard decoder, so **no encoder parity is needed to render**.
- Standalone impl: `lionhead_lz.py` (decompress, no deps) + `lionhead_lz_compress.py`
  (frame-compatible; size varies by match choices, both valid).

---

## 3. Compiled-def containers (`game.bin` / `frontend.bin` / `script.bin`)

The data model. **PROVEN read + field-merge; write path grammar-proven, runtime
gate pending.**

- **Inventory:** game.bin = 14,761 entries / 249 def types (**100% field-mergeable
  — every payload decodes with zero leftover**); frontend.bin = 810 entries / 8
  types; script.bin = 611 entries / 3 types.
- **Structure:** 9-byte header, u32 entryCount @ +9, entry table {defNameOff,
  nameOff, indexInDefinition}, payloads in **zlib chunks ≤ 32 KiB**. Names via a
  shared `names.bin`. **Engine append constraint:** each chunk must inflate to
  **≤ 32768 bytes** (fixed static inflate buffer, retail `0x9AD65A`/`0x138E19C`).
- **Field encoding:** `[u32 seed0-CRC(fieldName)][value]`; value types u32/i32/f32
  (4 B) / bool (1 B) / NUL-string / `[i32 count][elems]` vector.
- **Schema:** field-tag → type via CamelCase class-name mapping (`CREATURE` →
  `CCreatureDef`) + direct def-string keys; 53 remaining types aliased to known
  classes (no new decompiles). Example transfer functions (ported via FableWin
  RTTI): `CChestDef::Transfer @0x004DE204`, `CContainerRewardHeroDef::Transfer
  @0x004E8987`, `CControlsDef::Transfer @0x004330F0`. `PLAYER_GUI_PC`
  (`CPlayerGuiDef`, 463 fields) is the entire HUD layout DB.
- **CDefIndex cross-refs:** OBJECT payloads embed `{u32 typeHash, u32
  globalEntryIndex, ...}` records. **On append, every CDefIndex referencing an old
  global index must be re-pointed by name to the new index** — this is what makes
  a new def engine-accepted (community `.fmp`/ChocolateBox "ApplyLinks"; we
  reproduce it natively). Overwriting an existing def in place always works;
  appending failed until this retarget was applied.

---

## 4. Terrain — baked STB landscape (custom region PROVEN; custom terrain in progress)

**Retail does NOT bake terrain from the `.lev` at load.** The visible ground is a
**pre-baked mesh** per map in `FinalAlbion_RT.stb`; the `.lev` feeds only
collision/nav/height. No valid chunk → `BuildDefaultSection @0x00be6880` = a flat
void (this is why a new region is empty).

- STB = 25 SEA banks + 398 baked `<map>.lev` payloads + 1
  `__STATIC_MAP_COMMON_HEADER__`. Chunk = flat image of page-aligned
  (`0x1000`) file blocks; every byte is a **LZO1X FRAME**, an opaque **HDR**, or
  **PAD**. `CStaticMapInfoBlock` pointers are **absolute byte offsets** into the
  chunk. **PROVEN:** 18/18 donor frames decode byte-exact; 100% byte accounting;
  byte-identical round-trip (both a Python oracle and now the `forge::stbbake` C++).
- **Codec stack (all cracked):** outer LZO1X (§2); inner VB/IB = **CRangeCompressor**,
  a self-describing adaptive **bit-packer** with a `flags==0` RAW fallback (emit
  raw → engine decodes losslessly, no encoder parity); texture =
  `CTexture::SaveToDataStream @FableWin:0x030dbf30` = 19-byte header + raw DXT.
- Per-patch `Save @0x02ce3220`: `pw/ph/coords/IsWaterOnly/DetailMode`, then
  IndexCount/VertexCount/extents/IsDXT, DXT texture, VB (16-B packed record:
  `u16 x,u16 y,f32 z,u32 packedNormal,u16,u16`), IB (`IndexCount*3` u16), 4 edge
  strips, water flag.
- **OPEN:** the HDR quadtree directory (`CLandscapeBackgroundTreeNode::LoadHeader`)
  for *arbitrary new topology*. Same-topology maps (clone a donor grid) preserve
  HDR verbatim and swap only mesh frames + pointers — enough for custom
  heights/textures now. Editor bake oracle: `FableWin: UpdateStaticMapPass1
  @0x02d66c80`, `SavePatchesToTemporyStream @0x02deb570`, console
  `BuildRetailStaticMaps`.

---

## 5. LEV, navmesh, and new-region assembly (custom region walked live)

We teleported into and **walked** a brand-new region (`ForgeTest`, slot 399) from
an adult save.

- **`.lev` (PROVEN byte-exact, 398/398 via `lev_rw.py`):** header 25 B
  {headerSize 25, version 6404, obsOffset, navOffset}; cell grid `(W+1)×(H+1)` ×
  21 B/cell (height f32 at +5, world = raw × **2048**; theme/walkability +10..15;
  `+15==0xFF` blocks; `+20` preferred-path flag). Then an opaque obstacle/brush
  region (zeros in retail, preserved) and the nav sections at navOffset.
- **Navmesh (`CNavQuadTree`, algorithm recovered, oracle-validated on 398):**
  adaptive quad-tree, 32-unit root cells, levels 0–7; nodes = internal / navigable
  leaf (region + neighbours) / switchable leaf (door/gate keyed by **thing UID
  low32**) / blocked. Subdivide while mixed walkability/preferability down to
  1-unit (or 0.5). `TopologyWeights = 0xFF if cell+15==0, else 0x00 if +20==1,
  else 0x80` (no exceptions over 201,951 corpus cells). 249/398 retail LEVs are
  navless (interiors). Our terrain-only generator over-approximates ~7% (no
  thing-collision carving yet); byte-exact on no-thing levels.
- **World file (`.bwd` = compiled, PROVEN byte-exact via `wld_bwd.py`):** retail
  loads `FinalAlbion.bwd`, **not** the `.wld` text, unless `UseCompiledWorldFiles
  FALSE` in `userst.ini`. `u32 mapCount` + 398 `CMapInfo` {levelName, scriptName,
  used/isSea/proximity, box, u64 MapUID} + `u32 regionCount` + 141 `CRegion`
  {contains[]/sees[] slot lists, names, regionDef, minimap, exits}.
- **Minimal new region (proven):** WLD/BWD map+region records + LEV+TNG inside
  `FinalAlbion.wad` + a **MANDATORY `FinalAlbion_RT.stb` entry** (missing = guaranteed
  `0xC0000005` at world construction) + region membership in the `.bwd`
  (walk-freeze root cause was ForgeTest not being a region member). `mapwho` is
  runtime-only (spatial hash built post-load, no file). Region-connectivity graph
  (`*RegionGraph.txt`) from `CTCDRegionExit` placements drives AI routing (missing
  = no crash, just no inter-region AI).
- **Chest/world index:** 391 container entities across 141 regions (211 shops,
  148 chests, 16 silver keys, 16 keys); switchable nav leaves keyed by thing UID
  low32 (407/407 match).

---

## 6. Meshes (`graphics.big` MBANK)

**PROVEN** metadata + geometry codec; composition works.

- Types 1/2/4/5 = compiled meshes (3,295); type 3 = tagged BBM (3DMF, 1,383
  collision); 6/7/9 = animations (3,435). Compiled payload = relocatable
  serialized `C3DMesh2` image (LOD0 + optional zero-prim "ghost" LOD).
- **TOC Info descriptor (34–176 B, zero-leftover 3,294/3,295):** bounds, LOD
  count/offsets/sizes/distances, material count + **texture IDs (→ textures.big,
  100% resolve)**.
- **Vertex layout:** `sVert` ∈ {4,6,20,22}; strides 12/20/28/36. Position first
  (packed s16×3 via CVertexCompressionParams scale/offset, or FLOAT3). Packed
  normal = dword (10/11-bit); UVs = s11e4 i16 pairs. **Skinned prims add 8 B/vert**
  (4 joint bytes palette-indexed ÷3 → local slot, 4 weight bytes sum 255).
- **Geometry storage (CORRECTED):** vertex/index buffers are **LZO1X
  chunk-framed** `[u16 clen][clen LZO]` (0xFFFF→u32; 0→stored), 50–80% ratios —
  NOT plain deserialization. SUBM header = 5×u32 {nVerts, nFaces, nFaceIdx (==3×
  nFaces), sVert, flag}; index buffer plain u16.
- **Bones (PROVEN):** 5-part per-bone {BoneIndices u16 local→global, names
  (chunked-LZO), 60-B C3DBone, 48-B TRS, 64-B inverse-bind matrix row-major}.
- **Composition (PROVEN v2):** `compose_mesh(name, prims, materials, skeleton=
  clone_skeleton(donor))`; per-vertex skin auto-palette (cap 16/block → greedy
  multi-block partition). Blender bridge `anim_ops.py` round-trips retail anims
  (max quat err 1.85e-7).

---

## 7. Textures (`textures.big` / `frontend.big` GBANK)

**PROVEN** (corrected 2026-07-20).

- **Info (34 B, LE):** Width/Height (allocated pow2) @+0/+2, real FrameW/H @+6/+8,
  PixelFormatIdx @+12 (`0x1f`=DXT1, `0x20`=DXT3, `0x01`=A8R8G8B8), MipLevels @+17,
  FrameDataSize (uncompressed mip0) @+20, MipSize0 (on-disk mip0 region) @+24
  (0 → whole payload raw), 6-B CPixelFormatInit tail.
- **Payload:** **only mip0 is LZO1X-compressed** (block-framed §2); mips 1..n are
  **raw concatenated, no headers** (EgoCore-verified). Pixels = linear PC D3D9
  (DXT 4×4 blocks; ARGB stored BGRA).
- **Write:** `texture_build.py` — identity round-trip; retail re-encode PSNR
  46–66 dB (DXT reconvergence, expected); ARGB pixel-identical.

---

## 8. Animations (`graphics.big` types 6/7/9) & lipsync (`dialogue.big`)

**Both PROVEN byte-exact on the full corpus.**

- **Animation (3DAF, 3,435 entries):** one raw LZO1X stream (uncompressed marker
  `0x3E3E3E3E` `">>>>"`). Decompressed = 3DAF header + FourCC chunks
  `[char4][u32 size][payload]`: **ANRT** (duration, isCyclic), **AOBJ** (rig
  name), **XSEQ** (one per bone — byte-exact: `u32 boneIndex, i32 parent, cstr
  name, f32 samplesPerSec, u32 frameCount, f32 pos/scaleFactor, u16 rotCount +
  quat pool f32[4], palette, u16 posCount + i16[3] pool, palette` — palette =
  keyframe reduction), **MVEC** (root motion), **TMEV** (timing events), **AMSK**
  (bone mask), **HLPR**. fps is per-track (commonly 20). Quaternion convention =
  conjugate of mathutils. Write: `anim_build.py` byte-exact on all 3,435;
  compressed +5.3% vs retail (valid stream).
- **Lipsync (20,505 entries, all uncompressed):** `u32 visemeCount +
  visemeCount×{u8 idx, char[2] mnemonic, u8 NUL} + u32 blockMarker(0x2B=43 fps=
  22050/512) + u32 keyframeCount + per-frame {u8 pairCount, pairCount×{u8
  visemeIdx, u8 weight}}`. Info = f32 clip duration. Write: `lipsync_build.py`
  byte-exact all 20,505.

---

## 9. Effects / particles (`effects.big`)

**PROVEN** (uncompressed). PARTICLE_MAIN_PC, 1,165 entries = serialized
`CParticleEmitter`: u32 magic 0x64, name, EBOOL flags, f32 distances/fades,
priority, then `u32 systemCount` → per-system {name, scale, `u32 componentCount`
→ per-component {className, instanceID, fields, terminator `0x7B`}, terminator
`0x26`}. 10 component classes (CPSCUpdateNormal, CPSCEmitterGeneric,
CPSCRenderSprite, CPSCRenderMesh, …). Quantized floats `raw = value/scale*maxq +
bias`. References: game.bin `EParticleEmitter` id → effects entry; sprite/mesh
indices → textures.big / graphics.big. Write via `big_write rebuild(edits=)`.

---

## 10. Audio — `.lut` (voice) and `.lug` (SFX), both byte-exact

- **`.lut` (voice/dialogue, PROVEN identity + replace/add on 4/4 banks):** magic
  `LiOnHeAdLHAudioBankCompData`, 0x28 header → clip records {Index u32 1-based,
  const fields, Min/MaxDistance f32, Priority, `0xFFFFFFFF` sentinel, RIFF} →
  tail LookupTable (`size = 20 + (Count-1)×12`, deterministic — no hash table).
  Xbox IMA ADPCM (`0x0069`), mono 22050, block align 36. 20,213 clips total
  (Dialogue.lut 268 MB/12,134; ScriptDialogue.lut 229 MB/5,310; +2 more).
- **`.lug` (SFX/ambience, PROVEN identity on 50/50, replace+add):** magic
  `LiOnHeAd`, stream of `char[32] name + u32 size + payload`: `LHFileSegmentBankInfo`,
  `LHAudioWaveData` (packed RIFFs), `LHAudioBankSampleTable` (652-B records:
  sourcePath 260 B, id, waveId, riffSize/Offset, WAVEFORMATEX mirror @+292),
  `LHAudioBankCriteiaInfo` (event tag → sample-id list). Aliases: several ids → one
  blob. Xbox ADPCM + some PCM.
- **Event routing:** criteria string (`SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS`) → CRC
  → `CSoundMap` → id array → bank sample. Hero grass footsteps = even 820–838;
  weapon swing 1979–1995; metal-flesh impact 1635–1655.
- **Playback:** OpenAL; `alBufferData` sites `FUN_00c4dc40`/`FUN_00c4de00`;
  descriptor `FUN_00c5f070`, validated `FUN_00c5f110` (rejects bad
  nBlockAlign/format). On error the engine `alDeleteBuffers` and returns
  **without `alSourcePlay`** → silent, no crash.
- **Dialogue join (SOLVED):** the missing link is a 4th file family
  `data\Defs\<bank>snds.bin` = `{u32 count, count×{u32 crc0("SND_"+name), u32
  soundID}}` sorted. Chain: text.big (name + SpeechBank) → snds.bin (CRC →
  soundID N) → `.lut[N]` RIFF ↔ dialogue.big LIPSYNC N. `dialogue_pipeline.py`
  does REPLACE (proven) + ADD (staged). **OPEN:** live engine acceptance of an
  added clip.

---

## 11. Quests, cutscenes, and the script "VM"

**Verdict: quest LOGIC is native C++ (not bytecode); everything around it is data.**

- **Quest registry:** 161 compiled `NScript::CQ_<Name>Script` classes; table
  `@0x00CD52D0` maps name → allocator ptr; activation = `IsQuestRegistered` gate
  then indirect call (unknown names silently dropped). FSE hook seam `@0x00CDB355`.
  Persistence: quest name + `OnPersist(CPersistContext)` to save; reactivation
  refills members.
- **Cutscenes (`CCutsceneDef`, PROVEN all 595):** payload = 9-B header, `u32
  count @+9`, then NUL-terminated command strings from +0x0D; trailing bytes
  preserved. Interpreter `RunCutsceneMacro_Func @0x00CBFB7D` = text-command, NOT
  bytecode — **case-sensitive `strncmp` PREFIX** dispatch over **184 native
  verbs** (camera, `.Speak`/`.WalkTo`/`.PlayAnimation`, `Crowd*`, `WaitFlag`,
  `GiveHero*`, `SetDoorOpen`, `PlayMusic`…). Actor resolution: empty/"HERO" →
  hero; named → `FindRegisteredCutsceneActor @0x00CD2B86`. Native bridge =
  `CGameScriptInterface` singleton (891 methods, vtable-dispatched).
- **`.qst` (PROVEN):** plain ASCII, CRLF. `AddQuest("Name", TRUE|FALSE);` (TRUE =
  active at start) and `AddTestQuest(...)`. FinalAlbion.qst = 187 AddQuest + 112
  AddTestQuest. **Gotcha:** FSE custom quests need an `AddQuest` line appended;
  a Steam "verify integrity" silently wipes it. `forge quest deploy/doctor`
  handles staging.
- **FSE bindings:** 891 `CGameScriptInterface` methods cataloged; our fork exposes
  engine fns to Lua via `ASLR<>(addr)`. Proven bindings incl. `GoToMapSlot`,
  `PlaySoundId`, `GetGameLanguage`.

---

## 12. Teleport / cross-region movement (ForgeFSE) — PROVEN

`GoToMapSlot(mapSlot,x,y,z)`: `GetRegionNumberMapIsIn @0x004FC190` →
`LoadRegion @0x00500540` (force) → `SetPlayerPos @0x005063E0` →
`ActivateNavMap @0x0050AF10` → `EntityTeleportToPosition`. Bare
`EntityTeleportToPosition` no-ops from a non-adjacent region; the force
`LoadRegion` streams a distant/new region. Correct `CWorld` vtable slot for
`ResolveWorldMap` = **12 (0x30)**; slot 13 crashes.

---

## 13. Saves (`Profile.bin` / `.sav`, FableSave!) — container + editor PROVEN

- **Container:** 307,200-B fixed slab. Header: magic `FableSave!` + `u32
  total_data_len` (offset to trailer) + chunk ulens/clen. Two zlib (`78 DA`)
  sections: HEADER (world state, 23 fields, all confirmed) + chunk1
  (entities/quests). Trailer: seed-0 CRC-32 @ total_data_len, then zero-pad to
  300 KB.
- **SAVED_ENTITIES:** 36-B cell descriptors frame independently-zlib regions;
  each cell inflates to entities `[name\0][u64 UID][DEF-name\0][u32 baseLen][fields]
  [u32 0][u32 compCount][components]`. Hero = `PlayerCreature`/`CREATURE_HERO`
  with a `CTCHeroStats` component. Confirmed hero tags (seed-0 CRC): Money
  `b03ccbfd`, Morality `79a2d479`, Age `663c8f40` (f32), Fatness `b0da59fb`,
  RenownLevel `0c4ed775`.
- **Signature (MANDATORY, no bypass):** `CUserProfileManager::VerifySignature
  @0x00409730` — magic, `u32@8==0x2165`, `tdl+4≤size`, CRC valid, **size ==
  307200**, padding all-zero. `CWorld::LoadGameStateInternal @0x004A21F0`.
- **Gotcha (OPEN):** passing all signature invariants is necessary but **not
  sufficient** — an edited save can still boot to **childhood** (engine finds no
  valid restored HERO → starts fresh rather than rejecting). Under active RE:
  suspected the reframe mis-frames the SAVED_ENTITIES walk. **Lesson:** the game
  is ground truth over an offline decoder.

---

## 14. Frontend / UI / text

- **frontend.bin UI tree:** 785 `CUIDef` widgets (109 fields) forming a scene
  graph (Children = global entry indices); per-state GraphicIndex/Pos/Zoom/Colour;
  TextValue = literal or text.big symbol.
- **Graphic-index namespace is bank-local:** frontend widgets → frontend.big
  (394 DXT1); HUD → textures.big GBANK_MAIN_PC (6,290; HUD_HEALTH_BAR_* @5553–5565);
  effects → particles ids.
- **text.big (`BIGB`, 28,913 entries):** Type 0 (26,807 strings, UTF-16LE +
  SpeechBank/Speaker/Identifier + tags like `ANIM:`/`CAM:`), Type 1 (2,105 groups
  = weighted random variant bundles), Type 2 (narrator name list). Loader
  `NGameText::CDataBank @0x009c7d00`. Write: `text_build.py` (no-edit byte-exact;
  edit/add validated).

---

## 15. Hero, appearance, morphs, demon doors

- **Appearance:** hero = `MESH_HERO` (graphics.big 4299, type-5 skinned). Worn
  items bind via `CAppearanceModifierDef` {Graphics[] worn mesh ids, SuitPart slot
  1–5, CoversBodyAreaFlags, HideMaterials}; worn meshes share the hero rig's
  animation interpolators (`CEnginePrimitiveMorphedAnimatingMesh`). PROVEN 148/148
  vanilla items resolve.
- **Hero morph textures:** base skin sheets `HERO_SKIN_{FACE 1250, ARMS 1248,
  TORSO 1252, BACK 1249, LEGS 1251, MOUTH 1253}`; age/alignment/will/strength/
  fatness are **composited DXT3 overlays** (`COMP_HERO_*`) via
  `CHeroMorphDef.TextureMorphs` (156 records) — no mesh-texture swap. Custom
  tattoo/overlay authoring: `hero_overlay.py` (staged, gates pass).
- **Demon-door face:** `CREATURE_DEMON_DOOR_FACE_01` + `MESH_DEMON_DOOR_FACE_01`
  (16-bone `Demon` rig); 6 single-frame phoneme poses `PHONEME_{AI,EE,MM,OH,ST,WW}`
  with `AMSK` bone mask; lipsync = weighted blend of phoneme poses over dialogue
  LIPSYNC curves (43 fps). Byte-exact vs EgoCore reference.

---

## 16. Controller / input

`CControlsDef` (3 retail schemes decoded): 28-B `CActionInputControl` {GameAction
u32, ControllerType (1 Xbox/2 kbd/3 mouse), key (EInputKey), xbox, mouse,
direction}. **EInputKey = DirectInput DIK scancodes** (not Windows VK): WASD =
0x11/0x1E/0x1F/0x20. EXboxControllerButton 0–18 (13–16 dpad, 17/18 thumbsticks).
EMouseButtonControl 1–7. EGameAction ~90 ids (55=quick-slots, 60=Move, 9/10=Attack;
~75 ids have bindings but unsourced names). PC rumble is dead (vtable stubbed).

---

## 17. Debug-build unlock

The 2013 **Fable Anniversary** build ships `FableWin.pdb` (the level editor — STB
baker, `CTexture`, `CRangeCompressor`, `CLandscapeBackgroundPatch` symbols retail
strips) and `Ego_d.pdb` (269 MB engine symbols; use `ego_r.exe` for code bodies,
`Ego_d.exe` is a 23 KB stub). `FableWin.exe` is the **TLC 1.5 dev editor**
(retail-format output), so its bake is a byte-exact oracle for our emitters.

---

## 18. Coverage at a glance

| Subsystem | Read | Write | Status |
|-----------|------|-------|--------|
| BIG containers (8/8) | 100% | byte-exact roundtrip | PROVEN |
| LZO1X (Lionhead LZ) | full | frame-compatible enc | PROVEN |
| game.bin/frontend.bin defs | 100% (249 types) | field-merge 100% | PROVEN read; write gate pending |
| Meshes (MBANK) | 100% incl geometry | compose + skin | PROVEN |
| Textures (GBANK) | 100% | build (PSNR reconverge) | PROVEN |
| Animations (3DAF) | 100% (3,435) | byte-exact | PROVEN |
| Lipsync | 100% (20,505) | byte-exact | PROVEN |
| Effects | 100% (1,165) | edit/rebuild | PROVEN |
| Audio .lut/.lug | 100% (20k+ clips) | byte-exact + add | PROVEN (runtime add untested) |
| Dialogue join (snds.bin) | solved | replace/add | PROVEN read; add live-untested |
| .lev + navmesh | 100% (398) | lev byte-exact; nav gen | PROVEN (nav ~7% over-approx) |
| World .bwd | 100% | byte-exact | PROVEN |
| STB container + InfoBlock | 100% | byte-exact | PROVEN |
| STB baked terrain chunk | parsed + round-trip | emitter in progress | RECIPE (same-topology) / OPEN (arbitrary) |
| Cutscenes (595) | 100% | edit/re-encode | PROVEN |
| Quest VM / .qst | mapped | .qst append | PROVEN (logic is native C++) |
| Saves (FableSave!) | 100% | editor + re-sign | PROVEN (childhood-restore OPEN) |
| Text.big | 100% (28,913) | build | PROVEN |
| Appearance / hero morph | 100% | overlay author | PROVEN / staged |
| Demon-door lipsync | validated | — | PROVEN |
| Controller enums | empirical | data-edit | PROVEN (some names unsourced) |
| Custom region (live) | — | full assembly | **PROVEN live (teleport + walk)** |

**Biggest open items:** STB baked-chunk HDR quadtree grammar (arbitrary new
terrain topology); mesh vertex/index engine call-site for the chunked-LZO inflate
(on-disk format proven, load site TBD); childhood-save trigger past signature;
live engine-acceptance tests for audio/dialogue *adds*; font glyph-metric format.

---

*Shared freely. Addresses are for those exact binaries — verify against yours.
Happy to hand over any of the byte-exact reader/writer code (a C++ lib + Python
tools) or trade format notes.*
