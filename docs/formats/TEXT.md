# text.big — BIGB localization bank: container, payload grammar, engine loader, writer

*Merged reference (2026-09-07) of `TEXTBIG_FORMAT.md` (authored 2026-07-19, Task #14; loader
confirmation + 2026-07-20 corrections) and `TEXT_BIG_WRITE.md` (2026-07-20). The generic BIGB
container / writer is also covered in [BIG.md](BIG.md); the voice/lipsync join that text entries
participate in is in [AUDIO.md](AUDIO.md) (snds.bin) and [ANIM.md](ANIM.md) (LIPSYNC).*

**Status: on-disk format fully RE'd and VALIDATED** against the real retail `text.big` (28,913
entries decode into coherent English), **confirmed by three independent sources** (raw retail
bytes, the EgoCore completed-RE loader, the retail `Fable.exe` engine loader in Ghidra), and
**byte-exact WRITE proven** (`tools/text_build.py` no-edit rebuild is byte-identical; edit/add legs
keep all other entries byte-identical). Fields whose *purpose* is inferred rather than proven are
labeled **[hypothesis]**.

Tools: `tools/text_build.py` (payload codec + edit/add API + CLI) on top of `tools/big_write.py`
(BIGB container rebuild) and `tools/parse_bigb.py` (reader).

## Summary

`text.big` (and its siblings `dialogue.big`, `frontend.big`, `graphics.big`, `textures.big`,
`fonts.big`, `shaders.big`, `effects.big`) are all the **same container format**: a Lionhead
"**BIGB**" bank archive. The archive holds one or more named **sub-banks**; each sub-bank has a
flat **table of contents** (entry directory) and the entry *payload blobs* live at the front of the
file. For `text.big`, each payload is a **localized string record** (UTF-16LE content + metadata).

- **Magic:** ASCII `BIGB` at offset 0.
- **String content encoding:** **UTF-16 little-endian**, NUL-terminated (`0x0000`).
- **Metadata strings** (identifier / speaker / speech-bank / entry name): **length-prefixed 8-bit**
  (`u32 length` then raw bytes; treat as ASCII/Latin-1).
- **Numbers:** all little-endian.
- **Per-string ID:** a `u32` numeric ID *and* a human-readable name string (e.g.
  `TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC`), both stored in the TOC entry. There is **no hash**
  on the string itself — lookup is by the numeric ID; symbolic lookups CRC the Name at runtime (§7).

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
3. **Validating parser** (independent reimplementation from the two sources above): scratchpad
   `parse_textbig.py` — decodes 28,913 entries; sample output archived in FINDINGS.md.
4. **Retail `Fable.exe` loader** (Ghidra) — §7.

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
`statsCount < 1000`; otherwise it rewinds and assumes no stats header — `LoadSubBankEntries` in
`BankLoader.h`.)

### 3b. Entry records (`EntryCount` of them, back-to-back)

Each TOC entry is variable length:

| Type | Field | Meaning |
|------|-------|---------|
| u32 | `Magic` | always **42** (0x2A). Entry sentinel; EgoCore rejects records where magic≠42 unless `Size>0 && ID>0`. |
| u32 | `ID` | numeric entry ID — the runtime lookup key |
| i32 | `Type` | **0 = string, 1 = group, 2 = narrator list** (see §4) |
| u32 | `Size` | byte length of the payload blob |
| u32 | `Offset` | file-absolute offset of the payload blob |
| u32 | `CRC` | payload checksum — **0 on all 28,913 retail text.big entries** (plausible from EgoCore struct name; never recomputed) |
| u32 `len` + bytes | `Name` | length-prefixed entry name (e.g. `TEXT_QST_028_…`). Length includes any trailing NUL, which is stripped. |
| u32 | `Timestamp` | build timestamp — **0 on all retail text.big entries** |
| u32 | `DepCount` | number of dependency strings — **0 on all retail text.big entries** |
| `DepCount` × (u32 len + bytes) | `Dependencies` | length-prefixed dependency name strings |
| u32 | `InfoSize` | size of an optional per-entry subheader blob (4 for type-0, 5,396 for the type-2 NarratorList; used by texture/mesh/anim banks in the other .bigs) |
| `InfoSize` bytes | `Info` | subheader — see §4 |

