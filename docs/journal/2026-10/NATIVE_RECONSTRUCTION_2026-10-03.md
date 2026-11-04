# Native reconstruction - 2026-10-03 (in progress)

User requested continuing recreation work outside Lua until stopped. Work resumes
from the 2026-10-02 native journal on the existing dirty branch; unrelated work is
preserved.

## Output-backed input stream and stream-write overload (promoted)

Scratch: `work/codex-native-startup/memory-stream-copy-20261003`.

FableWin.pdb confirms the `CMemoryDataInputStream(const CMemoryDataOutputStream&)`
constructor and `CDataOutputStream::Write(const CMemoryDataOutputStream&)`
overload. Retail bodies are 0x00995EE0 (110 bytes) and 0x00995EB0 (42 bytes).
Both expand the output stream's `GetLength` and `PeekData` accessors in the
DataInputStream translation unit. The input borrows the complete written range,
independent of the output cursor; an empty array returns its own address, even
when it retains allocated capacity. The write overload dispatches exactly once
through output vtable slot 4, including for zero length.

Staged genuine C++ uses the existing shared stream classes. Constructor is
NEAR_MATCH (45/45 instructions, 93.33% ordered similarity); Write is exact MATCH.
The constructor differs in equivalent zero/nonzero branch layout. Byte tuning
stops at the passing near match as required by the reconstruction standard.

Extended constructor gate: all four previous providers retain exact patched
retail output; 300 comparative scenarios pass (12 existing + 288 new actual
x86 comparisons). New independent expectations cover null and allocated empty
arrays, unaligned data, signed/unsigned size boundaries, unused capacity,
cursor independence, preserved source storage, callee-saved registers and stack
cleanup. Calls clobber volatile registers. Linked compiled fixture adds 166
output-backed construction/write cases; provider omission still fails to link.

Dependent-header audit passes: 256 entries, zero regressions. Promoted both providers with one-shot
`work/codex-native-startup/land_memory_stream_copy.py` required passing proof
hashes and an audit with no regressions and archives every overwritten file.

Landed gate passes (6 exact/near providers, 300 comparative scenarios); focused candidate build passes all seven catalog entries in the translation unit. Symbol index refreshed for those entries. Archive: work/codex-native-startup/before-memory-stream-copy-20261003. Native closure refresh running.

The two boot leaves previously recorded as failing (GetWindowTitle 0x004023F0 and NGameText::CDataBank constructor 0x009C85A0) both pass their current parity and behavior gates. No fixture changes were needed today.

## Stream translation-unit consolidation (promoted)

The first closure exposed two duplicates: inline GetLength/PeekData emitted by
the new constructor unit conflicted with their earlier standalone strong
providers. Consolidated those members and CMemoryDataOutputStream::Write into
the DataInputStream unit, where retail expands their bodies. Retired the old
GetLength and Write source files; updated their three catalog/manifest/artifact
rows to the shared unit and its extended fixture. Existing standalone output
fixtures remain on disk as historical tests.

All ten catalog providers PASS (nine exact/relocation-exact, one near);
GetLength 7 bytes, PeekData 15 bytes, memory-output Write 129 bytes retain exact
retail output. Registered constructor gate now checks nine emitted providers;
300 native comparisons remain passing. Linked fixture retains the original
output-write growth, overwrite, zero-fill, negative/zero count and 2 GB limit
tests and checks GetLength throughout the 166 output-backed stream scenarios.
Focused build passes ten entries; symbol index refreshed.

Archive: `work/codex-native-startup/before-stream-unit-consolidation-20261003`.
A missing import in the first promotion script run was corrected after restoring
its two overwritten files; failed archive retained separately. No catalog or
manifest row had been written by that failed run.

First link refresh reached 942 sources / 634 unresolved / two duplicates,
pulling in additional already-landed families beyond the previous snapshot.
Post-consolidation closure is 940 / zero duplicates / 634 unresolved for that
seed set. Refresh including the latest accepted knockdown/creature lane is
962 sources / zero duplicates / 635 unresolved
(`native-gfmain-stream-unit-full-closure-20261003`). The source count decreases
by two versus the previous 964-source closure because the output accessor
providers now share their retail translation unit; this is no claim that 635
unresolved symbols have been solved.

## Current-thing-manager and definition query (staged)

Resumed the earlier staged dependency in
`work/codex-native-startup/thing-definition-manager-20261003`, using current
headers plus the previously recovered PDB/native declarations. Getter
0x004C7990 is `CThing::PeekDefinitionManager() const` (9 bytes); setter
0x004C74F0 is the static fastcall CThing member (7 bytes). Both relocation-exact.
Setter unit now also owns the real private static singleton storage at
0x013B8A1C; PE section evidence puts it in .data's loader-zeroed virtual tail.
Gate reads zero-filled COFF BSS with the existing coff_data helper, rather than
treating a BSS offset as file data. Raw-tail and BSS parser assumptions caused
two scratch-only gate failures, corrected from section evidence; no production
edits resulted.

Composed gate passes 108 native scenarios and 2,048 compiled scenarios, two
provider-omission links. Checks live manager rebinding, fresh reads of the
manager's definition reference at +0x20, source/receiver/neighbour preservation,
arbitrary unused receiver addresses and native callee-saved/stack behavior.
Factory allocation/construction and definition object lifetimes remain explicit
boundaries; no complete factory layout is claimed.

First header audit: 84 entries, only the two replaced providers fail their
isolated link because the composed fixture requires its other real provider.
Paired catalog fixtures now include the other provider; refreshed audit passes
all 84 entries without regressions. Both typed replacements promoted, old
misnamed flat sources/tests retired, archive
`before-thing-definition-manager-20261003`. Landed composed gate, both
candidate-required catalog fixtures, selected build and symbol-index refresh
PASS. Prototype metadata corrected separately (original display-engine receiver
and return type were wrong). Gate registered in gate_all. Closure now 964 sources /
zero duplicates / 634 unresolved (`native-gfmain-thing-definition-manager-closure-20261003`).

Next: underlying CThingCreatureBase::GetPCombatTypeDef 0x00664D40. Preserve its
original CDefPointer copy/assignment/lifetime semantics; a templated assignment
alone currently leaves implicit same-type assignment available in the shared
header. Recover this prerequisite from donor/native evidence before adding the
combat-definition getter.

## Same-type CDefPointer assignment (promoted)

Shared `fable_def_pointer.h` now spells out the original same-type assignment;
its existing templated conversion assignment did not suppress C++'s implicit
shallow copy. FableWin CCombatTypeDef-const specialization's thunk 0x017EB70B
leads to body 0x01DA50D0: compare the held pointers, release destination, reread
source, store it, lock it, return destination. This also agrees with the inline
lock/release sequence in retail GetPCombatTypeDef 0x00664D40.

Gate `gate_def_pointer_assignment.py` compares 4,096 emitted x86 scenarios
against the actual original donor body with the already-modelled Get/Release/Lock
boundaries, and runs 4,096 linked compiled scenarios. It checks null/alias/self
assignment, counter wrap, zero-reaching deletion, and callbacks which rebind the
source or destination. Both implementations must reread the source after release;
the destination is cleared after the deletion callback. Borrowed test pointees
are not physically freed. Provider omission fails to link. The old header fails
the new oracle on the first distinct null-to-nonnull assignment (recorded baseline
rejection); the staged and landed headers pass. 188 dependent catalog entries
show zero regressions.

Archive `before-def-pointer-assignment-20261003`; gate registered in gate_all.
Helper provider is under rebuild/tests/integration; no standalone retail-function
coverage is claimed. Latest creature/definition ancestry evidence is
`inspect_combat_def_types_20261003.py`: CThingCreatureBase's own PDef is
CDefPointer<const CThingCreatureDef> (FableWin +0x130; retail getter +0xE0),
and CThingCreatureDef::CombatTypeDef is a long (FableWin +0x134; retail +0x124).
Continue full getter and its typed template-definition lookups.

