# Extended offline runtime marathon

Scope: actual cross-VM state/lifecycle checks, staged root binding audit,
native audio update execution, and the next retail port's foundation. Installed
v23 and converter-owned files remain untouched. No game launch or audio/device
setting change occurred.

## Confirmed VM cleanup use-after-free

`EntityScriptData` owned its `sol::state` after the environment and protected
function members. Reverse C++ member destruction closed the VM before those
registry references were released. Actual manager lifecycle methods, compiled
with vendor Lua/sol and AddressSanitizer, reproduce a heap-use-after-free in
all six scenarios: unregister, clear, shutdown, reinitialize, replacement
cleanup, and static manager destruction at process exit.

The source fix explicitly clears `luaMain` and `env` in the data object's
destructor while its VM still exists. Default move operations preserve the
existing reference-before-VM assignment order; construction remains explicit.
No native entity layout, scheduler hook or allocator ABI changed.

Before proof: `work/runtime-entity-vm-asan-repro-20260912/`. The original header
is preserved there. After proof is included in
`work/runtime-marathon-state-conversation-asan-20260912/`: all six pass under
ASan. The test uses the actual manager register/unregister/reset methods;
engine binding registration and movie abandonment are doubles. This proves
the C++/Lua ownership defect, not the cause of any particular live game crash.

## Cross-VM and reload state

Actual `GetNamespacedKey`, `Get/SetStateBool`, manager storage methods and bool
persistence transfer are compiled and called through sol. Quest constructors
and persistence storage are doubles. The package's Init/OnPersist/adapter run.

- An independently registered entity VM and a separate root coroutine see the
  same parent quest flag. A different quest's flag remains isolated.
- A new wrapper with the same script file name sees that named state until
  Init clears it. Simulated load then restores saved `PieceOver`.
- Manager reinitialization clears named state; persistence restores it.
  Absent saved data reads false. A stored integer is not accepted as a bool.

These are three passing contract groups under ASan. The actual allocator uses
the quest script file name as the host name, matching the namespacing model.
The experiment supplies Init/OnPersist ordering; it does not establish native
save-file scheduling or arbitrary hot-reload safety.

## Borrowed Thing conversation compatibility and conversion safety

The next port uses its bound NPC in an ambient conversation. The original
`StartAmbientConversation` binding accepted only shared-pointer userdata.
Owned participants passed, while a borrowed bound NPC produced a sol
argument-type error. Preserved proof:
`work/runtime-ambient-borrowed-proof-20260912/ambient.log`.

Changing the method/binding to accept actor objects exposed an additional
problem in the shared conversion helper: probing borrowed userdata as
`std::shared_ptr<CScriptThing>` caused an ASan heap-buffer-overflow. That failed
intermediate check remains in `work/runtime-ambient-borrowed-fixed-20260912/`;
it was not accepted as a validated fix.

The final helper uses sol's checked `CScriptThing*` conversion, which resolves
both borrowed and owned Thing userdata without probing shared-pointer storage.
The ambient method uses it for both participants, retaining its native create/
add-person calls, flag values/defaults and failure handling. Existing Lua call
shape is unchanged; no speech flags or volume behavior were changed.

Final ASan checks cover owned and borrowed participants in both directions,
all four flag combinations, omitted/nil defaults, a single optional flag,
invalid/nil participants, factory failure and missing factory API. Ownership
counts remain stable. The existing 27 conversation-line expressions plus their
nil/default checks also pass under ASan using the corrected shared helper.

## Other completed work

All three staged roots (Meet Sister, Maze Research, Guardian Trophy Dealer)
run through actual `AddEntityBinding` and its sol registration. The native
names, full Lua paths, ordering and flags match their reviewed contracts.
No further root-path defect was found. The earlier Guardian fix remains intact.

The complete native audio position-update function now passes 91 cases,
including entry gates, all voice gates, mixed/empty voice lists and geometry.
It reflects world-space sources behind the listener forward while preserving
distance. Default relative speech geometry remains 0.5 metres forward.
See `new_oakvale_intro/runtime_evidence/AUDIO_UPDATE_REVIEW_2026-09-12.md`.
No audible volume defect or affected live line is identified by this result.

Guardian Sister Info's reviewed root, persistence and entity lifecycle match
19 native executions through its non-registered reference/adapter. Its seed
incorrectly combines completion/deactivation logic into root Main; the native
root returns after the region/time step, and predicate-failure cleanup is
separate. The installed macro and 14 text keys resolve, with 19 relevant
placements. Two authored fan-exit markers remain unresolved. See
`guardian_sister_info/runtime_evidence/ROOT_REVIEW_2026-09-12.md`.

## Validation and continuation

Final checks and build use fresh directories:

- ASan lifecycle/state/ambient/line checks:
  `work/runtime-marathon-state-conversation-asan-20260912/result.json`.
- Clean existing runtime regression:
  `work/runtime-marathon-verified-regression-20260912/result.json`.
- Release build: `work/runtime-marathon-verified-release-20260912/build.log`.
- Final evidence manifest: `work/runtime-marathon-verified-20260912.json`.

```
python tools/script_recovery/run_runtime_state_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/NEW_STATE_CHECKS --asan --conversation-cases work/runtime-marathon-verified-regression-20260912/conversation-lines.lua
python tools/script_recovery/run_meet_sister_runtime_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/NEW_REGRESSION
```

The updated DLL remains uninstalled. The latest v23 deployment's 25 installed
files still match. Father Escape and Bully skip user acceptance remains intact;
quiet/distant dialogue and missing retail fades remain separate unresolved
observations. No converter files or shared API metadata were regenerated.

Next substantial offline port work is Guardian Sister's full MazeAtTavern body:
priority-3 acquisition, live-config call-over distance, explicit animation
dispatch, hit/talk resource transitions and post-macro completion order. Do not
promote its seed or invent missing marker definitions. Native scheduling,
streaming, save-file round trips and audiovisual behavior remain live gates.


Final outcome: all 18 manifest checks and all 22 Guardian package revalidation
checks passed. Candidate DLL SHA-256:
`c9eaf045ae7fc8e780e7f077b92be700c76cb149f717052821bd00e8625e4f30`.
The native audio and Guardian Sister replays were repeated into fresh files
and matched their original evidence exactly. Eleven focused Python tests also
passed. The updated source runtime remains uninstalled.
