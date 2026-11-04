# Native reconstruction resumed — 2026-09-30

The user resumed native recreation and excluded Lua work. The shared checkout
still contains unfinished Lua changes; this session did not modify them.

The recovered WinMain startup probe was rerun with a fresh output directory:
`python tools/decomp_pipeline/play_seam.py --entry startup --out
work/codex-native-startup-20260930`. It linked two genuine sources, reported
one function trap and five data blocks, and exited 86 at `@GFMain@12`.
GFMain 0x00402510 remains the first native startup implementation target.

Read-only retail disassembly and decompilation were compared with the existing
GFMain ledger. The ledger covers all 257 validated engine calls at their exact
call sites and targets. Four direct calls are outside that ledger: `operator
delete` at 0x00402652 and 0x00403002, `operator new` at 0x00402E50, and
`__security_check_cookie` at 0x00403472. `extract_gfmain_calls.py
--check-ledger` now audits all 261 direct calls against retail without
overwriting the curated ledger. Raw export now requires `--output` and refuses
to replace the curated file. Both modes use Capstone because GNU objdump loses
instruction alignment in GFMain and sees only the 257 tracked calls. The
check passes on the installed retail executable, raw export has 261 rows,
and Python compilation passes.

The downstream font startup path was traced from retail evidence. System
manager Initialise 0x009A6610 constructs CFontManager 0x009E2E30 if either
font bank handle is present. The manager constructs static and streaming font
banks, opens supplied handles, creates symbol maps, and creates eight vertex
buffers. Its direct constructor dependencies include CFontDataBank 0x00A63630,
CStreamingFontDataBank 0x00A5F290, two counted-pointer Reset instantiations,
vector resize, and vertex-buffer Initialise. Read-only C and disassembly are
under `work/codex-native-fontctor/`. These are evidence, not landed bodies.

The bank header had flattened the async layer into `CBankFile` and made game
text and particle banks inherit the wrong constructor. Retail disassembly
shows both constructors call 0x009D5F80. The header now models CBankFile at
0x110 and CBankFileAsync at 0x164; game text and particle banks inherit the
async type. The existing game-text constructor and `GetPGroupEntry` remain
RELOCATION_MATCH with passing fixtures. Their object relocations now name
`CBankFileAsync::CBankFileAsync` and `CBankFileAsync::AsyncUpdatePending`.
The mislabeled manifest entries at 0x009D5F80 and 0x009D68B0 were corrected.

The real async constructor 0x009D5F80 was landed as genuine C++ using typed
counted pointers, STLport sets and list, and a Win32 critical section. It is
NEAR_MATCH: 53/53 instructions and 94.34% normalized opcode similarity. A
candidate-linked fixture checks base-constructor order, all three ownership
slots, both set sentinels, the disposal list, flags, and critical-section
initialization; it passes. A `DisableAsync(true)` mutation stays near-match
but fails the fixture. Header dependents showed zero regressions across 2,199
entries, and boot leaves were repaired to 123/123 after adding fixture-only
destructors. The full visual bootstrap gate passed for this constructor change.

The next async method, `AsyncUpdatePending` 0x009D68B0, was recovered from
its 16-byte body: it processes the disposal list, then the restart queue.
It is RELOCATION_MATCH with a candidate-linked call-order fixture. Reversing
the two calls still receives a relocation match, but the fixture fails; this
guards a behavior difference hidden by relocation masking. Its second callee
0x009D5A90 walks ReadsToRestart and has a provisional name pending its own
reconstruction. The old music-manager attribution there is contradicted by
the function body and its async-bank receiver. Final `gate_all.py --bootstrap`
passes: 123/123 boot leaves, component ABI, 0 regressions across 2,200 header
dependents, 0 missing bootstrap references, 0 unresolved ring symbols, and the
visual bootstrap. The startup probe still traps at GFMain; no native frontend
screen is claimed.

The Play-only null font manager is downstream of the missing real GFMain
startup. Next: recover GFMain 0x00402510 from its decompilation and call
ledger; keep font constructors behind the real bank and font-manager paths.

Font manager follow-up: a scratch `CFontManager` constructor 0x009E2E30
compiles with the bank, symbol-map, and vertex-buffer model. Reloading each
bank from its counted pointer after virtual calls reproduces retail register
allocation, giving a 93.21% opcode-similar near match to the 449-byte retail
body. A hand-owned 0x34-byte `CVertexBufferWin32` header now carries its
constructor, typed Initialise signature, and the observed field offsets. A
candidate-linked fixture exercises the constructor with empty and populated
bank handles, checking both banks, both symbol maps, the streaming entry,
and all eight vertex buffers. It passes, and the constructor is landed as
NEAR_MATCH. Its dependencies have not yet been promoted into the executable
startup seam, so GFMain still traps. The font bank header now models
`CFontDataBank : CBankFile` and
`CStreamingFontDataBank : CBankFileAsync`, matching their constructor calls
and keeping their field offsets at +0x110 and +0x164, respectively. The
header-dependent gate reports zero regressions across 2,201 catalog entries.

The retail `GetFreeVertexBuffer` 0x009E2CE0 advances `CurrentVB` modulo the
number of buffer slots and returns the selected pointer. The reconstructed
C++ is a 31-byte MATCH. A candidate-linked fixture checks nine consecutive
selections and unsigned counter wraparound. It passes; the function was
landed in the compiled catalog and manifest. Full native gates pass: 123/123
boot leaves, component ABI, zero regressions across 2,201 header dependents,
zero missing bootstrap references, zero unresolved ring symbols, and visual
bootstrap. The log is `work/codex-native-fontctor/getfree-gates.log`.

`CFontManager::ReleaseFont` 0x009E2B40 is now a relocation match. It checks
the static symbol map first, falls back to the streaming map, resolves the
matching entry, and calls `Unload` only if `CanUnload` allows it. Its
candidate-linked fixture covers both banks, unknown names, and denied unload;
all pass. This work exposed two catalog aliases misidentified as inventory
ability lookups: the non-const `GetFontInfo` methods at 0x00A635D0 and
0x00A5F0A0. Both now have typed C++ sources with byte-for-byte parity and
candidate-linked index fixtures. Their catalog, oracle, and manifest names
were corrected. The source methods were already compiled under misleading
filenames; their files and tests now carry the actual font-bank names.

Font constructor dependency work: the static and streaming
`CCountedPointer::Reset` methods at 0x009E3090 and 0x009E3100 are both
RELOCATION_MATCH. Candidate-linked tests cover old owner release, null reset,
new ownership blocks, and a shared old owner. The symbol-map constructor at
0x009E5200 and its counted-pointer reset at 0x009B1F50 were already landed;
their stale manifest identities were corrected. The font vertex-buffer
vector `resize` at 0x009E3580 is RELOCATION_MATCH and has a candidate-linked
fixture for shrink, grow, and equal-size calls.

The static and streaming font-bank constructors (0x00A63630 and 0x00A5F290)
are now RELOCATION_MATCH. The former derives from `CBankFile`, the latter
from `CBankFileAsync`. A hand-owned state-block header models their embedded
virtual state blocks, preserving sizes 0x10 and 0x14 and placement at +0x110
and +0x164. Candidate-linked fixtures verify the base constructor, both
vtable slots, empty font vectors, filename assignment, and the streaming
entry slot. Both pass. The full native gate is recorded in
`work/codex-native-fontctor/font-banks-final-gates.log`.

`CVertexBufferWin32::Initialise` 0x00A63150 is now RELOCATION_MATCH. It calls
the system manager's D3D9 device `CreateVertexBuffer` with the retail byte
size, flags, vertex type and pool, writes the buffer and stride metadata, and
copies four unit scale values and four zero words. The eight-value float
array reproduces the retail stack stores and `rep movsd`. A candidate-linked
fixture checks the D3D9 call arguments, all output fields, and success and
failure results; it passes. This completes the font-manager constructor's
directly called bank, symbol-map, vector, and vertex-buffer dependency set
at the compiled catalog level. Their deeper dependencies and integration
into the startup seam remain separate work.

The font-bank filename path now has a reconstructed `CCharString::operator=`
from `const char*` at 0x0099EFE0. It is RELOCATION_MATCH; the candidate-linked
fixture checks empty, nonempty, replacement, and null assignments, including
the new string-data reference count. The deeper `CBankFile` constructor at
0x009CD480 still needs typed reconstruction.

Two helpers called by that base constructor are now recovered. The four
`CPackedUIntArray` subobjects use the 16-byte constructor at 0x00A629C0;
its C++ is an exact byte match and its candidate-linked fixture checks every
initialized field and unchanged padding. The embedded retail
`CChecksumCacheFile` is 0x18 bytes with a 0x34-byte map header node. Its
constructor at 0x00A60C90 is RELOCATION_MATCH and the fixture checks map
sentinels, flags, and buffer-size transformation across four inputs. That
transformation is literally `(requested + 8) & ~8` in retail, including its
unusual behavior for small values.

The retail `CBankFile` object now has a typed 0x110-byte layout: three
four-word arrays, four packed arrays, three STLport maps, a vector map, block
order, two strings, and the checksum cache. Its 0x009CD480 constructor is
NEAR_MATCH (92/92 instructions, 97.83% opcode similarity); the only
non-relocation scheduling difference is a `lea` moved across three zero
stores. A candidate-linked fixture checks its nested constructor calls,
allocated map sentinels, offsets, and final size, alignment and format
defaults. The typed member constructors and destructors required updates to
older fixture boundaries: three bank-file fixtures now link their member
constructor stubs, the window-title fixture counts the actual member
destructions, and Stage 3 links the recovered packed-array constructor into
its unopened text-bank boundary. The full bootstrap passes, including the
Stage 3 startup executable and the visual checkpoint. The aggregate native
gate also passes: 123/123 boot leaves, component ABI, zero regressions across
2,215 dependent catalog entries, zero missing bootstrap references, zero
unresolved ring symbols, and visual bootstrap. Log:
`work/codex-native-fontctor/bankfile-final-gates.log`.

The two font bank state-block `GetDefault` methods at 0x00A632D0 and
0x00A5EE20 were retyped against a shared four-byte `CBankStateBlock` base
(matching the donor PDB), with the static and streaming concrete blocks still
0x10 and 0x14 bytes. Both compiled methods remain exact 22-byte and 25-byte
MATCHes. Their fixtures now link the compiled methods and check the output
fields and untouched bytes, instead of reimplementing the method in the
test. The startup probe still traps at GFMain, and the isolated system probe
still traps at the VC7.1 CPUID boundary in GFInitVectorMath; the next native
ownership path for font startup includes bank virtual I/O and destruction.
The aggregate gate after this retyping passes: 123/123 boot leaves, component
ABI, zero regressions across 2,216 dependent catalog entries, clean
references and ring link, and visual bootstrap. Log:
`work/codex-native-fontctor/state-default-final-gates.log`.

