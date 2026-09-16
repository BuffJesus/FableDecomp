# New Oakvale marathon — September 14

User explicitly corrected the early stop: **continue the marathon**, not just a
checkpoint. Work remains active. Do not stop after a passing narrow batch.

## Completed during the resumed marathon

- Four PostAttackStuff world adapters: returned-pointer lookup/action semantics,
  owned-output-before-key destruction, captured table pointer across lookup/Hero,
  fresh Hero and retained distance operand. `native_post_attack_world.py` executes
  original instructions including actual counted-Thing destruction: 240 combinations
  pass. Compiled common-owner/real-Lua harness passes 557 policies.
- `readable_post_attack_body.py` is wired into the builder after movie lowering.
  Emitted dispatcher passes 108 original control-flow comparisons with explicit
  phase boundaries. Existing movie caller tests still pass. One new negative test
  initially changed absent text; corrected to insert an actual extra frame, and the
  corrected test passes. Eight other tests in that original nine-test batch passed.
- All 64 missing resource methods used by emitted entities are composed into
  `work/oakvale_entity_integration`. Common generic conversation methods are reused;
  isolated vector-storage adapters are translated to monotonic IDs/map iteration.
  Text and StringMap releases erase entries. All registration templates compile.
  Actual composed owner passes 1,000 Lua allocation/release cycles, stale-handle,
  movie-exclusivity, reverse-close and closed-owner checks.
- Five missing quest methods staged at `work/oakvale_quest_integration`:
  StartConversationWithHero, AddConversationLineToHero, FaceThingByScriptName,
  SetStateFloat, GetStateFloat. Facing uses the returned alias while destroying the
  owned output. Float state passes IEEE bits through the existing namespaced integer
  store; delegated timer-write rejection stays active. Exact adapters/FSE-types/Lua
  thin-state harness passes 46 policies plus Lua special-value checks.
- Full isolated DLL builds succeeded for resource, quest, and lifecycle stages.
  Latest lifecycle DLL SHA256:
  `0f1cb4ab23147f896446e58e55f23c86a60c307eae32507dbdccb79d0d8b1243`.
  PE machine 0x014c. Build logs/hash manifests: `work/oakvale_runtime_build/`.
  Canonical runtime checkout and installed game remain unchanged.

## Latest validated checkpoint

- Full baseline suite completed: **1,511 tests passed**, exit 0; unittest 1490.447s,
  wrapper elapsed 1515.516s. Log/result: work/converter_marathon_suite_20260914_01.*.
  Modules added after suite discovery were validated separately.
- Lifecycle: native 90 cases; actual complete host/runtime 7 cases passed, including
  explicit scalar destructor return ABI (this pointer), flags 0/1/2/3, VM finalizers,
  timer construction failure and default compatibility.
- Latest full DLL includes Main registration over progress/mission/lifecycle: SHA256
  74f2b1faf6b586102299e8688453723fb5bbd32303d8e3d42a06e673ef5c5f87.
  work/oakvale_runtime_build/host-lifecycle-result.json records actual-host checks.
- Deed adapters: 36 objective policies; 28 live morality cases and 3 guard checks.
  Complete native deed helpers and emitted quest/shared shapes passed their tests.
  Builder now emits live morality, signed counter wrap, owned objective scopes.
- Mission: new retail_oakvale_mission.inc composes child transformation, house/start
  screen/music lifetime, captured-table hero killability and fresh active-name
  completion/deactivation scopes in work/oakvale_mission_integration.
  Native original scopes vs compiled actual owner/Lua: **72 traces + 48 exception
  policies passed**. Harness initially lacked StringMap destructor stub, then had
  undersized mock vtable; both corrected and clean rerun passed.
- Native DoMission dispatcher: **128 scenarios** pass (earlier commentary incorrectly
  said 256). Emitted final package: 48 additional dispatcher comparisons plus raw
  mutation rejection pass. Checks preserve native post-cancellation cleanup tail.
  Thread creation is a successful-registration boundary in these comparisons.
- Native allocator import at BFEA1A is MSVCR71 operator new; current Lua host uses
  malloc. Retail AddSpawnedFunction touches function+24 before its null check.
  Do not reproduce a null path as safe without proving allocator failure semantics.

