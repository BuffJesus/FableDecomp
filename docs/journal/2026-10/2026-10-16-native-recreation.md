# Native recreation, 2026-10-16

Continues `2026-10-15-native-recreation-continuation.md`. All landings are `native_exact_roots`
gates (fresh VC7.1 compile, relocation-audited, byte-identical), registered in `gate_all.py`,
`symbol_index.py` (verified roots + companions), `function_overrides.tsv` and `ARTIFACT_INDEX.tsv`.
Nothing committed.

## Landed (exact)

| Gate | Roots |
|---|---|
| `gate_native_version.py` | CVersion ctor 009EA080, operator== 009EA0A0 (absent from the manifest), GetAsString 009EA0D0 (/GS, `"%02d.%02d.%02d.%04d"` day/month/year/build) |
| `gate_native_console_command_line_ctors.py` | CConsoleCommandLine default 009EA2B0 (manifest said CMemoryDataOutputStream; vtable 0x129C400 RTTI says otherwise), copy 009EA2E0, from-text 009EEA10, scalar deleting dtor 009EE390 |
| `gate_native_console_guess_mode.py` | CConsole::SetGuessMode 009EAFF0, NextCommandGuess 009EB0D0, input-map iterator operator++ 009EE470 |
| `gate_native_console_registered_input.py` | CConsole::GetRegisteredConsoleInput 009EB220 |
| `gate_native_console_input_tree_lookups.py` | RegisteredConsoleInputs `_M_lower_bound` 009EEC10, `_M_find` 009EED20 |
| `gate_native_console_registered_input.py` (+) | CConsole::RunCommandLine 009EB430 (same TU as GetRegisteredConsoleInput, which it expands) |
| `gate_native_console_command_line.py` (+) | CConsole::RunTextCommand 009EC710 (expands Reset and Ego_r inline CConsoleCommandLine::SetString) |
| `gate_native_console_variable_process.py` | CConsoleVariable::ProcessCommandLine 009EC430 |
| `gate_native_console_binding_commands.py` | ConsoleRunScript 009ECB70, ConsoleAddKeyBinding 009ECC70, ConsoleAddStringBinding 009ECCE0, ConsoleRunStringBinding 009ECD40 (the last two absent from the manifest) |
| `gate_native_console_run_script.py` | CConsole::RunScript 009EC890 |
| `gate_native_console_help_bar.py` | CConsole::SetHelpBarString 009EB2C0 |
| `gate_native_string_not_equal_char.py` | CCharString::operator!=(char) 0099E520 (Ego_r inline operator==(char), now in fable_string.h) |
| `gate_native_string_edits.py` | CCharString::Truncate 0099F2A0, Erase 0099F400, SaveToFile 0099E5F0 |
| `gate_native_string_numbers.py` | GFIsCharStringNumber 0099E620, GFCharStringToInt 0099E7F0, GFCharStringToUHUGE 0099E870 |
| `gate_native_wide_string_operators.py` | CWideString::operator!=(const CWideString&) 0099B0F0, operator=(wchar_t) 0099B860 (manifest said UnassignString) |
| `gate_native_wide_string_append.py` | CWideString::operator+=(const CWideString&) 0099B8D0 |
| `gate_native_string_unsigned_format.py` | GFUIntToCharString 0099F8B0, GFUHUGEToCharString 0099F930 |
| `gate_native_string_bool_bytes_format.py` | GFBoolToCharString 0099F9B0, GFByteSizeToCharString 0099FA90 |
| `gate_native_wide_string_find_char.py` | CWideString::FindCharPos 0099B070 (manifest said CCharString) |
| `gate_native_system_registry_values.py` | CSystemRegistry GetSLONG 0099CF80, GetBOOL 0099CFE0, GetString 0099D000 (/GS), WriteSLONG 0099D0C0, WriteString 0099D140, WriteBinary 0099D180, GetSize 0099D1B0, DeleteValue 0099D1E0, GetBinary 0099D200 |
| `gate_native_dirty_exit_flag.py` | SetDirtyExitFlag 0099DBE0 (Ego_r body after the read is compiled out: lines 582-630), ClearDirtyExitFlag 0099DC50 |
| `gate_native_error_dialog_resources.py` | RetrieveStringByLanguage 0099D3A0, RetrieveEnglishString 0099D430 (absent from the manifest), RetrieveString 0099D470, LocalizedDialogBoxByLanguage 0099D4D0, LocalizedDialogBox 0099D520 |
| `gate_native_global_virtual_to_actual.py` | GFVCoordToACoord(C2DVector) 009E22C0 (new); GFVXToAX 009E1E10, GFVYToAY 009E1E40, GFVBoxToABox 009E1E70 upgraded from non-exact landings to MATCH |
| `gate_native_error_dialog_lookups.py` | EnumResName 0099D2C0, EnumResType 0099D320 (both absent from the manifest), nErrorStringFromErrorCode 0099DB80, nErrorUrlStringFromErrorCode 0099DBB0 |
| `gate_native_error_dialog_init_strings.py` | InitExternalStrings 0099E260 |
| `gate_native_error_dialog_hyperlink.py` | _HyperlinkParentProc 0099D5B0, _HyperlinkProc 0099D660, ConvertStaticToHyperlink 0099D7D0 (the Microsoft static-to-hyperlink sample, ANSI, Ego_r `MouseOver` global) |
| `gate_native_string_own_copy_or_empty.py` | CCharString::MakeOwnCopyOfDataOrEmptyString 0099EB40 |
| `gate_native_console_list_commands.py` | ConsoleConsoleListContainingString 009EB590, ConsoleCommandList 009EB690, ConsoleCommandListContainingString 009EB760, ConsoleVariableList 009EB8E0, ConsoleVariableListContainingString 009EB9B0 |

68 new roots, plus 3 existing landings upgraded to exact (71 roots in 26 new gates, plus RunTextCommand added to `gate_native_console_command_line.py`). Also superseded: 0099CFE0's fake `CEngineMap::PeekWaterHasType` landing (it is CSystemRegistry::GetBOOL). Legacy fake-struct catalog entries now superseded by genuine roots (retire later):
009EE390 (`CEngineSubPrimitiveText_scalardeletingdestructor_009ee390.cpp`) and 009EEC10
(`TreestdTmaptraitsCCharString_Lbound_009eec10.cpp`).

## Tooling

- `native_exact_roots.run` now also checks that every `??_C@` string literal referenced by a
  root has retail's bytes at its pinned address, and that every `__imp__Name@n` relocation is
  the named entry of the retail import table. Before this, only addresses were checked, and a
  mis-escaped literal (a heredoc ate the backslashes of a registry path) still passed. All 35
  native_exact_roots gates pass with both checks.
- `native_exact_roots.run` accepts relocation addends (a field of a global array,
  `ErrorArray+4`): the target must be symbol + addend. Previously the runner asserted a zero
  addend. All 37 gates re-pass.
- New tools: `tools/decomp_pipeline/native_compare.py` (compile one TU, then an aligned diff
  against retail per symbol; `QFE=1` switches toolset),
  `native_gate_generate.py` (writes a `gate_native_<name>.py` with reloc targets derived from
  retail, cross-checked against symbol_index) and `native_gate_register.py` (adds a gate to
  symbol_index providers/_verified_roots/companions, gate_all, function_overrides and
  ARTIFACT_INDEX, keeping their CRLF). Flow: write TU, then native_compare until MATCH, then
  generate, run the gate, register, then `symbol_index.py --only <TU stem>`.
- Unlanded scans: multi-function TUs are indexed under their primary address, so functions
  landed as unregistered companions (e.g. the CEngine forwarders in
  `CEngine_PrimitiveDispatch_00b23bb0.cpp`) look unlanded. Check the source before starting.

## Findings

- bbblibrary/errordialog.cpp is fully mapped from Ego_r: functions RetrieveStringByLanguage..
  ClearDirtyExitFlag, ErrorDialogProc, _DisplayErrorDialog@8, InitExternalStrings,
  nErrorStringFromErrorCode, EnumResName/EnumResType. Its globals have the same spacing in
  retail as in Ego_r: ErrorMessage 0x13BCA38, GlobalForceExit 0x13BCE3C,
  ErrorMessageTitle 0x13BCE40, ErrorMessageURL 0x13BCEC0, window_globals 0x13BD0C0, pConfig
  0x13BD6E8, DontShowAgain 0x13BD6F4, MouseOver 0x13BD6F8.

- Ego_r CConsole offsets are retail + 0x0C up to the maps, + 0x14 from the colours on
  (Ego maps are 0x10, retail STLport 0x0C).
