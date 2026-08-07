# EgoCore assessment — relevance to our modding goals (2026-07-31)

Triggered by the EgoCore GitHub update. **EgoCore** (github.com/eeeeeAeoN/EgoCore, by "AeoN") is an
actively-developed, open-source (free) C++ modding framework for Fable: The Lost Chapters. It just cut
its **first stable release, `31.7.26`** (today), leaving beta after ~4 months. Local copy studied:
`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master` (~July).

Three cross-reference agents mapped EgoCore's source against our docs (`DEF_LOAD_CONTRACT`,
`MESH_COMPOSE`, `BIG_ANIM_FORMAT`) and open gaps. **Bottom line: EgoCore independently confirms almost
every one of our reverse-engineering findings field-for-field, extends several, contradicts two (where
we should trust our own byte-proven data), and hands us working reference code for two long-standing
blockers (Mario-rig retarget, custom-NPC lipsync).**

---

## 1. Highest-value takeaways (actionable now)

1. **Mario-rig retarget is unblocked.** `Animations\GltfAnimImporter.h` retargets animation *tracks*
   into the target skeleton's own **parent-relative bind-local space** (derived from the target mesh's
   raw bone matrices: `bindLocal[i] = bindGlobal[i] × IBM[parent]`), re-baking translations/rotations
   per-bone (delta-from-bind for anim Type 7), with `PositionFactor` auto-scaled from actual extents.
   This is exactly the "animation translation retargeting" our HANDOFF said to try first and never
   built. **Action:** port `GltfAnimImporter::Import` + `AnimProperties::UpdateStandaloneBones` as the
   Mario harness; stop iterating per-piece bind-axis placement (the `work/mario_hero/stage_bindaxis4`
   dead-end in CLAUDE.md).

2. **Ghost-LOD rule is broader than our doc says — fix `MESH_COMPOSE.md`.** The authoritative bank-write
   path (`Banks\BankEditor.h:1028`, `FlushStagedEntries`) appends one uncounted empty ghost LOD after
   the last real LOD for **types 1, 2, 4, AND 5** — not just 1/5. Our doc cites the *stale*
   `MeshCompiler::CompileForExport` (2/5-only). The dev's own comment ("no idea why static meshes didn't
   crash before... TO INVESTIGATE") flags it as a defensive crash-guard. **Action:** update the doc; emit
   a ghost LOD for any type-1/2/4/5 compose.

3. **Custom-NPC lipsync can be auto-generated.** `Lipsync\SpeechAnalyzer.h` turns a WAV into a lipsync
   entry: 512-sample frames @22.05 kHz, Hann-windowed DFT, 5-phoneme (AA/EE/MM/OH/SZ) nearest-match,
   top-3 weighted keys/frame. Critical engine gotcha it encodes: **empty frames must emit ≥3 zero-weight
   keys or the mouth hangs open**. Pairs with our validated dialogue.big/.lut/snds.bin add path.

4. **"Engine-as-compiler" is a ground-truth oracle for def work.** EgoCore's production def path
   (`Definitions\CompilerBackend.h:230-313`) does NOT append to retail bins — it deletes
   game.bin/names.bin/frontend.bin/script.bin and **regenerates them by driving `Fable.exe` headless**
   (patches `dbugst.ini`: `AllowDataGeneration TRUE; UseCompiledDefs TRUE; BuildRetailStaticMaps TRUE`;
   one-byte force-patch at IDA VA `0x00C90613+6`). Even if FableForge keeps surgical-append, this gives
   us a **byte-exact generator to diff against** — settles the open A/B/C question in DEF_LOAD_CONTRACT.

## 2. Confirms our findings (independent corroboration)

- **crc0 hash** — EgoCore ships it as `CalculateFableCRC32`/`CalculateCRC32_Fable`: reflected CRC-32,
  poly `0xEDB88320`, **seed 0, no final inversion**, used for game.bin field tags (`WriteTag`). It even
  ships the *standard* variant (`0xFFFFFFFF` seed + `~crc`) separately — the exact "NOT ~crc32" negative
  we documented. Two independent implementations now agree with our `CCharString::ComputeCRC32 @0x404310`.
