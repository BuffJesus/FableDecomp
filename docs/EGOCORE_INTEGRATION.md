# EgoCore Integration

*Analysis of the EgoCore community reference source and its integration into our RE hub.*
*Source: `C:/Users/Cornelio/Documents/EgoCoreInspect/EgoCore-master/` (also `D:/Downloads/EgoCore-master.zip`).*
*Written 2026-07-20. Read-only pass over EgoCore + our docs/forgecore; no source was edited.*

---

## 0. TL;DR — the append question (highest-value)

**Does EgoCore contain native def-container append / cross-reference LINKING logic for
`game.bin`?  → NO for `game.bin`; YES (partial) for `frontend.bin` only.**

EgoCore ships **two** def toolchains:

1. **`FableDefCompiler/`** — a *native* writer that compiles `.def` text → **`frontend_custom.bin`
   + `names_custom.bin`** entirely in-process (no game exe). It implements only the **8 frontend
   def classes** (FRONT_END, UI, ENGINE, ENGINE_VIDEO_OPTIONS, CONTROL_SCHEME,
   CONFIG_OPTIONS_DEFAULTS_DEF, UI_MISC_THINGS_DEF, UI_ICONS_DEF). It does **not** implement
   OBJECT / THING / CREATURE / any `game.bin` class, and it has **no cross-reference/link-resolution
   pass** — see §2.4.
2. **`EgoCore/Definitions/CompilerBackend.h`** — the "stealth compiler". For **`game.bin` /
   `script.bin`** EgoCore does **not** author the container at all. It **drives the real
   `ego_r.exe`** in a hidden, minimized window (`-build_retail_static_maps`), after (a) patching
   `dbugst.ini`/`userst.ini` to `UseCompiledDefs TRUE` etc. and (b) a live 1-byte
   `WriteProcessMemory` patch at `ego_r.exe+0x00C90613+6`, then restores the INI. The game writes
   `game.bin`/`names.bin` itself.

So EgoCore's own authors concluded that native `game.bin` authoring was **not worth solving** and
delegated it to the engine. **Our RE is ahead of EgoCore here**: `forge::bin` already reads *and*
writes `game.bin`/`names.bin` natively (semantic round-trip, 14761/14761), and `forge::defdecode`
already decodes+encodes+field-merges payloads with the correct field-tag CRC. EgoCore does **not**
advance our native-append effort with new link logic — but it **independently confirms** our
container writer's algorithm bit-for-bit (chunking, zlib quirk, names.bin CRC/header, main-table
`{defNameOff, nameOff, indexInDefinition}`), which is strong validation. Details in §2.

The one genuinely new, directly-usable asset is EgoCore's **exact `frontend.bin` chunk-writer**
(`CDefinitionManager::SaveBinaryDefinitions` + `CompressBlock`), including the notorious Lionhead
"1 byte at a time" `deflate` loop and the 0x8000 chunk threshold — see §2.3. If we ever want native
`frontend.bin`/`script.bin` authoring (multi-bin merge is a live task), this is the reference.

---

## 1. Tree inventory

Root: `EgoCore-master/`. Two Visual Studio projects:

- **`EgoCore/`** — the main ImGui/DX11 tool (parsers, compilers, editors, renderers).
- **`FableDefCompiler/`** — a standalone native frontend-def compiler (linked into EgoCore as
  `DefCompiler.h`/`FableCompiler::CompileFrontendNative`).

### 1.1 EgoCore/ modules (by category)

Ignored: `Utilities/ImGUI/` (third-party Dear ImGui), `*.tlog` / `x64/` build output.
"WRITER" = can emit the game format, not just read it.

