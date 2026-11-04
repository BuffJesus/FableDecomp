# Gameplay-header seam: first compiler-output comparison

The staged seam has [123 direct-caller syntax passes](2026-10-14-gameplay-header-consumer-impact.md)
and [479 matching layout constants](2026-10-14-gameplay-header-layout-impact.md).
This turn compiles fresh original/staged implementation pairs with their recorded
native flag vectors. The changed header provides the reason for these impact
builds; no passing native fixture is rerun.

## Actual first builds

All 123 included original/staged pairs compile successfully with zero observed
input drift. InitLUA remains explicitly excluded. The four implementation copies
use their single added display include; the other 119 retain canonical source
paths. Flags are fixed, original source bodies remain unchanged, and no source,
flag or compiler fitting is performed.

The comparison records ordered nondebug sections: complete payloads, symbolic
relocation target names/kinds, symbol offsets, section flags and sizes. It excludes
debug sections, file bookkeeping and COFF auxiliary/COMDAT metadata, so it does
not assert full raw-object or ownership equality.

Result: **118 ordered nondebug equals / 5 preserved differences / 0 compiler
failures**. Both complete raw objects, compiler/showIncludes output and section
projections are retained for every pair.

Batch:
`work/modern/header-seams/20261014/game-header-display-geometry-amended2/consumer-codegen-first/`.
Result SHA256:
`9dc0096f8d23fb1c5ab3f0efd19c8637458ccaac81a612c3a58f2534b7e371d3`.

## Five bounded factual witnesses

Two files change only an observed three-contribution cycle:

| Source | Complete unchanged contributions reordered |
| --- | --- |
| Frontend `Init2` 0042E98F | Integer extents constructor, frontend SetGameComponent and SetQuitFromGame |
| Frontend `Run` 0042EC7C | Two colour constructors and frontend-init constructor |

Each contribution's full bytes, symbols, relocation targets, flags and sizes
match under that exact observed cycle; no general reorder permission is granted.
The first read-only witness draft incorrectly assumed the same cycle direction
for both files and stopped. Its source is preserved at
`consumer-codegen-first/difference-witness-preflight-failure/initial_checker.py`.
The corrected checker binds each actual cycle separately. No compiler rerun or
original result rewrite followed.

Three files change compiler-generated `$L/$T` label numbers:

- `CDisplayEngine::RenderHeroDrunkenness` 00435070.
- `CDisplayEngine::RenderHeroRegion` 00435000.
- `CTCCreatureModeManager::RemoveMode` 0073ABA0.

Their entire ordered section payloads, flags and sizes are identical. Every local
label binds uniquely to the same actual section offset and label family; every
changed relocation points to that corresponding actual label. All nonlocal symbol
offsets and relocation targets match. This is evidence for these three exact
objects, not a generic label-normalization rule or header-profile waiver.

Witness receipt SHA256:
`1f01ed8dc3eecf4f6483c6c1a621448d2868d08ab7aafbade836e43c658ecd5a`.
Status:
`FIVE_FIRST_CODEGEN_DIFFERENCES_FACTUALLY_EXPLAINED_NO_ACCEPTANCE`.
The five original `NONDEBUG_DIFFERENCE_PRESERVED` records remain unchanged and
unaccepted.

## Remaining acceptance

Full raw-COFF auxiliary/COMDAT ownership metadata, complete transitive/test/runtime
consumer closure and required behavior/profile checks still precede promotion.
Code payload equality alone does not prove class lifetime, pointer ownership,
retail grading, integrated startup or x64 compatibility. Current input guards
are not historical complete environment equality or atomic compiler-read snapshots.

This batch made 246 justified impact compiler calls, retained all first objects
and made no linker/native invocation. Canonical headers/sources/index/grades,
dashboard, existing native receipts, HANDOFF and ROADMAP remain unchanged. No
Lua work, game/GPU launch, native fixture retry, commit or push occurred.
