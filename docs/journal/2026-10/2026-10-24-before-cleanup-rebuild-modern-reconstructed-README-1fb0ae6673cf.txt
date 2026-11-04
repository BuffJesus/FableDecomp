# Reconstructed engine compositions in the modern build

Five static libraries build 50 original reconstructed source units under current
MSVC/clang-cl, on x86 and x64:

- `fable_reconstructed_events`: 21 event/player/component-map/intelligent-pointer sources.
- `fable_reconstructed_containers`: eight symbol-map, resource-list and definition-manager
  sources. One source uses a generated copy with declaration-only corrections described below.
- `fable_reconstructed_font`: font-manager construction and font-pointer sharing/release.
- `fable_reconstructed_wire`: event append/serialization, event getters and six package-set providers,
  also supported by Windows GCC.
- `fable_reconstructed_main_game`: package processing, frame advancement, system-event handling
  and quit-flag updates, checksum selection/world checksum arithmetic, thing traversal and client state
  (requires both reconstructed options).

The libraries supply no runtime stubs; their engine service dependencies remain unresolved
until an application or a fixture supplies them. Forty source units compile directly
unchanged; the font-release template is included unchanged with its const instantiation, and
nine units use generated compatibility copies. No retail reconstruction coverage is added.

Two CTest executables link this library against the existing recording callback fixtures:
`fable_reconstructed.InventoryEvents` (30,920 scenarios) and
`fable_reconstructed.PlayerEvents` (8,224 scenarios). They preserve the original case domains,
call/argument checks and memory snapshots. They do not launch the game or create a renderer.
These are bounded raw-storage/callback fixtures, not fully constructed world/player lifetimes.