- Header-inline ctors emitted out of line: a driver under `#pragma inline_depth(0)` with
  `::new (p) T(...)` (CBaseClass has its own operator new) produces the comdats.
- SetGuessMode: the front line is assigned a CCharString through the implicit
  `CConsoleCommandLine(const CCharString&)` conversion. An explicit `CConsoleCommandLine(x)`
  temporary makes VC7.1 address it through the ctor's returned `this` (edi) instead of a
  frame `lea`; a named local changes the destruction order.
- Ego_r CCharString has inline `Equals` (strcmp) and `StartsWith` (strncmp over the
  argument's length), now in `fable_string.h`; StartsWith needs `__forceinline` to expand in
  SetGuessMode. NextCommandGuess calls the CRT strcmp, so its TU has `#pragma function(strcmp)`.
- `CCharString::operator==(const char*)`'s empty case is now an if/return pair (as the
  CWideString one already was): the `&&` list commands branch straight off the inlined empty
  strcmp. `header_byte_sweep` over all 1,442 fable_string.h includers: only the list-command TU
  and three CDefinitionManager TUs changed (CompileDefinition 009AE7C0, CompileString 009AF1E0,
  ParseEnumForSymbols 009ACEE0). None was byte-exact before or after; their six definition/parser
  gates still pass. Ego_r inline `CCharString::Contains` (strstr) added.
- Temporaries in call arguments: an explicit `CCharString("")` filename temporary in the
  CStringParser call makes VC7.1 push the constant last argument first and build the copy of
  the line before it (retail RunCommandLine / ProcessCommandLine / RunTextCommand / RunScript);
  the implicit `""` conversion builds the temporary first. Single-argument calls behave the
  other way: `ReadAsStringUntilString(";", item)` and `RunTextCommand(dst.GetAsCArray())` need
  the implicit conversion (an explicit temporary gets addressed through the ctor's returned
  `this`).
- Layout: `if (!input) error; else process;` puts the error block first (RunCommandLine);
  `if (!PathExists) return;` hoists the `lea ecx,&file` above the branch (RunScript).
- Ego_r inline CCharString::EndsWith (`if (offset >= 0) return strcmp(...)==0; return false;`)
  needs `__forceinline` like StartsWith.
- Ego_r inline `CCharString::operator==(char)`: `if (storage_ != 0) { char text[2] = c,0;
  return strcmp(...) == 0; } return false;` with `__forceinline`. A second byte sweep over
  all 1,447 includers changed only the two new TUs that use it.
- CCharString::Insert 0099F3D0 is near only: retail packs the two-byte buffer into the char
  parameter's stack slot after the MakeOwnCopyOfDataOrEmptyString call. VC7.1 packs it only
  when the buffer is initialised before the call. Not landed.
- FableWin `_fablewin_api.tsv` confirms ctor overloads Ego_r lists poorly:
  `CBasicString(unsigned long, char)`, `CCharStringData(long, char)` (added, the latter
  `__forceinline`) and the *protected* `MakeOwnCopyOfDataOrEmptyString`. Sweeps after the
  CCharString char operators, the CWideString ==/= overloads and these ctors: 0 compile
  failures, and only the new TUs changed.
- lib_global_tools.cpp V->A conversions: `CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
  return (float)(display->GetRenderTargetDimensions().X * value / g_FableVirtualWidth_013961E8);`
  is exact. Binding the extents to a `const C2DExtentsI&` makes VC7.1 read them through the frame
  temporary. GFVBoxToABox then matches by calling the real scalars, so the old
  `Global_GFVBoxToABox_009e1e70.cpp` (private __forceinline copies) moved to
  `work/native-recreation/superseded/`. Its symbol_index provider, its ARTIFACT_INDEX row and
  `gate_native_virtual_box.py` now point at the scalar TU, and that behaviour gate passes
  (1,216 cases). The A->V side (GFAXToVX 009E1DA0 is exact only through a `volatile`
  cast; GFAYToVY, GFACoordToVCoord, GFABoxToVBox) needs the multiply after the call and is
  still open.
- InitExternalStrings: writing the adjusted LangID back to its local (`wLanguageId |= 0x400`)
  changes the whole stack layout. Retail adjusts a register copy
  (`gApplicationLanguage = (id & 0xFC00) == 0 ? id | 0x400 : id`).
- Win32 imports: functions that call one import twice need real `__declspec(dllimport)`
  declarations (VC7.1 then caches the import address in a register, e.g. `mov ebp,[imp]; call
  ebp` in GetBinary). The repo's `g_FableReg*` function-pointer variables reload every time.
- 0099DBE0 (manifest SetDirtyExitFlag) reads ExitFlag into a 16-byte buffer and returns a
  constant 0. That doesn't fit Ego_r's SetDirtyExitFlag, so it is not landed.
- `header_byte_sweep.py snapshot` takes header *basenames* (`fable_string.h`); a path matches
  nothing and reports `sources 0`.
- New `fable_generic_var.h`: Ego_r CGenericVar (0x1C, vfptr first, ValULONG +0x10) and the
  retail 12-byte vector as a specialization of the forward-declared `std::vector`.

## Not landed

- ErrorDialogProc 0099D8C0: near (stack-slot order of its two RECTs in the WM_INITDIALOG
  centring). `work/native-recreation/ui/0099d8c0/`.

- GFCharStringToFloat 0099E690: draft near (x87 allocation: retail keeps bottom_divide in a float
  stack slot). `work/native-recreation/strings/0099e690/`.
- operator+(const CCharString&, char) 0099F7A0: shares the landed 0099F570's residue (refcount
  increment as load/inc/store). `work/native-recreation/strings/0099f7a0/`.
- 009A0880 is a byte-identical second copy of SetAsSubString 0099EF40 (no identity yet).
- 0099B070 is CWideString::FindCharPos (wcschr), not the manifest's CCharString one.

- CConsole::SetHelpBarString 009EB2C0: compares the second parsed symbol with a two-byte
  buffer `{g_013caa48[0], 0}` ("("), short-circuiting on null storage. This is an unidentified
  inline CCharString compare. Notes: `work/native-recreation/ui/009eb430/NOTES.md`.

- CConsole::SetConsole{Text,HistoryWindow,ClientWindow,HelpWindow,ScrollBar}Colour
  009EAA50/80/B0/E0, 009EAB10: best shape matches order and size, but retail loads two of the
  three ULONGs as dwords where VC7.1 emits byte loads. About 15 variants were tried. Draft and
  notes: `work/native-recreation/ui/009eaa50/`.


## Late additions

All three are exact MATCH, gated and registered:

- **symbol_map_error**: CSymbolMap::Error 9E4F80 (/GS, cdecl varargs).
- **symbol_map_load**: LoadFromFile 9E5C90. Clear, UpdateDirty and vector<char>(n) are inlined; the parse-file CWideString static is at 0x013CAA34.
- **symbol_map_parse_buffer**: ParseBuffer 9E5AD0. The CStringParser(const char*) constructor is inlined.

- **symbol_info_vector_growth**: vector<CSymbolInfo>::_M_insert_overflow 9E76F0 is exact only with STLport's own `_M_clear(); _M_set(...)` ending, where the pair copy's shared impl body expands the clear. Also landed from the same TU: _M_clear 9E7250 and __uninitialized_copy/__uninitialized_fill_n 9E5FA0/9E5FE0.

- **date_and_time_misc**: CDateAndTime >= 9E9C40 and <= 9E9E10 (the negated strict compares), Initialise(SYSTEMTIME) 9E9C70, Initialise(CDateAndTime) 9E9D10 (field by field; `*this = x` gives rep movsd), ToString 9E9E30 (`CCharString result(...); return result;`, since VC7.1 has no NRVO).
- **definition_traits_out**: CPersistTraits<C2DVector/C3DVector/C3DAngles/CRGBColour>::TransferOut 9E7F00/9E7F90/9E8050/9E8110 (lib_definition_types.cpp; the text names are C2DCoordF/C3DCoordF). New header fable_definition_type_traits.h.
- **memory_file_byte_vector**: vector<uchar>::push_back 9E9880, back_insert_iterator::operator= 9E98C0 and __copy 9E9900, as roots of the existing CMemoryFile_Copy TU. native_gate_register put an existing TU into `_verified_roots`, which dropped the TU's other symbols from the index; I removed that entry by hand and kept only the companion address map. Unreferenced fastcall `copy` 9E9960 skipped.

