# Play seam: first trap-driven landings, 2026-09-26

`tools/decomp_pipeline/play_seam.py` links CGame::Play over every landed genuine source,
traps the rest, runs, and prints the first missing callee in execution order. A single
driver agent worked the queue; the lead verified with `gate_all.py --bootstrap`.

Traps cleared, in execution order:

1. `CNewFrontendGameComponent::CompileDefsPlatformSpecific` 0x42F5A9: builds the
   CStartupDefinitionManager over `xfrontend.bin` / `frontend.bin`, registers FRONT_END,
   UI_MISC_THINGS_DEF, UI_ICONS_DEF and (PC only) CONTROL_SCHEME, then
   InitAndCompilePlatformSpecific.
2. The wide-string path the operator+ units reached through a wrong-ABI stand-in:
   STLport `basic_string<wchar_t>::_M_range_initialize` 0x99C670 and
   `CWideString::AllocStringData` 0x99B3C0. `CWideStringData` now has its retail layout
   (`{ basic_string<wchar_t> Data; owners; }`); old field names compile through
   `__declspec(property)` shims so existing landings keep parity.
3. `CStartupDefinitionManager` ctor 0x433693, `InitAndCompile` 0x43368B, `ErrorCallback`
   0x43368C (the last two replaced stand-ins).
4. Lead: `_STL::_String_base<wchar_t>::_M_throw_length_error` 0x99C550 (STLport without
   exceptions: `puts("basic_string"); abort();`), which the real string ctors now
   reference. It is a verified bootstrap leaf and linked next to the operator+ units.

All RELOCATION_MATCH/MATCH with fixtures and mutation checks. compare_candidate_objects.py
maps operator names to their mangling codes when picking the symbol.

The seam now stops at the CDefinitionManager constructor 0x9B0470 (370 B): the start of the
definition compiler. Its retail layout (0xD0, CResourceList base, symbol maps, vector maps,
compressed def data, recompiled-defs map) is recorded in the driver's log; next are
CResourceList, CSymbolMap, AddDefClass 0x9B0AC0 and InitAndCompilePlatformSpecific 0x4336BC.

Verification: `gate_all.py --bootstrap`: 123/123 boot leaves, component ABI, 0 regressions
across 247 header dependents, bootstrap references, ring 0 unresolved, full bootstrap.

## Second driver pass: the definition manager's constructor path

Picked up an interrupted pass (uncommitted headers for CDefinitionManager's retail layout,
CResourceList/CSymbolMap/CDefPointer split out of the game and text-bank headers) whose
gate was red: three boot leaves and the particle OpenBank dependent failed to link once
CResourceList had an out-of-line constructor.

Landed, all RELOCATION_MATCH with a behaviour fixture (mutation-checked where noted):

| address | function | notes |
|---|---|---|
| 0x9FC570 | `CResourceList::CResourceList()` | new |
| 0x9FC5C0 | `CResourceList::~CResourceList()` | new |
| 0x9FC540 | `CResourceList::HandleFailedAllocation` | retyped in place: was an `extern "C"` byte-match, now the member (`return UnloadNextResource(0);`) |
| 0x9B0470 | `CDefinitionManager::CDefinitionManager` | new, mutation-checked; its unit also defines `CompiledDefDirectory` (retail initializer 0x1224050) |
| 0x9E5200 | `CSymbolMap::CSymbolMap()` | new, mutation-checked |
| 0x9B1F50 | `CCountedPointer<CSymbolMap>::Reset` | new, mutation-checked |
| 0x99EA60 | `CCharString::AllocStringData` | new, mutation-checked |
| 0x9A0590 | `CBasicString<char>::CBasicString(const char*)` | retyped in place (was a local-struct `InitFromCStr`) |
| 0x9A0300 | `CBasicString<char>::SetString(const char*, unsigned long)` | retyped in place (was `AssignFromData` on a local struct, defining its own `operator new[]`) |
| 0x9E5420 | `CSymbolMap::AddSymbolDirty(name, val)` | new, mutation-checked; unit also defines `CSymbolMap::CurrentFilename("NULL")` (retail 0x13CAA30) |
| 0x9E54E0 | `CSymbolMap::AddSymbolDirty(group, name, val)` | same source file as 0x9E5420 (second catalog entry, own fixture) |
| 0x9E7AB0 | `vector<pair<CCharString, CSymbolInfo> >::push_back` | new, mutation-checked |
| 0x9E6270 | `__uninitialized_copy<pair<CCharString, CSymbolInfo>*>` | new, mutation-checked (callee of the overflow path below) |
| 0x9E62E0 | `__uninitialized_fill_n<pair<CCharString, CSymbolInfo>*>` | new, mutation-checked (same) |
| 0x6FBCC0 | `__destroy_aux<pair<CCharString, CSymbolInfo>*>` | new, mutation-checked (same; a folded instantiation) |

Header model changes:

- `fable_resource_list.h`: `CFailedAllocationHandler` base (vtable 0x129B874, RTTI);
  `CIVCountedPointeeBase` ctor (`IVRefCount = 1`) and dtor inline; `CResource` ctor, dtor and
  `RemoveFromResourceList` inline; `RemoveResource` 0x9FC020 / `UnloadNextResource` 0x9FC170.
- `test_support/resource_list_stubs.h` now includes the three real CResourceList bodies and
  stubs only RemoveResource/UnloadNextResource; fixtures define DeleteCIVCountedPointeeBase.
  The GetWindowTitle, CDataBank (x2) and OpenBank fixtures and the phase-7 window-title
  boundary use the real bodies instead of their own empty `~CResourceList`.
- `fable_basic_string.h` (new): `CBasicString<T>` (13 bytes packed, 31-bit DataLength plus
  UseFastExtend). `CCharStringData` is now `{ CBasicString<char> Data; owners; }` with the
  retail ctors; the old field names (`text`, `unknown04`, `unknown08`, `flags0C`) are
  `__declspec(property)` shims. `owners` stays a real field: `--data->owners <= 0` through a
  property is C2120, and `delete[]` through the `text` property ICEs VC7.1, so
  `CCharString::UnassignString` 0x99E9B0 now uses `Data.PData`/`Data.DataLength` (still
  RELOCATION_MATCH). A fixture-only empty default ctor keeps static fixture storage compiling.
- `fable_symbol_map.h`: `CSymbolMap : CBaseClassNonCopyable`, `CSymbolInfo` ctor, the static
  `CurrentFilename`, both AddSymbolDirty forms, `UpdateDirty() const` (FableWin mangling).
- STLport: `pair` ctors; `_Rb_tree_iterator`, `insert_unique` 0x4AC380 and `set::insert`;
  `vector::push_back` / `_M_insert_overflow` declarations; `__true_type`/`__false_type` with
  empty ctors (retail never stores into a tag temporary; VC7.1 zero-fills `T()` of an empty
  POD); `(max)`, `_Construct`, `_Destroy`, and fastcall declarations of
  `__uninitialized_copy` 0x9E6270, `__uninitialized_fill_n` 0x9E62E0, `__destroy_aux` 0x6FBCC0.
  The header does not provide placement new (fixtures define their own); the landings that
  construct in place include `<new>`, whose `throw()` placement new gives retail's null check.
- `CVectorMap::Insert` (`__forceinline`; plain `inline` stays a call).

Seam note: every unresolved data symbol in the seam aliases one zero block, so a static
that is read before it is written must be defined by a landed unit (CompiledDefDirectory
crashed operator+ until its definition landed with the ctor).

The seam now stops at
`vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::_M_insert_overflow` 0x9E77E0 (the first
push_back into an empty SLONGMap). Near miss, not landed: STLport's body compiles to 262 bytes
against retail's 258, and the only difference is the first `__false_type()` temporary getting
its own stack slot (`sub esp, 8`) where retail overlays it on `__atend`'s argument slot
(`push ecx`), as both builds do for the other three tags. Tried: `__new_finish = __new_start`
first, the `_STLP_TRY` block scope, forwarding the tag parameter, a named tag, an explicit max,
the public `uninitialized_copy` wrapper, `optimize` pragmas, /GS, /Oa and the QFE-4035
compiler. Its three range helpers are landed (above). After it: `CSymbolMap::UpdateDirty`
0x9E5C30 (sort 0x9E7E40 when dirty, then a tail call 0x9E7C70 on SLONGMap), AddDefClass
0x9B0AC0 (profiled scope, CDefClassInfo insert, editor-only def reload) and
InitAndCompilePlatformSpecific 0x4336BC (a frame-pointer body that pushes the symbol and
compile path lists).

## Third driver pass: the symbol map's sort and the def-class registry

