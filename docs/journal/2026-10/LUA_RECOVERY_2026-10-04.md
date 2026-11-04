# Lua recovery — 2026-10-04

User resumed continuous Lua work. Active checkout `D:/Documents/FableTLC`, branch
`feat/novi-script-recovery`, HEAD `366c08e`. Preserved the shared dirty tree and
other active processes. Resumed owned batch 47 from
`work/codex_lua_mans_dialogue_state_20261003`.

## MansLover no-letter dialogue state promoted offline

The registered `V_SickChild` EC9D30 reviewed scalar row now names the one
quest `MansLoverState` read and its three scoped comparisons
`mansLoverDialogueState`. Native AB68/6B reads quest+6C; subtract/branch chain
selects state 0 pre-letter, state 2 man-love, and state 1/default hero-love
when health is positive. State 3 remains a default hero-love selection when
the separate health gate permits it. No converter or sidecar code changed.

The native fixture's expert vtable target was corrected from +0x24 to the
original 7E7399 +0x34 dispatch. Original-instruction probe passed 13,824
layouts across states -1/0/1/2/3/4, health bit patterns including negative
zero and NaN, wait/task/termination gates, owner/quest/GSI/stack/cookie
variation. Controlled engine and expert callees remain mocks; the original
branch, x87 health comparison, speech dispatch and stack/exit behavior run.

Existing four-way generated draft/readable Main scenarios: 864 passed on the
scratch candidate (before and after, state/health/wait/termination/cancellation
and movie handles). New review tests: 16 passed, including five stale
native/source/count reviews and five wrong-state dispatches in each style.
Fresh registered CLI before/after generation: 14 Lua files parse per style;
only MansLover changes. Draft has an exact inverse to the prior file after
removing the new name. Both styles preserve call order. Readable renumbering
and task-variable naming are confined to MansLover; draft/readable smoke JSON
are clean and identical before/after. Root native/evidence functions passed
after promotion. The broad related pytest runner was interrupted because
collection remained stalled under concurrent workspace work; the prior 2,633
related tests passed on unchanged shared converter code before this batch.
No broad-related completion is claimed for batch 47.

Guarded promotion wrote five owned files: unit evidence, new root regression,
draft/readable MansLover Lua, and readable report. The first promotion attempt
stopped before root writes because PowerShell's captured review log was UTF-16;
the BOM-aware read corrected the gate. Corpus audit: 371 Lua files, zero syntax
failures, 1,086 unresolved diagnostics, generic temporaries 6,206 -> 6,121;
statuses unchanged. The named offline candidate is
`work/codex_lua_mans_dialogue_state_20261003/mans-dialogue-state-candidate-20261004`:
34 Lua parse, 39 manifest hashes, Q_NewOakValeIntro/V_SickChild overrides,
sidecar source `07774e98c5675fb66aba7c23b4f1e4453b0434c0`, packaged DLL SHA256
`af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174`.
Installed v124 unchanged. No game launch, installation, or commit; actual
resource/string/camera ownership and story behavior remain runtime gates.

Next existing-unit work: isolate MansLover LeavingRegion's optional speech
random branch and `RetailRandModulo` parity, retaining one draw only on the
reached path, the subsequent task wait, and resource/movie/pause cleanup.
Inspect original ECAD/AE call sites and the existing generic RNG binding
before any evidence or generated change. Continue Lua recovery until user
asks to stop.

## MansLover optional departure speech RNG validated (test only)

Original ECAA7A flag test skips `rand` when LeavingRegion byte is zero.
ECAA85 calls the retail MSVCR71 import; ECAA8A/8F/91/92/95/96 reduces the
15-bit result to parity and enters optional speech on even values. The
existing `RetailRandModulo(32768)` sidecar calls the retail import and the
original CRT `rand` body masks to 0x7fff, so its Lua bit test has the same
reachable result and consumes exactly one draw. Existing native raw-random
test proves the CRT range. No binding or generator change was warranted.

The old departure harness always bypassed this branch (zero RNG draws).
A new saved LeavingRegion + one talk + absent letter scenario reaches it.
Original-instruction ECAA7A..ECAA9C/ECAB59 probe: 72 layouts over flag
0/1/255, rolls 0/1/2/3/32766/32767, owners and stacks; no draw for flag0,
one draw and even branch for nonzero. Generated draft/readable: 96 scenarios
with saved/unsaved state, those six rolls, health0/positive and wait0/2;
trace equality, exact draw count, optional speech key and cleanup pass.
Root `test_native_mans_departure_random.py` holds both checks; direct root
invocation passed. Test only; bundle, installed DLL, audit and game state
remain unchanged. More complete engine task/movie ownership remains a runtime
gate. Next inspect MansLover remaining scalar and Thing temporaries against
native ownership without renaming an operand on name alone.

## SickChild jabber index promoted offline

MansLover's remaining `iVar5` and `pCVar6` spellings span GSI table pointers,
speech flags, task results and Thing receivers; no single safe alias followed
from those names. The next existing SickChild diagnostic exposed a behavior
gap: retail EC60B3 initializes balanced-SP+1C to 10; EC61A9/AD feeds that
index to GFIntToCharString; missing text resets EBX to 10 at EC61F7; EC6237/3C
adds 10 and stores the next index. Generated Lua previously queued
`BOY_JABBERS_10` on every repeated conversation and left the stack update
as a TODO. The same stack slot is reused later as a real CString, so the
reviewed scalar alias applies only to the early index lifetime.

Owned scratch `work/codex_lua_sick_jabber_index_20261004` retains baseline
unit/source/native, before/after full registered generation and validation.
Native 96 original-instruction transitions pass across initial 10/high-bit
values, missing-text resets and stack positions. Draft/readable behavior
covers one to four conversations, absent index20 fallback, finish-state
cleanup and signed32 wrap. Baseline repeats key10; candidate advances
10/20/30/40. Eight stale native/source/count/alias reviews pass. Existing
bed/native evidence checks pass by direct invocation. Full registered CLI:
14 Lua parse per style, only SickChild changes; before outputs equal accepted
root. Draft/readable smoke JSON are clean and equal on their reached paths;
the targeted behavior harness exercises the newly reached repeated dialogue.
The broad related pytest collector stalled under concurrent workspace work;
no full-suite completion is claimed for this batch.

