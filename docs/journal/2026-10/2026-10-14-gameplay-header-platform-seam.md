# Gameplay-header platform seam: inventory and staged compiler proof

The user continues native recreation toward 64-bit modernization until asked to
stop. The connected menu draft remains pending review/final packet preparation.
This turn also advances the documented preparation item that gameplay-facing
headers should stop importing legacy graphics/input/audio/video interfaces.

## What is implemented

`tools/modern/header_surface.py` records legacy platform identifiers, local include
graphs, shortest include routes, current header hashes and explicit boundary
labels. It masks comments, ordinary/raw string literals and character literals,
including directive-looking lines inside raw strings. All textual conditional
branches are considered. Source file inventory/hashes are checked again before
the report returns.

Five meaningful tests pass: lexical exclusions/member-name false positives,
cycles/shortest paths/backend labels/external SDK includes, conditional-branch
qualification, raw-string fake directives/physical line numbers, and missing
roots/outside-directory resolution limits.

The generated inventory covers 1,669 actual headers and five explicitly selected
roots. `fable_game.h` reaches eleven local headers with Direct3D identifiers and
one external SDK include path through `fable_display_viewport.h`. The other four
selected roots (`fable_cgame.h`, `fable_iengine.h`, `fable_input_manager.h`,
`fable_nui_manager.h`) have no selected identifiers/known SDK include paths in
this local textual graph. This is a bounded syntactic inventory, not proof that
the entire gameplay API or an evaluated compile profile is platform independent.

Reports: `docs/modern/HEADER_PLATFORM_SURFACE.md` and
`docs/modern/HEADER_PLATFORM_SURFACE.json`. Canonical headers and the pinned
verifier were not edited; verifier warning integration remains open.

## Concrete staged header change

The first exclusive local stage replaced only the display backend include in
`fable_game.h` with a forward declaration of `CDisplayManager`. Its direct-source
census contains 124 literal `.cpp` consumers; three already include the full
display header explicitly. The other 121 require source/include/profile review,
rather than automatic edits. Transitive/forced/macro users and other source/test
roots remain outside this direct census.

First stage:
`work/modern/header-seams/20261014/game-header-forward-declaration/`.
Proposal SHA256:
`92c5a6766db1beac7cc3d7d1a40d3531f717c3dd1ab1512ca5c83d6f9fede84c`.

The original-header syntax control passed. The staged-header syntax check stopped
on two real by-value geometry fields: `CNewFrontendGameComponent::AVISrcBox` and
`AVIDstBox` require complete `C2DBoxI`. Its existing class-size assertion also
failed after the missing type. The first stage, diagnostics and STOP receipt
remain unchanged; the negative staged SDK control was not reached in that run.

The separate corrected stage imports existing `engine/C2DBoxI.h` directly and
forward-declares the display manager. It changes no field, method declaration,
geometry definition or existing layout assertion. All other copied source
headers remain byte-identical. Its local textual graph has no selected legacy
platform identifiers reachable from `fable_game.h`.

Corrected stage:
`work/modern/header-seams/20261014/game-header-forward-declaration-amended1/`.
Proposal SHA256:
`f39e93e86361778b12447ec1527d8b5d8b517a2433428f5403e36264bc6a312f`.

## Actual compiler evidence

VC7.1 `/Zs /showIncludes` records three passing outcome checks in the corrected
stage:

| Probe | Actual outcome |
| --- | --- |
| Original game header plus `IDirect3DDevice9*` declaration | Parses successfully |
| Corrected game header and display-manager pointer | Parses successfully, including existing header size assertions |
| Corrected game header plus the same `IDirect3DDevice9*` declaration | Fails with the expected missing SDK type diagnostic |

Corrected syntax receipt SHA256:
`c21ee5de75886b0e01717190dc7f7a5e8e99665859367458dc2267585fde45c8`.
The final receipt binds 5,152 observed inputs with zero drift. Current standard
headers first observed after a compiler invocation are explicitly post-invocation
observations, not retroactively added to its before map. Header declarations and
inline definitions were parsed; no engine implementation unit or provider object
was emitted. No linker or native fixture was invoked.

There were two syntax-only invocations in the stopped first stage and three in
the corrected stage. This is syntax/layout-size visibility evidence only, not
consumer code generation, complete field offsets, byte parity, behavior, source
profile equality or x64 compatibility.

## Next work

Review each direct caller's actual fixed profile and lost inherited declarations,
then prepare explicit implementation includes where needed. Full transitive and
test/runtime consumer closure and required behavioral/layout checks precede
promotion. Keep the ongoing native evidence audit and canonical input pins
intact. The staged result does not complete the modernization fork gate or the
permanent verifier-warning task.

Canonical source/header/index/grades, dashboard, historical HANDOFF/ROADMAP and
existing native receipts remain unchanged. No provider recompile, native rerun,
game/GPU launch, Lua work, commit or push occurred in this preparation work.
