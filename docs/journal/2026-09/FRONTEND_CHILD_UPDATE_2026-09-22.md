# Base component child-update recovery — 2026-09-22

Continuation: the remaining base Update, deletion-list and removal work described
as open below is now recovered. See [complete base update evidence](FRONTEND_BASE_UPDATE_2026-09-22.md).
This entry preserves the scope of the earlier partial recovery.

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
This continuation extracts substantial portions of `CComponent::Update`
00531EC0 into readable, shared C++ helpers. It does **not** mark the full method
recovered or change the presenter. Visual verification was authorized, but no
new visual change required a launch.

## Implemented and checked

`fable_ui_position_children.h` contains the position-child tree pass
00531EFC..005321E0 and the shared transform propagation used in all three passes.
It traverses the retail tree links, reloads children after callbacks, preserves
the direct local-position pointer where retail passes it, and recomputes
inherited positions from local position/parent transforms. It does not simply
copy RenderPosition. Position independence is queried once; zoom independence
is queried separately for normal and relative coordinates.

`fable_ui_child_update.h` reconstructs the live-child preparation/update prefix
00532200..005325AB and corresponding retiring-child prefix
005327C0..00532C0D. They adopt unparented children, consider position parents
separately from ordinary parents, and independently decide transform inheritance
and colour/frame update. A foreign child can receive position transforms without
receiving this parent's colour or frame update. Each accessor reloads collection
storage and the selected child after callback boundaries.

`fable_ui_counted_children.h` reconstructs the retiring-child completion/removal
tail 00532C0D..00532CD9, with two recovered complete helper functions:

- 00534EB0 searches counted entries by object pointer, returning the first match.
- 00535000 shifts entries using reference-counted assignment and returns the
  destination end. Equal control blocks skip the entire assignment, including
  when both are null. Different blocks release the old reference before copying
  the incoming object/control block and incrementing its count.

Retirement requires completion and current state 2. The selected entry's object
is searched again from the beginning, so duplicate entries remove the first
match. A zero deletion method requests state 0 before clearing the parent.
The tail shifts entries, decrements End, releases the trailing reference, and
zeros both trailing words. The outer retail loop still increments its index
after removal, skipping the shifted successor; preserve this when recovering
the full loop.

The existing `FableReferenceCount` definition was moved from `fable_boot.h` to
a small shared header and reused for child entries. No second incompatible
reference-count layout was introduced. Concrete deletion fields and vtable
slots replace previously unnamed ranges; static offset assertions guard them.
The PDB independently names PositionChildren, PPositionParent, Deletion,
CDeletion::Method and AssociatedParents; retail offsets differ from donor STL
layouts and are established by retail instructions.

## Validation

Bootstrap now runs all four new gate modes:

| Command suffix | Cases | Oracle scope |
| --- | ---: | --- |
| `check_position_children.py` | 1,952 | Real base Update through the position-child pass |
| `check_position_children.py --live` | 3,360 | Live child prefix, stopping before deletion processing |
| `check_position_children.py --retiring` | 3,360 | Retiring child prefix, stopping before completion/removal |
| `check_retiring_children.py` | 2,760 | 2,620 ownership-tail cases plus 140 direct helper cases |

Propagation tests include empty, balanced and skewed trees, parent/position-parent
combinations, all independence combinations, changing zoom query results,
relative coordinates, callbacks replacing child entries, and signed zero.
Callback order, receivers and direct-position pointer identity match exactly.
Vectors use the established tolerance 0.0001 + abs(retail)*0.000002; the largest
absolute difference is 0.0625 on large products. Ten position-tree samples differ
in low float bits because the previously recovered coordinate converter is
reassociated by VC7.1; these remain within the accepted functional tolerance.
No stricter byte-identity claim is made.

Ownership comparisons are exact for callback/destructor/delete traces, all
entry words, parent links, returned offsets and reference counts. Cases include
missing/empty searches, self/overlapping moves, duplicate objects, shared/null
control blocks and external owners. State/destructor/deallocation callbacks are
controlled boundaries. The oracle enters the real base Update prologue and
bypasses the already separately checked child prefix when testing the tail.
Production callback mutation during destruction is not exhaustively covered.

Startup/Play/StopWatch, component draw and connected state-motion regressions
pass after the shared-header changes. State independence (6,144 cases) and the
combined frontend animation gate also pass, including 1,034 hierarchy samples
and presenter compilation. Full bootstrap linking and live visual
fidelity are not established by these scoped gates.

## Remaining base Update work

The live-child deletion-request stage 005325AB..00532789 remains unrecovered.
Retail obtains CDeletion through slot FC, snapshots its associated-parent list
even for method 0, handles methods 1/2/3, and invokes slot 100 to move a child
out of the live collection. Method 3 searches the snapshot for the current
parent. The observed related functions need correct naming and recovery:

- 00531E90 (slot F8) assigns CDeletion and destroys its by-value list argument.
- 0052E850 (slot FC) returns the Deletion member at D4.
- 00533BC0 (slot 100), currently mislabeled as a script RemoveChildAt, moves an
  entry to ChildrenToDelete and performs counted-pointer/vector ownership work.
- 00535800, 0042CD84, 0042ABCA and 0042B4C3 implement list copy/assignment,
  destruction and insertion boundaries in this path; existing labels are suspect.

Recover that stage and its ownership helpers before composing a complete native
base Update. Do not register the partial extraction as a completed function.
Afterward connect it to state lifecycle/hierarchy checks and then the presenter.
