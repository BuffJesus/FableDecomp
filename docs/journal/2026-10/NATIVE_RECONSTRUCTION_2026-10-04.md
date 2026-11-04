## Definition binary loading recovered

LoadDef (0x009AD6E0) compiles to 550/526 bytes (behavioral). Seven linked fixtures and retail executions cover already loaded/unlisted/current/stale resources, missing classes, new binary loads, reentrant replacement after decompression and MakeFreeSpace failure. Existing defs only update MRU when their list frame differs. New defs report progress, decompress, look up the class, request payload-size + 37 bytes, advance the resource frame, allocate/name and internally load the instance, transfer binary fields, register its resource size and add it to the manager list. Retail ignores MakeFreeSpace's return value; the recovery preserves this behavior. Reset releases a reference introduced during decompression.

The linked gate uses native binary CPersistContext::Init, SetResourceSize and the typed CMemoryDataInputStream destructor. String storage, stream constructor/vtable, decompression, class lookup, allocation/virtual transfer, progress and resource list operations are mocked. Typed stream destruction matches 11 bytes after relocation (CDataStream vptr, then base destructor tailcall).

Selected closure after nested compilation/instantiation: 1,174 sources, zero duplicates, 445 unresolved, saved in work/codex-native-startup/native-gfmain-definition-subcompile-closure-20261004. This predates LoadDef and the typed stream destructor. Continue native engine recreation; Lua and FMP analysis remain excluded.

## Nested instantiation and compiler composition

InstantiateSubDef (0x009ADAE0) compiles to 404/417 bytes (behavioral). Four linked cases and retail executions confirm canonical class metadata, unset instance name (-1), Template clearing, parent AddSubDef arguments, global/class indices and retained returned/stored references. Linked fixtures execute shared vector growth, including the first class entry; cleanup releases both references and verifies final deletion. Class lookup, allocator/vtable, interning and parent LoadDef/AddSubDef remain mocked. Missing classes still load and briefly retain the parent before returning a null child, matching retail.

Main compilation now has an additional composed gate linking real CompileSubDefinition (21 native providers total). Seven cases pass. Its nested block produces a separate child transfer and unknown-symbol check before the outer transfer; no CompileSubDefinition mock remains in that gate. Child instantiation is still mocked there and separately checked by the new instantiation gate.

## Nested definition compilation recovered

CompileSubDefinition (0x009AE150) compiles to 558/568 bytes (behavioral). Four linked fixtures and retail executions agree on new/local/inherited children and a failed new child, transfer defaults, loaded indices, parent-entry removal, copy order, Setup/DefaultValsApplied writes and retained reference counts. Four native providers are linked; definition loading/instantiation, virtual operations and unknown-symbol reporting are mocked. Inherited-child replacement does not add a null guard before Copy, matching retail's assumption that instantiation succeeds on that path.

The selected main-definition closure now has 1,172 sources, zero duplicates, 446 unresolved, saved under work/codex-native-startup/native-gfmain-definition-body-closure-20261004. This precedes the nested-definition provider. Continue InstantiateSubDef and LoadDef.

## Main definition-body compilation recovered

CompileDefinition (0x009AE7C0) compiles to 1414/1456 bytes (behavioral). Seven linked cases using twenty native providers agree with seven retail executions: missing instance-name lookup, first/defaulted transfers, ready/missing/not-ready parents, nested blocks, retained refcounts and parser positions. The oracle caught and corrected the specialization guard: it tests parent Setup (bit 0), not Template (bit 1). Copy clears Setup and preserves the original target Template flag while retaining copied DefaultValsApplied. Transfer then sets Setup and DefaultValsApplied. Nested blocks are blanked except LF bytes before the remaining body is transferred.

Definition Load/Copy/Transfer, nested-definition compilation, unknown-symbol checks, symbol lookup, string storage and SetChar remain mocked in this composed fixture. CheckUnknownSymbols has a separate native composition gate; CompileSubDefinition and LoadDef are the next frontiers. This is behavioral recovery, not complete engine compilation.

Typed CDefString conversion at 0x00415DD0 matches 25 retail bytes after relocation and uses donor nonconst conversion signature. Typed CCharString::SetChar matches 32 bytes and passes three null/private/shared-storage delegation fixtures with MakeOwnCopyOfData mocked. Historical mislabeled providers are retained; these typed providers supply the real native symbols.

## Unknown definition fields and parser copy ownership

CStringParser copy construction (0x0046381B) now preserves source pointer, names, callback and all three cursor words while allocating a separate comment-list sentinel and nodes. Three linked fixtures (0/1/3 nodes) check independent list topology, boundary-pointer copying and destruction; retail execution confirms matching fields and allocation counts with mocked string copies.

SkipUntilWhitespace and ReadAsStringUntilWhitespace are linked into the delimiter gate, now 48 retail differential cases. CheckUnknownSymbols (0x009BBBF0) links seven native providers and passes five retail comparisons: empty/whitespace input, multiple remaining fields, and whitespace splitting even inside quotes. Diagnostic checks run on a parser copy and preserve the original parser's entire 40-byte state. String storage and PersistError formatting are mocked in this composed test; the separate persistence formatting gate checks its callback contract. Compiled CheckUnknownSymbols is 196/207 bytes (behavioral).

## Substring wrapper parity and remaining-input check

ReadAsStringUntilString (0x009BA330) and ReadAsStringUntilPastString (0x009BA3E0) each match 81 retail bytes after relocation. The delimiter gate now runs 32 differential linked cases across four functions. NextItemExists (0x009BA4F0) has a behavioral implementation (55/77 bytes) passing five retail comparisons. Success restores all three cursor words; EOF retains whitespace advancement, matching the lookahead family's success-only restoration.

Current selected closure (before latest delimiter wrappers/remaining-input): 1,161 sources, zero duplicate providers, 446 unresolved. Path: work/codex-native-startup/native-gfmain-definition-delimiter-persist-closure-20261004. Continue definition-body compilation and unknown-symbol diagnostics; parser copy/list ownership is needed for the latter.

## Definition-body delimiter helpers recovered

SkipUntilString, SkipPastString and ReadAsStringUntilString have behavioral providers. Twenty-four linked comparisons agree with retail execution on substring extraction, missing delimiters (cursor moved one past the terminator), immediate EOF, empty needles, quote transitions and unchanged line counters. Five native sources are linked; string storage is mocked. SkipUntilString compiles to 112/125 bytes.

Donor Ego_r and FableWin public symbols confirm ?PersistError@CPersistContext@@QBEXABVCStringParser@@ABVCCharString@@@Z. Corrected the provisional Error name to public PersistError const; persistence and composed enum gates still pass. CStringParser donor class methods independently confirm the delimiter names/signatures. Prior creature-serialization label at 0x009B9BA0 is corrected; historical source remains preserved.

## Enum expressions linked through recovered persistence

The composed enum gate links eighteen native sources, including real SLONG expression transfer. Eleven linked fixtures match eleven retail executions, including incrementing values after right-associative subtraction and shift/or/add expressions. The former expression-transfer mock is removed from this composed check; string storage, symbol maps/sorting and error reporting remain boundaries. Run tools/decomp_pipeline/gate_definition_enum_expressions.py.

## SLONG expression input and conditional consumption recovered

ReadAsInteger matches 111 retail bytes after relocation and passes three linked return/error cases. SkipPastNextItemIfIs passes eight linked cases against retail execution: case-sensitive prefix comparison, partial mismatch restoration, empty input matching, whitespace, quote-word padding and quote toggling. Behavioral compiled size 173/217.

CPersistTraitsSLONG::TransferIn compiles to 564/511 bytes (behavioral). Eleven linked cases using thirteen native providers agree with retail execution: named/missing symbols, right-associative |/+/- chains, signed right shifts, masked counts and 32-bit arithmetic wrap. String storage, symbol lookup/sort and error reporting remain mocked. CPersistContext::Error has a typed behavioral body (170/170 bytes; no asserted byte parity) with two callback/line formatting fixtures. Corrected 0x009BAA90's prior definition-manager label using its +0x0C context callback access and caller evidence.

Selected startup closure after enum/CompileString/instantiation: 1,157 sources, zero duplicates, 447 unresolved. Saved under work/codex-native-startup/native-gfmain-definition-enum-instantiation-closure-20261004. This predates integer-read/conditional-consume/SLONG/error provider indexing.

## Definition instantiation recovered

InstantiateDef at 0x009ADC90 compiles to 380/382 bytes (behavioral). Five linked fixtures and five retail Unicorn executions agree on missing classes, leaf/parent defs, flag preservation, canonical class names, global/class indices, symbol publication and retained refcount. Linked fixtures execute shared vector growth code, including first insertion into empty vectors; the retail oracle mocks growth/allocator/map/string boundaries. The stored info owns one reference after the temporary pointer releases its initial reference; fixture cleanup verifies final deletion.

CDefClassInfo's retail size is 20 bytes (name and allocator plus the 12-byte global-index vector); its earlier 16-byte comment was corrected. CompileDefinition and SLONG expression transfer remain frontier work.

## Native enum and definition compilation scans recovered

ParseEnumForSymbols compiles to 1000/1017 bytes (behavioral). Eight linked fixtures use fifteen native sources and agree with eight retail Unicorn executions under string/map/error/expression boundary mocks. This preserves named/anonymous groups, explicit integer values, incrementing values, separator diagnostics, EOF behavior and omission of an unassigned final name followed by a closing brace. Retail comparison corrected the diagnostic to name the enum entry rather than the separator.

CompileString (0x009AF1E0, formerly provisional LoadDefinitions) compiles to 744/670 bytes (behavioral). Six cases linked to eighteen native sources and six retail executions confirm template-first scans, exact case-sensitive class filtering, empty filter and empty input. CompileDefinition itself remains a mocked frontier.

Both donor symbol sets confirm const source pointers and a const parser reference for manager Error. Shared parser Init/source fields and dependent fixtures are reconciled; the path-list gate still passes. CParsedItem ConvertToString dispatch passes five cases with six-digit float precision and NULL for unrecognized tags; formatter boundaries remain mocked.

