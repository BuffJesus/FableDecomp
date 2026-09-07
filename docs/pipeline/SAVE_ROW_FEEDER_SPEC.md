# Save-Row Feeder — implementation spec (workflow-synthesized 2026-08-13)

Wires live save-metadata rows into the frontend Load Game screen. Gap closed:
`tools/save_metadata.py` proves the read contract (python, not in build); 
`FableSetVisualFrontendSaveRows` is the renderer sink (authored defaults only). 
Nothing native reads real Profile.bin/.sav and feeds the sink. See docs/engine/GAME_ENTRY_PATHS.md#continue-and-load-game-path.

## Verdict
Add a NEW translation unit `rebuild/integration/frontend_save_rows.cpp` (+ `.h`). Do NOT extend `save_metadata_runtime_boundary.cpp` â€” that TU is the boot-time .MET-from-.LUG generator (`GenerateMetFilesFromLugFiles` 0x00418c3b), a genuinely different retail seam (boot-owned, script-bank data, Profile.bin never touched). Confirmed by reading it: all 14 stubs are FableMet* .MET sidecar helpers wired into Phase 9 behind the `FABLETLC_EXECUTE_EXACT_SAVE_METADATA` flag; it has no Profile.bin/HEADER code and sharing it would conflate two seams. ONE new seam for the feeder itself, BUT it forces a SECOND supporting TU: a self-contained raw-DEFLATE inflate (`fable_inflate.c/.h`), because the checkpoint build links NO zlib/inflate anywhere and the .sav HEADER is an RFC-1950 zlib stream (0x78DA) that must be inflated. So: 1 feeder TU + 1 inflate TU, plus add the existing `crcCalcObject` to the visual link list to reuse `FableCRC_Calc_004014A0` (crc0) rather than re-implement it. The checkpoint already resolves the Saves dir and enumerates profile dirs (`RefreshVisualProfileNames`/`FableEnumerateVisualFrontendProfiles` at visual_boot_checkpoint.cpp:2734/462) and already routes Load Game via `action==66` (line 3669) â€” the feeder plugs into that existing scaffold.

## Call site
Feeder: `void FABLE_FASTCALL FableFeedVisualFrontendSaveRows(const wchar_t* profileDir)`. Body: (1) build profileDir+L\"\\\\Profile.bin\", ReadFile whole; (2) verify 'FablePro' magic@0, read total_data_len@0x0C, verify FableCRC_Calc_004014A0(0,buf,total_data_len)==*(u32*)(buf+total_data_len); (3) scan latin1 text [0x10:total_data_len) line-by-line for `SaveGameNamesN \"value\";`, extract N(1..50) + quoted value (hand-rolled, no regex); (4) assemble <=4 ordered (kind,filename) rows: AutoSave, AutoSave.qs (each only if profileDir\\<name> exists as a file), then ascending non-empty manual slots, dedup by filename; (5) per row read .sav, run container-walk + FableZlibInflate(chunk0) + HEADER decode; success -> label=registry name, action=0x11; failure -> label=filename, action=0xDC; (6) fill const char* names[4]/fable_u32 actions[4], call FableSetVisualFrontendSaveRows(names,actions,count); if count==0 call FableSetVisualFrontendSaveRows(0,0,0) to reset to authored defaults.\n\nBoot call site: visual_boot_checkpoint.cpp `ActivateVisualMainMenuSelection`, the `if (action == 66)` block (line 3669), the RefreshAvailableSavedGamesForProfile-equivalent. Insert the feeder call where g_VisualSaveMenuActive=true is set (line 3675), passing the currently-selected profile directory (Saves\\<profileName>). Runs on every Load-Game menu entry, matching retail refresh-on-open. Rejected alternative: one-shot after FableInitialiseVisualD3D9 succeeds (line 6006) â€” can't react to profile switches and populates rows the user may never view.

