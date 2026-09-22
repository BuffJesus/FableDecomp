# Archery scoring root-caused offline (2026-09-21)

Resume item (1) from the 2026-09-20 night block: the Skill-stage targets never scored even while their `SkillTarget`
Mains were alive (user shot them by hand; `SkillScore` 0 through ~3 minutes, no Lua error). Planned as an in-game
log-line run; instead it fell out of the generated Lua + the retail bytes with no run at all.

## The two converter defects (both generic, both in `SkillTarget::Main` 0x00D41D00)

1. **Double constants read as floats.** Retail scores a hit by the projectile binding's out value `d` against three
   `.rdata` doubles — `fcomp qword ptr [0x1238010]` = **0.25**, `[0x12316f0]` = **0.5**, `[0x129f0b0]` = **0.75** (x87
   idiom `fnstsw ax; test ah, 5; jp` = fall through on `d < K`):
   `d < 0.25 → +worth`, `< 0.5 → +2·worth`, `< 0.75 → +3·worth`, else `+4·worth`; the arming loop before it is
   `if 0.5 < d then break else score -= 1` (a weak hit costs a point and re-arms; the old Lua compared against `0.0`).
   Ghidra prints `(float)_DAT_01238010` for a float and a double alike; `float_at` unpacked 4 bytes, and the low dword
   of every one of these doubles is zero → all three rings were `0.0`, so the "+3" branch sat under an impossible
   condition (`0.0 <= d` → `d < 0.0`). Fix: `UnitConverter.double_constants()` disassembles the unit once and
   collects every absolute `qword ptr` x87 operand; `float_at` unpacks those as `<d`.
2. **Field-pointer read-modify-writes dropped.** VC7.1 spells `master->SkillScore += n` as
   `piVar1 = (int *)(*(int *)(this + 0x18) + 0xa4); *piVar1 = *piVar1 + iVar5;` (also `+ -1`, `+ 1`, `+ worth`).
   The master-field folds in `native_evidence_lowering.py` only matched the direct dereference, so every one of these
   stores was a `-- TODO(native)` comment: the score literally had no writer. Fix: `inline_field_pointer` (step 2a)
   substitutes `*piVar1` → `*(int *)(MASTER + 0xa4)` up to the register's next assignment, with `*piVar1 = ..` (a
   store THROUGH the pointer) explicitly not counting as a reassignment (the first attempt cut the scope there).

Same shape elsewhere, now also landed: `WillDummy` (`WillScore + 1` — the Will stage had the identical latent bug, and
its facing offsets are 0.25/0.5 not 0.0) and `TheRealGuildmaster` (`SkillScore + 1`). No other `*pVar = *pVar ± ..`
residue is left in any unit draft.

## Gates

Oakvale draft gate (fresh `convert_new_oakvale.convert` vs the committed package) identical; Orchard / Trader /
Gameflow drafts + readable stages regenerated with NO diff; Guild regenerated (draft todo 336, only
SkillTarget / WillDummy / TheRealGuildmaster changed); smoke Guild 1/1 (BirdKiller, pre-existing);
`test_skill_target_and_friendly_actor.py` 5/5 (new `SkillTargetScoringTests`; the old `0.0 < damage` pin corrected to
`0.5`); full suite log `work/converter_suite_20260921.log`. v6 + v7 bundles rebuilt (offline; nothing deployed or
launched — the install is shared).

## Next run (user-driven)

Skill stage from the melee-stage checklist or a post-melee save: shoot the three dummies, watch `SkillScore` climb
(front 1 / middle 3 / rear 9 × ring 1-4), and check what the sidecar's `MsgIsHitByHeroWithProjectileWeapon` binding
actually returns for `d` — the rings assume retail's out value (a 0..1 fraction), so if every hit lands as `+4·worth`
or `-1`, the binding's out value is the next suspect, not the script.

## Full suite 2026-09-21: 1644 ran, 8 failure groups, ALL pre-existing (reproduced on the stashed pre-change tree)

