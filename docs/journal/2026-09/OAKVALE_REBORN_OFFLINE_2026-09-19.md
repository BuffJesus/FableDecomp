# Oakvale Reborn offline pass — the quest scaffold, the text/VO stage, the linter, spike S6 (2026-09-19)

No game validation was possible (the user is testing other scripts), so this pass built everything
that can be proven offline. Nothing was installed; the two spike bundles (S1+S3, S6) and the v1 bundle
are preflighted and waiting.

## The authored FSE tree exists and smokes clean

`tools/oakvale_reborn/scaffold_from_stage.py` seeds `refs/script_recovery/authored/OakvaleReborn/FSE/`
from the v4-proven readable stage. Only Lua *file paths* change (`NewOakValeIntro/Entities/NOVI_X.lua` →
`OakvaleReborn/Entities/OVR_X.lua`, `require("OakvaleReborn.native_quest_helpers")`); every `NOVI_*`
string that names a retail TNG thing, section or cutscene actor stays — the stage is retail. The
override keeps `nativeName = "Q_NewOakValeIntro"` and the v4 profile shape (no `nativeLifetime`; that
is what played clean).

New modules:
- `scenes.lua` — `Scene.Acquire/AcquireHero` (retry-per-frame TryAcquire), `Scene.RunMacro` (actor map →
  StartMovie → Pause → FixMovieSequenceCamera → RunMacro, full pcall unwind; the
  `playPostAttackDadCutscene` shape), `Scene.Lua` (StartMovieSequence … EndMovieSequence +
  CameraResetToViewBehindHero), `Scene.Say` (resource `Speak`, selection 0), `Scene.Ask` (GiveHeroYesNoQuestion
  + MsgIsQuestionAnsweredYesOrNo poll, `1` = yes as in the retail BookTrader/AffairMan scripts).
- `stranger.lua` — `Stranger.Routine` (quest thread: wait for `DadFinishedIntro` + first deed/3 gold →
  `CreateCreature(CREATURE_TRADER_01, MK_OVIT_SCARE2 pos, "OVR_Stranger")` → core quest marker →
  `IsTalkedToByHero` → the offer beat → `StrangerOfferMade/StrangerAccepted` → `FadeOutAndKillEntity`) and
  `Stranger.Massacre` (`GiveHeroWeapon(OBJECT_HERO_SWORD_FIRST)`, `SetHeroWeaponsAsUsable`, every creature
  `EntitySetAsKillable`, `*GUARD*` defs `SetAttackHeroOnSight`, `AddQuestInfoCounter` kill count, six dead →
  `OverrideMusic(25)` + `AttackOver = true`). `GROWN_FOR_THE_NIGHT` switches in the
  `TurnCreatureInto(CREATURE_HERO)` fallback.
- `OakvaleReborn.lua` — `Init` adds the three flags + `MassacreKills`; `OnPersist` transfers `AttackOver`,
  `StrangerOfferMade`, `StrangerAccepted`; `Main` starts `StrangerRoutine`; the post-attack scene picks
  `CS_OVR_AFTERMATH_EVIL/GOOD` by branch.
- `OVR_Theresa.lua` — `finishTheresaChildhood` runs `CS_OVR_REFUSE` instead of the retail scene +
  `PlayAVIMovie`; the departure check is skipped once `StrangerAccepted`.

Every binding the new code uses was grepped in `LuaManager.cpp` / `LuaRetailResources.h` /
`NoviUnitBindings.h` (`CreateCreature(def, pos, scriptName)`, `GiveHeroYesNoQuestion(q, yes, no, "", true)`,
`EntitySetAsKillable(thing, bool)`, `SetAttackHeroOnSight(thing, bool)`, `TurnCreatureInto(thing, def)`,
`GetAllCreaturesExcludingHero()` → table, `FadeOutAndKillEntity(thing, bool, secs, bool)`, `Log`, `Pause`).

## The smoke runner covers authored packages and module tables

`smoke_run_unit.py --package-dir <FSE dir>` smokes any tree; files that `return` a module table get every
`Mod.fn` field called (mock `resources` for a parameter of that name, functions for `body/predicate/
target/add*`, a mock state table for `state/entityState/progress`). The mock also gained `xpcall`,
`GetStateBool → false` (0 was truthy and short-circuited every flag test), `GetVillagerSpeechLists`/
`NewBarrelWatchSnapshot`/`NewPresentedItemOutput` as objects, dotted function names and nested
table-constructor keys in `free_globals`. Baseline on the untouched v4 stage went 29 → 15 "problems",
all of them mock-shape artefacts (`Refresh()` returning nil, `deed` typed as a thing); the authored tree
reports the same 15 and **0 unknown methods, 0 load errors, every new function ok** — that is the gate
`build_custom_intro.py check` applies.

## Text + VO stage, offline-proven