## Parsed-item accessors recovered

Typed GetSymbol/GetString/GetInteger/GetFloat providers match four retail bytes each (16 bytes total), using donor signatures and enum-parser call evidence. Existing unrelated accessor labels have identical leaf bodies and may represent folded aliases; retain historical implementations while adding the typed parsed-item symbols.

Selected GFMain token-reader closure checkpoint: 1,150 sources, zero duplicates, 448 unresolved. This was built before the latest lookahead, symbol wrapper and definition-error providers; it is not an updated closure for those additions.

## Definition errors, lookahead and symbol wrapper

Definition manager Error passes two filename/line formatting cases (173/175 bytes; behavioral). Unlike the parser Error method it calls its configured callback unconditionally, matching retail.

PeekNextItem/PeekNextItemType pass four cursor-state cases. Success restores all three cursor dwords including quote-word padding; failure retains token-reader side effects. Behavioral grade (54/57 and 90/101 bytes respectively). Global EParsedItemType identity is preserved for the donor return signature.

ReadAsSymbol at 0x009BA790 matches 145 retail bytes after relocations and passes three token/error return paths. Corrected the earlier duplicate ReadAsString label with donor and retail evidence.

## Numeric converters and substring setter recovered

Recovered CParsedItem InitAsInteger/InitAsFloat with 19 differential retail Unicorn fixtures at x87 control word 0x027F, comparing numeric type and exact value bits. Cases include empty text, negative values, integer wraparound, negative zero, malformed separators and fractional precision. Behavioral grade (80/103 and 472/181 bytes); no byte parity and no claim of arbitrary-input floating-point equivalence.

CCharString::SetAsSubString at 0x0099EF40 matches all 45 retail bytes after its two calls are relocated. Five linked cases check old-reference release, null/empty text and preservation of explicit lengths including zero and -1. The setter delegates to already recovered UnassignString and AllocStringData.

## Error formatter and token scanners recovered

Parser Error matches 170 retail bytes and passes three filename/line/callback fixtures. Identifier and number scanners pass twelve linked consumption cases (189/191 and 240/223 bytes respectively; behavioral), with string storage/numeric conversion boundaries mocked. Numeric conversion helpers are drafted but not accepted pending differential execution checks, especially x87 rounding.

All ten current parser/definition gates pass after callback and access-signature reconciliation. Fixed symbol_index.py's parallel object-name collision for sources sharing an address: object names now include a hash of the source path. A targeted 18-source refresh completes with the expected typed signatures.

## Whitespace and single-character tokens recovered

Shared-layout SkipWhitespace passes seven cursor/line cases (121/124 bytes; behavioral). ReadNextSymbol passes five end-of-input, punctuation and quote cases (86/85 bytes; behavioral). The correctly typed protected const CalcInQuotes helper matches 43 retail bytes. Retail token tags are symbol=1, string=2, integer=3 and float=4, verified at the writers.

Retail Error's indirect callback uses ECX, confirming a fastcall callback taking a const CCharString reference. The shared parser declarations and constructor were corrected; typed Init and symbol-scan gates still pass, as does the existing constructor fixture. Refresh affected symbol-index entries because the corrected callback changes decorated names.

## Token dispatch recovered

`CStringParser::ReadNextItem` (0x009BA080) compiles to 136 bytes, the retail length, with differing register allocation (behavioral grade). Nine linked cases cover digits, negative numbers, standalone signs, leading decimal points, identifiers, underscores, quoted input and end-of-input. These validate dispatch; whitespace/token-reader boundaries are mocked. Donor names confirm protected ReadNextNumber/ReadNextString/ReadNextSymbol members and long-returning SkipWhitespace.

## String token wrapper recovered

`CStringParser::ReadAsString` (0x009BA6F0) matches 145 retail bytes after 11 relocations. Three linked cases cover string success, a non-string token, and failed token reading. Failure cases verify the error text and empty returned string. CParsedItem's 12-byte layout and string member at +8 are confirmed by donor layout and retail access. Next: ReadNextItem and parser Error. A fresh GFMain-only closure selected different historical candidates and produced 16 duplicates; it is not comparable to the established selected closure. Rebuild the latter from its saved source list for authoritative progress.

## Typed parser initialization recovered

`CStringParser::Init` at 0x009B9530 now uses the shared retail layout and matches all 55 bytes after the assignment call is relocated to 0x0099EFB0. Two linked cases check filename assignment, callback/base-line initialization and preservation of the vptr, temporary string, comment list and cursor padding. The legacy candidate's fake 12-byte string class remains historical; the new typed source provides the symbol needed by native parser callers.

## Definition symbol scan recovered

Recovered `CDefinitionManager::ParseStringForSymbols` (0x009ADE90). VC7.1 emits 700 bytes versus 701 retail bytes; behavioral grade. Two linked orchestration cases check directive pass order, template flag, resets, enum-only mode and the missing-class error string. Token/string and instantiation boundaries are mocked; this does not establish a complete working definition compiler. Next: typed Init/ReadAsString, ParseEnumForSymbols and Error providers, then the definition compilation parser.

## Whole-string parser search recovery

Recovered typed `CStringParser::SkipUntilWholeString` (0x009B9AA0) and `SkipPastWholeString` (0x009B9C00). Six linked cases and six retail Unicorn executions agree for exact keywords, identifier boundaries and the template-prefix collision. CRT/string-conversion boundaries are mocked with ASCII cases; locale-specific classification is not covered. Implementations are behavioral, not byte parity. Registered dedicated gate and corrected the latter routine's prior creature-serialization identity in the override table and manifest.

## String parser position recovery

Recovered native `CStringParser::SetStringPos` (0x009B9940) using the retail parser layout. Quote parity toggles on either single or double quote before the requested position; line/base offset fields remain unchanged. Ten linked cases pass. VC7.1 emits 53 bytes versus 62 retail bytes: behavioral grade. Dedicated gate: `gate_string_parser_position.py`.

The definition parser scan searches `#definition_template`, `#definition` and `enum`, confirmed from literal retail addresses. The existing provider labelled creature serialization at 0x009B9C00 is actually part of this parser path; its argument is CCharString and the +4 length field is CCharStringData length. Its callee 0x009B9AA0 is identified by donor audit as `CStringParser::SkipUntilWholeString`. Correct identities must be reconciled before replacing providers.

# Native recreation, 2026-10-04

## Wide filename sort helpers recovered

Recovered the bounded insertion pass (0x009B68D0), unguarded insertion
pass (0x009B2AC0), and introsort loop (0x009B7CF0). Their 200, 111, and
217-byte VC7.1 bodies match retail after call relocations. Nine linked
ordering and depth-limit cases pass, with three provider omissions checked.
The insertion source includes the linear-insert body so VC7.1 expands the
same below-first shifting path as retail.

Recovered the Hoare partition (0x009B3580, 146 bytes) and partial-sort
fallback (0x009B6A40, 130 bytes), also exact after relocations. The partition
temporary is destroyed before advancing the iterator, as retail does.
Linked partition and partial-sort contract cases pass with two provider
omissions. Heap construction, adjustment, and sorting remain external
dependencies of the fallback; the fixtures isolate those boundaries.

The five recovered helpers total **804 retail-matching bytes**. Gates:
`gate_widestring_sort_helpers.py` and `gate_widestring_partition_fallback.py`.

Selected GFMain closure:
`work/codex-native-startup/native-gfmain-definition-partition-fallback-closure-20261004`:
**1,134 sources, zero duplicates, 448 unresolved**. The five helper
dependencies are resolved; the next sort dependencies are the wide-string
heap construction, adjustment, and heap sorting routines.

## Definition path scans and wide filename sort

Retyped `CDefinitionManager::CreateSymbolsFromPathList` (0x009AEF60) and
`DoCompilePathList` (0x009AF480). Both enumerate each path pattern, convert
scanned filenames to lower-case wide strings, sort within that pattern, read
each file, remove line and block comments, set `CurrentFilename`, and pass
the resulting buffer to the appropriate parser. The symbol pass updates both
symbol maps after all patterns; the compile pass forwards the prefix and
returns immediately for an empty path list. A linked fixture tests three
patterns, six parsed files across the two passes, empty path lists, the
per-pattern file order, parser text and arguments, and three omitted-provider
links. Both methods compile with VC7.1 and the fixture passes.

The emitted bodies are 616/625 and 589/605 bytes, with 0.811 and 0.780
normalized opcode similarity respectively. These are behavioral recoveries,
not byte matches. `GFRemoveComments` is linked as a real provider. The
companion STLport `sort<CWideString*, less<CWideString>>` wrapper at 0x009B8440
emits 112/117 bytes and passes an isolated four-case helper-routing fixture;
its three out-of-line sort helpers remain on the frontier.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-definition-wide-sort-closure-20261004`:
**1,129 sources, zero duplicates, 448 unresolved**. The two path-method
dependencies and wide sort wrapper are resolved; the frontier now includes
`ParseStringForSymbols`, the 0x009AF1E0 definition parser, and the three
wide-sort helpers. The provisional `LoadDefinitions` name for 0x009AF1E0
needs confirmation from its body.

The extracted community SDK in `D:/Downloads/FableTLC-SDK-Inspection` was
reviewed for relevance. Its reports identify candidate hero offsets and
runtime hooks, but contain no game engine source or definition compiler
symbols. Those offsets remain unverified against the retail executable.

## Definition source comment removal recovered

Retyped the native `GFRemoveComments` helper at 0x00A60410 used by the
definition manager's symbol and compile path scans. The helper strips repeated
delimited spans, retains the counted line feeds, leaves an unterminated span
untouched, and supports an empty closing delimiter for a trailing comment.
The retail code also counts the byte immediately after a closing delimiter;
the typed version preserves that observed behavior. All eight cases were
confirmed against the retail routine in a Unicorn x86 oracle with CRT calls
hooked. VC7.1 emits 262 bytes
against 245 retail bytes, with 0.839 normalized instruction similarity, so
this is a behavioral recovery and is not byte parity. A linked eight-case
fixture and omitted-provider link check pass through
`tools/decomp_pipeline/gate_remove_comments.py`.

The linked `CreateSymbolsFromPathList` and `DoCompilePathList` bodies remain
the next definition compiler frontier. The `ultracodex` repository reviewed
today is an agent workflow runner; it provides no Fable code or native oracle.
The existing VC7.1 parity and linked fixture gates already implement the
useful verification pattern for this project.

## Bank-info map index recovered

Retyped `CBankFileManager::CBankInfoMap::operator[]` at 0x009AC5D0
and its default `CBankInfo` value: four empty wide strings and a null
registered-bank pointer. The VC7.1 body matches all 302 retail bytes
after 21 call relocations. A linked fixture uses the real bank-info
lower-bound provider and checks existing-key lookup, insertion hints,
default value fields, four wide-string constructions and copies, eight
wide-string destructions, and the string-key lifetime. Omission of
either the index or lower-bound provider prevents linking.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-bank-info-index-closure-20261004`:
**1,112 sources, zero duplicates, 450 unresolved**. Its bank-map
frontier is the bank-info hinted `InsertUnique` method.

