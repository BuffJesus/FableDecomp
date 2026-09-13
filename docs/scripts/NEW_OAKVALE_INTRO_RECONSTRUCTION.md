# NewOakValeIntro reconstruction handoff

Status: **trace-tested reconstructed port with experimental live allocator override** (playthrough in progress; not yet a verified retail port).

## Live ForgeFSE playtest checkpoint (2026-09-09)

ForgeFSE commit `6a7d719` adds a disabled-by-default, identity-preserving retail allocator hook.
The first disposable runtime run proved that it matches native `Q_NewOakValeIntro`, preserves the
retail script record, creates `NewOakValeIntro/NewOakValeIntro` as its Lua host, finalizes all 16
entity bindings, and registers `StartBarrelTimer`, `WatchBarrels`, `WatchForGotGold`, and
`ManageQuestCoreMarkers`. Three live entity hosts (`NOVI_Barrel`, `NOVI_TeddyGirl`, and
`NOVI_AffairMan`) were observed loading before that run ended, with no New Oakvale-local Lua or C++
exception in the log. This is runtime integration evidence, not an end-to-end playthrough.

`refs/script_recovery/new_oakvale_intro/forgefse_runtime_playtest.json` is generated from the live
log by `tools/script_recovery/verify_new_oakvale_runtime.py`. The isolated playtest registry at
`refs/script_recovery/new_oakvale_intro/runtime_playtest/quests.lua` deliberately contains no custom
quests: replacement is driven by `FSE/retail_override.lua`, so registering a second quest would be a
dangerous identity collision.

This status is intentionally provisional. A Lua file is not considered retail-correct while
any operand, call target, argument order, branch, or lifecycle behavior remains marked unknown,
inferred, or API-blocked. The evidence inventories are being re-audited to a zero-unknown
acceptance criterion across all 22 files.

## 2026-09-09 single-authority correction

The duplicate guard lecture is not supported by the Lua guard control flow. In the live log
(`FSE/FableScriptExtender.log`, SHA-256
`824244705312DE71F1CEACB01CBCEE68FF46BD6B647042A9B7635D7D68A95D60`) the guard path records exactly
one `NOVI_PROBE Guard claimed lecture`, one movie-sequence start, and one issue of each speech key
from `TEXT_QST_048_GUARD_CAUGHT_YOU_10` through `_60`. The same run created exactly three Lua guard
hosts, matching the three physical `NOVI_Guard` things in the level; the shared
`GuardsDealtWithBadDeeds` claim prevented another Lua host from entering the lecture.

The startup record also proves that the deployed configuration had the retail allocator override
disabled and registered the Lua quest separately as `NewOakValeIntro`. Retail gameflow retains its
native `Q_NewOakValeIntro` authority in that configuration. Combined with the user's observation of
two complete lectures, the evidence-backed root-cause inference is concurrent native and additive
Lua quest owners targeting the same `NOVI_*` script names. A single-authority runtime retest is still
required to confirm the inference; the guard's retail counters must not be changed to conceal it.

The corrected disposable profile is
`refs/script_recovery/new_oakvale_intro/runtime_playtest`: its `quests.lua` is empty, and its
disabled-by-default `retail_override.lua` attaches the Lua allocator and all sixteen entity
allocators to the existing `Q_NewOakValeIntro` `CScriptInfo`. This preserves the quest name consumed
by retail Gameflow and cannot register a competing `NewOakValeIntro` identity. `Main` emits the
read-only `NOVI_AUTHORITY` diagnostic; a correct runtime must report active name
`Q_NewOakValeIntro`, native `true`, and legacy `false`.

The static authority gate is:

```powershell
python tools/script_recovery/validate_new_oakvale_authority.py `
  --profile-fse refs/script_recovery/new_oakvale_intro/runtime_playtest `
  --source-fse refs/script_recovery/reconstructed/NewOakValeIntro/FSE `
  --output refs/script_recovery/new_oakvale_intro/runtime_evidence/authority-validation-20260909.json
