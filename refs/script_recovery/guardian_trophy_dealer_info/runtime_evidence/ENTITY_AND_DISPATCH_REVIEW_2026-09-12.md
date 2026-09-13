# Guardian complete entity body and explicit speech dispatch

Guardian's non-registered reference now contains Init and the complete Main
control flow. Twenty-eight executions of the original native Init/Main match
1,377 reference events. Existing 49 encounter/repeat and ten watcher cases still
pass, giving 87 native/reference cases. No FSE package is registered or installed.

## Entry and loops

Init at E27C60 reads the bound Thing's GetAngleXY and stores the float at entity
+1C. Main E27E90 registers a condition containing a copy of the bound Thing,
destroys its temporary condition, then yields one frame before querying
termination. A first-frame cancellation exits before allocating an actor resource.

The normal entry sets information flags false/true/false; constructs, prepares
and acquires Maze at priority 4; checks termination; sets killable=false with
the third native argument true; sets damageable=false and pushable=false; starts
the active quest's screen with false/false; and adds HUD_ORB_QUEST_CORE. Another
termination query precedes the first trigger loop. Failed acquisition uses
frame/query/retry; cancellation destroys the local resource even if empty.

The trigger evaluates MsgIsHitBy(SCRIPT_NAME_HERO), short-circuiting on success.
Otherwise it evaluates any special-ability hit and excludes ability 14 (Heal
Life). All native strings and the numeric ability argument are asserted by the
replay. A non-hit then checks use/talk. The first loop chooses hit before talk.
After completion, the repeat loop instead checks use even after a hit response,
so both repeat responses can execute in one iteration. It yields and queries
termination after that combined iteration. No extra frames or checks were added.

The full-body replay starts at the actual function entries, executes the native
prologue/epilogue and returns through a sentinel. It covers all first-entry
cancellation gates, failed acquisition/retry, idle loops, melee/special/heal/use
selection, simultaneous hit/use, changing repeat health, every macro cancellation
and repeated post-meeting query boundaries. APIs and resource operations remain
doubles. The previously independent body replays remain separate regression tests.

## Alive condition and scheduler boundary

The condition vtable is 12C32F0. Its evaluator CDF120 forwards to its copied
Thing's +12C IsAlive. Registration F35B10 -> CB7920 -> F35A10 clones the condition
through vtable +4 and assigns the active task's counted predicate at +2C.

Five additional executions run CB7950, F35A00 and CDF120 with synthetic task
state. Alive before/after resume continues. Dead before resume skips it; death
after resume fails the second check. Both failure paths set task+5, clear the
condition, invoke task vtable +14 predicate failure, then clean up the fiber.
Paused tasks do not evaluate/resume; a task with no condition resumes without
an alive check. Resume, IsAlive, condition destruction and fiber cleanup are
doubles; actual fiber switching and registration allocation are not executed.

The reference exposes register_alive_condition as a required adapter operation.
It must not become a no-op. Current RetailResources.Frame yields through the
native interface and IsActiveThreadTerminating reads native task state. Neither
alone installs this predicate. LuaEntityHost's post-return predicate-failure
fallback does not prove that the required pre/post-resume condition exists.
Prove the actual adapter registration path before staging Guardian. No broad
scheduler or existing-port behavior change was made based on this finding.

## Implemented runtime operations

LuaRetailResources.h now exposes:

- Speak(resourceId, target, key, selection, listen, sound2D, overFade).
- IsPerformingScriptTask(resourceId).

These dispatch through the chosen local resource's pImp.Data to expert vtable
+34 / +68, matching native wrappers 7E7390 / 7E7450. They do not use the cached
LuaEntityAPI control handle, acquire another resource, start a movie, clear
commands or perform an implicit wait. Live empty resources no-op for speech
and return false for task status. Invalid/released/wrong-kind/closed identifiers
are rejected by the existing scope checks. Missing expert function pointers
raise a runtime error. All flags are explicit.

Ten native wrapper executions verify empty/populated handles, receiver identity,
all six forwarded Speak arguments, null targets, selections 0/1/2, task false/true
and stack cleanup. The third Speak argument is a text-selection enum; the older
encounter review's description as wait-time was corrected. Guardian still uses
selection 0 and false/true/false speech flags; no audio behavior was altered.

The actual-header sol/C++ smoke test uses two distinct expert objects, verifies
each receiver and all arguments, and covers empty/released/wrong-kind/closed
resources. Three speech and two task calls reach the expected experts; empty
and rejected operations do not. All seven runtime binaries pass, including the
existing 14-case/255-event Meet Sister integration, Maze state/persistence and
New Oakvale speech/conversation operand checks. The Release build passes.

Proof: work/runtime-guardian-dispatch-20260912/result.json.
Build: work/runtime-guardian-dispatch-release-20260912/build.log.
Uninstalled source DLL SHA-256:
`58fcea9de937cc1f1302d5f1d6331105aef397116c013daa55c64b9013614c20`.
Both installed DLLs remain exact v23:
`c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31`.

## Remaining work

Explicit resource Thing copies/destruction and alive-condition registration
still need runtime support/proof. Then build the actual Guardian adapter and
compare its complete event traces through C++/sol. Resolve the archived quest-card
decode error/component references and check override identity before staging a
disabled package. Shared API metadata was not regenerated; the two new methods
have a separate unapplied proposal. Converter-owned files and installed assets
were untouched. Rendering, audible output, streaming and real save/load remain
live acceptance boundaries.

```powershell
python work/runtime-20260912-video-comparison/prove_guardian_entity.py
python work/runtime-20260912-video-comparison/prove_guardian_resource_dispatch.py
python work/runtime-20260912-video-comparison/prove_guardian_alive_condition.py
python -m unittest tools.script_recovery.test_guardian_entity tools.script_recovery.test_guardian_encounter tools.script_recovery.test_guardian_repeat tools.script_recovery.test_guardian_trophy_waiter tools.script_recovery.test_guardian_resource_lifetimes -v
python tools/script_recovery/run_meet_sister_runtime_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/guardian-runtime-NEW
```

The complete replay was derived from the earlier encounter harness using the
archived build_guardian_entity_replay.py; run the generated replay directly for
validation. Prior encounter-validation hashes are historical snapshots, now
superseded by entity-dispatch-validation-20260912.json.