`play_seam.py` now links `rebuild/seam_scaffold/*.cpp` (behaviourally faithful near-miss
bodies, never landed; see that folder's README). The pass started with a scaffold for
`_M_insert_overflow` 0x9E77E0; the byte-match agent then solved it (the TU must see the
bodies of `__uninitialized_copy` / `__uninitialized_fill_n` / `__destroy_aux`, now in
`fable_stlport_uninitialized.h`), it landed, and its scaffold was deleted.

Traps cleared, in execution order (all RELOCATION_MATCH with a fixture and at least one
killed mutant, except where noted):

| address | function | notes |
|---|---|---|
| 0x9E77E0 | `vector<pair<CCharString,CSymbolInfo>>::_M_insert_overflow` | byte-match agent's solve, integrated |
| 0x9E5C30 | `CSymbolMap::UpdateDirty` | inline `CVectorMap::Sort`, tail call |
| 0x9E7E40 | `sort<pair<CCharString,CSymbolInfo>*, CCompareString>` | final insertion pass expanded |
| 0x9E7D30 | `__introsort_loop<...CSymbolInfo...>` | comparator = `CCharString::FastCompare` (out of line, 0x9E6530) |
| 0x9E7590 | `__insertion_sort<...CSymbolInfo...>` | retail's form has no value-type tag |
| 0x9E7C70 | `CVectorMap<...>::DeallocateUnusedMemory` (Ego_r CArray member) | needs the `reserve`/`_M_range_insert` bodies visible (see below) |
| 0x433D3A | `CCountedPointer<CStartupDefinitionManager>::Reset` | de-baked (was an asm_bake entry) |
| 0x9B0AC0 | `CDefinitionManager::AddDefClass` | defines `UsingCompiledDefs` / `CreateChecksums` statics |
| 0x535470 | `vector<unsigned long>` copy ctor | shared instantiation |
| 0x9B9170 | `vector<pair<unsigned long,CDefClassInfo>>::push_back` | |
| 0x9B8DD0 | `..._M_insert_overflow` (DefClassEntry) | same body as 0x9E77E0 |
| 0x9B9490 | `sort<...CDefClassInfo...>` | final insertion pass stays a call (0x9B91F0) |
| 0x9B9320 | `__introsort_loop<...CDefClassInfo...>` | pivot copy expands the vector copy ctor |
| 0x9B8F70 | `__insertion_sort<...CDefClassInfo...>` | |
| 0x99F690 | `operator+(const char*, const CCharString&)` | ctor/copy/dtor bodies inline in the unit (as the CWideString operator+ units) |
| 0x99F0A0 | `CCharString::operator+=(const CCharString&)` | retyped in place (was a local-struct extern "C" body whose test tested its own copy) |
| 0x99EAF0 | `CCharString::MakeOwnCopyOfData` | new |
| 0x9A04E0 | `CBasicString<char>::Append` (FableWin name; header had "AppendData") | retyped in place |

Also landed (not traps; their bodies became COMDATs of the landings above and are now
verified too): `vector::reserve` 0x9E72C0, `vector::_M_range_insert` 0x9E73F0, and
`CCountedPointer<CStartupDefinitionManager>::ReleaseData` 0x433CC0 (retyped in place from a
local-struct `_Dest_val` body). `CStartupDefinitionManager::Set` 0x4336B0's unit now defines
`PClass` (retail 0x13B8760); its fixture no longer does.

Scaffolds (`rebuild/seam_scaffold/`): `CBasicString<char>::ExtendData` 0x9A01C0, 315 B vs
316: retail spills the new data length into the argument slot at the start of the
fast-extend branch and runs the highest-bit search in esi/edx; five source shapes tried.
Probably Lionhead's `AllocateDataFromStringLength` / `AllocateDataFromDataLength` helpers
(FableWin), not recovered.

Findings:

- VC7.1 uses what it knows about callee bodies compiled earlier in the unit. Two cases:
  0x9E77E0's tag temporaries (the byte-match agent's find) and 0x9E7C70, whose dead stores
  to the swapped temporary vanish only when `reserve` and `_M_range_insert` have visible
  bodies. STLport keeps those bodies in headers; so do we now:
  `fable_stlport_uninitialized.h` (included by `fable_stlport_vector.h`),
  `fable_stlport_vector_impl.h` (copy ctor, push_back, _M_insert_overflow, reserve,
  _M_range_insert; opt-in), `fable_stlport_algobase.h` (copy / copy_backward),
  `fable_stlport_algo.h` (sort declarations + inline glue), `fable_stlport_algo_impl.h`
  (sort), `fable_stlport_introsort.h`, `fable_stlport_insertion_sort.h`. The helper
  landings are now one-line explicit instantiations of those bodies.
- A body in a header is emitted as a COMDAT by every unit that instantiates it, and a
  fixture's explicit specialization of the same symbol is then LNK2005. So each sort level
  has its own header (a fixture stubs the level below), and fixtures of the three
  uninitialized helpers define `FABLE_STLPORT_DECLARATIONS_ONLY` so they call the landed
  instantiation.
- `~vector` now goes through STLport's `_Destroy(first, last)` with a reduced
  `__type_traits` (scalars/pointers trivial, classes not): class elements call
  `__destroy_aux` out of line, as retail does for pair<CCharString, CSymbolInfo>.
- The seam: every unresolved data symbol now gets its own zero block (`seam_traps.py`). One
  shared block made any counter bump (the CCharString instance count) look like a non-null
  `NProgressDisplay::PProgressDisplay`, and the seam crashed in DisplayProgress. Traps call
  `_exit` so static destructors do not report a second trap. `play_seam.py` rebuilds all
  objects each run: `link_closure.py` reused objects whose source was older, missing header
  edits (UpdateDirty was trapped under its stale non-const mangling).
- AddDefClass takes the NULLDEF_ path in the seam because `UsingCompiledDefs` is 0 (GFMain,
  which sets it from a console variable, does not run). Retail takes the same path by
  default anyway: GFMain copies 0x1375459 (initialised to 1) into `CreateChecksums`.

Header changes (all additive or layout-neutral): the STLport headers above; iterator tags
moved from `fable_stlport_string.h` to `fable_stlport_vector.h`; vector gained capacity,
get_allocator, swap, reserve, insert/_M_range_insert, the copy ctor declaration and the
private `_M_allocate_and_copy`/`_M_clear`/`_M_set`; `_Vector_base(n, a)`;
`CVectorMap::Sort` (inline), `DeallocateUnusedMemory`, `DeallocateMemory`;
`CCompareString::operator()` (FastCompare) and `CKeyPairCompareLess::operator()`;
`CCharString::FastCompare`, `operator+=(const CCharString&)`, `MakeOwnCopyOfData`,
`operator+(const char*, const CCharString&)`; `CBasicString::Append`/`ExtendData`;
`CDefinitionManager::CInstantiatedDefInfo` layout, `GetPDefClassInfo`, `InstantiateDef`,
`LoadDef`, the two static switches; `CPersistContext()` + `InitClearMode`; new
`fable_def_class_base.h` (CDefClassBase with the retail 25-slot vtable order; do not include
it together with the generated `engine/CDefClassBase.h`).

The seam now stops at `CDefinitionManager::InstantiateDef` 0x9ADC90 (the NULLDEF_ default
of FRONT_END), followed by `GetPDefClassInfo` 0x9AD2E0, `LoadDef` 0x9AD6E0,
`CPersistContext::InitClearMode` 0x9BAA70 and the def's `Transfer`.

## Fourth driver pass: instantiating the FRONT_END default

Starts where the third pass stopped (InstantiateDef 0x9ADC90). First integrated the
byte-match agent's `CBasicString<char>::ExtendData` 0x9A01C0 (five protected FableWin helpers
declared in `fable_basic_string.h`, bodies in the landing). Its scaffold file still exists
(deleting it was refused in this session); the lead should delete
`rebuild/seam_scaffold/CBasicString_char_ExtendData_009a01c0.cpp`.

Landed, all with a fixture and at least one killed mutant:

| address | function | notes |
|---|---|---|
| 0x9A01C0 | `CBasicString<char>::ExtendData` | byte-match agent's solve, integrated |
| 0x9AD580 | `CDefinitionManager::GetPDefClassInfo` (non-const) | inline `CVectorMap::Find`; the search takes its own copy of the key |
| 0x9B2230 | `CVectorMap<ulong, CDefClassInfo>::LowerBound` | MATCH; body only in the landing |
| 0x9ADC90 | `CDefinitionManager::InstantiateDef` | see findings |
| 0x433080 | `CFrontEndDef::Alloc` | the whole def hierarchy; manifest row added (xref: immediate at 0x42F623) |
| 0x4303B0 | `CParentDefClassBase::GetInstantiationName` | retyped in place; replaces a naked-asm bake |
| 0x4303C0 / 0x4303D0 / 0x4303E0 | `SetInstantiationName` / `IsParentDef` / `GetPSubDefInfoMap` | retyped in place (were local-struct bodies) |
| 0x9D5060 | `CDefString::SetString` | retyped in place; the file is still named `CRegion_SetMiniMapRegionExitTextOffsetX_009d5060.cpp` |
| 0x9FBEC0 | `CResource::CResource` | now defined in the CResourceList ctor unit (0x9FC570), which still matches; needs its own catalog entry pointing at that source |
| 0x9D4F90 | `CDefStringTable::AddString` | defines `g_DefStringTableText_013CA824` |
| 0x9D4DB0 | `CDefStringTable::CDefStringTable` | defines `GDefStringTable`; inline `CCRC::Calc` |
| 0x996AF0 | `CMemoryDataOutputStream` ctor | |
| 0x994B40 | `CMemoryDataOutputStream::GetLength` | MATCH |
| 0x994230 | `CDataOutputStream::WriteRepeatedByte` | `/GS` (catalog CompilerFlags) |
| 0x995AE0 | `CDataOutputStream::WriteNullTerminatedString` | |
| 0x4122D0 | `vector<char>::resize` | generic body in `fable_stlport_vector_impl.h` (also covers the `_emit` bake at 0x442770) |
| 0x4110D0 | `vector<char>::_M_fill_insert` | POD form |
| 0x519230 | `map<CCharString, long>::operator[]` | new `fable_stlport_tree_impl.h` |

