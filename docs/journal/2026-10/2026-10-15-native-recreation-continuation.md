# Native recreation resumed October 15

The user resumed recreation until asked to stop, excluding Lua. The October 14
bedtime pause is superseded. Work remains in the original-compatible native lane.

## Prior standalone observer run

At resume, `layout_observer_native_FIRST/` contains the launch/preflight records,
44 progress lines and empty stderr, but no `process.json`, `result.json`, timeout
record or post-run guard. Its last line reports 45,056 cases, 991,232 checkpoint
comparisons and 1,267,712 callback comparisons. PID 15544 is absent. These partial
records do not establish completion or a reason for termination. No restart was
performed. A separate resume-time input audit is in progress; it cannot replace
the missing original terminal evidence.

## Frontend review dependency

Preserved `sign_frontend_registration_native_once.py` and derived
`sign_frontend_registration_native_parser_bound_once.py` under the October 14
`shader-layout-chain/` directory. The new script checks and pins the separately
loaded `independent_review_saved_layout_pe.py` parser against the executable
review's `coff_parser_source_sha256`, before signoff. It also records that source
and hash in the signoff output. Its SHA256 is
`a53018d01c11d8b957ee35487857f65470d971b5b49840df43108aa4594dad58`.
Syntax inspection passed, and the actual parser matches the saved review field.
The signoff itself has not run. Pending independent seals and runner review
remain pending; this edit grants no native acceptance.

## Frame and registration composition draft

New work is grouped under
`work/native-recreation/frontend/0042df9e/20261015/`.
`prepare_frame_registration_draft.py` ran once, producing twelve source/header
files and a draft inventory in `frame-registration-DRAFT/`. Parent frozen inputs
and previous executables are preserved.

This inactive draft replaces frontend display BeginRender, EndRender and
SwapScreens substitutes with dependencies on retained genuine display/core
providers. Five full original frame bodies are copied separately, with six
explicit relative-call fixups. Original frontend selectors 20/21/22 route to
those original bodies. Device callbacks check receivers, arguments and call
counts; frame state remains in the compared arena. The existing five layout
scenarios and 2,949,120-case loop are retained.

Clock behavior remains a deterministic shared service in this draft, with
quarter-second progression to exercise elapsed-frame calculation. This is an
intermediate frame integration, not the completed clock-branch convergence.
Earlier owner-mutation/clock cases are preserved separately and are not claimed
as covered by this matrix. Full source review, retained-object inventory and
later build/executable checks remain necessary. No draft compiler, linker or
native invocation has run.

## Diagnostic frame build and first run

The resume-time audit completed: all 36,903 recorded input hashes still match,
and a process query found no surviving observer executable. Receipt:
`20261015/resume-audit/prior_observer_incomplete.json`. The 44 progress lines remain
partial evidence only; no original exit status or termination cause is known.

The frame input plan checked all eleven parent frozen sources, 63 retained
support objects and three added frame objects, plus all six original relative
call targets. Its initial five-symbol assertion omitted two compiler-generated
scoped-pointer helpers. That stop is preserved; the corrected inventory includes
both helpers and finds no new duplicate definitions.

A separate diagnostic source copy replaces only stale leading comments.
VC7.1 compilation passed, with no unbound includes or input drift. One driver
preflight first stopped because lookup inventories are name lists rather than
hash dictionaries; no compiler had run. The corrected driver resolves hashes
through the recorded input map. Compiler object:
`207717e19c5f9e2d1ef6307a5e8aa948b820c42e567ad1e1e71b9c49cc63007c`.

The first diagnostic link passed with 67 objects and no provider rebuild.
Executable SHA256:
`905d176a89ec4655a440d97c548e239c6565be97425f0101e51d49f0d6d3602c`.
Root PE inspection checked 203 mapped code symbols, 222 selected whole sections
and 87 original arrays. Five frame methods resolve to retained providers; clock
still resolves to the fixture. This is root diagnostic review, not an independent
review receipt. Actual compiler/link inputs and inherited lookup roots were
checked; no historical all-evidence transitive guard is claimed for this build.

The first diagnostic native run is active: session `53957`, PID `8380`, under
`20261015/frame-registration-diagnostic-native-FIRST/`, fixed 5,400-second limit.
Do not restart it. The exact 2,949,120-case / 5,898,240-snapshot output, exit status,
stderr and post-run input guard must pass before reporting diagnostic success.
Even a pass remains pending independent review and is not canonical coverage.

## Real clock composition

The separate `clock-frame-registration-DRAFT/` adds the retained genuine 168-byte
clock provider, its fourteen original absolute-operand fixups, per-side timer
state snapshots and controlled Windows timer callbacks. Existing fixture axes
cover all 64 seed/policy pairs; they are not additional Cartesian axes. The full
2,949,120-case matrix remains. The original frontend and original core frame
methods call the copied original clock; current callers use the real provider.

Diagnostic compilation passed with all actual includes bound. First link failed
because `winmm.lib` is absent from VC7.1's default library directory; this attempt
is preserved. A new link uses the earlier clock fixture's exact Windows SDK
WinMM import library, hash `046689f4...`, and passed without input drift.
Executable SHA256 is
`1a2c99c04fe6a4187aa3675b696c69e600d64ea4f05576a769f39df8a60af271`.
Root inspection checked 222 mapped code symbols, 238 whole sections, 88 original
arrays, all three current clock callers and the three named timer imports.

First real-clock diagnostic native invocation is active: session `10904`, PID
`14924`, output `clock-frame-registration-diagnostic-native-FIRST/`, fixed 5,400
seconds. Do not restart. No pass or independent closure has been reported.

## Float viewport conversion failure preserved

The remaining float viewport wrapper at `009BF490` is legacy assembly. Its
original reads bottom/right/top/left and uses four **32-bit** x87 FISTP stores.
VC7.1's own help documents `/QIfist`; a separate genuine C++ candidate and a
399,360-case differential fixture were built under
`work/native-recreation/rendering/009bf490/20261015/cpp-first/`.

Actual code inspection found four **64-bit** FISTP stores followed by low-word
extraction, 76 bytes / 23 instructions versus retail's 112 / 31. The first native
diagnostic stopped at case 336 (pattern 42, first precision/rounding mode):
status `80011` versus `80001`. This includes the signed 32-bit overflow input
`0x4f000000`. The candidate is rejected, with no input drift, integration or
coverage credit. `native-FIRST/result.json` preserves the exit-6 mismatch.

A bounded compile-only type probe found the same 64-bit store for signed int,
signed long, unsigned int and unsigned long under that documented profile.
Changing spelling among those types does not solve the exception semantics.
No assembly provider was added and the original canonical files are unchanged.

## Concurrent workspace work

Another workflow records its own registration comparison and vector-clear/
projection work in `2026-10-15-modernization-continuation.md`. Its process and
artifacts are outside this session's ownership. This session's two native PIDs
are 8380 and 14924; no other workflow's inputs or processes were changed.

## Post-presentation callback, bounded path

Under `work/native-recreation/rendering/009f73c0/20261015/`, a new standalone
original/current comparison covers valid empty query lists and failed query
creation. It observes frame counter wraparound, both GPU-info indices, clock
callback mutations and timestamp bit patterns across twelve x87 modes. List
helpers and clock are shared boundaries; allocation and wait-loop behavior are
outside this test.

The first original relocation inventory missed `ADD ECX, 013CB4C0` at byte 661.
The resulting access violation and a separate GDB fault-localization invocation
are preserved in `empty-query-first/`. A fresh `empty-query-immediate-fix/`
fixture adds the typed immediate relocation and reuses the unchanged provider
object. It passed **6,144 comparisons / 12,288 snapshots**, exit zero, empty stderr
and unchanged inputs. The fixed `/Od` provider has 184 instructions versus 184
retail instructions, 97.8261% similarity, meeting near-match. This remains bounded
diagnostic evidence pending independent review, not a new canonical claim.

The PE inspector also needed a correction: a named relocation can resolve to
another object's selected COMDAT. Its old assumption inferred a base for an
unselected variant. The corrected inspector verifies the named destination and
requires a complete matching selected section representative. Both reviewer
stops are preserved; no engine instructions changed to satisfy the inspector.

## Post-presentation frontend composition

The first integrated fixture compiled and linked, but its native diagnostic
crashed. A separate GDB invocation localized execution to timer data in the
arena: timer initialization at `+e000` had overwritten the existing menu vtable
at that address. This is a fixture-layout defect. The failure, debugger output
and all build inputs remain under `post-swap-clock-frame-registration-*`.

Fresh `post-swap-storage-progress-*` artifacts move the timer to `+f000`, its
sentinels to `+f020/+f040`, and the opaque failed-create handle to `+f180`.
The menu remains at `+e000`. Setup explicitly verifies the menu draw slot.
Only pending-frame bytes `+f00c..+f010` are writable in the new object range;
the other object and sentinel bytes remain guarded. Progress is flushed after
every 4,096 completed paired comparisons, outside comparison windows. The full
2,949,120-case corpus is unchanged.

Corrected compile/link passed without provider recompilation or input drift.
Executable SHA256:
`4b0063867d1e1309b7fe7c6e1c66178a2be0d5918e526deaba71336baef9a0f8`.
Root PE review checked 246 mapped code symbols, 255 selected sections, 89 original
arrays, the genuine post-swap call and all five current clock call sites.
The output protocol accepts exactly 720 ordered progress records plus the final
marker; twelve malformed/partial/process-failure synthetic variants were rejected.

The corrected first native invocation is active: session **12960**, PID **16376**,
output `post-swap-storage-progress-native-FIRST/`, fixed 5,400-second limit.
The earlier frame-only and clock/frame runs remain separate. Do not restart any
of these runs. Inspect saved terminal evidence before reporting a result.

## Existing-query callback comparison

`rendering/009f73c0/20261015/existing-query-first/` extends the standalone parent
comparison to zero through two pending/free queries, successful polls, temporary
not-ready replies followed by success, error cleanup, and both Issue return
classes. Successful creation of a new query remains excluded, so every existing
query payload is fully initialized and no unspecified timestamp bytes are masked.

The provider object is reused. Only its undefined `_free` and Sleep-import symbol
names are adapted to deterministic test services; every raw section and relocation
index is unchanged. The new fixture binds those services and shared insertion/
ClearAllQueries behavior to a resettable sixteen-node pool. Current and original
receive the same storage addresses, and the entire arena, pool-use flags, callback
sequence, counters, global timestamps and FP state are compared.

One fixture compile and link passed. Root PE inspection covered 42 mapped symbols
and 39 selected sections. The first native comparison passed **331,776 cases /
663,552 snapshots / twelve x87 modes**, exit zero, empty stderr and no input drift.
Result: `existing-query-first/native-FIRST/result.json`. This is diagnostic parent
behavior with shared services, pending independent review, not recovery credit for
the allocator/list/clear services or complete query creation/lifetime behavior.


## Genuine query cleanup and completed frame diagnostic

`rendering/009f73c0/20261015/real-query-clear-first/` replaces the common
ClearAllQueries boundary with the original 211-byte routine and the genuine
canonical C++ provider compiled at its documented /Od setting. The query Release
callbacks and list allocator remain controlled services. Root structural inspection
verified 42 mapped symbols / 39 selected sections. Its first inspection used a
public-method decorated name for a private method; that script is preserved as
`inspect_pe_public_name_attempt.py`. Correcting the name to the actual map symbol
required no build or provider changes.

The parent comparison passed 331,776 cases / 663,552 snapshots across twelve x87
modes, with empty stderr and no input drift. Cleanup itself has 59 instructions
against retail's 59, 98.305% normalized similarity, and complete decoding: near
match without tuning. `clear_similarity.json` pins the measured object.

`direct-query-clear-first/` adds direct cleanup of zero through two entries in
each list, including free-list-only states and repeated cleanup. All 432 cases /
864 snapshots passed. Both runs remain root diagnostics pending independent
review; shared list primitives, allocator and COM callbacks are still qualified
boundaries. No canonical coverage or complete GPU lifetime claim.

The long `frame-registration-diagnostic-native-FIRST` run completed successfully:
2,949,120 cases / 5,898,240 snapshots, exit zero, exact output, empty stderr and no
input drift. Real-clock and post-swap composition runs continue separately.


## Query-list oracle expansion

Three fresh comparisons progressively remove shared retail-side list helpers:
`real-query-pop-first`, `real-query-list-first`, and `real-query-push-first` under
`rendering/009f73c0/20261015/`. Each passed 331,776 cases / 663,552 snapshots with
empty stderr and no input drift. The final fixture uses full copied originals for
post-swap, cleanup, pop_back, size, iterator construction/increment/decrement/
inequality, and both push directions. The allocator/free, node creation, timing,
Sleep and query/device callbacks remain controlled. Retail free thunk BFEA14
resolves through IAT 1440154 to MSVCR71.dll!free, independently corroborated by the
FSE address label. Original code arrays and all selected current object sections
are bound in the root PE inspection (final: 52 symbols / 49 sections).

This establishes behavior on initialized existing queries and failed creation;
it does not establish near-match grades for the generic list helper bodies or
successful new-query lifetimes. The root diagnostics are not independent closure.

`clock-frame-registration-diagnostic-native-FIRST` also completed its full
2,949,120-case / 5,898,240-snapshot matrix with exact output, empty stderr, exit zero
and no input drift. The post-swap storage/progress composition remains running.


## Real clock, node providers, and longer queues

`real-query-clock-first` passed 331,776 paired cases with genuine GFGetTime on
both sides, deterministic own-executable timer IAT callbacks and all 64 seed/policy
combinations derived from existing fixture axes. The earlier explicit clock-return
and timestamp-mutation fixture remains separately preserved.

`real-query-node-first` compiled the genuine canonical NodeOperations source,
removed the fixture insertion/node-copy implementation, and adapted only its
undefined malloc symbol to the deterministic pool. Original node construction is
137 bytes with original payload copies and shared malloc/placement boundaries.
Its first PE review rejected the selected push providers: post.obj COMDATs won.
No native execution occurred for that rejected link. `real-query-node-preferred`
relinks identical objects with node.obj first; selected providers verified and
331,776 paired cases passed. This /Od-only node build failed near-match scoring.

The recorded canonical gate `tools/decomp_pipeline/gate_timer_query_nodes.py`
requires /Od /Ob1 /MT. `real-query-node-recorded-recipe` recompiles only that source
with those flags, reuses the other objects, verifies the selected node/push bodies,
and passes 331,776 cases / 663,552 snapshots. Near results reproduce the recorded
131/137-byte create (97.778%), 113/119-byte push_front (95.238%), and 111/117-byte
push_back (95.122%). No source tuning or canonical edits were made.