Guarded promotion wrote seven owned files: reviewed unit row, root regression,
draft/readable SickChild Lua, conversion JSON/Markdown and readable report.
Direct root native/generated/review regression passed after promotion. Audit:
371 Lua, syntax0, diagnostics1085 (down1), generic6121 unchanged; SickChild
status moves from needs-recovery to needs-readability-work. Named offline
bundle `work/codex_lua_sick_jabber_index_20261004/sick-jabber-index-candidate-20261004`:
34 Lua parse/39 verified manifest hashes, Q_NewOakValeIntro/V_SickChild,
same sidecar source `07774e98c5675fb66aba7c23b4f1e4453b0434c0` and DLL
SHA256 `af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174`.
Installed v124 unchanged; no game launch, installation or commit. Next inspect
SickChildsMother's remaining six unresolved diagnostics against their reached
native call sites, choosing one bounded behavior gap. Continue until stopped.

## SickChildsMother speech table loads promoted offline

The six diagnostics were three duplicated C table-pointer assignments in the
draft and readable files. Native ECDF31/ECE00E/ECE20D load the quest +0x48
`seh_Mother` resource speech table immediately before +0x34 dispatches at
ECDF4E/ECE028/ECE22A. The existing reviewed member-speech recovery already
restores those dispatches as `me:Speak`. The table-pointer assignments have no
remaining use after that restoration. Added an optional `deadTableLoads`
review to the shared member-speech adapter, gated by exact native windows,
load/call/receiver shape, resource classification, source count and full
function/source hashes. Only those three assignments are omitted.

Scratch `work/codex_lua_mother_speech_table_loads_20261004` retains baseline
and candidate adapter/evidence, all 29 registered-unit before/after builds,
smoke, behavior and adverse reviews. The full comparison found only
SickChildsMother changed in draft and readable. One Arena readable difference
appeared in the initial concurrent comparison; rebuilding Arena before and
after with the same fixed hash seed gave byte-identical output (SHA256
`ba97327e6fe75bfb06d795b0b968314f0d96bb57bfd0be1c7734b6247390c632`).
No Arena files were promoted. Draft/readable Mother smoke results are equal
before/after. A 256-case before/after behavior probe found no new trace
differences; 47 pre-existing style differences remained the same. The related
overlay test process returned zero with 9,407 passed, including 10 adverse
review cases. The promoted root regression passed 10/10.

Guarded promotion wrote the shared adapter, SickChild unit review, draft and
readable Mother Lua, conversion JSON/Markdown and readable report. Audit:
371 Lua, syntax0, diagnostics1079 (down6), generic6117 (down4). Named offline
bundle `work/codex_lua_mother_speech_table_loads_20261004/mother-speech-table-loads-candidate-20261004`:
34 Lua parse/39 verified manifest hashes, Q_NewOakValeIntro/V_SickChild,
sidecar source `07774e98c5675fb66aba7c23b4f1e4453b0434c0`, DLL SHA256
`af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174`.
Installed v124 unchanged; no game launch, installation or commit. Next inspect
the remaining SickChild-family diagnostics for a bounded native-backed
recovery. Continue Lua work until the user asks to stop.

## Witch speech table loads promoted offline

The same SickChild-family pattern appeared five times in Witch.Main. Original
ECECC6/ECED88/ECEE47/ECF46F/ECF6BB load the quest +0x58 `seh_Witch` table;
each value feeds only the immediately paired +0x34 speech dispatch. The
generated `me:Speak` calls already reproduce those dispatches, while five
decompiler `iVar10` assignments remained as TODO/nil pairs. The reviewed
dead-load gate now accepts the actual parent register only when the window
shows that register loaded from owner +0x14. Exact native/source hashes,
five source occurrences, load/call/receiver path and resource classification
still gate removal.

Scratch `work/codex_lua_witch_speech_table_loads_20261004` retains both
adapter/evidence versions and the fixed-seed 29-unit comparison. Only
SickChild Witch.lua changed in draft and readable; accepted SickChild before
inputs matched byte for byte. Original-instruction first-three speech probe
passed 12 owner/stack layouts; the existing native Witch probe covers the
later speech sites. Ten positive/adverse review cases passed. A 480-case
before/after speech and cleanup probe had identical traces per style, with
80 existing draft/readable differences unchanged. Both smoke styles were
identical before/after (13 files, zero problems). The related overlay process
returned zero with 9,674 passed, including Mother and Witch regressions;
post-promotion root review 10/10 and direct original-instruction probe pass.

Guarded promotion wrote the shared adapter, Witch unit review, draft/readable
Witch.lua, conversion JSON/Markdown and readable report. Audit: 371 Lua,
syntax0, diagnostics1069 (down10), generic6111 (down6). Named offline bundle
`work/codex_lua_witch_speech_table_loads_20261004/witch-speech-table-loads-candidate-20261004`:
34 Lua parse/39 verified manifest hashes, Q_NewOakValeIntro/V_SickChild,
sidecar source `07774e98c5675fb66aba7c23b4f1e4453b0434c0`, DLL SHA256
`af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174`.
Installed v124 unchanged; no game launch, installation or commit. Next
inspect the two SummoningTheShip `FadeInThing` diagnostics against the retail
helper and current sidecar; the existing `EntityFadeIn` wrapper has a
different call target and must not be substituted without native evidence.
Continue until stopped.

## SummoningTheShip retail FadeInThing recovered offline

SummonerAttacker.Main 0x00DF2380 and SummonerMinion.Main 0x00DF2AB0 each
called retail `FadeInThing` at 0x00CBE7AA with the embedded CScriptThing
(owner +8) in ECX and float 2.0 on the stack. The previously available
`EntityFadeIn` sidecar method calls a different GSI vtable slot. The retail
helper instead tests thing slot +0x12c, calls EntitySetAlpha at GSI +0x7f8
when alive, and calls NewScriptFrame at GSI +0x1c every step. Original byte
emulation over eight owner/stack/alive layouts found 30 frames, alpha starting
at 0 and ending near 0.966667 for live things, no alpha calls for dead things,
and RET 4 stack cleanup. The new x86 sidecar binding calls that exact helper;
the separate scratch MSBuild Release DLL compiled successfully.

