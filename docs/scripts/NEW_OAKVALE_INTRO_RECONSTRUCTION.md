# NewOakValeIntro reconstruction handoff

Status: **trace-tested reconstructed port** (shadow-only; not a verified retail port).

This status is intentionally provisional. A Lua file is not considered retail-correct while
any operand, call target, argument order, branch, or lifecycle behavior remains marked unknown,
inferred, or API-blocked. The evidence inventories are being re-audited to a zero-unknown
acceptance criterion across all 22 files.

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
The x86 reverse push order establishes the two colour roles; the engine's independently
recovered `CPlayerManager::GetMultiplayerColour` establishes the `CRGBColour` RGBA byte layout.

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

The 2026-09-08 checkpoint has 22 Lua files, 17 operation inventories (quest plus 16
entities), 52 catalogued native functions, no Lua syntax errors, no missing or wrong-scope
ForgeFSE bindings, and 105/105 deterministic fixtures matching their saved traces. In addition
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
models function-exit release. Partial unwind ordering for the later me/hero/victim run-off
acquisitions remains unresolved and is not inferred from the single-entity gates.
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

The coverage manifest classifies 30 functions as implemented and traced, 16 as implemented
with uncertain native arguments, 6 as lifecycle/data-only, no API-blocked functions, and
none as unimplemented. These
classifications combine native-operation coverage with mock-host traces; they do not mean
that every entity AI branch has been exercised in the game.

## Remaining parity limits

Two executable `NOVI.unsupported` call sites preserve retail intent where ForgeFSE
does not expose a direct equivalent. The larger API requirements manifest records 60
blocked semantic requirements or signature differences. No catalogued native function is
currently classified as wholly or partially API-blocked, but important call-level limits remain:
entity scripting acquisition/release semantics,
special-ability message variants, exact distance-boundary behavior, and some timer/pause
behavior. ForgeFSE's Windows-only runtime now selects retail PC platform branches explicitly;
the missing position-distance helper is reconstructed with squared 3D vector math.

The two explicit sites are conditional scheduler fallbacks (used only if the registered
entity `AcquireControl`/`ReleaseControl` methods are unavailable). The trader, Bully, and
AffairWife timer operands formerly listed here are now instruction-level recovered as
`TalkIntermittentTimer` and value `3`; the BarrelThug intro pause is likewise recovered as `3.0`
and AffairMan's kiss/hug pause as `0.4`. The installed retail `script.bin` resolves
`OVI_MoralityChangePerDeed` exactly: entry 597 (`SCRIPT_DEF`) decodes it as `0.001`, distinct
from entry 0 (`NULLDEF_CScriptDef`) whose default is zero. The port therefore applies `+0.001`
for good deeds and `-0.001` for bad deeds.

The wife argument loop no longer depends on an unavailable `TextEntryExists` binding. Retail
headers and the installed English `text.big` index prove `_10`, `_20`, `_30`, and `_40` exist
and `_50` does not, so the Lua uses that exact set and wraps to `_10` at the retail boundary.
BookTrader and BarrelThug timer reads now resolve their registered timer IDs from quest state;
previous revisions incorrectly passed field descriptor tables to `GetTimer`.

Only `AttackOver` is transferred by the native persistence routine. Mock traces cover fresh
start, reload after attack, attack transition, early termination, persistence, quest markers,
the barrel timer, barrel destruction, and gold acquisition. Game-runtime verification is still
needed for cutscene/camera behavior, section lifetime swaps, hero morphing, UI rendering and
timer cadence, entity AI blocking behavior, morality presentation, and unload/reload interruption
timing. Runtime verification should also confirm `IsObjectInThingsPossession` argument conversion:
native operation order is preserved at each call site, but no local ForgeFSE signature source is
available and recovered scripts contain both thing/object and object/thing call shapes.

Detailed evidence lives in `refs/script_recovery/new_oakvale_intro/`: `coverage.md`,
`validation_report.json`, `test_matrix.json`, `persistence_manifest.json`, `state_manifest.json`,
the per-script `entities` inventories, fixtures, and source-hashed traces.