| Category | Files | Format(s) | Reader | **Writer/Compiler** | Notes |
|---|---|---|:--:|:--:|---|
| **Animations** | AnimParser, AnimCompiler, AnimProperties, GltfAnimImporter | `.anim` (magic `3DAF`/`0x3E3E3E3E`, chunks ANRT/HLPR/AOBJ/XSEQ/TMEV/MVEC/AMSK/XALO) | ✅ | ✅ **AnimCompiler** (LZO); glTF→anim importer | validated 3DAF (our "answer key" for anim) |
| **Audio** | LugParser, MetParser, AudioBackend, AudioExplorer, AudioHelpers | `.lug` (`LiOnHeAd` sig), `.met` | ✅ (.lug) | ✅ **MetParser** writes `.met`; AudioBackend writes LiOnHeAd | `.lug` is parse-only; `.met` metadata is generated |
| **Banks** | BankLoader, BankBackend, BankExplorer, BankEditor, BankTabUI, BigBankCompiler, BinaryParser | `.big`/`BIGB` (magic `BIGB`, version @+4), `.bin` sound | ✅ | ✅ **BigBankCompiler** (subbank align); **BinaryParser::CompileSoundBinaries** (`.bin`) | dual CRC32 (std seed 0xFFFFFFFF + Fable seed 0) |
| **FSE** | FSEBackend, FSEAutosuggest, FSETabUI | FSE script editor | — | — | editor UX only; no bin format |
| **Fonts** | FontParser, FontBuilder, StreamingFontParser, StreamingFontBuilder, *Properties | legacy font, streaming font (type 0/1/2, LZO glyphs) | ✅ | ✅ **FontBuilder** (TTF→legacy, stbtt); **StreamingFontBuilder** (TTF→streaming, LZO 1024/4096 blocks) | |
| **Levels** | WADBackend | `.wad` (entry footer @+28) | ✅ | ❌ (unpack only) | no WAD writer — our `forge wad` is ahead |
| **Lipsync** | LipSyncParser, LipSyncCompiler, LipSyncProperties, SpeechAnalyzer | lipsync bank (BIGB, entry magic 42, ver 100) | ✅ | ✅ **LipSyncCompiler** | validated demon-door phonemes (our answer key) |
| **Meshes** | MeshParser, MeshCompiler, BBMParser, MeshProperties, MeshRenderer, GltfMeshImporter, GltfExporter | `.c3d` (LZO), `.bbm` collision | ✅ | ✅ **MeshCompiler** (`.c3d`, LZO bone/mat/prim blocks); ✅ **GltfExporter** (glTF 2.0 .gltf/.glb); glTF→mesh importer | validated bone blocks (our answer key) |
| **Mods** | ModManagerBackend, ModManagerCompiler, ModManagerUI, TngMerger | mod package orchestration; `.tng` merge | ✅ | ✅ **ModManagerCompiler** (orchestrates Bank/Mesh/Particle compilers for asset replacement) | asset-replacement model (not def-append) |
| **Particles** | ParticleParser, ParticleCompiler, ParticleProperties | emitter binary (magic `MPSA`/`0x4D505341`) | ✅ | ✅ **ParticleCompiler** (splines/orbit/flags) | |
| **Shaders** | ShaderParser, ShaderCompiler, ShaderProperties | D3D bytecode (VS `0xFFFE****`, PS `0xFFFF****`) | ✅ | ✅ **ShaderCompiler** (D3DAssemble via d3dcompiler_47.dll → Fable envelope) | |
| **Text** | TextParser, TextCompiler, TextBackend, TextProperties | text bank (BIGB, len-prefixed strings) | ✅ | ✅ **TextCompiler** | complements our text.big work |
| **Textures** | TextureParser, TextureBuilder, TextureExporter, TextureProperties, ImageBackend | Fable graphic (DXT1/3/5, ARGB8888, normal), `.dds` | ✅ | ✅ **TextureBuilder** (PNG/JPG→DXT+mips); ✅ **TextureExporter** (→DDS, magic `0x20534444`) | validated LZO texture (our answer key) |
| **Definitions** | DefBackend, DefExplorer, CompilerBackend, EventBackend | `.def`/`.tpl` text; anim-event `.txt`; drives container compile | ✅ (text) | ✅ text `.def` CRUD; ✅ anim-event `.txt`; container via ego_r.exe | **see §2** |
| **Utilities** | Utils, InputManager, ConfigBackend, FileDialogs, minilzo, stb_*, miniaudio | LZO, image/audio libs | — | — | vendored libs (minilzo, stb_dxt, stb_image) |

### 1.2 FableDefCompiler/ (the native frontend writer)

