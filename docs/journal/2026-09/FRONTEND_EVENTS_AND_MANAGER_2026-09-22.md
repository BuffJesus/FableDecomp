# Observable lifetime, event trees and manager configuration — 2026-09-22

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [observer services](FRONTEND_OBSERVER_SERVICES_2026-09-22.md). All work
was completed offline. A subsequent user-requested preview check is recorded below.

## Implemented checkpoint

Thirty-four complete functions now have readable implementations or replace
earlier misleading/assembly-only candidates. Acceptance is functional DIFFER,
not a claim of byte parity. Named layouts come from PDB fields, checked against
retail offsets; donor STL containers are not used as retail layouts.

| Area | Addresses |
| --- | --- |
| Observable construction/destruction/exclusive getter | 0042BE7B, 0042BEC0, 0042BEA9 |
| Observer-list construction/clear/destruction/copy/range insertion | 0042AC0A, 0042A1E3, 0042AC25, 0055CF50, 0055CE90 |
| Observable event dispatch | 0055CB10 |
| Manager input/meta-layer configuration | 0041DF10, 0041E1CD |
| Observer construction, filter, bulk registration and clearing | 0052D9E0, 0052D900, 0052D7B0, 0052D9A0 |
| Event lookup/range traversal/tree disposal | 0052DF20, 0052DEC0, 0052DCA0 |
| Tree rotations and insertion balancing | 0042951B, 0042955B, 0042971D |
| Event node insertion and registration | 0052E0E0, 0052E230, 0052DA20 |
| Event erasure/rebalancing and unregistration | 0042A34E, 0052D940 |
| Integer-map lookup/insertion for keys and layers | 0042D201, 0042D246 |
| Relative-coordinate initialization | 004299A8 |
| Complete manager and graphics-bank configuration constructors | 0041E3F6, 00415B80 |
| Input, layer and component map constructors | 0042BF67, 0042BF85, 0042D28B |

Several existing candidates had unrelated class names. List cleanup was labeled
GUI tree sorting, file completion or a music-map pair destructor. Observer
construction was labeled frontend-manager initialization. The replacements use
semantic names and real dependencies. Old assembly versions of observer
construction/registration and coordinate setup are archived under `src/asm_bake`,
with active bake grades removed. Corrected candidate tests link actual source
objects. Prior global parity reports and coverage have not been rewritten.

RTTI also corrects an earlier journal's singleton ownership: vtable 01230134 is
`NUISystem::CManager`, not `CFrontEndManager`; 01230044 is `CObservable` and
01246020 is `CObserver`. The singleton's historical FrontEnd link names are kept
for compatibility. Its actual constructor is CManager at 0041E3F6.

## Behavioral details

Observable dispatch prioritizes the single exclusive observer. If none exists,
it snapshots either the nonempty concurrent-exclusive list or the normal list.
This preserves iteration when callbacks clear or append registrations. A query
can replace the exclusive observer; processing reloads the current pointer.
Constructors allocate sentinels without initializing their unused payloads.
Observable destruction frees the concurrent list before the normal list.

Observer filtering checks PreventObservation, then searches its ordered event
set. ObserveAllEvents reloads the vtable for each registration and explicitly
skips IDs 34 and 35. First registration of event 25 immediately invokes the event
handler after insertion. Re-registering it does not invoke the handler again.
Tests include handlers clearing the tree during that first-registration call.

The recovered red-black tree preserves node identities, parent/left/right links,
colour changes, extrema and removal order. Individual removal searches even when
observation is prevented. A two-child removal relinks the successor node rather
than copying its event value. Generic tree helpers are named for this tested
instantiation; other retail callers may use the same code with different values.

SetInput stores the requested input type even for unsupported values. Type one
changes four mappings and leaves the remaining mappings intact; type zero writes
sixteen. SetMetaLayer clamps to 0..4. Layer zero writes keys -3..13 and skips
engine layer 0x48; the other layers only replace keys -3..7. Both preserve unrelated
entries. Missing map keys insert zero before the caller assigns their values.

Relative-coordinate setup first changes its enabled flag. Only a transition to
enabled queries the display and refreshes cached float dimensions. Repeatedly
enabling it does not refresh them when the viewport changes. Disabling it also
leaves the cached destination extents unchanged. The readable path is connected
to the previously recovered effective-dimension getters and manager UI scaling.

## Validation checkpoint

All gates compile VC7.1 code and compare against the retail executable with SHA256
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

| Gate | Cases | Connected scope |
| --- | ---: | --- |
| `check_ui_observer_lifetime.py` | 3,840 | Observable constructor, list lifetime, snapshot dispatch; mutation and nested dispatch |
| `check_ui_observer_events.py` | 4,464 | Observer constructor, ordered lookup/filter, ObserveAll and tree disposal |
| `check_ui_observer_lifetime.py --filter` | 1,440 | Actual event filtering connected to snapshot dispatch |
| `check_ui_event_registration.py` | 544 | Registration, insertion/rotations, individual removal/rebalancing, bulk registration and disposal |
| `check_ui_observer_chain.py --events` | 512 | Die, recursive cleanup, singleton, unregister and actual event-set clearing |
| `check_ui_manager_configuration.py` | 3,072 | Configuration with observed map-service boundaries |
| `check_ui_manager_configuration.py --maps` | 3,072 | Configuration with actual map lookup/insertion and tree balancing |
| `check_ui_coordinate_setup.py` | 576 | Relative-coordinate transitions, dimensions and manager scaling |
| `check_ui_manager_construction.py` | 324 | Singleton through complete manager construction, actual observable/maps/configuration/coordinate setup and graphics-bank initialization |