## Retail-bank alias insertion chain

Retyped the alias-map iterator advance (0x009A9860) and retreat
(0x009AA320). Advance emits 61 against 62 retail bytes with 0.881
normalized opcode similarity; retreat emits 79/79 bytes with identical
normalized opcodes. A linked fixture checks both traversal directions
through the end sentinel and inner subtree nodes.

`CBasicRetailBankAliasMap::InsertNode` at 0x009AB2F0 emits the exact 250
retail bytes after eight call fixups, including 0x18-byte node allocation,
two string copies, link updates and rebalancing. Unhinted `InsertUnique`
at 0x009AB3F0 matches all 254 bytes after five call fixups. Hinted
`InsertUnique` at 0x009ABDB0 emits 672 bytes against 668 retail bytes,
with 0.956 normalized opcode similarity. A composed fixture links those
providers to the real alias `operator[]` and lower-bound walk, then checks
insert, duplicate and wrong-hint behavior. Seven omitted-provider links
fail as expected.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-retail-bank-alias-insert-closure-20261004`:
**1,110 sources, zero duplicates, 450 unresolved**. Bank-info `operator[]`
is the remaining bank-map frontier item.

## Contained-bank InsertUnique chain

Retyped `CContainedBankMap::InsertUnique(iterator, value)` at 0x009AB870
using the recovered STLport hinted-insertion branch structure. The typed
VC7.1 body emits 672 bytes against 668 retail bytes, with 281/282
instructions and 0.956 normalized opcode similarity. It calls the real
iterator advance/retreat and node-insertion providers. A linked fixture
checks empty, duplicate, begin, end, interior and wrong-hint cases.

Retyped the unhinted `InsertUnique(value)` fallback at 0x009AB020. Its
254-byte body matches retail after five call relocations. A linked fixture
checks ordered inserts, the returned inserted flag, duplicate detection
and preservation of the original value. The hinted fixture now links
this real fallback. The two gates pass with six and four omitted-provider
checks respectively.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-contained-bank-insert-complete-closure-20261004`:
**1,105 sources, zero duplicates, 451 unresolved**. Bank-info `operator[]`
and alias-map `InsertUnique` remain on the bank-map frontier.

## Contained-bank insertion prerequisites

Recovered three retail callees of the contained-bank hinted insertion
path. `CContainedBankMap::iterator::operator--` at 0x009AA2D0 emits
79/79 bytes with 39/39 identical normalized instructions. Four bytes
differ through register selection, so it is a near match. Linked cases
cover end-to-maximum, rightmost-left-subtree descent and parent ascent.

`CContainedBankMap::CreateNode` at 0x009AA9E0 corrects a false
`CCreatureAction_NavToPos::Clone` label. Typed allocation plus placement
construction of its 0x18-byte value matches all 76 retail bytes after
two call fixups. A linked fixture checks the 0x28-byte node allocation,
string copy, and five retained bank fields.

`CContainedBankMap::InsertNode` at 0x009AAF70 matches all 173 retail
bytes after four call fixups. The fifth argument is a full-width word in
retail. Its linked fixture inserts root, left and right nodes using the
actual node factory and red-black rebalancing implementation, checking
header links, parent links, count, value fields and root color. The
iterator, factory and insertion gates include provider-omission checks.
The three providers are indexed. The selected GFMain closure at
`work/codex-native-startup/native-gfmain-contained-bank-insert-prereqs-closure-20261004`
has **1,099 sources, zero duplicates and 452 unresolved**. The providers
remain outside it until the parent `InsertUnique` is recovered.

## Alias-map index and contained-bank iterator

Retyped `CBasicRetailBankAliasMap::operator[]` at 0x009AC700 to use its
actual lower-bound and hinted insertion members. The explicit iterator
copy constructor yields a 150/149-byte VC7.1 near match with 0.945
normalized opcode similarity and 54 retail versus 56 candidate
instructions. A linked fixture checks empty, equal, lower and higher
keys, and omission of either the caller or lower-bound provider fails
linking. `gate_retail_bank_alias_index.py` passes.

Retyped the contained-bank iterator advance called at 0x009A9820 from
the hinted insertion body. It emits 61 bytes against 62 retail bytes,
with 0.881 normalized opcode similarity. A linked fixture checks the
next parent, leftmost node of a right subtree, and transition from the
maximum node to the end sentinel; its omitted-provider link fails.
`gate_contained_bank_iterator_advance.py` passes. This helper is indexed
but remains outside the selected closure until its insertion caller is
recovered.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-retail-bank-alias-index-closure-20261004`:
**1,099 sources, zero duplicates, 452 unresolved**. Its frontier has
both alias and contained-bank `InsertUnique` methods.

## Bank-map lower-bound walks

Retyped `NBankFileManager::CContainedBankMap::LowerBound` (0x009AAD60),
`CBankFileManager::CBankInfoMap::LowerBound` (0x009AADB0) and
`CBasicRetailBankAliasMap::LowerBound` (0x009AAE00). Each typed VC7.1 body
matches all 65 retail bytes after masking its one call relocation to
`CBasicString<char>::operator<`. The linked fixture checks empty trees and
four search positions in each map; omission of any one provider prevents
linking. The old false STL-map catalog entries and six source/test files
were removed, and the symbol index and manifest now carry the actual
class methods. `gate_bank_maps_lower_bound.py` passes.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-bank-lower-bound-closure-20261004`:
**1,097 sources, zero duplicates, 452 unresolved**. The next bank-map
frontier includes contained-bank `InsertUnique` and bank-info/alias
`operator[]`.

## Retail-bank lifetime and contained map

The retail `CCountedPointer<NBankFileManager::CRetailBank>::DeleteData`
callback at 0x009A90B0 is byte-identical across all 11 bytes. Its linked
fixture verifies null handling and virtual deleting-destructor dispatch.
The `CRetailBank` vtable at 0x0129B010 points to 0x009AC7A0, correcting a
false landscape-patch label. VC7.1 emits its scalar deleting destructor as
an exact 30-byte match after two call fixups. The real class destructor at
0x009AC7C0 was falsely labeled as a mesh-stripifier destructor; the typed
source emits 149 bytes against 161 retail bytes, with 0.934 normalized
instruction similarity. Its linked fixture checks string destruction, map
cleanup, threaded-file and disk-file ownership release, base destruction,
and shared-reference preservation.

`NBankFileManager::CContainedBankMap::Erase` at 0x009AA780 was another
text-tree mislabel. Its typed right-subtree-first erase emits 59 bytes
against 59 retail bytes with identical normalized instructions. A linked
three-node fixture checks destruction order and the null root. The old
false landscape-patch source and catalog entry at 0x009AC7A0 were removed.
Selected builds, provider-omission checks and existing retail-bank
constructor, threaded-file reset and OpenRetailBank fixtures pass after
adding the actual counted-pointer and map cleanup behavior.

The selected GFMain closure is
`work/codex-native-startup/native-gfmain-retail-bank-lifetime-closure-20261004`:
**1,096 sources, zero duplicates, 453 unresolved**.

## Typed build-number chain

Replaced `GFGetBuildNumber2`'s remaining placeholder string and disk-file
classes with genuine `CWideString`, `CDiskFileWin32`, `CAFile` and
`CCharString` types. The new source follows the retail lifetime order:
construct and probe the path; construct a disk file using a second path;
read its contents; assign the global; destroy contents and file. The old
candidate-only header was removed. The typed VC7.1 body is 160 bytes against
162 retail bytes, with 44/44 instructions and 0.955 normalized similarity
after four retail absolute-address fixups. Linked present/absent-path and
provider-omission gates, plus the selected build, pass. Eight invented
frontier symbols disappear; one genuine file-backed string constructor
appears.

That constructor, `CCharString(CAFile*,long)` at 0x0099FA50, now has a
52/52-byte, 18/18-instruction near match with identical normalized
instructions. Its linked fixture covers `-1`, zero and positive lengths,
the null process pointer, receiver identity and the instance count. The
selected VC7.1 build and omitted-provider link pass. The global
`FableFullBuildVersion` at 0x013B85E0 is a four-byte `CCharString` with
retail `"Unknown"` initializer at 0x01219F20 and destructor thunk at
0x01228920; its linked data gate passes.

Current GFMain closure:
`work/codex-native-startup/native-gfmain-charstring-from-file-closure-20261004`,
**1,093 sources, zero duplicates, 455 unresolved**. No complete startup or
game launch is claimed.

## Build-number path probe corrected

