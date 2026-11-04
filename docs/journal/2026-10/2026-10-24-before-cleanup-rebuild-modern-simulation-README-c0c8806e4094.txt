# Bounded simulation behavior

`fable_simulation` starts the modern simulation library with the generic
`CThing::GetChecksum` result calculation (retail `0x004C7DA0`, vtable slot 23).
It also provides the measured persistent-ID index through `ThingIdIndex`.
`ThingStore` now owns initialized value state with validated runtime handles;
full gameplay initialization/components and a game session remain open. This is an ordinary
C++23 static library with no other engine-library dependencies.

## Checksum API and ownership

`ThingChecksumInput` owns scalar metadata and an optional six-float physics value.
The caller creates, modifies and destroys those values. `thing_checksum` borrows
them only for its call, retains nothing, allocates nothing and changes no state.
There are no entity pointers, file-layout headers, OS services, Lua or GPU types.
Concurrent calls with independently stable inputs need no synchronization.

`ThingChecksumMode` selects precision 24/53/64 and nearest/down/up/toward-zero
rounding explicitly. The default is the measured normal/forced update-entry
contract, precision 53 and nearest. Invalid enum values return
`ThingChecksumError::invalid_numeric_mode`. Local types return zero; absent
physics returns the unsigned integer base. Other inputs preserve the native
addition ordering, unsigned conversion and CRT integer-helper result semantics.

The implementation decodes binary32 component representations and performs
bounded integer arithmetic in units of 2^-149, with a private five-limb magnitude.
Six finite binary32 values and the integer base fit that representation even
before each explicit rounding step. The public API exposes normal float values;
the private arithmetic is neither an on-disk format nor a general x87 emulator.
Host floating-point controls and exception flags are preserved. Retail FP status
flags/unmasked exceptions are not emulated; this API models the checksum word.

See [THING_CHECKSUM_NUMERICS](../../../docs/modern/THING_CHECKSUM_NUMERICS.md) for
field evidence, instruction order, signed-64 boundary behavior and corpus provenance.
No canonical source provider, byte-parity grade or source-count promotion is made.

## Verification

The ordinary CTest target `fable_simulation.ThingChecksum` uses literal native
observations for precision pressure, conversion boundaries, early returns,
nonfinite/subnormal values and invalid modes. It repeats valid cases under all
four host rounding modes and checks that controls and exception flags survive.
Those tests need no retail assets and run in the existing Windows/Linux workflow.

For the complete locally captured oracle corpus:

```powershell
cmake --build work/modern_build_x64 --target fable_thing_checksum_tests
work/modern_build_x64/simulation/fable_thing_checksum_tests.exe <oracle-proof>/corpus.bin results.bin
```

The optional corpus path is test-only transport. It validates framing/reserved
fields, compares each result in all four host rounding modes and reports the first
differing record. It writes one computed little-endian result word per record.

Full proof: `work/modern_thing_kernel_20261018/validation/summary.json` and
`retail_result_words.json`. All 109,608 retail records match on MSVC x86/x64,
clang-cl x64, Windows GCC, and a desktop-free/reconstruction-free x64 build.
Output streams match the original retail words exactly (SHA-256
`e0033fe5fdf57c88d49f08247188e81e6f3d5233b093d8d1e92e9ea540f7865a`).
The full suite passes 36 tests per MSVC/clang configuration, 20 GCC, and 17 in
the desktop-free build. Local Linux execution is still unverified.

## Remaining integration

The integration module now adapts typed reconstructed metadata/physics fields to
these values, and the aggregate replay uses that adapter through its slot-23
shim. Its borrowed fixtures do not establish live object ownership. See
[integration/README](../integration/README.md).
This library does not implement complete retail factory/component lifetimes, derived-class checksum
overrides, the rest of physics/world simulation, RNG, or a headless game session.
Those are separate evidence and ownership gates in
[ENGINE_ARCHITECTURE](../../../docs/modern/ENGINE_ARCHITECTURE.md).

## Persistent-ID index