A `reviewedFadeInThing` row for each registered function gates source
rewriting by full source/native hashes, direct call target and push/receiver
window, embedded thing path, helper hash/slots/ret, and retail float constants.
Only the reviewed synthetic call lowers to `me:FadeInThing(2.0)`. Scratch
`work/codex_lua_summoning_fadein_20261004` retains before/after source,
29-unit fixed-seed builds, an 11-case review suite, original helper probe,
and 16 reached/early-exit draft/readable behavior traces. Only Attacker and
Minion changed in either Lua style. Draft and readable smoke are identical
before/after; the draft's one quest free-global report is pre-existing, while
readable smoke has zero problems. Post-promotion root generation matches the
candidate byte for byte; root review 12/12 and previous Mother/Witch 20/20
pass. No in-game behavior has been claimed.

Guarded promotion wrote the generic converter/lifter branch, reviewed
adapter/evidence, two entities in each Lua style, reports and root
regression. Audit: 371 Lua, syntax0, diagnostics1067 (down2), generic6111.
New offline bundle
`work/codex_lua_summoning_fadein_20261004/summoning-fade-in-candidate-20261004`:
29 Lua parse/34 verified hashes, Q_NewOakValeIntro/Q_SummoningTheShip,
baseline sidecar source `07774e98c5675fb66aba7c23b4f1e4453b0434c0`,
three-file source patch SHA256
`da5cc6ffef2e5a71334c717b69efe8a93d2e9aeebd52708a21774e103301a712`,
new DLL SHA256 `68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 unchanged; no game launch, installation or outer-repo commit.
Next inspect remaining registered script diagnostics and select a bounded
native-backed recovery. Continue until user stops.

## SummoningTheShip Briar Rose summoner comments recovered offline

STS_BriarRose.Main 0x00DF1830 previously lost the `SummonersAlive` +0x48
range read, the three selected `SUMMONER_*_LEFT` keys, and the boolean return
from `MakeBriarRoseComment` at 0x00DF20F0. The native window 0x00DF1BDE-
0x00DF1C14 proves the 1..3 gate, call at 0x00DF1BF2, AL result test, and
cached-count update only after success. The reviewed adapter requires exact
decompile/body/window hashes and the direct call site before splitting the
decompiler's compound condition and exposing the selected key to Lua.

Scratch `work/codex_lua_briar_comment_20261004` retains the baseline hashes,
native review, fixed-seed 29-unit draft/readable comparison, ten Lua behavior
cases, smoke JSON, and guarded promotion inputs. Only STS_BriarRose.lua
changed in either style. Counts 0/4 skip the summoner key; counts 1/2/3 select
the corresponding key; a true helper result latches after one attempt, while
false retries on later frames. Root regression 3/3 and the eight-file smoke
comparison pass; smoke JSON is identical before/after with zero problems.
Post-promotion regeneration matches the candidate's nine Lua files byte for
byte in each style. Audit: 371 Lua, syntax0, diagnostics1063 (down4),
generic6111.

Offline bundle `work/codex_lua_briar_comment_20261004/briar-comment-candidate-20261004`
contains 29 parsed Lua files and 34 verified hashes, with Q_NewOakValeIntro
and Q_SummoningTheShip overrides. It carries the prior reviewed FadeInThing
sidecar DLL SHA256 `68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
The user shared ultracodex for workflow review; its staged, plain-file run
record ideas match the existing scratch evidence and acceptance gates, so no
runtime integration was needed. Next inspect Briar Rose's `} else {` native
branch that currently remains a generated TODO. Continue until user stops.

## SummoningTheShip Briar Rose wave else branch recovered offline

STS_BriarRose.Main 0x00DF1830 had a printed `} else {` line that the Lua
lifter treated as a TODO, placing the dead-target reacquisition inside the
`CurrentAttackWave == 1` path. Retail instructions 0x00DF19A8-0x00DF1A64
instead branch to that reacquisition when the +0x4C wave field differs from
1; the registered field is `CurrentAttackWave`. The reviewed adapter splits
the printed brace line for the existing lifter, gated by exact decompile,
native-body, and branch-window hashes plus the comparison/jump instruction
sequence. The readable pass proves one branch-local termination check dead
because no scheduler advance occurs between the loop guard and that check.

Scratch `work/codex_lua_briar_wave_else_20261004` retains the baseline
snapshot, fixed-seed 29-unit comparison, native review, before/after Lua
wave-transition fixture, adverse review cases, smoke JSON, and promotion
inputs. Only STS_BriarRose.lua changed in draft and readable. A held wave 1
does not reacquire a dead cached target; a 1-to-2 wave change does; wave 2
from entry and a living cached target do not reacquire. Root regression 3/3,
equal eight-file smoke with zero problems, and exact post-promotion nine-file
regeneration in both styles passed. Audit: 371 Lua, syntax0,
diagnostics1062 (down1), generic6111.

Named offline bundle
`work/codex_lua_briar_wave_else_20261004/briar-wave-else-candidate-20261004`
has 29 parsed Lua files, 34 verified hashes, Q_NewOakValeIntro and
Q_SummoningTheShip overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Next inspect other registered unresolved behavior and continue until stopped.

## Trader Escort EndTrader cleanup continuation recovered offline

EndTrader.Main 0x00E055B0 had one nested termination path that ran the
hoisted movie destructor and then returned at a `TODO(native)` continuation.
Retail instructions at 0x00E05A4D branch to 0x00E0600E, unpause at
0x00E06014, destroy the movie at 0x00E06021, destroy the conversation vector
at 0x00E0602A, and release the acquired resource at 0x00E0603C before RET.
The shared cleanup-region pass now tries its emitted-label `goto` after
hoisting and keeps it only when the whole Lua function parses under Lua 5.4;
an illegal jump into a sibling block retains the marked return. The existing
blocking-Speak cancellation pass can then reuse that same exit when FSE's
blocking `Speak` returns false. Retail Speak itself is void; the extra guard
is the reviewed sidecar wait/cancellation adaptation.

