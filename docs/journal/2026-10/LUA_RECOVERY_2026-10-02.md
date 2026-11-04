# Lua recovery — 2026-10-02

Resumed from the 2026-10-01 handoff: ArenaCellExitGuard.Main (native 0x00F1B9C0)
speech recovery. Scratch: `work/codex_lua_arena_exit_guard_speech_20261002`.

## ExitGuard speech: precise failed check

`native_reviewed_resource_speech._recover` raised at the hero-listener source check
(line 119): the module required the listener argument to end in `pCVar6` and
`pCVar6 = GSI->GetHero();` to appear anywhere earlier. Ghidra names this function's
hero temporary `pCVar11`. The native side was already proven (EAX pushed straight
from the `call [reg+280]` GetHero). Yesterday's hypothesis (hardcoded hero variable)
confirmed; frame depths 312/316 were not involved.

Fix: the listener argument must be a plain (optionally `(CScriptThing *)`-cast)
variable whose *latest* assignment before the call is `GSI->GetHero()`. Stricter
than before for promoted rows (an intervening reassignment now rejects). Guard1/2
rows re-verify unchanged (restore compares the full witness inventory).

Witness: 10 Speak / 24 IsPerformingScriptTask / 2 ClearCommands, all listeners hero,
six keys (FIRST, FIRST_EXTRA, SECOND, STAYING, LEAVING, ATTACKED). Row registered
under `reviewedResourceSpeech["0x00f1b9c0"]` (purely additive unit edit).

## Pre-existing SetQuestAsFailed operand defect (found by reviewing the diff)

Arena's Ghidra spelling of the thiscall is `CCharString__AssignFromWide(&DAT_0122d70c)`
— ECX (the destination stack string) is dropped, unlike other units'
`AssignFromWide(&CStack_8,0x122d70c)`. The lifter then saw `pCVar18 = &xStack_ec` as
a non-string, put it in the `useWideMessage` bool slot, dropped `bVar19`, and filled
`wideMessage` from a stale temporary: promoted readable was
`SetQuestAsFailed(name, true, "", scratchValue45)` / `(…, nil --[[missing]], …)`;
with speech recovered the stale temp became the LEAVING speech key. Native
(0xF1C076..0xF1C0A8): `push L""; lea ecx,[esp+0x54]; call AssignFromWide;
push 1; push &slot; push 1; push GetActiveQuestName(); call [edi+0x484]`.

New `tools/script_recovery/native_wide_assign_receivers.py`, hooked after the reviewed
restores (so their lowered-source hash gates are untouched — a pre-lowering hook
correctly tripped the constructor row's gate and was moved). Pairs printed calls with
native sites through `callOrder`; each site must be exactly push-literal / lea ecx
[esp+d] / call 0x99B6B0 with `d - depth == ecxStack`, printed literal == UTF-16 bytes,
the slot name unique in the text. Any disagreement leaves the function unchanged.
Both sites now: `SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)`.
Retail order quirk preserved: one site sets MissionFailed after the fail call, the
other before (verified in native).

## Validation

- Behavior (`behavior_speech.py`, physical-frame termination model): 92,160 cases
  per style (ArenaState 3/5, six talk/hit/spell/idle plans, yes/no, pending answers,
  task waits 0/1/3, termination frame 0..39, health 0/100, busy, bool/always-true
  frame returns, near/far marker). Zero errors, resources and movies always closed,
  all six keys with native speaker/listener/flags, 20,928 fail calls with the native
  operands, draft and readable observable traces identical in every case.
- Adverse: the same harness rejects the promoted baseline (draft 89,856 failures —
  `me:IsPerformingScriptTask` receiver; readable 88,512 — `SetQuestAsFailed(…, nil)`).
- 15 new tests (`test_native_reviewed_resource_speech_exit_guard.py`): operand/wait
  counts, stale/renamed hero listener rejected, row drift rejected, wide-assign
  recovery and seven disagreement cases leave source unchanged.
- Broad suite: 953 passed + 43 subtests.
- Full 29-unit A/B (before = snapshot converter + pre-change speech module, new row
  removed): only `Arena/Entities/ArenaCellExitGuard.lua` changes; before == promoted,
  after == prototype. bandit_camp/chicken_kicking/guild_training (other
  AssignFromWide users) unchanged.
- Smoke 16 files per style: idle runs identical before/after; existing findings
  unchanged (draft 4, readable 2: InitialiseArenaRounds pending, extraout_EAX,
  ArenaEnemy.OnPredicateFail nil arithmetic).

**PROMOTED OFFLINE** (draft + readable ExitGuard, reports). Corpus 371 parse;
86 needs-recovery / 130 needs-readability / 155 human-review (was 87/129/155).
ExitGuard readable has zero TODOs. Arena still NOT packaged or game tested (v124
bundle unchanged). No commit.

## ArenaEnemy round-snapshot reads, creature counters, health bar

Scratch `work/codex_lua_arena_enemy_20261002`. Promoted ArenaEnemy had 5 diagnostics
and a runtime defect beyond readability: OnPredicateFail never decremented the
quest's `TotalCreatures_<group>` counter nor awarded DeathScore crowd points (the
draft crashed on nil arithmetic — the existing smoke finding), the Main group loop
that sets `myCreatureType` was `if false`, and the big-creature health bar call was
a TODO with `healthBarIndex = nil`.

Native (0xF1A990 / 0xF1A8F0): entity reaches the quest via `this+0x14`; rounds
stride 0x38, waves 0x3C, groups 0x38 — exactly the fields `native_arena_rounds` and
the sidecar's InitialiseArenaRounds snapshot expose (wave +0x28 NumWaveCreatures,
group +0x28 CreatureType, +0x34 DeathScore). TotalCreatures = quest +0xD0 stride 4
(unit member map). Colour bytes F1AC84..F1AC98: B=0, G=R=A=0xFF (yellow); BGRA order
corroborated by Aeon's FireHeart port (native 0xff00ff00 → green).

New `tools/script_recovery/native_reviewed_arena_enemy.py` (hooked after the wide-assign
repair): body sha256 gates for both functions; exact-count rewrites into spellings the
lifter already supports (`QUESTSTATE_Get/SetInt(__key(...))`, `ENGINE_StrCmp`,
`ENGINE_Colour`); group byte cursor proven to step 0x38 in lock step with the index
from zero; all-or-nothing per function. Inline CCharString equality treated as string
equality (non-empty def names).

Validation: behavior harness vs a Python oracle of the native semantics (last
matching group wins, none → 0, duplicates, all three big-creature icons, hero /
non-hero kill) — 40/40 both styles; promoted baseline 40/40 failures (draft crash,
readable silently skips counters). 10 new tests (drift in native bytes, cursor
stride/start, colour, offsets → unchanged). Broad suite 963 passed + 43 subtests.
- 29-unit A/B: only `Arena/Entities/ArenaEnemy.lua`; before == promoted, after ==
  prototype. Smoke: only ArenaEnemy delta — draft OnPredicateFail crash gone, both
  styles read the snapshot / write the counter, no unknown or pending methods.

**PROMOTED OFFLINE.** Corpus 371 parse; 85 needs-recovery / 131 needs-readability /
155 human-review; unresolved diagnostics 1193 → 1188. Not packaged / game tested.

## WhisperAlly, Roth, FireHeart: pause guard, dword colours, creature totals, IsAlive

Scratch `work/codex_lua_arena_whisper_20261002`.

Two generic lowering fixes in `native_evidence_lowering.py`:
- CWideScreenMagicPauseEntities inlined dtor with an empty argument list. At
  WhisperAlly F232D9 the dtor's `push 0` precedes a conditional jump shared by both
  dtor sites (F232E3 / F242E3), so Ghidra printed `()`. The guard ctor (0xCBE09A:
  stores the interface, calls slot 0x5EC with 1) is the only writer of that slot.
  Roth had the same shape: its readable called `PauseAllNonScriptedEntities(thing ~= 0)`
  / `(scratchValue23 ~= 0)` / `(switch3 ~= 0)` from stale temporaries — truthy, i.e.
  cleanup would have left the world paused. Now `false`; 3 Roth TODOs gone.
- `fold_dword_colours` accepts the slot's type cast on the stored literal
  (`(CCharString)` / `(CRGBColour)`) and a bare `&slot` for slots declared CRGBColour.
  Fixes WhisperAlly's ally health bar (yellow), its round bar (raw `0xffff0000` was
  passed where ForgeFSE expects a colour table) and SummoningTheShip FireHeart's bar
  (green — identical to Aeon's independent port).

New `native_reviewed_whisper_ally.py` (Main body sha, wrapper vtable entry and
IsAlive body gated): three TotalCreatures sums (byte cursor 0xD0..0xDC step 4 → index
0..2) get their own `creatureTotal` local (the -0x108 slot also carries conversation
ids and a timer; every path back to the outer while test passes the end-of-body
re-sum at LAB_00f241aa); F23A3D/F23E37 wrapper slot 0x12C = 0x004AB130
CScriptThing::IsAlive (same 28-byte body the GTDI_Maze witness verified).
WhisperAlly now has only the 4 `MsgHitEnemyWithMeleeWeapon` TODOs — a ForgeFSE
binding gap, not converter work.

Validation: 8 new tests; broad 971 passed + 43 subtests; 29-unit A/B changes only
Roth, WhisperAlly, FireHeart (before == promoted for both units, draft and
readable); smoke no errors/unknown methods; WhisperAlly idle trace grows because the
totals are now real reads. **PROMOTED OFFLINE.** 84 needs-recovery / 132
needs-readability / 155 human-review; diagnostics 1173.

## Roth: constructor, speech, actor binding; callOrder dtor pairing (corpus-wide fix)

Scratch `work/codex_lua_arena_roth_20261002`. Promoted Roth was badly broken at
runtime, not just unreadable: its inlined resource (slot -0x144) was never assigned
(`resources:NewResource()` result discarded, every Prepare/TryAcquire/Speak got nil);
none of its 9 lines were spoken (Speak dropped); the CS_ARENA_ROTHWIMP cutscene never
bound "Roth" (SetActor TODO); 8 health tests were TODOs so `if pCVar22_b3` read nil;
cleanup called DestroyMovie on the resource and ReleaseResource on a movie.

Fixes (all native-gated, each with tests):
- Lifter `RE_LOCAL_ASSIGN` accepts the lowering's own byte-split names (`xVarN_bK`).
- Speech hero check also accepts a cast on the GetHero assignment
  (`p1 = (undefined **)GSI->GetHero();`). Roth row: 9 Speak / 20 task / 2 Clear.
- Constructor row for Roth (frame 348, slot -324; ctor F2188C..F218B2, dtor
  F22C5D..F22C71, 6 acquisitions). New optional `actorMapResourceSites` in the
  constructor module: F21CBD (setter 0xCD3D2E, map at -188, pushes -324) →
  `SetActor(actorMap, "Roth", resource)`.
- Lowering: staged C3DVector member copy into a named local (`uVar14 = V.x; ...;
  center._4_4_ = uVar17; ...`) → `center = ENGINE_VectorCopy(V)`.
- **Label pairing** (`disambiguate_call_labels`): bsim gives the resource dtor
  0x7E74D0 and the movie dtor 0x6E7B80 one label. When the whole-text reconciliation
  fails, pairing fell back to *address* order, which differs from print order. Now
  falls back to this label's own `callOrder` (the decompiler's print order) when it
  covers every site. Corrected outside Arena too: bordello Madame, GTDI_Maze, SickChild
  ManInLove each had a cutscene movie "released" instead of destroyed (all three
  objects verified as StartMovie results); Flick/Needle/Shadow cleanups now release
  their resource instead of DestroyMovie(nil). Magicman: cosmetic temp renumbering.
  Two reviewed rows had compensated for the old pairing: Guard2's constructor row
  (rewrite retargeted to the alias spelling with an explicit occurrence; post-restore
  text byte-identical, so its speech/cleanup rows are untouched) and the BordelloLady
  lifetime module (accepts the corrected-kind spelling; BordelloLady output unchanged).
  Legacy rows still validate (test added).

Validation: Roth harness 47,040 cases per style — 0 failures, all 9 lines from Roth to
the hero, cutscene binds Roth, pause balanced, every resource/movie/map/timer cleaned,
draft == readable traces; promoted baseline fails all 47,040. Broad 981 passed + 43
subtests. 29-unit A/B: only the 8 files above (before == promoted, draft and
readable, all four units). Smoke: only Roth's idle trace (+1 call); bordello, GTDI,
SickChild identical. **PROMOTED OFFLINE.** 83 needs-recovery / 133 needs-readability /
155 human-review; diagnostics 1155.

Found, not yet fixed: Flick, Needle and Shadow have Roth's inlined-constructor defect
(`meControl` never assigned, 12-16 uses each) and need constructor + speech rows.

## Flick, Needle, Shadow + lifter flow fixes

Scratch `work/codex_lua_arena_mates_20261002`. Same defect as Roth: inlined stack
resource never assigned (`meControl` nil) so all 23 of their lines were silent and
every talk/hit errored (`invalid resource nil` in 26,880 of 33,600 harness cases each).

- `prepare_rows.py` discovers the constructor/destructor anchors from the bytes and
  asserts every shape (frame = sub esp + 16; slots -160/-172/-208; ctor xor/lea/mov/
  call 0x99A380, push, vtable 0x127094C, zeros +8/+12; dtor lea/zero/zero/base vtable/
  call 0x99A430), then builds speech rows: Flick 6, Needle 7, Shadow 10 Speak (hero).
- `native_receiver_reaching._reaching`: a predecessor-less MSVC alignment filler
  (`lea ecx,[ecx]` after a jmp, Needle F15B8D) no longer counts as an entry path.
  Side effect, verified correct: Madame's presented-item out-parameter now resolves to
  its real slot (-0x17C, constructed empty, filled by MsgIsPresentedWithItem) instead of
  -0x178; the Madame presented-item module accepts that spelling (same repair).