`ThingIdIndex` owns UID-to-handle records, retaining the first value on duplicate
insertion and erasing by UID regardless of notifying object identity. Persistent
64-bit IDs and slot/generation handles are separate value types in
`ThingIdentity.hpp`. Lookup returns a copy; no map view or entity pointer escapes.
The session owns the index. `ThingStore` now owns initialized records, issues and
validates handles, and keeps ordered UID notifications consistent. The index alone does
not allocate entities, prove liveness, or implement UID assignment/remapping.

Ordinary CTest runs asset-free cases for duplicate/removal behavior, 64-bit UID
boundaries, zero UID, generation preservation and copied lookup results. An
optional native corpus argument checks explicit inputs/results and every mapping
after each operation, reporting the first differing field. All 8,941 native
operations and 93,167 mapping observations match on MSVC x86/x64, clang-cl,
Windows GCC, and the desktop-free build. Full suites pass 38 tests per MSVC/clang
configuration, 21 GCC and 18 desktop-free. This does not establish local Linux
execution or an owned headless gameplay session.

See [THING_UID_INDEX](../../../docs/modern/THING_UID_INDEX.md) for native addresses,
ownership/API semantics, reproduction and proof under
`work/modern_uid_index_20261018/{build_validation,corpus_validation,retail_probe}`.

## Owned initialized state

`ThingStore` owns a fixed-capacity slot array, global/per-type registration links
and its UID index. Handles carry a slot, generation and process-local owner tag;
every access validates all three. Clear/reuse does not revive handles, generations
retire rather than wrap, and stores cannot be copied or moved. Reads and order
extraction return copies. UID-index allocation failure rolls back registration.
The store accepts caller-initialized values, not raw retail objects or save cells.

Its public API matches all 16,384 native registration transitions and all 109,608
numeric records through owned creation/read/update/checksum/destruction, under
four host rounding modes on MSVC x86/x64, clang-cl, GCC and desktop-free x64.
Full suites pass 39 tests per MSVC/clang configuration, 22 GCC and 19 desktop-free.
These are bounded component comparisons; they do not satisfy the representative
headless gameplay-session gate or prove local Linux execution.

See [THING_STORE](../../../docs/modern/THING_STORE.md) for ownership, failure and
retirement contracts, supported state, native comparison scope and reproduction.
Full proof: `work/modern_thing_store_20261018/validation/summary.json` with archived
exact inputs, complete logs, target metadata and native corpus markers.

## UID allocation and map-relative identity

`ThingUid` implements the measured global/local/map-specific UID allocator and
serialization remapper. Caller-owned counters and narrow map lookup/sequence
capabilities replace retail manager/world access. Prefix arithmetic, wrapping,
LevelLess filtering, game-state bypass and the exact counter/capability call
order are preserved. These values remain distinct from runtime handle identity.
Collision repair and loaded-counter advancement now use narrow map/index
capabilities and an owned map-identity table. Full spawn/load integration remains
open; allocation and single-pass repair do not promise uniqueness.

All 5,160 native requests and 992 capability observations match on MSVC x86/x64,
clang-cl, GCC and desktop-free x64. Ordinary asset-free tests also compose generated
IDs and relative-map round trips with owned lookup, deletion and slot reuse.
Full suites now pass 40 tests per MSVC/clang configuration, 23 GCC and 20 desktop-
free. See [THING_UID_ALLOCATION](../../../docs/modern/THING_UID_ALLOCATION.md) for
the native schema evidence identifying LevelLess, API ownership and reproduction.

`ThingUidMapTable` owns map UID metadata and optional sequence counters, with
copy-returning reads and checked indices. The repair corpus compares 4,200 native
requests and 12,839 ordered capability observations through this concrete table.
The complete repair corpus and prior allocation/remapping corpus match on all
five builds. Latest full suites pass 41 MSVC/clang, 24 GCC and 21 desktop-free.
See [THING_UID_REPAIR](../../../docs/modern/THING_UID_REPAIR.md) for call order,
loaded-state semantics, collision handling and the remaining session gates.

## Owned generic components

`ThingComponents` owns uniquely allocated component instances, sorted membership,
validated component handles, cached role identities and update order. A session
weak queue owns copied pending-removal references. `ThingStore` lazily owns these
collections and tears them down before UID/list removal. Factories and typed
parameters are narrow portable interfaces; no raw component views escape.