## Global definition lookup (promoted)

Const 0x009AD9E0 and mutable 0x009ADA10 now use genuine
`CDefinitionManager::GetPDefFromGlobalIndex(long)` declarations and shared
layout, replacing the flat and misnamed providers. Both 44-byte functions
are relocation-exact. Loading occurs before reading the table and selected
entry, so loader callbacks which replace either are observed.

Composed proof: 288 native comparisons, 1,440 compiled scenarios, two
provider-omission links, and 50 dependent catalog entries without regressions.
Fixtures cover padded negative indices, null pointers, counter wrapping and
temporary destruction. Landed gate, both required-provider catalog fixtures,
selected build and focused symbol-index refresh pass. Archive
`before-global-def-lookup-20261003`; gate registered in gate_all.

## Symbol-map output lookup (promoted)

PDB identifies 0x009E5170 as `CSymbolMap::FindSymbol(const CCharString&,
long&) const`. Its full body is 76 bytes, including the final `ret 8`;
the initial scratch gate used a truncated 74-byte range and was corrected
from disassembly. Genuine member implementation is relocation-exact.

Gate executes the real lower-bound, FastCompare, value lookup, existence
check and FindSymbol providers together: 2,040 native comparisons and
2,040 compiled scenarios, five omission links, no emulated call boundaries.
Missing names preserve the output; present zero and negative values succeed.
92 dependent catalog entries show no regressions. Archive
`before-find-symbol-20261003`; landed gate passes and gate_all includes it.
Continue definition-name index lookup and its CRC-map lower-bound dependency.

## Definition-name index (promoted)

0x009AD410 is the const `CDefinitionManager::GetDefGlobalIndexFromName`;
0x009B21F0 is the const lower-bound for unsigned-long -> long entries.
PDB confirms the value is long, correcting the manifest's propagated
CDefString specialization. Binary search matches all 60 bytes; name lookup
accepts NEAR_MATCH at 91.97% opcode similarity (68 vs 69 instructions).
Stop byte tuning at that accepted threshold.

Name lookup rejects null and zero-length names first. A nonempty CRC map
takes precedence; a missing CRC does not fall back to symbols. The cached
map end survives CRC/search calls. The inline search helper keeps a separate
key copy for LowerBound. These behaviors are preserved in genuine C++.

Six real providers, native CRC table verified against all 256 polynomial
entries, no emulated call boundaries: 3,060 native comparisons and 3,060
compiled cases cover both map paths plus deliberately missing CRCs when
symbols exist. All six provider omissions fail to link. 50 dependent catalog
entries have no regressions; both required-provider catalog fixtures pass
(MATCH / NEAR_MATCH), as do the landed gate, selected build and focused
symbol-index refresh. The first paired fixture failed because VC7.1 instantiated
the string-map search before its explicit specialization; ordering the existing
string specialization before the generic unsigned-key provider corrected it.
Archive `before-name-index-20261003`; gate registered in gate_all.

Latest completed global-index closure: 966 sources / zero duplicates / 634
unresolved (`native-gfmain-global-def-closure-20261003`). Continue const and
mutable instantiation-name lookup 0x009ADA40 / 0x009ADA90, scratch byte-exact.

## Instantiation-name definition pointers (promoted)

Genuine const 0x009ADA40 and mutable 0x009ADA90 overloads each match 77
relocated bytes. They always query the name index first, load only for a
positive signed index, then reread the instantiated table/entry before taking
the reference. Zero and negative indices return an empty pointer.

Proof: 432 native comparisons and 2,160 compiled cases with the name-index
and loader calls as explicit modeled boundaries. Name-index provider itself
was separately validated above with its real CRC and symbol dependencies.
Checks include receiver/name identity, volatile-register clobbering, positive
indices 1/3/7, zero, -1 and INT_MIN, null definitions, wrapped counts,
loader table/entry rebinding and temporary destruction. Two provider-omission
links fail. Both catalog fixtures pass exact parity and behavior; 52 dependent
catalog entries have no regressions. Landed gate, selected build and focused
symbol-index refresh pass. Archive `before-named-def-lookup-20261003`;
gate registered in gate_all.

Next scratch batch: generic `GetDef<CCombatTypeDef>` by global index
0x00669640 and instantiation name 0x00669770, both relocation-exact already
(108 / 104 bytes). Preserve output on failure, temporary reference acquisition
and release, same-pointer assignment, and deletion callbacks.

## Combat-definition template helpers (promoted)

Generic two-argument `CDefinitionManager::GetDef<T>` overloads by index and
name are instantiated for CCombatTypeDef at 0x00669640 / 0x00669770. Both
match retail exactly (108 / 104 bytes); propagated manifest specializations
for unrelated definition types were corrected using the native combat getter's
call path. Existing three-argument frontend overload remains independent.

24,576 native comparisons execute real emitted helper bodies against retail
with pointer-acquisition and virtual deletion as explicit boundaries. All
three pointee counts vary independently across 0, 1, 2 and UINT_MAX; source,
destination and deletion callbacks cover aliases, temporary rebinding and
output rebinding. 12,288 compiled scenarios validate failure preservation,
alias behavior, wrapping counts and output-rebinding deletion callbacks. All
borrowed fixture pointees remain allocated. Both provider omissions fail;
both required-provider catalog fixtures pass exact parity and behavior.
54 dependent catalog entries show no regressions. Landed gate, selected build
and focused symbol-index refresh pass. Archive
`before-combat-def-helpers-20261003`; gate registered in gate_all.

Latest pre-helper closure: 971 sources / zero duplicates / 634 unresolved
(`native-gfmain-definition-name-closure-20261003`). Full creature getter is
the next dependency. PDB proves CThingCreatureDef derives from CThingBaseDef,
which derives from CParentDefClassBase; CTCCombat's +0x28 field is a
CDefPointer<const CCombatTypeDef>, accessed by a const getter. Keep the
creature's own +0xE0 definition pointer distinct from object-definition fields.

## Creature combat-definition getter (promoted)

`CThingCreatureBase::GetPCombatTypeDef() const` at 0x00664D40 now follows
the original primary CThing / CThingPhysical / CThingGameObject ancestry.
Partial typed layouts put its own PDef at retail +0xE0, the creature
definition's CombatTypeDef index at +0x124, and CTCCombat's typed
PCombatTypeDef at +0x28. PDB inheritance and offsets, retail field reads,
and static assertions support these partial prefixes.

The getter takes the combat component's definition when present. Otherwise
it queries the configured index, or constructs `COMBAT_TYPE_NULL` for a zero
index. The candidate accepts NEAR_MATCH: 280 built vs 268 retail bytes,
102 vs 98 instructions, 91% opcode similarity. Stop byte tuning at the
accepted threshold. Its shared translation unit also emits the original
`CThing::GetTC(ETCInterfaceType) const` body at 0x00436990 exactly (45 bytes);
the old standalone provider and test were retired to prevent duplicate
definitions.

The composed gate passes 2,592 native comparisons and 2,592 compiled cases.
It executes the real component lower-bound and definition-manager query;
the previously proved GetDef index/name helpers, string lifetime, and virtual
deletion are explicit boundaries. Cases cover map sizes, component presence,
null component definitions, negative/zero/positive indices, null fallback
results, independent counter wrap, stack and register preservation, and
untouched object neighbours. Four provider-omission links fail. Both
required-provider catalog fixtures pass; 91 dependent-header entries have
zero regressions. Selected build and focused symbol-index refresh pass.
Archive `before-creature-combat-20261003`; gate registered in gate_all.

Full closure is now 973 sources / zero duplicates / 633 unresolved
(`native-gfmain-creature-combat-closure-20261003`). Continue native frontier;
the GFMain closure remains incomplete.

## Bank counted-pointer copy constructors (promoted)