Retail `GFGetBuildNumber2` calls 0x00999230 at 0x00401F46 with the
wide-string temporary in ECX. That callee is the existing static
`CAFile::PathExists`, while the candidate header had invented a
`CWideString::PathExists` method. The source and linked fixture now use
the real symbol. The selected VC7.1 build and focused near-match gate pass:
162/162 bytes, 44/44 instructions, 0.909 normalized opcode similarity.
The earlier relocation-match catalog grade was corrected to `NEAR_MATCH`.
The refreshed GFMain closure removes the placeholder symbol and has
**1,091 sources, zero duplicates, 464 unresolved** at
`work/codex-native-startup/native-gfmain-build-number-path-closure-20261004`.

I also inspected retail `CWideString::operator+=(wchar_t)` at 0x0099B9A0.
Its capacity-growth and fill helpers need further reconstruction; an initial
scratch candidate is 150/203 bytes at 0.683 normalized similarity and was
kept outside the catalog and closure.

## CAFile current-path global

`g_CAFileCurrentPath_013BCA10` is a four-byte `CWideString` in the retail
`.data` zero-filled tail. CRT thunks 0x01223D80 and 0x0122A4E0 call the
wide-string constructor and destructor on that address. The typed provider
passes COFF layout, thunk, linked lifetime and omitted-provider checks.
The refreshed GFMain closure is
`work/codex-native-startup/native-gfmain-cafile-current-path-closure-20261004`:
**1,091 sources, zero duplicates, 465 unresolved**.

## Definition path and wide-string counter globals

GFMain assigns its compiled-definition directory to `g_013ca7dc`, a
four-byte `CWideString` at retail 0x013CA7DC. CRT initializer 0x01224050
calls the wide-string default constructor on that address; shutdown thunk
0x0122A6B0 calls its destructor. The typed data provider passes its COFF
layout, retail zero-filled-tail, thunk, linked lifetime and omitted-provider
checks.

`g_CWideStringInstanceCount_013BCA20` is a four-byte counter in the retail
`.data` zero-filled tail. Its provider aliases the older C name
`g_charStringRefTrace` to the same physical object. The linked fixture checks
both names, initialization, and cross-name read/write; omitted-provider
linking fails as expected. The two sources remove two GFMain frontier
symbols. Current closure:
`work/codex-native-startup/native-gfmain-wide-count-closure-20261004`,
**1,090 sources, zero duplicates, 466 unresolved**.

## Bank manager lifetime and container cleanup

The global at retail 0x013CA79C is a 0x38-byte `CBankFileManager` with two
native symbol aliases. Its CRT initializer and destructor thunks call
0x009A7AC0 and 0x009A7B70, correcting false hero-screen and landscape
labels. The typed constructor is 172/172 bytes with 0.966 opcode similarity;
the destructor is 229/229 bytes with 0.989 similarity. Both pass linked
layout/order fixtures and provider-omission links. The global gate verifies
the COFF data symbol, aliases, retail zero-filled tail, and CRT references.

Destructor dependencies: `CBasicRetailBankAliasMap::Erase` at 0x009AA830
has a 70/70-byte near match with identical normalized instructions, and
`CBankManagerWidePathSet::Erase` at 0x009AA880 has a 59/59-byte near match
by the same measure. Their bytes differ after relocation masking.
`CRetailBankList::clear` at 0x009AAE50 emits 83
bytes against 89 retail bytes with 0.914 opcode similarity. Their composed
fixture checks null roots, recursive destruction order, shared reference
release, sentinel reset, and repeat clear; three omission links fail as
expected. Selected constructor/destructor builds and existing bank gates
pass. The focused symbol index includes all five functions and the object.

GFMain closure after these additions:
`work/codex-native-startup/native-gfmain-bank-manager-containers-closure-20261004`,
**1,088 sources, zero duplicates, 468 unresolved symbols**. This is a link
closure; no complete game startup has been demonstrated.

Resumed the native GFMain closure without touching the paused Lua recovery. The
latest prior closure was 1,044 sources, zero duplicates, 511 unresolved symbols.

## CCompressorZlib::InternalCompress

Reconstructed `0x00A3AA40` as genuine VC7.1 C++ in
`rebuild/src/compiled/00/a3/CCompressorZlib_InternalCompress_00a3aa40.cpp`.
The function returns zero for empty input. Otherwise it initializes a local
zlib 1.2.1 stream with the engine's Alloc/Free callbacks, feeds one byte of
input and one byte of output at a time until all input is consumed or output
capacity is reached, then repeats finish calls until zlib returns stream-end,
ends the stream and returns `total_out`. The actual retail body is 192 bytes
through `ret 0x10`; the coverage inventory's 189-byte figure omitted the
three-byte return instruction.

The `/O2 /MT` VC7.1 body is byte-identical after resolving all seven
relocations (version literal, Alloc, Free, deflateInit, two deflate calls,
deflateEnd). `gate_zlib_internal_compress.py` also links the actual provider
against a zlib-call recorder: 16 cases cover zero input, input exhaustion,
output-capacity stopping, two finish calls, stream lifetime and callbacks.
The provider-omission link fails as expected. The selected candidate build
passes. The existing zlib-wrapper gate still passes (9 compiled scenarios,
72 retail comparisons). Catalog grade: `RELOCATION_MATCH` / `PASS`.

Focused symbol-index refresh and native closure: **1,045 sources, zero
duplicates, 512 unresolved symbols** at
`work/codex-native-startup/native-gfmain-zlib-internal-compress-closure-20261004`.
The new provider resolves its own symbol and exposes three lower-level zlib
symbols (`z_deflateInit_`, `z_deflate`, `z_deflateEnd`). Their retail bodies
remain outside the closure. No game launch or full startup execution claimed.

Next native work: recover the lower-level zlib dependencies from native
evidence, or continue another independent GFMain frontier provider. Keep the
blocked shader-bank security exclusions and the paused Lua lane untouched.

## Display and window runtime data

Added one typed data provider for six existing native globals: retail gamma
`1.0f` at 0x0138E0B8; zero-initialized saved sound volume, deactivation flag,
window-system pointer, and X/Y virtual-pixel scales. The gate checks the COFF
names and all 29 initial bytes against the retail `.data` initializer or its
zero-filled tail. A linked fixture passes 16 round trips and the omitted
provider fails to link. Existing system-activation, window-procedure and
virtual-pixel gates still pass (512, 3,253 and 1,250 compiled cases).

Focused symbol-index refresh resolves all six references. Latest GFMain
closure: **1,046 sources, zero duplicates, 506 unresolved symbols** at
`work/codex-native-startup/native-gfmain-display-runtime-data-closure-20261004`.
This is data coverage only; no additional function is claimed.

## z_deflateEnd

Reconstructed retail zlib's `z_deflateEnd` at 0x00C00AE0 from its native
control flow. The source uses the existing `z_stream_s` ABI and a local
partial state layout naming only five observed fields. It rejects null stream
or state and invalid status with `-2`; releases up to four optional
allocations in retail order, then the state; clears the stream's state pointer;
and returns `-3` for status 0x71 or zero for the other accepted statuses.

VC7.1 emits the exact 178-byte body with no relocations. The linked fixture
covers 66 cases: null inputs, three accepted statuses, one invalid status,
all 16 optional-allocation masks, callback order and result values. An omitted
provider fails to link; the selected candidate build passes. Focused symbol
index and closure refresh pass: **1,047 sources, zero duplicates, 505 unresolved
symbols** at `work/codex-native-startup/native-gfmain-z-deflate-end-closure-20261004`.
The remaining zlib frontier includes `z_deflateInit_` and `z_deflate`.

## z_deflateInit_

The retail `z_deflateInit_` wrapper at 0x00C021C0 is reconstructed as a
direct call to `z_deflateInit2_` with method 8, window 15, memory level 8
and strategy zero. VC7.1 emits the exact 37-byte body after its single call
relocation. A linked fixture checks 11 compression levels, all forwarded
arguments and the return value; the omitted-provider link fails, and the
selected candidate build passes. Focused symbol index and closure refresh
pass: **1,048 sources, zero duplicates, 505 unresolved symbols** at
`work/codex-native-startup/native-gfmain-z-deflate-init-closure-20261004`.
The count is unchanged because `z_deflateInit2_` replaces this wrapper on
the frontier. Native zlib work now reaches the initializer and deflate loop.

## z_deflateInit2_ and zlib errors

Recovered `z_deflateInit2_` at 0x00C01F90 using the upstream zlib 1.2.1
function and its original `deflate.h` layout, with only the syntax adaptations
needed for VC7.1 C++. The 558-byte function matches retail after five
relocations: zcalloc, zcfree, deflateReset, the z_errmsg table, and
deflateEnd. The linked 24-case fixture covers valid levels/window forms,
method/memory/strategy/version/stream-size rejection, null stream,
all five allocation failures and cleanup, and success state. It confirms
retail allocates all four child buffers before checking whether any failed.
Provider omission and selected candidate build pass.

The original 1.2.1 `z_errmsg` ten-pointer table is now a typed data provider.
Its gate checks each target's string bytes against retail, ten linked
lookups, and a provider-omission link. It does not claim that the linker will
place identical immutable string literals at retail addresses.

Latest focused index and GFMain closure: **1,050 sources, zero duplicates,
505 unresolved symbols** at
`work/codex-native-startup/native-gfmain-zlib-error-messages-closure-20261004`.
The zlib frontier includes `z_deflateReset` and `z_deflate`. Reset's retail
body calls internal `lm_init` with an observed register contract; that helper
needs an evidenced implementation before reset is promoted. No full startup
execution or compression round-trip through the native closure is claimed.

Scratch follow-up: retrieved the official zlib 1.2.1 fossil into
`work/codex-native-startup/zlib121-source-20261004` (not a production source
or committed retail asset). Compiling its unmodified `deflate.c` as C with
VC7.1 `/O2 /MT /DZ_PREFIX` yields relocation-masked retail matches and exact
function lengths for `z_deflateReset` (145/4 relocs), internal `lm_init`
(135/4), `z_deflateInit2_` (558/5), `z_deflateEnd` (178/0) and `z_deflate`
(1,080/17). Reproducible probe:
`python work/codex-native-startup/probe_upstream_deflate_unit_20261004.py`.
The full upstream C translation unit is a verified source oracle, not yet a
promoted provider. Its K&R signatures do not compile as C++, and linking the
whole C object would duplicate separately landed zlib symbols. Next work is
to adapt/reset the remaining functions and helpers under the existing
per-entry provider model, then check the complete compression path.