```

It checks the disabled repository default, exactly one native override, absence of either quest
identity from the custom registry, exact retail entity-binding order, duplicate bindings, and every
allocator source. Its three unit tests cover the valid profile, an accidental legacy custom quest,
and an accidentally enabled source-control default.

The package under `refs/script_recovery/reconstructed/NewOakValeIntro/FSE` independently
reconstructs the retail `Q_NewOakValeIntro` quest from native decompilation, PDB-derived
names, registry facts, operation IR, and the repository's historical notes. It is deliberately
kept outside the live ForgeFSE package tree so it cannot silently replace the retail quest.

## Retail identity and lifecycle

- Registered script: `Q_NewOakValeIntro`
- Native class: `NScript::CQ_NewOakValeIntroScript`
- Retail section: `S_QNOVI`
- Constructor: `0x00DAAC00`; vtable: `0x012D7A28`
- `RegisterMain`: `0x00DAACE0`; `Init`: `0x00DAADD0`; `OnPersist`: `0x00DAADA0`
- `Main`: `0x00DABAC0`; `DoMission`: `0x00DBDE40`
- `AttackStuff`: `0x00DBE3C0`; `PostAttackStuff`: `0x00DBEB20`
- Destructor: `0x00DBEFA0`

`Q_NewOakValeIntro_PreAttack` is the childhood section activated before the raid.
`Q__OakValeIntro_PostAttack` is a separate post-raid section. The Lua preserves that
boundary and does not treat the latter as the complete prologue.

The native `Main` binding order is: `NOVI_LiveFather`, `NOVI_Theresa`, `NOVI_Guard`,
`NOVI_Villager`, `NOVI_Bully`, `NOVI_Victim`, `NOVI_TeddyGirl`, `NOVI_AffairMan`,
`NOVI_AffairWoman`, `NOVI_AffairWife`, `NOVI_BookTrader`, `NOVI_BarrelMan`,
`NOVI_BarrelThug`, `NOVI_Barrel`, `NOVI_CreatedBeetle`, and `OVI_DeadFather`.

The Bully hit HUD is now resolved at instruction level. `CNOVI_Bully::Main` instructions
`0x00DBC3EB` and `0x00DBC3F0-0x00DBC446` call `AddQuestInfoBar` with current value 4,
maximum 0, opaque green filled colour `{0,255,0,255}`, opaque blue empty colour
`{0,0,255,255}`, icon `HUD_QUEST_ICON_GRANDSON`, empty text, and final float `1.0`.
The x86 reverse push order establishes the two colour roles; the PDB-derived
`CRGBColour_u_0_s_0` layout and direct native channel writes establish the BGRA byte layout.

## Validation snapshot

Run from the repository root:

```powershell
python tools/script_recovery/validate_reconstructed_package.py `
  --fse-root refs/script_recovery/reconstructed/NewOakValeIntro/FSE `
  --package Q_NewOakValeIntro `
  --fixtures refs/script_recovery/new_oakvale_intro/fixtures `
  --traces refs/script_recovery/new_oakvale_intro/traces `
  --output refs/script_recovery/new_oakvale_intro/validation_report.json
```

The latest 2026-09-10 checkpoint has 23 Lua files, 17 operation inventories (quest plus 16
entities), 52 catalogued native functions, no Lua syntax errors, no missing or wrong-scope
ForgeFSE bindings, and 119/119 deterministic fixtures matching their saved traces. Fixture metadata
can require or forbid calls by argument value/count and enforce ordered call subsequences, so key
outcomes such as `OBJECT_GOLD_1`, broken-stock dialogue, deed ownership, Wife route retries, and
Bully visibility transitions fail validation if the API name remains present but its semantics
change. Operation/operand uncertainty is tracked separately from runtime observations: the Affair
Wife inventory has no unresolved native reconstruction facts, while retaining the intermittent
ended-short route report and its identity/position probes under `runtimeObservations`. In addition
to the quest lifecycle suite, coverage includes entity initialization, entry interruption,
beetle timer expiry and cleanup, barrel tutorial/destruction reporting, dead-father marker and
control cleanup, guard idle/talk behavior, father/Theresa resource release, both teddy handoff
helpers, deterministic villager reputation-line selection, and the bully/trader walk-home
distance fallback, plus hero-special hit reactions for the teddy girl, book trader, and barrel
thug. The affair suite traces the husband's hush-money offer and first-bad-deed bookkeeping,
the lover's run to the escape marker and off-screen removal, and the wife's accepted-affair
run to her husband through the first retail argument entry,
`TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10`. The victim suite reaches the first direct hero-hit
path through bad-deed bookkeeping and the `CS_OAKVALEINTRO_BRATHIT` cutscene with its retail
`HERO` and `BRAT` actor roles.
It also covers the freed victim's movement/scared-state restoration and thanks/tutorial path,
plus the later-hit two-speaker exchange without replaying the first-hit cutscene. Victim-local
booleans now carry their proven retail defaults for isolated `Main`, and initial control-acquisition
termination exits through modeled resource release before the bully lookup. Nested victim resource
unwind ordering remains runtime/disassembly work and is not generalized from the initial gate.
Guard coverage now includes the complete first-offence violence lecture and the repeat-offence
path through the distant chase announcement, follow task, lecture-range wait, player-control
wait, shortened introduction, and multi-crime list.
Live-father coverage now runs the full `CS_OAKVALE_INTRO_FATHER` actor setup through the PC
highlighting card and good-deed HUD counter, and separately verifies a two-good-deed gold payout,
the exact `GetHeroGold() > 3` decision, objective-one fallback, movie cleanup, and final control
release.
Theresa coverage now connects the nearby-hero meet, accepted chocolate handover, live-guard
removal, objective update, outro-marker transition, all three HUD removals, outro cutscene, raid
movie, fade/music change, `AttackOver`, and final control release in one deterministic lifecycle.
The supporting trace-tool unit suite has 23/23 passing tests. Fixture results now recursively
materialize entity handles inside returned vectors, so collection APIs such as
`GetAllThingsWithScriptName` exercise callable entity proxies rather than inert tables.
Book-trader coverage now includes the complete first sweets purchase at exactly three gold,
the retail object/charge operands, objective and `GivenSweets` updates, plus the expired-timer,
winning-random-roll, nearby-hero ambient shout. Native instructions prove that the shout reads
`TalkIntermittentTimer` at parent offset `0x104` and resets it to `3`; its traces assert the exact
`SetTimer` event before the animation and conversation line. All four scripted-control acquisition sites now propagate host termination:
loop-top failure exits before movement, sale failure before its movie/question, hit-action failure
after the retail deed writes but before its movie, and shout-action failure after conversation
creation but before animation or line queuing. Dedicated traces verify each cleanup boundary.
Bully coverage now includes the first numbered intimidation/`VictimShake` handshake and a complete
four-hit subdual: health-bar creation and decrements, both non-teddy `BULLYRUN` cutscenes with the
retail actor and `$BRATLINE` mappings, good-deed bookkeeping, `BullyRanOff`, and entity removal.
Initial, loop-top, item-refusal, and talk acquisitions now stop at their native termination exits;
focused traces cover the initial, loop, and talk boundaries, while walk-home interruption also
models function-exit release. The later me/hero/victim run-off unwind is now instruction-proven:
a terminating Hero retry reaches `0x00DBCC33` and destroys Hero then Bully resources; a terminating
Victim retry reaches `0x00DBCC2A` and destroys Victim, Hero, then Bully resources. Lua mirrors both.
Dialogue coverage now carries one Bully instance across two talks: the first emits `GET_LOST` and
sets `DoneIntro`; the next misses teddy possession, observes `HeroAttackedVictim`, and emits the
one-shot `IN_COMMON` response. Its loop-tail termination now also models the native resource
destructor with explicit control release.
Barrel-thug coverage now follows the leave-in-charge wait through marker teleport, the recovered
retail `Pause(3.0)`, explanation and hero follow, then the first timer-nine
temptation tier. A second path verifies the returned-barrel-man/broken-barrel `OUTRO` response and
that its closed nag gate creates no dangling conversation. Intro, chat, and hit-action control
acquisition now propagate termination at their native boundaries; traces verify that intro stops
before polling/teleport, while chat and hit failures close their already-open movie brackets before
release, with hit deed writes retained because retail performs them before action acquisition.
Barrel-man coverage now exercises the warehouse handoff itself: the proximity trigger, one
timer-bounded approach step, favour speech, fade, hero/worker teleports, 45-second watch timer,
and `BarrelManLeftHeroInCharge` transition. A complete continuation reaches timer value 15,
chooses the off-screen return marker, walks back, sees and thanks the hero, awards the first good
deed, restores the normal villager brain, and releases control. Its entity-local fields also carry
their proven retail defaults when the mock host invokes `Main` in isolation. The complementary
absent-hero continuation selects the alternate marker when the primary return point is on-screen,
creates the `BARRELMAN_WHERE_GONE` conversation, shows the unattended-warehouse instruction, and
records first-bad-deed state without taking the visible/nearby hero branch.