`real-query-long-queues` reuses these providers and extends pending/free counts
to zero through seven (fourteen existing query handles total), with disjoint IDs,
a sixteen-node deterministic pool, full arena/pool/callback/global/FP comparisons,
and twelve x87 modes. Its first run passes **2,359,296 cases / 4,718,592 snapshots**,
empty stderr and no input drift. Root PE proof covers 72 symbols / 66 sections.
The inherited result qualification text conservatively still mentions shared list/
clock services; the exact final marker and provider proof identify the genuine
node/clock providers. Remaining boundaries are allocator, placement-new identity,
COM/timer/Sleep callbacks and borrowed timer/list lifetime. Successful query creation
and allocation failure remain excluded. These are root diagnostics, not independent
closure or canonical promotion.


## Reset and actual timer/list lifetime

`query-reset-sequence` passed 2,359,296 cases / 4,718,592 snapshots after composing
OnPostSwapScreens with two genuine OnPreDeviceReset calls. All existing queries
are released and both lists/pool are empty; original and current release/free order
and complete state agree. Only fixture/reset source was compiled; prior providers
were reused. The original reset body is 19 bytes and calls genuine cleanup.

`query-owned-lifetime` adds genuine construction/destruction and list.clear, all
compiled with the recorded /Od /Ob1 /MT recipe. Original bodies (354/107/92 bytes)
are copied in full with typed absolute/call fixups. Current allocator undefined
symbols alone are rebound to the same deterministic pool; all COFF raw sections
and relocation indices remain unchanged. Construction actually owns the two
sentinel allocations. The pool grows to 32 nodes and query handles move to arena
+2048 to avoid overlap. Immediate construction state, all globals, pool flags,
callback trace and FP state are compared before existing-query population.

Its first native run passed 2,359,296 cases / 4,718,592 terminal snapshots, plus
constructor comparisons in every case; exit zero, empty stderr and no input drift.
Root PE proof covers 77 symbols / 72 sections. Fresh similarity: constructor
342/354 bytes, 84/88 instructions, 96.512%; destructor 107/107 bytes, 37/37
instructions, 100% normalized; list.clear 92/92 bytes, 37/37 instructions, 94.595%.
All three meet near thresholds. These remain root diagnostics, not independent
closure; actual system allocator/COM/GPU operation and successful new-query creation
are excluded. The inherited result qualification is conservative/stale regarding
shared list/clock services and borrowed lifetime; this section and selected provider
proof describe the exact expanded scope.

`query-lifetime-reset-modes` is now running 7,077,888 cases: the same lifetime
sequence with zero, one, or two reset callbacks before destruction, so nonempty
destructor cleanup is exercised. Native timeout 360 seconds, session 31472; do not
restart it if the handle is lost. Larger frontend post-swap composition is also
still running separately.


## Full reset modes, omission checks, and GetInfo extension

`query-lifetime-reset-modes/native-FIRST/result.json` completed successfully:
**7,077,888 cases / 14,155,776 terminal snapshots**, with constructor comparisons
in every case, twelve x87 modes, zero/one/two resets, and no input drift. All eight
required-provider omissions fail for their expected unresolved symbols, with no
compiler or native invocations in the omission check. Receipt:
`query-lifetime-reset-modes/omission-FIRST/result.json`.

`query-lifetime-info` adds the genuine GetInfo source (/O2, its source pragmas
disable optimization/inline depth), full original GetInfo138B and info-constructor46B,
and three read checkpoints: before swap, after swap, and after resets. Both sides
use real Windows Interlocked calls on fixture-owned globals in one thread. Every
named returned field is compared. Padding at offsets20..23 and44..47 is excluded:
the original constructor never initializes those bytes and GetInfo copies twelve
stack words; the decision is recorded explicitly in info_scope.json. Root PE proof
passes82symbols/77sections. First native run is active (session60536,360second
limit); preserve terminal result before considering further changes.


## All three large frontend diagnostics complete

`frontend/0042df9e/20261015/post-swap-storage-progress-native-FIRST/result.json`
now passes: 2,949,120 cases / 5,898,240 snapshots, all720 ordered progress records,
exact final output, exit zero, empty stderr, no timeout, no input drift. Sessions
53957,10904,12960 are complete; do not restart them. These three frame variants
remain root diagnostics pending independent closure. The post-swap composition
retains empty-query failure/device/UI/timer-API and borrowed-lifetime boundaries;
the larger owned-query lifecycle comparison is a separate fixture.

`query-lifetime-info` also completed7,077,888 cases,14,155,776 terminal snapshots
and three named-field GetInfo comparisons per case, with no input drift.
`query-info-boundary-mutations` now tests deterministic changes at increment/
decrement boundaries: latest-index switching, CPU timestamp/count updates before
copy, and selected-block timestamp/count updates after copy. The actual atomic
operation still executes on owned globals. Only two undefined import-symbol names
in the unchanged GetInfo object are rebound to those wrappers; raw sections and
relocation indices are checked unchanged. No actual concurrent-thread guarantee
is claimed. The fixture event storage grows to512 entries. Root selected-provider
proof passes88symbols/81sections; native session75952 is active,360second timeout.


## GetInfo boundary result and active full frontend query composition

`query-info-boundary-mutations/native-FIRST/result.json` passed all7,077,888cases,
14,155,776terminal snapshots and three getter checkpoints per case, with no input
drift. Session75952 is complete. This validates deterministic atomic-boundary
interleavings only, not arbitrary multithreaded access.

`frontend/0042df9e/20261015/existing-query-frame-first/` now connects retained
node-creation/push and cleanup providers to the full frontend/frame/clock/shader/
registration fixture. Original-side size/iterator/pop/push/node/cleanup routines
are full copied bodies. Zero/one pending and reusable query counts derive from
Mutation/ErrorMode; polling policy derives from the five existing layout cases.
The original full matrix is preserved rather than multiplied by new axes. Raw
arena guards explicitly permit only Pending/FinishedFrame, sentinel links and the
bounded eight-node pool; query handles/vtable and intervening guards stay protected.
Pool flags/counters, complete arena/global state, traces, FP state, borrowed-heap
state and all earlier guards are compared. Query calls and final pool occupancy
also have independent expected-count checks.

One fixture compile, no provider rebuilds;71 retained/current objects link.
Root PE proof covers268symbols/276sections/94original arrays, verifies genuine
node/clear/post selections, five clock calls and one post-swap call. New strict
progress protocol has one positive and ten negative synthetic tests. The first
native run is active: session26505,PID12336,timeout5400seconds. Last observed
16,384completed paired cases. Output is under existing-query-frame-first/native-FIRST;
check saved terminal evidence before any rerun. Controlled allocator, timer APIs,
device/UI, failed new-query creation and borrowed timer/sentinel lifetime remain.
The separately passed owned-lifecycle fixture is not silently merged into this claim.


## Native IME coordinator and cursor-helper recovery

`ui/00404c00/20261015/native-first` directly compares full original69B and genuine
C++ CIME::Render plus its alpha data provider. Existing oracle callback semantics
are retained and expanded to all256 alpha values and12x87 modes. First native
run passes196,608cases/983,040complete16KB callback-state snapshots, empty stderr,
no drift. Root PE11symbols/14sections; source body69B/22instructions versus same
retail counts,95.455% normalized similarity (near). Controlled edit-colour, IME UI,
manager singleton and sprite draw remain boundaries, no pixels rendered.

Next helper0041A980, NUISystem::CSprite::DrawWithRenderManager, has no existing
provider. Evidence at `ui/0041a980/20261015/evidence-first`: full377B/114instruction
body, eight direct calls, unsigned-to-float and0.5constants, retail sprite vtable
0122F5D4 slot106 ->0052F1E0, and DIA donor fields/methods for Ego_r and FableWin.
Slot106 is IsPositionIsCenter. Distinct donor layouts are not treated as retail
byte offsets. RelativeRenderPosition/RelativeRenderZoom, Size, Width/Height and
GraphicIndex are reconciled with retail accesses. Draw2DScaledGraphic9DD210 is
retail ret20, despite stale manifest-gap bytes showing another shape. Donor
signature is const box&, const graphic bank&, three unsigned-long arguments.

A broad XML scan was cancelled after it proved unhelpful; existing DIA helpers
completed the donor queries. No external/repository processes were interrupted.
`cpp-first` preserves an offsetof-declaration build failure. `cpp-include-fix`
adds only stddef.h, with identical candidate body and /O2 /MT flags. It builds
371B/111instructions versus377B/114,84.444% normalized similarity: below near.
No tuning or canonical promotion. Four staged headers supply a partial, explicitly
qualified object view and required method declarations; canonical headers remain
unchanged. Candidate uses the observed rectangle/centering/coordinate conversion
and counted-bank ownership sequence. Bounded native original/current behavior,
especially x87 ordering and callback lifetime effects, is the next step.

## Cursor candidate reaches near and passes native behavior

`ui/0041a980/20261015/native-candidate-first` passes884,736cases and
16,809,984complete16KB callback snapshots across12x87 modes. It includes unsigned
width/height boundaries, exceptional floats, centering, geometry mutations,
manager changes, counted-bank lifetime changes and controlled deletion callbacks.
Coordinate conversion, drawing and service lookup remain controlled boundaries.
The initial candidate's84.444% similarity does not qualify it for promotion.

`cpp-sequenced-size` follows the original width-before-height conversion and
size-before-position addition order explicitly in ordinary C++. Same /O2 /MT
recipe and staged headers:377B/114instructions,100% normalized similarity,
nearPASS but NOT relocation-exact. Candidate SHA256
`a882f98166806b93b71a47a22af2649feb809b48a1e82b10c11a2c55491f5ea3`.
`native-sequenced-size` reuses the fixture object and relinks only the new
candidate. Root PE20symbols/24sections; first run passes the same884,736cases/
16,809,984snapshots, exit0, empty stderr, no drift. Stop byte tuning here.
Canonical headers, provider inventory and coverage remain unchanged; partial
borrowed class views and independent closure remain open.

`ime-composition-first` now joins the genuine IME and cursor providers, both
reused without recompilation, against both full retail bodies. Only the fixture
is compiled. Root PE23symbols/28sections verifies both selected providers and
both original arrays. Five IME paths include disabled, no cursor, cursor with
and without edit colour, and cursor installed by the UI callback. The cursor's
full matrix and callback mutations remain. First native invocation is active
(session97732), budget540seconds, expected4,423,680cases/71,663,616snapshots.
Do not infer success until the saved terminal result exists. No pixels or GPU
operations are exercised.

## IME/cursor composition and genuine coordinate chain

`ime-composition-first` terminal result is PASS:4,423,680cases and71,663,616
snapshots, exit0, empty stderr, no drift. All three provider omission links fail
with the expected unresolved symbol. Its inherited result qualification's100%
normalized figure refers to the cursor only; IME remains95.455% near.

`coordinate-composition-first` replaces both shared conversion callbacks with
full retail/genuine C++ GFAXToVX, GFAYToVY and GetRenderTargetDimensions. Actual
virtual-dimension data provider included. Root PE22symbols/27sections. X40B and
dimensions24B are relocationMATCH; Y49B/14instructions is92.857% near. First
native run passes884,736cases/16,809,984snapshots, empty stderr, no drift. The
matrix includes signed/zero dimensions, double exceptional inputs, and changed
display/global values inside the system callback. Five provider omission links
fail with the specific expected missing symbol. Work-only, no canonical grades.

`ime-coordinate-composition-first` reuses all seven provider objects from those
passing fixtures, compiling only a new combined fixture. Root PE25symbols/
31sections verifies the linked providers and full retail arrays. Seven required
provider omission links pass. First native run active(session74933), expected
4,423,680cases/71,663,616snapshots,540second budget. System lookup, graphics draw,
IME UI, edit colour, centering and deletion callbacks remain boundaries. Root
review only; no independent closure, pixels or GPU operations claimed.

## Existing-query frontend terminal pass and centering getter

`frontend/0042df9e/20261015/existing-query-frame-first/native-FIRST/result.json`
is now PASS:2,949,120cases/5,898,240snapshots,720ordered progress records,
exact terminal output, exit0, no timeout, no drift and empty stderr. This closes
the first root diagnostic invocation only. Initialized queries, real cleanup/
node operations and real clock execute inside the frame chain; allocator,
timer APIs, device/UI and failed creation remain controlled. Borrowed timer/
sentinel lifetime and independent closure are still open.

`ui/0052f1e0/20261015/native-first` reconstructs
CComponent::IsPositionIsCenter from retail byte +0x12E and donor FlagsSetTwo
(one-byte field, virtual slot106). Staged partial header gains only that field
and its offset assertion. Genuine C++ `(unsigned(FlagsSetTwo)>>1)&1` compiles
14B/5instructions, exactMATCH on its first /O2 /MT compile. Root PE4symbols/
5sections; native first run passes65,536cases, all256flag bytes times256poison
contexts, exact return and unchanged512B memory/control/status. No promotion.

`ime-coordinate-composition-first` preserves an access-violation run(exit
3221225477, no drift). The copied IME original and X-converter original both
occupied code+1024: fixture storage collision, not evidence of provider failure.
Fresh `ime-coordinate-storage-fix` moves IME to+2048; unchanged genuine provider
objects, one fixture compile, root PE25symbols/31sections. First native run
active(session82370: consult saved result or live process before any
rerun),540second budget. Original ranges are sprite[0,377),X[1024,1064),
Y[1280,1329),dimensions[1536,1560),IME[2048,2117) within4096B allocation.

## Corrected full chain passes; render-colour getter recovered

`ime-coordinate-storage-fix` passes4,423,680cases/71,663,616snapshots, exit0,
empty stderr, no drift. `copied_code_storage_audit.json` derives original code
ranges from fixture assignments and arrays, rejects the preserved first overlap,
and accepts the corrected layouts. `ime-coordinate-center-first` also passes
4,423,680cases/71,663,616snapshots with genuine original/current centering getter
called through the mutation-observer wrapper. Root PE26symbols/32sections;
no provider recompiles. All flag-byte values occur across the existing matrix.

`ui/0052e960/20261015/native-first` reconstructs CComponent::GetRenderColour:
retail vtable slot27 ->0052E960,7B returns this+0x94; donor identifies RenderColour
as CRGBColour and the const-reference signature. A staged field/offset assertion
replaces only opaque bytes. First /O2 /MT compile is exact7B/2instructionMATCH;
root PE4symbols/5sections. Native first run passes65,536 alpha/poison contexts,
checking exact reference identity, alpha and unchanged512B memory/FP. Partial
borrowed layout, no full construction or independent closure.