- **Def registry** keyed by `crc0(className)` → `map<u32, DefClassInfo{Name,NameCRC,AllocFunc}>` — our
  `map<unsigned_long, CDefClassInfo>` exactly. Header layout (names.bin magic `0x7AB1E`, randomID, count,
  tableSize; game.bin safeBool+randomID+noDefs) matches; chunk offset-bias (`nInChunk*2`) matches.
- **3DAF anim** (`AnimParser.h`) — payload = `u32 decompSize` + ONE raw LZO1X stream (magic `0x3E3E3E3E`
  = already-uncompressed sentinel); chunks `[fourcc][u32 size]`, nesting, MVEC superchunk; ONE XSEQ per
  bone track; XSEQ track = quat `f32[4]` pool + palette + `i16[3]×PositionFactor` pos pool + palette,
  palette-index width conditional on pool>255. All byte-exact with our `§9.2`.
- **Mesh skinning** (`GltfMeshImporter::ImportType5`) — BonesPerVertex=3, PalettedFlag=1, max 3
  influences (4th zeroed), **weights sum to 255** (remainder dumped into `fw[0]`), joint byte =
  paletteIndex×3, bone blocks (60B each) cloned raw. Confirms our `mesh_rw` observations write-side.
- **DegenerateTriangles** trailing dummy-material sentinel + **LODSizes[] = per-LOD byte sizes** (ghost
  excluded) + Info-blob field order — all confirmed against `BankEditor.h:1065-1073` / `MeshCompiler.h`.
- **Lipsync id chain** — `FetchLipSyncData(soundID)` looks up dialogue.big by `entry.ID == soundID`,
  confirming our "LIPSYNC id == .lut clip Index == soundID". FPS = `22050/512 = 43` (our `blockMarker
  0x2B`). SubBank routing + `SPEAKER_FEMALE1` dep confirm DEMON_DOOR/DIALOGUE_PIPELINE.

## 3. Extends us (new capability / detail we lacked)

- **Full mesh-type map** (`Utilities\Utils.h`): 1 static, **2 static-repeated/instanced**, **3 physics
  (CMESH/BBM collision)**, **4 particle**, 5 skinned. We only had 1 and 5.
  - **Type-2 (instanced foliage/grass)** is a cheap new capability: type-1 geometry + `RepeatingMeshReps`,
    forced **stride-36 uncompressed** vertex layout (a *third* static layout beyond our float-20/packed-12),
    with index/vertex buffers physically tiled ×reps (cap 65535). Our `MESH_COMPOSE §4` lists this as
    unsupported.
  - **Type-4 particle** compiles via the same `CompileSingleLOD` grammar.
  - **Type-3** is BBM physics (separate `3DMF` writer), wired to a mesh via the Info-blob `PhysicsIndex`
    — so a collidable custom prop needs a paired type-3 entry (we emit `PhysicsIndex=0`).
- **Byte-exact game.bin parity details** we don't document: (a) compressed stream is **zlib deflate
  level 1 fed one byte at a time** to bit-match Lionhead; (b) game.bin carries **`dependencyCRC =
  0xE86E4CDE`** after the safeBool byte; (c) a per-class **`NULLDEF_<name>`** string + null instance is
  emitted — check whether retail names.bin carries these and whether our append tool must preserve them.
- **Redefine-keys input half** (`DefObjects.h:131-171`): hardcoded **KB_\* (119), MOUSE_\* (13),
  XBOX_PAD_\* (19)** name→index tables (plain 0-based array indices) + the UI field set
  (`ActionText/KeyText/Redefiner/UndefinedWarning`, `CActionInputControl` 28-byte layout). Directly
  liftable for control-scheme authoring.
- **Anim authoring details** (`AnimCompiler.h`): BoneIndex sentinel **`31450`** = "no global rig ID";
  parent-index remap via topological sort; constant-track collapse; Scene Root / Movement* tracks skip
  palette compression; writer = re-serialize + `lzo1x_1_compress` + `u32 decompSize` prefix (confirms §9.5).
- **Anim Type semantics**: Type 6 = normal, **Type 7 = DELTA (bind-relative)**, **Type 9 = has AMSK
  partial-body mask** (auto-detected from AMSK bits), not strictly "single-frame phoneme pose."