- Lifter: (1) at `else` / `else if`, register (`xVarN`) scalar kinds revert to their
  kind at the `if` — Flick's `else { if (iVar9 != 1)` lifted as `not iVar9` because
  the then-arm reused iVar9 for a bool (THIRD_HINT unreachable). Same fix in
  BB_BeardyBaldyMan and MansLover (`elseif iVar == 1`). (2) a declared `char` stored
  from char literals and never from a call result is a byte flag: boolean stores become
  0/1 so `== '\0'` tests hold in every state (DoorGuard c_stk_1f5, Assassin1,
  CampHostageGuard, ChickenMaster, CombatApprentice, TheRealGuildmaster(Will)).
  Restricting to registers and excluding call-stored slots was necessary: the first
  versions shifted Madame's operand guesses (GetTimer(4) -> nil) and mixed
  representations in DarkwoodTrader; both reverted before promotion.

Validation: Needle/Shadow 33,600 cases per style 0 failures, all lines, draft ==
readable. Flick draft 0 failures 6/6; Flick readable 0 failures in the bool-return
frame mode (ForgeFSE `NewScriptFrame(me)` returns `!terminating` for Arena), 12,720
failures only in the always-true (Oakvale-lifetime) mode from its pre-existing
frame-return idiom — recorded, not hidden. DoorGuard: yesterday's harness 3,072 cases
per style still 0 failures. 5 new lifter/reaching tests fail on the baseline modules.
Broad 986 passed + 43 subtests. 29-unit A/B: Arena (DoorGuard, Flick, Needle,
Shadow), BanditCamp (Assassin1, CampHostageGuard), BeardyBaldy, Bordello (Madame),
ChickenKicking, GuildTraining (2), SickChild (MansLover); smoke identical outside Arena.
**PROMOTED OFFLINE.** GuildTraining: only the two draft files — its promoted
`draft` (3 files) and `readable_converter` trees already drift from the current
converter (pre-existing, untouched). Several touched quests were previously played
in-game; these are refs-only changes, the v124 bundle is unchanged.

## String slots assigned by CCharString::operator=(slot, "LIT")

Scratch `work/codex_lua_arena_main_20261002`. Arena GivePrizeFund (0x00F21300) spoke
`""` instead of the round's prize line in all 8 rounds: the slot is built `""` and
re-assigned per switch case through `CCharString::operator=` (0x99EFE0, verified: release
then rebuild from the char*), but the lifter only lifted that assignment when the slot
was already a string local, so each was a TODO and the consumer got the start literal.