`ime-coordinate-colour-first` includes both getters, reusing all nine genuine
providers without recompiling them. Colour callback delegates to original/current
GetRenderColour on one of two borrowed components, retaining edit-pointer mutation
and alternate-colour behavior. Root PE27symbols/33sections; nine expected-symbol
omission links pass. Source-derived seven original ranges are nonoverlapping
(`copied_colour_code_storage_audit.json`). First native run active(session86960),
expected4,423,680cases/71,663,616snapshots,540second budget.

Next native boundary: Draw2DScaledGraphic009DD210. Full retail interval to next
entry009DD8F0 saved under `rendering/009dd210/20261015/evidence-first`; do not
mistake its first failure-path ret for the whole body. It uses CGraphicInfo::GetFrame
009FF8D0 and existing typed graphic-frame cleanup. Read-only, no-analysis Ghidra
export of both functions is active(session10834,300second budget). The local
Ghidra HTTP service was unavailable; no GUI or service was launched. No C++ draw
candidate or canonical mutation yet.

## Full IME chain pass; scaled-graphic draw reaches near

`ime-coordinate-colour-first` passes4,423,680cases/71,663,616snapshots, exit0,
empty stderr, no drift. This completes the first root behavior run joining IME,
cursor, X/Y conversions, dimension retrieval, centering and colour getters. Nine
provider omission checks pass. Callback adapters for getters still inject the
observed test mutations; system, manager, IME UI and draw APIs remain boundaries.

Draw2DScaledGraphic evidence export completed exit0 without timeout/drift. Ghidra
bytes equal retail exactly for860B/276instructions ending009DD56B, two returns;
the larger manifest interval includes other code and is not the function size.
The decompilation mislabels both scalar conversion helpers and009FC810. Actual
calls are GFVXToAX/GFVYToAY;009FC810 is CGraphicFrameData::GetUVBox, supported by
FableWin's32B CUVBox (U[4],V[4]) and receiver/return ABI. Read-only donor exports
also establish GetFrame's const signature, Add2DTri's three const vertex refs,
and CGenericVertex's separate SetZ and SetRHWFromZ methods. Constants read from
retail are ±0.01f,0,1,mutable reciprocal clamp100,and three four-index UV tables.

`rendering/009dd210/20261015/cpp-first` preserves a missing geometry include;
`cpp-geometry-include-fix` preserves a staged GetUVBox declaration hidden by the
canonical graphic-frame include. `cpp-frame-include-fix` stages the unchanged
frame header too, resolving that include chain, and builds742B/238instructions,
85.214% similarity, below near. `cpp-vertex-methods` restores donor-backed SetZ
and SetRHWFromZ as inline template methods instead of a local scalar helper:
824B/265instructions versus860B/276,92.052% normalized,3.986% instruction delta,
nearPASS. Object SHA256
`9812c3dc6027b1f8eae98735431e8b27d185591399da37604e90e3e284bf8ed9`.
No fabricated constants, asm or canonical edits. Stop byte tuning after behavior.

Native preparation `native-first` stopped before compilation because an assumed
29relocations was wrong: retail has28 typed operands (candidate29). Fresh
`native-relocation-count-fix` preserves a fixture compile error: global Frames
counter collided with CGraphicInfo::Frames. `native-frame-counter-fix` renames
only that counter and builds successfully. Root PE26symbols/30sections verifies
full original860B and selected candidate. First native run active(session60664),
expected1,179,648cases/20,545,536snapshots,12x87 modes,540second budget. Covers
all64 low flag combinations,16 float patterns, success/failure/null or live frame,
reference counts, texture info ownership, and callback mutations. Coordinates,
GetFrame,GetUVBox,texture dimensions,triangle submission,Uninitialise and delete
are controlled boundaries. Snapshots compare full16KB arena and all defined
vertex fields; retail leaves SpecularColour unwritten here, so that field is
explicitly excluded. No pixels, full graphics closure or promotion claimed.

## Draw behavior pass and UV recovery

`native-frame-counter-fix` passes1,179,648cases/20,545,536snapshots, exit0, empty
stderr, no drift. Draw candidate near+behavior achieved; no more byte tuning.
`rendering/009fc810/20261015/cpp-first` reconstructs CGraphicFrameData::GetUVBox:
full191B/54instruction retail body, initial TLX/TLY reads before the dimension
callback and live Width/Height/TLX/TLY reads afterward. Retail0x38000100 scale
matches binary32(1/32767). Donor CUVBox arrays and const by-value return determine
ABI. First /O2 /MT candidate187B/52instructions,94.340% near, SHA256
`dc49389481883d434484f097fe2da927ba1e8a78d56ca028dc8a681b02de17b5`.

`native-uv-composition-first` reuses draw and UV objects, compiling only fixture.
Original arrays at[0,860)and[1024,1215). Root PE26symbols/32sections. It passes
1,179,648cases/20,545,536snapshots with ushort UV offsets/extents including32767,
32768,65535; dimension callback changes frame UV fields and texture identity.
GetUVBox near+behavior achieved. The old UV callback is removed; dimensions,
triangle submission, GetFrame, coordinates and frame cleanup still controlled.

`native-texture-dimensions-first` adds genuine CTexture::GetDimensions, first
compile43B exactMATCH. Its complete original at[1536,1579) calls fake COM slot17
GetLevelDesc. Callbacks fill the descriptor on success and failure HRESULT paths;
this does not claim behavior for unwritten descriptors on failure. PE review
first rejected a faulty assertion that UV had two provider objects; preserved
`inspect_pe.py`, corrected `inspect_pe_corrected.py`, no relink or provider
changes. Root PE27symbols/33sections then passed. Native first run active
(session65169),1,179,648cases/20,545,536snapshots expected. Consult saved result.

## Complete draw helper chain and exact Add2DTri recovery

`native-texture-dimensions-first` and `native-coordinate-chain-first` each pass
1,179,648cases/20,545,536snapshots, exit0, empty stderr, no drift. Coordinates
GFVXToAX/GFVYToAY are91.667% near; display dimensions24B exact. The latter run
uses genuine conversion/dimension/global providers with system callbacks changing
input rectangles, display sizes and virtual scales.

`native-frame-cleanup-first` adds actual CGraphicFrameData::Uninitialise69B exact,
with only its undefined delete binding adapted in isolated COFF (all raw sections/
relocs unchanged). Original at[3328,3397), disjoint from existing code arrays.
Tests now include UsesSurface and non-surface flags. Pass1,179,648cases/
20,054,016snapshots; no drift or stderr. Seven selected-provider omission links
fail with the required unresolved symbol; unadapted diagnostic objects are
explicitly excluded from those links. All helper code remains work-only.

`rendering/009dc030/20261015/evidence-first` has read-only Ghidra export, verified
648B Add2DTri and donor private signature. Native GetCurrentAWindow00A0AAA0 is
mislabelled DoSizeof in decompiler output; canonical getter evidence is used.
`cpp-first` reconstructs three explicit vertex copies/normalizations/pushes,
then the texture/window/blend metadata push. First /O2 /MT compile is exact648B/
195instructions after22typed relocations, SHA256
`73cd89f46bda4afcf41266f9b871039870b7f932ef99ab7003d9b3c9109cbd8e`.
Staged declaration correctly uses private Add2DTri (AAEX). Earlier draw work used
public QAEX for that controlled boundary; before composing callers, correct the
staged access declaration and prove unchanged bodies, rather than silently
claiming the old external spelling is donor-canonical.

`native-first` preserves fixture compile errors (overloaded overflow method
addresses and nested-loop field scope); no native invocation. The PE inspector
was inadvertently attempted after that build failure and found no build result.
Fresh `native-signature-fix` uses explicit member-pointer types and local loop
variable; provider unchanged. Root PE24symbols/27sections. Pass49,152cases/
724,992full64KB snapshots,12x87 modes: offscreen, all capacity0..3(vertex)and0..1
(info), texture presence,16float patterns,8callback mutation sets. Window,
overflow,string/texture lifetime and quick-draw copy remain controlled. The copy
boundary copies only caller-defined info fields; unused union/colour/font/padding
are not asserted. Its C++ member default construction is observed consistently
on both sides; this is not the actual copy-constructor closure.

`native-window-chain-first` adds genuine12B exact GetCurrentAWindow and removes
that callback. Overflow can switch the display pointer before subsequent vertex
captures; texture/copy boundaries mutate window fields before/after metadata
capture. Pass49,152cases/380,928snapshots, empty stderr, no drift. Root PE24symbols/
27sections. Next useful boundary is genuine texture retain/release and eventually
real quick-draw copy/vector overflow. No rendering, canonical promotion or
independent closure is claimed by these root diagnostics.

## Triangle queue retains real texture and string ownership

`native-texture-chain-first` preserves a fixture-only missing ULONG typedef
compile error; `native-com-return-type-fix` uses unsigned long, same x86 COM ABI.
It passes49,152cases/356,352snapshots with native CTexture assignment98B exact
and Uninitialise34B exact. COM callbacks remain controlled; AddRef mutates the
source texture after capture and changes the window before metadata capture.

`native-info-copy-first` adds actual CQuickDrawTriInfo copy164B exact and CTexture
copy68B exact, with controlled string copying. Pass49,152cases/380,928snapshots.
A queued texture retains its own reference after temporary-info release. The
fixture masks only destination info bytes36..59 copied from caller-uninitialized
union/colour/font fields; all initialized fields, input vertices, other64KB state
and FP are compared. Destination info remains live; no queue teardown claim.

`native-string-chain-first` adds actual CWideString constructor15B,copy66B,
destructor68B and instance-count data. All three bodies exact. First parity
check used public UnassignString spelling and was rejected; corrected to private
AAEX in the parity-only target map, without build or run changes. Original
strings at[2816,2831),[3072,3138),[3328,3396), all inside4096B code allocation.
Only destructor undefined free/delete symbols are adapted to fail-fast fixture
bindings; raw COFF sections/relocations unchanged. Unassign/free/delete are not
entered because this caller's strings stay null. Instance counts cover wrap
boundaries and retain one live queued string. Root PE28symbols/32sections;
pass49,152cases/233,472snapshots, exit0, empty stderr, no drift. The info mask is
applied at post-construction observations (second texture AddRef or final).
Vector-overflow allocation/copy remains controlled, as do COM AddRef/Release.
Next investigation: missing vertex-vector growth009E14B0 and existing info-vector
growth009E1750; do not alter the other workflow's vector-clear work.

## Genuine vertex growth, standalone and queued

`rendering/009e14b0/20261015/cpp-first` applies the existing generic STLport
non-POD insertion algorithm to32B CTVertexRHWColSpecTex1 elements. Retail294B/
111instructions and malloc/free calls are verified against a read-only export;
first compile is relocation-exact, SHA256
`5cc2355611f36fd24aa582e30188584b83ad0b4e2170dea3e9ce4c59c477b19a`.
Standalone `native-first` passes44,160cases/262,272full32KB snapshots: old sizes
0..8, every insertion position, fill0..4, suffix flag, external/aliased values,
empty null/non-null storage, allocator mutation and old-buffer poisoning. FP
unchanged. Successful bounded allocation only. Root PE11symbols/13sections;
only undefined malloc/free names adapted, raw sections/relocations unchanged.

`009dc030/20261015/native-vertex-growth-first` reuses that provider plus all prior
queue-chain providers; only fixture compiled. Original growth[3584,3878) fits
inside4096B allocation, disjoint from other original bodies. Three successive
native growth calls can occur for an initially empty queue; bounded heap slots
are tracked and freed buffers poisoned. Pass49,152cases/331,776snapshots, exit0,
empty stderr, no drift. Root PE30symbols/34sections. Info-vector overflow remains
controlled; other previously stated info-tail masking/borrowed lifetime limits
remain.

`rendering/009e1750/20261015/cpp-first` independently compiles the existing
canonical info-vector growth source without modifying it. Exact243B/94instructions,
SHA256 `0799d5792685109d557623571c6cc8d493fe23d47deaa1aa4990ab24eca54dbd`.
Its emitted suffix-copy helper targets009E11A0 and its clear helper009E1650;
Ghidra's combat-wheel/ResetRings labels are incorrect here. No canonical clear
source edits. Next work is bounded standalone growth with genuine element
copy/destruction and ownership accounting, before composing this final vector
boundary into Add2DTri.

### Genuine info growth and complete triangle-queue storage

`rendering/009e1750/20261015/native-zero-fixups` passes24,576cases/
287,232 full32KB state snapshots, unchanged FP, exit0, empty stderr, no drift.
Growth243B, range-copy46B (009E11A0), clear58B (009E1650) all MATCH.
Real quick-draw/texture/string copy and cleanup, null borrowed strings,
old sizes0..4, every insertion position, fill0..3, suffix handling, aliased
values, owned COM counts, and allocator/AddRef mutations. Successful bounded
allocation only. First build stopped on zero-length fixup arrays for two
relocation-free bodies; preserved; fresh stage uses zero install count.
Root PE27symbols/30sections. No independent closure or canonical promotion.

`009dc030/20261015/native-info-growth-first` composes both actual vector growth
routines with Add2DTri and prior window/copy/lifetime providers. Pass49,152cases/
331,776snapshots, 12x87 modes, exit0, empty stderr, no drift. Only fixture
compiled. Info capacity0 starts null; bounded60B allocator, free traps on
this empty insertion path (nonempty cleanup covered standalone). Copied retail
code expanded to8192B; explicit13-range nonoverlap audit saved. The first PE
review could not save because preparation accidentally copied prior review;
that artifact remains preserved. `inspect_pe_current.py` verifies the current
executable33symbols/37sections; `run_native_current.py` pins that current
review. No native run used the stale review. Queued info tail36..59 still
masked because Add2DTri leaves those stack fields uninitialized.

`009dd210/20261015/cpp-private-triangle` corrects only staged Add2DTri access
from public to donor-evidenced private before caller/callee composition.
Recompiled Draw stays824B/265instructions/92.052%near. Code/data and relocation
tables unchanged; sole functional symbol change QAEX->AAEX. CodeView and .file
build paths differ as expected (first overly broad all-section audit rejected
those differences; corrected audit explicitly records metadata changes).
Object SHA d63e0b78ec23101577f731bcd1ca7fa45774ef1713c2713423f5efcb47c4fec8.
Next: compose scaled draw with this complete triangle queue. All work remains
native C++, work-only, with controlled external callbacks and no GPU launch.

