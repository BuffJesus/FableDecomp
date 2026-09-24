# Frontend lifetime and state recovery — 2026-09-22

Worktree: D:/Documents/FableTLC-native-play, branch wip/native-cgame-play.
These are offline retail differential recoveries. They are not yet a replacement
for the presenter's flat animation controller or proof of full visual fidelity.
No GUI launched in this increment. The later queued-fade timing correction is
integrated in the flat presenter controller, as detailed below.

## Recoveries and validation

| Recovery | Retail | Validation | Parity |
| --- | --- | --- | --- |
| Sprite visibility decision | 0041AFA0 prefix | 1,024 cases | Semantic extraction |
| Primitive RemoveFromList | 00B8FE60 | 1,681 cases, 2,417 snapshots | MATCH, 116/116 bytes |
| Component Draw | 00530260 | 515 exact callback traces | Functional DIFFER |
| State completion query | 0052C8F0 | 2,560 combined query cases | MATCH, 37/37 bytes |
| ChangedStateLastUpdate | 0041C5E0 | Same combined query cases | Functional DIFFER |
| UpdateStateChange | 0052CD20 | 2,176 task/dispatch cases | Functional DIFFER |
| ChangeState | 0052CF40 | 1,696 request cases | Functional DIFFER |
| ProcessChangeState | 0052C920 | 1,824 setup traces | Functional DIFFER |
| Update / InternalChanged / ChildrenChanged | 0052C7E0 / 0052C780 / 0052CBF0 | 2,624 combined cases | Functional DIFFER |
| Connected state/colour lifecycle | Eight state methods + colour routines | 15 scenarios, 480 frames, exact state and colour history | Functional integration |
| Presenter timing vs coastal hierarchies | Native 29/22-node trees | 1,034 updates, exact state/seed/time/alpha | Integrated queued-fade correction |

Run tools/decomp_pipeline/check_frontend_animation.py (includes visibility),
check_primitive_removal.py, check_component_draw.py, check_state_progress.py,
check_state_tasks.py, check_state_request.py, check_state_process.py, and
check_state_update.py, and check_state_lifecycle.py. All pass. Bootstrap runs these gates. The two MATCH
gates reject byte drift, while the other recoveries accept functional equivalence.

## Evidence and limits

Sprite NoRenderNextFrame is retail +170 (donor +1A0). The first invisible draw
still submits a sprite; subsequent invisible draws release it. Alpha zero or
both render zoom axes nonpositive are invisible. Unordered zoom comparisons
remain visible. The visibility gate tests the actual Draw prefix, stopping at
submission or return with an empty handle. It excludes geometry and nonempty
handle release. The sprite constructor clears NoRenderNextFrame.

RemoveFromList repairs both primitive and mask-group links, promotes the next
representative when necessary, and clears all four links. Persistent grid-owned
nodes decrement LocalCount and clear GridCell; the NonPersistent flag suppresses
that action. Tests execute complete native add/remove without doubles, including
repeated removal. CEngineSceneGridCell's quarantined generated header is used
only for LocalCount+34, independently verified in retail; the quarantine conflict
concerns a different old padding range.

Component Draw visits live then retiring children in forward order. Child
definitions are also read forward and AddChild appends. Draw asks suppression
twice and re-fetches entries after virtual calls. In the retiring foreign-parent
branch retail queries the corresponding LIVE entry. The recovery preserves this
quirk. Tests compare complete query/draw traces and three callback mutations;
Draw callbacks are doubles. Invalid vector layouts remain outside the contract.

Normal sprites use mask 0x80000000, or 0x00200000 with flag 0x400. This comes from
primitive Update 00BAD8A0, not a guessed mask number. The earlier persistent-list
probe uses synthetic equal mask 4 and only establishes same-mask list behavior.

The state progress layout uses PDB member names with retail offsets:
StatesBeingDone+134, StatesDone+138, one-pointer StatesToDo+13C,
ParentUpdateTime+140, CurrentState+144, TargetState+148, RequestedState+14C,
UpdateTime+150, PreviousUpdateChanged+154, PreviousState+158. Donor STL storage
is larger; its offsets must not be copied verbatim. HasCompletedStateChange is
a semantic name for +C4, not a claimed original symbol.

Completion requires an empty queue AND all active task bits satisfied. The
completion-edge method compares PreviousUpdateChanged with the virtual query
and, if different, queries again. Tests include differing query responses and
noncanonical stored bytes. The queue dispatcher pops only one task per call;
starting tasks and polling completion are mutually exclusive branches. It keeps
previously finished bits when popping. Child dispatch requires this parent;
type 8 skips requested states 1, 3, and 4. Final completion records PreviousState
and commits TargetState only when the queue and task masks permit it. The task
gate doubles free and virtual callees, compares call order, list integrity and
state fields, and exercises callback mutations.

