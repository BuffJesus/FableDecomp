# QS_MeetSister preparation (not activated)

New Oakvale visual/audio verification remains the first priority. This recovery
was performed while waiting for the user's next New Oakvale launch signal.
No converter outputs, seed sources, installed registry or scripts were changed.

`retail-entity-byte-recovery-20260912.json` records the installed executable hash,
raw bytes and disassembly for the bindings and both entity bodies. This fills
an evidence gap: native_threads/QS_MeetSister.json contains no recovered entity
functions, whereas the native bodies do exist.

| Entity | Registration order | Allocator | Vtable | Main |
| --- | --- | --- | --- | --- |
| MeetSisterMessenger | first | 0x00E28F70 | 0x012E2230 | 0x00E292C0 |
| MeetSisterSister | second | 0x00E29040 | 0x012E224C | 0x00E295E0 |

Both Init slots point to the empty function 0x00CDEBB0. Both allocators construct
the bound thing at host +8, parent at +0x14 and master data at +0x18.

The Messenger waits for IsRegionLoaded("ArenaExterior"), yielding and checking
termination. Native actors are case-sensitive `Hero` and `Messenger`; macro is
`CS_ARENA_LEAVE_THERESA_01`. It acquires scripted resources and enters a movie
scope with PauseAllNonScriptedEntities(true). Macro pushes skippable=1 and zero
setup/input/flags. RemoveThing is called **inside** the paused movie scope with
false,true operands, before unpause/movie destruction. Do not replace this with
a generic wrapper that closes its movie scope before RemoveThing.

The Sister uses actors `Hero` and `Sister` and macro
`CS_ARENA_LEAVE_THERESA_02`. Its native order differs: unpause, movie destruction,
actor-map teardown and one resource release precede RemoveThing(bound me,false,true).
Then SetQuestAsCompleted(activeQuest,false,false,false),
DeactivateQuestLater(activeQuest,0), FadeScreenIn and final resource release.

Existing seed scripts perform a GetThingWithScriptName self-lookup and remove
that result. The reviewed native bodies use bound thing/resources instead; do
not carry the seed lookup into a reviewed port without native evidence. Seed
PlayCutscene calls also do not establish the native movie/resource lifetime.

Remaining review before constructing/activating a supported port:

- Trace both acquisition loops and exact control priority/termination cleanup.
- Review the differing movie/map/resource destruction boundaries against Forge
  StartMovieSequence and RunCutsceneWithSetup behavior.
- Decode both installed macro records, including setup and skip conditions.
- Add meaningful fixtures for region waiting, termination, skip-independent
  cleanup and completion; mock success alone is not native playback proof.
- Preserve single authority and stage deployment only after New Oakvale fixes
  have been verified in game.

## Additional preparation

Both entity bodies acquire their bound entity at priority 4, retrying after
NewScriptFrame and checking the entity termination helper 0x00F35B30. The Hero
acquisition is also priority 4, but is a single native call whose result is not
tested; do not describe it as an observed retry loop. The normal control-resource
unwind is Hero followed by the bound entity. The cancellation-path Bully fix
documents why a parent-quest-only termination query would be insufficient here.

`installed-macros-20260912.json` now contains both macros decoded without leftover
bytes from the hashed installed script.bin. Messenger has 13 commands and skip:
FadeOut, GamePause 0.5, Hero.Teleport P_MARKERMESSENGER2, FadeIn. Sister has 49
commands and skip: FadeOut, GamePause 0.5, NoDialogCam false, Hero.Teleport
P_HEROWT, Sister.Teleport P_SISTERWT, FadeIn. Authored actor names retain their
mixed case. This resolves the macro-record recovery item above; playback and
camera/marker dependency validation remain pending.

## Executable reference port and remaining runtime adapter

`../reference/quest.lua` now reconstructs root Main and both entity Main bodies.
It is deliberately outside an FSE package. Its explicit `h` adapter models native
resource ownership; these names are not new registered Forge APIs. Eight tests
execute this Lua with Lupa (`test_meet_sister_reference.py`), covering region wait,
termination before/during acquisition, retries, failed single Hero acquisition,
normal completion, and termination during the macro.