### Scaled draw reaches the genuine triangle queue

`rendering/009dd210/20261015/native-queue-chain-first` passes1,179,648cases/
28,213,248 snapshots, 12x87 modes, full64KB arena with explicit exclusions,
exit0, empty stderr, no drift. Root PE51symbols/60sections,20 selected-provider
objects including fixture. Draw uses the private Add2DTri symbol and actual
window, both vector growth routines, copy/texture/string lifetime chain, UV,
coordinate, texture dimensions and frame cleanup helpers. First triangle can
trigger growth repeatedly; second can copy and destroy previously queued info.
COM references/string count and final six vertices/two infos checked separately.
Texture AddRef can change the frame's texture between triangle submissions.
Original queue code occupies disjoint ranges4096..8762 inside12288 allocation;
previous draw bodies end3397. GetFrame/system/COM/allocators remain controlled.
Reserved vertex SpecularColour and info tail36..59 are excluded from snapshots:
retail sources are unwritten; all other bytes and FP are compared. No queue
teardown, actual rendering, allocation-failure, independent review or promotion.

The build harness successfully compiled/linked once, then erroneously tried an
obsolete uninitialise-adaptation block. `finalize_existing_link.py` validates
saved successful compile/link commands, include/provider hashes, and zero drift;
no recompile/relink. Link receipt is `build-FIRST/uninitialise.process.json`.
Only after recovery receipt and full PE check was native execution started.

### GetFrame recovery candidate and bounded behavior matrix

`rendering/009ff8d0/20261015/evidence-first` verifies578 retail bytes from the
existing read-only Ghidra export and captures donor private forced-load ABI.
`cpp-first`667B/248instructions/82.018% rejected near threshold. A shared loaded
assignment tail matches the retail control structure; `cpp-shared-loaded-tail`
is561B/206instructions/90.338%near, object SHA
8fb6da9fd9bfaa01523c9edd28604f9bb1fed7af164244a085f621b019e1b943.
No further byte tuning pending behavior. `native-first` built and PE22symbols/
24sections verified.491,520-case matrix started (session68227); consult saved
native-FIRST result before continuation. Controlled constructors avoid claiming
full loader construction; full32KB/FP comparison and old-output cleanup,
missing/existing frames, signed mip levels, options, list touches, forced load,
and allocator/list/loading/cleanup pointer mutations. No allocation failures.

GetFrame `native-first` finished: PASS491,520cases/3,129,600 snapshots,
exit0, empty stderr, no drift. First near candidate retained; no further tuning.
Scaled draw queue omission links:19/19 providers verified from saved link
receipts. Initial assertion selected info growth's internal range-copy helper;
without the whole growth object, linker correctly reports the external growth
entry. `review_saved_omissions.py` checks the correct name in existing output;
no relink, all other18 assertions unchanged. `omission-FIRST/corrected_review.json`
is current; original failed assertion report remains preserved.

### Frame acquisition reaches draw; real resource touches and forced loading

`009dd210/20261015/native-frame-helper-name` passes884,736cases/
33,030,144 snapshots,12x87 modes, exit0, empty stderr, no drift. Genuine GetFrame
now feeds scaled draw/full triangle queue. Valid graphic indices only; missing
and existing unloaded data, forced-load failure/success and loaded cache paths.
GetFrame raises the returned frame reference before draw cleanup. Root
PE58symbols/67sections. First stage fixture failed compilation because member
CFrameData::Frame shadowed helper Frame(); fresh stage qualifies ::Frame().
Loader/constructor/resource-list callbacks still controlled here. No GPU launch.

`009ff8d0/20261015/native-resource-chain-first` passes491,520cases/
2,818,304 snapshots with genuine ListRemove47B/15instructions and ListInsert26B/
8instructions, both MATCH. Full32KB/FP plus circular ring invariant. Existing
borrowed resource nodes and controlled pointee deletion; no complete resource
lifetime claim. Root PE22symbols/24sections, two helper recompiles.

Read-only `009ff6b0/20261015/evidence-first` identifies forced-loading wrapper82B.
009FC440 is CResource::RemoveFromResourceList (Ghidra OnKill label wrong).
`cpp-first` failed on duplicate canonical/staged resource declarations;
`cpp-header-closure` stages its actual include closure consistently.82B/
29instructions/100% normalized similarity, but relocation-aware grade NEAR_MATCH,
not byte exact. No tuning needed. SHA
8cc8e3f45195df3a79b97faf9606d879cdc61fa9068f9a11768641eacd8da901.

Forced `native-first` exited2 before comparison: multiple-inheritance bank-file
member pointers exceed4B. `native-file-adapters` uses explicit ABI-compatible
fastcall wrappers; then original-phase assertion exposed fixture Finished name
shadowing CBankFileAsyncData::Finished. Preserved both failures. That intermediate
stage also retained a copied old PE review; current reviewer/runner used new
review filename, never executed against stale review. `native-finished-name`
qualifies ::Finished and passes524,288cases/2,621,440 snapshots, full8KB/FP.
Actual RemoveFromResourceList16B and SetAsUnloadable17B helpers MATCH.
`native-resource-remove-first` adds actual RemoveResource80B/26instructions MATCH,
recovered from canonical method without its unrelated loading-queue function.
Pass524,288cases/2,359,296 snapshots, resource counts/allocated memory and ring
links independently checked. Root PE14symbols/17sections. File finished/wait
and upload callbacks remain controlled; no actual disk wait/upload. All statuses
are root work-only diagnostics pending independent review and promotion.

### Full forced/resource draw chain and real lock-scoped file checks

`009dd210/20261015/native-forced-header-closure` passes884,736cases/
32,636,928 snapshots,12x87 modes, exit0, empty stderr, no drift. RootPE66symbols/
75sections. Genuine GetFrame/forced-load/list touches/resource removal, scaled
draw/full queue and prior helpers. Circular resource links/count/memory checked.
Constructor/BeginLoading/file completion/wait/upload, system and COM/allocators
still controlled. First `native-forced-resource-first` compilation stopped on
mixed canonical/staged include identities; consistent actual include closure
fixed the fixture in a fresh stage. No provider recompiles. Runtime allowance
900s for expanded matrix, based on prior ~8-minute composition; no timeout.

`009d54e0/20261015/cpp-first` rebuilds canonical bank-file accessors unchanged.
IsFinished65B/24instructions MATCH; SHA
6c13f15b37f63cbfc5b1eeb64e2973782b7e1004f5b4a09b2bd43634848d5bf5.
`native-first` includes genuine critical-section ctor54B and dtor25B, both MATCH.
Only isolated COFF import names bound to controlled API pointer slots; code/data
and relocs unchanged. Hooks invoke real Win32 Enter/Leave on initialized locks.
Pass65,536cases/393,216 snapshots, full4KB/FP, all Finished bytes, restart poisons,
field changes at acquisition/release, and bank-pointer changes while captured
lock remains live. RootPE14symbols/14sections; single-thread only, no scheduler
claim. Both lock providers compiled, no canonical edits.

Read-only `009d5b60/20261015/evidence-first` verifies WaitUntilFinished130B.
Corrected Ghidra labels:009D5A90 RestartAbortedReads,00A62960 lock Release,
0098F370 ThreadedFile::WaitForObjectToFinish. `cpp-first` failed on counted-pointer
specialization include order; `cpp-threaded-include-first` includes threaded-file
before bank definitions and matches130B/43instructions (see native parity).
SHA74208d0c81884f2cdb838ec67ecc4675601e59440c0f6c097a2006d5a9ab6f7e.
`native-first` passes16,384cases/106,496snapshots, rootPE21symbols/21sections.
Real critical ctor/dtor reused, actual Release25B compiled; full4KB/FP and held
lock state checked independently. All setup/restart callbacks run under lock;
threaded-file wait runs after Release. Bank and UID mutations checked. These
three callbacks are controlled; no actual async wait/thread scheduling claim.

### File completion/wait composition running; buffer setup recovered

`009dd210/20261015/native-file-wait-event-type` built and root PE79symbols/
85sections verified,31 provider objects including fixture. Matrix session10164
was started with900s allowance; inspect native-FIRST/result.json before resuming.
Actual IsFinished/WaitUntilFinished/lock ctor/dtor/release now compose with prior
full draw chain. Real initialized Win32 critical section at arena+B14C; callbacks
check held state, release before threaded wait, SetupBuffer/restart counts and
AllowRestart. Failed SetupBuffer may make WaitUntilFinished false; forced loader
ignores that result and still uploads, matching retail call flow. Still controlled
constructor/BeginLoading/SetupBuffer/restart/threaded wait/upload/system/COM and
bounded allocators. First stage failed only event UID ULONG->unsigned pointer;
fresh stage uses explicit local argument.32 copied original ranges audited
nonoverlapping within16384B. Existing uninitialized field masks unchanged.

Read-only `009d52d0/20261015/evidence-first` exports SetupBuffer310B and inner
BeginLoading1265B. SetupBuffer selected first as bounded allocation decision.
Donor method ABI receipts saved for memory allocator and handle/movable pools.
`cpp-first`310B/115instructions/100% normalized; native relocation grade pending.
SHA f6d94a4475682520ed5c07f351e91bfec735a158d05b544e856bca23ebcb38dc.
Typed staged declarations for AllocateResource/GetPointerFromHandle and inline
CMemoryAllocator::Allocate(unsigned long,unsigned long); no canonical edits.
Next behavior work covers try-lock, existing buffer, pool success/failure,
blocking heap fallback, null results and callback mutations.

File-wait draw `native-file-wait-event-type` finished PASS884,736cases/
35,389,440snapshots, exit0, empty stderr, no drift. Session10164 completed;
no active process remains for it. IsFinished and WaitUntilFinished plus all lock
lifetime routines are genuine; thread scheduling/wait itself remains controlled.

SetupBuffer `009d52d0/20261015/native-first` relocation-aware MATCH310B,
pass65,536cases/569,344snapshots, full4KB/FP, real Win32 blocking/try locks,
controlled allocation callbacks, separate branch/count/pointer checks. Actual
force1 pool failure falls back to heap; null heap result still returns true and
Data receives the skip-adjusted value, matching retail. RootPE26symbols/24sections.
`native-lookup-chain-first` adds typed CBankFile::Align24B and
CHandleBasedMemoryPool::GetPointerFromHandle19B, both independently checked
against retail via lookup_parity.json. Pass65,536cases/499,712snapshots, full8KB,
real lock lifetime, valid table indices0..7, null header entry returns0x20,
non-power-of-two alignments and unsigned wrap. Bank/pool callbacks can change
subsequent captured inputs; no memory payload dereferenced. RootPE26symbols/
24sections. Only lookup helpers compiled; core SetupBuffer and locks reused.
Next useful composition: real SetupBuffer under WaitUntilFinished's outer lock,
checking reentrant lock depth and release-before-threaded-wait ordering.

### Native wait/setup and restart composition

`rendering/009d52d0/20261015/native-wait-composition-first` passed
65,536 cases / 575,488 snapshots with actual WaitUntilFinished, SetupBuffer,
Align, handle lookup and lock lifetime. Nested real Win32 critical sections
reach depth two; the threaded-file wait callback observes depth zero.

`rendering/009dd210/20261015/native-buffer-setup-first` passed
884,736 cases / 36,790,272 snapshots with actual SetupBuffer in the full
scaled-draw chain. Root PE review identifies 87 symbols / 91 sections across
34 objects. Pool and heap allocation backends remain controlled.

`rendering/009d5a90/20261015/native-first` passed 27,648 cases / 608,256
snapshots for RestartAbortedReads and recursive tree erase. Empty through
eight-node ordered acyclic trees cover balanced, left-chain and right-chain
shapes; checks include in-order AddObject callbacks, captured UID destinations,
unique node frees, cleared header/count, full 16KB state and FP state.
This does not validate tree insertion/balancing or threaded-file scheduling.

Full-draw restart composition is running in
`rendering/009dd210/20261015/native-restart-live-thread`. The preceding
`native-restart-chain-first` built and passed PE review but was not executed:
a pre-run fixture review caught its stale fixed threaded-file pointer check.
The fresh stage checks the live bank pointer after AddObject mutations.
Both use the existing native restart and erase providers; no provider retuning.
All results remain root diagnostics pending independent review, with no
canonical coverage promotion or whole-game/GPU claim.

### Restart integrated; UploadFrame native behavior passed

`rendering/009dd210/20261015/native-restart-live-thread` passed
884,736 cases / 38,461,440 snapshots. Root PE review: 89 mapped code symbols,
93 sections, 36 objects. Actual restart traversal and recursive erase now run
under the real wait lock in the full draw chain, with empty through four-node
right-chain restart queues, independent UID/header/count/unique-free checks,
and live threaded-file changes. AddObject and disk waiting remain controlled.

`rendering/009ff220/20261015/cpp-first` compiles the existing UploadFrame C++
to 533 bytes / 188 instructions versus retail 540 / 188, 91.489% normalized
similarity (near); SHA256 2ab2b4461f67b0541490abe5560c7523c959a67a2c15c1ec8a7193ce45785b5c.
The copied read-only export body was verified byte-for-byte against retail.
`native-first` passed 55,296 cases / 910,080 snapshots, full 16KB/FP, volume
and ordinary paths, failed/successful loads, flat animation, null/one/two refs,
file-data cleanup, owner/mipmap callback changes and wrapping size arithmetic.
Root PE review: 24 symbols / 26 sections; isolated undefined delete binding
preserves all object sections/relocations and all other symbols. Loading,
managed texture operations, frame cleanup, sizes and resource callbacks remain
controlled here. Preparing actual managed-texture and accounting helpers next.
No canonical promotion, independent acceptance, real upload, or GPU claim.

### Native upload helpers composed

UploadFrame stages under `rendering/009ff220/20261015` passed in sequence:
`native-managed-chain-first`: 55,296 cases / 863,424 snapshots;
`native-accounting-chain-first`: 55,296 / 642,240;
`native-size-chain-first`: 55,296 / 533,952;
`native-frame-cleanup-first`: 55,296 / 524,736.
The last stage uses actual frame cleanup, managed assignment/release, frame
and managed sizes, resource sizing and bank-list insertion. Root PE identifies
26 symbols / 28 sections from nine objects. Controlled boundaries are the
loaders, payload ReleaseTexture/delete callbacks and final delete hooks.

