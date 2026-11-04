# Corrected gameplay-header fixture runtime

Continues [fixture linking and selected body ownership](2026-10-14-gameplay-header-fixture-link-impact.md).
The previous turn yielded new link/runtime evidence; this turn extends that
evidence to the three remaining affected fixture pairs. The eleven-file patch
remains unapplied and the modern x64 engine fork remains gated.

## Reviewed and executed

`tools/modern/run_game_header_corrected_fixture_hosts.py` records the primary
agent's source/mock review before invoking the six retained original/staged
hosts. All six pass unchanged assertions. No compiler or linker rerun occurs.
Selected engine roots were already verified in the actual linked PE bytes.

The two destructor tests run the same four scenarios: active/final-reference
teardown with polymorphic pointers, quit/shared-reference teardown with null
polymorphic pointers, scalar deletion without freeing, and scalar deletion with
freeing. Their source lines compare equal; differing original byte hashes are
preserved. These are duplicate scenario sets, not eight independent cases.
Checks cover exact teardown event counts/order/receivers, flags and counted
pointer clearing, shared reference decrements and delete logging. The flag-one
case checks selected events and freeing rather than the complete teardown trace.
Fake vtables and raw A5-filled storage do not prove genuine engine lifetime.

Frontend Run checks intro failure followed by system quit, two attract-mode
frames followed by main-game handoff, dev frontend taking precedence over main
game, and plain quit. String comparison checks the exact recording-call log;
other assertions check selected field/global state and replacement components.
File/bank access, media, sound, font loading, render clear/swap, timers, system
update, input/update/draw and component startup are fixture stand-ins. Sleep(0)
on the two intro movies remains an actual Windows API call. No graphics or media
API executes. Random-movie expectation shares the seeded CRT rand routine and
does not independently establish random selection correctness. Script arrays and
the strcat logger have no general bounds guards; this review covers only the
four bounded scenarios. Raw receiver storage and local construction/lifetime
substitutes remain qualifications.

All three amended fixtures are used on both original/staged header sides. The
original-header side is an amended fixture baseline, not retroactive acceptance
of the untouched compile failures. Removing the duplicate empty base destructor
and invalid specialization marker preserves event loggers and assertions.

Result:
`work/modern/header-seams/20261014/game-header-display-geometry-amended2/fixture-links-first/corrected-fixture-runtime-first/result.json`
SHA256 `ac3d65a834f8bf74a46b347a3f6c6c287a09f15f9eb49d8b396b2a91f6100668`.

Together with the prior two next-present hosts, all eight linked affected hosts
now pass bounded runtime checks. This is a new recorded x86 host profile and
primary-agent review, not independent review, retail differential testing,
complete engine behavior or x64 readiness. Passing stdout reports are retained;
there are no independent per-case snapshots. Broader test/runtime sources still
have syntax-only evidence.

## Continue toward integrated frontend

Return to the frontend/menu composition proposal under
`work/native-recreation/frontend/0042df9e/20261014/menu-chain-root-support-amended1/`.
Inspect current files and prior owner closure before using them. Source
preparation, compilation and executable ownership evidence may advance locally;
the fresh native-composition source freeze and independent review gate must still
precede its first execution. Do not rerun already closed native coordinate or
frontend/window fixtures. This header-seam work grants no waiver to those gates.

Canonical code/tests, index/grades, dashboards, HANDOFF/ROADMAP and old native
receipts remain unchanged. No provider recompilation, retail-code execution,
game/GPU launch, Lua recovery, commit or push occurred this turn.