The TOC ends exactly at `FooterOffset` (verified: TOC walk terminated at 0x658267).
**TOC packing:** text.big's TOC starts UNALIGNED right after the last payload byte (graphics.big
pads to 4).

---

## 4. Payload blob layouts (by entry `Type`)

Payload starts at the entry's `Offset` and is `Size` bytes long. From `CTextParser::Parse`
(`TextParser.h`), verified byte-for-byte against real entries; the writer is a byte-exact port of
`CTextParser::Parse/Recompile` (codec: 28,912 payloads re-encode byte-exact).

### Type 0 — String entry (the common case; 26,807 in text.big)

| Order | Type | Field |
|-------|------|-------|
| 1 | UTF-16LE, `\0\0`-terminated | `Content` — the localized display string |
| 2 | u32 `len` + bytes | `SpeechBank` — e.g. `ScriptDialogue.lug` (voice-over bank), or empty |
| 3 | u32 `len` + bytes | `Speaker` — e.g. `FARMER`, `NONE`, or empty |
| 4 | u32 `len` + bytes | `Identifier` — equals the entry `Name` on ALL 26,807 retail entries |
| 5 | u32 | `TagCount` |
| 6 | `TagCount` × { i32 `Position`; NUL-term ASCII `Name` } | `Tags` — inline conversation directives |

```
UTF-16LE Content, terminated \0\0
u32 len + bytes   SpeechBank   (e.g. "ScriptDialogue.lug", often empty)
u32 len + bytes   Speaker      (e.g. "FARMER", "NONE", often empty)
u32 len + bytes   Identifier   (== entry Name on ALL 26,807 retail entries)
u32 TagCount
TagCount × { i32 Position; NUL-terminated ASCII Name }   # ANIM:/CAM:/attitude tags
```

**Tag names** carry conversation control (verified against real data + `TextProperties.h`):
- `ANIM:<name>` — play an animation (e.g. `ANIM:SCRIPT_CHEER_1`).
- `CAM:(prot,pos)(prot,focus)(zoom)` — camera directive.
- `CONVERSATION_ATTITUDE_*` — speaker mood (`EConversationAttitude` enum).
- other = custom/manual tag.
`Position` is the character index into `Content` at which the tag fires.

**Type-0 TOC `Info` blob (4 bytes) = u32 index of the entry's `Speaker` into the NarratorList**
(verified 26,807/26,807: `NONE`=0, empty speaker=3, `FARMER`=4, …). Distinct Info values = 379 =
NarratorList count. The writer sets this on `add` and re-patches it when `set --speaker` changes
the speaker.

### Type 1 — Group entry (2,105 in text.big)

A group bundles several string IDs (e.g. random banter variants):

| Order | Type | Field |
|-------|------|-------|
| 1 | u32 | `Count` |
| 2 | `Count` × u32 | member entry `ID`s |

(Verified: group `TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND` → count=4, member IDs
`[16879, 16872, 16761, 16858]`, each resolvable as a type-0 entry in the same bank.)

### Type 2 — Narrator list (1 in text.big)

The narrator/speaker-name list. Its TOC `Size` is 0 and there is **no payload blob** — the
narrator data lives in the TOC record's **`Info` blob** (5,396 bytes in retail):

| Order | Type | Field |
|-------|------|-------|
| 1 | u32 | `Count` (= 379 in retail) |
| 2 | `Count` × NUL-term ASCII | narrator/speaker name strings |

- The record is `[NarratorList]` @ 0x656d1d, entry ID 28913; the `[NarratorList]` signature
  visible at 0x656d39 is the record's own length-prefixed *Name* string.
- **NarratorList TOC `Offset` invariant:** retail stores Size=0 and Offset = end-of-payload-region
  = the sub-bank TOC offset (the compiler wrote "current file position" for the empty blob, same as
  EgoCore `TextCompiler::CompileTextBank`). `text_build` re-points it after a resize so the
  invariant holds in rebuilt banks.
- Since `big_write.rebuild` copies TOC records verbatim, narrator data survives every rebuild
  automatically.

---

## 5. Cross-references (how strings are used)

- **Runtime text lookup:** by numeric `ID` (the TOC `ID` field). The human-readable `Name` /
  `Identifier` is the authoring key; game code/scripts reference the compiled numeric index.
  **IDs are 1..28913, sorted, contiguous**; runtime lookup indexes an array by ID
  (`NGameText::CDataBank::GetTextInfo`), so new entries take `max_id+1`.
