# Fable: The Lost Chapters — Runtime Save-Game / Persist-Stream Format (.sav)

**Status:** CRACKED (code-derived + validated against real save files on disk).
**Date:** 2026-07-19.

> **★ CORRECTION (2026-07-19, later): save-stream field tags are seed 0, NOT 0xFFFFFFFF.**
> This doc originally claimed the per-field tag = `CCharString::GetCRC()` = `CCRC::Calc(0xFFFFFFFF, …)`.
> That is EMPIRICALLY DISPROVEN by real save bytes — confirmed on THREE independent tags across
> multiple saves: `WorldName`=`a8de4f22`, `Money`(gold)=`b03ccbfd`, `Morality`=`79a2d479`, each
> equal to `CCRC::Calc(0, name)`; the seed-0xFFFFFFFF values do NOT appear on disk. So the
> save-stream field-tag hash is the SAME seed-0 hash as game.bin (see FINDINGS.md "★ CANONICAL CRC
> REFERENCE"). **Every "seed 0xFFFFFFFF" for a field tag below is superseded — use seed 0.** The
> `0xFFFFFFFF` in the `GetCRC`/`CheckCRC` decompile below is a mechanism footnote awaiting a fresh
> `GetCRC` re-decompile (two readings conflict), but it does not change the actionable fact.
> **Tooling bug:** `tools/save_edit.py:get_crc` defaults to seed 0xFFFFFFFF — for field/stat tags
> call it with seed 0 (or `savecrc.getcrc(name, 0)`); the trailer-signature helper (seed 0) is fine.

**Scope:** the *runtime persist STREAM* written by `CPersistContext` in binary mode — one half of
the CRC finding, kept separate from the offline-compiled **game.bin** definition database (see the
"★ CANONICAL CRC REFERENCE" block in `FINDINGS.md`).

| | game.bin (def DB) | Save game (.sav runtime stream — THIS doc) |
|---|---|---|
| Producer | offline def compiler | `CWorld::SaveGameStateInternal` at runtime |
| Per-field tag hash | `CCRC::Calc(0, name, len)` (seed 0) | `CCRC::Calc(0, name, len)` (seed 0 — SAME hash; see CORRECTION) |
| Compression | (def-specific) | **zlib**, per-section chunks |
| Container | `[u32 count][elems]` | `[i32 count][elems]` (identical shape) |

Both use the SAME reflected CRC-32 with **seed 0**. The earlier "only the seed differs" framing was
itself the error — there is no seed difference.

---

## 1. Evidence base (≥2 sources per claim)

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
    `LoadEntitiesBinary` (`0x00521240`).
  - `ghidra_out/decomp_zlibchunk.c` — `CDataOutputStream::WriteZlibCompressed` (`0x009967e0`).

**Real save files (ground truth, parsed byte-exact):**
`%USERPROFILE%\Documents\My Games\Fable\Saves\<Hero>\`:
`AutoSave`, `AutoSave.qs`, `AutoSave.qs.hs`, `Manual - Save1`, `Profile.bin`.
Three parsed here: `Test/AutoSave`, `Cornelio/Manual - Save1`, `Cornelio/AutoSave.qs`.
Every one begins with the `FableSave!` magic, has the header dwords, and inflates to a
`HEADER\0` + `ENTITIES` persist stream whose first field tag matches the code path.

No community/wiki byte-level save spec exists (web search: only save-file *download* sites,
no format doc; fabletlcmod.com wiki cert-fails over TLS). So this spec is novel and rests on
**decompiled writer + real-file parse** as its two independent sources.

---

## 2. Where saves live

`%USERPROFILE%\Documents\My Games\Fable\Saves\<HeroName>\`
- `AutoSave`            — main autosave (binary `FableSave!`)
- `AutoSave.qs`         — quest-start save (binary `FableSave!`)
- `AutoSave.qs.hs`      — companion "hero state" blob to the quest save
- `Manual - Save1` …    — manual saves (binary `FableSave!`)
- `Profile.bin`         — per-hero profile/options (**different format** — see §7)

All observed save files are exactly **307200 bytes** (0x4B000). The writer preallocates this
(`_Cons_val(..., 0x4B000)` in `SaveGameStateInternal`): the file is a **fixed 300 KB slab**;
the real payload is the compressed prefix, the rest is slack. `<HeroName>` is the profile
name; `Default`/`Test`/`Cornelio` seen on disk. Retail path helpers:
`GetAutoSavePathName`, `GetAutoSaveFileName`, `GetAutoSaveCheckPointFileName`,
`GetAutoSaveDisplayName` (all reference the `"AutoSave"` string at retail `0x0122de9c`).

---

## 3. On-disk file layout (`FableSave!` binary saves)

### 3.1 File header (16 bytes after magic)

```
off  size  field
0x00 10    magic         "FableSave!"           (ASCII, no NUL)
0x0A  2    pad           00 00
0x0C  4    u32  signature/version               [hypothesis: save signature — see below]
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

The `+0x0C` dword differs per save and per playthrough length; it is not any of the sizes.
Given the config flag **`UseSaveGameSignatures`** (retail string `0x0402f21c` / donor
`0x0402f21c`), this is almost certainly the **save signature** (a CRC/hash over the payload),
used to detect tampering/corruption. [hypothesis — exact hash input not yet confirmed; the
16 trailing framing bytes after the last chunk, e.g. `a6 17 6b b0 …`, are the likely
signature material.] Two more save-system config flags exist:
**`UseBinarySaveGames`** (`0x0402f208`) selects this binary format over the text format, and
**`UseRetailSaveGameSystem`** (`0x0402f2f0`). Retail ships with binary + retail system ON.

### 3.2 Chunk chain (zlib)

Each section is compressed independently by
`CDataOutputStream::WriteZlibCompressed` (retail `0x009967e0`), which drives
**`CCompressorZlib`** (`SetStreamInput` → loop `CompressStream` → `FinishCompressStream`)
and emits the raw zlib stream (RFC-1950, `78 DA` = deflate/best-compression). Framing:

```
chunk0:  [zlib blob, length = header.chunk0_compressed_len]       (@ file 0x1C)
         [u32 chunk1_compressed_len]                              (framing dword)
chunk1:  [zlib blob]                                              (the big payload)
         [16 bytes trailer]  <- signature material [hypothesis]
         [zero padding to 300 KB]
```

Chunk0's uncompressed length lives in the file header (+0x10); chunk1's uncompressed length
is in the header too (+0x14); chunk1's *compressed* length is the framing dword written right
after chunk0. Inflating both and concatenating gives the full `CPersistContext` binary stream.
(A save may have >2 sections; `SaveGameStateInternal` calls `StartSection`/`EndSection`
several times — HEADER, ENTITIES, plus sub-sections — but on disk they are grouped into these
zlib chunks.)

---

## 4. The inflated persist stream (`CPersistContext` binary mode)

Reconstructed from the decompiled `CWorld::SaveGameStateInternal` (retail `0x0049f4c0`) and
confirmed byte-for-byte against the inflated real saves.

### 4.1 Sections

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

### 4.2 Per-field encoding: `[CRC tag][value]`

Every `CPersistContext::Transfer<T>(name, value)` call first invokes
`CheckCRC(name)` (donor `0x018747b0`), then serialises the value. `CheckCRC`:

```c
// mode = *(ctx+0x18):  0=text-in 1=text-out 2=binary-in 3=binary-out 4=copy
if (mode == 3) {                                   // binary OUT (save)
    ULONG tag = CCharString(name).GetCRC();        // seed 0xFFFFFFFF reflected CRC-32
    stream.WriteULONG(tag);                         // 4 bytes, little-endian
} else if (mode == 2) {                            // binary IN (load)
    ULONG got = stream.ReadULONG();
    assert( got == CCharString(name).GetCRC() );    // "stream_crc==CCharString(name).GetCRC()"
}
// text modes 0/1 do NOTHING here (no CRC in text saves).
```

So each field in a binary save is:

```
[u32  GetCRC(fieldName), little-endian]   [value bytes]
```

The tag is `GetCRC` = **seed `0xFFFFFFFF`** — the persist variant. (game.bin uses the same
table but **seed 0**; do not conflate.) The assert literal in the binary is exactly
`stream_crc==CCharString(name).GetCRC()`
(source `…\bbblibrary\lib_persist_context.hpp:0xD7`).

### 4.3 Value encodings by type

From the `Transfer<T>` binary branches (`case 2` = in, `case 3` = out) which delegate to
`_CPersistTraits<T>::TransferBinaryIn/Out`, plus the stream primitives:

| type (Transfer<T>) | binary value bytes |
|---|---|
| `long` / `signed char`→padded / `uint32` (`K`) | 4-byte LE (WriteULONG/ReadULONG → 4 bytes) |
| `bool` (`_N`) | 1 byte (traits write the raw byte) |
| `float` (`M`) | 4-byte IEEE-754 LE |
| `CCharString` | **raw chars, NUL-terminated, no length prefix** — proven by the on-disk `"FinalAlbion\0"` value and by `WriteString` (writes `GetLength()` chars via the stream). Empty string = `00`. |
| `Vector<T>` (`TransferVector` / `GFSerialiseVectorBinaryOut`) | `[i32 count]` then `count ×` `TransferBinaryOut<T>(element)` (no per-element tag). `count` written by `WriteSLONG` (signed 4-byte). |
| `CIntelligentPointer<CThing>` (object refs — heavily used in HEADER) | 4-byte handle/UID (serialised as a long) |

This mirrors the game.bin value table exactly (same widths, same NUL-terminated strings, same
`[count][elems]` vectors) — the only difference is the **tag seed** and the **zlib wrapping**.

Field/serialisation ORDER == the `Transfer()` call order inside `SaveGameStateInternal`
(and each section's writer), not any offset order.

---

## 5. Loader / writer entry points (addresses)

| role | symbol | donor (FableWin) | retail (Fable.exe) |
|---|---|---|---|
| Save writer (top level) | `CWorld::SaveGameStateInternal(CWideString,bool)` | — | **`0x0049F4C0`** (HEADER write @0x0049F5A2, ENTITIES @0x0049FAA5) |
| Entity loader (binary) | `LoadEntitiesBinary` | — | **`0x00521240`** (uses "Loading entities from script", `0x00521488`) |
| Entity loaders (text/script) | `LoadEntitiesFromScript` | — | `0x00520F44` / `0x00521106` |
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
| Save-path helpers | `GetAutoSave*PathName/FileName` | — | `0x00406695`, `0x00406695+`, `0x00406FC5`, `0x004066F2`, `0x00408*` |

Config console vars (registered in `NGlobalConsole::Initialise` / `GFRunInitScripts`):
`UseBinarySaveGames` (`0x0402F208`), `UseSaveGameSignatures` (`0x0402F21C`),
`UseRetailSaveGameSystem` (`0x0402F2F0`), plus commands `SaveGameState`/`LoadGameState`.
(Donor VAs shown for the config strings; retail `UseRetailSaveGameSystem` at `0x0122E85C`.)

---

## 6. CRC primitive — exact table

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

**`CCharString::GetCRC()`** — donor `0x0186D2A0` — is exactly
`CCRC::Calc(0xFFFFFFFF, GetAsCharArray(), GetByteLength())`.
`GetByteLength` == string length **excluding** the NUL, so the hash is over the raw ASCII
name as-is (no terminator). **[CORRECTION: this is NOT what stamps the on-disk save field tags —
those are empirically seed 0 (WorldName/Money/Morality verified). Either GetCRC isn't the tag
source, or the `0xFFFFFFFF` seed reading here is a decompile misorder; a fresh GetCRC decompile is
needed. Use seed 0 for all on-disk tags.]**

**Seed table (CORRECTED — canonical reference is FINDINGS.md "★ CANONICAL CRC REFERENCE"):**
| caller | seed | used by |
|---|---|---|
| all on-disk name→tag hashes | `0x00000000` | save-stream field tags (this doc) + game.bin def tags + text.big symbols + save trailer = ONE seed-0 hash |
| `CCharString::GetCRC()` (mechanism TBD) | contested | the `0xFFFFFFFF` in its decompile is disputed; on-disk save tags are seed 0 regardless — re-decompile pending |
| `NGameText::CDataBank::GetTextBySymbol` | `0x00000000` | text.big / BIGB symbol→ID |

**Bonus — `FUN_009cc410` (retail, text.big CRC→entry-index map):** decompiled — it is
`CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<…>>::LowerBound` at
`this+0xD0`, i.e. a **binary search of a sorted (u32-key → CDefString) vector**. It takes a
u32 key (a seed-0 CRC of a text symbol, per the canonical reference), binary-searches the
sorted key/value pairs, and returns the paired entry index (`puVar1[1]`) or 0 on miss. This
closes the text.big symbol-resolution detail: `GetTextBySymbol` computes the seed-0 CRC of the
symbol name, then this map turns that CRC into the string-bank entry index. (Full decompile in
`ghidra_out/decomp_savegame_retail.log`.)

---

## 7. Profile.bin — the TEXT persist format (mode 0/1)

`Profile.bin` uses a **different magic** and the **text** persist mode, not the binary one:

```
off  data
0x00 "FablePro"                         (8-byte magic; string at donor 0x0401BD3C etc.)
0x08 00 00 00 00
0x0C u32                                (size/version, e.g. 0x000046A4)
0x10 ...  key/value text lines, e.g.:
     ProfileName "Cornelio";\r\n
     SubtitlingEnabled TRUE;\r\n
     VibrationEnabled TRUE;\r\n
     CameraXYInverted FALSE;\r\n
     CameraYZInverted FALSE;\r\n
     ...
```

This is `CPersistContext` in **text mode** (mode 0/1): `TransferObjectSaveText` /
`TransferObjectLoadText`, which write `Name value;` lines and — per `CheckCRC` — emit **no CRC
tags** (text modes return early from `CheckCRC`). Human-readable and trivially editable. Good
cross-check that the CRC-tagged stream is exclusive to the *binary* modes.

---

## 8. Editability verdict

**Verdict: editable, with two guard rails.** A `FableSave!` file is a well-understood
container — `[FableSave! + header] → zlib chunks → CPersistContext binary stream of
[GetCRC(name)][value] fields grouped into named sections`. To edit hero stats / gold /
morality / inventory / quest flags:

1. **Read:** parse the 16-byte header, inflate chunk0 (HEADER) and chunk1 (main/ENTITIES) via
   zlib, concatenate → the persist stream.
2. **Locate a field:** walk sections (`name\0`, `u32 size`, then `[tag][value]*`); a field is
   found by `tag == CCRC::Calc(0, fieldName)` (seed 0 — see top CORRECTION; NOT 0xFFFFFFFF). Field types are known (§4.3), so
   values can be typed and rewritten in place (ints/floats fixed width; strings NUL-terminated;
   vectors `[i32 count][elems]`).
3. **Write back:** re-serialise, **re-deflate** each section (zlib `78 DA`), **fix the header
   dwords** (chunk0/chunk1 uncompressed + chunk0 compressed lengths, and the chunk1 compressed
   framing dword), and pad the file back to 300 KB.

**The one real obstacle is the signature.** The `+0x0C` dword (and the 16-byte trailer) is
governed by `UseSaveGameSignatures`. If the retail build validates it on load, an edited save
needs the signature recomputed. Mitigations (in priority order):
- **Recompute it** — it is built from the same `CCRC`/`CCharString::GetCRC` family already
  fully reversed here; deriving the exact input (likely the raw payload or the two chunk
  buffers) is a small follow-up (decompile the caller of `SaveGameStateInternal` that fills
  `+0x0C`, and the load-time verifier). [open item]
- **Disable the check** — flip `UseSaveGameSignatures` to false via console/config, if the
  retail loader honours it. [needs live confirmation]

Everything except the signature-recompute detail is fully specified and validated. This makes
a standalone TLC save editor (or a FableForge `forge save` subcommand) straightforward:
reuse the game.bin field decoder's CRC core with **seed 0xFFFFFFFF** and wrap the section/zlib
container described here.

---

## 9. Open items

1. **Signature/`+0x0C` dword** — exact hash input + whether the retail loader enforces it.
   Decompile the `SaveGameStateInternal` caller and the load-time verifier.
2. **Full HEADER field roster** — brute-force the observed HEADER tags against a hero/world
   field wordlist via `GetCRC` (first tag `a8de4f22` → the world-name field preceding the
   `"FinalAlbion"` string value; not yet name-matched). The tag→name map for hero stats/gold/
   morality/quest flags is the last mile for a turnkey editor; walk `SaveGameStateInternal`'s
   `Transfer<>` call sequence to recover names in order.
3. **`.qs.hs` companion blob** — structure of the "hero state" side file paired with `.qs`.
4. **Sub-section list** — enumerate every `StartSection` name emitted (HEADER + ENTITIES +
   the sub-sections seen in the writer) for a complete section map.

*(Consolidation into FINDINGS.md/HANDOFF.md intentionally deferred to avoid races with the
concurrent game.bin agent — see run constraints.)*