The PDB identifies 0x0041BA30 and 0x00428905 as copy constructors for
`CCountedPointer<CGraphicDataBank>` and `CCountedPointer<CMeshDataBank>`.
Their genuine typed C++ providers each match all 25 retail bytes. Two older
sources with unrelated propagated function names were retired. The gate passes
576 native comparisons and 288 compiled cases; both provider omissions fail
to link. Catalog grading, selected build, and focused symbol-index refresh
pass. Archive `before-bank-pointer-copies-20261003` preserves prior sources.

## Graphic-bank counted-pointer ownership (promoted)

`CCountedPointer<CGraphicDataBank>` now has genuine `ReleaseData` at 0x00419108,
`ShareData` at 0x00419134, and copy assignment at 0x004194BA. All 105 retail
bytes match after relocations (44 / 38 / 23). Three misleadingly named prior
providers were retired. The native gate passes 10,368 comparisons, including
deletion callbacks that rebind source or destination storage, and the compiled
fixture passes 864 cases. The required-provider omission fails; all three
catalog grades, landed gate, selected build, and focused symbol-index refresh
pass. Archive `before-graphic-pointer-assignment-20261003` preserves prior
sources.

The refreshed GFMain closure has 976 sources, zero duplicates, and 629
unresolved symbols (`native-gfmain-bank-pointer-closure-20261003`). Continue
with counted-pointer lifetime and bank-manager dependencies from this frontier.

## Mesh-bank counted-pointer lifetime (promoted)

`CCountedPointer<CMeshDataBank>::ReleaseData` at 0x0042918C and `ShareData` at
0x004291B8 now have genuine typed providers. The call from frontend interface
cleanup confirms the release address; both implementations match all 82 retail
bytes after relocations (44 / 38). The two unrelated, misnamed catalog sources
and fixtures were retired to archive `before-mesh-pointer-lifetime-20261003`.
The gate passes 6,912 native comparisons and 576 compiled cases, including
counter wrap, shared ownership, and deletion callbacks that rebind pointer
storage. A missing-provider link fails. Both catalog grades, the landed gate,
selected build, and focused symbol-index refresh pass.

Current GFMain closure: 977 sources, zero duplicates, 628 unresolved symbols
(`native-gfmain-mesh-pointer-closure-20261003`).

## Particle environment counted-pointer copy (promoted)

The frontend particle initializer calls 0x00418FBC to copy a
`CCountedPointer<NParticleEngine::CAEnvironmentInteraction>` by value. The
typed constructor matches all 25 retail bytes. It replaces a previously
misidentified `CBankFileAsyncEntry` provider, archived as
`before-particle-pointer-copy-20261003`. The native oracle and compiled
fixture each pass 288 cases covering null blocks, aliasing, and count wrap;
the provider-omission link fails. The landed gate, catalog grade, selected
build, and focused symbol-index refresh pass.

Current GFMain closure: 978 sources, zero duplicates, 627 unresolved symbols
(`native-gfmain-particle-pointer-closure-20261003`). The separate raw-pointer
constructor at 0x0041956C was initially below threshold and is continued below.

## Particle environment raw-pointer constructor (promoted)

`CCountedPointer<NParticleEngine::CAEnvironmentInteraction>` now constructs
from a raw pointer at 0x0041956C. Genuine C++ accepts NEAR_MATCH: 58 built vs
62 retail bytes, 21 vs 23 instructions, 90.91% opcode similarity. Stop byte
tuning at this accepted threshold. The native gate executes 18 retail/candidate
comparisons covering null data, successful allocation, failed allocation,
and initial object sentinels. The 192-case compiled fixture validates count
initialization, pointer storage, and the deletion function pointer. A provider
omission fails to link. Catalog behavior is linked to the candidate, and
passes NEAR_MATCH / PASS after a self-contained fixture was corrected.
The landed gate, selected build, and focused symbol-index refresh pass.
Archive `before-particle-pointer-raw-20261003` records prior catalog state.

Current GFMain closure: 979 sources, zero duplicates, 626 unresolved symbols
(`native-gfmain-particle-raw-closure-20261003`).

## Particle environment global storage (promoted)

The native particle-environment setter reads the counted pointer at
0x013D2E2C. Its eight bytes lie in the PE's zero-fill data tail; the typed
`NParticleEngine::PEnvironmentInteraction` provider emits eight zeroed BSS
bytes at the symbol. The gate confirms the address/size against the PE,
rejects a missing-provider link, and passes 12 compiled storage scenarios.
The landed gate and focused symbol-index refresh pass. This is data coverage
only; no function parity is claimed. Archive
`before-particle-environment-global-20261003` records the prior index state.

Current GFMain closure: 980 sources, zero duplicates, 625 unresolved symbols
(`native-gfmain-particle-global-closure-20261003`).

## Particle manager global storage (promoted)

The three remaining namespace counted-pointer globals have typed providers:
`PEmitterDatabase` at 0x013D2E14, `PEmitterManager` at 0x013D2E1C,
and `PPSCManager` at 0x013D2E24. Each is eight bytes in the retail PE's
zero-fill data tail and emits zeroed BSS in its provider object. All three
provider-omission links fail; the combined compiled fixture passes 36 storage
cases. The landed gate and four-entry focused symbol-index refresh pass.
Each is a separate translation unit, so there is no claim that their COFF
section order fixes final linked addresses. This is data coverage only.
Archive `before-particle-manager-globals-20261003` records prior index state.

Current GFMain closure: 983 sources, zero duplicates, 622 unresolved symbols
(`native-gfmain-particle-managers-closure-20261003`).

## Particle manager counted-pointer reset (promoted)

The database, emitter-manager, and PSC-manager `Reset` providers at
0x00AECCC0 / 0x00AECAC0 / 0x00AECC30 each match all 103 retail bytes after
relocations. The initial instruction comparison was 97.22%, but full target
relocation proved stronger exact parity. The gate passes 288 native
comparisons, including zero/wrapped counts, null/non-null replacements,
allocation failure, and a deletion callback that rebinds the control block.
The compiled fixture passes 72 cases. Pointee destructors are explicit
link-only fixture boundaries; the tested callbacks do not invoke them.
Provider omission fails; all three catalog grades are RELOCATION_MATCH / PASS.
The landed gate, selected build and focused symbol-index refresh pass.
Archive `before-particle-manager-reset-20261003` records prior catalog state.

Current GFMain closure: 984 sources, zero duplicates, 622 unresolved symbols
(`native-gfmain-particle-reset-closure-20261003`). The three Reset symbols
are resolved, exposing three real pointee destructor dependencies in their
place. No net unresolved-count reduction is claimed for this batch.

## Particle bank pointer setters (promoted)

`NParticleEngine::SetGraphicBank` at 0x00AEAA80 and `SetMeshBank` at
0x00AEAA90 each match all seven relocated retail bytes. They store the
supplied bank address in distinct four-byte globals at 0x013D2E08 and
0x013D2E0C, both in the retail PE's zero-fill tail. The typed source emits
both zeroed globals, and its compiled fixture passes nine identity/isolation
cases. The provider-omission link fails, both catalog grades are
RELOCATION_MATCH / PASS, and the landed gate, selected build and focused
symbol-index refresh pass. Archive `before-particle-bank-setters-20261003`.

Current GFMain closure: 985 sources, zero duplicates, 621 unresolved symbols
(`native-gfmain-particle-bank-setters-closure-20261003`).

## PSC-manager destructor (promoted)

`NParticleEngine::CPSCManager::~CPSCManager` at 0x00AECA80 matches all 57
relocated retail bytes. It iterates 16-byte entries, destroys the string at
offset +12 before the one at +0, and releases non-null backing storage after
the walk. The gate passes 54 native comparisons, including a callback that
mutates the stored begin/end pointers, and 18 compiled cases. String
destruction and `operator delete[]` are explicit native boundaries; the
compiled fixture logs the string-destruction order. Provider omission fails,
catalog grading is RELOCATION_MATCH / PASS, and the landed gate, selected
build and focused symbol-index refresh pass. Archive
`before-psc-manager-destructor-20261003`.

