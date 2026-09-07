# Cutscenes — CCutsceneDef macro stream, RunCutsceneMacro_Func verb API, content census

Reference (created 2026-09-07) for the retail cutscene system: the `CCutsceneDef` text-command
payload in `script.bin`, the native executor `RunCutsceneMacro_Func` @ `0x00CBFB7D`, its closed
184-verb vocabulary, and the shipped-content census/validator. The interpreter's architectural
finding (no bytecode VM; `strncmp` verb chain) and its per-verb slices are described in
[QUEST_SCRIPTS.md](QUEST_SCRIPTS.md#cutscene-and-region-script-interpreter); this file holds the
dated facts. Retail `Fable.exe` @ ImageBase 0x400000.

Artifacts (all under `ghidra_out/`): `runcutscenemacro_full.c` (full decomp, 1.02 MB),
`cutscene_verb_slices.md`, `cutscene_native_verbs.txt`, `cutscene_verb_census.tsv`,
`cutscene_verb_args.tsv`, `cutscene_exact_verb_xrefs.log`, `labels_cutscene_helpers.tsv`,
`decomp_cutscene_helpers.log`. Tools: `tools/ghidra_scripts/FindExactStrXref.java`,
`tools/ghidra_scripts/DumpVerbSlices.java`, `tools/extract_cutscene_arg_sigs.py`; FableForge
`forge script cutscenes|cutscene|command-stats|validate|verbs`.

## Verified facts (from FINDINGS log)

- **2026-07-19 — Cutscene macro command stream and native handler xrefs.** `forge script cutscenes
  <game-root>` decodes all 595 retail `CCutsceneDef` records: payload = 9-byte header, u32 command
  count at `+0x09`, then that many NUL-terminated command strings at `+0x0D`. `forge script cutscene
  <game-root> CS_ARENA_ROUND_1` = entry 15, 2,310 bytes / 69 commands, starting `UseCamera
  P_ARENACUOC,-1,NULL,0,0`, `PutUpYourSwords`, `Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1`.
  Executor `RunCutsceneMacro_Func` (`0x00CBFB7D`); camera preload helper `CutsceneCameraPreloader`
  (`0x00CBF29F`). Exact string xrefs (`FindExactStrXref.java` → `cutscene_exact_verb_xrefs.log`):
  `UseCamera` → `RunCutsceneMacro_Func+0xA3BC` and `CutsceneCameraPreloader+0x10C`;
  `DoCameraPreloading` → `+0x8B53` and `CutsceneCameraPreloader+0x29`; `FadeIn` → `+0x10DA7`;
  `FadeOut` → `+0x10E0E`; `Create` → `+0xC6C9`; `CreateEffect` → `+0xC01D`; `NoLoadUseCamera` →
  `+0xA2EC`; `PutUpYourSwords` → `+0x9785`; `GamePause` → `+0x8D54`; `DoScriptFrame` → `+0x7508`;
  `RemoveExtras` → `+0x6F50`; `WaitActiveDialog` → `+0x69EE`.
- **2026-07-19 — Retail cutscene verb census (full install).** 595 `CCutsceneDef` records contain
  18,648 macro commands across 1,563 distinct verbs (long tail = entity-scoped `<NAME>.Verb` forms).
  `forge script command-stats <game-root>` (FableForge `82be9d7`) head: `GamePause` 2,982 in 480
  scenes; `UseCamera` 1,713 in 512; `DoScriptFrame` 762 in 293; `FadeOut` 684 in 416; `FadeIn` 569 in
  424; `HERO.Teleport` 521 in 347; `WaitActiveDialog` 365 in 135; `DoCameraPreloading` 338 in 311;
  `HERO.PlayAnimation` 314 in 162; `Create` 312 in 131; `RemoveExtras` 301 in 155. The dispatcher must
  split `X.Verb` on `.` and route the suffix per named actor.
- **2026-07-19 — RunCutsceneMacro_Func interpreter architecture (verb-slice pass).** Full decompile
  (14,801 pseudo-C lines, 186 s at 512 MB payload cap) sliced at the 12 xrefs by `DumpVerbSlices.java`
  → `cutscene_verb_slices.md` + `runcutscenemacro_full.c`. Dispatch: per verb
  `CCharString(verbLiteral) -> GetLength -> strncmp(command, verb, len) == 0` in a linear if/else
  chain (prefix match, case-sensitive); args pre-split into `CCharString` locals `local_44` (arg1),
  `local_40`, `local_3c`, `local_38`, `local_34` (arg5). Blocking waits: `GamePause <seconds>[,clock]`
  — with `clock` (stricmp) waits on `GFGetTime()` until start+seconds, else iterates `seconds * 15.0`
  engine ticks; each tick pumps cutscene-manager vtbl `+0x1c`, `PlayCutsceneSound(soundMap,
  DAT_013B83C8++)`, breaks on `IsCutsceneSkipped()`; `WaitActiveDialog` loops while manager vtbl
  `+0x5C0` (dialog-active) is true. Cutscene manager global `DAT_0143E8F8`, slots: `+0x1C` per-tick
  pump, `+0x118` hero as `CScriptThing`, `+0x120` find `CScriptThing` by name, `+0x5C0` dialog-active.
  Actor resolution `GetCutsceneActorScriptThing` (`0x00CBF9DE`): empty or `"HERO"` → `+0x118`; else
  `FindRegisteredCutsceneActor` (`0x00CD2B86`) over the `RegisterActor` map, fallback `+0x120`.
  `GFCharStringToFloat` (`0x0099E690`) = hand-rolled sign/int/fraction parser over
  `CCharString{ptr,len}`; `GFStringToFloat`/`GFStringToInt` handle `char*`. Adjacent verbs found:
  `SetDoorOpen` (after GamePause), `RegisterActor` (after WaitActiveDialog). `DAT_013B83C8` =
  sequential cutscene-sound cursor; wait loops also exit when `*(char*)(DAT_013D2838 + 5)` is set.
  Labels `labels_cutscene_helpers.tsv` (3/3). Caveat: many surrounding names inside the function are
  stale BSim/RTTI artifacts (`_Cons_val<...>`, `C3DClothPrimitive::~C3DClothPrimitive`,
  `SetMiniMapRegionExitTextOffsetX`) — trust control flow and verified helpers.
- **2026-07-19 — COMPLETE native cutscene verb API (184 verbs) + content census.** Regex over
  `runcutscenemacro_full.c` (every `CCharString::CCharString(_, "<literal>", -1)` with `strncmp`
  against `local_c` within 700 chars): exactly **184 verbs** — 61 entity-scoped `.Verb` + 123
  globals (`cutscene_native_verbs.txt`); census vs the 156 used actions in
  `cutscene_verb_census.tsv`. **43 native but unused** in all 595 cutscenes: `CreateLight`,
  `CameraRig`, `CameraLookAt`, `CameraEffect`, `LiftRock`, `ExitGame`, `HUD`, `Print`,
  `GiveHeroMorality`, `HeroTattoo`, `WaitFlag`, `RegisterScript`, `.TurnInto`, `.Decapitate`,
  `.FightWith`, `.ModifyHealth`, `.AILevel`, `.Killable`, … (free headroom for mods). Prefix-match
  dispatch means malformed content still executes: `DoScriptFrame1` → `DoScriptFrame`,
  `.PlayLoopingAnimation` → `.PlayLoopingAnim`, `.PlayCombatAnimation` → `.PlayCombatAnim`,
  `RemoveThing` → `Remove`, `.DialogSpeakSpeak` → `.DialogSpeak`. Genuinely dead content commands
  (silent no-ops): `.ClearActions`, `DoScriptName`, `.SetEntityMaxWalkingSpeed` (correct
  `.EntitySetMaxWalkingSpeed`), `.SetScriptedMode`, `AnimationPhysics`, `ClearCommands` (bare global),
  `dGamePause`, `nop`.
- **2026-07-19 — Shipped-content cutscene bug census (validator ground truth).** `forge script
  validate <game-root>` (FableForge `18f81e7`; `forge::cutscene::resolveVerb` replicates the
  case-sensitive strncmp prefix dispatch, unit-tested) over 595 cutscenes / 18,647 commands: 10
  distinct dead commands and 20 prefix-accident commands. Dead: `Teacher.ClearActions` (27 uses in 27
  scenes — native is `.ClearCommands`), `DoScriptName` (x4), `HERO.SetEntityMaxWalkingSpeed` (x2),
  `GamePAuse`, `Fadeout` (case typos), `Hero.SetScriptedMode` (native `.AddScriptedMode`),
  `AnimationPhysics`, `ClearCommands`, `dGamePause`, `nop`. Loose: `.PlayLoopingAnimation`/
  `.PlayCombatAnimation` (trailing `ation`), `DoScriptFrame1`, `RemoveThing`,
  `Mother.DialogSpeakSpeak`. The 27-use `Teacher.ClearActions` never clears that actor's queue in
  retail — a latent behaviour a faithful port must not "fix".
- **2026-07-19 — Cutscene verb dispatch ORDER + heuristic arg signatures.**
  `tools/extract_cutscene_arg_sigs.py` walks `runcutscenemacro_full.c`, recovers each verb's dispatch
  anchor in file order (= chain order) and classifies the arg slots read (`local_44`=arg1 ..
  `local_18`=arg12, stride 4) → `cutscene_verb_args.tsv` (184 rows: verb, order, argcount,
  signature). The chain is ordered longest-prefix-first for every colliding group:
  `RemoveAllThings` (75) < `RemoveAll` (76) < `RemoveExtras` (79) < `Remove` (168); same for the
  `Camera*` and `Crowd*` families — prefix dispatch is unambiguous by construction. Arg-slot mapping
  verified against the hand-read slices (UseCamera arg1=camera name, arg2=time). Signatures are a
  lower bound: float args parsed by `__fastcall` `GFCharStringToFloat` (`0x0099E690`) appear argless
  (fall back to `str`); some `.Verb` handlers delegate to shared helpers and show argcount 0. Drives
  `forge script verbs` autocomplete; not a hard arg-count contract.