| File | Role |
|---|---|
| `DefCompiler.cpp` | `FableCompiler::CompileFrontendNative(root)` — wires up the 8 frontend classes, symbol-header list, def-file list; writes `Data/CompiledDefs/frontend_custom.bin` + `names_custom.bin` + a per-def `DebugDump/`. |
| `DefinitionManager.{h,cpp}` | The container writer: symbol map build, 2-pass `.def` parse, `SaveBinaryDefinitions()`, `CompressBlock()`. **§2.3 crown jewel.** |
| `DefObjects.h` | The 8 frontend def classes + `CPersistContext::Transfer` overloads; `IDefObject`/`CDefObject` base with `Transfer`/`CopyFrom`/`GetClassName`/`GetInstantiationName`. |
| `BinaryStreams.h` | `CDefStringTable` (names.bin writer, CRC), `CPersistContext` (tag protocol + text scanner + binary emit), `CMemoryDataOutputStream`. **§2.1/§2.2.** |
| `StringParser.{h,cpp}` | Lionhead-style lexer (`CStringParser`) used to tokenize `.def` text. |

### 1.3 Writers EgoCore has (native emit)

`.anim`, `.met`, `.big`/BIGB banks, sound `.bin`, legacy + streaming fonts, `.c3d` meshes, glTF
export, particle binaries, D3D shaders, text banks, textures (DXT + DDS), mod packages, and
**`frontend.bin`+`names.bin`** (frontend classes only). **No writer** for: WAD levels, FSE scripts,
`.lug` audio banks (parse-only), and **`game.bin`/`script.bin` defs** (delegated to `ego_r.exe`).

---

## 2. DEF CONTAINER + LINKING (append-relevant deep dive)

This is the section the append crack cares about. Everything here is from `FableDefCompiler/` and
`EgoCore/Definitions/`; the inventory agent confirmed **no other module** references
`CompiledDefinition`, `CDefIndex`, `globalEntryIndex`, `ApplyLinks`, `indexInDefinition`,
`defNameOff`, or `GlobalIndex`.

### 2.1 names.bin writer — `CDefStringTable` (`BinaryStreams.h`)

Confirms our `forge::bin` names.bin model exactly.

- **Header (16 bytes as written):** `u32 magic=0x7AB1E`, `u32 m_RandomID`, `u32 m_StringCount`,
  `u32 streamLength`, then the string stream.
  - Note our reader (`forge/bin.hpp`) documents a **20-byte** header with magic at +4 = `0xA8E36C34`
    on the retail PC build. EgoCore's magic `0x7AB1E` and its `m_RandomID` default `0xDEADBEEF` are
    the *compiler's* values; the retail `randomID` observed in DefinitionManager is `0xA8E36C34`
    (used as the game.bin `randomID` field — see §2.2). **The retail names.bin "magic" our reader
    sees at +4 is actually this per-build randomID**, and the true first u32 is a separate magic —
    EgoCore's layout reconciles the two: `[magic][randomID][count][length]`. This resolves a small
    ambiguity in our header doc (see §3, GAP).
- **String stream:** starts with a 4-byte zero pad (so offset 0 is reserved/"empty" =⇒ `(u32)-1`),
  then per string: `u32 crc` + `ASCIIZ name`. **Offsets stored elsewhere point at the `crc` word**
  (i.e. relative to the stream start; our reader calls it "relative to end of header" — same anchor).
- **CRC = seed `0xFFFFFFFF`, reflected poly `0xEDB88320`, final invert, lowercased input**
  (`CDefStringTable::GetCRC`). This is the **name/string CRC**, *distinct* from the field-tag CRC
  (§2.2). Matches our note: name CRC seeds 0xFFFFFFFF; field-tag CRC seeds 0.
- **Dedup:** `AddString` returns the existing offset if the string was already added — so a merged
  names.bin that only re-adds existing strings is byte-stable. Confirms our "unchanged name sets
  produce byte-identical names.bin" guarantee.

### 2.2 game.bin / frontend.bin container layout — `SaveBinaryDefinitions()`

The write order (frontend classes, but the **container framing is identical** to game.bin) is:

```
[u8  safeBinary=0]
[u32 dependencyCRC = 0xE86E4CDE]      // frontend's; game.bin has its own
[u32 randomID      = 0xA8E36C34]      // == retail PC randomID (ties bin↔names)
[u32 noDefs]                          // entry count
// --- main/roster table: noDefs x ---
  [u32 classTablePos]   // names.bin offset of the DEFINITION/class name  (== defNameOffset)
  [u32 instTablePos]    // names.bin offset of the instance name          (== nameOffset)
  [u32 classIndex = 0]  // == indexInDefinition   (ALWAYS 0 here; see §2.4)
// --- chunk map ---
[u32 chunkMapCount]
  chunkMapCount x { u32 firstDefIndex, u32 compressedOffset }
[u32 compressedSize]
[ compressed stream ]                 // zlib chunks
```

This **matches `forge/bin.hpp` field-for-field**: our `{ s32 defNameOffset, s32 nameOffset,
s32 indexInDefinition }` main table = EgoCore's `{classTablePos, instTablePos, classIndex}`; our
chunk table `{ s32 firstEntry, s32 compressedOffset }` = EgoCore's `{firstDefIndex,
compressedOffset}`. The 9-byte game.bin header our reader preserves = EgoCore's
`[u8 safe][u32 depCRC][u32 randomID]` (+`u32 count` = "@9"). **Full independent confirmation of our
container model.**

**Per-entry payload** (`CPersistContext` MODE_SAVE_BINARY):
- `TransferObjectHeader(version)` emits `[u8 1][s32 version]` (version 256 = `00 01 00 00`; UI uses
  257). This is (part of) the "variable untagged base-class prefix" our decoder documents.
- Each field: `WriteTag(name)` = `[s32 fieldTagCRC]` then the value. **Field-tag CRC =
  `CalculateFableCRC32`: seed 0, reflected `0xEDB88320`, NO final invert, case-sensitive input.**
  → **This is EXACTLY our `forge::defdecode::fieldTag` (seed-0 reflected CRC-32, no inversion).**
  EgoCore independently confirms the single most important RE result we have for game.bin.
- Value encodings match ours: int32/uint32/float/enum = 4B; bool = 1B; `CCharString` =
  chars+`\0`; `CWideString` = UTF-16LE + u16 NUL; `Vector_<T>` = `[s32 count][T…]`; `CDefString`
  (an object/graphic ref) = `[s32 names.bin offset]` (or `-1`). Map = `[s32 count]{key,val}…`.

### 2.3 The chunk writer — `CompressBlock()` (crown jewel for native frontend authoring)

The exact algorithm, reproduce verbatim if we ever add native frontend/script writing:

1. Serialize defs in order into `uncompressedBuffer`, recording each def's start as a `u16` offset.
2. **Chunk threshold:** before appending def *i*, if
   `bufferSize + offsetTableBytes + thisDefSize + 2 > 0x8000` (32 KiB) and the buffer is non-empty,
   flush the current block. (Our writer's "≤32 KiB zlib chunks" = this.)
3. **Block layout before compression:** `[u16 offset]*N` then the concatenated def bytes, where each
   stored offset is **shifted by `N*2`** (the offset-table size) so it points into the final
   inflated block. (This is the "each inflated chunk starts with u16 payload offsets" our reader
   documents.)
4. **Compression = Lionhead zlib quirk:** `deflateInit(level=1)`, then feed **1 input byte and take
   1 output byte per `deflate(Z_NO_FLUSH)` call** in a loop, then finish 1 byte at a time with
   `Z_FINISH`. This "1-byte-at-a-time loop of madness" reproduces the *exact* compressed bytes the
   retail generator emits (level-1, tiny-buffer streaming changes deflate block boundaries). If we
   ever need **byte-identical** (not just semantic) frontend.bin output, this is mandatory. Our
   `forge::bin` is semantic-round-trip only (re-chunks/re-compresses) — acceptable for game.bin,
   but this is the recipe for bit-exact.
5. `chunkMap.push_back({firstDefIndexInBlock, offsetIntoCompressedStream})`.

### 2.4 LINKING / cross-reference resolution — how refs are stored

**The key append finding.** There is **no post-pass "ApplyLinks" reindex step** in EgoCore, because
the on-disk format doesn't need one:

- **`classIndex` / `indexInDefinition` is written as literally `0`** for every entry
  (`SaveBinaryDefinitions` hardcodes `uint32_t classIndex = 0`). So this third main-table column is
  **not** a global cross-reference index in the frontend writer — it is a within-definition ordinal
  that the frontend path leaves 0. (In retail `game.bin` it is populated; our reader keeps it and
  our writer preserves it verbatim — correct.)
- **Object/graphic/def references inside payloads are stored as `names.bin` string offsets**
  (`CDefString` → `[s32 offset]`), NOT as global entry indices, in the frontend classes EgoCore
  implements. The engine resolves offset→string→object at load.
- **BUT** for `game.bin` gameplay refs (e.g. a chest's `OpenerObject`, reward `ObjectFamilies`), our
  own RE (FINDINGS.md 2026-07-18, §"resolves the OpenerObject global def index back to a def name")
  established these are stored as **global entry indices** = the **row position in the main table**,
  resolved index↔name at runtime via `CDefinitionManager::GetDef<>`. **Entry order IS the global
  definition index** (our `forge/bin.hpp` header comment).

**Implication for native append (this is the actionable conclusion):**
Because cross-refs in `game.bin` are **global row indices**, *appending* new entries at the end is
safe **iff you never renumber existing rows**:
- **APPEND at the tail** (new rows after the last existing entry) → every existing index is
  unchanged → no link fixup needed. This is exactly what our hero-overlay / stat-item tools already
  do ("append {…} records", "156→158 records") and it round-trips 14761/14761.
- **INSERT / delete in the middle** → every ref whose target sits at or after the insert point
  shifts → you must rewrite those payload indices. EgoCore has **no** code for this (it never
  inserts; the stealth compiler regenerates the whole container from text). Our field-decoder
  (`forge::defdecode`) is the tool that *could* do a reindex pass, but the cheap, proven path is
  **tail-append + patch only the specific ref you intend to add**.
- **Nameless companion defs** (`name == ""` sub-defs, e.g. inline `<CChestDef …>` inside an OBJECT):
  EgoCore's `CompileDefinition` recursively compiles inline `<Class Name …>` sub-defs as **separate
  top-level `m_InstantiatedDefs` entries** in encounter order (DefinitionManager.cpp
  `CompileDefinition`, the `'<' … '>'` scan). So a parent + its companion sub-defs occupy
  **consecutive main-table rows**, and the parent references the child by that row index. When
  appending such a group, **append the whole group contiguously** and point the parent's ref at the
  child's new row index. This matches our observed sub-def grouping (`name:"" for sub-defs` in
  `forge::bin::Entry`).

**Bottom line:** EgoCore gives us **no new link-resolution algorithm** we didn't already have — it
*confirms* that native tail-append needs no global reindex, which de-risks our existing append path.
The only place EgoCore would beat a from-scratch effort is bit-exact frontend chunking (§2.3).

### 2.5 The `.def` text→object compiler (bonus: schema/authoring reference)

`CDefinitionManager` is also a working **`.def` text compiler** we don't have. Useful pieces:
- **2-pass compile:** templates (`#definition_template`) first, then `#definition`; `specialises`
  copies a parent then overrides (`CopyFrom`). Redefining without `specialises` resets to a fresh
  object (vanilla parity).