Scratch `work/codex_lua_endtrader_cleanup_20261004` retains the native
body/tail hashes, legal/illegal jump checks, before/after full-Main termination
traces, equal 16-file smoke JSON, and fixed-seed 29-unit comparison. Only
EndTrader.lua changed in draft/readable. The termination fixture proves the
accepted path omitted `ReleaseResource` and the candidate releases it after
destroying movie2, both for a terminating speech wait and a false blocking
Speak result. Root cleanup/native tests 11/11, blocking-speech tests 18/18,
and exact post-promotion 17-file regeneration in both styles passed. Audit:
371 Lua, syntax0, diagnostics1061 (down1), generic6111; the two restored
gotos raise the goto count to 4610.

Named offline bundle
`work/codex_lua_endtrader_cleanup_20261004/endtrader-cleanup-candidate-20261004`
has 37 parsed Lua files, 42 verified hashes, Q_NewOakValeIntro and
Q_TraderEscort overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## DragonBossFight Dragon health-phase else branches recovered offline

Dragon.Main 0x00D25A00 has three exclusive health phases. Retail branches
0x00D25AE6, 0x00D25B4C, and 0x00D25BAB select phase 0, 1, and 2;
stores at 0x00D25B12, 0x00D25B78, and 0x00D25BD3 advance DragonState.
The decompiler's two `else if` clauses printed as `} else {` plus `if` and
the Lua lifter emitted both braces as TODOs, leaving phase 1 and 2 nested
under phase 0. A native-gated adapter splits those exact printed lines for
the existing lifter. It checks the complete decompile and native body hashes,
the branch-window hash, and ten comparison/jump/store instructions.

Scratch `work/codex_lua_dragon_phase_else_20261004` retains baseline hashes,
the 29-unit fixed-seed draft/readable comparison, before/after Lua phase
fixture, adverse review cases, equal five-file smoke JSON, and promotion
inputs. Only Dragon.lua changed in either style. At health 40 in phase 1 and
health 10 in phase 2, the candidate advances to phases 2 and 3 and updates
TargetNumMinions; accepted Lua did not advance. Phase 0, phase 3, and
above-threshold cases remain unchanged. Root review 3/3 and exact
post-promotion six-file regeneration in both styles passed. Audit: 371 Lua,
syntax0, diagnostics1059 (down2), generic6111.

Named offline bundle
`work/codex_lua_dragon_phase_else_20261004/dragon-phase-else-candidate-20261004`
has 26 parsed Lua files, 31 verified hashes, Q_NewOakValeIntro and
Q_DragonBossFight overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Next recover the native `TargetNumSummoners` write at 0x00D25BFD, currently
marked TODO in Dragon.Main, with its three incoming global-data values.

## DragonBossFight summoner target write recovered offline

Dragon.Main 0x00D25A00 reads game-data offsets 0xBD4, 0xBD8, and 0xBDC
(3028, 3032, 3036) after its three health-phase transitions. Retail
instructions 0x00D25B40 and 0x00D25BA2 jump to the shared store at
0x00D25BFD; the third path falls through. That store writes the selected
value to quest field +0x6C, registered as `TargetNumSummoners`. The Lua
lifter lost the `this_00` parent alias across the shared label and emitted
the store as a TODO. A reviewed adapter materializes the direct parent
expression at the one write, gated by complete source and native-body hashes,
a store-window hash, and twelve global-read, paired-minion-write, branch,
and shared-store instructions.

Scratch `work/codex_lua_dragon_summoner_target_20261004` retains the
baseline hashes, native review, seven-case before/after Lua fixture,
fixed-seed 29-unit draft/readable comparison, five-file smoke outputs, and
guarded promotion inputs. Only Dragon.lua changed in either style. The three
advancing phases now set summoner targets 3028, 3032, and 3036 after their
existing minion targets; idle and above-threshold cases remain unchanged.
The smoke runner reaches the repaired path: both forms have zero problems
and equal frames/trace, with exactly seven additional state writes over its
eight-frame run. Root review 3/3 and exact post-promotion six-file
regeneration in both styles passed. Audit: 371 Lua, syntax0,
diagnostics1058 (down1), generic6111.

Named offline bundle
`work/codex_lua_dragon_summoner_target_20261004/dragon-summoner-target-candidate-20261004`
has 26 parsed Lua files, 31 verified hashes, Q_NewOakValeIntro and
Q_DragonBossFight overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Next inspect SpawnSummoners' two indirect position calls and creature-key
selection against retail instructions before recovering them.

## DragonBossFight summoner spawning recovered offline

Q_DragonBossFight.SpawnSummoners 0x00D26E60 indexes its +0x58
SummonerSpawnPoints vector with +0x64 NextSummonerSpawnPoint twice. Retail
indirect calls 0x00D26EFA and 0x00D26F87 use slot 0x18 to obtain each
entry's position, passing it to effect slot 0x190 and creature slot 0x16C.
The creature definition comes from the native two-way
CREATURE_SUMMONER_01/02 string selection. The generated Lua previously
passed nil positions and nil definition. A native-gated post-lift adapter
restores two `GetStateListAt(...):GetPos()` calls and the selected string,
requiring exact exported-source/native-body hashes, the four native slot
sites, registered index field, both creature literals, and the exact lifted
shape/diagnostics.

Scratch `work/codex_lua_dragon_spawn_review_20261004` retains baseline
hashes, decompile/instruction review, five before/after Lua behavior cases,
adverse review tests, fixed-seed 29-unit draft/readable comparison, equal
five-file smoke JSON with zero problems, and promotion inputs. Only
DragonBossFight.lua changed in either style. The behavior fixture covers both
creature keys, two positions, index wraparound, zero requested spawns, and
early termination. Root review 3/3 and exact post-promotion six-file
regeneration in both styles passed. Audit: 371 Lua, syntax0,
diagnostics1054 (down4), generic6117. The four removed diagnostics were the
two unresolved position calls and two missing spawn arguments.