All nine modes pass: 17,844 cases. The three corrected map-constructor candidate
tests and the coordinate-setter candidate also compile, link and pass.

Registration tests compare complete surviving tree state after each removal,
plus exact allocation/free and event-handler traces. Ordered, reversed, duplicate,
extreme signed values and deterministic random sequences are covered. Map tests
compare all node links, colours, keys and values after repeated configuration
changes, starting with both seeded and missing entries. Coordinate tests compare
float bits and query timing, including stale caches and signed integer limits.

Allocators/free and external event handlers remain controlled services. General
observer trees and map sentinels are manually initialized in some gates; the
observer-constructor gates separately validate native initialization. No claim
is made about allocation failure, malformed containers or callbacks freeing a
currently executing observer. Renderer integration is not covered. The constructor
gate uses controlled external services as detailed below. Functional insertion can use a simpler search than
retail while preserving the tested unique-container behavior.

Bootstrap includes these gates. Small repaired candidate tests supplement the
connected comparisons; they are not substitutes for them. No visual fidelity or
full game rebuild is claimed.

## Continuing dependencies

CManager construction 0041E3F6 is now readable and connected to the real singleton.
The gate compares all 208 object bytes, allocated sentinel/tree bytes (including
untouched padding and payloads), allocation order, service traces, and a second
cached singleton lookup. Cases include poisoned storage, coordinate flags 0/1/255,
signed dimension limits, null/non-null bank references, and changing display
receivers between format queries. The recovered 44-byte CGraphicDataBankInit
constructor 00415B80 replaces its incorrect CNavigationLayer identification.
Input/layer map constructors allocate 24-byte sentinels; the counted-component
map constructor allocates a 28-byte sentinel. The three previous unrelated
CopyBackBufferToTexture labels and candidate/catalog paths are corrected.

Manager construction selects GBANK_FRONT_END or GBANK_MAIN using byte +9 of
global 013B871C's pointee. The bank configuration preserves the interpolated-alpha
format default, assigns six queried formats, and disables dithering. Readable
construction preserves fields that retail leaves untouched, including the input
manager pointer, right-stick state, mouse movement and key state.

External allocation, system/display access and pixel-format queries, string
construction/destruction, graphics-bank factory and SetGraphicsBank ownership
remain controlled doubles. This proves constructor behavior at those boundaries,
not asset loading or device initialization. Counted graphics-bank ownership is
not validated by assigning a pointer in the constructor fixture.

Next concrete recovery: SetGraphicsBank 0042A9B7 and its counted-reference
assignment/release dependencies 00419134/00419108. Then connect the string,
graphics-bank factory and display services needed for native creation; continue
frontend actions and persistent renderer integration. Do not restart completed
observer/event/map recovery or treat old checkpoint open items as current.

## End-of-session handoff

All changes are saved, uncommitted, in the native worktree on
`wip/native-cgame-play`; the separate script-recovery checkout was not modified.
No game or GUI was launched. All checks started by this session have completed.
At shutdown review, two separate script-recovery unittest processes were still
running; they were left untouched because they belong to the other work lane.
No background native-engine task must survive shutdown. The ignored `work/`
folders contain reports and build products; source, gates and this journal carry
the reproducible handoff. Run the individual gates listed above from this
worktree with `python tools/decomp_pipeline/<gate>` when resuming validation.

The presenter is unchanged. Native creation, event handlers/actions, persistent
rendering and the reported visual uncanniness remain open.

## User-requested visual check after shutdown handoff

Launched the 17:42 partial preview (fresh presenter plus baseline objects), copied
as work/visual-resume/FableTLC-NightPreview.exe beside its extracted data. Launching
from visual-fade-preview first exited before a window appeared; placing the exe
beside the data resolved startup. Used --retail-frontend-reference-size (1024x768).
This preview does not integrate the newer manager/observer/base-update recoveries.

Visually checked title -> main menu -> Options -> Audio Options -> Cancel -> Back.
Navigation returned to the main menu and the preview closed normally. No settings
were applied and retail was not launched. Successive main-menu captures clearly
show changing illumination and light shafts. Conspicuous hard bands in the shafts
remain a visual concern; no contemporaneous retail A/B was performed, so their
cause and fidelity are not established. This is a presentation smoke check, not
full-engine validation or a claim that visual uncanniness is fixed.

Captures: work/visual-resume/night-title.png, night-main-a.png, night-main-b.png,
night-options.png, night-audio.png, night-options-return.png, night-main-return.png.
The isolated preview is closed; no task from this check remains running.