The subsequent request recovery validates all five scheduling modes, missing
states, interrupted requests, repeated requests, inherited duration (including
NaN and negative values), and child dispatch. Native list insertion executes;
malloc/free and virtual dependencies are doubles. ProcessChangeState validates
position/zoom squared-distance threshold 0.0001f, modulo-256 colour deltas,
unused-but-observable current-state lookup, noncanonical linear flags and
callback mutations. Frame Update and InternalChanged/ChildrenChanged validate
completion snapshot timing, parent transform/colour resets, all three timer
pairs, live-child completion and mutations across the base Update boundary.
The base CComponent::Update remains an explicit unrecovered external dependency.
Propagated manifest names for 0052C8F0 and 0052CBF0 were incorrect. Manifest and
function overrides now use HasCompletedStateChange and ChildrenChanged with
evidence explicitly marking them semantic names, not recovered original symbols.

The connected lifecycle fixture links all eight recovered state methods to the
recovered colour implementation. Its oracle executes the corresponding native
methods, relative colour change, colour setter and colour update. It covers
all five scheduling modes at durations 0, 0.5 and 2 seconds, repeated requests
and interruption across 32 frames each. All 480 frame histories match exactly,
including state/task fields, queue contents, completion edges, timer bits and
colour channels. The fixture allows the colour extraction's established
one-channel tolerance, but observed error is zero. Lookup, allocator and colour
independence are doubles. Its oracle now executes native base Update with empty
collections and quiescent transform timers; the compiled dependency remains
colour-only. This fixture does not validate general ownership or actions.

## Queued fade timing integrated in the presenter

The hierarchy oracle now uses 52 definition-derived nodes from retail frontend.bin:
coastal root 685 containing swapping group 686 (29 nodes) and sunbeam group 687
(22 nodes). Both swapping groups update through the real parent's base traversal. The
checked-in fixture retains relevant type, children and state fields, not assets.
Its frontend.bin source hash is verified before running the hierarchy oracle.
Controlled construction supplies state-zero transforms/colours, empty position
and deletion lists, and proper parent links. State-map lookup and allocation are
doubles; native base Update, child traversal, parent colour propagation, state
requests/tasks, colour methods and swapping/RNG all execute. Root parent is a
fixture sentinel to exclude unrelated manager scaling. No rendering occurs.

This exposed the missing queued startup in FableFrontendAnimation: ChangeState
does not immediately start a colour timer. On the following update children
advance the old colour first, then ProcessChangeState starts their queued fade.
The old bridge failed 1,024 of 1,034 samples. A PendingTransition field now
defers target setup to that point; both groups match every checked state ID,
seed, swap-time bit pattern and alpha exactly across quarter-second steps,
zero-delta updates and large stalls. The bridge remains specific to these
simultaneous colour transitions, not a general state/ownership implementation.
check_frontend_hierarchy.py is included in check_frontend_animation.py.

The combined gate passes after the correction, including presenter compilation.
A partial preview executable was also relinked successfully at
work/visual-fade-preview/FableTLC-Reconstruction-VisualCheckpoint.exe: fresh
presenter object plus 100 existing baseline objects/resources. This is not a
full bootstrap rebuild, and no GUI was launched for this timing-only increment.

## Remaining visual work

The next controlled probe, tools/decomp_pipeline/probe_frontend_lifetime_order.py,
executes native component Draw through the same combined tree. Its sprite callback
uses the separately verified visibility decision; a separate native primitive
list executes AddToList/RemoveFromList. It does not execute engine factories,
handles, refcount destruction, depth sorting or rendering. Two scenarios cover
an empty same-mask list and one pre-existing same-mask anchor. Neither asserts
the real page's surrounding primitive population. All 160 updates / 6,720 tile
visits pass complete membership/backlink checks; all submitted tile layers are 7.

At frame 2 (quarter-second steps), with the pre-existing anchor, visible list
blocks are background frame 691, sunbeam frame 694, background frame 688,
sunbeam frame 692, six tiles each. At frame 33 the blocks are sunbeam 692,
sunbeam 693, background 691. Without the anchor, the representative's frame can
split into one tile at the front and five later. Thus lifetime-dependent
interleaving exists under BOTH controlled starting conditions; a fixed
backgrounds-then-sunbeams pass cannot represent those list histories. This is
conditional collection-order evidence, not a proved final retail draw order.
Keep the presenter composition unchanged until surrounding page primitives and
the full submission/sort path establish the correct starting conditions.

The presenter's compact controller now matches the tested background timing,
but still bypasses general native queued state processing. ChangeState,
ProcessChangeState and changing-component Update are recovered. Recover general
base Update, constructor/ownership dependencies and renderer integration next.
ChangingStateComponent::Update
0052C7E0 saves completion BEFORE the base update and advances tasks afterward
only when the saved result was false. InternalChanged checks position, zoom,
and colour elapsed/duration pairs; ChildrenChanged checks every live child.

Draw ordering also depends on persistent sprite history. Main menu definitions
put foreground before the coastal background; press-start and credits have
different orders. Coastal background children are sunbeam then background, each
containing six-tile frame groups. A universal guessed foreground anchor is not
valid. Preserve component/primitive lifetimes before replacing flat compositing.