Named offline bundle
`work/codex_lua_dragon_spawn_review_20261004/dragon-spawn-summoners-candidate-20261004`
has 26 parsed Lua files, 31 verified hashes, Q_NewOakValeIntro and
Q_DragonBossFight overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## BeardyBaldy local beard-helper calls recovered offline

BB_BeardyBaldyMan.Main 0x00E50FD0 calls the same entity's
IsHeroWearingBeard helper 0x00E53670 at native sites 0x00E524E0 and
0x00E52FED. The helper is already emitted in its Lua entity file, but the
caller lifter treated the parent `this + 0x14` operand as an extra helper
argument and generated two nil Boolean values. A native-gated post-lift
adapter calls the local helper with `quest, me` at both sites. It checks the
caller and helper source/body hashes, direct native targets, registered
entity-helper address, exact lifted call shapes, and four related diagnostics.

Scratch `work/codex_lua_beardy_helper_call_20261004` retains baseline
hashes, full Main before/after phase-3 dialogue fixture, adverse native review,
fixed-seed 29-unit draft/readable comparison, equal three-file smoke JSON
with zero problems, and promotion inputs. Only BB_BeardyBaldyMan.lua changed
in either style. With no beard, phase 2/3 dialogue is unchanged; with a beard,
phase 2 selects the beard-specific line and phase 3 reaches the finale line
instead of the wrong-ending line. Root review 3/3 and exact post-promotion
four-file regeneration in both styles passed. Audit: 371 Lua, syntax0,
diagnostics1050 (down4), generic6113 (down4).

Named offline bundle
`work/codex_lua_beardy_helper_call_20261004/beardy-helper-call-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_BeardyBaldy overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## GameflowAssistance renown message indexing recovered offline

GameflowAssistance.Tutorial_RenownLevelIncrease 0x00CF02A0 constructs seven
renown-level message strings and indexes the local array by the hero's
renown level. Retail 0x00CF0343 obtains the level, 0x00CF034C caches it in
EDI, 0x00CF03BA addresses the string array at `esp + edi*4 + 0x20`, and
0x00CF03BF calls the message API at vtable slot 0x448. Generated Lua had
collapsed that indexed string to the level-seven literal for every level.
A reviewed adapter emits a level-to-key table in the tutorial function and
uses the native index for levels 1 through 7. The existing level-seven key
is retained as fallback for out-of-range state. The adapter checks complete
source/native-body hashes, all seven native literals, the decompiled array
expression, and the index/call instructions.

Scratch `work/codex_lua_renown_message_20261004` retains baseline hashes,
0-through-9 before/after Lua traces, adverse review tests, fixed-seed
29-unit draft/readable comparison, equal one-file smoke with zero problems,
and promotion inputs. Only GameflowAssistance.lua changed in either style.
Levels 1 through 7 now send their matching keys; 0, 8, and 9 send none.
Saved-level updates and quest-card removals match accepted Lua. Root review
3/3 and exact post-promotion two-file regeneration in both styles passed.
Audit remains 371 Lua, syntax0, diagnostics1050, generic6113.

Named offline bundle
`work/codex_lua_renown_message_20261004/renown-message-candidate-20261004`
has 22 parsed Lua files, 27 verified hashes, Q_NewOakValeIntro and
GameflowAssistance overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## GameflowAssistance renown temporary-string teardown removed offline

Tutorial_RenownLevelIncrease 0x00CF02A0 creates a C++ local array of eight
CCharString values. Its native early-return paths destroy those temporaries
with 0x0099EAE0; the Lua lifter had emitted four decrement-only loops and
three `_vector_constructor_iterator_` TODOs for that teardown. Lua holds
the seven level messages as ordinary strings and has no native array or
manual reference count to release. A source/native-body-gated adapter removes
only those exact no-op loops, the unused Lua register counter, and the three
cleanup TODOs while retaining all termination checks and returns.

Scratch `work/codex_lua_renown_cleanup_20261004` retains baseline hashes,
30 before/after full-function traces across three renown levels and ten
termination timings, adverse review tests, fixed-seed 29-unit draft/readable
comparison, equal one-file smoke with zero problems, and promotion inputs.
Only GameflowAssistance.lua changed in either style. The traces have identical
state updates, messages, and quest-card removals; the draft's uninitialized
register arithmetic also disappears. Root review 3/3 and exact
post-promotion two-file regeneration in both styles passed. Audit: 371 Lua,
syntax0, diagnostics1047 (down3), generic6095 (down18). The tutorial
readable function has zero unresolved diagnostics.

Named offline bundle
`work/codex_lua_renown_cleanup_20261004/renown-cleanup-candidate-20261004`
has 22 parsed Lua files, 27 verified hashes, Q_NewOakValeIntro and
GameflowAssistance overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.


## SingingStones live-effect cleanup recovered offline

V_SingingStones.WatchForCompleteTune 0x00ED2BB0 calls
KillAllThingsInVector 0x00CBED82 at 0x00ED2F0A, 0x00ED312F, and
0x00ED32CC; WatchForRegionLeaving 0x00ED2A50 calls it at 0x00ED2B77.
The native helper checks each vector entry's IsAlive slot 0x12C and calls
RemoveThing slot 0x1B0 with false,true for live entries. All four sites
are followed by vector erase. The reviewed adapter uses
GetStateListCopy("EffectList"), IsAlive, RemoveThing(false,true), and
StateListClear. It gates caller source/body hashes, native direct sites,
helper semantic IR and Lua, and sidecar API source hashes. It removes five
unresolved TODOs and leaves unrelated lowering unchanged.

Scratch `work/codex_lua_singing_effect_cleanup_20261004` retains baseline
hashes, native adverse review, live/dead region-exit behavior cases,
fixed-seed 29-unit draft/readable comparison, equal three-file smoke with
zero problems, and promotion inputs. Only V_SingingStones.lua changed in
either style. Root review 2/2 and exact post-promotion eight-file Lua
regeneration in both styles passed. Audit: 371 Lua, syntax0,
diagnostics1042 (down5), generic6095. The accepted WatchForCompleteTune
control flow still has an unconditional break before its three cleanup
branches; a separate native-backed control-flow recovery is needed to
exercise those branches end to end.

Named offline bundle
`work/codex_lua_singing_effect_cleanup_20261004/singing-effect-cleanup-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_SingingStones overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## SingingStones playlist comparisons recovered offline

