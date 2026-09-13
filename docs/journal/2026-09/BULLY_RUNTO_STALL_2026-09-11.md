# Bully RunTo zero-movement investigation — 2026-09-11

## Corrected observation

The four incomplete captures do not show a path task stalling after 2.3 m. Installed
`StartOakValeEast.tng` places `MK_OIBR_BULLY1` at local
`(20.479492, 92.742493, 13.707302)`, which is world
`(3604.479, 860.742, 13.707)` in every capture. That is exactly the incomplete endpoint.
`CS_OAKVALEINTRO_BULLYRUN1` teleports BULLY to this marker before issuing
`BULLY.RunTo MK_OIBR_BULLY2,0,FALSE,FALSE,FALSE,TRUE`. The measured 2.326–2.365 m
was therefore the authored teleport from the pre-cutscene position to `BULLY1`; the failed
RunTo itself moved zero metres. The one successful capture reaches `BULLY2` exactly.

This refutes a fixed obstruction at the previously reported endpoint and narrows the defect to
movement-task issue, immediate rejection/completion, or cancellation before the first observable
movement frame.

## Proven boundary mismatch

Retail Bully Main (`0x00DBC86B–0x00DBCCF4`) constructs distinct priority-4 resources in the order
Bully, Hero, Victim before calling the macro. Forge's `RunCutsceneWithSetup` instead finds and copies
the already-live resources owned by the three isolated entity Lua VMs; every archived run logs
`Reused existing script-control handle` for BULLY. This is a real difference, but it is not yet proven
to be the reason the RunTo sometimes makes zero movement: the same reuse path also appears in the one
successful capture.

Actor-table enumeration order is not a surviving cause. Success used BRAT/HERO/BULLY, while failed
captures include BRAT/HERO/BULLY, HERO/BRAT/BULLY, BULLY/HERO/BRAT, and HERO/BULLY/BRAT. The engine
actor map is a `std::map<CCharString,...>`, so insertion order is not its lookup contract anyway.

## Decisive next test

Forge now passively hooks the retail
`CScriptGameResourceObjectScriptedThing::MoveToPosition` (`0x00903BC0`) and
`IsPerformingScriptTask` (`0x00904EE0`) boundaries only while a Lua-driven cutscene macro is bracketed.
For every call it logs `[CutsceneMoveDiag]` with controller identity, destination, radius, move type,
both booleans, and every native poll result. It forwards all operands and return values unchanged.

One Bully run now distinguishes the remaining cases:

- no `issue`: interpreter/command decoding path;
- `issue` followed immediately by `active=0`: engine rejected or synchronously completed the task;
- one or more `active=1`, then `active=0` at BULLY1: task cancellation after successful issue;
- `active=1` while position advances: the movement path works and the earlier failure is timing/state.

The controller can be matched directly to the BULLY `resourceData` logged by
`RunCutsceneWithSetup`, avoiding an actor-identity guess.

## Fix boundary

No behavioral fix is justified until that trace exists. The retail-faithful fix must repair task
issue/ownership so the in-macro poll remains active until the authored marker is reached. Reordering
Lua, moving `Drawable FALSE`, or retaining the post-macro compatibility move does not repair the
native contract and is not proposed as the final fix.

## v14 trace and video result

The user supplied `Fable - The Lost Chapters_2026.09.11-19.38.mp4`; at roughly 2:55 it shows the
compatibility run proceeding normally down the road and the Bully disappearing while still plainly
inside the BRAT-facing shot. Archived log
`interactive-20260911-v14-diag-bully-video.log` (SHA-256
`3D50FC7B2CE484DC84FB6BF3DDC2559C7AAFB03601C713B4BAA60A43A410952D`) records native RUN1 ending
at `MK_OIBR_BULLY1`, followed by the compatibility move.

This exposed a separate, proven defect in that compatibility move: it stopped polling at inferred
2 m distance and immediately hid the actor, even though its scripted movement task was still active.
Retail's proven wait contract polls `IsPerformingScriptTask` until false. v15 changes the compatibility
loop to that exact task predicate, retaining only the 300-frame damage guard, and therefore hides only
after the issued run completes. The correct marker-form native boundary is `MoveToThing` at
`0x00903E20` (slot `0x14`), so v15 also adds that passive hook; v14 had instrumented only the
position-form sibling. v15 is installed but the game was left closed at the user's request. Offline
gates: 119/119 fixtures, 345/345 recovery tests.