Unlike Bully RUN1, neither Meet Sister body queries termination after the macro.
Messenger still removes its bound thing while paused. Sister still closes the
movie/map, releases Hero, removes itself and completes the quest. Do not copy the
Bully cancellation gate here. Sister obtains the active quest name twice.

The adapter must provide distinct named resources (`new_resource`, `prepare`,
`acquire`, `release`), reference-copying actor maps (`new_map`, `map_actor`,
`destroy_map`), explicit movie lifetime (`start_movie`, `pause`, `destroy_movie`),
and direct macro dispatch (`run_macro`) without implicit control acquisition.
`frame` and `terminating` are distinct operations, mirroring the native calls.
The other operations are direct quest/interface operations named in the model.

Current Forge gaps preventing a faithful executable package:

1. `LuaEntityAPI::AcquireControl` retries until success/termination. Retail's Hero
   attempt in both entities is one unchecked call. A boolean success mock does
   not prove this path is portable with that existing helper.
2. `RunCutsceneWithSetup` owns temporary actor handles and tears them down with
   its map before returning, even when it reuses the caller's movie. Retail's
   Messenger map remains alive past removal/movie teardown; Sister destroys its
   map after movie teardown and before releasing Hero/removal.
3. Existing per-actor control caching/borrowing cannot express all independent
   resource identities and empty-resource copies on a failed Hero acquisition.

These are adapter-design requirements, not permission to change the currently
installed runtime or approximate the sequence. No converter files were changed.

## Installed dependency closure

All 19 distinct marker/camera names in both normal and skip macros resolve
uniquely in installed ArenaExterior.tng or HauntedHouse.tng, with definitions,
complete records and hashes archived in installed-marker-camera-dependencies-20260912.json.
Eight text references (two groups, five direct speech entries and one objective)
resolve in English text.big; group members and source hash are archived in
installed-dialogue-dependencies-20260912.json. This proves asset presence, not
camera playback or audio output.

The bound actors themselves live in the QS_MeetSister sections of those two TNGs.
FinalAlbion.qst declares AddQuest("QS_MeetSister", FALSE) and provides a retail
test-quest entry at ArenaHSP. Do not change that declaration or force activation
in the New Oakvale profile while its runtime testing is pending.

Evidence-label correction: 0xE28DE0 is quest Main; RegisterMain is 0xE28D40.
The earlier byte JSON called the former `register_main`; corrected to `quest_main`.
The earlier table's numeric "Binding ID" column did not establish a runtime ID
from the archived bytes; replaced with the proven registration order.

## Native execution comparison

Unicorn now executes the installed x86 bodies at E292C0 and E295E0 through
normal, failed Hero acquisition, successful retry, acquisition cancellation,
region wait/cancellation and post-acquisition termination paths. Fourteen
scenarios match the actual Lua reference call-for-call, including each release
and map/movie boundary. Native return stacks balance. Engine operations are
stubbed and script resources start empty: this is script control-flow evidence,
not native scheduler, renderer, or internal reference-count emulation.

Result: native-lua-flow-comparison-20260912.json.
Reproducer from workspace root:
`python work/runtime-20260912-video-comparison/prove_meet_sister_native_flow.py`.
It uses the isolated Unicorn package under work/runtime_re_tools and executes
actual reference Lua via the Lupa test host. Unknown native callees fail the run.

## Binding flags: additional proven runtime gap

The numeric values formerly called Binding IDs have now been traced: native
Messenger binding+0x18 is 1 (E28E33); Sister is 0 (E28E97). Binding factory
E29110 reads that field at E2916B and passes it as the last constructor operand
to CE1110. CE117B stores it at active-script+0x3C. The matching native/PDB
CActiveEntityScriptBase layout names +0x3C `Flags`.

For an existing active script, CB88B0 (OnScriptedEntityDeactivated) takes a
special branch when its trailing boolean is true AND Flags bit 0 is set:
it calls F359F0(true), which sets CSpawnedFuncBase+0x28 FuncPaused. Otherwise
it sets the finished byte +5 and calls SaveEntityScript. The existing-script
activation path at CB899F tests the same bit: set -> F359F0(false); clear ->
entity Init and rescheduling through CB7EE0. These are lifecycle differences,
not a cosmetic identifier. The meaning of the deactivation boolean is not
assumed here. Byte proof: binding-flags-native-bytes-20260912.json.