Release compiles 68B/28 instructions versus retail73B/30, 93.103% normalized
near; assignment72B/28 instructions is 100% normalized. Resource SetSize46B,
AddResource52B, frame Sizeof42B and managed Sizeof51B are byte-identical:
`helper_byte_review.json` records actual raw-byte comparisons. Initial size
helpers used a conditional expression and failed similarity (62.1%/74.3%);
`sizes-branch` preserves retail branch structure and passes, with no tuning
after acceptance. Successful stages retain genuine VC7.1 objects and typed
layouts; no opaque byte-array implementation.

Full scaled-draw upload composition is running in
`rendering/009dd210/20261015/native-upload-chain-first`, adding seven reused
providers. Root PE: 103 symbols / 107 sections, 43 objects. Original code
allocation expanded to32KB; `audit_upload_code_ranges.py` independently checks
the copied body intervals. Graphic loading backends remain controlled.
All results are root diagnostics pending independent review.

### Frame-data constructor baseline

`rendering/009fe380/20261015/cpp-first` meets near thresholds:175B/58
instructions versus retail181B/59, 90.598% normalized similarity, object SHA
63525f8e930ecdabc3e3b38898b25af7966781c9fadef3ee711e943176b9474d.
Read-only export verified against retail. `native-first` passed16,384 cases /
204,800 snapshots, full8KB/FP. It verifies exact linked frame/managed vtables,
base constructor callback order, owner changes, injected frame-data presence,
untouched LoadingMipmapLevel/padding and wrapping size/frame arithmetic.
Fixture-only undefined vtable aliases preserve all raw sections/relocations;
PE review checks49 symbols/46sections. Base constructors, size/accounting
callbacks are controlled; virtual teardown traps are never executed. No
whole-lifetime claim. Full draw/upload composition remains running separately.

### Constructor base and resource composition

Actual CResource33B and CBaseClass9B constructors match retail with typed
vtable relocations (`009fe380/20261015/base_typed_parity.json`). The generic
similarity helper lacked those retail data relocations and initially reported
90%/66.7%; explicit vtable target binding proves both MATCH without retuning.
`native-base-chain-first` passed1,024 cases /6,144 snapshots.
`native-resource-chain-first` then passed1,024 /2,048 with genuine base
constructors, size helpers, SetResourceSize and AddResource. All256 poison
bytes, two banks, empty/nonempty lists; independent circular links/counts/
accounting, exact linked vtables and untouched loading-mip/padding checks.
Root PE46 symbols/43sections, eight objects. Constructors are genuine; virtual
teardown remains trapped/unexecuted and complete object lifetime is not claimed.

### Full draw/upload accepted diagnostically; BeginLoading started

`009dd210/20261015/native-upload-chain-first` passed884,736 cases /
40,034,304 snapshots. All44 copied original-body ranges are disjoint inside32KB.
`native-constructor-chain-first` is now running with actual frame/base
constructors through the existing ABI adapter; root PE139 symbols/138sections,
46 objects,47 disjoint original bodies ending at19409. Virtual teardown is
trapped/unexecuted; no lifetime closure claim.

New `009fde80/20261015` stages recover BeginLoading1265B/438 instructions.
`cpp-first` failed due staged/canonical header identity conflicts; fresh
`cpp-header-closure` resolves the quoted include closure but only88.159%
normalized similarity. `cpp-inline-file-release` explicitly inlines the
file counted-pointer release as retail does:1224B/427 instructions,
92.486% near, SHA6480951791dfe6652954107e9ce6e322c2c2e4bf5f24c1a0867e999deacfa5ec.
No behavior acceptance yet. Important corrected Ghidra identities: final
9FC3D0 is AddToLoadingQueue,9D58D0 is GetAsyncEntry;9D6100 has a hidden
counted-pointer return and explicit priority from TextureLoadingPriority
01396C64. Next work is bounded behavioral validation before any integration.

### Full constructor integration and BeginLoading admission passed

`009dd210/20261015/native-constructor-chain-first` passed884,736 cases /
40,034,304 snapshots; no input drift. Constructor now runs beneath actual
GetFrame through the existing ABI adapter. All47 original bodies are bounded
and disjoint. BeginLoading and actual texture/disk backends remain controlled.

`009fde80/20261015/native-admission-first` passed12,288 cases /134,464
snapshots with the near1224B BeginLoading provider, full16KB/FP, signed small
mipmap limits, unsigned request clamps, loaded/pending early returns, touch
metadata, allocation retry halving, volume format initialization, captured
header/manager versus live owner state, and unlocking live managed pointers.
All allocation callbacks fail in this stage. Successful allocation/abort/read/
loading-queue paths are deliberately trapped; next fixture must cover those
before integrating this provider. RootPE39symbols/39sections, two objects.

### BeginLoading successful-read behavior and managed lifetime composition

`009fde80/20261015/native-success-first` stopped on retail phase case4 due a
fixture assertion: freshly allocated synthetic texture data had a poisoned
Locked byte. The fresh `native-success-allocated-lock` initializes that byte
in the allocation callback and passes36,864 cases /854,144 snapshots.
Cases cover allocation fail/succeed/retry, existing file abort success/failure,
forced waits, returned counted-pointer ownership, captured/live owner changes,
frame-sliced and compressed-mip read offsets, priority, resource/queue calls.
Independent unsigned offset/size and refcount checks accompany full16KB/FP.
Native managed assignment/release/destructor composition in
`native-managed-chain-first` passes36,864 /793,728, rootPE48symbols/49sections.
CManagedTexture destructor61B/21instructions meets95.238% near; loading queue
insertion52B/17 meets100% normalized. Base destructor and payload callbacks
remain controlled in this standalone stage. Successful disk/texture backends
are simulated; no GPU/disk/whole-lifetime claim. Root diagnostics only.

### Full BeginLoading composition started

`009dd210/20261015/native-begin-loading-first` is running. The fixture now
uses actual BeginLoading, managed destructor, loading-queue insertion and base
destructor beneath GetFrame/draw. RootPE153symbols/152sections,50 objects;
51 disjoint original bodies inside32KB (last end21807). Full chain is scoped
to valid non-flat 2D graphics, with texture allocation and GetAsyncEntry/
BeginReadDataAutoAlloc backends controlled; standalone BeginLoading tests
cover volume, retries and abort cases. File/entry counted-pointer returns use
valid control blocks and independent final refcount assertions. Actual loading
queue emptiness, draw resources, buffers and triangles remain checked.
Base destructor7B and loading queue insertion52B have explicit typed MATCH
proof in `009fde80/20261015/base_dtor_queue_parity.json`. Generic relocation
metadata omitted base vtable relocation; no source tuning was needed.

### Full BeginLoading integration; async-entry/read wrappers

`009dd210/20261015/native-begin-loading-first` passed884,736 cases /
44,359,680 snapshots without drift. Actual BeginLoading, constructor, upload,
resource/loading queues, wait/setup/restart and triangle queuing are composed;
texture/disk allocation backends remain bounded. RootPE153symbols/152sections,
50 objects,51 audited original ranges. No GPU/disk or whole-game closure.

`009d58d0/20261015/cpp-first` GetAsyncEntry152B/58 instructions plus Align24B
have typed MATCH proofs. `native-first` passed6,144 cases/49,152 snapshots,
full8KB/FP, entry/control-block allocation null/success, index extremes,
callback changes to entry bank and alignment. Returned ownership detached;
virtual teardown untested. Actual entry vtable/deleter symbol identities
are bound with fixture-only COFF aliases, with full raw section/reloc proof.

`009d6100/20261015/cpp-first` BeginReadDataAutoAlloc235B/93 instructions
meets96.774% near. `native-first` passed131,072 cases/1,441,792 snapshots,
full8KB/FP, cached/noncached offsets, non-power-of-two alignment, unsigned
size/skip overflow, priority extremes, control-block allocation failure and
live bank changes across callbacks. Genuine Align reused; data construction
and SetupRead remain bounded. Null object allocation not exercised because
retail immediately passes a dereferenced null to SetupRead. No lifetime claim.
Next: trace SetupRead and file-data construction before further composition.

SetupRead 009d5970: cpp-direct-cache-branch189B/65 instructions meets91.603%
normalized near against191B/66 retail. Earlier expression-shape trials retained.
DIA confirms GetEntryDataOffset and protected ReadData signatures; staged-only
friend access. native-first passed524,288 cases/3,932,160 full8KB/FP snapshots,
including noncanonical bool bytes, callback bank/buffer mutations, non-power-of-two
alignment, and synchronous SetupBuffer(false return) still followed by ReadData.
RootPE14symbols/17sections; bounded SetupBuffer/offset/read/queue callbacks.

009d6100/native-setup-chain-first compile failure preserved (fixture AddObject
parameter lacked const). Fresh native-setup-chain-const passed131,072 cases/
1,835,008 snapshots with genuine BeginReadDataAutoAlloc, SetupRead and Align.
RootPE20symbols/23sections, four objects; no input drift. Both sync and async
branches, control-block failure, retained references, live-bank changes and
unsigned offset/size extremes checked. Construction and disk/queue boundaries
remain controlled; returned ownership detached, no lifetime or hardware claim.
All results remain root diagnostics pending independent review; no promotion.
Next: file-data constructor and its registration boundary.

009d5df0 file-data constructor: cpp-first183B/53 instructions, typed MATCH
(all seven call/vtable targets), object15fe78a3209165742380fe4cca84f1f45406ae1dfc0314017a28e78848df9860.
native-first link failure retained (two base callback slots missing). Fresh
native-base-callbacks passed65,536 cases/655,360 full8KB/FP snapshots;
RootPE33symbols/33sections. Checks all256 poison bytes, length/skip extremes,
both actual MI vtables, descriptor defaults and untouched padding, live-bank
changes, and InternalAllocatedBufferSize cleared after registration.
native-real-locks also passed65,536/655,360 with real recovered lock helpers,
initialized Win32 critical sections and captured-lock release, RootPE37/34.
Base constructor and tree insertion remain controlled; no virtual teardown or
concurrency claim. Next trace registration9d7cf0 and base constructor99a2f0.

File-data base closure: NonCopy9B/3 instructions typed MATCH,
object475cf89457cbebd0c65295608a22429a5e3268e5208ef3e014dc1a45304bf001.
native-base-chain-first duplicate CBase constructor link failure preserved;
native-base-chain-provider passed65,536 cases/524,288 snapshots, RootPE36/33,
five objects. File-data constructor, NonCopy base, lock helpers now genuine;
registration remains bounded. Actual vtables and captured lock verified.

Registration9d7cf0 cpp-first180B/78 has normalized100%; typed main-body check
succeeds but bundled predecessor9d74e0 is78B/35 vs79B/39 retail,91.892%
opcode similarity and10.256% instruction delta, just outside threshold.
cpp-direct-iterator keeps main unchanged but predecessor36 instructions/88%
normalized also fails. Neither predecessor admitted or composed. Saved exports
include insertion9d7990(179B) and NonCopy99a2f0(9B); earlier Ghidra labels for
these generic tree methods are not trusted semantic identities.

009d6100/native-constructor-chain-first passed131,072 cases/2,621,440 snapshots,
RootPE52symbols/49sections/eight objects, full8KB/FP and no drift. Genuine
BeginReadDataAutoAlloc, Align, SetupRead, file-data constructor, NonCopy base
and lock helpers composed with real initialized Win32 locks. Registry insert,
SetupBuffer, entry-offset lookup, disk ReadData and queued AddObject remain
bounded. Control-block failure and detached returned ownership as before.
No virtual teardown, actual disk or concurrency claim. Next bank metadata helpers.

009cc270 metadata-build-protected: GetEntryDataOffset36B, GetAlignment7B,
ReadData40B typed MATCH. Earlier batch verifier stopped after compile because
ReadData's protected symbol was incorrectly searched as public; failed directory
preserved. native-first passed262,144 cases/1,835,008 full8KB/FP snapshots,
RootPE13/16. Runtime/packed offsets, unsigned extremes, and live disk object/
vtable changes between virtual SetPosition(slot5) and Read(slot3) checked.
Packed lookup and actual virtual disk implementations are controlled.
009d6100/native-metadata-chain-first composes these with the full read chain:
131,072 cases/2,490,368 snapshots PASS, RootPE54/51, eleven objects. Original
bodies at0/512/1024/2048/2304/2560/2816/3072/3104/3168 stay disjoint in4096B.
Registration, SetupBuffer, packed-array lookup, virtual disk methods and queued
AddObject remain bounded; no disk/hardware, lifetime or concurrency claim.
Next packed lookup a629f0; predecessor tree helper remains unadmitted as above.

00a629f0 Packed lookup101B/43 instructions typed MATCH, exact33-word mask
array; native-first passed819,200 cases/1,638,400 full4KB/FP snapshots with an
independent per-bit oracle, widths0..32, crossing words, bias overflow and
multiplication-wrapped indices with valid effective addresses. Zero-width null
backing pointer included. RootPE5/9. No invalid-width/out-of-bounds claim.
009d6100/native-packed-chain-first passed131,072/2,359,296, RootPE54/52,
12 objects. native-buffer-chain-first adds real SetupBuffer on pool success:
131,072/2,752,512 PASS RootPE62/58,13 objects. native-heap-fallback-first adds
pool failure/heap fallback/null allocation:131,072/2,949,120 PASS.
native-pointer-chain-first adds real handle lookup in expanded16KB arena:
131,072 cases/2,883,584 snapshots PASS, RootPE62/58,14 objects. All no drift.
Original pointer at3968+19 within4096B; buffer setup3584+310, packed3328+101.
Registration, pool/heap allocation, virtual disk methods and AddObject remain
controlled. Successful file-data object allocation only; detached ownership,
no teardown, actual disk, hardware or concurrency claim. Root review pending.

Entry lifetime009d58d0: typed MATCH DeleteData9d6a60(11B), scalar deleting
Entry destructor9d6fa0(36B), NonCopy destructor99a300(7B). The retail labels
for unrelated CEditTransaction/CLandscape/NHero types are folded/misleading;
entry vtable129b880 and its target bind the identities. native-lifetime-first
passed6,144 cases/61,440 snapshots RootPE24/25. Successful entry/control-block
released through the actual linked virtual destructor and owned deallocation
callbacks; memory poisoned after each free. Original DeleteData separately
invoked. Base construction and size query bounded. No cleanup claim for the
control-block-allocation-failure case, whose output remains detached.

009dd210/native-entry-chain-first built/rootPE161symbols159sections50objects;
52 disjoint copied retail bodies, last22168/32768. Actual GetAsyncEntry,
DeleteData/entry virtual destructor/NonCopy destructor integrated with full
loading and draw chain. Entry and info deallocation distinct from frame frees;
base constructor remains bounded. Align now supplied by exact GetAsyncEntry
provider; Base destructor supplied by exact NonCopy destructor TU. Native
comparison running; result not yet claimed.