There is intentionally no yes/no prompt in Barrel Man's warehouse handoff. Retail
`CNOVI_BarrelMan::Main` (`0x00DB5330`) speaks `TEXT_QST_048_BARRELMAN_FAVOUR` at call site
`0x00DB6385` and then proceeds directly to the fade/teleport sequence through game-interface slot
`0x5d4`. Its complete 6,128-byte instruction stream contains neither a call through
`GiveHeroYesNoQuestion`'s interface slot `0x1c8` nor any question/answer text keys. The separate
retail question idiom used by Teddy Girl, the Bully, Book Trader, Theresa, and the affair pair is
`GiveHeroYesNoQuestion(...)`, followed by per-frame polling of `MsgIsQuestionAnsweredYesOrNo()`
until the value is non-negative; button one is `1`, button two is `0`, and button three is `2`.
Affair-husband coverage now distinguishes the first near and ranged exchanges: near range emits
the reversed woman/man `_10` pair and sets `HeroDiscoveredInfidelity`, while ranged distance emits
the screen-message `_10` pair without discovery. Deterministic random inputs separately verify the
`ReceiveKiss`/`GIVE_KISS` and `ReceiveHug`/`GIVE_HUG` handshakes.
Affair-woman coverage consumes and clears the matching `ReceiveKiss` flag, traces a direct-hit
complaint and first-bad-deed path with the `TalkingToWoman` bracket restored, and verifies the
`BUSY` talk interaction through action clearing, hero-facing speech, inferred husband-facing
cleanup, movie teardown, and both nested/main control releases.
Teddy-girl coverage now includes the separate ruined-teddy outcome: the bully-proximity gate,
`TEDDY_RUINED` conversation, master-data `TeddySolution = "C"`, run toward the affair wife,
off-screen check, despawn, and final control cleanup. Offer, presented-item, ruined-walkoff, chat,
and scold acquisitions now propagate host termination before dependent movie or movement work.
Failure traces cover every distinct ordering boundary: the ruined conversation/master write and
scold ally/deed writes remain because retail performs them before their respective acquisitions.
Villager coverage now joins a direct female-villager hit to the next talk interaction, proving the
first violence bad deed, attacked movie line, entity-local `HeroDidHitMe` memory, and female
done-bad-deeds reproach. A separate female/both-deeds mutter collides with the previous speech
index, yields the native retry frame, stores the replacement index, and queues the `_40` line.
Affair-wife idle coverage now proves the equivalent native timer sequence independently: the
nearby-hero “where's my husband?” branch reads `TalkIntermittentTimer`, writes value `3`, then
queues its two-person conversation.

