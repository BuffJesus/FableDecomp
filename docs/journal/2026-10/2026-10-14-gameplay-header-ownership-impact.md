# Gameplay-header seam: actual linker ownership metadata

The [first compiler-output batch](2026-10-14-gameplay-header-codegen-impact.md)
retains 246 actual objects for 123 original/staged pairs. This turn inspects their
raw COFF ownership metadata without another compiler, linker or native run.

## Actual ownership comparison

All 123 pairs match under the existing five exact factual witnesses. The reader
checks actual symbol-table storage classes, type/value/section bindings, weak
fallback target identities/search modes, and nondebug section auxiliary records:
length, relocation count, checksum, COMDAT selection and associative owner.
Objects are verified against the hashes saved by their actual compiler invocations.

Only the already recorded two contribution cycles and three actual local-label
offset mappings are used. All other metadata must compare literally. File/debug
bookkeeping and timestamps are excluded; this is linker-ownership equality under
specific observed mappings, not raw file byte identity or a general normalization
rule. The original five difference records remain unaccepted.

Receipt:
`work/modern/header-seams/20261014/game-header-display-geometry-amended2/consumer-codegen-first/raw_coff_ownership_metadata_review.json`.
SHA256:
`f9604c155be16caedb4424e9ebf6d416a3bfc013c759dfb12add9299d31c120d`.
Status: `RAW_COFF_OWNERSHIP_METADATA_FACTUAL_REVIEW_COMPLETE`.
123 equal pairs / zero unequal pairs / zero observed input drift.

## Scoped frontend behavioral evidence binding

The staged frontend draw object at `consumer-codegen-first/068/staged.obj` contains
the exact complete 477-byte typed Draw body and every identical call/data
relocation from the retained object used in the passing connected frontend/window
test. That prior result is bound by SHA256
`22f0ac55a28a84db96c514412a3b3c6f70eb8e4d692e1f9cc4abd1407bef5343`.
Its 3,145,728 comparisons are historical test scope, not new runtime results.

Scoped body identity receipt SHA256:
`a7cc1b8508714a95c65af745cb1df00d4ceb0d45443591b350a99a77073321c6`.
Status: `STAGED_FRONTEND_FULL_BODY_IDENTICAL_TO_TESTED_RETAINED_BODY`.

This proves machine-code reuse for that exact function. It does not expand the
borrowed-storage/common-service/FP/direct-QAE fixture domain, establish historical
profile equality, qualify other object sections or callers' behaviors, or close
the active owner's native packet. The old fixture was not restarted.

## Remaining work

Full transitive/forced/macro and test/runtime consumer closure remains open.
InitLUA stays capped. Required native profile/behavior/lifetime acceptance and the
ongoing canonical input audit still precede source/header promotion. The staged
five-file patch is unapplied, and no x64 engine build is claimed.

Canonical source/header/index/grades, dashboard, existing native receipts,
HANDOFF and ROADMAP remain unchanged. No compiler/linker/native invocation, Lua
recovery, game/GPU launch, commit or push occurred in this turn.
