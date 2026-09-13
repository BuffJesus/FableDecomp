# Original FSE distribution constraint

## Current compatibility status

The isolated unchanged-original-FSE plus independent-add-on design has now run
through the entire childhood locally. User reported retail-like behavior except
quiet cutscene speech. This is a locally compiled unmodified upstream DLL, not a
verified maintainer release binary. Save/reload and other custom quests remain
open; no distribution. See [overnight handoff](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md)
for exact binaries, captures and checklist. Earlier feasibility-only conclusions
below predate this live result.


User wants a tester distribution for eeeeeAeoN/FableScriptExtender with its DLL
completely unchanged. Do not ship our DLL as an alleged compatible package.
Original retail accuracy remains the target; reduced behavior was not authorized.

Read-only upstream checkout D:/Code/FableScriptExtender-upstream-latest is clean
and HEAD b2432e82b2ef1e5fd50b8fac2a8809bd2bd107d6 matches remote HEAD/master queried
on September 12. No game installation, converter files or upstream files changed.

Reproducible registration audit:

    python tools/script_recovery/audit_new_oakvale_upstream.py --upstream D:/Code/FableScriptExtender-upstream-latest --output work/NEW_UPSTREAM_AUDIT.json

Initial result work/new-oakvale-original-fse-20260912/api-audit.json pins the
requirements and registration source hashes. Eight reviewed call names absent:

- Quest: RetailRandModulo
- Entity: IsDistanceFromPositionOver
- Quest: AddNewConversation, AddPersonToConversation
- Quest: IsObjectInThingsPossession, IsDistanceBetweenThingsOver
- Quest: OverrideAutomaticHouseLocking, OpenHouseDoors

This is a registration inventory, not proof all eight are impossible to adapt.
Distance operations are candidates for Lua wrappers after vector/edge semantics
are checked. Combined conversation helpers require an ordering/lifetime review.
Replacing RetailRandModulo with math.random would change retail RNG consumption;
it is not an equivalent implementation. No reviewed stock-Lua path to the native
RNG has been established. House/inventory alternatives remain to be investigated.

Confirmed existing-name differences:

- LuaEntityAPI.cpp:39 AcquireControl returns true immediately for an existing
  handle, ignoring requested nesting/priority; the tested port requires different
  priorities to create a nested level and outermost-only native release.
- LuaQuestState.h:39 RunCutsceneWithSetup returns void. cpp:1666 onward constructs
  another local movie object, pauses entities, runs macro, then releases it. Our
  port relies on the tested ownership-aware implementation and skip return.
- LuaQuestState.cpp:2202 PersistTransferBool has three parameters and uses current
  value as native default; the port explicitly supplies default false separately.
- dllmain.cpp:163 onward reads quests.lua; there is no retail_override.lua loader
  in this upstream source. Our tested profile has an empty legacy registry and
  uses the identity-preserving override. Copying that profile to stock does not
  register the port. Merely adding a second quest risks duplicate native execution;
  original-FSE registration/identity replacement needs independent proof.

Do not incorrectly claim upstream lacks all our APIs: it already exposes control
acquisition, RunCutsceneWithSetup, persistence, frame return values, and dispatches
OnPredicateFail. The problem includes semantics, not just names.

Current result: tested v23 payload is NOT compatible with unchanged original FSE.
No playable stock-compatible archive has been produced or claimed validated.
Next work is a separate Lua-only compatibility feasibility pass: audit equivalent
stock operations and registration replacement; exercise actual upstream binding
semantics in harnesses. If exact native operations remain unavailable, the combined
unchanged-DLL and retail-parity requirements cannot be met by plain Lua alone.
Do not conceal that with no-op fallbacks or promote a partial prologue as the port.

Separate audio investigation remains open: Father voice is quiet on camera shots
facing Father through headphones. New backend observer is prepared but has not
captured a live Father run. This request did not resolve or cancel that issue.