The coverage manifest classifies 46 functions as implemented and traced, none with uncertain
native arguments, 6 as lifecycle/data-only, no API-blocked functions, and
none as unimplemented. These
classifications combine native-operation coverage with mock-host traces; they do not mean
that every entity AI branch has been exercised in the game.

## Remaining parity limits

Validation currently reports no executable unresolved-call placeholders. Forge now exposes the entity control, special-ability message,
retail position-distance, timer/pause, platform, and cutscene adapter paths used by this package.
The obsolete generic unknown-value/unsupported-call scaffold has been removed entirely now that no
reconstructed call site or constant depends on it.

The trader, Bully, and
AffairWife timer operands formerly listed here are now instruction-level recovered as
`TalkIntermittentTimer` and value `3`; the BarrelThug intro pause is likewise recovered as `3.0`
and AffairMan's kiss/hug pause as `0.4`. The installed retail `script.bin` resolves
`OVI_MoralityChangePerDeed` exactly: entry 597 (`SCRIPT_DEF`) decodes it as `0.001`, distinct
from entry 0 (`NULLDEF_CScriptDef`) whose default is zero. The port therefore applies `+0.001`
for good deeds and `-0.001` for bad deeds. This is now a reproducible boundary audit rather than a
document-only conclusion: it pins the retail `script.bin` hash, requires a clean entry-597 schema
decode, locks both exact native deed-function hashes and their shared `+0xd64` load/opposite signs,
and verifies Forge slot `0x270` plus the Lua calls. Morality magnitude is therefore no longer listed
as runtime-only. StartBarrelTimer's instruction stream also proves
the initial clock bar is current `0`, max `45`, with opaque green fill and empty colours; both
timer reads load the `WatchTimer` ID directly from parent offset `+0x108`.
The reload-only post-attack deactivation delay is likewise exact: `Main` zeroes `EBP` in its
prologue and passes that unchanged zero to `DeactivateQuest`.

The wife argument loop no longer depends on an unavailable `TextEntryExists` binding. Retail
headers and the installed English `text.big` index prove `_10`, `_20`, `_30`, and `_40` exist
and `_50` does not, so the Lua uses that exact set and wraps to `_10` at the retail boundary.
BookTrader and BarrelThug timer reads now resolve their registered timer IDs from quest state;
previous revisions incorrectly passed field descriptor tables to `GetTimer`.

Only `AttackOver` is transferred by the native persistence routine. Mock traces cover fresh
start, reload after attack, attack transition, early termination, persistence, quest markers,
the barrel timer, barrel destruction, and gold acquisition. Subsequent single-authority playtests
proved the childhood path, guard lecture ownership, Father cutscene skip, timer countdown, barrel
predicate dispatch, and quest completion. Runtime work remains for the Affair Wife husband-position
anomaly and broader unload/reload interruption coverage. `IsObjectInThingsPossession` argument
conversion is runtime-proven: dump
`Fable.exe_260909_175951.dmp` faults in the Forge binding when Teddy Girl supplies the operands in
thing/object order, while the binding prototype and the working Father/Theresa sites require
`(objectDefName, thing)`. Teddy Girl and both Bully item paths now use that proven order.

The same runtime pass exposed two host-integration defects outside the recovered quest logic.
The native quest constructor supplies an empty section string when registering
`StartBarrelTimer`, `WatchBarrels`, `WatchForGotGold`, and `ManageQuestCoreMarkers`; Forge had
registered all four under the literal section `Class`, so none ran in Oakvale. The Lua registration
now preserves the empty native section. Destroyed Forge entity hosts also returned from their frame
wait without receiving native vtable slot 5. `LuaEntityHost` now translates a completed `Main` on a
proven-dead bound thing into a one-shot `OnPredicateFail`, restoring the barrel notification that
drives counting, beetle/gold creation, and Barrel Man's outcome.

September 9 runtime evidence established that `WatchTimer` is registered twice by the retail quest
constructor and that zero is a valid ForgeFSE timer handle. Lua's truthy-zero semantics had prevented
both registrations; initialization now registers them unconditionally. Retail disassembly fixes the
values at 45 seconds initially and 15 seconds at Barrel Man's return transition, so the effective
warehouse-watch interval is 30 seconds. The HUD is created at value 0 with a 45 maximum and then
updated from the live countdown, matching the native calls; instrumentation records each changed value
to distinguish presentation defects from timer defects.

The reconstructed quest now reports completion under the native identifier `Q_NewOakValeIntro`, which
is the exact message awaited by both retail flow and Aeon's downloaded Lua gameflow port. The latter's
full package passes the ForgeFSE binding validator with no missing APIs after adding ABI-backed wrappers
for no-resource quest activation, batch activation using retail's three-pointer `CArray<CCharString>`
layout, final-quest preparation, platform detection, and the native category-2 tutorial log adapter.

Detailed evidence lives in `refs/script_recovery/new_oakvale_intro/`: `coverage.md`,
`validation_report.json`, `test_matrix.json`, `persistence_manifest.json`, `state_manifest.json`,
the per-script `entities` inventories, fixtures, and source-hashed traces.

## 2026-09-09 continuation checkpoint

The zero-unknown audit is satisfied: all operation records now have `argsKnown: true`, including
the quest inventory. Passing fixtures supplement rather than substitute for that native evidence.