Forge's FinalizeEntityBindings allocates a zeroed binding and does not set
unknown_zero (+0x18). Existing AddEntityBinding exposes only name/path. A faithful
port needs a supported way to pass these flags through registration. The Lua
reference now explicitly binds Messenger with 1 and Sister with 0; that is an
adapter contract, not an existing callable FSE signature.

The native emulator now also executes quest Main, including the actual binding
field stores. Total native/Lua comparisons: 15, all passing. The earlier numeric
column was removed because its ID interpretation was unsupported; the values
themselves were present and now have a proven Flags meaning. No runtime patch
or shared API manifest change has been made.

## Offline runtime implementation: optional binding flags

Added `Quest:AddEntityBinding(name, path, flags)` in the runtime checkout.
The third operand is `sol::optional<int>`; omitted/nil stays zero. Finalization
copies the value into binding+0x18 without changing the native structure layout.
Existing bindings keep their previous behavior; the Meet Sister adapter can now
request Messenger=1 and Sister=0 once its remaining resource operations exist.

Verified the runtime-selected vtable 0x12EA57C uses factory 0xE7ED60. Its actual
installed bytes load +0x18 at E7EDBB and call CE1110, whose actual bytes store
Flags at +0x3C. Six Unicorn executions preserve 0, 1, 2, 3, 0x80000000 and
0xFFFFFFFF through both functions, with balanced return stacks and unchanged
UID operands. Allocator, Init, string/task and refcount helpers are stubbed;
this is forwarding/constructor proof, not a scheduler lifecycle test.
Reproducer: `work/runtime-20260912-video-comparison/prove_binding_flags_factory.py`.
Evidence: `runtime-binding-factory-emulation-20260912.json`.

A standalone C++ harness extracts the actual changed method, binding-info
structure and sol registration, then compiles with this checkout's sol and
Lua sources. Omitted, explicit nil, 1 and 0 all pass. It substitutes only the
quest holder/logger; it does not invoke in-game FinalizeEntityBindings.
Reproducer: `work/runtime-20260912-video-comparison/check_binding_flags_lua.py`;
its generated directory includes the executable, source and result.log.
Release x86 build, control ABI audit, and 10 focused reference/observer tests
also pass. Validation hashes: `binding-flags-offline-validation-20260912.json`.

No shared API manifests or overlays regenerated: coordinate that metadata update
with the converter session before regeneration. This is an offline source/build
change, not a deployment or a complete Meet Sister port. Explicit resource
identity, single-attempt acquisition, and caller-owned map lifetimes remain
required. All 25 installed v23 manifest files still match; game remains closed.

## Offline explicit resources and staged Meet Sister package

Implemented `LuaRetailResources.h` in the runtime checkout and registered
`Quest:WithRetailResources(callback)`. The callback receives explicit, stable
resource/map/movie locals. `TryAcquire` makes one native attempt; Lua owns its
retry and termination decisions. `SetActor` copies even an empty resource.
`RunMacro` borrows the caller's map and movie, passes nil flags/input plus the
explicit setup/skippable operands, and leaves cleanup to the caller. `Frame`
calls the native frame operation without an additional termination query.
Existing AcquireControl and RunCutsceneWithSetup are unchanged.

Explicit ReleaseResource/DestroyActorMap/DestroyMovie preserve the native
normal-path order. Callback exit/error closes any remaining locals in reverse
creation order, unpauses if necessary, and invalidates the scope. Retaining the
Lua scope does not defer native release to GC; calls on a closed scope fail.
This is an error fallback, not an added native cancellation branch. The reviewed
port explicitly closes all locals on its normal and cancellation branches.
Mixing this scope with legacy cached control/movie helpers is not reviewed.

`reference/runtime_adapter.lua` bridges the native-reviewed reference to actual
runtime API names. A disabled package is staged at
`refs/script_recovery/reconstructed/MeetSister/FSE`: root and both entity entry
points, reference, adapter, empty legacy quests table and disabled retail
allocator override for QS_MeetSister. Neither game files nor quest activation
were changed. The reference remains canonical; package copies currently match.