The adjacent `CreateCRC` virtual methods at 0x00A63890 (static font state)
and 0x00A5F6A0 (streaming font state) now use the hand-owned concrete state
layout and the actual two-stack-argument, receiver-preserving call shape of
the common CRC helper at 0x00A00950. Each candidate remains a 12-byte
RELOCATION_MATCH. The candidate-linked fixtures check the receiver, state
address, respective 12/16-byte length, and returned CRC surrogate. The full
bootstrap passes after the vtable declarations changed. The helper itself is
still unlanded: a scratch zero-seeded table implementation passes an
independent bitwise CRC fixture, but its 69-byte build scores 85.19% opcode
similarity, below the near-match threshold. Scratch source and comparison
remain under `work/codex-native-fontctor/crc_calc_*`.
The aggregate gate after the two `CreateCRC` retypings passes: 123/123 boot
leaves, component ABI, zero regressions across 2,218 dependent catalog
entries, clean references and ring link, and visual bootstrap. Log:
`work/codex-native-fontctor/state-crc-final-gates.log`.

The last two members in the static and streaming state-block vtables, `Set`
at 0x00A632F0 and 0x00A5EE40, have been retyped as protected virtual calls
over `CBankStateBlock const*`. Their old catalog tests only printed pass
markers. New candidate-linked fixtures check all copied fields, untouched
padding and vtable, and unchanged source storage. Both bodies remain exact
25-byte and 31-byte MATCHes. Together with `GetDefault` and `CreateCRC`, the
four-entry vtable shape for each font state block is now explicitly modeled.
The aggregate gate is running after this last header change.
It passes: 123/123 boot leaves, component ABI, zero regressions across
2,220 dependent catalog entries, clean references and ring link, and visual
bootstrap. Log: `work/codex-native-fontctor/state-set-final-gates.log`.

The static font bank's `CloseFonts` at 0x00A633A0 is now reconstructed and
landed as a 123-byte RELOCATION_MATCH. Retail walks the cached font entries
backwards; a loaded entry gets `CanUnload` and `Unload` calls before its
deleting destructor, and every non-null entry is deleted and its slot
cleared. The filename is assigned an empty string after the vector is empty.
The candidate-linked fixture checks reverse call order, a null slot, loaded
and unloaded entries, deletion, slot clearing, the filename receiver, and an
empty second call. Preserving the entry pointer as a local value, rather than
a reference to the vector slot, reproduced the retail register allocation.
The aggregate native gate is running after the catalog/header promotion.
It passed with 123/123 boot leaves, component ABI, zero regressions across
2,221 dependent entries, clean references and ring link, and visual
bootstrap. Log: `work/codex-native-fontctor/close-fonts-final-gates.log`.

The streaming font bank's `CloseFonts` at 0x00A5F2F0 is also landed as a
161-byte RELOCATION_MATCH. It traverses entries forward, conditionally calls
their load/unload methods, deletes each non-null entry, then clears the vector
and filename. Its candidate-linked fixture checks call order, a null slot,
unloaded and loaded entries, vector finish, untouched slot values, the
filename receiver, and an empty second call. This differs intentionally from
the static bank's reverse traversal and slot clearing. The final aggregate
gate covering both methods is running.
It passed: 123/123 boot leaves, component ABI, zero regressions across
2,222 dependent catalog entries, clean references and ring link, and visual
bootstrap. Log: `work/codex-native-fontctor/both-close-fonts-final-gates.log`.

Teardown evidence for the next boundary: the static bank vtable at
0x0129DA84 points to its 83-byte deleting destructor at 0x00A63B30; the
streaming bank vtable at 0x0129D5CC points to a 30-byte deleting wrapper at
0x00A5FA30, which calls the 126-byte ordinary destructor at 0x00A5F3A0.
Those three bodies are currently mislabeled in the manifest as unrelated
classes; the two unlanded destructor rows have been corrected to their font
bank identities. A scratch compile of typed destructor bodies was 84 bytes
for the static deleting destructor and 127 bytes for the streaming ordinary
destructor. The static destructor remains a later teardown target.

The streaming teardown is now promoted. `CStreamingFontDataBank`'s ordinary
destructor at 0x00A5F3A0 is a candidate-linked NEAR_MATCH: 126 retail versus
127 built bytes, 31 versus 32 instructions, and 92.06% normalized opcode
similarity. The fixture checks `CloseFonts`, final and shared counted-owner
paths, state-block destruction, and async/base destruction order; it also
deletes a heap bank through the virtual deleting wrapper. That wrapper at
0x00A5FA30 is a RELOCATION_MATCH from the same typed C++ source. The former
generic `CLandscapeBackgroundPatch` placeholder source and print-only test
were removed from the catalog; catalog, oracle, manifest, and artifact-index
identities now point to the real streaming bank destructor. The aggregate
native gate is running after this replacement.
It passes: 123/123 boot leaves, component ABI, zero regressions across
2,224 dependent catalog entries, clean references and ring link, and visual
bootstrap. Log: `work/codex-native-fontctor/streaming-dtor-final-gates.log`.

`CVertexBufferWin32`'s 43-byte deleting destructor at 0x009E2890 is now
retyped from a wrong `CThingTrackNode` manifest identity and landed as a
RELOCATION_MATCH. The typed destructor calls `Uninitialise`; its
candidate-linked fixture checks that call before noncopyable-base teardown
for both direct and heap destruction. The deeper `Uninitialise` routine at
0x00A63200 is now a 53-byte RELOCATION_MATCH. It skips an empty buffer;
otherwise it obtains the system manager, calls the render manager with the
buffer and 1.0f, releases the COM buffer, and nulls the field. Its
candidate-linked fixture verifies the empty case, call order, arguments, and
release. The three-byte render-manager method at 0x00A0AA70 is an exact
`ret 8` match with a typed two-argument fixture. `OnVertexBufferRelease` is a
provisional name because the original name is absent from current PDB evidence.
Both entries now have typed catalog, oracle, manifest, and artifact-index rows.
The old print-only generic callback fixture was removed. The aggregate gate
passes after this promotion: 123/123 boot leaves, component ABI, zero
regressions across 2,227 dependent catalog entries, clean references and
ring link, and visual bootstrap. Log:
`work/codex-native-fontctor/vertex-buffer-release-final-gates.log`.

The font bank virtual `Close` methods at 0x00A635C0 and 0x00A5F420 are now
typed native C++ bodies. Each calls its own `CloseFonts` then tail-calls
`CBankFile::Close`, matching the 16-byte retail instruction shape and
relocations. Candidate-linked fixtures verify call order for each bank.
The `CBankFile` header now declares the intervening virtual methods so
`Close` occupies retail vtable slot 5, as observed in `CFontManager`'s
destructor. The former generic sources and self-implementing tests are
removed. The aggregate gate passes: 123/123 boot leaves, component ABI, zero
regressions across 2,229 dependent catalog entries, clean references and
ring link, and visual bootstrap. The `CBankFile` mock vtable slots were
provided in shared fixture support and the Stage 3 window-title boundary.
Log: `work/codex-native-fontctor/font-bank-close-final-gates.log`.

`CFontManager`'s ordinary destructor at 0x009E2D00 is now a typed candidate.
It closes and releases the static bank, deletes each vertex buffer, then lets
the vector and remaining counted fields unwind. A candidate-linked fixture
checks the close receiver and exact `CRVVSYT` teardown order. The generated
body is 296/296 retail bytes, has the same instruction count, and has 98.28%
normalized opcode similarity (NEAR_MATCH). The same C++ source generates the
deleting wrapper at 0x009E3560: 30/30 bytes and identical normalized
instructions (RELOCATION_MATCH). Its former `CLandscapeBackgroundPatch`
placeholder identity, source, and print-only test were removed. The aggregate
gate passes after the destructor promotion: 123/123 boot leaves, component
ABI, zero regressions across 2,231 dependent catalog entries, clean references
and ring link, and visual bootstrap. Mock constructor fixtures gained an empty
virtual destructor to complete their test vtables. Log:
`work/codex-native-fontctor/font-manager-dtor-final-gates.log`.

The five-byte virtual `CBankFile::Close` entry at 0x009CE360 now has a typed
forwarder and candidate-linked receiver fixture. Retail tail-jumps to
0x009CD850, a larger close implementation whose current manifest label
(`CDefStringTable::Destructor`) is contradicted by this dispatch. The
forwarder's target name `CloseBankFileImpl` remains provisional pending
recovery of that body. Its object is a relocation match to retail's jump.

The static `CFontDataBank` deleting destructor at 0x00A63B30 is also promoted.
An empty state-block destructor definition visible in its translation unit
lets VC7.1 inline the state vtable reset, matching the retail schedule. The
result is 83/83 bytes, the same instruction count, and 91.30% normalized
opcode similarity (NEAR_MATCH). Its candidate-linked fixture checks
`CloseFonts` before base destruction for direct and heap deletion. The
aggregate gate passes on these two landings: 123/123 boot leaves, component
ABI, zero regressions across 2,233 dependent catalog entries, clean references
and ring link, and visual bootstrap. Log:
`work/codex-native-fontctor/bank-close-static-dtor-final-gates.log`.

The same static bank translation unit emits
`CFontBankStateBlock`'s deleting destructor at 0x00A63320. It is a 31/31-byte
NEAR_MATCH (90.91% normalized opcode similarity); the linked fixture now
allocates and deletes the state block as well as the bank. The manifest's
unknown vtable identity was replaced with the typed destructor identity.
The static bank's ordinary destructor at 0x00A63680 was also reidentified
from an unrelated `CTCAssassinRush::FinishMove` row, but its 87.5% scratch
similarity is below the near-match threshold, so that body remains unlanded.
The aggregate gate passes on the state-block wrapper: 123/123 boot leaves,
component ABI, zero regressions across 2,234 dependent catalog entries,
clean references and ring link, and visual bootstrap. Log:
`work/codex-native-fontctor/state-block-dtor-final-gates.log`.

The streaming bank destructor translation unit now keeps the empty
`CStreamingFontBankStateBlock` destructor visible to VC7.1. Its ordinary bank
destructor at 0x00A5F3A0 improves from 127/126 bytes and 92.06% to 126/126
bytes and 93.55% normalized opcode similarity, with equal instruction count.
Its candidate-linked fixture still checks close, final/shared entry release,
and async/base destruction order, and now deletes a streaming state block.
That translation unit emits the streaming state block deleting destructor at
0x00A5EE70: 31/31 bytes and 90.91% normalized similarity (NEAR_MATCH).
The existing bank deleting wrapper at 0x00A5FA30 remains a 30/30-byte
RELOCATION_MATCH from the same source. The aggregate gate is pending on this
streaming revision and new wrapper catalog entry. That gate passes:
123/123 boot leaves, component ABI, zero regressions across 2,235 dependent
entries, clean references and ring link, and visual bootstrap. Log:
`work/codex-native-fontctor/streaming-state-dtor-final-gates.log`.

Relocation-aware verification supersedes the earlier similarity-only grades.
Masking exactly the COFF relocation words establishes RELOCATION_MATCH for
both static bank destructors (0x00A63680, 0x00A63B30), both state block deleting
destructors (0x00A63320, 0x00A5EE70), and the revised streaming bank destructor
and wrapper (0x00A5F3A0, 0x00A5FA30). The ordinary static bank destructor is now
cataloged and linked to the same direct/heap destruction fixture.

The manager destructor's remaining two byte differences were the order of
clearing Info and Data. Clearing Info first reproduces retail, yielding a
296-byte RELOCATION_MATCH at 0x009E2D00; its 30-byte wrapper remains a
RELOCATION_MATCH. All eight selected candidate builds pass. Reproducible
comparison: `work/codex-native-fontctor/check_font_teardown_relocations.py`;
results: `work/codex-native-fontctor/font-teardown-relocation-proof.log`.

