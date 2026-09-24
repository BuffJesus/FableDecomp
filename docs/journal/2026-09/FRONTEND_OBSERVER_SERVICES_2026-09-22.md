# Frontend manager and observer services — 2026-09-22

Branch `wip/native-cgame-play`, worktree `D:\Documents\FableTLC-native-play`.
Continues [base Update and cleanup recovery](FRONTEND_BASE_UPDATE_2026-09-22.md).
All work was offline; the screen remained available for the other agent.

## Readable implementations

| Address | Function |
| --- | --- |
| 0041E5F2 | CFrontEndManager::GetInstance |
| 0055C930 | CObservable::SetExclusiveObserver |
| 0055C940 | CObservable::ClearExclusiveObserver |
| 0055C950 | CObservable::ClearObservers |
| 0055C9C0 | CObservable::RemoveObserver |
| 0055CA00 | CObservable::RemoveConcurrentExclusiveObserver |
| 0055CA40 | CObservable::AddObserver |
| 0055CA90 | CObservable::AddConcurrentExclusiveObserver |
| 0055CAE0 | CObservable::HasExclusiveObservers |

These are functional recoveries, recorded as DIFFER rather than byte matches.
Observable function names are semantic names established by the instructions;
they are not claims of recovered original symbols. The PDB supplies the field
names Observers, m_pExclusiveObserver and ConcurrentExclusiveObservers. Retail
uses offsets 4, 8 and C, unlike the donor's larger list layout. The frontend
manager constructor installs vtable 01230134; its slot 14 points to 0055C9C0.

The singleton replaces an existing emitted-assembly body with ordinary C++.
Its original body is archived under `src/asm_bake`; the active bake backlog and
candidate grade are corrected. It uses singleton slot 013B8710, allocates D0
bytes on a cache miss and stores the constructor's return value. Failed
allocation or a null constructor result leaves it eligible to retry. The shared
FableUiGetManager binding now delegates to this recovered accessor. Allocation
00BFEA1A and concrete manager construction 0041E3F6 remain external services.

Observer addition suppresses duplicates. Individual removal finds the first
matching observer, invokes observer slot 14 before unlinking, reloads the links
after that notification and frees the node. Normal and concurrent-exclusive lists
have the same operations but remain separate collections. Bulk clearing frees
both lists and resets the exclusive observer without invoking notifications.
ClearExclusiveObserver ignores its argument and clears unconditionally.
HasExclusiveObservers returns true for either a single exclusive observer or a
nonempty concurrent-exclusive list; readable code uses an emptiness check where
retail counts nodes. This is equivalent for valid finite lists.

## Evidence

`check_ui_manager_services.py` passes **1,056** comparisons against retail
execution: cached/missing singleton, allocation failure, null/same/different
constructor results and repeated calls; empty/populated observer lists,
duplicates, absent targets, append/unlink during notification, exclusive state,
bulk clear and exact allocation/free order. Both lists' contents and links are
checked. Allocator, constructor, notification and free are controlled boundaries.

`check_ui_observer_chain.py` passes **512** connected comparisons, executing real
Die, RemoveObserverRecursive, GetInstance and RemoveObserver in both native and
rebuilt code. Cases cover empty/wide/deep/branching component hierarchies, all
registration subsets and duplicate root registration. They check notification,
component callback and free ordering, plus surviving registrations. This chain
uses an already-created manager; it does not replace its lookup or unregister
methods with doubles. State requests, observer notifications, component hooks
and free remain controlled services. Existing mutation-focused recursive-cleanup
checks still pass all **80** cases.

The original singleton candidate test also compiles and links the new source
object and passes. Bootstrap runs both new gates. Retail executable hash remains
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

This does not establish behavior for malformed lists, observer callbacks deleting
the node currently being removed, or list allocation failure. Those are outside
the tested valid-container contract; the recovery does not invent defensive
behavior absent from retail. No full game build or visual comparison is claimed.

## Next dependencies

The manager constructor at 0041E3F6 calls an observable base constructor at
0042BE7B, currently mislabeled CConsoleCommandParameters in the manifest. Retail
0042BE7B installs vtable 01230044 and constructs list heads at +4 and +C via
0042AC0A, clearing the exclusive pointer at +8. The latter allocates a 12-byte
self-linked sentinel. Base cleanup 0042BEC0 destroys the concurrent list before
the normal list via 0042AC25. These addresses were inspected but are **not**
registered as recovered by this increment. Recover these constructors/ownership
dependencies next, then concrete manager construction and frontend actions.

The presenter remains unchanged. Full construction, event dispatch, persistent
renderer integration and visual fidelity remain open. Historical global parity
reports and coverage counts were not rewritten.
