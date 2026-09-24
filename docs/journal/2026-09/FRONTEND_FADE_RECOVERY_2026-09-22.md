# Frontend fade recovery — 2026-09-22

User observation: the entire reconstruction feels uncanny, with retail's menu
background dimming/brightening as one example. A navigation smoke pass is not a
visual-fidelity pass. Work stays on wip/native-cgame-play in the native worktree.

## Evidence and correction

The previous presenter drew the outgoing opaque background at alpha 255 and
linearly faded the next frame over it. Retail's compiled frontend definitions
instead give each BLENDING_BG_COASTAL child an alpha of 1 in its own state and 0
in the other states. Each child has its own UpdateTime; children 1–3 use 8 seconds
and child 4 uses 2. The sunbeam children have 2-second transitions. The swapping
parents have zero SwappingTimes and RandomSwap true. These are separate dwell
and interpolation concepts, not a single fixed animation timer.

Retail vtable 012485AC slot +98 points to 0052EC60: it snapshots current colour
into the starting colour, saves target and duration, and resets elapsed time.
Slot +A0 points to 0052F900. Its interpolation prefix (through 0052FE38) computes
start + (target-start) * (2u-u*u), clamps and truncates to a byte, then snaps to the
target at completion. Constants -2 and 0.5 were read from 01246050 and 0122F59C.
The third argument to 0052EC60 is not used by that body; do not infer colour's
linear/nonlinear branch solely from the definition flag. Position and zoom have
separate update paths.

Verified by running the installed retail instructions, including CRT __ftol2,
in Unicorn. For a unit-duration alpha transition:

| Progress | Incoming alpha | Outgoing alpha |
| --- | --- | --- |
| 0 | 0 | 255 |
| 1/4 | 111 | 143 |
| 1/2 | 191 | 63 |
| 3/4 | 239 | 15 |
| 1 | 255 | 0 |

The scaffold now uses a small documented functional extraction of that curve,
fades both opaque-background children independently, and waits for the slower
of the outgoing/incoming child durations before starting another transition.
Sunbeam alphas use the same recovered curve. Static-reference mode is untouched.
This makes the layers' transparency vary during a transition instead of keeping
an opaque outgoing base. It is a grounded correction, not proof that every
reported brightness difference or the historical coastal spatial mismatch is
resolved. Native rendering order/parent colour still need full-chain verification.

## Validation

`python tools/decomp_pipeline/check_frontend_fade.py`

Pass: 260 samples (both directions, 2/8-second durations, 65 samples each), zero
alpha-byte difference in this grid; the gate permits at most one byte for float
rounding beyond byte-identical instruction scheduling. The retail executable
SHA256 is pinned, no retail bytes are committed, and the emulation stops before
parent-colour/vtable work. The VC7.1 presenter compilation also passes. This is
not a byte-parity claim for a full reconstructed CGuiComponent method.

A preview was linked with the freshly compiled presenter and the existing 100
other bootstrap inputs. This is a partial rebuild, not a fresh full bootstrap.
It ran at 1024x768 with boot videos skipped, using the isolated loose assets.
Inspected main-menu captures at successive points of the animation; Options and
Audio navigation also worked. Closed normally. Artifacts:
work/visual-fade-preview/ and work/visual-resume/fade-*.png. Build helper:
work/frontend_component/build_fade_preview.py. The shared checkout and installed
game were not modified.

## Remaining fidelity boundaries

- Native swapping Initialise at 00547360 uses seed 13 per component. Update at
  00547380 calls GFRandomNoRepeat 005472A0; it advances seed by *0x24A1+0x24DF,
  calls GFROR13, takes modulo count and retries repeats. The scaffold's shared
  LCG/skip-index sequence remains different. Recover and test it next.
- Native swapping calls the base update, observes completion, and requests a new
  state only when current==target and the dwell expires. Scaffold wall-clock
  catch-up and page resets are still approximations.
- The independent child traversal, inherited colour, draw ordering, text and
  state propagation are not replaced by native component ownership yet.
- The prior coastal comparison found a spatial mismatch despite similar mean
  RGB. Do not conclude that tweaking overall brightness solves it.


## Follow-up: native random selection and stall handling

The presenter now uses the retail random algorithm, with separate background and
sunbeam seeds initialized to 13. Verified GFROR13 at 00497890 is a right rotate by
13, and GFRandomNoRepeat at 005472A0 advances the seed using unsigned 32-bit
multiply/add, rotates, takes modulo count, and redraws repeated values. This
replaces the scaffold's unrelated shared LCG and skip-index selection.

The bounded retry behavior is preserved: initial draw plus at most 101 retries;
the final retry emits the original wide debug string even if its value differs.
Count zero and count one still advance the seed and return zero.

`python tools/decomp_pipeline/check_frontend_random.py` compares compiled VC7.1
output to the entire retail function and its rotate helper in Unicorn. 4,480
cases pass with identical selected values AND next seeds: counts 0/1/2/3/4/7/
0xFFFFFFFF, five initial seeds, 128 chained selections each. Retry-exhaustion
logging is preserved from static analysis but not exercised in that grid.
This remains an inline functional extraction used by the scaffold, not a new
byte-parity coverage claim for a standalone landed engine function.

Seed-13 background targets start 3,2,1,3,0,3,1,3; sunbeam targets start
2,0,1,0,1,0,1,0. Separate ownership means sunbeam redraws no longer perturb the
background stream. There is still one scaffold pair of seeds shared across
pages/themes, rather than native instances with their full lifetimes.