### Font teardown relocation-target correction

The earlier masked-byte comparison did not establish the identity of the
relocation targets. Auditing the retail vtable writes exposed a missing
inheritance relationship: both font state blocks derive from the polymorphic
four-byte `CBankStateBlock`. Their destruction must restore base vtable
0x0129C82C. The shared header now represents that relationship, and the proof
script asserts the base-vtable symbol at every matching retail write. It also
rejects symbol-selection fallback instead of silently comparing another body.

The two state deleting entries now have standalone typed ABI adapters with
candidate-linked fixtures for flags 0, 1, and 2, payload preservation, return
identity, and conditional deallocation. All eight teardown byte comparisons
pass with the corrected base-vtable relocations. Seventeen selected constructor,
destructor, and state-method fixtures pass after updating constructor fixture
dependencies for the full typed base. The full corrected gate is running in
`work/codex-native-fontctor/corrected-font-base-gates.log`; the earlier
`exact-font-teardown-final-gates.log` run was stopped and is not a passing gate.

Tracing the next cleanup target, 0x009CD850, shows four retail 12-byte arrays
at +0x08, +0x14, +0x20, and +0x2C. The current shared bank header still uses
three donor-sized 16-byte placeholders in that region; correction is pending.
The +0x2C array owns update-data pointers, whose +0x0C string small-vector clear
at 0x009D2110 has a 69-byte scratch relocation match. A linked scratch fixture
checks reverse element destruction using the allocation cookie, array deletion,
null handling, and repeat clear. Including `<new>` is necessary for VC7.1 to
select array delete rather than scalar delete; target identity matters here too.

The corrected font-base aggregate gate is complete: 123/123 boot leaves,
component ABI, zero regressions across 2,236 dependent catalog entries, clean
references and ring link, and visual bootstrap all pass.

### Bank close and owned cleanup

The bank array correction is now in the shared header: Symbols +0x08,
Checksums +0x14, 12-byte CRuntimeData elements +0x20, and UpdateData pointers
+0x2C. The constructor remains a 402-byte relocation match. The recovered
cleanup batch is cataloged with candidate-linked tests:

- 0x009CD850 `CloseBankFileImpl`: 538 bytes / 195 instructions. Clears owned
  update records, runtime storage, symbols, bank ownership, three string maps,
  the CRC map's logical range, and four packed arrays, then resets Size.
- 0x009D2110 `CSmallVector<CCharString,8>::Clear`: 69 bytes; the older coverage
  row's 66-byte length omitted the final instructions. Tests use the allocation
  cookie and confirm reverse element destruction and array deletion.
- 0x009D3FB0 `ReleaseBankSymbols`: 95 bytes, with the existing typed string-vector
  erase at 0x0043336A. The adapter name describes the explicit cleanup operation.
- 0x00A629D0 `ClearPackedBankArray`: 29 bytes. Replaces a standalone duplicate
  class / delete-stub fixture with the shared layout and true array-delete ABI.
- 0x00579435 string-key map `_M_erase`: 53 bytes. Replaces the incorrectly named
  tree-walk placeholder with the typed recursive tree implementation. The linked
  fixture verifies right subtree, current node, and left subtree destruction.

All five have exact relocation-masked bytes and individually checked call
targets; 11 selected builds covering this batch and affected constructors and
destructors pass. Proof: `work/codex-native-fontctor/landed-bank-cleanup-proof.log`.
Superseded packed/tree source and fixture copies are preserved under
`work/codex-native-fontctor/superseded-cleanup/`. The main cleanup fixture also
checks final/shared ownership, null records, tree-header reset, retained CRC
capacity, repeat close, and preservation of unrelated bank settings.

The aggregate gate for this batch is running in
`work/codex-native-fontctor/bank-cleanup-final-gates.log`.

A connected scratch fixture now links nine recovered objects, from the
0x009CE360 Close forwarder through bank cleanup, small-vector clear, symbol
release, packed reset, tree erase, string-vector erase, string-range destruction,
and the CCharString destructor. Only string-storage release and counted-owner
callbacks are fixture boundaries. It passes the complete destruction order and
repeat-close checks: `work/codex-native-fontctor/check_bank_cleanup_integration.py`.

The following destructor batch is still in scratch: CBankFile 0x009CE180
(473 bytes), CChecksumCacheFile 0x00A60CE0 (82 bytes), and CPackedUIntArray
0x00A62AC0 (29 bytes). All have relocation-masked matches. The bank/cache linked
fixtures pass member-order and empty/nonempty cache checks. Bank-specific map
ownership supplies the tree destruction missing from the broader shared map
model; these header changes have not yet been promoted.

The bank-cleanup aggregate gate subsequently passed all checks, including zero
regressions across 2,241 dependent entries and visual bootstrap. The destructor
batch is now promoted, with two additional checksum helpers: `Close` 0x00A60C30
(84 bytes) and wide-string cache tree erase 0x00A60A00 (59 bytes). `Close` saves
and clears only when the filename is nonempty, then resets the filename; its
identity agrees with the donor `CChecksumCacheFile::Close` symbol and retail
callers. All five candidates have checked relocation targets (38 total) and
passing linked fixtures. Proof: `work/codex-native-fontctor/landed-bank-destructor-proof.log`.

`FableBankOwnedMap` supplies bank/cache-specific destruction while preserving
the broader shared map model for its other users. `CPackedUIntArray` and
`CChecksumCacheFile` now declare their native destructors. Constructor fixtures
provide the additional external cleanup dependencies; unexpected nonempty maps
abort in those stubs. The definition loader's existing `Open` and `GetChecksum`
cache method declarations were restored after a selected build exposed their
absence. Nineteen selected builds pass, and the earlier connected cleanup
fixture still passes. The next aggregate gate is running in
`work/codex-native-fontctor/bank-destructor-final-gates.log`.

That destructor gate passed: 123/123 leaves, component ABI, zero regressions
across 2,246 dependent entries, references, ring, and visual bootstrap.

### Checksum save and typed file output

The bank deleting destructor at 0x009D47A0 now uses the CBankFile destructor
translation unit and a direct/heap destruction fixture, replacing the unrelated
landscape stub. It matches retail's 30 bytes.

The file-output/cache batch is cataloged: CFileDataOutputStream constructor
0x00994400 (48 bytes), destructor 0x00994430 (31), SetPosition 0x00994450 (8),
GetPosition 0x00994460 (8), Close 0x00994470 (1), Write 0x00994480 (23),
CDataOutputStream::WriteDateAndTime 0x00994180 (163), and checksum Save
0x00A60AA0 (388). Thirteen selected builds plus both bank destructor aliases
pass. The date writer emits a 12-byte record with four byte-sized components
and two word-sized components; its fixture checks intentional narrowing.

Output streams now derive from the shared CDataStream interface, whose vtable
has three pure slots after the destructor. CDataOutputStream's destructor is
implicit: an explicit empty destructor produced the wrong base-vtable target
during NewLine cleanup despite identical masked bytes. The revised file-stream
destructor keeps its own vtable until NewLine destruction, then restores
CDataStream's 0x0129A69C table. CAFile is marked novtable to reproduce retail's
omitted intermediate vptr stores. Cache entries now name their evidenced
TimeStamp and Checksum fields (32 bytes). Per-call relocation targets are checked
in `work/codex-native-fontctor/landed-file-output-proof.log`.

The full gate for this batch is running in
`work/codex-native-fontctor/file-output-cache-save-final-gates.log`.

Two further scratch string helpers match and pass: WriteNullTerminatedString
0x00995AE0 (48 bytes) and CWideString::ToCharString 0x0099B2C0 (177). The latter
preserves retail's low-byte narrowing, including embedded zero bytes before the
char-string constructor sees the buffer. A connected save fixture links eleven
objects and verifies the exact serialized header, names, timestamps, CRCs,
missing-file skipping, and repeat-save behavior. It uses the WriteSLONG provider
emitted by the timestamp TU after confirming that provider's 15-byte retail
match, avoiding a duplicate strong definition from a second object. Disk I/O is
recorded in memory; the fixture writes no real cache file. Reproducer:
`work/codex-native-fontctor/check_cache_save_integration.py`.

The file-output/cache full gate completed with ALL GATES PASS: 123/123 leaves,
component ABI, zero regressions across 2,254 header dependents, references, ring,
and the visual bootstrap checkpoint. This remains the authored bootstrap check,
not a complete native frontend.

CWideString::ToCharString 0x0099B2C0 is now cataloged. The existing
WriteNullTerminatedString 0x00995AE0 candidate now has stronger embedded-NUL and
allocated-empty coverage, current manifest/oracle metadata, and target proof.
Both selected builds pass. The connected cache-save fixture now links exclusively
catalog objects and passes its byte-for-byte in-memory output checks.

CFileDataOutputStream's deleting destructor 0x00996F30 is a catalog alias of the
ordinary destructor TU. Its 52 bytes and all relocation targets match retail;
the shared fixture checks direct destruction and virtual heap deletion (newline,
base, then allocation release). Both aliases pass. Updated target proof is in
`work/codex-native-fontctor/landed-file-output-proof.log`.

Next investigation: CBankFileAsync destructor 0x009D6490 drains/aborts pending
reads under a CCriticalSectionLock, temporarily releases the lock and sleeps
10 ms when abort cannot yet finish, unregisters the bank from its failure
handler, then destroys owned containers and pointers. Native call order shows
0x00A62960 is Release, while 0x00A629A0 is the destructor; their identical bodies
were previously given duplicate destructor names. Donor PDB names confirm a
Release method and UnregisterBank on CLoadingMemoryFailureHandler.

The critical-section guard is now a shared typed interface in
`rebuild/include/fable_critical_section_lock.h`. Constructor 0x00A62920 (54
bytes), Release 0x00A62960 (25), Lock 0x00A62980 (25), and destructor 0x00A629A0
(25) all match retail including import-slot identities and pass linked candidate
fixtures. Release and destructor no longer publish duplicate destructor symbols;
Lock no longer dispatches through a fixed retail process address or a synthetic
Methods subclass. Tests exercise both constructor branches, failed try-lock,
nonzero try-lock result, idempotent lock/release, destructor states, receiver and
padding preservation. Import interception is scoped to the fixture executable
and restored before reporting; no production import slots are patched.
Proof: `work/codex-native-fontctor/landed-critical-section-proof.log`.

### Asynchronous bank destruction

CBankFileAsync destructor 0x009D6490 (524 bytes), deleting destructor
0x009D81A0 (30), async pointer-set tree erase 0x009D7430 (51), and counted-pointer
list clear 0x009D7DB0 (89) are cataloged with retail byte and relocation-target
proof. The deleting entry now aliases the ordinary destructor TU, replacing an
unrelated landscape stub. All six vtable slots were checked against retail's
0x0129B8D4 table; missing async overrides are now declared on CBankFileAsync.

Bank async sets have a scoped owning wrapper that clears tree nodes and frees
the header during destruction. A typed tree-erasure boundary names the
0x0042A34E dependency without pretending that dependency is reconstructed yet.
The loading-failure handler interface records the observed 0x28-byte retail
layout (12-byte Banks vector at +4, critical section at +0x10).