`NOVI_Villager` is now at zero unknown operations: disassembly at
`0x00DAE109`-`0x00DAE12F` proves its two ally calls are reciprocal `(me, hero)` and `(hero, me)`.
`NOVI_LiveFather` is also at zero: `0x00DB93CF`-`0x00DB93D8` explicitly pushes its stored
`ME_THING` into `ClearThingHasInformation`.
`OVI_DeadFather` is now at zero too. The signature and push sequence at
`0x00DB8476`-`0x00DB84A5` recover all seven animation flags as
`false, true, false, true, true, false, false`; the retail byte at `0x01375748` is `1`.
The marker-removal call at `0x00DB84E8`-`0x00DB84EE` explicitly pushes the stored `ME_THING`.
`NOVI_BarrelMan` is also at zero. Disassembly proves the facing call's trailing flag is false,
the visibility test is directionally `(hero, barrelMan)` (corrected in Lua), and the favour-path
teleport is exactly `(hero, guardPoint, false)`.
`NOVI_BookTrader` is now at zero as well: its hit ability is directly `14`, ally calls are
reciprocal, control priority is `4`, the full shouting-animation flags are recovered, and the
roll-up conversation line is `(conversation, text, false, trader, hero)`.
`NOVI_BarrelThug` is now at zero too. All three control acquisitions use priority `4`; both nag
timer reads use `PARENT->WatchTimer`; conversation creation and tier-line speaker/listener order
are proven; and the hit path directly establishes ability `14`, reciprocal allies, deed `2`, and
the full movie, pause, acquisition, and speech operands.
`NOVI_AffairWoman` is now at zero as well. Disassembly directly proves hit ability `14`, movie
pause true/false, nested control priority `4`, and both full facing calls. It also corrects two
behavioral inferences: the man (not the woman) faces the hero at the start of the busy exchange,
and the woman becomes pushable again before fleeing. Her removal is exactly `(me, false, true)`.
`NOVI_TeddyGirl` is now at zero too. All previously dropped control priorities are directly `4`;
the non-teddy presentation comparison names `OBJECT_TEDDY_BEAR_UNGIVEABLE`; and the ruined-teddy
conversation operands, removal flags, information target, hit ability `14`, and reciprocal ally
calls are all instruction-proven.
`NOVI_AffairMan` is now at zero as well. Retail stack order proves priority `4`, hit ability `14`,
all near/ranged conversation participants and speakers, the active-conversation handle, and every
facing pair. The first encounter required one correction: the woman, not the man, faces the hero.
`NOVI_AffairWife` is now at zero too. The main control acquisition is priority `3` (not `4`), while
the nested movie acquisitions use `4`; hit ability `14`, reciprocal allies, pause flags, the active
conversation handle, and all argument-conversation operands are instruction-proven. Retail also
required three Lua corrections: both post-speech facing calls target her husband, and the wife/man
conversation lines use each other as listeners rather than null listeners.
The later runtime clarification is intermittent rather than a consistently wrong destination: the
wife has successfully reached her husband in other runs. Accordingly native movement remains the
primary path. Only when its task has ended while she is still outside the native 3 m arrival test does
the compatibility layer reissue the same move toward the husband's live position, with an immediate
check and a four-retry cap; it never teleports her. Normal and prematurely ended movement fixtures
prove the one-command and retry paths separately.
`NOVI_Guard` is now at zero as well. Its Init operands, reciprocal ally pairs, facing flags, both
priority-`4` acquisitions, pause flags, ability `14`, and all talk-line participants are directly
recovered. The exit cutscene-behaviour enum required a Lua correction from inferred `0` to retail `2`.
`NOVI_Victim` is now at zero too. Every scripted-control acquisition uses priority `4`; all scared,
pushable, movement, facing, hit-ability, ally, movie, and speech operands are instruction-proven.
Retail conversation stack order required correcting the bully participant and both speaker/listener
pairs in the evil-bros exchange; its misleading second text-key name does not denote the speaker.
`NOVI_Bully` completes the zero-unknown queue. All scripted-control acquisitions use priority `4`,
the move command uses radius `0.0` (while `2.0` is only its arrival test), and ability `14`, reciprocal
allies, cutscene operands, speech flags, animation flags, and information targets are recovered.
Retail stack locals required correcting every recurring participant/listener and facing target to the
victim; the run-off information-clear also targets the victim rather than the bully.
### Shipped cutscene payload checks (2026-09-09)

The installed retail `script.bin` was decoded with FableForge rather than inferring visible
behaviour from the Lua control flow. `CS_OAKVALEINTRO_BULLYRUN1` contains 35 commands. Its relevant
tail is `BULLY.RunTo MK_OIBR_BULLY2,0,FALSE,FALSE,FALSE,TRUE`, a 0.5-second `GamePause`, the victim
speech, a Hero teleport, and finally `BULLY.Drawable FALSE`. Therefore the retail bully is meant to
run briefly and then become non-drawable; immediate disappearance means the native `RunTo` command
is not acting on the expected scripted-resource actor, not that Lua removes the bully too early.
`CS_OAKVALEINTRO_BULLYRUN2` is the five-command teddy award scene and directly executes `GiveHero
OBJECT_TEDDY_BEAR_UNGIVEABLE`.