- **Voice-over link:** `SpeechBank` (`*.lug`/`*.lut`) names only the bank; the sound ID `<N>` is
  resolved from the `Identifier` via `Data\Defs\*snds.bin` header defs
  (`{crc0("SND_"+Name), soundID}` sorted pairs; EgoCore `ResolveAudioID`,
  `gamesnds.bin`/`dialoguesnds.bin`/`scriptdialoguesnds.bin`). `dialogue.big` holds the matching
  lip-sync/phoneme data (`LIPSYNC_*` sub-bank, EgoCore `LipSyncParser`). Full join:
  [AUDIO.md](AUDIO.md).
- **`.lut` companions** in `data\lang\English\` (`Dialogue.lut`, `ScriptDialogue.lut`, …) are the
  actual audio banks referenced by `SpeechBank` (no `ScriptDialogue.lug` exists on disk).

## 6. Read/edit path (for FableForge / a native reader)

1. Parse header → footer → sub-bank dir (§1–2).
2. For each sub-bank, read stats header + `EntryCount` TOC entries (§3).
3. For each type-0 entry, seek to `Offset`, read `Size` bytes, decode §4 type-0.
4. Edit: mutate `Content`/`Speaker`/`SpeechBank`/`Identifier`/`Tags`, re-serialize the blob
   (UTF-16LE content + length-prefixed metadata + tags — mirror of `CTextParser::Recompile`), then
   **rebuild the bank**: rewrite payload region + fix each entry's `Offset`/`Size` in the TOC + fix
   `FooterOffset`/`FooterSize`. (EgoCore does this in `SaveBigBank`; verify byte-safe round-trip on
   an unmodified file before shipping a writer — done, §8.)

**Encoding note:** `Content` is UTF-16LE and may contain non-ASCII (accented chars in other
languages). Metadata strings are single-byte (ASCII for English). The tag `Name` and narrator
strings are NUL-terminated single-byte; all other strings in the TOC are `u32`-length-prefixed
single-byte.

---

## 7. Loader confirmation (retail Fable.exe, Ghidra) — DONE (third source)

The text.big reader is the **`NGameText::CDataBank`** class, a subclass of the generic BIGB bank
reader **`CBankFile`**. Key retail addresses / evidence:

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
  hashed with **`CCRC::Calc` seeded 0** (the SAME seed-0 reflected CRC-32 — crc0 — the `game.bin`
  field-tag compiler uses; only `CCharString::GetCRC` — the save-stream variant — seeds
  `0xFFFFFFFF`. See FINDINGS.md "★ CANONICAL CRC REFERENCE"), then mapped to the numeric entry
  index, then `GetEntryDataType==0` is required, and the entry's **`CWideString`** (UTF-16
  content) is returned. So the on-disk `Name` field is the authoring key; runtime symbolic lookups
  CRC it. **[Note]** `FUN_009cc410` is the CRC→entry-index map (not yet decompiled in detail).
- `NGameText::CTextEntry` runtime object (ctor @ `0x009c8250`) holds 4 `CCharString`s
  (Identifier/Speaker/SpeechBank + one more) + the `CDataBankTextEntry*`, matching the type-0
  payload's metadata strings.

Logs: `ghidra_out/decomp_localisation.c` (148 fns incl. all of the above),
`ghidra_out/decomp_textbig_loader.c` (108 fns incl. `GetLanguage`/`GetFontBankName`),
`ghidra_out/decomp_textbig_strxref.log`, `ghidra_out/decomp_textbig_bankfile.log`.

---

## 8. Writer — `tools/text_build.py` (2026-07-20)

### 8.1 CLI

```
# inspect
python tools/text_build.py list  <text.big> --filter GOSSIP
python tools/text_build.py get   <text.big> TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC
python tools/text_build.py narrators <text.big>          # 379 speaker names

# edit an existing string (works on a COPY; -o may not equal input)
python tools/text_build.py set <copy.big> <NAME> --text "New line." -o out.big

# add a brand-new entry (id auto = max+1; speaker must be a NarratorList name)
python tools/text_build.py add <copy.big> TEXT_MY_NEW_LINE --text "Hi." --speaker NONE -o out.big