## zlib deflate and tree cores promoted

Adapted the original zlib 1.2.1 `deflate.c` and `trees.c` signatures for
VC7.1 C++, preserving upstream algorithms and excluding the three already
landed deflate entry points. The deflate unit matches five retail bodies after
relocation: `z_deflate` 1,080 bytes, `z_deflateReset` 145, local `lm_init`
135, `putShortMSB` 39 and `flush_pending` 100. The tree unit matches four:
`_tr_init` 100, `_tr_stored_block` 160, `_tr_align` 497 and
`_tr_flush_block` 485. Both use the original upstream headers. The focused
gates, candidate fixtures and selected catalog builds pass; there are no
duplicate definitions of the three earlier deflate functions.

The composed C++ compression fixture links reconstructed zlib initialization,
allocator, checksum, error-message, deflate and tree providers. Six sizes
from zero to 4,096 bytes round trip through Python zlib decompression, and
omitting either core provider fails the link. This verifies the compression
path in an isolated fixture, not full GFMain startup execution.

The upstream 1.2.1 `compressBound` helper had a gap-scan name in the manifest.
Its 21-byte VC7.1 output occurs exactly at retail 0x00C03EB0, with no
relocations. Promoted it with 11 linked input cases spanning both shift
thresholds and integer wraparound, an omitted-provider link, a selected
catalog build, and a focused symbol-index entry. The compression fixture now
links this real helper too; its three provider-omission checks and six round
trips pass. Focused closure: **1,053 sources, zero duplicates, 503 unresolved
symbols** at
`work/codex-native-startup/native-gfmain-zlib-compress-bound-closure-20261004`.
No unresolved deflate or tree symbol remains on that frontier. The remaining
frontier is primarily game and UI code; full startup execution is still open.

## z_compress2 promoted

Adapted the upstream zlib 1.2.1 `compress2` function to ANSI C++ syntax.
VC7.1 emits a 169-byte body at 0x00C03DE0 that matches retail after five
relocations, including the checked `1.2.1` version string. Its selected
catalog build and focused gate pass seven linked cases covering successful
output length, init error, buffer exhaustion, deflate error and end error;
omitting the provider fails to link. The composed compression fixture now
executes `compress2` through the real deflate/tree/allocator providers as a
seventh decompression-verified round trip. The new focused closure is **1,054
sources, zero duplicates, 503 unresolved** at
`work/codex-native-startup/native-gfmain-zlib-compress2-closure-20261004`.
The unresolved count stays constant because `compress2` was an adjacent
unresolved manifest entry, not a symbol on the GFMain frontier.

## Tree helper coverage expanded

Audited every emitted tree helper in the promoted C++ source. Eleven more
named helpers match retail, covering block initialization, heap maintenance,
bit-length generation, tree scanning and sending, data-type selection, bit
buffer flush/windup, block copy and bit-length-tree construction. A gap-scan
missed the 23-byte `bi_reverse` at 0x00C05020; its exact body has one unique
location in retail `.text`. The tree gate now checks **16 exact or relocated
retail bodies**, its 16-case linked fixture and omitted-provider link still
pass, and all 12 added catalog entries pass selected builds. The composed
seven-round-trip compression fixture still passes.

The C++ `gen_codes`, `build_tree` and `compress_block` bodies are not marked
byte matches. A scratch compile of unmodified upstream C confirms retail
masked matches for `build_tree` (498 bytes) and `compress_block` (1,055 bytes),
while the C++ versions differ in code generation; `gen_codes` differs in
both languages. These functions remain functional members of the linked
compression path but are not claimed as byte-matched manifest entries.

## CCRC instance method and lookup table

Recovered the unresolved `CCRC::Calc(const void*, unsigned) const` at
0x00A00950. The manifest had mislabeled that address as a three-argument
global fastcall; the emitted method is a 69-byte `thiscall` body with one
relocation to the CRC table. Its zero-length early return is significant to
VC7.1 code generation and gives a full retail byte match after relocation.

Added the standard reflected CRC-32 table at 0x0129A168 from the public
0xEDB88320 polynomial. The gate compares all 1,024 initialized bytes with
retail, then links the real method and table for 17 cases against an
independent bitwise reference. Omission of either provider fails to link;
both selected catalog builds and the focused symbol-index refresh pass.
The source table is generated from the polynomial, not copied from the retail
image.

Focused GFMain closure after both providers: **1,056 sources, zero duplicates,
501 unresolved symbols** at
`work/codex-native-startup/native-gfmain-ccrc-instance-closure-20261004`.
Both CRC symbols disappeared from the frontier; the method/table add two
genuine linked sources.

## Startup graphics and recording data

Added one typed provider for six retail zero-initialized settings:
checkpoint-save deletion, maximum texture height and width, texture bit
depth, render-only-while-recording, and movie recording. The names and types
come from the existing native callers and shared game header. The gate checks
all six COFF symbols and 15 initializer bytes against retail `.data` or its
zero-filled tail, runs 18 linked read/write cases, and confirms that omitting
the provider breaks the link. This is data coverage only; no caller behavior
is claimed by the data gate.

Focused GFMain closure after the provider: **1,057 sources, zero duplicates,
495 unresolved symbols** at
`work/codex-native-startup/native-gfmain-startup-graphics-data-closure-20261004`.
All six settings disappear from the frontier.

## GFMain runtime flag aliases reconciled

Retyped three existing physical bytes in the GFMain runtime provider using
the names and types already present in their other callers:
`g_FableStartMainGame_013B8605` (unsigned byte),
`g_FableRunFromDVD_013B8615` (bool), and
`g_FablePresentationIntervalOverride_013B8628` (bool). GFMain now references
those same symbols. The data gate still checks all 16 zero-initialized
switches and linked writes; the full GFMain gate still passes its 3,952-byte
near match, 391 relocations and 58 simulated scenarios. Focused symbol index
and closure refresh pass: **1,057 sources, zero duplicates, 492 unresolved**
at `work/codex-native-startup/native-gfmain-flag-aliases-closure-20261004`.
The three semantic symbols leave the frontier without adding parallel
storage. The separate 0x013B8616 retail-bank/navigation alias remains for a
cross-caller review.

## Retail-bank flag unified across native callers

The shared 0x013B8616 byte had three unresolved linker names: the retail PDB
name `GUseRetailBanks`, a frontend navigation alias, and a startup retail-bank
alias. The existing GFMain runtime data unit now defines the PDB-named bool
once, and GFMain, the frontend run loop, console-variable registration and
main-game post-init reference it. The two world-path callers already used
`GUseRetailBanks`. Their physical retail address and boolean use agree.

The 16-switch data gate and full GFMain near-match/simulation gate still pass.
Selected frontend and post-init builds pass, and the console registrar's
standalone linked fixture passes. The focused symbol-index refresh and GFMain
closure yield **1,057 sources, zero duplicates, 489 unresolved symbols** at
`work/codex-native-startup/native-gfmain-retail-bank-alias-closure-20261004`.
All three former names disappear from that frontier. Older phase-only
fixtures retain their address-labelled variables; they are separate test
models, not providers in the native closure.

## CDataOutputStream::WriteEBOOL retyped

The frontier still held the real `CDataOutputStream::WriteEBOOL` symbol at
0x00993E30 even though a matching catalog source existed. That source defined
the same algorithm on a placeholder subclass, so it emitted a different
linker symbol. Retyped the method onto the shared `CDataOutputStream` class
and replaced the test's independent copy with a linked derived-stream
fixture. VC7.1 emits the exact 28 retail bytes with no relocations. The gate
covers true, false and a noncanonical nonzero bool, plus a provider-omission
link; selected catalog build and focused symbol index pass.

GFMain closure after promotion: **1,058 sources, zero duplicates, 488
unresolved symbols** at
`work/codex-native-startup/native-gfmain-write-ebool-closure-20261004`.
The real stream method is no longer on the frontier.

## CFrontEndDef::GetSizeofClass retyped

The 0x00431E30 catalog source previously returned the correct 0x54 constant
from a placeholder `T::Get` method, leaving the actual virtual
`CFrontEndDef::GetSizeofClass` unresolved. Retyped the body onto the shared
frontend definition class and replaced a print-only test with a linked call
through that class. VC7.1 emits the exact six-byte retail body; the focused
gate checks the method symbol, retail bytes, return value, class size and
provider-omission link. Selected catalog build and symbol index pass.

Latest GFMain closure: **1,059 sources, zero duplicates, 487 unresolved
symbols** at
`work/codex-native-startup/native-gfmain-frontend-def-size-closure-20261004`.
The real frontend definition method leaves the frontier.

## Console command formatted name and suffix

Retyped the 0x009EA3A0 catalog method onto the shared
`CConsoleCommandBase` and corrected its manifest signature. Its typed
`Name + g_013caa48` body matches all 26 retail bytes after two
relocations. The focused gate links an actual caller, checks the argument
addresses and return path, and verifies provider omission; the selected
catalog build and focused symbol index pass.

Retail code constructs the referenced `CCharString` global at 0x013CAA48
from the literal `"("`. The new typed provider emits the same 29-byte CRT
initializer and 10-byte destructor thunk after relocation, and the four-byte
object occupies uninitialized data. Its gate compares both routines with
retail, checks the literal, exercises initialization through a linked fixture,
and verifies an omitted-provider link failure. The two console gates are
registered in `gate_all.py`.

The selected-source GFMain closure now has **1,061 sources, zero duplicates,
486 unresolved symbols** at
`work/codex-native-startup/native-gfmain-console-suffix-closure-20261004`.
The suffix global leaves the frontier. The `CCharString` binary operator+
at 0x0099F570 remains an explicit unresolved dependency; a simple copy and
append candidate did not match retail and was left in scratch. The closure
tool now accepts `--seed-file` so a selected source set can be refreshed
without accidentally choosing alternate providers.