`CS_OAKVALE_INTRO_FATHER` contains the retail child placement and animation itself: it teleports Hero
to `MK_OVI_ID_HERO`, teleports Father to `MK_OVI_ID_DAD`, then runs `CS_WAKING_UP_LOOP` and
`CS_WAKING_UP_ON_STEPS` before the dialogue cameras. Its dialogue commands are also native cutscene
verbs, not Forge-generated conversation lines. This rules out guessed Lua coordinates, a manual
sleep animation, or synthetic subtitle suppression as faithful fixes; remaining placement, sound,
subtitle, and skip defects must be traced through cutscene actor-resource and global cutscene/input
state.

Aeon's downloaded Discord ports provide a useful Forge-specific comparison:
`GuardianSisterInfo2/Entities/MazeAtTavern.lua` and `Fisherman/Entities/Fisherman.lua` explicitly
release their entity control handle before starting a cutscene that reacquires that entity. That is
not the lifetime emitted by the retail New Oakvale functions, which keep outer and nested scripted
resources simultaneously, but it confirms that control ownership is a known seam in Forge's
cutscene wrapper. The current diagnostic DLL logs each actor and assigned resource identity plus
retail GUI bytes `+0xF4`, `+0xF6`, `+0xF8`, `+0xF9`, and `+0xFA` immediately before and after
`RunCutsceneMacro_Func`. This is instrumentation only. Deployed DLL SHA-256:
`F34AC330F28E6F7C3E1FBBB83823DA7ACF59CE503225B251DB8E2E6DF38FA979`. The
per-frame "already has a control handle" message was also removed from this build; it represented
no state transition and had produced hundreds of synchronous log writes during each macro.

### Raid section-swap boundary (2026-09-10)

The attack transition is now locked across all three layers. The exact retail `AttackStuff`
function is 285 bytes at `0x00DBE3C0-0x00DBE4DD` with SHA-256
`E842CCECCF47BFB0BC2C829A199B0889340D6FD750ED429578FD489B732E80DC`. It activates
`Q__OakValeIntro_PostAttack` through GSI slot `0x450`, then immediately deactivates
`Q_NewOakValeIntro_PreAttack` with delay zero through slot `0x460`, before changing time, theme,
and objective. Forge resolves those exact slots with matching `CCharString const*` and delay
signatures, and Lua preserves both spellings and the complete order. The fixture now asserts every
argument and the ordered call sequence. Only the engine-side timing of things spawned or destroyed
by those proven section calls remains runtime-only; the reconstruction/API boundary does not.

### Childhood Hero morph boundary (2026-09-10)

The childhood morph is also closed across retail, Forge, and Lua. Retail `DoMission` is an exact
1,172-byte region at `0x00DBDE40` with SHA-256
`E3785B3E55A0B1F5556C67316632FAEEC99B891B1BCE0743BD1B762153A91542`; it gets the Hero through
slot `0x118`, calls `TurnCreatureInto` through slot `0x178` with `CREATURE_HERO_CHILD`, activates the
pre-attack section, yields one frame, and gets the Hero again before slot `0x814` changes killability.
Forge resolves slot 94 (`0x178`) with the retail caller-owned result-buffer signature, zeroes that
buffer, and wraps the returned `CScriptThing` with the established ownership helper. Lua deliberately
discards the temporary morph result and reacquires the live Hero after the same frame boundary before
disabling killability and sleeping. The fresh-start fixture asserts these operands and ordering. Only
the engine's actual rendered morph outcome remains runtime-only; handle lifetime and call semantics do
not.

### Dead-father cutscene execution boundary (2026-09-10)

The final childhood cutscene is no longer a blanket runtime unknown. The complete retail
`PostAttackStuff` audit fixes the movie/pause/camera sequence and exact
`CS_OAKVALEINTRO_HESDEADJIM` macro. Lua preserves the ordered camera-fix true/macro/false bracket,
and the single-authority completed-childhood log proves the blocking macro entered, set cutscene mode
with the skippable flag, exited, and restored `actionMode=0`, `guiActionMode=0`, and `inCutscene=0`.
A composite regression audit now joins those facts. Only visual camera-composition parity remains a
runtime QA item; execution, bracketing, and state restoration are proven.

### Ambient entity and warehouse HUD audit (2026-09-09)

