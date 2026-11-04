# Gameplay-header seam: affected fixture hosts

Continues [baseline fixture readiness](2026-10-14-baseline-fixture-readiness.md).
The proposed header/source/test patch remains unapplied. This is preparatory
platform separation in the existing x86 recovery, not an x64 engine build.

## Eight links and selected engine-body ownership

`tools/modern/check_game_header_fixture_links.py` built original/staged hosts for
four affected tests, reusing their eight retained provider objects from the
recorded 123-pair codegen audit. All eight fixture compilations and links passed.
There were zero provider recompilations. Cases are the main-component destructor,
its scalar deleting destructor, next-present-time calculation and frontend Run.

The three source-corrected fixtures use their amended copies on both sides. Thus
their original side is an explicitly corrected fixture baseline with the original
header, not the failing untouched test. The next-present fixture retains its
canonical source on the original side and adds only its explicit display header
on the staged side. Host compilation uses explicit `/Od /W3 /MT /GS`; the retained
providers keep their recorded flags. This is a new host profile, not historical
operational-profile identity. Compiler includes and library-directory inputs are
pinned before the run and guarded afterwards. Implicit library selection is not
an exhaustive linker-read trace.

Local result:
`work/modern/header-seams/20261014/game-header-display-geometry-amended2/fixture-links-first/result.json`
SHA256 `eb94528205ffcdec7af61e8cee750f99994e666725de20044b96260d312832b3`.

`review_game_header_fixture_link_ownership.py` checks the actual linker maps and
linked PE bytes for all eight selected engine roots. Every complete code-section
contribution matches its retained object after resolving every body relocation.
The map attributes each root to the intended original/staged provider object.
Repeated compiler-local labels are resolved by object ownership. The first
checker that incorrectly required globally unique local labels is retained in
`ownership-review-preflight-failure/checker.py`; it made no acceptance receipt.

Ownership receipt SHA256:
`de80e2ea8363e0ed952bedefe9b51220e73ce6ad12279048ecc511d8023986cd`.
Scope is the four selected roots. This does not accept every fixture helper,
COMDAT selection, mock, library contribution or genuine object lifetime.

## First bounded runtime check

After primary-agent source review, `run_game_header_next_present_hosts.py` ran
only the original/staged next-present-time hosts. Both passed the unchanged six
cases: future/past/equal presentation time, two old-timing cases and GPU count
ahead of CPU count. Assertions check exact double results, one timer call, one
clock call and the timer receiver. Raw zeroed component/display storage and a
borrowed timer remain explicit fixture/lifetime limitations. The two pass logs
are fixture reporting, not independent per-case snapshots.

Runtime receipt SHA256:
`7204d6bc8c44c1e1d6757377706993145b86d827ebfb8f665a1bb5aa38559fc6`.
This is bounded regression evidence under the recorded new host profile, not
independent review or a fresh retail differential test. No retail code, graphics
operation or game launch occurs. The other three linked fixture pairs remain
unexecuted pending their source/mock review.

## Continue

Review the three corrected fixture bodies and their recording boundaries, then
evaluate their linked hosts with the same qualifications. The remaining broad
test/runtime contexts still have syntax evidence only. Do not rerun the closed
coordinate or frontend/window native fixtures, substitute those results for
this header change, or claim engine-wide runtime or x64 readiness.

Canonical headers/sources/tests, indexes/grades, dashboards, old native receipts,
HANDOFF and ROADMAP remain unchanged. No commit, push or Lua recovery occurred.