`test_bully_proximity` (44 cases, mock host lacks `RetailRandModulo`), `test_live_father_intro` (`RunMacro`),
`test_watch_barrels_loop` (78 cases) + `test_watch_barrels_readable`, and the install audits
`test_audit_barrel_reward_component` / `test_audit_final_barrel_gold_release` / `test_audit_dead_father_cutscene_asset`
("installed TNG differs from baseline: StartOakValeWest.tng" = FableForge's probe cubes + barrel, `names.bin` hash).
Oakvale-lane environment drift (ForgeFSE-retail-shadow bindings / the shared install), not the converter. Everything
Guild/unit-lane is green; `work/converter_suite_20260921.log`.

## Also today: the PreMelee Guildmaster's XP orb (resume item 4) — fixed at the converter

`TheRealGuildmaster` 0x00D52E90: the orb comes back through a hidden-result slot and is copied into the polled thing
with `CCountedPointer<..>::operator=((CCountedPointer<..> *)(auStack_160 + 4), (int)&*(int *)(pCVar6 + 0x4))` — the
array-slot `(X + 4)` spelling of the thing's Data field, which the explicit-handle fold only matched as `&slot`. Now
`CScriptThing::operator=((CScriptThing *)auStack_160,(int)pCVar6)`, so the draft reads `xStack_160 = pCVar6`,
`EntitySetCutsceneBehaviour(xStack_160, 2)` and `while xStack_160 ~= nil and xStack_160:IsAlive()` — the pick-up wait
and its 10-s nag are live. The journal's other claim ("the orb position operand is the actor map") was a misread of the
readable stage's naming: the slot is the smash-effect vector at the orb call and only becomes the actor map later
(same Lua local, assigned in order) — nothing to fix there. `GuildmasterExperienceOrbTests` added (6/6); Oakvale gate
identical; other units no diff; smoke Guild 1/1; v6/v7 rebuilt. **Checklist consequence:** `xp_to_alarm` in
`guild_woods_return.json` now teleports the hero onto `PreMeleeDummyMarker` so the orb is collected (before, ALARM
followed PASSED at once because the wait was skipped); untested until the next run.

## Also today: the moving dummies (SKILL_MOVE) — every segment teleport was a TODO

`SkillTarget::Main` moves a dummy one segment per frame with `EntityTeleportToPosition(me, &fStack_64, angle, ..)`
where the C3DVector is three adjacent float slots (`fStack_64/60/5c` = x/y/z; two of the three call sites go
through a `pPos = (C3DVector_bv *)&fStack_a0;` alias set in an if/else). Ghidra's `&fStack_64` reached the lifter
as an unresolved stack address, so all three teleports (four vectors) were `-- TODO(native)` and the dummies stood
still. New `fold_stack_vector_builds` (runs on Ghidra's slot names, before `restore_stack_operands` drifts the
address-taken slot to `xStack_58`): the operand becomes `ENGINE_Vector3(x, y, z)` (Lua `{x = .., y = .., z = ..}`),
the component stores stay as plain float locals, an alias is re-issued after the last component store of its
block. Three follow-ups it needed: the operands are masked from `rename_scalar_stack_locals`' hidden-slot scan (an
overlap artefact left `fStack_64` unrenamed and its store refused), `kind_of` sees an inline vector, and
`EntityTeleportToPosition`'s `pos` is tagged `nativeKind: vector` (it had been left untagged BECAUSE of this
function: the untagged slot let the resolved vector fall into the float slot). Guild draft todo 336 -> 329; the
readable stage inlines the arithmetic (`{x = (step * i + start), ..}`). `SkillTargetMovingDummyTests` (7/7);
`test_cross_branch_goto`'s WoodsStage mock now lets the XP orb be collected (its always-true `IsAlive` never ended
the now-live orb wait). Gates: Oakvale identical, other units no diff, smoke Guild 1/1, Guild-touching tests
40/40, lifter/converter targeted 112/112. v6/v7 rebuilt.

## Also today: three more Skill/Woods-Departure gaps closed (Guild draft todo 336 -> 310)

* **Out-of-ring check (Skill Guildmaster 0x00D5AE70)**: `IsDistanceBetweenThingsOver(hero, ArcheryRing, 6.0)` was
  `(nil, nil, 6.0)` -- the GSI pointer cached in a register the export mistyped as a by-value string
  (`CVar10 = *(CCharString_bv *)(this + 4); (**(code **)(*(int *)CVar10 + 0x120))((void *)CVar10, ..)`).
  `normalise_typed_decompile` rewrites it to the plain interface-alias spelling; `strip_receiver_arguments` then
  drops a register alias (and its spill) once the `GSI->` calls are its only reads before the next assignment.
* **Grade text**: `std::map<CCharString,CCharString>::operator[]((map<..> *)&iStack_1d0, aCStack_118)` -- the map
  operand's own cast -- now folds to `resources:SetString(map, "$GRADE", TEXT_.._GRADE_X)` feeding
  `RunMacroWithStrings("CS_GUILD_SKILL_WON_START", ..)` (7 grades).
* **FinalMaze (WoodsDeparture) `PauseAllNonScriptedEntities(true/false)`** x8: the interface pointer held in a stack
  slot (`xStack_7c = *(int **)(this + 4)`), which `annotate_interface_slots` excluded from the alias set. Now
  admitted when the slot is only ever assigned that way AND it is the object pointer (`*(int **)`), not a cached
  vtable (`**(int **)`): the vtable form in NOVI_Guard prints a vcall without its receiver and moved the Oakvale
  gate, so it stays excluded. FinalMaze 0 TODOs.
Remaining Skill residue: `xStack_1ec = 1 - xStack_1ec` (the first/second-comment toggle on repeat talks; cosmetic).
Gates after all of it: Oakvale identical, other units no diff, smoke Guild 1/1, targeted 137/137; v6/v7 rebuilt.

## Also today: the Will Guildmaster's cutscene actor maps (Guild draft todo 310 -> 305)

0x00D5E0C0's WILL_CONTINUE / PLAY_WHISPER macros ran with HERO and WHISPER missing from their actor maps: HERO's
value printed as `&CStack_204` -- the hero's TryAcquire'd resource `xStack_200` through the 4-byte ESP drift on a
byte-split address (the bytes push the same `[esp+0x21c]` to TryAcquire at 0xd5e6ce and to the map store at
0xd5e74c); WHISPER's key was named 4 bytes apart by the slot restoration between its ctor (`xStack_19c`) and its
operator[] use (`xStack_1a0`). Two generic folds: `fold_drifted_actor_values` (an actor value that is no resource
but whose slot-minus-4 IS one acquired in the function -- an actor value can only be a resource) and a re-key in
`fold_actor_maps` (the nearest literal-string ctor above an operator[] whose use name differs by exactly 4 and is
otherwise unused in between). `WillGuildmasterActorMapTests` (8/8; targeted 138/138). Residual in that entity: two
epilogue `Release`-style vcalls on a cleanup object (cosmetic).

## Also today: WoodsWill's bandit resources (Guild draft todo 305 -> 295; the quest file 10 -> 0)