The component test compares the complete native corpus through the public owned
collection API and separately checks real Store ownership, replacement invalidation,
generation retirement, exception cleanup and teardown ordering. Full recovered
definition-driven creation and derived gameplay components remain gates. See
[THING_COMPONENT_STORE](../../../docs/modern/THING_COMPONENT_STORE.md).

All 9,394 native component operations and 9,177 snapshots match on the five build
configurations. Latest suites pass 42 MSVC/clang, 25 GCC and 22 desktop-free, with
full registration/checksum/UID corpora retained. Proof is under
`work/modern_owned_components_20261018/validation/`.

## Staged generic creation

`ThingInitialization` now registers generic entities before UID publication, owns
typed definition/string data, preserves callback order and notifies creation only
after OnCreate callbacks. Existing `create_initialized` still publishes complete
caller values immediately. Component callbacks may attach other types through
copied owner-phase iteration and immediate catch-up; active replacement/lifetime
operations are guarded. All 1,277 native cases and 8,800 comparable boundary states
match across MSVC x86/x64, clang-cl, GCC and desktop-free x64. Latest suites pass
43 MSVC/clang tests, 26 GCC and 23 desktop-free, retaining the complete component,
registration, numeric and UID corpora. Full validation and remaining gates are recorded
in [THING_STAGED_CREATION](../../../docs/modern/THING_STAGED_CREATION.md).

## Definition component construction

`ThingDefinition` now owns component name keys/replacement settings and the
StartKillLocked value. `ThingDefinitionConstruction` resolves owned names,
captures the conditional replacement policy, attaches through the actual owned
Store path and assigns the lock last. A dedicated guard spans policy and name
lookup callbacks as well as attachments. An assembled creation test explicitly
delegates the generic host hook to this service before script strings/Mapwho.

All 1,592 native definition cases and 26,810 requests match through normalized
request/ownership comparison. Latest suites pass 44 MSVC/clang tests, 27 GCC and
24 desktop-free, retaining every earlier creation/component/registration/numeric/
UID corpus. Authoritative proof: `work/modern_owned_definition_20261018/final_validation/`.
The native definition orchestration controls AddTC; actual attachment has its
separate native reference. Decoded definitions, derived components and later
activation remain gates. See [THING_DEFINITION_STORE](../../../docs/modern/THING_DEFINITION_STORE.md).

## Initial activation and owned membership

`ThingActivation` now implements both generic initial-activation phases, preserving
quest-before-counter timing, immediate catch-up and copied component dispatch.
`ThingStore` owns separate per-type update and global render lists with fixed
slot links, idempotent insertion and copied handle extraction. Cached physics is
notified before first update insertion. Disposal unlinks owned records safely;
native Kill/active removal remains separate work.

All 1,608 native cases/5,660 phases match 28,206 observable snapshots. The native
trace retains 31,648 events including internal manager entry markers. Latest
full suites pass 45 MSVC/clang tests, 28 GCC and 25 desktop-free, retaining all
prior corpora. An assembled golden connects creation, OnCreate, both activation
phases and disposal. Authoritative proof:
`work/modern_owned_activation_20261018/final_validation/`. See
[THING_ACTIVATION_STORE](../../../docs/modern/THING_ACTIVATION_STORE.md) for host
boundaries, comparison scope and remaining derived behavior/session gates.


## Owned Kill and queued sweep

`ThingKill` connects the recovered request sequence to owned component callbacks,
render removal and the Store's shared weak-handle queue. Sweep notifies cached
physics before update unlink, consumes same-sweep appends and performs actual
component/weak/UID disposal. Duplicate or expired generations are skipped safely;
partial failures retain prior effects and permit retry.

All 1,524 native Kill cases/2,477 requests match 9,064 observable snapshots, and
1,108 sweep cases/4,220 sweeps/1,936 deletions match 11,841. Full suites now pass
47 MSVC/clang tests, 30 GCC and 27 desktop-free, retaining every earlier corpus.
An assembled golden connects creation through activation, Kill and sweep.
Authoritative proof: `work/modern_owned_kill_20261018/validation/`.
See [THING_KILL_STORE](../../../docs/modern/THING_KILL_STORE.md) for callbacks,
normalization and ownership rules. Tick execution, actual assets/derived gameplay
and representative headless gameplay remain gates.