Current GFMain closure: 986 sources, zero duplicates, 620 unresolved symbols
(`native-gfmain-psc-destructor-closure-20261003`).

## Particle emitter-database constructor and layout (promoted)

`NParticleEngine::CParticleEmitterDatabase` now uses a typed `CSymbolMap`
member at +0, a 16-byte initialized state block at +0x28, the counted bank
pointer at +0x38, and two `CCharString` members at +0x44 / +0x48. A four-byte
unknown field after the 0x24-byte symbol map preserves the retail offsets;
size assertions prove the state block and complete 0x4C-byte object.
This member order makes the constructor at 0x00AE8BF0 match all 55 retail
bytes after relocations. Its native and compiled fixtures each pass five
initial-fill cases, checking member-call order and field writes; constructor
calls are explicit native boundaries. Provider omission fails. The staged
header audit initially found four fixture-only missing cleanup/constructor
boundaries; after those fixtures were updated, all 19 dependent catalog
entries passed with zero regressions. The landed gate, exact catalog grade,
selected build and focused symbol-index refresh pass. Archive
`before-particle-database-ctor-20261003` preserves the prior header and
catalog state.

Current GFMain closure: 987 sources, zero duplicates, 619 unresolved symbols
(`native-gfmain-particle-database-ctor-closure-20261003`).

## Hero-information unpause global (promoted)

`GUnpauseAfterClosingHeroInformationScreen` is the initialized byte at
0x0137D76C read and set by `CWorld::EACloseHeroInformationScreen`. Its new
typed provider emits `01`, matching the retail PE's raw `.data` byte. The
compiled fixture passes eight storage cases from the true initial value;
omitting the provider fails the link. The landed gate and focused symbol-index
refresh pass. This is data coverage only, with no new reconstructed function.
Archive `before-unpause-global-20261003` preserves the prior index and gate
list. The GFMain closure now has 988 sources, zero duplicates, and 618
unresolved symbols (`native-gfmain-unpause-global-closure-20261003`).

The particle emitter-database destructor has a staged exact 111-byte
relocated match, and its constructor remains exact under the staged headers.
It is not promoted: the needed `CSymbolMap` destructor change affects a
shared header, and the 118-entry staged audit found 14 regressions. The
staged code remains in `work/codex-native-startup/particle-database-dtor-20261003`
for a later isolated repair. A C++ replacement for the baked
`GFLimitReturn<double>` helper was also held back because its best codegen
comparison was below the accepted near-match threshold.

## Package-set getters and layout (promoted)

`CGameEventPackageSet::GetPackage` at 0x009F1730 and `GetNoPackages` at
0x009F1750 now have genuine typed sources. The layout names the count at +0
and 50 `CGameEventPackage` slots at +4, preserving the PDB's 0x13A14-byte
class size. Both functions match all 17 and three retail bytes. The combined
fixture passes 40 compiled cases; both provider-omission links fail. All 113
dependent catalog entries passed the staged-header audit without regressions.
The landed gate, both catalog grades (`MATCH` / `PASS`), selected build, and
focused symbol-index refresh pass. The former address occupants were generic
misidentifications (`CMouse::GetEventFromStore` and `CInputEvent::GetKey`);
their source and test files are preserved in
`before-package-set-getters-20261003` and retired from the catalog.

Current GFMain closure: 990 sources, zero duplicates, 616 unresolved symbols
(`native-gfmain-package-set-getters-closure-20261003`).

## Package-set append and clear (promoted)

`CGameEventPackageSet::AddPackage` at 0x009F16F0 and `Clear` at 0x009F1720
now use the typed package array and count. Their 37 and seven bytes match
retail exactly. The compiled fixture passes 32 whole-object append/clear
cases, including the last package slot and preservation of untouched bytes;
two provider-omission links fail. Both catalog grades are `MATCH` / `PASS`,
and the landed gate, selected build, and focused symbol-index refresh pass.
The old `AddPackage` overlay and unrelated `CBase` constructor at the second
address were archived in `before-package-set-mutations-20261003` and retired.

Current GFMain closure: 992 sources, zero duplicates, 614 unresolved symbols
(`native-gfmain-package-set-mutations-closure-20261003`).

## Package-set constructor and member initialization (promoted)

`CGameEventPackageSet::CGameEventPackageSet` at 0x009F1A80 now constructs
the typed 50-package array and clears its count. An explicit default
`CGameEventPackage` constructor clears each package's event count before
constructing its 40 events. The resulting object supplies exact retail
bodies for the package-set constructor (60 relocated bytes), the emitted
package constructor at 0x00419220 (35 bytes), and the emitted event
constructor at 0x004159A0 (11 bytes). The gate passes 2,051 compiled field
checks across the complete object and a provider-omission link. The staged
header audit covered 157 catalog entries with zero regressions. Its first
pass exposed one extractor false positive at the unrelated one-byte
`CGameComponent::SetQuit`; a precise catalog `Leaf` hint selects the actual
virtual symbol, and both baseline and staged copies match. The landed gate,
constructor catalog grade (`RELOCATION_MATCH` / `PASS`), selected build, and
focused symbol-index refresh pass. Archive
`before-package-set-ctor-20261003` preserves the prior header and catalog.

Current GFMain closure: 993 sources, zero duplicates, 613 unresolved symbols
(`native-gfmain-package-set-ctor-closure-20261003`). The emitted member
constructors have retail parity evidence, but only the package-set constructor
is newly counted as a closure source.

## Event-package clear and getters (promoted)

`CGameEventPackage::Clear` at 0x009F16C0, `GetEvent` at 0x009F16D0, and
`GetNoEvents` at 0x009F16E0 match their complete eight-, 14-, and four-byte
retail bodies. The compiled fixture passes 48 cases for count preservation,
event stride, and clear's single-field write; omitting each provider fails
its link. The two getter sources were already typed and retained, while their
fixtures and catalog metadata were corrected. The generic clear source was
retired and archived in `before-event-package-accessors-20261003`. The
landed gate, three catalog grades (`MATCH` / `PASS`), selected build, and
focused symbol-index refresh pass.

Current GFMain closure: 994 sources, zero duplicates, 612 unresolved symbols
(`native-gfmain-event-package-accessors-closure-20261003`). The two retained
getter sources were already reachable; clear accounts for the one new source.

## Event-package append and replacement (promoted)

`CGameEventPackage::AddEvent` at 0x009F1650 now scans for a matching event
type and signed player when the incoming event has `Replacement` set. It
replaces the first match without changing the count; otherwise it appends a
40-byte event and increments the count. The typed C++ body matches all 105
retail bytes exactly. Its compiled fixture passes 96 whole-object cases
covering empty, one-entry, multi-entry, and last-slot packages, no-replace,
first/last matches, duplicate matches, and type/player mismatches. Provider
omission fails. The landed gate, catalog `MATCH` / `PASS`, selected build,
and focused symbol-index refresh pass. Archive
`before-event-package-add-20261003` preserves prior catalog state.

Current GFMain closure: 995 sources, zero duplicates, 611 unresolved symbols
(`native-gfmain-event-package-add-closure-20261003`).

## Typed package-set compression (promoted)

The existing `CGameEventPackageSet::CompressIntoBuffer` at 0x009F19A0
now uses the shared typed package-set header rather than a second local set
definition. Its event-tail view starts at each event's +4 byte, preserving
the optimized retail loop and all 215 exact bytes. A new candidate-linked
fixture passes 20 wire-format scenarios from empty sets through 50 packages
with 40 events each, checking type truncation, the replacement bit, player,
timestamp, variable payload length, output tail, and unchanged input. The
provider-omission link fails. The landed gate, catalog `MATCH` / `PASS`,
selected build, and focused symbol-index refresh pass. Archive
`before-package-set-compress-typed-20261003` preserves the old overlay and
self-contained fixture.

Current GFMain closure remains 995 sources, zero duplicates, and 611
unresolved symbols (`native-gfmain-package-set-compress-typed-closure-20261003`);
this was a source and test improvement for an already reachable provider.