WatchForCompleteTune 0x00ED2BB0 compares four `YourPersonalPlayList`
entries at this+0x6C..0x78 against `PlayList` at 0x4C..0x58 and
`RudePlayList` at 0x5C..0x68. Retail tests `piVar6[-8]` and
`piVar6[-4]` against each personal entry, advancing the pointer four
times. The two generated `if false` placeholders forced the normal tune
branch and made both other outcomes unreachable. A source/native-body and
field-layout-gated adapter restores both indexed GetStateInt comparisons.
The readable optimizer then keeps the normal-tune break inside its true
match branch.

Scratch `work/codex_lua_singing_playlist_compare_20261004` retains
baseline hashes, adverse native/field tests, full-function before/after
normal, rude, and other-mismatch cases, fixed-seed 29-unit draft/readable
comparison, equal three-file smoke with zero problems, and promotion inputs.
Only V_SingingStones.lua changed in either style. Root review 2/2 and exact
post-promotion eight-file Lua regeneration passed. Audit: 371 Lua, syntax0,
diagnostics1040 (down2), generic6107 (up12 as the branches become reachable).
The remaining switch-built message suffix is unresolved and is a separate
native-backed recovery target.

Named offline bundle
`work/codex_lua_singing_playlist_compare_20261004/singing-playlist-compare-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_SingingStones overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## SingingStones personal tune name suffix recovered offline

WatchForCompleteTune 0x00ED2BB0 starts its mismatch message with
`TEXT_QST_060_NAME_`. Its four-entry loop reads YourPersonalPlayList at
this+0x6C, dispatches note values 0..3 to retail bytes A/B/C/D at
0x0122DCE0, 0x0122DCE4, 0x012D8858, and 0x012CD3C0, then calls
CCharString::operator+= at native 0x00ED2FBC. The generated Lua kept the
letter cases but lost the switch input and append operation. A
source/native-body, direct-call, retail-letter, field-layout, and exact
lifted-shape gated adapter reads each state entry and appends the selected
letter before AddLineToConversation. The native default skips an out of
range entry.

Scratch `work/codex_lua_singing_name_suffix_20261004` retains baseline
hashes, adverse native/field tests, 22 valid personal tune permutations and
two out of range full-function before/after cases, fixed-seed 29-unit
draft/readable comparison, equal three-file smoke with zero problems, and
promotion inputs. Only V_SingingStones.lua changed in either style. Root
review 2/2 and exact post-promotion eight-file Lua regeneration passed.
Audit: 371 Lua, syntax0, diagnostics1037 (down3), generic6108. The
V_SingingStones readable quest Lua has zero unresolved diagnostics.

Named offline bundle
`work/codex_lua_singing_name_suffix_20261004/singing-name-suffix-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_SingingStones overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch, installation, or outer commit.
Continue registered script recovery until the user stops.

## SingingStone hit and playlist path recovered offline

SingingStone.Main 0x00ED3370 sets cStack_4D true for a hero hit or a
special hit other than heal, and false otherwise. It compares the stone's
0x1C number with prior YourPersonalPlayList entries at parent 0x6C+4*i,
skips an already played stone, then writes the number into the current
playlist slot before increasing CurrentPlayListIndex at parent 0x48. A
source/native-body and quest/entity-field-gated adapter restores the two
hit-flag stores, two prior-note comparisons, and the selected-note store.

Scratch `work/codex_lua_singing_stone_play_20261004` retains baseline
hashes, adverse native/field tests, full entity Main before/after cases for
hero hit, special hit, excluded heal, no hit, repeated stone and a new
stone after one prior note, fixed-seed 29-unit draft/readable comparison,
and promotion inputs. Only Entities/SingingStone.lua changed in either
style. Both three-file smoke runs report zero problems; the no-hit mock
previously entered conversation and now waits for a frame, as expected.
Root review 3/3 and exact post-promotion eight-file Lua regeneration passed.
Audit: 371 Lua, syntax0, diagnostics1032 (down5), generic6111.

Named offline bundle
`work/codex_lua_singing_stone_play_20261004/singing-stone-play-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_SingingStones overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
The user authorized a game launch if it helps. This batch has no in-game
proof yet; installed v124 remains unchanged, and startup alone would not
exercise the SingingStones note path. No outer commit. Continue registered
script recovery until the user stops.

## SingingStone hit-created EffectList append recovered offline

After a valid hit in SingingStone.Main 0x00ED3370, retail creates a
MARKTELEPORTER effect at the stone position plus 1.0 z and appends its
CScriptThing to the parent EffectList vector at 0x7C. Native 0x00ED3840
calls the vector insertion helper when capacity is exhausted; the other
branch writes the copied thing and advances end by 0xC. The generated Lua
created the effect but left both vector branches as pointer/refcount TODOs.
A source/native-body, direct-helper-site, list-layout and exact-lifted-shape
gated adapter uses the existing StateListPush binding with the created
effect. The same entity already uses that binding in its prior-note effect
path.

Scratch `work/codex_lua_singing_effect_push_20261004` retains baseline
hashes, adverse native/list tests, a full entity-to-quest Lua fixture that
creates one effect then has WatchForRegionLeaving remove and clear it,
fixed-seed 29-unit draft/readable comparison, equal three-file smoke with
zero problems, and promotion inputs. Only Entities/SingingStone.lua changed
in either style. Root review 2/2 and exact post-promotion eight-file Lua
regeneration passed. Audit: 371 Lua, syntax0, diagnostics1023 (down9),
generic6111. SingingStone.lua now has six unresolved readable diagnostics.

Named offline bundle
`work/codex_lua_singing_effect_push_20261004/singing-effect-push-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_SingingStones overrides, and the prior FadeInThing DLL SHA256
`68baa8daa187f0b6f407a923c912778950beda3076d058709f008773f9997373`.
Installed v124 is unchanged; no game launch or outer commit. The user
authorized a launch if useful, but no established later-story runner
checkpoint reaches SingingStones; a startup launch alone cannot validate
this note/effect path. Continue registered script recovery until stopped.