| Area | Retail evidence | Lua/Forge state | Discrepancy or correction |
|---|---|---|---|
| Barrel Man offer | `CNOVI_BarrelMan::Main` `0x00DB5330`: `Speak` at `0x00DB6385`, then GSI `+0x5d4` fade | Same mandatory speech/fade/teleport path | No yes/no prompt exists in retail; adding one would be incorrect |
| Barrel Man return visibility | Assembly `0x00DB5A88-0x00DB5AB5`: GSI `+0x9e0(hero, man)`, then direct 10 m distance fallback | Argument order matches, but this call mixes the shared handle returned by `GetHero` with raw entity-host `me` userdata | Runtime dies exactly after targetability and before the visibility result; changed this binding to normalize both Lua handle shapes through the same converter already proven by the preceding facing call |
| Warehouse bar creation | `StartBarrelTimer` `0x00DB4FF4-0x00DB503B`: x86 right-to-left pushes are scale, text, texture, colour2, colour1, `0`, then `45`; `CDrawQuestInfo::AddBar` `0x0064F7FC` consumes the first float at `[ebp+8]` and second at `[ebp+0xc]` | Corrected to `AddQuestInfoBar(45, 0, green, green, HUD_CLOCK_ICON, "", 1)` | The former `(0,45)` reconstruction reversed the floats and exactly explains the permanently empty fill. The Forge typedef and vtable slot are correct. |
| Warehouse bar updates | Retail GSI `+0x530(bar, GetTimer(WatchTimer), -1, -1)` every frame | Exact call; live probes record 45 down through 0 | Timer duration and update path remain unchanged; the creation-order correction gives the bar the same initial/current value and zero sentinel used by retail. |
| Bar colour | Retail packed colours `0xff00ff00` in range and `0xffff0000` out of range; `CRGBColour` is RGBA | Byte parser reversed red and blue despite the declared RGBA structure | Corrected parser to `{r,g,b,a}`; green is invariant, so this fixes the out-of-range hue but does not alone explain an empty fill |
| Generic villagers | Native `NOVI_Villager` `0x00DADF00/0x00DADF80`: retail AI remains active; script handles hit, talk, and intermittent reputation mutters | Same events, 1/100 roll, shared 3-second timer, 5 m gate, sex/reputation tables | No schedule override is introduced by Lua; each entity host has an isolated Lua state, so `HeroDidHitMe` is per villager as in native |
| Named villagers and movement | Affair trio, trader, victim, thug, guard, Father, Theresa, and Teddy Girl use recovered native home/marker moves and retain TNG creature/navigation data | Lua replaces script callbacks only; original world things, AI definitions, home positions, and navigation remain | Runtime seams remain scripted-control lifetime, cutscene actor ownership, and message delivery; do not synthesize schedules in Lua |
| State restoration | Quest `OnPersist` transfers recovered parent quest fields; entity `Init` resets native constructor-local fields | Forge creates an isolated VM per entity host and invokes Lua `OnPersist` when one exists | No blanket entity persistence added: recovered retail entities expose no custom `OnPersist`, and inventing persistence for constructor-local fields would change reload behavior |

The return judgment was re-audited after a retail comparison in which smashing the warehouse stock
led to a scolding while leaving it untouched led to thanks. The native branch at
`0x00DB5A88-0x00DB5ABC` contains no read of `BarrelBrokenPersistent` or any deed counter. It is an
exact short-circuit `CanThingBe_Seen_ByOtherThing(hero, barrelMan) ||
IsDistanceBetweenThingsUnder(barrelMan, hero, 10.0)`: true jumps to the thanks/good-deed block at
`0x00DB5C28`; false enters the where-gone/unattended/bad-deed-1 block at `0x00DB5AC2`. The installed
retail `StartOakValeEast.tng` supplies the spatial explanation: `M_WHouse_ManStart` is
`(1.986572,44.650269,7.000001)`, `M_WHouse_GuardPoint` is
`(4.910889,43.934875,7.000001)`, while the scripted barrels are farther into the warehouse (for
example `(12.227051,55.111145,7.0)` and `(6.580322,32.410828,7.0)`). Thus a normal stock-smashing
run moves the Hero out of the return detection envelope; destruction is not itself the condition.
The byte-exact reconstruction preserves the native short circuit and does not call the distance
helper after a successful visibility result. The playable compatibility package deliberately adds a
narrow `BarrelBrokenPersistent` override before the thanks block. That is an explicit evidence-led
departure from this TLC executable branch, not a revised claim about its bytes: Anniversary footage
and the requested morality outcome both require the stock-specific rebuke after destruction.

Forge's distance binding formerly accepted two raw `CScriptThing*` parameters even though entity Lua
mixes raw `me` with the `shared_ptr<CScriptThing>` userdata returned by `GetHero`. It now takes
`sol::object` and normalizes both operand shapes through the same proven converter used by the
visibility/facing bindings before calling retail fastcall `0x00CBE2FF`; the companion over-distance
binding was corrected at the same seam. This was an ABI-hardening defect, but the next instrumented
run falsified it as the cause of the reported thanks: the logged man and Hero positions were
`(3585.987,812.650,7.000)` and `(3585.949,814.624,7.020)`, and both native and independently computed
distance were 1.975 m. The true result of the 10 m test was therefore expected. The man coordinate
also exactly equals local `M_WHouse_ManStart` `(1.986572,44.650269,7.000001)` plus the loaded map
sector origin `(3584,768)`, disproving a wrong-marker or coordinate-conversion hypothesis.

The retail helper itself was verified from `0x00CBE2FF-0x00CBE36E`: it validates both things, reads
both positions via vtable `+0x18`, computes squared XYZ distance, compares against the squared float
argument, and returns with `ret 4`. All six scripted barrels in installed `StartOakValeEast.tng` are
13.07-18.01 m from `M_WHouse_ManStart`. Consequently retail scolds a Hero who remains among the stock
because that Hero is undetected, then selects the stock-specific `LETDOWN_BROKEN` group on later talk.
A Hero who has returned to within 10 m receives `BARRELMAN_THANKS` even if barrels were broken; that
odd edge case is what the TLC executable implements. The current compatibility Lua instead speaks
the exact localized TLC `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN` group and suppresses the thanks and
good-deed write whenever persistent breakage is set. Deterministic fixtures cover both the
unattended-then-talk sequence seen in the reference video and the previously observed nearby return.