Full draw entry chain completed: native-entry-chain-first passed884,736 cases/
50,257,920 snapshots,12 x87 modes, no drift. Six newly selected entry/base
bodies separately audited as typed MATCH in entry_provider_typed_parity.json.

Predecessor issue resolved: retail9d74e0 retains the iterator in EAX and is
consistent with prefix operator-- returning iterator&, rather than the void
base helper used by earlier contexts. Ordinary C++ prefix-operator body with
local-node traversal (no tuning attributes) yields exact79B/39 instructions;
main insert_unique180B remains typed MATCH. cpp-iterator-return object
bde71a0e835a0c13573ebf105a39c32d62216484190c4cf42e305bfd01963449.
DIA public-symbol queries returned no matching entries; this identity is from
retail/codegen evidence, not a claimed PDB confirmation. Earlier failed shapes
remain preserved. native-first passed69,120 cases/241,920 snapshots, including
returned iterator reference, valid predecessor/end cases, ordered BST shapes,
duplicates and bounded insertion decisions. RootPE10/12.
Node insertion9d7990(179B), Rebalance42971d(190B), rotations42951b/42955b(64B
each) all typed MATCH. Rebalance uses existing source's /Os pragma. Full
native-insertion-chain-first passed47,312 cases/2,313,808 snapshots RootPE18/20:
all permutations0..7 initial keys, duplicate reinsertion, new min/max, independent
parent/order/reachability/count/extrema/red-child/black-height/root invariants
at every step. Only malloc20 is bounded; successful allocations, no destruction
claim. Registration can now be composed using actual tree insertion.

009d6100/native-registration-chain-first passed131,072 cases/2,883,584 full16KB/FP
snapshots RootPE71/67,17 objects. Actual constructor registration now uses unique
search, prefix decrement, node insertion, rebalance and rotations with0..3 prior
valid registrations; independent red-black checks after construction. Node malloc
is bounded, plus pool/heap/disk/queue boundaries as above.19 disjoint original
bodies end5568 inside8192B code allocation. Next full draw async-read composition.

Full draw/read composition: native-read-chain-first compile failure preserved
(duplicate FixtureMalloc name, registry vs queue). Fresh native-read-chain-registry
isolates registry allocator through a proven raw-section-preserving COFF rename.
Build/rootPE190symbols187sections60objects PASS;65 disjoint original bodies end
24768/32768. Native comparison running since12:23 local, session34742, planned
884,736 cases/58,515,456 snapshots and1200s timeout; not yet claimed. Actual
read allocation/setup, file-data constructor/registration and bank metadata now
use graphic bank500 as owner; constructor locks are tracked separately. Initial
thread queue callback retains an explicit borrowed file owner and simulates
finished/buffer/restart state. Full fixture scope remains asynchronous reads.

009cc240 metadata-build-FIRST size37B/type38B typed MATCH. native-first composes
offset/size/type/packed lookup and passed1,081,344 cases/2,162,688 snapshots,
RootPE8/12. All mixed packed/runtime selectors, widths0..32, bias overflow,
independent bit oracle and runtime type bytes0..255. No disk callbacks.
009d58d0/native-complete-entry-first passed6,144 cases/45,056 snapshots,
RootPE21/25: actual NonCopy base construction, GetEntryDataSize and packed lookup
added to genuine entry creation/cleanup; only allocation/deallocation bounded.
Failure outputs still detached; no cleanup claim when control block is absent.
Next after full draw/read result: replace bounded NonCopy construction and bank
size/type getters there with these independently verified providers.

Full draw/read native-read-chain-registry completed PASS:884,736 cases,
58,515,456 snapshots,12 x87 modes,no input drift. Root diagnostic pending review.
Prepared native-bank-fields-first with actual NonCopy constructor and size/type
getters: build/rootPE187symbols185sections62objects PASS;68 original bodies,
end24998/32768. Native run started session67624 after prior run completed;
expected884,736 cases/51,437,568 snapshots,1200s timeout. Do not duplicate.
File-data destructor009d61f0 evidence exported read-only,348B verified against
retail. Staging typed cleanup including registration removal, threaded cancel
retry, pool release and allocator shutdown. Retail synchronizes before testing
the buffer pointer; existing inline deallocation helper has a different null
boundary, so this variant is isolated in scratch. First compile failure preserved.

009d61f0 destructor cpp-first failed incomplete pool type, preserved. Fresh
cpp-pool-closure compiles348B/102instr,25 typed relocations exact MATCH,
SHA217dbca175eb73b13604a3ab508ebe3cba2391e6db3f8b84560a02930de7575e.
native-first failed fixture's implicit CBase destructor trap. Corrected fresh
native-base-cleanup PASS131,072cases/1,899,008snapshots;rootPE46/45. Full8KB/FP,
bounded tree/cancel/lock/pool/allocator callbacks, retry0..3, bank/UID mutations,
allocator singleton/shutdown changes, aborted/pool/handle/buffer branches.
009d6350 Abort cpp-first81.67% rejected; cpp-shared-exit196B62instr7relocs MATCH,
SHAbd6f13d00669868e6b6b232e72e9b69d913e5808c16fe1d50178e0a33e1a14b2.
native-first PASS131,072cases/1,085,440snapshots;rootPE49/45. Full8KB/FP,
finished early return, restart membership/cancel success/failure, live owner
mutation, pool cleanup and prior Aborted states. All root diagnostics pending
independent review. Full draw bank-fields run session67624 still running12:43.

## Evening resume (17:50): bank-fields full chain result

native-bank-fields-first/native-FIRST completed PASS at 12:45: exit0, empty
stderr, 884,736 cases/51,437,568 snapshots, 12 x87 modes, no input drift.
Actual NonCopy constructor plus bank size/type getters now replace the earlier
bounded versions in the full draw/read chain. Root diagnostic pending
independent review, same as 009d61f0 base-cleanup and 009d6350 native-first.
Next: compose file-data destructor/Abort teardown into the full chain.

009d61f0 cleanup provider compile (from before the usage stop): cleanup-build-FIRST
COMPILE_PASS, KeyErase 83B near 96.97%/33 instr, Lock 25B 100%. New stage
native-provider-cleanup composes the destructor (original retail and compiled
candidate) with genuine CCriticalSectionLock ctor54B/dtor25B/Release25B/Lock25B
on real Win32 critical sections in both banks, plus the full seven-source key
erase closure (equal_range, distance, range erase, _M_erase, rebalance-for-erase,
rotations, extrema) on independently built fixture red-black trees: 32 subset
shapes per non-aborted case, key present/absent in both erased trees, bank pointer
moved during the first node free. CRT free renamed to checked poisoning FixtureFree.
build-FIRST failed (scratch STLport headers clash with canonical sources; preserved);
build-canonical-providers failed (opaque _RTL_CRITICAL_SECTION in fixture; preserved);
build-section-view BUILD_PASS, providers compiled against canonical rebuild/include
only, map placement verified per symbol. Native run (2,162,688 cases) started.
Independent review of bank-fields, base-cleanup and Abort native-first requested.
native-provider-cleanup/native-FIRST STOP preserved (exit31 after all2,162,688
cases passed): fixture coverage guard found the (first-absent, second-present)
key partition unreachable; shape multipliers correlated the key bits. Fixed by
deriving erased-tree key bits from independent shape bits (all8 tree0/1/3 key
combinations). build-shape-partitions BUILD_PASS; native-shape-partitions PASS:
2,162,688 cases/27,990,016 full8KB/FP snapshots/25,952,256 independent RB tree
checks, key partitions524,288 each, 43s. omissions-FIRST: dropping each of11
provider objects fails link LNK2019. Pending independent review. No coverage change
(all providers already landed); this upgrades destructor evidence from bounded lock
and erase callbacks to genuine lock and tree-erase composition.

## Evening resume (Claude): bank-fields result and four canonical promotions

Session resumed after the earlier run hit its usage limit. `native-bank-fields-first`
had completed: PASS 884,736 cases / 51,437,568 snapshots, 12 x87 modes, no input
drift (root diagnostic only, as before).

Promoted to canonical `rebuild/src/compiled/` (sources compile from canonical
`rebuild/include` to the same root bytes as the work objects):

- `009D61F0` `CBankFileAsyncData::~CBankFileAsyncData` 348B/102, 25 relocations, MATCH.
- `009D6350` `CBankFileAsyncData::Abort` 196B/62, 7 relocations, MATCH.
- `009CC270` `CBankFile::GetEntryDataOffset` 36B/12, 1 relocation (packed lookup A629F0), MATCH.
- `009CBF60` `CBankFile::ReadData` 40B/17, no relocations, MATCH. Manifest label
  `CMapsAndRegionsDialog::OpenMapDialog` is a fold: PDB mangling
  `?ReadData@CBankFile@@IBEXKPAXK@Z` exists and the sole retail caller is 0x9D59AD
  (SetupRead). Corrected via `function_overrides.tsv`.

Canonical header changes are additive only: `FableBankOwnedSet::erase(const K&)`;
`CBankFile::GetEntryDataOffset` (public) and `ReadData` (protected const). Test-only
friends from the work headers were not migrated.

New shared helper `tools/decomp_pipeline/native_exact_roots.py`: fresh compile, every
relocation resolved through an independently pinned symbol→address table and
checked against the retail displacement, patched root == retail, input hashes stable.
Gates `gate_native_bank_async_data_teardown.py` and `gate_native_bank_entry_metadata.py`
use it, are registered in `gate_all.py` and pass. Regression gates (data constructor,
key erase, auto-alloc 110,772/544,254, registration 480/1136/20) still pass after the
header edits.

Not promoted:
- `009D74E0` prefix `operator--`: exact only with an out-of-line-bodied iterator
  `operator--` in `fable_stlport_tree.h`. That changes a shared template used by already
  landed tree code; it needs a full re-verify of tree dependents first.
- `0099A300` is already in the canonical base-class TU (`CBaseClass_CBaseClass_0099a2d0.cpp`).
- `009D6FA0` Entry scalar deleting destructor is a compiler-emitted COMDAT of landed
  `GetAsyncEntry` TU; no separate source.
- `009CBB80` GetAlignment is already landed under folded name `global_DrawGetEnvironment`.

Allocator stage native-allocator-cleanup (derived by prepare_fixture.py/
prepare_scripts.py from the provider stage; exact-edit scripts kept). Adds genuine
out-of-line allocator support (operator delete15B, Enter/LeaveSynchronisation12B
each) on a real fixture-owned critical section (COFF rename of
g_CMemoryAllocatorCriticalSection_013CAA70) with VirtualFree import bound to a
checked non-releasing callback, plus genuine GetNoAllocations7B reading real
NoAllocations(+40C) of two full0x49C allocator objects in a16KB arena. Bounded
InternalDeallocate now decrements the real counter (Mutation16 adds0x10000).
Mutation4 (inside former bounded Enter) is no longer expressible and was dropped.
build-FIRST stop: Enter/Leave near score0.667, expected because retail has no
.reloc table so push/IAT immediates cannot be masked. build-reloc-grade stop
(base-relocation rule, same cause). build-resolved-targets BUILD_PASS: dir32
fixups resolved to the targets proven by gate_allocator_out_of_line_support.py
(0x13CAA70/0x143FE0C/0x143FE10/0x143FD40), byte-equal retail. Native PASS:
1,081,344 cases/11,499,520 full16KB/FP snapshots/12,976,128 tree checks, key
partitions262,144 each, heap frees102,400, allocator deletes38,400, retained
after shutdown38,400. omissions-FIRST: all13 provider objects required.
Remaining destructor boundaries: RemoveObject, Sleep, handle-pool Free,
InternalDeallocate, allocator destructor, NonCopy base destructor.
Finding (pool-free call identity): retail 0x9d61f0 calls 0xA62ED0, the 5B
`jmp 0xA02B30` stub landed as CMovableResourceMemoryPool::FreeResource
(RELOCATION_MATCH). The landed (untracked, concurrent session) destructor source
calls MemoryPool->Free(...), binding CHandleBasedMemoryPool::Free (0xA02B30). Bytes
are identical, but the faithful call is MemoryPool->FreeResource(MemoryHandle).
Reported to peer sessions; their files are untouched here.
Independent review (evening, read-only agent): bank-fields native-FIRST,
009d61f0 native-base-cleanup and 009d6350 native-first all ACCEPT; review JSON
`independent_review_20261015_evening.json` in each directory. Retail byte arrays,
fixup coverage, counts and no-asm verified; typed parity re-derived (0 diffs
outside relocations). Concerns: rebuild/include/fable_bank_owned_set.h edited
17:58 after those runs (scratch copies used, landing must re-verify); 009d61f0
similarity.json 0.9706 vs typed_parity 1.0 (bytes support 1.0); base-destructor
stand-in skips the retail vtable write; bank-fields excludes uninitialised
fields and its 60 linked objects were not re-reviewed.

Also promoted: `009D52D0` `CBankFileAsyncData::SetupBuffer` 310B/115, 17 relocations, and
`009D5B60` `WaitUntilFinished` 130B/43, 7 relocations, both MATCH (gate
`gate_native_bank_async_data_buffer.py`; work wait-composition 65,536 and native-first
16,384 cases). Additional additive header declarations: pool `GetPointerFromHandle` /
`AllocateResource`, inline `CMemoryAllocator::Allocate(size,category)`, and
`friend class CBankFileAsyncData` in `CBankFileAsync` for `RestartAbortedReads`. A compile
sweep of all 693 canonical TUs that transitively include the touched headers is clean.

Identity correction (from peer session fabletlc-05): retail 0xA62ED0 is
`CMovableResourceMemoryPool::FreeResource`, a 5-byte jmp to `CHandleBasedMemoryPool::Free`
0xA02B30. The destructor and Abort sources first called `Free`, and the gate mapped that
symbol to 0xA62ED0. That was byte-identical but the wrong identity. Both now call
`FreeResource`. Gates re-pass. Every gate address table was then audited against
manifest names: no other mismatch.
native-base-genuine (prepare_stage.py from allocator stage): genuine canonical
CBaseClass TU supplies CBaseClassNonCopyable::~ 0x99A300 (7B, writes
??_7CBaseClass@@6B@ = retail 0x1231710, resolved-target exact); fixture now checks
the restored CBaseClass vtable and CMovableResource vtable after each phase
(closes review concern). PASS 1,081,344 cases/9,336,832 full16KB/FP snapshots,
same tree/allocator partitions; 14 provider omissions fail link.
Peer fabletlc-dd confirmed the FreeResource finding and fixed the landed
009D61F0/009D6350 sources plus the teardown gate (still MATCH).
native-landed-source: compiles the LANDED rebuild/src destructor (peer's
FreeResource fix) with canonical headers only; root graded 348B, equal outside
relocations, near0.9706, relocation includes ?FreeResource@CMovableResourceMemoryPool.
Genuine FreeResource 0xA62ED0 (5B jmp to Free) linked for both phases; Free stays
bounded. native-FIRST: fixture PASS output but STOP preserved: pinned whole-tree
rebuild/include/fable_display_viewport.h changed mid-run (peer edit; not included
by any TU here). native-included-pins reran the unchanged exe once, pinning
artifacts, provider sources and only /showIncludes headers: PASS 1,081,344 cases/
9,336,832 full16KB/FP snapshots, no drift. 15 provider omissions fail link.
Lesson: with concurrent sessions editing rebuild/include, pin used headers only.