Also removed the elapsed-time catch-up loops: native 00547380 makes at most one
new-state request per Update and sets LastSwapTime to current time. The presenter
now advances each group at most once per render update and starts the next
transition at that sampled time, so stalls cannot consume multiple unseen states.
This does not reproduce the base component's exact completion notification/frame
ordering; native state lifecycle recovery remains next.

Validation: random differential gate passes, existing fade gate still passes
260/260 exactly, presenter compiles. Relinked the preview with the updated presenter
and the same baseline bootstrap inputs. Captured main menu before/after another
transition under work/visual-resume/random-menu-*.png, inspected both, and closed
normally. This is a partial rebuild and smoke check, not full visual parity.

## Follow-up: colour state and swap completion

`fable_ui_colour.h` now represents the named colour/timing fields and extracts
SetTargetColour (0052EC60) and UpdateColour (0052F900). The latter's complete
retail body is emulated, including parent-colour multiplication and CRT float
conversion; only the virtual independence query is doubled. 1,296 cases pass
with at most one byte of channel rounding difference and matching time fields.
Zero/negative duration setters preserve elapsed time as retail does.

`fable_ui_swapping.h` extracts the decisions after base Update in 00547380.
1,520 differential cases cover dwell, current/target differences, completion,
random/sequential selection, empty lists and duplicate/non-contiguous IDs.
The native ID-as-excluded-index quirk is retained. Base Update, virtual queries
and ChangeState are explicit doubles; the state search and RNG execute retail
instructions. This does not validate native base state propagation.

`fable_frontend_animation.h` bridges those rules to the presenter. Both child
durations must finish; completion is recorded without consuming another random
number, and selection resumes on a later update. Stalls finish the active fade
without fast-forwarding unseen transitions. The controller fixture covers these
boundaries and independent streams. The old presenter timer loops are removed.
This is semantic extraction, not additional landed byte-parity function coverage.

Combined check: `python tools/decomp_pipeline/check_frontend_animation.py`.
All four differential suites and the controller fixture pass. Bootstrap now
invokes this gate. Presenter compilation/link passed using the same 100 baseline
inputs, so this remains a partial rebuild rather than full bootstrap validation.
The preview was inspected at the main menu in successive captures:
`work/visual-resume/controller-main.png` and `controller-main-moving.png`.
Both show the menu and changing illumination. Earlier controller capture names
are unreliable (foreground/title/page mismatch); they are not menu-navigation
evidence. The capture helper now checks foreground ownership before input.
The isolated preview closed normally; no retail launch or installed-game edits.

Draw order remains unresolved. CComponent::Draw 00530260 visits child vectors
forward; CSprite::Draw 0041AFA0 submits cached primitives with increasing indices
or direct engine submissions. CManager::GetLayer 0041E3B2 clamps the requested
layer to [-3,13], looks up a map at +70, and falls back to map[0]. Neither this
nor tree order proves final engine compositing. The presenter retains its prior
background-then-sunbeam ordering pending the engine batching/cache trace.

## Draw-order investigation checkpoint

Follow-up: [the next checkpoint](FRONTEND_DRAW_ORDER_2026-09-22.md) resolves the
normal 2D constructor default to stable sorting. The false-flag observations
below are not evidence of the normal frontend path shuffling equal depths.

`python tools/decomp_pipeline/probe_frontend_draw_order.py` now runs two actual
retail routines without dependency doubles (pinned executable, offline Unicorn).
This is an evidence probe, not a reconstructed-C++ or visual-parity gate.

- AddToList 00B8FDF0 groups by RenderLayerMask (+30), using NextPrimitive (+38)
  and NextLayerMask (+40). A new mask group is prepended; another primitive in
  an existing group is inserted immediately after its representative. Four
  equal-mask submissions 0,1,2,3 traverse as 0,3,2,1. Three probe chains validate
  membership and primitive backlinks.
- CEnginePrimitiveRenderer2D::Render 00B4A700 traverses NextPrimitive and calls
  primitive-manager slot +28, then RenderType(2,true) at 00B849F0. RenderType
  has per-list flags for sorting and alternate behavior; recover their setup
  before deciding which path applies to the frontend.
- SortList 00B84990's false-flag path uses the native comparison sort below 256
  elements, radix sort otherwise. Ten equal-key lists of sizes 1..255 execute
  the small-list path without stubs. Tested sizes through 16 retain order;
  17/24/32 reverse the equal-key list; larger samples reorder it differently.
  Therefore a stable sort is not a universally equivalent replacement.
- The true-flag sort path and radix path are not validated by this probe.
  The frontend's real sort flag, cache submission order, list size and layer
  distance map remain unresolved. Do not apply the probe permutation to the
  presenter as if it established live draw order.

Retail renderer fields here are four bytes earlier than the generated donor
CEnginePrimitiveRenderer2D header: First2DPrim +40, displacement +44, layer
distances +88, sort distances +1D4. Do not import that donor layout unchanged.
The two methods both labelled Layer2DToDistance at 00B4A6A0/00B4A6B0 read the
sort-distance/geometric-distance arrays respectively. Sprite submission through
00B84720 quantizes (MaxSortDistance - distance) * SortDistanceMultiplier and
clamps the key to [0,0x3FFFFFFF]. Existing propagated labels on nearby helpers
are unreliable; retain address-based evidence.