The destructor fixture covers empty sets, two unsuccessful aborts with release /
Sleep(10) / reacquire, successful removal, unregister ordering, disposal-list and
restart-set cleanup, final/shared reference owners, and virtual heap deletion.
Focused container fixtures cover null trees, non-owning data pointers, empty and
populated disposal lists, final/shared/null owners, and repeated clear. Twelve
selected builds pass. Seven catalog objects compose successfully in
`work/codex-native-fontctor/check_async_integration.py`, including the actual lock
methods and list/set cleanup. The integration fixture also shares one ownership
block between a disposal entry and a member so cleanup must decrement in order.
Proof: `work/codex-native-fontctor/landed-async-cleanup-proof.log`.

Existing isolated lifetime fixtures use forwarding async virtual stubs so their
base-method observations remain valid, plus a rejecting stub for unexpected
populated sets. Full gate now running in
`work/codex-native-fontctor/async-cleanup-final-gates.log`.

Scratch follow-up: UnregisterBank 0x009D5A30 is behaviorally recovered and tested
with the actual guard and search helper. It removes only the first matching bank,
preserves order/capacity, and always releases the lock, including absent and empty
cases. Current output is 90 bytes versus retail 89 (NEAR_MATCH): VC7.1 preserves
the end pointer in a register across __find instead of reloading it. All four
call targets and their order match retail. It is not claimed byte-identical.
The pointer-specialized __find at 0x009D6E80 is an exact 112-byte match and passes
exhaustive positions across lengths 0 through 13 plus duplicate-first coverage.
Both remain scratch while the asynchronous batch's full gate finishes.
Reproducers: check_async_unregister.py, check_unregister_targets.py,
check_unregister_integration.py under work/codex-native-fontctor.

The asynchronous-cleanup full gate completed: ALL GATES PASS, 123/123 boot
leaves, component ABI, zero regressions across 2,263 header dependents, references,
ring, and visual bootstrap. The phase-7 unopened-bank boundary also uses the new
async forwarding/empty-set fixture support.

UnregisterBank 0x009D5A30 and __find 0x009D6E80 are now cataloged with the honest
NEAR_MATCH / MATCH grades described above. Both selected builds and the connected
fixture using four catalog objects pass. Target evidence is saved in
`work/codex-native-fontctor/landed-async-unregister-proof.log`.

The cache-load follow-up is in scratch: Open 0x00A60D40 emits 621 versus retail
623 bytes (98.46% opcode similarity), with equivalent disk-vtable stores but a
different stack arrangement for its temporary char string. Its linked fixture
passes missing-file, incompatible-version, empty/negative count, duplicate-name,
empty/populated old-map, and both buffered/overflow-read paths. It checks clearing
stale state, loaded timestamp/checksum values, preserved requested buffer size,
0x4000 file-stream buffer, and destruction order. It is not yet cataloged.
Reproducer: `work/codex-native-fontctor/check_cache_open.py`.

### Cache loading and compact timestamp reads

CChecksumCacheFile::Open 0x00A60D40 is now cataloged as NEAR_MATCH (621/623 bytes,
21 verified direct-call targets and two verified disk-vtable references).
CDataInputStream::ReadDateAndTime 0x00995770 matches all 631 bytes with eight
verified call targets. CDateAndTime's seven-argument constructor 0x009E9A70 is an
exact 53-byte match. The input/date headers expose these nonvirtual operations.

Seven selected builds pass, including existing Save, timestamp writer,
stream-copy, and dependency-check callers. The date fixture checks unsigned
byte/word expansion, a negative year, buffer availability from 0 through 12
bytes, and preserved receiver/buffer state. The three-object cache-open
integration passes using catalog objects and the recovered timestamp path.
Proof: `work/codex-native-fontctor/landed-cache-open-proof.log`.
No broader gate has been claimed for these last nonvirtual header additions;
the last full gate remains the async cleanup pass.

### Cache map ordering and lookup

Recovering the cache-map index exposed a missing CWideString comparison method:
the generic comparator previously resolved `<` through the implicit text-pointer
conversion. Retail calls its lexical operator at 0x0099AF70 instead. The operator
is now declared and implemented, matching 56 bytes and the wcscmp/empty-buffer
targets. Tests explicitly place "z" before "a" in memory, distinguish allocated
empty from null strings, and exercise high UCS-2 values and embedded NULs.

With that declaration, cache-map operator[] 0x00A617E0 matches 136 bytes and the
lower-bound helper 0x00A609C0 matches 51 bytes. Focused fixtures cover existing
keys, insert-before hints, end hints, default timestamp construction, temporary
key lifetime, empty trees, and missing/equal/between/after queries. The three
catalog objects also pass a connected lookup/index fixture (node insertion
remains an external boundary). Five selected builds pass. Proof:
`work/codex-native-fontctor/landed-cache-map-proof.log`.

The changed comparison resolution warrants a broad gate; it is running in
`work/codex-native-fontctor/cache-map-final-gates.log`. Do not claim it has passed
until that log finishes. All further experiments stay in scratch during it.

Scratch insertion follow-up: cache-tree _M_insert 0x00A61270 (233 bytes),
insert_unique 0x00A61360 (155), and hinted insert_unique 0x00A614B0 (634) match
retail. Their connected fixture exercises five hint/order patterns, duplicate
preservation, node payloads, root/parent/extrema links, strict sorted order,
red-parent constraints, equal black heights, and complete string-owner cleanup.

Target validation caught the typed decrement boundary: retail 0x00A60890 returns
the iterator receiver, whereas an unspecialized expansion called a void base
helper. A scoped typed operator-- now matches the 79-byte body. The typed ++
operator 0x00A60780 matches 62 bytes. The fixture traverses all nodes both ways
and checks the returned references. All five target proofs pass in
`work/codex-native-fontctor/cache-insert-proof.log`. This batch remains scratch
until the currently running cache-map gate completes.

## Cache insertion promoted and lifecycle roundtrip

The preceding cache-map full gate finished with ALL GATES PASS: 123/123 boot
leaves, component ABI, zero regressions across 2271 dependent catalog entries,
bootstrap references, zero unresolved ring externals, and visual checkpoint.

Promoted the three insertion routines and two iterator aliases described above.
All five selected candidate builds and retail byte/target proofs pass. Explicit
Leaf selectors ensure each alias/helper is checked against its own COMDAT. Added
the same selectors to twelve earlier ordinary/deleting destructor entries;
all twelve selected builds pass. No source behavior changed in that correction.

The three insertion catalog objects pass the connected red-black invariant
fixture. Cache Open now also passes its 14-case real-map fixture using ten
catalog objects instead of scratch insertion objects.

The roundtrip fixture links 24 catalog objects, including the actual checksum
cache constructor, Open, Save, Close, destructor, insertion/lookup/erase helpers,
date reader/writer, string narrowing, and file-output stream methods. It loads
three records with a duplicate key, retains the latest value, saves two records
in lexical order with byte-for-byte format verification, closes/clears the map,
checks repeated Close, reopens through forced overflow reads, and destroys the
cache with no surviving string owners. Disk I/O, input stream buffering/string
reads, and string allocation/assignment remain fixture boundaries.
`work/codex-native-fontctor/check_cache_roundtrip.py` reports
CHECKSUM_CACHE_ROUNDTRIP_PASS. The native Close gateway replaced the earlier
fixture-only Save gateway.

A full gate for the shared iterator specialization and catalog selections is
running in `work/codex-native-fontctor/cache-insert-final-gates.log`; no result
is claimed yet. Further reconstruction stays in scratch until it completes.

## Checksum retrieval and filesystem timestamp follow-up (scratch)

Five additional native bodies now match their retail bytes and all relocation
or import targets in scratch:

- CChecksumCacheFile::GetChecksum 0x00A60FB0: 411 bytes.
- Cache-tree _M_find 0x00A608F0: 83 bytes.
- CDateAndTime::operator== 0x009E9AD0: 71 bytes, exact.
- CAFile::GetFileModifiedTime 0x009992A0: 119 bytes.
- CDateAndTime(FILETIME const&) 0x009E9DB0: 86 bytes.

GetChecksum's direct chunk-size conditional reproduces retail register allocation;
a superficially equivalent assign-then-if form did not. Five focused fixtures
pass. The connected checksum fixture links the real tree and CRC implementations
and checks missing files, first fill, unchanged timestamp hits, changed timestamp
replacement, 0/1/16/17/63-byte inputs, final partial chunks, dirty state, read
counts, and string cleanup. CRC expectations use an independent bitwise reference.

Timestamp tests check path/context forwarding, LastWriteTime selection, string
cleanup, real FileTimeToSystemTime conversion, and placement-construction guards.
The attribute query's return value is ignored by retail. A controlled-output
fixture checks that behavior even for a false return; this does not establish a
useful timestamp after an actual failing OS query.

Scratch proof: `work/codex-native-fontctor/cache-get-proof.log`.
All five remain scratch while the insertion/header gate is running.

## Checksum retrieval promoted

The insertion batch full gate completed: ALL GATES PASS, 123/123 boot leaves,
component ABI, zero regressions across 2276 dependent entries, references,
zero unresolved ring externals, and visual bootstrap checkpoint.

Promoted all five checksum/timestamp routines from the preceding section.
Each selected candidate build passes, and the landed byte/target proof is
`work/codex-native-fontctor/landed-cache-get-proof.log`. The checksum connected
fixture now uses catalog objects for GetChecksum, tree find, and date equality;
it passes with the existing catalog map objects and rebuilt native CRC source.

Target review corrected VC7's array-allocation declarations before promotion:
GetChecksum includes <new>, emitting operator new[]/delete[] for retail
0x00BFEB22/0x00BFEB1C. Scalar operator new is 0x00BFEA1A and must not be treated
as that same relocation merely because the call bytes can be masked.

## Native input-stream cache roundtrip

Twelve existing date/filesystem/input-stream candidates rebuilt successfully
with the new checksum/timestamp declarations.

Promoted CDataInputStream::ReadNullTerminatedString 0x00994C80 (149 bytes,
relocation match), CCharString::operator+=(char) 0x0099F180 (110 versus 106,
near match), and CBasicString<char>::Append(char, ulong) 0x009A0460 (115 bytes,
relocation match). Corrected the old manifest's mistaken wide-string label for
0x0099F180. The near match keeps a separate two-byte local buffer instead of
reusing the argument slot; all four call targets and behavior checks pass.
`work/codex-native-fontctor/landed-cache-read-proof.log` checks targets for all
three. Tests cover shared ownership, null versus allocated data, append of NUL
and high-bit bytes, repeat counts 0/1/2/7/19, aligned zero padding, and input
terminators/strings across every relevant initial-window boundary.

The stronger cache lifecycle roundtrip now links 35 catalog objects, including
the actual input-stream constructors, Close/destructor, position setter,
buffered/unbuffered source hooks, overflow reader, and null-string reader. It
passes `CHECKSUM_CACHE_NATIVE_INPUT_ROUNDTRIP_PASS` from
`work/codex-native-fontctor/check_cache_native_input_roundtrip.py`. Disk access
and basic string allocation remain fixture boundaries; output bytes, tree
contents, saved ordering, repeated Close, and final owner counts are checked.
The prior forced-overflow fixture remains separately available.