The warehouse colour discrepancy was below the script layer. The PDB layout defines
`CRGBColour` as BGRA, and the native timer thread writes its red state as bytes `ff 00 00 ff` at
`0x00DB50F8-0x00DB511F`. ForgeFSE had declared the four bytes RGBA, turning requested red into blue.
The canonical Forge binding now preserves Lua's named R/G/B channels while emitting BGRA bytes.
A composite closure audit joins the exact StartBarrelTimer operands, native bar field/render chain,
Forge bridge, archived complete 45-through-0 runtime trace, and the interactive confirmation that the
corrected out-of-range colour is red rather than blue. The New Oakvale timer's colour rendering and
cadence are therefore no longer classified as runtime-only.

The deed sequence is likewise split across owners in retail: `WatchBarrels` records bad deed 0 when
it consumes the first `BarrelBrokenInstantaneous`; an undetected Hero makes Barrel Man record bad
deed 1 during his unattended-warehouse judgment; `NOVI_Guard` later compares the cumulative
`BadDeedsPerformed` value with `GuardsDealtWithBadDeeds` and claims one lecture for that accumulated
delta. Barrel Man's complaint and the guard's one chase are therefore distinct intended events, but
two guard lectures are not.

Retail has three `NOVI_Guard` instances, but each rechecks the shared counters and the winner assigns
`GuardsDealtWithBadDeeds = BadDeedsPerformed` before speaking, so they cannot independently claim the
same counter value. The instrumented run logged exactly one claim (`bad=1`, `dealt=1`) and one call
for every lecture text key; it does not contain two scripted lectures. What can sound like two
scoldings is the native pre-chase conversation, “Look, there's the little swine. I'm going after
him!”, followed by the formal multi-line lecture beginning “Right! Got you, you little ruffian!”;
the pre-chase line is emitted only when the guard starts more than 3 m away. The unspoken “You
committed a bad deed” HUD message is a third distinct feedback event. Suppressing any of these
without identifying the repeated wording would remove retail-authored behavior rather than fix a
duplicate.

Created Beetle's lifetime loop is also instruction-exact. `RegisterTimer` returns the timer ID into
`EAX`; retail stores it in `EDI` at `0x00DB8124` and pushes that same register before both
`GetTimer` calls (`0x00DB8137` and `0x00DB8159`). The latter push is issued before the termination
branch and is consumed by the second poll on the fall-through path. Lua's single `timer` local thus
matches the retail operand flow directly, rather than filling a decompiler omission from signature
inference.

Barrel Man's numeric movement operand 1 is the named `ENTITY_MOVE_RUN` enum, and special ability
`0xE`, which retail excludes from its generic special-ability hit condition, is
`HERO_ABILITY_HEAL_LIFE_SPELL`. These names come from Forge's retail-aligned entity API definitions;
the numeric behavior was already exact.

The remaining AI runtime qualification is now deliberately narrow. Archived Forge logs prove that
`Speak_Blocking` issues its native command and waits a positive number of frames for completion;
the completed-childhood capture also reaches `TEXT_QST_048_AFFAIRMAN_GOOD_LAD`, proving at least one
successful Wife-to-husband outcome. The user's playtests also establish that the same route can end
short on other runs. Accordingly this is classified as intermittent endpoint/pathfinding variance,
not an always-failing Wife script. Visual completion of the Wife retry and Bully run-off fallback
still needs an interactive pass; generic speech/conversation blocking is no longer listed as wholly
unproven.

Normal quest-completion teardown is also runtime-closed. The archived completed playthrough shows
twelve entity classes unwinding (including Affair Wife and Dead Father), the dead-father predicate
callback, quest persistence, `SetQuestAsCompleted` return, the deleting `LuaQuestHost` destructor,
and retained `CScriptThing` reference release in that order. Forge statically guards quest Main,
spawned thread runners, and entity Main against active-thread termination; both deleting and
non-deleting destructor paths release coroutine registry references, and entity unregistration is
idempotent. The remaining lifecycle-only observation is repeated mid-childhood region unload/reload
re-entry timing, not ordinary completion teardown.

The raid section swap's scripted entity timing is runtime-closed as well. Three independent captures
show post-attack `OVI_DeadFather` allocation and Init, followed by the pre-attack `NOVI_LiveFather`
Main unwind and Dead Father Main entry. Combined with the byte-locked activate/deactivate call order,
this proves the father replacement crosses the engine section boundary. Only visual and non-scripted
world-population parity after the swap remains an interactive observation.

Dead Father's cutscene composition is now data-proven rather than opaque. The retained retail
`script.bin` schema-decodes `CCutsceneDef` entry 485 with zero leftover bytes: 89 ordered macro
commands, thirteen camera uses over eleven named camera records, and six exact skip-cleanup commands.
Every named camera resolves in the hash-locked retail `StartOakValeWest.tng` to an authored scripted
or scripted-spline camera point. Runtime evidence already proves the engine executes that macro and
restores state, leaving only rendered camera/animation presentation parity for a live visual check.
