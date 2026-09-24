# Frontend transforms and state inheritance — 2026-09-22

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
All work in this increment was offline. No game or preview was launched.

Recovered readable C++ for the following retail routines, sharing the typed
component/state layout rather than repeating raw member offsets:

| Retail address | Recovered role | Acceptance |
| --- | --- | --- |
| 0052E580 / 0052E530 | Forward / inverse coordinate conversion | Functional |
| 0052FFD0 / 0052F5C0 | UpdatePosition / UpdateZoom | Functional |
| 0041CF47 | CManager::GetUIScale | Functional |
| 0041CC14 / 0041CC2B | Effective coordinate width / height | 23 bytes each, relocation match |
| 0052E9C0 / 0052EA30 | Position target / delta | Functional |
| 0052EBB0 / 0052EC20 | Zoom target / delta | Functional |
| 0052F1A0 / 0052F1B0 | Base position / zoom independence forwarding | Functional |
| 0052C870 / 0052C8B0 | Target-state position / zoom independence | Functional |

The two dimension getters replace existing compiled bodies with incorrect
CTCLook names. The base independence methods were incorrectly attributed to
CThingFilter_IsValid. The zoom state query duplicated the position query's name,
and the absolute zoom setter had a propagated CSprite vfunc label. Manifest and
override corrections record the retail evidence and distinguish semantic names
from recovered original symbols. Both conversion functions return vectors via
a hidden ECX output pointer; the prior scalar-float prototypes were wrong.

## Observed behavior

Position and zoom use the same ease-out curve as the existing colour recovery,
but preserve specific float intermediate stores. VC7.1 otherwise retains extra
x87 precision and changes cancellation results. The shared helper explicitly
preserves the coefficient and travel stores. Elapsed time is stored as float;
endpoint comparison uses the retained sum.

Absolute setters reset elapsed time only for positive durations. Zero, negative
and unordered durations immediately set the value but leave elapsed untouched.
The linear argument is unused in these vector setters. Delta methods dispatch
through distinct absolute-target vtable slots, now named correctly in the header.

Relative position deltas are inverse-converted, then scaled through two distinct
manager calls before adding the current position. Query/callback order matters:
callbacks can mutate the current transform. Zoom updates can call manager scale
four times, and query independence twice. The code preserves those observations.

Manager scaling is gated by a context pointer and compares effective dimensions
against **1024 x 768**. If either dimension is smaller, both axes receive their
respective width/1024 and height/768 ratios; the other axis can exceed one.
The context's concrete owner and manager singleton remain unrecovered.

Changing-state independence first calls the base virtual independence query,
then always looks up the current **target** state through slot 218. Position ORs
in state flag 0x20; zoom ORs in 0x40. The initial independence result is retained
across the state lookup. Slot 21C remains the separately identified state lookup
used by state processing; the lookup implementations themselves remain open.

## Validation

All gates are wired into `rebuild/build_bootstrap.ps1`:

- `check_ui_transform.py`: 2,137 position/zoom/conversion cases. Exact elapsed
  bits and callback traces. Vector tolerance is 0.0001 + abs(retail)*0.000002;
  maximum observed absolute difference 0.03125 on large extrapolated values.
- `check_ui_scale.py`: 1,210 cases, actual retail scale and dimension getters
  without dependency doubles. Exact floats except NaN payloads. Explicit global
  relocation binding proves both 23-byte getter matches.
- `check_ui_vector_change.py`: 2,219 cases, exact float bits except NaN payloads
  and exact traces. Includes input aliasing, immediate/animated changes, inverse
  conversion and the actual scale/getter chain. Only relative query and manager
  singleton are doubled.
- `check_state_independence.py`: 6,144 cases, exact results and callback traces,
  including all low-byte state flags, missing states and callback mutations.
- `check_state_lifecycle.py --motion`: 480 frames / 15 scenarios connect state
  queue processing, real target/delta setters, transform updates, state-dependent
  inheritance and colour. Includes interrupted/repeated requests, zero-delta
  frames and stalls. Exact transform/state histories; colour channel error zero.
  The original colour-only 480-frame lifecycle also passes.
- Existing component draw (515) and state processing (1,824) regressions pass.
- State completion (2,560), task dispatch (2,176), requests (1,696) and update
  coordination (2,624) regressions pass after the shared vtable changes.
- Combined frontend animation gate passes, including all 1,034 hierarchy
  samples and a successful presenter compile. No visual launch was needed.

The lifecycle oracle executes native base Update with empty collections. Its
compiled counterpart explicitly advances only recovered transforms and colour;
it is **not** a recovered general base Update. Relative/overall independence,
state lookup and allocation remain controlled dependencies in that fixture.

These routines are not yet connected to the flat visual presenter. This work
does not demonstrate a new visual improvement. Next: general base Update's
parent-to-child transform propagation and ownership, component construction,
then persistent sprite lifetime and renderer integration. Preserve the earlier
hierarchy timing fix and avoid guessing a universal foreground draw anchor.
