# Guardian Thing ownership, native condition and adapter integration

The actual Guardian reference and runtime adapter now match all 28 native
Init/Main scenarios (1,377 events) through the compiled RetailResources layer.
Nine runtime binaries and the Release build pass. This closes the entity-body
adapter gate; root/watcher/persistence adapter integration and disabled-package
assembly remain. Nothing was installed or activated.

## Explicit Thing locals

RetailResources now manages Thing entries alongside resources, maps and movies:

- NewThingFromResource(resource): invoke that expert's GetScriptThing (+30), or
  construct an empty Thing for an empty live resource.
- NewThingFromScriptName(name): use the native hidden-result lookup ABI.
- ThingHealth, ThingAlive and SetTeleporterActive: operate on the selected
  owned Thing without returning a raw pointer to Lua.
- DestroyThing: run the native non-deleting destructor at the explicit boundary.

Scope cleanup also releases remaining Thing entries; released, wrong-kind and
closed handles are rejected. A Thing copy may outlive the resource that produced
it. No cached actor control or garbage-collection timing substitutes for this
local ownership. Existing shared-pointer helper behavior was not changed.

Sixteen native executions cover populated/empty resource getters, one/two output
copies, initial reference counts one/two, and source-first/source-last destruction.
The resource getter 7E7490, copy constructor 4ABE90 and destructor 4AA840 execute.
Expert GetScriptThing tails into the actual copy constructor. Base construction,
base destruction, final object deletion and allocator free callbacks are doubles.
All copies have the expected vtable/Data/Info; all destroyed wrappers clear
Data/Info. Final reference destruction deletes the object and frees Info exactly
once. Eight cases reach that final deletion; the others retain an external owner.

## Native alive-condition registration

Quest:RegisterBoundAliveCondition is an entity-VM binding. It obtains the captured
host and its native parent, then calls RegisterRetailAliveCondition. The helper
checks for a current task at parent+2C before constructing a temporary 16-byte
condition: vtable 12C32F0 followed by a counted Thing copy. It calls native F35B10,
which clones the condition into the active task. The helper then destroys only
its temporary Thing copy. The task owns the clone and its subsequent lifetime.

The +2C check follows CB7920/CB7940, not the older CScriptBase_Retail PActiveThread
member at +38. No shared struct layout or scheduler hook was changed. The
operation is explicit in Guardian's Main before its first frame. Existing
scripts do not gain an implicit condition from this change.

Eight native registration executions cover one/two registrations, source-first
or condition-first teardown, and alive false/true. Actual parent forwarding,
F35B10/CB7920/F35A10/F35AA0, CDF130 cloning and counted destructors execute.
Replacing the condition releases the old clone. Releasing the source before
evaluation leaves the new clone valid. All allocations/free calls balance and
the underlying Thing is deleted exactly once. Allocation/free, base teardown
and IsAlive implementations are doubles. The previous five scheduler cases
already establish how this condition gates pre/post-resume execution; real
fiber switching remains outside these tests.

The C++ test extracts the actual LuaManager binding and compiles the actual
condition helper. It checks successful registration/replacement, absent current
thread rejection, and temporary-copy cleanup on a synthetic C++ registration
exception. It also exercises the actual sol Thing methods, lookup, retained copies
after resource destruction, missing lookup, and stale-handle rejection.

## Actual Guardian adapter

reference/runtime_adapter.lua routes all resource, map, movie, speech, task,
Thing and lookup operations through RetailResources. It registers the native
alive condition and uses the bound instance. Hero getters remain at their
individual call sites; there is no cached replacement for directed ally calls
or acquisition retries. Talk uses IsTalkedToByHero (native Thing slot +6C), and
the Heal Life predicate uses ability 14. All movie and speech flags are explicit.

guardian_cpp_integration.cpp loads the actual reference/adapter, actual resource
header and actual condition binding. Native API and ordinary Quest/actor methods
are doubles. Expected events come from the independent native full-body replay.
All 28 scenarios match all 1,377 events, including cleanup order, health-driven
speech, combined repeat-hit/talk iterations, completion state and cancellation.
Resources, maps, movie state and temporary Things are empty at callback exit;
the condition clone remains until the synthetic scheduler cleanup, then its
reference count returns to the original owner. Ordinary Quest wrapper internals,
real macro playback and real scheduler execution are not reproduced by this test.

The other eight binaries pass, preserving Meet Sister's 14 cases/255 events,
Maze state/persistence checks, and New Oakvale speech/conversation operands.
The initial wider build exposed a duplicate test-only g_pCScriptThingVTable
definition after the shared fixture gained that symbol. The fixture was
consolidated; the clean nine-binary run passes. No runtime defect was attributed
to this harness failure.

## Quest-card component verification

The existing installed OBJECT_QUEST_CARD_FIND_TROPHY_DEALER is global entry 3736.
Its documented component tag at byte 77 matches CQuestCardDef; index 12321 at
byte 81 resolves to that definition, and byte 85 points back to OBJECT 3736.
All 18 card fields decode with zero leftover bytes. Title/summary IDs resolve
to "Find the Archaeologist" / "Find the Archaeologist missing in Witchwood."
Its category reference resolves to INVENTORY_CATEGORY_QUEST_CARD_BLANK and its
region is Witchwood2. The audit archives payloads and source hashes.

This resolves the required card-component presence/link check. The generic
OBJECT decoder still cannot decode its entire payload: it supplies 29 bytes
for the int32 DieOffTimer field. No generic parser change or asset rewrite was
made. Unrelated OBJECT fields and actual in-game card display are not certified.
The documented, tag-checked component links avoid relying on that failed decode.

## Validation and next work

Native reproducers in work/runtime-20260912-video-comparison:
prove_guardian_thing_lifetime.py and prove_guardian_condition_registration.py.
Evidence: native-thing-lifetime-20260912.json and
native-condition-registration-20260912.json. Card repro:

```powershell
python -m tools.script_recovery.audit_guardian_quest_card --output work/guardian-card-NEW.json
python tools/script_recovery/run_meet_sister_runtime_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/guardian-adapter-NEW
```

The runner requires a fresh directory and hashes the actual Guardian reference,
adapter, evidence, C++ sources, runtime headers and extracted-binding source.
Clean result: work/runtime-guardian-adapter-20260912/result.json.
Release: work/runtime-guardian-adapter-release-20260912/build.log.
Current source DLL, installed DLL and API-source hashes are in
thing-condition-adapter-validation-20260912.json. Shared metadata changes are
an unapplied proposal only. Converter-owned files were not edited.

Next: exercise the root, watcher and persistent PieceOver through the actual
adapter; check final override identity/conflicts; then assemble a disabled
package and validate its entry files. Corpus registry evidence remains quest
QS_GuardianTrophyDealerInfo / entity GTDI_Maze ID 69. This is seed registry
evidence, not a native numeric ID recovered from a vtable.

Installed v23 remains unchanged. User acceptance of restored Father Escape and
Bully's retail-matching skip behavior remains recorded. Missing retail fade
rendering is unresolved; quiet/distant speech remains a separate report. None
of this batch is claimed to fix or verify audible output or fade pixels.