## Latest additional progress

- Gold watcher had objective text incorrectly repeated as region1. Native passes two
  empty region strings. Corrected WatchForGotGold and AttackStuff: 282 native cases,
  compiled owner objective lifetime/alias/error harness 144 policies passed.
- Builder integration initially matched a function name inside a CreateThread
  comment. Anchored top-level function boundaries, then all 8 package regression
  tests passed. Emitted progress callers have 72 native comparisons.
- Main now structured after the verified 16 bindings/objective lowering. Four native
  tail comparisons pass; base and game binding-finalization calls are already both
  present in LuaQuestState::FinalizeEntityBindings.
- RegisterMain is host-owned in the ledger. Retail uses empty section, operator new,
  native spawned vtable and virtual Main thunk CDD440. Added opt-in fix in
  prepare_oakvale_main_registration.py over progress stage. Full DLL and cumulative
  git apply --check pass. Canonical checkout remains unchanged.
- Actual complete host tests: 20 Main registration allocation/string policies,
  4 executions through original virtual-Main thunk, 7 lifecycle cases all passed.
- API inventory: 281 required methods in 8 actual Lua usertypes checked in real host.
  tools/script_recovery/oakvale_api_inventory.py produces work/oakvale_api_inventory/.
  Includes colon calls and dot method references; local entity-state helpers are
  explicitly outside the native registration check. No unclassified receivers.
- Persistence: 12 original native OnPersist one-byte/default cases and 48 complete
  actual-host emitted OnPersist save/read/missing-default cases pass. Only AttackOver
  transfers; other sampled state and owned timer IDs stay intact.
- Persistent readable package regenerated: 51-function ledger, 18/18 syntax passes,
  zero scratch local names. Disabled Quests={} retained. RegisterMain/destructor
  ledger rows point to C++ host; this is not a whole-game completion claim.

## Integration checkpoint after the second full suite

- Second full suite completed: **1,528 tests pass**, exit code 0; log and result
  `work/converter_marathon_suite_20260914_02.*`. Native persistence and the expanded
  all-sixteen-factory ownership delta were added after discovery and pass separately.
- Current runtime stage: `work/oakvale_entity_ownership_integration`. Full Win32 DLL
  SHA256 `32fcdc8e48b9fcadee261e0e96b970a198557f8f464173aaba0328403498ac0c`.
  This supersedes the earlier Main-registration DLL listed above.
- Native CreateThread now uses the same owned-string/operator-new registration path
  as Main, rolls back slots and Lua anchors before engine handoff, and uses the native
  empty default region. Actual host: 20 policies, 4 dispatches, 4 exhaustion cases,
  12 Lua option cases passed.
- Quest and entity Main/frame query placement matches native explicit Lua queries:
  16 quest frame cases and 8 entity-specific frame cases passed.
- Parent shutdown closes child VMs before parent state, detaches retained child hosts,
  and rejects late work. Eight child finalizers passed with live parent state,
  reentrant removal and safe later entity callbacks.
- All sixteen native entity factories retain the Thing and initialize script reference
  count to one. 192 original native executions pass. Actual allocator host checks
  cover 192 success/null/throw/shared-release cases, including repeated early virtual
  destructor notifications without double release. Native helper instructions remain
  test oracles under work/, not reconstruction artifacts.
- Actual emitted Init runs twice per host (42 then 84 owned speech strings); all
  eight calls and populated teardown passed. 48 persistence cases passed.
- Prepared disabled review bundle `work/oakvale_marathon_bundle_20260914` using
  `prepare_oakvale_marathon_bundle.py`: checks canonical, staged, object, Lua and
  harness input hashes before copying. It contains the correct opt-in nativeLifetime
  setting; the older reconstructed playtest profile lacks this setting.

## Native scheduler teardown finding (active)

The 16 Windows fiber interleavings passed. Further native tracing found a material
ordering issue: CSpawnedFunc destructor CDD4C0 tails into A44620, which calls
CASuspendableProcess::TerminateProcess at A4B200. It sets byte+5 and repeatedly
resumes virtual+4 while byte+4 is set. Closing Lua before base destruction could
therefore resume a suspended callback into a closed VM.