Scaffolds added: `STLMap_CCharString_long_find_0042b467.cpp` (`_Rb_tree::_M_find`, 91 vs 92 B:
retail's frame-pointer, size-optimised unit keeps the loop unrotated and the candidate in a
stack slot) and `CMemoryDataOutputStream_Write_PeekData_00996cc0.cpp` (Write matches apart from
PeekData's inlined empty test; PeekData alone is `sub` vs our `cmp`).

Findings:

- `/O2` includes `/Ob2`: VC7.1 expands any function whose body is earlier in the unit.
  Retail's ResourceList.cpp, MemoryDataOutputStream.cpp and DefStringTable.cpp each inline a
  sibling that other units call out of line (CResource ctor, PeekData, CCRC::Calc). Model it by
  defining the sibling in the same landing; the second address then needs a catalog entry that
  points at that source.
- A callee that looked like `CDefString(const CCharString&)` does not return `this`, so it is a
  plain member (`SetString`) and the converting ctor is inline. Getting that right also fixed
  InstantiateDef's scheduling. InstantiateDef needs the global index as a `long`, because the
  unsigned `GlobalIndices.push_back` then gets a converted copy. It also needs the new info
  held in a named local, and the POD `_M_insert_overflow` body visible.
- The def-class hierarchy is now modelled: `CDefPointeeBase : CResource`, with lock/unlock on
  IVRefCount. It replaces the stub with two unnamed virtuals and `RefCount`, and four fixtures
  were ported. `CDefClassBase` has an inline ctor and bitfields. `CParentDefClassBase` (0x39 B)
  and `CFrontEndDef` (0x54 B, `fable_frontend_def.h`) are new. `CDataOutputStream` is now a
  `CBaseClassNonCopyable` with the retail slot order (still spelled `struct`).
- STLport tree lookups return node pointers (`_M_find`, `_M_lower_bound`), and the iterator has
  user ctors, including a copy ctor that assigns in its body. Without those, the hinted
  `insert_unique` and `operator[]` do not match.
- `CDefPointer(T*)` takes over the new object's reference without locking it.

The seam now stops at `_Rb_tree<CCharString, pair<const CCharString,long>>::_M_lower_bound`
0x47547D (73 B, /O2, less<CCharString> and `CBasicString<char>::operator<` expanded to a
`Compare` 0x411570 call). After it comes the hinted `insert_unique` 0x516AF0.

## Fifth driver pass: the string-table map, LoadDef, and the startup def classes

Starts at `_Rb_tree<CCharString, long>::_M_lower_bound` 0x47547D. The seam now runs through
the FRONT_END registration, LoadDef, the def string table's map inserts, the first def
transfer and `InitAndCompilePlatformSpecific`, and stops at `CDefinitionManager::Compile`
0x9B08C0 (the def compiler proper: CheckDependencies, the compiled-file open through
CDiskFileWin32 / CFileDataInputStream, LoadBinaryDefinitions, CreateSymbolsFromPathList).

Landed, all with a fixture and at least one killed mutant (source or header mutation):

| address | function | notes |
|---|---|---|
| 0x516AF0 | `_Rb_tree<CCharString,long>::insert_unique(hint, v)` | new; matches once `_Rb_global`'s step bodies are visible |
| 0x429950 | `CBasicString<char>::operator<` | retyped in place; replaces an `_emit` bake; oracle row corrected to 88 B (was a truncated 85) |
| 0x411570 | `CBasicString<char>::Compare` | retyped in place (was a free `strcmp_signed`) |
| 0x4294E2 | `_Rb_global<bool>::_M_decrement` | new, /Os |
| 0x42951B / 0x42955B | `_Rb_global<bool>::_Rotate_left` / `_Rotate_right` | retyped in place (the first was an `_emit` bake, the second a local-struct body) |
| 0x42971D | `_Rb_global<bool>::_Rebalance` | new, /Os |
| 0x9AD6E0 | `CDefinitionManager::LoadDef` | new; the three progress strings are block-scoped named locals |
| 0x9BAA70 | `CPersistContext::InitClearMode` | retyped in place (was a local-struct fastcall) |
| 0x9BAA30 / 0x9BAA50 | `CPersistContext::Init(CMemoryDataInputStream&, bool)` / `Init(CDataOutputStream&, bool)` | the first retyped (was a naked-asm "ctor"); not constructors: they do not return `this` |
| 0x4331C0 | `CFrontEndDef::Transfer` | retyped in place (was an `_emit` bake); `#pragma inline_depth(0)` |
| 0x4331F9 / 0x431102 | binary-only member transfers `<vector<CCharString>>` / `<long>` | retyped in place; /Os; `TransferBinaryVector` / `TransferBinaryMember` (names not recovered) |
| 0x404500 | `CPersistContext::CheckCRC` | new; WriteULONG must not be visible (see below) |
| 0x433227 | `GFSerialiseVectorBinaryOut<CCharString>` | retyped in place (was an `_emit` bake); /Os |
| 0x993EA0 / 0x993EB0 | `CDataOutputStream::WriteULONG` / `WriteSLONG` | retyped in place (were local-struct `WriteCBYTE`s) |
| 0x9B8C00 / 0x9B76F0 | `__linear_insert` / `__unguarded_linear_insert` `<pair<ulong, CDefClassInfo>*>` | new; the second DefClassInfo sort |
| 0x4336BC | `CStartupDefinitionManager::InitAndCompilePlatformSpecific` | new, 1507 B, /Os; the path lists, five def classes, Compile |
| 0x41A080 / 0x9A76A0 | `NResourceDirectoryNames::GetDefDir` / `NBankFileManager::GetHeaderDir` | retyped in place (were local-struct bodies) |
| 0x99C7E0 | `basic_string<wchar_t>::append(first, last, forward_iterator_tag)` | new; 306 B including the `int3` after the noreturn length-error call |
| 0x99B940 | `CWideString::operator+=(const wchar_t*)` | retyped in place: `storage_->Data.append(text)` instead of a five-argument stand-in |
| 0x4128A0 | `vector<CWideString>::push_back` | retyped in place (was a local-struct body) |

The three wide `operator+` boot leaves (0x99BE70, 0x99BF30, 0x99BFF0) now append through the
real STLport append; their fixtures check the text instead of a stand-in call count.

Scaffolds added (`rebuild/seam_scaffold/`): `_M_lower_bound` 0x47547D (65 vs 73 B: retail
expands operator< to a Compare call and hoists the key, which needs Compare's body in sight,
yet does not expand Compare; `__declspec(noinline)` gives 73 B with ebx/edi swapped),
unhinted `insert_unique` 0x431606, `_M_insert` 0x4316CF and `_M_create_node` 0x431773 (a
size-optimised unit that still expands the key compare down to Compare, or the pair copy),
`vector<unsigned long>::operator=` 0x45BC09 (/Os; retail zero-fills and copies the POD tag and
keeps `_M_clear` out of line) and `vector<CWideString>::operator=` 0x433DF0 (retail expands
`_M_allocate_and_copy` and `_M_clear`). The insert_unique(hint) scaffold from earlier in the
pass was deleted when it landed.

Not integrated: the byte-match agent's `_M_find` 0x42B467 and `CMemoryDataOutputStream::Write`
0x996CC0 solves (copying them from its temp folder was refused by the permission system). Its
header diff is applied (`CCharString::operator<` is `__forceinline`, `fable_carray.h`,
`CMemoryDataOutputStream::Data` is a `CArrayVector<char>`); the two scaffolds remain, and
0x42AAB3's catalog entry is unchanged.

Findings:

- Per-unit body visibility decides several matches. `CheckCRC` 0x404500 matches only if
  `WriteULONG` is a declaration (retail calls 0x993EA0), while `CDefStringTable::AddString`
  expands it: the body moved to the opt-in `fable_data_output_stream_inline.h`. The hinted
  insert needs the `_Rb_global` step bodies; `_M_lower_bound` needs `Compare`'s body but not
  its expansion, which no header arrangement reproduced.
- Named block-scoped locals are packed into dead argument slots and addressed with `lea`;
  unnamed temporaries are not, and VC7.1 uses the returned `eax` instead (LoadDef's progress
  strings and class-name lookup, CheckCRC).
- VC7.1 inlines template members defined outside the class even under /Ob1, and `/Obn` given
  after `/O2` does not take effect; `#pragma inline_depth(0)` or `auto_inline(off)` does
  (CFrontEndDef::Transfer keeps both member transfers out of line with /O2 registers).
- A function that ends with `eax = 0` after storing through `ecx` is not a constructor:
  0x9BAA30 is `CPersistContext::Init(stream, bool)` (FableWin), so LoadDef builds an empty
  context and calls Init.
- `CDefPointer::Reset(T*)` takes over the reference without locking (LoadDef: Release, then
  the plain store), like `CDefPointer(T*)`.
- Retail's `userst.ini` has `UseCompiledDefs TRUE;` and `AllowDataGeneration FALSE;`, the
  console variables behind `UsingCompiledDefs` (0x13B8617 -> 0x13CA7D8) and `CreateChecksums`
  (0x1375459 -> 0x138E189). Left at zero, the seam's AddDefClass transferred a NULLDEF_
  default in the defaults mode, and the binary-only vector transfer wrote to the context's
  unset save stream. `play_seam.py`'s main now applies the two switches as GFMain would.
- `seam_traps.py` bound C-linkage globals (`_g_*`) to function traps, so a zero-initialised
  retail global (the bank header directory 0x13CA7D4) read as code bytes; they now get zero
  data blocks.
- Header model changes: `CBasicString<T>::Compare` (body under
  `FABLE_BASIC_STRING_DECLARATIONS_ONLY`) and an inline `operator<`, which
  `CCharString::operator<` now uses; STLport `_Rb_global<bool>` (steps, rotations, rebalance)
  behind an `_Rb_tree_base_iterator` base, `_Select1st::operator()`, `pair::first_type`,
  tree `_M_insert` / `_M_create_node` / `size` / `_M_leftmost` / `_M_rightmost` / `_S_key`;
  `vector::operator=` (declared; body in the opt-in `fable_stlport_vector_assign.h`), the POD
  `__uninitialized_copy` is fastcall and `_M_allocate_and_copy` follows `is_POD_type`;
  new `fable_stlport_linear_insert.h` / `fable_stlport_unguarded_linear_insert.h`;
  `basic_string::append` (C string, range, forward-iterator), `size`, `capacity`,
  `_M_deallocate_block`; `CPersistTraits<long>` / `<CCharString>`, the two binary-only
  transfers, `GFSerialiseVectorBinaryOut`; `CDataInputStream()` (out of line, 0x993B20),
  `GetLength`, an inline `CMemoryDataInputStream()`; `CResource::SetResourceSize`,
  `CResourceList::FrameEnd` / `AddResource`, `CDefStringTable::GetString`,
  `CDefinitionManager::DecompressDefinitionData` / const `GetPDefClassInfo` /
  `SetSymbolPathList` / `SetCompilePathList` / `Compile`; the startup def allocators;
  `GetDefDir`, `GetHeaderDir`. Fixtures that define a `CFileDataInputStream` constructor
  (and `integration/phase6_runtime_boundary.cpp`) now also define `CDataInputStream()`.

Verification: `gate_all.py --bootstrap` (header dependents: 0 regressions across 328 entries).
The seam stops at `CDefinitionManager::Compile` 0x9B08C0.

## Sixth driver pass: the compiled-def load

Starts at `CDefinitionManager::Compile` 0x9B08C0. The seam now runs the whole compiled
path: dependency check, the def file's open through CDiskFileWin32 / CFileDataInputStream,
the chunked stream reads, names.bin, LoadBinaryDefinitions and the stream teardown. It
stops at `CDefinitionManager::GetDef<CFrontEndDef>` 0x4300FB (a /Os frame-pointer
template, folded with the other GetDef<T> bodies; it calls the const
GetPDefFromClassIndex 0x9ADE10).

Landed (all with a fixture and at least one killed mutant; M = MATCH, R = RELOCATION_MATCH):

| address | function | grade | notes |
|---|---|---|---|
| 0x9B08C0 | `CDefinitionManager::Compile` | R | unit defines `CreatingCompiledDefs` (retail 0x138E188, initialised to 1) |
| 0x9B05F0 | `CDefinitionManager::CheckDependencies` | R | new `fable_checksum_cache_file.h` |
| 0x9974A0 | `NFile::CalcCurrentPath` | R /GS | |
| 0x997F80 | `CAFile::GetCurrentPath` | R | needs the if/return form of `CWideString::operator==` |
| 0x998C90 | `CAFile::ExpandPath` | R | |
| 0x999140 | `CAFile::ConvertPathToFullPath(path, full)` | R | |
| 0x99A6A0 | `CDiskFileWin32::Open` | R | jump table: verify_land_jumptable.py; real temp-file fixture |
| 0x99AF00 / 0x99AF10 | `CWideString::operator const wchar_t*` / `GetAsCharArray` | R | retyped in place (were local-struct `CCharString` bodies) |
| 0x993B20 + 0x994700 | `CDataInputStream()` + `CFileDataInputStream(file, n)` | R | one unit source (the base ctor expands into the file ctor); 0x994700 repointed |
| 0x993BC0 | `CDataInputStream::SetPosition` | M | retyped in place |
| 0x993CA0 | `CDataInputStream::ReadWithSrcChunkOverflow` | R | new source, catalog repointed |
| 0x993B90 / 0x993BA0 / 0x993BB0 | base source hooks | M | retyped in place |
| 0x9943B0 / 0x994360 / 0x9943D0 | `CFileDataInputStream` source hooks | M | 0x9943B0 retyped; the other two new |
| 0x993C80 + 0x994300 | `CDataInputStream::Close` + `CFileDataInputStream::Close` | M / R | one unit source; 0x993C80 repointed |
| 0x994780 | `~CFileDataInputStream` | R | new source, catalog repointed |
| 0x9D4AB0 | `CDefStringTable::ReadTable` | R | names.bin |
| 0x996BC0 | `CMemoryDataOutputStream::ReserveSize` | R | retyped in place (was a `_Cons_val` local body) |
| 0x996970 | `CDataOutputStream::CopyFrom` | R | |
| 0x997330 | `vector<char>::reserve` | R | one-line instantiation |
| 0x9964E0 | `CDataInputStream::CopyTo` | R | inline `vector(n)` body now in `fable_stlport_vector_impl.h` |
| 0x9AD950 | `CDefinitionManager::ClearInstantiatedDefs` | R | retyped in place |
| 0x9D4A20 | `CDefStringTable::GetStringCRC` | R | retyped in place |

Catalog repoints (old file left on disk: the first three are asm bakes the OpenRetailBank
ring still links by path; the fourth is an orphaned local-struct body):
0x994700 `CFileDataInputStream_CFileDataInputStream_00994700.cpp` -> `CDataInputStream_CDataInputStream_00993b20.cpp`;
0x993CA0 `CDataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp` -> `DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp`;
0x994780 `CFileDataInputStream_dtor_00994780.cpp` -> `CFileDataInputStream_Destructor_00994780.cpp`;
0x993C80 `CDataInputStream_Close_00993c80.cpp` -> `CFileDataInputStream_Close_00994300.cpp`.
Oracle rows for 0x993B20 / 0x994700 / 0x994780 now carry mangled prefixes (`::??0...`,
`::??1...`) so parity does not pick the emitted `??_G`.

Scaffolds added: `CDefinitionManager_LoadBinaryDefinitions_009afb90.cpp` (1344 B; best
1358-1390 B: VC7.1 hoists constant 0 into a register from the def-count read on and inlines
the one-argument `resize` retail calls at 0x9B8350) and `CDefString_TransferBinaryIn_0044fc00.cpp`
(same 128 B, but the three duplicated failure tails of the inline Read guard are laid out
in a different order).

Findings:

- The compiled def file's "version" at 0x13CA854 is `GDefStringTable.RandomID` (table +
  0x2C): a compiled file only loads over the names.bin it was built with. The seam's main now
  does GFMain's part: `CompiledDefDirectory` = the install's `Data\CompiledDefs\` (absolute,
  read-only; `FABLE_INSTALL` overrides), `CreatingCompiledDefs = false`, then names.bin via
  `LoadTable(true)`.
- The input-stream vtables are modelled (slots 1-10 of 0x129A6B0 / 0x129A728); the base and
  CDataStream destructors are inline, as retail's ~CFileDataInputStream shows. Fixtures that
  emit a stream vtable include `test_support/data_input_stream_stubs.h` (per-slot
  `FABLE_STUBS_NO_*` opt-outs); private hooks are reached through the test-only friend
  `CDataInputStreamTestAccess`. The ring's aliases for SetPosition / ReadNullTerminatedString
  were updated to the new manglings.
- `new T[n]` / `delete[]` call operator new/delete (not `[]`) unless `<new>` is in the TU;
  retail calls the array forms (0x994700, 0x994300).
- CAFile's ctor/dtor are inline in retail (no bodies of their own); the header still declares
  them out of line (17 fixtures define recording stubs), so `seam_traps.py` binds them to the
  CBaseClassNonCopyable bodies (`INLINE_ALIASES`).
- `CArrayVector<T>::GetAsCArray()` (non-const) tests `size() > 0` with the element pointer as
  default; the const form keeps `size() == 0`.
- link_closure.py / play_seam.py pass objects through response files (the command line
  passed 32K).

Header changes: `fable_definition_manager.h` (Compile's callees, CreatingCompiledDefs,
ClearInstantiatedDefs, inline GetPDefClassInfo(crc), CInstantiatedDefInfo(),
CompressedDefData as CArrayVector), `fable_data_input_stream.h` (vtables, inline dtors,
CopyTo, test friend), `fable_data_output_stream.h` (CopyFrom, ReserveSize),
`fable_def_string.h` (TransferBinaryIn, GetStringCRC, LoadTable, ReadTable),
`fable_filesystem.h` (scan overloads, NFile::CalcCurrentPath, ConvertPathToFullPath(path,
full), GetAbsoluteFilePath, ValidatePathnameIsOnWriteableDrive, CreateDirectoryForFile),
`fable_string.h` (operator== form, GetAsLowerCase, GetAsCharArray, operator+=(wchar_t),
CCharString operator+(s,s), GFIntToCharString, GFGetLastWindowsError),
`fable_carray.h`, `fable_vector_map.h` (CVectorMap : CArrayVector), `fable_stlport_vector.h`
(const begin/end), `fable_stlport_vector_impl.h` (vector(n)), `fable_stlport_algobase.h`
(unsigned char fill_n), new `fable_checksum_cache_file.h`, new
`test_support/data_input_stream_stubs.h`.

Verification: `gate_all.py --bootstrap`: 123/123 boot leaves, component ABI, 0 regressions across 357
header-dependent entries, bootstrap refs, ring 0 unresolved (runtime probe also passes), full
bootstrap. `integration/phase6_runtime_boundary.cpp` defines the new stream slots as boundary
stand-ins. The seam stops at `CDefinitionManager::GetDef<CFrontEndDef>` 0x4300FB.

## Lead: six near-miss solves landed (2026-09-27)

Two isolated byte-match agents solved six seam scaffolds; the lead landed them (the permission
system had refused the driver copying them out of the scratch dir; the user approved landing).
All RELOCATION_MATCH with fixtures and killed mutants, verified against the repo headers:

- `_Rb_tree<CCharString,long>::_M_find` 0x42B467: a `/Os` unit (`#pragma optimize("s", on)`);
  the frame pointer comes from /Os alone. `CCharString::operator<` is `__forceinline`, which keeps
  `less<CCharString>::operator()` out of line (and that body matches 0x42AAB3, still an `_emit`
  bake: point its catalog entry at this source by mangled name as a follow-up).