Validation:
- Release x86 build passes (retail-resources-build.log).
- Actual C++ resource header and sol registration compile against vendor Lua;
  smoke checks single failed acquisition, empty actor-map copy, map ownership
  after local release, stable addresses through 1,000 allocations, invalid-kind
  and released handles, Lua error cleanup, and retained closed-scope rejection.
  Reproducer source: work/runtime-20260912-video-comparison/retail_resources_smoke.cpp;
  build/run: binding-flags-lua-check/resources.cmd in that directory;
  result: binding-flags-lua-check/resources-result.log.
- 15 native script scenarios match through the actual Lua adapter, and again
  through staged package Init/Main entry points. Engine/Quest calls are doubles.
- Native 8ABD10 assignment and 7E74D0 destruction were executed for five actual
  reference-count cases: empty, shared-info assignment, replacement of last old
  reference, local-before-map and map-before-local release. Actual arithmetic
  selects the deleter/free calls; those two engine operations are stubbed.
  Reproducer: prove_resource_refcounts.py in the same work directory; evidence
  native-resource-refcounts-20260912.json. This is not live actor destruction.
- 12 focused unit tests, existing control ABI audit, and all 122 New Oakvale
  package fixtures pass. Build/source/package hashes are recorded in
  resource-adapter-offline-validation-20260912.json.

Remaining: shared API metadata coordination before regeneration; engine/scheduler
and in-game lifecycle testing. The common LuaEntityHost Main entry has its existing
pre-entry termination guard; native-body replay exercises entered bodies and does
not prove scheduler behavior before entry. No global lifecycle guard was changed.
The installed build remains v23; New Oakvale Escape/fade/audio capture is still
pending the user's launch timing.

## Staged-package registration/API validation

Added `tools/script_recovery/validate_meet_sister_package.py`: all seven Lua files
compile, the staged native override is disabled, legacy registration is empty,
entity identities/files/IDs are consistent, reference copies match, and the
adapter's 12 Quest plus 12 RetailResources method names are present in the actual
runtime registration source. Result: staged-package-validation-20260912.json.
This is a binding-presence/structure check, not shared-metadata or playback proof.

The generic validate_reconstructed_package CLI was attempted first. It requires a
legacy quests.lua entry and returned an early report without summary, causing
KeyError('summary') when printing. Its all-non-Quest-receivers-are-entities model
also does not fit this explicit-resource adapter. It was left unchanged; do not
add a second legacy quest identity merely to satisfy that checker. Existing
New Oakvale validation still passes all 122 fixtures.

Final installed-file check after this work: all 25 v23 manifest entries match.
No runtime build or staged Meet Sister file was deployed. API coordination note:
work/meet-sister-runtime-api-coordination-20260912.md.

## Executed binding lifecycle branches

Twelve native existing-script activation/deactivation cases now pass for flags
0/1/2/3 and both deactivation booleans. Actual branch and FuncPaused writes execute;
lookup/save/reschedule/notifications are stubs. Evidence: native-binding-lifecycle-20260912.json.
Live streaming/scheduler validation remains pending.

## Actual C++ resource wrapper plus staged entity integration

Closed the gap between Lua-only replay and the standalone C++ wrapper smoke:
all 14 entity scenarios now execute the staged Init/Main files and runtime
adapter through vendor sol/Lua and the actual LuaRetailResources.h. Calls at
the engine boundary are instrumented doubles; Quest methods are doubles too.
The combined call trace matches all 255 archived native events, including
failed Hero acquisition, bound retries, pre-macro cancellation and termination
inside the macro. Every case ends unpaused, with no mock movie/map/resource
ownership or retained mock reference counts. The existing wrapper error-cleanup
smoke also passes again. No runtime/package implementation change was needed.

Reproduce from the repository root:
`python work/runtime-20260912-video-comparison/prepare_meet_sister_cpp_integration.py`
then run `meet-sister-integration.cmd` from that work directory's
`binding-flags-lua-check` subdirectory. Harness: meet_sister_cpp_integration.cpp;
log: binding-flags-lua-check/meet-sister-integration-result.log.
Evidence/hashes: cpp-staged-port-integration-20260912.json.
This strengthens interface and cleanup-order validation; it does not execute
native engine operations, the scheduler or live cutscene playback. Root binding
is outside this resource harness and retains its separate existing tests.
The package remains disabled and uninstalled; installed v23 is unchanged.