## World frame counter storage (promoted)

`CWorld::Frame` at 0x013B89BC is now a typed four-byte zero-filled data
provider. The retail PE places the address beyond `.data`'s raw bytes, and
the provider emits four zero BSS bytes. A compiled fixture links the existing
`CWorld::GetFrame` function and passes eight signed-value round trips;
omitting either the data or reader provider fails the link. The landed gate
and focused symbol-index refresh pass. This is data coverage only. Archive
`before-world-frame-global-20261003` preserves the prior index state.

Current GFMain closure: 996 sources, zero duplicates, 610 unresolved symbols
(`native-gfmain-world-frame-global-closure-20261003`).

`CGameEventPackageSet::InitFromCompressedBuffer` and its event reader remain
staged in `work/codex-native-startup/package-set-decompress-20261003`.
The first typed package-set candidate reproduced the 221-byte body length
with `/GS`, but its best opcode score was 87.77%, below the project's
accepted near-match threshold; its reader was also below threshold. Neither
source nor header change was promoted.

## World checksum and thing-manager chain (promoted)

`CWorld::GetChecksum` at 0x0049E200 now reads its typed `PThingManager`
pointer at +0x50, calls `CThingManager::GetNonLocalThingChecksum`, then adds
the static world frame. The pointer offset is also confirmed by the retail
`GetThingManager` and `PeekThingManager` accessors. Its 17 relocated bytes
match retail exactly. A 32-case compiled fixture covers signed frame values,
unsigned checksum wraparound, and a callback that changes the frame during
the thing-manager call; both function and frame-data omission links fail.
The staged changes to `fable_world.h` and `fable_thing_manager.h` passed all
89 dependent catalog entries without regression. The landed gate, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol-index refresh
pass. Archive `before-world-checksum-20261003` preserves the prior headers.
The intermediate closure had 997 sources, zero duplicates, and 610
unresolved symbols: the checksum provider replaced its own unresolved symbol
with the thing-manager checksum dependency.

`CThingManager::GetNonLocalThingChecksum` at 0x0051E530 now walks the native
list at manager+0x18, skips things whose Dead bit at +0x91 is set, and adds
the virtual checksum at vtable slot +0x5C. It re-reads the list head after
each callback, matching all 48 retail bytes. Its compiled fixture passes
80 live/dead, wraparound, and head-rebinding cases; provider omission fails.
The landed gate, catalog `MATCH` / `PASS`, selected build, and focused
symbol-index refresh pass. Archive `before-thing-manager-checksum-20261003`.
That closure reached 998 sources, zero duplicates, and 609 unresolved symbols.

The formerly generic `CWorld::GetThingManager` and `PeekThingManager`
address occupants are now the genuine virtual methods. Each matches its
four-byte retail body. The compiled identity fixture passes 12 cases, two
provider-omission links fail, and the landed gate, both catalog `MATCH` /
`PASS` grades, selected build, and focused symbol-index refresh pass.
Archive `before-world-thing-manager-accessors-20261003` keeps their prior
sources and tests. Current GFMain closure: 1,000 sources, zero duplicates,
609 unresolved symbols
(`native-gfmain-world-thing-manager-accessors-closure-20261003`). The two
accessors were added as audited sources; they do not change the unresolved
frontier count.

## World-map accessors (promoted)

`CWorld::GetWorldMap` and `PeekWorldMap` at 0x004AE8B0/0x004AE8C0 now use
the typed `PWorldMap` field at +0x14. Each genuine virtual method matches its
four retail bytes exactly. A compiled identity fixture passes 12 cases and
both provider-omission links fail. The shared-header change passed 93
dependent catalog entries without regression. The landed gate, both catalog
`MATCH` / `PASS` grades, selected build, and focused symbol-index refresh
pass. Archive `before-world-map-accessors-20261003` preserves the old
generic occupants. Current GFMain closure: 1,002 sources, zero duplicates,
609 unresolved symbols
(`native-gfmain-world-map-accessors-closure-20261003`).

The `CIDrawWorld` override `CWorld::DrawGetWorldMap` at 0x0049E1A0 now
uses a narrow native view of `CWorldMap`'s second polymorphic base. VC7.1's
ordinary base-pointer conversion reproduces all 14 retail bytes, including
the null case and +4 adjustment. The compiled fixture passes 12 pointer
identity cases, and provider omission fails to link. The landed gate,
catalog `MATCH` / `PASS`, selected build, and focused symbol-index refresh
pass. Archive `before-world-draw-map-20261003` preserves the former flat
layout source. Current GFMain closure: 1,003 sources, zero duplicates, 609
unresolved symbols (`native-gfmain-world-draw-map-closure-20261003`).

`CWorld::DrawGetEnvironment` at 0x0049E350 now reads the shared typed
`PEnvironment` pointer at world+0x18. Its secondary-interface `this` pointer
accounts for the retail `mov eax,[ecx+0x14]`; the genuine override matches
all four bytes. The compiled fixture passes 12 null/non-null and alignment
cases, and provider omission fails to link. The landed gate, catalog
`MATCH` / `PASS`, selected build, and focused symbol-index refresh pass.
Archive `before-world-draw-environment-20261003` preserves the former
flat-layout source. Current GFMain closure: 1,004 sources, zero duplicates,
609 unresolved symbols
(`native-gfmain-world-draw-environment-closure-20261003`).

`CWorld::DrawGetProceduralTexture` at 0x004AE890 now uses its genuine
virtual signature and returns null, matching all three retail bytes. The
compiled fixture passes 32 caller-pointer cases, and omitting the provider
fails to link. The landed gate, catalog `MATCH` / `PASS`, selected build,
and focused symbol-index refresh pass. Archive
`before-world-draw-procedural-20261003` preserves the generic stand-in.
Current GFMain closure: 1,005 sources, zero duplicates, 609 unresolved
symbols (`native-gfmain-world-draw-procedural-closure-20261003`).

`CWorld::PostLoadInit` at 0x0049D970 now sets the typed `JustLoaded` byte
at +0x80. The virtual method matches all eight retail bytes. A compiled
fixture checks 16 initial-value and alignment cases; provider omission
fails to link. The shared-header audit passed 98 dependent catalog entries
without regression. The landed gate, catalog `MATCH` / `PASS`, selected
build, and focused symbol-index refresh pass. Archive
`before-world-post-load-init-20261003` preserves the former print-only
fixture and generic layout source. Current GFMain closure: 1,006 sources,
zero duplicates, 609 unresolved symbols
(`native-gfmain-world-post-load-init-closure-20261003`).

The anonymous `$E2` catalog occupant at 0x0049D820 is now the real
`CWorld::ChangeResolution(unsigned long,unsigned long,unsigned long)`.
Its empty body matches the retail three-byte `ret 0x0c`, and a compiled
fixture verifies 16 calls leave world storage unchanged; provider omission
fails to link. The landed gate, catalog `MATCH` / `PASS`, selected build,
and focused symbol-index refresh pass. Archive
`before-world-change-resolution-20261003` keeps the old anonymous source
and print-only fixture. Current GFMain closure: 1,007 sources, zero
duplicates, 608 unresolved symbols
(`native-gfmain-world-change-resolution-closure-20261003`). This closes
one unresolved native symbol.

The adjacent anonymous `$E2` occupant at 0x0049D830 is now
`CWorld::ChangeTextureColourDepth(long)`. Its empty body matches the
retail three-byte `ret 4`; a compiled fixture verifies 16 calls leave
world storage unchanged, and omission fails to link. The landed gate,
catalog `MATCH` / `PASS`, selected build, and focused symbol-index refresh
pass. Archive `before-world-change-texture-depth-20261003` keeps the old
anonymous source and print-only fixture. Current GFMain closure: 1,008
sources, zero duplicates, 607 unresolved symbols
(`native-gfmain-world-change-texture-depth-closure-20261003`).

