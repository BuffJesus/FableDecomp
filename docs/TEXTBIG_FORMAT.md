# text.big / dialogue.big — BIGB localization bank format (Task #14)

*Authored 2026-07-19. Status: on-disk format fully RE'd and VALIDATED against the real retail
`text.big` (28,913 entries decode into coherent English). Engine-loader confirm in Ghidra: see
"Loader confirmation" section (attempted; state recorded there).*

## Summary

`text.big` (and its siblings `dialogue.big`, `frontend.big`, `graphics.big`, `textures.big`,
`fonts.big`, `shaders.big`, `effects.big`) are all the **same container format**: a Lionhead
"**BIGB**" bank archive. The archive holds one or more named **sub-banks**; each sub-bank has a
flat **table of contents** (entry directory) and the entry *payload blobs* live at the front of the
file. For `text.big`, each payload is a **localized string record** (UTF-16LE content + metadata).

- **Magic:** ASCII `BIGB` at offset 0.
- **String content encoding:** **UTF-16 little-endian**, NUL-terminated (`0x0000`).
- **Metadata strings** (identifier / speaker / speech-bank / entry name): **length-prefixed
  8-bit** (`u32 length` then raw bytes; treat as ASCII/Latin-1).
- **Numbers:** all little-endian.
- **Per-string ID:** a `u32` numeric ID *and* a human-readable name string
  (e.g. `TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC`), both stored in the TOC entry. There is **no
  hash** on the string itself — lookup is by the numeric ID (see dialogue-sound cross-reference).

## Evidence / sources

1. **Raw bytes** of the retail file
   `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\text.big`
   (6,652,561 bytes). Header hexdump and a full walking parse validate every field below.