Removed a stale asm_bake catalog label from the already-native 0x00994700
constructor entry and gave both input constructors explicit Leaf selectors.
Forced rebuilding both constructor entries succeeds. Their C++ source was
already present; this is catalog/verification repair, not a new reconstruction.

The full gate for these string-overload/header additions is running in
`work/codex-native-fontctor/cache-read-final-gates.log`; no pass claimed yet.

## Real string ownership connected (scratch)

A 59-object cache roundtrip now passes with the actual string constructors,
assignments, allocation helpers, conversion, and destructors in addition to
the previously connected map and stream routines. Both native instance counters
return to zero. This is instance-lifetime evidence; the fixture does not
independently count every CRT heap allocation. Disk operations remain simulated.
`work/codex-native-fontctor/check_cache_native_strings_roundtrip.py` reports
CHECKSUM_CACHE_NATIVE_STRINGS_ROUNDTRIP_PASS.

This exposed two stale catalog entries: CCharString's default ctor 0x0099E4B0
uses a separate g_charStringCount and its test redefines the target; wide-string
assignment 0x0099B7D0 names a placeholder Release instead of UnassignString.
Existing proper shared-header versions are available and are now proved/tested
in scratch for catalog repair. Their correct bodies match 15 and 47 bytes,
including the real global counter and release target. No catalog change yet.

New scratch reconstruction: CWideString(CCharString const&) 0x0099B440 matches
135 bytes and the STL wide-string fill constructor 0x0099CA70 matches 118 bytes
(including the final int3 after the noreturn length-error call). The conversion
allocates even for an empty input and widens each stored byte with &0xFF,
preserving embedded NULs and high-bit bytes. Tests cover null/allocated-empty,
explicit-length input, owner/counter behavior, fill counts 0/1/2/5/13, and
rejection of wrapped/oversized counts through the length-error boundary.

All four exact relocation-target proofs pass in
`work/codex-native-fontctor/cache-strings-proof.log`. These changes remain
scratch while `cache-read-final-gates.log` finishes.

## Native strings promoted and physical-file roundtrip

The previous string-reader gate passed completely: 123/123 boot leaves,
component ABI, zero regressions across 2284 dependents, bootstrap references,
zero unresolved ring externals, and visual checkpoint.

Promoted six verified bodies/catalog repairs:
- 0x0099B440 wide-string conversion constructor, 135 bytes.
- 0x0099CA70 STL wide-string fill constructor, 118 bytes.
- 0x0099E4B0 narrow default constructor, 15 bytes (shared counter repair).
- 0x0099B7D0 wide copy assignment, 47 bytes (real UnassignString repair).
- 0x0099E4C0 narrow const-char conversion, 15 bytes (real member/global repair).
- 0x00A61150 cache filename/buffer constructor, 85 bytes.
All are relocation matches; all six candidate builds and landed target proofs
pass. The old standalone accessor also had the wrong symbol and a separate
empty-string global; its replacement uses the actual shared class declaration.
Updated recovered ABI metadata rather than retaining the old wide-string/char
and unknown-function guesses. Proof: `landed-cache-strings-proof.log`.

The native-string memory-backed roundtrip passes with these catalog objects.
More substantially, `check_cache_real_disk_roundtrip.py` now passes against
real unique temporary files in the scratch directory: native disk Open, Read,
Write, seek, Close, file-existence and modification-time queries; native cache
load/save/tree/string logic; and the real CRC implementation. It verifies
exact saved bytes, duplicate replacement, sorted entries, real timestamp hits,
CRC32 0xCBF43926 for "123456789", and invalidation after replacing the content
with "abc" and setting a different modification time (CRC32 0x352441C2).
Both checksums survive save/reopen; the new cache-opening constructor is used.
Every created temporary file is deleted by the successful run, and both native
string instance counts return to zero.

Scope: absolute-path normalization/directory-policy helpers, base-class
lifetime bodies, and unused error-message construction remain fixture
boundaries. This validates the successful disk-cache lifecycle, not every
filesystem error path or a full native game startup.

Full gate for the latest constructor/header/catalog changes is now running in
`work/codex-native-fontctor/cache-strings-final-gates.log`; no result claimed.

## Latest cache gate and return to native GFMain

The cache-strings full gate completed with one harness failure: the default
char-string constructor fixture now prints `CHAR_STRING_DEFAULT_0099e4b0_PASS`,
but the bootstrap still expected the previous marker. Corrected the bootstrap;
targeted 0099e4b0/0099b7d0 boot checks then passed. The original full run remains
recorded as failed, with component ABI, zero regressions across 2290 header
dependents, bootstrap references, ring linkage, and visual checkpoint passing.

Audited all 3952 bytes / 995 instructions of GFMain 00402510, including 49
branches and ten indirect calls beyond the 261 direct-call ledger entries.
Scratch inventories: `work/codex-native-startup/gfmain-operands.json`,
`gfmain-indirect-calls.json`, and `gfmain-branches.json`, generated by
`audit_gfmain_operands.py`. The opening installs callbacks at 013bc9fc/013bc860
and 013bca00/013bc864, conditionally registers console variables via 0137548f,
and releases the installer counted owner inline. These details must survive
the future native GFMain body; existing authored phase adapters simplify them.

## Typed startup symbols and a real localisation correction

Promoted the actual CIME::Get accessor at 00404a80 (6 bytes, global 013b7cd8),
replacing the misleading AddChildPrimitive free-function name. Startup phase 9
now calls the shared typed CIME interface and its boundary owns typed storage.
The source previously returned the correct address under the wrong ABI symbol.

Connected the existing native bodies to the declared NLocalisation symbols:
GetTextBankName 00415260 and GetLanguageFolder 00415530. Updated actual callers,
fixtures, catalog, manifest, boot oracles, and symbol index.

Full relocation validation discovered that both text-bank and streaming-font
selectors had French and Italian IDs swapped: retail is 12 = French and
16 = Italian. Their prior fixtures repeated the wrong mapping. Corrected both
selectors and fixtures. Each selector now matches all 227 bytes (including
switch tables) and all 27 relocation targets; the folder factory matches 19
bytes and both targets. Landed-object proofs and behavior checks pass, as does
the typed IME getter proof. Five catalog builds pass, including the dependent
CMainGameComponent::InitialiseText caller.

Expanded the two boot oracles from 178 instruction bytes to the complete 227
bytes including switch tables. `tools/check_boot_object.py` now validates
absolute same-section relocation destinations, rather than only masking them.
The preserved old selector objects fail this new check at table offset 0xc0;
the corrected selectors pass all ten internal targets each. Regression proof:
`work/codex-native-startup/check_switch_regression.py`.

Full follow-up gate is running in
`work/codex-native-startup/localisation-final-gates.log`. Native GFMain itself
is still unreconstructed; these fixes do not establish a full game startup.

## Native language detection and bank lookup promoted

The localisation follow-up gate exposed two directory fixture definitions
still using the old language-folder name in `LanguageDirectory_test.inl`.
Corrected that shared fixture; both targeted boot checks pass. It also exposed
the same French-ID mistake in the authored Phase 7 fixture (16 instead of 12),
which is corrected. The run remains recorded as failed; its ABI, 2293 header
dependents, reference check, and ring passed. The relocation gate itself found
no additional internal-target failures in the 123 boot leaves. Gate diagnostics
now include stderr so a rejected relocation reports its reason.

Recovered and promoted:
- 00415070 NLocalisation::GetLanguage, 495 bytes / 50 relocation targets.
- 009a80b0 CBankFileManager::BankExists, 145 bytes / six call targets.
- 009ab4f0 contained-bank tree Find, 112 bytes / two call targets.
- 009ab560 development-bank tree Find, 112 bytes / two call targets.
- 009ab5d0 bank-alias tree Find, 112 bytes / two call targets.
All five are relocation matches with every target independently checked.
The PDB names confirm GetLanguage returns unsigned long and BankExists is a
const member taking a const CCharString reference. Added the real manager flag
at +0x14 and alias map at +0x18, retaining existing field offsets and layout.

The language coordinator checks registered text banks in retail priority order:
English, French, Japanese, German, Korean, Chinese, Spanish, Italian. Successful
detection is cached; no match leaves zero and is retried. A unit fixture checks
all 256 bank-presence subsets and six pre-cached values, including exact query
order and string cleanup before return.

The integration fixture connects all five bodies with the recovered string
comparison and then with nine real narrow-string allocation/lifetime objects.
The landed-object run also compiles its fixture against production headers.
It passes all 256 language subsets, alias resolution, development/retail modes,
a match in the second retail bank, an empty retail list, null/same-owner keys,
and returns the native narrow-string instance counter to zero. Scope: tree
construction/insertion, bank object construction, and unused wide-string
members are fixture boundaries; filesystem loading is not exercised here.
Proof: `check_bank_language_integration.py --native-strings --landed` prints
`BANK_LANGUAGE_NATIVE_STRINGS_PASS`.

Seven selected catalog builds pass (five new bodies plus both selectors).
Updated selectors to call the real NLocalisation::GetLanguage ABI; the authored
bootstrap still supplies its explicit state-backed language boundary, while
the catalog now supplies the native implementation for native dependency links.
Updated artifact and symbol indexes. Full gate now running:
`work/codex-native-startup/bank-language-final-gates.log`.

## Text-bank teardown recovered in scratch (not promoted)

The bank/language full gate completed with bootstrap visual checkpoint, refs,
ring, component ABI, and 2298 header dependents passing. Its sole boot failure
was a fixture helper named Find shadowed by the new CBankInfoMap::Find. Renamed
that fixture helper to FindFixtureEntry; targeted 009a8170 passes. A full rerun
is active in work/codex-native-startup/bank-language-recheck-gates.log; it has
passed 123/123 boot leaves and component ABI so far.

Retail text-bank teardown exposes four 12-byte vectors at +1e0, +1ec, +1f8,
+204 (Narrators), followed by Seed +210. The prior three 16-byte placeholder
arrays had the same total size and zero initialization, hiding incorrect field
offsets. Scratch header corrects these fields and intrusive pointer copy /
release semantics, defines the 8-byte text-buffer entry, and adds native Close.
Resource-bank teardown also proves the missing CBaseClassNonCopyable base;
adding it preserves size and offsets. No production header promotion yet.

Scratch proofs now validate complete bytes AND every relocation destination:
- 009c8660 CloseTextAndGroups, 154 bytes / five calls.
- 009ca660 and 009ca6c0 text/group copy helpers, 88 and 82 exact bytes.
- 009cabc0 and 009cac00 text/group destruction helpers, 50 exact bytes each.
- 009c87c0 text-bank destructor, 202 bytes / twelve targets.
- 009cb840 deleting destructor, 30 bytes / two targets.
- 009c8bf0 Close, 16 bytes / two targets.
- 009fc690 resource-bank destructor, 103 bytes / twelve targets.
- 009fc750 resource-bank deleting destructor, 30 bytes / two targets.
- Existing 009c85a0 text-bank constructor remains a 141-byte relocation match
  with all five destinations checked against the corrected scratch layout.