Further promotions (all MATCH via `native_exact_roots`, registered in `gate_all.py`):
- `009DC030` `CRenderManager2D::Add2DTri` 648B/195, 22 relocations (`gate_native_render2d_add_tri.py`;
  seven work triangle-queue compositions, 49,152 cases each). Header: private `Add2DTri`
  declaration. A plain-`inline` helper for the thrice-repeated vertex transform is not
  inlined under /O2 (254B call form), so the repeated form is retained. 0x99AED0
  manifest `CCharString` is a fold of the CWideString default ctor, already pinned by gfmain/display gates.
- `009E14B0` `vector<CTVertexRHWColSpecTex1>::_M_insert_overflow` 294B/111 (`gate_native_render2d_vertex_growth.py`).
- `009E11A0` `__uninitialized_copy<CQuickDrawTriInfo*>` 46B and `009E1650`
  `vector<CQuickDrawTriInfo>::_M_clear` 58B: explicit-instantiation sources
  (`gate_native_quick_draw_info_helpers.py`); also COMDATs of the landed 009E1750 unit.

Deferred, with reasons:
- `009FC440` / `009FF6B0`: work sources rely on a `FABLE_RESOURCE_NOINLINE_REMOVE` macro making
  the header-inline `RemoveFromResourceList` `__declspec(noinline)`. That's a shared-header
  design decision, not a mechanical promotion.
- `0052F1E0` `IsPositionIsCenter`: the work header invents 47 placeholder virtual slots and padding.
native-pool-free (prepare_stage.py from landed-source): genuine
CHandleBasedMemoryPool::Free 0xA02B30 (201B near 0.9848), InsertFreeArea 81B and
CombineAreas 141B exact, reached through genuine FreeResource from both the retail
and landed destructor, on a real 0x464 pool at Arena+0x3000 with real 16-entry
handle table, 3-header contiguous chain and free lists; all 18 prev/next
(none/free/allocated) x single/multithreaded shapes (real pool critical section),
slots0/5/15, padding0/12, first-unused -1/3, largest0/max. Independent post-state
model (PoolFree_test expectations: handle free list, counts, survivor size/bucket,
largest, links, two free-list entries) or untouched pool when no free. PASS
1,081,344 cases/8,812,544 full16KB/FP snapshots, first run. Remaining destructor
boundaries: RemoveObject (threaded file manager), Sleep, InternalDeallocate,
allocator destructor (shutdown path).
native-internal-deallocate (prepare_stage.py from pool stage + four patch_*.py
attempts, all stops preserved): genuine InternalDeallocate 0x9F2920 (155B near
0.9831) + GetAllocationSize 0x9F2390 (110B near 0.9189) on real allocation records
(area header, tracking metadata, range-id and header-byte-id paths, valid/invalid
id, two fake sub-allocator vtables with checked query/Deallocate slots, real
AllocatorInfo[30] and range globals). Root now compiled from peer's merged
CBankFileAsyncData_Lifetime_009d5410.cpp (destructor bytes unchanged).
Stops: build-FIRST link (GetAllocationSize TU emits its own inline Enter/Leave +
direct section ref) -> section rebound + /FORCE:MULTIPLE limited to identical
Enter/Leave duplicates, map proves AllocatorSupport copies kept; build-lifetime-tu
header changed mid-build by peer -> build snapshots rebuild/include and sources;
native-FIRST code57 fixture model wrongly skipped dealloc when aborted;
native-model-fix code57 in-range id setup wrapped below 16MB -> arena fixed at
0x30000000. native-fixed-arena PASS 1,081,344 cases/9,183,744 full16KB/FP
snapshots, heap frees76,800 (0xCD wipe + tracking accounting checked), deletes
28,800; 20 provider omissions fail link. Remaining boundaries: RemoveObject,
Sleep (OS), allocator destructor 0x9F2310.
native-allocator-dtor (prepare_stage.py from deallocate stage): genuine
CMemoryAllocator::~CMemoryAllocator 0x9F2310 (116B near0.95) on the shutdown path;
CRT free -> checked recorder, DeleteCriticalSection import -> checked
non-deleting callback (retail then LeaveSynchronisation on the same section),
CMemoryAllocatorBase::operator delete 0xA66750 checked fixture definition.
Real audit buffers, sub-allocator counts (A2/B1) and size-class tables in both
allocator objects; independent release-order model (audit, 15 size classes,
AllocatorInfo sub-allocators, one section delete). Stops preserved: build-FIRST
(section ref unbound), native-FIRST guard71 after all cases passed (B audit and
B deletability both keyed off Pattern&1). native-shapes PASS 1,081,344 cases/
9,542,144 full16KB/FP snapshots, all four shutdown shapes; 21 omissions fail.
Destructor composition now leaves only RemoveObject (threaded-file manager),
Sleep (OS) and sub-allocator operator delete (0xA66750) bounded.

Async bank lifetime TU restructured to retail shape: `CBankFileAsyncData_Lifetime_009d5410.cpp`
defines private `FreeBuffer` (009D5410, 47B, PDB `?FreeBuffer@CBankFileAsyncData@@AAEXXZ`), the
destructor and Abort, both now calling `FreeBuffer()`. `/O2` implies `/Ob2`, so the compiler
auto-inlines it exactly as retail does; all three roots are exact from this one TU. The separate
destructor/Abort files were removed (gate updated; peer session informed of the new path).

`CBankFileAsync::HandleFailedAllocation` 009D5BF0 draft: near_match (92.8% opcode, 109/113
instructions) but 9 bytes short. Retail aligns three loop heads (`lea esp,[esp]`/`nop`) and
spills `this`; neither QFE-4035 nor /G5/G6/G7/GB reproduces it. Stopped after two attempts;
draft at `work/native-recreation/rendering/009d5bf0/20261015-evening/`. Not landed (no
behavior fixture). PDB-confirmed declarations added to the header: `HandleFailedAllocation`,
`friend class CBankFileAsync` in CBankFileAsyncData, and STLport `set::size/end/upper_bound`.

New exact landings (`gate_native_bank_async_failure_handler.py`):
- `009D66A0` `CBankFileAsync::SetLoadingMemoryFailureHandler` 136B/49 (counted-pointer
  assignment inlined via the per-source template-member pattern).
- `009D6420` `CLoadingMemoryFailureHandler::RegisterBank` 97B/35. Manifest label
  `vector_push_back_4byte_list` is generic; the body locks +0x10 and pushes onto Banks (+4).
- `009D7FC0` `vector<CBankFileAsync*>::_M_insert_overflow` (POD) 216B/85, explicit instantiation.
Coordination: fabletlc-dd cleared 0x98E000-0x990600 plus fable_gfuninitialise.h and
fable_threaded_file.h for this session (CThreadedFileManager::RemoveObject
0x98EB20); dd stays in 0x9D4000-0x9D8800. Retail manager layout recovery running
read-only; header edits only after the evidence report.
Independent night review (read-only agent): provider-cleanup, allocator-cleanup,
base-genuine, landed-source, pool-free and internal-deallocate all ACCEPT
(`independent_review_20261015_night.json` per stage; allocator-dtor post-dates the
review request, not yet reviewed). Recomputed artifact hashes, original.h = retail
348B with 25 resolving fixups, provider grades spot-checked with pefile,
/FORCE:MULTIPLE limited to byte-identical Enter/Leave. Minor concerns: linker
folds ~CCriticalSectionLock and Release (identical retail bodies) so a swapped
call would go unseen; exact_mod_relocations and root_grade mask rel32 targets
without naming them; stages 1-3 reuse the morning root object; stages 4-5 pin the
pre-merge Destructor_009d61f0.cpp path (gone after the peer merge; stage 6+
snapshot it); stage-6 AllocatorSupport omission now reports 2 unresolved, not 6.
native-allocator-dtor/audit_root_targets.py -> root_targets_audit.json
ROOT_TARGETS_MATCH: all25 relocations of the landed (snapshotted Lifetime TU)
destructor name the symbol whose retail address equals original.h's fixup target
at that site, kind matching (rel32/dir32). Separately distinguishes Release
0xA62960 from ~CCriticalSectionLock 0xA629A0 and FreeResource 0xA62ED0, closing
the review's swapped-call/masked-rel32 concern for the root.

Handler lane (all exact, `gate_native_bank_async_failure_handler.py`): handler TU
`CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp` defines ctor 009D5EB0 (34B), virtual
HandleFailedAllocation 009D5F10 (104B) and destructor; its compiler-emitted scalar deleting
destructor 009D8140 (63B) also matches. RTTI shows base `CFailedAllocationHandler` (vtable
0x129B874), so that class moved unchanged from `fable_resource_list.h` into the new
`fable_failed_allocation_handler.h` and `CLoadingMemoryFailureHandler` now derives from it.
Regression: 1,082-TU compile sweep clean; 13 resource/async gates plus bank cleanup, restart
and virtual-layout gates pass. Import slots confirmed from the PE import table
(InitializeCriticalSection/DeleteCriticalSection/Sleep/memmove).

Compiler-emitted deleting destructors added to the teardown gate: CBankFileAsync 009D81A0
(from landed 009D6490 TU), CBankFileAsyncData 009D8180 + W3 thunk 009D80A0 (lifetime TU),
CBankFileAsyncEntry 009D6FA0 (landed GetAsyncEntry TU). Manifest labels
CWaterPatchMesh/CLandscapeBackgroundPatch on these are folds; overrides added.

More exact roots:
- `009D5260` private Entry ctor: never called directly in retail (auto-inlined), so the
  definition moved from the header into the GetAsyncEntry TU, which still emits identical
  GetAsyncEntry bytes (gate_bank_async_entry 1134 and font-streaming chain still pass).
- `009D5140` CFailedAllocationHandler scalar deleting destructor (from handler TU).
- `009D5D30` private caller-buffer CBankFileAsyncData ctor 179B (same file as 009D5DF0).
- `009D6060` `CBankFileAsyncEntry::BeginReadData` 160B, own file. A first attempt placed it in
  the 009D6100 file: auto-alloc bytes were unchanged, but its gate's link control failed on the
  extra dependency. Lesson: some gates link whole objects; keep new roots in separate files.
- `009D5580` is a byte-identical unfolded duplicate of GetData 009D5530 (already noted as a
  clone, no credit).
Follow-up, not done: `CBankFile::Align` is defined `__declspec(noinline)` in the GetAsyncEntry
TU as a link provider. Retail's Align 009CBB90 lives in the CBankFile TU. Moving it touches
`symbol_index.py` special cases and two gates.

## CThreadedFileManager::RemoveObject 0x98EB20 landed (MATCH) with uid find 0x9902D0

