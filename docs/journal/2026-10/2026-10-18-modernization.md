# Modernization continuation — 2026-10-18

User authorized continuous 64-bit/modernization work until asked to stop. Later steering:
the user is away; coordinate launches around the Lua work. This lane has launched no game,
attached to no game process, injected no input, and changed no installed files. Keep using
offline/headless validation while Lua runtime work needs the shared install/desktop. A missing
Fable process alone is not a reserved launch slot.

## Portable input recording and playback

Added `InputRecording.hpp/.cpp` and the `fable_input_recording` library under
`rebuild/modern/platform`. It records each 15 Hz world tick's mouse frame, keyboard held/pressed
bitmaps and ordered transitions, and four pad frames. The format has fixed-width little-endian
fields, exact binary32 pad axes, a version/rate header, caller-supplied seed metadata, contiguous
tick numbering, bounded payload lengths, per-record CRC-32 and a required completion footer.
`next()` streams one tick for playback; it does not poll live input.

`fable_rawmouse_probe --seconds N --record path.finput` now records the standalone probe,
including XInput pads. `fable_input_check path.finput` validates a complete file and prints
counts. Format and ownership/error contracts are in the platform README. No retail format or
parity source was changed. This supplies input transport only: engine RNG capture/restoration,
retail hooks, deterministic world replay and divergence reporting remain future work.

Validation in `work/modern_input_20261018/final/`:

- MSVC x86, MSVC x64 and GCC: all 11 modern-tree tests pass. New library builds without
  warnings under `/W4 /permissive-` and `-Wall -Wextra -Wconversion -Wpedantic`.
- Recorder tests include 8,000 mouse motions totaling ±2,400,000,000, short key/button taps,
  neutral ticks retaining held state, pad slots and signed-zero bits; every truncation and
  single-byte corruption of the 310-byte fixture; malformed version/rate/order/length/count/
  boolean/axis/footer; maximum event count; sequence boundaries and output errors.
- Three compiler builds emit the identical fixture, SHA-256
  `17a21ba9aa4376b304b46bba8fb7972cfe35ebbb493d0565f331858202895c23`.
  All nine compiler-reader/fixture combinations pass. Separate Python `struct`/`binascii`
  parsing validates positions, endian order, float bits and CRCs.
- One-second hidden/background probe observation: 15 ticks, 22 mouse reports, 0 key events;
  the checker gives the same counts. No game launch, visible window, or injected input.

Build/check logs and scripts are local work artifacts. Nothing committed or pushed.

## Reconstructed inventory-event composition on x64

`tools/modern/modern_inventory_events.py` compiles six unchanged reconstructed sources:
the map/status/delete-menu event handlers, `CWorld::GetPlayer`,
`CPlayer::GetPControlledCreature`, and the component map's `LowerBound`.
The original `InventoryEvents_test.cpp` runs as the VC7.1 baseline. Modern builds use a
scratch copy with type-sized/aligned raw buffers, matching full-buffer snapshots, and a
named `ControlledCreature` member address through an existing friend access declaration.
Fresh copied headers have layout assertions disabled and the retail bitset substituted,
as in the behavior census. No parity file changes.

All three builds pass **30,920 scenarios each**, preserving the original complete domain:
four fill patterns, five component maps, ten lower-bound queries, all 256 signed player-byte
values, component-present/absent paths and all three leaf callbacks. Full storage, map and
event snapshots remain checked. Proof:
`work/modern_inventory_events/20261018T161002847539Z/summary.json`.
Original source/harness hashes are unchanged. The first attempt's modern compile failures
(private member access, before using the existing friend) are preserved separately in
`20261018T160922303016Z`; its original VC7.1 baseline passed.

This is bounded callback/layout evidence, not a fully constructed engine/world or gameplay
replay. Other census failures remain unproven: finding a fixed-width harness shortcut alone
does not exclude a simultaneous source bug. The inventory result proves only this composition.

## Further validation and keyboard correction

- `RawKeyboardWin32::feed` is the shared deterministic report path used by `handle`. It now
  distinguishes left/right Shift/Ctrl/Alt using MakeCode/E0, filters key overrun/fake reports,
  and preserves the other side when one modifier is released. Tests call this path directly;
  no OS input injection. Evidence/API references are in the platform README. The fork still
  uses VK codes; retail EInputKey uses DIK scan codes, so engine binding translation is open.
- Added strict warning flags to the Win32 platform library. A C4334 recorder warning found in
  final log review was corrected with an explicit u64 bitmap mask. The initial `final/` logs
  retain that warning; `warning_clean/` and subsequent `keyboard/` builds are clean.
- **All 12 modern-tree tests pass** with MSVC x86, MSVC x64, GCC 15 and Clang-cl 19.1.5 x64.
  The Clang build is `work/modern_build_clang`, logs under
  `work/modern_input_20261018/clang`. New platform code compiles without warnings. This is
  Windows validation; Linux CI has not run for these changes.
- All four compilers produce the same 310-byte recording fixture (hash above). The additional
  seven Clang reader/fixture combinations pass, completing all 16 combinations. Independent
  wire validation and the first passive capture remain in `final/verification.json`;
  `clang/cross_check.json` records the additional checks.
- `tools/modern/modern_player_events.py`: nine unchanged source files, **8,224 scenarios per
  architecture**, VC7.1/x86/x64 all pass. Only scratch player/world storage size/alignment
  changes. Proof: `work/modern_player_events/20261018T161456595076Z/summary.json`.
- `tools/modern/modern_font_pointer_sharing.py`: the original font-pointer sharing/release
  composition passes **1,200 scenarios per architecture**, including aliasing, refcounts,
  real heap control blocks, const sharing and re-entrant callback mutations. Scratch raw
  pointer buffers use sizeof/alignment instead of two 32-bit words. Proof:
  `work/modern_font_pointer_sharing/20261018T161803411450Z/summary.json`.

## Symbol-map modern-compiler blocker and portable lifetime fixtures

Current MSVC rejects the specialization of `CVectorMap`'s implicit destructor (C2995).
Adding an explicit empty primary destructor to a **copied** `fable_vector_map.h` fixes it.
`tools/modern/modern_symbol_map_lifetime.py` retests all eight previously VC7.1-passing
fixtures blocked by this spelling. **8/8 pass on VC7.1 and modern x86**. On x64, four pass;
three fail raw-layout checks and the font-manager fixture cannot link 28 virtual methods
(its x86 scaffolding does not supply the x64 symbols). Logs:
`work/modern_symbol_map_lifetime/20261018T162155700677Z/summary.json`.
The initial scratch probe's compiler-discovery exception and corrected successful probe
are preserved in `work/modern_input_20261018/symbol_probe*.py` and `symbol_probe/`.
No shared/parity header edit or byte-parity claim.

The symbol-map lifetime and Clear fixtures initialize three string subobjects using `j*4`.
Scratch copies now use the actual named `Entry.first`, `Entry.second.GroupName` and
`Entry.second.EntryName` addresses for both initialization and expected destructor order.
The original tree shapes, alias patterns, element counts, heap allocation/free and refcount
checks remain. `tools/modern/modern_symbol_map_destruction.py` passes all three builds:

- Default destruction: **1,458 scenarios**, proof
  `work/modern_symbol_map_destruction/20261018T162325661065Z/summary.json`.
- `--clear`: **1,458 scenarios**, including repeated Clear, proof
  `work/modern_symbol_map_clear/20261018T162400091153Z/summary.json`.
- `--construct`: the constructor fixture passes with typed vector/tree/flag checks and
  aligned storage, proof
  `work/modern_symbol_map_constructor/20261018T162458182143Z/summary.json`.

These are targeted retests, not a rerun/recount of the whole census. The census report now
qualifies its heuristic harness tags; `--report --retail-bitset` regenerates it from saved
results without executing tests or rewriting the saved result JSON. Future runs save the
bitset configuration and report-only runs reject a mismatching flag.

Next useful offline work: remaining x64 fixture assumptions (definition-manager constructor,
player/creature and resource-pool compositions); integrate the proven modern-header spelling
into a reusable fork overlay before a fresh full census. The font-manager test needs genuine
typed x64 scaffolding rather than x86 decorated-symbol aliases. Continue to coordinate any
actual game launch with Lua; no launch slot has been reserved by this lane.

## Continuation: definition manager and regular CMake event targets

Previous goal turn made progress (new input transport and targeted x64 evidence). Continued
offline without game launches, input injection, installed-file changes or lease claims.

`tools/modern/modern_definition_manager_constructor.py` now passes the existing constructor
fixture under VC7.1, modern x86 and x64. Its copy replaces byte offsets with protected members
exposed by the existing derived probe, checks empty vectors/maps and untouched field bytes,
compares full-width callback/member addresses, aligns placement storage and widens the dummy
wide-string representation. The exact original call log and symbol-map creation/reset order
remain. It also executes the original resource-list constructor/destructor/allocation-handler
providers through the fixture's existing support header. No engine source change. Proof:
`work/modern_definition_manager_constructor/20261018T162851229267Z/summary.json`.

Added `rebuild/modern/reconstructed`:

- `fable_reconstructed_events` builds the union of the proven inventory/player compositions:
  **14 unchanged reconstructed source files**, as an ordinary static library.
- CTest now links the real library for `fable_reconstructed.InventoryEvents` (30,920 cases)
  and `fable_reconstructed.PlayerEvents` (8,224 cases). Callback scaffolding remains in the
  fixture executables; the library itself supplies no runtime stand-ins.
- Default on for Windows MSVC/clang-cl, x86/x64; opt out with
  `FABLE_BUILD_RECONSTRUCTED_EVENTS=OFF`. GCC/Linux original-header support remains open;
  the portable modules continue to build there without this target.
- Shared snapshot/compiler helpers moved to `tools/modern/modern_fixture_support.py`.
  Existing targeted tools import it. New `prepare_reconstructed_events.py` generates
  content-addressed header/harness copies inside the build tree, with input/output hashes.
  Completed snapshots are reused only if unchanged. Incomplete preparations are preserved
  and a new numbered attempt is created; completed snapshot edits are rejected.
- The snapshot copies apply the measured retail bitset/destructor compatibility changes,
  disable retail pointer/layout assertions, and adapt harness storage/member checks. The
  player harness explicitly declares its reused loop index, avoiding `/Zc:forScope-` in
  the CMake targets. Legacy unsigned-short wchar ABI is confined to the recovered target.
- `test_prepare_reconstructed_events.py` is a real CLI integration test for cache reuse,
  added-file detection, preservation/recovery of interrupted preparation, and refusal to
  write into parity inputs. It verifies representative original inputs remain untouched.
  It preserves its generated test artifacts in the build/work directory.

Validation: `work/modern_input_20261018/cmake_events/summary.json` is PASS and records that
all 14 source hashes remained unchanged. MSVC x86, MSVC x64 and clang-cl 19.1.5 x64 pass
**all 15 CTest cases**; Windows GCC 15 passes its **12 portable cases**. Local snapshot CLI
integration tests also pass independently. Existing narrowing diagnostics remain in the
borrowed headers/source (`fable_string.h`, lower-bound ptrdiff-to-long and fixture sentinel
addresses); this does not claim whole-engine pointer-width correctness or warning-free
reconstructed code. The portable modules' prior clean builds remain separate evidence.

The modern-tree CI filters now include the reconstructed sources/shared headers and the
snapshot tooling/fixtures on which the build depends. YAML parses locally; no remote workflow
run, commit or push occurred. The modern README, roadmap and index point to the new targets.

Next: extend regular modern builds to the proven symbol-map/lifetime and definition-manager
compositions, then address the remaining font-manager x64 link scaffolding and player/resource
fixtures. The actual world/runtime replay gate is still open; these are bounded engine pieces.

## Continuation: container/font libraries and Clang declaration compatibility

The preceding goal turn made progress (14-source event library and validated CMake snapshots).
Continued offline; no game launch, installed-file edit, input injection or runtime-lease claim.

The regular modern build now includes **23 original source units** in three static libraries:
14 event units, eight symbol-map/resource-list/definition-manager units, and the font-manager
constructor. Twenty-two compile directly unchanged. `CSymbolMap_ContainerLifetime` is compiled
from a generated copy with declaration-only corrections; all three original algorithm bodies
remain unchanged. The snapshot manifest records the exact edits and original source hash.
No parity sources/headers/fixtures were edited, and no reconstruction coverage was added.

New regular CTests:

- SymbolMapLifetime / SymbolMapClear: 1,458 scenarios each, with the real library's destructor,
  tree cleanup, vector deallocation and Clear providers.
- SymbolMapConstructor and DefinitionManagerConstructor: the prior portable fixture checks.
  The resource-list support copy now links its three original providers from the library
  instead of including their source bodies into the test. Construction executes; separately
  linked destructor/allocation-handler code does not gain a runtime claim from this fixture.
- FontManagerConstructor: empty/nonempty handles, bank/reset/open/map/streaming-init counters,
  eight recorded vertex buffers per manager, with **28 typed aborting unused bank virtual
  definitions** replacing the x86-only decorated-symbol aliases. No real bank I/O, GPU,
  streaming or full manager destruction. Standalone VC7.1/x86/x64 comparison is PASS:
  `work/modern_font_manager_constructor/20261018T165043921908Z/summary.json`.

Clang exposed invalid legacy declaration patterns in the lifetime source: explicit
instantiations of already explicitly specialized methods, plus `_M_erase` specialized only
after its first use. The generated copy declares `_M_erase` first and removes the three
redundant explicit instantiations. The copied vector-map and set headers declare their empty
primary destructors in class and define the same bodies out of class, so both compilers emit
the specialization providers without that legacy workaround. No algorithm/field/ABI rewrite.

Failure evidence is preserved under `work/modern_containers_20261018/`: the first Clang compile
failure, the following missing vector-map destructor link, and the intermediate MSVC missing
set destructor link. These led to the out-of-class primary definitions, rather than disabling
the new targets or suppressing compiler errors. Old snapshots remain intact.

