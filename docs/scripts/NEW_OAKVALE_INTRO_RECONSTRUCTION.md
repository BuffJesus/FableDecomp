# NewOakValeIntro reconstruction handoff

Status: **trace-tested reconstructed port** (shadow-only; not a verified retail port).

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
ForgeFSE bindings, and 71/71 deterministic fixtures matching their saved traces. In addition
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
plus the later-hit two-speaker exchange without replaying the first-hit cutscene.
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

The coverage manifest classifies 29 functions as implemented and traced, 17 as implemented
with uncertain native arguments, 6 as lifecycle/data-only, no API-blocked functions, and
none as unimplemented. These
classifications combine native-operation coverage with mock-host traces; they do not mean
that every entity AI branch has been exercised in the game.

## Remaining parity limits

Six executable `NOVI.unsupported` call sites preserve retail intent where ForgeFSE
does not expose a direct equivalent. The larger API requirements manifest records 64
blocked semantic requirements or signature differences. No catalogued native function is
currently classified as wholly or partially API-blocked, but important call-level limits remain:
entity scripting acquisition/release semantics, morality values loaded from game data,
special-ability message variants, exact distance-boundary behavior, and some timer/pause
behavior. ForgeFSE's Windows-only runtime now selects retail PC platform branches explicitly;
the missing position-distance helper is reconstructed with squared 3D vector math.

The six explicit sites are two conditional scheduler fallbacks (used only if the registered
entity `AcquireControl`/`ReleaseControl` methods are unavailable), positive and negative deed
morality calls whose game-data float is unrecovered, and one `Pause` plus one `SetTimer` whose
retail duration operands were dropped by the decompiler. Supplying guessed durations or morality
would reduce the metric while making the reconstruction less faithful, so these remain runtime
audit points.

The wife argument loop no longer depends on an unavailable `TextEntryExists` binding. Retail
headers and the installed English `text.big` index prove `_10`, `_20`, `_30`, and `_40` exist
and `_50` does not, so the Lua uses that exact set and wraps to `_10` at the retail boundary.
BookTrader and BarrelThug timer reads now resolve their registered timer IDs from quest state;
previous revisions incorrectly passed field descriptor tables to `GetTimer`.

Only `AttackOver` is transferred by the native persistence routine. Mock traces cover fresh
start, reload after attack, attack transition, early termination, persistence, quest markers,
the barrel timer, barrel destruction, and gold acquisition. Game-runtime verification is still
needed for cutscene/camera behavior, section lifetime swaps, hero morphing, UI rendering and
timer cadence, entity AI blocking behavior, morality amounts, and unload/reload interruption
timing.

Detailed evidence lives in `refs/script_recovery/new_oakvale_intro/`: `coverage.md`,
`validation_report.json`, `test_matrix.json`, `persistence_manifest.json`, `state_manifest.json`,
the per-script `entities` inventories, fixtures, and source-hashed traces.