- `CMemoryDataOutputStream::Write` 0x996CC0 (+ `PeekData` 0x995E70, MATCH, no catalog entry of
  its own yet): `Data` is a Lionhead `CArray<char>` (modelled as `CArrayVector` in
  fable_carray.h); PeekData is `GetAsCArray()`, whose inline `size() == 0` gives retail's `sub`.
- `vector<CWideString>::operator=` 0x433DF0: retail length is 369 B, not 413 (the old oracle row
  ran into padding and two thunks).
- `_Rb_tree<CCharString,long>::insert_unique` 0x431606, `_M_insert` 0x4316CF, and
  `_M_create_node` 0x431773 (de-baked in place from an `_emit` bake): one `/Os` unit that does not
  see Compare's body.

**Modelling knob, flagged:** `FABLE_UNIT_INLINE` (rebuild_abi.h, default `inline`) is defined as
`__forceinline` by these units. Retail expands small helpers (less/op<, the pair copy, `_Construct`,
`_Destroy` range, `_M_allocate_and_copy`, `_M_clear`, `__copy_ptrs`) that VC7.1 RTM/QFE keep out
of line under every flag and pragma tried. `__forceinline` is the only thing that reproduces it;
there is no evidence of retail's actual mechanism. A 352-source regression compile showed no byte changes.

Still near misses: `_M_lower_bound` 0x47547D (73 v 73: key storage hoist needs Compare visible but
not inlined), `vector<ulong>::operator=` 0x45BC09 (177 v 184), LoadBinaryDefinitions 0x9AFB90,
CDefString::TransferBinaryIn 0x44FC00. The seam stops at `GetDef<CFrontEndDef>` 0x4300FB.

## Seventh driver pass resumed (2026-09-26)

Recovered the interrupted round's four new bodies: GetDef<CFrontEndDef> 0x4300FB,
GetPDefFromClassIndex 0x9ADE10, const GetPDefClassInfo 0x9AD2E0, and const
CVectorMap<CDefClassInfo>::LowerBound 0x9B2290. Independently verified each with
retail parity and its behavior fixture. Also verified the inherited catalog cleanup:
less<CCharString> 0x42AAB3 now uses the genuine M_find unit, and PeekData 0x995E70
and CResource's constructor 0x9FBEC0 have their own catalog entries and fixtures.

The resumed seam initially crashed at GetPDefFromClassIndex +0x15: GlobalIndices
was empty. Diagnostic copies of the load and lookup bodies showed 810 definitions
and nine registered classes, but no matching class CRCs. seam_traps had bound
`g_FableCRCTable_0129A168` to zero data. Therefore runtime class registration
computed zero keys while names.bin supplied actual CRCs. The seam main now owns
and initializes the reflected 0xEDB88320 table before loading names.bin. All 256
computed entries were compared directly with the installed retail table at
0x0129A168 and matched. No retail bytes are embedded in the seam source.