## Plan
- 1. Add self-contained inflate TU `rebuild/integration/fable_inflate.c` (+ .h): a puff/tinfl-style raw-DEFLATE decoder exposing `int FableZlibInflate(const unsigned char* src, unsigned int srcLen, unsigned char* dst, unsigned int dstCap, unsigned int* outLen);` Handles RFC-1950 wrapper (skip 0x78/0xDA CMF/FLG, reject FDICT, ignore trailing Adler32) then DEFLATE. Compile as C89 under VC7.1 (no STL, no VLAs).
- 2. Add feeder TU `rebuild/integration/frontend_save_rows.cpp` (+ `frontend_save_rows.h`) exposing `void FABLE_FASTCALL FableFeedVisualFrontendSaveRows(const wchar_t* profileDir)`: reads one profile dir's Profile.bin (registry parse) and up to 4 rows' .sav HEADERs, decodes them, calls FableSetVisualFrontendSaveRows(names,actions,count).
- 3. Reuse existing CRC: add `$crcCalcObject` to `$visualRuntimeObjects` (build_bootstrap.ps1:4455) so `FableCRC_Calc_004014A0(seed,data,len)` links; declare it extern in the feeder. It is exactly crc0 (poly 0xEDB88320, caller seed, no final inversion) for BOTH Profile.bin trailer verify (seed 0) and HEADER field-tag verify (seed 0).
- 4. Register the two new sources in build_bootstrap.ps1: declare `$frontendSaveRowsSource/$fableInflateSource` + their `.obj`, add cl.exe compile steps mirroring the existing per-TU pattern, and append `$frontendSaveRowsObject,$fableInflateObject,$crcCalcObject` to `$visualRuntimeObjects` (linked at line 4843).
- 5. Wire the call site in visual_boot_checkpoint.cpp: in the saved-games-menu-enter handler (`action==66`, lines 3669-3681 where g_VisualSaveMenuActive=true and FableSetVisualFrontendSaveMenu(true) run), first resolve the selected profile dir (Saves\<selectedProfileName>) and call FableFeedVisualFrontendSaveRows(profileDirW). Retail-faithful: RefreshAvailableSavedGamesForProfile runs on menu entry (autosave-first then ascending manual slots).
- 6. Resolve the selected profile dir: reuse RefreshVisualProfileNames' Saves path (visual_boot_checkpoint.cpp:2734) + g_VisualProfileSelection/g_VisualProfileNames to build Saves\<name>. If Load Game is entered with no chosen profile, fall back to the most-recently-modified profile dir (retail default-profile behavior) â€” flagged as risk.
- 7. Feeder internal order: (a) verify Profile.bin trailer CRC (seed-0 over file[0:total_data_len]); (b) parse SaveGameNames1..50 from latin1 text [0x10:total_data_len); (c) ordered rows: AutoSave then AutoSave.qs (if file exists) then ascending non-empty manual slots, dedup by filename, cap 4; (d) per row open .sav, walk container, inflate chunk0, decode HEADER: action=0x11 on success else 0xDC; (e) fill const char* names[4]+fable_u32 actions[4], call the sink; count==0 -> FableSetVisualFrontendSaveRows(0,0,0) to reset to authored defaults.

## New / edited files
- rebuild/integration/frontend_save_rows.cpp â€” NEW feeder TU. `void FABLE_FASTCALL FableFeedVisualFrontendSaveRows(const wchar_t* profileDir)`. Contains: Profile.bin registry parser, FableSave! container walker (0x00 magic / 0x0C total_data_len==trailer_pos / 0x10 chunk0_ulen / 0x14 chunk1_ulen / 0x18 chunk0_clen / 0x1C chunk0 zlib), HEADER decoder (23-field schema, per-field seed-0 CRC tag check, no-trailing-bytes enforce), row-label formatter, sink call.
- rebuild/integration/frontend_save_rows.h â€” NEW. Declares FableFeedVisualFrontendSaveRows plus test-only helpers: `int FableDecodeSaveHeaderForTest(const unsigned char* fileBytes, unsigned int fileLen, FableSaveHeaderRow* out);` and `unsigned int FableParseProfileRegistryForTest(const unsigned char* profileBytes, unsigned int len, FableProfileSlot* slots, unsigned int cap);`
- rebuild/integration/fable_inflate.c â€” NEW. Self-contained RFC-1950/DEFLATE inflate (`FableZlibInflate`), no STL/third-party, C89 for VC7.1. Only chunk0 (HEADER) is ever inflated; chunk1 skipped.
- rebuild/integration/fable_inflate.h â€” NEW. extern "C" declaration of FableZlibInflate for the C++ feeder.
- rebuild/tests/integration/FrontendSaveRows_test.cpp â€” NEW console unit test. Runs feeder helpers against the real local profiles under %USERPROFILE%\Documents\My Games\Fable\Saves\<name>; asserts row count/order/action/labels vs the python golden.
- EDIT rebuild/build_bootstrap.ps1 â€” register the two new sources+objects, add cl.exe compile steps, append $frontendSaveRowsObject,$fableInflateObject,$crcCalcObject to $visualRuntimeObjects (~line 4455).
- EDIT rebuild/integration/visual_boot_checkpoint.cpp â€” in the action==66 handler (~line 3669) call FableFeedVisualFrontendSaveRows(selectedProfileDirW).