## CCharString binary addition recovered

The former 0x0099F570 `CCharString::AppendData` manifest label was false:
the retail PDB symbol and call sites identify the fastcall binary
`operator+(const CCharString&, const CCharString&)`. The new catalog source
copies the left string, appends the right, and returns the result with the
copy constructor and destructor inlined as in retail. Its 126-byte VC7.1
body is a near match to the 130-byte retail routine: the first 23 bytes
and all 88 bytes after the copy block match exactly after seven relocations.
The four-byte difference is the shared-owner increment: retail uses a
register load/increment/store, while this source emits one memory increment.
Both paths increment the same owner field once.

The focused gate checks the retail control and call shape, runs three
linked lifetime cases (shared left storage, aliased inputs, empty left),
and requires the provider for linking. The selected catalog build and
focused symbol index pass. GFMain selected-source closure is now **1,062
sources, zero duplicates, 485 unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-plus-closure-20261004`.
The binary operator leaves the frontier; the broader console formatter
composition remains available for a follow-up gate.

## Console formatting chain and string counter

The composed console gate now links the genuine formatter, binary string
operator and dynamically initialized `"("` suffix together. A linked fixture
checks the literal construction once, both nonempty and empty command-name
paths, the operator's append argument, shared-owner counts, return-value
lifetime and final release. Omitting each of the three providers separately
breaks the link. This closes the composition gap left by the standalone
gates without claiming a retail implementation for the append primitive
used as a fixture boundary.

The 0x013BD800 `CCharString` instance counter was still unresolved. A typed
four-byte zero-filled storage provider now matches the retail `.data` tail,
passes linked initialization/write checks and an omitted-provider link,
and is indexed under its semantic symbol. The selected-source GFMain
closure is **1,063 sources, zero duplicates, 484 unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-count-closure-20261004`.
The counter leaves the frontier. An older `GetSubString` catalog source
still declares a distinct C-linkage name for this same address; it is not
in the selected closure and needs retyping before that source can be
composed safely.

## CCharString::GetSubString retyped

The 0x0099EC70 catalog unit previously compiled to 148 matching retail
bytes under a false pointer-return method symbol on a local struct, and
referenced the instance counter through a separate C-linkage name. PDB
evidence identifies the actual const `CCharString::GetSubString(unsigned
long, unsigned long)` returning `CCharString` by value. Retyped the source
and fixture onto the shared class and canonical counter. Its VC7.1 method
is a **150/148-byte near match** (51 instructions each, normalized
similarity 0.902); the emitted 15-byte default-constructor COMDAT matches
retail exactly after relocation. The linked fixture covers allocation from
`[start,end)`, a terminator result and a null source, including copy and
destructor counts. Provider omission, selected catalog build and focused
symbol index pass.

The selected-source GFMain closure uses this unit's exact default-constructor
COMDAT in place of the previously selected standalone constructor source.
It has **1,063 sources, zero duplicates, 484 unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-substring-reconciled-closure-20261004`.
The standalone constructor source remains in the catalog; selecting both
objects together would duplicate that symbol. The old false substring
method and C-linkage counter alias are absent from the retyped unit.

## GFIntToCharString recovered

`GFIntToCharString` at 0x0099F830 is the small integer formatter called by
`CConsoleCommandBase::GetHelp`. A typed `CCharString` temporary, `Sprintf`
with the retail `"%d"` literal, and return-by-value copy emit **all 116
retail bytes exactly after nine relocations** under VC7.1. The linked
fixture covers zero, positive, negative and minimum signed integers,
checks the format call and returned storage lifetime, and requires the
provider for linking. Selected catalog build and focused symbol index pass.

The selected GFMain closure adds this source with **1,064 sources, zero
duplicates, 484 unresolved symbols** at
`work/codex-native-startup/native-gfmain-gfint-charstring-closure-20261004`.
The formatter symbol leaves the frontier and exposes its real
`CCharString::Sprintf` callee at 0x0099F1F0. The 313-byte console help
routine and its remaining string/parameter helpers are still open.

## CCharString::Sprintf recovered and composed

Retyped the 0x0099F1F0 formatter as `CCharString::Sprintf(const char*, ...)`.
The VC7.1 body matches **all 172 retail bytes after six relocations**:
format-length plus 0x400 allocation, zero fill, `_vsnprintf`, forced last
terminator, old-storage release, optional new 17-byte string-data object,
and buffer free. The focused linked fixture covers nonempty replacement,
empty output, and truncated long output, with an omission link. Selected
catalog build and symbol index pass.

A second fixture links the real `GFIntToCharString` and `Sprintf` providers
together for five signed integers, checking text, owner count and return
lifetime; omitting either provider breaks the link. The selected GFMain
closure is **1,065 sources, zero duplicates, 483 unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-sprintf-closure-20261004`.
`Sprintf` leaves the frontier without new unresolved native symbols.

## Console help literal string operations

Retyped the string-plus-literal binary operator at 0x0099F600 from a false
`CCharString::AppendCString` label. The shared-class source emits a
126/130-byte near match: the 23-byte prefix and 88-byte tail match retail
after relocation, while the copy path uses one memory increment in place
of retail's register load/increment/store. Three linked ownership cases,
an omission check and the selected catalog build pass.

The operator exposed `CCharString::operator+=(const char*)` at 0x0099F100.
Its typed source matches the retail 122-byte instruction sequence exactly
after normalized register allocation. Five linked cases cover existing
storage with nonempty/empty text and empty storage with nonempty, empty or
null text; provider omission and selected build pass. The selected-source
GFMain closure now has **1,067 sources, zero duplicates, 481 unresolved
symbols** at
`work/codex-native-startup/native-gfmain-charstring-append-literal-closure-20261004`.
Both string operations leave the frontier. Their composed lifetime path
and the console command `GetHelp` routine remain open.

## Console command help recovered

Retyped `CConsoleCommandBase::GetHelp` at 0x009EA7D0. The retail vtable at
0x0122E638 shows that its slot-3 call is `GetTypeAsString`, followed by the
literal `" with "`. The routine formats zero parameters as `"no parameters"`;
otherwise it appends the integer count, `" parameter"`, a plural suffix for
counts above one, and each parameter type separated by a space. Keeping each
vector-size read in the source reproduces the retail control flow. VC7.1
emits **313/313 bytes, 93/93 instructions**, normalized opcode similarity
**0.903**, with the expected string and parameter-helper calls.

The focused gate links the real provider and checks zero, one and two
parameter cases, virtual type-string dispatch, parameter-helper call counts,
and a missing-provider link. Selected catalog build and focused symbol-index
refresh pass. The selected GFMain closure has **1,068 sources, zero
duplicates, 481 unresolved symbols** at
`work/codex-native-startup/native-gfmain-console-help-closure-20261004`.
`GetHelp` leaves the frontier and exposes
`CConsoleCommandParameters::GetParameterTypeAsString` at 0x009EA740.

## Console parameter type string recovered

The typed `GetParameterTypeAsString` switch matches **all 136 retail bytes**
after 16 address fixups, including its five-entry jump table. Its source case
order is bool, slong, ulong, float, string, with `"null"` for the null enum
and values outside 1–5. A linked fixture checks nine enum and fallback
values and requires the provider. Selected catalog build and symbol-index
refresh pass. The selected GFMain closure has **1,069 sources, zero
duplicates, 480 unresolved symbols** at
`work/codex-native-startup/native-gfmain-console-parameter-closure-20261004`.
The helper leaves the frontier without adding a new native dependency.
The composed linked fixture runs the real `GetHelp` and parameter helper
together for zero, one and two parameters and requires both providers.

## Console variable help and generic value printing

Retyped `CConsoleVariable::GetHelp` at 0x009EA3C0. It calls vtable slot 3
(`GetTypeAsString`), appends `" of type "`, recognizes bool/float/slong/string,
and appends a value rendered through `CGenericVarRef`. Its VC7.1 body is a
**206/206-byte, 62/62-instruction near match** with normalized opcode
similarity **0.903**. The linked fixture checks five type branches and
requires the provider.

Retyped `CGenericVarRef::PrintToString` at 0x00A63C00. The compiler emits
**all 180 retail bytes after 17 address fixups**, including the five-entry
switch table. Signed integers and floats use `CCharString::Sprintf`, booleans
assign `"TRUE"`/`"FALSE"`, strings copy a temporary, and null/unsigned/
generic values leave the output unchanged. Nine direct linked cases and a
provider omission pass. A composed fixture links the real variable help and
printer for six values, with omission checks for both providers. Selected
catalog builds and focused symbol-index refreshes pass.

The selected GFMain closure has **1,071 sources, zero duplicates, 479
unresolved symbols** at
`work/codex-native-startup/native-gfmain-generic-var-print-closure-20261004`.
Both recovered symbols leave the frontier; the printer adds no new native
dependency.

## Console command callbacks recovered

Retyped the classless callback at 0x00415040 as
`CConsoleClasslessCommand::CallCommandFunction`. The body forwards the
`CGenericVarVector` reference in ECX to its stored fastcall function pointer
at object offset 0x14. VC7.1 emits the **exact 12 retail bytes**, including
two alignment bytes. A linked fixture exercises virtual dispatch twice and
requires the provider. The manifest's generic `vfunc_5` label is corrected
to the typed symbol.

Retail RTTI identifies the bound `CConsoleCommand<CConsole>` vtable at
0x0129C4E0 and its callback at 0x009EFEB0. The explicit C++ specialization
loads the receiver from offset 0x14, passes the parameter vector in EDX,
and calls the stored fastcall pointer at offset 0x18. VC7.1 emits the
**exact 15 retail bytes**. A linked fixture checks both pointer identities,
two invocations, the 0x1C object size, and provider omission. The generic
`vfunc_5` manifest label is replaced with the typed specialization symbol.

