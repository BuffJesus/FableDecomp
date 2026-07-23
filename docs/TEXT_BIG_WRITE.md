# text.big WRITE path — recipe + evidence (2026-07-20)

Writer: `tools/text_build.py` (payload codec + edit/add API + CLI) on top of
`tools/big_write.py` (BIGB container rebuild) and `tools/parse_bigb.py` (reader).
Format reference: `docs/TEXTBIG_FORMAT.md` (container + payload grammar, 3-source
verified). This doc covers only what the WRITE path added/learned.

## TL;DR

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

Python API: `TextBank(path)` → `.decode(name)`, `.set_text(name, text=, speaker=,
speechbank=, tags=)`, `.add_text(name, text, speaker=, speechbank=, tags=, eid=)`,
`.add_group(name, member_ids)`, `.save(out_path)`.

## Text entry payload format (type 0) — what the writer emits

Byte-exact port of EgoCore `CTextParser::Parse/Recompile`
(`EgoCore-master\EgoCore\Text\TextParser.h`):

```
UTF-16LE Content, terminated \0\0
u32 len + bytes   SpeechBank   (e.g. "ScriptDialogue.lug", often empty)
u32 len + bytes   Speaker      (e.g. "FARMER", "NONE", often empty)
u32 len + bytes   Identifier   (== entry Name on ALL 26,807 retail entries)
u32 TagCount
TagCount × { i32 Position; NUL-terminated ASCII Name }   # ANIM:/CAM:/attitude tags
```

Type 1 (group) payload = `u32 count + count×u32 member IDs`.

## New facts established by the write path (beyond TEXTBIG_FORMAT.md)

1. **Type-0 TOC `Info` blob (4 bytes) = u32 index of `Speaker` into the
   NarratorList.** Verified 26,807/26,807 retail entries (`NONE`=0, empty
   speaker=3, `FARMER`=4, …). Distinct Info values = 379 = NarratorList count.
   The writer sets this on `add` and re-patches it when `set --speaker` changes
   the speaker. (Was "[hypothesis] timestamp-adjacent unknown" territory before.)
2. **The NarratorList (type-2) payload lives in its TOC record's `Info` blob**
   (5,396 bytes = `u32 count(379)` + 379 NUL-terminated names), NOT in the
   payload region. TEXTBIG_FORMAT.md §4's "engine scans the payload region for
   the `[NarratorList]` signature" was a misread: the signature match at
   0x656d39 in retail is the TOC record's own *Name* string (rec @0x656d1d).
   Since `big_write.rebuild` copies TOC records verbatim, narrator data
   survives every rebuild automatically.
3. **NarratorList TOC `Offset` invariant:** retail stores Size=0 and
   Offset = end-of-payload-region = the sub-bank TOC offset (the compiler wrote
   "current file position" for the empty blob, same as EgoCore
   `TextCompiler::CompileTextBank`). `text_build` re-points it after a resize so
   the invariant holds in rebuilt banks.
4. **TOC packing:** text.big's TOC starts UNALIGNED right after the last
   payload byte (graphics.big pads to 4). `big_write.rebuild` gained a
   backward-compatible `toc_align=` parameter (default 4 = old behaviour;
   text path passes 1). Existing consumers (texture_build, blender compose)
   unaffected — compose_test.py re-run green after the change.
5. **CRC / Timestamp / DepCount are 0 on all 28,913 retail text.big entries**,
   so new records can safely write zeros (same as EgoCore's writer).
6. **IDs are 1..28913, sorted, contiguous**; runtime lookup indexes an array by
   ID (`NGameText::CDataBank::GetTextInfo`), so new entries take `max_id+1`.
   Identifier is always written = Name (retail invariant, and
   `GetTextBySymbol` CRCs the Name).

## Validation evidence (retail text.big, 6,652,561 B, 28,913 entries; all runs
against COPIES — `selftest` copies the input itself)

`python tools/text_build.py selftest <retail text.big> <workdir>` output:

```
codec: 28912 payloads re-encode byte-exact
(a) no-edit rebuild: byte-identical OK
(b) edit TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC: decoded back OK, all other entries byte-identical
(c) add TEXT_FORGE_SELFTEST_NEW id=28914: decoded back OK, stats/entry_count bumped, retail entries byte-identical
TEXT SELFTEST: ALL ASSERTIONS PASSED
```

Details per leg:
- **(a)** `rebuild()` with no edits → output == input, byte-for-byte
  (6,652,561 B). Requires `toc_align=1` (fact 4).
- **(b)** one string replaced (incl. non-ASCII chars → UTF-16LE checked);
  re-parse: new content decodes back; every OTHER entry's payload bytes AND
  TOC fields (id/type/crc/timestamp/deps/info) identical to source; narrator
  offset invariant holds. Same check with `--speaker FARMER`: payload speaker
  + Info blob both update (index 4), bank-wide speaker↔Info audit = 0 mismatches.
- **(c)** new type-0 entry appended: id=28914, decodes back (content, speaker
  index, identifier==name), footer `entry_count` 28913→28914, TOC stats header
  type-0 count 26807→26808, all retail entries byte-identical. Group add
  (`add_group`, member-id type-checked) round-trips too. `parse_bigb.py` on
  both edited and added banks: **RESULT: CLEAN** (TOC ends at footer).
- **Regression:** `blender_addon/tests/compose_test.py` vs a graphics.big copy
  after the `toc_align` change: `COMPOSE TEST: ALL ASSERTIONS PASSED`.

## Open questions

- **In-game confirm not yet run** (no live-game leg this session): the rebuilt
  bank is grammar- and byte-level faithful, but a Fable.exe smoke test
  (edit a visible line, e.g. the Flourish onscreen-help, then trigger it)
  is the remaining proof. Same status texture/mesh writers had at this stage.
- **Adding a NEW narrator/speaker name** is unsupported: it needs a rewrite of
  the type-2 record's Info blob (length change ⇒ big_write's verbatim-record
  contract doesn't cover it) and unknown engine sensitivity to narrator list
  growth. Workaround: reuse an existing narrator (379 available) or empty.
- Localized sub-banks other than `TEXT_ENGLISH_MAIN` untested (grammar is
  identical; `TextBank` just picks the first `TEXT_*` sub-bank).
- `frontend.big`/`fonts.big` text-adjacent banks not in scope here.
