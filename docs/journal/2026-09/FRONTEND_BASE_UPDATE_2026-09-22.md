# Complete base component update recovery — 2026-09-22

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
This continues [the child-pass recovery](FRONTEND_CHILD_UPDATE_2026-09-22.md).
`CComponent::Update` (00531EC0) now has a complete readable C++ implementation:
local time and transform/colour updates, position children, live children and
deletion requests, then retiring children and completion. The presenter is
unchanged in this increment. No game or GUI was launched: the screen was reserved
for another agent.

## Recovered functions

| Retail address | Function | Acceptance |
| --- | --- | --- |
| 0042ABCA | DestroyDeletionParents | Functional |
| 0042CD84 | CopyDeletion | Functional |
| 0052E850 | CComponent::GetDeletion | MATCH, 7 bytes |
| 00531E90 | CComponent::SetDeletion | Functional |
| 00531EC0 | CComponent::Update | Functional |
| 00535800 | AssignDeletionParents | Functional |
| 005354E0 | EraseCountedChild | Functional |
| 005359D0 | ReallocateCountedChildren | Functional |
| 00533BC0 | CComponent::RemoveChildAt | Functional |
| 005303F0 | CComponent::RemoveObserverRecursive | Functional |
| 00530720 | CComponent::Die | Functional |

The destructor replaces a misnamed `CActiveFile::OnReadFinished` forwarding
wrapper. Its previous relocation-match label does not apply to this complete
readable implementation. `Die` replaces naked assembly; the old implementation
is retained under `src/asm_bake` as evidence, but removed from the active bake
backlog. Manifest overrides and the existing candidate entries now reference the
correct source/test names. Historical parity reports and coverage were not
rewritten to imply a new global audit.

## Behavior preserved

Deletion requests copy their associated-parent list before inspecting the method,
even for method zero. Method three removes this parent from the copy, not from
the source list; when other parents remain, the modified copy is not assigned
back. SetDeletion consumes and destroys its by-value list argument. Tests compare
node reuse, list links and allocation/free order as well as final contents.

RemoveChildAt searches for the first matching object, calls Die, appends a counted
entry to the retiring collection and erases the live entry. Shape indices are
updated using the requested index, even when duplicate objects cause an earlier
entry to be erased. Counted-vector growth preserves reference operations and the
retail capacity rule. Equal reference-control blocks skip assignment, including
two null blocks. The outer update loops still advance after erasure, preserving
retail's skipped shifted successor.

Die requests state two before recursive observer removal. Recursion obtains the
manager separately for every visited component, passes the secondary interface
at component +4, invokes the component callback at vtable +214 and traverses only
live children. It reloads collection state after callbacks. The callback's
`ObserverRemoved` name describes the observed call site; concrete overrides are
not recovered by this work.

## Offline validation

All tests compile readable sources with VC7.1 and compare against retail x86
execution where indicated. Retail SHA256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

| Gate | Cases/frames | Scope |
| --- | ---: | --- |
| `check_ui_deletion.py` | 4,500 | Lists, setter/getter and live deletion policy; getter byte match |
| `check_retiring_children.py` | 4,040 | Retirement, counted helpers, erase/growth and RemoveChildAt |
| `check_position_children.py --frame` | 9,504 | Complete native base Update from entry through return |
| `check_ui_observer.py` | 80 | Die and recursive cleanup, exact callback traces |
| `check_state_lifecycle.py` | 480 | Connected state/colour lifecycle using the real rebuilt base Update |
| `check_state_lifecycle.py --motion` | 480 | Connected movement/zoom/colour lifecycle |
| `check_position_children.py` | 1,952 | Position-child tree propagation |
| `check_position_children.py --live` | 3,360 | Live-child preparation/update prefix |
| `check_position_children.py --retiring` | 3,360 | Retiring-child preparation/update prefix |

Whole-frame cases cover live-to-retiring transfers and completion/erasure, all
four deletion methods, parent and position-parent combinations, and callback
mutation. They execute real removal, list and counted-vector helpers. Root local
updates and concrete child callbacks, including Die, are controlled boundaries;
Die's implementation has its separate gate. The lifecycle gates connect real
position/zoom/colour updates with empty child collections. Neither gate alone
represents a fully constructed frontend.

Numeric propagation uses the existing `1e-4 + abs(expected)*2e-6` tolerance for
VC7.1/x87 reassociation. Callback order, pointer identities, collection results,
allocation/free order and time bits are exact in the full-frame comparison.
Allocator, object destruction and CRT memmove are controlled services in the
ownership gate. Allocation failure and malformed containers are not established
by these tests. Observer tests double manager lookup/unregistration and state
request. Four repaired candidate tests also compile and link the actual source
objects, covering Die, the list destructor and the two coordinate getters.

Bootstrap includes the new gates. Component draw and combined frontend animation
remain regression checks; these do not constitute visual verification.

## Next work

Recover concrete manager lookup and observer unregistration, then frontend
construction/action dependencies and persistent rendering integration. The
allocator/manager service declarations currently have fixture bindings and need
engine bindings when integrated. The flat presenter does not yet use this full
ownership implementation. Visual uncanniness and native rendering order remain
open; this increment makes no new visual-fidelity claim.