check_textbank_buffers.py now links the real buffer-clear implementation to an
ABI lifetime fixture: shared references, null entries, repeated clear, capacity
retention, and untouched Seed pass. Bank storage is borrowed and pointees are
intrusive ABI probes, not real loaded text/group objects. Full bank lifecycle
integration and production promotion remain outstanding.

Follow-up scratch verification:
- Added the real CResourceBank constructor 009fc5f0: 156-byte relocation match,
  all six targets checked. Its lifecycle fixture checks both list defaults,
  detached sentinels, and reverse removal order for attached sentinel probes.
- Buffer fixture now exercises overlapping nonempty text/group erase as well
  as full clear, checking narrator-index copies and intrusive reference counts.
- check_textbank_lifetime.py links native text-bank ctor/dtor/Close, resource-bank
  ctor/dtor, buffer helpers, and typed counted Reset/DeleteData. Virtual Close,
  repeated Close, virtual deletion, and Reset releasing a populated bank pass.
  Async/file base lifetimes and strings remain fixture boundaries in this test.
- A scratch-versus-production audit of 81 affected catalog entries is running
  in textbank-header-audit.log, before any shared-header promotion.

## Text-bank and resource ownership promoted

The prior bank/language rerun completed ALL GATES PASS: 123/123 boot leaves,
component ABI, zero regressions across 2298 header dependents, zero missing
bootstrap references, zero unresolved externals, and visual bootstrap checkpoint.
This is the authored bootstrap checkpoint, not native GFMain completion.

Promoted 13 entries: resource-bank ctor/dtor/deleting wrapper; text-bank
CloseTextAndGroups, two copy helpers, two destroy helpers, dtor/deleting wrapper,
Close; and typed CCountedPointer<CDataBank>::Reset/DeleteData. Preserved the
folded physics-interface Reset instantiation for existing callers. Replaced
three mislabeled generic providers (009fc750, 009cb840, 00403650), whose previous
fixtures only printed PASS, with actual typed providers and lifecycle fixtures.
Superseded sources/tests are archived in work/codex-native-startup/superseded-textbank.
The promotion script initially stopped on two pre-existing generic destructor
entries; resumed after identifying and archiving them. Do not rerun the script.

Production header now has four correctly sized text-bank vectors, Narrators,
the native Close override, the resource-bank base class, and balanced intrusive
pointer copy/assignment/destruction. Updated isolated Close boundaries and the
GetPGroupEntry fixture's vector access. Window-title fixture previously forged
a pointer return assuming copying did not increment its reference; replaced it
with an ordinary owning return. All four affected boot fixtures pass.

All 13 candidate builds pass. Landed proof scripts verify bytes and every target
against production sources/headers; the resource lifecycle, typed Reset failure
and shared-owner tests, and combined text-bank lifecycle also pass. The scratch
header audit found zero regressions among 81 affected catalog entries before
promotion. Updated symbol index and 22 artifact entries using actual catalog
source/test paths, and corrected 13 donor-derived prototype records.

Full post-promotion gate is running:
work/codex-native-startup/textbank-final-gates.log (session 36462).
It has passed 123/123 boot leaves and component ABI so far.

## Resource-list follow-up in scratch

Recovered CResourceList::RemoveResource 009fc020 (80 exact bytes) and the typed
SetMaximumMemory 009fc520 (20 bytes with its sole tail call to ShrinkResources
009fc210 validated). Removal tests use native list construction/addition and
cover middle/end removal, null-next behavior, both inconsistent-link branches,
and destructor-driven removal. The maximum-memory test links the native shrink
loop and checks successful reduction, unload refusal, attempt bounds, and the
native unsigned interpretation of a negative limit. Not promoted yet;
land_resource_accounting.py is prepared but has NOT been executed.

Connected native RemoveResource to the landed resource-bank destructor with
both sentinels attached to an external owner list. Counts and allocated memory
return to zero. The resource-bank TU also emits the list/resource lifetime
bodies, so choose that provider once; linking the standalone list ctor/dtor
objects alongside it produces duplicate definitions. Added explicit proofs that
its emitted list ctor 009fc570 (77 bytes) and dtor 009fc5c0 (47 bytes) also match
retail, including every relocation.

check_textbank_native_group.py now connects native text-bank and resource-bank
lifetimes, typed Reset, AddResource, RemoveResource, SetMaximumMemory and Shrink
with a real CGroupEntry-derived lifetime probe. Close destroys the attached
group, updates native list counts/memory to zero, and leaves no stale links.
File/async bases remain boundaries; the group probe has no loaded data payload.

CResource's declaration lacked eight virtual slots. Retail table 012354a4 and
Ego_r method order confirm all ten slots. Scratch fable_resource_list.h adds
UnloadResource, GetUnloadableMemory, IsResourceLoaded, OnResourceLoaded,
CanAbortResourceLoad, OnResourceLoadAborted, GetDebugMemoryInUse and
ResetDebugMemoryInUse, plus CResourceList friendship for private ListRemove.
check_resource_virtuals.py proves all eight default method bodies exactly and
all ten vtable slot identities/retail targets (slot 0 is the vector deleting
wrapper symbol). This header remains scratch, not production.

A production-versus-scratch audit of this resource header completed with zero
regressions across 226 affected catalog entries:
work/codex-native-startup/resource-vtable-header-audit.log.

Scratch UnloadNextResource 009fc170 currently compiles to 157 bytes versus retail
149; it is NOT an exact match and is NOT promoted. The strict parity script
check_resource_unload.py still fails as intended. /Os produced 135 bytes and was
reverted; the source is clean /O2 with direct decrement/test matching retail's
reference handling. check_resource_unload_behavior.py passes empty/capped,
young/small, successful/refused callback, callback deletion, transfer, detach,
and unsigned frame-wrap cases with native add/remove/link helpers. PDB evidence
is saved in resource-methods.txt, resource-layout.txt, resource-list-layout.txt.

The text-bank full post-promotion gate completed ALL GATES PASS: all 123 boot
leaves, component ABI, zero regressions across 2311 dependent entries, no missing
bootstrap references, no unresolved externals, and visual bootstrap checkpoint.

Promoted native RemoveResource 009fc020 and corrected SetMaximumMemory 009fc520;
land_resource_accounting.py has now been executed and must NOT be rerun. The
latter replaces a donor-mislabeled CInputProcessGameBase::AddToInterface body
whose old test supplied its own implementation. Both actual candidate builds
pass; landed byte/call-target proofs and native resource-bank/group integrations
pass against production headers. Fixture dependencies use test-support headers
so isolated catalog grading can relocate the fixture. Symbol/artifact indexes
are refreshed (24 startup artifact records now use the real catalog paths).

The resource virtual-header audit passed all comparisons with zero regressions
across 226 dependent catalog entries. The header remains scratch. The native
unload-loop candidate received formal NEAR_MATCH with candidate-linked behavior
PASS: 69 versus 67 instructions, opcode similarity 0.970588, complete decode,
157 versus 149 bytes. Proof directory: unload-grade-pjln_p71. It is still NOT
promoted. The strict exact comparison remains failing by design.

Subsequent update: the resource virtual header and UnloadNextResource are now
promoted. UnloadNextResource remains explicitly NEAR_MATCH (157 versus 149 bytes),
with candidate-linked behavior PASS. Native pressure integration also passes.
The full resource-unload-final-gates.log completed ALL GATES PASS: 123/123 boot
leaves, component ABI, zero regressions across 2313 header-dependent entries,
zero missing references, zero unresolved externals, and visual checkpoint PASS.

Fixed gate_header_dependents.py temporary fixture copying: quoted dependencies
outside rebuild/include resolve to their original absolute paths, while header
dependencies within the include tree still use the temporary revision. A concrete
three-candidate reproduction passes after the fix; the full gate includes it.

Scratch CIV deletion and nine resource-family lifetime bodies match every retail
byte and relocation target. Candidate integration covers last-owner deletion,
virtual destruction, deleting flags zero, attached sentinels, and balanced frees.
Their landing scripts have not yet run at the time of this entry.

Tracing WindowTitle exposed incorrect prior identities: 009c9070 is GetPTextEntry,
009c8890 is GetPGroupEntry, and 009c7a40 is CTextEntry::GetText. Corrected scratch
types restore the retail text/group layouts. All three corrected bodies match
bytes and every call target (245/247/22 bytes respectively); isolated behavior
tests pass. Production identity migration remains pending.

Subsequent promotion: land_civ_delete.py, land_resource_family.py, and
land_text_identities.py have now executed; do NOT rerun these one-shot scripts.
The seven selected resource candidates build and grade PASS (six relocation
matches, one exact match). Text lookup, group lookup and text getter now have
correctly named canonical sources, catalog entries, layouts and oracles. Removed
the spurious CBankFileRecovered layout; corrected GFMain call ledger and Phase6
boundary names. Landed identity proofs verify every call target, and all three
candidate behavior tests pass. The full post-promotion gate is running in
text-identities-final-gates.log; its result is not yet known.

WindowTitle caller proof now checks all 274 bytes and all 19 relocation targets,
including literal contents: check_window_title_targets.py. A separate scratch
integration links production WindowTitle, GetPTextEntry and GetText together;
missing symbol, missing entry and localized title paths pass with balanced
references. Bank I/O and string storage are fixture boundaries in that test.

Scratch text/group UnloadResource bodies now match all 54 bytes each and the
IsFinished call target (009c7e00 is TEXT despite its old group label; 009c7eb0
is GROUP). Initial empty-pointer copy assignment compiled to 48 bytes. FableWin
symbols confirm CIVCountedPointer Release and Reset(T*) methods; modelling that
native reset sequence produces the retail double clearing without changing
existing copy assignment. Virtual dispatch tests cover pending reads, last-owner
deletion, shared ownership, and empty owners; six allocations/frees balance.
Files: check_text_resource_unload.py, check_text_resource_unload_behavior.py,
text-resource-unload/. These header/method changes are NOT promoted yet.

The text-identities full gate completed ALL GATES PASS: 123/123 boot leaves,
component ABI, zero regressions across 2318 dependent entries, zero missing
references/unresolved symbols, and visual bootstrap checkpoint PASS.

After that gate, land_text_resource_unload.py executed: both unload overrides
and CIV Release/Reset are now production. The text/group lookup and WindowTitle
fixtures link the native overrides; WindowTitle's async fixture rejects access
because it supplies only loaded text. Scratch header-consumer tests also passed
for WindowTitle, text lookup and GetText before promotion.

land_text_bank_read_accessors.py also executed: BeginLoadingText 009c8630 (43
bytes), BeginLoadingGroup 009c8220 (44), GetEntryDataType 009cc210 (38),
GetEntryDataSize 009cc240 (37), and CPackedUIntArray::operator[] 00a629f0 (101).
The 33 mask words at 013979e8 are verified verbatim against retail and provided
by the packed-reader source. Tests cover 12672 packed values (widths 0..32,
cross-word values and bias wraparound), zero-width no-read, raw runtime fields,
and one-based packed bank indexing. Dispatcher tests cover wrong types, missing
entries and correct receiver/index forwarding. Do NOT rerun either landing script.

check_text_bank_read_landed.py verifies all seven promoted functions against
production headers, every relocation target, and actual candidate behavior:
all RELOCATION_MATCH/PASS. Proof directory text-bank-read-landed-pi3eylwm.
Full gate now running in text-bank-read-final-gates.log; result still pending.