Both selected catalog builds, focused gates, and symbol-index refreshes
pass. The selected GFMain closure has **1,073 sources, zero duplicates,
477 unresolved symbols** at
`work/codex-native-startup/native-gfmain-bound-console-call-closure-20261004`.
Both callbacks leave its frontier without adding native dependencies.

## Last Windows error recovered

Retyped `GFGetLastWindowsError` at 0x00A5ED60. It gets the current Win32
error, asks `FormatMessageW` to allocate the system message, converts the
result through `CWideString::ToCharString`, frees the Win32 buffer, and
returns an empty `CCharString` when no message is available. VC7.1 emits
**all 136 retail bytes after ten address fixups**. The retail path leaves
the message pointer uninitialized until `FormatMessageW` writes it; matching
that order removes eight bytes from the first probe.

A linked fixture uses `kernel32.lib` to exercise an existing system message
and a code with no message, checks wide-string construction/conversion
counts, and requires the native provider. The selected catalog build and
focused symbol-index refresh pass. The selected GFMain closure is **1,074
sources, zero duplicates, 476 unresolved symbols** at
`work/codex-native-startup/native-gfmain-last-windows-error-closure-20261004`.
The helper leaves the frontier without adding a native dependency.

## Basic string equality retyped

The existing 0x004115A0 source used a local `CBasicStringChar` stand-in.
Retyped it as the genuine `CBasicString<char>::operator==(const char*)`
declared in the shared header. VC7.1 still emits **all 88 retail bytes
exactly**. The linked fixture covers six equal/different/length cases and
requires the provider. The selected catalog build and focused gate pass.

`CCharStringData::operator==` now forwards inline to its leading `Data`
member. The selected closure automatically adds the genuine template
provider and removes the artificial, previously unresolved
`CCharStringData::operator==` symbol. It has **1,075 sources, zero
duplicates, 475 unresolved symbols** at
`work/codex-native-startup/native-gfmain-basic-string-equal-closure-20261004`.

## CCharString uppercase recovered

Retyped the retail method at 0x0099EE20 as
`CCharString::GetAsUpperCase() const`, correcting the manifest's `ToUpper`
label. It mirrors the recovered lowercase ownership path, then converts
ASCII bytes with `toupper` while preserving bytes at or above 0x80. VC7.1
emits a **277/278-byte near match** with 0.985 normalized opcode similarity.

The linked fixture covers empty input, mixed case, shared-data isolation,
allocation failure, and an extended byte that must be preserved. Provider
omission and selected catalog build pass. The selected GFMain closure adds
the source automatically and has **1,076 sources, zero duplicates, 474
unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-uppercase-closure-20261004`.
The uppercase symbol leaves the frontier without exposing another native
dependency.

## Char assignment recovered

`CCharString::operator=(char)` at 0x0099F040 now has a genuine VC7.1
source with an **exact 89-byte relocation match** and the same 36 retail
instructions. Its three fixups are `UnassignString`, `operator new`, and
the `CBasicString<char>` constructor. The linked fixture checks empty,
replacement, extended-byte, clear, and allocation-failure behavior;
provider omission and the selected catalog build pass. This symbol is
not on the current GFMain frontier, so its 1,076-source, zero-duplicate,
474-unresolved closure remains the current checkpoint.


## Whitespace wrapper corrected

The prior `_Dest_val` allocator helper label at 0x0099F560 was false.
`CBankFileManager::OpenIniFile` calls that address with a `CCharString`
receiver immediately after `ReadLine`. Its body calls 0x0099F450 and
0x0099F4D0, the leading and trailing whitespace methods identified in
the engine API. Retyped the source as
`CCharString::RemoveWhiteSpaceBeginEnd`; VC7.1 matches **all 16 retail
bytes after two relocation fixups**. A linked fixture checks call order
and receiver; provider omission and selected catalog build pass. The
two trim callees remain to be recovered.
The refreshed GFMain closure contains **1,077 sources, zero duplicates,
475 unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-trim-wrapper-closure-20261004`.
The wrapper replaces its own unresolved symbol and exposes the two real
callees.

## Whitespace trim methods recovered

`CCharString::RemoveWhiteSpaceBegin` at 0x0099F450 now matches **all 117
retail bytes after four relocations**. Its retail loop leaves an
all-whitespace string unchanged; the linked fixture checks that case,
empty input, mixed whitespace, no change, and shared-data isolation.
`RemoveWhiteSpaceEnd` at 0x0099F4D0 is a **134/135-byte near match** with
0.912 normalized opcode similarity and five linked cases. A composed
fixture links both methods with `RemoveWhiteSpaceBeginEnd`, checks a shared
string through both trim passes, and verifies unchanged empty, all-space,
and interior text. Three provider-omission checks and both selected
candidate builds pass.

The selected GFMain closure is now **1,079 sources, zero duplicates, 473
unresolved symbols** at
`work/codex-native-startup/native-gfmain-charstring-trim-sides-closure-20261004`.
Both trim symbols leave the frontier without exposing another dependency.

## Bank quoted string recovered

`CBankFileManager::ReadQuotedString` at 0x009A77D0 now scans to an opening
quote, consumes it, assigns the text through the next quote, and consumes
the closing quote when present. The VC7.1 source is a **249/255-byte near
match** with 0.958 normalized opcode similarity and the same 95 retail
instructions. The linked fixture covers a missing opening quote, normal
quoted text, an unterminated quote, an empty value, and the stream overflow
path. Provider omission and selected candidate build pass.

The shared `ReadCBYTE` declaration now forces inlining, as retail does in
this caller. A fresh compile of `CBankFileManager::OpenIniFile` still has
its 1,732-byte retail length and 0.968 normalized opcode similarity.
The rebuilt selected GFMain closure has **1,080 sources, zero duplicates,
472 unresolved symbols** at
`work/codex-native-startup/native-gfmain-bank-read-quoted-closure-20261004`.
The quoted-string method leaves the frontier without a new dependency.
Also corrected the existing 0x0099EAE0 manifest label to its verified
`CCharString` destructor identity.

## Bank manager close recovered

`CBankFileManager::Close` at 0x009A7990 now removes retail banks from
the tail of its list, releasing counted ownership, then clears the bank
map. VC7.1 emits a **132/135-byte near match** with 0.956 normalized
opcode similarity and 56/57 instructions. The linked fixture covers an
empty manager, reverse-order release, a shared bank kept alive, map
clear, and repeat close. Provider omission and selected build pass.

The refreshed GFMain closure has **1,081 sources, zero duplicates, 471
unresolved symbols** at
`work/codex-native-startup/native-gfmain-bank-manager-close-closure-20261004`.
The close symbol leaves the frontier without adding a new dependency.

## Definition memory shrink path recovered

`CDefinitionManager::RemoveUnusedMemory` at 0x009AE5F0 now has a typed
182/182-byte relocation match. It shrinks instantiated-definition
pointers, then the definition symbol map, each class's global-index
array, and each instantiated definition's child map when present.
Retail makes a fresh virtual child-map query before the final shrink;
the typed source preserves both calls. A linked fixture checks the
delegate order, two classes, one child map and one null child map, and
requires both the manager and symbol-map wrapper providers. Its
selected catalog build passes.

The symbol-map delegate at 0x009E5350 was mislabeled as a particle
`_Cons_val` helper. It is `CSymbolMap::DeallocateUnusedMemory`, an
8/8-byte relocation match that adjusts `this` by four and tail-calls
the existing vector-map shrink at 0x009E7C70. Its linked fixture checks
the adjusted receiver; dedicated gate and selected build pass.

The resulting closure exposed three missing template shrink symbols.
Two `CArrayVector::DeallocateUnusedMemory` instantiations at 0x009B6E00
and 0x009B6EB0, the child-map shrink at 0x009B79B0, and their three
storage-release helpers at 0x009B4A90, 0x009B4B80, and 0x009B5F30 now
match **659/659 retail bytes after relocations**. The child-map release
uses STLport's typed 12-byte pair copy helper; retaining that specialization
is necessary for the 90-byte body. A full linked fixture uses heap-backed
arrays to verify compaction, preserved entries, both virtual child-map
queries, and stable repeated calls. `gate_definition_shrink_helpers.py`
checks the six bodies, retail call targets, five provider omissions, and
the composed fixture. The refreshed GFMain closure has **1,125 sources,
zero duplicates, 445 unresolved symbols** at
`work/codex-native-startup/native-gfmain-definition-shrink-full-closure-20261004`.
All three template shrink symbols introduced by the manager body now
leave the frontier. The existing symbol-map memory gate still passes
276 compiled and 276 comparative cases.

## Definition scramble no-op retyped

`CMainGameComponent::Run` calls `CDefinitionManager::ScrambleDefs(bool)`
at 0x009AC9E0. Retail contains only `ret 4`; the prior catalog row was
an anonymous `$E2` stand-in with a print-only test. The typed member
matches all three bytes. A linked fixture checks both boolean values,
unchanged manager storage, and provider omission. The dedicated gate and
selected catalog build pass. The selected GFMain closure has **1,120
sources, zero duplicates, 446 unresolved symbols** at
`work/codex-native-startup/native-gfmain-definition-scramble-closure-20261004`.

## Definition symbol clearing corrected

The manifest and catalog label at 0x009ACB10 was a false particle
`_Cons_val` helper backed by a generic stand-in and print-only test.
`CMainGameComponent::InitDefinitions` calls this address when it compiles
definitions. Retail loads `CDefinitionManager::PSymbolMap.Data` at +0x58
and tail-calls `CSymbolMap::Clear` at 0x009E5250. Typed
`CDefinitionManager::ClearSymbols` matches all eight bytes after its
single relocation. A linked fixture checks delegation through +0x58
with a distinct pointer at +0x60 and requires the real forwarder.
The dedicated gate and selected catalog build pass. The selected GFMain
closure has **1,119 sources, zero duplicates, 447 unresolved symbols** at
`work/codex-native-startup/native-gfmain-definition-clear-symbols-closure-20261004`.

## Render double-limit helper retyped