2. **EgoCore** (completed community RE, MIT-licensed) loader source — the authoritative second
   source. Container reader: `EgoCore\Banks\BankLoader.h` (`CreateBankFromDisk`,
   `LoadSubBankEntries`). Entry record struct: `EgoCore\Banks\BankBackend.h` (`BankEntry`,
   `ReadBankString`). Per-entry string decode/recompile: `EgoCore\Text\TextParser.h`
   (`CTextParser::Parse` / `Recompile`). Path:
   `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\EgoCore\`.
3. **Validating parser** (independent reimplementation from the two sources above):
   scratchpad `parse_textbig.py` — decodes 28,913 entries; sample output archived in FINDINGS.md.

Both sources agree on every field. Where a field's *purpose* is inferred rather than proven, it is
labeled **[hypothesis]**.

---

## 1. File header (16 bytes, offset 0)

| Off | Type | Field | text.big value | Meaning |
|----:|------|-------|----------------|---------|
| 0x00 | char[4] | `Magic` | `"BIGB"` | container magic |
| 0x04 | u32 | `Version` | `100` (0x64) | container/file version |
| 0x08 | u32 | `FooterOffset` | `0x658267` | file offset of the sub-bank directory |
| 0x0C | u32 | `FooterSize` | `0x2A` (42) | byte size of the sub-bank directory |

The **entry payload blobs begin immediately after the header, at offset 0x10**, and run up to
`FooterOffset`. (Verified: the first TOC entry's `Offset` field = 0x10, and the bytes there are the
UTF-16LE for "If you get three hits…".)

## 2. Sub-bank directory ("footer", at `FooterOffset`)

| Off | Type | Field | Meaning |
|----:|------|-------|---------|
| +0x00 | u32 | `SubBankCount` | number of sub-banks (text.big = 1) |

Then `SubBankCount` records, each:

| Type | Field | text.big value | Meaning |
|------|-------|----------------|---------|
| char[] + `\0` | `Name` | `"TEXT_ENGLISH_MAIN"` | NUL-terminated sub-bank name |
| u32 | `Version` | `106` | sub-bank format version |
| u32 | `EntryCount` | `28913` | number of TOC entries in this sub-bank |
| u32 | `Offset` | `0x407541` | file offset of this sub-bank's TOC |
| u32 | `Size` | `0x250D26` | byte size of the TOC region |
| u32 | `Align` | `1` | payload alignment |

The sub-bank `Name` prefix selects the bank kind in the engine tooling: `TEXT_*`→text,
`LIPSYNC_*`→dialogue, `MBANK_*`/`GBANK_*`→mesh/texture graphics, `*FONT*`, `*SHADER*`,
`PARTICLE_*` (see `ResolveBankType` in `BankBackend.h`).

## 3. Sub-bank TOC (at sub-bank `Offset`)

### 3a. Stats header

The TOC opens with a small type-count table:

| Type | Field | text.big value | Meaning |
|------|-------|----------------|---------|
| u32 | `StatsCount` | `3` | number of (type,count) pairs |
| u32 × (StatsCount×2) | pairs | `(0,26807)(1,2105)(2,1)` | per-entry-type counts |

For text.big: **26,807 type-0 strings + 2,105 type-1 groups + 1 type-2 narrator list = 28,913**,
matching `EntryCount` exactly. (EgoCore reads this as `statsCount` then `statsCount*2` u32s when
`statsCount < 1000`; otherwise it rewinds and assumes no stats header —
`LoadSubBankEntries` in `BankLoader.h`.)

### 3b. Entry records (`EntryCount` of them, back-to-back)

Each TOC entry is variable length:

| Type | Field | Meaning |
|------|-------|---------|
| u32 | `Magic` | always **42** (0x2A). Entry sentinel; EgoCore rejects records where magic≠42 unless `Size>0 && ID>0`. |
| u32 | `ID` | numeric entry ID — the runtime lookup key |
| i32 | `Type` | **0 = string, 1 = group, 2 = narrator list** (see §4) |
| u32 | `Size` | byte length of the payload blob |
| u32 | `Offset` | file-absolute offset of the payload blob |
| u32 | `CRC` | payload checksum **[hypothesis: CRC of the blob; not verified]** |
| u32 `len` + bytes | `Name` | length-prefixed entry name (e.g. `TEXT_QST_028_…`). Length includes any trailing NUL, which is stripped. |
| u32 | `Timestamp` | build timestamp **[hypothesis]** |
| u32 | `DepCount` | number of dependency strings |
| `DepCount` × (u32 len + bytes) | `Dependencies` | length-prefixed dependency name strings |
| u32 | `InfoSize` | size of an optional per-entry subheader blob |
| `InfoSize` bytes | `Info` | subheader (0 bytes for text; used by texture/mesh/anim banks) |

The TOC ends exactly at `FooterOffset` (verified: TOC walk terminated at 0x658267).

---

## 4. Payload blob layouts (by entry `Type`)

Payload starts at the entry's `Offset` and is `Size` bytes long. From `CTextParser::Parse`
(`TextParser.h`), verified byte-for-byte against real entries.

### Type 0 — String entry (the common case; 26,807 in text.big)

| Order | Type | Field |
|-------|------|-------|
| 1 | UTF-16LE, `\0\0`-terminated | `Content` — the localized display string |
| 2 | u32 `len` + bytes | `SpeechBank` — e.g. `ScriptDialogue.lug` (voice-over bank), or empty |
| 3 | u32 `len` + bytes | `Speaker` — e.g. `FARMER`, `NONE`, or empty |
| 4 | u32 `len` + bytes | `Identifier` — usually equals the entry `Name` |
| 5 | u32 | `TagCount` |
| 6 | `TagCount` × { i32 `Position`; NUL-term ASCII `Name` } | `Tags` — inline conversation directives |

**Tag names** carry conversation control (verified against real data + `TextProperties.h`):
- `ANIM:<name>` — play an animation (e.g. `ANIM:SCRIPT_CHEER_1`).
- `CAM:(prot,pos)(prot,focus)(zoom)` — camera directive.
- `CONVERSATION_ATTITUDE_*` — speaker mood (`EConversationAttitude` enum).
- other = custom/manual tag.
`Position` is the character index into `Content` at which the tag fires.

### Type 1 — Group entry (2,105 in text.big)

A group bundles several string IDs (e.g. random banter variants):

| Order | Type | Field |
|-------|------|-------|
| 1 | u32 | `Count` |
| 2 | `Count` × u32 | member entry `ID`s |

(Verified: group `TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND` → count=4, member IDs
`[16879, 16872, 16761, 16858]`, each resolvable as a type-0 entry in the same bank.)

### Type 2 — Narrator list (1 in text.big)

The narrator/speaker-name list. **CORRECTED 2026-07-20 (write-path work):** its TOC `Size` is 0
and there is **no payload blob** — the narrator data lives in the TOC record's **`Info` blob**
(5,396 bytes in retail):

| Order | Type | Field |
|-------|------|-------|
| 1 | u32 | `Count` (= 379 in retail) |
| 2 | `Count` × NUL-term ASCII | narrator/speaker name strings |

The earlier "scan the payload region for the `[NarratorList]` signature" note was a misread: the
signature found in the file is the TOC record's own length-prefixed *Name* string (`[NarratorList]`,
record @ 0x656d1d, entry ID 28913). Its TOC `Offset` field equals the sub-bank TOC offset (the
retail compiler wrote the current end-of-payload file position for the empty blob).

**Type-0 `Info` blob (4 bytes) = u32 index of the entry's `Speaker` into this narrator list**
(verified 26,807/26,807: `NONE`=0, empty=3, `FARMER`=4, …). See `docs/TEXT_BIG_WRITE.md`.

---

## 5. Cross-references (how strings are used)

- **Runtime text lookup:** by numeric `ID` (the TOC `ID` field). The human-readable `Name` /
  `Identifier` is the authoring key; game code/scripts reference the compiled numeric index.
- **Voice-over link:** `SpeechBank` (`*.lug`/`*.lut`) + the sound ID resolved from the
  `Identifier` via `Data\Defs\*snds.bin` header defs (EgoCore `ResolveAudioID`,
  `gamesnds.bin`/`dialoguesnds.bin`/`scriptdialoguesnds.bin`). `dialogue.big` holds the matching
  lip-sync/phoneme data (`LIPSYNC_*` sub-bank, EgoCore `LipSyncParser`).
- **`.lut` companions** in `data\lang\English\` (`Dialogue.lut`, `ScriptDialogue.lut`, …) are the
  actual audio banks referenced by `SpeechBank`.

## 6. Read/edit path for FableForge

1. Parse header → footer → sub-bank dir (§1–2).
2. For each sub-bank, read stats header + `EntryCount` TOC entries (§3).
3. For each type-0 entry, seek to `Offset`, read `Size` bytes, decode §4 type-0.
4. Edit: mutate `Content`/`Speaker`/`SpeechBank`/`Identifier`/`Tags`, re-serialize the blob
   (UTF-16LE content + length-prefixed metadata + tags — mirror of `CTextParser::Recompile`),
   then **rebuild the bank**: rewrite payload region + fix each entry's `Offset`/`Size` in the TOC
   + fix `FooterOffset`/`FooterSize`. (EgoCore does this in `SaveBigBank`; **verify byte-safe
   round-trip on an unmodified file before shipping a writer**, same discipline as `game.bin`.)

### Encoding note for a native reader
`Content` is UTF-16LE and may contain non-ASCII (accented chars in other languages). Metadata
strings are single-byte (ASCII for English). The tag `Name` and narrator strings are NUL-terminated
single-byte; all other strings in the TOC are `u32`-length-prefixed single-byte.

## 7. Open / unverified items

- `CRC` (TOC +0x14) and `Timestamp` fields are **[hypothesis]** — plausible from names/positions in
  EgoCore's struct; not independently validated by recomputation. Not needed for reading.
  **Update 2026-07-20:** both (and `DepCount`) are **0 on all 28,913 retail text.big entries**,
  so the writer emits zeros for new records.
- ~~Byte-exact **write** round-trip not yet proven for `.big`~~ **PROVEN 2026-07-20**:
  `tools/text_build.py` no-edit rebuild of retail text.big is byte-identical; edit/add legs keep
  all other entries byte-identical. Recipe + evidence: `docs/TEXT_BIG_WRITE.md`.
- Loader confirmation inside retail `Fable.exe` — see next section.

## 8. Loader confirmation (retail Fable.exe, Ghidra) — DONE (third source)

Confirmed against the retail engine. The text.big reader is the **`NGameText::CDataBank`** class,
a subclass of the generic BIGB bank reader **`CBankFile`**. Key retail addresses / evidence:

- **`.big` file family** as string literals, all sharing the BIGB container (log
  `ghidra_out/decomp_textbig_bankfile.log`): `text.big` @ `0x0122da24` (3 refs), plus
  `graphics.big`, `textures.big`, `dialogue.big`, `fonts.big`, `shaders.big`, `effects.big`,
  `temp.big`. Confirms the container is one shared format across all these files.
- **Sub-bank name selector** `NLocalisation::GetLanguage` @ `0x00415070` returns
  `"TEXT_ENGLISH_MAIN"` and per-language codes; the localized variants
  `TEXT_{FRENCH,ITALIAN,SPANISH,CHINESE,KOREAN,GERMAN,JAPANESE,ENGLISH}_MAIN` are all present
  (0x0122e8e8..0x0122e974). This is exactly our footer sub-bank `Name` field.
- **Entry type dispatch** `CBankFile::GetEntryDataType(id)` returns the per-entry `Type`.
  `NGameText::CDataBank::GetTextInfo(id)` @ `0x009c7d00` requires `Type==0` (**string**) then
  indexes `*(this+0x1e0)[id*4]` → `CDataBankTextEntry*`. Directly confirms **Type 0 = string** and
  **numeric-ID lookup**.
- **Group semantics** `NGameText::CDataBank::GetTextEntryFromGroup` @ `0x009c9280` requires
  `Type==1` (**group**), iterates the group's member-ID vector, resolves each via `GetTextInfo`,
  and does a **weighted random pick** (engine RNG `state*0x24a1+0x24df`, `GFROR13`). Confirms
  **Type 1 = group** and its random-variant purpose.
- **Symbol → ID resolution** `NGameText::CDataBank::GetTextBySymbol(CCharString)` @ `0x009c95e0`:
  `id = FUN_009cc410( CCRC::Calc(0, nameChars, nameLen) )` — the human-readable symbol name is
  hashed with **`CCRC::Calc` seeded 0** (note: this is the SAME seed-0 reflected CRC-32 the
  `game.bin` field-tag compiler uses; only `CCharString::GetCRC` — the save-stream variant — seeds
  `0xFFFFFFFF`. See FINDINGS.md "★ CANONICAL CRC REFERENCE"), then mapped to the numeric entry index, then `GetEntryDataType==0` is
  required, and the entry's **`CWideString`** (UTF-16 content) is returned. So the on-disk `Name`
  field is the authoring key; runtime symbolic lookups CRC it. **[Note]** `FUN_009cc410` is the
  CRC→entry-index map (not yet decompiled in detail); `CCRC::Calc` table/polynomial is the same
  open item tracked for #12/#13.
- `NGameText::CTextEntry` runtime object (ctor @ `0x009c8250`) holds 4 `CCharString`s
  (Identifier/Speaker/SpeechBank + one more) + the `CDataBankTextEntry*`, matching the type-0
  payload's metadata strings.

Logs: `ghidra_out/decomp_localisation.c` (148 fns incl. all of the above),
`ghidra_out/decomp_textbig_loader.c` (108 fns incl. `GetLanguage`/`GetFontBankName`),
`ghidra_out/decomp_textbig_strxref.log`, `ghidra_out/decomp_textbig_bankfile.log`.

**Three independent sources now agree** on the format: raw retail bytes, the EgoCore completed-RE
loader, and the retail `Fable.exe` engine loader.