## Generic normal and bullet-time ticks

`ThingTick` dispatches insertion-ordered component updates with explicit
`WorldTick` frames, preserves normal Dead/ToBeRemoved timing and processes the
shared stable-node weak removal queue. Bullet-time uses its separate carrying
capability/pass. Retention reads owned definition/force/appearance activity state.
Replacement of other types and tail appends can run in the same pass; queued
removal retains native stale membership/cache identities with validated uses.

All 2,448 native cases/4,156 retention queries match 27,437 observable snapshots
(30,245 native events retained). Full suites pass 48 MSVC/clang tests, 31 GCC and
28 desktop-free, including every earlier corpus. The assembled lifecycle now
includes both tick paths and live component removal before Kill/sweep.
Proof: `work/modern_owned_tick_20261018/validation/`. See
[THING_TICK_STORE](../../../docs/modern/THING_TICK_STORE.md) for ownership,
normalization and remaining manager/derived gameplay/scheduling gates.


## Tick-driven entity update membership

`ThingTick::set_as_frame_updated` and `set_force_frame_update` now connect the
recovered entity wrappers to actual Store insertion. Tick callbacks may insert
or remove same-owner update membership; nested physics notifications preserve
the outer executing-type/phase guards. Clearing force leaves membership intact.
All 1,204 composed native cases/10,367 observations match, with no event filter.
Full suites pass 49 MSVC/clang tests, 32 GCC, 29 desktop-free, retaining all prior
corpora. Proof: `work/modern_owned_tick_membership_20261018/validation/`.
See [THING_TICK_MEMBERSHIP](../../../docs/modern/THING_TICK_MEMBERSHIP.md).
Manager traversal is the next implementation gate.


The [owned manager traversal](../../../docs/modern/THING_WORLD_UPDATE_STORE.md) now passes 1,241 native cases,
2,448 calls and 9,926 observable events plus final states. All five builds pass
(50 MSVC/clang tests, 33 GCC, 30 desktop-free). The assembled lifecycle uses
actual manager-to-generic-tick delegation and final death sweep. Bullet policy
and derived gameplay remain explicit controlled boundaries.


The [portable full-update policy](../../../docs/modern/BULLET_FULL_UPDATE_STORE.md) now matches all 30,840 native
records in four host rounding modes and drives the assembled lifecycle fixture.
All five builds pass (51 MSVC/clang, 34 GCC, 31 desktop-free tests). The broader
affected-entity predicate and real multiplier registration remain open.


The [owned affected-entity policy](../../../docs/modern/BULLET_AFFECTED_STORE.md) now matches 3,597 native cases
and 32,040 comparable events plus final states. The assembled lifecycle uses
both recovered bullet policies. All five builds pass (52 MSVC/clang, 35 GCC,
32 desktop-free tests); component/player reference and script capabilities
remain explicit integration boundaries.


The [owned script-control predicate](../../../docs/modern/THING_SCRIPT_CONTROL_STORE.md) now matches 1,288 native cases
and 2,574 queries, with protected delegation from the affected policy. All five
builds pass (53 MSVC/clang, 36 GCC, 33 desktop-free tests). Action execution and
component/player reference ownership remain separate integration milestones.


The [owned affected/script composition](../../../docs/modern/BULLET_SCRIPT_COMPOSITION.md) matches 4,109 native cases,
9,834 calls and 42,573 comparable observations. All five builds pass
(54 MSVC/clang, 37 GCC, 34 desktop-free tests). Component/player reference
ownership remains the next integration boundary; full gameplay is still open.


The [owned entity-reference value](../../../docs/modern/THING_REFERENCE_OWNERSHIP.md) validates targets against its borrowed
Store and expires safely across teardown and slot reuse. Five-build regression
passes (55 MSVC/clang, 38 GCC, 35 desktop-free tests). Component/player target
adapters remain open.


[Guarded component observations](../../../docs/modern/THING_COMPONENT_ACCESS.md) now permit exact delegated reads from
affected/script policies while protecting owner and component lifetimes. All
five builds pass (56 MSVC/clang, 39 GCC, 36 desktop-free tests). Production
component/player target adapters remain open.