Additional scratch work NOT promoted: typed CCharString/CWideString Sizeof
(23 bytes each), CTextEntry::Sizeof 009c7d90 (99) and CGroupEntry::Sizeof
009c7e70 (51), all bytes and call targets verified. Integration covers null/empty
strings, packed narrow capacity bits, wide length, vector capacity, and async
payload accounting (DoSizeof remains a fixture boundary). See
check_text_resource_size.py and check_text_resource_size_behavior.py.

WindowTitle's scratch integration now links native bank GetEntryDataType and
packed decoding as well as native GetPTextEntry/GetText. Both packed and runtime
metadata pass missing symbol, missing entry, localized text and wrong entry type,
with balanced references. check_window_title_native_bank.py uses the generated
fixture from check_window_title_native_text.py (rerun the latter first).

New scratch native async-data work: PDB/FableWin establish two base interfaces,
72-byte layout, BankFile reference at +28, Finished +44 and AllowRestart +46.
The real method at 009d5620 is CBankFileAsyncData::Sizeof, not DoSizeof; corrected
the scratch size helper's declaration and relocation identity accordingly.
IsFinished 009d54e0 matches all 65 bytes and three lock ctor/dtor calls; Sizeof
matches seven bytes exactly. A real Windows thread-contention test links native
CCriticalSectionLock constructor/destructor and verifies pending/finished,
restart disabling, blocked access while locked, and size/wraparound cases.
No async-data header/body changes are promoted yet. Files:
check_async_data_accessors.py, check_async_data_accessors_behavior.py.

check_resource_size_native_group.py connects the text/group size bodies,
typed string size bodies and native async-data Sizeof; payload accounting now
passes without an async size stub. Its merged scratch include tree is the next
header candidate. audit_async_data_header.py is comparing 107 dependent catalog
entries with production; only its after-fixtures replace two static async-data
objects with raw layout views (the typed class now has a bank reference member).
Audit log async-data-header-audit.log; result pending. The production full gate
text-bank-read-final-gates.log is also still running.

Async-data staged audit completed with zero regressions across 107 dependent
catalog entries (pre-existing failures remain pre-existing). Staged boot-only
consumer checks also pass for text lookup, GetText, WindowTitle and CDataBank
constructor: check_async_header_boot_consumers.py. Prepared but NOT EXECUTED:
land_async_data_and_resource_sizes.py. It guards on the pending production gate
being ALL GATES PASS, archives three older generic providers, promotes six native
methods, and adds the previously missing 009c7e70 manifest entry. It also updates
the two lookup fixtures' borrowed async-data storage to match the typed class.

The production text-bank-read full gate completed ALL GATES PASS: 123/123 boot
leaves, component ABI, zero regressions across 2325 dependent entries, zero
missing references/unresolved externals, and visual bootstrap checkpoint PASS.

land_async_data_and_resource_sizes.py and land_bank_symbol_lookup.py have now
executed; do NOT rerun. Their initial preflight correctly stopped before mutation
because PowerShell logs were UTF-16; the readers were fixed before execution.
Promoted six async/size methods and the typed bank symbol lookup/lower-bound pair.
The old FableVectorMapRecovered view was actually CBankFile::FindIndexBySymbolCRC
(009cc410, 60 bytes); native const UInt/UInt LowerBound is 009d20d0 (60 bytes).
Their integration passes 16896 cases including duplicate-first selection, empty
maps, and unsigned edge keys. Boot oracle, bootstrap source/fixture paths, GFMain
ledger and manifest identities now use the real bank member. Removed the obsolete
fake vector-map view. Three generic size providers are archived in scratch.

check_async_sizes_symbols_landed.py verifies all eight production functions,
every relocation target, and candidate-linked behavior: four MATCH/PASS and four
RELOCATION_MATCH/PASS. Proof directory async-sizes-symbols-landed-0_g4h_j8.
The new full gate runs in async-sizes-symbols-final-gates.log; result pending.

The async/size/symbol promotion gate completed ALL GATES PASS: 123/123 boot
leaves, component ABI, zero regressions across 2333 dependent catalog entries,
zero missing references or unresolved externals, and visual bootstrap PASS.

The three WindowTitle integration scripts were rerun against current production
headers. check_window_title_native_lookup_chain.py now links native CRC, unsigned
symbol lower bound, bank symbol lookup, packed/raw metadata access, text-entry
acquisition, text retrieval, and unload helpers with GetWindowTitle. Both metadata
routes pass missing-symbol, missing-entry, localized-text, wrong-type and balanced
reference cases. The 1024-byte CRC table matches retail; the independently
calculated zero-seed title CRC is 4f466a84. Disk I/O and string storage remain
fixture boundaries; this is not a full disk-bank startup claim.

Native GFMain 00402510 analysis resumed. NativeGFMainDecompile.java opens the
project read-only and rolls back its temporary type transaction explicitly.
make_gfmain_native_typing.py now supplies 84 direct-function prototypes, 85 names,
and five indirect-call overrides, based on native headers and the retail call
sites. The exit thunk retains its non-returning CRT declaration. Correcting
GFGetSystemManager, parser/persistence object sizes, and system initialization
prototypes recovered one coherent 200-byte CSystemManagerInit local and removed
the earlier phantom register parameters. Output: gfmain-native-typed-decompile.c;
log marker NATIVE_GFMAIN_TYPED_DECOMPILE_PASS helpers=84. This remains analysis,
not promoted native GFMain C++ or a claim of full game startup.

The native GFMain control-flow oracle now executes the retail instructions in
Unicorn with explicit simulated callees/imports. It never calls host filesystem,
registry, window or process APIs. check_gfmain_retail_control_flow.py passes 57
scenarios and covers 75/82 conditional branch edges: normal launch, settings-file
presence/opening, retail versus INI bank routes, permission combinations, EULA
and configuration exits, initialization failures, and shared/final reference
cleanup. It checks stack balance, preserved registers and the security cookie.
The 7 uncovered edges remain unclaimed. The second directory permission result
replaces the first when both directory handles open; authored Phase 6 already
preserves that behavior.

A complete scratch C++ GFMain draft now compiles with VC7.1 /O2 /Oy /GS:
gfmain-native-source/GFMain_00402510.cpp. prepare_gfmain_native_source.py creates
scratch header changes only (LoadFromFile declaration, async static setter,
implicit CStringParser cleanup and inline CAFile destruction). Production headers
are untouched by this draft. CSystemManager and installer declarations still
need reconciliation with shared headers before promotion. The retail-bank suffix
was read directly from 0122daa4 and corrected to L"pc\\".

audit_gfmain_native_draft.py resolves every direct call target. The current draft
has 260 direct calls versus retail's 261, due to a shared wide-string cleanup
block; the executed destructor sequence agrees in all tested scenarios. Its code
is NOT an exact byte match: the latest installer-argument evaluation correction
produces 3974 bytes versus retail's 3952. diff_gfmain_instructions.py reports 851
matching masked instructions of retail's 995; this is diagnostic, not a parity
grade. The earlier 3949-byte draft passed compare_gfmain_native_draft.py in all
57 scenarios, comparing return/exit behavior, call order, string/scalar arguments,
cleanup calls and normalized 200-byte system-init state. Rerun that comparison
after the latest installer setter change before relying on the current object.
Do not promote the scratch draft or claim full native startup yet.

Further GFMain recovery reached 3952 bytes and all 261 direct calls. Declaring
the three Win32 imports as dllimport functions (rather than mutable function
pointers), exposing the already verified Reset template body, and preserving
the native temporary conversions recovered the original register allocation
and cleanup. check_gfmain_draft_relocations.py validates all 391 relocation
targets/literal contents. Only two non-relocation bytes differ: offsets 0xe96
and 0xe99 select ECX rather than EAX for the same profile argument's LEA/PUSH.
This is NEAR_MATCH, not exact parity. All 57 scenarios pass, now also comparing
the exact set of global bytes written and their final values. This stronger
check accompanies a corrected one-byte declaration for 01375754.

The shared-header staging audit initially exposed two issues: fixture includes
of ../src failed when the include directory moved, and existing CAFile destructor
stubs conflicted with the recovered inline destructor. gate_header_dependents.py
now preserves fixture-side external provider paths as well as header-side ones.
23 consumer files were staged: redundant destructor definitions were removed;
file cleanup observations moved to the actual CBaseClassNonCopyable teardown;
two main-component fixtures dropped the artificial empty-CAFile event while
retaining pathname/base ordering and allocation/refcount assertions. Three
checksum providers now use the shared destructor definition.

gfmain-draft-header-audit-v3.log completed zero regressions across 490 affected
entries. land_gfmain_shared_headers.py executed once, promoting 29 files (six
headers, including the extracted fable_system_manager_state.h, and 23 consumers).
Backups and hashes are in before-gfmain-shared-headers/manifest.json. Do not rerun
the one-shot promotion. GFMain itself remains in scratch. The new production
gate is gfmain-headers-final-gates.log (session 90515): 123/123 boot leaves and
component ABI have passed; remaining gates pending at this journal update.

Native GFMain promotion is now complete: canonical
rebuild/src/compiled/00/40/GFMain_00402510.cpp uses C linkage @GFMain@12.
The dedicated gate and execution harness live in tools/decomp_pipeline,
with ABI/symbol/scenario metadata in rebuild/integration/native_gfmain.
Latest standalone gate: NATIVE_GFMAIN_PASS, NEAR_MATCH, 3952 bytes,
391 checked relocation targets, 58 comparative execution scenarios.
The additional scenario exercises the profile branch containing the two
register-choice differences using an explicitly simulated callee mutation.
Config and registry references now use GConfigOptions members and
CSystemRegistry static names. Manifest and artifact index rows updated.
The runnable bootstrap still uses the authored coordinator; do not claim
native GFMain runtime integration. Scratch source/package copies are older
than canonical and must not overwrite it.

The broad shared-header gate completed 123/123 boot leaves, component ABI,
zero header regressions across 2333 entries, and zero missing bootstrap refs.
Ring linkage initially failed because runtime_real_names.cpp retained a
redundant empty CAFile destructor. Removed that duplicate; focused link_smoke
rerun passes 31/31 objects and zero unresolved externals (gfmain-ring-recheck.log).
The original full-gate process is still building the visual bootstrap.

Runtime symbol inventory is preliminary: 78 resolved / 77 unresolved root
symbols including ordinary CRT/imports and provisional globals; this is not
77 missing native functions or proof of full link closure.
PDB evidence identifies startup callbacks 00404c70 and 00404ce0 as static
NUISystem::CIME::ProcessWinMessage(HWND__*, uint, uint, long, long&) and
FilterMessage(tagMSG*), both bool fastcall. Retail sizes 107 and 5 bytes.
The latter tail-jumps to 00cb2b90. Recovery remains scratch investigation.

Visual bootstrap recheck completed VISUAL_BOOT_CHECKPOINT PASS after removing
one additional duplicate empty CAFile destructor in
rebuild/integration/save_metadata_runtime_boundary.cpp. Log:
gfmain-bootstrap-recheck.log. The original all-gates log records its earlier
failures; the focused ring and bootstrap reruns resolve those concrete failures.