With real CRCs the loader reaches its previously skipped NameCRCToIndexMap sort.
Two further bodies now match retail with behavior fixtures:

- 0x9B8520, STLport sort<pair<unsigned long,long>>: 115 bytes, including both
  returns (the first ret is not the function boundary). Fixture checks empty,
  singleton, threshold and larger ranges, depth budget, and helper call order.
- 0x9B7ED0, its __introsort_loop: 196 bytes. Fixture checks threshold, heap-sort
  fallback, median selection with paired value, recursion and depth consumption.

Both use existing shared template bodies. Changing sort's depth multiplier from
2 to 3 fails its fixture; changing introsort's depth-zero fallback to depth-one
fails its fixture. Both originals are RELOCATION_MATCH / PASS.

The clean seam now stops at __unguarded_partition<pair<unsigned long,long>>,
0x9B3890 (call from 0x9B7F4A). Pending sort helpers also include insertion_sort
0x9B69A0, unguarded_insertion_sort_aux 0x9B2D40 and partial_sort 0x9B6B70.
The four pre-existing near-miss scaffolds remain; this is not full playable boot.

Reproduction: `python tools/decomp_pipeline/play_seam.py --reindex 004300fb
009ade10 009ad2e0 009b2290 009b8520 009b7ed0` (one command line). All nine catalog
entries passed individual parity/behavior checks. The full pre-checkpoint gate
run passed 123/123 boot leaves, component ABI, 370 header dependents with zero
regressions, bootstrap references, bank ring link, and full bootstrap. The final
`gate_all.py --bootstrap` run also passed: 123/123 boot leaves, component ABI,
zero regressions across 372 header dependents, zero missing bootstrap references,
zero unresolved bank-ring symbols, and VISUAL_BOOT_CHECKPOINT PASS.

## Eighth driver pass: definition-name sorting (2026-09-26)

Five more compiler-emitted C++ functions landed, each checked against retail and
with a behavior fixture that links the candidate object. No existing shared-header
body changed; two opt-in template headers add partition and the final insertion pass.

| Address | Function on pair<unsigned long,long> | Retail bytes | Parity |
| --- | --- | ---: | --- |
| 009B3890 | __unguarded_partition | 85 | MATCH |
| 009B69A0 | __insertion_sort | 62 | RELOCATION_MATCH |
| 009B2D40 | __unguarded_insertion_sort_aux | 73 | MATCH |
| 009B5920 | __linear_insert | 99 | RELOCATION_MATCH |
| 009B1AA0 | __unguarded_linear_insert | 51 | MATCH |

Fixtures cover unsigned CRC ordering (including the high bit), duplicate keys,
key/index pairing, sentinels and range boundaries. Partition runs 326 cases;
unguarded insertion covers 132 length/order combinations. Each fixture rejects a
mutation: skipped insertion element or a lost value during entry copying. The
linear-insert fixtures include declarations only, to force linkage to the candidate
object rather than instantiate a replacement inside the test translation unit.

The clean seam passes the name-map sort and now traps at
CDefinitionManager::DecompressDefinitionData, 0x009AD5F0 (231 bytes). Reproduce:
`python tools/decomp_pipeline/play_seam.py --reindex 009b3890 009b69a0 009b2d40 009b5920 009b1aa0`.
The depth-exhaustion fallback __partial_sort 0x009B6B70 is still unresolved; the
current compiled-def input does not take it. The four existing near-miss scaffolds
remain, so this is not full playable boot.

Next-function evidence from retail disassembly and existing shared declarations:
- UpperBound on CompressedOffsets (0x009B1FF0) selects the compressed block;
  adjacent entries supply the first definition index and compressed byte offset.
- Globals 0x0138E190 / 194 cache the manager pointer and compressed block offset;
  0x0138E198 is decompressed size, with a 0x8000-byte buffer at 0x0138E19C.
- A cache miss constructs a compressor, calls Init 0x00A3AA10 and
  InternalDecompress 0x00A3AB00, then destroys it at 0x00A3ACC0.
- The decompressed prefix is a ushort offset table. Interior definition lengths
  use the next offset; the block's last definition ends at decompressed size.
  OpenMemorySource 0x00994570 receives that subrange.
- Do not trust the current compressor catalog labels: 0x00A3A7D0 is a zero-return
  leaf called without arguments here, and 0x00A3AEA0 is the compressor base ctor
  call. The generated CCompressorZlib header has the donor's 0x5C layout; retail
  accesses its flags at 0x54..0x56 (the retail base vector is four bytes smaller).
  Model this against constructor/vtable evidence before changing shared types.

Gate run: 123/123 boot leaves, component ABI, 377 header dependents with zero
regressions, zero missing bootstrap refs, zero unresolved bank-ring symbols, and
full VISUAL_BOOT_CHECKPOINT PASS. The final rerun after fixture and source-mapping
updates also passed all gates (work/codex-round8/final-gates.log).

## Ninth driver pass: decompress the front-end definition (2026-09-26)

The seam now decompresses its actual compiled definition block, resolves the class
name, makes resource space and reaches CParentDefClassBase::InternalLoadBinary
0x009AED70. Twenty new catalog entries plus three corrected existing entries are
verified: 23 addresses / 9,208 retail bytes. Each behavior fixture requires its
candidate object, and each rejects a deliberate mutation.

Engine functions: CDefinitionManager::DecompressDefinitionData 0x9AD5F0;
CVectorMap<ulong,ulong>::UpperBound 0x9B1FF0; CDefStringTable::GetString 0x9D49B0;
CResourceList::MakeFreeSpace 0x9FC4F0, ShrinkResources 0x9FC210, FrameEnd 0x9FC150;
CACompressor ctor 0xA3AEA0; CCompressorZlib::InternalDecompress 0xA3AB00,
dtor 0xA3ACC0, Alloc 0xA3A9C0, Free 0xA3AA00. Existing entries corrected:
OpenMemorySource 0x994570 replaces naked assembly with typed C++;
GetRequiredWorkspaceSize 0xA3A7D0 replaces the mislabeled AddChildPrimitive;
Initialise 0xA3AA10 replaces the anonymous local-struct implementation.

The compressor headers now model retail: CACompressor 0x1C bytes with a 12-byte
STLport OwnWorkspace; CCompressorZlib 0x58, Stream at 0x1C and flags at 0x54.
PDB access modifiers and static calling conventions are preserved. CACompressor's
destructor is implicit: an explicit empty body added a non-retail vptr store.
CCompressorZlib's destructor expands FinishCompressStream/FinishDecompressStream;
the latter's SetupStream call preserves retail's stream-base register choice.
The definition cache is keyed by manager and block offset; its ushort offset table
selects interior definitions, while the final definition ends at decompressed size.

Nine zlib functions use the original 1.2.1 source from the official archive:
https://zlib.net/fossils/zlib-1.2.1.tar.gz. The version was identified from retail's
initialization call and string, then verified by matching compiled instructions.
License and provenance are preserved under rebuild/include/third_party/zlib121.
C++ adaptations are ANSI parameter lists, renaming the C identifier `this`, and
explicit internal_state pointer casts. Algorithms and constant tables are unchanged.

| Address | zlib function | Bytes | Parity |
| --- | --- | ---: | --- |
| 00C02310 | inflateInit_ | 26 | RELOCATION_MATCH |
| 00C02240 | inflateInit2_ | 199 | RELOCATION_MATCH |
| 00C021F0 | inflateReset | 73 | MATCH |
| 00C038D0 | inflateEnd | 72 | MATCH |
| 00C02470 | inflate | 5204 | RELOCATION_MATCH |
| 00C06060 | inflate_fast | 1131 | RELOCATION_MATCH |
| 00C064D0 | inflate_table | 1279 | RELOCATION_MATCH, /GS |
| 00C03CB0 | adler32 | 297 | MATCH |
| 00C05FD0 | crc32 | 24 | RELOCATION_MATCH |

Use verify_land_jumptable.py's raw COFF extraction for inflate: disassembly alone
reports only 5,072 bytes, omitting its 132-byte jump table. The raw 5,204-byte body
matches, including the table after relocation masking. Inflate_table needs /GS;
the flag is recorded in its catalog entry. The CRC wrapper emits its original
static crc32_little helper and constant tables in the same unit.

Each zlib fixture omits its candidate implementation and links that compiled object
into the rest of the codec. Synthetic inputs exercise stored/fixed/dynamic blocks,
gzip, full buffers and 7-byte-input/19-byte-output streaming, reset/reuse, bad
versions and sizes, bad window sizes, allocation failure, checksum rejection,
allocation/free balance and independent checksum vectors. No retail fixture data.

Reproduce the current stop with play_seam.py --reindex over this pass's addresses
(the list is in work/codex-round9/landed-items.json). In particular refresh the
compressor entries after access-modifier corrections. The last observed trap is
?InternalLoadBinary@CParentDefClassBase@@UAEXAAVCDataInputStream@@@Z at 0x9AED70.
That loader calls CDefClassBase::InternalLoadBinary 0x9ACB20, reads a ushort count,
reserves the subdef map at +0x29, then reads CRC/index/original-parent triples.

Still open: four pre-existing near-miss scaffolds; the index sort's depth-exhaustion
heap fallback; resource eviction's UnloadNextResource. The current compiled input
does not exercise those latter two paths. This is not full playable boot.

The intermediate full gate passes: 123/123 boot leaves, component ABI, 396 header
dependents with zero regressions, bootstrap refs, ring link and full bootstrap.
The final gate also passes: 123 boot leaves, 400 header dependents with zero
regressions, zero missing refs/unresolved ring symbols and full bootstrap
(work/codex-round9/final-gates.log).

## Round 10: deserialize the definition header and its movie-name vector

Four new addresses and two corrected catalog entries match retail (1,137 bytes).
All six tests need the candidate object: a test-only link fails, linking the
candidate passes, and a deliberate mutation fails.

| Address | Function | Bytes |
| --- | --- | ---: |
| 009ACB20 | CDefClassBase::InternalLoadBinary | 292 |
| 009AED70 | CParentDefClassBase::InternalLoadBinary | 492 |
| 00433273 | GFSerialiseVectorBinaryIn<CCharString> | 185 |
| 0043332C | vector<CCharString>::resize | 62 |
| 0043336A | vector<CCharString>::erase | 73 |
| 00414E00 | string range destruction | 33 |