Player lookup (10,656 cases) and controlled-creature access/expired-reference cleanup
(9,216 cases) now execute their real player-manager and intelligent-pointer providers.
Font-pointer sharing covers 1,200 alias/refcount/re-entrant callback cases. The portable
observer checks non-null resource frees exactly and reports null-delete calls separately:
MSVC makes 20 and Clang makes none. C++ leaves that null-call choice unspecified; this does
not claim identical allocator-call traces. [C++ delete-expression rule](https://eel.is/c++draft/expr.delete#6).

## Engine event bridge

`fable_engine_event_bridge` connects the portable `multiplayer::GameEventCodec` to the original
`CGameEvent`. It preserves the ID, replacement bit, source/player byte and payload; initializes
unused payload bytes; and rejects invalid outgoing events before reading unset fields, IDs
outside the wire range, and payloads above 32 bytes. Incoming decoding retains the codec's
distinct truncated-header/payload and oversized-payload errors and reports bytes consumed.
The bridge uses the reconstructed `AddData` method for bounded nonempty payloads.

`GameEventBridge` checks **84,480 cases** against the actual reconstructed serializer: five
IDs, all 256 source bytes, both replacement states, and every payload length 0..32. It also
checks buffer guards, unchanged source objects, initialized unused bytes, malformed input,
invalid engine states, and concatenated-record consumption. The serializer copy replaces its
local duplicate class with the shared class, adds the method declaration to the copied header,
and maps member spellings to that header; the wire algorithm is unchanged.

`PlayerCreatureReplay` materializes serialized events and invokes the registered creature
action through its real world/player lookup and pointer cleanup chain. It retains the full
9,216-case access fixture; 3,072 of those cases replay events into the world action. Its event
ID (71) is derived from the original 87-row registration source. Transport allocations finish
before the fixture's core deletion-observation window. This establishes a bounded event path;
raw input/action mapping, RNG/world-state replay and full dispatch remain open.

`GameEventPackageCodec` now implements the original timestamped package framing, capped at
50 packages and 40 events each (72,251 bytes maximum). The package bridge owns the original
large aggregate on the heap, initializes unused slots, checks native counts/event validity,
and preserves every timestamp bit and event order. It copies materialized events directly,
so duplicate replacement records are not collapsed by collection-time semantics.
`GameEventPackageBridge` compares **660 combinations** against the actual original package-set
writer, including empty/full counts, all payload lengths, replacement records, guards and input
immutability. Portable tests reject all **72,251 truncations** of the largest batch, plus fixed
expected bytes and malformed count/payload cases. The bounded player replay above now passes
its 3,072 events through recorder-file writing/reading, timestamped package serialization,
decoding and native materialization. The framing uses the actual `NoPlayers` field and
length-prefixed package records; reader EOF is checked before the world action. These are
synthetic bounded recordings, with no world-tick dispatch or RNG/world replay. Those and
raw action mapping remain open.

## Main-game package replay

`MainGamePackageReplay` executes the original `ProcessEventPackage` and
`UpdateFromEventPackageSet` against real package getters. Its recording event boundary checks
event pointers, order, payloads and state; world, checksum, network and display calls remain
recording callbacks. `MainGameSystemReplay` instead links the original `ProcessEvent` and
`SetQuit` bodies. A compiler-generated five-slot virtual-interface shim supplies the quit-call
boundary and invokes the original flag setter. Gameplay events continue to the world boundary;
the component itself handles only quit.

Both fixtures cover **1,600 cases**: five initial input frames, five server frames, both initial
render/quit flags and 16 recording shapes. Cases include empty/full capacities, stale, duplicate,
unsorted and signed-limit timestamps, and state carried across several file records. Checks
cover state at every callback, render permission versus the sticky AllowRender flag, advancing
input/server frames, checksum/seed/network arguments, whole-component/guard changes and
immutable package inputs. A separate literal retail-fixture anchor applies 11/12/13/20 from
8/11/12/13/13/20 and requires render permissions false/false/true/true and server frames
12/12/13/20. Callback totals agree across x86/x64 and Clang: **76,320** package-order and
**37,920** system-handler observations.

`MainGameStateReplay` replaces the checksum-selection/world-sum and client-update boundaries
with the actual original providers. Its **12,800 cases** add all eight local/loading/saving
combinations. A plain local game returns checksum 1 even with a null world pointer; remote
selection works with a null input-manager pointer. Real world checksum arithmetic preserves
32-bit wraparound across signed frame values. Real local-client updates store the current
server frame, checksum and seed; remote updates preserve every byte. Direct probes also change
all three arguments twice, including backwards frames and extreme values. The input manager,
world frame, manager storage and unapproved component bytes must remain unchanged.

The thing-manager aggregate remains a recording leaf. A controlled world callback varies its
returned sum between packages, checking successive native client updates; this is fixture
stimulus rather than an implemented world algorithm. Cross-compiler observations match:
**260,880 callbacks**, **33,040 replay leaf queries**, **11,200 direct checksum queries** and
**25,600 direct client updates**. The copied client header adds the same test friend for named
field access. The following Aggregate fixture replaces its thing-manager recording leaf.

`MainGameAggregateReplay` runs the real thing-manager traversal through the same **12,800-case**
recorded pipeline. Slot-23 dispatch now invokes the typed
`fable_thing_checksum_bridge` adapter and portable simulation checksum calculation.
Test stimulus supplies kinematic/definition/player fields; the callback no longer
returns a stored checksum word. Empty lists, dead entries and changing/wrapping sums are checked
before the real world checksum and client updates. Exact virtual-call order and immutable
manager/node/thing storage are checked. Cross-compiler totals match: **240,020 callbacks**,
**12,180 replay thing queries**, **7,000 direct thing queries**, **25,600 direct client updates**,
and **19,180 actual typed-adapter queries**. Metadata/physics/thing storage is checked
immediately around every query. See [integration](../integration/README.md) for the
109,608-record retail comparison and the remaining borrowed-lifetime qualification.

The modern factory copy names the donor's fields using the retail STLport vector/list types.
The vector element stays incomplete in the traversal unit. The integration unit
privately defines the bounded type-info/physics representations used by the generic
checksum adapter; their allocation/destruction and factory lifetimes are not reconstructed.
The list pointer stays at retail x86 +0x18 (factory size 0x1c, manager prefix 0x34); native x64
moves it to +0x28 (sizes 0x30/0x60). A generated source copy changes only the two list-head
loads to the named field, preserving the loop's sentinel reload after virtual callbacks.
`ThingManagerChecksumParity` retains all **80** original cases, including sentinel mutation;
the unchanged VC7.1 fixture and modern x86/x64 copies also pass a standalone comparison.
Field offsets and sizes are independently checked before the modern fixture runs.

The copied main-game header adds a manifest-recorded friend for named-field fixture access;
no layout or algorithms change. Storage uses native type sizes/alignment and a pointer-sized
vptr copy. This remains a raw-storage/virtual-boundary composition, not a constructed component
lifetime, the real main-game vtable, world simulation, networking or rendering. The library
providers themselves contain no test doubles. The package bridge also uses the real event
getters when converting outgoing native state, including on Windows GCC.

The six package-set sources change only their include to a generated narrow header extracted
from the original class declaration. The copied broad gameplay header includes that same narrow
header, and MSVC/Clang tests include both paths to check compatibility. Provider algorithms are
unchanged; the original writer retains target-specific typed/unaligned accesses on Windows.
Portable encoding/decoding uses explicit byte operations instead.

Five further fixtures link the container/font libraries: symbol-map construction, destruction
and repeated Clear (1,458 lifetime/1,458 Clear scenarios), definition-manager construction, and
font-manager construction with empty/nonempty handles. Resource-list providers come from the
library rather than being included into the fixture. Resource-list construction is exercised;
the linked destructor/allocation-handler bodies do not gain separate runtime coverage here.
Font-bank and vertex-buffer behavior remains recording scaffolding: no actual file, GPU,
font streaming or full manager destruction is tested. Its 28 unused bank virtual slots have
compiler-checked aborting definitions instead of x86-only decorated-symbol aliases.

The fixture runner requires exit code zero **and** an exact output marker for the original case
domain. It rejects a failed process even when the process printed that marker.

`fable_reconstructed.SnapshotPreparation` also checks cache reuse, rejection of an unexpected
generated file, rejection of output inside parity inputs, and recovery into a new directory
after an interrupted preparation. Test artifacts are preserved in the build directory.
The same Python suite checks the fixture-runner success/failure contract.

The gameplay/container/font targets default on for MSVC/clang-cl on Windows
(`FABLE_BUILD_RECONSTRUCTED_EVENTS`). The wire/bridge targets default on for Windows, including
GCC (`FABLE_BUILD_RECONSTRUCTED_WIRE`). Set both options OFF for only the portable modules.
Both default off on Linux; the original engine's 32-bit-long layout is not an LP64 layout.

`tools/modern/prepare_reconstructed_events.py` generates build-only header and harness snapshots.
Each snapshot is keyed by the complete header/adapter/fixture inputs and has a manifest of input
and generated-file hashes. Reconfiguration reuses identical snapshots, verifies they are unchanged,
and creates a new directory when inputs change. Prior snapshots and failures are preserved.
Neither parity sources, parity fixtures nor shared headers are edited.

The copied headers disable the retail pointer-width/layout assertions, substitute
`compat::retail_bitset<274>` for the known VC7.1 library-layout dependency, and explicitly declare
the vector-map destructor so modern compilers accept its specializations. The copied fixtures
use type-sized/aligned raw buffers and a named controlled-creature member address. The player
fixture also declares its reused loop index explicitly. `/Zc:wchar_t-` stays confined to this
library family and its consumers; the other modern modules keep native `wchar_t`.

The symbol-map lifetime source needs a syntax adaptation for Clang: declare its `_M_erase`
specialization before the first use, and remove three redundant explicit instantiations of
already explicit specializations. The set/vector-map primary destructor bodies remain empty
and are defined out of class in copied headers so both compilers emit the specialization
providers consistently. The copied vector-map header also includes the algorithm declarations
it uses, enabling conforming `/permissive-` throughout the MSVC reconstructed targets. All edits
are in the snapshot manifest; original sources/headers remain unchanged. Existing narrowing
warnings in borrowed code remain open.

Local validation: MSVC x86/x64 and clang-cl x64 pass all **33** modern-tree CTests. Windows GCC
passes **17**, including the original wire/package providers, bridges and snapshot tests. GCC gameplay
headers and Linux execution of the new original-header targets remain unsupported.

This does not establish whole-engine x64 layout correctness, Linux engine support, deterministic
world replay or new retail reconstruction coverage. See the modernization journal for the original
VC7.1/modern-x86/x64 comparisons and the remaining engine integration work.