`GFLimitReturn<double>` at 0x0041919C now has a typed VC7.1 template
specialization in place of a naked byte emitter. The size-optimized body
is 37/37 bytes and 16/16 decoded instructions, with 0.875 normalized
opcode similarity. The differing instruction order is in the second
comparison and return-pointer selection. A linked fixture checks lower,
interior, upper, equality, inverted bounds, NaN, signed zero, and provider
omission cases. The dedicated gate and selected catalog build pass. The
source now defines the decorated symbol required by native render code.
The GFMain closure has **1,118 sources, zero duplicates, 448 unresolved
symbols** at
`work/codex-native-startup/native-gfmain-limit-return-double-closure-20261004`.

## Bank info insertion chain recovered

The bank-info map now has typed successor and predecessor iterators,
unhinted and hinted `InsertUnique`, and `InsertNode`. Retail call targets
separate the bank-info predecessor at 0x009AA320 from the alias-map
predecessor at 0x009AA370; the earlier alias source address was corrected
in the manifest, override, index, and gate. The bank-info iterator bodies
are 61/62 and 79/79-byte near matches, with linked traversal cases.

Unhinted insertion matches 254/254 bytes after five call fixups; node
linking matches 206/206 bytes after six. The node object's emitted
`std::pair<const CCharString,CBankInfo>` copy constructor also matches
the 79/79-byte retail helper at 0x009A9AD0. Hinted insertion is a
672/668-byte near match, and its compiled call targets agree with retail.
The composed `operator[]` fixture covers empty, duplicate, left, middle,
right, and hinted insertion, the registered-bank pointer, and omission
of each of the seven map providers. `gate_bank_info_iterators.py` and
`gate_bank_info_insertion.py` are registered in `gate_all.py` and pass.
The refreshed GFMain closure has **1,117 sources, zero duplicates, 449
unresolved symbols** at
`work/codex-native-startup/native-gfmain-bank-info-insert-closure-20261004`.

## Bank info map erase corrected

The prior retail label at 0x009AA7C0, `LTextTreeWalkThrough::BuildTreeArray`,
was false. `CBankFileManager::Close` calls the address with the bank map's
root node. The body recursively erases each right subtree, destroys the
current bank entry, frees its node, then iterates down the left subtree.
The typed `CBankInfoMap::Erase` VC7.1 source matches **all 100 retail bytes
after seven relocations** and all 38 instructions.

A linked fixture checks null input, right-root-left destruction order,
and the four wide-string destructors followed by the key destructor for
each node. Provider omission and selected catalog build pass. The `Close`
and `Erase` sources also pass a composed map-cleanup fixture, with the
erase provider required at link time. The GFMain
closure has **1,082 sources, zero duplicates, 470 unresolved symbols** at
`work/codex-native-startup/native-gfmain-bank-info-erase-closure-20261004`.

## Text persistence save and bool output

TransferObjectSaveText (0x009BAD60) and the bool output adapter (0x00403F90)
match all 59 and 38 retail bytes after relocations. The composed fixture
retains an existing prefix and checks TRUE/FALSE field output with the retail
space separator and semicolon/CRLF terminator. String storage and append are
mocked boundaries. gate_persist_save_bool.py passes and is registered.

## Boolean text input

The typed traits reader at 0x00403FF0 and actual bool dispatch adapter at
0x00403FC0 pass twelve linked cases with native token readers, plus twelve
retail execution checks. Accepted tokens are TRUE/BTRUE and FALSE/BFALSE,
case-insensitively. Unknown values preserve the prior destination; numeric
1/0 generate a parser error as well as the boolean persistence error. The
traits body is behavioral (472 compiled versus 505 retail bytes), not an exact
match. String storage and error callbacks are mocked. The gate is registered.
TransferObjectLoadText has a draft but is not yet validated or registered.

## Named text field loading

TransferObjectLoadText at 0x009BBA40 passes six linked cases with fifteen
native providers and six retail execution checks. It preserves the original
parser, handles repeated matches and SingleTransfer/Finished, requires a
terminating semicolon, and blanks consumed text without changing LF bytes.
String storage, field virtual transfer and contextual errors are mocked.
The body is behavioral (457 compiled bytes), and the gate is registered.

## IME colour transfer

The helper at 0x00405220 passes two linked and two retail execution cases.
R/G/B map to bits 16/8/0, and A defaults from byte 0x013750B0. Each channel
uses its original value as its default. The return forces alpha to 255 even
when the A transfer changes it. The 301-byte provider is behavioral versus
290 retail bytes. Signed-char transfer and string storage remain boundaries.

## Byte persistence dispatch

TransferSignedChar at 0x00405A60 passes five linked and retail mode cases.
The wrapper uses unsigned-byte component/traits providers: binary output is
a zero-extended SLONG, text output prints 0..255, and text input truncates
the SLONG result modulo 256. Native byte text adapters and shared SLONG
writer are linked in the fixture. Byte output (31 bytes) and SLONG output
(39 bytes) match retail after relocation. The dispatcher and byte input
remain behavioral. Binary IO, CRC and field dispatch are mocked. A draft
byte binary input provider exists but has not yet passed its own gate.

## Byte binary input

The traits provider at 0x00405940 passes twelve linked and retail cases
covering six values in both source-window paths. It consumes four bytes
and keeps the low byte. Position, window pointer and remaining-byte updates
agree. Overflow refill is mocked; rejected reads are outside the fixture
scope because retail leaves the result unspecified. Grade is behavioral,
144 compiled versus 142 retail bytes. The provider and gate are registered.

## Composed byte text persistence

Eight linked cases connect 24 native providers through the dispatcher,
named-field loader/save, byte adapters, SLONG expressions and token readers.
They cover defaults on/off, missing fields, repeated fields, symbols, shifts,
negative truncation and unsigned decimal save. Original parser state remains
unchanged. String storage, symbol lookup/sort, formatting, CRC and contextual
errors remain mocked. gate_persist_byte_composed.py is registered.

## Definition cache and IME alpha data

Five typed globals match 32,781 initialized retail bytes: cached manager
null, cached offset 0xFFFFFFFF, cached size zero, a zeroed 32 KiB block and
IME alpha 255. The block extent also follows the decompressor destination
capacity at 0x009AD5F0. The data gate verifies COFF BSS extents and initialized
bytes against the retail image. Providers are registered in the index.
The last completed closure before these globals contains 1,184 sources,
zero duplicates and 440 unresolved.

## Definition class insertion and partition

The auxiliary insertion pass at 0x009B80C0 matches 171 retail bytes after
relocation. The partition at 0x009B8630 is behavioral. Sixty-one linked
cases check name/index-vector payload integrity, sorting and partition
invariants. Twenty-nine partition cases additionally compare exact key
order and split point against retail execution, with retail swap mocked
as full-entry exchange. Native copies/assignment use vector implementations;
string storage is mocked. Both providers and the gate are registered.
The completed cache-data closure has 1,185 sources, zero duplicates and
436 unresolved, before these sorting providers.

## Definition class heap fallback and complete sort

The partial-sort entry at 0x009B9030 matches all 244 retail bytes after six
relocations. Its emitted heap helpers are behavioral providers verified
through 82 linked cases against exact retail key order. Linked entries have
real name/index-vector payloads; the retail fixtures use empty vector payloads
and mocked string/vector assignment. The complete sort links eight native
providers and passes 27 cases across the 16-element threshold, ascending,
reverse and duplicate inputs, and forced zero-depth heap fallback. Both
gates are registered. The preceding helper closure completed with 1,187
sources, zero duplicates and 433 unresolved.

## Definition manager cleanup

The destructor at 0x009B02E0 passes four linked and retail cases covering
empty through three-node recompiled trees, mixed branches, retained def
references and counted-symbol-map callback release. Native tree erase at
0x009B40F0 matches all 75 retail bytes after relocation. The destructor
remains behavioral (379 versus 387 bytes); ClearInstantiatedDefs, strings
and resource-list cleanup are mocked. The current tree header omits its
destructor, so the manager explicitly clears nodes and frees the header.
Both providers and the gate are registered. Complete-sort closure completed
with 1,188 sources, zero duplicates and 432 unresolved before cleanup.

## Definition block compression

Donor confirms CompressDefinitionBlock at the retail 0x009AE390 helper,
correcting the prior primitive-compression label. Four linked/retail cases
verify table-prefix adjustment, 16-bit wrapping, compressor capacity, existing
compressed-prefix retention, block index/position and temporary input cleanup.
Vector growth/clearing is native; memory-stream storage and compressor runtime
are mocked. The 669-byte body is behavioral versus 600 retail bytes. The
parameter uses the current CArrayVector retail layout model; its name differs
from the donor CArray type. Provider and gate are registered.

## Definition binary save

SaveBinaryDefinitions at 0x009AF7A0 passes seven linked/retail cases, with
exact serialized output and block first-index/count/payload/offset checks.
Cases include empty, small, exactly 32 KiB, one-byte overflow, oversized
single defs and empty defs after a full block. References release after
serialization; optional allocator statistics run before and after release.
The body is behavioral (999 versus 1007 bytes). Stream storage, table save,
def virtual serialization, block compressor, offset sort/shrink and stats
are mocked. Donor confirms SaveTable(bool), GetNoAllocatedAreas and stream
Clear. Native Clear (87 versus 83 bytes) is linked in save/block fixtures,
including reserved-empty buffer release. Using Clear brings the block helper
to 608 compiled bytes. Providers and gates are registered.
Cleanup closure completed with 1,190 sources, zero duplicates and 431
unresolved before the save providers.

## Composed definition saving

Seven linked cases now use the actual SaveBinaryDefinitions,
CompressDefinitionBlock, binary context Init and memory-stream Clear
providers. The compressor boundary checks packed payload bytes, adjusted
16-bit offset entries, input length and scratch capacity. Controlled compressed
output allows exact serialized comparison with the retail outer-save fixture.
Table save, virtual def transfer, stream storage, offset sorting/shrinking and
allocator statistics remain mocked. The composed gate is registered.
