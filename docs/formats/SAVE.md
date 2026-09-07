# Fable: The Lost Chapters — Save-game formats (`.sav` / `Profile.bin`)

**Status:** container CRACKED + write path VALIDATED + load-time signature verdict REACHED
(decompile-confirmed). Entity graph walked end-to-end on 6 real saves; hero stats, inventory and
quest text decoded and round-trip edited. **Merged 2026-09-07** from eight documents (all facts kept;
each source now stubs to a section here):

| former doc | original status / date | now in |
|---|---|---|
| `SAVEGAME_FORMAT.md` | CRACKED, 2026-07-19 (container/zlib/CRC/persist-stream grammar) | [Container and header layout](#container-and-header-layout), [Section grammar](#section-grammar) |
| `SAVE_FORMAT.md` | doc index | [Overview](#overview) |
| `SAVE_HEADER_MAP.md` | HEADER 23/23 + PLAYER 2/2 CONFIRMED, 2026-07-19 | [Section grammar](#section-grammar) §3.4–3.5 |
| `SAVE_HERO_STATS.md` | gold/morality/age/renown CONFIRMED + round-trip editor, 2026-07-19 | [Hero stats](#hero-stats) |
| `SAVE_ENTITY_GRAPH.md` | VALIDATED, 2026-07-19 second pass + 2026-07-20 §9.5 pass | [Entity graph](#entity-graph) |
| `SAVE_PROFILE_INDEX.md` | CRACKED on 4 profiles, install STAGED, 2026-07-20 | [Profile index](#profile-index) |
| `SAVE_SIG_ENFORCEMENT.md` | VERDICT REACHED, 2026-07-20 | [Signature enforcement](#signature-enforcement) |
| `SAVE_WRITER.md` | gates A–D PASS 5/5, signature SOLVED, 2026-07-19 | [Writer and tools](#writer-and-tools) |

**Citation convention:** `§n` inside a top-level section refers to that section's own numbered
subsections (the [Entity graph](#entity-graph) section keeps the original SAVE_ENTITY_GRAPH numbering,
so "SAVE_ENTITY_GRAPH.md §9.5.1" == [Entity graph](#entity-graph) §9.5.1). Cross-section references
are written as `[Section](#anchor) §n`.

---

## Overview

### 1.1 File set and where saves live

`%USERPROFILE%\Documents\My Games\Fable\Saves\<HeroName>\`
- `AutoSave`            — main autosave (binary `FableSave!`)
- `AutoSave.qs`         — quest-start save (binary `FableSave!`)
- `AutoSave.qs.hs`      — companion "hero state" blob to the quest save
- `Manual - Save1` …    — manual saves (binary `FableSave!`)
- `Profile.bin`         — per-hero profile/options + the manual-save registry (**different
  container**: `FablePro`, text persist mode — see [Profile index](#profile-index))

All observed `.sav` files are exactly **307200 bytes** (0x4B000). The writer preallocates this
(`_Cons_val(..., 0x4B000)` in `SaveGameStateInternal`): the file is a **fixed 300 KB slab**;
the real payload is the compressed prefix, the rest is zero slack. `<HeroName>` is the profile
name; `Default`/`Test`/`Cornelio`/`1` seen on disk. Retail path helpers:
`GetAutoSavePathName`, `GetAutoSaveFileName`, `GetAutoSaveCheckPointFileName`,
`GetAutoSaveDisplayName` (all reference the `"AutoSave"` string at retail `0x0122de9c`).

`AutoSave` / `AutoSave.qs` are reserved special slots and are NOT registered in `Profile.bin`;
only **manual** saves are (`SaveGameNames1..50`). A hand-copied `.sav` with no registry entry is
invisible in the in-game LOAD list ([Profile index](#profile-index) §3).

### 1.2 Scope: the runtime persist STREAM vs game.bin

The `.sav` body is the *runtime persist STREAM* written by `CPersistContext` in binary mode — one
half of the CRC finding, kept separate from the offline-compiled **game.bin** definition database
(see the "★ CANONICAL CRC REFERENCE" block in `FINDINGS.md`).

| | game.bin (def DB) | Save game (.sav runtime stream) |
|---|---|---|
| Producer | offline def compiler | `CWorld::SaveGameStateInternal` at runtime |
| Per-field tag hash | `CCRC::Calc(0, name, len)` (seed 0) | `CCRC::Calc(0, name, len)` (seed 0 — SAME hash; see §1.3) |
| Compression | (def-specific) | **zlib**, per-section chunks |
| Container | `[u32 count][elems]` | `[i32 count][elems]` (identical shape) |

Both use the SAME reflected CRC-32 with **seed 0**. The earlier "only the seed differs" framing was
itself the error — there is no seed difference.

### 1.3 ★ Canonical CRC rule — every on-disk hash is seed 0

**Every persist-stream field tag = `CCRC::Calc(0, fieldName, len)`** — reflected CRC-32, poly
`0xEDB88320`, seed 0, **no final inversion** (`tools/save_tools/savecrc.py getcrc(name, 0)`). The
same primitive, same seed, signs the `.sav` trailer, the `Profile.bin` trailer, game.bin def tags
and text.big symbols.

Proof (first four HEADER tags, ground-truth bytes vs. computed, 4 real saves):

| field | on-disk tag (LE) | Calc(seed=0) | Calc(seed=0xFFFFFFFF) |
|---|---|---|---|
| `WorldName`          | `22 4f de a8` = a8de4f22 | **a8de4f22 ✓** | b128a473 ✗ |
| `WorldFrame`         | `a0 ca 0f 5b` = 5b0fcaa0 | **5b0fcaa0 ✓** | 477a5d29 ✗ |
| `TeleportingEnabled` | `a7 e6 24 d3` = d324e6a7 | **d324e6a7 ✓** | 4bc0d615 ✗ |
| `SavingEnabled`      | `20 62 75 70` = 70756220 | **70756220 ✓** | 80fedb5d ✗ |

Also confirmed on THREE independent tags across multiple saves: `WorldName`=`a8de4f22`,
`Money`(gold)=`b03ccbfd`, `Morality`=`79a2d479`, each equal to `CCRC::Calc(0, name)`; the
seed-0xFFFFFFFF values do NOT appear on disk. All 23 HEADER field tags and both PLAYER tags in 4
real saves match `Calc(seed=0, name, len)`. `GetCRC` computes over the raw ASCII field name, no
NUL terminator.

The `0xffffffff` seen all over `CheckCRC`/`GetCRC` in the donor decompile is the **`CCharString`
constructor's length argument (`-1` = "call strlen")**, not a CRC seed — that is the source of the
original confusion. Net effect: **save tags and game.bin tags use the identical `Calc(0, name)`.**

Dated corrections (kept for provenance):
- 2026-07-19 (SAVEGAME_FORMAT first draft): claimed per-field tag = `CCharString::GetCRC()` =
  `CCRC::Calc(0xFFFFFFFF, …)`. EMPIRICALLY DISPROVEN the same day by real save bytes
  (WorldName/Money/Morality). Every "seed 0xFFFFFFFF" for a field tag is superseded — use seed 0.
- 2026-07-19 (SAVE_HEADER_MAP §0): 23/23 + 2/2 tags match seed 0; three-way seed table collapses
  (save = game.bin = seed 0).
- 2026-07-19 (SAVE_WRITER): field tags were wrongly noted there as seed 0xFFFFFFFF; corrected —
  all on-disk hashes (tags AND trailer) are seed 0.
- **Tooling gotcha:** `tools/save_edit.py:get_crc` defaults to seed 0xFFFFFFFF — for field/stat
  tags call it with seed 0 (or `savecrc.getcrc(name, 0)`); the trailer-signature helper
  `save_edit.sig_crc` / `ccrc_calc(data, 0)` (seed 0) is correct and unaffected.

### 1.4 Nesting recap (all layers, validated)

```
file ("FableSave!", fixed 307200 B)
 └─ chunk1 (zlib)  →  inflated persist stream
     └─ "SAVED_ENTITIES\0" [u32 sectionLen]
         └─ ~25 INDEPENDENTLY zlib-compressed CELLS (78 xx magic, self-framed)
             └─ each inflated cell = [u32 head] + run of top-level ENTITIES
                 └─ entity = header + base fields + N COMPONENTS
                     └─ component = [class\0][framing][u32 dataLen] + [tag][value] fields
```

### 1.5 Reading map (the former `SAVE_FORMAT.md` index)

| topic | section |
|---|---|
| container: magic, header dwords, zlib chunks, CRC primitive | [Container and header layout](#container-and-header-layout) |
| persist-stream `[tag][value]` grammar (field-tag seed **0**), HEADER + PLAYER tag→name map (world name, PlayerCharacterUID, …) | [Section grammar](#section-grammar) |
| hero CTCHeroStats field map (Money/Morality/Age/Fatness/RenownLevel; experience-pool tags) | [Hero stats](#hero-stats) |
| SAVED_ENTITIES entity graph: cell records (36-byte descriptor, §9.5.1), entity/component framing, inventory grammar (§9.5.2), QUESTS/FACTIONS/REGIONS, the productized editor (§9.5.3) | [Entity graph](#entity-graph) |
| `Profile.bin` registry / LOAD-list index | [Profile index](#profile-index) |
| load-time signature verdict: trailer CRC (seed 0, over `file[0:total_data_len]`), exact 0x4B000 size, zero padding — ENFORCED unconditionally in retail | [Signature enforcement](#signature-enforcement) |
| write-side recipe validation (gates A–D), tools | [Writer and tools](#writer-and-tools) |

Tooling: `tools/save_edit.py` (container read/verify/rebuild + hero report/set-gold/set-stat/
set-qty/add-item, all re-signed), `tools/parse_save_entities.py` (read-only graph walker +
quest-flip PoC), `tools/save_tools/savecrc.py` (CRC helpers), `tools/save_install.py`
(Profile.bin registry install), `tools/save_metadata.py` (frontend LOAD-list join).

**Key invariants for any writer:** re-sign trailer (seed-0 CRC), keep file exactly 307200 bytes,
zero slack; when editing a SAVED_ENTITIES cell also patch the cell descriptor
`recLen/clen/ulen` ([Entity graph](#entity-graph) §9.5.1 CORRECTION), the section length dword, and
`chunk1_ulen`.

### 1.6 Evidence base (≥2 sources per claim)

**Decompiled code (authoritative):**
- Donor **FableWin.exe** (carries PDB names), logs:
  - `ghidra_out/decomp_savegame_donor.log` — `CPersistContext::CheckCRC` (`0x018747b0`),
    `Transfer<long>` (`0x018951e0`/`0x01895220`), `Transfer<bool>` (`0x018951a0`),
    `Transfer<uint32>` (`0x01895cb0`), `Transfer<float>` (`0x01895cf0`),
    `Transfer<CCharString>` (`0x01895d30`/`0x0189c530`), `TransferVector` (`0x01894de0`),
    `CCharString::GetCRC` (`0x0186d2a0`), `CCRC::Calc` (`0x0186d2d0`), and the full CRC table.
  - `ghidra_out/decomp_persist_primitives.c` — `WriteULONG`, `WriteString`, `ReadULONG`,
    `GFSerialiseVectorBinaryOut` (the vector count encoding), `GetByteLength`.
- Retail **Fable.exe** (RVA base `0x400000`), logs:
  - `ghidra_out/decomp_savegame_retail.log` — string xrefs pinning the retail save subsystem.
  - `ghidra_out/decomp_savegame_retail2.log` — `CWorld::SaveGameStateInternal` (`0x0049f4c0`),
    `LoadEntitiesBinary` (`0x00521240`); HEADER block at lines 171–341.
  - `ghidra_out/decomp_zlibchunk.c` — `CDataOutputStream::WriteZlibCompressed` (`0x009967e0`).
  - `ghidra_out/decomp_save_header_pass{1..5}.log`, `ghidra_out/decomp_save_hero_entity.c`
    (PLAYER writer lines 441–567), `ghidra_out/decomp_save_header_body.c`.
  - `ghidra_out/decomp_hero_stats_classes.c` — `CTCHeroStats` accessors (runtime layout).
  - `ghidra_out/decomp_save_hero_stats.log` — `LoadEntitiesBinary`, `CThingManager` child-walk
    (`0x00530460`), Def transfers.
  - `ghidra_out/decomp_save_sig_load{1..4}.log` — `VerifySignature`, `LoadGameStateInternal`,
    callers, raw string scans (headless DecompFuncs / FindStrRaw.java, project FableTLC, 2026-07-20).

**Real save files (ground truth, parsed byte-exact):** `Test/AutoSave`, `Default/AutoSave`,
`Cornelio/AutoSave`, `Cornelio/Manual - Save1`, `Cornelio/AutoSave.qs`, plus profile `1`
(fresh hero, 2026-07-20) — six saves cross-validated; four `Profile.bin` (Cornelio / 1 / Default /
Test). Every `.sav` begins with the `FableSave!` magic, has the header dwords, and inflates to a
`HEADER\0` + `ENTITIES` persist stream whose first field tag matches the code path.

No community/wiki byte-level save spec exists (web search: only save-file *download* sites,
no format doc; fabletlcmod.com wiki cert-fails over TLS). So this spec is novel and rests on
**decompiled writer + real-file parse** as its two independent sources.

---

## Container and header layout

### 2.1 `FableSave!` on-disk layout (write-accurate, CONFIRMED 5/5)

```
0x00  "FableSave!"                10  magic (ASCII, no NUL)
0x0A  00 00                        2  pad
0x0C  u32 total_data_len          == trailer_pos (offset to trailer; = end of compressed data)
0x10  u32 chunk0_uncompressed_len == inflated HEADER-section size
0x14  u32 chunk1_uncompressed_len == inflated main/ENTITIES payload size
0x18  u32 chunk0_compressed_len   == byte length of first zlib blob
0x1C  chunk0 zlib stream (78 DA), chunk0_compressed_len bytes
      u32 chunk1_compressed_len   (inter-chunk framing dword)
      chunk1 zlib stream (78 DA)
@trailer_pos:
      u32 trailer_sig             = CCRC::Calc(0, file[0:trailer_pos])   <-- THE signature
      00 00 ... 00                zero padding to 0x4B000 (307200) bytes
```

Original 2026-07-19 header reading (same offsets; the `+0x0C` dword was then labelled
"signature/version [hypothesis]"):

```
off  size  field
0x00 10    magic         "FableSave!"           (ASCII, no NUL)
0x0A  2    pad           00 00
0x0C  4    u32  signature/version               [superseded: = total_data_len, see below]
0x10  4    u32  chunk0_uncompressed_len          (== inflated HEADER-section size)
0x14  4    u32  chunk1_uncompressed_len          (== inflated main/ENTITIES payload size)
0x18  4    u32  chunk0_compressed_len            (byte length of the first zlib blob)
0x1C ...   chunk0 zlib stream (starts 0x78 0xDA)
```

Validated across 3 saves:

| file | +0x0C sig | chunk0 ulen | chunk1 ulen | chunk0 clen |
|---|---|---|---|---|
| Test/AutoSave       | 0x000036A2 | 0xDB (219) | 0x54C6 (21702) | 0xC4 (196) |
| Cornelio/Manual-Save1 | 0x00013365 | 0xFF (255) | 0x161B0 (90544) | 0xED (237) |
| Cornelio/AutoSave.qs  | 0x000131F1 | 0x101 (257) | 0x16064 (90212) | 0xEC (236) |

The `+0x0C` dword differs per save and per playthrough length; it is not any of the sizes above.

- 2026-07-19 (SAVE_WRITER): **`+0x0C` is NOT a signature** — it is the **total compressed-data
  length** = the file offset at which the trailer begins. Hypothesis corrected. Evidence
  (`+0x0C == trailer_pos`, 5/5 saves):

| file | +0x0C | trailer_pos | equal |
|---|---|---|---|
| Cornelio/AutoSave     | 78321 | 78321 | yes |
| Cornelio/AutoSave.qs  | 78321 | 78321 | yes |
| Cornelio/Manual-Save1 | 78693 | 78693 | yes |
| Default/AutoSave      | 14084 | 14084 | yes |
| Test/AutoSave         | 13986 | 13986 | yes |

- 2026-07-19 (SAVE_WRITER): the signature is the **4-byte** trailer at `trailer_pos` (the first
  draft guessed "16 trailing bytes", e.g. `a6 17 6b b0 …` — that dword IS the trailer of
  Test/AutoSave, `b06b17a6` LE; the following 12 bytes are already zero slab padding).
- 2026-07-20 (SAVE_SIG_ENFORCEMENT): the loader reads `+0x0C` as `total_data_len`, bounds-checks
  `total_data_len + 4 <= fileSize`, and uses it as both CRC input length and trailer locator.

### 2.2 Chunk chain (zlib)

Each section is compressed independently by
`CDataOutputStream::WriteZlibCompressed` (retail `0x009967e0`), which drives
**`CCompressorZlib`** (`SetStreamInput` → loop `CompressStream` → `FinishCompressStream`)
and emits the raw zlib stream (RFC-1950, `78 DA` = deflate/best-compression). Framing:

```
chunk0:  [zlib blob, length = header.chunk0_compressed_len]       (@ file 0x1C)
         [u32 chunk1_compressed_len]                              (framing dword)
chunk1:  [zlib blob]                                              (the big payload)
         [u32 trailer_sig]  <- seed-0 CRC over file[0:trailer_pos] (CONFIRMED)
         [zero padding to 300 KB]
```

Chunk0's uncompressed length lives in the file header (+0x10); chunk1's uncompressed length
is in the header too (+0x14); chunk1's *compressed* length is the framing dword written right
after chunk0. Inflating both and concatenating gives the full `CPersistContext` binary stream.
(A save may have >2 sections; `SaveGameStateInternal` calls `StartSection`/`EndSection`
several times — HEADER, ENTITIES, plus sub-sections — but on disk they are grouped into these
zlib chunks.)

### 2.3 Trailer signature (CONFIRMED 5/5 — gate D)

`trailer = ccrc_calc(seed=0, data = bytes[0 .. trailer_pos))` where the input is the literal file
prefix: `"FableSave!" + pad + 16-byte header + chunk0 zlib + u32 chunk1_clen + chunk1 zlib`.
No final XOR/inversion (matches `CCRC::Calc`, §2.5). Equivalent to `zlib.crc32` **only** if you
pre/post-invert; the game uses the raw seed-0 table form, which is what `ccrc_calc(data, 0)` in
`save_edit.py` implements and what matched.

    file                    stored     computed
    Cornelio/AutoSave       a3ba1c9c = a3ba1c9c
    Cornelio/AutoSave.qs    a3ba1c9c = a3ba1c9c
    Cornelio/Manual-Save1   0ddf1f62 = 0ddf1f62
    Default/AutoSave        36dfc709 = 36dfc709
    Test/AutoSave           b06b17a6 = b06b17a6

The loader recomputes exactly this ([Signature enforcement](#signature-enforcement) §1c).

### 2.4 zlib reproduction note (does our deflate reproduce original compressed bytes?)

**No — and it does not need to.** Python's `zlib.compress(data, 9)` emits a valid `78 DA` stream that
inflates to the identical payload, but its Huffman/match choices differ slightly from the compressor
the game shipped (`CCompressorZlib`), so the compressed bytes — and therefore whole-file bytes —
differ:

| file | whole-file identical | orig compressed | ours | delta |
|---|---|---|---|---|
| Cornelio/AutoSave     | No | 78289 | 78327 | +38 |
| Cornelio/AutoSave.qs  | No | 78289 | 78327 | +38 |
| Cornelio/Manual-Save1 | No | 78661 | 78693 | +32 |
| Default/AutoSave      | No | 14052 | 14058 |  +6 |
| Test/AutoSave         | No | 13954 | 13967 | +13 |

Delta is tiny (+6…+38 bytes on 14–78 KB). This is fine because:
- The header length dwords are **recomputed** from our actual compressed sizes.
- The `+0x0C` total_data_len and the trailer signature are **recomputed** over our actual bytes.
- The file stays far under the 300 KB slab.
The game reads lengths from the header and inflates — it never requires byte-identical compressed
output. Whole-file byte-identity is therefore **not a requirement** and is **not achievable** with
stock zlib; the pipeline is correct without it.

### 2.5 CRC primitive — exact table

**`CCRC::Calc(seed, data, len)`** — donor `0x0186D2D0`. Full decompiled body:

```c
uint Calc(uint seed, const void *data, uint len) {
    uint crc = seed;
    for (uint i = 0; i < len; ++i)
        crc = (crc >> 8) ^ table[ (crc ^ ((byte*)data)[i]) & 0xFF ];
    return crc;                       // NO final inversion
}
```

- **Reflected CRC-32, polynomial `0xEDB88320`** (the standard zlib/PKZIP table).
- **Seed is caller-supplied**; there is **no final XOR/inversion**.
- Table lives at donor VA **`0x0449BE30`** (256 × u32 = 1024 bytes). Full dump:
  `ghidra_out/crc_table_full.txt`. First entries `00000000 77073096 EE0E612C 990951BA …`,
  last entry `2D02EF8D` — byte-identical to the canonical zlib CRC-32 table.

**`CCharString::GetCRC()`** — donor `0x0186D2A0` — decompiles as
`CCRC::Calc(0xFFFFFFFF, GetAsCharArray(), GetByteLength())`, but the `0xFFFFFFFF` is the
`CCharString(name, -1)` strlen argument, not the seed (§1.3). `GetByteLength` == string length
**excluding** the NUL, so the hash is over the raw ASCII name as-is (no terminator). On-disk tags
are seed 0 regardless.

**Seed table (canonical reference is FINDINGS.md "★ CANONICAL CRC REFERENCE"):**

| caller | seed | used by |
|---|---|---|
| all on-disk name→tag hashes | `0x00000000` | save-stream field tags + game.bin def tags + text.big symbols + save trailer + Profile.bin trailer = ONE seed-0 hash |
| `CCharString::GetCRC()` | `0x00000000` (the decompile's `0xFFFFFFFF` is the ctor length arg `-1`) | field tags |
| `NGameText::CDataBank::GetTextBySymbol` | `0x00000000` | text.big / BIGB symbol→ID |

**Bonus — `FUN_009cc410` (retail, text.big CRC→entry-index map):** decompiled — it is
`CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<…>>::LowerBound` at
`this+0xD0`, i.e. a **binary search of a sorted (u32-key → CDefString) vector**. It takes a
u32 key (a seed-0 CRC of a text symbol, per the canonical reference), binary-searches the
sorted key/value pairs, and returns the paired entry index (`puVar1[1]`) or 0 on miss. This
closes the text.big symbol-resolution detail: `GetTextBySymbol` computes the seed-0 CRC of the
symbol name, then this map turns that CRC into the string-bank entry index. (Full decompile in
`ghidra_out/decomp_savegame_retail.log`.)

### 2.6 Save-system config flags

Config console vars (registered in `NGlobalConsole::Initialise` / `GFRunInitScripts`):
`UseBinarySaveGames` (donor `0x0402F208`) selects the binary format over the text format,
`UseSaveGameSignatures` (donor `0x0402F21C`), `UseRetailSaveGameSystem` (donor `0x0402F2F0`,
retail `0x0122E85C`), plus commands `SaveGameState`/`LoadGameState`. Retail ships with binary +
retail system ON.

- 2026-07-20: **`UseSaveGameSignatures` and `UseBinarySaveGames` do NOT exist in retail
  `Fable.exe`** (raw image scan, 0 hits each; `UseRetailSaveGameSystem` present at `0x0122E85C`
  as scan sanity check). They are donor/debug-build-only console vars. There is **no signature
  off-switch** in retail ([Signature enforcement](#signature-enforcement) §5).

### 2.7 `Profile.bin` container (`FablePro`) — summary

`Profile.bin` uses a **different magic** and the **text** persist mode, not the binary one:

```
off  data
0x00 "FablePro"                         (8-byte magic; string at donor 0x0401BD3C etc.)
0x08 00 00 00 00
0x0C u32                                total_data_len (e.g. 0x000046A4) = trailer offset
0x10 ...  key/value text lines, e.g.:
     ProfileName "Cornelio";\r\n
     SubtitlingEnabled TRUE;\r\n
     VibrationEnabled TRUE;\r\n
     CameraXYInverted FALSE;\r\n
     CameraYZInverted FALSE;\r\n
     ...
EOF-4 u32 trailer = CCRC::Calc(0, file[0:total_data_len])
```

This is `CPersistContext` in **text mode** (mode 0/1): `TransferObjectSaveText` /
`TransferObjectLoadText`, which write `Name value;` lines and — per `CheckCRC` — emit **no CRC
tags** (text modes return early from `CheckCRC`). Human-readable and trivially editable (but the
trailer must be re-signed). Good cross-check that the CRC-tagged stream is exclusive to the
*binary* modes. Full layout, registry grammar and tool: [Profile index](#profile-index).

### 2.8 Loader / writer entry points (addresses)

| role | symbol | donor (FableWin) | retail (Fable.exe) |
|---|---|---|---|
| Save writer (top level) | `CWorld::SaveGameStateInternal(CWideString,bool)` | — | **`0x0049F4C0`** (HEADER write @0x0049F5A2, ENTITIES @0x0049FAA5) |
| Save loader (top level) | `CWorld::LoadGameStateInternal(CWideString*,bool)` | — | **`0x004A21F0`** (calls `VerifySignature` first, @0x004a220f) |
| Signature verifier | `CUserProfileManager::VerifySignature(CWideString const&)` | — | **`0x00409730`** |
| Profile loader | `CUserProfileManager::Load` | — | `0x0040D350` (calls VerifySignature @0x0040d38b) |
| Cached-validity helpers | `FUN_005957d9` / `FUN_0047ed0a` | — | `0x005957D9` / `0x0047ED0A` (VerifySignature @0x005957fd / @0x0047ed2e) |
| Entity loader (binary) | `LoadEntitiesBinary` | — | **`0x00521240`** (uses "Loading entities from script", `0x00521488`) |
| Entity loaders (text/script) | `LoadEntitiesFromScript` | — | `0x00520F44` / `0x00521106` |
| SAVED_ENTITIES writer | `CThingManager::SaveGameState` | — | `0x00530490` (child-walk `0x00530460`) |
| PLAYER writer | `CPlayerManager::SaveGameState` | `0x01805EF8` | `0x00445A20` |
| QUESTS writer (text) | `CQuestManager::SaveGameState` | `0x01819106` | — |
| Persist CRC gate | `CPersistContext::CheckCRC` | `0x018747B0` | (call-resolved; same logic) |
| zlib chunk writer | `CDataOutputStream::WriteZlibCompressed` | — | **`0x009967E0`** (drives `CCompressorZlib`) |
| Transfer<long> | `…Transfer<J>` | `0x018951E0` / `0x01895220` | — |
| Transfer<bool> | `…Transfer<_N>` | `0x018951A0` | — |
| Transfer<uint32> | `…Transfer<K>` | `0x01895CB0` | — |
| Transfer<float> | `…Transfer<M>` | `0x01895CF0` | — |
| Transfer<CCharString> | `…Transfer<VCCharString>` | `0x01895D30` / `0x0189C530` | — |
| TransferVector | `…TransferVector<…>` | `0x01894DE0` | — |
| CCharString::GetCRC | `?GetCRC@CCharString@@QBEKXZ` | `0x0186D2A0` | — |
| CCRC::Calc | `?Calc@CCRC@@SIKKPBXK@Z` | `0x0186D2D0` | — |
| CRC table | 256 × u32 | `0x0449BE30` | — |
| Save-path helpers | `GetAutoSave*PathName/FileName` | — | `0x00406695`, `0x00406695+`, `0x00406FC5`, `0x004066F2`, `0x00408*` |
| Def transfers (game.bin, NOT live stats) | `CGoldDef::Transfer` / `CExperienceDef::Transfer` / `CHeroExperienceDef::Transfer` / `CHeroStatsDef::Transfer` / `CCreatureStatsDef::Transfer` | — | `0x004F6DBD` / `0x00506A30` / `0x0058A900` / `0x004873BF` / `0x00457202` |
| `CTCHeroStats` UI controller (GuiVarTransferStruct, not persist) | — | — | `0x0056F241` |
| `CTCHeroStats::GetHeroStatExperience`/`AddHeroStatExperience` | — | — | retail `0x0057a…` |

---

## Section grammar

### 3.1 Sections

A section is opened with `CPersistContext::StartSection(writeSection, CCharString name)` and
closed with `EndSection`. In the binary stream a section is introduced by its **name written
as a raw string** followed by a **u32 section size**, then the tagged field stream:

```
[section-name ASCII, NUL-terminated][u32 sectionByteLen][field]...
```

Observed, byte-exact, at the very start of chunk0 in every save:
```
48 45 41 44 45 52 00        "HEADER\0"
D0 00 00 00                 section length = 0xD0 (208)     (0xF4 in the larger saves)
22 4F DE A8                 first field CRC tag  (a8de4f22 LE)
46 69 6E 61 6C 41 6C 62 69 6F 6E 00   "FinalAlbion\0"   <- the world/level name (string value)
...                         (more [tag][value] fields)
```
Chunk1 begins the main body; its first bytes carry the `ENTITIES` / `SAVED_ENTITIES` sections
(`SaveGameStateInternal` writes the `"ENTITIES"` literal, string at retail `0x01238e60`,
xref from `0x0049faa5`; `"SAVED_ENTITIES"` at `0x01245d18`). The entity loader is
`LoadEntitiesBinary` (retail `0x00521240`).

Known section / marker names (from retail string table): `HEADER`, `ENTITIES`,
`SAVED_ENTITIES`, and the text-mode markers `START_SAVED_ENTITIES` / `END_SAVED_ENTITIES`.

**Section order in chunk1** — `CWorld::SaveGameStateInternal` opens sections in this order:
`ENTITIES` → `SAVED_ENTITIES` (via `CThingManager::SaveGameState`) → then, when normal save:
`PLAYER` → `QUESTS` → `REGIONS` → `FACTIONS` → text markers.

Only `HEADER` is a clean top-level named section; the remainder is the `ENTITIES`/`SAVED_ENTITIES`
nested entity stream. The generic section walker in `save_edit.py` recognises `HEADER` and treats
the rest as an opaque residual tail — identity still holds (gate A), and field edits reach into it
via tag scan.

### 3.2 Per-field encoding: `[CRC tag][value]`

Every `CPersistContext::Transfer<T>(name, value)` call first invokes
`CheckCRC(name)` (donor `0x018747b0`), then serialises the value. `CheckCRC`:

```c
// mode = *(ctx+0x18):  0=text-in 1=text-out 2=binary-in 3=binary-out 4=copy
if (mode == 3) {                                   // binary OUT (save)
    ULONG tag = CCharString(name).GetCRC();        // seed-0 reflected CRC-32 (the decompile's
    stream.WriteULONG(tag);                         //   0xFFFFFFFF is the ctor's strlen arg)
} else if (mode == 2) {                            // binary IN (load)
    ULONG got = stream.ReadULONG();
    assert( got == CCharString(name).GetCRC() );    // "stream_crc==CCharString(name).GetCRC()"
}
// text modes 0/1 do NOTHING here (no CRC in text saves).
```

So each field in a binary save is:

```
[u32  Calc(0, fieldName), little-endian]   [value bytes]
```

The assert literal in the binary is exactly `stream_crc==CCharString(name).GetCRC()`
(source `…\bbblibrary\lib_persist_context.hpp:0xD7`).

### 3.3 Value encodings by type

From the `Transfer<T>` binary branches (`case 2` = in, `case 3` = out) which delegate to
`_CPersistTraits<T>::TransferBinaryIn/Out`, plus the stream primitives:

| type (Transfer<T>) | binary value bytes |
|---|---|
| `long` / `signed char`→padded / `uint32` (`K`) | 4-byte LE (WriteULONG/ReadULONG → 4 bytes) |
| `bool` (`_N`) | 1 byte (traits write the raw byte) |
| `float` (`M`) | 4-byte IEEE-754 LE |
| `CCharString` | **raw chars, NUL-terminated, no length prefix** — proven by the on-disk `"FinalAlbion\0"` value and by `WriteString` (writes `GetLength()` chars via the stream). Empty string = `00`. |
| `Vector<T>` (`TransferVector` / `GFSerialiseVectorBinaryOut`) | `[i32 count]` then `count ×` `TransferBinaryOut<T>(element)` (no per-element tag). `count` written by `WriteSLONG` (signed 4-byte). |
| `CIntelligentPointer<CThing>` (object refs — heavily used in HEADER) | resolves to the object's NUL-terminated name string on disk (see §3.4 notes) |
| `CWideString` (e.g. `MostRecentManualSaveName`) | UTF-16LE cstr, NUL-terminated |
| `CFloatRange` position | 3× float LE (12 B) |
| `u64` (e.g. `PlayerCharacterUID`, Transfer<_K>) | 8 B LE |

This mirrors the game.bin value table exactly (same widths, same NUL-terminated strings, same
`[count][elems]` vectors) — the only difference is the **zlib wrapping**. Value width is
**per-field (type-driven), NOT self-describing** — a `Money` field is a 4-byte int, a bool field
is 1 byte, `Age` is a 4-byte float.

Field/serialisation ORDER == the `Transfer()` call order inside `SaveGameStateInternal`
(and each section's writer), not any offset order.

### 3.4 HEADER section — full tag → name → type map (23/23 CONFIRMED)

Sources: writer `CWorld::SaveGameStateInternal` retail `0x0049F4C0`, HEADER block decompiled in
`ghidra_out/decomp_savegame_retail2.log` lines 171–341 (field order = `Transfer<T>(ctx,"Name",…)`
call order; `T` = on-disk value type); ground truth = 4 inflated real saves — every field's
`Calc(0,name)` tag is present at the expected byte offset in call order in **all four**, and the
typed decoder consumes the HEADER section byte-exactly (parsed_to == section end, 0 bytes remaining)
in all four. Tools: `tools/save_tools/savecrc.py` (GetCRC + inflater),
`tools/save_tools/decode_header.py` (typed HEADER decoder).

Section framing on disk: `"HEADER\0" [u32 sectionByteLen] [field]...`, each field `[u32 tag][value]`.
Offsets below are into the **inflated** HEADER section of `Cornelio/Manual - Save1` (chunk0), for
orientation; every tag is confirmed in all 4 saves.

| # | tag (hex) | field name | Transfer\<T\> | on-disk type | infl.off | notes |
|--:|---|---|---|---|---|---|
| 0 | `a8de4f22` | **WorldName** | IntelPtr\<CThing\> | ASCII cstr, NUL-term | +0x00b | world/level name → `"FinalAlbion"` |
| 1 | `5b0fcaa0` | **WorldFrame** | signed_char→long | u32 LE | +0x01b | frame/tick counter (740 / 195 / 138230 / 139834) |
| 2 | `d324e6a7` | TeleportingEnabled | bool | 1 byte | +0x023 | |
| 3 | `70756220` | SavingEnabled | bool | 1 byte | +0x028 | |
| 4 | `6a28abd2` | ExperienceSpendingEnabled | bool | 1 byte | +0x02d | |
| 5 | `3f6e5950` | CreatureGenerationEnabled | bool | 1 byte | +0x032 | |
| 6 | `5899107a` | CreatureGenerationDisabledGroups | long | u32 LE | +0x037 | bitmask (0 in all samples) |
| 7 | `452dddb2` | HeroSleepingEnabled | bool | 1 byte | +0x03f | 0 early / 1 later saves |
| 8 | `2409e2a4` | MapTableShowQuestCardsOnUsed | bool | 1 byte | +0x044 | |
| 9 | `1bfb95b6` | MiniMapEnabled | bool | 1 byte | +0x049 | |
| 10 | `4c7c1d48` | MiniMapActiveBeforeDisabled | bool | 1 byte | +0x04e | |
| 11 | `ad260f50` | GuildMasterMessagesEnabled | bool | 1 byte | +0x053 | |
| 12 | `82818b2c` | SummonerDeathExplosionAffectsHero | bool | 1 byte | +0x058 | |
| 13 | `3831a50f` | MostRecentSaveType | signed_char→long | u32 LE | +0x05d | enum ESaveType (1/3 seen) |
| 14 | `6c9212bd` | MostRecentSaveTypeBeforeManualSave | signed_char→long | u32 LE | +0x065 | enum (2/3 seen) |
| 15 | `9a72a8db` | MostRecentManualSaveName | IntelPtr\<CThing\> (CWideString) | UTF-16LE cstr, NUL-term | +0x06d | `"Manual - Save1"`; empty in autosaves |
| 16 | `80926404` | SaveGameMarkerPos | CFloatRange | 3× float LE (12 B) | +0x08f | world XYZ of save marker |
| 17 | `aedf95f2` | SaveGameMarkerAngleXY | signed_char→float | float LE (4 B) | +0x09f | facing angle |
| 18 | `34b73c37` | GuildSealRecallPos | CFloatRange | 3× float LE (12 B) | +0x0a7 | 0,0,0 in samples |
| 19 | `0a111ac7` | GuildSealRecallAngleXY | signed_char→float | float LE (4 B) | +0x0b7 | |
| 20 | `fd9cf30d` | CurrentRegionName | IntelPtr\<CThing\> | ASCII cstr | +0x0bf | `"TXT_REGION_GREATWOOD_..."` |
| 21 | `eb6d5050` | CurrentRegionMinimapGraphicName | IntelPtr\<CThing\> | ASCII cstr | +0x0e1 | `"MINIMAP_GREATWOOD..."` |
| 22 | `45242440` | TotalTimePlayed | (double→) | 4 B LE | +0x0f7 | seconds played; last HEADER field |

Notes:
- Fields 2–22 are only written when `this[0x102] == 0` (i.e. not a "reload minimal" context) — that
  branch is taken by all normal player saves, so all appear in real saves.
- `IntelPtr<CThing>` names in the writer resolve to a plain NUL-terminated string on disk (the
  object's name), exactly as §3.3 states.
- `TotalTimePlayed` is passed from a `double` in the writer but occupies 4 bytes here; the section
  consumes exactly, so treat it as a 4-byte trailing value (float 0.0 in the 4 samples; the true play
  time may be tracked elsewhere / written as the low dword).

**Decoded sample values (proof the map is usable):**
`Cornelio/Manual - Save1`: WorldName=`FinalAlbion`, WorldFrame=138230, MostRecentManualSaveName=
`Manual - Save1`, SaveGameMarkerPos=(3114.79, 3000.x, z), CurrentRegionName=
`TXT_REGION_GREATWOOD_TELEPORT`, CurrentRegionMinimapGraphicName=`MINIMAP_GREATWOOD`.
`Test/AutoSave`: WorldName=`FinalAlbion`, WorldFrame=740, CurrentRegionName=`TXT_REGION_OAKVALE`,
save name empty. Run `python tools/save_tools/decode_header.py` for the full aligned table.

### 3.5 PLAYER section — 2 fields CONFIRMED (chunk1 body)

`CPlayerManager::SaveGameState` (donor `0x01805EF8` / retail `0x00445A20`,
`ghidra_out/decomp_save_hero_entity.c` lines 441–567) writes **only two tagged fields**:

| tag (hex) | field name | Transfer\<T\> | type | confirmed at body offset |
|---|---|---|---|---|
| `96adc827` | **PlayerCharacterUID** | Transfer\<_K\> | u64 (8 B LE) | Test @0x29ee, Manual @0x12b27, AutoQs @0x129d7 |
| `9a7e84f7` | **PlayerRegionName** | Transfer\<CCharString\> | ASCII cstr | immediately after (e.g. `"Greatwood"`) |

`PlayerCharacterUID` is the **UID of the hero CThing** — the pointer to where the hero's live
gold/morality/experience actually live ([Hero stats](#hero-stats)). Stored LE on disk as
`27 c8 ad 96`. Sample UIDs: `0xffffff0000003ae3` (Cornelio) / `0xffffff00000000c2` (Test/Default) —
the low bytes + a `0xffffff` high-word marker; it is written verbatim in the entity header. Both
fields confirmed present in all 3 parsed body streams.

### 3.6 SAVED_ENTITIES section — body and the 36-byte cell descriptor

Inside the inflated chunk1 stream:
```
"SAVED_ENTITIES\0"
[u32 sectionLen]              <- = se_payload_len: compressed byte length of all cells (header→PLAYER). PATCH on edit.
[u32 0][u32 N?][u32 0]        <- section head
cell records …                <- N cells (25 in Cornelio/Manual-Save1); each self-framed zlib (78 xx)
                                 so the loader can also walk them by magic
[trailing index table]
```

Each cell record (CONFIRMED, 81/81 cells across 6 saves):

```
[u64 0]                     marker
[u64 recLen = 29 + clen]    record length (covers cellId..data + 9 trailing zero bytes)
[u64 cellId]                stable region/cell identifier (e.g. hero cell 28885a in Cornelio,
                            3020df in fresh saves; ids identical across saves of one layout)
[u64 clen]                  compressed length of the zlib cell
[u32 ulen]                  uncompressed (inflated) length
[clen bytes]                the zlib stream (78 xx)
[9 zero bytes]
```

The 36-byte descriptor = `[u64 0][u64 recLen=29+clen][u64 cellId][u64 clen][u32 ulen]`. Empty
(unloaded-region) slots appear as all-zero runs between records; after the last record the section
carries a trailing index table of `[u32 X][u32 cellId][u32 0][u32 hash?][u32 0]` entries mapping
small ids (entity/things index?) to cellId — it references **cellId, not byte offsets**, so it needs
no patching on edits. [index-table semantics = hypothesis; record framing = CONFIRMED by 81/81
invariant checks `marker==0 && recLen==29+clen && clen==zlib consumed && ulen==inflated && 9-byte
zero trailer`.]

**Edit rule (★ 2026-07-20 CORRECTION):** any SAVED_ENTITIES cell edit must patch the cell
descriptor (`recLen=29+clen` / `clen` / `ulen` when the inflated size changes) AND `sectionLen`
AND `chunk1_ulen` (SAVED_ENTITIES lives in chunk1's *uncompressed* stream, so its length tracks
the spliced compressed bytes; chunk0 is unchanged), then re-sign — patching only the section
length + chunk1_ulen mis-frames the engine's record walk for every cell after the edited one.
`save_edit.replace_cell()` does all three.

- 2026-07-19 (superseded observation): the section head was described as a "small header of
  per-cell metadata (u64-ish fields; e.g. the first cell's uncompressed size 0x37e=894 and
  compressed size 0x215=533 appear here)" — those are the first cell's `ulen`/`clen` descriptor
  fields, fully decoded 2026-07-20 as above.

Full graph walk (cells, entities, components, inventory, hero cell): [Entity graph](#entity-graph).

### 3.7 QUESTS / FACTIONS / REGIONS are TEXT, not tagged binary

The QUESTS section (`CQuestManager::SaveGameState` text writer, donor `0x01819106`) emits literal
markers `START_QUEST_CARDS` / `END_QUEST_CARDS` / `START_ACTIVE_QUESTS` / `START_SAVED_QUESTS` and
quest names (`Q_HobbeCave`, `Q_TraderEscort`, `CS_OakValeRevisited`, `COMPLETED`, `SCRIPT_DATA`…)
as plain text — confirmed in the body-stream string dump (offsets 0x12b41+). FACTIONS is likewise
text (`Name "FACTION_HERO";` …). So quest completion state is editable as **text sub-scripts**,
not via a CRC tag. REGIONS is a plain list of region names (LookoutPoint, BowerstoneSlums,
StartOakVale, …). The text is Lionhead-LZO1X compressed inside the binary stream — blob framing,
decoded structure and the flip recipe: [Entity graph](#entity-graph) §7 and §8.1.

---

## Entity graph

**Status:** VALIDATED against real saves — graph walked end-to-end, real inventory + quest
state decoded, and the quest write path (LZO re-encode + container re-sign) EXECUTED and
re-verified (§8.1). **Date:** 2026-07-19 (second pass); §9.5 added 2026-07-20.
**Tool:** `tools/parse_save_entities.py` (read-only, pure stdlib + `tools/lionhead_lz.py`).
Primary validation save: `Cornelio/Manual - Save1` (played hero). Cross-checked against
`Test/AutoSave` and `Default/AutoSave` (fresh new-game heroes) and `Cornelio/AutoSave.qs`
(same playthrough) — see §9; profile `1` added in §9.5.

### 1. Nesting recap (all layers, validated)

See [Overview](#overview) §1.4 for the layer diagram. Cornelio/Manual-Save1: **25 cells, 383,284
bytes total inflated** (matches [Hero stats](#hero-stats) §4 byte-exact). Cell sizes range 396 B
(cell 23) to 134,587 B (cell 24); the hero lives in **cell 10** (43,920 B inflated — the cell
containing `CTCHeroStats\0`).

Per-cell table (Cornelio/Manual-Save1, `parse_save_entities.py … cells`):

| cell | comp_off | comp_len | inflated | first-u32 |
|---|---|---|---|---|
| 0 | 124 | 533 | 894 | 1 |
| 1 | 694 | 3001 | 20889 | 2 |
| 2 | 3732 | 2417 | 7034 | 2 |
| 3 | 6186 | 1621 | 5203 | 1 |
| 4 | 7844 | 2012 | 5300 | 2 |
| 5 | 9893 | 501 | 688 | 1 |
| 6 | 10431 | 2144 | 7811 | 2 |
| 7 | 12612 | 2129 | 8121 | 1 |
| 8 | 14778 | 1504 | 5591 | 2 |
| 9 | 16319 | 657 | 1680 | 1 |
| **10** | 17013 | 13056 | **43920 (hero)** | 2 |
| 11 | 30106 | 2243 | 5450 | 1 |
| 12 | 32386 | 1096 | 2450 | 1 |
| 13 | 33807 | 515 | 840 | 1 |
| 14 | 34415 | 784 | 2264 | 1 |
| 15 | 35342 | 653 | 1097 | 1 |
| 16 | 36064 | 9734 | 92280 | 2 |
| 17 | 45939 | 1675 | 3026 | 2 |
| 18 | 47659 | 2592 | 6360 | 1 |
| 19 | 50325 | 412 | 636 | 1 |
| 20 | 50774 | 407 | 653 | 1 |
| 21 | 52209 | 4529 | 24729 | 1 |
| 22 | 56775 | 522 | 1385 | 1 |
| 23 | 58363 | 302 | 396 | 1 |
| 24 | 58766 | 15475 | 134587 | 4 |

(`comp_off` is relative to the SAVED_ENTITIES data start; the small u32 at cell start is
recorded as-is — it is 1/2/4 on these cells and is NOT proven to be the full top-level entity
count, since the name-anchored walker recovers more entities per cell than it. Treat as
[hypothesis: sub-group count]. The walker does not depend on it.)

### 2. Entity header framing (CONFIRMED on real bytes)

```
[inst-name ASCII \0]                 e.g. "PlayerCreature", "Object", "Thing"
[u32 A][u32 B]                       spatial/cell fields
[EngineClass ASCII \0]               Object | Marker | Village | AICreature | Building | …
[u32 flagsA][u32 mid][u32 flagsB]    mid = 4 (fresh) or 0xffffffff (no-parent marker)
[u64 UID]                            e.g. ffffff0000003ae3 (hero) — matches PLAYER section's
                                     PlayerCharacterUID (tag 96adc827) verbatim
[u32 z = 0]
[DEF-name UPPER_SNAKE \0]            game.bin thing type, e.g. CREATURE_HERO, OBJECT_BARREL_BREAKABLE
[u32 baseFieldLen] baseFieldLen bytes of [tag][value] base fields
                                     hero base fields DECODED: tag 78b48786 =
                                     getcrc("Health",0) → f32 80.0, tag eba5fd9d =
                                     getcrc("Scale",0) → f32 1.0   (both CONFIRMED by CRC
                                     name-match + sane values)
[u32 0][u32 componentCount][u32 0]
component × componentCount
```

Earlier (2026-07-19, SAVE_HERO_STATS) positional summary of the same framing:
`[instance-name ASCII\0][pad][u64 UID][pad][DEF-name ASCII\0][u32 componentBlockLen][components…]`.

The walker (`entity_headers`) anchors on `\0<EngineClass>\0` word boundaries and demands
`mid ∈ {4, 0xffffffff}`, `z == 0`, and an UPPER_SNAKE DEF-name after the 24-byte middle
block — this rejects the `Creature`-inside-`AICreature` substring false positives and nested
child records.

**Full enumeration** (`entity_records`) is class-independent — the hero's own header carries
NO engine-class token, and child entities nest below top level — so the complete walk anchors
on `[u64 UID (hi dword nonzero)][u32 0][DEF-name with known prefix \0][u32 baseFieldLen]`
(DEF prefixes: `OBJECT_ CREATURE_ MARKER_ BUILDING_ VILLAGE_ DUMMY_ THING_ HOLDING_`).
**Cross-check:** every entity record is closed by an `END\0` marker; on Cornelio/Manual-Save1
the anchor walk finds **348 records** and the cells contain **exactly 348 `END\0` markers** —
the enumeration is complete and exact.

Component framing (CONFIRMED, all cells):

```
[class-name ASCII \0]     CTC* (147 distinct classes observed)
[u32 pad = 0][u8 0]
[u32 dataLen]
dataLen bytes             [u32 seed-0 CRC(fieldName)][value] … (value width type-driven,
                          NOT self-describing: int/float = 4 B, bool = 1 B,
                          CCharString = raw chars + NUL, vector = [i32 count][elems])
[u32 sep = 0]             inter-component separator
```

### 3. Entity census (Cornelio/Manual-Save1)

**348 entity records total** (`entity_records`, END-marker cross-check 348/348): 267 are
classed top-level headers (`entities` command); the rest are children + the class-token-less
hero header. Classed top-level breakdown:

| EngineClass | count |
|---|---|
| Object | 153 |
| AICreature | 48 |
| Building | 32 |
| Marker | 19 |
| Village | 15 |

**108 distinct DEF-names** across all records (89 among classed top-levels alone,
`defcensus`). Top of the list: `OBJECT_BARREL_BREAKABLE` ×38,
`OBJECT_BOOKSHELF_01` ×16, `OBJECT_CUPBOARD_MEDIUM` ×16, `OBJECT_GUILD_BED_APPRENTICE_01` ×12,
`MARKER_FISHING_SPOT` ×10, `OBJECT_CHEST_OPENABLE` ×8, `CREATURE_GUARD` ×7,
`CREATURE_GUILD_APPRENTICE_MALE` ×7, `MARKER_DIGGING_SPOT` ×6,
`VILLAGE_WANDERING_POPULATION_{GOOD,EVIL}_GREATWOOD` ×3 each, `OBJECT_SILVERKEY_CHEST_5` ×2 —
i.e. exactly the mutable world state you'd expect a save to persist (breakables, containers,
NPCs, village populations, quest markers), consistent with the region/quest layout of an
early-game playthrough (Guild + Bowerstone + Greatwood cells).

### 4. Component-class census (147 distinct CTC* classes)

`census` command, all cells, Cornelio/Manual-Save1 (top 30 shown; full list reproducible):

```
CTCMapwho 348           CTCSoundPlayer 298      CTCGraphicAppearance 279
CTCHitLocations 264     CTCPhysicsStandard 229  CTCTargeted 217
CTCEntityEvents 191     CTCVillageMember 185    CTCGraphicAppearanceStaticMesh 171
CTCContainerRewardHero 120  CTCAnimationComplex 93  CTCVolumeContainmentTracker 84
CTCEnemy 80             CTCCreatureOpinionOfHero 76  CTCCreatureNavigation 69
CTCTalk 69              CTCScriptedControl 69   CTCCreatureModeManager 69
CTCEmoteIcon 69         CTCDayOrNightOnlySupport 69  CTCTurncoat 69
CTCSummonableCreature 69  CTCAIScratchpad 69    CTCOverheadDisplay 69
CTCSleep 69             CTCPhysicsNavigator 68  CTCAvoidRegionExit 68
CTCLook 65              CTCCarrying 65          CTCWeaponTrail 65
```

**Hero-singleton components** (count == 1 across the whole save — all live on the hero
entity in cell 10, useful as locator anchors): `CTCHeroStats`, `CTCHeroMorph`,
`CTCHeroExperience`, `CTCInventory`, `CTCInventoryClothing`, `CTCInventoryWeapons`,
`CTCInventoryStats`, `CTCInventoryExperience`, `CTCInventoryTrade`, `CTCInventoryQuests`,
`CTCInventoryMap`, `CTCInventoryAbilities`, `CTCGamePlayer`, `CTCPhysicsControlled`,
`CTCHeroOpinionDeedLog`, `CTCHeroRangedAttacks`, `CTCSpecialAbilities`, `CTCBoastUI`,
`CTCQuickAccessMenu`, `CTCInGameMenu`, `CTCHeroGuide`, `CTCStealth`, `CTCFishing`, …
Also world singletons: `CTCTeleporter`, `CTCDoor`, `CTCBoss`, `CTCBlacksmiths`,
`CTCTavernGameMaster`, `CTCOracleMinigame`, `CTCFireheartMinigame`.

Notable gameplay-editing anchors: `CTCChest` ×11 (chest state — pairs with the retail
`CChestDef` findings, `CChestDef::Transfer` 0x004DE204), `CTCSearchableContainer` ×54,
`CTCContainerRewardHero` ×120 (reward tables), `CTCBuyableHouse` ×32, `CTCShop` ×10,
`CTCGold` ×50, `CTCQuestCard` ×15, `CTCCreatureOpinionOfHero` ×76.

### 5. Hero stats (re-validated through the graph walker)

`herostats` command locates the hero cell by `CTCHeroStats\0` (cell 10) and reads the
seed-0 tags in the component window — output on Cornelio/Manual-Save1:

| field | tag | value | matches [Hero stats](#hero-stats) §2 |
|---|---|---|---|
| Money | b03ccbfd | 1625 | YES |
| Morality | 79a2d479 | 483 | YES |
| Age | 663c8f40 | 26.4 (f32) | YES |
| Fatness | b0da59fb | 0.0 (f32) | YES |
| RenownLevel | 0c4ed775 | 2 | YES |

### 6. Hero inventory (CTCInventory / CTCInventoryClothing / CTCInventoryWeapons)

CONFIRMED on-disk layout of an inventory component's data block:

```
[u32 listTag = getcrc("InventoryItems", 0) = 2b837863]
[u32 itemCount][u8]
itemCount × record:
    [u32 nameLen][nameLen ASCII chars, no NUL]      OBJECT_* def name
    [u32 quantity]
    [u32 unkA][u8 ext]                              ext != 0 → variable extension payload
                                                    (money-bag contents, weapon augmentation
                                                    sublists [u32 n][records], hotkey CRCs)
then: run of NUL-terminated OBJECT_* names                  (quick-access / equipped refs)
then: further [tag][value] fields (e.g. equipped-weapon cstr in CTCInventoryWeapons)
```

Weapons use the literal name `"NULL"` for empty augment slots (skipped by the decoder).
Validated on Cornelio/Manual-Save1:

| component | declared count | decoded |
|---|---|---|
| CTCInventory | 62 | 62 |
| CTCInventoryClothing | 40 | 40 (worn pieces + `HERO_SUIT_*` outfit records) |
| CTCInventoryWeapons | 3 | 4 = 3 weapons + 1 **nested** `OBJECT_FLAME_AUGMENTATION` record inside `OBJECT_HERO_STICK`'s augment sublist |

Decoded sample (all names are valid game.bin OBJECT_* defs, quantities sane):
`OBJECT_RESURRECTION_PHIAL ×2, OBJECT_GUILD_SEAL_1 ×1, OBJECT_HAND_LAMP ×1,
OBJECT_FISHING_ROD ×1, OBJECT_SPADE ×1, OBJECT_HERO_MONEY_BAG ×1, OBJECT_SILVER_KEY ×7,
OBJECT_MANA_POTION ×14, OBJECT_HEALTH_POTION ×3, OBJECT_TROPHY_WHISPER_BROOCH_01 ×1, …`
Weapons: `OBJECT_HERO_STICK ×1, OBJECT_EBONY_CUTLASS ×1, OBJECT_OAK_LONGBOW ×1`.

**The trailing name table = equipped / quick-access state** (values are the proof):

- `CTCInventoryClothing`: exactly the 5 currently worn pieces — `OBJECT_HERO_NO_HAT,
  OBJECT_HERO_SHIRT_LEATHERARMOUR_GOOD, OBJECT_HERO_GLOVES/TROUSERS/BOOTS_LEATHERARMOUR`.
- `CTCInventoryWeapons`: `OBJECT_EBONY_CUTLASS` — the equipped weapon.
- `CTCInventory`: 9 names (money bag, 5 expressions, lightning ability, trophy tooth,
  mana potion) — the quick-access slot assignments [hypothesis on exact slot order].

Component-tail tags after the tables (`a7d70546`, `a10e3cad`, `c93f54bc`,
`691625d6`→equipped-weapon cstr, `1f5c5c94`/`f85a6bb9`→u32 values, …): names unrecovered
(2-strike wordlist brutes failed; structure recorded by the tool). Exact record grammar: §9.5.2.

### 7. QUESTS / FACTIONS / REGIONS body sections (chunk1, outside SAVED_ENTITIES)

All three parse byte-exact on Cornelio/Manual-Save1 (`report`):

- **QUESTS** — one Lionhead-LZO1X text blob (**17,457 bytes** of persist-script text after
  decompression) + a **119-byte binary tagged trailer**. LZ blob framing (CONFIRMED on 30
  blobs: 1 QUESTS + 29 FACTIONS): `[u32 ulen][u16 clen][clen bytes LZO1X stream][raw literal
  tail]` — the stream self-terminates ~3 bytes short of ulen; the tail bytes are stored raw.
  Decoder: `tools/lionhead_lz.py`. Trailer tag names are UNRECOVERED (2 wordlist brutes
  failed) — structure only (`parse_quest_trailer`).

#### 7.1 Decoded QUESTS text structure (the quest read/flip surface)

Plain persist-text (`Name value;` grammar — text mode, so **no CRC tags**). Blocks, in
order, from the real Cornelio/Manual-Save1 decode:

```
START_QUEST_CARDS                          quest cards held, one CSV line each:
OBJECT_QUEST_CARD_HOBBE_CAVE, Q_HobbeCave, 0     [cardDef, questName, flag]
END_QUEST_CARDS
START_ACTIVE_QUEST_CARDS                   accepted cards as u64 DECIMAL entity UIDs
18446742974197958578                             (= 0xffffff00… card-object UIDs, §2)
… END_ACTIVE_QUEST_CARDS
START_COMPLETED_QUESTS                     completed quests, same u64 UID form
18446742974197924033                             (9 entries in the sample)
… END_COMPLETED_QUESTS
START_ACTIVE_QUESTS                        running quest scripts by NAME (48 in sample:
Q_SunnyvaleMaster, Gameflow, V_SickChild, …)
END_ACTIVE_QUESTS
START_SAVED_QUESTS                         one block per quest script:
START_NEW_QUEST V_SickChild
START_SCRIPT_DATA                            per-quest persistent variables:
FinishedQuest FALSE;  GotFishingSpotMushroom TRUE;  MansLoverState 0; …
END_SCRIPT_DATA
START_ENTITY_SCRIPTS                         per-entity script state, keyed by u64 UID:
START_NEW_ENTITY_SCRIPT 179907590094853509
GivenObject TRUE;
END_NEW_ENTITY_SCRIPT …
END_NEW_QUEST … END_SAVED_QUESTS
```

**Read:** `parse_save_entities.py <save> quests` prints the full text.
**Flip:** quest completion lives in (a) the `COMPLETED_QUESTS` u64-UID list and (b) the
quest's own `SCRIPT_DATA` vars (`FinishedQuest TRUE;` etc.) — edit the text, re-encode
(§8.1). The 119-byte binary trailer references `"Q_TraderEscort"` ×3 (the currently
tracked/boast quest) with small ints (3/1/3/9) and bools.
- **FACTIONS** — `[u32 headTag = 4f0e45c3][u32 count]` then count × `[u32 recTag][LZ blob]`.
  **29/29 records** consume the section byte-exact. Text per record:
  `Name "FACTION_X"; NumberOfAllies n; Ally "…"; … EndFaction`. recTag is NOT
  `getcrc(factionName, 0)` — names unrecovered.
- **REGIONS** — `[u32 a][u32 b][u32 c]` head then run of `[region-name \0][19 bytes flags]`.
  **141 records** on Cornelio/Manual-Save1.

### 8. Reproduce

```
python tools/parse_save_entities.py "<save>" cells       # cell inventory
python tools/parse_save_entities.py "<save>" census      # CTC* component census
python tools/parse_save_entities.py "<save>" entities    # 267 top-level entity headers
python tools/parse_save_entities.py "<save>" defcensus   # DEF-name census
python tools/parse_save_entities.py "<save>" herostats   # confirmed CTCHeroStats fields
python tools/parse_save_entities.py "<save>" inventory   # hero inventory decode
python tools/parse_save_entities.py "<save>" quests      # QUESTS LZ text + trailer
python tools/parse_save_entities.py "<save>" factions    # 29 faction records
python tools/parse_save_entities.py "<save>" regions     # region flag records
python tools/parse_save_entities.py "<save>" report      # one-shot summary
```

All commands above are read-only. `flipquest <questName> <outfile>` is the write-path PoC
(writes only to the given output file, never to the source save).

#### 8.1 Write path — EXECUTED and re-verified (quest flip round-trip)

There is no LZO1X *compressor* port, but the grammar admits a **store-mode stream**
(`lzo_store()`: one initial literal run, one 3-byte M3 match on a repeated 3-gram, one
extended literal run, EOS marker; blob rebuilt in the retail shape
`[u32 ulen][u16 clen][stream][3 raw bytes]`). Validated:
`lionhead_lz.decompress(lzo_store(T)) == T` for the real QUESTS text **and all 29 faction
texts of all 3 saves** (90 round-trips) — the decoder is a 1:1 port of the engine routine,
so grammar-level acceptance is proven.

Executed PoC on a COPY of `Cornelio/Manual - Save1`:

```
python tools/parse_save_entities.py <copy> flipquest Q_HobbeCave <out>
→ wrote 307,200 bytes, trailer sig re-verified, quest 'Q_HobbeCave' now in COMPLETED_QUESTS
```

Pipeline: decode QUESTS text → insert the quest into `START_COMPLETED_QUESTS` →
`replace_quests_text()` (re-encodes the blob, patches the QUESTS `[u32 sectionByteLen]`) →
`save_edit.build_save()` (re-deflates chunks, fixes header dwords, **recomputes the seed-0
trailer signature**, pads to 300 KB). The rebuilt save re-reads cleanly, the binary quest
trailer is byte-identical, and the decoded text shows the flip. (A full flip should also
set the quest's `FinishedQuest TRUE;` and drop it from `START_ACTIVE_QUESTS` — same text
edit. Retail's in-game acceptance of the store-mode stream remains a live-fire unknown; the
trailer-signature question was closed 2026-07-20 — [Signature enforcement](#signature-enforcement).)

#### 8.2 Save-editor recipe — inventory edit

1. `save_edit.read_save(raw)` (verifies trailer sig); find `SAVED_ENTITIES\0` in chunk1,
   inflate cells (`inflate_cells`), pick the cell containing `CTCHeroStats\0`.
2. Find `CTCInventory\0`, skip `[u32 0][u8 0][u32 dataLen]`; check
   `[u32 2b837863][u32 itemCount]`; walk records (§6) to `[u32 nameLen][name]` of the
   target item and overwrite the following `u32 quantity` **in place** (fixed width — no
   reframe). Adding/removing whole items shifts lengths (component `dataLen`, `itemCount`,
   cell size) — prefer quantity edits until the record-extension grammar is closed.
3. Re-deflate the cell (`zlib.compress(cell, 9)`), splice it back, patch the
   `SAVED_ENTITIES [u32 payloadLen]` by the compressed-size delta, set
   `chunk1_ulen += delta`, then `save_edit.build_save(...)` (re-signs + pads) — identical
   to the validated [Hero stats](#hero-stats) §5 gold/morality round-trip. **Plus (§9.5.1
   CORRECTION): patch the cell descriptor `recLen`/`clen`/`ulen`.**

### 9. Cross-save validation (4 saves, all clean)

Every command above was run on all four saves; all parse without error and all invariants
hold (`report` + `herostats`):

| | Cornelio/Manual-Save1 | Cornelio/AutoSave.qs | Test/AutoSave | Default/AutoSave |
|---|---|---|---|---|
| cells | 25 | 25 | 2 | 2 |
| inflated bytes | 383,284 | 382,797 | 38,824 | 39,983 |
| entity records (walk) | 348 | 348 | 44 | 46 |
| `END\0` cross-check | 348 | 347¹ | 44 | 46 |
| classed top-levels | 267 | 266 | 42 | 42 |
| distinct DEF-names | 108 | 108 | 18 | 20 |
| hero cell idx | 10 | 12 | 0 | 0 |
| Money / Morality | 1625 / 483 | 1625 / 483 | 0 / 0 | 0 / 0 |
| Age / Fatness | 26.4 / 0.0 | 26.4 / 0.0 | 18.0 / 0.0 | 18.0 / 0.0 |
| RenownLevel | 2 | 2 | 0 | 0 |
| CTCInventory (count=decoded) | 62=62 | 62=62 | 5=5 | 5=5 |
| CTCInventoryClothing | 40=40 | 40=40 | 0=0 | 0=0 |
| CTCInventoryWeapons | 3 decl / 4 dec¹ᵃ | 3 decl / 4 dec¹ᵃ | 0=0 | 0=0 |
| QUESTS text / trailer B | 17,457 / 119 | 17,457 / 119 | 7,010 / 8 | 6,002 / 8 |
| FACTIONS records | 29 | 29 | 29 | 29 |
| REGIONS records | 141 | 141 | 141 | 141 |

¹ AutoSave.qs: 348 anchor-walked records vs 347 `END\0` — a 1-record delta on this one save
(anchor walk finds one record whose END marker is absent or split; benign, under
investigation). All other saves cross-check exactly.
¹ᵃ The 4th decoded weapons record is the nested `OBJECT_FLAME_AUGMENTATION` inside
`OBJECT_HERO_STICK`'s augment sublist (§6) — declared count counts weapons only.

Consistency arguments:
- Both Cornelio saves (same playthrough, minutes apart) decode to **identical** hero stats,
  inventory counts, QUESTS text size, and DEF-name census (108) — deterministic decode.
- Test and Default are fresh new-game heroes → canonical starting values (Money 0,
  Morality 0, Age 18.0, RenownLevel 0, 5 starting inventory items, no clothing/weapons yet)
  and the same 2-cell starting world (42 classed top-levels; Object 31 / Building 10 /
  Village 1).
- FACTIONS (29) and REGIONS (141) are invariant across all saves — they enumerate static
  world tables, not per-playthrough state.
- Hero cell index moves (10 vs 12 vs 0) — locate the hero by `CTCHeroStats\0` content, never
  by cell position.

### 9.5 ★ 2026-07-20 pass — cell RECORD framing decoded + productized editor

**New saves:** profile `1` (fresh hero, 2026-07-20) parsed clean alongside the original four —
six saves total now cross-validated.

#### 9.5.1 SAVED_ENTITIES cell records (CONFIRMED, 81/81 cells across 6 saves)

The "small u32 header of per-cell metadata" ([Hero stats](#hero-stats) §4) is now fully decoded. The
SAVED_ENTITIES section body is `[u32 0][u32 N?][u32 0]` then a run of **cell records**:

```
[u64 0]                     marker
[u64 recLen = 29 + clen]    record length (covers cellId..data + 9 trailing zero bytes)
[u64 cellId]                stable region/cell identifier (e.g. hero cell 28885a in Cornelio,
                            3020df in fresh saves; ids identical across saves of one layout)
[u64 clen]                  compressed length of the zlib cell
[u32 ulen]                  uncompressed (inflated) length
[clen bytes]                the zlib stream (78 xx)
[9 zero bytes]
```

Empty (unloaded-region) slots appear as all-zero runs between records; after the last record
the section carries a trailing index table of `[u32 X][u32 cellId][u32 0][u32 hash?][u32 0]`
entries mapping small ids (entity/things index?) to cellId — it references **cellId, not byte
offsets**, so it needs no patching on edits. [index-table semantics = hypothesis; record
framing = CONFIRMED by 81/81 invariant checks `marker==0 && recLen==29+clen && clen==zlib
consumed && ulen==inflated && 9-byte zero trailer`.]

> **★ CORRECTION to §8.1/§8.2 and [Hero stats](#hero-stats) §5:** patching only the SAVED_ENTITIES
> section length + chunk1_ulen is NOT sufficient — the per-cell `recLen`/`clen` (and `ulen`
> when the inflated size changes) in this descriptor MUST be patched too, or the engine's
> record walk mis-frames every cell after the edited one. The earlier gold/quest PoCs
> predated this finding; `save_edit.replace_cell()` now does all three.

#### 9.5.2 Inventory record grammar (exact, byte-confirmed)

```
component data = [u32 2b837863 = getcrc0("InventoryItems")][u32 itemCount][u8 0]
record         = [u32 nameLen][nameLen ASCII chars, no NUL][u32 quantity][u32 unkA][u8 ext]
                 (+ variable extension payload iff ext != 0)
```
- `unkA` = 0 for most items but **nonzero on used tools** (Cornelio: FISHING_ROD 65,
  SPADE 233) — [hypothesis: per-item usage counter].
- Money-bag extension (`OBJECT_HERO_MONEY_BAG`, ext=1): 20 bytes = 15×00 + `01 00 00 00` + 00
  (recorded; internal meaning still open).
- Component tail on a fresh hero: exactly 3 `[tag][u32 0]` fields (`a7d70546`, `a10e3cad`,
  `92e50fdb`) — names still unrecovered.

#### 9.5.3 Productized editor — `tools/save_edit.py` (validated this pass)

```
save_edit.py report   <save>                    hero stats + all inventory components
save_edit.py set-gold <save> <out> <amount>     Money (CTCHeroStats)
save_edit.py set-stat <save> <out> <name> <v>   Money|Morality|RenownLevel (i32), Age|Fatness (f32)
save_edit.py set-qty  <save> <out> <DEF> <qty>  in-place u32 quantity edit
save_edit.py add-item <save> <out> <DEF> [qty]  inserts a fresh record (ext=0,unkA=0) at list
                                                front; def name validated against names.bin
                                                (13,593 defs); refuses duplicates (use set-qty)
```
Every write: locates the hero cell by `CTCHeroStats`, edits the inflated cell, re-deflates,
patches descriptor `recLen/clen/ulen` + section len + chunk1_ulen, rebuilds + re-signs the
container (trailer CRC seed 0, exact 300 KB, zero padding — [Signature enforcement](#signature-enforcement)
§6 recipe). `verify_edit()` then asserts byte-level surgical scope: chunk0, every non-hero cell
(compressed AND inflated), and all of chunk1 outside SAVED_ENTITIES byte-identical; hero-cell
inflated diff printed as exact ranges. Validated on real-save copies:
- set-gold 1625→999999: 3-byte diff (value bytes only); set-stat Morality 483→−750: 4 bytes;
- set-qty SILVER_KEY 7→30: 1 byte; add-item HEALTH_POTION×50 / EBONY_KATANA×1: exactly
  `dataLen+33/32`, `itemCount+1`, inserted record — nothing else.
- Edited saves re-parse clean through the independent `parse_save_entities.py` walker
  (348 records, END cross-check 348, census unchanged).

**NOT yet live-tested in-game** (user away) — staged copies only under the session scratchpad;
real saves untouched. Live QA: load an edited copy, confirm gold/stat/item appears; specifically
confirm (a) list-front insertion order is accepted, (b) a general-list add of a weapon def is
enough for it to appear (vs needing a CTCInventoryWeapons record), (c) qty>stack-max behaviour.

### 10. Open items

Consolidated into [Open questions](#open-questions) (entity-graph items E1–E5).

---

## Hero stats

**Status:** The hero's live editable stats are LOCATED and the primary ones (gold, morality,
age, renown-level) are **CONFIRMED with real save values** and a **working round-trip editor**.
**Date:** 2026-07-19. This closes the former SAVE_HEADER_MAP §4 follow-up (hero live-stats:
gold / morality / experience / renown).

### 0. TL;DR verdict — TAGGED, not positional (but you must know each field's type)

The hero's live stats are **NOT** flat HEADER/PLAYER fields. They live in the hero `CThing`
entity inside the **`SAVED_ENTITIES`** entity graph. That entity graph has THREE nesting layers:

1. **SAVED_ENTITIES section** (inside chunk1) → a small u32 header + a run of **independently
   zlib-compressed spatial "cells"** (one per loaded region/cell). The hero lives in exactly one
   cell (in the sample: the largest, index 10 of 25).
2. **Inside an inflated cell:** a sequence of **entities**, each framed positionally as
   `[instance-name ASCII\0][pad][u64 UID][pad][DEF-name ASCII\0][u32 componentBlockLen][components…]`.
3. **Inside each component:** `[component-class-name ASCII\0][pad][u32 dataLen]` then a
   **tagged field stream** `[u32 seed0-CRC(fieldName)][value]…` — the SAME `[tag][value]`
   shape as the HEADER section, **but the value width is per-field (type-driven), NOT
   self-describing** (a `Money` field is a 4-byte int, a bool field is 1 byte, `Age` is a
   4-byte float). So a save editor finds a stat by (a) walking to the hero entity, (b) walking
   to the right component by class-name, (c) matching the seed-0 CRC tag of the field name, and
   (d) knowing that field's type to read/overwrite the right number of bytes.

So: **tagged at the field level** (recover name→seed0-tag→type per stat), **positional at the
entity/component level** (walk by name). It is emphatically not a flat brute-forceable tag blob.

- 2026-07-19 (SAVE_HEADER_MAP §4, superseded the same day): the hero blob was described as
  `[type][UID][component data]…` with gold/morality/experience field names
  "[hypothesis / not yet recovered]"; the `CGoldDef`/`CExperienceDef`/`CHeroExperienceDef`/
  `CHeroStatsDef`/`CCreatureStatsDef::Transfer` addresses ([Container and header layout](#container-and-header-layout)
  §2.8) are the **DEF** (game.bin) structures holding design-time thresholds/multipliers
  (`ExperienceLevelThresholds`, `WillExperience`, `SkillExperience`, `StrengthExperience`,
  `MoralityChange` — from `donor_def_transfers.c`), not the hero's live totals; the live totals
  mirror these categories on the runtime hero CThing.

### 1. Locating the hero entity (reproducible)

- **Hero UID** = `PlayerCharacterUID` from the PLAYER section ([Section grammar](#section-grammar) §3.5,
  tag `getcrc("PlayerCharacterUID",0)` = `96adc827`, stored LE on disk as `27 c8 ad 96`), a u64.
  In the samples the hero UID is `0xffffff0000003ae3` (Cornelio) / `0xffffff00000000c2`
  (Test/Default). The UID is the low bytes + a `0xffffff` high-word marker; it is written
  verbatim in the entity header.
- The hero entity is **unique and trivially found**: in the inflated SAVED_ENTITIES cell there
  is exactly ONE `PlayerCreature\0` (instance name) → `[u64 UID]` → `CREATURE_HERO\0` (def
  name), and exactly ONE `CTCHeroStats\0` component. Confirmed: counts = 1/1/1 in the sample.
- Practical locator: inflate the SAVED_ENTITIES cells (§4), find the cell containing
  `CTCHeroStats\0`; that cell is the hero's. The stats are in a ~0x120-byte window right after
  the `CTCHeroStats\0` component header.

#### Hero entity framing (bytes, Cornelio/Manual-Save1, cell-relative)
```
50 6c 61 79 65 72 43 72 65 61 74 75 72 65 00   "PlayerCreature\0"   (instance name)
… 00-pad …
e3 3a 00 00 00 ff ff ff                        u64 UID = ffffff0000003ae3
… 00-pad …
43 52 45 41 54 55 52 45 5f 48 45 52 4f 00      "CREATURE_HERO\0"    (def name)
10 00 …                                        u32 component-block framing
43 52 45 41 …                                  first component ("CTCPhysicsControlled", etc.)
…
43 54 43 48 65 72 6f 53 74 61 74 73 00         "CTCHeroStats\0"     (the stats component)
00 00 00 00  46 0c 00 00                        pad + u32 dataLen (0x0c46)
79 d4 a2 79  e3 01 00 00  …                      [Morality tag][value] … tagged field stream
```

The hero entity carries several components in order (seen in the sample):
`CTCVolumeContainmentTracker`, `CTCHitLocations`, then the graphics/physics components
`CTCPhysicsControlled`, `CTCHeroMorph` (appearance), and **`CTCHeroStats`** (the stat store),
plus others. `CTCHeroMorph` and `CTCHeroStats` both carry `Strength/Skill/Will/Age/Morality`
tags — but `CTCHeroMorph`'s are **float 0..1 appearance-morph weights**, whereas
**`CTCHeroStats`'s are the real integer stats.** Do not confuse them; edit the ones in
`CTCHeroStats`.

### 2. CTCHeroStats field map — CONFIRMED stats

Field tag = `getcrc(name, 0)`. Values read at the tag+4 offset. Offsets below are into the
inflated hero cell for `Cornelio/Manual-Save1` (`CTCHeroStats` component @ cell-off 0x1600
region); each value cross-checked across 4 saves.

| field name | seed0 tag (LE on disk) | type | Cornelio (played) | Test/Default (new game) | status |
|---|---|---|---|---|---|
| **Morality** | `79a2d479` (`79 d4 a2 79`) | i32 (signed) | **483** | **0** | **CONFIRMED** |
| **Money** (= gold) | `b03ccbfd` (`fd cb 3c b0`) | i32/u32 | **1625** | **0** | **CONFIRMED** |
| **Age** | `663c8f40` (`40 8f 3c 66`) | f32 | **26.4** | **18.0** | **CONFIRMED** |
| **Fatness** | `b0da59fb` (`fb 59 da b0`) | f32 | 0.0 | 0.0 | **CONFIRMED** |
| **RenownLevel** | `0c4ed775` (`75 d7 4e 0c`) | i32 | **2** | **0** | **CONFIRMED** |

Cross-validation logic: Test and Default are fresh new-game heroes → Morality 0, gold 0,
Age 18.0, RenownLevel 0 (canonical starting values). Cornelio (a played hero) → Morality 483
(good-ish), gold 1625, Age 26.4, RenownLevel 2. Both Cornelio saves (Manual-Save1 and
AutoSave.qs, same playthrough) decode to the **identical** values. All sane and in range.

#### Experience / renown pools — positionally CONFIRMED, name = [hypothesis]

Between `Money` and `RenownLevel` in the `CTCHeroStats` stream is a run of `[u32 tag][i32 value]`
fields that are the hero's **experience pools** (general + per-stat strength/skill/will and the
renown accumulator). They decode as regular 4-byte ints and clearly differ between a played and
a new-game hero, so their POSITION and TYPE are confirmed, but their exact field *names* were
not recovered this pass (they do not match an obvious wordlist). Observed aligned tags in order:

| tag (seed0) | Cornelio value | Test value | note |
|---|---|---|---|
| `896e293a` | 0 | 0 | [hypothesis] pool/flag |
| `c731e25b` | 3190 (`0x0c76`) | 0 | [hypothesis] an experience pool (0 at new game) |
| `695d2a96` | 1350 (`0x546`) | 1000 (`0x3e8`) | [hypothesis] an experience/starting pool |
| `37c59c98` | 0 | 0 | [hypothesis] |
| `059107c4` | 0 | (2) | [hypothesis] |

In-memory (runtime) the stat-experience array is a 4-long block: `CTCHeroStats+0x118` →
`[general, strength, skill, will]` (from `CTCHeroStats::GetHeroStatExperience`/`AddHeroStatExperience`,
retail `0x0057a…`, index 0/1/2/3). The four serialized experience tags above mirror that array;
matching each tag to strength/skill/will/general by name is the only remaining [hypothesis].

**Health / Physique:** the hero's `Health` (tag `78b48786`, f32 = 80.0) appears in the entity
header block (near the UID/`CREATURE_HERO`), value 80.0 in Cornelio — the creature's current
health, CONFIRMED present but living on the base creature record, not in `CTCHeroStats`.
(`Scale` `eba5fd9d` → f32 1.0 is the other decoded base field — [Entity graph](#entity-graph) §2.)

### 3. Evidence (≥2 sources per confirmed stat)

**Decompiled code (retail Fable.exe, base 0x400000):**
- `ghidra_out/decomp_hero_stats_classes.c` — the `CTCHeroStats` accessors that pin the runtime
  layout: `CTCHeroStats::SetMorality` (morality stored at **`this+0x28`**),
  `GetHeroStatExperience`/`AddHeroStatExperience` (stat-exp array at **`this+0x118`**, 4 longs),
  `GetPercentageThroughRenownLevel`/`GetMaxBoastingCrowdForRenownLevel` (renown level at
  **`this+0x70`**, renown value at **`this+0x78`**). `SetMorality` clamps to
  `±*(def+0xd0)` and mirrors morality → `CTCHeroMorph::SetAge` (why `CTCHeroMorph` carries a
  parallel float `Age`/`Morality`).
- `ghidra_out/decomp_save_hero_stats.log` — `LoadEntitiesBinary` (retail `0x00521240`),
  `CThingManager` child-walk (`0x00530460`), Def transfers. Confirms the entity graph is loaded
  from the binary SAVED_ENTITIES stream.

**Real save files (ground truth, byte-exact):** `Test/AutoSave`, `Default/AutoSave`,
`Cornelio/AutoSave.qs`, `Cornelio/Manual - Save1`. Every confirmed stat's seed-0 CRC tag
sits at the expected place in the hero's `CTCHeroStats` window in all four, decoding to the sane
values in §2. The tag identities are independently proven by `getcrc(name,0)` matching the
on-disk 4 bytes (e.g. `getcrc("Morality",0)=79a2d479`, `getcrc("Money",0)=b03ccbfd`).

### 4. SAVED_ENTITIES container layout (for the editor)

Inside the inflated chunk1 stream:
```
"SAVED_ENTITIES\0"
[u32 se_payload_len]          <- compressed byte length of all cells (header→PLAYER). PATCH on edit.
[u32 0][u32 ?][u32 0]…        <- small header of per-cell metadata (u64-ish fields; e.g. the first
                                 cell's uncompressed size 0x37e=894 and compressed size 0x215=533
                                 appear here). Cells are self-framed zlib so the loader can also
                                 walk them by the 78 xx magic.
[cell 0: zlib 78 01 …]        <- inflates to one region's entity records
[cell 1: zlib …]
…                             <- N cells (25 in Cornelio/Manual-Save1)
```
Each cell inflates to a run of entities (§1 framing). Concatenating all inflated cells gives the
full entity graph (383,284 bytes in the sample). The hero is in the cell that contains
`CTCHeroStats\0`. The "small header" is the 36-byte per-cell descriptor, fully decoded 2026-07-20
([Entity graph](#entity-graph) §9.5.1 / [Section grammar](#section-grammar) §3.6).

### 5. Save-editor recipe: edit gold / morality (VALIDATED round-trip)

> **★ 2026-07-20 CORRECTION:** step 5 below is INCOMPLETE — each cell is framed by a 36-byte
> descriptor `[u64 0][u64 recLen=29+clen][u64 cellId][u64 clen][u32 ulen]` whose
> `recLen`/`clen`/`ulen` must ALSO be patched on any cell edit ([Entity graph](#entity-graph) §9.5.1).
> Use `save_edit.py set-gold / set-stat / set-qty / add-item`, which do this correctly.

This composes with `tools/save_edit.py` (which already solves the container: zlib chunks +
the **seed-0 trailer signature** over `file[0:trailer_pos]` — that "signature" open item is
CLOSED; `read_save` verifies it and `build_save` re-signs it). Steps:

1. `sf = save_edit.read_save(raw)` — inflates chunk0||chunk1 and **verifies the trailer sig**.
2. In `sf.payload`, find `SAVED_ENTITIES\0`; walk the zlib cells between it and `PLAYER\0`,
   inflating each; pick the cell containing `CTCHeroStats\0` (the hero cell).
3. In the inflated hero cell, find `CTCHeroStats\0`, then within the next ~0x120 bytes find the
   seed-0 tag (`getcrc("Money",0)` for gold, `getcrc("Morality",0)` for morality) and overwrite
   the 4-byte value at tag+4 (Money = u32 gold; Morality = **signed** i32, negative = evil).
4. Re-deflate the edited cell (`zlib.compress(cell, 9)` → `78 DA`; the loader accepts it).
5. Splice the new compressed cell back into `sf.payload` in place of the old one. The
   uncompressed cell size is unchanged (fixed-width int edit) but the **compressed** size shifts,
   so patch the `SAVED_ENTITIES [u32 se_payload_len]` field (`+0x00`) by the compressed delta
   (and, per the CORRECTION above, the cell descriptor `recLen`/`clen`).
6. `new_c1_ulen = sf.chunk1_ulen + compressed_delta` (SAVED_ENTITIES lives in chunk1's
   *uncompressed* stream, so its length tracks the spliced compressed bytes). chunk0 is unchanged.
7. `out = save_edit.build_save(sf.chunk0_ulen, new_c1_ulen, new_payload)` — re-deflates chunks,
   fixes header dwords, **recomputes the seed-0 trailer signature**, pads to 300 KB.
8. Re-read with `save_edit.read_save(out)` to confirm the trailer re-verifies.

**Validated:** the PoC `hero_edit_test.py` did exactly this on a COPY of `Cornelio/Manual-Save1`,
setting gold 1625→999999 and morality 483→-1000. The rebuilt 307,200-byte file re-read cleanly,
the **trailer signature re-verified** (new sig `cf9b3395`), PLAYER/QUESTS/REGIONS/FACTIONS and
`PlayerCharacterUID` stayed intact, and the hero cell re-decoded to the edited values.
(Whether the retail loader enforces the trailer sig on load was then the one live-fire unknown;
answered YES on 2026-07-20 — [Signature enforcement](#signature-enforcement).)

> Note on `save_edit.py` seeds: that file's `get_crc` default seed is `0xFFFFFFFF`, but the
> confirmed field-tag seed is **0** ([Overview](#overview) §1.3). When locating hero-stat tags use
> `getcrc(name, 0)` (as in `tools/save_tools/savecrc.py`), not `save_edit.get_crc`. The trailer
> signature `save_edit.sig_crc` (seed 0) is correct and unaffected.

### 6. Reproduce

```
# inflate a save, get chunk0/chunk1
python tools/save_tools/savecrc.py inflate "<save file>" <outdir>

# compute a hero-stat tag (seed 0):
python -c "import sys;sys.path.insert(0,'tools/save_tools');from savecrc import getcrc;print('%08x'%getcrc('Money',0))"
#   Money=b03ccbfd  Morality=79a2d479  Age=663c8f40  RenownLevel=0c4ed775

# container read/verify + rebuild pipeline (trailer sig included):
python tools/save_edit.py "<save file>"        # prints "trailer sig=… VERIFIED"
```

Scratchpad artifacts used this pass (not committed): inflated `*.entities.bin`, the cell
inflater, cross-validator, and the validated round-trip editor `hero_edit_test.py`.

---

## Profile index

**Status:** CRACKED + validated on 4 real profiles; transfer procedure derived and a
corrected install STAGED (not deployed). **Date:** 2026-07-20.
**Problem solved:** a hand-copied `.sav` in the profile dir does NOT appear in the in-game
LOAD list — the game enumerates *manual* saves from `Profile.bin`, not by scanning the dir.
The shared trailer-signature gate ([Signature enforcement](#signature-enforcement)) covers BOTH
`FableSav` and `FablePro` containers. **Tool:** `tools/save_install.py`.

### 0. TL;DR

- `Profile.bin` is a **`FablePro`** container: 16-byte header + **plaintext persist-script**
  (`Name value;\r\n`, latin1) + **4-byte trailer CRC**. No zlib, no padding.
- The manual-save registry is 50 fixed text slots `SaveGameNames1..50`. A slot is shown in
  the LOAD list **iff its string is non-empty**. The string is BOTH the label AND the
  on-disk filename (`SaveGameNames1 "Manual - Save1"` → file `…\Manual - Save1`). No GUID /
  slot-id / embedded save-id — the key is the exact filename.
- `AutoSave` / `AutoSave.qs` are reserved special slots, NOT in the registry; they show
  up regardless. Only **manual** saves are registered here.
- The trailer is a **seed-0 reflected CRC-32** over `file[0:total_data_len]` — the SAME
  primitive as the `.sav` trailer — and it is **hard-enforced on load**
  (`CUserProfileManager::VerifySignature` @ retail `0x00409730`). Editing the text WITHOUT
  recomputing the trailer bricks the whole profile. **This is the only checksum that blocks
  a naive edit.** It does NOT checksum the referenced `.sav` files.
- Per-slot LOAD-screen metadata (hero name, chapter/region, playtime, thumbnail) is read
  lazily FROM THE `.sav` itself at enumerate time — it is not stored in `Profile.bin`. A
  valid edited `.sav` displays correctly with no extra work.

### 1. Container format (CONFIRMED on Cornelio / 1 / Default / Test)

```
0x00  8    magic  "FablePro"
0x08  u32  0                       (always 0 in the 4 real profiles)
0x0C  u32  total_data_len          == byte offset of the trailer == filelen - 4
0x10  ..   persist-script text     `Name value;\r\n` grammar, latin1, NUL-free
EOF-4 u32  trailer signature       = CCRC::Calc(seed=0, file[0:total_data_len])
```

- Reflected CRC-32, poly `0xEDB88320`, table = game's `0x0449BE30`, **no final inversion**,
  seed **0** — identical to the `.sav` trailer and to save-stream field tags.
- **No zlib** anywhere (unlike `.sav`); the payload is raw text.
- **No padding**: `bytes_after_trailer == 0` on all 4 profiles. (The `VerifySignature`
  decompile mentions a `0x4000`/`0x20`-pad branch for `FablePro`
  ([Signature enforcement](#signature-enforcement) §1d), but the real profiles are
  variable-length 16 689–18 084 B and unpadded, so that size/pad branch is not applied to
  profiles as written — the **CRC gate is the operative check**. Empirically verified:
  `Calc(0, prefix) == stored` on all four.)

Sample header dwords / trailer verification (all `MATCH=True`):

| profile   | filelen | total_data_len (@0x0C) | trailer   | CRC(seed0) |
|-----------|--------:|-----------------------:|-----------|------------|
| Cornelio  | 18088   | 18084                  | 94ADE8E3  | 94ADE8E3 ✓ |
| 1         | 16693   | 16689                  | 171E6CF1  | 171E6CF1 ✓ |
| Default   | 16890   | 16886                  | 8CEC5F75  | 8CEC5F75 ✓ |
| Test      | 16887   | 16883                  | 676D17E2  | 676D17E2 ✓ |

### 2. The text payload

One flat persist-script. Notable blocks, in file order:

- `ProfileName "<name>";` — the profile/hero-profile display name (== the dir name).
- Options: `SubtitlingEnabled`, `VibrationEnabled`, `Camera*Inverted`, `Gamma`, `HUDAlpha`,
  `SoundVolume`/`MusicVolume`/`DialogueVolume`, `CameraSensitivity`, `Antialiasing`,
  `TextureDetail`/`ShadowDetail`/`EffectsDetail`/`MeshDetail`, `Resolution`, `RefreshRate`,
  `ScreenAspectRatio`, `VerticalSyncEnabled`, … (settings-menu state).
- `AssignedInputsPassive[0..N] CActionInputControl(...);` — full input-binding table.
- `GivenTutorialCategories[0..N] <int>;` — tutorial progress.
- **`SaveGameNames1..50 "<name>";`** — the manual-save registry (see §3). Last block in file.

#### 2.1 The registry (`SaveGameNames`)

```
SaveGameNames1  "Manual - Save1";     <- occupied  => shown in LOAD list, file "Manual - Save1"
SaveGameNames2  "";                    <- empty     => slot 2 free
...
SaveGameNames50 "";
```

- Exactly **50 slots**, always all present (empty ones are `""`).
- **Occupied ⇔ non-empty string ⇔ appears in the LOAD list.** The string is the display
  label AND the exact filename in the same profile dir.
- Cross-check: profile `1` has an `AutoSave` on disk but **zero** occupied `SaveGameNames`
  slots — confirming autosaves are not registered here and only manual saves are.

### 3. Why a hand-copied `.sav` doesn't appear — and the fix

The game builds the LOAD list from the `SaveGameNames` registry, then reads each referenced
file's HEADER for the on-screen metadata. A file dropped in the dir with **no matching
registry entry is invisible**. (Observed: `Manual - Save2`, 307 200 B, present on disk,
missing from the LOAD list; `SaveGameNames2` was `""`.)

#### Transfer procedures

**(a) Overwrite an existing listed slot (simplest, no Profile.bin edit).**
Copy the edited `.sav` over the file named by an occupied slot (e.g. overwrite
`Manual - Save1`). `Profile.bin` is untouched — its entry still names the same file and it
does **not** checksum the `.sav`. Zero registry/CRC risk. Downside: you replace an existing
save.

**(b) Add a new slot (keeps existing saves).**
1. Copy the edited `.sav` into the profile dir under a new filename (e.g. `Manual - Save2`).
2. Set the first empty `SaveGameNamesN` to that exact filename.
3. **Recompute the `Profile.bin` trailer CRC** (seed 0, over the new `file[0:total_data_len]`)
   and update the `total_data_len` dword @ `0x0C`. Skipping this → `VerifySignature` fails →
   the entire profile fails to load.

`tools/save_install.py` implements (b) (and (a) when the slot name matches an occupied slot).

#### Checksum verdict

- **Yes, one checksum blocks a naive Profile.bin edit:** the seed-0 CRC-32 trailer. It must
  be recomputed after any text change (the tool does this and self-verifies).
- **No checksum blocks copying the `.sav` file** — the registry entry references only the
  filename; nothing in `Profile.bin` hashes the `.sav` contents. Procedure (a) therefore
  needs no `Profile.bin` change at all.

### 4. Tool — `tools/save_install.py`

```
python tools/save_install.py inspect  <profileDir>
python tools/save_install.py install  <edited.sav> <profileDir> <slotName> [--out-dir DIR] [--apply]
    <slotName> = "new"            -> first empty slot, file "Manual - SaveN"
               = "<existing name>"-> overwrite that occupied slot's file in place (proc a)
               = "Some New Name"  -> new slot, file+label = "Some New Name" (proc b)
```

- Default (no `--apply`): **STAGES** a corrected `Profile.bin` + the renamed `.sav` +
  `INSTALL.txt` into `--out-dir`; the real profile dir is untouched.
- `--apply`: writes the real profile dir directly (backs up `Profile.bin` → `Profile.bin.bak`
  first). Game must be closed.
- `inspect` reports every occupied slot, whether its file exists, and flags any
  **unregistered** files in the dir (the ones that won't show in-game).
- Every build recomputes + self-verifies the trailer and re-parses the result before writing.

### 4.1 Tool — `tools/save_metadata.py`

The frontend-facing read path is now available as a reusable, read-only join:

```text
python tools/save_metadata.py <profileDir>
python tools/save_metadata.py <profileDir> --json
```

It emits autosave-first rows, then only the non-empty `SaveGameNames1..50`
registry entries. Each referenced `FableSave!` is trailer-checked and its
confirmed 23-field `HEADER` is decoded (`WorldName`, `WorldFrame`, region and
minimap names, marker data, and play time). Missing or malformed files remain
visible as diagnostic rows with frontend action `0xDC`; fully validated files
carry action `0x11`. Unregistered directory files are intentionally ignored,
matching retail enumeration. The native C++ runtime still needs to consume this
row data before the live game load ownership boundary is connected.

### 5. Staged corrected install (ready to deploy)

`work/save_install/` contains the fix for the gold-edited `Manual - Save2`:

| file | note |
|---|---|
| `Profile.bin` (18 102 B) | Cornelio profile with `SaveGameNames2 "Manual - Save2"` added, trailer re-signed `A34E698D` (self-verifies). Slot 1 preserved. |
| `Manual - Save2` (307 200 B) | the gold-edited save (copy of the on-disk file). |
| `INSTALL.txt` | copy-in instructions (game closed; back up the old Profile.bin). |

Deploy = copy both files into `C:\Users\Cornelio\Documents\My Games\Fable\Saves\Cornelio\`
(overwriting `Profile.bin`), with the game closed. Then `Manual - Save2` appears in the
in-game LOAD list.

### 6. What still needs a live test

- **In-game confirmation** that the registered slot appears and loads (user was away; real
  profile untouched). This is the single unverified step; the on-disk / signature side is
  fully validated. If the LOAD screen shows a slot but empty metadata, the metadata read is
  from the `.sav` HEADER (already valid in the gold save), not from `Profile.bin`.
- Confirm the game does not *rewrite* `Profile.bin` on entering the LOAD menu in a way that
  drops an externally-added slot before you can load it (unlikely — it rewrites on SAVE, not
  on browse — but only a live run proves it).
- `dw08` (@0x08) is 0 in all 4 profiles; assumed reserved. Not load-bearing for install.

---

## Signature enforcement

**Status:** VERDICT REACHED (decompile-confirmed). **Date:** 2026-07-20.
**Question:** does retail `Fable.exe` ENFORCE the save trailer signature when loading a save?
**Answer: YES — ENFORCED, unconditionally, as a hard gate at the top of the load path.**

Evidence log: `ghidra_out/decomp_save_sig_load1.log` (headless DecompFuncs on retail
`Fable.exe`, project FableTLC, 2026-07-20).

### 1. The verifier: `CUserProfileManager::VerifySignature` @ retail `0x00409730`

Named in the DB from the BSim/RTTI port (`?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z`,
`ghidra_out/bsim_port_audit_05final.tsv` row `409730`); body decompiled and it does
exactly what the name says. Takes a file *path* (CWideString), opens the file itself, and returns
`bool`. Full decompile in the log; the load-bearing excerpts:

**(a) Magic dispatch** — accepts either container, first 8 bytes:
```c
CDiskFileWin32::Read(file, local_c, 8, 0);
bVar1  = memcmp8(local_c, "FablePro");   // profile container
bVar10 = memcmp8(local_c, "FableSav");   // save container
if (!bVar1 && !bVar10) return false;
```

**(b) Magic tail** — for `FableSav`, the next u32 (file[8..0xB]) must be `0x2165`
(= `65 21 00 00` = `"e!"` + 2 NULs, completing `"FableSave!"` + pad):
```c
if ((!bVar10) || (local_5c == 0x2165)) { ... } else return false;
```

**(c) Signature check — THE gate.** Reads the u32 at file offset `0x0C`
(`total_data_len` = trailer position), bounds-checks it against the file
size, re-reads the whole prefix `file[0 : total_data_len]` from position 0, reads the stored
u32 trailer at `total_data_len`, and compares against a fresh seed-0 CRC:
```c
if (uVar2 + 4 <= fileSize) {                 // uVar2 = u32 @ 0x0C = total_data_len
    FUN_00411910(uVar2);                     // resize buffer to total_data_len
    CDataInputStream::SetPosition(stream, 0);
    /* read total_data_len bytes into buffer (whole file prefix incl. header) */
    /* read next u32 -> local_5c = stored trailer signature */
    uVar2 = CCRC::Calc(0, buffer, total_data_len);
    if (uVar2 == local_5c) { ... }           // MISMATCH -> falls through -> return false
}
```
This is byte-for-byte the recipe the write side confirmed ([Container and header layout](#container-and-header-layout) §2.3):
**trailer = reflected CRC-32 (poly 0xEDB88320, zlib table), seed 0, no final inversion, over
`file[0 : total_data_len]`** — the literal on-disk bytes including the `FableSave!` magic and
the 16-byte header, everything up to (not including) the stored 4-byte trailer.

**(d) File size + padding check** — after the CRC matches, the file must be *exactly* the
expected slab size and the slack must be a constant fill byte:
```c
uVar4 = (-(uint)bVar1 & 0xfffb9000) + 0x4b000;   // FablePro -> 0x4000, FableSav -> 0x4B000
if (fileSize == uVar4) {
    /* stream the remaining bytes after the trailer in 0x4000 chunks: */
    if (byte != (!bVar1 - 1U & 0x20))   // FableSav -> every pad byte must be 0x00
        goto LAB_00409b34;              //           (FablePro -> 0x20, i.e. spaces)
    ...
    return true;                        // only exit that returns true
}
```
So for a `FableSave!` file: size must be exactly `0x4B000` (307200) and every byte from
`total_data_len + 4` to EOF must be `0x00`. Profiles (`FablePro`): size `0x4000`, pad `0x20`
(but see [Profile index](#profile-index) §1 — real profiles are variable-length and unpadded, so
the CRC gate is the operative check for them).

Every other path returns `false`. There is **no config-flag consult inside VerifySignature** —
no read of `UseSaveGameSignatures` or any global; the function validates unconditionally.

### 2. The enforcement site: `CWorld::LoadGameStateInternal` @ retail `0x004A21F0`

First statements of the function (decompile, same log):
```c
bool CWorld::LoadGameStateInternal(CWorld *this, CWideString *param_1, bool param_2)
{
    bVar2 = CUserProfileManager::VerifySignature(param_1);
    if (!bVar2) {
        return false;          // <-- HARD REJECT: load aborts before reading anything
    }
    if (param_2) { NProgressDisplay::StartProgress(...); PrepareForLoad(this); }
    ...                        // only now opens the file and parses FableSav/zlib/persist
}
```
The signature check is the *very first thing* the loader does, before `PrepareForLoad`, before
the zlib chunks are touched. A bad or missing trailer means `LoadGameStateInternal` returns
`false` and the save never loads. No branch bypasses it.

### 3. All callers of `VerifySignature` (xrefs, FindRefs.java)

| call site | caller | role |
|---|---|---|
| `0x0040d38b` | `Load` @ `0x0040d350` (CUserProfileManager) | profile/save load wrapper |
| `0x004a220f` | `CWorld::LoadGameStateInternal` @ `0x004a21f0` | THE save loader (above) |
| `0x005957fd` | `FUN_005957d9` | (see §5 audit) |
| `0x0047ed2e` | `FUN_0047ed0a` | (see §5 audit) |

### 4. Verdict table

| check | on-disk field | load behaviour | verdict |
|---|---|---|---|
| Header dword @ `0x0C` | `total_data_len` (NOT a hash — the SAVE_WRITER correction stands) | used as the CRC input length + trailer locator; an inconsistent value shifts/fails the CRC or the `+4 <= fileSize` bound | **ENFORCED (structurally)** |
| 4-byte trailer @ `total_data_len` | seed-0 CRC-32 of `file[0:total_data_len]` | computed-vs-stored compare; mismatch → `return false` | **ENFORCED** |
| File size | exactly `0x4B000` | equality check | **ENFORCED** |
| Padding | `0x00` from `total_data_len+4` to EOF | per-byte check | **ENFORCED** |

### 5. Secondary audit (all closed)

**`UseSaveGameSignatures` does NOT exist in retail — there is no off-switch.**
Raw memory scan of the whole retail image (`FindStrRaw.java`, log
`ghidra_out/decomp_save_sig_load3.log` / `..._load4.log`):
- `"UseSaveGameSignatures"` — **0 hits** in retail `Fable.exe`.
- `"UseBinarySaveGames"` — **0 hits**.
- `"UseRetailSaveGameSystem"` — present at `0x0122E85C` (scan sanity check; matches
  [Container and header layout](#container-and-header-layout) §2.6, proving the scan works).

So the config-flag mitigation originally suggested ("flip `UseSaveGameSignatures`") is
**impossible in retail** — those console vars are donor/debug-build only (donor `FableWin.exe`
strings at `0x0402F21C`/`0x0402F208`). Retail validates unconditionally; VerifySignature reads
no global flag.

**The other two callers** (`ghidra_out/decomp_save_sig_load2.log`) are consistent:
- `CUserProfileManager::Load` @ `0x0040D350` — gates **Profile.bin** load the same way:
  `if (VerifySignature(path)) { ... LoadGameState(file); }` — bad profile signature =
  profile silently not loaded. (Profiles use fill byte `0x20` and size `0x4000`, per §1d.)
  Note: `Profile.bin` payload is text-mode persist, but the *container* still carries the
  `FablePro` magic + seed-0 CRC trailer, and it IS checked.
- `FUN_005957d9` @ `0x005957D9` and `FUN_0047ED0A` @ `0x0047ED0A` — twin cached-validity
  helpers: compute `VerifySignature(path)` once, cache `{path, bool}` at `this+0xC4`/`+0xE0`,
  return the cached bool on repeat queries. This is the save-slot/UI "is this save valid?"
  predicate — an unsigned save would not even appear loadable in the UI.

### 6. Implications for the save editor (`forge save edit`)

**An edited save MUST be re-signed — and the proven writer recipe is exactly what the loader
checks.** Requirements for a loadable file (all verified against the loader decompile):
1. `"FableSave!"` + 2 NUL pad (bytes 0..0xB; loader checks 8-byte `FableSav` + u32 `0x2165`).
2. u32 @ `0x0C` = `total_data_len` = byte offset of the trailer = total size of
   header + chunk0 + framing dword + chunk1 (i.e. everything before the trailer).
3. u32 trailer at `total_data_len` = `CCRC::Calc(0, file[0:total_data_len])` — reflected
   CRC-32, poly `0xEDB88320`, seed 0, **no final XOR**, over the literal file bytes.
4. Zero-pad from `total_data_len + 4` to exactly `0x4B000` (307200 bytes). Non-zero slack
   bytes or a different file size are ALSO rejection causes, not cosmetic.

There is **no config bypass in retail** (§5) — re-signing is mandatory, not optional.

`tools/save_edit.py` already does all four (gates A–D pass 5/5, [Writer and tools](#writer-and-tools) §2),
so the round-trip editor's output is load-valid by construction. **`forge save edit` is safe to ship**
provided it reproduces the same four rules. Residual (low) risk: this is a static verdict;
a 30-second live confirmation (load a re-signed edited save in-game) is still worth doing as
the final QA gate, but no *signature* obstacle remains.

---

## Writer and tools

**Status:** READ→EDIT→WRITE pipeline built and gated. Signature **SOLVED (CONFIRMED)** on the
write side (2026-07-19) and **ENFORCED** on the load side (2026-07-20).
**Tool:** `tools/save_edit.py` (pure Python, stdlib `struct`+`zlib` only — no Ghidra, no deps).
The gates record only what is **provable on real bytes**; the retail game loading an edited save
could not run in that environment — the go/no-go (§4) is explicit about what the gates DO and
DON'T prove.

### 1. Tool inventory

| tool | role |
|---|---|
| `tools/save_edit.py <save>` | dump container + sections + verify signature; library: `read_save` / `build_save` / `replace_cell` / `verify_edit` / `sig_crc` / `ccrc_calc` |
| `tools/save_edit.py report <save>` | hero stats + all inventory components |
| `tools/save_edit.py set-gold <save> <out> <amount>` | Money (CTCHeroStats) |
| `tools/save_edit.py set-stat <save> <out> <name> <v>` | Money\|Morality\|RenownLevel (i32), Age\|Fatness (f32) |
| `tools/save_edit.py set-qty <save> <out> <DEF> <qty>` | in-place u32 quantity edit |
| `tools/save_edit.py add-item <save> <out> <DEF> [qty]` | inserts a fresh record (ext=0,unkA=0) at list front; def name validated against names.bin (13,593 defs); refuses duplicates (use set-qty) |
| `tools/parse_save_entities.py <save> cells\|census\|entities\|defcensus\|herostats\|inventory\|quests\|factions\|regions\|report` | read-only graph walker ([Entity graph](#entity-graph) §8) |
| `tools/parse_save_entities.py <copy> flipquest <questName> <out>` | quest-flip write PoC (LZO store-mode re-encode + re-sign) |
| `tools/save_tools/savecrc.py inflate <save> <outdir>` | → `*.header.bin`, `*.body.bin` |
| `tools/save_tools/savecrc.py crc <name> ...` | prints `Calc(0xFFFFFFFF)` — for SAVE tags use seed 0: `savecrc.getcrc(name, 0)` |
| `tools/save_tools/decode_header.py` | aligned typed HEADER table (23 fields) |
| `tools/lionhead_lz.py` | Lionhead LZO1X decoder (1:1 engine port) + `lzo_store()` |
| `tools/save_install.py inspect\|install` | Profile.bin registry install ([Profile index](#profile-index) §4) |
| `tools/save_metadata.py <profileDir> [--json]` | frontend LOAD-list join: autosave-first rows + registered saves, trailer-checked, HEADER decoded; action `0x11` valid / `0xDC` diagnostic ([Profile index](#profile-index) §4.1) |

Every `save_edit.py` write: locates the hero cell by `CTCHeroStats`, edits the inflated cell,
re-deflates, patches descriptor `recLen/clen/ulen` + section len + chunk1_ulen, rebuilds + re-signs
the container (trailer CRC seed 0, exact 300 KB, zero padding). Validation detail:
[Entity graph](#entity-graph) §9.5.3.

### 2. Gate results (all on real bytes, all 5 saves)

Harness: read each save, run each gate as an actual `assert`-style comparison, report per file.

#### GATE A — inflated-stream identity (section walker is lossless)  ✅ PASS 5/5
Walk the inflated payload into sections (`name\0`, `u32 size`, body), re-emit every section header+body
back-to-back plus any residual bytes, and assert the reconstruction is **byte-identical** to the
original inflated payload. This proves the section walker / serializer loses nothing.

    Cornelio/AutoSave      ident=True   payload=90469 B
    Cornelio/AutoSave.qs   ident=True   payload=90469 B
    Cornelio/Manual-Save1  ident=True   payload=90799 B
    Default/AutoSave       ident=True   payload=22011 B
    Test/AutoSave          ident=True   payload=21921 B

(Only `HEADER` is a clean top-level named section; the remainder is the `ENTITIES`/`SAVED_ENTITIES`
nested entity stream. The walker recognises `HEADER` and treats the rest as an opaque residual tail —
identity still holds, and field edits reach into it via tag scan. A full positional entity schema is
out of scope for a generic set-field-by-tag editor — it is supplied by [Entity graph](#entity-graph).)

#### GATE B — pipeline lossless round-trip (read → no edit → write → re-read)  ✅ PASS 5/5
Full container rebuild: re-deflate both chunks, fix all header dwords, append recomputed seed-0
trailer, pad to 0x4B000. Re-read the produced file and assert inflated payload + chunk sizes are
identical to the original, and the file is exactly 307200 bytes. The re-read **also re-verifies the
trailer signature** on our own output (so our writer produces a self-consistent, signature-valid file).

    all: payload_identical=True  chunk_sizes_identical=True  slab=307200=True

#### GATE C — single-field edit  ✅ PASS 5/5
Locate a HEADER `u32` field by its CRC tag, set it to a sentinel (`0xC0FFEE01`), write, re-read, and
assert (a) the re-read field equals the sentinel and (b) the **only** bytes that changed in the whole
inflated payload are exactly that field's 4 value bytes.

    edited HEADER tag 5b0fcaa0 (u32): reread==sentinel, diff bytes == exactly the 4 value bytes, 5/5

#### GATE D — signature  ✅ CONFIRMED 5/5
Assert `stored_trailer == CCRC::Calc(0, file[0:trailer_pos])` — values and formula in
[Container and header layout](#container-and-header-layout) §2.3.

Reproducing the gates:

    python tools/save_edit.py <FableSave-file>     # dump container + sections + verify signature

Full gate harness (kept in scratchpad, reads copies of the real saves — real saves are treated as
read-only; all writes go to scratchpad `out/`): `run_gates.py`. Summary line it prints:

    SUMMARY: A(identity)=True  B(roundtrip)=True  C(edit)=True  D(signature)=True

### 3. Generic edit recipe (any tagged field)

**Verdict: editable.** A `FableSave!` file is a well-understood container — `[FableSave! + header]
→ zlib chunks → CPersistContext binary stream of [Calc(0,name)][value] fields grouped into named
sections`. To edit hero stats / gold / morality / inventory / quest flags:

1. **Read container:** parse `FableSave!` + 16-byte header (`+0x0C` total_data_len, `+0x10`
   chunk0 ulen, `+0x14` chunk1 ulen, `+0x18` chunk0 clen), inflate chunk0 (HEADER) and chunk1
   (ENTITIES/SAVED_ENTITIES/PLAYER/QUESTS/…) with zlib (`78 DA`), concatenate → the persist
   stream. `tools/save_tools/savecrc.py inflate <file>` does this.
2. **Locate a field:** walk `"section\0" [u32 len]` then `[u32 tag][value]`; a field is the one
   whose `tag == Calc(seed=0, name)` (NOT 0xFFFFFFFF). Types per
   [Section grammar](#section-grammar) §3.3–3.5, so values can be typed and rewritten in place
   (ints/floats fixed width; strings NUL-terminated with section-length fixup; vectors
   `[i32 count][elems]`).
3. **Edit common cases:**
   - **World/level:** `WorldName` (HEADER) — string.
   - **Where the "save here" marker is:** `SaveGameMarkerPos` (3 floats) + `SaveGameMarkerAngleXY`.
   - **Current region label:** `CurrentRegionName` / `CurrentRegionMinimapGraphicName` (strings).
   - **Feature toggles:** the bool block (Teleporting/Saving/MiniMap/ExperienceSpending…Enabled).
   - **Which hero:** `PlayerCharacterUID` (PLAYER) points at the hero entity.
   - **Gold / Morality / Age / Fatness / RenownLevel:** `CTCHeroStats` tags inside the hero cell —
     [Hero stats](#hero-stats) §2/§5 (`save_edit.py set-gold/set-stat`).
   - **Inventory:** [Entity graph](#entity-graph) §8.2 / §9.5.2 (`save_edit.py set-qty/add-item`).
   - **Quest completion:** edit the TEXT quest sub-scripts in the QUESTS section (`COMPLETED_QUESTS`
     UID list + `FinishedQuest TRUE;`) — [Entity graph](#entity-graph) §7.1/§8.1 (`flipquest`).
4. **Write back:** re-serialise, **re-deflate each section** (zlib best-compression `78 DA`), fix
   the header dwords (chunk0/chunk1 ulen, chunk0 clen, and the chunk1 compressed-length framing
   dword), for SAVED_ENTITIES cell edits patch descriptor `recLen/clen/ulen` + `sectionLen` +
   `chunk1_ulen`, write `+0x0C = total_data_len`, append the seed-0 trailer, pad to the fixed
   **300 KB (0x4B000)** slab with zeros.

Guard rails: all observed saves are exactly **307200 bytes** — keep the slab + zero padding; the
signature is mandatory (no retail bypass) — [Signature enforcement](#signature-enforcement) §6.
`Profile.bin` edits need their own trailer re-sign — [Profile index](#profile-index) §3.

### 4. Go / No-Go for a save editor

**GO — with one caveat that only a live game-load can close.**

What is **safe now** (proven offline):
- Reading any `FableSave!` file: header split, chunk inflate, section walk, tag decode.
- Editing a field by CRC tag (fixed-width int/float/bool; NUL-terminated string with section-length
  fixup) and producing a **structurally valid, signature-valid, correctly-sized** save.
- The signature is solved and reproduced, so edited saves carry a **correct trailer** — no need to
  disable `UseSaveGameSignatures` (which does not exist in retail anyway), and no risk of a
  signature-mismatch rejection.

What still needs **live game validation**:
- Whether the retail loader accepts a save whose **compressed bytes differ** from its own compressor's
  output. Highly likely yes (it inflates by length), but unconfirmed against the actual binary.
- **Semantic** validity of an edited field value: the gates prove byte-level correctness, not that a
  given HEADER/ENTITIES field accepts an arbitrary value without the game clamping/rejecting it.
- In-game acceptance of the LZO store-mode QUESTS stream and of `add-item` list-front insertion
  ([Entity graph](#entity-graph) §8.1, §9.5.3).

**Bottom line:** the container/signature layer is **done and validated**; a save editor can be built
on `tools/save_edit.py` today. The only unproven link is the game actually loading the result, which
requires running the retail build — recommended as the next confirmation step, but no reversed
signature routine is blocking it (the signature is solved). This makes a standalone TLC save editor
(or a FableForge `forge save` subcommand) straightforward: reuse the game.bin field decoder's CRC
core with **seed 0** and wrap the section/zlib container described here.

### 5. Corrections fed back to the format spec (now applied in this merged doc)

- `+0x0C` is **total_data_len (offset to trailer)**, not a signature/version.
- The signature is the **4-byte** trailer at `trailer_pos` (the first draft guessed "16 trailing
  bytes"); the following 12 bytes are already zero slab padding. Formula **CONFIRMED**:
  seed-0 CRC-32 over `file[0:trailer_pos]`.
- The original open item "signature input + formula" is **CLOSED**; "does the loader enforce it"
  was CLOSED (YES) by the 2026-07-20 decompile.
- Field tags are seed 0 (see [Overview](#overview) §1.3).
- Original consolidation note: merging into FINDINGS.md/HANDOFF.md was intentionally deferred at the
  time to avoid races with the concurrent game.bin agent.

---

## Verified facts

Addresses + the evidence line that pins each. Donor = `FableWin.exe` (PDB names); retail =
`Fable.exe` (ImageBase 0x400000).

| fact | value | evidence |
|---|---|---|
| `.sav` magic / pad | `"FableSave!"` + `00 00`; loader checks 8-byte `FableSav` + u32 `0x2165` | 6 real saves; `VerifySignature` decompile (`decomp_save_sig_load1.log`) |
| `.sav` slab size | exactly 307200 B (0x4B000), zero slack | all saves on disk; `_Cons_val(...,0x4B000)` in `SaveGameStateInternal`; loader equality + per-byte `0x00` check |
| `+0x0C` | `total_data_len` = trailer offset (78321/78321/78693/14084/13986) | 5/5 saves; loader uses it as CRC length + trailer locator |
| `+0x10/+0x14/+0x18` | chunk0 ulen / chunk1 ulen / chunk0 clen | 3-save table §2.1; gate B round-trip |
| chunk1 clen | inter-chunk framing dword after chunk0 | gate A/B 5/5 |
| Trailer signature | u32 `CCRC::Calc(0, file[0:total_data_len])`, no final XOR; a3ba1c9c / a3ba1c9c / 0ddf1f62 / 36dfc709 / b06b17a6 | gate D 5/5; loader recomputes identically |
| Signature enforced on load | YES, unconditionally; first statement of `CWorld::LoadGameStateInternal` | decompile `0x004A21F0`; no global flag read in `0x00409730` |
| `UseSaveGameSignatures` / `UseBinarySaveGames` in retail | ABSENT (0 hits); `UseRetailSaveGameSystem` @ `0x0122E85C` | `FindStrRaw.java` scan, `decomp_save_sig_load3/4.log`; donor strings `0x0402F21C`/`0x0402F208`/`0x0402F2F0` |
| Field-tag hash | reflected CRC-32 poly `0xEDB88320`, seed 0, no inversion; table donor `0x0449BE30` (`00000000 77073096 … 2D02EF8D`) | 23 HEADER + 2 PLAYER + 5 CTCHeroStats + 2 base-field tags match on 4–6 saves; `CCRC::Calc` donor `0x0186D2D0` |
| CheckCRC assert literal | `stream_crc==CCharString(name).GetCRC()` @ `lib_persist_context.hpp:0xD7` | donor `0x018747B0` decompile |
| Persist modes | `*(ctx+0x18)`: 0 text-in, 1 text-out, 2 binary-in, 3 binary-out, 4 copy; text modes emit no tags | `CheckCRC` decompile; Profile.bin is text mode |
| HEADER section | `"HEADER\0"` + u32 len (0xD0 / 0xF4) + 23 tagged fields (table §3.4) | 4 saves, decoder consumes exactly |
| PLAYER section | `PlayerCharacterUID` `96adc827` u64 @ Test 0x29ee / Manual 0x12b27 / AutoQs 0x129d7; `PlayerRegionName` `9a7e84f7` | `CPlayerManager::SaveGameState` donor `0x01805EF8` / retail `0x00445A20`; 3 body streams |
| chunk1 section order | ENTITIES → SAVED_ENTITIES → PLAYER → QUESTS → REGIONS → FACTIONS → text markers | `SaveGameStateInternal` decompile; `"ENTITIES"` @ `0x01238e60` (xref `0x0049faa5`), `"SAVED_ENTITIES"` @ `0x01245d18` |
| SAVED_ENTITIES cell descriptor | `[u64 0][u64 recLen=29+clen][u64 cellId][u64 clen][u32 ulen][zlib][9×00]` | 81/81 cells, 6 saves |
| Cell-edit rule | patch `recLen`/`clen`/`ulen` + `sectionLen` + `chunk1_ulen`, re-sign | `save_edit.replace_cell`, `verify_edit` byte-scope asserts |
| Entity record framing | inst-name / A,B / EngineClass / flagsA,mid,flagsB / u64 UID / 0 / DEF-name / baseFieldLen+fields / 0,componentCount,0 / components / `END\0` | 348 records = 348 `END\0` (Manual-Save1) |
| Hero base fields | `Health` `78b48786` f32 80.0; `Scale` `eba5fd9d` f32 1.0 | CRC name-match + sane values |
| Component framing | `[class\0][u32 0][u8 0][u32 dataLen][tagged fields][u32 0]`; 147 CTC* classes | all cells |
| Hero locator | unique `PlayerCreature\0` → UID → `CREATURE_HERO\0`; unique `CTCHeroStats\0`; hero UIDs `ffffff0000003ae3` / `ffffff00000000c2` | counts 1/1/1; cell idx varies 10/12/0 |
| CTCHeroStats tags | Morality `79a2d479` i32; Money `b03ccbfd` i32; Age `663c8f40` f32; Fatness `b0da59fb` f32; RenownLevel `0c4ed775` i32 | 4 saves (483/1625/26.4/0.0/2 vs 0/0/18.0/0.0/0) |
| CTCHeroStats runtime layout | morality `+0x28`; renown level `+0x70`, renown value `+0x78`; stat-exp array `+0x118` [general,strength,skill,will] | `decomp_hero_stats_classes.c` (`SetMorality`, `GetHeroStatExperience`, `GetPercentageThroughRenownLevel`) |
| Inventory grammar | `[2b837863][u32 itemCount][u8 0]` + `[u32 nameLen][name][u32 qty][u32 unkA][u8 ext]` records | 62=62 / 40=40 / 3(+1 nested) decoded |
| QUESTS/FACTIONS LZ framing | `[u32 ulen][u16 clen][LZO1X][raw tail]`; QUESTS 17,457 B text + 119 B trailer; FACTIONS `[4f0e45c3][count]` + 29 recs; REGIONS 141 recs | 30/30 blobs; 90 store-mode round-trips |
| Quest flip write path | executed on a copy; 307,200 B out, sig re-verified, trailer byte-identical | `flipquest Q_HobbeCave` |
| Profile.bin container | `"FablePro"` + u32 0 + u32 total_data_len + text + u32 seed-0 trailer; no zlib/padding | 4 profiles (94ADE8E3 / 171E6CF1 / 8CEC5F75 / 676D17E2 match) |
| Registry | `SaveGameNames1..50`, non-empty ⇔ listed, string = filename; autosaves not registered | 4 profiles; profile `1` has AutoSave but 0 slots |
| Profile signature enforced | `CUserProfileManager::Load` `0x0040D350` gates on `VerifySignature` | `decomp_save_sig_load2.log` |
| Cached-validity predicates | `0x005957D9` / `0x0047ED0A` cache `{path,bool}` at `this+0xC4`/`+0xE0` | same log |
| zlib chunk writer | `CDataOutputStream::WriteZlibCompressed` retail `0x009967E0` → `CCompressorZlib` | `decomp_zlibchunk.c` |
| Python zlib vs retail | inflates identical; compressed differs +6…+38 B — not required to match | gate B |
| text.big CRC map | `FUN_009cc410` = `CVectorMap<ulong,CDefString>::LowerBound` @ `this+0xD0` | `decomp_savegame_retail.log` |

---

## Open questions

Closed items are kept (struck through in prose) so the history is auditable.

**Container**
- C1. ~~Signature / `+0x0C` dword — exact hash input + whether the retail loader enforces it.~~
  CLOSED: `+0x0C` = total_data_len; trailer = seed-0 CRC over `file[0:total_data_len]`;
  ENFORCED unconditionally (2026-07-19 / 2026-07-20).
- C2. ~~Full HEADER field roster.~~ CLOSED: 23/23 named + typed (2026-07-19).
- C3. **`.qs.hs` companion blob** — structure of the "hero state" side file paired with `.qs`.
- C4. **Sub-section list** — enumerate every `StartSection` name emitted (HEADER + ENTITIES +
  the sub-sections seen in the writer) for a complete section map (PLAYER/QUESTS/REGIONS/
  FACTIONS confirmed; text markers not enumerated).
- C5. `TotalTimePlayed` written from a `double` but occupies 4 bytes (0.0 in samples) — where
  the true play time lives.
- C6. `ESaveType` enum values (1/3 and 2/3 seen) not named.
- C7. Live confirmation that the retail loader accepts Python-zlib compressed chunks (differs
  from `CCompressorZlib` output by +6…+38 B) — highly likely yes.

**Entity graph** (former SAVE_ENTITY_GRAPH §10)
- E1. First-u32-of-cell semantics (1/2/4 observed) — not the walker-recovered entity count;
  [hypothesis: sub-group count]. Walker does not depend on it.
- E2. QUESTS-trailer, FACTIONS head/recTag, and inventory component-tail field names
  (`a7d70546`, `a10e3cad`, `92e50fdb`, `c93f54bc`, `691625d6`, `1f5c5c94`, `f85a6bb9`) —
  unrecovered (2-strike wordlist brutes done); needs `CQuestManager::SaveGameState` /
  `CFactionManager` / `CTCInventory::Transfer` decompiles (deferred — no-Ghidra pass).
- E3. Entity base-field tag→name map: hero's two base fields recovered (`Health` 78b48786,
  `Scale` eba5fd9d); other classes' base fields not yet brute-forced. Inventory record extension
  payload internals (money-bag state 15×00 + `01 00 00 00` + 00, augment sub-record grammar) and
  the `[u8]` between itemCount and the first record. `unkA` [hypothesis: per-item usage counter].
  REGIONS 19-byte flag block per-byte meaning. Trailing SAVED_ENTITIES index-table semantics.
- E4. Child-entity parent↔child containment tree — records (incl. children) are fully
  enumerated via the UID/DEF anchor and cross-checked by `END\0` counts, but the explicit
  nesting relation (which child belongs to which parent) is not yet emitted by the walker.
- E5. Cornelio/AutoSave.qs: 348 anchor records vs 347 `END\0` markers (1-record delta on this
  one save; all others exact).
- E6. Quick-access slot order in the `CTCInventory` trailing name table [hypothesis].

**Hero stats**
- H1. Experience-pool tag names (`896e293a`, `c731e25b`, `695d2a96`, `37c59c98`, `059107c4`) —
  position/type confirmed, mapping to general/strength/skill/will/renown by name is the
  remaining [hypothesis].
- H2. Semantic clamping of edited values (e.g. `SetMorality` clamps to `±*(def+0xd0)` at
  runtime; qty > stack-max behaviour).

**Live QA (all offline-proven, none yet loaded in-game)**
- L1. Load an edited, re-signed `.sav` in retail; confirm gold/stat/item appears; confirm
  (a) list-front `add-item` insertion order is accepted, (b) a general-list add of a weapon def is
  enough for it to appear (vs needing a `CTCInventoryWeapons` record), (c) qty>stack-max.
- L2. Load a `flipquest` save (LZO store-mode stream) in retail.
- L3. `Profile.bin` registry install: confirm the added `SaveGameNamesN` slot appears and loads,
  and that the game does not rewrite `Profile.bin` on entering the LOAD menu in a way that drops
  an externally-added slot (staged in `work/save_install/`).
- L4. `Profile.bin` `dw08` (@0x08) is 0 in all 4 profiles — assumed reserved.
- L5. `save_metadata.py` row data still needs to be consumed by the native C++ runtime before the
  live game-load ownership boundary is connected.