The base loader reads three byte flags into their existing packed bitfields.
The parent loader reads a ushort subdef count and CRC/index/original-parent
triples, reserves the map and clears Dirty. Tests cover direct/chunk-overflow
reads, zero/nonzero counts, signed indices, and neighboring bit preservation.
The shared vector helpers emitted by the parent loader are not additional
parity claims.

The string-vector reader constructs its persistence traits, reads a signed count,
resizes with an empty string, and assigns each null-terminated string. Fixtures
cover existing content, empty entries, populated/empty lists, consumption, and
balanced string lifetimes. Vector tests cover shrink/equal/grow dispatch,
middle/full/empty erasure, and empty/partial/full range destruction. The existing
resize address had an unrelated CArray label; range destruction was an asm bake.
Their legacy filenames remain, with catalog and source mappings corrected.
The new fable_stlport_char_string_vector.h declares the non-POD specializations;
the general vector header's POD erase remains unchanged.

Codegen: ReadValue<unsigned char> forces the base-loader read paths inline.
The string reader needs a local vector begin pointer and size optimization.
Erase uses the standard counted copy loop expanded through a force-inline helper.
VC7.1 needs an explicit erase instantiation after its specialization to emit it.

Observed Play seam: 363 genuine sources, 401 unresolved symbols, six duplicate
symbols; the named trap is vector<CCharString>::_M_fill_insert at 0x004333B3.
Reproduce with python tools/decomp_pipeline/play_seam.py --out work/codex-play-seam
--reindex 009acb20 009aed70 00433273 0043332c 0043336a 00414e00.
The four earlier near-miss scaffolds remain. This is not full playable boot.

Resume scratch: work/codex-round10/string_fill has a non-landed insertion
candidate. Retail expands uninitialized copy, backward copy, and fill, but calls
its three-argument uninitialized fill at 0x00433504 and overflow at 0x00433530.
The candidate still differs, including tag initialization and register allocation;
no fill implementation or new scaffold is committed.

Final validation: 123/123 boot leaves, component ABI, 406 header dependents with
zero regressions, zero missing bootstrap references, zero unresolved ring symbols,
and full bootstrap. ALL GATES PASS (work/codex-round10/final-gates.log).

## Round 11: finish definition loading and reach console construction

Nine new addresses and seven corrected entries match 1,686 retail bytes: five
MATCH and eleven RELOCATION_MATCH. All 16 behavioral fixtures require the actual
candidate object, pass with it, and reject a deliberate source mutation.

| Address | Function | Bytes |
| --- | --- | ---: |
| 004333B3 | STLStringVector::_M_fill_insert | 337 |
| 00433504 | STLStringVector::uninitialized_fill_n | 38 |
| 00433530 | STLStringVector::_M_insert_overflow | 316 |
| 00995D30 | CMemoryDataInputStream::ReadNullTerminatedString | 138 |
| 0040FE60 | CPersistTraitsLong::TransferBinaryIn | 129 |
| 009FBF20 | CResource::SetResourceSize | 46 |
| 009FBFE0 | CResourceList::AddResource | 49 |
| 009FBF90 | CResource::ListInsert | 26 |
| 009FBFB0 | CResource::ListRemove | 47 |
| 00993B10 | CMemoryDataInputStream::~CMemoryDataInputStream | 11 |
| 009BCF70 | StaticLoadXMVCode | 78 |
| 009F0320 | CCodeSectionManager::CCodeSectionManager | 131 |
| 009F02C0 | CCodeSectionManager::LoadSection | 43 |
| 00413590 | CCountedPointerCodeSectionManager::Reset | 103 |
| 009F0240 | CCodeSectionManager::GetSectionDesc | 123 |
| 009F0D90 | CCodeSectionTree::find | 71 |

String-vector insertion now handles spare capacity and reallocation, including
aliased fill values and balanced string lifetimes. The old unsigned-long fill
label was an asm bake; its actual CCharString placement-copy loop replaces it.
The overflow function is 316 bytes: the 313-byte Ghidra range omitted the final
three-byte return instruction. Local inline STL helpers reproduce retail's
copy/backward-copy/fill expansion and value-initialized empty tag.

Memory-stream string reads and destruction now use genuine virtual member
symbols. Their old local class/free-function labels could pass isolated parity
without resolving the real seam calls. Destructor selection uses the explicit
??1CMemoryDataInputStream@@ leaf to avoid selecting its deleting destructor.
The bank harness's duplicate input-stream destructor stub and stale aliases were
removed; legacy raw thunks now refer to the real virtual symbols.

The signed-long persistence reader uses the retail scalar read's two early
rejection exits. Valid direct and overflow reads are tested across signed edge
values. A rejected read leaves the scalar unspecified, as in retail.
Resource size accounting and list insertion/removal now use shared real classes;
fixtures exercise detached/attached resources, sentinel links and removal guards.

The XMV reference-counted load now constructs the real code-section singleton,
its three maps and descriptor lookup. The manager's 0x2c layout follows retail;
RTTI at 0x0129C764 confirms the empty CTBaseSingleton base. Tests cover allocation,
shared-pointer release/reset, invalid handles, repeated loads, fallback descriptor
reset and reference identity. The unsigned tree lookup reuses the standard
STLport _M_find body with its named header temporary. Fixtures include empty
and populated trees, missing keys and values above 0x7fffffff.

Observed Play seam: 379 genuine sources, 396 unresolved symbols, six duplicate
symbols. It completes definition loading and proceeds through resource and video
code-section initialization to CConsole::CConsole (??0CConsole@@QAE@XZ).
Reproduce: python tools/decomp_pipeline/play_seam.py --out work/codex-play-seam
--reindex 009f0320 009f0d90. The four earlier near-miss scaffolds remain; this is
not full playable boot. Heap fallback and resource eviction remain open.

Next constructor is 0x009ECD80, 418 bytes through the return at 0x009ECF21.
Its propagated CHeroInformationScreens label is wrong: the written vtable at
0x0129C600 has CConsole RTTI and CTBaseSingleton<CConsole> as its only base.
The 0x9c retail object has maps at 0x04/0x10/0x40, command lines at 0x1c/0x2c,
a string at 0x3c, history list at 0x4c, and color/default fields through 0x98.
The existing fable_console.h still has an opaque body; reconcile it with those
retail offsets and the named Ego_r fields before implementing the constructor.
Disassembly is saved in work/codex-round11/next-console-disasm.txt.

Scratch and verification ledger: work/codex-round11. Each key has source, test,
metadata, parity result and a rejected mutation. Legacy filenames are retained
for corrected catalog entries; source, test, manifest and oracle mappings agree.

Final validation: 123/123 boot leaves, component ABI, 422 header dependents with
zero regressions, zero missing bootstrap references, zero unresolved ring symbols,
and full bootstrap. ALL GATES PASS (work/codex-round11/final-gates.log).

## Round 12: reconstruct console initialization and reach the frontend manager

16 new addresses match 4,996 retail bytes (one MATCH, fifteen RELOCATION_MATCH).
Every behavioral fixture requires its candidate object and rejects a deliberate
mutation. These are compiled C++ bodies, with no new seam scaffolds.

| Address | Function | Bytes |
| --- | --- | ---: |
| 009ECD80 | CConsole::CConsole | 418 |
| 00413520 | CCountedPointerConsole::Reset | 103 |
| 009ED190 | CConsole::Initialise | 2266 |
| 009EBB80 | CConsoleCommandParameters::CConsoleCommandParameters | 125 |
| 009EBC00 | CConsoleCommandParameters::CConsoleCommandParameters | 171 |
| 00419F50 | STLConsoleParameters::vector | 112 |
| 009EF900 | STLConsoleParameters::_M_insert_overflow | 227 |
| 009EC5E0 | CConsole::AddInput | 296 |
| 0099ED10 | CCharString::GetAsLowerCase | 271 |
| 009EFB80 | CConsoleInputTree::insert_unique | 254 |
| 009EF750 | CConsoleInputTree::_M_insert | 259 |
| 009EE7B0 | CConsoleHistoryList::clear | 58 |
| 009EB260 | CConsole::SendMessageToUser | 94 |
| 009EAF80 | CConsole::AddCommandLineToHistory | 107 |
| 009EEB00 | CConsoleHistoryList::insert | 92 |
| 009EAC80 | CConsole::ScrollToHistoryBeginning | 143 |

The console now constructs its maps, command buffers and history list, registers
14 bound commands and ConsoleAlpha, and inserts the opening message. Tests cover
all registration allocation-failure positions, lowercase lookup keys, duplicate
ownership, enum-vector growth and copy, history boundaries at 255/256/257 entries,
virtual destruction, and visible-window limits including zero and negative values.

Retail RTTI identifies 009ECD80 as CConsole, correcting the old hero-screen label.
Its PC layout is 0x9C bytes. Parameter storage is a real STL vector, and 00419F50
is the vector copy constructor rather than a wrapper copy or range constructor.
The command-line destructor is implicit; spelling it out changes emitted code.
Full vector helper visibility reproduces the compiler's temporary-stack reuse.
ScrollToHistoryBeginning is 143 bytes; Ghidra's 140-byte range omitted its return.
Four existing fixtures now provide dependencies introduced by the typed members.

The seam passed console initialization and reached CNewFrontendGameComponent::Run.
It exposed a harness bug: the data-symbol classifier omitted underscore-prefixed
MSVC type codes, so Boolean globals became executable function traps. Writes to
PlayingFullScreenMovie then faulted. The classifier now recognizes these data
symbols; checks distinguish Boolean/integer data from constructors and Boolean
returning functions. Missing data still has the harness's zero defaults, not a
claim that all retail initialization has been reconstructed.

Final seam: 391 genuine sources, 417 unresolved symbols, six duplicate providers,
and the same four existing scaffolds. It exits 86 at
`??0CFrontEndManager@NUISystem@@QAE@XZ` (NUISystem::CFrontEndManager constructor).
This remains a trap-linked reachability checkpoint, not playable boot.

Final validation: 123/123 boot leaves, component ABI, 438 header dependents with
zero regressions, zero missing bootstrap references, zero unresolved ring symbols,
and full bootstrap. ALL GATES PASS (work/codex-round12/gates3.log).