`Q_GuildTrainingWoodsWill::Main` 0x00D67890 holds one resource per WillBandit in a local `CArray<resource>`:
`pvStack_2c = 0; uStack_28 = 0; uStack_24 = 0;` (begin/end/cap), a stack template through the resource ctor
(0x7E72A0), `CArray<..>::push_back(&arr, count, tmpl)` (0xCD3CCB = n copies), elements at `begin + 0x10*k`
(the TryAcquire loop, the BAN1/BAN2/BAN3 actor-map stores), and the array destructor (0xCD319B, twice). New
`fold_local_resource_arrays` (runs before constant propagation, which had turned the zeroed begin into
`(0x0 + iVar12)`): `arr = RESLIST_New(n)` (a Lua list of fresh resources), `RESLIST_At(arr, k)`, `RESLIST_Destroy`
(release every element), the template spelled as its own stack resource; plus `StartScriptingEntity`'s operand
split now tolerates commas inside an operand, the actor-map alias accepts a list element, and the generic vector
lifetime cleanup accepts VC7.1's backward `clear()` (`end = end + -3`). `WoodsWillBanditResourcesTests` (9/9).
Gates: Oakvale identical, other units no diff, smoke Guild 1/1, targeted 138/138; v6/v7 rebuilt.

## Also today: CheckFriendlyAttacks' Maze checks + the two SaveXP cutscenes (Guild draft todo 295 -> 290)

* `Q_GuildTraining::Main` calls two helpers bsim labelled alike (`RunSaveXPCutscene2` on 0xD496F0 and 0xD49A20);
  the label->local-function map only took unambiguous labels, so once `disambiguate_call_labels` split them per
  site (`__at<addr>`) neither call resolved: `RunSaveXPCutscene` / `RunSaveXPCutscene2` were never called. The
  renamed labels now map by target address.
* CheckFriendlyAttacks 0x00D45060 asks whether the hero hit the Maze through the Maze thing's Info pointer under its
  own Ghidra name (`uStack_9c` = `auStack_a0 + 4`: nulled, null-tested, vcall receiver for 0x54 / 0xa8 / 0xa4). The
  2026-09-20 fix canonicalised the object to `xStack_90`, so slot arithmetic on the new name could not find the field;
  `restore_stack_operands` now carries such a field along with the object (`xStack_90._4_4_`), and the checks read
  `maze:MsgIsHitByHero()` / `MsgIsHitByAnySpecialAbilityFromHero()` / `MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)`.
  (A first, lowering-side attempt keyed on "a nulled slot with a thing cast 4 bytes above" over-matched five slots
  in other functions -- reverted; the restore stage is the only place that knows the canonical mapping.)
`FriendlyAttackMazeChecksTests` (10/10). Gates unchanged: Oakvale identical, other units no diff, smoke 1/1,
targeted 139/139; v6/v7 rebuilt.

## Also today: BirdKiller's marker list (smoke Guild 1/1 -> 0/0)