Retail PC manager layout recovered read-only (agent report, evidence addresses per
field): CThreadedFileManager : CBaseClassNonCopyable, sizeof0x178, vtable0x129A164,
ctor0x98F3A0/dtor0x98E690; DeviceQueue[2][7] (12B STLport map + CurrentPosition),
CurrentDeviceAtPriorityLevel+0xE4, CurrentDeviceEndOfTimeSlice+0x100,
CurrentlyReadingObject+0x138, Objects map+0x13C, Initialised/ExitThread/
ThreadFinished+0x148..A, CriticalSection+0x14C (4-aligned unsigned long[6]),
Event+0x164, NextObjectUID+0x168, BlockPendingObjectProcessingRefCount+0x16C,
DeviceTimeSlice+0x170. CFileObject 0x40 exactly per PDB; EFileLoadingStatus
0..4 from Ego_r S_CONSTANT. Attempts (work/native-recreation/threaded/0098eb20/
20261015): cpp-first private nested type blocks explicit specialisation;
cpp-public-types missing _RTL_CRITICAL_SECTION forward; cpp-rtl-forward
unsigned char[0x18] section misaligned to 0x14B; cpp-aligned-section MATCH
156B/63 instructions/8 relocations resolved (grader first read 0x9A, regraded 0x9C).
find 0x9902D0: 71B exact (two return tails; first cut at 60 was wrong), needs the
explicit instantiation line after the specialisation like 0x9F0D90.
native-first/native-FIRST PASS: 75,264 cases/230,400 full16KB snapshots/
225,792 RB tree checks; partitions absent43,008/in-flight18,432/removed13,824;
genuine find, erase(iterator)+rebalance/rotations/extrema, real critical section,
bounded RemoveObjectFromPendingMap (status mutation), GlobalFree, operator delete.
Header sweep: all106 TUs transitively including fable_gfuninitialise.h compiled
against before/after include copies; 0 code/data differences after normalising
.debug$S and $L/$T numbering (sweep_includers.py/compare_sweeps.py).
Canonical landing: fable_gfuninitialise.h full layout, new fable_threaded_file_object.h,
00/98/CThreadedFileManager_RemoveObject_0098eb20.cpp, 00/99/STLThreadedUIDTree_find_
009902d0.cpp, gate_native_threaded_file_manager_remove.py (remove_object=MATCH
uid_find=MATCH) registered in gate_all after the uid iterator erase gate; overrides
and ARTIFACT_INDEX rows added. Re-verified: bank async open, bank async data
teardown, threaded UID iterator erase (one early AssertionError not reproducible
in three reruns; consistent with its input-hash check seeing a concurrent peer
header edit). Coverage numbers not hand-edited; dashboard not regenerated.
RemoveObjectFromPendingMap 0x98E770 DEFERRED (two graded candidates): calls
resolve correctly (_M_find 0x98FE70 out of line, iterator operator++ 0x98F9D0,
erase(const key&) 0x990CD0). cpp-map-erase 0.4745 (queue address cached for
erase); cpp-reindex 91B/35 instructions 0.8571 (retail keeps &CurrentPosition in
ebp across find, re-indexes DeviceQueue with reloaded Priority for erase);
cpp-inline-index 94B 0.7536. Lane snapshot adds map::erase(const key&) to
fable_stlport_tree.h (not canonical; that header is fabletlc-dd's). No third
candidate without new evidence.
Pending-queue iterator operator++ 0x98F9D0 landed MATCH (62B/30 instructions,
no relocations): explicit instantiation of the canonical STLport
_Rb_tree_iterator<pair<const CPendingObjectSortKey,CFileObject*>>::operator++
(header _M_increment inlined) in 00/98/STLPendingObjectTree_IteratorIncrement_
0098f9d0.cpp; byte-identical to landed CBankInfoMap iterator ++ 0x9A9E20.
Added as third root of gate_native_threaded_file_manager_remove (gate_all
expectation updated), overrides/ARTIFACT_INDEX rows added.

Indexing: every evening landing is now registered in `tools/decomp_pipeline/symbol_index.py`:
dedicated-provider entries, a `_verified_roots` row filter (verified roots only) and
companion addresses for multi-root TUs (lifetime TU, handler TU, 009D5D30 in the 009D5DF0
file, Entry ctor/deleting dtor in the GetAsyncEntry TU, CountEntries in the EntryExists TU).
Note: a later duplicate dict key in the companions literal silently overrides an earlier
one; merge into an existing source key. `symbol_index.py --only <source>` refreshes
`rebuild/build/symbol_index.tsv`; rows checked by hand.

Corrections:
- Withdrew my 009D81A0 claim; `build_candidates.ps1` already catalogs it (genuine destructor TU).
  My unlanded scan read only ARTIFACT_INDEX, so it missed catalog-only landings.
  All other claimed addresses were checked against build_candidates/bootstrap.
- 009D8180 also has a legacy catalog entry: `CLandscapeBackgroundPatch_vectordeletingdestructor_009d8180.cpp`,
  a fake struct with the same deleting-destructor shape and wrong identity (its 009D81A0 twin
  was already deleted). The genuine root is CBankFileAsyncData `??_G` in the lifetime TU.
  Retiring the fake entry is left for review; not deleted.

`CBankFile::CountEntries` 009CC3B0 (83B exact) added to the EntryExists TU, where retail
auto-inlines EntryExists (gate_bank_update still passes). GetTotalDataSize 009CC450 is exact
only in a TU that has both EntryExists and GetEntryDataSize. Those live in two cataloged
files (009CC190 / 009CC210), so it needs a catalog-level merge: deferred.

Pending: `CBankFileAsync::OpenReadOnly` 009D56C0 (348B) needs a
`CBankFileManager::FindContainedBank` declaration plus NBankFileManager types
(`fable_retail_banks.h`). Peer session fabletlc-05 has just restructured
`fable_gfuninitialise.h`, so it can be attempted now.
Pending sort-key less<> 0x98F570 (64B) and pending _M_find 0x98FE70 (150B) landed
MATCH, no relocations, one TU 00/98/STLPendingObjectTree_Find_0098fe70.cpp
(generic _M_find body + explicit instantiations). cpp-first near (find 0.9692,
less 0.9259: retail loads the left sort key fully first); cpp-cached-keys
exact with both sort keys cached in locals. Retail order: PFile->physicalSortKey_,
Desc.FilePos, object address, all unsigned. Inline operator< added to canonical
fable_threaded_file_object.h, which now includes fable_threaded_file.h first
(CCountedPointer<CThreadedFile> specialisation order). Note the agent's 77B
extent for 0x98F570 ran into the adjacent 11B sort-key ctor at 0x98F5B0.
Gate now five roots: remove_object, uid_find, pending_less, pending_find,
pending_increment all MATCH; gate_all expectation updated.
Pending-queue key-erase closure landed MATCH, one TU 00/99/STLPendingObjectTree_
KeyErase_00990cd0.cpp (async-tree precedents' equal_range/erase/range-erase/
__distance/_M_erase bodies instantiated for the pending tree): erase(key)
0x990CD0 92B, erase(first,last) 0x9905D0 189B, _M_upper_bound 0x98FCA0 89B,
_M_lower_bound 0x9901D0 89B, __distance 0x98FF50 78B, _M_erase 0x98FD40 51B.
The agent map had upper/lower swapped: MSVC evaluates pair(lower_bound(k),
upper_bound(k)) right to left, so erase(key) calls upper first; bytes (key
compared before node) and the gate's per-site relocation audit both agree.
Gate now 11 roots, all MATCH; gate_all expectation updated; overrides/
ARTIFACT_INDEX rows added. Threaded-file TU tally tonight: 11 functions landed
(RemoveObject, uid find, pending less, _M_find, ++ and the key-erase closure x6).
RemoveObjectFromPendingMap 0x98E770 still deferred.

CBankFile file accessors (`CBankFile_FileAccessors_009cba40.cpp`, all exact, metadata gate):
GetBankHandle 009CBA40 (25B), GetBankPathname 009CBF10 (20B, virtual GetPathname slot 11),
private MakeWritable 009CBF90 (70B). The header's `unknown8d_[0x0b]` now carries the Ego_r.pdb
fields ReadOnly/WrittenToFile/Solid/ChangeCount/FileValid/UpdatingFlag (retail = Ego_r
offset −0x10), with static asserts; layout unchanged, 623-TU compile sweep clean.
OpenReadOnly 009D56C0 is blocked on header architecture: `fable_retail_banks.h` explicitly
specialises `CCountedPointer<CDiskFileWin32>`, while `fable_game_text_bank.h` has already
implicitly instantiated the generic template (CBankFile::BankFile), so the two can't share a TU.
Retail also calls the CRetailBank/CThreadedFile counted-pointer destructors out of line (9A9D60/9A9C40).
Uid-map key-erase closure landed MATCH, first candidate, TU 00/99/
STLThreadedUIDTree_KeyErase_00990d60.cpp: erase(key) 0x990D60 83B, equal_range
0x9907D0 83B (out of line here, unlike the pending map), __distance 0x98FFA0 78B,
erase(first,last) 0x9906E0 189B, _M_erase 0x98FD80 51B. Gate now 16 roots all
MATCH (gate_all expectation updated); overrides/ARTIFACT_INDEX rows added.
Threaded-file TU tally tonight: 16 functions landed; RemoveObjectFromPendingMap
0x98E770 deferred. Next candidate: OnFinishObject 0x98EBC0.
OnFinishObject 0x98EBC0 landed MATCH (121B/39 instructions/5 relocations, first
candidate). native-first PASS 15,360 cases/86,016 full16KB snapshots/46,080 RB
tree checks, partitions 3,840 each (queued/unqueued x present/absent): retail
bytes vs candidate, genuine uid erase(key) closure + rebalance, real critical
section held by caller and released around a vtable-slot-2 completion callback
that mutates queue flag/status/current object/UID. Canonical additive edits:
OnFinishObject declaration (fable_gfuninitialise.h) and map::erase(const key&)
(fable_stlport_tree.h, fabletlc-dd notified); bank async open/teardown, uid
iterator erase and async key-erase gates re-pass. Gate now 17 roots MATCH.
Threaded-file TU tally tonight: 17 functions landed.

More exact roots: `NGameText::CDataBank::GetLastModifiedTime` 009CB710 (RTTI: slot 18 of the
CDataBank vtable 0x129B4D4; manifest's CStreamingFontDataBank label is wrong);
`CSymbolMap::FindFirstSymbolWithVal(long)` 009E5090 and `(const CCharString&, long)` 009E50C0
(`gate_native_symbol_map_queries.py`). The latter needed PDB-named inline
`CCharString::operator==(const CCharString&)` and `CBasicString<char>::operator==(const CBasicString&)`
(early length return, then `return Compare(...) == 0`, which reproduces retail's materialised
bool). Safety check: before/after compile of all 1,439 TUs that include fable_string.h, comparing
named-symbol section bytes and relocations (local $L/$T label names normalised, .debug excluded):
only the intended TU changed. The sweep tool (scratchpad `bytesweep.py`) is worth promoting
into tools/ for header changes.
Pending-map behaviour evidence (work/native-recreation/threaded/pending-map/
20261015/native-first): phase 0 links the retail bytes of all nine pending-map
functions to each other (9 rel32 fixups, free/_Rebalance_for_erase external),
phase 1 the compiled canonical closure. native-FIRST STOP preserved (code60):
fixture reused one expected-snapshot buffer across three checkpoints (diagnostic
build-diag2 showed the node count from the post-erase checkpoint). native-snapshots
PASS: 4,096 cases/172,032 calls/24,576 full16KB snapshots/12,288 RB checks,
present/absent 2,048 each; 8 objects over 3 files with sort-key and file-position
ties. Gate behaviour-evidence list now cites RemoveObject, OnFinishObject and
pending-map fixtures.
symbol_index.py registration (generated from the gate ROOTS by
work/native-recreation/threaded/register_symbol_index.py): 7 files added to the
dedicated-provider list, _verified_roots and companions; --only reindex shows all
17 symbols at their retail addresses in rebuild/build/symbol_index.tsv. Like the
other native_exact_roots landings tonight, they are not catalog entries in
build_candidates.ps1. Coverage numbers not hand-edited; dashboard not regenerated.

CDateAndTime (`gate_native_date_and_time.py`, all exact): SYSTEMTIME ctor 009E9A30, operator!=
009E9B20 (manifest says operator==; it is `!(*this == other)` with the landed 009E9AD0 == auto-inlined,
so it was added to that Equal TU; catalog fixture DATE_EQUAL_009e9ad0_PASS still passes),
operator< 009E9B80, operator> 009E9BE0 (manifest says operator<; setg = ??O), ToFileTime 009E9D40
(SystemTimeToFileTime import slot 0x143FD4C confirmed). New reusable tool
`tools/decomp_pipeline/header_byte_sweep.py` (snapshot/diff), self-tested clean on 140 TUs; GOTCHAS updated.
SetObjectPriority 0x98EFF0 landed NEAR_MATCH (151B/59 instructions, 0.9831).
Evidence: cpp-first 139B (called the uid _M_find directly; out-of-line find
specialisation now declared in canonical fable_threaded_file_object.h);
cpp-ctor-key near 0.9831 after adding the retail CPendingObjectSortKey(CFileObject*)
constructor (0x98F5B0) to the canonical header. AddObjectToPendingMap 0x98ECA0
deferred after four candidates (best cpp-queue-ref 0.7742; retail computes the
queue address before storing the key) — its out-of-line copy stays in the TU,
unclaimed. native-first PASS 241,920 cases/594,432 full16KB snapshots
(absent138,240/unmoved76,032/moved27,648): genuine uid find, real critical
section, bounded pending removal (device flip/priority scribble) and pending
operator[] (destination queue/key/priority checked, value slot receives object).
Gate gains NEAR_ROOTS (size, instruction count, similarity floor 0.98, call/IAT
target set equal to retail): set_object_priority=NEAR_MATCH; gate_all, overrides,
ARTIFACT_INDEX and symbol_index (verified root only) registered. Tally: 18
threaded-file functions landed tonight (17 MATCH, 1 NEAR).

CBankFile::GetUnusedSpace 009CC4C0 (38B, named local `space = FileSize` reproduces the load-before-call
order) added to the metadata gate; `unknownf4_` became Ego_r `bool WarningFlag` (+0xF4, static assert).
WriteBankWarning 009CBD90 waits on an unmodelled `CProgressCallback` (slot 3).

CDriveManager (`gate_native_drive_manager.py`, all exact, one TU `CDriveManager_FileOperations_009e4b70.cpp`):
DeleteDirectory 009E4B70, DeleteFileW 009E4BC0 (manifest label "DeleteDirectory" wrong), DriveExists
009E4AE0 and GetDriveFromPath 009E4C70. The last two carry /GS cookies, so the TU builds with /GS
(`native_exact_roots` now takes per-root `flags`). EDriveReturn enumerators and the
locals/line numbers come from `ghidra_out/ego_r_pdb.xml` (function entries list locals and
lib_drive_manager.cpp line tables, which is very useful for statement order). Retail mangles wchar_t
natively (`_W`); the canonical tree builds without /Zc:wchar_t (`G`). Bytes are unaffected; noted in gate.
__security_cookie 0x139C8A8 confirmed from __security_check_cookie's body.
Running total for this evening: 44 exact roots across 12 gates.

`CDriveManager::GetItemFromPath` 009E4CB0: near only (94.6% opcode, 74/74 instructions, same size)
after five loop shapes guided by the PDB line table. The residue is an extra `xor ecx,ecx` before
16-bit loads, missing loop-head padding and a non-rotated inner loop; /Zc:wchar_t and QFE-4035 don't
change it. That's the same toolset-level class as HandleFailedAllocation. Draft at
`work/native-recreation/ui/009e4cb0/GetItemFromPath_near_draft.cpp`, not landed. The declaration
(PDB-named) stays in the header. CreateDirectoryW 009E4DA0 (/GS, 0x21C frame) not attempted yet.

CConsole command line (`gate_native_console_command_line.py`, one genuine TU
`CConsole_CommandLineBuffers_009ea5a0.cpp`): new exact roots ResetCommandLineBuffers 009EA5A0,
RevertToUserEntry 009EA660, PrevInHistory 009EA980 and NextInHistory 009EA9E0. They depend on
Reset and the buffer copies being auto-inlined, so the TU also re-derives Reset 009EA4F0 and the
copies 009EA620/009EA640 in shared-header form (exact; verified in the gate, not re-claimed in the
index). Retail's strict load/store order comes from the implicit `CConsoleCommandLine::operator=`
(`FrontCommandLine = BackCommandLine`); the legacy files fake it with `volatile` and local structs.
Identity correction: 009EA620 is CopyFrontBufferToBackBuffer (its legacy file is named
CopyBack...). Follow-up: retire the three legacy fake-struct files in favour of this TU (they share
no link today: catalog builds each with its own test). PDB-named private helpers
(IsGuessMode/IsHistoryMode/UnsetGuessMode/UnsetHistoryMode) are inline in fable_console.h.
Running total: 48 exact roots across 13 gates.

Also: CConsole::SetHistoryMode 009EAB80 (82B exact, loop-alignment nop reproduced). Stopped here (usage limit): 49 exact roots / 13 gates. Next candidates: SetGuessMode 009EAFF0 (needs out-of-line CConsoleCommandLine(const CCharString&) 009EEA10), SetHelpBarString 009EB2C0 (CStringParser), CreateDirectoryW 009E4DA0 (/GS); crawl/next_smallest.py for a ranked queue.