The [owned player target table](../../../docs/modern/THING_PLAYER_TARGETS.md) matches 6,912 native cases and 13,824
selection/target pairs and feeds the bullet-policy fixture. All five builds pass
(57 MSVC/clang, 40 GCC, 37 desktop-free tests). Full player creation and
component71/75 target integration remain open.


[Owned carryable/particle target queries](../../../docs/modern/THING_COMPONENT_TARGETS.md) now drive the combined replay with
real player/entity/component references: 4,109 cases and 41,884 comparable
observations match. Five builds pass (58 MSVC/clang, 41 GCC, 38 desktop-free).
Full carrying/attachment commands and lifecycle side effects remain open.


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



The [owned particle ClearAttachments command](../../../docs/modern/THING_PARTICLE_CLEAR_ATTACHMENTS.md)
uses guarded access to the particle and tracker, owns attachment strings/flags,
and preserves AlwaysUpdate and pending creation state. Its composed corpus
projection and validation receipts are documented in the contract. Attachment
creation, resource effects and tick-time callers remain open.


The [owned emitter attachment update](../../../docs/modern/PARTICLE_EMITTER_ATTACHMENT_UPDATE.md)
adds pending orientation and FloatOffset state, value interpolation capabilities,
validated resource identities and exact finite offset arithmetic. Physics,
appearance/water and emitter backends remain explicit capabilities. Its staged
replay matches the qualified instruction corpus and native arithmetic snippets;
see the contract and current validation receipt for the promoted build status.


The [prepared AttachToThing command](../../../docs/modern/THING_PARTICLE_ATTACHMENTS.md)
composes private clear/update cores under one primary scope, acquiring old/new
trackers and sound separately. Definition and criteria leases preserve measured
release order. Sound/string services and the affected predicate remain explicit
controlled capabilities; public policy queries and recursive commands remain
rejected. This does not complete the gameplay/resource backends.


[Scoped affected-policy reads](../../../docs/modern/THING_COMPONENT_POLICY_SCOPES.md)
retain the command's primary guard and delegate only exact observed component
reads. Related mutation, public policy recursion and nested observations stay
rejected. The prepared attachment caller can receive a borrowed affected-policy
instance; its earlier controlled policy-host path remains explicit when absent.
Script delegation preserves the same owner/type protection. Real backend and
full gameplay integration remain separate.


The [script-query attachment composition](../../../docs/modern/PARTICLE_SCRIPT_CONTROL_COMPOSITION.md)
replays the attachment caller through the genuine owned affected and script-control
facades, reading guarded component31 lock/list state. Prepared action entries
serve only list-emptiness observations; no actions or Lua execute. This expands
combined query coverage while real script/action backends remain separate.


The [owned sound-name gate](../../../docs/modern/PARTICLE_SOUND_NAME_GATE.md)
reads borrowed name bytes from a held definition lease. Empty or leading-NUL
values skip criteria construction, matching the native C-string comparison.
The query no longer depends on a host-provided nonempty boolean. Other string,
sound and resource backend services remain explicit capabilities.


The [criteria byte-value helper](../../../docs/modern/PARTICLE_SOUND_CRITERIA_VALUE.md)
builds the recovered literal/uppercase value from a borrowed name and copied
128-byte character map, preserving both C-string boundaries and high bytes.
It does not mutate the process locale or expose packed string storage. The
attachment caller's criteria capability is still separate integration work.

Particle attachment criteria values use owned name/map snapshots; see
[the caller criteria contract](../../../docs/modern/PARTICLE_CRITERIA_ATTACHMENT.md).

Owned sub-definition snapshots use byte-length CRC keys and copied lookup values;
see [the lookup contract](../../../docs/modern/SUB_DEFINITION_INDEX.md).

Particle sound-definition replay composes owned immutable sub-definition snapshots
with caller leases; see [the integration contract](../../../docs/modern/PARTICLE_SUBDEFINITION_ATTACHMENT.md).

Prepared definition-cache caller replay composes the portable assets recency owner
with sub-definition values and leases; see [the cache integration contract](../../../docs/modern/PARTICLE_CACHED_DEFINITION_ATTACHMENT.md).