- **Symbol map:** parses `enum {…}` (with `<<`, `|`, `+`, `-` expression eval) and `#define` from
  header `.h` files into an int symbol table, so identifier field values resolve.
- **`CPersistContext` MODE_LOAD_TEXT:** the field scanner — finds `FieldName value;` as whole words,
  reads the typed value, then **blanks the consumed region** so it can't re-match (mirrors the
  engine's `TransferObjectLoadText`). Handles `Vec.Add(...)`, `Vec[i] = ...`, `Map[k] = v`,
  `Vec.clear()/resize()`, and comment stripping. This is a concrete reference for a future
  **FableForge `.def` text ⇄ binary** authoring path (we currently edit payloads at the byte level).
- The 8 frontend classes' full field lists + default values + `Transfer` order in `DefObjects.h`
  are a ready-made schema for those types (cross-check against our `def_schema.json`).

---

## 3. EgoCore ↔ our RE: confirm / gap / writer map

| Our format / tool | EgoCore module | CONFIRMS | Fills a GAP | Has a WRITER we lack |
|---|---|---|---|---|
| `game.bin` container (`forge::bin`) | FableDefCompiler `SaveBinaryDefinitions` | ✅ header, main table `{defNameOff,nameOff,indexInDef}`, chunk table, 0x8000 chunking, zlib | — | No (we already write it; ours is more complete — game.bin classes) |
| game.bin **field-tag CRC** (`forge::defdecode::fieldTag`) | `CPersistContext::CalculateFableCRC32` | ✅ **seed-0 reflected CRC, no invert** — exact match | — | — |
| names.bin (`forge::bin`) | `CDefStringTable` | ✅ crc(seed 0xFFFFFFFF, lowercased) + ASCIIZ, offset anchor, dedup stability | ⚠ clarifies header = `[magic 0x7AB1E][randomID 0xA8E36C34][count][len]` (our reader labels the +4 word "magic"; it's the randomID) | — |
| field value encodings (`forge::defdecode`) | `CPersistContext::Transfer*` | ✅ int/uint/float/enum=4B, bool=1B, CCharString `\0`, CWideString UTF-16+NUL, Vector `[count][T]`, CDefString=offset | — | — |
| `frontend.bin`/`script.bin` native write | FableDefCompiler + CompressBlock | — | ⚠ **bit-exact chunker** (1-byte deflate loop) if we want byte-identical multi-bin authoring | ✅ **native frontend writer + `.def` text compiler** (we lack both) |
| def cross-ref = global row index | our FINDINGS + EgoCore stealth model | ✅ append-at-tail = no reindex; EgoCore never inserts | — | — |
| WAD levels (`forge wad`) | Levels/WADBackend | ✅ entry footer @+28 | — | ❌ EgoCore unpack-only — **we are ahead** |
| `.big`/BIGB banks (`forge big`) | Banks/BankLoader + BigBankCompiler | ✅ magic `BIGB`, ver@+4, subbank align | — | ✅ EgoCore has a bank **compiler** (subbank align) — compare vs our BIG_WRITER |
| meshes (BIG_MESH_*) | Meshes/MeshCompiler, GltfExporter | ✅ `.c3d` LZO bone/mat/prim blocks (our answer key) | ⚠ MeshCompiler is a full **c3d writer**; GltfExporter is glTF-out | ✅ **c3d writer + glTF export** — relevant to custom meshes |
| textures (BIG_TEXTURE_*) | Textures/TextureBuilder/Exporter | ✅ DXT1/3/5 + LZO (answer key), CGraphicHeader mips | — | ✅ **PNG/JPG→DXT+mips builder + DDS export** |
| animations (ANIM_WRITER) | Animations/AnimCompiler | ✅ 3DAF chunks (answer key) | — | ✅ **anim compiler + glTF→anim** — compare vs our ANIM_WRITER |
| lipsync (DEMON_DOOR_FACE) | Lipsync/LipSyncCompiler | ✅ phonemes (answer key), BIGB entry magic 42 ver 100 | — | ✅ **lipsync bank compiler** |
| audio (LUG/LUT) | Audio/LugParser, MetParser | ✅ `LiOnHeAd` sig | ⚠ `.met` metadata gen (sidecar) | partial (`.met` only; `.lug` parse-only) |
| text.big (task #14) | Text/TextCompiler | ✅ BIGB len-prefixed strings | ⚠ working text-bank **writer** | ✅ **text bank compiler** |
| particles (EFFECTS_FORMAT) | Particles/ParticleCompiler | ✅ magic `MPSA` | ⚠ spline/orbit encode | ✅ **particle compiler** |
| shaders | Shaders/ShaderCompiler | ✅ VS/PS bytecode envelope | ⚠ D3DAssemble recompile path | ✅ **shader compiler** (d3dcompiler_47) |
| fonts | Fonts/*Builder | — | ⚠ TTF→legacy + streaming (LZO) | ✅ **font bakers** |
| `.fmp`/mod packaging (task #13) | Mods/ModManagerCompiler, TngMerger | ✅ asset-replacement model | — | different model (per-asset compile vs our record-delta merge) |
| STB terrain baker | *(none)* | — | ❌ EgoCore has **no STB/terrain writer** | ❌ no help here |
| FSE scripting | FSE/* | — | ❌ editor UX only, no bindings | ❌ no help |

---

## 4. Concrete next actions EgoCore enables

Ordered by value. (Read-only doc; another agent owns forge::bin — these are *proposals*.)

1. **Lock the field-tag CRC as doubly-confirmed.** EgoCore's `CalculateFableCRC32` (seed 0, no
   invert) is an independent second source for our `forge::defdecode::fieldTag`. Add a one-line
   citation in `defdecode.hpp`'s header comment / FINDINGS "CANONICAL CRC REFERENCE": *"confirmed
   by EgoCore FableDefCompiler CPersistContext::CalculateFableCRC32."* Our evidence discipline wants
   ≥2 sources — this closes it.
2. **Reconcile the names.bin header doc.** Update `forge/bin.hpp` / DEFINITION_HEADER_INDEX to note
   the header is `[u32 magic][u32 randomID][u32 count][u32 streamLen]`, and that the value our
   reader reads at +4 (`0xA8E36C34`) is the **randomID**, matching game.bin's randomID field — not a
   second magic. Low-risk clarity fix; verify against a retail `names.bin` hexdump.
3. **Native tail-append is validated as link-safe — proceed.** EgoCore confirms cross-refs are
   global row indices with no reindex pass. Keep the append rule explicit in the writer's contract:
   *append only at tail; never insert/reorder existing rows; append sub-def groups contiguously and
   point the parent ref at the child's new row index.* This is exactly what `hero_overlay` /
   stat-item append already do; document it as the general rule for `forge::bin::addEntry`.
4. **If/when we want native `frontend.bin`/`script.bin` writing (multi-bin merge, task in HANDOFF
   §"Next best tasks 1"):** port `CDefinitionManager::SaveBinaryDefinitions` + `CompressBlock`
   (the 1-byte deflate loop) for **bit-exact** output, and the 8 frontend classes from `DefObjects.h`
   as their schema. This is the only place EgoCore has code we genuinely lack.
5. **Custom meshes (unblock target):** diff EgoCore `MeshCompiler` (`.c3d` LZO block layout: helpers/
   dummies/names/bones/transforms, version-1 volumes) against `BIG_MESH_GEOMETRY.md`/`MESH_COMPOSE.md`
   to fill any remaining write-side gaps; `GltfExporter`/`GltfMeshImporter` are a ready DCC bridge to
   compare with `MESH_EXPORT_BRIDGE`/the Blender addon.
6. **Cross-check bank/text/particle/shader writers** against our BIG_WRITER / text.big / EFFECTS work
   as independent oracles (each is a second implementation of a format we RE'd).
7. **Optional `.def` text authoring path:** EgoCore's `CPersistContext` MODE_LOAD_TEXT + symbol-map +
   2-pass template/specialises compiler is a complete reference if we ever add `.def` text ⇄ binary
   round-tripping to FableForge (currently byte-level only). Not urgent, but the design is proven.
8. **STB terrain baker + FSE bindings: EgoCore offers nothing** — do not spend time looking there.

---

## 5. Provenance / evidence notes

- EgoCore is a **second independent implementation**, so per our METHODOLOGY evidence rule it counts
  as one corroborating source — strongest where it agrees with our Ghidra-derived findings (field
  CRC, container framing). Where it *differs* (native game.bin authoring), the difference is
  informative: the community judged it not worth solving and used the engine, whereas **we already
  solved native game.bin read/write** — a real lead for FableForge over EgoCore.
- Key files quoted: `FableDefCompiler/DefinitionManager.cpp` (`SaveBinaryDefinitions`,
  `CompressBlock`, `CompileDefinition`), `FableDefCompiler/BinaryStreams.h` (`CDefStringTable`,
  `CPersistContext::CalculateFableCRC32`/`WriteTag`), `FableDefCompiler/DefCompiler.cpp`,
  `EgoCore/Definitions/CompilerBackend.h` (stealth ego_r.exe driver), `EgoCore/Definitions/DefExplorer.h:154`
  ("Bypasses ego_r.exe" tooltip — frontend only).
- Cross-referenced against: `docs/COMPILED_DEFINITION_INDEX.md`, `docs/DEF_SCHEMA_COVERAGE.md`,
  `docs/FINDINGS.md` (field-tag CRC + reward-index resolution), `docs/HANDOFF.md` (append proofs),
  and `libs/forgecore/include/forge/{bin,defdecode}.hpp`.