The test runner now checks **both** process exit zero and the exact expected output marker.
CTest's `PASS_REGULAR_EXPRESSION` alone ignores the exit code, so that intermediate approach
was replaced before final validation. The Python contract checks reject a process that prints
the marker and exits 7, and reject a partial-word marker. The snapshot integration CTest runs
five Python tests total, covering these contracts and the prior cache/recovery/containment
checks. [CTest property semantics](https://cmake.org/cmake/help/latest/prop_test/PASS_REGULAR_EXPRESSION.html).

Final validation:
`work/modern_containers_20261018/validation_completed/summary.json` is **PASS**, with all 23
original source hashes unchanged. **20/20 CTests pass on MSVC x86, MSVC x64 and clang-cl x64**;
Windows GCC passes its **12 portable tests**. Existing narrowing/legacy fixture warnings remain
visible. Workflow dependency filters include the new adapters, runner and fixtures; YAML parses
locally. No remote CI run, commit or push occurred.

Next useful work: preserve the new ordinary-library/strict-runner path while bringing the
proven font-pointer sharing and remaining player/resource compositions into it; then advance
actual runtime integration and replay. Full world replay, native GPU/streaming integration,
GCC/Linux engine-header support and the fork's full acceptance gate remain open.

## Continuation: ownership providers and first bounded engine-event replay

Previous goal turn made progress (23-source libraries and a complete modern test matrix).
This continuation stayed offline: no game launch, input injection, install changes, commits
or pushes. Lua runtime reservations remain independent; no launch slot claimed here.

Added actual player-manager lookup/main-player methods, controlled-creature const access and
`CBaseIntelligentPointer` operations to the event library. Scratch fixtures now size/alignment
their buffers by the types, allocate `sizeof(CBaseObjectPointer)`, and clear/check the actual
named pointer field in expected snapshots. All original case axes and full-buffer/refcount/
deletion checks remain. `modern_player_ownership.py` comparisons pass VC7.1, x86 and x64:

- Lookup: 10,656 scenarios,
  `work/modern_player_lookup/20261018T170450995424Z/summary.json`.
- Creature access/expired-pointer cleanup: 9,216 scenarios,
  `work/modern_player_creature/20261018T170456540151Z/summary.json`.

Font-pointer sharing/release now links from the normal font library. The original release
template is included unchanged in a generated instantiation TU for mutable and const pointers;
the fixture no longer includes its provider body. Full 1,200-case domain retained.

`fable_reconstructed_wire` adds the original event `AddData` and serializer providers.
The serializer's generated copy replaces its local duplicate class with the shared declaration,
maps member spellings and preserves the signed player's byte bits. Wire algorithm unchanged.
`modern_game_event_adapters.py` and the snapshot manifest record each edit. The shared header
copy gains the real nonvirtual method declaration without changing the event layout. Explicit
bridge assertions require 32-bit long, 40-byte events and the proven payload/cursor offsets.

New modern `fable_engine_event_bridge` converts between the portable owned codec and engine
events, using the actual reconstructed append for bounded nonempty payloads. It rejects
invalid outgoing events **before** reading the default constructor's unset fields, rejects
oversized IDs/payloads, preserves all player/source bits, initializes unused payload bytes,
and propagates incoming framing errors/bytes consumed. `GameEventBridge` checks **84,480**
cases against the actual reconstructed serializer (five IDs × 256 source bytes × two
replacement states × 33 lengths), plus invalid/truncated records, guards, source immutability,
unused bytes and concatenated-record consumption.

`PlayerCreatureReplay` takes event ID 71 from the original complete 87-row registration source,
round-trips events through the codec/bridge, and calls the registered world action with the
materialized event. Actual world/player-manager lookup and intelligent-pointer cleanup execute.
Of the full **9,216** access cases, **3,072** enter that action through serialized events.
Transport temporaries are destroyed before resetting the core deallocation-observation window.
This is a bounded path, not raw-input mapping, package/world-tick dispatch or RNG/world replay.

The wire/bridge targets also build on **Windows GCC**, while original gameplay headers remain
MSVC/clang-cl only. `FABLE_BUILD_RECONSTRUCTED_WIRE` defaults on for Windows; Linux's LP64 long
layout is not accepted by this engine adapter. Set it and `FABLE_BUILD_RECONSTRUCTED_EVENTS`
OFF for only the portable modules. Linux defaults remain off for both original-header paths.

Combining legacy headers and C++23 initially exposed MSVC's nonconforming-mode concepts error,
then a real missing `_STL::copy` declaration under strict lookup. The copied vector-map header
now includes its required algorithm header. **All regular MSVC reconstructed targets use
`/permissive-`**, rather than weakening the modern consumer. Failure logs are preserved in
`work/modern_event_bridge_20261018/msvc_x64_replay_{first,conforming}.log`.

One Clang ownership fixture initially failed 20 checks. A diagnostic copy retained all 1,200
cases and identified only the re-entrant mode that clears `Info` before `delete Info`:
callbacks and all actual freed-block/refcount checks matched; only the null deallocator-call
counter differed. The copied observer now requires exact **non-null** frees and allows/reports
at most one null call only in that mode. Original provider/retail fixture untouched. MSVC
reports 20 null calls and Clang 0. This preserves the resource obligations but qualifies away
identical null-deallocator callback traces, which C++ does not require.
[C++ delete-expression rule](https://eel.is/c++draft/expr.delete#6).
Initial failure and per-case diagnosis:
`work/modern_event_bridge_20261018/validation_attempt1/clang.log` and
`font_sharing_diagnostic/run.log`. No ownership algorithm or compiler optimization flag was changed.

Final evidence: `work/modern_event_bridge_20261018/validation_completed/summary.json` is PASS,
with **34 provider source inputs** plus registration metadata unchanged. **25/25 CTests pass
on MSVC x86, MSVC x64 and clang-cl x64; 14/14 on Windows GCC**, now including the two actual
wire providers/bridge and snapshot checks. Exact CTest logs are archived beside the summary;
`runtime_observations.json` binds their hashes, case markers and null-delete observations.
Existing borrowed-code narrowing warnings remain visible. No full census/coverage recount or
whole-engine completion claim. Workflow filters include all new adapters/fixtures.

Next: package/framing evidence and an event replay runner, actual action generation from the
input accumulators/retail bindings, additional state-bearing player/resource code, and full
runtime/RNG/state comparison. The full fork acceptance gate remains unfinished.


## Continuation: timestamped package codec and engine bridge

This continuation stayed offline around Lua work. No game launches, input injection,
installed-file writes, commits, pushes or parity-input edits. The prior turn made progress;
the active modernization goal continues.

Added `multiplayer::GameEventPackageCodec`, using the original package-set writer's framing:
one package-count byte, then each package's event-count byte and little-endian signed
32-bit timestamp, then existing event records. Original capacity is 50 packages/40 events,
maximum 72,251 bytes. The portable model owns vectors and validates before encoding; prefix
decode checks every boundary, reports the offending record offset, preserves all timestamp
bits and leaves duplicates/replacement flags in recorded order. No collection-time merging
is applied to decoded records. Corrected FRAME_TIMING's in-memory event-count width from
u8 to u32 and named the package's first field timestamp rather than id, matching the actual
shared declaration/provider bodies; event Type is also long in memory, u16 on wire.

Six original package-set providers (ctor, add, clear, getters and serializer) now join the
normal wire library. Generated copies change only `fable_game.h` to a narrow header extracted
verbatim from its original package-set class declaration. The copied broad header includes
that same extracted header, avoiding duplicate class definitions; MSVC/Clang tests include
both paths. Snapshot manifests record the extraction/include edits and hash all six originals.
Original provider algorithms retain their target-specific typed/unaligned accesses; this is
Windows x86/x64 evidence, not a sanitizer/alignment-safety or LP64 claim. The portable codec
uses explicit byte operations. No original decoder or unguarded collector is exposed to
untrusted input.

The modern package bridge validates native package/event counts before walking arrays,
checks event validity/ID/payload through the existing event bridge, owns the original 80 KiB
aggregate on the heap, and initializes unused slots before memberwise copies/observations.
Native materialization writes recorded events directly, avoiding AddEvent's replacement
coalescing; the original AddPackage body supplies the bounded package copy. Timestamp bits
are preserved by fixed-width portable storage and Windows's required 32-bit long layout.

Tests:

- Portable fixed expected bytes (negative timestamp and duplicate replacement records),
  empty/concatenated sets, count/payload failures with exact error offsets, minimum/maximum
  timestamps, maximum-size roundtrip, and **72,274 rejected prefixes**: all 23 truncations of
  the fixed example plus all 72,251 truncations of the maximum set.
- **660 differential combinations** against the actual compiled original package serializer:
  five package counts (0/1/2/5/50), four event counts (0/1/3/40), 33 payload lengths. Checks
  byte identity, guards, full native-input immutability, reverse conversion, exact prefix
  consumption and original Clear. Malformed native counts/events/IDs/payloads are rejected.
- The existing **9,216-case** player-access replay fixture now sends its **3,072 world-action
  events** through timestamped package encode/decode/native materialization before real
  world/player-manager lookup and expired-reference cleanup. ID 71 still derives from the
  original registration table. Transport temporaries still finish before the core deletion
  observation window. This does not run world-tick dispatch, RNG or full world-state replay.

Final proof: `work/modern_package_bridge_20261018/validation_completed/summary.json` PASS,
**40 provider source inputs unchanged**, with original registration metadata also unchanged.
**27/27 CTests** pass on MSVC x86/x64 and clang-cl x64; **16/16** on Windows GCC. Exact CTest
logs, snapshot manifests, signatures, marker observations and log hashes are archived beside
the summary. Initial matrix and final header-coexistence matrix are retained separately;
all passed. Borrowed-code warnings remain visible. No coverage recount or remote CI run.

Next: retail recorder-file version/length framing and an offline replay reader/runner, then
raw action generation from the input accumulators/bindings, state-bearing code and full
runtime/RNG/state comparison. Future launches still require coordination with the Lua lease
and live runtime state. The full modernization acceptance gate remains unfinished.


## Continuation: retail recorder framing and cross-compiler files

Previous goal turn was progress (40-provider package bridge, full matrix). This turn also
stayed offline around Lua work: no game launch, input injection, install changes, commits,
pushes or parity-input edits. The modernization goal remains active.

Inspecting the actual recorder initialization sources exposed a documentation error:
`CEventPackageFileHeader` contains **long NoPlayers**, and `InitEventPackageSaving` at
0x004160CC sets it to 1. Earlier FRAME_TIMING/October 16 notes called that word a version.
It is now correctly named in the current framing documentation. `AddEventPackageSetToSave`
0x004161A7 writes a signed four-byte size followed by the exact compressed package bytes;
`GetEventPackageSetFromSave` 0x00416148 reads that pair. Four source files and the header
are hashed separately in the cross-recording proof; they are framing evidence, not new
compiled provider units in this continuation.

Added `EventRecordingReader`/`Writer` to the portable multiplayer library. Both use explicit
little-endian four-byte fields, preserve the signed player-count bits without an invented
version/range gate, and stream one frame at a time. Read rejects zero/negative/oversized
lengths before payload allocation (valid range 1..72,251), truncations, malformed packages
and extra bytes within a declared frame. Errors retain absolute offsets and package causes.
I/O/decode failures are latched; invalid caller data writes nothing and can be corrected.
Writer finish flushes without an invented footer. Reader distinguishes empty batches from
EOF and returns stable EOF between frames. Retail has no footer/checksum, so a cut precisely
between records is inherently indistinguishable from a complete recording.

`fable_event_check <recording.evp>` opens files read-only and reports player count, frame/
package/event totals and first/last timestamps, with nonzero exit status on malformed input.
No time ordering, RNG, transport or actual world dispatch is invented. The existing bounded
creature replay now writes and reads a complete framed recording, checks its header, batch
and EOF, then materializes the native event before the original world/player lookup and
stale-reference cleanup. It retains 9,216 access cases / 3,072 world-action events; transport
allocations are still outside the core deallocation-observation window.

The new recording test covers fixed expected bytes, every cut of a two-frame example,
header-only files, signed header bit patterns, maximum frames, malformed length/package/
payload cases, error offsets, failed reads/writes/flushes with stream exception masks both
off and on, latched errors and recovery from invalid caller data. An initial test expected
an incomplete length header at cut 24; diagnostics showed the length was complete and its
body absent, correctly reported at offset 24. The test expectation was corrected; the
reader algorithm did not change for that diagnosis. Initial/diagnostic logs are preserved.

A real Windows GCC environment issue appeared: its test remained in the Windows loader,
with zero CPU, rather than reaching main. GDB attached only to that known test process;
the retained stack shows the loader hard-error/wait. Tesseract's libstdc++ DLL precedes the
compiler on the inherited PATH. An export audit of all 67 executable libstdc++ imports found
two missing fpos/Mbstatet seekg/seekpos exports in that DLL and none missing in the configured
compiler's DLL. The stalled test was closed by its verified PID/path. CTest now loads a
build-local script which prepends the configured MinGW compiler directory for every child
test directory. The host PATH and installed DLLs were not changed. The recorder test has
a 30-second CTest timeout. Evidence: `gcc_loader_stack.txt`, `gcc_runtime_exports.json`,
initial failure matrix under `work/modern_event_recording_20261018/validation_attempt1`.
The final matrix ran with the ordinary inherited host environment; the generated CTest
script supplied the compiler runtime. Cross-file invocations explicitly use that directory.

Final proof: `work/modern_event_recording_20261018/validation_completed/summary.json` PASS,
**40 provider inputs plus registration metadata unchanged**. **28/28 tests pass on MSVC
x86/x64 and clang-cl x64; 17/17 on Windows GCC**. CTest logs, snapshot manifests/signatures,
exact markers/log hashes and the GCC runtime script are archived alongside it.

`work/modern_event_recording_20261018/cross_recordings/summary.json` also PASS: all four
compilers write the same **72,280-byte** three-frame synthetic file, SHA256
`c671f106ee5a6f0d291b0da85b58e41019e494870f241ecbcbca6038e77d6a40`.
All **16** cross-reader combinations agree on 3 frames, 51 packages and 2,001 events;
**52** CLI checks cover malformed files, header-only/non-version headers, usage and missing
files. These are synthetic/offline proofs, not successful full retail recorder playback.
Existing original package-writer differential evidence remains the serializer oracle.

Next evidence-bearing integration: compile the original ProcessEventPackage and
UpdateFromEventPackageSet with their real package readers, then observe event order, stale/
duplicate timestamp handling, frame/render gating and world/network/display callback order
in a bounded composition. Actual action generation, complete dispatch, RNG/world-state
comparison and whole-engine x64 acceptance remain open. Coordinate any later game launch
with the Lua lease and live process state; no launch is needed for this next composition.


## Continuation: original main-game frame/package/system-event composition

Previous goal turn was progress (streaming retail recorder and cross-compiler proofs).
This turn stayed offline around Lua work: no game launch, input injection, install writes,
commits, pushes or parity edits. The active modernization goal remains unfinished.

Added six unchanged providers to the regular modern build: CGameEventPackage GetEvent/
GetNoEvents and CMainGameComponent ProcessEventPackage, UpdateFromEventPackageSet,
ProcessEvent and SetQuit. The first two join the wire library on Windows GCC as well;
the main-game four form `fable_reconstructed_main_game` under MSVC/clang-cl with both
reconstructed options enabled. The package bridge's outgoing native conversion now calls
the actual package readers after checking event counts, so GCC exercises them in its
existing differential/malformed-state tests. No original provider body changed.

Source inspection confirms ProcessEvent handles only GAME_EVENT_QUIT; gameplay packages
are passed to CWorld::Update. Two modern fixtures separate these evidence boundaries:

- MainGamePackageReplay calls the actual package loop/frame updater/getters while recording
  ProcessEvent, world, checksum, network and display boundaries. It checks exact event
  pointers/order and captured event values, package immutability, per-callback component
  state, world/render permission, feedback ordering, advanced frame values and checksum/
  seed/network arguments.
- MainGameSystemReplay instead links the original ProcessEvent and SetQuit. Its compiler-
  generated five-slot primary interface supplies the virtual quit boundary, recording it
  and calling the original qualified flag setter. It verifies quit handling while default
  gameplay IDs do not generate system callbacks. Frame/world processing continues after
  quit just as the original updater does.

Each covers **1,600 cases**: five initial input frames, five initial server frames, both
initial AllowRender/Quit values, and 16 recording patterns. Signed extremes, stale/equal/
unsorted timestamps, zero/full package/event counts, empty records and state spanning file
records are included. Events round-trip through the real portable recorder/codec, native
materialization, original getters and main-game methods. Decoded frame contents/order are
compared to the generated input model before dispatch. The full raw component plus guards
must differ only in named progress/render/quit fields; package and boundary receiver buffers
must remain unchanged. The modern copied header adds one manifest-recorded friend for named
field access, with no layout or provider-algorithm changes. No fixed retail member offsets,
four-byte pointer writes or decorated-symbol aliases are used in the new fixture.

An independent literal anchor from the original UpdateFromEventPackageSet parity fixture
starts input 10/server 12, sees 8/11/12/13/13/20 and must apply 11/12/13/20, render permissions
false/false/true/true, and server frames 12/12/13/20. Literal event/quit and W/C/K/U/D traces
also anchor the expanded event-content case independently of the generated state oracle.
This covers the subtle difference between sticky AllowRender and the per-update render
argument, plus state at feedback before frame advancement versus checksum/network after it.

Limits remain explicit: component/world/display storage is a bounded aligned raw fixture,
not a successfully constructed engine lifetime. The virtual shim is not the real main-game
vtable. World simulation, the actual checksum implementation, networking and display work
remain recorded dependencies. The Package fixture deliberately replaces ProcessEvent;
the System fixture supplies the actual original handler/setter. No full gameplay dispatch,
RNG/world-state oracle or new retail coverage claim is made.

Initial and final matrices both passed. The final pass also exercised the newly wired real
package getters and removed new fixture aggregate/raw-byte warning noise without weakening
warning flags or borrowed-code diagnostics. Final evidence:
`work/modern_main_game_replay_20261018/validation_verified/summary.json` PASS,
**46 original provider source inputs plus registration metadata unchanged**.
**30/30 CTests** pass on MSVC x86/x64 and clang-cl x64; **17/17** on Windows GCC.
New fixture observations match across x86/x64/Clang: **76,320 package-order callbacks** and
**37,920 system-handler callbacks**. Exact CTest logs, snapshot manifests/signatures, modern
fixture/provider input hashes, executable hashes and observation counts are archived in
that directory. Earlier successful proofs remain preserved; no broad census/coverage or
remote CI run was requested or performed. Modern-tree workflow path filters already cover
all changed files.

Next: bring in the genuine local-client frame/checksum/seed updates and checksum-selection
path to replace those recording boundaries, then actual action generation and state-bearing
world integration. The complete x64 runtime/world/RNG acceptance gate remains open. Continue
offline; any future game launch still requires checking the Lua lease and live runtime state.


## Continuation: actual client state and checksum-selection/world-sum providers

Previous turn made progress (46-provider original package/frame/system composition).
This continuation stayed offline around Lua work: no game, input injection, install edits,
commits, pushes or parity-input changes. The modernization goal remains active.

Added three unchanged original providers to the modern main-game library:
CNetworkClient::Update 0x004AE9D0, CMainGameComponent::PeekWorldChecksum 0x00416392 and
CWorld::GetChecksum 0x0049E200. This replaces the corresponding recording boundaries in a
third MainGameStateReplay fixture. The earlier Package and System fixtures retain their
original distinct scopes and case domains.

The new fixture has **12,800 cases**: the existing 1,600 frame/flag/recording shapes times
all eight LocalGame/Loading/Saving combinations. Native inline input flags are initialized
through the existing input-manager test friend; a protected derived accessor controls the
static world frame. A new manifest-recorded friend in the copied network-client header
allows named access to its local frame and checksum/seed fields. Original headers/provider
algorithms are unchanged. Initial compilation correctly rejected direct protected Frame
access in the fixture; the derived accessor resolved that fixture-only declaration error.
The initial failure log remains preserved.

Direct checksum probes establish the original short circuits: a plain local game returns 1
with a null world pointer; remote selection works with the global input-manager pointer
null. Every query must leave the full component bytes, input bytes, manager storage and
world frame unchanged. World checksum arithmetic is the real original sum of the manager's
unsigned result and signed static frame, with expected modulo-2^32 values computed through
a wider expression. Eight data pairs include signed limits, zero and wraparound.

The thing-manager aggregate is still a recording leaf. The real body reads a list pointer
at fixed +0x18 in the opaque CClassFactory prefix, which cannot be treated as a recovered
x64 layout. Its original source/header are pinned as deferred evidence. The controlled
world callback varies the leaf sum between packages (configuration XOR timestamp bits),
so successive actual client updates must store changing checksums. This is test stimulus,
not a claim about CWorld::Update or the actual thing aggregation algorithm.

Real local-client Update stores the current server frame, selected world checksum and seed;
remote Update must preserve all bytes. Two extra direct calls per case change all arguments,
including backwards signed-limit frames and alternating zero/0xffffffff checksum/seed values.
Full-component snapshots permit only the three named client fields plus the previously
established frame/render/quit changes. The direct calls are restored before framed replay.
The actual ProcessEvent/SetQuit path and the independent literal frame/render anchor remain.
World simulation, thing aggregation, display work, the real main-game vtable and constructed
component/world/input lifetimes remain unproven raw-storage/recording boundaries.

Final full matrix: `work/modern_client_checksum_20261018/validation_verified/summary.json`
PASS, **49 original provider inputs plus registration metadata unchanged**. **31/31 tests**
pass on MSVC x86/x64 and clang-cl x64; Windows GCC retains **17/17** (new gameplay-header
providers remain MSVC/clang-cl only). A final byte-view comparison cleanup removed the new
Clang raw-class memcmp warning; the related three MainGame fixtures were rebuilt/retested
on all three affected toolchains: `fixture_verified/summary.json` PASS. Full-suite console
logs remain in validation_verified; archived LastTest logs for those three toolchains cover
the final targeted tests, while GCC's archived LastTest log covers its full suite. Log scope
and hashes are explicit in runtime_observations.json. Snapshot manifests, fixture inputs and
executable hashes are archived too. Borrowed-code warnings remain visible.

Observations agree across x86/x64/Clang: **260,880 state-fixture callbacks**, **33,040 replay
leaf-checksum queries**, **11,200 direct leaf queries**, **25,600 direct client updates**.
The original two fixture observations remain 76,320 / 37,920. No coverage recount or remote
CI run; workflow filters already cover every changed modern-tree/adapter file.

Next: recover the factory-list field using the original class/list evidence, preserve the
32-bit layout while letting pointer-bearing fields widen in the modern copy, and integrate
the actual non-local thing checksum traversal including dead entries, wraparound and sentinel
mutation. Then continue actual action generation/world/RNG integration. No launch is needed
for that work; later game launches still require Lua lease/live-state coordination.


## Continuation: typed factory prefix and real non-local thing traversal

Previous goal turn was progress (49-provider client/checksum composition). This turn stayed
offline around Lua work: no game launch, input injection, installed-file changes, commits,
pushes or parity-input edits. The active modernization objective remains incomplete.

The factory-list pointer was the next real x64 hazard: the original checksum source reads
+0x18 twice and the canonical shared CClassFactory is opaque. Donor field names/order come
from engine/CClassFactory.h / struct_layouts_egor.tsv: ClassTypeInfo vector, MaxClassTypeID,
NoAllocatedClasses and ClassList. Retail uses the established STLport three-pointer vector
and one-pointer list, rather than the donor's Dinkumware sizes. The modern copied header now
uses those actual STLport field types; CClassTypeInfo remains incomplete and its operations,
allocation/destruction and factory lifetimes are deliberately not claimed. The six following
thing-manager references widen naturally with the base prefix.

`modern_thing_checksum.py` records exact single-occurrence header/source/fixture edits.
The source copy changes only the two head loads to `ClassList._M_node`, retaining the same
node structure, dead check, virtual slot-23 checksum call, addition and per-iteration sentinel
reload. No traversal algorithm or canonical file changed. Its adapted original fixture uses
type-sized/aligned storage, named list-head and Dead-field writes and a pointer-sized vptr
copy. A friend in the copied factory gives fixture access only. Before the modern fixture
runs, independent checks require vector/count/list offsets and factory/manager prefix sizes:

- x86: vector +4, counts +0x10/+0x14, list +0x18, factory 0x1c, manager prefix 0x34.
- x64: vector +8, counts +0x20/+0x24, list +0x28, factory 0x30, manager prefix 0x60.

The unchanged original VC7.1 fixture and modern x86/x64 copies all pass **80 cases** (empty/
one/three nodes, all dead masks, three sum patterns and eight sentinel-mutation cases).
The mutation case changes the factory's sentinel during a virtual checksum callback; the
loop must reread it and stop after one live callback. Standalone evidence:
`work/modern_thing_checksum/20261018T181902059581Z/summary.json` PASS, originals unchanged.
An earlier successful standalone run is also preserved. This checks compatibility of the
used prefix, not the untested meanings/operations of the preceding factory metadata fields.

The actual adapted traversal now joins fable_reconstructed_main_game as its eighth provider.
The normal CMake suite adds ThingManagerChecksumParity (same 80 cases) and
MainGameAggregateReplay (**12,800 cases**). The latter removes the prior thing-manager stub:
framed recorder bytes reach original main-game/frame/system-event handling, the actual world
checksum, actual list/dead-entry traversal, individual virtual checksum callbacks and actual
client updates. Only the individual thing values remain recorded checksum dependencies.
The previous Package/System/State fixtures keep their distinct scopes unchanged.

The Aggregate fixture derives its setup from the already adapted canonical fixture, removes
only its standalone main and adds test exports/observers. Empty/one/three-node lists and
dead masks vary with recording shape; individual values change at the world boundary as
controlled test data. An independent wider sum predicts modulo-2^32 results. Exact live-thing
callback order, direct query counts, full manager/node/head/thing immutability, client fields,
component guards and input/world-frame state are checked. The standalone 80-case test retains
sentinel mutation; replay glue disables that mutation and verifies all its buffers unchanged.
All snapshots/manifests include the exact generated glue edits/exports and original inputs.

Final matrix: `work/modern_thing_checksum_20261018/validation_verified/summary.json` PASS,
**50 original provider source inputs plus registration metadata unchanged**. **33/33 tests**
pass on MSVC x86/x64 and clang-cl x64; **17/17** on Windows GCC (gameplay-header targets remain
MSVC/clang-cl only). Full CTest logs were archived immediately by the validation driver.
Snapshot manifests/signatures, fixture input hashes, executable hashes and exact observations
are archived there. Aggregate observations match across all three affected toolchains:
**240,020 callbacks**, **12,180 replay live-thing queries**, **7,000 direct thing queries**,
**25,600 direct client updates**. Earlier fixture counts remain unchanged. The x86/x64 fixture
layout check emits MSVC's constant-condition warning under /W4; borrowed-code warnings stay
visible and warning flags were not weakened. No whole-tree census/coverage recount or remote
CI run. Workflow push/PR filters now include the new adapter and canonical fixture; parsed
filters were checked locally.

Remaining limits: aligned raw component/factory/thing storage does not prove constructed
lifetimes; factory metadata operations and the real CThing virtual table are not integrated.
Individual thing checksums, actual world simulation, RNG/state replay and raw action generation
remain open. Next trace the real CThing::GetChecksum leaf from native evidence (the PDB names
it; no matching compiled provider was found), then continue state-bearing world/input work.
Any future game launch still requires Lua lease/live-process coordination; none is planned
for that next evidence pass.

## Runtime architecture reconciliation and input include boundary

Incorporated the user's supplied architecture review into
`docs/modern/ENGINE_ARCHITECTURE.md`: module responsibilities and allowed
dependencies, explicit object ownership/borrows, persistent IDs versus validated
runtime handles, limited quest capabilities, prepared render views, separate
format/runtime/conversion representations, preserved timing and RNG, and a real
owned headless-session gate. These are contracts for future runtime work, not
claims of implemented entity storage or a complete session. Existing borrowed
replay fixtures remain evidence tools.

Corrected the modernization plan's opening status and outdated frame-rate section
to match FRAME_TIMING's fixed 15 Hz world and interpolated rendering. The scripting
redesign file is present locally at docs/engine/SCRIPTING_REDESIGN.md. Reconciled
the historical October 14 status page and modern-tree overview.

Moved RawMouseWin32.hpp and XInputPad.hpp from the portable include root to
platform/win32/include, exported only by fable_platform_win32; include spellings
remain stable for backend consumers. FABLE_BUILD_DESKTOP_TOOLS=OFF omits the title
viewer and both input probes. Existing defaults stay enabled.

Validation: 33/33 tests on MSVC x86/x64 and clang-cl x64; 17/17 Windows GCC.
Separate tool-free, reconstructed-target-free MSVC x64 configuration: 14/14 tests,
no desktop targets in generated graph. Compiler checks confirmed both backend
headers unavailable via only the portable include root and available via the
backend include root. Proof: work/modern_architecture_20261018/validation/summary.json
and headless_summary.json with build/CTest/compiler logs. Canonical 50 providers
and registration metadata unchanged. No game launches, input capture/injection,
install writes or coverage promotion. Full graph/private-header/API policy checks
remain future work as the runtime targets appear.

## Automated architecture boundaries

The previous architecture reconciliation was concrete progress. Added
rebuild/modern/cmake/Architecture.cmake to export actual configured target metadata
and run tools/modern/check_modern_boundaries.py at configure time, as an ALL build
target, and under CTest. All modern configurations now require Python >=3.10.
Existing modern-tree CI path filters include the checker and its tests.

The explicit library policy rejects unknown modern libraries, target cycles,
forbidden dependencies (including aliases/private link-only and direct-interface
injection), application dependencies from reusable libraries, and leaked public
include roots. Source checks cover declared implementation/interface sources and
public headers, reject foreign private headers, retail layout includes in portable
code, undeclared modern header dependencies, and unapproved external headers.
Standard headers are allowed; zlib/LZO implementation headers are approved only for
save internals. Macro includes and unsupported generator expressions fail for
review. Canonical/generated reconstruction internals are explicit exceptions; this
is not a complete compiler or semantic ownership verifier.

Thirty tests pass, including actual isolated CMake configure failures for cycles
and aliases and a build failure after a source-only forbidden-header edit. Test
projects use LANGUAGES NONE and interface libraries; no game/assets/compiler or
desktop window is needed. Fixed the initial false classification of third-party
zlib.h as a retail layout by excluding parity third_party headers from that name
index; the final tests cover it. CMake directory-scope link markers and aliases are
covered by a real configure test.

Full validation: 35/35 MSVC x86/x64 and clang-cl x64; 19/19 Windows GCC; separate
desktop-free/reconstruction-free MSVC x64 16/16. Final architecture build/CTest
reruns passed all five configurations after final gate edits, and hashes verify
those inputs unchanged. Original 50 providers and registration metadata unchanged.
Proof: work/modern_architecture_gates_20261018/{validation/summary.json,
headless_summary.json,final_gates/summary.json}, configured target graphs, source
hashes and archived full/targeted CTest logs. No live Linux claim or game-session
ownership claim. No launches, installation writes, input injection, coverage
promotion or changes to Lua generation.

## Generic CThing checksum retail-byte oracle and numeric contract

The previous architecture-gate turn was concrete progress. Returned to the
per-thing callback still used by the bounded main-game replay composition.
Pinned generic CThing vtable slot 23 (01239ABC -> 004C7DA0), its 108-byte body,
the 117-byte __ftol2 helper BFEA70, unsigned conversion constant 122DCB4,
and update-entry FP-control spans to the unmodified Steam image hash.

Added tools/modern/oracles/ThingChecksumRetailOracle.cpp and the hash-pinned
thing_checksum_retail_oracle.py driver. Initial fixed-address reservations
failed safely due to host startup mappings. The final oracle relocates three
address operands in a private code allocation, checks all other leaf bytes and
the entire helper unchanged, and executes neither retail startup nor imports.
No game process launches or attaches. Canonical source providers and registry
remain unchanged; original bytes/images stay only under ignored work.

The complete corpus contains 9,134 inputs per precision/rounding profile, 109,608
records across 24/53/64-bit precision and nearest/down/up/chop modes. Local flags
use deliberately inaccessible physics pointers to verify the early return.
Other cases include signed player values, no-physics integer wrap, component
order/cancellation, subnormals, NaN/Inf and signed-64 conversion boundaries.
Every call leaves fixture input buffers/global binding unchanged. Two fresh
helper processes generate byte-identical corpora.

An independent exact-rational model reproduces each addition's x87 rounding,
unsigned-base conversion and the actual CRT integer helper's shortcuts. All
109,608 outputs agree. FP status flags are archived, not modeled. The base
pressure example returns 00000000 / FFFFFF81 / FFFFFF80 under nearest at
24/53/64 bits respectively. Of 3,547 finite bounded binary64 comparisons per
precision, 2,401 differ at 24, none at 53 and 842 at 64. Ordinary truncation
misses the helper's -2^63+0.5 nearest case, whose native checksum is zero.

Native normal-update 41829C..4182BB and forced-update 417798..4177B7 explicitly
select precision 53 and rounding nearest. The matching existing reconstructed
update sources and fable_fpu.h were read only. This proves entry controls, not
control preservation through every downstream callback. It gives the upcoming
normal-path port a measured numeric context rather than choosing host defaults.

Eight asset-free regression tests cover measured numeric outputs and malformed
corpus/code archives; wired into Windows/Linux modern-tree CI without retail
assets. Tests pass locally; no local Linux execution claim. The modern CMake
source/runtime targets are unchanged, so no unrelated behavior matrix rerun.

Frozen proof: work/modern_thing_leaf/20261018T185619469406Z/summary.json, with
source/driver/executable hashes, original/relocated spans, profiles, repeat output
and exact-model analysis. Corpus SHA256:
93bcbc7b6fd48e5fb9b8b236c82b59b1fb69823423db0d3f5996c1e466c2b1e0.
modern_inputs_unchanged.json verifies all existing 50 provider inputs and
registration against the preceding architecture validation checkpoint.
See docs/modern/THING_CHECKSUM_NUMERICS.md for field offsets, arithmetic order,
helper boundaries and the proof record format. No modern checksum provider or
live class/physics lifetime is claimed yet; these remain the next implementation
work. No Lua changes, input injection, install writes or coverage promotion.

## Portable simulation checksum result implementation

The preceding turn established the retail oracle and exact numerical contract.
Added the first bounded `fable_simulation` behavior: generic CThing checksum
results on caller-owned scalar metadata and optional position/velocity values.
Public interfaces contain no engine pointers, Windows/GPU/Lua types or retail
layouts. Explicit precision and rounding enums cover all 12 measured modes,
defaulting to the verified 53-bit/nearest update-entry contract; invalid enum
values return an explicit error. No pointers/state are retained or allocated.
The architecture registry grants this target no engine dependencies.

The private implementation represents exact multiples of 2^-149 in five uint64
limbs. Every finite input/addition fits the proved 320-bit bound. It decodes
binary32 bits, follows the native component order and unsigned-base correction,
rounds after each measured operation, then preserves the actual __ftol2 result
shortcuts. It emits no FP arithmetic, alters no host FP state and is not a
general x87 emulator. Native FP status flags/unmasked exceptions remain outside
this value API's contract. No raw native bytes or inline assembly are linked.

Asset-free golden cases use literal native observations for precision pressure,
signed-64 boundaries, nonfinite/subnormal inputs, early returns and invalid modes.
Valid cases run under all four host rounding modes while preserving an existing
inexact flag. The same executable optionally reads the frozen native corpus,
compares each record four times and reports the first differing record.

All 109,608 native records match on MSVC x86, MSVC x64, clang-cl x64, Windows GCC
and a desktop-free/reconstruction-free x64 configuration. Independent extraction
of original expected words verifies every generated result stream byte-for-byte:
e0033fe5fdf57c88d49f08247188e81e6f3d5233b093d8d1e92e9ea540f7865a.
Full CTest suites pass 36/36 per MSVC/clang configuration, 20/20 GCC and
17/17 desktop-free. Source/proof inputs and the existing canonical provider
inputs remain unchanged during validation. Proof:
work/modern_thing_kernel_20261018/validation/summary.json and
retail_result_words.json, with build/CTest/comparison logs and configured graphs.

Canonical counts remain 50 provider units; this new modern implementation is
not a parity-provider promotion. The reconstructed replay's per-thing callback
still needs a typed class-metadata/physics adapter. Entity/factory ownership,
derived overrides, full world simulation, RNG and a representative headless
session remain open. No game launches, install writes, input injection, Lua
changes, commits or coverage promotion.

## Typed generic thing-checksum adapter and aggregate replay integration

The previous turn implemented and validated the portable checksum calculation.
Added fable_thing_checksum_bridge in rebuild/modern/integration. Its public
header forward-declares old CThing/manager types and exposes only a synchronous
borrowed call plus explicit mode/errors; no old layout enters portable simulation.
The manager is passed explicitly. Type metadata is bounds-checked before lookup,
local types return before reading physics, and typed component values are copied
by representation to preserve signaling-NaN bits/host FP state. Missing metadata
and invalid numeric modes produce distinct errors. Arbitrary pointer validity,
allocation/destruction and live lifetime are caller obligations, not claims.

Build-only snapshot patches add narrow friends and split the copied CThing
opaque tail into measured physics/definition/signed-player fields. The copied
factory's type-info element remains incomplete in traversal units; only the
private integration header defines bounded CClassTypeInfo and CTCPhysicsBase
representations. Unused allocator/definition/flag tails carry layout only.
Canonical headers/providers/fixtures remain untouched. The generator records
these new patches and replay declarations in its immutable manifest.

Runtime guards pin x86/x64 type-info size 20/32 (Local 9/13, Name 16/24),
physics size 64/88 (Position 12/24, Velocity 40/52), and CThing type 16/32,
PhysicsTC 96/136, DefGlobalIndex 140/184, PlayerNumber 144/188. The initial
x64 estimate omitted the component map's comparator/dirty tail alignment; the
32-byte map explains the extra eight bytes, and the guard caught that estimate
before any parity claim. Native x86 offsets agree with instruction operands.
Retail RTTI from vtable125D2C4 confirms the zero-displacement primary chain
CTCPhysicsBase -> CTCBase -> CBaseClassNonCopyable -> CBaseClass. Evidence:
work/modern_thing_bridge_20261018/physics_rtti.json, pinned Steam image hash.

The adapter golden/corpus fixtures check local early return with an inaccessible
physics pointer, null/short metadata, all modes, missing-mode errors, full borrowed
state immutability, host rounding and an existing inexact flag. All 109,608 native
records match on MSVC x86/x64 and clang-cl x64, each under four host rounding
modes. All output words equal the frozen retail stream:
e0033fe5fdf57c88d49f08247188e81e6f3d5233b093d8d1e92e9ea540f7865a.

MainGameAggregateReplay's slot-23 callback now invokes this adapter and the
portable numeric code. It no longer returns values[index]. Test stimulus sets
representable high-byte position plus low-byte definition, with type i/player -i
canceling, to preserve the independently expected frame sums. Immediate before/
after checks prevent later stimulus from masking mutations. All 12,800 scenarios
retain 240,020 callbacks, 12,180 replay thing queries, 7,000 direct thing queries
and 25,600 direct client updates. A separate counter proves 19,180 actual bridge
queries. The original 80-case traversal fixture keeps its stub and sentinel-
mutation tests. This is bounded generic behavior composition, not a genuine
complete CThing vtable, constructed entity lifetime or real world update.

Full suites pass 37/37 MSVC x86/x64 and clang-cl, 20/20 Windows GCC, and 17/17
desktop-free/reconstruction-free x64. GCC/desktop-free exclude this legacy
gameplay adapter. Proof: work/modern_thing_bridge_20261018/validation/summary.json,
runtime_observations.json, complete CTest/build/corpus logs and configured graphs.
Inputs remain unchanged during validation; active generated callbacks were checked
to use ModernReadThingChecksum and omit the former direct result return.
Canonical provider count stays 50. No game launches, input injection, install
writes, Lua edits, coverage promotion, commits or pushes. Entity/factory/physics
ownership, derived overrides, world simulation/RNG and an owned headless session
remain open.


## Modern registration ownership evidence, 2026-10-18

Added tools/modern/thing_registration_retail_probe.py and
[THING_REGISTRATION](../../modern/THING_REGISTRATION.md). The frozen bounded native
probe passed 16,384 construct/reinsert/unregister transitions with exact list,
counter, iterator, notification and allocation/free ordering checks. Four mutated
instruction controls reject, and repeated seed observations match. Proof:
work/modern_thing_dispatch_20261018/validation/final_registration_probe.json and
final_negative_controls.json. The probe uses Unicorn and controlled allocator/
notification callbacks; no retail startup or complete object lifetime executes.

Every one of 2,625 manifest vtables parses. All 17 with CThing ancestry use generic
checksum slot 23 at 004C7DA0. This closes the override question for these listed
tables, not unlisted/dynamically modified dispatch. The manifest's 00A37300
unregister annotation is misleading: it removes and reinserts with three stack
arguments. Actual unregister 00A37160 receives ECX=object, stack=factory, notifies
before unlinking and never frees the object. Constructor bool chooses front/back;
both branches register. Do not use these manifest signatures as ownership APIs.
Next: trace CThing destruction/creation and factory lifetime before implementing
owned session objects. No canonical provider/count changes, C++ runtime edits,
Lua edits, game launches, installation writes, commits or pushes this checkpoint.


## Native empty thing/factory lifecycle reference, 2026-10-18

Added tools/modern/thing_lifetime_retail_probe.py and
[THING_LIFETIME](../../modern/THING_LIFETIME.md). Native factory initialization/container
helpers/teardown and empty-component CThing construction/destruction execute in
Unicorn, with controlled allocator and factory notification callbacks. All 64
seeds x 32 batches pass: 17,626 constructions/destructions, 8,763 reinsertions,
17,626 notifications and all 70,852 allocations freed. Four mutated instruction
controls reject and repeated seed observations agree. Frozen proof:
work/modern_thing_lifetime_20261018/validation/lifecycle_probe.json and
negative_controls.json; all three imported tool dependencies stayed hash-stable.

CThing constructor prepends; optional ConstructFromParams reorder appends.
Constructor definition/player remain untouched until later initialization.
Factory initialization is A373B0, teardown A37360 despite misleading manifest
names. Factory teardown releases registration storage, not registered entities.
The real CThingManager create/delete slots are 5202F0/5215F0, forwarding UID data
to the manager +58 index via 52A200/52AFB0. Recover that index and component cleanup
before claiming real ownership. Controlled notification is not the real index.
Empty strings/control blocks only; actual allocator, derived deleting wrappers,
nonempty components/deletion queues and gameplay initialization remain unproved.
No C++ runtime/parity inputs or coverage counts changed at this checkpoint;
no game launches, Lua edits, installation writes, commits or pushes.


The final lifecycle probe also executes CThing slot-0 wrapper 004CD3E0, alternating
bare destruction and flags 0/1/2/3/255. It passes 14,687 wrapper calls and 8,821
controlled object-storage release requests, always after native destruction and
unregister complete. Bit 0 alone selects release; even flag 2 retains storage.
All prior lifecycle totals remain unchanged. Five mutated controls now reject,
including the wrong deleting-flag bit. Superseding frozen reports:
work/modern_thing_lifetime_20261018/validation/final_lifecycle_probe.json,
final_negative_controls.json and summary.json. Exact imported tool sources are
archived under validation/sources. The actual allocator and derived wrappers
remain outside this proof; the base slot-0 wrapper is now covered.


## Portable persistent thing-ID index, 2026-10-18

Added simulation/ThingIdentity.hpp, ThingIdIndex.hpp and ThingIdIndex.cpp to the
existing portable fable_simulation library. Persistent 64-bit IDs and runtime
slot/generation handles are distinct types. The session owns the index records;
entities remain separately owned. Insert retains the first handle on duplicate
UID, find returns an optional value copy, erase removes by UID without an object-
identity condition, and clear drops only records. The index is noncopyable/movable,
requires external synchronization, and does not issue/validate handles or UIDs.

New tools/modern/thing_uid_index_retail_probe.py executes real CThingManager
create/delete wrappers, original tree insert/erase/lower-bound and red/black
balancing bodies in Unicorn with controlled malloc/free. Initial manager/tree/
thing prefixes remain fixture storage, not constructed full engine objects.
An independent model checks every mapping, parent/color/black-height rules,
ordering, size/min/max and allocation balance. Native corpus: 16 seeds, 8,941
operations and 93,167 mapping observations; 2,960 node allocations all freed.
There are 1,190 different-token duplicates retaining the first pointer and 1,311
successful erases notified by a different token. All erases use the real delete
wrapper; 2,085 insertions use the real create wrapper. Four instruction mutations
reject; repeated native seed output agrees. Seven altered/malformed comparison
corpora reject with a field/line/UID diagnostic.

The complete explicit corpus matches modern C++ on MSVC x86/x64, clang-cl x64,
Windows GCC and desktop-free/reconstruction-free x64. Full CTest suites pass
38/38 per MSVC/clang configuration, 21/21 GCC and 18/18 desktop-free. No local
Linux run is claimed. Full proof: work/modern_uid_index_20261018/summary.json,
retail_probe, build_validation, corpus_validation, negative_controls.json and
transport_validation. Logs/LastTest logs/target metadata and exact source inputs
are archived; compiled/oracle inputs still match current sources. README updates
after tests are recorded separately. The 50 canonical providers plus one event-
registration-table input remain unchanged; modern code earns no canonical credit.

Read [THING_UID_INDEX](../../modern/THING_UID_INDEX.md) for scope, addresses, API and
reproduction. The native lower-bound helper plus explicit equality establishes
mapping observations; this is not a recovered public quest/GetThingWithUID API.
The existing aggregate replay still uses borrowed engine prefixes. Next: entity
ownership and validated handle generations, UID initialization/remapping and
component cleanup, then a representative owned headless gameplay scenario.
No game launches, Lua changes, installation writes, coverage promotion, commits
or pushes at this checkpoint.


## Owned initialized thing state and validated handles, 2026-10-18

Added simulation/ThingStore.hpp and ThingStore.cpp to fable_simulation. The session
owns a fixed-capacity slot array, global/per-type registration links and its UID
index. ThingHandle now has owner/slot/generation fields. Every access validates
all three; separate stores cannot alias handles, clear/reuse does not revive old
handles, final-generation slots retire permanently and the owner-ID issuer
saturates instead of wrapping. Store copy/move are disabled. Reads and order
extraction return copies. Failed UID-index allocation rolls back prepared state
and both lists. Duplicate UID deletion remains key-only, matching native behavior.
This owns initialized value state, not legacy CThing/CTC instances. Creation does
not run ConstructFromParams or assign/remap UIDs; setters do not implement Kill
or movement. Actual gameplay initialization/component lifetimes/ticks remain open.

The registration probe optionally exports explicit operation/global/type-order
observations. All 64 trace hashes match the frozen pre-export reference. All
16,384 native transitions match the owned store public API. All 109,608 numeric
records also match through owned create/read/physics replacement/checksum/destroy
in four host rounding modes, preserving FP controls/flags and copied physics bits.
Four modified native instruction controls reject. UID-index regression (8,941
operations/93,167 mappings) also passes after adding the handle owner field.

Full suites pass 39/39 MSVC x86/x64 and clang-cl, 22/22 Windows GCC, 19/19 desktop-
free/reconstruction-free x64. Each build passes both complete optional store
corpora and the existing UID-index corpus. Ordinary tests cover capacity, type/
order errors, copied reads, no partial output, stale/foreign handles, generation
retirement, identity saturation, duplicate teardown, allocation-failure rollback
and dead/local/physics checksum composition. No local Linux run is claimed.

Proof: work/modern_thing_store_20261018/summary.json, validation/summary.json,
native_registration.json, registration.txt, negative_controls.json and
uid_regression/. Complete logs, LastTest logs, target metadata and exact tested
source inputs are archived. Final updates consist of documentation and one
ThingIdentity comment (explicitly tracked); runtime code matches tested inputs.
Fifty original providers and the registration table remain unchanged. Read
[THING_STORE](../../modern/THING_STORE.md) for scope/contracts/reproduction. The aggregate
compatibility replay still uses borrowed prefixes; this is not the representative
owned gameplay-session gate. Next: recovered UID initialization/remapping and
component cleanup, then explicit ticks with representative recovered behavior.
No game launches, Lua changes, install writes, coverage promotion, commits or pushes.


## Recovered UID allocation and serialization remapping, 2026-10-18

Added simulation/ThingUid.hpp and ThingUid.cpp. Caller-owned global/local UID
counters and narrow map-UID lookup/map-sequence interfaces replace engine/world
objects. The allocator preserves native prefix ADD (not OR), unsigned 64-bit wrap,
map-specific early local consumption, level-less zero results, positive-map
lookup/sequence order, and nonpositive-map behavior. The remapper preserves
SerialisingGameState bypass, local/global prefix precedence and map-relative
conversion of the low-40-bit identity. No RNG, clocks, retail layouts or entity
pointers enter the public API. Provider exceptions preserve consumed counters.

The definition flag is LevelLess: native Transfer 676BE0 reads +41 bit3 and
transfers string 125B628, while ForceNoSerialise uses bit6/string125B578. The early
working API name was corrected from that schema evidence before full validation.
Native CreateUID extracts the LevelLess flag and type locality. Constructor
stores initialize local/global sequences to 0/1, but full manager initialization
is not claimed. Read THING_UID_ALLOCATION.md before binding this to definitions.

New thing_uid_allocation_retail_probe.py executes original allocator/remapper,
CreateUID, GetMapUID, map-sequence increment and CRT multiply/composition. Only
map-object virtual lookup is controlled. All 5,160 requests and 992 capability
observations match an independent model: 820 direct allocator calls, 820 through
CreateUID, 3,520 remaps, 807 map-UID reads and 185 sequence operations. Counter
values at callbacks and all final counter/result fields are compared explicitly.
Four instruction controls reject; repeated 3,160 boundary cases agree exactly.

The entire corpus matches C++ on MSVC x86/x64, clang-cl x64, Windows GCC and
reconstruction-free/desktop-free x64. Full suites pass 40/40 per MSVC/clang build,
23/23 GCC and 20/20 desktop-free. Ordinary tests compose generated IDs and relative
round trips with owned state, lookup, destruction and slot reuse, and verify
partial consumption when a provider throws. This is identity-component composition,
not full entity initialization or a whole saved-world round trip. No local Linux
run is claimed. Full proof: work/modern_uid_assignment_20261018/summary.json,
retail_probe/, negative_controls.json, definition_flag_evidence.json and validation/.
Exact inputs, full logs/LastTest logs and configured target metadata are archived.
Compiled/oracle inputs remain unchanged; documentation updates are tracked apart.
The 50 canonical provider inputs and their registration table remain unchanged.

Next: CheckAndFixUID collision repair/loaded-counter advancement, real map
providers, full ConstructFromParams/level-script integration, component cleanup
and explicit world ticks. The owned representative gameplay-session gate remains
open. No game launches, Lua edits, install writes, coverage promotion, commits or
pushes at this checkpoint.


## 2026-10-18: modern loaded-UID repair and owned map identity

Implemented CheckAndFixUID behavior in portable ThingUid. Nonzero loaded IDs
advance their original local/global or loaded-map counter before index lookup.
Only a non-null indexed thing collides, including a dead thing. A collision uses
the original prefix and existing allocator once, without a retry or index edit.
Zero returns immediately. Narrow map/index capabilities replace manager/world
access; provider exceptions preserve prior counter effects.

ThingUidMapTable owns copied map UID metadata and optional sequence counters.
Reverse lookup skips index zero, takes the first match and ignores load flags.
Loaded means Used && Activated && map presence. Allocation ignores those flags
and increments the full-width sequence modulo 2^64. Reads return copies and
indices/absent counters are checked. This is owned identity state, not terrain
or complete CMap storage. Ordinary tests compose repair before owned entity
registration while preserving the indexed dead original.

The new offline native probe executes original repair and every callee, with no
substituted native calls. Its explicit metadata and small index-tree fixtures
produce 4,200 requests, 1,709 replacements and 12,839 ordered observations. In
548 cases the replacement is already occupied and repair still stops. Six native
instruction mutations reject; a second complete capture is byte-identical.

All repair results/counters/events and the earlier 5,160 allocation/remapping
requests match on MSVC x86/x64, clang-cl x64, Windows GCC and desktop-free x64.
Full suites pass 41/41 per MSVC/clang build, 24/24 GCC and 21/21 desktop-free.
No local Linux run is claimed. Proof: work/modern_uid_repair_20261018/summary.json,
retail_probe/, negative_controls.json and validation/ with frozen source inputs,
full logs, LastTest logs, target metadata and hashes. Fifty canonical provider
inputs and their registration table stayed unchanged. Portable extraction earns
no canonical coverage. See docs/modern/THING_UID_REPAIR.md.

Next: native ConstructFromParams at 004CA010, full level-script initialization,
map/session assembly, component cleanup and explicit ticks. A static entry review
is pinned under next_initialization_review.json, not runtime acceptance. The
per-type helper 0051E580 is a metadata read, not a sequence increment. The owned
representative gameplay-session gate remains open. No game launches, installed
game writes, Lua edits, coverage promotion, commits or pushes at this checkpoint.


## 2026-10-18: native thing initialization and component-attachment reference

New thing_initialization_retail_probe.py executes original ConstructFromParams
004CA010 and native UID/map, registration reorder, counted-definition/string
reference management, generic AddComponent, sort/bitset, stage dispatch and update-
list insertion helpers. External definition lookup, interning/script lookup,
definition virtual slot15, editor query, component factory/virtual methods and
node allocation/free remain explicit controlled services. Real derived component
constructors/initializers/destructors, replacement, reallocation, complete
construction-to-teardown and an owned C++ spawn are not claimed.

The independent model agrees on 3,080 requests and 30,112 ordered state/call
observations: 1,648 attachments, 47 duplicate destructions, 633 stage calls,
579 update nodes, 1,280 reorders and 414 old definitions retained after a
positive-index lookup miss. String/definition references, node balances, both
registration lists/counts, component vector/membership/caches, update order,
stack/callee-saved registers and untouched object bytes are checked. A nonempty
serialization section is cleared through native string assignment.

Initialization reads a world-frame value, not an OS clock; assigns parameters,
clears links/LevelPersistent/serialization section, resolves a nonzero definition,
assigns UID, reads per-type flags, normalizes signed player bytes to 0..3 or 4,
then invokes definition behavior/script lookup. It conditionally requests
CTCEditor and always requests CTCMapwho. Failed lookup preserves an old definition;
this caller ignores the resolver return. These component requests do not turn
the overall return false on the exercised missing/refused-component paths.

The native non-replacement attachment path checks permission before its factory,
destroys a newly returned duplicate, caches selected component roles, sorts by
type, sets the membership bit, dispatches stages 1/2/3 for non-dead components,
and appends update membership when requested. Stages 4..7 are not clamped to 3.
The fixture component types stress generic branches; they are not the actual
CTCEditor/CTCMapwho implementations. The component vector starts empty with spare
capacity, and interning/refcount service fixtures have explicit retained owners.

Ten altered-native controls reject and a second full capture is byte-identical.
Authoritative proof: work/modern_thing_initialization_20261018/validation/final_summary.json,
final_native/{summary.json,corpus.jsonl}, final_negative_controls.json and
final_sources/. Earlier captures preceded final nonempty-section/event checks
or the complete mapped map-index-range fixture. Final observable results agree. Compiled modern
inputs and the 50 canonical provider inputs plus registration table/selected
headers match the preceding UID-repair checkpoint. The existing five-build
suites were not repeated for this probe/documentation-only change; no new C++
initialization parity or canonical coverage is claimed. Read
docs/modern/THING_INITIALIZATION.md for exact scope and ordering.

Next: owned generic component attachment, stage dispatch and complete teardown,
then connection to entity creation and the representative headless session.
ThingStore::create_initialized still only inserts caller-initialized values.
Keep launch/Lua setup untouched. No game launch, install write, Lua edit, canonical
promotion/recount, commit or push at this checkpoint.


## 2026-10-18: component replacement and prepared ownership teardown reference

New thing_components_retail_probe.py runs original CThing/factory construction
and destruction, AddTC replacement, sorting/membership, parameter/stage dispatch,
update-list insertion/removal, pending weak-owner cleanup and the CTCBase deleting
wrapper. Component factory/type/role/update/parameter/stage methods, delete
notification and allocation remain controlled. Component constructors/derived
cleanup, vector growth, scheduling/enqueue and reentrancy are not claimed.

The final 32-seed replay matches an independent model on 9,394 operations and
9,177 full callback snapshots: 3,216 factory calls, 1,973 component releases,
1,109 parameter callbacks, 1,666 stage callbacks, 1,225 queued owner/component
references and 1,213 owner destructions. Every operation compares result,
sorted entries, membership words, role caches, update order and pending pairs;
callbacks compare the same explicit state. Weak-reference, thing/registration/
string counts, stack/callee-saved registers and all tracked allocation release
are checked. The fixture-owned global queue sentinel is released separately.

Replacement updates the new role cache before removing all matching old update
and pending entries, releases each pending owner reference, destroys the old
component, then replaces its entry and dispatches parameters/stages/update query.
Owner teardown removes its pending records and disables adding, clears each
membership bit before sorted component destruction, and nulls each vector entry
afterward. It clears the logical vector before unregister; old update identities
remain through unregister and their nodes are released later. Prepared parameters
can change ToBeRemoved and HasFrameUpdate before the native decisions. Marking
removal during stage one does not cancel the remaining stages chosen at entry.

Correction: component byte +8 is ToBeRemoved, distinct from entity Dead. Donor
CTCBase fields and native 00686800 constructor/004C73E0 dispatch establish this.
The initialization probe now uses tc_to_be_removed instead of tc_dead. Repeating
all 3,080 initialization cases preserves every JSON field/value and event except
the key rename. See removal_flag_correction/ and removal_flag_evidence.json.
Earlier handoff text calling this a dead component was a naming error, not a
change to the executed byte or branch semantics.

Thirteen altered-native controls reject and both JSON/numeric full replays are
byte-identical on repetition. Authoritative proof:
work/modern_thing_components_20261018/validation/{summary.json,final_native/,
final_negative_controls.json}; native sources are archived in final_native/sources/.
Earlier retail_probe/ captures preceded parameter/stage side-effect cases.
Numeric format FABLE_THING_COMPONENTS_V1 is documented in THING_COMPONENTS.md for
the next C++ comparison. Full modern compiled inputs and selected canonical
provider/header inputs match the UID-repair checkpoint; no compiler suites were
repeated for this native-tool/documentation change. No C++ component parity or
canonical coverage is claimed. No game launch, Lua edit, installed-game write,
canonical recount/promotion, commit or push.

Next concrete work is owned C++ components and a session-owned weak pending queue,
with factory transfer of unique ownership, validated entity/component handles,
typed CTCParamsBase-style parameters, permission-before-factory ordering,
replacement invalidation, exact callback visibility and teardown before UID/list
removal. Compare through the public owned API against all 9,394 native operations
and 9,177 snapshots, then run the five build configurations. Preserve ToBeRemoved
and the parameter/stage mutation cases. Do not treat caller-initialized ThingStore
values as a recovered spawn. Complete derived behavior, initialization/session
assembly, explicit ticks and representative gameplay remain later gates.


## 2026-10-18: owned modern generic components and weak removal queue

Implemented portable ThingComponent/Params/Factory, ThingComponents and the
session-owned ThingComponentRemovalQueue in fable_simulation. Factories transfer
unique ownership; no raw component object, reference or borrowed view is exposed
by collections/ThingStore. Parameters and factory names are borrowed only for
one call. Handles validate parent incarnation, type, generation and collection
identity. Weak role/update/queue references do not keep entities alive. Queue and
collection copy/move are disabled; the queue must outlive its collections.

ThingStore lazily owns each entity's component collection. destroy/clear execute
the terminal component phase before UID-index/list removal, then release
collection containers afterward. Real Store goldens confirm that destructor
callbacks see the registered parent and expired dying component handles. Read
snapshots preserve sorted entries, membership, cached identities and update
ordering as value copies. Collection teardown purges only its own weak records,
including with a separate same-parent collection borrowing a standalone queue.

The generic path preserves permission-before-factory, duplicate candidate
destruction, role preparation before old destruction, all matching update/queue
removals, membership/parameter/stage order and later update query. ToBeRemoved
is separate from entity Dead. Parameters can change removal/update choices;
marking removal during stage one does not cancel the remaining selected stages.
Owner teardown clears each bit before sorted destruction, nulls entries afterward
and retains expired update identities through unregister. Typed parameters do
not expose retail CTCParamsBase layouts. No World/OS/Lua/graphics dependency.

Runtime generations never wrap. Every bound candidate consumes an incarnation,
including a role callback that throws after publishing a weak cache identity.
A later successful retry cannot revive that handle. Goldens cover failed-factory
and failed-role cleanup, accepted ownership after a parameter exception,
wrong-parent candidates, generation exhaustion, snapshots, duplicate/removal
records, foreign collections and entity reuse. Mutation reentry is explicitly
unsupported; read-only callback snapshots are supported. Native allocator/error
or reentrant behavior is not claimed.

All 9,394 native component operations and 9,177 callback snapshots match the
public owned collection API on MSVC x86/x64, clang-cl x64, Windows GCC and
desktop-free/reconstruction-free x64. The corpus uses a prepared-owner adapter
that observes unregister after real collection teardown, then removes its entity;
it does not instrument production ThingStore notification or recover spawning.
Separate goldens exercise actual Store-owned collections and destruction order.
Full suites pass 42/42 per MSVC/clang build, 25/25 GCC and 22/22 desktop-free.
Each also retains all 16,384 registration transitions, 109,608 checksum records
in four host rounding modes, 4,200 repair requests/12,839 observations and
5,160 allocation/remap requests/992 observations. No local Linux execution.

Proof: work/modern_owned_components_20261018/{summary.json,validation/} with exact
sources, full logs/LastTest logs, configured targets, binary hashes and complete
corpus markers. Code/oracle/canonical inputs remained stable throughout testing.
Fifty canonical provider inputs plus their registration table and selected
headers are unchanged; portable extraction earns no canonical coverage. Native
proof remains work/modern_thing_components_20261018/validation/ with thirteen
mutations rejected and repeat-identical corpora. Read THING_COMPONENT_STORE.md.

Next: connect typed definition/string data and the recovered initialization
orchestration, preserving registration/UID visibility and callback order. The
current create_initialized API still inserts caller-initialized state; generic
type-2 components do not yet populate ThingState.physics. Real derived behavior,
complete level-script creation, reentrant callbacks, pending-removal scheduling,
map/session assembly, explicit world ticks and representative gameplay remain
open. No game launches, installed-game writes, Lua edits, canonical recount or
promotion, commits or pushes at this checkpoint.


## 2026-10-18: native generic creation, callback mutation and delayed UID publication

New thing_creation_retail_probe.py composes original generic Create 004CA1D0,
CThing construction, ConstructFromParams, OnCreate 004C9560, actual UID-tree
notifications and complete destruction. The fastcall wrapper allocates native
size 0x98/type15 after its always-true allocation guard; these are oracle facts,
not proposed 64-bit layouts. It returns initialized registration before UID
publication. CClass::NotifyCreated 00A36F80 forwards to manager slot2/005202F0
only after OnCreate's component callbacks; the probe runs real tree operations.

OnCreate increments its low-three-bit stage before checking owner Dead. Already-
Dead objects return false and do not publish. Live objects refresh a positive
definition index, apply loading/persistence flags, read SerialisingGameState
(return discarded), prepare a cached driver, copy component identities, read
ToBeRemoved live before each slot4 callback, then publish. Pause flags follow
publication. A positive-index lookup miss retains an old definition reference but
skips the global-loading setting; GameStatePersisted still uses a retained def.
Default script strings are not refreshed by OnCreate's second lookup. Native
Transfer evidence identifies definition+42 bit0 IsLoadableGlobal and bit1
GameStatePersisted; evidence is static, not a full serializer run.

Valid callback reentry adds a component through native AddTC. It catches up to
stage1 immediately and is absent from the copied outer iteration, so it is not
called twice. Actual native component vectors grow from zero and can grow again
during that reentry. Marking a later component ToBeRemoved skips its callback.
Marking the OWNER Dead during a callback does not cancel remaining callbacks or
publication: the native method checks owner Dead only at entry. This was observed
in 68 cases that publish with Dead set at return. A new reentrant child still
catches up on that path. Do not add a later safety-style owner-Dead gate.

The independent model agrees on 1,277 cases and 13,906 boundary observations:
1,276 created registrations, 715 publications, 561 already-Dead rejections and
101 reentrant components. Thirteen instruction controls reject, including a
real premature tree insertion before callbacks in a no-driver case (the original
serializing read remains). The complete final replay is byte-identical on repeat.
The final corpus includes owner-death-during-callback cases; earlier initial_probe
and retail_probe captures preceded them. Definition/script/component services,
virtual definition construction and allocation remain controlled. Real derived
behavior, definition component-set construction, script text/execution and later
activation phases are not claimed. Script observations are prepared reference
tokens/refcounts; do not claim decoded-text parity from them. Raw flag bytes
include allocator poison in retained storage bits; do not copy undefined native
storage or raw heap sizes into modern public interfaces.

Proof: work/modern_thing_creation_20261018/validation/{summary.json,final_native/,
final_negative_controls.json,sources/}; definition_flags.json and discovery.json
are retained beside it. Sources/dependencies, exact native spans and corpus hashes
are pinned. Modern compiled sources and selected canonical provider/header inputs
match the owned-component checkpoint; no compiler suites were repeated for this
probe/documentation-only change. No new C++ creation parity or canonical coverage.
Read docs/modern/THING_CREATION.md.

Next concrete work: extend the owned component path for valid reentrant attachment
and copied owner-phase iteration, then connect owned registered-but-unpublished
creation, typed definition/string data, explicit world-frame input and delayed UID
notification. The current collection mutating_ guard deliberately rejects all
mutation reentry and is insufficient for this recovered caller. Replace blanket
exclusion with safe active-type/phase rules; retain protection against destroying
or replacing a currently executing component. Snapshot identities, then re-read
live removal flags per callback. Keep initialize-stage catch-up distinct from
owner-phase dispatch and preserve entry-only owner Dead checking.

ThingStore::create_initialized must keep its existing complete-value contract;
it cannot substitute for the original Create workflow. Constructor registration,
field readiness and UID publication need explicit separate state. Do not
conditionally skip native key-only deletion solely because publication never
occurred. No implicit collision repair is present in the base creation path.
Real definition component construction 004CA930 is a following data-integration
gate: native +39 pointer/+3D signed count, 9-byte entries with name key +0 and
replacement byte +8, reserve current+count, optional forced replacement from
thing typeFlags bit1/world byte+102, then StartKillLocked from def+41 bit2 into
thing+91 bit6. This is currently STATIC REVIEW ONLY; its virtual hook is controlled
in the creation probe. Recover/validate typed data before claiming that behavior.

No game launches, installed-game writes, Lua edits, canonical recount/promotion,
commits or pushes. Full creation integration, real derived gameplay, later phases,
scheduling, maps/session, explicit ticks and representative gameplay remain open.


## Owned staged generic creation, 2026-10-18

`ThingInitialization` now connects owned registration, typed definition/string
snapshots, explicit world-frame input, UID allocation and delayed OnCreate
publication. Constructor registration and parameter readiness are distinct from
UID lookup. `create_initialized` retains its complete-value/immediate-publication
contract. No raw runtime views, retail layouts, Lua or platform dependencies escape.

Component callbacks can attach other types with immediate catch-up. Owner-phase
iteration copies identities and reads current ToBeRemoved flags per entry. The
owner Dead check remains entry-only; callback death does not cancel publication.
Executing-type replacement and active-owner destruction are refused. A disposal
guard spans component destructors and metadata/container cleanup; recursive owner
destruction returns false and attachment returns refusal without invoking a factory.
Exceptions clean up partial creation without rolling back consumed UID counters;
OnCreate exceptions retain owned state. Key-only deletion also applies to an
unpublished duplicate.

All 1,277 native creation cases/8,800 comparable boundary states match across
MSVC x86/x64, clang-cl, Windows GCC and desktop-free/reconstruction-free x64.
Full suites pass 43/43 MSVC/clang, 26/26 GCC and 23/23 desktop-free. Each build
also passes 9,394 component operations/9,177 snapshots, 16,384 registration
transitions, 109,608 checksum records in four host rounding modes, 4,200 UID
repair and 5,160 allocation/remap requests. Exact sources, binaries, full logs,
CTest logs and target metadata: `work/modern_staged_creation_20261018/final_validation/`.
Aggregate: `work/modern_staged_creation_20261018/summary.json`. The older
`validation/` predates the full-disposal guard. Only the simulation README changed
after the final test freeze, as documented in the aggregate. Fifty canonical
provider inputs and their registration table are unchanged; no coverage promotion.

Next: recover nonempty native definition construction at 004CA930, then introduce
its typed component-set data and real construction path. The creation host's
construct_definition hook is still controlled, not a production implementation.
The static +39/+3D/9-byte-entry review is not native execution proof. Real derived
behavior, physics, later activation, removal scheduling, maps/session and explicit
world ticks remain. This is not a representative headless gameplay scenario.

No game launches, installed-game writes, Lua changes, commits or pushes occurred.


## Native nonempty definition construction, 2026-10-18

The next reference gate is now executed, not static review only:
`thing_definition_retail_probe.py` runs original 004CA930, actual vector
reservation/copy and string temporary destruction. Allocation, name lookup and
AddTC are controlled; it does not yet execute definition-driven actual AddTC.
All 1,592 cases/26,810 entries/55,212 observations match, including 901 nonempty
sets and up to 127 entries. A complete repeat is byte-identical; thirteen native
instruction mutations are rejected. Proof and exact sources:
`work/modern_thing_definition_20261018/validation/`. See
`docs/modern/THING_DEFINITION_CONSTRUCTION.md` for the measured offsets and scope.

Forced replacement is captured before iteration from thing type_flags bit1 and
manager+24 object's +102 byte. Each nine-byte entry supplies name key +0 and
replacement byte +8; +4 is unused by this body. AddTC gets null params and its
return does not stop traversal. Final StartKillLocked assignment follows every
request, writes both directions and preserves other flags. Immutable 0..127
entry definitions are the validated domain; negative counts or callback mutation
of definitions are not claimed.

Next implementation: owned typed definition component-set data and a concrete
construction service using Store/factory capabilities, with conditional policy
query and final kill-lock update. Wire it into the generic host explicitly;
its current hook is still controlled. Retain derived virtual override boundaries
and compare ordered requests with the new corpus. No later activation, physics,
world ticks or representative gameplay session is implemented by this reference.
The prior staged C++ five-build proof remains unchanged except documented README
edits. No launches, installation writes, Lua changes, commits or pushes.


## Owned definition component construction, 2026-10-18

`ThingDefinitionConstruction` now connects typed component sets to actual Store
ownership. It captures conditional forced replacement once, resolves owned names,
requests each attachment with null parameters, tolerates ordinary refusal and
assigns StartKillLocked only after traversal. `ThingDefinition` owns the component
sequence; ThingState has a separate lock flag. The service guard spans policy and
name lookup as well as component calls, refusing recursive owner destruction,
reconfiguration and definition traversal. Exceptions retain accepted components
and the old lock value; generic creation supplies its existing full cleanup.
An assembled test wires the real service into the generic creation host and
checks components -> script strings -> Mapwho -> delayed OnCreate publication.
Definitions remain immutable through the call; 0..127 entries are the recovered
domain. Type flags are the captured per-entity value supplied by the caller.

Final five-build validation passes 44 MSVC x86/x64/clang tests, 27 GCC and 24
desktop-free. Each also passes 1,592 definition cases/26,810 requests, 1,277
creation cases/8,800 states and every previous component/registration/numeric/UID
corpus. Canonical 50 provider inputs + registration table remain unchanged.
Authoritative proof: `work/modern_owned_definition_20261018/final_validation/`;
aggregate `work/modern_owned_definition_20261018/summary.json`. The earlier
`validation/` predates the final regression returning typed active_lifecycle
from OnCreate during an independent component catch-up callback. Only the
simulation README changed after final source freeze, recorded in the aggregate.

The definition comparison composes observed native orchestration requests with
separately validated AddTC ownership rules. It does not claim a new native
CreateFromDef-to-AddTC execution. Real decoded assets/derived components remain
fixtures. Details: `docs/modern/THING_DEFINITION_STORE.md`.

Next behavior gate: InitialActivate (manifest entry 004C9690, generic vtable slot9)
and OnInitialActivate (004C97B0, slot10). These names/slots are confirmed by the
manifest and pinned vtable bytes; behavior has not yet been executed or ported.
Recover callback ordering/flag changes and component stage2 before implementing;
retain stage catch-up versus owner dispatch and original timing. Later activation,
physics, removal scheduling, maps/session and explicit world ticks remain. There
is still no representative gameplay session. No launches, installation writes,
Lua changes, commits, pushes or canonical promotion occurred.

Activation starting evidence: `work/modern_thing_activation_20261018/initial_disassembly.json` records pinned candidate spans, verified return extents and vtable slots. Manifest names suggest update-list insertion before phase2, quest OnActivateEntity before phase3 and drawable/render-list handling afterward. These remain static hints until executed; do not treat drawing policy as a generic Activate call.


## Native initial activation reference, 2026-10-18

Original InitialActivate 004C9690 and OnInitialActivate 004C97B0 now execute in
private x86 emulation with actual AddTC, snapshot/growth, update/render insertion
and generic drawable query/set. All 1,608 cases/5,660 phases/31,648 observations
match, including 1,070 attached children, 632 physics notifications and 326 render
requests. Full independent repeat is byte-identical; eighteen instruction controls
reject. Authoritative native proof/exact sources:
`work/modern_thing_activation_20261018/validation/`; aggregate parent summary.json.
Numeric export for the next C++ comparison:
`work/modern_owned_activation_20261018/{export.py,export.json,activation.txt}`.

Phase2 increments low3 stage, sets owner+91 bit3, adds to per-type updates (physics
vslot46 before first insertion), copies components and dispatches component phase2
with live ToBeRemoved checks. Editor query follows callbacks; outside editor, a
present definition with IsDrawable false requests SetAsDrawable(false). IsDrawable
true/absent definition/editor do not restore a flag cleared by callbacks.
Phase3 calls quest OnActivateEntity BEFORE stage advance, then snapshots/dispatches
phase3 and adds to render list if virtual DrawIsDrawable returns true. Generic
query requires +92 bit1, +91 bit3, !Dead. Neither phase has an entry Dead guard.
Both insertion helpers are idempotent, including single-member heads, and preserve
peer order. These phases do not remove existing membership if later nondrawable/dead.

Child attachment from quest notification catches up at the OLD counter and then
participates in the next owner snapshot; a child can receive phase3 twice at that
boundary. Child attachment during a phase callback instead stays outside that old
snapshot. Do not deduplicate or defer those calls. Counter and explicit callback
phase remain separate, including wrap/repeated/reversed inputs.

Definition +41 bit4 is statically correlated to Transfer IsDrawable. The graphics
boundary is CTCGraphicAppearance::SetAsChanged(15) at 004BF9E0, controlled in this
probe; it is not a universal message bus. SetAsDrawable's special component
notifications for types 33/69/70/75/216 are outside this corpus (types2/7/91).
Quest/editor/derived methods, graphics dirty handling and allocation are controlled.
The fixture explicitly detaches update/render lists before native destruction;
this is NOT native active-entity destruction/Kill parity. Details:
`docs/modern/THING_ACTIVATION.md`.

A long-lived Windows Unicorn instance failed at host level after 1,573 complete
cases; the neighboring ten cases pass alone. The whole unchanged 1,608-case corpus
now runs with fresh 128-case instances and explicit hook cleanup, and all 1,573
historical complete cases match byte for byte. The failed retail_probe directory
is diagnostic, not authoritative; batched_native and validation are authoritative.
No C++ compiled sources or the 50 canonical providers+registration input changed;
the prior 44/27/24 build proof stands without claiming new builds this checkpoint.
No launches, install writes, Lua changes, commits, pushes or coverage promotion.

Next concrete implementation: owned per-type update and global render membership
in ThingStore (fixed slot links, copied validated handle extraction), a guarded
physics registration callback before insertion, semantic initial-activation and
drawable fields, definition IsDrawable, and a portable ThingActivation facade.
Expose narrow immediate quest/editor/drawable capabilities. Preserve original
callback/snapshot timing and duplicate registration rules. Keep SetAsDrawable's
unrecovered special-derived behavior explicit, not a silent flag-only stub.
Owned disposal must unlink memberships safely without claiming native Kill timing.
Compare the complete numeric corpus across five builds plus all prior corpora.
World 15 Hz scheduling, RNG order, physics, active removal, real decoded definitions,
derived behavior and the representative headless gameplay session remain open.


## Owned initial activation and membership, 2026-10-18

ThingActivation now executes InitialActivate/OnInitialActivate through real Store
update/render membership. Store uses distinct fixed-slot links for per-type
updates and global render order, copied validated handles, idempotent insertion
and no container growth for list operations. Generic state has initially_activated
and drawable flags; typed definitions have IsDrawable. Physics callback slot46
occurs before first insertion, including ToBeRemoved physics. Owner phase2 follows
the snapshot; quest notification precedes phase3 counter advance/snapshot. Callback
catch-up, absence of an entry Dead guard, repeated calls and 3-bit wrap remain intact.
Required narrow quest/editor/drawable host operations retain the controlled/native
integration boundaries; SetAsDrawable special-derived effects are not silently
replaced by raw flag setters. All owner lifetime/configuration reentry is guarded.

Direct update registration guards physics callbacks too. Exceptions preserve
prior state/components and release guards. Owned disposal keeps existing
memberships visible through component destruction, then unlinks them before
UID/registration removal and slot reuse. This is modern ownership cleanup, not
native Kill timing. Dead/drawable setters do not remove old membership.

Final five-build proof passes 45 MSVC x86/x64/clang tests, 28 GCC and 25 desktop-
free. Each matches all 1,608 activation cases/5,660 phases/28,206 observable
snapshots plus every earlier definition/creation/component/registration/numeric/
UID corpus. Native trace retains 31,648 events; internal method-entry markers
(3,116 update, 326 render) have no modern external callback and are excluded
explicitly in comparison.json. Actual list effects/physics-before-insertion are
checked by observable snapshots and direct public Store goldens.
An assembled golden runs actual creation -> OnCreate publication -> both
activation phases -> owned disposal, with controlled assets/derived/quest behavior.
This is still not the representative recovered gameplay-session gate.

Authoritative proof: `work/modern_owned_activation_20261018/final_validation/`;
aggregate parent summary.json. Earlier validation/ predates the assembled golden.
Only the simulation README changed after final test freeze, recorded in aggregate.
Fifty canonical provider inputs + registration table remain unchanged, with no
canonical coverage promotion. Details: `docs/modern/THING_ACTIVATION_STORE.md`.
No launches, install writes, Lua changes, commits or pushes occurred.

Next native gate: CThing::Kill(bool) at 004C9B80. Static starting evidence is
`work/modern_thing_kill_20261018/initial_disassembly.json`, NOT execution proof.
It checks Dead (+91 bit0), an in-progress +92 bit2 and StartKillLocked (+91 bit6)
BEFORE reading its bool argument. Do not interpret the bool as overriding lock.
It conditionally calls entity vslot13, sets +92 bit2, conditionally sets +93 bit1
and calls vslot11, then calls vslot12, sets Dead, calls UpdateThingScanInclusion,
queues through 005202B0 and removes render membership via 0051E000.
005202B0 starts with manager+48 vector append; it is NOT direct update unlink.
Its manifest camera label is misleading; generic vslot11 target 004C7D50 also has
a particle-class manifest label. Recover bodies/ABI from pinned bytes and verify
queue growth, recursive Kill suppression, callback timing and later manager sweep
before porting. Active update unlink/deallocation may be deferred beyond Kill.
Real decoded definitions/derived behavior, physics, scheduler/maps and explicit
15 Hz world ticks/RNG order remain open; keep this separate from ordinary destroy.


## Native Kill and queue request reference, 2026-10-18

Original CThing::Kill(bool) now executes with actual generic entity slots11/12,
component slots13/14, AddTC, native pending-vector append/growth and render removal.
All 1,524 cases/2,477 outer requests/11,000 observations match. There are 563 new
queue entries, 151 nested requests and 5 cases with two new queue entries.
Full independent repeat is byte-identical; 26 instruction controls are rejected.
Authoritative proof/exact sources: `work/modern_thing_kill_20261018/final_validation/`
and final_native/; aggregate parent summary.json. Numeric comparison data:
`work/modern_owned_kill_20261018/{export.py,export.json,kill.txt}`.

Kill guards Dead (+91 bit0), in-progress (+92 bit2) and StartKillLocked (+91 bit6) BEFORE
reading the bool. False skips the virtual predicate and killed pass, not the lock.
The true-path predicate executes before in-progress is set; no guard is repeated
on return. Predicate side effects setting Dead/lock do not veto a true return.
Bounded predicate recursion can complete an inner Kill and then the outer call
can issue callbacks/queue append again. Preserve observed queue duplicates and
callback order; do not add an early semantic guard or collapse those effects.
This does not prove such overrides are safe inputs to native UpdateDeaths.

A true request sets +93 bit1, calls entity vslot11 (generic 004C7D50), disables
attachment, invokes every sorted component slot13 regardless ToBeRemoved and
unconditionally enables attachment. Entity vslot12 (004C7CF0, OnKill) always
notifies CQuestManager::OnDeactivateEntity 004AFB00 with current +93 bit4, then
disables attachment and invokes every component slot14 without removal filtering,
re-enables attachment and calls manager OnThingKill 00520C80. The quest bool's
semantic field name is still unknown; do not invent one from its bit position.
Quest attachment can join the following pass; component-pass attachment is
refused before factory invocation. Recursion in those callbacks is suppressed
by the already-set in-progress bit, unlike predicate recursion.
Only afterward does Kill set Dead, call UpdateThingScanInclusion, queue through
005202B0 and RemoveFromRenderList 0051E000. Update membership, components and
ordinary registration remain intact in this function; no entity is destroyed.

The quest boundary was verified as OnDeactivateEntity, correcting historical
quest_death trace labels to quest_deactivate. Final proof uses corrected names;
label_reconciliation.json proves all other observations are identical. Earlier
retail_probe/validation/ are historical, not the current authoritative sources.
Predicate/quest/history/derived callbacks and allocator/memcpy are controlled.
Type58 scan behavior is absent. Fixture queue discard/list detach before native
destruction is explicit and is NOT execution of UpdateDeaths or active disposal.
Details: `docs/modern/THING_KILL.md`. The 50 canonical provider inputs+registration
table and all modern compiled inputs remain unchanged (documented README-only
post-freeze delta from previous proof). No new C++ build result is claimed.
No launches, installed writes, Lua changes, commits, pushes or coverage promotion.

Next: execute manager UpdateDeaths 0051F070 and RemoveFromUpdateList 0051ECC0 with
original deleting wrappers/disposal. Static candidate evidence is sweep_static.json,
not execution proof. The apparent sweep removes update membership BEFORE deletion,
reloads vector length after each deletion (possibly consuming newly appended work),
then resets logical end while keeping capacity. It does not visibly deduplicate.
Confirm actual extents and native valid-domain disposal, queue growth during
callbacks, order and handling of duplicated/expired pointers. Then connect the
owned Kill/pending queue and sweep using validated handles, preserving valid
callback timing without copying native dangling-pointer behavior. A Kill request
may be valid during activation/component callbacks; do not reuse a blanket
owner-lifecycle exclusion that silently rejects those requests. Owned disposal
must remain guarded during an executing request. Real quest/history, scan/derived
behavior, physics, maps, world 15 Hz scheduling and the representative scenario remain.


## Native queued-death disposal reference, 2026-10-18

Original Kill -> UpdateDeaths 0051F070 -> RemoveFromUpdateList 0051ECC0 -> deleting
wrappers now execute through actual component/weak/UID/class/container cleanup and
entity release. All 1,108 cases/4,220 sweeps/1,936 recorded deletions/30,481 snapshots
match. There are 294 callback-triggered enqueues, 597 physics-removal notifications
and 2,216 component destructors. Full independent repeat is byte-identical; 24
instruction controls reject. Authoritative proof/exact sources:
`work/modern_thing_sweep_20261018/validation/`; aggregate parent summary.json.
Numeric data beside Kill corpus:
`work/modern_owned_kill_20261018/{export_sweep.py,sweep_export.json,sweep.txt}`.

Native update removal calls cached physics vslot45 BEFORE unlinking, including
ToBeRemoved physics; absent membership does not notify again. The sweep then calls
the real entity deleting wrapper with flag1. CThing destructor decrements its thing
counter and disables attachment, purges owner weak pending records, clears each
component type bit before destruction, then notifies UID deletion. At component
destructor entry update membership is gone, ordinary registration/UIDs remain.
Actual key-only UID erase precedes class registration unlink. Strings, weak-control
storage and containers release afterward; object release is last. Global thing
count and registration count differ transiently during destruction.

The sweep rereads vector storage/length after EACH deletion. A physics-removal
or controlled derived component destructor can Kill another live owner; append
may grow/reallocate storage, and the SAME sweep consumes it. Queue end resets to
beginning only at completion; capacity remains and later batches reuse storage.
Never capture an initial work count or retain a reference into the growing queue.
The valid-lifetime corpus uses unique owner pointers, mixed types/order, missing/
removed components, duplicate UIDs, weak-queue duplicates, multiple and empty
batches, and bounded acyclic callback workloads. All surviving fixture owners are
also cleaned up through original Kill/UpdateDeaths; no manual active unlink or
raw object cleanup substitutes for that path. Only the prepared queue container
and class-factory teardown follow, with every allocation and string accounted.

A separate duplicate_dispatch probe runs only the original OUTER sweep with
controlled unlink/deleting calls that retain their records: 4 cases/11 deleting
dispatches show duplicate pointer values are NOT deduplicated. It does not execute
native destructors on stale/freed pointers or claim duplicate lifetime safety.
Keep prior Kill callback/queue effects, then use modern generation validation for
expired entries rather than copying native dangling-pointer behavior.
Details: `docs/modern/THING_SWEEP.md`. Predicate's true leaf executes natively;
quest/history and derived callbacks, allocator/memcpy remain controlled. Type58
scan behavior remains absent. Initial failed development captures corrected the
16-type factory string baseline and callback-runner closure; they are historical.
All modern compiled inputs and 50 canonical provider+registration inputs remain
unchanged, aside from the previously documented README-only post-freeze delta.
No new C++ builds, canonical promotion, launches, install writes, Lua changes,
commits or pushes are claimed.

Next: IMPLEMENT owned Kill and queued sweep together using both frozen corpora.
Add semantic kill-started/notified/quest-deactivation-argument state, component
slots13/14 with attachment disabled and no removal filtering, and preserve
predicate-before-started behavior (bounded predicate recursion can queue twice).
The quest operation is OnDeactivateEntity 004AFB00, not a death-only event.
Do not use broad activation/component lifecycle exclusion to reject valid Kill
requests; use request execution depth to protect ownership while preserving
native guards/timing. Add physics vslot45 before actual update unlink, preserve
live queue growth, then dispose through real Store ownership/UID cleanup.
Current ThingComponents owner_phase_active_/per-type bool guards may need saved
state/depth for valid nested Kill dispatch; do not accidentally release an outer
executing-type guard. Queue values must be copied validated handles; skip expired
records safely, retain duplicate request effects and clear reusable storage after
draining. Whole sweep must not recursively destroy an executing owner. Match
both complete corpora plus all earlier five-build suites, then assemble full
creation/activation/Kill/sweep. Real quest/history/scan, derived physics, maps and
explicit 15 Hz scheduling/RNG order and a representative gameplay scenario remain.


## Owned Kill and queued disposal checkpoint (2026-10-18)

`ThingKill` now executes the recovered generic Kill sequence through required
quest/predicate/history/scan capabilities and actual owned component dispatch.
`ThingStore` owns a shared weak-handle death queue. Sweep removes update membership
after the cached physics callback, then destroys through owned component/weak/UID
cleanup. Callback appends are consumed in the same sweep; expired generations and
duplicate entries are safely skipped. Saved callback guards preserve valid nested
Kill behavior without releasing an outer executing-type guard. Exceptions retain
prior effects; retry also handles an already-disposed prefix.

All 1,524 native Kill cases/2,477 requests match 9,064 observable snapshots;
all 1,108 sweep cases/4,220 sweeps/1,936 deletions match 11,841 snapshots.
Full native traces retain 11,000/30,481 events respectively; internal instrumentation
is explicitly excluded from the modern comparison. All earlier numeric corpora
also pass in five configurations: 47 tests each MSVC x86/x64 and clang-cl,
30 GCC, 27 with desktop tools/reconstructed objects disabled. Architecture gates
pass in each configuration. Frozen sources, exact result markers, executable
hashes and full logs: `work/modern_owned_kill_20261018/validation/`.
The 50 selected canonical provider inputs, registration table and four selected
layout headers remain unchanged. The simulation README alone is updated after
the source freeze; the proof records that documentation-only hash delta.

An assembled golden connects actual owned definition construction, creation,
OnCreate, both activation phases, Kill and sweep in the same simulation library.
Assets, quests/history/scan and derived components remain controlled fixtures;
this does not satisfy the representative recovered gameplay-session gate.
Details: `docs/modern/THING_KILL_STORE.md`.

Next: recover native world-update traversal and entity/component tick behavior
before implementing explicit world ticks. Preserve callback/list mutation order,
15 Hz scheduling and RNG consumption. Real assets, derived gameplay/physics,
quests/history/scan, maps and representative headless gameplay remain open.
Work remains offline: no launches, installed-game writes, Lua edits, canonical
promotion/recount, commits or pushes.


## Native world-update traversal checkpoint (2026-10-18)

Offline `WorldUpdateThings`/`FrameUpdate` now has a reference: 1,241 cases,
2,448 calls, 2,613 encountered records, 1,619 frame dispatches, 170 bullet-time
dispatches, 392 callback actions and 14,355 explicit observations. Full repeat
is byte-identical; all 24 instruction mutations reject. Actual native list
insertion/removal execute; bullet predicates, entity vslots19/20/44 and the final
sweep are controlled. Proof: `work/modern_world_update_20261018/final_validation/`.
Details and limits: `docs/modern/THING_WORLD_UPDATE.md`.

Important: traverse types1..15, sample type controls at type entry, increment
visit count even for skipped entities, cache each successor before callbacks,
and always tail-call sweep even when global updates are disabled. A complete
frame-start snapshot changes append behavior. A cached removed successor can
still be visited while alive; its cleared next link then ends the type traversal.
Three-actor fixtures distinguish those cases without executing stale storage.

Next: recover generic WorldUpdate 004C8050, WorldUpdateBulletTime 004C8BA0 and
IsFrameUpdated 004C7BF0. Primary CThing vtable01239ABC establishes slots19/20/44.
Normal generic update uses its insertion-ordered component update list, component
slot9 with ToBeRemoved filtering and Dead checks, then shared pending component
removal. Bullet-time uses slot10 and a type70 operation. These bodies are static
leads only; do not substitute activation snapshots or Kill's sorted vector.
Keep full/affected bullet-time policy explicit until separately recovered.
Only then implement owned tick traversal, preserving these timings with validated
handles/lifetime guards, and connect real disposal after traversal.

No modern compiled or selected canonical inputs changed during this research;
the prior 47/30/27 five-build proof remains current. The only earlier post-freeze
delta is its recorded simulation README. No launches, install writes, Lua edits,
canonical promotion/recount, commits or pushes. Scheduling/RNG and representative
recovered gameplay remain open gates.


## Native generic tick checkpoint (2026-10-18)

Generic WorldUpdate 004C8050, WorldUpdateBulletTime 004C8BA0 and IsFrameUpdated
004C7BF0 now execute in a private native reference with actual constructors,
AddTC, replacement, container/weak removal, component deletion and full teardown.
2,448 cases cover 2,832 normal calls, 1,324 bullet calls, 4,156 retention queries,
3,968 component callbacks, 1,878 deletions, 287 actions and 30,245 observations.
All explicit state agrees with the independent model; full repeat is byte-identical
and all 33 instruction mutations reject. Proof:
`work/modern_thing_tick_20261018/validation/`. Details: `docs/modern/THING_TICK.md`.
Numeric corpus `work/modern_thing_tick_20261018/tick.txt` retains every event;
only ToBeRemoved snapshots normalize nonzero bytes to true. Native JSON keeps
raw bytes. Modern comparison must explicitly declare any internal marker filter.

Normal updates use insertion-ordered component update membership, checking Dead
before each component and afterward; ToBeRemoved suppresses slot9. Successors
are loaded AFTER callbacks, allowing tail appends and replacement catch-up during
the same pass. The global pending-removal drain purges expired/dead foreign owner
records, leaves other live owners' records, and for this owner clears a true
driver cache, unlinks updates, erases the sorted component entry and deletes
before weak/node release. Native membership bits and physics/appearance cached
identities can remain stale. Preserve observed values with validated handles,
never unsafe dereferences. Valid fixtures exclude deletion through duplicate
component pointers and replacement of the executing component.

Bullet updates first call controlled type70 operation006A1D60 (manifest labels
UpdateCarriedObjects), then slot10 without owner Dead checks or pending drain.
Retention returns true for null definition or definition+41 bit1. Otherwise
actual IsDrawable executes via slot27; drawable + cached appearance byte58 or
byte59 bit2 retains, with ForceFrameUpdate (+92 bit3) as the remaining fallback.
Derived meanings of appearance fields, carrying logic and bullet policy remain.

Next: port owned generic tick/component removal and retention, then connect manager
traversal and real Kill sweep. Use both frozen native corpora; keep the manager's
BEFORE-callback successor semantics distinct from component AFTER-callback reads.
Use explicit world tick data, guard active ownership, preserve callback replacement
and queue growth, and compare state field-by-field in every build configuration.
Add a creation/activation/tick/Kill/sweep assembly in the same headless library.
Prepared definitions/components and diagnostic pending records do not satisfy
the representative recovered gameplay gate. Actual assets, derived gameplay,
global component updates, bullet policy, scheduling and RNG remain open.

All modern compiled sources and 50 selected canonical providers plus registration
and four selected headers remain unchanged during this research. The 47 MSVC/
clang, 30 GCC, 27 desktop-free five-build proof remains current; its sole recorded
post-freeze delta is the earlier simulation README. No new C++ builds, launches,
install writes, Lua edits, canonical promotions/recounts, commits or pushes.


## Owned generic tick checkpoint (2026-10-18)

`ThingTick` now ports generic normal/bullet-time update and retention against
actual owned components. Callbacks receive an explicit u32 WorldTick frame; no
clock, scheduler, renderer or RNG enters the module. Normal update preserves
post-callback successor reads, ToBeRemoved/Dead timing, same-pass appends and
replacement of other types. The shared weak removal queue uses stable nodes,
allowing destructor appends and foreign-owner cleanup during the drain. It erases
updates/vector entries before actual component destruction, retains native stale
membership/non-driver cache identities, and validates every later cached use.
Kill may run inside update/driver callbacks; sweep waits for the tick to return.

All 2,448 native cases, 2,832 normal calls, 1,324 bullet calls and 4,156 queries
match 27,437 observable snapshots plus final state. Native JSON retains all 30,245
events. Only internal removal-type query and drawable helper entry markers
(codes13/17) are excluded; their resulting state/decisions remain compared.
ToBeRemoved snapshots normalize nonzero bytes to true. Every external factory,
component, driver, carrying, destructor and caller boundary remains compared.
Details: `docs/modern/THING_TICK_STORE.md`.

Five configurations pass all earlier corpora plus the new tick corpus: 48 tests
each MSVC x86/x64 and clang-cl, 31 GCC, 28 desktop-free. Architecture gates pass.
Frozen source/log/target/binary evidence:
`work/modern_owned_tick_20261018/validation/`. The 50 selected canonical provider
inputs, registration table and four selected headers are unchanged. The simulation
README alone is updated after the freeze, with its hash delta recorded.

Goldens cover execution/lifetime guards, throwing callbacks/retry, duplicate and
stale component records, reused owner slots, destructor queue append/foreign purge,
and Kill during update and pending driver callbacks. The assembled lifecycle now
connects definition construction, creation, OnCreate, both activation phases,
normal and bullet ticks, actual live physics-component removal, then Kill/sweep.
The owner retains UID and update membership during component removal. CPU assets,
quests/history/scan and derived components remain fixtures; this is not a
representative recovered gameplay scenario.

Next: implement manager traversal using its frozen native corpus, connecting
normal/bullet dispatch, retention-driven update unlink and actual final death sweep.
Keep manager BEFORE-callback successors distinct from component AFTER-callback
reads. Manager traversal must handle cached removed successors through validated
handles and preserve type/global control sampling. Resolve tick callbacks that
need same-owner update membership operations against explicit native composition;
existing active-phase registration guards remain until that boundary is proven.
Do not route every derived entity through an assumed generic implementation.
Keep bullet full/affected policy explicit until separately recovered. Actual
global component updates, assets/derived gameplay, scheduling, RNG and a
representative headless session remain open. No launches, install writes, Lua
edits, canonical promotion/recount, commits or pushes.


## Tick membership composition checkpoint (2026-10-18)

The active-phase membership gap is closed. A new native composition reference
executes generic normal/bullet bodies, SetAsFrameUpdated 004C7C40,
SetForceFrameUpdate 004C7C60, actual manager add/remove, and controlled physics
notifications inside component callbacks. All 1,204 cases, 1,185 normal calls,
1,124 bullet calls and 10,367 observations agree with the independent model.
Full repeat is byte-identical; all 22 instruction mutations reject. Native proof:
`work/modern_tick_membership_20261018/validation/`. Entity/component prefixes and
nodes are prepared; native construction/teardown remains separate evidence.

The owned C++ path matches EVERY snapshot and final state, with no event filter.
Tick methods now expose the recovered entity operations. SetForce stores before
registration and false does not unlink; both entity wrappers guard Dead, while
raw manager add/remove do not. Cached physics is notified before membership
changes, including ToBeRemoved physics. A private tick-aware notification path
preserves saved outer component/phase guards; other lifecycle restrictions remain.
Repeated unfinished transitions are refused, and throwing callbacks retain prior
effects/pre-link state while guards unwind for retry. Details:
`docs/modern/THING_TICK_MEMBERSHIP.md`.

All earlier corpora plus this composition pass five configurations: 49 tests each
MSVC x86/x64 and clang-cl, 32 GCC, 29 desktop-free. Architecture gates pass.
Proof: `work/modern_owned_tick_membership_20261018/validation/`. The 50 selected
canonical providers, registration table and four selected headers are unchanged.
Only the simulation README changes after source freeze; its hash delta is recorded.

Next: IMPLEMENT manager traversal now. Its numeric `world.txt` and export recipe
are under `work/modern_world_update_20261018/`, retaining all 14,355 native events
across 1,241 cases/2,448 calls. Normalize flags91 with predicate mask0x29 and bool
controls; raw native JSON retains all bits. Explicit internal event exclusions
2(frame wrapper),8(manager remove),11(empty controlled sweep) leave 9,926 observable
snapshots. Verify real final sweep separately through actual Kill/disposal goldens.
Implementation notes: `work/modern_world_update_20261018/owned_port_design.md`.
Preserve manager BEFORE-callback cached successors, removed-but-live visitation,
types1..15, global/type sampling, skipped-entity visit count and normal/bullet/
retention ordering. Require narrow entity dispatch/bullet policy capabilities;
do not silently run every derived entity through generic ThingTick. Protect
executing owners and manager reentry across both traversal and final sweep, and
use actual same-Store death disposal. Scheduling/RNG, real assets/derived gameplay,
global component updates and representative headless gameplay remain open.
No launches, install writes, Lua edits, canonical promotion/recount, commits/pushes.


## Owned manager traversal checkpoint (2026-10-18)

`ThingWorldUpdate` now connects per-type traversal to owned entities, explicit
normal/bullet dispatch, retention and actual deferred death disposal. All 1,241
native cases/2,448 calls match 9,926 observable events plus final states; the
numeric corpus retains all 14,355 native events. Native internal entry events
are excluded explicitly from the owned comparison. Global/type controls,
cached-successor timing, counter/marker state and callback list mutations agree.
Generation validation stops traversal through a destroyed successor rather than
visiting a reused slot; this safety behavior is outside native live-storage proof.

The assembled fixture now runs actual creation, initialization, activation,
manager traversal, generic normal/bullet ticks, pending component removal, Kill
and final manager-driven sweep. Assets, quests, scan services, bullet policy and
derived component behavior remain controlled; this is not a gameplay scenario.

All earlier corpora and the new traversal pass five builds: 50 tests each on
MSVC x86/x64 and clang-cl, 33 on GCC, 30 desktop-free. Proof and frozen source,
binary, target and log hashes: `work/modern_owned_world_update_20261018/validation/`.
The 50 selected canonical provider sources, registration source and four selected
headers are unchanged. No game launch, Lua change, commit or push occurred.

Next: execute the bullet-policy dependencies against the pinned native image,
then connect recovered capabilities. Static preparation is in
`work/modern_bullet_policy_20261018/next_scope.md`; it is not execution evidence.
Scheduler/RNG, real definitions/assets, derived gameplay and a representative
headless session remain open. Details: `docs/modern/THING_WORLD_UPDATE_STORE.md`.


## Bullet full-update native reference (2026-10-18)

Actual multiplier traversal, _ftol2 conversion and signed frame remainder now
match an independent exact model across 2,570 inputs / 30,840 records and all
12 x87 precision/rounding profiles. Full repeat is identical; seven instruction
mutations reject. Observations include 20,464 returns and 10,376 deliberate
fault cases (zero divisor or signed overflow); 230 inputs are mode-sensitive.
The private oracle builds with warnings as errors and verifies all relocated
code outside six declared address operands. Prepared trees and controlled globals
are not gameplay ownership or scheduler evidence. No game or Lua launch occurred.

Proof: `work/modern_bullet_policy_20261018/final_validation/`.
Contract: `docs/modern/BULLET_FULL_UPDATE.md`. The previous five-build modern
checkpoint and selected canonical inputs are unchanged. Next is an owned exact
numeric policy with explicit division errors, preserving multiplier order and
validating collection arithmetic bounds; affected-entity policy remains separate.


## Owned bullet full-update policy checkpoint (2026-10-18)

Portable `bullet_update_divisor` and `bullet_time_for_full_update` now match all
2,570 native inputs / 30,840 records in four host rounding modes. Divisor,
boolean results and explicit zero-divisor/signed-overflow errors compare. Host
floating controls/flags remain untouched. The ordered sum uses integer arithmetic;
the 18-limb bound covers the unsigned-32 collection domain, beyond the native
fixture's eight entries. The checksum shares the private arithmetic kernel while
retaining its own five-limb bound and operation order; its full corpus passes.

The assembled lifecycle now uses the actual full-update decision for normal
frame 8 and bullet frame 9 through the manager, generic ticks and death sweep.
Active-bullet and affected-entity policies are still controlled. Registration of
real multipliers, affected-entity behavior, scheduler/RNG and a representative
headless gameplay session remain open.

Five builds pass: 51 tests each on MSVC x86/x64 and clang-cl, 34 GCC, 31
desktop-free, with all earlier corpora replayed. Frozen proof:
`work/modern_owned_bullet_policy_20261018/validation/`. The 50 selected canonical
providers, registration source and four selected headers remain unchanged.
Contract: `docs/modern/BULLET_FULL_UPDATE_STORE.md`. No game launch, Lua edit,
commit or push occurred. Next: execute the full affected-entity predicate and
its ordered linked-entity/capability observations before replacing that boundary.


## Affected-entity native composition checkpoint (2026-10-18)

The full 598-byte IsAffectedByBulletTime root now executes with actual weak
resolution/cleanup, component71 getter, fading, component lower-bound and player
lookup bodies. A separate explicit model matches 3,597 cases / 8,810 calls /
44,681 ordered events, including 486 redirect visits and 561 release requests.
Full repeat is identical; 26 instruction mutations reject (25 assertion
failures, one invalid-read control). Script-control responses and final record
deallocation remain controlled. Prepared valid storage and acyclic chains are
not evidence for arbitrary missing components or native cycle safety.

Raw entity tests at +24/+28/+2C/+30/+3C are component membership bits for
38/71/75/100/146/242/249. The owned port must preserve those bits independently
of live component storage. GetPItem can clear an expired reference after decrement
and release; component75 resolution occurs up to three times before a redirect.
Script responses also cannot be cached across multiple observed queries.

Proof: `work/modern_bullet_affected_20261018/composed_validation/`.
Contract: `docs/modern/BULLET_AFFECTED.md`. This supersedes earlier controlled
lookup receipts. Modern and selected canonical inputs remain unchanged from the
prior five-build checkpoint. No game launch, Lua edit, commit or push occurred.
Next: owned affected-entity policy with explicit capabilities and lifecycle,
stale-handle and cycle policies; real script control and gameplay remain open.


## Owned affected-entity policy checkpoint (2026-10-18)

`ThingBulletAffected` now uses actual Store membership, component identities,
entity state and validated target handles. Required host capabilities preserve
repeated weak/script queries; weak-record cleanup is modeled by the corpus host,
not claimed as a newly implemented weak allocator. The owned policy matches
3,597 cases / 8,810 calls / 32,040 comparable events plus final states. Numeric
export retains all 44,681 native events; internal visit/search entries are omitted
explicitly from the owned comparison.

Goldens cover owner/type guards, cross-facade reentry, nested tick refusal,
exception retry, cycles, reused-slot stale targets and actual pending-removal
membership behavior. Missing required storage and unsafe redirects return typed
errors. Manager delegation is supported before the actual generic tick. The
assembled lifecycle now uses both recovered bullet full-update and affected
policies; main-player/script/component gameplay capabilities remain fixtures.

All five builds pass: 52 tests each on MSVC x86/x64 and clang-cl, 35 GCC, 32
desktop-free, with all previous corpora rerun. Frozen proof:
`work/modern_owned_bullet_affected_20261018/validation/`.
Contract: `docs/modern/BULLET_AFFECTED_STORE.md`. Selected canonical inputs remain
unchanged. No game launch, Lua edit, commit or push occurred. Next: actual script-
control and component/player reference capabilities, followed by measured full
bullet-selection composition. Real assets, scheduling/RNG and representative
headless gameplay remain open.


## Native script-control predicate checkpoint (2026-10-18)

Actual IsUnderScriptControl 004C8C90 (81 bytes), LowerBound 004365B0 (60) and
list-empty helper007122C0 (15) match 1,288 cases / 2,574 calls / 6,975 ordered
observations. All prepared input data stays unchanged. Full repeat is identical;
11 instruction mutations reject (six comparison failures, five invalid-read
controls). Poisoned skipped storage proves absent membership bypasses map lookup
and nonzero lock bypasses the list query. No Lua, actions or game run.

Proof: `work/modern_script_control_20261018/final_validation/`.
Contract: `docs/modern/THING_SCRIPT_CONTROL.md`. Modern and selected canonical
inputs still match the previous five-build checkpoint. Next is the owned query
with actual Store membership/component validation and narrow lock/action-state
capabilities, supporting delegation from ThingBulletAffected. This does not
implement scheduling or action ownership; full gameplay remains open.


## Owned script-control checkpoint (2026-10-18)

The owned predicate matches 1,288 native cases / 2,574 results, with 1,267 lock
and 560 list observations in the measured order. It uses actual Store membership
and validated component31 identities, protects the owner/component through host
calls, and permits protected delegation from ThingBulletAffected. The headless
lifecycle fixture now invokes this leaf through the world/affected path. Lock and
list observations remain explicit capabilities; action execution is not supplied.

All five builds pass: 53 tests each on MSVC x86/x64 and clang-cl, 36 GCC, 33
desktop-free. All earlier corpora pass and frozen inputs stayed unchanged.
Proof: `work/modern_owned_script_control_20261018/validation/`.
Contract: `docs/modern/THING_SCRIPT_CONTROL_STORE.md`. The selected 50 canonical
providers, registration source and four headers remain unchanged. Next: actual
native affected/script composition and component/player reference capabilities.
Real actions, assets, scheduler/RNG and representative gameplay remain open.
No game launch, Lua editing, canonical promotion, commit or push occurred.


## Owned affected/script composition checkpoint (2026-10-18)

The actual native affected predicate now calls the actual script-control predicate
and shared lower-bound/list helpers. Its 4,109 cases / 9,834 calls / 58,143 events
repeat identically; all 36 distinct instruction mutations reject. Prepared data
changes only at predicted weak-reference fields. Frozen reference:
`work/modern_bullet_script_composition_20261018/final_validation/`.

The composed owned C++ path matches 42,573 comparable observations and all final
states, including 4,557 script queries. Internal native visits/map searches remain
in the export but are excluded from the owned-storage comparison. The original
affected fixture remains covered; its setup is shared with the composition test.
Five-build regression passes: 54 tests each MSVC x86/x64 and clang-cl, 37 GCC,
34 desktop-free, plus every earlier corpus. Frozen inputs and the selected
50 canonical providers, registration source and four headers stayed unchanged.
Proof: `work/modern_owned_bullet_script_20261018/validation/`.
Contract: `docs/modern/BULLET_SCRIPT_COMPOSITION.md`.

Next is component/player reference ownership. Existing evidence is available in
`rebuild/compile-gate/intelligent-pointer.json` and `intelligent-lifetime.json`,
with source `rebuild/src/compiled/00/a0/CBaseIntelligentPointer_Operations_00a01b20.cpp`
and gates `tools/decomp_pipeline/gate_intelligent_pointer.py` / `gate_intelligent_lifetime.py`.
These were read, not rerun or promoted. Resolve lifetime semantics before choosing
an owned adapter; do not recreate retail pointer graphs without need. Current
weak-reference behavior is still fixture-provided. Actions, assets, multiplier
registration, scheduling/RNG and representative gameplay remain open. No game
launch, Lua edit, installed-game write, canonical promotion, commit or push.


## Reference expiration evidence checkpoint (2026-10-18)

Existing intelligent-pointer operation/lifetime provider, header and test hashes
match their saved receipts. The new bounded native base-destruction/GetPItem
composition passes 16 prepared cases, identical repeat and three mutation
controls; full data snapshots and release counts compare. CThing tails into base
expiration after component/container teardown. This supports generation-validated,
non-owning modern handles, with Dead distinct from destroyed. No reference adapter
has been implemented yet; component/player ownership remains the next code step.
Contract: `docs/modern/THING_REFERENCE_OWNERSHIP.md`.
Evidence: `work/modern_reference_ownership_20261018/`.
The previous five-build C++ checkpoint is unchanged; no game/Lua work occurred.


## Owned entity-reference value checkpoint (2026-10-18)

`ThingReference` now stores an optional validated handle and borrows one Store.
Invalid/foreign assignment retains the old target; copies clear independently;
resolution clears expired identities. Dead remains resolvable, component teardown
retains owner resolution, and slot reuse cannot revive a stale reference. Store
must outlive references and copies; copy assignment cannot rebind sessions.

Focused lifetime/isolation/teardown tests and the full five-build regression pass:
55 tests each on MSVC x86/x64 and clang-cl, 38 GCC, 35 desktop-free. Every prior
native corpus passes. Frozen inputs and the selected 50 canonical providers,
registration source and four headers stayed unchanged. Receipt:
`work/modern_owned_reference_20261018/validation/summary.json`.
Contract: `docs/modern/THING_REFERENCE_OWNERSHIP.md`.

This implements the reference value, not component/player target adapters. Next
is guarded access to exact owned component identities, then separate adapters
for component71's linked getter and component75/player weak references. Do not
cache raw component pointers or assume those native representations are identical.
Action execution, assets, scheduling/RNG and representative gameplay remain open.
No game launch, Lua edit, canonical promotion, commit or push occurred.


## Guarded component access checkpoint (2026-10-18)

`ThingComponentAccess` validates exact owner/collection/type/generation and lends
const component access only for an observer callback. The callback copies its
result; references/views must not escape. Private per-owner observation markers
permit only the exact component selected by an affected/script host call. Saved
guards preserve outer protection; nested observers, tick/world updates, kill/sweep
and policy recursion are refused. Exceptions restore all prior guards.

Focused tests cover stale replacement/removal, identity mismatch, pending removal,
owner/type protection, cross-facade recursion, cross-owner tick/kill exclusion,
exceptions and exact delegated reads. A real component-owned ThingReference is
resolved through the access path; this is fixture integration, not production
component71/75 behavior. Full five-build regression passes: 56 tests each on
MSVC x86/x64 and clang-cl, 39 GCC, 36 desktop-free, plus all prior native corpora.
Frozen inputs and selected 50 canonical providers, registration source and four
headers stayed unchanged. Receipt:
`work/modern_component_access_20261018/validation/summary.json`.
Contract: `docs/modern/THING_COMPONENT_ACCESS.md`.

Next: production component/player reference adapters. Constraints are recorded in
`work/modern_component_access_20261018/adapter_next.md`: component71 linked getter
is distinct from component75/player weak resolution; preserve repeated calls and
expire after component teardown. Do not cache raw pointers. Script actions, assets,
scheduling/RNG and representative gameplay remain open. No game launch, Lua edit,
installed-game write, canonical promotion, commit or push occurred.


## Owned player target table checkpoint (2026-10-18)

`ThingPlayerTargets` owns ordered player-number entries and validated weak entity
references. It preserves first-match duplicates, neutral entry-index fallback and
signed 32-bit main selection. Invalid assignment retains the prior target; expired
references never resolve reused slots or fall through to another duplicate player.
This is selection/target state, not full CPlayer ownership or CreatePlayers.

The new offline native reference executes actual main selection/GetPlayer, then
GetPItem in the driver. All 6,912 cases / 13,824 selection-and-target pairs match,
repeat identically, and reject three selection mutations. Native data snapshots
and release timing compare; allocator-specific observations stay in JSON and are
excluded from the handle-based numeric export. Existing player lookup receipts
still match their provider/header/test hashes and were not rerun.
Reference: `work/modern_player_targets_20261018/final_native/`.

Owned C++ matches every selection and target. Bullet-policy goldens use the real
table for live/Dead target behavior. All five builds pass: 57 tests each on MSVC
x86/x64 and clang-cl, 40 GCC, 37 desktop-free, plus all prior native corpora. Frozen
inputs and selected 50 canonical providers, registration source and four headers
stayed unchanged. Receipt:
`work/modern_owned_player_targets_20261018/validation/summary.json`.
Contract: `docs/modern/THING_PLAYER_TARGETS.md`.

Next: distinct component71/75 target state through guarded access. Do not mistake
the +34 player target used by this policy for the separately recovered controlled-
creature field. Full player creation, actions, assets, scheduler/RNG and representative
gameplay remain open. No game launch, Lua edit, installed-game write, canonical
promotion, commit or push occurred.


## Component target identity checkpoint (2026-10-18)

Donor symbols, resolved FableWin thunks, type getters and PDB layouts identify
component71 as CTCCarryable and component75 as CTCDParticleEmitter. Carryable +10
is CTCCarrying* PCarryingTC; its getter returns that component's parent Thing.
The carrying component has interface type70. The modern link therefore needs a
component handle (including generation/collection), not only an entity reference.
Particle attachment uses PAttachedToThing and a separate AlwaysUpdate flag.
The player +34 reference is PeekPPlayerCharacter, distinct from ControlledCreature.

Setter inspection shows SetAsCarriedBy and SetAlwaysUpdate perform more work than
field assignment. Do not expose raw snapshot mutation as those gameplay commands.
Next: bounded owned target-query adapters with correct component/entity lifetimes,
then full setter/attachment recovery. Existing native query evidence remains the
behavioral reference. No new modern implementation is claimed by this investigation.
Contract: `docs/modern/COMPONENT_TARGET_IDENTITIES.md`.
Hashed evidence: `work/modern_component_targets_20261018/identity_summary.json`.
Previous five-build input hashes, including the documented README delta, still
match (57 MSVC/clang, 40 GCC, 37 desktop-free). No game/Lua activity occurred.


## Owned component target query checkpoint (2026-10-18)

ThingCarryableTargetState owns a validated carrying-component handle (type70),
while ThingParticleTargetState owns an entity reference and independent AlwaysUpdate
flag. ThingComponentTargets reads those states through exact guarded component
access and rejects missing source interfaces. load_snapshot is state initialization,
not SetAsCarriedBy/AttachToThing/ClearAttachments/SetAlwaysUpdate gameplay.

The composed affected/script replay now uses actual owned player, carrying and
particle target state: all 4,109 native cases / 9,834 calls / 41,884 comparable
observations match. Internal visits/searches and 689 allocator-release events are
excluded, along with native pointer/refcount bookkeeping; original traces remain
intact and older fixtures retain those comparisons. Exact projection is recorded
in `work/modern_owned_component_targets_20261018/comparison_scope.json`.
Goldens cover component removal while its parent survives, generation replacement,
pending removal, atomic snapshot rejection, role errors and independent flag lifetime.

Full five-build regression passes: 58 tests each on MSVC x86/x64 and clang-cl,
41 GCC, 38 desktop-free, plus every earlier native corpus. Frozen inputs and
selected 50 canonical providers, registration source and four headers stayed
unchanged. Receipt:
`work/modern_owned_component_targets_20261018/validation/summary.json`.
Contract: `docs/modern/THING_COMPONENT_TARGETS.md`.

Next: recover actual carrying/particle transitions and callback ordering. Donor
roots and misleading existing OnKill filename cautions are recorded in
`work/modern_owned_component_targets_20261018/next_transition_recovery.md`.
Modern invalidation tests do not establish native detach/destructor semantics.
Full component gameplay, player creation, script actions, assets, scheduler/RNG
and representative gameplay remain open. No game launch, Lua edit, installed-game
write, canonical promotion, commit or push occurred.


## Native particle AlwaysUpdate transition checkpoint (2026-10-18)

Actual SetAlwaysUpdate 006E0590 (85 bytes) plus GetPItem 00A01B50 now pass
2,048 cases / 6,144 calls / 28,160 observations. Full repeat is identical;
eight mutations reject (five comparisons, three invalid accesses). Skipped creation
parameters are poisoned; all prepared data is compared after each case. Mutation
preimages were verified independently. Proof:
`work/modern_particle_always_update_20261018/final_validation/`.
Contract: `docs/modern/PARTICLE_ALWAYS_UPDATE.md`.

Order: resolve particle emitter at component+14; resolve it again if live and
update emitter+44 bit4; otherwise update creation-parameter byte3 only while waiting;
then write component+45 bit1. The +14 emitter resource is distinct from the +1C
attached Thing. Do not use the entity attachment handle as emitter identity.
The owned gameplay command is not implemented yet: it needs guarded mutable
component execution, resource capabilities and explicit pending-creation ownership.

Candidate later roots: ClearAttachments 006E0AF0 (225), SetAsCarriedBy 00705D40
(526); disassembly is in work/modern_target_transitions_20261018, without executed
transition claims. Prior modern checkpoint input hashes still match (58 MSVC/clang,
41 GCC, 38 desktop-free). No game launch, Lua editing or canonical promotion.


## Owned particle AlwaysUpdate checkpoint (2026-10-18)

`ThingParticleAlwaysUpdate` now preserves the measured resource-resolution and
write order through a guarded component command. The component owns pending
creation state; emitter identity uses a separate host-validated resource handle.
The final component flag is written only after successful resource/pending work.
Missing parameters and stale resources return explicit errors; prior effects survive
failure. Read delegation does not authorize mutation. Commands currently require
an idle lifecycle context; resource loading/rendering and tick-time callers remain open.

All five builds pass: 59 tests each MSVC x86/x64 and clang-cl, 42 GCC, 39 headless,
plus all prior replays and 2,048 particle cases / 6,144 calls / 28,160 observations.
Frozen inputs, selected 50 canonical providers, registration source and four headers
stayed unchanged. Weak-resource callbacks remain controlled; native packed fields
are compared through documented semantic bool projections in the owned replay.
Receipt: `work/modern_owned_particle_always_20261018/validation/summary.json`.
Contract: `docs/modern/PARTICLE_ALWAYS_UPDATE.md`.
No game launch, Lua regeneration, installed-game write, canonical promotion,
commit or push. Full attachment/carrying transitions and representative gameplay
remain open.


## Native particle ClearAttachments checkpoint (2026-10-18)

Retail ClearAttachments 006E0AF0 now executes with actual weak lifetime helpers
and map lookup: 5,120 cases, 10,240 calls, 58,880 observations, identical repeat,
and nine rejected mutations. Tracker detachment, string assignment and allocation
release remain controlled. Full prepared data is compared after each case.
Proof: `work/modern_particle_clear_20261018/final_validation/`.
Contract: `docs/modern/PARTICLE_CLEAR_ATTACHMENTS.md`.

Donor call tracing corrects a misleading BSim label: retail 0071A8F0 is the
particle tracker's SetParticleAsDetached in this caller, not creature-action
AddDependentThing. Type216 is CTCParticleAttachmentTracker; its method removes
matching AttachedParticles list entries immediately. An executed tracker/list
comparison is still needed before an owned detach port.

The root updates reset flags before clearing the attached weak reference, then
assigns an empty position name, then clears final attachment flags and index.
Prior modern five-build inputs remain unchanged (59 MSVC/clang, 42 GCC, 39
headless), with the recorded README-only delta. No game launch or Lua changes.


## Native particle tracker removal checkpoint (2026-10-18)

The actual tracker detach callback, list removal, comparator, node erasure and
weak lifetime helpers now execute together with only allocation/deallocation
controlled. 1,059 cases / 2,118 calls / 61,451 observations pass, with an identical
repeat, full prepared-data comparisons and six rejected mutations. Exhaustive
short lists and seeded lists up to 32 entries cover duplicates, absent pointers,
shared expired records and owner-record creation.

The scan resolves the owner before each entry, removes every matching node,
preserves survivor order and lazily clears expired references even in surviving
nodes. Unlinking precedes weak destruction and node release. The temporary owner
reference survives the scan. Receipt:
`work/modern_particle_tracker_20261018/final_validation/`.
Contract: `docs/modern/PARTICLE_ATTACHMENT_TRACKER.md`.

Next: compose ClearAttachments with this actual tracker body, then establish
scoped access to both owned components for the modern command. Existing nested
command rejection must not be bypassed through raw pointers. Prior modern code
and selected canonical inputs remain unchanged; no game launch or Lua changes.


## Composed particle detachment checkpoint (2026-10-18)

ClearAttachments now executes with the actual tracker callback, list scan,
comparison, erasure and weak-lifetime code. Only allocation/deallocation and
string storage remain controlled. 8,297 cases / 16,594 calls / 217,781 observations
pass, with an identical repeat and all fifteen mutation controls rejected.
Self-attachment cases share the owner's weak record and cover alive/Dead owners;
tracker temporary ownership ends before the particle clears its own reference.
Receipt: `work/modern_particle_detach_composition_20261018/final_validation/`.
Contract: `docs/modern/PARTICLE_DETACH_COMPOSITION.md`.

Next: implement owned tracker/attachment state and guarded mutation of both
components, including two components on one owner. Existing read delegation
must not authorize mutation. Replay semantic state against this combined corpus.
Prior native references and modern five-build code inputs remain unchanged.
No game launch, Lua regeneration or canonical promotion occurred.


## Owned particle tracker state checkpoint (2026-10-18)

`ThingParticleTrackerState` now owns ordered validated entity references. Snapshot
replacement validates before changing state. Detach removes all matches, preserves
survivor order and lazily clears expired entries without deleting their null slots.
Dead owners remain valid; stale/foreign owners fail, and old generations never
revive. A live component's caller still supplies its lifecycle guard.

The owned replay matches 1,059 native cases and 2,118 ordered survivor lists plus
removal counts. Allocation/record counts and intermediate native observations are
explicitly excluded from this semantic projection, and remain in the native proof.
All five builds pass: 60 tests each MSVC x86/x64 and clang-cl, 43 GCC, 40 headless,
with every earlier replay. Frozen inputs and selected 50 canonical providers,
registration source and four headers stayed unchanged.
Receipt: `work/modern_owned_particle_tracker_20261018/validation/summary.json`.
Contract: `docs/modern/THING_PARTICLE_TRACKER_STATE.md`.

Next: guarded selection/mutation of two components, owned particle attachment
fields, and the full detach command against the combined native corpus. The global
access flag alone does not protect a second owner from destruction; acquire its
owner/type guards explicitly, with same-owner and exception tests. No game launch,
Lua edit, canonical promotion, commit or push occurred.


## Related-component command checkpoint and session pause (2026-10-18)

`ThingComponentAccess::execute_related` now holds the primary guard during
selection, validates the related identity and guards both components during
execution. Same-owner components and exact self-selection are supported. Public
recursion and read-delegated mutation remain rejected. Selection effects survive
acquisition failure; all scoped flags restore after failures and exceptions.

All five builds pass: 61 tests each MSVC x86/x64 and clang-cl, 44 GCC, 41 headless,
plus all previous replays (26 commands per build). Frozen inputs and selected
50 canonical providers, registration source and four headers stayed unchanged.
Receipt: `work/modern_component_relations_20261018/validation/summary.json`.
Contract: `docs/modern/THING_COMPONENT_RELATIONS.md`.

The user requested bedtime handoff and a stop for the next session. This
modernization lane is paused; its matrix finished and no lane-owned build/probe
process remains live. Next implementation: owned particle attachment fields and
ClearAttachments using the related-component command and owned tracker, compared
against the 8,297-case composed native reference. Full resume details are in
`docs/modern/NEXT_SESSION.md`. No game launch, Lua regeneration, installed-game
write, canonical promotion, commit or push occurred.