`build_custom_intro.py text` stages manifest `lines`: subtitle-only lines through `text_build.add_text`
(speechbank `ScriptDialogue.lug`, speaker `UNSPOKEN`/`NONE` for HUD/question text as retail does); `vo: true`
lines through `dialogue_pipeline.stage(--add)` chained on the staged tree so text.big, `<bank>snds.bin`,
`<bank>.lut` and `dialogue.big` grow together. Proven on a scratch manifest with one voiced line from a
planted synthesis: `ALL CHECKS PASSED` (crc-sorted snds pair 3061, lut IDENTITY fixpoint, 2769 retail clips
identical, lipsync 98 frames), overlay = 6 files. The real manifest's 7 placeholder lines stage into a
text.big that round-trips byte-identical (28,920 entries).

`elevenlabs_vo.py` (`--dry-run` lists lines/voices/character cost; `pcm_22050` → PCM16 RIFF; trim below
-50 dBFS keeping 150 ms; peak -1 dBFS / RMS -20 dBFS; sha256 cache; `ELEVENLABS_API_KEY` only) — the
post-processing was exercised from a planted cache entry (4.0 s → 2.28 s, RMS exactly -20 dBFS). Later the same night the user supplied a key (env only): the four Stranger lines were synthesised with
George (`JBFqnCBsd6RMkjVDRZzb`), 208 characters, and `all --tag v1` staged them into ScriptDialogue2
(clips 3061-3064, lipsync curves, snds pairs) with ALL CHECKS PASSED per line; v1's overlay is now 6 files.

## cs_lint.py

Manifest/`.cs`/Lua cross-checks: unique `TEXT_OVR_` keys, speakers in the 379-name NarratorList, `vo` needs
`bank`; `source` xor `clone_of`; `.cs` sections; every verb in the 184-verb table (actor verbs as `.Verb`);
actor prefixes declared (manifest `actors` + `RegisterActor` + `Create` names + HERO); `'TEXT_*'` retail or
manifest; camera/marker/thing targets of `UseCamera/NoLoadUseCamera/.Teleport/.WalkTo/.RunTo/.LookToThing/
Create/CreateEffect/PlaySound/RemoveExtras/SetDoorOpen` in the StartOakVale TNGs (extracted once from
`FinalAlbion.wad` into `work/oakvale_reborn/tng_cache`), manifest markers or actors; Lua `RunMacro("CS_OVR_*")`
and `"TEXT_OVR_*"` literals exist. A deliberately broken manifest trips all 24 expected findings; the real
one is clean.

## Cutscenes: `clone_of` and the refuse draft

`CS_OVR_REFUSE.cs` (draft) = the retail Theresa scene's staging (RegisterActor, gate, cameras, teleports,
`SetupCond` Create/RegisterActor mirrored) with the raid removed and `SetTime 22`; `CS_OVR_AFTERMATH_GOOD/EVIL`
are `clone_of: CS_OAKVALEINTRO_HESDEADJIM` (dumped from pristine, re-set under the new name). Staged
script.bin: 599 defs, **599/599 round-trip byte-identical**, validate clean (the 4 "loose" prefix-slop commands
are retail's own in the cloned scene).

## Install is a layer, not a forge stage entry

The live install already carries a FableForge world stage (text.big among it), so `pristine` is "as installed"
(it says so per file) and `install`/`restore` keep `<file>.ovrbak` + `install_receipt.json` and never touch
`forge_stage_manifest.json`; `restore` refuses a file whose bytes changed underneath (`--force` overrides).

## Waiting on the game

Bundles: `bundle-spike-s1` (S1+S3), `bundle-spike-s6` (child combat; Lua only, no overlay needed;
`spike_s6.py --grown` builds the fallback variant), `bundle-v1` (the scaffolded quest; needs `install`).
Commands and pass tables in `CHECKLIST.md`. Not done: STORY.md beats (user), `CS_OVR_OFFER`, new `CAM_OVR_*`
markers (S2), a real Stranger look, VO casting (`eleven_voice_id`).

## Later: the beats, locked and built

The user reacted to a drafted beat sheet: **never named, Father dies, protect them, 6 kills, keep the cold
open.** "Father dies" on the refuse road was taken as: the Stranger does himself what the hero refused
(kills Father, takes Theresa, burns Oakvale), so both roads keep the retail section swap and the dead-father
stage — the choice is whose hand. Built: `CS_OVR_COLDOPEN` (dawn on the raid's cliff camera, `SetTime 6` →
12 restored from Lua), `CS_OVR_OFFER` (Father-intro cameras, four lines, held wide; question + answer in
Lua), `CS_OVR_REFUSE` (Theresa's vision, the fence at night, `ENFLAME_COLUMN` fires on the bandit markers,
Father `STANDARD_DEAD`, the Stranger `ST_HOLDING_ANOTHER` with Theresa cowering, both `.FadeOut`),
aftermath clones with `insert_before` (build + lint support). `stranger.lua` gained `ColdOpen`, the watcher
(deed-keyed comment within 5 m, 12 s cooldown, offer opens on talk or on passing with the chocolates), and the
protected set; `OVR_LiveFather` waits for `ColdOpenDone` (persisted) and shows `TEXT_OVR_DAD_010` after
the highlighting box. 20 lines (14 voiced, 813 chars; George/Lily), 601/601 round-trip, lint 0, gate 0.