Resume from the frontend-manager constructor. The console input-tree erase body
at 009EF6E0 (108 bytes) is also identified but not reconstructed; heap fallback,
resource eviction, and the existing four scaffold parity gaps remain open.

## Round 13: construct the frontend manager and expose the font startup prerequisite

Ten addresses cover 762 retail bytes, all RELOCATION_MATCH: five new entries and
five corrected existing entries, including three assembly bakes replaced with
compiler-emitted C++. Each fixture requires the candidate object and rejects
a deliberate mutation. No new seam scaffold was added.

| Address | Function | Bytes |
| --- | --- | ---: |
| 005953E2 | NUISystem::CFrontEndManager::CFrontEndManager | 203 |
| 0052D9E0 | NUISystem::CObserver::CObserver | 59 |
| 0042AB01 | STLActionParamList::list | 27 |
| 0059B310 | STLScreenDeque::deque | 21 |
| 0059AEF9 | STLScreenDequeBase::_Deque_base | 49 |
| 0059AC9F | STLScreenDequeBase::_M_initialize_map | 150 |
| 0059AA43 | STLScreenDequeBase::_M_create_nodes | 33 |
| 009E2C80 | CFontManager::GetFont | 91 |
| 009E51C0 | CSymbolMap::SymbolExists | 61 |
| 009E5120 | CSymbolMap::GetSymbolSLONG | 68 |

Retail bytes, RTTI and donor member names establish the frontend manager's 0xE0
PC layout. Its CObserver base owns a set<EEvent>; action parameters are a list,
and screen history is a stack over a deque. Tests check ordered member
construction, sentinel links, initialized flags, untouched counters/pointers and
surrounding memory. The previous CFrontEndManager::Initialize label at 0052D9E0
actually denotes CObserver construction. The old CGameEvent::AddUBYTE at 0059B310
is deque construction. The action-list constructor is 27 bytes including ret 4.

The deque uses 128-byte blocks (32 screen pointers), a centered map with at least
eight slots, and one extra block when the logical size is an exact block multiple.
Tests cover zero, boundary and growing sizes, independent allocations, and
preservation of caller-owned map slots. The CDateAndTime declaration moved to
its own shared header so both frontend members and game code see its real type.
Two pre-existing fixtures now provide the typed member dependencies.

CFontManager::GetFont prefers static fonts, then streaming fonts, and falls back
to static lookup when neither symbol table contains the name. Four combinations
check routing, short-circuit behavior, IDs and returned pointers. SymbolExists
and GetSymbolSLONG use the const sorted-vector search with FastCompare and a
cached end iterator; empty/missing, zero-valued and negative symbols are tested.

The Play-only seam now links 401 genuine sources (420 unresolved symbols, six
duplicates, four existing scaffolds) and passes frontend-manager construction.
Its current result is an access violation, NOT a missing-callee trap: GetFont+8
reads through a null CFontManager receiver during InitialiseGraphics. Reproduced
with work/codex-play-seam/diagnose.py: ECX=ESI=0 at linked address 0040EED8.
This exposes the harness's missing startup prerequisite. CSystemManager::Initialise
009A6610 creates PFontManager when font handles are present; GFInitialise 004022B0
publishes that pointer to g_FableFontManager_013B838C. main_source in play_seam.py
currently applies definition switches and directly enters Play, skipping both.
The verified font code does not add a null guard absent from retail, and the
harness does not invent a font manager or bypass graphics initialization.

Final validation: 123/123 boot leaves, component ABI, 448 header dependents with
zero regressions, zero missing bootstrap references, zero unresolved ring symbols,
and full bootstrap. ALL GATES PASS (work/codex-round13/final-gates.log).

Resume by connecting the real system-manager/GFInitialise startup path to the
Play seam, accounting for the display and font-bank dependencies. CFontManager's
constructor is 009E2E30; its static/streaming getters are 009E2BA0/009E2C10.
The next font-symbol dependency is const LowerBound at 009E6910, followed by
FastCompare at 009E6530. Font availability has not been demonstrated in the seam.
Heap fallback, resource eviction, and the four existing scaffold parity gaps
remain open. This is still a reachability checkpoint, not playable boot.

## Round 14: probe real system initialization and recover manager construction

The visible frontend estimate is approximately 80% (rough range 70-85%) for the
current title/menu/profile/options/save-selection slice. This is an engineering
judgment, not a measured completion ratio, and does not mean 80% of native startup
or of the full game is reconstructed. This round adds no visible screen.

Twelve functions cover 701 retail bytes: four MATCH and eight RELOCATION_MATCH.
Every behavior fixture requires the candidate object (test-only linking fails),
and deliberate mutations fail. No new seam scaffold was added.
The twelve sources pass crawl/purity.py, including recursive checks of shared
headers for unsanctioned assembly. Functional bootstrap adapters and the four
existing Play scaffolds are excluded from reconstruction claims. The visible
80% estimate is not a byte-pure frontend completion figure; that requires a
defined frontend dependency inventory and verified coverage of that inventory.

| Address | Function | Bytes |
| --- | --- | ---: |
| 009A52F0 | CSystemManager::InitGUIDFromName | 61 |
| 004016F0 | CDisplayManagerInit::CDisplayManagerInit | 78 |
| 009F4910 | CScratchBuffer::Initialise | 61 |
| 009F4950 | CScratchBuffer::Uninitialise | 43 |
| 009F4980 | CScratchBuffer::Lock | 29 |
| 009F49A0 | CScratchBuffer::Unlock | 5 |
| 009A73D0 | CCountedPointerDriveManager::Reset | 103 |
| 009E4AD0 | CDriveManager::Initialise | 15 |
| 009F8690 | CGraphicBankManager::CGraphicBankManager | 43 |
| 00A0A110 | CMeshBankManager::CMeshBankManager | 57 |
| 009A71F0 | CCountedPointerGraphicBankManager::Reset | 103 |
| 009A7260 | CCountedPointerMeshBankManager::Reset | 103 |

Scratch-buffer ownership now uses its real CInitBaseClass base and typed fields.
Tests cover base/allocation failure, valid/invalid cleanup, locking twice, null
buffers, and preserved fields. Including <new> is necessary for the VC7.1 array
allocation/deallocation imports; relocation-insensitive parity alone did not
catch the wrong scalar allocation target, while the behavior fixtures did.
The GUID routine copies at most 16 name bytes and zero-pads shorter names.
Display defaults preserve the intentionally untouched field and surrounding bytes.

Graphics-bank and mesh-bank managers own circular STLport lists. Their tests
check sentinel links, allocation sizes, base construction, mesh secondary-base
placement, guards, and counted-pointer shared/final release paths. Existing
system boundary fixtures now explicitly provide empty-bank destructor stand-ins
and aligned scratch receiver storage; these are test/runtime-boundary fixtures,
not retail implementations.

play_seam.py --entry system is a separate diagnostic that calls the reconstructed
CSystemManager::Initialise with retail default flags (zero), empty font handles,
and harness C++-constructed globals. It links real Windows imports using the
installed SDK and does not use the four Play scaffolds. It passes GUID/display
initialization, scratch allocation, drive setup, and both bank-manager constructors.
The current trap is CProfileManager::CProfileManager at 009F9160 (242 retail bytes).
This is the performance profiler, not the player-profile menu.

The default Play probe remains separate and still faults in GetFont with its
missing font-manager startup prerequisite. The system diagnostic has no display,
input, audio, or fonts requested, and does not run GFMain/GFInitialise. Neither
probe is a complete native frontend boot. The visible-bootstrap gate continues
to exercise the existing authored runtime boundary.

Final validation: 123/123 boot leaves, component ABI, 460 header dependents with
zero regressions, zero missing bootstrap references, zero unresolved ring symbols,
and full bootstrap. ALL GATES PASS (work/codex-round14/final-gates.log).

Resume at 009F9160. Retail profile-manager layout is 0x78 bytes: profile list at
0x4c, ID/name maps at 0x50/0x5c, profile-tree list at 0x68, current entry at 0x6c,
and NeedsSaving at 0x70. The constructor allocates 12/24/24/104-byte sentinels,
conditionally publishes the singleton and reads the display render manager,
then inserts a default tree entry. PDB offsets diverge after the first container;
retail disassembly is authoritative. Scratch ctor/dtor experiments were excluded
from this checkpoint. After profiler construction, continue real system startup
and then connect GFInitialise/font-bank inputs before re-entering Play.

## Round 15: graduate three scaffolds and construct the performance profiler

The functional-first policy closes ten functions spanning 2,551 retail bytes:
six RELOCATION_MATCH and four NEAR_MATCH. All are genuine C++, require their
candidate object for a passing behavior fixture, and reject a deliberate mutation.
No new seam scaffold or hand-written instruction was added.

| Address | Function | Bytes | Grade |
| --- | --- | ---: | --- |
| 0044FC00 | CDefString::TransferBinaryIn | 128 | NEAR_MATCH |
| 009AFB90 | CDefinitionManager::LoadBinaryDefinitions | 1344 | NEAR_MATCH |
| 009F9160 | CProfileManager::CProfileManager | 242 | RELOCATION_MATCH |
| 009F9C40 | STLProfileTreeList::insert | 98 | RELOCATION_MATCH |
| 009F9450 | CProfile::CProfile | 91 | RELOCATION_MATCH |
| 009F9930 | STLProfileChildrenVector::vector | 95 | RELOCATION_MATCH |
| 009F9BA0 | STLProfileTreeList::insert | 87 | RELOCATION_MATCH |
| 009A7510 | CCountedPointerProfileManager::Reset | 103 | RELOCATION_MATCH |
| 00A0DB60 | _global::GFInitMaths | 290 | NEAR_MATCH |
| 0047547D | STLStringLongTree::_M_lower_bound | 73 | NEAR_MATCH |

The former definition-string read (53/53 instructions, 100% normalized similarity),
binary-definition loader (486/503, 92.42%), and string-map lower-bound (34/34,
94.12%) are now cataloged reconstruction. Their scaffolds were removed only after
candidate-linked verification. The last Play scaffold is vector<unsigned long>
assignment at 0045BC09: its 81/70 instructions and 70.20% similarity under /O2 /Os
remain below the initial thresholds. No threshold was relaxed to admit it.