`CMeshBankManager::OnPostDeviceReset` at 0x00A09D80 now uses its genuine
virtual signature and returns true, matching all three retail bytes. A
compiled fixture passes 32 caller-pointer cases and provider omission
fails to link. The landed gate, catalog `MATCH` / `PASS`, selected build,
and focused symbol-index refresh pass. Archive
`before-mesh-bank-post-reset-20261003` preserves the generic standalone
source. Current GFMain closure: 1,009 sources, zero duplicates, 606
unresolved symbols (`native-gfmain-mesh-bank-post-reset-closure-20261003`).

`CMeshBankManager::OnPreDeviceReset` at 0x00A09D90 now traverses its
STLport mesh-bank list and calls each bank's device-resource release
method. VC7.1 reproduces the 34-byte retail body after resolving its
single call relocation. The compiled fixture passes 16 empty and ordered
list cases, and provider omission fails. A new oracle row records the
retail body and decorated symbol. The landed gate, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol-index
refresh pass. Current GFMain closure: 1,010 sources, zero duplicates,
606 unresolved symbols (`native-gfmain-mesh-bank-pre-reset-closure-20261003`):
the manager method is resolved, while the called mesh-data-bank method is
the next frontier. Archive `before-mesh-bank-pre-reset-20261003` keeps
the previous catalog and oracle state.

`CMeshDataBank::FreeDeviceSpecificResources` at 0x00A24000 now walks
its native STLport vector at +0x380, skips null mesh infos, and calls
each non-null info's virtual resource-release slot. It re-reads the
vector length after each callback, reproducing all 73 retail bytes.
A compiled fixture passes 24 empty, null-entry, ordered-call, and
callback-shrink cases; provider omission fails. The new retail oracle
row, landed gate, catalog `MATCH` / `PASS`, selected build, and focused
symbol-index refresh pass. Current GFMain closure: 1,011 sources, zero
duplicates, 605 unresolved symbols
(`native-gfmain-mesh-data-bank-free-device-closure-20261003`). Archive
`before-mesh-data-bank-free-device-20261003` keeps the previous catalog
and oracle state.

`CConsoleVariable::GetTypeAsString` at 0x00413700 now returns the retail
`"variable"` text through the native `CCharString` constructor. Its
25-byte body matches after resolving the string and constructor
relocations. A linked fixture passes 16 return-value, constructor-length,
and caller-pointer cases; provider omission fails. The new retail oracle
row, landed gate, catalog `RELOCATION_MATCH` / `PASS`, selected build,
and focused symbol-index refresh pass. Current GFMain closure: 1,012
sources, zero duplicates, 604 unresolved symbols
(`native-gfmain-console-variable-type-string-closure-20261003`). Archive
`before-console-variable-type-string-20261003` preserves the previous
catalog and oracle state.

`CConsoleCommandBase::GetTypeAsString` at 0x00414F70 now returns the
retail `"command"` text through the same native string constructor.
Its 25-byte body matches after resolving two relocations. A linked
fixture passes 16 return-value and constructor-argument cases, and
provider omission fails. The existing oracle row was byte-correct;
the prior asm-baked catalog occupant is archived in
`before-console-command-type-string-20261003`. The landed gate, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol-index
refresh pass. Current GFMain closure: 1,013 sources, zero duplicates,
603 unresolved symbols
(`native-gfmain-console-command-type-string-closure-20261003`).

`_zcalloc` at 0x00C06030 now multiplies item count by size and calls
the MSVCR71 `malloc` import at 0x01440158. Its 20-byte body matches
retail after the import-pointer relocation. A linked fixture passes
64 zero, ordinary, and wraparound multiplication cases, checking the
forwarded allocation size and result. The gate also verifies the PE
import identity and provider omission. The new oracle row, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol-index
refresh pass. Current GFMain closure: 1,014 sources, zero duplicates,
602 unresolved symbols (`native-gfmain-zcalloc-closure-20261003`).
Archive `before-zcalloc-20261003` preserves the previous catalog and
oracle state.

`_zcfree` at 0x00C06050 now forwards its pointer argument to the
MSVCR71 `free` import at 0x01440154, ignoring the zlib opaque argument.
The 13-byte body matches retail after the import relocation, and a
linked fixture passes 16 pointer-forwarding cases with provider
omission. FableWin PDB evidence corrected the prior propagated
`operator_new` oracle label. The landed gate, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol-index
refresh pass. Archive `before-zcfree-20261003` preserves the prior
source, test, and oracle label. Current GFMain closure: 1,015 sources,
zero duplicates, 601 unresolved symbols
(`native-gfmain-zcfree-closure-20261003`).

`CCompressorZlib::CompressAsImage` at 0x0052AE10 now uses ordinary
C++ to call its virtual `Compress` slot with the five compression
arguments, ignoring three image-dimension parameters. VC7.1 reproduces
all 33 retail bytes, replacing the naked-assembly forwarder. A compiled
fixture passes 32 argument-identity and dispatch cases with provider
omission. The landed gate, catalog `MATCH` / `PASS`, selected build,
and focused symbol-index refresh pass. Archive
`before-zlib-compress-as-image-20261003` preserves the former asm source
and fixture. Current GFMain closure: 1,016 sources, zero duplicates,
600 unresolved symbols
(`native-gfmain-zlib-compress-as-image-closure-20261003`).

`CConsoleVariable::GetFormattedName` at 0x009EA490 now returns
`Name + " "` through the existing fastcall `CCharString` operator.
Its 26-byte body matches retail after resolving the literal and
operator relocations. A linked fixture passes 16 cases checking the
`Name` reference, space literal, result object, and provider omission.
The new retail oracle row, landed gate, catalog `RELOCATION_MATCH` /
`PASS`, selected build, and focused symbol-index refresh pass. Current
GFMain closure: 1,017 sources, zero duplicates, 599 unresolved symbols
(`native-gfmain-console-variable-formatted-name-closure-20261003`).
Archive `before-console-variable-formatted-name-20261003` preserves the
prior catalog and oracle state.

`CAFile::GetFileInformation` at 0x00412090 now returns a default
`CFileInformation`, constructing its three `CDateAndTime` fields in
creation/access/write order. VC7.1 reproduces all 34 retail bytes after
resolving the three constructor-call relocations. A linked fixture passes
16 cases checking constructor order, zeroed timestamp fields, and no
mutation of the file object; the provider-omission link fails as expected.
The dedicated gate, catalog `RELOCATION_MATCH` / `PASS`, selected build,
and focused symbol index pass. The archive
`before-cafile-file-info-20261003` preserves prior catalog, gate, oracle,
and manifest state. Current GFMain closure: 1,018 sources, zero
duplicates, 598 unresolved symbols
(`native-gfmain-cafile-file-info-closure-20261003`).

`CAFile::m_errorCallback` at 0x013BC9FC and
`CAFile::m_harddiskErrorCallback` at 0x013BCA00 now have genuine
zero-filled four-byte data providers. The dedicated gate checks COFF
storage and retail PE `.data` zero-fill, rejects a missing-provider
link, and passes 16 linked callback-dispatch cases. The focused symbol
index resolves both names. Archive `before-cafile-callback-data-20261003`
preserves prior index, artifact, and gate state. Current GFMain closure:
1,019 sources, zero duplicates, 596 unresolved symbols
(`native-gfmain-cafile-callback-data-closure-20261003`).

The seven adjacent `GFInitialise` cache pointers at 0x013B837C through
0x013B8394 now have typed zero-filled storage: graphic bank manager,
render state, render manager 2D, input manager, font manager, display
manager, and sound manager. The dedicated gate verifies each COFF
symbol and PE `.data` zero-fill, a missing-provider link, and 16 linked
round-trip cases. The focused symbol index resolves all seven names.
Archive `before-startup-cache-data-20261003` preserves the previous
artifact, index, and gate state. Current GFMain closure: 1,020 sources,
zero duplicates, 589 unresolved symbols
(`native-gfmain-startup-cache-data-closure-20261003`).