Native CB7F60 already drains the spawned list at base+4 and process vector at base+8,
sets/restores the active process at base+2C, and clears these containers. Added an
opt-in call before CleanupThreads/VM close in `prepare_oakvale_quiescence.py`.
Original native traversal + TerminateProcess pass 64 combinations of list/vector
length and resume count. Entity Thing special branch remains a boundary here.

Current stage `work/oakvale_quiescence_integration` built successfully. DLL SHA256
`c0340571998fb050a2c46f41c71aac2ef0991fd3ce806a91db7e8424e7eca19e`.
Actual-host extension passes eight suspended Lua callbacks resumed by the original
TerminateProcess before anchor/VM/state cleanup, plus all previous host checks.
The fixture loader initially expected four helpers; corrected to five after adding
the native termination helper. Full rerun passed. No process remains pending.

Native quiescence matrix expanded to 192 cases, including entity Thing validity/kill
branches, shared reference preservation and an inert second drain. Virtual Thing
operations and container frees remain doubles; actual native traversal is executed.

Fresh disabled bundle: `work/oakvale_marathon_bundle_quiescence_20260914` (27 files).
All source, staged, object, Lua and actual-host evidence hashes verified. Earlier
bundle is superseded. Canonical runtime and installed game remain unchanged.

Continuing native restore callback ordering review. Real gameplay remains unverified.

## Activation/restore ordering and review preparation

- Original native activation tail 4B3FE8 calls CB7900 (virtual Init, then
  RegisterMain), then conditionally CB8690 LoadGameState. Four save/attachment
  combinations execute in `native_oakvale_restore_order.py` and pass. This proves
  the activation tail, not the complete manager/parser or live save behavior.
- Native manager deactivation at 4B3A77 already calls CB7F60 before releasing the
  script. The added host drain protects direct destruction and is inert after a
  prior engine drain; no observed live crash is claimed.
- Eight emitted Init calls now go through the actual host virtual Init callback and
  load the actual quest file, rather than invoking the Lua function directly. Full
  actual-host checks passed. The harness wrote a runtime log into the package while
  using that path; moved the log into work/, then isolated the full Lua fixture under
  work/oakvale_runtime_build/host-package/FSE. Fresh run is shell 93569; consume it.
- Four native delta test modules pass in work/oakvale_final_native_deltas.*:
  persistence, all-sixteen-factory ownership, process quiescence and activation order.
- Bundle writer rejects disagreements between evidence hashes and copies only
  validated Lua inputs, excluding runtime logs. Rebuild final review bundle after
  the isolated-fixture rerun. Earlier review bundles are superseded.
- Rollback snapshot prepared read-only in work/oakvale_marathon_rollback_20260914;
  its earlier preflight included a log path, so restrict any subsequent deployment
  to the final bundle's validated payload paths and recheck installed hashes.
- Asked user whether to temporarily install the candidate for a fresh disposable
  profile or continue offline. Approval is pending; do not infer it from elapsed time.
  Installed game files and saves remain untouched. Native/Lua byte fixtures are
  test oracles, excluded from the bundle, and not claimed as decomp coverage.

## Authoritative review checkpoint

- Isolated-fixture host run 93569 completed successfully. All actual-host checks pass;
  no test/build process remains pending. Runtime log now stays under work/.
- Final candidate: `work/oakvale_marathon_candidate_20260914` (30 hashed files;
  21 runtime payload paths). Override disabled, standalone Quests empty. DLL SHA256
  `c0340571998fb050a2c46f41c71aac2ef0991fd3ce806a91db7e8424e7eca19e`.
- Final rollback preflight: `work/oakvale_marathon_rollback_20260914/candidate-preflight.json`.
  All final payload hashes, original installed hashes and rollback copies verified.
  This supersedes the earlier preflight containing the harness log.
- Latest runtime patch: `work/oakvale_quiescence_integration/oakvale-quiescence.patch`;
  cumulative git apply --check passes. Canonical/runtime source hashes unchanged.
- Native activation order and host virtual Init/persistence are verified within the
  documented offline boundaries. Live playthrough, real save/reload and unload parity
  remain open. User choice about temporary installation/fresh disposable profile is
  pending. Do not activate until answered. No installed files or saves were changed.