## SingingStone effect name and position ABI recovered

Both native CreateEffectAtPosition calls in SingingStone.Main 0x00ED3370
pass `MARKTELEPORTER`, a position with z increased by the retail float
1.0 at 0x0122DED8, `StonesEffect`, and false/false flags. The hit path's
generated Lua omitted the script name; the prior-note path also passed
the unadjusted position and a hero object as the fifth argument. The
sidecar's CreateEffectAtPos binding always supplied an empty script name.
In isolated sidecar source at
`work/codex_lua_singing_effect_named_20261004/sidecar_candidate`, a trailing
optional script-name parameter now reaches the existing native API argument.
The Lua adapter is gated by native source/body, the retail float data,
sidecar source/build/patch/DLL hashes, and both exact lifted call shapes.

The x86 Release sidecar build succeeded. Source base is
`07774e98c5675fb66aba7c23b4f1e4453b0434c0`; exported patch SHA256 is
`8ee190cf7a5b68f0a89cef3fcc653284090230cdb463f30850b0f15ff9f48b81`,
and candidate DLL SHA256 is
`2a134819497130c27076cf9e4b29dea0759990b55ec17badd82f77bdf98d7a3a`.
Scratch retains full entity argument cases for hit and prior-note effects,
native/sidecar adverse tests, fixed-seed 29-unit draft/readable comparison,
equal three-file zero-problem smoke, and promotion inputs. Only
Entities/SingingStone.lua changed in either style. Root review 2/2 and
exact post-promotion eight-file Lua regeneration passed. Audit: 371 Lua,
syntax0, diagnostics1020 (down3), generic6111.

Named offline bundle
`work/codex_lua_singing_effect_named_20261004/singing-effect-named-candidate-20261004`
has 24 parsed Lua files, 29 verified hashes, Q_NewOakValeIntro and
V_SingingStones overrides, and the new sidecar DLL above.

The user authorized a game launch. A save-safe startup probe staged an
existing adult checkpoint with runner tag `singing_named_startup_20261004`.
The game loaded the new DLL and logged “Original FSE callback chained;
independent New Oakvale hosts enabled.” The frontend driver then failed to
reach Load Game, so no save, quest, or effect was exercised in game. The
runner closed Fable and restored the staged profile; all 561 save files
and both checked installed DLLs match prelaunch SHA256 snapshots exactly.
Evidence is in `work/runner/singing_named_startup_20261004_FableScriptExtender.log`
and the scratch runtime baseline. Installed v124 remains unchanged; no
outer commit. Continue registered script recovery until the user stops.

## SingingStone temporary pointer teardown removed

Retail SingingStone.Main 0x00ED3370 releases a C++ counted temporary after
the frame loop and returns through LAB_00ed3906. The lifted block instead
created a nil-only condition, two unresolved refcount TODOs and an unreachable

`nil:GetName()` branch. A source/body-hash-gated adapter removes exactly that
block while preserving the return label and other control flow. Six full-Main
traces (hero hit, special hit, excluded heal, no hit, repeat note, new note)
match before and after. Adverse native-body and lifted-shape gates pass.

Scratch is work/codex_lua_singing_temp_cleanup_20261004. Fixed-seed
29-unit comparison changed only Entities/SingingStone.lua in draft and
readable output. Both three-file smoke reports are equal with zero problems;
root review 2/2 and exact post-promotion eight-file Lua regeneration pass.
Audit: 371 Lua, syntax0, diagnostics1017 (down3), generic6106 (down5).
The named offline bundle
work/codex_lua_singing_temp_cleanup_20261004/singing-temp-cleanup-candidate-20261004
contains 24 parsed Lua files and 29 verified manifest hashes. It carries
the previously built named-effect sidecar DLL SHA256
2a134819497130c27076cf9e4b29dea0759990b55ec17badd82f77bdf98d7a3a.
It is neither installed nor gameplay tested. The prior startup-only probe
remains the available game evidence; no further launch was needed for this
cleanup. Preserve other dirty lanes and continue registered-script recovery
until the user stops.

## SingingStones camera name-and-offset calls recovered

WatchForCompleteTune 0x00ED2BB0 has two native slot-0x670 calls that pass
`CAM_Stones`, null offset and rotation pointers, duration -1.0, and flags
0/-1. The previous generated Lua supplied a nil camera name and five
misordered arguments. The stock sidecar name-and-offset overload required
tables, so the scratch x86 Release sidecar now receives those positions as
`sol::object` and passes native null pointers when Lua supplies nil. A
compiled sol probe checks that two explicit nil arguments preserve the
following float and flags, and that table arguments still bind. The native
and sidecar source/body hashes, sidecar patch and DLL hashes, exact two
lifted call shapes, and adverse cases gate the adapter.

Scratch is `work/codex_lua_singing_camera_20261004`. Full Lua traces show
both rude and other mismatched tunes now call the camera with the retail
six arguments; the normal tune still makes no camera call. Fixed-seed
29-unit comparison changed only V_SingingStones.lua in both styles. Both
three-file smoke reports are equal with zero problems. Root review 2/2,
exact post-promotion eight-file Lua regeneration, and package validation
pass. The converter reports two fewer TODO diagnostics for this function;
full readability audit remains 371 Lua, syntax0, diagnostics1017,
generic6106. The named offline bundle
`work/codex_lua_singing_camera_20261004/singing-camera-candidate-20261004`
contains 24 parsed Lua files and 29 verified manifest hashes. Exported
combined sidecar patch SHA256 is
`17cc739922a886cc39df9311b115a925a1fadd87847470c750c3e17a4f098b40`;
DLL SHA256 is
`e777ce0ef6d82822f41ab7371f80f37ebec9786effc99c47c4de616ace42ec2c`.
No install, gameplay test, or outer commit. The user is using their screen
again and asks for permission before any visual game launch. Continue
registered-script recovery offline until the user stops.

## Wasp Boss world-map offset restored