## Port notes (save_metadata.py -> VC7.1 C)
- crc0 (Profile.bin trailer + HEADER field tags): DO NOT re-implement. Reuse compiled `FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 len)` (rebuild/src/compiled/00/40/Global_CCRC_Calc_004014a0.cpp) by adding $crcCalcObject to the visual link list + `extern "C"` decl. Matches python crc0 exactly: reflected poly 0xEDB88320, caller seed, NO final inversion. HEADER field tag = FableCRC_Calc_004014A0(0, fieldNameAscii, strlen(name)) with NO trailing NUL (== save_metadata.py savecrc.getcrc(name,0)).
- Profile.bin registry parse (save_install.py:109-126 port): text is latin1 in [0x10:total_data_len). No-STL: hand-roll a line scanner; if a line starts 'SaveGameNames', parse the trailing int (1..50), then copy between the first '"' and next '"' as the filename (may be empty=unoccupied) into fixed `char slots[50][128]`. Treat both \r and \n as line terminators (lines are CRLF). RISK: escaped/embedded quotes untested (reader open-question) â€” take first-quote..next-quote literally, no unescape.
- Row ordering (save_metadata.py:203-228 port): autosave-first. GetFileAttributesExW(profileDir\AutoSave) and \AutoSave.qs; add each only if it exists as a non-directory file. Then slots 1..50 ascending, skip empty and already-seen filenames. Cap emitted list at 4 (sink hard-caps at 4). Dedup by exact filename compare.
- FableSave! container walk (save_edit.read_save port): magic 'FableSave!' (10 bytes, no NUL)@0; u32@0x0C is total_data_len == trailer_pos (offset to trailer), NOT a per-save CRC â€” readers mislabeled 0x0C as 'signature/CRC hypothesis'; read_save proves sig_field==trailer_pos is REQUIRED. u32@0x10=chunk0_ulen, u32@0x14=chunk1_ulen, u32@0x18=chunk0_clen; chunk0 zlib@0x1C len chunk0_clen. Only chunk0 (HEADER) is needed â€” do NOT read/inflate chunk1. Trailer CRC at trailer_pos = FableCRC_Calc_004014A0(0,file,trailer_pos); verify ==*(u32*)(file+trailer_pos). Trust header lengths, not the fixed 0x4B000 file size.
- Inflate (the one genuinely new native primitive): checkpoint links no zlib. Add fable_inflate.c (puff/tinfl-style raw DEFLATE). chunk0 is RFC-1950: byte0=0x78 CMF, byte1=0xDA FLG; skip 2 header bytes, decode DEFLATE, ignore trailing 4-byte Adler32. Reject FDICT (FLG bit5). Bound all writes by dstCap; require outLen==chunk0_ulen else fail the row (0xDC). C89-clean: fixed-size Huffman tables on the stack, no VLAs, no STL.
- HEADER decode (save_metadata.decode_header port): inflated chunk0 = 'HEADER\0' then u32 section_len then fields. Verify magic; read section_len; for each of the 23 schema fields in exact order: read u32 tag, verify ==FableCRC_Calc_004014A0(0,name,len); consume by kind: ascii=NUL-term latin1, wide=UTF-16LE NUL-term (2-byte 0x0000), u32=4B, bool=1B, float=4B, float3=12B. Enforce final offset==section_end (no trailing bytes) like python, else 0xDC. The 23 tags are known constants; hard-coding them is acceptable, computing keeps them in sync.
- Label content: MVP = use the registry filename as the row label (retail SaveGameNamesN value IS both filename and display label). Sink buffers are 128B strncpy-truncated. Enrichment deferred: HEADER lacks Gold/Level/Morality/Renown (those live in SAVED_ENTITIES/chunk1, reader open-question confirmed) so a rich label from HEADER can only add CurrentRegionName + TotalTimePlayed, not hero stats â€” keep MVP filename+valid/invalid, note enrichment as follow-up.
- FABLE_FASTCALL/ABI: sink is `void FABLE_FASTCALL FableSetVisualFrontendSaveRows(const char* const* names, const fable_u32* actions, fable_u32 count)`. Use the project FABLE_FASTCALL macro (visual TUs already do). Feeder is FABLE_FASTCALL too. The C++->C call into fable_inflate.c must be extern "C".
- Style constraints (CLAUDE.md): these are hand-authored integration TUs, not verify_and_land oracles, so the __thiscall/static_assert/&name; harness gotchas don't gate them, but keep to the project's no-STL integration style (raw arrays, hand-rolled string ops as in visual_boot_checkpoint.cpp).