Binary-loading fixtures cover EOF, dependency CRC and string-table ID rejection,
zero definitions, the optional dependency check, known/unknown classes, global
indices, names and class indices, sorted name-to-index entries, compressed offsets,
raw compressed data, safe-format state, and progress/callee order. The four-byte
string transfer covers unaligned data, negative/index bit patterns, fast/overflow
reads, stream accounting and the invalid-position guard (without asserting an
uninitialized return local). Inverting the dependency check still scores NEAR_MATCH
but fails its fixture: similarity is explicitly not treated as behavior proof.

String-map lower-bound tests empty/equal/shared-string/interior/end/null cases and
signed high-bit characters. A per-unit noinline attribute keeps CBasicString::Compare
out of line, matching the observed retail call boundary; the comparison algorithm
and shared default inline policy are unchanged. Once it qualified, tuning stopped.

CProfileManager is the performance profiler, not the player-profile menu. PDB names
and retail offsets establish its 0x78 layout, 0x48 CProfile and 0x60 profile-tree entry.
Construction initializes four container sentinels, conditionally publishes the
singleton, reads the display renderer only when present, inserts a default root,
and sets the measured flags/counters. Fixtures test both singleton states and both
display states, the GFGetSystemManager call count, root insertion, sentinel links,
untouched fields/padding and guards. Root insertion copies the embedded profile and
child-pointer vector and links the new list node. Fixtures cover insertion at several
positions, independent vector ownership, empty vectors, copy fields and preservation
of padding. Counted reset covers null/shared/final release and allocation failure.
Existing system boundary fixtures explicitly supply empty-profiler destruction;
those fixture bodies are not counted as reconstruction.

GFInitMaths constructs reciprocal-root, cosine, acos and asin lookup tables at runtime
from the retail algorithm. The source owns typed BSS storage, not baked table assets.
Its 73/69 instructions score 90.14%. Fixtures check every table entry, reciprocal-root
quantization checksum, monotonic inverse-trig output, exact endpoints/wrap entries,
sentinels, and repeat initialization. Trig results are checked against an independent
/Od double-precision libm oracle with 2.4e-7 absolute tolerance for float/x87 rounding;
this does not claim an independent bit-for-bit retail execution of all trig entries.
The quantization fixture rejects an altered special entry.

System diagnostic: 38 genuine sources, 32 unresolved symbols, two duplicates, no
Play scaffolds. It now passes profiler construction, counted reset and math tables,
then traps at NVectorMath::GFInitVectorMath (00A5B850). Retail uses CPUID leaf 1,
tests EDX bit 25, and sets byte 013D2880 when supported. Both installed VC7.1 toolsets (RTM and QFE-4035) emit warning C4163
for #pragma intrinsic(__cpuid): the intrinsic is unavailable. The remaining ordinary
call in that probe does not implement CPUID. No asm bake, platform-API substitute,
or bypass was introduced. This compiler/primitive boundary remains explicit.

Play diagnostic: 405 genuine sources, 418 unresolved symbols, five duplicates and
one scaffold. It still faults at CFontManager::GetFont+8 with a null receiver because
the Play-only entry skips font startup; no new visible screen or playable boot is
claimed. The system entry still uses default flags with no font/display requested.

Near-match auditing now includes newly cataloged near matches absent from the old
object report, in addition to previous DIFFER rows. The dashboard counts five verified
near matches (the prior one plus these four), separately from exact-byte totals, and
only subtracts old DIFFER promotions from the historical difference count. The audit
rechecks current source and fixtures; it is not a fresh build of every historical row.
18 focused Python tests pass, including new-catalog inclusion, deduplication and
preservation of existing exact grades. README regeneration passes --check.

Final gates: 123/123 boot leaves, component ABI, 470 header dependents with zero
regressions, zero missing bootstrap references, zero unresolved ring symbols and
full bootstrap. ALL GATES PASS (work/codex-round15/gates.log).

Resume at the CPUID primitive boundary, or continue independent frontend dependencies
(font construction/static/streaming lookup). Resolve the unsigned-long vector-assignment
scaffold with the near-match standard, then connect actual GFInitialise/font-bank inputs
to Play. The four scaffolds from round 14 are now one; do not resurrect the three removed
bodies. Preserve the shared checkout/index and keep modernization fork work gated.

## Round 16 - candidate-linked near-match rendering fixtures

Two existing DIFFER implementations now qualify under the functional-first standard:
00989760 CShaderRenderManager::UpdateAmbient (85 bytes, 24/24 instructions,
91.67% normalized similarity), and 009DF060 CStateBlockFunctionSold::Apply
(1,736 bytes, 442/442 instructions, 100% normalized similarity).

The ambient declarations moved into fable_shader_ambient.h with offset assertions;
both fixtures now include declarations and link the compiled candidate instead of
including implementation source. Ambient cases cover unclamped components, negative
zero, a failing device call, repeated calls with a clear dirty bit, and preservation
of unrelated bytes. Sold cases cover all 17 queued states, unchanged requests,
already captured/queued entries, zero capture mask, and runtime changes to shared
state globals. Corrected the malformed Sold oracle name that had forced the verifier
to fall back to the largest symbol; the near-match gate still requires named selection.

Both candidates pass required-candidate fixtures. Mutating the ambient dirty mask
fails; mutating a Sold state value remains NEAR_MATCH but fails behavior. The audit
now verifies seven near matches across 47 reviewed rows. No instruction tuning or
threshold relaxation was needed for these promotions.

The unsigned-long vector assignment remains a scaffold: /Os gives 70/81 instructions
and 70.20% similarity; keeping FABLE_UNIT_INLINE helpers out of line gives 66/81 and
74.83%. Neither qualifies. Experiments remain in work/codex-round16, with no header
changes from those experiments.

Font follow-up: GetStaticFont 009E2BA0 and GetStreamingFont 009E2C10 acquire the bank
entry, set CFontBank::PManager at +0x0C, then return counted ownership. The 00A635E0
and 00A5F090 entries are currently mislabeled inventory getters; their bank layouts
need retail reconciliation before integration. The font-manager ctor also resizes
its vector to eight and creates eight vertex buffers. No bypass or fake font manager
was introduced. Execution stops remain GFInitVectorMath's unsupported VC7.1 CPUID
intrinsic and the Play probe's missing font startup; this round adds verified rendering
coverage, not a newly visible frontend screen.

Validation: all gates PASS, including 123 boot leaves, component ABI, 471 header
dependents with zero regressions, no missing bootstrap references, no unresolved
ring symbols, and the full visual bootstrap. Logs: work/codex-round16/gates.log.

## Round 17 - real font lookup, acquisition and ownership

Eleven candidate-linked functions cover 616 retail bytes: four MATCH, five
RELOCATION_MATCH, two NEAR_MATCH. Four functions are newly reconstructed; seven
replace misnamed, weakly tested or assembly-backed catalog implementations.

| Retail | Function | Grade |
|---|---|---|
| 00A635E0 | CFontDataBank::GetFontInfo | MATCH |
| 00A5F090 | CStreamingFontDataBank::GetFontInfo | MATCH |
| 009E2BA0 | CFontManager::GetStaticFont | RELOCATION_MATCH |
| 009E2C10 | CFontManager::GetStreamingFont | RELOCATION_MATCH |
| 00A634F0 | CFontDataBankEntry::AcquireFont | NEAR_MATCH |
| 00A5F1C0 | CStreamingFontDataBankEntry::AcquireFont | NEAR_MATCH |
| 00A63560 | CFontDataBankEntry::CanUnload | MATCH |
| 00A5F230 | CStreamingFontDataBankEntry::CanUnload | MATCH |
| 0041947A | CCountedPointer<CFontBank>::Reset | RELOCATION_MATCH |
| 004057A0 | CCountedPointer<CFontBank>::ReleaseData | RELOCATION_MATCH |
| 0042F948 | CCountedPointer<const CFontBank>::ReleaseData | RELOCATION_MATCH |

Both AcquireFont bodies have 35/35 instructions and 94.29% normalized similarity;
byte tuning stopped there. They snapshot Bank/Index before allocation, construct
only for an empty cache, Reset the cached ownership, then return another reference.
Fixtures cover cached/unowned pointers, allocation failure, argument snapshots across
an allocator callback, manager writes, index forwarding, call order, and reference
lifetimes. CanUnload checks RefCount == 1, not the old invented load-state enum.
Reset 0041947A is now genuine C++ instead of an asm bake. Const release has its own
44-byte /Os oracle: CleanUpInterfaces 0042DD28 calls 0042F948 at 0042DDA7; it was not
folded into the 53-byte mutable release body. Eleven deliberate mutations all fail
candidate-linked fixtures, including several mutants that remain numerically near.

Retail constructors and GetFontInfo prove the bank vector offsets 0x120/0x178 and
sizes 0x130/0x190. The larger Ego_r bank bases are not retail layouts. Curated models
live in fable_font_bank.h, fable_font_data_banks.h and fable_font_instances.h, with
engine-path forwarding headers. The generator's existing hand_owned() detection
protects all seven classes; engine INDEX records the corrected sizes/ownership.
The static/streaming font instance models retain flattened base storage and declare
only the constructor ABI needed here; their constructor bodies remain unreconstructed.

The header-dependency gate now accepts NEAR_MATCH only with PASS and requests a
candidate-linked fixture for that grade. Previously it labeled near-match/PASS as
still bad and could miss regressions from a near-match baseline. Two new gate tests
plus the existing policy tests pass (20 total). Exact historical gates retain their
existing behavior. The near-match audit verifies nine rows out of 49 reviewed.

The native Play closure resolves the added lookup/acquire/reset/release methods and
still requires the real CStaticFontBank (00AB8E10) and CStreamingFontBank (009A3C20)
constructors. Play's missing CFontManager startup and the independent system probe's
GFInitVectorMath/CPUID stop are unchanged; no fake manager, asm exception, embedded
font assets, or modernization fork was introduced. One vector-assignment scaffold
remains. Historical aggregate exact-byte reports have not been globally regenerated.
Scratch evidence, candidates, mutation results and probe logs: work/codex-round17.

Final validation: all gates PASS (123 boot leaves, ABI, 487 header dependents with
zero regressions, no missing bootstrap references, no unresolved ring symbols, full
visual bootstrap). The Play closure is 414 genuine sources, 416 unresolved symbols,
five duplicates and one scaffold. Crash diagnosis confirms GetFont+8 with ECX=0.
Logs: verified-gates.log, candidates.log, verified-play.log and play-diagnose.log
under work/codex-round17. Shared Lua index entries preserved across native commit.