WaspBoss.Init 0x00E0E820 calls retail slot 0x440 with the Q_WaspBoss
string and a C2DCoordI holding x=10, y=10. The decompile initializes
both adjacent 32-bit members to 10; the existing sidecar maps that slot
to SetQuestWorldMapOffset(name, x, y). Generated Lua previously supplied
only x and an unresolved nil y. The new adapter is gated by native source
and body hashes, exact native and lifted call shapes, and adverse tests.

Scratch is `work/codex_lua_wasp_offset_20261004`. A full Init mock confirms
only the map offset changes from (10, nil) to (10, 10). Fixed-seed 29-unit
comparison changed only WaspBoss/WaspBoss.lua in both styles. Both ten-file
smoke reports are equal with zero problems. Root review 2/2, exact
post-promotion 11-file Lua regeneration in each style, and bundle syntax
and manifest checks pass. The converter reports one fewer TODO for this
function; full readability audit remains 371 Lua, syntax0,
diagnostics1017, generic6106. Named offline bundle
`work/codex_lua_wasp_offset_20261004/wasp-offset-candidate-20261004`
has 31 parsed Lua files and 36 verified manifest hashes, with the prior
camera sidecar DLL SHA256
`e777ce0ef6d82822f41ab7371f80f37ebec9786effc99c47c4de616ace42ec2c`.
No install, gameplay test, or outer commit. Ask the user before any visual
game launch; they are using their screen again. Continue native-backed
registered Lua recovery offline until stopped.

## Tour Guide follower target retained on second follow

TourGuideFollower.Main 0x00EE4DD0 loads TourGuideGuide into a script-thing
slot and passes that same slot to two native FollowThing calls. The native
release of the slot is on a termination branch; the active conversation
loop retains it. Generated Lua previously passed nil at the second call.
A native source/body-hash-gated adapter restores the existing guide
receiver and rejects changed lifted shapes. The full Main fixture reaches
both calls and confirms the guide is reused; an adverse native-body case
also passes.

Scratch is `work/codex_lua_tour_follower_20261004`. Fixed-seed 29-unit
comparison changed only Entities/TourGuideFollower.lua in both styles.
The four-file smoke reports are identical with two baseline findings:
WatchForNoFollowers reports an unknown MsgIsRegionUnloaded method, and the
mock invocation of MoveToNextWaypoint indexes a number as a position.
Neither is in the changed entity. Root review 2/2, exact post-promotion
five-file Lua regeneration in each style, and package syntax and manifest
checks pass. The converter reports one fewer TODO for this function;
full readability audit remains 371 Lua, syntax0, diagnostics1017,
generic6106. Named offline bundle
`work/codex_lua_tour_follower_20261004/tour-follow-target-candidate-20261004`
has 25 parsed Lua files and 30 verified manifest hashes, with the prior
camera sidecar DLL SHA256
`e777ce0ef6d82822f41ab7371f80f37ebec9786effc99c47c4de616ace42ec2c`.
No install, gameplay test, or outer commit. Ask the user before any visual
game launch. Continue registered Lua recovery offline until stopped.

## Trader Conflict awareness operand recovered

TraderConflictGood.WatchForHittingEnemies 0x00DFC630 calls retail slot
0x9F0 with the hero and the current inner-loop AllCreatures entry. The
second operand is retained in pCVar7 just before the call. Generated Lua
previously passed nil. A native source/body-hash-gated adapter restores
that existing entry without adding a list lookup. Full watcher fixtures
cover both inner-loop indices, distance true short-circuit, awareness
false, and awareness true, preserving targeting side effects. Native-body
and lifted-shape adverse gates pass.

Scratch is `work/codex_lua_trader_awareness_20261004`. Fixed-seed 29-unit
comparison changed only TraderConflictGood/TraderConflictGood.lua in both
styles. The readable pass folds the corrected condition into an equivalent
short-circuit; the focused cases cover that fold. Both 15-file smoke
reports are equal with zero problems. Root review 2/2, exact
post-promotion 16-file Lua regeneration in each style, and package
syntax and manifest checks pass. The converter reports one fewer TODO
for this function. Full readability audit: 371 Lua, syntax0,
diagnostics1017, generic6106, gotos4605 (down3), nativeNames6348
(down5). Named offline bundle
`work/codex_lua_trader_awareness_20261004/trader-awareness-candidate-20261004`
has 36 parsed Lua files and 41 verified manifest hashes, with the prior
camera sidecar DLL SHA256
`e777ce0ef6d82822f41ab7371f80f37ebec9786effc99c47c4de616ace42ec2c`.
No install, gameplay test, or outer commit. Ask the user before any visual
game launch. Continue native-backed registered Lua recovery offline until
stopped.

## Bedtime pause: Trader Escort sound recovery remains scratch

The user asked to stop for the night and shut off the computer. I stopped the
fixed-seed 29-unit comparison; 40 comparison logs exist, 38 completed and two
empty at interruption. No comparison process remains. No new root converter,
evidence, generated Lua, sidecar, or package was promoted from this batch.
The temporary root regression test was removed before pausing.

Scratch `work/codex_lua_trader_sound_20261004` holds a native-hash and
instruction-window-gated candidate for the shared +0x3ac CDefString vector.
BanditCronies.Main and DarkwoodAssassinSpawn.Main now sample one sound name
through the already packaged `ReadGlobalGameDataDefStringAt` binding; the
Assassin reuses it at both later distance gates. Candidate conversion parses
all 16 Trader Escort Lua files (todo 60 -> 53). Four focused tests pass,
covering full draft/readable calls, one random draw, reuse, early termination,
and adverse source/native changes. Controlled smoke with a three-entry vector
passes 16 files/zero problems before and after. Stock smoke uses `1` for both
vector pointers and reaches modulo zero in the candidate; that mock is not
retail vector data. The prior camera sidecar DLL e777ce0e contains the vector
reader. The comparison's completed target and unrelated units changed only
the two intended Trader Escort entity files, but full29 is incomplete and
must be rerun before promotion. No game launch occurred; ask before any
future visual launch. Resume Lua work only when the user asks.