IME ProcessWinMessage (00404c70,107 bytes) and FilterMessage (00404ce0,5 bytes)
are promoted in CIME_MessageCallbacks_00404c70.cpp, shared declarations,
220-case behavior fixture, catalog, native oracle, manifest, artifact index,
and symbol index. Both RELOCATION_MATCH with all four combined relocations
checked. Import correction during recovery: 00cb3570 is
ImeUi_ToggleLanguageBar(int), not ImeUi_FinalizeString(bool); the full integer
argument explains retail's zero-extension. Filter tailcalls
ImeUi_IgnoreHotKey(const tagMSG*). Names/types from donor public records.
check_ime_callbacks_landed.py passes both; affected existing Get/Initialise
bootstrap leaves remain RELOCATION_MATCH/PASS.

Resolved the two remaining file-error hooks through actual FableWin GFMain
call sites and incremental jump thunks: 017bf778 -> 01913a40 DoDirtyDisc();
0184959a -> 0186d070 GFHardDiskError(const char*). PDB S_PUB32 records confirm
these names and signatures. Retail bodies 0041a710 and 00401b60 are each RET.
Added Global_FileErrorCallbacks_00401b60.cpp and fable_startup declarations.
GFMain now registers actual typed functions, with CAFile's named static
callback fields replacing two provisional globals. Threaded-file slots remain
provisional void pointers pending ownership consolidation.

gate_native_gfmain.py now also compiles and checks all four callback bodies,
including each relocation target. Latest production report:
NATIVE_GFMAIN_PASS grade=NEAR_MATCH bytes=3952 relocations=391 scenarios=58
callbacks=4. The startup machine-code grade remains NEAR_MATCH; callback
bodies are exact after relocations (file-error hooks exact raw bytes).
Runtime startup integration remains outstanding. One-shot
land_ime_message_callbacks.py has executed; DO NOT rerun it.

Recovered and promoted GFIsSystemErrorFailure (00401bc0): 50 bytes including
its native switch tables, all four internal relocations verified. Exact failure
codes are 2,4,5,10,12,13,14,15; unsigned out-of-range dispatch returns false.
Behavior fixture checks 131168 inputs, including signed/high-bit extremes.
Production catalog fixture now RELOCATION_MATCH/PASS. Added metadata for
GFHardDiskError and DoDirtyDisc with raw MATCH and behavior_test=NOT_RUN
(the native GFMain gate checks their one-byte bodies directly).
Native GFMain gate now verifies five callback bodies, in addition to its own
391 relocation targets and 58 execution scenarios. Corrected GFMain's
write-permission helper declaration by including fable_write_permissions.h;
it now binds to the existing C-linkage provider. Indexed GFMain, error hooks,
IME methods, and the predicate for future runtime closure work.

A concrete extraction bug surfaced on the classifier's zero-filled relocation
table: objdump elided bytes, making a 50-byte object look like 40 bytes.
verify_and_land.py and check_one.py now request -z (disassemble zero bytes).
Both extracted streams were checked against the raw COFF section and now equal
all 50 bytes. Landed predicate and IME fixtures pass after the fix.

Full callback batch gate: native-startup-callbacks-final-gates.log. Boot leaves
123/123, component ABI, native GFMain, bootstrap refs, bank ring, and visual
bootstrap PASS. Header baseline staging failed on a deleted subdirectory
(stlport_tree/insert_unique_hint.h from land/boot-debake). Fixed staging to
create parent directories before restoring baseline headers. Focused rerun:
native-startup-callbacks-header-recheck.log, session 60936, 645 entries,
still running at this entry. Do not claim ALL GATES PASS until it completes.

Read-only native link-closure probe completed three rounds, compiling 162
sources and selecting 235 for a possible next round. Final checked link had
408 unresolved symbols and three duplicate providers (wide constructor,
char destructor, wide GetLength). This is a dependency frontier, not a working
native executable. No executable from that probe was run.

Next staged batch: definition-table-unification. FableWin confirms the setter
name SetTableFilename (009d4a40), previously guessed SetTablePath on an artificial
CDefStringTableRecovered class. Staging uses fable_def_string.h's full native
class, makes fable_definition_table.h a compatibility include, updates the
LoadTable field names, and uses GDefStringTable in GFMain. Stage6 keeps its
authored filename-only storage explicit, exposing a native layout view.
Staged GFMain passes all391 targets/58 scenarios; setter8bytes and LoadTable
164bytes retain RELOCATION_MATCH/PASS; Stage6 boundary compiles. Broader staged
header audit pending: definition-table-header-audit.log, session36667.
No production definition-table headers or consumers have been changed yet.
The CDefinitionManager protected globals were deliberately left with their
current labels; exposing their inline setters is a separate recovery task.

The callback header audit finished four regressions against land/boot-debake:
font GetFontInfo fixtures selected the newly visible non-const overload while
testing the const provider; world-path TUs instantiated generic counted pointers
before importing their retail specializations. Fixed the two font fixtures to
call through const bank views, and reordered the four world source/test includes
so specializations precede instantiation. Focused rerun passes all four
(MATCH/PASS font accessors; RELOCATION_MATCH/PASS world paths), proof directory
callback-header-fixed-5bc4vcgk. No source behavior was changed.

Definition-table staged audit finished 0 regressions /150 entries. Promoted
15 existing files with backups/hashes in before-definition-table-unification.
Setter source/test renamed to CDefStringTable_SetTableFilename_009d4a40;
manifest identity corrected from an unrelated template label. Boot oracle now
has the correct symbol and actual retail target displacement. Updated GFMain
binding metadata, call ledger and RUNNABLE row, and refreshed symbol index.
Production GFMain still passes 3952/391/58 plus5callback bodies; setter8bytes
and LoadTable164bytes both pass the boot-object and behavior gates.
Definition-table visual bootstrap completed VISUAL_BOOT_CHECKPOINT PASS in
definition-table-bootstrap.log. One-shot land_definition_table_unification.py
has executed; do not rerun the landing or overwrite canonical with old scratch.

Symbol-index classification bug found during closure investigation: dumpbin
prints COMDAT selection in section-symbol auxiliary records, not "Communal"
in section headers. The parser now recognizes selections2..7 as mergeable,
while selection1 remains strong. Verified against real inline char destructor,
wide GetLength, and their standalone/operator providers. Only refreshed sources
have updated index classifications so far; most of the existing index was not
rebuilt. The three closure duplicate providers remain unresolved.

Next investigation is CSystemManager::ProcessWindowMessages (009a4f20,141bytes),
the actual consumer of the recovered IME filter: critical-section guard,
PeekMessageW loop, optional fastcall filter, TranslateMessage/DispatchMessageW
for unfiltered messages. Retail import slots identified directly from the PE.
No production edits for this message-loop recovery yet.

## Shutdown checkpoint and resume

Stopped at the user's explicit bedtime request on 2026-09-30. Native recreation
only; no Lua edits, commits, or deployments were made by this work. Preserve the
unrelated dirty working tree and do not overwrite docs/HANDOFF.md.

### Saved and verified

- Canonical GFMain: rebuild/src/compiled/00/40/GFMain_00402510.cpp.
  NEAR_MATCH: 3952 bytes, 261 direct calls, all391 relocation targets verified;
  only offsets0xe96 and0xe99 differ (equivalent ECX/EAX LEA/PUSH register choice).
  58 comparative retail/candidate execution scenarios pass with simulated callees.
- Five callback bodies checked by tools/decomp_pipeline/gate_native_gfmain.py:
  GFHardDiskError00401b60 and DoDirtyDisc0041a710 (one-byte RET each),
  CIME::ProcessWinMessage00404c70 (107bytes), FilterMessage00404ce0 (5bytes),
  GFIsSystemErrorFailure00401bc0 (50bytes including switch tables).
  IME behavior220cases; error-classifier behavior131168inputs.
- Native definition table unified. Canonical setter is
  rebuild/src/compiled/00/9d/CDefStringTable_SetTableFilename_009d4a40.cpp.
  Setter8bytes and LoadTable164bytes: RELOCATION_MATCH/PASS.
  Header audit: zero regressions across150entries.
- Latest executable verification: definition-table-bootstrap.log ends with
  VISUAL_BOOT_CHECKPOINT PASS. The executable remains an authored visual
  checkpoint and still uses the authored GFMain coordinator.
- Main report: rebuild/compile-gate/gfmain-native.json. Relevant logs and
  reproducible proof scripts are under work/codex-native-startup.

### Validation caveat

There is no single new all-green gate_all log for the final tree. The callback
full run passed all non-header gates. Its header staging failure was fixed;
rerunning that header audit found four consumer regressions, and all four were
then repaired and individually rechecked successfully (callback-header-fixed-
5bc4vcgk). The later definition-table change passed its150-entry staged audit,
its two production boot leaves, the native GFMain gate, and the full visual
bootstrap. On resume, a clean consolidated gate_all run can capture these
results together; do not label the historical failed logs ALL GATES PASS.

### Exact next unfinished task

CSystemManager::ProcessWindowMessages at009a4f20 is SCRATCH ONLY. Run:

    python work/codex-native-startup/recover_window_message_loop.py

Scratch directory: work/codex-native-startup/window-message-loop.
Its C++ body compiles to141bytes, matching retail size, but the masked byte
comparison currently FAILS. No parity.json was produced and no behavior fixture
has been written. Next compare the object disassembly with retail to locate the
instruction difference, then test queue order, filter suppression, translation,
dispatch, and critical-section release before considering promotion.

This scratch also proposes typed IME/filter function-pointer fields in copies
of fable_system.h and fable_system_manager_state.h, plus a local Win32 MSG layout
header. NONE of those message-loop header changes are promoted. Future promotion
must adapt GFMain's casts, gfmain_phase7's address assignments, and the
CSystemManager_Initialise fixture's pointer sentinels, then verify affected leaves.
Original retail imports: EnterCriticalSection IAT0143FE0C,
LeaveCriticalSection0143FE10, PeekMessageW01440370,
TranslateMessage0144036C, DispatchMessageW01440368.

### Runtime integration frontier and preservation

- Native GFMain remains unconnected to the runnable executable. The scratch
  three-round closure probe did not link: its last checked round compiled162
  sources, had408 unresolved symbols and3 duplicate string providers;235 sources
  were selected for a possible next round. No probe executable was run.
- Symbol-index COMDAT classification was corrected and checked on real objects;
  only refreshed portions of rebuild/build/symbol_index.tsv use the new result.
- One-shot land_gfmain_shared_headers.py, land_native_gfmain.py,
  land_ime_message_callbacks.py, land_system_error_predicate.py, and
  land_definition_table_unification.py have already executed. DO NOT rerun them.
- Old gfmain-native-source/gfmain-gate-package and definition-table staging
  copies can be stale. Canonical production source is authoritative.
- Do not overwrite the historical retail-parity.tsv, unrelated changes, or
  docs/HANDOFF.md. Do not launch Lua recovery for this task.

All recreation build/test processes launched here have finished. A separate
Python process running tools/check_all.py --no-build (PID2344 at shutdown check,
FABLEFORGE_AUTOMATION_HIDDEN wrapper) was present; it was not launched by this
native work and was left untouched. No recreation process needs to remain
running for the saved state to survive shutdown.
