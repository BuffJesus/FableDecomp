# Legacy cutscene helper cleanup review

The normal actor/flag/input map cleanup paths use distinct native destructors.
A defective fallback in RunCutsceneWithSetup could use the actor-map destructor
for a boolean map when the boolean destructor pointer was unavailable. The
source now rejects a supplied setup table before allocation if either required
setup-map destructor is missing, and always uses the boolean destructor for
boolean maps. This also prevents calling a missing string-map destructor.

Both destructor pointers are initialized in the normal installed v23 build.
This missing-API defect is not established as the cause of any live report.
No installed files were changed.

## Native evidence

`prove_cutscene_map_cleanup.py` executes 12 cases using installed executable
bytes. Both CDBF70 and 9AC2D0 initialize the same eight-byte map header and the
same sentinel links at +4/+8/+C. The former allocates a 0x24-byte actor sentinel;
the latter a 0x18-byte sentinel. A larger sentinel alone is compatible with
the boolean header/cleanup contract and is not a proven defect.

Six cases use the boolean destructor 7ADD50. Empty maps free the sentinel;
one-node trees destroy their key and free node/sentinel for both false and true
values. The boolean node value is at +0x14. Four populated-tree cases using
the actor destructor CDBFB0 instead reach CD27B4, which dispatches 7E74D0 on
node+0x14 as though it were a scripted resource. The replay stops there before
allowing invalid resource accesses. Two empty-tree fallback cases are harmless,
which is why testing only an empty map would miss the defect.

Node trees are supplied explicitly. Allocation/free and string destruction
are doubles. This verifies destructor traversal/dispatch, not the game heap or
full parser execution. Evidence: native-map-cleanup-contract-20260912.json.

## Ownership paths reviewed

| Path | Observed helper cleanup |
| --- | --- |
| Pre-entry termination or unavailable required APIs | Returns before allocation |
| Actor acquisition cancellation | Destroys current local, prior local copies, then actor map and map buffer |
| Normal caller-owned movie | Leaves movie ownership with caller; destroys temporary actor copies, flags, input map, actor map |
| Normal helper-owned movie | Unfixes if requested, unpauses and destroys movie, then the same map/local cleanup |
| Boolean-map cleanup | Now requires and calls 7ADD50; no actor-destructor fallback |
| String-map cleanup | Requires 9AC310 before entering setup; then destroys and frees its buffer |

Actor entries are reference-counted copies. Destroying a local copy before its
map copy does not by itself establish premature destruction of the underlying
resource. This review does not assert that the legacy helper matches each
retail caller's complete map/resource lifetime or acquisition order. The
explicit RetailResources API exists for ports requiring those native locals.

The helper's raw allocations still lack a general C++ exception unwind guard.
Malformed Lua conversions or allocation exceptions are outside the reviewed
normal/cancellation paths; this audit does not certify exception safety.
Changing all ownership machinery is separate work requiring actual-code
integration tests, not a justification for speculative live behavior changes.

## Validation

```powershell
python work/runtime-20260912-video-comparison/prove_cutscene_map_cleanup.py
python tools/script_recovery/run_meet_sister_runtime_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/runtime-map-cleanup-NEW
```

Native replay: 12 cases. Clean runtime results are in
work/runtime-map-cleanup-20260912/result.json; the six existing C++/Lua binaries
cover the current runtime source snapshot with engine doubles. Release build
command/log are in work/runtime-map-cleanup-release-20260912/. No speech,
movement, skip or rendering policy changed.
