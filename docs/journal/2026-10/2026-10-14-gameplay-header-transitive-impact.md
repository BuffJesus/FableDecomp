# Gameplay-header seam: transitive, test and runtime callers

The direct-caller work remains preserved in the earlier October 14 journals.
This continuation expands the actual dependency census and checks newly found
implementation and test/runtime consumers without applying the proposed patch.

## Actual static routes and census

Five headers reach `fable_game.h`: the game header itself and four indirect
routes through engine-component parameters, file information, game-event
dispatch and world-map declarations. The literal-include census searches six
explicit roots: compiled implementations, tests, runtime, integration, SDK and
seam scaffold. It masks comments/literals and checks matching-file/header hashes
again before returning.

The census finds **275 consumers**: 124 original direct implementation consumers,
14 additional implementation files and 137 test/runtime or other sources. All
new implementation files have recorded native compiler profiles. These are
static literal routes; macro/forced/compiler-specific lookup, alternative
relative spelling and outside-root users are not silently declared covered.

Census SHA256:
`28c49e014b406c7a66de27209590db537fa457bec77c788559dc61612000f385`.

## Fourteen new implementation pairs

All fourteen original/staged code-generation pairs compile and compare identical
under the same ordered nondebug projection used by the direct batch. Their
recorded flag vectors and actual canonical source paths remain fixed. No source
include/body/flag fitting occurs; no old passing native fixture is rerun.

These cover file/time readers, world-map lookup, game-event dispatch and engine
frame/layer/primitive rendering parents. No separate full auxiliary ownership or
behavior acceptance is inferred from their section equality.

Result SHA256:
`715bc171f84e56834454c3f16d4c9749decb07e19a25705cc73c0507a796c71a`.
28 compiler calls / 14 ordered nondebug equals / zero failures / zero observed
input drift / zero linker/native calls. Combined implementation scope is 137
included sources, with the original InitLUA caller still capped.

## Test/runtime syntax checks

Of 137 additional source files, the InitLUA test remains explicitly excluded.
The other 136 receive original/staged syntax-only checks under an explicitly
recorded common `/Od /W3 /MT` profile. This is not claimed as each fixture's
historical operational recipe. Resolved literal headers and included implementation
files are pinned before the batch, and actual compiler includes are checked
against those existing hashes.

First result: **126 pass both / 3 new staged failures / 7 original baseline
failures**. All complete outputs remain. First result SHA256:
`d56cb2310ee78c8b933ffd0733b9256dd4d00511f92bf753153b4210d1696f40`.

The three valid staged differences require complete display declarations in
their own test implementations:

- Main-game constructor device-reset callback test.
- Next-present-completion timing test using `CVsyncCallbackTimerInfo`.
- Integrated frontend-draw test defining renderer/display substitutes.

Three local copies add only an explicit display header. Their original local
directory is included explicitly because their physical source paths change.
Only these three changed copies are checked again; all three pass. The 126
unchanged passing records remain reused, rather than re-executed. Thus **129
comparable test/runtime sources parse successfully**, while seven baseline
contexts remain unaccepted and the InitLUA test remains capped.

Explicit test include result SHA256:
`ce099c1c88b1dadf5de2921681c6bfa03949c14c41f9e9198313245e35ff0f78`.
No test/native executable is built or run in these syntax phases.

## Seven baseline contexts require separate recipe/ownership work

Two main-game destructor tests redefine an already defined `CBaseClass`
destructor. The text-initialization test redefines `NGameText::CDataBank` and
declares a member absent from the active type. The frontend Run test has an
invalid explicit singleton specialization under this common profile. Three
primitive-frame/render tests lack their generated retail/alias headers in the
current lookup roots.

These are actual failures with the original headers, not newly introduced seam
failures. They are not fixed, ignored, counted as passes or automatically rerun
with different flags. Their real operational recipes, generated-header availability
and source/type ownership must be reviewed before any fixture acceptance claim.

## Remaining gates

The proposal and all source/test copies remain local and unapplied. Full macro,
forced and outside-root closure, actual fixture recipes, runtime behavior and
canonical promotion remain open. The new fourteen implementation pairs have not
received the earlier direct batch's complete auxiliary metadata review. Existing
native constructor/lifecycle/source-profile/borrowed-state qualifications remain.

This turn made 28 impact compiler calls, 272 syntax-only calls and three changed
test-source syntax checks; it made no linker/native/GPU/game invocation. Canonical
headers/sources/tests/index/grades, dashboard, existing native receipts, HANDOFF
and ROADMAP remain unchanged. No Lua recovery, commit or push occurred.