Five startup recording switches now have one-byte zero-filled providers:
physical-DVD mode at 0x013B85F2, save/load input events at
0x013B85F5/0x013B85F6, and save/load event packages at
0x013B85F7/0x013B85F8. Their dedicated gate verifies retail PE
zero-fill, exact COFF symbols, provider omission, and 16 linked
independent-write cases. The focused symbol index resolves all five.
Archive `before-startup-recording-flags-20261003` preserves prior
artifact, index, and gate state. Current GFMain closure: 1,021 sources,
zero duplicates, 584 unresolved symbols
(`native-gfmain-startup-recording-flags-closure-20261003`).

Five compiled-resource switches now have one-byte zero-filled providers:
global things and animation events at 0x013B8609/0x013B860A,
definitions, world files, and sound symbols at
0x013B8617-0x013B8619. The gate verifies each COFF symbol and retail
PE zero-fill, missing-provider failure, and 16 linked round-trip cases.
The focused symbol index resolves all five. Archive
`before-compiled-resource-flags-20261003` preserves prior artifact,
index, and gate state. Current GFMain closure: 1,022 sources, zero
duplicates, 579 unresolved symbols
(`native-gfmain-compiled-resource-flags-closure-20261003`).

Eleven initialized startup scalars at 0x01375444-0x01375498 now carry
their retail `.data` defaults: six enabled booleans, half-refresh rate
35, maximum texture size 2048, requested texture bit depth 16,
installer buffer size 0x180000, and buffer count 12. Their gate checks
each COFF symbol and exact initialized PE bytes, rejects provider
omission, and passes 16 linked write/read cases. The focused symbol
index resolves all eleven. Archive
`before-startup-initialized-scalars-20261003` preserves prior artifact,
index, and gate state. Current GFMain closure: 1,023 sources, zero
duplicates, 568 unresolved symbols
(`native-gfmain-startup-initialized-scalars-closure-20261003`).

The virtual-pixel width and height doubles at 0x013961E8 and
0x013961F0 now carry their retail initialized value 65536.0. Their
gate checks exact 16-byte COFF/PE data, a missing-provider link, and
16 linked write/read cases. The focused symbol index resolves both
names. Archive `before-virtual-dimensions-data-20261003` preserves
prior artifact, index, and gate state. Current GFMain closure: 1,024
sources, zero duplicates, 566 unresolved symbols
(`native-gfmain-virtual-dimensions-data-closure-20261003`).

Four `GFGetTime` storage symbols now have zero-filled native providers:
high-resolution mode and initialization state (one byte each), and
timer frequency and start tick (eight bytes each). Their gate verifies
COFF/retail PE zero-fill, a missing-provider link, and 16 linked
round-trip cases. The focused symbol index resolves all four. Archive
`before-gfgettime-data-20261003` preserves prior artifact, index, and
gate state. Current GFMain closure: 1,025 sources, zero duplicates,
562 unresolved symbols (`native-gfmain-gfgettime-data-closure-20261003`).

Five GFMain frame/status globals now have zero-initialized native
providers: render frame, world frame, forced render FPS, cached
language, and save-game launch failure. The gate handles the two raw
initialized zero words and three zero-filled `.data` tail entries,
checking COFF names and sizes, provider omission, and 16 linked
round-trip cases. The focused symbol index resolves all five. Archive
`before-gfmain-frame-status-20261003` preserves prior artifact, index,
and gate state. Current GFMain closure: 1,026 sources, zero duplicates,
557 unresolved symbols (`native-gfmain-frame-status-closure-20261003`).

`CWorld::EventIsWorldEvent` at 0x0049D8C0 now indexes the static
`CMainGameComponent` event-dispatch table and tests its first action
word. A volatile read keeps the table-address addition and memory
comparison in the genuine VC7.1 output, yielding all 23 retail bytes
after the table relocation. A linked fixture passes 16 event types
against populated and empty dispatch rows and confirms the world
object is untouched; provider omission fails. The landed gate, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol index
pass. Archive `before-world-event-is-world-20261003` preserves the
previous catalog, oracle, and manifest state. Current GFMain closure:
1,027 sources, zero duplicates, 556 unresolved symbols
(`native-gfmain-world-event-is-world-closure-20261003`).

`CFrontEndDef::Copy` at 0x00432E50 now uses the genuine class layout
and forwards to its out-of-line assignment operator at 0x00432E60.
An explicit assignment declaration prevents VC7.1 from emitting the
inaccurate implicit body in this unit; the five-byte jump matches
retail after relocation. A linked fixture passes 16 forwarded-identity
cases and rejects provider omission. The staged shared-header audit
covered 124 catalog dependents with zero regressions. The old generic
catalog source and fixture are archived in
`before-frontend-def-copy-20261003`; the corrected oracle label,
catalog `RELOCATION_MATCH` / `PASS`, selected build, landed gate, and
focused index pass. Current GFMain closure: 1,028 sources, zero
duplicates, 556 unresolved symbols, with the genuine assignment
dependency exposed (`native-gfmain-frontend-def-copy-closure-20261003`).

`CResource::operator=` at 0x00431F60 now copies its six non-vtable
fields through ordinary C++. VC7.1 reproduces all 45 retail bytes;
16 linked cases verify the copied fields, preserved vptr, and returned
object, with provider omission. A staged audit of 421 catalog entries
depending on the shared resource header found zero regressions. The
landed gate, catalog `MATCH` / `PASS`, selected build, and focused
symbol index pass. The former naked-byte catalog occupant and fixture,
which mislabeled this address as `CTCPhysicsBase`, are archived in
`before-resource-assignment-20261003`. Current GFMain closure: 1,029
sources, zero duplicates, 556 unresolved symbols
(`native-gfmain-resource-assignment-closure-20261003`).

`CFrontEndDef::operator=` at 0x00432E60 now performs the full resource,
definition-field, packed-flag, subdefinition-map, attract-mode-vector,
and graphic-field assignment. Its 137 VC7.1 bytes match retail after
resolving calls to `CResource::operator=` and the two STLport vector
assignment providers. A linked fixture passes 16 direct-assignment and
`Copy`-forwarding cases with four omission checks; the landed gate,
catalog `RELOCATION_MATCH` / `PASS`, selected build, and focused symbol
index pass. Archive `before-frontend-def-assignment-20261003` preserves
the prior catalog, oracle, and manifest state. Current GFMain closure:
1,030 sources, zero duplicates, 557 unresolved symbols; replacing the
assignment dependency exposed two vector-assignment dependencies
(`native-gfmain-frontend-def-assignment-closure-20261003`).

The `_STL::vector<pair<unsigned long, CDefClassBase::CSubDefInfo>>`
clear helper at 0x0043211D now uses the typed STLport vector layout.
The pair's trivial-destructor trait yields the retail 14-byte null
check and `free` call after relocation. A linked fixture passes 16
null/non-null cases, checks the storage fields stay intact, and rejects
provider omission. The landed gate, catalog `RELOCATION_MATCH` /
`PASS`, selected build, and focused symbol index pass. The former
`CActiveFile::OnReadFinished` catalog label, source, and fixture are
archived in `before-pair-vector-clear-20261003`. Current GFMain
closure: 1,031 sources, zero duplicates, 557 unresolved symbols
(`native-gfmain-pair-vector-clear-closure-20261003`).

Five GFMain singleton/movie-code storage symbols now have native
zero-initialized providers: retired game component, XMV code handle
and reference count, memory allocator, and profile manager. The gate
checks their exact COFF names and four-byte storage against retail
raw-zero or zero-filled `.data`, rejects a missing-provider link, and
passes 16 linked round-trip cases. The focused index resolves all five.
Archive `before-gfmain-singleton-code-data-20261003` preserves prior
artifact, index, and gate state. Current GFMain closure: 1,032 sources,
zero duplicates, 552 unresolved symbols
(`native-gfmain-singleton-code-data-closure-20261003`).