## Verification
- UNIT TEST (primary, offline, no D3D9): rebuild/tests/integration/FrontendSaveRows_test.cpp (console subsystem, mirrors VisualBootCheckpoint_test.cpp harness). For each real profile dir under %USERPROFILE%\Documents\My Games\Fable\Saves\<name> (confirmed present: 1, 21, 25, ...), run FableParseProfileRegistryForTest + FableDecodeSaveHeaderForTest and assert row count/order/action match the python golden and the AutoSave row decodes action=0x11.
- GOLDEN oracle: `python tools/save_metadata.py "<profileDir>" --json` per profile (already runs â€” profile 1 yields AutoSave action=17=0x11, CurrentRegionName=TXT_REGION_OAKVALE, MINIMAP_STARTOAKVALE). Dump golden JSON to scratchpad; C++ test compares filename/kind/action per row for byte-for-byte parity of the native port against the reference decoder on real data.
- BYTE CHECKS in the test: assert Profile.bin trailer CRC verifies (FableCRC_Calc_004014A0(0,buf,total_data_len)==trailer); assert FableSave! 0x0C field == computed trailer_pos; assert inflate of chunk0 returns exactly chunk0_ulen bytes and begins with 'HEADER\0'; assert all 23 HEADER field tags match their seed-0 CRC (catches tag/endianness drift).
- INFLATE self-check: micro-test that inflates chunk0 of one real .sav and compares first N bytes + total length against python zlib.decompress output (dump reference bytes to scratchpad). Isolates the one new primitive before it's trusted in the feeder.
- VISUAL CHECKPOINT (integration): build via rebuild/build_bootstrap.ps1 (new objects linked), launch FableTLC-Reconstruction-VisualCheckpoint.exe (1280x720), synth-click Main Menu -> Load Game (action==66), screenshot via Graphics.CopyFromScreen + 3x NearestNeighbor crop (recipe in docs/pipeline/VISUAL_PARITY_STATUS.md). Confirm the four rows now show REAL registry names (AutoSave + user's manual save names) with valid rows white (0x11) and any missing/corrupt row dimmed gray (0xDC), versus the previous authored 'AutoSave/Save 1/Save 2/Save 3' defaults.
- REGRESSION: run VisualBootCheckpoint_test.cpp to confirm the new link additions ($crcCalcObject etc.) don't break the checkpoint; confirm the .MET boundary (save_metadata_runtime_boundary.cpp) test and Phase 9 fixture still pass (untouched).

## Risks
- INFLATE is net-new native code and the highest-risk item: a subtly wrong DEFLATE decoder can silently mis-decode plausible-but-wrong HEADER values or overrun. Mitigate with the inflate self-check vs python zlib, strict dstCap bounds, and require outLen==chunk0_ulen.
- Selected-profile-dir ambiguity: it is unclear which profile is 'current' when Load Game is entered from the main menu (retail loads the active/most-recent profile). If no profile is selected at action==66 the feeder must default (most-recently-modified Saves\<dir>) â€” a behavioral guess, not RE-proven. Confirm against retail.
- Row label semantics unconfirmed: retail may display a formatted region/date/time string rather than the raw registry name (reader open-question #4 unanswered). MVP uses the filename; true retail label format is not yet RE'd â€” enrichment is speculative.
- Profile.bin registry edge cases (escaped/embedded quotes, malformed SaveGameNamesN lines, multiline) are reader open-questions and NOT covered by the python reference parser either. Native parser should fail-soft (skip the slot), not crash. Untested against adversarial Profile.bin.
- 0x0C mislabeled by readers as a per-save CRC 'signature (hypothesis)'; it is actually total_data_len==trailer_pos (proven in save_edit.read_save). Following the reader summary verbatim would add a wrong CRC check. This spec corrects it; note the reader constants list is misleading here.
- chunk1/SAVED_ENTITIES is deliberately NOT decoded â€” hero Gold/Level/Morality/Renown are unavailable for the label (they live in the entity graph, reader open-question confirmed). Rich rows would be a separate, much larger deserialization effort.
- Build-list churn: adding $crcCalcObject to $visualRuntimeObjects could cause a duplicate-symbol link error if FableCRC_Calc_004014A0 / g_FableCRCTable_0129A168 is already pulled in transitively. Verify with a clean link; if duplicated, drop $crcCalcObject and call the already-linked provider.

## Appendix — mapped subsystem findings

### Fable TLC Save Browser Metadata Enumeration Algorithm (save_metadata.py)
The save enumeration system reads the Profile.bin registry to discover which save files should appear in the in-game LOAD list. It loads the registry, enumerates autosaves first (if present), then manual saves from SaveGameNames1..50 registry entries in slot order, validates each file by decoding its FableSave! container and HEADER section, and emits row action codes (0x11=valid, 0xDC=invalid/missing). The system enforces CRC validation on both Profile.bin trailer and each save's FableSave! trailer before marking a row as loadable. Unregistered files in the directory are deliberately ignored, matching retail behavior.
Constants:
  - FablePro magic: b'FablePro' (8 bytes)
  - Profile.bin offset 0x00: magic FablePro
  - Profile.bin offset 0x08: reserved dword (0)
  - Profile.bin offset 0x0C: total_data_len (offset to trailer)
  - Profile.bin trailer offset: total_data_len (u32 seed-0 CRC)
  - Profile.bin text range: [0x10:total_data_len)
  - Profile.bin text encoding: latin1
  - Profile.bin registry slots: SaveGameNames1..SaveGameNames50 (exactly 50)
  - Profile.bin slot pattern: 'SaveGameNamesN "value";' where N in 1..50
  - FableSave! magic: b'FableSave!' (10 bytes, no NUL terminator)
  - FableSave! offset 0x00: magic FableSave!
  - FableSave! offset 0x0A: padding (2 bytes, value 0x00 0x00)
  - FableSave! offset 0x0C: total_data_len (u32 LE, equals offset to trailer)
  - FableSave! offset 0x10: chunk0_uncompressed_len (u32 LE)
  - FableSave! offset 0x14: chunk1_uncompressed_len (u32 LE)
  - FableSave! offset 0x18: chunk0_compressed_len (u32 LE)
  - FableSave! offset 0x1C: chunk0 zlib stream (length = chunk0_compressed_len)
  - HEADER magic: b'HEADER\0' (7 bytes including NUL at offset 0)
  - HEADER offset [name_nul:name_nul+1]: NUL terminator of magic
  - HEADER offset [name_nul+1:name_nul+5]: u32 LE section length (bytes of inflated data)
  - HEADER offset [name_nul+5]: start of field data
  - Autosave filename 1: 'AutoSave' (no extension)
  - Autosave filename 2: 'AutoSave.qs' (with .qs extension)
  - Action code LOADABLE: 0x11 (valid row, file exists and decodes)
  - Action code INVALID: 0xDC (missing file or corrupt/invalid row)
  - CRC polynomial: 0xEDB88320 (reflected, standard zlib)
  - CRC seed for save-stream tags (legacy): 0xFFFFFFFF
  - CRC seed for HEADER field tags: 0x00000000 (seed-0)
  - CRC seed for file trailer: 0x00000000 (seed-0)
  - CRC finalization: NO final inversion (output value not XORed with 0xFFFFFFFF)
  - File slab size: 0x4B000 (307200 bytes, fixed preallocated size)
  - Maximum manual save slots: 50
  - HEADER field schema size: 23 fields (fixed, no variability)
  - WorldName kind: ascii (null-terminated latin1)
  - WorldFrame kind: u32 (4-byte LE unsigned)
  - TeleportingEnabled kind: bool (1 byte)
  - SavingEnabled kind: bool (1 byte)
  - ExperienceSpendingEnabled kind: bool (1 byte)
  - CreatureGenerationEnabled kind: bool (1 byte)
  - CreatureGenerationDisabledGroups kind: u32 (4-byte LE unsigned)
  - HeroSleepingEnabled kind: bool (1 byte)
  - MapTableShowQuestCardsOnUsed kind: bool (1 byte)
  - MiniMapEnabled kind: bool (1 byte)
  - MiniMapActiveBeforeDisabled kind: bool (1 byte)
  - GuildMasterMessagesEnabled kind: bool (1 byte)
  - SummonerDeathExplosionAffectsHero kind: bool (1 byte)
  - MostRecentSaveType kind: u32 (4-byte LE unsigned)
  - MostRecentSaveTypeBeforeManualSave kind: u32 (4-byte LE unsigned)
  - MostRecentManualSaveName kind: wide (UTF-16LE null-terminated)
  - SaveGameMarkerPos kind: float3 (3x 4-byte IEEE-754 LE)
  - SaveGameMarkerAngleXY kind: float (4-byte IEEE-754 LE)
  - GuildSealRecallPos kind: float3 (3x 4-byte IEEE-754 LE)
  - GuildSealRecallAngleXY kind: float (4-byte IEEE-754 LE)
  - CurrentRegionName kind: ascii (null-terminated latin1)
  - CurrentRegionMinimapGraphicName kind: ascii (null-terminated latin1)
  - TotalTimePlayed kind: float (4-byte IEEE-754 LE)
  - ASCII field terminator: 0x00 (single byte)
  - UTF-16LE (wide) field terminator: 0x00 0x00 (2 bytes, value 0x0000)
  - Field tag size: 4 bytes (u32 LE)
  - Section header size: [name NUL][u32 length] = variable + 4 bytes
Open questions:
  - Are there any edge cases in Profile.bin persist-script parsing (multiline handling, escaped quotes, special characters) beyond what save_install.py:112-126 shows?
  - Does enumerate_profile skip corrupt registry entries gracefully, or will a malformed SaveGameNamesN entry crash the parser?
  - When a HEADER field tag mismatch occurs (save_metadata.py:141-144), is the entire row marked invalid with 0xDC, or only that specific field skipped?
  - How does the frontend display the 23 HEADER fields in the UIâ€”which fields map to on-screen labels like 'Hero Name', 'Time Played', 'Region', 'Level'?
  - Are both chunk0 and chunk1 ALWAYS present in every valid FableSave!, or can one be empty (zero uncompressed_len)?
  - What happens if chunk0_uncompressed_len or chunk1_uncompressed_len is 0? Does decode_header() handle empty HEADER sections?

### FableSetVisualFrontendSaveRows contract and native feeder integration
FableSetVisualFrontendSaveRows is the narrow renderer-side injection point for live save-row metadata (labels and action codes) in the Fable TLC frontend. It accepts up to 4 Profile.bin-derived save labels plus their action codes (0x11 for valid/loadable, 0xDC for missing/corrupt), copies them into global storage with bounded buffers (128 bytes per label), and dims invalid rows during rendering. Passing null names resets to authored checkpoint defaults. This boundary was added in commit ec9be11 to bridge the Python save_metadata.py decoder (commit 460dafc) with the renderer, but does not yet connect Enter key to the native world-load owner.
Constants:
  - Max rows: 4
  - Label buffer size: 128 bytes per row
  - Action valid code: 0x11 (white text, 0xFFFFFFFF diffuse color)
  - Action invalid code: 0xDC (gray text, 0xFF808080 diffuse color)
  - Default names: AutoSave, Save 1, Save 2, Save 3
  - Initial state all rows: action=0x11 (valid)
  - Dimming threshold: any action != 0x11 gets dimmed to 0xFF808080
Open questions:
  - What input validation exists for malformed action codes (e.g., other than 0x11 or 0xDC)?
  - Does the feeder need to handle label strings longer than 127 chars, or is truncation acceptable?
  - Which native caller currently invokes FableSetVisualFrontendSaveRows - is it connected to Profile.bin read yet?
  - Is the Enter key on a 0xDC row currently blocked, or does it fall through to an error state?
  - Does the renderer need to emit separate disabled-row quads beyond just dimming the label color?

### save_metadata_runtime_boundary.cpp (0x00418c3b GenerateMetFilesFromLugFiles .MET-from-.LUG family) vs. frontend save enumeration path
The file reconstructs a NATIVE BOOT-TIME .MET metadata generation path (0x00418c3b GenerateMetFilesFromLugFiles, called from Phase 9 at 0x004032D5-0x00403389). This path enumerates retail .LUG script-bank files in a wildcard search, extracts their stem and extension via FableMetSplitPath, constructs parallel .MET sidecar filenames, and writes structured metadata via FableMetDumpToFile (all currently stubbed). This is a DIFFERENT parallel native path from the frontend save-list enumeration (Profile.bin registry to Load Game list), which lives in game-loop (not boot), processes player save data (not ship script banks), and surfaces region/time/hero metadata via AddRegionAndTimeInfo (0x00597228) and ConstructFileDescription (0x00595CC1) to the Load dialog.
Constants:
  - 0x00418c3b: real address of CMainGameComponent::GenerateMetFilesFromLugFiles
  - 0x5A17C0DE: g_FableSecurityCookie_0139C8A8 validation value
  - 0x0122ED7C: g_FableMetPersistVTable_0122ED7C (vtable reference at offset 0x7C in system manager)
  - 0xCC: vtable method offset for metadata writer (call dword ptr [ebx+0xCC])
  - 399 bytes: retail body size per HANDOFF.md
  - 34 relocations: extern references outside behavior matching
  - 0x004032D5-0x00403389: Phase 9 address range
  - Stack frame: ebp-0x1EC to ebp+0x1E8 (0x3F4 total)
  - L"*.lug": wildcard search pattern
  - L".met": output extension
Open questions:
  - CRITICAL VERDICT QUESTION: Is this .MET generation seam (boot-time script-bank metadata) the SAME seam as the frontend save-row enumeration (game-loop player save metadata via Profile.bin registry), or DIFFERENT parallel paths? Answer: DIFFERENT. This is boot-only, script-bank-only, uses the MET sidecar family (not HEADER or save row actions 0x11/0xDC). Frontend save enumeration lives in CUserProfileManager (game-loop) via LoadFileList (0x4091c0), uses Profile.bin registry and .sav HEADER fields, outputs row action codes. No shared code path documented.
  - Build integration: Phase 9 fixture is conditional on processSaveFileMetadata flag; exact execution conditional on FABLETLC_EXECUTE_EXACT_SAVE_METADATA define. Should the live-row feeder build on save_metadata_runtime_boundary.cpp or be a separate TU? Answer: SEPARATE TU. The .MET path is boot-owned data generation; the live-row feeder for frontend save list is game-loop-owned enumeration consumer. Architectural seams differ (boot vs game, script data vs save data, generation vs browsing). Integration point is only the conditional flag in Phase 9 state.
  - Remaining work scope: all 14 FableMet* stubs currently no-op. Retail implementation requires: (1) enumerate actual .lug files from configured directory, (2) parse/split each filename, (3) construct .met sidecar paths, (4) generate structured metadata (presumed binary format mirroring retail bank-metadata layout), (5) serialize to .met files. Test fixture verifies call counts but not actual file I/O or format. Real implementation blocked on .MET binary format specification.

### Save File On-Disk Byte Contract (.sav, Profile.bin, HEADER fields)
Complete byte-level specification for Fable: The Lost Chapters save files on disk. Defines (1) Profile.bin registry structure for 50 manual save slots, (2) .sav container HEADER with 23 fields and CRC tags, (3) CRC-32 algorithm implementation, (4) FableSave! trailer signature. Sufficient to port save_metadata.py to C++ without Python dependencies.
Constants:
  - Magic 'FableSave!' = 0x46 0x61 0x62 0x6C 0x65 0x53 0x61 0x76 0x65 0x21 (10 bytes)
  - Magic 'FablePro' = 0x46 0x61 0x62 0x6C 0x65 0x50 0x72 0x6F (8 bytes)
  - File size: 307200 bytes = 0x4B000
  - Profile.bin reserved dword at 0x08: always 0x00000000
  - Profile.bin total_data_len at 0x0C: file_size - 4 = points to u32 trailer CRC
  - Zlib magic: 0x78 0xDA (deflate best-compression)
  - CRC polynomial: 0xEDB88320 (reflected)
  - CRC seed for on-disk tags: 0x00000000 (NOT 0xFFFFFFFF)
  - HEADER field tags: WorldName=0xa8de4f22, WorldFrame=0x5b0fcaa0, TeleportingEnabled=0xd324e6a7, SavingEnabled=0x70756220, ExperienceSpendingEnabled=0x6a28abd2, CreatureGenerationEnabled=0x3f6e5950, CreatureGenerationDisabledGroups=0x5899107a, HeroSleepingEnabled=0x452dddb2, MapTableShowQuestCardsOnUsed=0x2409e2a4, MiniMapEnabled=0x1bfb95b6, MiniMapActiveBeforeDisabled=0x4c7c1d48, GuildMasterMessagesEnabled=0xad260f50, SummonerDeathExplosionAffectsHero=0x82818b2c, MostRecentSaveType=0x3831a50f, MostRecentSaveTypeBeforeManualSave=0x6c9212bd, MostRecentManualSaveName=0x9a72a8db, SaveGameMarkerPos=0x80926404, SaveGameMarkerAngleXY=0xaedf95f2, GuildSealRecallPos=0x34b73c37, GuildSealRecallAngleXY=0x0a111ac7, CurrentRegionName=0xfd9cf30d, CurrentRegionMinimapGraphicName=0xeb6d5050, TotalTimePlayed=0x45242440
  - PLAYER section tags: PlayerCharacterUID=0x96adc827, PlayerRegionName=0x9a7e84f7
  - SaveGameNames registry format: 50 slots, format='SaveGameNamesN "string";\r\n', N=1..50, string=optional display label and filename
Open questions:
  - Exact hash input for .sav signature dword at 0x0C: is it the full payload bytes, just the chunks, or the trailer? Decompile SaveGameStateInternal caller + load-time verifier.
  - Whether retail loader enforces .sav signature check on load (UseSaveGameSignatures flag). If enforced, all edited saves require signature recompute or config override.
  - Hero live stats (Gold/Morality/Experience/Renown) field names: not in HEADER as tagged fields; they live in SAVED_ENTITIES entity graph as hero CThing's component blob. Requires decompile of CThingManager::SaveGameState -> hero entity Transfer to recover field names and tags.
  - Exact meaning of .sav signature dword: CRC seed, input range, whether it gates load acceptance.
---

## 2026-08-13 addendum — save-preview circle investigation + field enrichment

**Retail display facts (from resources/UIScreenshots(Retail)/ContinueGameScreen.png):**
the Load Game list shows a single `AutoSave` row then manual saves as `Save 1`,
`Save 2`, `Save 3` (slot-index labels, NOT the raw registry filename; the
`AutoSave.qs` quicksave is not listed). The feeder now emits exactly this
(retail-faithful labels, commit history).

**The circular preview image is a REGION MINIMAP, not a per-save screenshot.**
The frontend element is `UI_VIEW_RING_SMALL` (fable_visual_d3d9.h:197,
visual_boot_d3d9.cpp:4540): a ring ornament + region-minimap panel, design
(314,37), 256x256. The current reconstruction bakes a STATIC ring per save-cell
index via `FableComputeSaveViewportAtlasRect(g_SaveSelection,...)` sampling
`g_OptionsTexture` — i.e. it is keyed on the selection index, not the actual
save's region. Retail keys the inner minimap on the selected save's
`CurrentRegionMinimapGraphicName` HEADER field (e.g. `MINIMAP_GREATWOOD`), which
`SAVE_PROFILE_INDEX.md` lists among the LOAD-screen metadata (hero name,
chapter/region, playtime, thumbnail). No per-save screenshot is stored in the
.sav; the "thumbnail" is the region minimap graphic.

**Enrichment landed (this commit):** the feeder's existing chunk0 HEADER decode
now also captures `CurrentRegionName`, `CurrentRegionMinimapGraphicName`, and
`TotalTimePlayed` into `FableSaveHeaderInfo` per row (verified 19/19 vs the
save_metadata.py golden). This is the data layer the retail-faithful preview
needs — no extra inflation (chunk0 was already decoded for validity).

**Remaining to make the preview retail-faithful (follow-up):**
1. Map `MINIMAP_*` graphic name -> a texture/atlas rect. Open question: where the
   MINIMAP_* graphics live (frontend .big vs a dedicated minimap atlas) — needs an
   asset dig.
2. Wire selection -> the selected row's `info.minimapName` -> draw that minimap
   inside the ring, replacing the static per-index bake in visual_boot_d3d9.cpp
   (~L4540). The renderer sink (FableSetVisualFrontendSaveRows) would need to also
   carry the per-row minimap name (sink extension), or a parallel setter.

---

## 2026-08-13 — build + call-site wiring LANDED (compile-verified)

Both new TUs are now wired into the visual checkpoint:
- **build_bootstrap.ps1**: declared `$frontendSaveRowsSource`/`$fableInflateSource` +
  `$frontendSaveRowsObject`/`$fableInflateObject`, added both to the `$required`
  existence gate, added cl.exe compile steps (mirroring the save-metadata boundary),
  and appended both objects to `$visualRuntimeObjects` (after `$visualBootD3D9Object`).
  Feeder is linked ONLY into the visual checkpoint exe (not the staged-phase exes).
- **visual_boot_checkpoint.cpp**: `#include "frontend_save_rows.h"`; retain the
  activated profile in `g_VisualActiveProfileName` (set at the 0x124 LoadProfile
  activation); extracted `BuildVisualSavesRoot()` from `RefreshVisualProfileNames`
  and added `ResolveActiveProfileSaveDir()`; at the `action==66` Load-Game handler
  call `FableFeedVisualFrontendSaveRows(Saves\<activeProfile>)` (falls back to
  `FableFeedVisualFrontendSaveRows(0)` = authored defaults when unresolved).

Verified: all three TUs (fable_inflate.c, frontend_save_rows.cpp, visual_boot_checkpoint.cpp)
compile clean under the exact build flags (/W3 /MT /GS /O2 /Oy); build_bootstrap.ps1
parses clean; the feeder<->checkpoint fastcall extern-"C" linkage was proven by the
standalone test link. kernel32 file APIs resolve implicitly (the checkpoint already
uses FindFirstFileW/GetFileAttributesExW without listing kernel32.lib).

NOT done here: the full `build_bootstrap.ps1` run + on-screen confirmation — the full
bootstrap stops at the pre-existing FABLETLC_WINMAIN_BEHAVIOR gate before the visual
link, and there is no display in this environment. Visual QA (synth-click Main Menu ->
Load Game, screenshot) is the remaining check, per docs/pipeline/VISUAL_PARITY_STATUS.md.