Offline-only direction: user selected "Keep working offline only" and is at work,
unavailable for live verification. No activation, installation or save/profile changes.
Continue native/compiled offline integration; do not ask again about live testing.

## Offline continuation: entity callback policy

User explicitly selected offline-only and is at work; no live verification requests.
Native CActiveEntityScriptBase constructor CE1110 installs vtable 12C3594. Slots 5/6
are CE1090/CE10A0, which forward via process+34 to entity host slots 5/6. Four
original-byte null/present forwarding cases pass. Native Barrel predicate callback
DB7DB0 writes its state each invocation without the host compatibility dedup guard.

The current host infers predicate failure after Main returns and suppresses later
callbacks globally. Prepared opt-in native callback policy in
work/oakvale_entity_callbacks_integration: rely on engine predicate dispatch and
honor each explicit callback; retain legacy inference/dedup behavior. Expanded
actual-host regression is running against the earlier DLL in shell 41245 to verify
it detects the mismatch, before building the new stage. No runtime installed.

### Offline entity callback results

- Previous DLL fails the new native timing assertion after Main returns (line 345).
  Preserved regression evidence in work/oakvale_entity_callback_regression/before.*.
- Callback-policy DLL built with SHA256
  80ed2cd6c4eb814a583c5130db4ab20951f80e60d4394ec42a58536b63c74e3d.
  Actual host passes four native forwarding/timing scenarios plus legacy policy.
- Expanded harness passes 64 real entity VM file loads/default callbacks, 8 empty
  virtual Init calls, and 8 emitted Barrel predicate callbacks through native CE1090.
  Barrel flags and all three position components update on both invocations.
  Initial fixture compile needed an explicit cast to the runtime's declared Thing
  implementation pointer type; corrected, then the complete host suite passed.
- All sixteen native vtables have empty OnPersist and OnInterrupted. Only Barrel's
  predicate body has effects; DeadFather and the shared default are empty. Vtable
  tables and empty bodies are hash-pinned in native_oakvale_entity_callbacks_witness.
- Further scalar ABI check executes all sixteen original destructors with flags0?3
  (64 cases): native returns this and releases Thing references. Host previously
  declared void(bool). Prepared unsigned-flags/this-return correction while retaining
  its custom counted-pointer memory owner in work/oakvale_entity_scalar_abi_integration.
- Latest ABI DLL built SHA256
  545f709a46c288ff3a881a09a79cf846f34cf874e162684779e34bf546116e74.
  Cumulative patch applies cleanly. Expanded virtual return-value checks running in
  shell 57313. Consume result before freezing the new offline candidate.
- Six focused native tests pass: work/oakvale_offline_native_deltas.*. Full suite
  counts remain historical 1,511 and 1,528; these later native/host deltas are separate.

### Latest offline checkpoint

ABI host test57313 passed, including direct virtual destructor flags0?3 return-value
checks. All prior host tests remain green. Regression after-evidence is preserved
beside before-evidence in work/oakvale_entity_callback_regression/.

Current runtime stage: work/oakvale_entity_scalar_abi_integration.
Current disabled bundle: work/oakvale_offline_candidate_20260914, all30file hashes
verified, no runtime log payload. DLL545f709a46c288ff3a881a09a79cf846f34cf874e162684779e34bf546116e74.
No build/test processes pending. Live validation remains deliberately deferred;
user is at work and selected offline-only. Game, profiles and saves unchanged.

## Readability checkpoint and Guild focus

User requested Hero's Guild recovery while New Oakvale awaits live verification; all work remains offline. Replaced inline hexadecimal signed-wrap expressions in deed counting, father payment, bully speech cycling, and guard deed comparison with a readable counter branch or shared generator helper `wrapSignedInt32`. All 11 focused native/readability tests passed (37.688s). Persistent readable package regenerated: 51 functions, zero scratch names, 18/18 Lua syntax checks. The frozen work/oakvale_offline_candidate_20260914 bundle and its actual-host evidence predate this regeneration; they have not been refreshed or installed. No DLL changes in this readability batch.
