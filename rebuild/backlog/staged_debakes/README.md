# Staged genuine de-bake sources — ALL 8 LANDED 2026-08-19

All eight templates below are landed genuine (91 bakes replaced, +16 new members,
107 total; harness re-verified MATCH/RELOCATION_MATCH). Landed with
`tools/decomp_pipeline/crawl/debake_family.py <template> <this-dir>/<src>.cpp <prefix> --apply`
followed by `verify_and_land.py <prefix>_land.json <prefix>_oracle.tsv --land`.
Sources are kept here as the family templates for future harvests.

## Original staging notes

Each is genuine compiler-emitted C++ PROVEN byte-exact (calls masked) via tools/decomp_pipeline/vchk.py
against the retail oracle. They replace existing naked `_emit` bakes. To land each: remove the
baked catalog/source/test/oracle entries for the template addr, verify_and_land the genuine
head, then harvest_skeleton to clone the family genuinely. In-source `#pragma` is honored.

IMPORTANT: verify only with vchk.py (Python subprocess). Manual `cl` in Git Bash mangles
`/flag` args via MSYS path conversion → bogus results. All findings below are vchk-verified.

| template | fn | len | family | pragma |
|----------|----|----:|-------:|--------|
| 004a88a0 | CCountedPointer::ShareData (67B) | 67 | 92 | (base) **ALREADY LANDED genuine** |
| 00412b00 | vector_deleting_destructor | 30 | 22 | (base) |
| 004477c0 | _Uninit_copy (8B stride) | 50 | 31 | (base) |
| 00448280 | _Uninit_copy (12B stride) | 56 | 7 | (base) |
| 004190e2 | ShareData (helper-release) | 38 | 8 | s |
| 0042ab1c | OnReadFinished | 23 | 8 | s |
| 0042a1b6 | BuildTreeArray (recursive free) | 45 | 5 | s |
| 00419463 | operator= (fwd to ShareData) | 23 | 4 | s |
| 0042abe1 | OnKill (CTCCarryable) | 41 | 6 | s |

Staged families total 91 functions across 8 templates (+ the 92 ShareData-67 already landed).
GetMeshEffect (47B x7): permuter-near-miss 49v47 — retail keeps cur in ecx for the virtual
call; hand-model regalloc picks eax (+1 `mov ecx,eax`). Needs permuter.
Still DEFER (need permuter): _Dest_val<…> (reloc-mask misalign, ~130), UpdateShadowScene
& SortTreeRecursively (per-instance immediate divergence). GetMeshEffect/OnKill have
dead-store/reload artifacts — retry with care.