## 4. Contradicts us — trust OUR byte-proven data, not EgoCore

- **`indexInDefinition` dense counter.** EgoCore's *standalone* writer (`DefinitionManager.cpp:513-517`)
  writes `classIndex = 0` for every roster entry. This contradicts our byte-proven finding that the
  third main-table dword is a per-class dense index (0..N-1) and that entry order IS the runtime global
  index. EgoCore's standalone writer is only wired for `frontend.bin` (all-UI) and may not exercise the
  field; **keep FableForge's `bin.cpp:243 perDefinition[def]++`** — our retail diff wins. (A/B-testable.)
- **names.bin string-table CRC.** EgoCore's `CDefStringTable::GetCRC` uses the **standard** CRC
  (`0xFFFFFFFF` + `~crc`) **with `tolower`** — which by our 13593/13593-name proof is the *wrong* hash
  for names.bin (crc0, no lowercasing). EgoCore's own code is internally inconsistent here (tag CRC =
  crc0, string-table CRC = standard). **Do not copy `CDefStringTable::GetCRC`; keep crc0.**
- Minor: EgoCore's `EControllerType` map (`DefObjects.h:112-115`) disagrees with its own header comment
  (XBOX_PAD→1 vs comment's 0). Verify against retail before trusting either.

## 5. `EGameAction` name-gap — closed for the frontend ActionOrder

EgoCore does **not** contain the `EGameAction` integer→display-name table (grep `GAME_ACTION_*` /
`GameActionNames` = 0). Its `CActionInputControl.GameAction` reads a raw int from whatever `GAME_ACTION_*`
symbol appears in the parsed def-header `.h` enum — i.e. it *relies on the very source we're missing*.
So EgoCore alone still has no display-name table. The repository's local `debug_build/FableWin.pdb`
does, however: DIA recovers `GAME_ACTION_NULL = 0` through `GAME_ACTION_BETTING = 114` and the
`GAME_ACTION_COUNT = 115` sentinel. This closes the authoritative integer-to-enum-identifier gap
and covers every Redefine `ActionOrder` id. All 31 frontend ActionOrder correlations now resolve
through `TEXT_GUI_ACTION_*`, with the enum and display layers recorded as
`REDEFINE_ACTION_ENUM_NAMES` and `REDEFINE_ACTION_DISPLAY_TEXT`. EgoCore ships the sibling
KB/MOUSE/XBOX tables as plain
ordered index arrays, which confirms the missing EGameAction table is likewise a simple ordered enum —
reinforcing that the enum is an ordered table, not a hash. The remaining
frontend work is exact localized binding/capture parity while scrolling; the
source recovery and a native diagnostic page materialization are now closed.
EgoCore closes the key/button half today.

## 6. Concrete follow-ups (filed / recommended)

- [ ] **Update `MESH_COMPOSE.md`**: ghost LOD applies to types 1/2/4/5 (not just 1/5); document type-2
      instanced (stride-36 + tiling), type-4 particle, type-3 BBM-via-PhysicsIndex, and the third static
      vertex layout (stride 36 uncompressed).
- [ ] **Update `BIG_ANIM_FORMAT.md`**: one-line note that Type 9 = AMSK-masked (not only phoneme pose);
      add BoneIndex sentinel `31450`.
- [ ] **Update `DEF_LOAD_CONTRACT.md`**: add zlib-level-1-one-byte-at-a-time, `dependencyCRC=0xE86E4CDE`,
      `NULLDEF_*` per-class entries; note the engine-as-compiler generator as a parity oracle.
- [ ] **FableForge**: keep crc0 + dense-index (do NOT adopt EgoCore's GetCRC/classIndex=0); consider
      lifting the KB/MOUSE/XBOX name tables; evaluate the engine-as-compiler path as a validation oracle.
- [ ] **Port** `GltfAnimImporter::Import` + `UpdateStandaloneBones` for the Mario rig; evaluate
      `SpeechAnalyzer` for custom-NPC lipsync.

Source references throughout are to the local EgoCore copy; EgoCore is free/open-source (see its LICENSE).
This assessment is our own synthesis of behavior + file:line pointers, not a copy of EgoCore source.