- **console_command_line_editing**: 9 private CConsoleCommandLine members (GetString, SetAsUserEntry, IsUserEntry, Decrement/Get/IncrementCursorPosition, RemoveCharBefore/AfterCursor, SetString) at 9EA4B0-9EA960, under Ego_r's private (`AAE`/`ABE`) names. engine/CConsoleCommandLine.h now declares them private, with CConsole as a friend. Inline bodies follow the class unless `FABLE_CONSOLE_COMMAND_LINE_OUT_OF_LINE` is defined; the same scheme is used below.
- **generic_var_lifetime**: CGenericVar ctor 9EA150 (sets Type in the body; an init list doesn't match), dtor 9EA170, scalar deleting dtor 9EA230, copy ctor 9EA360. They're inline in the header (Ego_r lib_generic_var.hpp) unless `FABLE_GENERIC_VAR_OUT_OF_LINE` is defined.
- **console_mode_helpers**: CConsole Is/UnsetGuessMode, Is/UnsetHistoryMode, GetHelpBarString, ConfirmAlternative 9EA600, ResetHistory 9EAA40, plus CConsoleCommandParameters::GetCorrectlyTypedParameter 9EA6C0 (exact only with the inline CGenericVar lifetime). Out-of-line switch: `FABLE_CONSOLE_MODE_OUT_OF_LINE`.

- **console_command_line_editing** (extended): AddCharAtCursor 9EAB40 (Insert, then the expanded IncrementCursorPosition). native_gate_register doesn't extend an existing TU's roots, so I added it to symbol_index by hand.
- **console_history_scroll**: ScrollUpThroughHistory 9EABE0 and ScrollDownThroughHistory 9EAC30. ScrollToHistoryEnd 9EAD10 is in the same TU as a 4-line near match (retail pops esi right after the size test). STLport's fastcall `advance` for list iterators now lives in fable_stlport_list.h; it was a private copy in the ScrollToHistoryBeginning 9EAC80 TU, which still matches.

- **console_binding_commands** (extended): the private AddKeyBinding 9ECB00, AddStringBinding 9ECB20 and RunStringBinding 9ECB40 were already emitted, byte-exact, by the existing binding TU. They're now gated roots.
- **console_command_line** (extended): UserInputComplete 9ECA30, appended to the shared CConsole_CommandLineBuffers TU so VC7.1 auto-inlines ResetCommandLineBuffers/Reset as retail does. Every private CConsoleCommandLine member now has an inline header body (except under the out-of-line switch).
- Tool: scratchpad add_roots.py appends roots to an existing TU's symbol_index `_verified_roots`/`companions` entries, for gates that grow after first registration.

- **console_command_parameters**: CConsoleCommandParameters one-type ctor 9EBB30 and four-type ctor 9EBCB0, added to the old two-type ctor TU (9EBB80, now natively gated). vector<EGenericVarType>::push_back 9EBD90 was already emitted there byte-exact. The `_verified_roots` entry was removed after registration so the TU keeps its full symbol set.

- **console_destructor**: ~CConsole 9ECF30 (606B) and CTBaseSingleton<CConsole>'s scalar deleting dtor 9EDB70. The body is KeyBindingMap.clear(), StringBindingMap.clear(), `it->second.Reset(0)` over RegisteredConsoleInputs, then RegisteredConsoleInputs.clear() and CommandLineHistory.clear(). Reset rather than ReleaseData: Reset clears Info before Data, which is the store order retail shows. The TU expands the owning STLport destructors (three ~map specialisations and list clear/~list). fable_console.h now specialises CTBaseSingleton<CConsole> with a virtual destructor (RTTI vtable 0x0129C44C), because ~CConsole ends by restoring that vtable. The CConsole ctor 9ECD80 still matches, and all 44 units that include fable_console.h compile.

- **wide_string_sprintf**: CWideString::Sprintf 99BA70 (/GS; Ego_r sbuffer wchar[8000] plus dbuffer CArray, used when the format is over 4000 chars) and SprintfDynamic 99BB80. The TU defines UnassignString inline and operator=(const wchar_t*) `__forceinline`, because retail expands both from the same file. SprintfDynamic is exact only with `resize(length + 4096)` computed separately from `size`.

- **wide_string_length_file**: CWideString::SetLength 99BC50 (resize(n) through STLport's out-of-line resize(n, c) 99CD60, now declared in fable_stlport_string.h; a new string is spaces) and LoadFromFile 99BD00. Retail bug: LoadFromFile's default length is `GetPosition() - GetLength()`, the reverse of CCharString::LoadFromFile, so a -1 length request is never positive. Only that operand order matches, so it's kept.

- **wide_string_own_copy**: CWideString::MakeOwnCopyOfData 99B560, MakeOwnCopyOfDataOrEmptyString 99B5E0 and SetAsSubString 99B770. Retail loads the text pointer before `new`, which only matches when the copy goes through AllocStringData(text, -1) defined inline in the TU. SetAsSubString still calls the out-of-line AllocStringData, as retail does.

- **basic_string_char_edits**: CBasicString<char> CopyAndTerminate 9A0040, Truncate 9A0080, Erase 9A0130, and the (length, char) / (text, length) ctors 9A0550 / 9A05E0 (explicit instantiation emits the in-class bodies). Truncate is exact once written through the FableWin helper names: Align, AllocateDataFromStringLength/DataLength, CopyAndTerminate, SetStringLength, ClearStringEnd. The destructor (9A0190/9A0430) is not modelled: declaring one would change every CCharStringData user.
- **Checker fix**: native_exact_roots now requires that a direct call into a retail `jmp [IAT]` thunk names that import. `system_registry_values` GetBinary had been passing with malloc/free where retail calls operator new[]/delete[] (??_U/??_V, separate thunks). Fixed to new[]/delete[] and regenerated. The stricter check also found **console_guess_mode** NextCommandGuess calling strcmp where retail calls `_stricmp` (thunk 0x00BFEBA8; retail imports no strcmp at all). The unchanged-entry test is case-insensitive. It's fixed with CCharString::EqualsCaseInsensitive, the `#pragma function(strcmp)` workaround is gone, the unused case-sensitive Equals helper is removed, and the gate is regenerated. A byte-exact gate had passed with a behaviour bug because both thunks are plain `call rel32` sites.
  Full re-run of all 510 native gates under the stricter checker: 10 failures.
  - Fixed: console_guess_mode (the real strcmp/_stricmp bug), the three `*_global` gates and gfmain (details below).
  - Remaining: the five threaded_file gates (also below).
  The other failures were not caused by this session's changes:
  - gate_native_engine_manager_global, gate_native_progress_display_global and gate_native_shader_manager_global fail the same kind of assertion: they expect a zero-initialised global (e.g. CCountedPointer, 8 bytes; the shader manager, 1000) as raw zero bytes, but VC7.1 now emits it with no raw data (uninitialised). The failure reproduces with HEAD's fable_string.h and without today's list-header edit, so it predates this session. Cause: gate_game_component_abi.Coff was changed to return no bytes for `.bss` sections (PointerToRawData 0), but these gates still compared the raw length. coff_data already yields zeros for `.bss`. Fixed by checking `c.section_metadata[sec]['raw_size']`, which is the same size check; all three pass again (19, 33 and 11 snapshots).
  - The gate_native_threaded_file_* gates fail in their integration tests (e.g. NativeThreadedFileWait_test: the test-only CThreadedFileManager global now gets a synthesised `??0CThreadedFileManager` constructor). This is independent of today's fable_string.h; fable_gfuninitialise.h was last changed 2026-10-15 19:09. Left for the threaded-file lane.
  - gate_native_gfmain stops at "Unmapped GFMain data symbol ??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z": rebuild/integration/native_gfmain/symbols.json (2026-10-02) still names the constructor with the old cdecl `CCharString*` callback (P6AXPAV1@@Z), but fable_string_parser.h has the fastcall `const CCharString&` typedef. I added the current mangled name as an alias for the same retail target 0x00404720; the gate passes again (NEAR_MATCH, 58 scenarios, 5 callbacks).

- **string_parser_readers**: CStringParser ReadLineAsString 9BA390 and ReadAsStringUntilWholeString 9BA440. ReadLineAsString is exact only with SkipPastNewline 9B9D70 defined in the same unit: retail reuses ecx (`this`) after the call because VC7.1 sees, within one unit, that the callee leaves ecx alone. Near matches kept in the TU but not gated: SkipPastNewline, ReadAsStringUntilWhitespace(CParsedItem&) 9BA2D0, ReadAsFloat 9BA650. CParsedItem::InitAsString is now an inline in the header.

- **parsed_item_convert**: CParsedItem::ConvertToString 9B98B0 (a switch with an embedded jump table; case order INTEGER, FLOAT, SYMBOL/STRING, default "NULL") and CStringParser::MoveStringPos 9B9980 (`count > 0`, plain `StringPos += count`). native_exact_roots now accepts `code_size` for roots that end in a jump table.

- **string_parser_quoted**: CStringParser::ReadNextItemAsQuotedString 9BA1D0. The closing quote is the first ' or " not escaped. In the result "\\" becomes one backslash and any other backslash is dropped. The position update is written out directly (signed indices; `escaped` declared before `end`) with CalcInQuotes expanded.

- **string_parser_comments**: CStringParser IsPosInComments 9BA830 (`PStart <= pos && PEnd > pos` operand order), GetNextCommentStart 9BA860 and GetNextCommentEnd 9BA890, walking the comment list through a new minimal STLport iterator in fable_string_parser.h's std::list. native_exact_roots now resolves jump-table `$L` labels by section offset; the old name-based mapping broke when the header change renumbered them.

- **string_parser_helpers**: CStringParser CountLinesParsed 9B95A0 (public, per Ego_r), ReadStringBetween 9B95C0 and CharSeperatesString 9B96A0 (isalpha/isdigit/'_'), plus CParsedItem InitAsSymbol 9B96E0, InitAsString(text, length) 9B9830 and InitAsString(CCharString) 9B9850. The CCharString form is inline for callers unless `FABLE_PARSED_ITEM_OUT_OF_LINE` is defined. All 54 units including fable_string_parser.h compile.

- **string_parser_uhuge**: CStringParser::ReadAsUHUGE 9BA5B0 (64-bit decimal via __allmul; MoveStringPos expanded, CalcInQuotes still a call). Finding: MoveStringPos and ReadAsUHUGE are exact only with CalcInQuotes defined non-inline in the same TU. Within one unit VC7.1 knows it only reads memory, so callers keep or reload StringPos instead of spilling it. A local-copy workaround in MoveStringPos was replaced by this faithful form. Both TUs index only their verified roots, so CalcInQuotes is attributed to its own unit (9B9570) alone.

- **persist_sections**: CPersistContext StartSection 9BAB60 and EndSection 9BABF0, plus the new class CPersistContextWriteSection (Ego_r: Name, StartPosition).
  - Binary save: StartSection writes the name and a zero length slot; EndSection patches in the byte length and restores the position.
  - Text save: "START_" + section.Name + "

" and "END_" + section.Name + "



".
  - All 35 units including fable_persist_context.h compile.

- **data_stream_writers**: 13 CDataOutputStream writers, 993E50-9942D0, in Ego_r lib_data_stream order: UBYTE, SBYTE, CBYTE, UWORD, SWORD, UHUGE, SHUGE, Float, Double, NullTerminatedWString (pointer read before the length), 3DVector/3DAngles (WriteFloat expanded) and AlignData (one `alignment - 1` mask).
  - Supersedes the fake "WriteCBYTE" shims at 993E50/993E80/993E90/993EC0. One of them was a WriteCBYTE that wrote 8 bytes. Like the GetBOOL case, their catalog rows remain to be retired later.
  - Not gated: WriteColour 993F30 (near: retail copies in field order B, G, R, A) and WritePixelFormat 993F60 (CPixelFormat is not modelled).
  - Found with scratchpad bysrc.py: Ego_r functions per source file, flagged against symbol_index.

- **data_stream_string_writers**: CDataOutputStream WriteString 9959F0, WriteStringNewLine 995A20 (WriteString(text), then NewLine's pointer taken into a local before its length) and WritePresizedString 995A80 (data pointer first, then the expanded WriteSLONG(length), then the text).

- **wide_string_compare**: CWideString Contains 99AF20 (wcsstr), <= 99AFB0, > 99AFF0, >= 99B030 (wcscmp), SaveToFile 99B0C0 (characters without terminator, nothing when empty), !=(wchar_t) 99B1D0 (non-empty path first) and GetCRC 99B260 (GetLength expanded, from the STLport string's size).

- **string_not_equal_string / string_hex_to_int**: CCharString::operator!=(const CCharString&) 99E900 (the negated inline ==) and GFHexCharStringToInt 99E750 (skips spaces/tabs; exact with a per-case `digit` and one `value * 16 + digit`).
  - **De-bake**: CCharString_NotEqual_0099e900.cpp in HEAD was a `__declspec(naked)` asm bake (asm_bake_list row "less<CCharString>"). My genuine TU replaced it in place; I wrote the path without first checking it existed, but the result is the wanted de-bake.
  - Its catalog block no longer carries `Grade = 'asm_bake'` and the bake-list row is gone; both edited byte-wise, because `tag_asm_bakes.py --apply` would rewrite the whole catalog with LF endings.
  - The July shim test was rewritten against the genuine class (7 cases, same pass pattern). It passes under VC7.1.
  - The other gated sources that differ from HEAD predate this session.

- **string_data_read_file**: CCharStringData::ReadFromFile 99E4D0 (Ego_r public member; reads into the existing buffer when length > 0).

- **data_input_source_chunk**: CDataInputStream SetPositionAssumeWithinSourceChunk 993C10, CanSeekToPosWithinSourceChunk 993C40 (a bool returned straight from the expression gives retail's `mov eax, 1`) and OpenStream 993C60.

- **buffered_file_stream**: CBufferedFileDataOutputStream, 7 roots: ctor 9944A0 (`FilePos = FileSize = file.GetPosition()`), dtor 9947A0 (`Flush(); delete[] Buffer;` because Close is virtual and isn't expanded from the dtor), SetPosition 994500, GetPosition 994510, Flush 994520, Close 994810 and Write 994870 (the offset is recomputed only on the flush path).
  - The class model is now in fable_data_output_stream.h (Ego_r layout 0x24, vtable 0x0129A710).
  - Supersedes the SetPosition/GetPosition shims at 994500/994510.
  - native_compare reported "diff lines 0" with DIFF because it masks jump displacements; a raw byte compare found the one-byte `jle` difference.

- **CRGBColour copy constructor**: fable_rgb_colour.h's explicit copy ctor now copies in field order (B, G, R, A). WriteColour 993F30 is exact with it (now a root in data_stream_writers); the compiler-generated copy would be one dword move. All 97 gates whose sources reach fable_rgb_colour.h pass with it.
- **data_input_colour**: CDataInputStream::ReadColour 9945E0, a zeroed `unsigned char data[4]` through the inline Read, returned as `CRGBColour(data[2], data[1], data[0], data[3])`.

- **file_path_helpers**: NFile::ConvertDriveLetterToNumber 997470 (a/A = 1, else 0), CAFile GetHDSectorSize 9975B0 / GetDVDSectorSize 9975C0 (0x800, in Ego_r order), ParserIsFinished 9975D0 and IsForbiddenSymbolInComponent 9975E0 (the 7-character table at 0x0129A79C; exact with a `forbidden` flag and `break`). The private statics are declared with Ego_r's access (`CI`).

- **file_split_path**: CAFile::SplitPath, wide 997620 (`_wsplitpath`) and narrow 9976D0 (`_splitpath`, through the out-of-line operator const char*), with Ego_r's stack buffers drive[3], dir/fname/ext[256]. Also MakePath 997780 (`_wmakepath` into path_buffer[260]), SetFilenameExtension 997800 (SplitPath, then MakePath) and GetFilenameFromPathname 997930 (SplitPath expanded, `fname + ext`; VC7.1's inlining matches retail's here). All /GS. NFile::GetItemFromPath 997880 is in the same TU as an ungated near match: 37 diff lines, with the backward-walk loop shape unresolved. The 28 gates reaching fable_filesystem.h pass after its CAFile additions.
- Tools: scratchpad affected_gates.py lists the native gates whose sources reach a header, transitively; run_gates.py runs a list of them. Use these after shared-header edits instead of the whole suite.

- **file_item_from_path**: NFile::GetItemFromPath(const CWideString&, unsigned long, CWideString&) 997B40 (/GS, t_out[260]). It is in its own TU so the character form stays an out-of-line call, as in retail.

- Near matches added this stretch (TUs written, not gated): CAFile::ConvertDirectoryAndFilenameToPath 9979E0 (9 lines; esi/edi swapped. The directory loop tests index GetLength(), the terminator, so it never trims; retail's slip, kept).

- **file_path_parser** (997DA0 ParserIsSeparatorNext, 997E90 ParserReadPastSeparator, 998170 ParserReadPastDriveLabel). Retail expands `CCharString == const char*` in 998170 but also keeps an out-of-line copy (99E960). The new opt-in header switch FABLE_CHAR_STRING_EQUALS_FORCEINLINE makes the operator `__forceinline` for TUs that need the expansion. Separators are tested through the forceinline `== char`.
- **file_syntax_valid**: CAFile::IsFilePathSyntaxValid 998A30, a 512-byte state machine over Ego_r's CExpectedTextType {ComponentOK, SeparatorOK, DriveLabelOK, FinishedOK}, which now has an inline Set in fable_filesystem.h. Retail expands ParserIsFinished, ParserIsComponentNext and ParserIsDriveLabelNext in it and calls the readers out of line. So those three look-aheads (9975D0, 9981D0, 998210) moved into this TU and out of file_path_helpers and file_path_parser. The single-function drive-label TU and its gate are retired; gate_all, symbol_index, ARTIFACT_INDEX and overrides are updated. Loop form: `do { if (finished) break; ... } while (valid); return valid && finished;`.
- Near match: CAFile::ParserReadPastComponent 998010 (CAFile_ParserReadPastComponent_00998010.cpp). Retail keeps `component` in its stack slot and loads its storage once for both "." / ".." compares.

- **file_absolute_path** / **file_absolute_path_wide**: CAFile::GetAbsoluteFilePath narrow 998250 (786 B, Ego_r locals; `ReadAsStringUntilString("\\", item)` with the *implicit* literal conversion, since an explicit CCharString("\\") temp orders the pushes differently) and the wide round trip 998C30. The free `operator+(const char*, const CCharString&)` (99F690) is now declared in fable_string.h.
- **file_queries**: ConvertPathToFullPath(CCharString) 9990B0 (two named locals), GetFileModifiedTimeAsSomeWackyWin32BigNumber 999320 (`_allmul` on ftLastWriteTime), IsFileReadOnly 9993B0, SetFileReadOnly 999420, IsPathFile 999A30. Win32 is declared locally, as in CAFile_GetFileModifiedTime_009992a0.cpp, because the tree has no <windows.h>.
- **file_scan** / **file_scan_strings**: StartFileScan / ScanNextFile, wchar_t* forms 999760 / 999850 (_wfindfirst/_wfindnext; the failure block is last in retail) and the CWideString forms 999C50 / 999CB0 (Ego_r tfilename[261]).
- **file_validate_pathname**: ValidatePathnameIsOnWriteableDrive 999970 expands both the narrow SplitPath and the wide ConvertPathToFullPath. The narrow SplitPath (9976D0) moved here from file_split_path. ConvertPathToFullPath is exposed inline through the opt-in switch FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE. CAFile::DebugUseReadOnlyDrives (0x013BC9F8) is now modelled.
- **file_full_path_wide**: global_ConvertPathToFullPath_00999110.cpp is de-faked. The old candidate passed a default-constructed scratch CWideString where retail passes GetCurrentPath(), and used a stand-in class. It is now `ExpandPath(path, GetCurrentPath())` with the real headers. Its self-contained candidate test is unchanged; build_candidates falls back to the test-only link as before.
- Gate run after the fable_filesystem.h / fable_string.h edits: 242 run, 1 fail. The failure is indexed_raster, `CreateDevice 88760868` (D3D runtime, environmental).

- **file_split_path** (now 7 roots): CreateDirectoryW 9994A0 (Ego_r directory[261]; first GetItemFromPath, then a do-while; drive letter read once into a local, `_chdrive((unsigned short)...)`) and CreateDirectoryForFile 999620. Both expand the wide SplitPath, NFile::ConvertDriveLetterToNumber and ConvertPathToFullPath, so ConvertDriveLetterToNumber moved here from file_path_helpers; that gate keeps three roots. IsRoomForFile 999AB0 is in the TU as an ungated near match: same 415 bytes and instructions, but drive, directory and the expanded temp land in different stack slots, and declaration order and ULARGE_INTEGER-vs-__int64 variants don't move them.
- Pattern: in lib_file_abstract.cpp, retail inlines small same-file helpers (SplitPath, ConvertDriveLetterToNumber, ConvertPathToFullPath, ParserIsFinished, ParserIsComponentNext, ParserIsDriveLabelNext) into whichever caller its heuristics allow. Group callers with the callee bodies they expand, or use an opt-in inline header switch when the callee already lives in another landed TU.

- **file_compare**: CAFile::CompareFiles 998F60 (Ego_r CArray<unsigned char> src/dst buffers of 4 KB, `_STL::min` by reference, inline memcmp). The CArray(n) constructor only inlines with fable_stlport_vector_impl.h included. `(min)` was added next to `(max)` in fable_stlport_vector.h.
- **file_validate_pathname** +GetRelativeFilePath 998570: 1,201 bytes, exact on the first draft. It expands the narrow SplitPath for path1 and calls it for path2, which is VC's own choice inside the TU.
- **file_copy_filename**: GetCopyFilename 997BB0. Three traps: (1) `pow(10.0, int)` picks VC7.1's inline `pow(double,int)` and realigns the stack; retail is `pow(10.0, (double)n)` through _CIpow. (2) The '.' scan reads the copy (basic_name) with the original's length. (3) The tail sits in an `else { }` block so its locals share the loop's stack slots.

- **data_input_vectors**: CDataInputStream::Read2DVector 995030 and Read3DVector 9950F0 (named float locals), Read3DAngles 995210 and ReadMatrix3x4 995340 (1,064 B). The last two assign each member from `ReadValue<float>()`, so every read goes through one shared temporary slot, as in retail.
- **data_stream_writers** +Write2DVector 993F90 (supersedes the `CDataOutputStreamMethods` stand-in, a different mangled name, left in place like the WriteCBYTE ones) and +WriteMatrix3x4 994070 (twelve expanded WriteFloat calls).

- **data_input_colour** +ReadPixelFormat 994670: CPixelFormatInit is zeroed with `memset` (a dword plus a word; aggregate `= {0}` stores a byte plus a misaligned dword) and passed to a new inline `CPixelFormat(const CPixelFormatInit&)` that calls Initialise 0x9E3640. **data_stream_writers** +WritePixelFormat 993F60 (via `CPixelFormat::GetInit` 0x9E3870, now declared).
- **data_input_presized_string**: ReadPresizedString 996390. The length comes from `ReadValue<long>()`, which keeps it in a register like retail. The happy path is in `if (length != 0) { }` with the empty-string return last, and the TU doesn't include fable_stlport_vector_impl.h, so `vector<char>(n)` stays a call to 0x411910.
- **memory_output_copy_to_string**: CMemoryDataOutputStream::CopyDataToString 995E80.
- Deferred: CCompressedMemoryDataInputStream (ctor 995DC0, dtor 995DF0, virtuals near 9949F0). It needs the inlined CDataInputStream ctor (out of line in CDataInputStream_CDataInputStream_00993b20.cpp), a scoped_ptr<CCompressorZlib> and the zlib wrapper at 0xA3A840.

- **bank_symbol_helpers**: CBankFile::SplitSymbol 9CBAA0, IsSymbolPublic 9CBDF0, ConvertPathToFilename 9CBE10. Retail's SplitSymbol copies the whole symbol, not the computed main part, into `*mainsymbol`; the slip is kept and commented.
- **bank_recovery_file**: GetRecoveryFilePath 9CBFE0 (drive + dir + filename + L".rec"; locals declared drive, dir, filename for retail's slot order) and DeleteRecoveryFile 9CC160 (PathExists, DeleteFileW).
- **bank_total_data_size**: GetTotalDataSize 9CC450, in the EntryExists/CountEntries TU, where EntryExists already expands. GetEntryDataSize expands through the opt-in switch FABLE_BANK_FILE_ENTRY_DATA_SIZE_INLINE in fable_game_text_bank.h.

- **resource_user_dirs**: the existing NResourceDirectoryNames_GetSaveDir_0041a540.cpp was genuine but ungated, with all three symbols indexed at 41A540. Their real addresses are GetUserDataDir 41A4C0, GetSaveDirWithoutTrailingBlackslash 41A4F0 and GetSaveDir 41A540. Added GetScoreboardImageCacheDir 41A5B0, GetPhotojournalDir 41A600 and GetTattooUserDir 41A650. **resource_data_dirs**: GetBrushDir 41A0E0, GetTattooDataDir 41A6A0.
- **profile_debug_save_dir** / **resource_debug_save_dir**: CUserProfileManager::GetDebugSaveDir 406780 (`L"Data\\" + GetSaveDir()`) and NResourceDirectoryNames::GetDebugSaveDir 41A1A0.
- **resource_fixed_dirs**: eleven getters under their Ego_r names, placed by Ego_r's address order: GetMiscDir 41A0A0, GetFontBankDir 41A120, GetCacheDir 41A180, GetBoneConfigDir 41A290, GetSoundDir 41A2B0, GetHDDialogueDir 41A330, GetDVDSoundDir 41A390, GetHDSoundDir 41A3B0, GetHDMusicDir 41A3D0, GetDVDMusicDir 41A3F0 and GetStaticMapDir 41A410. They supersede the FableGet*Directory_* and `S::M` stand-ins, which have different mangled names and are left in place. This also settles the header's open GetMiscDir/GetHDMiscDir question.
- After the 176-gate run, the `virtual_box` failure was a timeout under load; it passes when run alone.
- Still deferred: CCompressedMemoryDataInputStream, CBankFileEntryUpdateData::SetFilenames (CSmallVector), IsRoomForFile (stack slots).

- **lib_global_tools virtual/actual conversions**: the C2DCoordI/C2DBoxI overloads GFVBoxToABox 9E1F50, GFABoxToVBox 9E20F0, GFACoordToVCoord 9E2240 and GFVCoordToACoord 9E2330.
  - C2DCoordI is now in fable_display_geometry.h.
  - GFABoxToVBox(C2DBoxF) 9E2020 and GFACoordToVCoord(C2DVector) 9E21D0 were behaviour-verified only. Both are now exact: their `virtualX` helper bound GetRenderTargetDimensions() to a `const&` local, where retail divides through the returned pointer.
  - Integer box forms: `(float)dims.X * long_corner` gives retail's `fild; fimul`. A float or double parameter spills through a slot instead.
  - Gates: global_virtual_to_actual (6 roots), global_actual_to_virtual_coord, global_actual_to_virtual_box.

- **global_2d_distance**: the three GFGet2DDistance overloads, 9E2430 (C2DCoordI), 9E2460 (C2DVector) and 9E2480 (four longs). The old candidate source took pointers to a local struct (mangled `PBUC2DCoordI`); it is rewritten with Ego_r's reference signatures, and its self-contained candidate test is unchanged.
- **global_x_to_y**: GFXToY 9E23A0 (the `x *=` sits inside the returned expression, so both dimension temporaries share one full expression) and GFYToX 9E23F0.
- **global_system_start_time**: GFSetSystemStartTime 9E1CB0. GFGetFastSubFrameTimer 9E1C70 and GFRecalibrateFastSubFrameTimer 9E24B0 use `rdtsc`, which VC7.1 can only emit through inline asm (forbidden), so they are irreducible.
- **global_box_distance**: GFGet2DBoxDistance 9E1AD0/9E1B00 and GFGet3DBoxDistance 9E1B30/9E1B80 (float and integer). The 3D forms declare dz first. C3DCoordI (C2DCoordI + Z) is now in fable_display_geometry.h.
- Near match: GFStringToInt(const wchar_t*) 9E1A80. Instructions are identical; for an empty string VC7.1 threads the `jbe` past the `negative` test where retail lands on it.

- **global_x_to_y**, **global_box_distance** etc. as above. Also:
- **input_event_setters**: all 27 CInputEvent setters, A03BC0–A0415F, in Ego_r order. They supersede the `CInputEventRetail` stand-ins (different mangled names, left in place). Changes to fable_input_event.h:
  - The first union now spans 12 bytes: key / char / {JoystickPos, JoystickButton} / HeldMouseMovement. Mouse-button *held* events copy the movement to +0x00, after the times.
  - The second union holds MouseMovement or JoystickButtonPressureFraction.
  - MousePos sits at +0x18.
  - Store order matters per setter: KeyPressed stores Type, then both times, then device and key; the joystick setters store the button first. The C3DVector copies are whole-struct assignments through reference accessors.
  - GetMouseWheelMovement now reads GetMouseMovement().Z.
  - All 7 gates reaching the header pass, and the other 8 including TUs compile.
- Near match: the CConsole colour commands 9EAA50–9EAB10 (CConsole_ColourCommands_009eaa50.cpp). Retail stores the bytes straight into the member, in an order no tried field-wise assignment reproduces.

- Near matches: CInputEvent::GetMouseMovement A043D0 and GetHeldMouseMovement A043F0 (CInputEvent_Getters_00a043d0.cpp). GetMouseMovement now has Ego_r's by-value signature in the header, inline by default and out of line under FABLE_INPUT_EVENT_MOVEMENT_OUT_OF_LINE. Retail's dword copy needs a C3DVector member in the data unions, which VC7.1 forbids (C2620). The 7 input-event gates still pass after the signature change.

- **system_manager_accessors**: CSystemManager::GetSize 9A5010 (`sizeof`), InitFromExternalApplicationWindow 9A5240 (now `HWND__*`, Ego_r's mangling; the Initialise TU's inline copy and call are retyped with a cast, and Initialise is still exact), UninitFromExternalWindow 9A5250, GetDebugManager 9A52D0 and GetApplicationName 9A5460. Also UninitWindow 9A5210, the private Ego_r symbol (`AAEXXZ`). The two existing `QAE` UninitWindow landings at 0x644627 and 0x79E500 are mislabeled and should be reviewed. All 16 gates reaching fable_system_manager_state.h pass.
- **system_manager_exception_handler**: ExceptionHandler 9A5020. Retail switches on the signed int code (`jg`, not `ja`). The FLT_OVERFLOW case shares the divide-by-zero text, a merged block that is reproduced with the same literal. The gate sets `code_size=0x1d4` for the embedded jump table (the generator does not set it on its own).

- **display_small_queries**: CDisplayManager::CalcPresentationIntervalTime 9BE380 (private), FlushVideoMemeory 9BE400 (EvictManagedResources), GetAvailableTextureMemory 9BE410, GetAVisibleTelevisionScreenArea 9BECB0 (the back buffer) and GetVertexShaderVersion 9BEDE0. Also ReadDeviceRefreshRate 9BE340 (`throw CGenericException()` on a failed GetAdapterDisplayMode; `FAILED` is not defined in this tree, so the test is `< 0`) and CopyBackBufferToTexture 9BF5E0 (through BlitRenderTargetSurfaceToTexture 9BF350, which is declared but not yet reconstructed). ScaleForAntialiasing 9BED80 is a near match: retail keeps the scale on the FPU like a `double` local, but loads float constants.

- **display_antialiasing_mode**: IsAntiAliasingModeValid 9BE3C0 (CheckDeviceMultiSampleType == D3D_OK, format looked up into a local first), added to CDisplayManager_FormatChecks_009be8f0.cpp. The custom display_format_checks behaviour gate still passes. The TU's three functions now index at their real addresses (IsValidRenderTargetFormat is 9BE930, not the TU's 9BE8F0).
- Index lesson: rows only carry the TU's base address unless symbol_index.py has a companions entry for the TU, so gaps.py reports landed functions as gaps. When a gap turns out landed, add the companions entry.

- **display_virtual_coords**: CDisplayManager::SetVirtualCoordsAsResolutionIndependant 9BEA10. Independent means InitVirtualPixels 9BE980 (out of line); otherwise the per-pixel scales are fixed at 65536/800 and 65536/600, floored, and the virtual size is taken from the render target.

- Near match: CSurface::Lock 9F33E0 (CSurface_Lock_009f33e0.cpp). Retail keeps width and height in ebx/ebp across GetDesc and LockRect. Irreducible here: SetViewport(C2DBoxF) 9BF490 rounds with `fistp`, which VC7.1 only emits through inline asm.

- **surface_pitch**: CSurface::GetPitch 9F3390 (GetPixelFormat/GetDimensions expanded as in GetByteLength; the width goes in a named local so it sits in esi across the GetColourDepth call). **surface_render_target**: InitialiseAsRenderTarget 9F3310 (the InitialiseAsDepthBuffer pattern, with width and height read into locals before the device fetch). ReadOnlyLock 9F3010 is Lock's body with D3DLOCK_READONLY and shares Lock's near-match residue.

- Near matches: CSurface::CopyFromMemoryAutoSwizzle 9F3470 and CopyToMemoryAutoUnswizzle 9F3520 (CSurface_AutoSwizzleCopies_009f3470.cpp). An inline GetPixelFormat in the TU reproduces the prologue exactly; the row loop (countdown in the dead parameter slot) does not yet.

- **surface_swizzle**: CSurface::Swizzle/Unswizzle. The CSurfaceLockedRect& forms 9F2E40/9F2E50 are empty on PC; they supersede the `E2` stand-ins there, which have different names. The flag forms 9F4330/9F4360 are Lock, the expanded empty call, then UnlockRect. **texture_swizzle**: the four CTexture forwards 9FA0C0–9FA180, through GetSurfaceLevel with an inline temporary-CSurface destructor. All eight exact on the first draft.

- **texture_swizzle_all**: CTexture::SwizzleAll 9F9FA0 / UnswizzleAll 9FA030, with GetSurfaceLevel expanded through the opt-in FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE.
- Index fix: CTexture_PreallocatedStorage_009fa230.cpp now has a companions entry (InitialiseFromPreallocatedTexture at 9FA230, CalcByteLength at 9F9EE0). New scratch helper add_companion.py creates or extends a TU's companions entry. Follow-up: other multi-function TUs probably still index everything at the TU base address.

- **bank_manager_paths**: CBankFileManager::GetDevBankFilePath 9A7CA0, GetDevHeaderFilePath 9A7E20 and GetRetailHeaderFilePath 9A7ED0 (aliases resolved inline; base path + stored path; empty or a throw when unknown).
  - On an unknown bank, GetDevBankFilePath builds `resolved + " bank not found!"` and throws CGenericException while that temporary is still live: no destructor runs, because nothing is compiled with EH.
  - fable_exception.h gives CGenericException an ignored `explicit (const char*)` constructor and an explicit default one. The two existing behaviour-gated near matches that throw (CreateDevice, ResetDevice) keep identical diffs, and Init and ReadDeviceRefreshRate stay exact.
  - GetRetailBankFilePath 9A7D40 waits on models for the WidePaths set find/insert (9AA4C0/9AAEB0) and CreateRetailBank 9A7720.

- **bank_manager_paths** also covers LookupBankAlias 9A7C60 (`return Aliases.Resolve(handle)`).
- **2D renderer (frontend-first lane)**:
  - **render2d_draw_buffer_adds**: AddLineToDrawBuffer 9DC4C0, AddTriToDrawBuffer 9DC4E0, AddQuadToDrawBuffer 9DC500. Blend mode 1 goes to AddAlphaTri; a quad is (a,b,c) + (a,c,d).
  - **render2d_draw_box**: Draw2DBox 9DE3F0, which is Draw2DBoxWithZ at z 0. Draw2DBoxWithZ itself is still near.
  - **render2d_text**: Draw2DText 9DD8F0 is `Draw3DScaledText(C3DVector(x, y, 0), 1.0f, ...)`. Retail inlines Draw3DScaledText, so Draw2DText lives in the Draw3DScaledText TU (same-file inlining); companions are indexed.
  - **render2d_add2dline** and **render2d_circle**: Add2DLine 9DC2C0 and Draw2DCircle 9DE2A0, landed from the 2026-10-13 work-only exact sources in `work/parity_addtext/`. The circle was tidied (the `0.6666…f` literal is `2.0f / 3.0f`) and is still exact.
- **render_core_draw_nonindexed**: CRenderManagerCore::DrawCurrentBuffersNonIndexed A0AD80.
  - fable_d3d9.h's IDirect3DDevice9 now runs through DrawPrimitive (slot 81) in SDK order, so this is written with real `SetTexture` / `DrawPrimitive` calls instead of the `(*(void***)PD3DDevice)[n]` slot casts its siblings used.
  - fable_render_state_manager.h gained Ego_r's `EPrimitiveType`.
- The `work/parity_addtext/*WORK_ONLY*` JSONs record which earlier 2D results were exact (`complete_relocation_match`) and are the next landing source. Draw2DTriangle, Draw2DBoxWithZ, Draw2DGraphic, the scaled graphics, AddTri and AddAlphaTri are near only.

- **D3D slot-cast de-hack**: fable_d3d9.h's IDirect3DDevice9 now runs through SetFVF (slot 89) in SDK order, and `D3DFMT_INDEX16` has been added. Every `(*(void***)device)[n]` call in the render core and 2D renderer is replaced with a named method: SetTexture, DrawPrimitive, DrawIndexedPrimitive, DrawPrimitiveUP, DrawIndexedPrimitiveUP, SetFVF.
  - Seven exact functions stay exact: A0AEA0, A0AF40, A0AB50, A0AC60, A0ABE0, A0AE10, A0AA30.
  - The near DrawBufferedPolys 9D9C80 and Render2DDrawList 9DA9F0 keep identical diff counts (337/991, 335/901).
- **Callee-swap fix**: AddTriToDrawBuffer and AddQuadToDrawBuffer were first landed with AddTri and AddAlphaTri swapped. That still showed MATCH because neither callee is indexed and the gate generator pins derived callees from the retail site.
  - Retail sends blend mode 1 to opaque AddTri 9DB810 and everything else to AddAlphaTri 9DBD10.
  - native_gate_generate.py now prints `NAME-CHECK` for derived pins whose labels disagree, and it fires on the swapped draft.
  - An audit of this session's derived function pins found no other swaps (see GOTCHAS).

- **Render core (lib_render_manager_core.cpp)**. Retail addresses were placed by matching Ego_r's function order and argument byte counts.
  - **render_core_draw_indexed**: IsRendering A0AA60 (its body is ICF-shared with an inventory query that is indexed under a stand-in name), GetCurrentAWindowInt A0AAB0, both DrawCurrentBuffersIndexed overloads A0ACA0/A0AD40 (primitive type read from the selected index buffer before RealiseRenderState), AsTriFan A0AE40, AsPointList A0AE70.
  - **render_core_transforms**: SetD3DMatrix A0AAD0 (fable_d3d9.h now defines the SDK `_D3DMATRIX`), SetWorldTransform(index) A0B1E0, SetTextureTransform A0B280 (stage table `g_FableTextureTransformStates_0139703C` = 16..23), SetPerspectiveTransform(16 floats) A0B3E0 (SetD3DMatrix inlined).
    - The 3x4 expansion must write the (0,0,0,1) column as floats here, otherwise VC7.1 shares a zeroed register.
    - These setters live in their own TU because the extern table broke the link of the existing renderer_matrices behaviour fixture.
  - **render_core_empty_hooks**: SetDirectionalLight A0A9F0, SetPointLight A0AA00, DiscardTextureManagerBytes A0AA50, OnPostViewportChanged A0AAC0, all empty on PC.
  - MatrixSetters companions are now indexed: SetViewTransform A0B0A0, SetWorldTransform A0B140, SetPerspectiveTransform A0B320.
  - Left in this file: CheckBufferedTextures, which is not yet located in retail.

- **render_target_construct**: CRenderTarget(CSurface&, CSurface&) A10DC0 and SetZBufferAsScreenZBuffer A10EC0 (adopts GetDepthStencilSurface without an AddRef). AttachSurfaces companions are now indexed (AttachTarget A10E60, AttachZBuffer A10E90).
- **Font data bank**:
  - **font_entry_unload**: CFontDataBankEntry::Unload A63580 (an inline `CCountedPointer<CFontBank>::ReleaseData` specialization: release, Info=0, Data=0) and ~CFontDataBankEntry A63480.
    - The entry now derives from `CBaseClassNonCopyable`, per the PDB (the destructor tail-jumps to the folded base destructor at 99A300). The layout is unchanged.
  - **font_entry_acquire**: AcquireFont A634F0 is upgraded from near to exact by reading `Index` before `Bank`.
  - **font_bank_load_font**: CFontDataBank::LoadFont A63350.
  - **font_bank_entry_added**: CFontDataBank::NotifyEntryAdded A636C0 (`while (index >= Fonts.size())`; the entry constructor `(type, index, bank)` is inline).
  - font_entry_unload also covers IsLoaded A63550 (vtable slot 1; indexed under the stand-in `IsSet@T`) and the scalar deleting destructor A63970.

- **2D primitives upgraded from near to exact.** Each had an unaccepted 2026-10-13 work-only draft (81–92%).
  - Draw2DTriangle 9DDFF0, Draw2DLine 9DD960, Draw2DBoxWithZ 9DDC60, Draw2DBoxOutline 9DE140 and Draw2DBoxColours 9DDA50 are each gated (render2d_triangle / _line / _box_with_z / _box_outline / _box_colours).
  - fable_render2d_vertex.h now carries Ego_r's `CGenericVertex::SetZ` / `SetRHWFromZ`, confirmed as PDB members, plus the clamp global `g_FableVertexReciprocalLimit_0139652C` (100.0f).
  - What closed the gaps, all source shape and no fitted constants:
    - Vertices in a `v[n]` array. Retail lays the first vertex lowest; separate locals reverse that.
    - The blend mode as `(flags & 2) ? 2 : 1`, which VC7.1 emits as setne/inc.
    - Field assignments in retail's store order, e.g. RHW last.
    - The left coordinate copied into its local before conversion (`float left = box.left; left = GFVXToAX(left);`), or read through a `C2DVector top_left` for BoxWithZ. Each picks retail's stack slots.
    - Box outline: four `Draw2DLine(...) == true` terms chained with `&&`.
  - Parked: the rectangular Draw2DTexture 9DC6E0 is at 42/99 diff lines with per-vertex order X, Y, Z, RHW, colour, specular, U, V, plus `GFVBoxToABox`, a `v[4]` array and the blend `(flags&2)?2:((flags&1)?3:1)`.
    - Retail's interleaved stores look like Ego_r's `SetXYZ` / `SetColour` / `SetU1V1` helpers. Those are named in the PDB but their signatures were inlined away, so they are not modelled.
  - Draw2DLine clears SpecularColour; the triangle and boxes don't. Draw2DBoxColours' declaration takes `const CRGBColour* const`, per Ego_r's QBV mangling.

- **Display manager**:
  - display_small_queries now also covers the PC no-op hooks ChangePresentationIntervalLockToGivenFrameRateOrLower 9BE330, ReleaseTile 9BEE20 and AllocTile 9BEE30 (returns 0). Ego_r's function order placed them; ProcessScreenshotCapture is ICF-folded into another `ret`.
  - **display_swap_screens**: SwapScreens 9BEEB0 (was landed but ungated) and FlipToGDISurface 9BF2D0, which is `if (ExclusiveMode) while (CurrentScreenIndex != 0) SwapScreens();` with SwapScreens inlined from the same TU.
  - **display_render_targets**: SetRenderTarget 9BF160 and SetRenderTargetAsBackScreen 9BF220 are now genuine CDisplayManager members. They supersede the stand-in "View" class landings, which are left in place.
    - New pieces: a `CDisplayManager::RenderTargetDimensions()` reference accessor over the dimension storage, and Ego_r's inline `CSurface::GetD3DSurface()`. Retail loads the argument before the vtable, which only an accessor call reproduces.
  - Parked de-bake: SetViewport(const C2DBoxI&) 9BEF80 is still the naked-asm bake (stand-in `SetIntegerViewportEndpoint`, used by build_bootstrap.ps1).
    - A genuine C++ draft is at 123/145 diff lines: clamp to the target with right/bottom exclusive, early-out if unchanged, widen zero-size edges and set ViewportOffscreen, SetViewport, cache the int and float boxes, then `if (PRenderManager)` (scoped_ptr safe-bool) plus the shader manager's OnPostViewportChanged.
    - Retail's clamp idiom isn't matched yet. The draft lives in the session scratchpad, not in rebuild/src.

- **New lane: chunked-file mesh/anim chunks** (lib_chunked_file.cpp, lib_3d_mesh_file_*.cpp, lib_3d_anim_file_*.cpp).
  - New shared header `fable_chunked_file.h`. It models CChunkedFileChunk / CChunkedFileSuperChunk with Ego_r virtual order and signatures in retail layout: CArray is 12 bytes, so SuperChunk is 0x2C, not Ego_r's 0x34.
  - Each chunk class is added with its retail vtable addresses. Worksheet tool: `scratchpad/chunkws.py <Class> [slots]`.
  - Gated:
    - chunked_file_base: 11 base virtuals, including SuperChunk::InitNewInstance, which copies RegisteredSubChunkTypes.
    - mesh_submesh_chunk, mesh_transformation_chunk, mesh_bone_chunk, mesh_primitive_chunk.
    - file_container_chunks: mesh Helpers, MaterialList, mesh Root and anim Helpers.
    - mesh_x_leaf_chunks: XTextureIndex, XMappingInfo.
    - anim_file_chunks: anim Root, XAllocationSize.
    - anim_timing_event_chunk, mesh_material_ext_chunk, anim_bone_mask_chunk.
  - Several stand-in landings at these addresses (fake local classes, literal string addresses) are superseded.
  - Idioms:
    - ReadFromFile records `start = stream.GetPosition()` and passes `size - (GetPosition() - start)` on to CChunkedFileSuperChunk::ReadFromFile.
    - CreateNewInstance is `InitNewInstance(new T)` with the constructor inlined.
    - CMatrix3x4::SetIdentity is two chained assignments (zeros, then `E11 = E22 = E33 = 1.0f`).
    - CDataInputStream gained Ego_r's inline ReadEBOOL / ReadULONG / ReadFloat.
  - Second wave, all gated:
    - Anim: anim_xsequence_chunk, anim_movement_vector_chunk, anim_object_chunk, anim_sequence_chunk (info/write/create).
    - Mesh: mesh_x_chunks (XBoundingSphere, XAnimatedPrimitive, XBoundingBox), mesh_map_chunk, mesh_unique_vertices_chunk / mesh_smoothing_groups_chunk (including the scalar deleting destructors), mesh_vertices_chunk / mesh_triangles_chunk, mesh_group_chunk, mesh_convex_volume_chunk, mesh_helper_point_chunk, mesh_dummy_object_chunk, mesh_material_chunk (+ ResetParams), mesh_cloth_chunk, mesh_engine_material_chunk (+ the out-of-line constructor), mesh_partition_chunk, mesh_xtriangle_strip_chunk, mesh_xuv_tangent_chunk.
    - Base: chunked_file_base now also holds both base constructors, ~CChunkedFileChunk / ~CChunkedFileSuperChunk and their deleting destructors, CChunkedFileSuperChunk::WriteToFile (size-patched sub-chunks) and CChunkedFileRawDataChunk.
  - Findings:
    - The const `CArray::GetAsCArray()` is the `size() > 0` form. This only shows for elements wider than a byte; verified across 241 CArray-including gates.
    - Vertex/triangle arrays (types with constructors) inline `resize` as `erase` / `_M_fill_insert` calls.
    - Ego_r's CRGBColour is 4-byte aligned (dword union). The material chunk pads explicitly.
    - Info strings are `"Name: " + GetName()` with Ego_r's by-value getters, or GFFloat/IntToCharString chains evaluated right to left.
  - Still open:
    - The reads of Group, ConvexVolume, HelperPoint, Dummy, Material, Cloth, AnimSequence, XUVTangent, XVertexData and XAnimatedPrimitiveBlock.
    - SuperChunk::ReadFromFile AA8830 (registered-type lookup plus an inlined counted-pointer push_back) and RemoveAllSubChunks AA8400 (needs the class-element erase).
    - The X-chunk scalar deleting destructors (retail inlines an implicit destructor without a vptr reset; not reproduced).
  - Third wave: the ReadFromFile functions of HelperPoint AA9BA0, DummyObject AA9E60 (version-gated transform and flag), Material AC0010, ConvexVolume AAAB10, AnimSequence AAF850, Cloth AC6750 and XUVTangent AA6A60 are exact, each added to its class gate.
    - AnimSequence's read is `clear(); reserve(0);` then push_back from a named local, with the bone type read only if bytes remain.
    - ConvexVolume reuses the inline-resize specialization.
    - fable_stlport_vector.h gained STLport's front()/back().
  - Group read ABCC80 is a near draft (27/206: register allocation, plus a `this` reload around back()). It is kept in the session scratchpad, out of the TU. The erase there is STLport's element-by-element `__copy` path, inlined.
  - Near, left out: a lone ULONG read into a member (XTextureIndex AA4D80, XAllocationSize AA4570, MaterialExt AC02D0). Retail's inline-Read range checks jump to a different one of three identical error tails.

Parked: ParseEnum 9E55A0 (work/native-recreation/strings/009e55a0/NOTES.md); CPersistTraits<C2DVector>::TransferIn 9E81E0 and its C3DVector/C3DAngles/CRGBColour siblings (work/native-recreation/persist/009e81e0/NOTES.md); CWideString operator+ with wchar_t 99C080/99C120 (strings/0099c080); GFFloatToCharString 9A08B0 (strings/009a08b0; retail's helper inlining is uneven).

## Stop point (bedtime)
- Landed and gated exact: XUVTangent read AA6A60 and XVertexData (CVertexCompressionParams write AA4D20 and read AA5460; chunk write AA58C0, create AA59C0, read AA66B0).
- Group read ABCC80 is still a near draft and is kept out of its TU.
- New lane: CHandleBasedMemoryPool area helpers, in `rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp`. **Drafted but not registered or gated yet.**
  - Exact: CHeader ctor A01D20, GetHandleFromPointer A01D40, CreateArea A01D50, FindLargestFreeArea A01F50, UpdateLargestFreeArea A01F80, ValidateLargestFreeArea A01FE0.
  - FindFreeArea A01ED0 is near: 18-19/46 lines, register allocation around the free-list-index helper.
  - Next: land the six exact functions (park FindFreeArea), then TestIfAreaIsGrowable A02020 and the rest of the A02xxx range.
  - Unnamed out-of-line free-list-index helper at A01D00; RemoveFreeArea has an out-of-line copy at A01E00.
- The fable_stlport_vector.h front()/back() addition had its affected-gate run (340 gates) interrupted by the shutdown. Re-run it before committing.