`CBankFileAsync::OpenFileReadWrite` at 0x009D5200 now forwards the
filename and both flags to `CBankFile::OpenFileReadWrite`, then sets
`DisableAsync` at +0x149. Its 36-byte VC7.1 body matches retail after
the base-call relocation. A linked fixture passes 16 argument, order,
and flag cases, and provider omission fails. The landed gate, catalog
`RELOCATION_MATCH` / `PASS`, selected build, and focused symbol index
pass. Archive `before-bank-file-async-open-rw-20261003` preserves the
prior catalog, oracle, and manifest state. Current GFMain closure:
1,033 sources, zero duplicates, 551 unresolved symbols
(`native-gfmain-bank-file-async-open-rw-closure-20261003`).

Both font-bank entry-removal overrides are now native providers:
`CFontDataBank::NotifyEntryRemoved` at 0x00A635F0 and
`CStreamingFontDataBank::NotifyEntryRemoved` at 0x00A5F0B0. Each typed
C++ body exactly matches its 63-byte retail routine, including virtual
vector-deleting destructor dispatch and reloading `Fonts` storage after
that call. The linked fixture passes 32 bank/case combinations,
including null entries and destructor-induced storage replacement;
three omitted-provider links fail as expected. Both selected builds,
catalog `MATCH` / `PASS` grades, focused symbol index, and landed gate
pass. Archive `before-font-notify-removed-20261003` preserves prior
catalog, oracle, manifest, and index state. Current GFMain closure:
1,035 sources, zero duplicates, 549 unresolved symbols
(`native-gfmain-font-notify-removed-closure-20261003`).

Both font-bank `GetLastModifiedTime` overrides now return the retail
default date (year/month/day/hour/minute 1, second/millisecond 0).
Each 32-byte body matches retail after relocating the shared
`CDateAndTime` constructor call. A linked fixture passes 32 cases,
checks the hidden return object and unchanged bank/name storage, and
rejects three omitted-provider links. Both catalog
`RELOCATION_MATCH` / `PASS` grades, selected builds, focused symbol
index, and landed gate pass. Archive
`before-font-last-modified-20261003` preserves prior catalog, oracle,
manifest, and index state. Current GFMain closure: 1,037 sources,
zero duplicates, 547 unresolved symbols
(`native-gfmain-font-last-modified-closure-20261003`).

`CBankFileAsync::OpenReadWrite` at 0x009D51D0 now forwards the
character-string filename and both flags to `CBankFile::OpenReadWrite`,
then sets `DisableAsync` at +0x149. Its 36-byte body matches retail
after the base-call relocation. A linked fixture passes 16 argument,
ordering, and flag cases, and the omitted-provider link fails. The
landed gate, catalog `RELOCATION_MATCH` / `PASS`, selected build, and
focused symbol index pass. Archive `before-bank-async-open-rw-20261003`
preserves prior catalog, oracle, and manifest state. Current GFMain
closure: 1,038 sources, zero duplicates, 546 unresolved symbols
(`native-gfmain-bank-async-open-rw-closure-20261003`).

The async-bank failure-handling mode at 0x013CA868 now has a typed,
zero-initialized four-byte native provider. Its gate checks the exact
COFF name and storage against retail `.data`, passes 16 linked
round-trip cases, and rejects a missing-provider link. The focused
symbol index resolves it. Archive
`before-bank-async-failure-mode-20261003` preserves prior artifact,
index, and gate state. Current GFMain closure: 1,039 sources, zero
duplicates, 545 unresolved symbols
(`native-gfmain-bank-async-failure-mode-closure-20261003`).

`CFontDataBank::PrepareDataForInclusion` at 0x00A63420 now has a
genuine typed empty member body. The VC7.1 compiler emits the exact
32-byte retail cleanup for its by-value counted-pointer argument,
including the control-block release and final `operator delete` call.
A linked fixture passes 16 null/owned cases and rejects the omitted
provider. Catalog `RELOCATION_MATCH` / `PASS`, selected build, focused
symbol index, and landed gate pass. The previous free-function model,
independent behavior test, and stale `CWorldMap` oracle label are
corrected; archive `before-font-prepare-empty-20261003` preserves the
old artifacts. Current GFMain closure: 1,040 sources, zero duplicates,
544 unresolved symbols
(`native-gfmain-font-prepare-empty-closure-20261003`).

`CStreamingFontDataBank::PrepareDataForInclusion` at 0x00A5F0F0
now uses the same genuine typed empty member body. Its 32-byte
counted-pointer cleanup matches retail after relocation, and the
linked fixture passes 16 null/owned cases plus provider omission.
Catalog `RELOCATION_MATCH` / `PASS`, selected build, focused symbol
index, and landed gate pass. The old standalone type model, behavior
test, and stale `CWorldMap` oracle label are corrected; archive
`before-streaming-font-prepare-empty-20261003` preserves the old
artifacts. Current GFMain closure: 1,040 sources, zero duplicates,
543 unresolved symbols
(`native-gfmain-streaming-font-prepare-empty-closure-20261003`).

The already-landed `CCompressorZlib::Compress` and `Decompress`
providers were added to the GFMain closure after their existing gate
reconfirmed both retail bodies, nine linked scenarios, 72 comparative
checks, and omission links. They resolve both wrapper symbols and
expose `InternalCompress` as the next dependency. No source or catalog
change was needed. Current GFMain closure: 1,042 sources, zero
duplicates, 542 unresolved symbols
(`native-gfmain-zlib-wrappers-closure-20261003`).

GFMain's four address-only references for `AllowDataGeneration`,
`PlayIntroMovies`, `UseCompiledSoundSymbols`, and `UseCompiledDefs`
now use the typed storage providers already in the closure. The full
GFMain gate retains its accepted 3,952-byte near match with the same
two differences and passes 391 relocation checks, 58 simulated
scenarios, and five callback checks. The oracle resolver now recognizes
named address-suffixed globals. Archive
`before-gfmain-data-aliases-20261003` preserves the prior source and
resolver. Current GFMain closure: 1,042 sources, zero duplicates, 538
unresolved symbols (`native-gfmain-data-aliases-closure-20261003`).

Eleven GFMain startup scalars at 0x01375447 through 0x01375754 now
have typed providers using their retail initial values: four booleans,
two bytes, and five longs (26 bytes total). The gate checks every COFF
name and raw `.data` byte, passes 16 linked round-trip cases, and
rejects a missing-provider link. The focused symbol index resolves the
source. Archive `before-gfmain-startup-raw-scalars-20261003` preserves
prior index, artifact, and gate state. Current GFMain closure: 1,043
sources, zero duplicates, 527 unresolved symbols
(`native-gfmain-startup-raw-scalars-closure-20261003`).

Sixteen GFMain runtime settings at 0x013B8604 through 0x013B864F
now have native zero-initialized providers: fifteen booleans and one
long, totaling 19 bytes. The gate checks every COFF name against
retail zero-filled `.data`, passes 16 linked round-trip cases, and
rejects a missing-provider link. The focused symbol index resolves
the source. Archive `before-gfmain-runtime-flags-20261003` preserves
prior index, artifact, and gate state. Current GFMain closure: 1,044
sources, zero duplicates, 511 unresolved symbols
(`native-gfmain-runtime-flags-closure-20261003`).

Pause handoff for 2026-10-03: the user is shutting down the computer.
Resume native, non-Lua work from the closure above; do not rerun the
successful one-shot `land_*.py` scripts. `CCompressorZlib`'s existing
wrapper providers expose `InternalCompress` at 0x00A3AA40 as a new
dependency. A staged, unpromoted
`CParticleEnvironmentInteraction::GetWindVelocity` candidate is in
`work/codex-native-startup/particle-wind-velocity-20261003`; its
world/environment behavior is typed, but the floating-point machine
code does not yet meet parity, so it has not been landed. The full
GFMain gate still passes at its accepted 3,952-byte near match, 391
relocations, 58 scenarios, and five callback checks. No native
reconstruction process needs to remain running between sessions.
