# Meet Sister scheduler-entry review

The bounded entry audit found no evidence supporting a change to the common
LuaEntityHost pre-entry guard. This does not establish live fiber/streaming parity.

## Creation and activation

Native quest Main E28DE0 constructs bindings using vtable 12E2214. Its factory
slot +4 resolves to E29110: allocate the entity, invoke its Init slot +8, construct
the active wrapper with CE1110, then assign wrapper vtable 12E2268. The Forge
selected factory E7ED60 follows this pattern with wrapper vtable 12EA604.
The binding's +18 flags reach active+3C. Messenger uses 1; Sister uses 0.
The already archived factory execution tests cover this forwarding and Init order.

Existing-instance activation CB8960 and deactivation CB88B0 are covered by the
12 native lifecycle cases. Flag bit 0 preserves the paused script on the relevant
deactivation path; activation resumes it. Otherwise activation invokes Init and
reschedules. Save, lookup and rescheduling calls were doubles in that proof.

## Entry and termination

CB7959 assigns the scheduled active object to parent+2C before the resume path.
CB795C checks active+28 (paused). Further predicate/interruption branches are in
CB7969-CB79C6. The normal resume call is at CB7997.

The coroutine entry A446A0 checks active+5 at A446D1. A nonzero byte bypasses
active vtable slot +10 and reaches the terminal yield loop at A44726. Otherwise
it sets active+4 running and calls slot +10 at A446EF.

Meet Sister's wrapper E29220 and Forge's E7EE70 both call entity Main through
active+34, entity vtable slot +4, then set active+5 to 1 when Main returns. The
wrapper itself has no extra termination filter; that filter is in the entry
trampoline. Therefore directly calling the wrapper on an already terminating
synthetic object would not model the normal entry path.

LuaEntityHost.cpp adds a query before dispatching Lua Main. F35B30 obtains the
entity's parent through vtable +C, then CB7940 reads parent+2C and byte +5. In a
normal first entry with the current-task association above, it reads the same
non-terminating task that the native coroutine gate just admitted.

## Executed checks and boundary

`native-scheduler-entry-20260912.json` contains four executed cases: retail/Forge
wrappers crossed with initially terminating/not terminating. The actual native
trampoline and wrappers run. Terminating cases never invoke entity Main; normal
cases invoke it once, observe false through the actual termination helper,
then finish and reach the terminal yield. Reproducer:
`work/runtime-20260912-video-comparison/prove_meet_sister_scheduler_entry.py`.

The parent/current-task association is synthetic, modelling the native CB7959
store. Entity Main is a double whose only operation is the native termination
query. Actual fiber switching, thread interleaving, predicate transitions and
region scheduling are not executed. A concurrent change between the gate and
Main is not excluded by these tests. No runtime guard was removed or bypassed.

Together with the 14 entered-body C++/Lua scenarios, the factory tests and the
12 existing-script activation tests, this closes the offline entry-contract
audit. Live scheduler/streaming acceptance remains outstanding.