First attempt (lowering the assignment to a constructor) was WRONG and reverted: the
lifter then constant-propagated the last literal across branches (every round ROUND8;
Arena Main's macro name collapsed to one constant). Final fix in the lifter: a stack
slot listed in `literal_assigned_slots` is materialised as a string variable at its
construction (keeping the start value for paths that never reassign it -- Main's
"CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC" default was nil before), its literal stays the
operand value until the first reassignment (so ChickenMaster's
MsgIsTalkedToBy("SCRIPT_NAME_HERO") still folds to IsTalkedToByHero -- an intermediate
version broke that), and each `operator=(slot, "LIT")` becomes an assignment.

Corrected beyond Arena: ChickenMaster's CS_CHICKING_LANDED cutscene name, SickChild
TalkingTrader1/2 REPEATB_10 lines and Witch ONHIT_20/30 lines (all TODOs before),
TC_GuardSpawnPoint start value. GivePrizeFund check: new 0 failures per style (gold and
both lines for multipliers 0..7, `""` on the default path; the default path's second line
reads an uninitialised native char* -- undefined in retail, not checked); baseline fails
all 8 rounds. New lifter test fails on the baseline lifter. Broad 987 passed + 43
subtests. 29-unit A/B: Arena.lua, ChickenMaster, TalkingTrader1/2, Witch,
TC_GuardSpawnPoint; before == promoted for all 4 units; smoke identical.
**PROMOTED OFFLINE.** 82 needs-recovery / 134 needs-readability / 155 human-review;
diagnostics 1142.

## Arena.lua: CrowdChecker arrays, Init spawn flags, dead base-dtor stubs

Scratch `work/codex_lua_arena_main2_20261002`.
- **CrowdChecker crashed** whenever it re-evaluated the crowd level: the threshold
  array `int aiStack_c[3] = {0,200,400}` was never built, so `aiStack_c[iVar3]` indexed
  nil; its per-CrowdSoundSource handle array (F1D15C: lea ecx -0x12C, call 0xA96100
  Vector_ZeroInit with the source count) was three TODOs, so old loops were never
  stopped. New sha-gated `native_reviewed_crowd_checker.py` + `ENGINE_IntArray /
  ZeroArray / ArrayAt / ArraySet` expansions in `finish_lua` (zero-based C -> `+ 1`).
  Harness (75 cases: points 0..900, termination frames, 0/1/3 sources): new 0 failures,
  draft == readable, 140 sounds; every StopSound targets that source's previous handle
  (24 checks); baseline crashes in all 60 cases that reach the crowd evaluation.
- Init (0xCFA700) clears `ArenaSpawnNeeded_<i>`: CFAA1B stores BL whose only reaching
  definition is `xor ebx,ebx` (CFA849); added to the existing address-gated array rule.
- `0x99A2E0` (`mov [ecx],0x1231710; ret`, a base dtor's vtable store) dropped when its
  phantom result is dead (5 PlayWave cleanups).
4 new tests; broad 991 passed + 43 subtests; 29-unit A/B: only Arena.lua; smoke: Init
trace grows (the 16 flag stores). **PROMOTED OFFLINE.** Diagnostics 1128.

## Arena AnimateCrowd handles and reviewed Main output drift

Scratch `work/codex_lua_arena_audience_20261002`; baseline commit
`366c08e64a4323a6147fe4d5caa675e31799d51e`, existing dirty source preserved.
No commits, packaging, install changes or game launch.

- New `native_reviewed_arena_audience.py`: hash-gated AnimateCrowd F1EC70.
  Retail F1ECD8/F1ECE0 copy vector element Data/Info; F1ECEC retains Info;
  EBX starts at 0 (F1ECC3), advances by 12 (F1EE75). The lowered empty handle
  plus residual Data store now become LOCALLIST_At. All six animation calls and
  EntitySetCutsceneBehaviour use the actual audience handle. Random order/count
  and native loop control retained; drift remains unresolved.
- Discovered pre-existing baseline generator/promoted-output drift in Arena Main:
  dirty generic member-alias lowering already restored both Whisper assignments.
  Reviewed before reconciliation: F12286 points ESI at quest +0x108; F12295 stores
  the GetThingWithScriptName return through operator= 0x8AB980; F12367 stores the
  CreateCreature return into that same member. Getter aliases refresh after writes.
  The regenerated Main now stores Whisper before checking/acquiring it.

Validation commands (from root): `python -m pytest -q
tools/script_recovery/test_native_reviewed_arena_audience.py` (41 passed),
`python -m pytest -q tools/script_recovery/test_thing_member_assignment_alias.py`
(3 passed; negative cases: reassigned alias and unknown offset), and scratch
`run_tests.py` (1032 passed + 43 subtests). Initial new test setup used the wrong
output path; fixed to FSE/Arena/Arena.lua. Constructor emulation was excluded from
the focused fixture after Unicorn's Windows access-violation logging; these tests
do not claim constructor coverage. The broad suite completes successfully.

Scratch `ab/compare.py`: all 29 registered units regenerated before/after;
only Arena/Arena.lua changes, solely AnimateCrowd. `build_review.py` regenerates
both readable packages from their explicit scratch draft inputs. `behavior.py`:
30 audience cases per style, new 0 failures; baseline fails all 24 nonempty cases.
`main_behavior.py`: 18 isolated lookup/spawn cases per style (rounds 2/3/7,
missing/dead/alive lookup, stale saved handle alive/dead), new 0 failures;
promoted baseline 12 failures per style. These exercise the reviewed Main fragment,
not the full quest. `smoke_all.py` and `smoke_readable.py`: identical before/after.
Source shape, hash and unrelated-unit checks retained in scratch artifacts.

`promote.py` validates that pre-existing output drift is confined to Main, then
copies the tested Arena.lua and reports; every promoted byte matches scratch.
Corpus audit: 371 parse, diagnostics 1128 -> 1125 (Arena.lua 11 -> 8);
82 needs-recovery / 134 needs-readability / 155 human-review. Counts are mechanical,
not completion percentages. **PROMOTED OFFLINE**, installed bundle remains v124.

Next: PlayWave multi-marker CreateCreatureNearby drops its returned handle and
uses a byte cursor as radius; inspect the call-site pushes before correcting.
Random-object spawn still needs the CDefString table read and spherical position.

## PlayWave multi-marker radius and returned creature handle

Scratch `work/codex_lua_arena_wave_20261002`, **PROMOTED OFFLINE**. No game launch,
packaging, installation or main-checkout commits. Installed bundle remains v124.

New `native_reviewed_arena_wave.py` repairs the exporter attaching pending pushes
to GetPos: F1F67B/F1F684/F1F687 push false, ArenaEnemy and float 1.0 before
F1F693 GetPos; F1F6B2 CreateCreatureNearby consumes these with position, creature
definition and hidden output. F1F6BD copies the result into local -0xF4.
The native-body-hash/source-shape gated recovery restores explicit spawn operands.
Generic `lower_after_annotate` folds the adjacent known GSI thing return followed
by a bare-pointer local handle copy. It runs after GSI method names exist.
Unknown constructors, overwritten registers and unknown destination types stay
unresolved. Cast-bearing copies retain the existing lowering.

Superseded attempts preserved: `ab` widened bare copies to all constructors,
changing Trader Escort dialogue receivers and producing a missing argument; not
promoted. `ab_narrow_wrong_layer` restricted source calls too early, before GSI
annotation, so the Arena copy stayed unresolved. Broad logs for both attempts
were interrupted and renamed; neither is completed validation. Final `ab_final`
reuses the identical baseline outputs, regenerates all 29 units with final code,
and changes only Arena/Arena.lua. Trader Escort remains byte-identical.

Validation: `python -m pytest -q tools/script_recovery/test_native_reviewed_arena_wave.py`
7 passed (`focused_final2.log`); scratch `run_tests.py` 1039 passed +43 subtests.
Arena round/counter suite 86 passed. `behavior_wave.py` runs actual PlayWave through
HUD setup: rounds 5/7, all three groups, countdown on/off, six marker patterns;
72 cases per style, new 0 failures, old 72 failures; all matching marker positions
used in order, radius 1, ArenaEnemy script, returned actor reaches behavior/target
calls, counters/countdown preserved. Draft/readable traces agree.
Draft/readable smoke identical. `reuse_validation.py` checks every generated Arena
Lua byte equals the tested initial Arena output before reusing its readable,
behavior, smoke and round-test proof; only unrelated Trader Escort output was
removed by the narrowed rule. `promote.py` requires baseline==promoted for all
Arena Lua and copies exactly the tested output/report bytes. Audit: 371 parse,
diagnostics 1125 ->1124; Arena.lua 8 ->7. Counts are mechanical, not coverage.

Next work in progress: `work/codex_lua_arena_rewards_20261002` owns a clean cloned
sidecar rooted at 906a5fc39bd82d5208b5ff88b47125925413adb5. Generic CDefString-vector
read and native horizontal vector rotation bindings pass 22 compiled x86 mocked
ABI/lifetime cases; DLL build ongoing. Neither installed nor packaged; converter
reward-object branch not yet changed. Retail global +0xA6C is the reward-token
vector (PDB CScriptDef::RewardObjects at +2828; retail/PDB layout differs). Native
A14040 rotates the Cartesian vector by turns, ret4; it is not merely a coordinate
conversion despite the propagated label. Native helper sha in rotation_native.txt
and the script's console output. Preserve the retail two RNG draws and z offset.

## PlayWave reward objects and readable operator precedence

Scratch `work/codex_lua_arena_rewards_20261002`, **PROMOTED OFFLINE**: 47 Lua/report
files, exact tested bytes. No root commits, installation or game launch; v124 remains
installed. Arena packaging/live proof pending. GuildTraining's seven style-affected
files are deferred for pre-existing converter drift; `guild_style` separately
regenerates from its promoted draft. Preserve its hand-reviewed readable tree;
the generated package stage is readable_converter.

Hash/source-shape gated `native_reviewed_arena_reward.py` restores PlayWave rewards.
F20CF9..D05 uses globals+A6C/A70 vector<CDefString>, stride 4. Choice rand F20D08
and DIV precede angle rand F20D3A, including empty-vector fault order. F20D1F..D2F
builds (14,0,5); F20D59/D5F multiplies by float 1/360; F20D6E calls A14040, horizontal
Cartesian rotation by turns, ret4. All center xyz components are added at F20DA2..DCE;
F20E07 converts the selected token then F20E2B creates it with empty scriptname.
Signed /100 reward thresholds are exact at 100 and 600. PlayWave SHA
e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc;
A14040 SHA 6c019921b6785843ded10f21808d9f392920a1f5aaae629646fdde6ecc6a6f0b.

Generic sidecar ReadGlobalGameDataDefStringAt handles tokens rather than the existing
CCharString vector layout, copying before temporary destruction (including exception
cleanup). RotateVectorHorizontal calls retail with float turns and a copied vector.
22 compiled x86 mocked ABI/lifetime cases pass; no live or independent mathematics
claim. Owned candidate source commit 5d217e0e506e62e4b314bafa3bf5fb86dec4ae42,
baseline 906a5fc39bd82d5208b5ff88b47125925413adb5. Exported patch:
tools/script_recovery/sidecar_patches/novi-native-definition-vector-and-horizontal-rotation.patch
SHA 7efa304e57575d03d7b8b72865eeecc16382cc3b95e85470a73b7de7025cf238.
Release x86 DLL SHA 3b5550fab57bf4e973caad919f24f7236b5279a0dd0ca2a3657c28bcfaa866bc
built/linked; not packaged or installed. Details: sidecar_build.json.

Reward tests exposed a shared readable bug: unrecognized bitwise/floor-division/
length operators were treated as atomic, removing significant grouping. Operator
recognition now preserves it. Eight semantic regressions pass, including unary
operators versus exponentiation. Across all 29 units, draft changes only Arena.lua;
readable changes 36 files in 16 units. Outside Arena, 168 lines in 35 files retain
grouping, apart from two equivalent numeric equality/not-inequality conditions.
style_review.json/readable_diff.txt record review. All 16 readable smoke comparisons
and Arena draft smoke are identical. GuildTraining baseline drift is separate.

Focused reward suite: 17 passed. Actual PlayWave harness: 1520 cases/style, after
0 failures versus baseline 1140/style. Points -100/0/1/99/100/599/600/900, counts
1/3/7, choice 0/1/32767, seven angles, three center positions, empty vectors.
RNG order, selected token, passed turns, xyz offsets, scriptname and thresholds
checked; draft/readable traces agree. Earlier spawn harness: 72 cases/style still
pass. Broader suite: 1405 passed +564 subtests (tests.log), runner exit 0 despite
Unicorn access-violation tracebacks. All 534 scratch Lua parse. Promotion requires
baseline equality, completed tests/behavior/smoke and sidecar hashes.
Corpus 371 parse, diagnostics 1124 ->1119; Arena readable 7 ->2. Draft also retains
an unresolved empty-string constructor on a null-marker branch.

Superseded: p1 prototypes; premature readable generation while draft A/B was still
writing; initial smoke/behavior and interrupted tests with incomplete mocks. Only
readable_final, smoke_*_final, focused3, bitwise2, final behavior and completed
tests.log support promotion. Dependent builds must wait for producer completion.
Guild_style first import attempt ran a script's top-level code; failed before
generation, corrected by loading only its snapshot class. document.py first
creation attempt had a quoting syntax error; no files changed, then corrected.

Next: F14250 is mov eax,[ecx+44];ret, SHA
75315a13a7503cb9a126675518a19ff201faa8c510e8a849d0f971d9c61bbb2b.
Main F13DB8 compares byte[eax+74] to 1, master WhisperKilledByHero. Recover through
the proven pointer. Keep CrowdChecker unresolved jump until CFG review.
WhisperAlly's four melee-hit diagnostics and Arena packaging/live proof remain.

## Arena Main master-data read

Scratch `work/codex_lua_arena_master_20261002`, **PROMOTED OFFLINE**, five generated
Lua/report files. No root commits, packaging, installation or launch; v124 stays
installed. New generic native_master_accessor_reads.py recognizes a trivial retail
MOV EAX,[ECX+master_offset];RET getter, validates the actual direct CALL target and
following CMP BYTE [EAX+mapped_bool_offset],0/1. A direct self receiver and exactly
one textual/native call are required; escaping pointer slots, mismatched operands,
unknown fields/types, entity ownership and instruction drift stay unresolved.
Signed disp8 offsets are checked. The read is lowered through normal master state.

Arena F14250 bytes 8B4144C3, SHA
75315a13a7503cb9a126675518a19ff201faa8c510e8a849d0f971d9c61bbb2b;
Main F13DB8 calls it, F13DBD CMP BYTE [EAX+74],1 selects Lady Grey scene TWO versus
ONE. Master +74 is WhisperKilledByHero. Propagated CWorld::DrawGetEnvironment is
misleading. The old draft returned nothing from helper_F14250 and indexed nil;
readable inlined this as DrawGetEnvironment(quest)[116]. Main now reads the master
flag directly. The unused getter diagnostic remains explicitly unresolved rather
than being removed solely to lower the count.

Focused suite 26 passed: 18 positive/adverse lowering cases plus eight full Main
cases (both styles, killed flag false/true, cancellation after the read). Main's
initialization and independent crowd thread are isolated by no-op helper overrides;
the complete generated Main is exercised, not a copied branch. Final generated
behavior_main.py has four cases/style, new 0 failures versus old 4/style. Draft and
readable traces agree. All 29 draft A/B units regenerated; only Arena.lua changes.
Arena draft/readable smoke identical. Related Roth, Arena quest and round suite:
82 passed. The first PowerShell invocation reported exit 1 despite a passing
summary and caught Unicorn tracebacks; run_related.py repeated it via subprocess,
recorded actual returncode 0 in related_exit.json (confirmed log). Initial attempt
named a nonexistent evidence-lowering test file and ran no tests; excluded.

Promotion first failed its readable baseline gate without writing references:
regeneration chose timerId5 rather than timerId for the third countdown timer.
It is the saved immutable alias of the same RegisterTimer result. Exact one-block
drift is separately gated; all other Lua bytes must match. 72 wave cases/style
pass for promoted, regenerated baseline and candidate, with identical traces.
This preserves the drift in baseline_drift.json and backups. Generated Lua was
not edited by hand. Final five files equal tested scratch bytes. Corpus 371 parse,
diagnostics remain 1119 (Arena readable still 2). Last full broad suite was 1405
tests +564 subtests before this hook; current hook has the focused/related/A/B proof.

Next: CrowdChecker's final native branch returns to F1D1E0 while Arena is loaded;
the current unresolved goto ends the thread after one iteration. Owned scratch
work/codex_lua_arena_crowd_cycle_20261002 captures its actual normalized statements.
The target sits inside a conditional outer loop and three if scopes; existing
constant-true direct-body re-entry handling cannot express it. Preserve initialization,
frame/yield order, cancellation, pause behavior and region unload while repairing CFG.
GuildTraining style synchronization remains deferred: converter_comparison.json
records 19 generated-stage drift files (do not confuse hand-reviewed readable with
readable_converter). WhisperAlly melee-hit binding and Arena packaging/live proof remain.

## Conditional loop re-entry: Arena crowd and Fisherman's Wife

Scratch work/codex_lua_arena_crowd_cycle_20261002, **PROMOTED OFFLINE**: ten
generated Lua/report files across Arena and HerosOldHouse. No root commits,
package, installation or launch. Generic native_loop_reentry now accepts a
backward edge from outside a conditional while into its nested if tails.
Only a simple register condition (optionally negated) and register-copy comma
operands are accepted. Cleanup epilogues, raw switches, complex conditions,
ambiguous transfers and inner-loop continues stay unresolved. Outer break and
continue target fresh loop labels; inner breaks retain their own structured loop.
Initialization, comma copies and per-iteration frame/termination order are retained.

Arena CrowdChecker F1CCE0..F1EBD0 SHA
c095beeeebfe44aa55720c555183efdcb6c1463616eb69feebcb5ce6bfcc4802;
F1E37C JNE F1D1E0 (0F855EEEFFFF) returns to the frame poll while Arena remains
loaded. Previously the unresolved edge ended the thread after one iteration.
Actual whole CrowdChecker passes 32 cases/style, versus all 32 failures/style
before: cancellation, unload, reload and pause, 1/2/4/7 cycles, hero hit false/true.
Timer lifetime, HUD initialization per region, reinitialization after reload,
hit penalty and timed decay are asserted. Empty audience arrays and engine calls
are mocked; this is offline behavior evidence, not an in-game result.

All29 A/B changes only Arena.lua and HerosOldHouse/Entities/FishermansWife.lua.
Native Wife Main D8BCD0..D8D597 SHA
eaae6fc1e3942797e67c59bd177f3008c0c5b9b8a98374e4b1729b1252153597.
D8CC48 task query, D8CC53 JNE D8CC31; false falls through D8CC55 inside
while LeavingHappy==0. Movement, fade and flag reload precede the next iteration.
Old generated shared-tail return skipped the polling loop and entered the happy
departure with an unset actor. Actual whole Main passes 36 cases/style: speech
delay 0/1/3, movement delay 0/1/2, hit false/true and cancellation at frame 12/17.
Both styles have equal traces. Old 12 failures/style are the delayed-speech hit
cases. Retained root regression test_native_wife_loop_reentry.py: 72 passed.
Talk/gift branches, non-null marker position and retail engine calls are outside
this fixture; the non-null delayed-speech tail still has a pre-existing missing
position assignment and remains a separate recovery gate.

Focused CFG/crowd suite 131 passed; broader source-stable suite **1541 passed +564
subtests**, returncode 0, before adding the 72-case Wife regression. All four
readable builds parse. Draft/readable smoke identical for both affected units.
Promotion checks exact tested bytes, snapshots all ten replaced files and accepts
only the previously documented one-block immutable timerId5 countdown alias drift.
72 wave cases/style across promoted/before/after confirm equal traces.
Corpus 371 parses, statuses unchanged 82/134/155, diagnostics 1119 -> **1118**.
Arena readable now has one retained diagnostic: unused trivial master getter.

Superseded prototypes and focused1..4 logs are excluded: the first prototype
matched a cleanup edge; early synthetic fixtures exposed unsupported nonvoid
inline returns and inner-loop continues. The final rule rejects inner continues,
and focused_final plus completed tests/comparison/readable/behavior reports are
the promotion evidence. An initial Wife harness incorrectly returned true after
termination from NewScriptFrame; corrected to the binding contract before proof.

Next: WhisperAlly missing MsgHitEnemyWithMeleeWeapon binding. Its isolated sidecar
candidate built at source 07774e98c5675fb66aba7c23b4f1e4453b0434c0, 37 compiled x86
ABI cases passed. Patch remains isolated until this promotion is complete so it
cannot change sidecar binding evidence during A/B. Arena packaging/live proof and
GuildTraining's 19 generated-style drift files remain pending.

## WhisperAlly melee event binding and offline Arena candidate

Scratch work/codex_lua_arena_melee_20261002, **PROMOTED OFFLINE**, five generated
Lua/report files and novi-melee-enemy-event.patch. No root commits or installation;
v124 remains installed. Candidate source was cloned from the reviewed reward
sidecar 5d217e0e506e62e4b314bafa3bf5fb86dec4ae42 and committed in that owned clone
as **07774e98c5675fb66aba7c23b4f1e4453b0434c0**. Only seven binding lines added.
Header SHA 830a35aaaa8dd2c18cf1fc7c01badf78aa0263ebcf98c78f3e3cb7526fa611f5;
patch SHA 9544bfb208648b43f2b3cb0c43233826e31971112a896b964448632240c7cf1b;
Release x86 sidecar SHA af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.

CScriptThing wrapper table 01238C8C+B8 -> 004AADC0, 18 retail bytes
8B490485C9750332C0C38B01FFA0B8000000, SHA
e18aebdf30fb1ad470100213181489356a9a50e8c18f460250a48fbb2e628f8c.
Null Data returns false; otherwise tail-call implementation vtable B8 with this
in ECX, no explicit arguments, bool result. Actual EntityScriptingAPI declarations
and new lambda compiled as x86: 37 receiver/null/return/stack cases pass, executing
the retail wrapper instructions on a mocked underlying event query. Additional
vendored Lua/sol userdata integration: 71 total cases pass, including both Lua
boolean branches and null guards. Implementation event query remains mocked;
these are offline ABI checks, not live game event evidence.

WhisperAlly Main F22D80..F25823 uses B8 at F2396F/F23D9E/F24111/F248C1.
Converter obtains the actual new lambda's Entity bool signature from the exported
patch. Baseline binding manifest was snapshotted before export; all29 regenerated
with old/current manifests and identical source/evidence. Only WhisperAlly.lua
changes: draft removes four unavailable-binding comments, retaining all calls.
Readable also drops two redundant termination checks after those comment barriers
disappear. Exact existing CFG prune reproduces precisely those two removals;
no scheduler advance occurs between each and its dominating check. Actual whole
Main: 64 cases per version/style (ally combat, final duel, rounds0/7, event false/true,
hit/hero-hit priorities, cancellation frame3/5), all pass with equal four-way traces.
Root regression plus reviewed Whisper suite: **136 passed**, subprocess return0.
Both Arena smoke reports identical; exact baseline bytes matched with no alias drift.
Promotion uses tested bytes and backups. Corpus371 parses, statuses81/135/155;
diagnostics **1118 ->1114**, WhisperAlly readable diagnostics now zero.

Named package **arena-offline-candidate-20261002** assembled in this scratch:
43 Lua files parse, 48 manifest hashes verified. Contains established Oakvale stage
plus Arena and ArenaHoldingScript overrides and the above exact sidecar DLL.
All Arena script bytes equal promoted readable output. Installed:false;
game_tested:false. The installed v124 bundle remains unchanged. Package validation
initially omitted ArenaHoldingScript from the expected unit list, then incorrectly
compared the unit's root quests.lua boilerplate to the deliberately assembled
retail-only quests.lua. Corrected identity checks cover actual script files and
all staged/manifest bytes; no generated script was hand edited. Earlier readable
review assertion missed the two CFG-pruned checks; corrected with explicit proof.
Initial C++ integration include paths and initial final-duel harness state name
were corrected before successful reports; failed attempts are not acceptance proof.

Next: generic shared-tail copier incorrectly treats an inner else at depth0 as an
unreachable enclosing else, dropping FisherWifeLeaveMarker:GetPos after delayed
attack speech. Owned work/codex_lua_wife_marker_20261002 prototype restores it:
13 focused candidate tests and 144 actual Main cases pass, old24 marker failures.
Production generator/source and references for this next fix are not yet changed.
GuildTraining style synchronization and remaining Arena getter/null-string evidence
remain pending. Arena runtime proof has not been attempted.

## Preserve inner else branches when copying shared tails

Scratch work/codex_lua_wife_marker_20261002, **PROMOTED OFFLINE**: 25 generated
Lua/report files in BookCollecting, Bordello, HerosOldHouse, SickChild and
WhiteBalverine. GuildTraining output is explicitly deferred below. No root commits,
installation or launch. Installed v124 and the named offline Arena candidate remain
unchanged; the other five unit changes have not been packaged.

native_goto_scopes.duplicate_sibling_tails formerly skipped every separate-line
else at copied-region depth0, including an else belonging to an inner conditional
that was copied in full. It now tracks whether a preceding closure actually exits
an enclosing block before skipping that sibling else. Nested copied if/else chains
remain intact; genuine enclosing else chains remain skipped. Unsupported combined
enclosing brace/else forms still stay unresolved. Generic focused fixture: 13 cases,
including call order and outer-else exclusion. CFG/Wife/related suite: 224 tests +8
subtests. Wider suite **1826 tests +564 subtests**, subprocess return0, completed
with this generic fix before adding the compatible Bordello adapter below.

FishermansWife native D8BCD0..D8D597 SHA
eaae6fc1e3942797e67c59bd177f3008c0c5b9b8a98374e4b1729b1252153597.
D8CCE5 chooses null fallback versus GetPos call at D8CCF0; EAX is pushed as the
MoveToPosition vector at D8CCFE, call D8CD03. Previously the delayed-speech copied
tail lost the GetPos else, so a present marker used an unset position. Actual whole
Main passes 72 cases/style: delays0/1/3 and0/1/2, hit false/true, termination12/17,
absent/present marker. Both styles agree. Before12 failures/style become zero;
144-case retained Wife regression is included in the broad suite.

Four other copied event windows restore hero hit OR (magic AND NOT healing14),
including clearing a previous flag. KG_Chief additionally suppresses it at
BalverineState7. Exact contiguous generated draft predicate windows are executed,
not handwritten replacements: 32 cases/unit, four units (128 total). Callback
order is asserted. Before failures: Guard8, PreMeleeMaze16, SickChild16, KG_Chief16;
after zero. These are predicate-window checks, not full surrounding Main/movie
or in-game tests. Retained fresh-generation regression: **128 passed**.
Native/export hashes and excerpts are in hit_native_evidence.json:
Guard Main E40420 SHA b53f0601baf1206bb3f5a6929715a8a6b59e35ba9e31a741bcf881817da0c229;
PreMeleeMaze Main D43DB0 SHA4771b6078914be1a5446eb587e6e523340fab4a1f1fcadb4ccb293565e518c77;
SickChild Main EC5DE0 SHA1e77c6bd5ea23a4c4be260d5835b515fed37492c2173b4750de839d094fc8d6d;
KG_Chief Main E17840 SHAe5942a9ec0ee0f405ff3ac6d75f099fc9b3883a00b7936280eb4abfbbcfba5e3.

Teacher helper E55CE0..E566C1 SHA
3e25caaf1c013ac02c374d1bf6012f451311bb10bcc9a87f71d1e12aeacc49d9:
two copied good-hat else branches reappear AFTER unconditional false returns.
They are unreachable; no live reward repair is claimed. Actual AskForBook reward
and interruption behavior: 720 cases/version/style, all four traces equal. Teacher
reward/interrupt/intro/wrong-time suite: 98 passed. Initial commentary interpreted
these additions as skipped rewards; native/generated inspection corrected that.

First A/B stopped on the reviewed BordelloLady adapter's exact-shape guard: the
general copier now restores the same hero-hit else the adapter supplied. The
adapter accepts only that exact independently restored else, normalizes it and
applies the original native-byte/export/window-pinned repair. Unknown targets,
duplicate regions and all prior evidence drift still fail closed. Owned13 tests;
retained compatible adapter + prior reviewed suite: **26 passed**. Whole generated
BordelloLady Main remains byte-identical. No evidence fingerprint was weakened.

Final all29 generation completed using snapshots of both changed modules. The
first report reader still pointed at the earlier partial comparison tree; its
summary is excluded. correct_comparison_report.py requires all29 report pairs and
nonempty Lua trees, then compares the actual comparison_final paths. Reviewed
draft delta is exactly the six entity files above. All six regenerated readable
stages parse and have the same six-file delta. Draft/readable smoke identical
for every affected unit. Final Wife and Teacher traces use completed final paths.

GuildTraining baseline has three draft drift files (SkillTarget, MeleeOpponent,
FinalMaze) and twenty generated-readable drift files; baseline_drift_review.json
lists exact paths. PreMeleeMaze itself matches before, but its whole-unit reports
would incorporate the unrelated drift. All GuildTraining references, including
hand-reviewed readable, remain untouched until a dedicated reconciliation gate.
Earlier 19-file style observations are superseded by this exact current count.
Every promoted unit has zero baseline Lua drift and exact tested file/report bytes.
Corpus371 parses; statuses81/135/155 and diagnostics1114 unchanged. Metric changes
reflect restored branches, not a completion percentage.

Excluded artifacts: initial failed A/B, its partial files, compare_final's incorrect
reader summary, initial combined-brace positive fixtures (unsupported by the
lifter), and first hit test selecting Q_GuildTrainingPreMelee rather than the actual
owner Q_GuildTraining. Corrected hit fixture passes all128. No generated Lua was
hand edited. Broad caught Unicorn access-violation diagnostics are not test failures;
the completed subprocess returned0 with the above passing summary.

Next owned investigation: work/codex_lua_arena_spawn_select_20261002. Actual PlayWave
backward index is a 32-bit register; Lua's -1 comparison against0xffffffff misses
wraparound on larger alternating waves. A hash-pinned isolated prototype masks
the decrement/restoration and lowers the known empty-string copy on null markers.
Prototype3 generates and parses both styles; production source/references have
not changed for this next work. Native windows and multi-spawn behavior remain
to validate. GuildTraining reconciliation and Arena runtime proof remain pending.

## Arena backward spawn selection and missing-marker string

PROMOTED OFFLINE: five Lua/report files, owned scratch
work/codex_lua_arena_spawn_select_20261002. Native-reviewed helper masks the
initial backward index, decrement and both count-1 resets to 32 bits. Retail
F1F356 LEA EAX,[EBX-1], F1F359 CMP EAX,-1 and F1FE6F DEC EAX/F1FE70 CMP
wrap at 32 bits; Lua -1 did not equal0xffffffff, causing alternating waves to
create fewer enemies while the HUD still expected the configured total.
Full PlayWave native hash remains e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc.
F1F315 pushes established empty CCharString global0143E8EC; F1F321 copies it
into the missing-marker data string. The reviewed exported stack-array copy is
now lowered to ENGINE_EmptyString rather than leaving nil input to the parser.
The helper requires all source occurrences and the complete native body hash;
missing, duplicate, changed operands or changed/missing native bytes fail closed.
Native windows and hashes: native_evidence.json. No generated Lua hand edits.

Actual whole PlayWave to HUD: 912 cases/version/style, count1/2/3/5, round2/3,
groups0/1/2, short flag both, creature count1/2/4/7, every valid index plus nil and
invalid text. Before576 failures/style, after0; final draft/readable traces agree.
Retained fresh-generation regression plus wave/rounds tests1901 passed, related
Arena suite103 passed; both subprocesses return0. Engine operations mocked,
not an in-game or complete PlayWave proof. Actor identity is shared in this spawn
fixture; target-receiver correctness is explicitly a separate next gate.

Initial scratch fixture incorrectly halved creatures for ShortWave and exposed
nonnull IsNull=true handles to readable Lua. Neither represents the interface:
ShortWave skips countdown only, and stock GetFurthestWithScriptName delegates to
WrapScriptThingOutput, which converts native IsNull results to nil. Superseded
prototype1/2 and first behavior matrix are excluded. Corrected final artifacts:
behavior_final.json (all four variants), focused.log, related.log.

All29 nonempty report/tree pairs verified; draft only Arena/Arena.lua changes.
Both generated readable stages parse, same one-file delta, and both promoted
baselines match byte-for-byte before. Smoke differs only in PlayWave calls51->52:
the zero-marker decrement now enters count-1 reset and the native termination
check. Every other smoke field/function unchanged; review_smoke.py asserts exact
allowed delta, smoke_reviewed.json records it. Other existing pending/free-global
smoke findings remain. Exact tested bytes promoted with backups; corpus371 parse,
statuses81/135/155 and diagnostics1114 unchanged.

Named offline package arena-spawn-candidate-20261002 contains43 parsed Lua and48
verified manifest hashes, sidecar07774e98c5675fb66aba7c23b4f1e4453b0434c0,
DLL af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
No installation or game launch; installed v124 unchanged. package_validation.json
is definitive completion evidence. Prior named candidate retained unchanged.

Next owned scratch work/codex_lua_arena_target_handles_20261002 captures actual
lifter state for explicit &xStack_c0 thing operands. Native forward spawn target
calls use retained newly created creature, but draft drops that addressed operand
and backfills unrelated handles. Investigate generic positional thing-reference
handling and verify distinct creature/hero/Whisper identities before promotion.
GuildTraining baseline reconciliation and Arena runtime proof remain pending.

## Arena retained creature target operands

PROMOTED OFFLINE: generic Lifter.interface_call now keeps addressed operands
whose independently known kind is thing, alongside existing number/vector
references. Unknown/bool references retain diagnostics; hidden returned thing
slots are still removed and tracked before input-argument placement. Root source
is byte-identical to candidate_lift_native_lua.py, proven by installed_source.json.
Owned scratch work/codex_lua_arena_target_handles_20261002; no root commit.

Before the explicit &xStack_c0 creature operand was dropped on forward spawning.
GiveThingBestEnemyTarget then backfilled the second argument from unrelated
staged things: enemy receiver became hero/Whisper, target became a spawn marker.
Forward EntitySetCutsceneBehaviour also used a transient register instead of the
retained stack snapshot (it happened to still hold the same creature there).
The generic fix retains the two positional target arguments and the snapshot.
Native calls F1FA24/F1FA8B use creature stack+a0 and Whisper/hero returned EAX;
backward F1FDB8/F1FE1C use stack+88 analogously. Stack offsets account for each
preceding push. Retail PlayWave full hash remains
 e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc.
Native windows, bytes and hashes in native_evidence.json. Retained distinct handle
regression512 passed; generic positive/adverse/reference-kind boundaries16 passed,
including unchanged unit-mode/unknown/bool behavior. Root regression files:
test_native_arena_target_handles.py, test_known_thing_reference_operands.py.

Actual whole PlayWave to HUD256 cases/version/style: round2/3, short flag both,
markers at every index in count1/3, creatures1/2/4/7, alive/dead Whisper and both
rand selection bits. Created creatures, hero and Whisper have distinct identities.
Before256 failures/style, after0; draft/readable final traces identical. Assert
receiver, target and random-call count/domain32768 (one per live-Whisper spawn,
zero for dead). Mock engine, no full-wave/game equivalence claim. Initial prototype
patched a fresh Lifter class but converter kept its imported old class; rejected.
Prototype2 patches the converter alias and is superseded by final A/B paths.
Initial focused harness assumed a different baseline fallback and was corrected
from actual emitted output; it is excluded. No generated Lua hand edits.

Broader completed suite: 1826 passed, 564 subtests passed, subprocess return0 (tests_exit.json).
All29 nonempty report/tree pairs complete, only Arena/Arena.lua changes. Draft
diff exactly three receiver/argument lines; readable locals are consistently
renamed as those retained values become used. Both styles parse and smoke
identically. Actual reward paths1520 cases/version/style (all6080): zero failures,
all four traces equal, protecting the later code touched by readable renaming.
Promotion baseline drift was exactly two countdown reads of immutable timerId5
versus timerId; both assigned once, same registered timer. All72 countdown cases
for promoted/before/after (short both, four rounds, three groups/counts) match frames,
timer IDs, four dialogue lines and positions. Target callback is neutralized only
in this timer oracle; separate512 target cases retain identity assertions. First
fixture incorrectly expected three dialogue lines, corrected to native3/2/1/GO.
Baseline normalization allows precisely those first two timer aliases; no other
changed file/byte accepted. Promoted five Lua/report files with backups and
verified tested bytes. Corpus371 parses; installed v124 unchanged. Named offline
arena-target-candidate-20261002:43 parsed Lua/48 manifest hashes, existing melee
sidecar source07774e98c5675fb66aba7c23b4f1e4453b0434c0, DLL
 af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
No game launch or installation; prior candidates retained unchanged.

Next: reconcile GuildTraining generated baselines. Three draft drift files are
only prior math.random(0,32767) versus reviewed RetailRandModulo(32768) calls
(one SkillTarget, four MeleeOpponent, three FinalMaze). Twenty generated readable
drift files include significant operator-grouping and naming; PreMeleeMaze also
awaits the accepted shared-tail else repair. Preserve hand-reviewed readable and
verify each generated drift before reports are promoted. Arena runtime proof
remains pending.

## Reconcile deferred GuildTraining generated baselines

PROMOTED OFFLINE:28 Lua/report files, owned work/codex_lua_guild_baseline_20261002.
Four draft Lua +21 generated readable Lua +three aggregate reports. Hand-reviewed
readable tree is byte-unchanged (hand_reviewed_hashes.json before/after guard).
No generator changes in this reconciliation; the only root source edit is the
speech-test fixture's existing native RNG API. Installed v124 unchanged.

Exact draft delta: PreMeleeMaze is byte-identical to the earlier independently
reviewed shared-tail after output; SkillTarget1, MeleeOpponent4, FinalMaze3 calls
replace math.random(0,32767) with RetailRandModulo(32768), with no other byte
changes. Complete native CRT call inventories and body hashes pinned in
rng_native_evidence.json (SkillTargetD41D00, MeleeOpponentD56790, FinalMazeD647F0).
Raw/mod/partial RNG suite45 passed +5 subtests, including actual CRT range proof.
Eight draws retain their positions and masks. No generated Lua hand edits.

Readable drift21 includes the new PreMelee copied else, position snapshots,
boolean grouping/negation, discarded predicate scratch flags, RNG calls/names
and KeepTabsOnWhisper frame checks. Larger paths verified from actual whole Lua:
- SkillTarget864 cases/variant (four variants3456): dummy0/1/2, speed1/3/5,
  moving/static, start0/4, nil/positive/boundary/mixed projectile patterns,
  draws0/1/3, limits8/20. Movement, frames, score/state, speech, sound and RNG
  traces identical; first interpolation positions analytically asserted.
- MeleeOpponent576 cases/variant (2304): tutorial stages, intro0/2/4, health0/6,
  talk/hit/weapon both, draws0/1/32767, limits6/20/45/70. All traces equal;
  native RNG branches exercised1440 draws in after-readable matrix.
- FinalMaze, ArtifactThief and WillWhisper288 cases/file/variant: weapon,
  projectile, hero hit, magic and heal booleans; draws0/1/32767 and limits3/12/40.
  All before/after/style normalized full traces equal. FinalMaze adaptive weapon
  profile exercises phases0/1/2 and win macro. Pure termination bookkeeping and
  already-cached stable hero lookups normalized; engine calls/args/frames retained.
  First profile held the wrong weapon through every phase and reached onlyphase1;
  excluded. Full engine implementations are mocked, not in-game equivalence.
- KeepTabsOnWhisper288 cases/variant: level load1/3/6, quest expiry1/4/7, actor
  present/alive both, cancellation3/6/10/20, optional unload/reload. All four traces
  equal, exact creation frame/position asserted. Includes all three post-quest
  yields and cancellation during them. NativeD3CBD0 body
  5ecb198be5f2b1e12fb1f6993170c594ef2d60873a36370c857f73e033b0c625.
- PreMelee exact copied predicate32 cases/variant: hero hit OR magic&&!heal14,
  callback order and prior-flag clearing. Before16 failures/style, after0.
  This predicate-window oracle excludes surrounding movies/speech/frame loops.

NaN handling is intentional native fidelity, not just formatting. Extra full
Melee matrix864 cases/variant includes NaN: after-readable always equals both
drafts; old readable differs in96 NaN cases. Actual retail x87 comparison
D56AD2..D56AE5 executes8 f32 inputs (-inf,-1,-0,0,.5,6,inf,NaN) in Unicorn.
Fcomp literal0122DEDC=0, fnstsw/testAH41 set positive false for unordered NaN;
modern ordered test agrees, old inverted <= test disagrees. Window SHA
6286c3d53b48fcfc9ee0322138819d9b2ff0262091efdb4791cb641059a543c3.
GetHealth binding GSI index264 matches call vtable420. Getter/engine itself is
mocked; comparison instructions are retail bytes. melee_float_cpu.json definitive.
Other small diffs are reviewed negations/parentheses/list comparisons protected
by completed generator suite1826+564 from preceding source change.

Existing Guild suite baseline/after initially each100 failures,1189 pass +144
subtests, with identical failure IDs: test_guild_speech_values fixture lacked
RetailRandModulo after existing generator RNG recovery. Minimal fixture mapping
returns the supplied native rand modulo argument and preserves random event
recording; math.random adapter retained. Native instruction speech oracle and
all assertions unchanged. Dedicated693 native speech cases pass. Final existing
Guild suite1289 pass +144 subtests, Python subprocess exit0,42.40s.
First direct PowerShell redirected run reported shell1 despite passing summary;
final_checked_exit.json/log are definitive. No failures hidden/reclassified.
Initial full-matrix fixtures missing SkillRepeatKnown/SkillRepeating or wrongly
putting arrow sound on target instead of native hero were rejected and corrected.

No generator/evidence changes here: completed preceding all29 A/B proves only
Arena changed, Guild candidate identical before/after source overlay. Reconciliation
uses that completed fresh Guild draft. Every scratch draft/readable file parses;
both whole Guild smoke reports equal before/after, with existing limitations
retained. Candidate reports/source exact, promotion backups present. Corpus371
parses, statuses81/135/155 and diagnostics1114 unchanged; generic temporary count
6530->6267, gotos4335->4338/native names6087->6092 reflect restored paths and flag
cleanup, not a completion percentage.

Named offline guild-reconciled-candidate-20261002:58 Lua parse/63 manifest hashes,
Oakvale established stage plus all nine registered GuildTraining scripts, exact
tested readable_converter files (hand-reviewed tree never copied over).
Source sidecar07774e98c5675fb66aba7c23b4f1e4453b0434c0; DLL
af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
Standard quests/override scaffolding is generated by stage tool; initial helper
wrongly compared its empty custom-quest table to converter scaffolding and was
corrected, no executable Lua changes. Not installed/game-tested; previous named
Arena candidates unchanged. package_validation.json definitive.

Next: inspect generated CheckFriendlyAttacks predicate before warning progression
(nativeD45060 SHAed5daf17718d2c9bd14fcb50a8481d1b85b3d95ffe777363f4db6b6f0a8bf7d5).
Generated draft/readable_converter appear to clear the friendly-hit predicate at
a join, unlike the preserved hand-reviewed path. Need native control-flow evidence
and actual whole checker warning/punishment cases before changing lowering.
Arena/Guild in-game proof remains pending.

## Friendly-attack warning hypothesis rejected

Owned work/codex_lua_guild_friendly_guard_20261002. No generator, sidecar or
generated-reference changes. Actual whole CheckFriendlyAttacks executed6144
cases/style, draft and readable_converter, all12288 passed with identical
event/state traces. Hero bare/melee/ranged/no hit, Maze absent/present,
Maze hit/magic/heal14 combinations, initial warnings0/1/2/3, delayed level
load, resource acquisition delay, active Skill/Will test wait and cancellation.
Warnings1/2/3 increment exactly once; fourth-warning macro/resources exercise
normal completion and cancellation. Only ordinary villagers receive the
friendly/unkillable settings; sparrow, both Whisper definitions and Maze exclude.
Native CheckFriendlyAttacksD45060 bodySHA
ed5daf17718d2c9bd14fcb50a8481d1b85b3d95ffe777363f4db6b6f0a8bf7d5,
copied evidence/decompile native_evidence.json. Native true predicate flows past
LAB_d45782 false assignment. Generated FLOW jump correctly preserves it.
The prior apparent unconditional predicate-clear hypothesis was wrong; no fix
is warranted. This is Lua mock behavior plus native source inspection, not an
in-game run or whole native function execution. Installed v124 unchanged.
Command: python work/codex_lua_guild_friendly_guard_20261002/behavior.py, exit0.
Next: verify MeleeApprentice resource/movie cancellation and dialogue paths.

## MeleeApprentice complete generated resource paths checked

No generator/reference changes. Owned friendly-guard work/melee.py executes
4224 cases/style (8448 total) of actual whole Init/Main. All traces identical.
Stages:none,Skill,Will,Departure,WoodsDeparture,WoodsWill; alive/dead health,
talk/no talk, acquire delay, scripted-task duration, termination frames0/1/3/8,
cutscene/test handover both, normal marker absent/present. Mandatory Woods
path marker remains present there. Resource/movie handles tracked distinctly;
every acquired resource released exactly once, every movie destroyed exactly
once, including cancellation during task polls. Native MainD40CF0 full extent
SHA1d8215afa65a01b7c22b81851af8738377fcca2c933184714613ba7a9b2f7266
captured in native_functions.json/MeleeApprentice.c. Native body inspection plus Lua mocks, not full native or
retail runtime execution. Existing goto diagnostics refer to recovered cleanup
paths; no removal warranted. Command: python work/codex_lua_guild_friendly_guard_20261002/melee.py exit0.

ACTIVE next: speed-test ApprenticeSpeedTest MainD3E2E0's shown-once flag.
Generated GameState3 condition contains0==0 and repeatedly sets information.
Actual native D3E570..D3E59E byteflag[esp+4b] preserves one update. Baseline
whole Lua48 cases/style has13 repeated-update failures per style; initial
scratch fix48/style passes,8 native window scenarios/32iterations pass with
external engine calls mocked. Owned work/codex_lua_guild_speed_byte_20261002.
No source/reference promotion yet. Initial broad plain-clear fold rejected by
BordelloLady setup guard because packed slot also carries outgoing scalar
arguments. Narrowing to fully classified independent flag slots; all29 A/B,
adverse/broad tests and smoke still required before promotion. Discard initial
aborted broad run (candidate revised); no validation claim from that run.

## ApprenticeSpeedTest shown-once byte flag recovered

PROMOTED OFFLINE:5 Lua/report files (two ApprenticeSpeedTest Lua, three reports),
native_evidence_lowering.py and two new regression files. Owned
work/codex_lua_guild_speed_byte_20261002. Hand-reviewed Guild readable untouched.
Native MainD3E2E0 full extent SHA4334b54d124c31c76f915fd43049959f5b24fe38032420b19609a658cdd1056c.
The plain high-byte clear uStack_220&0xffffff was treated as a live whole-word
read, preventing the independent flag recovery. Draft emitted0==0 for the
GameState3 check; readable also discarded the later race-start flag check.
Now explicit local flag persists across frames and both checks, with native
full-word zero reset preserving its false projection.

Generic rule: accept a plain high-byte clear only where remaining containing
word uses consist of byte projection, zero reset, own CONCAT13 stores and
scalar declaration. Retain mixed lifetimes/escaping or arithmetic uses.
Initial broad candidate changed BordelloLady's outgoing scalar slots and was
rejected by its hash/operand-pinned reviewed setup adapter. Revised rule excludes
nonzero whole-word scalar stores (including0x41800000 float16.0), word copies,
arithmetic, addressed slots and packed multi-byte flags. All29 final nonempty
A/B trees verified; only GuildTraining/Entities/ApprenticeSpeedTest.lua changes.
Bordello is byte-identical. Initial aborted broad run is excluded from results.

Actual whole Lua idle48 cases/style and race-start18/style. Baseline13 idle+6
race failures/style; after0. Other event traces byte-equal after excluding only
intended information updates; both after styles equal. Delayed acquisition,
GameState timing, frame termination and race-start flag reuse exercised. Movie
cleanup/resource release and timers verified by retained tests. This covers
idle and entry to race, not complete race outcomes/retry/post-race dialogue.
Fresh native generation retained suite132 actual cases +22 generic/previous
high-byte cases: before39fail/115pass, candidate154pass, promoted root154pass
(18.87s, retained_exit.json/tool result). Independent native instruction window
D3E570..D3E59E SHA02e59aef3a3f89cb1a5d926ae9a1f1b8aa6ceda0d5f40ba2b30710ee3212eab9
executes32 iterations across8 initial-byte/cancellation scenarios in Unicorn;
actual byte load/test/jumps/store, external termination/GSI calls mocked.

Final broad subprocess exit0:1826 tests +564 subtests,790.71s. Known caught
Windows Unicorn access-violation diagnostics coexist with that passing summary;
process exit and final test summary recorded, not inferred from stderr.
Both Guild draft/readable smoke JSON identical; all fresh readable before
files match current refs exactly (zero baseline drift), only same one file
changes after. All scratch Lua parse. Source beforeSHA2d8179ca536d808b0fdd24eb9ee810802fb460403c6254726db96b93b08f18a4,
promotedSHAeb26c0548ef4490d68fdfe21215a9d906af3094b035748f22f574e4955fc4842. Exact backups pre_promotion/source_before.py.
Corpus371 parse,81/135/155 statuses,1114 diagnostics unchanged. Counts generic
6287/gotos4339/native names6093/machine temporaries4668; restored live flag code
and different local naming account for movement, not completion-rate claims.

Named guild-speed-flag-candidate-20261002:58 Lua parse/63 manifest hashes verified,
Oakvale+the nine existing Guild scripts. Same sidecar07774e98c5675fb66aba7c23b4f1e4453b0434c0,
DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
Previous candidates preserved. Installed v124 unchanged; no game launch.
Next: full ApprenticeSpeedTest race success/failure/retry/cancellation and
post-race dialogue paths from native evidence; leave switch-rejection diagnostic
unless an actual lost behavior is proved.

## ApprenticeSpeedTest initial/retry deadline timer identity recovered

PROMOTED OFFLINE:5 Lua/report files, converter hook +new generic
native_registered_timer_calls.py +two regression files. One earlier owned
information-flag fixture now accepts any registered timer ID (previously its
mock rejected ID3); saved baseline_test_native_speed_information_flag.py. No
sidecar changes or generated Lua hand edits. Owned
work/codex_lua_guild_speed_timers_20261002. Installed v124 unchanged.

Native MainD3E2E0 full extent SHA4334b54d124c31c76f915fd43049959f5b24fe38032420b19609a658cdd1056c.
RegisterTimer third result D3E4F7 stored at frame slot-0x250. Initial SetTimer
D3EC57 pushes duration then reads [esp+1c] (entry slot+18); retry D3FC05 reads
[esp+18] before pushing. Both use third ID. Decompiled calls wrongly named
second ID iStack_258. Draft/readable set the second timer to raceTime+20 while
third remained0; GetTimer(third) forced failure even after success. Full trace
with continued talk showed repeated rewards via that incorrect failure/retry
path. New Lua sets the third timer, preserving the second dialogue timer.

Generic evidence: unique scalar RegisterTimer result name +exact result-store
stack slot; global GSI receiver/vtable proven from native register definitions;
strict two-argument SetTimer setup and frame/depth; actual memory-loaded stack
operand from call IR. Cross only hash-pinned __ftol2 with preserved nonvolatile
registers, full helperBFEA70..BFEAE5 SHA
5333b186c02836974c6f792303aeb2c00d856316b93ccbbe65f51def6ae661b4.
Unknown receiver/global, byte/partial register writes, ambiguous call order,
mutable result name, indexed store, mismatched depth, complex printed operand
or changed callee bytes leave the original argument untouched. No guessed ID.
16 actual native caller windows (distinct IDs/four triples, duration0/25,
both setup orders) in Unicorn; SetTimer itself mocked, caller push/load/call
instructions original. Hashes/windows in native_timer_windows.json.

Actual whole speed Main96 lifecycle cases/style: success/no platform/too slow,
limits3/6/12/25, blocking speech0/2 ticks, question wait0/2, hero near/far.
Includes initial and retry setup, ambient switch conversations80 lines, success
and failure speech, post-race questions, cancellation, movies/resources and
three distinct IDs117/134/151. Both after styles identical. Third timer receives
25 for every race start; second never receives25. Success gold once at clock
4+3*speechTicks+questionTicks if before cancellation; no failure gold. Mock
clock advances one timer tick per script frame, not retail wall-clock seconds.
Binding Speak is blocking (stock LuaManager.cpp195 delegates Speak_Blocking);
initial nonblocking fixture was corrected and its partial traces superseded.
Before132 failures/60 passes in192 retained lifecycle cases, after192 passes;
9 adverse/identity cases yield201 total passed (17.53s). Root355 passed32.93s
including prior132 information cases +high-byte suites. Broad exit0:
1958 tests +564 subtests,856.65s. Known caught Windows Unicorn stderr traces
do not replace process exit/final summary as completion evidence.

All29 nonempty before/after trees: only ApprenticeSpeedTest.lua changes. Both
Guild smoke styles identical. Fresh before readable/draft byte-match current
refs, zero drift. Readable exact delta: two SetTimer(timerId4,GUI_RaceTime+20)
receivers become timerId3; no other readable bytes changed. Hand-reviewed Guild
readable preserved. Backups pre_promotion/source_before.py. Converter before
SHAd41f8a897ec7dde1a17a7719e6afb76d48ad16f58a6d1f599ac51f78c0d510ef, afterSHAb8b518f259f96b4030b66b73f31f0002e5cf30f4d03ef294cd1ad814e1a17fb5;
new moduleSHA9a4458bdd179739b79f87d5c31dd6f6f94e3b8b47f7e8dab58cec28a2b0c1b1f.
Corpus371 parse,81/135/155 statuses,1114 diagnostics and metrics unchanged;
two wrong timer operands were parseable and not flagged by diagnostic counts.

Named guild-speed-timers-candidate-20261002:58 Lua parse/63 hashes verified,
Oakvale+nine existing Guild scripts; same07774e98c5675fb66aba7c23b4f1e4453b0434c0
sidecar, DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
No installation or game launch. Previous bundles preserved. Next: existing
SkillApprentice Main cleanup/yield paths, proving behavior before changing any
diagnostic-only labels.

## SkillApprentice whole lifecycle and native counter identities verified

No source/reference changes. Owned work/codex_lua_guild_skill_lifecycle_20261002.
Actual whole Init/Main1536 cases/style,3072 total, identical event traces and
zero errors. Accept/decline, already testing, blocking speech0/2, first shot
delay0/3, controller/resource handover0/3, cancellation3/8/15, score increments
10/50, old high0/100, outside ring and Guild warning. Distinct timer/resource/
movie/map handles checked for exactly-once cleanup and no lifetime leaks.
Analytical review:40 completed tests,144 warning/ring aborts,384 declines,
768 already-testing interactions,200 cancellations;30 completed new highs.
Completed grade and macro sequence derived independently from seven retail
thresholds; grades never lowered, new highs monotonic, normal completion resets
HeroTakingGuildTest. Mock clock advances one tick per frame and definition
duration shortened to four ticks; not retail runtime equivalence.

Native MainD4C720..D4DE75 SHAf86a4dee1e123e75f1d089b6e584ce157acf5717df5b92dc690f347824f4619c.
Apparent high-score HUD receiver mix-up rejected: initial updateD4D464 uses
first counter EDI, both high/current updatesD4D51E/D4D53A use second counter
frame slot+48 ([esp+50] after two pushes).24 original caller instruction cases
with distinct IDs and score boundaries pass in Unicorn; external counter call
mocked. Both Lua forms match native. Do not change this retail behavior.
Evidence: native.json/native.c, before.json, review.json, native_counters.json.
No generated Lua edits, package, installation or broad test rerun needed.
Next: WillApprentice persistent help timer and bool-valued abort flag.

## WillApprentice numeric abort snapshot: active scratch correction

Owned work/codex_lua_guild_will_abort_20261002. No promotion yet. Native
MainD4EFE0..D505D5 SHA94210d61682022f637e8572682456d9cf1eb7a0bd1287edf9b14084bbe468be6.
After no-will help line D4FE24 reloads BL from saved byte[esp+4b]; D4FCC2
tests zero and continues.18 native reload/test cases pass, six byte values
and three old EBX values, no mocked engine calls. Existing Lua cVar5 = numeric
c_stk_115 copies0 into a boolean-tested loop flag; zero is truthy in Lua,
so help prematurely aborts the test. Both Lua forms affected.

Scratch generic fold_char_flags explicitly zero-tests a copied numeric byte
snapshot only when plain declared char/byte/undefined1 slot, all definitions
literal0/1 and no address or mutation. Candidate final270 checks pass11.57s
(14 identity/adverse +256 actual freshly generated WillApprentice lifecycle).
Baseline actual20 fail236 pass11.70s. Whole promoted-source baseline1536
cases/style vs candidate1536/style:112 changed/style, all accepted tests with
zero will energy. Completion/grades independently checked; persistent help
timer77 preserved, other handles released once. Definition timer shortened to
four mock ticks, no retail runtime claim.

Pre-existing64 draft/readable differences preserved: when blocking Speak
returns false on termination while mocked task remains active, draft polls
once again and yields one extra frame; readable exits on Speak result. Same
case identities before/after, no later macro/master-state changes. Binding
Speak_Blocking itself yields before checking termination (LuaEntityAPI.cpp
2823..2831). Separate duplicate-poll review remains; do not silently dismiss
or claim all Will style traces identical.

Initial guard missed inline nonliteral assignment and prefix increment; two
added adverse tests failed. Stopped owned broad/all29 before revision; artifacts
comparison_superseded_guard and tests_superseded_guard.log explicitly superseded.
Final candidate now checks all assignments and both increment forms. Broad
and all29 reruns active. Next readable A/B, both smoke forms, exact guarded
promotion/audit and named offline package if all gates pass. Installed v124
unchanged. User requested continued Lua work until stopped; no stop received.

## WillApprentice abort snapshot correction promoted offline

PROMOTED:5 Lua/report files, generic fold_char_flags extension and two new
regression files. Owned work/codex_lua_guild_will_abort_20261002; backups
pre_promotion/source_before.py, hand-reviewed Guild readable unchanged.
Exact readable delta: scratchValue3 = scratchValue4 becomes scratchValue3 =
scratchValue4 ~= 0. After no-will reminder, numeric byte zero now continues the
test; saved one still aborts. No sidecar modification or generated hand edit.
Source lowering beforeSHAeb26c0548ef4490d68fdfe21215a9d906af3094b035748f22f574e4955fc4842,
afterSHA50fadede10c112a2ca63de6c50eb34233b38333d2817db0a8d158784d8443a90. Final snapshot guarded against source drift.

Baseline actual20 failed236 passed; candidate/root270 passed (14 generic
identity/adverse +256 freshly generated whole Will lifecycle). Broad2169 tests
+564 subtests pass, subprocess exit0; precise elapsed/result in broad_exit.json
and tests.log. Initial guard revision run explicitly superseded (two added
adverse failures; stopped broad/all29 before input change). Final all29 have
nonempty before/after FSE trees; only WillApprentice.lua differs. Both smoke
styles identical. Full Guild readable before byte-matches current refs; only
that one line changes. Parallel readable builder's final file-only equality
assertion failed because full build adds native comments and hoists immutable
WillHelpTimer getter. Inspected those differences and then ran actual full
readable1536 scenarios; every trace equals the file-only candidate. Separate
final_generated_review.py verifies exact promoted delta and baseline identity.
No validation claim from that failed wrapper assertion alone.

Native abort reload/test18 cases pass with original instructions, no engine
mocking. Actual full Lua1536 cases/style,112 changed/style, exclusively accepted
zero-energy scenarios. Earlier entry details timer/resources/grade oracle and
mock time limitations. Pre-existing64 blocking speech cancellation differences
still preserved. Additional native return-speech pollingD4FACA..D4FB0A16 cases
with task/frame/termination helpers mocked confirm one yield to cancellation
then immediate exit; native_speech_poll.json. Draft's duplicate poll can yield
one extra frame after binding Speak_Blocking already returned false. This is
the next generic binding/generation recovery task, not hidden as equivalence.

Corpus audit summary: {"counts": {"genericTemporaries": 6287, "gotos": 4339, "machineTemporaries": 4668, "nativeNames": 6093, "unresolvedDiagnostics": 1114}, "files": 371, "statuses": {"needs-human-review": 155, "needs-readability-work": 135, "needs-recovery": 81}, "syntaxFailures": 0}.
Named guild-will-abort-candidate-20261002:58 Lua parse/63 manifest hashes;
Oakvale+nine existing Guild overrides, same sidecar source07774e98c5675fb66aba7c23b4f1e4453b0434c0
DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174. No installation/game launch; installed v124 unchanged.
User's continue-until-stopped instruction remains active. Next: duplicate
post-blocking speech polling/cancellation, native/sidecar evidence first.

## Blocking speech duplicate cancellation poll: active scratch recovery

Owned work/codex_lua_guild_speech_cancel_20261002; root/reference unchanged
after the completed Will abort-byte promotion. Native return-speech poll
D4FACA..D4FB0A16 cases: task query/frame/termination helpers mocked, original
retail control flow; cancellation yields to limit once, then exits immediately.
Speak_Blocking binding itself already implements that wait and returns false
on termination. Manifest Entity/bool/blocking=true; excerpt/hash captured in
binding_contract.json and Speak_Blocking.cpp. Existing draft polls again when
mock task remains active after the binding returned false, yielding one extra
frame. The readable fold only avoids it on some speech paths.

First two lifter-query prototypes discarded:98 cancellation failures remain.
Emitter uses automatic rN = me:Speak result assignments and resource wrapper
dispatch; simple adjacency missed these. More fundamentally, suppressing the
query alone can bypass a native loop's only terminating cleanup if no check
follows the loop. Do not promote candidate_lift_native_lua.py or revise.py.

Final current prototype: generic native_blocking_speech_cancellation.py with
one converter file hook. Match the same actor's immediate task query, optional
single flag alias, exact native wait, first NewScriptFrame and termination
predicate/inverse, and an already recovered exiting cleanup. Reuse that exact
exit when Speak returns false; retain the original wait. No guessed return,
label or resource. Skip missing blocking contract, different actor/query,
intervening work, alternate loop condition/predicate, nonexiting/ambiguous/
TODO cleanup, local or nested cleanup blocks. Existing result assignment kept.

Candidate466 checks pass32.22s (18 identity/adverse +448 actual fresh whole
WillApprentice draft/readable). Baseline98 fail350 pass13.84s, captured completed
retained_before.log/retained_before_exit.json. Actual tests include first/waiting
speech, accept/decline/already testing, task2/5, cancellations3/4, grades,
warning/distance abort, persistent help timer, and resource/movie lifetimes.
Native time mocked; no retail runtime claim. Root baseline1536 cases/style
captured in before.json. Broad2439+564 expected (not completed claim) and all29
before/after regeneration active against frozen candidate. Next: inspect changed
units, full readable regeneration +whole traces, both smoke forms, required
guarded promotion/audit and named offline bundle. Installed v124 unchanged.
User continue-until-stopped remains active. No stop received.

## Blocking speech terminating cleanup promoted across existing units

PROMOTED OFFLINE:184 Lua/report files (65 draft +65 readable +54 reports)
across18 existing units, converter hook +new generic module +two regressions.
No lifter change, no sidecar change. Owned
work/codex_lua_guild_speech_cancel_20261002; backups pre_promotion/source_before.py.
Guarded before/reference identity for every affected full draft/readable tree,
zero baseline drift. Hand-reviewed Guild tree preserved. Source converter
beforeSHAb8b518f259f96b4030b66b73f31f0002e5cf30f4d03ef294cd1ad814e1a17fb5,
afterSHA562ef61d1e192a9835f8391d99d1c4131d00a99cf1d70c57b420045d09a355e8; moduleSHAad921f3fdc4496eb4316e96b5aad02b02d82afa19a8240d3bf84671e019fdedd.

Generic adapter371 matched waits in65 files: exact same-actor immediate task
query/optional flag copy, native wait's first frame, explicit termination
predicate/inverse, and already recovered exiting cleanup. On blocking Speak
false, reuse that exact native exit before a duplicate poll can yield again.
Existing result assignment and original wait retained; no guessed cleanup or
new binding. Adverse cases reject missing Entity/bool/blocking contract,
different query actor, intervening work, alternate loop/predicate/condition,
nonexiting, local/nested or unresolved cleanup. First two lifter prototypes
are explicitly discarded; do not promote those files.

Native Will return-speech pollD4FACA..D4FB0A16 cases, original instructions,
task/frame/termination mocked. Pin actual packaged source's Speak_Blocking
excerpt in binding_contract.json/Speak_Blocking.cpp (source07774e... from owned
sidecar candidate); older session-start source inspection was cross-checked
against this actual source before promotion. Baseline98 failed350 passed13.84s.
Candidate466 checks pass32.22s (18 identity/adverse +448 actual newly generated
Will draft/readable lifecycle), promoted root466 pass17.25s. Broad2439 passed,
564 subtests,1087.62s, subprocess0; caught Windows Unicorn stderr retained.
Full actual Will1536 cases/style: draft128/readable64 traces repaired. Both
now identical, every exit at cancellation limit, all semantic events and cleanup
ordering preserved after removing obsolete extra frame/cleanup-clock delay.
Full generated readable equals file-only candidate traces. Mock timer duration
four ticks; no retail whole-game equivalence claim.

All29 nonempty before/after trees; only18 units change. Adapter evidence maps
each changed file; Speak/frame call occurrence counts unchanged. Both smoke
styles identical across all18. Generic smoke's successful Speak now returns
true (it previously returned nil despite declared bool/blocking binding),
same correction in both before/after, documented in prepare_smoke.py.
No hidden normalization of changed cancellation traces. Fresh full readable
builds all18,65 readable deltas, parse checked. Per-file reuse only when complete
source/style/frame/relative-path/writer inputs identical, typed cache keys and
deep-copy reports; cache counts retained. Initial wrapper failed serializing
tuple writer keys before building, then revised wrapper completed all18 child
builds but its final review imported nonexistent script_unit. Neither wrapper
failure is a validation claim. Independent final_readable_review.py verified
all18 completed reports, full syntax, exact baselines and changed file sets;
wrapper imports fixed only after completion, no regeneration needed.

Corpus {"counts": {"genericTemporaries": 6288, "gotos": 4614, "machineTemporaries": 4668, "nativeNames": 6371, "unresolvedDiagnostics": 1114}, "files": 371, "statuses": {"needs-human-review": 155, "needs-readability-work": 135, "needs-recovery": 81}, "syntaxFailures": 0}.
Gotos/native names increase because existing addressed native exits are reused
in the new early guards; no diagnostic suppression or label-removal claim.
Named speech-cancellation-candidate-20261002:185 Lua parse/190 manifest hashes,
18 existing units +established Oakvale override (32 script identities), same
sidecar source07774e98c5675fb66aba7c23b4f1e4453b0434c0 DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
No installation/game launch; installed v124 unchanged, older bundles preserved.
Next: CombatApprentice MainD4A270 full fight/grade and cancellation paths.
User continue-until-stopped remains active; no stop received.


## CombatApprentice score float narrowing (active scratch)

Owned work/codex_lua_combat_apprentice_20261002. Actual promoted Init/Main
3456 cases/style, 6912 total, no errors, identical traces; cleanup handles
distinct and released once. Matrix covers accept/decline/already testing,
missing/dead/live partner, fight duration, speech/macro blocking, resources,
win/loss/block/ranged/lightning/warnings and cancellation. review.json checks
54 grade scenes, 15 grade commits and native macro/grade ordering.
Main D4A270..D4C700 SHA635cf65bcb0ab9b646d099c0e4852c70b2e47c500ff6adc91b526c8cc7fd330a.
Native D4BD4F..D4BD9A score window: original x87 arithmetic/FSTP32/threshold
loop, termination helper only mocked false. 18 cases, both nearest-rounding
53/64-bit precision modes; eight baseline grade mismatches. Explicit final
(float) cast remains in lowered C but RE_CAST erases it in Lifter.expr.
Whole-script boundary baseline32 cases,16 wrong grade selections, corrected
scratch prototype32 cases,0 mismatches in draft/file-readable forms.
Candidate preserves explicit float casts around scalar add/subtract through
Lua standard string.pack/unpack; excludes calls/pointers/integer casts and
unsupported operations. Twenty focused guard/precision checks pass after
one adverse integer-cast rejection initially failed and was corrected.
All29 nonempty A/B running; first incomplete run stopped before a guard
revision, no validation claim from it. No generator/ref promotion yet.
Typical bounded health arithmetic tested, not universal x87 extended-precision
equivalence across arbitrary exponent spreads or rounding modes. Timer and
resource/macro helpers modeled; no game launch, installed v124 unchanged.
Next: inspect all29 deltas, retained actual regression, full readable/smoke
and broader tests before any promotion. Continue-until-stopped remains active.


## Combat score float narrowing promoted offline

Owned work/codex_lua_combat_apprentice_20261002. The explicit final float
cast survives lowered C but was erased by Lifter.expr. Generic native_float32
now preserves scalar add/subtract cast narrowing using Lua standard float
pack/unpack. Unknown calls, pointers, integer conversions and unsupported
operators excluded. Actual pinned sidecar enables string library in both
entity/quest VMs; no new binding or DLL required.

CombatApprentice MainD4A270..D4C700
SHA635cf65bcb0ab9b646d099c0e4852c70b2e47c500ff6adc91b526c8cc7fd330a.
Native scoreD4BD4F..D4BD9A executes original x87 arithmetic, FSTP32 and
threshold loop; termination helper mocked false. Eighteen cases, eight old
Lua grade mismatches. RealGuildmaster second grade window18 cases also
confirms eight old mismatches. Fifty original health subtraction/store cases
(Arena/BanditCamp/WoodsWill/DarkwoodTrader/WaspVictim), fourteen original
StatueMaster fraction stores:100 native cases total, all stores match float32.
Nearest rounding modes with PC53/PC64 checked; register/stack seeds recorded.
No universal x87 arithmetic equivalence across arbitrary exponent spreads,
precision/rounding modes or prior world-time arithmetic claim.

Actual fresh converter regression baseline8 failed8 passed25.35s; candidate
16 pass20.86s. Twenty generic guard/precision checks and promoted root36
checks pass. Native integer-cast adverse test initially failed, guard fixed
before final comparison/broad runs. Both whole Lua forms32 boundary cases,
16 baseline wrong grade selections ->zero candidate wrong selections.
Actual full Init/Main3456 scenarios/style (6912 total), every trace unchanged
for established lifecycle inputs; win/loss/block/illegal attacks/warnings,
cancellation, blocking speech/macros, resources and once-cleanup all checked.
Broader2941 tests +564 subtests pass, subprocess0; exact log retained.

All29 nonempty A/B; nine draft/readable files across six units. Twenty-three
other units identical. Only explicit narrowing wrappers differ. Fresh full
readable builds and both smoke styles identical across all six units.
Arena main baseline drift is two reads through immutable timerId5=timerId
(assigned once, never mutated); exact proof baseline_drift_review.json.
Unchanged root Arena main preserved byte-for-byte, as is hand-reviewed Guild
readable. First incomplete A/B stopped before guard revision; no completion
claim from it. Native health-window initial page mapping failure fixed,
all50 rerun successfully. Early broad wrapper NameError occurred before
pytest, corrected before completed broad gate.

Promoted 24 refs plus lifter, generic module and two tests,
exact guarded backups and source hashes retained. Corpus {"counts": {"genericTemporaries": 6288, "gotos": 4614, "machineTemporaries": 4668, "nativeNames": 6371, "unresolvedDiagnostics": 1114}, "files": 371, "statuses": {"needs-human-review": 155, "needs-readability-work": 135, "needs-recovery": 81}, "syntaxFailures": 0}.
Named float-score-candidate-20261002: 130 Lua parse /
135 manifest hashes, six existing units plus
established Oakvale override. Sidecar source 07774e98c5675fb66aba7c23b4f1e4453b0434c0
DLL af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174. No installation/game launch; v124 unchanged.
Next: GuildTrainingWoodsWill MainD67890 full bandit setup/battle lifecycle and
existing native helper/resource handling. Continue-until-stopped remains active.


## Guild Woods Will lifecycle and raw animation byte (active scratch)

Owned work/codex_lua_guild_woods_will_20261002. Actual quest Main, four helper
threads and WillWhisper entity under owned scheduler:6144 cases/style,12288
total, no errors, identical traces. Three retail bandit placements modeled;
load/acquisition/fight/help/macro delays, hero health/death, Xbox/PC help,
Whisper melee/special/heal attacks, unload/reload, cancellation.1992 completion,
1584 failure,2568 no-result cancellation scenarios. Strict distinct resource,
movie and actor-map once-cleanup. Scheduler/deactivation/control arbitration
and engine calls mocked; not retail whole-game proof. Initial944 failures/style
were fixture expecting Xbox help key on PC; preserved before_fixture_pc_help,
then corrected explicit PC expectation and all12288 rerun successfully.

Native MainD67890..D685F2
SHA5618eef81f67a64136e1b13cfbb977885ccfb7a1b05e5777ea35f547dc979c1b;
WillWhisperD68810..D68ADF
SHA7e4dc0e8f7736000b31aa6689b6077b2e59199071ae8430762fc360aebbfff29.
Native identities for four helpers retained. Sidecar NewResource auto-prepares
on first TryAcquire; absent explicit bandit Prepare is intentional, not a fix.

Native animationD68AAC samples writable byte01375748 AFTER CString ctor.
Generic dat_bool read the static01010101 fill and emitted true. Baseline
constant-one lifecycle cannot exercise this mismatch. New actual entity80
cases:48 raw-byte mismatches; fresh converter baseline48 failed32 passed10.01s.
Candidate80 behavioral+14 ambiguity/native-byte guards94 pass10.05s. Candidate
converter hook restores only direct 7E73D0 calls with nine source operands,
complete callOrder pairing, six proved constant flags and IR unsigned-byte
load in native stack argument5. Lifter forwards tracked resource to existing
PlayAnimationWithNativeArgument5. Other animation forms remain outside this
first scoped correction; no loop-animation/wider mutable-global claim.

Original native caller and resource shim24 cases:raw0/1/2/127/128/255,
before-ctor byte0/85, populated/empty expert. CString ctor changes byte, caller
samples it afterward; original tail dispatch/RET32 run, ctor/dtor/expert mocked.
First harness resource offset seed was wrong (0x20 vsnative0x2c), corrected
and all24 rerun. Candidate matcher initially missed source line breaks;
instrumented paired source spans, corrected before focused/all29 runs.
Exact packaged source0777 helper, FableString and resource/CString types compiled
x86:21 cases pass (raw forwarding, empty/invalid expert, CString/action errors,
invalid resource); registry/engine/global-address mapping mocked. Source
fragments/hashes and PE machine retained; no new DLL or install.

All29 A/B running, already changed WillWhisper, three BanditCamp entities and
TraderEscort FleshEatingBalverine. No root generator/ref promotion. Next:
inspect completed deltas/native evidence, full readable/smoke, broad tests,
full lifecycle with raw1 before exact guarded promotion and named package.
Installed v124 unchanged; continue-until-stopped remains active.


## Guild Woods Will raw animation byte promoted offline

Completed owned work/codex_lua_guild_woods_will_20261002. Generic recovery
routes proven direct resource PlayAnimation fifth-byte loads to the existing
PlayAnimationWithNativeArgument5 ABI helper, preserving the acquired resource,
key and six other flags. Native stack operands/helper bytes/callOrder required;
unknown operands, changed helper, incomplete caller and ambiguous pairing skip.
No blanket dat_bool change; loop animation and other writable globals remain
outside this correction. Existing sidecar samples byte01375748 after CString
construction and forwards its raw unsigned value without bool normalization.
Native caller/shim24 cases and exact-source x86 helper21 cases passed; mocked
engine/registry/constructor boundaries recorded in pending entry and reports.

Fresh actual converter baseline48 failed32 passed; candidate94 tests passed.
Promoted root94 passed3.16s. Full readable/draft80 raw-byte cases identical and
correct. Actual quest Main/four helpers/WillWhisper6144 lifecycle scenarios per
form (12288 total) remain trace-identical at raw1 with exact acquired receiver
assertions; resources/movies/maps released once. Broad3035 tests +564 subtests
passed982.16s, subprocess0. Caught Unicorn Windows fault diagnostics remain in
the log; final pytest summary and subprocess exit establish this gate.

All29 nonempty A/B: seven entity files across GuildTraining, BanditCamp,
TraderEscort, WaspBoss and WhiteBalverine. Twenty native/source paired sites
become22 emitted Lua calls: WB_WhiteBalverineE158F0 existing sibling-tail pass
copies two calls into another branch in BOTH baseline and candidate. Captured
before/after statements prove4->6; exact inverse edits reproduce baseline Lua
in all seven files. Every caller native hash/stack witness retained. No new
control-flow duplication introduced. Twenty-four other units byte-identical.
Fresh full readable builds have the same seven deltas and zero baseline drift.
Both smoke forms retain identical diagnostics, outcomes, frame and call counts;
four units have identical traces. Wasp HornetDrone trace intentionally changes
me:PlayAnimation (readable arg1:PlayAnimation) to the resource ABI helper.
Actual smoke JSON retained, sole receiver/method delta explicitly reviewed.

Promoted 24 refs plus converter/lifter, generic evidence module
and two tests. Exact baselines, backups and candidate hashes retained;
hand-reviewed Guild readable preserved. Corpus371 parse,1114 diagnostics;
6288 generic/4614 goto/6371 native/4668 machine temporaries unchanged;
statuses81 needs-recovery/135 readability/155 human-review unchanged.
Named raw-animation-byte-candidate-20261002: 114 Lua parsed,
119 manifest hashes verified, five existing
units plus Oakvale, sidecar source07774e98c5675fb66aba7c23b4f1e4453b0434c0,
DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174. No installation or game launch;v124 unchanged.
Next: existing GuildTrainingWoodsDeparture quest/helpers and FinalMaze
training phases, timers, hit predicates and cancellation. Continue-until-stopped
instruction remains active.


## Guild departure/Maze lifecycle and delayed-load draft failure (active scratch)

Owned work/codex_lua_guild_departure_20261002. FinalMazeD647F0..D664CD
SHAdd5f24ff7d82da3dde30d4aaf3cda31f6da19e4d00b0e0180311a1b4c4eaeba2:
actual Init/Main2048 cases/style,4096 total, identical traces and strict
resource/movie/map/timer once-cleanup.248 completed1800 cancelled per style;
1792 melee/736 skill/268 lightning phase entries;508 speeches per style.
Correct/wrong/mixed weapon/lightning classes, control/macro/speech delays,
live/dead Maze health, active-conversation windows, RNG0/1 and cancellation.
Seven-hit phase transitions and expected warning classes checked. Original
six caller hit predicates324 cases, non-one native bool returns/stale output
byte included, all match expected OR and optional !conversation gating.
CString constructors and engine virtual predicates mocked; no in-game claim.

Whole WoodsDeparture quest Main/four helpers288 cases/style initially:
draft144 fail144 pass; readable288 pass. Delayed GuildWoods load makes Ghidra
CVar5 cleanup word copy a reused local_24 string; surviving bit8 guard then
performs Lua bit operation on string. Fresh retained converter regression:
144 fail432 pass. Existing generic native_reviewed_temporary_cleanup mechanism
removes only pinned flag8 set and exact temporary CString destructor guard.
MainD61D20..D62137 SHA074e510834bbb7ef943f4475f26d9f7a141b6a264e3f896c06f088610463c106;
constructor D61F3B, bit setD61F49, testD61F87/clearD61F90/destructorD61F93.
Original guard ten native inputs proves only conditional destructor and bit
clear, preserves other bits. Engine/thread operations retained. Candidate
registered evidence only; no generator/sidecar source changes proposed.
Fresh candidate576 pass2.29s. First candidate failed closed at exact span
guard because existing hook had already renamed local_24->xStack_24 and
joined std template lines. Captured hook input, corrected exact evidence span;
original source/native hashes unchanged. Failed candidate/log/partial A/B
preserved as attempt1. Validation wrapper initially referenced absent smoke
filename, corrected before broad test start. No root promotion yet.

All29 final A/B and broad regression now running. Next completed comparisons,
full readable/smoke, whole helper traces and exact guarded evidence/ref
promotion with a separate named offline Guild candidate. Installedv124
unchanged; continue-until-stopped instruction active.


## Artifact thief movie branch cleanup (queued scratch while departure gate runs)

Owned work/codex_lua_guild_artifact_20261002. Existing registered ArtifactThief
MainD62480..D63CB3 SHAbe063b420d7c78e2d9839af8bc3f437b573fa3e07c9858fb5d58af99f65b28f9.
Actual attack96 scenarios/style:30 unreleased movies per form, no Lua errors,
strict timer/resource cleanup otherwise. First native movie constructor in
printed C maps to D62A8A entryStack-312, second toD628DD entryStack-188;
callOrder pairing proves source/native ordering (numerical Ghidra slot names
are not native stack offsets). Normal member releaseD62BCD/xStack_130 and
D62A21/xStack_b4 both join base destructorD62BDE after base-vtable stores.
Existing native_movie_phi_cleanup/native_reviewed_movie_members mechanism
already handles this shape, but requires registered reviewedMovieMembers
evidence. Omitted row drops normal branch destruction in both Lua forms.

Pinned row uses two movie start/member release pairs and shared base call;
source/native hashes and surrounding instructions retained. Scratch actual
entity regenerates with two DestroyMovie calls. All96 attack scenarios per
form pass,30 old leaks corrected. Fresh retained tests before60 failed132
passed17.00s; candidate192 passed18.49s. Six original native branch cleanup
executions prove correct distinct movie member, base-vtable change and shared
destructor; release/base helpers mocked, other movie untouched. No full
engine/ABI or in-game claim.

Wider dialogue/purchase2048 cases per form:672 baseline movie leaks per form
->zero candidate.384 question/96 purchase/768 lamp/768 attack cases per form.
Exact trace review proves ONLY missing destruction events added; all other
dialogue, gold transfer, movement, attacks and cancellation events unchanged.
Generic guard tests already exist for reviewed movie lifetimes/changed native,
source, frame, slot, members and ownership. No generator/DLL changes proposed.
No root mutation or broad/full-readable/all29 completion claim for this chunk.
Departure temporary-cleanup gate runs first; then take fresh root baseline,
merge this row into registered evidence, all29 A/B and full readable/smoke,
broader regression, exact promotion/backups and unique offline candidate.
Installed v124 unchanged; continue-until-stopped active.


## Woods departure delayed-load cleanup promoted offline

Completed owned work/codex_lua_guild_departure_20261002. Existing reviewed
temporary-cleanup mechanism and pinned registered evidence remove only the
bit8 WatchForTermination temporary-string flag set/destructor guard. No
generator or DLL source change. Exact inverse generated edit is three dead
draft Lua lines; native engine operations/helper bodies untouched. Full29
nonempty A/B: only GuildTrainingWoodsDeparture quest draft changes,28 other
units identical. Full readable output byte-identical; zero baseline drift.
Both smoke forms identical.576 fresh actual converter cases now pass;
baseline144 fail432 pass ->candidate576 pass2.29s; promoted576 pass2.35s.
Whole full-build helper288 cases/style trace-identical to prior readable and
every previously passing draft path.16 additional pre-level-load cancellation
cases create no bindings/threads/Maze. FinalMaze4096 actual lifecycle cases,
324 original native predicate cases and ten native temporary-destructor guard
cases passed; mock engine/constructor/destructor boundaries retained.

Broad3611 tests+564 subtests passed999.22s, subprocess0; caught Unicorn Windows
fault stderr retained with final passing summary. Promoted 3
refs (draft+conversion reports), registered evidence and actual lifecycle test.
Exact unit/source guards/backups retained; no readable rewrite; hand-reviewed
Guild tree preserved. Corpus371 parse/1114 diagnostics and all inventory
counts/statuses unchanged. Named woods-departure-cleanup-candidate-20261002,
58 Lua parsed/63 manifest
hashes, established Guild+Oakvale overrides, same sidecar source
07774e98c5675fb66aba7c23b4f1e4453b0434c0 and DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
Offline only; installedv124 unchanged. Next: prepared ArtifactThief branch
movie cleanup row, take fresh baseline including this promoted metadata,
full A/B/readable/smoke and justified movie/resource regression before promotion.
Continue-until-stopped active.


## Artifact thief movie cleanup promoted offline

Completed work/codex_lua_guild_artifact_20261002. Registered reviewedMovieMembers
row uses existing generic shared-base destructor fold; no generator/DLL source
change. Original constructor/member-release stack identities and shared base
destructor pinned. Distinct native movie branch cleanup six cases passed;
release/base helpers mocked, other movie untouched. Actual fresh attack192
tests baseline60 failed132 passed17.00s ->candidate192 pass18.49s. Full dialogue
2048 cases/style:672 old leaks per form ->zero.384 question/96 purchase/768
lamp/768 attack cases per form. Exact events unchanged except missing releases.
Full before/after readable builds verify4096 candidate lifecycle traces match
focused candidate;192 attack cases remain correct. Both smoke forms identical.

All29 A/B: only ArtifactThief draft/readable change;28 other units identical,
zero baseline drift. Draft replaces two dead pointer assignments with branch
movie releases and removes their dead declaration. Readable adds the same two
DestroyMovie calls. Promoted 4 refs plus registered evidence
and actual lifetime test, exact backups/hash guards; hand-reviewed Guild tree
preserved. First promotion preflight rejected an inverse-edit check because
the same movie-release text also exists on cancellation paths; no mutation
occurred. Guard refined to exact lamp-transfer/pause/branch spans, then passes.

Relevant movie/resource/cleanup/readability regression1534 tests +549 subtests
passed466.06s, subprocess0. Selection justified in regression_selection.json;
prior3611+564 full suite against identical converter/lifter/sidecar completed
immediately before this evidence-only change. Promoted actual artifact+departure
root768 tests pass16.33s. Caught Unicorn stderr retained with final passing exit.
Corpus371 Lua parse/1114 diagnostics; all inventory counts/statuses unchanged.
Named artifact-movie-cleanup-candidate-20261002:58 Lua parse,
63 hashes; established Guild/Oakvale overrides;
same sidecar source07774e98c5675fb66aba7c23b4f1e4453b0434c0,
DLLaf5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174. Offline only; installedv124 unchanged.

Next discovered raw animation coverage issue: ArtifactThief waveD626AD uses
same proven helper7E73D0 and byte01375748 after CString ctorD62687. Native
complete pairing is available at the earlier movie-hook input, but late raw
adapter input cannot reconcile full callOrder and correctly declines. Static
true remains in this wave; prior raw fix explicitly excluded unpaired cases.
Owned next work/codex_lua_animation_pairing_20261002 verifies24 original wave
caller/shim cases, including ctor-mutated byte and populated/empty resource.
Investigate earlier guarded recovery while preserving late fallback, focused
actual wave regression and all29/full-readable/broader generator checks.
Continue-until-stopped active; no blanket mutable-global or looping ABI claim.


## Earlier guarded raw animation pairing (active generator scratch)

Owned work/codex_lua_animation_pairing_20261002. Root unchanged after completed
artifact movie promotion. Existing raw-animation adapter can prove waveD626AD
at the earlier movie-hook input, but late fully transformed input has no
complete callOrder reconciliation and correctly declines. Candidate converter
adds guarded recovery immediately before reviewedMovieMembers cleanup/call
label rewrites; retains existing late fallback. Native helper/byte/flags/receiver
proof and ambiguity exclusions unchanged. No new API/DLL or blanket dat_bool
change. Original ArtifactThief wave ctorD62687, raw byte loadD6268E and
helperD626AD..shim7E73D0:24 actual caller/shim cases passed, constructor changes
byte before sampling, raw0/1/2/127/128/255, populated/empty resource. Expert,
CString ctor/dtor mocked; exact native bytes, stack cleanup and hashes retained.

Actual wave120 cases cover raw0/1/2/128/255, repeated wave after byte change,
near/far hero, control acquisition and cancellation. Fresh baseline36 failed
178 passed17.82s including existing generic14/WillWhisper80 checks. First
candidate214 passed15.32s. Added two actual converter adverse cases. Deleting
callOrder also invalidates unrelated registered movie review source pairing:
initial new-negative candidate1 failed215 passed30.99s; equivalent baseline
fails same reviewed movie shared-base inventory guard10.12s. Failure retained;
negative fixture now omits only unrelated movie evidence to isolate raw-byte
recovery guards. Normal fixture retains all registered movie evidence. Two
isolated adverse cases pass6.86s; final full focused216 pass49.10s. Missing
callOrder/changed helper bytes emit no raw helper and retain old animation.

All29 A/B, full readable builds and broader generator regression running
against frozen candidate source/tests; no completion/promotion claim. Next:
inspect every new delta/native pairing, preserve previous raw recoveries and
movie/quest fixes, full lifecycle replay at raw1 and changing-byte focused cases,
smoke/API trace review, exact guarded promotion and new offline candidate.
Installedv124 unchanged, same sidecar0777/af5d; continue-until-stopped active.


## Earlier guarded raw animation pairing promoted offline

Completed work/codex_lua_animation_pairing_20261002. The converter now invokes
the existing guarded adapter before movie-member cleanup rewrites, retaining
its late fallback. Complete source/native pairing, exact helper bytes, receiver,
raw byte address and remaining flag proofs still required. No new API or DLL.
Captured 15 native sites across eight functions/six existing registered units.
All29 baseline/candidate regeneration: eight draft files change;23 units identical.
Full readable builds match the reviewed eight-file deltas; zero baseline drift.
Inverse edits recover baseline exactly. Both smoke forms for all six units
have identical results/traces. Guild hand-reviewed readable tree preserved.

24 original ArtifactThief caller/shim cases passed, including constructor-mutated
byte and empty/populated resource. Helpers mocked; original bytes and stack
purge checked. Actual focused216 tests pass49.10s, including missing complete
callOrder and changed helper byte guards. The initial adverse fixture failure
and equivalent baseline failure are retained: unrelated movie evidence also
requires callOrder. Isolated animation negative fixture excludes that evidence;
normal cases retain it. Full thief lifecycle4096 traces at raw1 unchanged,
without movie leaks. Another120 full wave traces preserve changing-byte flags.

Frozen candidate broader3925 tests+564 subtests pass1096.40s. Promoted root984
focused tests pass33.56s. Exact guards/backups promote29 refs, converter hook
and actual wave test. Corpus371 parse;1114 unresolved diagnostics unchanged.
Generic temporaries6290 (up2); gotos4614/native names6371/machine temporaries4668.
SickChild's dynamic animation key remains unresolved: xStack_94 is declared
and read but never assigned, preserving previous nil argument and original
CDefString diagnostic. No key recovery or retail whole-game equivalence claim.

Named early-animation-pairing-candidate-20261002:112 Lua parse/117 manifest
hashes; six units plus established Oakvale override. Same sidecar source
07774e98c5675fb66aba7c23b4f1e4453b0434c0 and DLL
af5d377eec7690387b8d6af830e8285763f4268fc0512c7ef3b5fd4be70fd174.
Offline only; installedv124 unchanged. Continue-until-stopped authorization
active. Next inspect existing SickChild sister definition-string/vector key
evidence; keep unresolved behavior explicit until native/ABI operands proven.


## Bedtime stop requested; SickChild sister key remains scratch

User explicitly requested documentation update and stop for tonight. Continuous
work authorization is paused by that request. No autonomous continuation until
the user resumes. All owned processes stopped; no background Lua work remains.
Latest accepted promotion is earlier guarded animation pairing, documented
above:3925 tests+564 subtests,984 root tests,112 Lua/117 bundle hashes.
Installedv124 unchanged; no commit, installation or game launch tonight.

Owned next work/codex_lua_sick_sister_key_20261002 is NOT promoted. Original
SickChildsSister Main EC68A0..EC7FEA SHA
2f949a56539aa4becc803d1104f2fc6fa4f06250d14680aa2978cad90272a0fa.
EC6AEF..EC6B51 proves global first+74c/last+750, stride4, one BFEB16 RNG,
unsigned DIV remainder, hidden-output conversion415D70 atEC6B20, raw animation
helper7E73D0 atEC6B43 and temporary destructor99EAE0 atEC6B4C. Existing0777
sidecar ReadGlobalGameDataDefStringAt already converts these tokens; no DLL
change needed. PDB candidate field SickChildsSisterAttentionAnims is+734;
global offset-name table at+74c labels a float, so that label must NOT replace
retail operand evidence. Native offsets and vector layout control this recovery.

Scratch generic reviewed-definition-vector adapter validates full native/export
hashes, conversion helper hash, exact reviewed selection/conversion/cleanup
window, unique supported source expression, offset and hidden output. It replaces
only the unresolved conversion with the existing binding, preserving first/last
reads, RNG count/order and animation arguments. Candidate converter invokes it
for entity and quest lowering; owned unit_after.json supplies the single review.
Root converter/evidence unchanged and hash-checked at this checkpoint. Scratch
full SickChild generation parses49 functions/13 files,150 TODOs; not a corpus
audit or promotion. Existing dynamic key remains unresolved in accepted root.

Fresh actual entity draft/readable108 lifecycle cases: baseline72 failures,
36 passes15.62s ->candidate108 passes15.46s. Vector counts1/3/7,RNG0/1/32767,
cancel limits1/3/15,task delays0/2, repeated waves, exact selected keys and
resource/timer cleanup. Added10 rejection/no-review guards; final candidate118
passes22.87s. Logs and subprocess exits retained. Actual retail caller/token
conversion wrapper/resource shim96 cases pass: counts1/3/7,RNG0/1/6/32767,
raw bytes0/2/128/255, empty/populated expert; one draw, correct hidden output,
key alive during animation, cleanup and stack balance. String manager, RNG,
expert action and destructor mocked; no retail gameplay equivalence claim.
Initial native harness placed expert at wrong frame offset+20; two failed runs
retained in session history, then inspected actual ECX+8 and corrected to+28.
Native code unchanged. Initial prepare preflight expected one dump-hook site;
there are two(entity/quest); corrected scratch preparation without root mutation.

Commands: python work/codex_lua_sick_sister_key_20261002/run_focused.py before;
same after; python work/codex_lua_sick_sister_key_20261002/native_conversion.py.
All29 comparison was running when bedtime arrived: Arena pair completed with
no delta; other logs/outputs PARTIAL. Owned parent3560 and exact compare.py
children stopped; session72951 exit1 is intentional interruption, not success.
Do not treat comparison_final directories as completed evidence. Root metadata,
adapter and actual tests remain unpromoted. No broader/readable/smoke claim yet.

Resume from bedtime_checkpoint.json: inspect source/evidence hashes, complete
all29 A/B, full readable/smoke, relevant broader regressions, review key/ABI
limits, prepare portable root tests, exact promotion/backups/audit and new named
offline bundle. Preserve prior promotions and unrelated/native-lane edits.
Do not rerun prepare_candidate.py blindly; baseline/owned artifacts already exist.