The standing smoke problem (`BirdKiller.lua:43 arithmetic on a nil value`) was the sparrow-marker vector's count:
the end pointer drifted to `x_stk_60` and the idiom was spelled with one cast (`(int)x_stk_60 - iStack_68`), a form
neither end-pointer rule in `fold_local_thing_vectors` accepted (both-cast / no-cast only, and the no-cast rule
wanted at least one zero store -- the drifted name's zeroing had already been folded away). Now `#iStack_68`; every
sparrow is spawned. Smoke: Guild 0/0, Orchard 0/0, Gameflow 0/0, Trader 1 (unchanged files, pre-existing).

## Also today: TraderToRescue's exception-state word (Trader draft todo 45 -> 39)

The one Trader smoke problem (`FREE GLOBALS: __unknown_push, unaff_EBP, unaff_EBX`): `unaff_EBP` was the EH-state
word threaded through three registers (`uVar12 = unaff_EBP | 1; .. unaff_EBP = uVar12 | 8; .. uVar14 = uVar12 & ..`),
which `drop_eh_state_flags` only recognised under one name; and `rand((int)unaff_EDI,unaff_ESI,unaff_EBP,unaff_EBX)`
counted as a non-flag use. Now a closed set of registers whose every line is a flag shape or a member-to-member copy
(set or clear) is one flag, `rand(..)` folds before the check, and `(char)SUB41(x,0)` is the low-byte sign test:
every `& 0x20`-style guard and its destructor block are gone. `unaff_EBX` (the first CTimer's id, which the bytes
load from `[esp+0x18]` at every site) is NOT recovered: the export's per-site pushed slots disagree across those
sites (one names the other timer), so the register->slot recovery in `restore_stack_operands` only fires when every
site of a register agrees on one object -- here it does not, and the free global stays (documented residual; a
disassembly-backed per-site operand reader would settle it).

## Autopilot run 1 (12:51): CRASH at SkillTarget's first frame -- caused by today's moving-dummy fix, fixed

The game died on `ENTERING LUA CALL for SkillTarget` (the first of three; yesterday all three entered back to back).
Cause: the frame-0 marker lookups `GetNearestWithScriptName(pCVar6, "StaticDummyMarker2"/"1")` had ALWAYS been in the
draft with a nil receiver on the dummy-2/3 paths (Ghidra's `pCVar6` = `edi` = `this + 8`, but a sibling branch's
reassignment ended the alias textually), and the old readable stage dropped them as dead stores because their only
consumers were the TODO'd teleports. Today's vector fix made them live -- and `GetNearestWithScriptName(nil, ..)` is a
native crash, not a Lua error. Bytes: all five slot-0x134 sites `push edi`. New `resolve_me_register_uses` (entity
scripts): a `pCVarN` operand whose nearest DOMINATING definition (block structure by braces; a definition's own
statement does not count; the explicit receiver slot of a vcall through the same register is left to the annotator)
is `(this + 8)` is `me`. Guild/Oakvale/other units otherwise unchanged; CombatApprentice's
`SetFriendsWithEverythingFlag(true)` was the receiver-slot regression caught by `test_binding_flags_and_melee_stage`.
Lesson (GOTCHAS): a fold that makes previously-dead code live must be re-checked for operands that were only ever
"fine" because they were dead.

## Sidecar: `MsgGetThingsKilled` binding (new, DLL rebuilt)

TraderConflictGood::WatchForKilledPeople polls `hero:MsgGetThingsKilled(&uids)` every frame to clear the no-kills
reward flag; ForgeFSE had no binding, so the flag never cleared. `LuaEntityAPI::MsgGetThingsKilled(CScriptThing*) -> bool`
(retail vtable 0xDC) hands retail a zeroed raw begin/end/cap vector and frees the buffer with the game's `free`
(never ours); the lowering drops the out operand, its zeroing and the `if (v) free(v)` guard. Also there: the
follower-vs-AllCreatures `IsEqualTo` loop (element Data vcall printed without casts on an int-typed begin slot).
Trader draft todo 45 -> 35. Smoke Trader now reports 2: the `unaff_EBX` free global (unchanged) and a mock
call-budget overflow in WatchForKilledPeople (two calls per frame in an endless watcher: harness artefact).

## Autopilot run 2 (13:1x): childhood through PASSED clean; the XP orb is collected by WALKING, not by a teleport

Everything up to `passed_scene` PASSED first try (SkillTarget's three Mains entered, no Lua error, punch 7/7, stick
7/7). `xp_to_alarm` FAILED: the orb wait is live (timer registered + the 10-s nag conversation queued right after
PASSED_20 -- retail behaviour), but 13 `hero teleport PreMeleeDummyMarker` never collected the orb. Driven by hand on
the same live game, a short cross walk (W/S/A/D holds) through the marker picked it up at once and ALARM_10..40 ran.
Checklist step updated (teleport + walk); the chain was resumed on the live game from `quest_start_screen` with a
spliced checklist (`.scratch_tmp/resume_after_alarm.json`) rather than relaunching 45 minutes.

## Resume 2 (13:3x-14:1x): childhood + melee stage clean; two driver fixes; the BADHERO residual fixed for real

Spliced from the quest card on the live game (`--tail-back` added to the driver: an attached run's first marker had
been logged seconds before the attach): card, woods, 10 beetles, return, WOODSWON, split AVI, then the whole teen
melee stage hands-free -- attack 7/7, block 5/5, the Whisper fight, the grade -- until `continue_answer`: the
driver's `clear` verb only clicks on a detected game-info box and the Continue/Retake question is not one (sat there
120 s; one real `lmb` answered it: MELEE_CONTINUE, Skill host up). Step fixed (`lmb 1`).
Skill stage: SKILL_START, the bow, TutorialState 2 -- then every autopilot arrow hit a PERSON: four seal warnings in
a row (`[HitEvidence]` shows no Maze hit, so today's Maze checks did not fire -- these were the engine's ranged
friendly-hit messages), BADHERO ran, and its cleanup raised `Invalid or released retail resource` at
`GuildTraining.lua:710` = the known residual (`ReleaseResource(actorMap)`: the export put the hero resource's
destructor at the actor map's slot). Fixed generically: `fold_actor_map_releases` -- a release whose operand is an
actor map takes the nearest resource acquired above with no release since (runs after the TryAcquire respell).
The archery steps now FACE the nearest `SkillTarget` from the ring (`EntitySetFacingAngleTowardsThing`) and shoot
without a SPACE lock (the lock picked a person); `GUILD_SEAL_*_WARNING` is a forbidden marker there so a mis-aimed
run fails fast. Run 3 (full chain, 14:2x) launched with all of it.

## Run 3 (13:51-14:1x) + hand-driven Skill stage: THE ARCHERY SCORES, THE DUMMIES MOVE, the Will stage is reached

Full chain hands-free through 35 steps (orb collected first try, Continue clicked, Skill host, bow hand-over), then
`archery_first_target` failed on time: the screenshot showed the hero in the ring facing the dummies with the BOW ON
HIS BACK -- the checklist's single `E` was swallowed (a box was still up). Driven by hand on the live game:
* `E` + a 1.5 s draw: **SkillScore 1**, HUD 1/3, the front dummy knocked down (`skill_stage_2of3_2026-09-21.png` is
  the 2/3 frame). Each static-phase hit is exactly retail's `+1` from the arming loop; the ring multipliers belong to
  the moving phase. Facing the second/third-nearest `SkillTarget` (a `table.sort` by `GetDistanceBetweenThings`) plus
  a SPACE lock: 2/3, 3/3 -> `CS_GUILD_SKILL_MOVE` (SKILL_MOVE_10..50), `MovingDummiesNeeded = true`, TutorialState 4.
* Moving round (10 arrows, 00:51): the dummies SLIDE along the wall (`skill_stage_moving_2026-09-21.png`: the front
  one far left -- this morning's `fold_stack_vector_builds` teleports), one hit scored **+3** = front worth 1 x the
  0.5..0.75 ring: the double-constant rings and the `piVar1` stores, in-game. Two tutorial boxes pause the round
  (first-person aim; "Press E to unsheathe your bow" after the cutscene re-sheathes it); the hero drifted out of the
  6 m ring during the hand shots -> `DISQUALIFIED_10/20` (the out-of-ring check fixed today, firing as retail does),
  the Continue question, SKILL_CONTINUE, quest end, **`GuildTrainingWill` host live**: talked to the Guildmaster,
  `WILL_COMMENT_FIRST` nag x3, no Lua error. Nobody had reached the Will stage on our scripts before.
No `LUA RUNTIME ERROR` anywhere in the run (the BADHERO release fix was not exercised: no warnings this time).
Autopilot residue for the Skill stage: draw the bow reliably (verify by a capture, or press E after every box), face
the nearest TARGETABLE dummy (sort by distance and rotate through 1..3 as they go down), clear the two moving-round
boxes, stay inside the 6 m ring. Log archived `work/ab_runs/v6-20260921-135146/`.

## Evening: TraderToRescue's timers settled from the bytes; Skill/Will checklists rebuilt; run 4

* `_register_operand_slot` (convert_quest_unit.py): for a stale-register operand at a known vcall site, disassemble
  the function, take the push matching the operand's position (right-to-left), and follow that register to its
  `mov REG, [esp+X]`; the slot is `-(sub + 4*prologue pushes - X)` (callee-saved registers may cross earlier calls).
  Ghidra's `unaff_EBX` was a `push esi`; all sites resolve to `-0x16c`, the first CTimer. Accepted only when every
  site of a register agrees. A by-value CScriptThing push (`sub esp,0xc; mov ecx,esp; push SRC; call 0x4ABE90`) is
  now one operand of the walker (`SetIsPushableByHero(me, true)` -- the old guess said `hero`). The epilogue's
  `DeregisterTimer(0x1)/(0xa)` were drifted slot names (`xStack_160/168` for `164/16c`): nearest registered timer
  within 8 bytes, each consumed once per run. TraderToRescue: no free globals left.
* Checklists: `xp_to_alarm` holds LEFT SHIFT after the teleport (attracts XP orbs -- user tip; PASSED first try in
  run 4); `guild_skill_stage.json` rewritten hands-free: `EntityUnsheatheRangedWeapon` when
  `IsEntityWieldingRangedWeapon` is false (keyboard E gets swallowed by boxes), face + `EntitySetRangedTarget` the
  (SkillScore mod 3)-th nearest SkillTarget, 1.5 s draw; SKILL_MOVE, the moving round, Continue.
  `guild_will_stage.json` DRAFT: host, talk (TutorialState 2 + WILL_LIGHTNING), first bolt via RMB (untested: PC
  casts with the right mouse button per INPUT.md). `gamewin.ps1` knows LSHIFT/RSHIFT now.

## Run 4 (17:xx) + hand-driven Will: the Skill stage is hands-free; two more converter bugs from the Will stage

* Run 4 chain: 35 steps hands-free to the Skill static phase; that step failed because the aim chunk teleported the
  hero onto the ring EVERY shot (cancels the release). Resumed with "teleport only when > 3 m from the ring" + SPACE
  lock: **archery_static PASS, 1/2/3 -> SKILL_MOVE in 11 sends**; the moving round PASSED as DISQUALIFIED -- and that
  was a CONVERTER bug: the WON/DISQUALIFIED selector is the left-the-ring byte `cStack_215`, which Ghidra read as
  `uStack_21c._3_1_` (`mov al,[esp+0x1b]` with one push outstanding = the flag at [esp+0x1f]; the timer id's top
  byte in Ghidra's frame) -> an unassigned local, `nil == 0` false, every round disqualified. `_drifted_byte_slices`
  now tries +4 when the sliced slot is a whole object (a constructor took its address); `SkillDisqualifiedFlagTests`.
  A `skip` ESC landing after SKILL_MOVE ended opened the pause menu (channel frozen) -- that step no longer skips.
* Will stage (splice, then by hand): host, talk -> TutorialState 2 + WILL_LIGHTNING all PASS first try. Casting on PC:
  NOT RMB, NOT the quick-slot key -- LEFT SHIFT held (action 86 "Activate Spell Mode") + the attack button; new driver
  verb `chord LSHIFT 1200` (gamewin.ps1 'chord'). The first bolt that landed raised `WillDummy.lua:53 sol: no matching
  function call` -- `EntitySetFacingAngle(me, nil, true)`: the spin angle store `fStack_a0 = angle + 0.25` had been
  respelled as the OBJECT `xStack_94` by the slot restoration (a by-value float operand), so the lifter refused the
  store. Fixes: `name_at` keeps Ghidra's float prefix when no object was constructed at the slot, a bare `fStack_`
  call operand is no hidden-result slot, and (unit mode only -- the Oakvale gate moved otherwise) arithmetic with a
  float literal is a number. WillDummy: 1 TODO -> 0. Run 5 (full chain + Will) launched 17:5x.

## Run 5 (18:xx): static archery hands-free in 4 sends; my WillDummy fix broke SkillTarget -- both fixed for real

`archery_static` PASSED in four sends (1/2/3 -> SKILL_MOVE, fully automatic). Then a sol error in `SkillTarget.lua`'s
third teleport: `EntityTeleportToPosition(me, pPos, false, false)` -- no angle. Cause: the float-prefix rule from the
WillDummy fix respelled the angle copy (Ghidra `fStack_164`, export slot -0x158) as `fStack_158`, which is ALSO
Ghidra's name for the marker's z, so the two merged and the operand vanished. Final rule: a Ghidra float slot with
no constructed object keeps GHIDRA'S OWN NAME (store and by-value use agree by construction; the export's per-site
number is what drifts) -- `f_stk_a0` in WillDummy, `f_stk_164` in SkillTarget; the moving-dummy pin now demands the
five-operand teleport at all three sites. (A heredoc turned `\b` into backspace bytes inside a regex on the way --
GOTCHAS already says to use the Edit tool for backslashes; it cost twenty minutes.) Run 6 launched 18:4x.

## Run 6 (19:xx): 45 steps / 0 failed through the Will round; the "CS_GUILD_WILL_WON" subtitle was a Speak key

Full chain hands-free: woods -> melee -> skill (static 1/2/3, moving round WON) -> Will (host, talk, lightning,
`chord LSHIFT` bolts: WillScore 1 -> 3 -> 6 -> 9, WILL_WON, WILL_CONTINUE, the continue-or-play-with-Whisper
question). `work/autopilot_skill_20260921k.log`: **45 step(s), 0 failed**. I answered the question by hand and the
Guildmaster's next subtitle read literally "CS_GUILD_WILL_WON".

Not the `$GRADE` string map (retail and the sidecar both use `std::map<CCharString,CCharString>::operator[]`
0x9AC700; the WON macro rendered its grade fine). The two Speak sites after the question (0x00D61088 / 0x00D5FD3F
and their mirrors) pass the text key as a BARE .rdata ADDRESS Ghidra never resolved: `Speak(me, 0x12d1148, 0,0,1,0)`
= `TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO`, `0x12d1368` = `..._YES`. `place_args` saw a number where
the string slot wanted a string, and filled the slot from the temporaries -- whichever literal was last pushed
("CS_GUILD_WILL_WON", "$GRADE", "WHISPER", "grade" depending on the branch). Fix (lift_native_lua.py, place_args):
when the binding has a string parameter, an operand matching `RE_ADDR_LITERAL` whose bytes are a printable
.rdata string becomes that string literal first. Only the Will Guildmaster changed (the NOT_START Speak also got
its real key back as a side effect: the stale temporaries were no longer stolen); Oakvale gate identical, other
units byte-identical, smoke Guild draft/readable 0/0, Orchard 0, Gameflow 0, Trader 0 (the mock call-budget
artefact is gone too), targeted tests 143 + `WillGuildmasterSpeakKeyTests`. v6 + v7 rebuilt; run 7 (chain +
Will checklist) launched 19:5x.

## Offline while run 7 plays: thing-call bools were numbers (a real in-game bug), and the Will apprentice's kill-on-unload

* `r4:SetFriendsWithEverythingFlag(0)` / `r3:SetToKillOnLevelUnload(0)`: every CScriptThing slot call with a `_N`
  operand passed Ghidra's `0`/`1`. The sidecar's sol2 has no safeties (`lua_toboolean`), and `0` is TRUTHY in Lua,
  so each "clear" SET the flag (the apprentices and beetles were friends-with-everything before their fights,
  the spawned apprentices were flagged to die on level unload). `thing_call` now coerces a `_N` slot's operand
  (`thing_bool_slots`), 13 sites across Guild; Oakvale gate identical (its thing calls carry no bool literals).
* Will Guildmaster 0x00D5E0C0: two of its three `SetToKillOnLevelUnload` sites were TODOs. Byte-identical code
  (`mov ecx,[esp+0xa4]; test; call [edx+0x118]`) printed `auStack_1b4._4_4_` at the first and `auStack_1b4._0_4_`
  at the other two (Ghidra's ESP model 4 bytes off for those sites). The Data-field idioms (validity test, vcall
  through `*(int *)X._0_4_ + SLOT`) now also accept `_0_4_` on a stack thing -- never a vtable read (that would be
  `X._0_4_ + SLOT`) -- with the counted-release guard the `_4_4_` rule already had (AppleGirl's release moved
  without it). Guild draft todo 287 -> 285; `ThingBoolOperandTests`; smoke all 0.

## Run 7 (20:xx): 45 / 45 again with the Speak keys fixed; `this_NN` aliases were dropped (nil releases on cleanup paths)

`work/autopilot_will_20260921b.log`: **45 step(s), 0 failed** through the Will round on the rebuilt v6 (the run
that carries the Speak-key fix). Offline in the meantime: every `this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;`
alias in the drafts was a `TODO(native)` because `RE_LOCAL_ASSIGN` excluded any name STARTING with `this` (the
lookahead meant `this` alone; `this_00` / `this_01` are Ghidra's names for a merged cleanup tail's receiver).
Every consumer of the alias stayed live: `resources:DestroyMovie(this_00)` (PreMeleeMaze, CombatApprentice),
`resources:ReleaseResource(this_01)` x8 (Melee Guildmaster), all on the thread-termination / cleanup paths -- a
nil handle to the sidecar on the exact path the user's "third warning kills the fight" report ran through.
`this\b` in the lookahead; Guild draft todo 285 -> 275; Oakvale gate identical (its `this = ..` aliases stay TODO,
`this` alone is still excluded); smoke all 0; targeted tests 146. (The heredoc ate `\b` into a backspace byte
AGAIN -- repaired with a chr(92) replace; the Edit tool is the rule.)

## Offline (run 8 playing): the PreMelee "hit the dummy" nag re-armed every frame

PreMelee Guildmaster 0x00D52E90 keeps the last `DummyHits` in `CStack_180` (init 0) and re-arms the 10 s nag timer
only when the count changes. The export typed the field read as a by-value CCharString (the slot had been a string
temp earlier), so the store stayed a TODO and the `(CCharString_bv)0x0` init was inlined: `if 0 ~= DummyHits then
SetTimer(10)` -- true on every frame after the first hit, so the nag never fired again. Three generic pieces:
`normalise_typed_decompile` retypes a parent int field read through a CCharString cast (and the local's init/decl);
the lifter stores a state getter into a stack slot in unit mode (the slot path only knew literals/temps); and a
slot that later stores a state getter keeps its literal init as a real local instead of a temporary. Guild draft
todo 275 -> 273; `PreMeleeNagTimerTests`; Oakvale gate identical; smoke all 0; targeted 147.

## Run 8 (20:xx): the friendly-fire chain -- arrows at point blank never hit, the melee variant froze the game

`guild_skill_badhero.json` (chain -> Skill state 2 -> shoot the Guildmaster): 34 steps PASS, then `badhero_warnings`
timed out with `HeroWarnings` 0 -- the hero stood at the Guildmaster with an arrow nocked (`scratchpad/badhero.png`)
and nothing registered. Attached rerun with a MELEE swing (EntityUnsheatheMeleeWeapon + lmb): the Skill quest raised
"Press 'E' to unsheathe your bow" and the game FROZE on it (log stopped at 17:28, Next / E / clicks ignored; killed).
Third variant = ranged from the ArcheryRing facing the Guildmaster (run 4 got four warnings exactly this way);
run 9 launched 17:3x with it. `guild_will_stage.json` extended past the round (repeat question -> WILL_CONTINUE ->
adulthood answer -> AVI -> Departure host) for the next chain.

## Offline: slot-zero residues; run 9 lost the woods step to a channel timeout

* `X._4_4_ = (int *)0x0` / `X[0] = (int *)0x0` beside an emitted `r1 = nil` (PreMeleeMaze, both ScorpionHomes) are the
  stack thing's Data nulled: `RE_SLOT_ZERO` accepts them. Guild draft todo 273 -> 269; of the 269, 14 are not
  label/goto/cleanup bookkeeping and none is a live gap (persist-name notes, `CreateEffect` has no binding, dead
  `if false` bodies, a cleanup-path register load). TraderConflict's 14 semantic TODOs are all TraderToRescue's
  broken-stack Main (`xStack_148` declared six ways) -- a separate lane.
* Run 9 (the ranged badhero chain from a fresh launch): 13 PASS, then `to_the_woods` "channel TIMEOUT after 15 s"
  (the retail transition step; first time in nine runs). The game was fine (child in the guild); resumed attached
  from that step with `--tail-back 200` (run 9b, `work/autopilot_badhero_20260921e.log`).

## Run 9b: reached the Skill stage, then the second instruction box ate every send; `clear` now clicks the icon

The attached resume ran 21 steps to Skill state 2, then `badhero_warnings` timed out with the "As before, press and
hold LMB... Try aiming by zooming in" box up the whole time (`scratchpad/badhero6.png`). `clear` detected the box
but sent a bare `lmb` where the cursor happened to sit -- with the bow out that is a draw, not a dismissal (the
earlier runs got lucky with the cursor position). `game_info_box_pos` (the lowest band of the box's green pixels =
the mouse icon on the 'Next' row) and `clear` now clicks 20 px right of it. Also lowered: PreMeleeWhisper's
`GetDataString` on each PreMeleeChatMarker (vector element vcall printed `(i + V)` without the `(int)` cast --
`tonumber(nil)` meant Whisper never removed the chosen chat marker); Guild draft todo 269 -> 268.

## Runs 9c-9e: the bow auto-aims AWAY from friendlies -- friendly fire moves to the Melee stage

With the box cleared (the icon click works), three ranged variants all left `HeroWarnings` 0: facing the
Guildmaster + lock (the lock picked a dummy, 9c), facing without a lock (the bow's aim reticle still chose the
dummy, 9d), the Guildmaster teleported ONTO the nearest dummy (still the dummy, 9e; `scratchpad/badhero7-9.png`).
Fable's bow auto-aim filters friendlies, so `MsgHitFriendWithRangedWeapon` needs first-person manual aim -- not
worth driving. The user's third-warning report came from MELEE hits anyway: `guild_melee_badhero.json` = the Melee
stage up to `attack_draw_sword`, then four sword swings on the Guildmaster per iteration until BADHERO, the
recovery check (HeroWarnings back to 0, host alive), then the stage's own attack/block/battle steps must still pass.
Run 10 launched 18:3x (fresh chain: woods + melee_badhero). Full suite `work/converter_suite_20260921e.log`: 0 FAIL/ERROR lines.

## Runs 10/10b: sword swings on the Guildmaster do not register either -- friendly fire is left to a hand test

Melee stage, sword out, four swings per iteration: ON the Guildmaster with a lock (run 10), 1.2 m beside him facing
him without a lock (10b, `scratchpad/badhero10-11.png`): `HeroWarnings` 0 both times, the 0/7 tutorial counter
untouched. The trainer is not a hittable target for a driven swing (the user's 2026-09-20 third warning came from
hand play -- most likely Whisper outside the sanctioned rounds). Time-boxed and STOPPED: the converter side of the
punishment path is pinned by tests (`FriendlyAttackPunishmentTests`, `BadHeroReleaseTests`, the `this_NN` cleanup
aliases in `ThingBoolOperandTests`' neighbours) and the in-game proof is a hand test: hit Whisper three times
before the fight and watch `HeroWarnings` / `CS_GUILD_BADHERO` / the host surviving in the log. Next run = the
full chain with the extended Will checklist (adulthood transition, Departure host).

## Run 11 (19:xx): 46 steps -- WILL_CONTINUE, the adulthood answer, Departure AND WoodsDeparture hosts, Maze meeting

Full chain + the extended Will checklist: `will_repeat_answer` PASS (Continue -> WILL_CONTINUE -> the play-with-Whisper
question), then `will_adult_answer` "failed" only because its expect named the wrong key: retail's YES branch
(answer 1 = Continue) speaks `..PLAY_WHISPER_QUESTION_NO` ("no, you won't play"), and the AVI / teenager / confiscate
bindings do not log. The log proves the block ran (MeleeApprentice + WillApprentice recreated, DEPARTURE_CHAT
10/20/30 + DEPARTURE_YES = the Departure GM_DONE cutscene, `GuildTrainingDeparture` AND `GuildTrainingWoodsDeparture`
hosts created, GameState 9). The hero model stayed teen with the bow on his back (`scratchpad/adult2.png`) -- whether
`SetHeroAsTeenager(false)` / `ConfiscateAllHeroWeapons` take effect only on the next level load is an open question.
Attached probe: the retail transition into GuildWoods binds ArtifactThief / FinalMaze / ScorpionHome, Maze meets
the hero (MEETING_30, MAZE_START, the 0/7 counter). `guild_departure_stage.json` written (GM_DONE, woods, the three
Maze phases: 7 sword / 7 bow / 7 lightning hits -> MAZE_WIN -> MissionSucceeded); the Maze phases run attached now.
Full suite rerun clean: `work/converter_suite_20260921f.log` = the morning baseline's 8 groups exactly.

## Run 11b (20:xx): GUILD TRAINING COMPLETED -- the whole arc runs on the converter's Lua

Attached on the run-11 game: the retail transition into GuildWoods, then `guild_departure_stage.json`'s three Maze
phases hands-free -- **7 sword hits (SKILL_FIRST in 8 sends), 7 bow hits (LIGHTNING_FIRST in 14), 7 lightning
bolts (VICTORY_10..30, MAZE_WIN, `SetQuestAsCompleted Q_GuildTrainingWoodsDeparture` in 10)**. The transition back
to the Guild ran EXIT_WOODS, the FrescoDome ceremony (SEAL_GIVE / SEAL_RECEIVE) and the return to the Cullis room
with no further input: the hero came out ADULT in the hero outfit (`guild_adult_hero_2026-09-21.png`). Last gate:
"Step into the light" = walk into OBJECT_EXPERIENCE_SPENDING_POINT (a teleport onto it does not trigger it; the
pause-menu Experience page is a different screen), ESC out -> `MsgOnLeavingExperienceSpendingScreen` -> SAVEXP2 ->
**`SetQuestAsCompleted: ENTER name='Q_GuildTraining'`**: "You have successfully completed your training and
graduated as a Hero" (`guild_training_completed_2026-09-21.png`; 609 general XP, 40 renown, 2 phials, lamp, potion).
Zero LUA RUNTIME ERRORs from the post-woods save to graduation. Two driver lessons: a `skip` ESC after VICTORY (no
scene up) opened the pause menu; the XP screen's X button did not take the click, ESC does. The checklist's
`departure_return` / `graduation` steps carry the working inputs; the chain from the save is now
woods_return + melee + skill + will + departure (untested end-to-end in ONE launch -- next run).

## Run 12 (20:xx-21:xx): 48 steps in one launch to the adulthood block; three driver gaps closed on the resume

One launch, five checklists: 48 PASS through `will_adult_answer`; `departure_host` then sat 200 s on the
Guildmaster's post-answer line ("I hope you're ready...") -- a Speak SUBTITLE box whose Next icon sits at y~687,
below the detector's row window (370-680). Window widened to 720/960 (`game_info_box_pos` finds it at 895,686).
Resumes 12b-12d on the same game: Departure host + GM_DONE + WoodsDeparture host PASS; `departure_to_woods` lost
the channel to a `skip` ESC landing after GM_DONE (pause menu) -> `skip` removed from every free-roam step of the
Departure checklist; the woods test PASS again (sword 5 / bow 14 / lightning 8 sends); the optional `thief_lamp`
step found NO ArtifactThief in the world (the binding registered, no EntityAllocator for it -- the thing is not
placed in this playthrough; step dropped); then `departure_return` (EXIT_WOODS + ceremony), `ceremony`,
`graduation` (walk into the light, ESC) PASS: **`SetQuestAsCompleted Q_GuildTraining` in 3 sends, fully driven**.
Run 13 = the same five checklists in one launch with these fixes.

## Run 13 (21:xx-22:xx): 58 / 58 IN ONE LAUNCH -- the Guild arc from the post-woods save to graduation, hands-free

`work/autopilot_full_20260921e.log` / `docs/journal/2026-09/autopilot_full_guild_2026-09-21e.json`: woods (beetles,
WOODSWON, AVI) -> Melee (attack 7, block 5, battle, grade, Continue) -> Skill (static 1/2/3, SKILL_MOVE, moving
round, Continue) -> Will (lightning, bolts to WILL_WON, repeat, adulthood) -> Departure (GM_DONE, the woods, Maze
sword 7 / bow 15 / lightning 9 sends, MAZE_WIN, EXIT_WOODS, ceremony, the experience light) ->
**`SetQuestAsCompleted Q_GuildTraining`**. 0 `LUA RUNTIME ERROR` lines in the whole log. Nobody at the keyboard.
Log archived by `ab_playtest.py collect v6`. Next: the same chain on v7 (the converter's Gameflow) + compare.
