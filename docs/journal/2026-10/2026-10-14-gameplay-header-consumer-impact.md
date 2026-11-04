# Gameplay-header seam: direct-caller impact and implementation includes

The previous [seam journal](2026-10-14-gameplay-header-platform-seam.md) records
the initial inventory, standalone syntax proof and preserved first failure.
This continuation checks real direct callers and corrects the geometry choice
before any canonical promotion.

## Complete first direct-caller syntax comparison

Of 124 direct callers, 123 have one recorded native flag profile in the closed
scope3 metadata. The remaining `CMainGameComponent::InitLUA` caller stays
explicitly excluded by the current native checkpoint. No InitLUA compiler call,
Lua recovery or capped startup retry occurred.

The syntax audit preserves each original flag vector except replacing `/c` with
`/Zs`, controls the include roots/environment, pins canonical/staged/standard
headers and compiler components before the batch, checks actual showIncludes
against those existing pins, and records before/after guards. It parses the
original implementation source with both header sets but emits no object.
This is current syntax compatibility, not full historical profile equality.

All 123 original callers parsed successfully. The first amended header parsed
for 81 callers and failed for 42. Full pair outputs remain at
`work/modern/header-seams/20261014/game-header-forward-declaration-amended1/consumer-syntax-first/`.
First audit result SHA256:
`54b52370c4f6fdb6baae094d130052be8c3fd35f5c5ff8d063431523f8cfda41`.

## Geometry correction from actual diagnostics

The first geometry choice was a mistake: `engine/C2DBoxI.h` declares a different
class from `fable_display_geometry.h`, which the original display/surface chain
actually uses. The generated declaration has signed long coordinates/method
declarations, while the display declaration has unsigned coordinate names with
signed aliases. Many callers that import the display chain encountered a class
redefinition. Passing standalone size assertions alone did not establish that
the chosen declaration was the correct original type.

The separate amendment2 imports **the existing original display geometry header**
directly. It retains its integer/float rectangle definitions unchanged and
forward-declares the display manager. It does not edit either geometry definition,
any game field, method declaration or layout assertion. The staged game-header
graph remains free of selected legacy platform identifiers. All earlier stages,
syntax STOP and 42 failing diagnostics remain preserved.

Current stage:
`work/modern/header-seams/20261014/game-header-display-geometry-amended2/`.
Proposal SHA256:
`26508475559e9d4d0c7e0f4db5da49ca91af346d79c8fe03c541bd99d64379fb`.
Standalone three-probe syntax result SHA256:
`922a00f4f65f6acfa7bf3bc6af902fce0a077b1a25df82a4291239557f323831`.

The new full direct-caller comparison retains all 123 passing original controls.
Its staged side passes **119**, leaving four actual implementation dependencies.
No baseline failures or source/flag tuning occurred.
Second audit result SHA256:
`95f9f997e61ff4b9114972e2595a9ae22694465bfc61afa4b6f93bb6d2059320`.

## Four implementation-only includes

The four remaining implementation files require complete display/render services:

| Source | Actual dependency |
| --- | --- |
| `CMainGameComponent_CMainGameComponent_00418dca.cpp` | Register the device-reset callback with the display manager |
| `CMainGameComponent_GetGameTimeOfNextPresentCompletion_004166e2.cpp` | By-value `CVsyncCallbackTimerInfo` from display timing |
| `CMainGameComponent_Init_004184bd.cpp` | Read display refresh rate |
| `CNewFrontendGameComponent_Draw_0042df9e.cpp` | Call actual renderer/display methods |

New local copies add one explicit `fable_display_viewport.h` include after their
game header. Removing that one added line yields the exact original source bytes.
Canonical files are untouched. Only those four changed copies were checked again;
the 119 passing unchanged callers retain their first amendment2 results.

All four new syntax checks pass. The combined included scope is **123 syntax
passes**, with one explicit InitLUA exclusion. New source paths differ from the
canonical ones: no source-path transport, anonymous-name normalization, object
parity or native behavior acceptance is claimed.
Explicit implementation include result SHA256:
`5a413a3d0dfa4d0994c5c9ac2b92abd196cacef864adbc6c8f477a531d2684ad`.

A concrete five-file patch is saved locally as
`game-header-display-geometry-amended2/reviewable_proposal_UNAPPLIED.patch`.
It changes the one public-header dependency and adds four implementation includes.
**The patch has not been applied.**

## Remaining acceptance

Syntax success does not establish complete field offsets, code generation,
historical include/profile equality, retail parity, behavior or x64 compatibility.
Transitive/forced/macro users, other test/runtime roots and the capped InitLUA
context are not silently covered by this direct-caller scope. Full required
native/header impact and behavior gates precede promotion, preserving the ongoing
canonical input audit and all existing native receipts.

This continuation ran 246 syntax-only calls per full comparison, three standalone
controls for amendment2 and four new implementation checks. No engine provider
object, linker invocation, native fixture, original-game/GPU launch, canonical
header/source/index/grade change, dashboard refresh, commit or push occurred.