# full validation suite (copies the input itself; never writes to it)
python tools/text_build.py selftest <retail text.big> <workdir>
```

Python API: `TextBank(path)` → `.decode(name)`, `.set_text(name, text=, speaker=, speechbank=,
tags=)`, `.add_text(name, text, speaker=, speechbank=, tags=, eid=)`, `.add_group(name,
member_ids)`, `.save(out_path)`.

### 8.2 Facts established by the write path

1. **Type-0 TOC `Info` blob (4 bytes) = u32 Speaker index into the NarratorList** (§4) — was
   "[hypothesis] timestamp-adjacent unknown" territory before.
2. **The NarratorList (type-2) payload lives in its TOC record's `Info` blob** (5,396 bytes =
   `u32 count(379)` + 379 NUL-terminated names), NOT in the payload region (§4).
3. **NarratorList TOC `Offset` invariant** (§4) — re-pointed after a resize.
4. **TOC packing:** text.big's TOC is unaligned (§3b). `big_write.rebuild` gained a
   backward-compatible `toc_align=` parameter (default 4 = old behaviour; text path passes 1).
   Existing consumers (texture_build, blender compose) unaffected — compose_test.py re-run green
   after the change.
5. **CRC / Timestamp / DepCount are 0 on all 28,913 retail text.big entries**, so new records can
   safely write zeros (same as EgoCore's writer).
6. **IDs are 1..28913, sorted, contiguous**; new entries take `max_id+1`. Identifier is always
   written = Name (retail invariant, and `GetTextBySymbol` CRCs the Name).

### 8.3 Validation evidence (retail text.big, 6,652,561 B, 28,913 entries; all runs against COPIES)

`python tools/text_build.py selftest <retail text.big> <workdir>` output:

```
codec: 28912 payloads re-encode byte-exact
(a) no-edit rebuild: byte-identical OK
(b) edit TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC: decoded back OK, all other entries byte-identical
(c) add TEXT_FORGE_SELFTEST_NEW id=28914: decoded back OK, stats/entry_count bumped, retail entries byte-identical
TEXT SELFTEST: ALL ASSERTIONS PASSED
```

- **(a)** `rebuild()` with no edits → output == input, byte-for-byte (6,652,561 B). Requires
  `toc_align=1`.
- **(b)** one string replaced (incl. non-ASCII chars → UTF-16LE checked); re-parse: new content
  decodes back; every OTHER entry's payload bytes AND TOC fields (id/type/crc/timestamp/deps/info)
  identical to source; narrator offset invariant holds. Same check with `--speaker FARMER`: payload
  speaker + Info blob both update (index 4), bank-wide speaker↔Info audit = 0 mismatches.
- **(c)** new type-0 entry appended: id=28914, decodes back (content, speaker index,
  identifier==name), footer `entry_count` 28913→28914, TOC stats header type-0 count 26807→26808,
  all retail entries byte-identical. Group add (`add_group`, member-id type-checked) round-trips
  too. `parse_bigb.py` on both edited and added banks: **RESULT: CLEAN** (TOC ends at footer).
- **Regression:** `blender_addon/tests/compose_test.py` vs a graphics.big copy after the
  `toc_align` change: `COMPOSE TEST: ALL ASSERTIONS PASSED`.

---

## 9. Open / unverified items

- **In-game confirm of a rebuilt text.big** was not run in the 2026-07-20 session (edit a visible
  line, e.g. the Flourish onscreen-help, then trigger it). The dialogue pipeline's demon-door
  REPLACE set stages a text edit for exactly this smoke test ([AUDIO.md](AUDIO.md)).
- **Adding a NEW narrator/speaker name** is unsupported: it needs a rewrite of the type-2 record's
  Info blob (length change ⇒ big_write's verbatim-record contract doesn't cover it) and unknown
  engine sensitivity to narrator list growth. Workaround: reuse an existing narrator (379
  available) or empty.
- Localized sub-banks other than `TEXT_ENGLISH_MAIN` untested (grammar is identical; `TextBank`
  just picks the first `TEXT_*` sub-bank).
- `frontend.big`/`fonts.big` text-adjacent banks not in scope here ([FRONTEND_FORMAT.md](FRONTEND_FORMAT.md)).
- `FUN_009cc410` (CRC→entry-index map) not decompiled in detail.

## 10. Dated findings

- **2026-07-19** — Container + payload grammar decoded and validated (28,913 entries); EgoCore
  cross-check; Ghidra loader confirmation (`NGameText::CDataBank`, `GetTextInfo` 0x009c7d00,
  `GetTextEntryFromGroup` 0x009c9280, `GetTextBySymbol` 0x009c95e0, `GetLanguage` 0x00415070).
- **2026-07-20** — `text_build.py` shipped; byte-identical no-edit rebuild; type-0 Info = speaker
  index (26,807/26,807); NarratorList lives in the TOC Info blob; CRC/Timestamp/DepCount all 0;
  `toc_align=1`.
- **2026-09-07** — `TEXTBIG_FORMAT.md` + `TEXT_BIG_WRITE.md` merged into this file.

## 11. Superseded

- **"Type 2 — Narrator list: the engine scans the payload region for the `[NarratorList]`
  signature"** (2026-07-19 §4) — misread; the signature found at 0x656d39 is the TOC record's own
  length-prefixed Name string (record @ 0x656d1d); the data is in the record's `Info` blob (§4).
- **`Info` "(0 bytes for text; used by texture/mesh/anim banks)"** — type-0 entries carry a 4-byte
  Speaker index and the type-2 record a 5,396-byte narrator list (§4).
- **`CRC` "[hypothesis: CRC of the blob; not verified]" and `Timestamp` "[hypothesis]"** — both
  (and `DepCount`) are 0 on all 28,913 retail entries; purpose moot for text.big.
- **"Byte-exact write round-trip not yet proven for `.big`"** — PROVEN 2026-07-20 (§8.3).
- **"`CCRC::Calc` table/polynomial is the same open item tracked for #12/#13"** — resolved
  project-wide: crc0 = reflected CRC-32 poly 0xEDB88320, seed 0, no final inversion
  (`CCharString::ComputeCRC32` 0x00404310; CLAUDE.md).

## Verified facts (from FINDINGS log)

- **2026-07-19 — text.big / dialogue.big BIGB localization format CRACKED + validated (task #14).**
  Three independent sources agree (raw bytes + EgoCore completed-RE loader + retail `Fable.exe`
  loader); grammar is §1–§7 above. Retail header values: `Version=100`, `FooterOffset=0x658267`,
  `FooterSize=0x2A`; footer `SubBankCount=1`, sub-bank `TEXT_ENGLISH_MAIN` `Version 106`,
  `EntryCount 28913`; TOC stats header `StatsCount=3` = `(0,26807)(1,2105)(2,1)` (sums to 28913);
  every TOC entry `Magic==42`; TOC walk terminates exactly at `FooterOffset`. Validating parser
  `scratchpad/parse_textbig.py` decoded all 28,913 entries — samples:
  `[1] TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC` speaker=NONE bank=ScriptDialogue.lug
  ("If you get three hits in a row without reply, you can Flourish by pressing [B]…");
  `[835] TEXT_CS_035_END_10` speaker=FARMER, 1 tag `(0,"ANIM:SCRIPT_CHEER_1")`
  ("Well done! You really taught the Hobbes a lesson."); group
  `TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND` count=4, member IDs [16879,16872,16761,16858]. Retail
  loader (`NGameText::CDataBank : CBankFile`): `text.big` string @ `0x0122da24`;
  `NLocalisation::GetLanguage` @ `0x00415070` → `TEXT_*_MAIN` variants at `0x0122e8e8..0x0122e974`;
  `GetTextInfo` @ `0x009c7d00` requires `Type==0` and indexes `*(this+0x1e0)[id]`;
  `GetTextEntryFromGroup` @ `0x009c9280` requires `Type==1`, weighted-random pick (engine RNG
  `state*0x24a1+0x24df`); `GetTextBySymbol` @ `0x009c95e0` does
  `id = FUN_009cc410( CCRC::Calc(0, name, len) )` — seed-0 crc0, the same hash as game.bin field
  tags and save-stream tags; `FUN_009cc410` = `CVectorMap<u32,CDefString>::LowerBound`. Logs:
  `ghidra_out/decomp_localisation.c`, `decomp_textbig_loader.c`, `decomp_textbig_strxref.log`,
  `decomp_textbig_bankfile.log`. (The 2026-07-19 "write round-trip NOT yet proven" and the TOC
  `CRC`/`Timestamp` [hypothesis] were closed 2026-07-20 — §8, §11.)
