# v6 playtest: the Guild handoff survives, RunTutorials dies on its first loop (2026-09-19, midday)

## What the morning's crashes were

Four New Game runs (10:39-11:13, profiles `Arse Face` .. `MeowMix`) on the v5 bundle all died in the childhood ->
Guild transition and hung `Fable.exe`. None was launched through `local_test.py`, so no log survived; the root
cause was found in the sidecar and landed there without a journal entry:

- `sidecar-abi-v2` `7ea4377` **GetAll\* fills: retail owns the vector buffer (cross-heap free crash on entering the
  Guild)** -- the `GetAllThings*` bindings freed the `std::vector<CScriptThing>` storage the retail allocator owned.
- `8ba7abb` entity `me` is a non-owning `shared_ptr` handle (shared_ptr-typed quest bindings reinterpreted the raw
  userdata).

`local-candidate-v6` (built 11:15) = the three converter units + that DLL + **Aeon's `LUAGameflow.lua` registered
as the override of the retail `Gameflow` script** (`build_unit_playtest_package.py --gameflow`). Do not also stage
`--unit gameflow` (duplicate `nativeName`, `check_override` rejects the table).

## The v6 run (11:35, `local_test.py --launch`, log captured)

Log: `local-candidate-v6/NoviCompatibility/FableScriptExtender.log`, session `runs/20260919-113501-630165/`.

- **Transition clean.** `LUAGameflow` stage 0 -> 100, `Q_GuildTraining` allocator replaced, `Main` ran,
  `GetThingWithScriptName("GuildArrivalHSP")` resolved, `CS_GUILD_ARRIVE` entered and exited (log 944-952).
- **One Lua error, and it is the whole "no quests, everyone standing around" report:**
  `!!! LUA RUNTIME ERROR in thread 'RunTutorials': GuildTraining.lua:197: attempt to perform arithmetic on a nil
  value (field '_4_4_')`. `RunTutorials` is the thread that drives every guild tutorial stage; it died on the first
  loop (`AppleMarker`), so PreMelee bound its entities and nothing else ever started.
- Player-reported, not investigated (NOVI readable Oakvale script, not the units): beating the victim before
  talking to anyone gives the teddy but the Bully interaction afterwards is wrong; the barrel man yells from across
  the map if you run after breaking the barrels.
- Custom tattoo cards visibly arrive at New Game (adult hero glimpsed, item pickups) -- see below.

## Converter fixes (all generic, Oakvale gate identical, tests 16/16)

1. **Split-array vector spelling** (`native_evidence_lowering.py` `canonicalise_split_array_vectors`, run first
   in `fold_local_thing_vectors`). Ghidra typed the local `std::vector<CScriptThing>` as one 12-byte stack array:
   `auStack_54._0_4_` begin, `._4_4_` end, `uStack_4c` capacity, the array passed bare to
   `GetAllThingsWithScriptName`. Every fold pattern expected three named slots. The canonicaliser respells it into
   that form (byte count parenthesised, the `>> 0x1f` sign-fix line dropped, `*(int *)(V._0_4_ + i)` kept in the
   elem pass's spelling). The loop now lowers to `for each marker: CreateObject(...); SetThingPersistent;
   RemoveThing`.
2. **`_align` on a vtable call with a truncated push record** (`convert_quest_unit.py`). The typed export's
   `pushedStack` for `CreateObject(&r, &name, GetPos(elem), &script)` has 3 entries (its backward scan stopped at
   the `GetPos` vcall between the pushes); `_align` accepted `lead = 2` and treated the hidden-result push as a
   register operand, shifting every operand left (def name := result slot). A vtable call is `__thiscall`:
   `lead` must be exactly 1, else no alignment. Also fixed `GetThingWithScriptName("HeroDepartureStartMarker")`
   (was `nil --[[missing]]`) in Departure.
3. **`CreateObject` position operand** (`lift_native_lua.py` manifest overlay). Only `CreateCreature` /
   `SetWanderCentrePoint` / `IsCameraPosOnScreen` had their `sol::table` position tagged `nativeKind: vector`;
   `CreateObject`'s was untagged, so `place_args` filled it from the numeric pool (the loop counter). Now every
   `Create*` spawner's `position`/`pos` is tagged. Tagging *all* position tables was tried and reverted: it let an
   unresolved stack `C3DVector` (`&xStack_40`, `EntityTeleportToPosition` in SkillTarget) leak into an emitted
   call where a `TODO` comment stood.

Result (`readable_converter/FSE/GuildTraining/GuildTraining.lua`):

```lua
appleMarker = quest:GetAllThingsWithScriptName("AppleMarker")
if #appleMarker ~= 0 then
    ...
        quest:SetThingPersistent(quest:CreateObject("OBJECT_APPLE_RED_01", appleMarker[scratchValue + 1]:GetPos(), ""), true)
        quest:SetThingPersistent(appleMarker[scratchValue + 1], true)
        quest:RemoveThing(appleMarker[scratchValue + 1], false, true)
```

and line 157 `CreateObject("PreMeleeDummy", GetThingWithScriptName("OBJECT_STRAW_DUMMY_01"):GetPos(), "PreMeleeDummyMarker")`.

Gates: Oakvale draft identical (re-run after the lifter change); smoke Guild draft 1 (BirdKiller `x_stk_60`,
pre-existing) / readable 1 (ScorpionHome ownership review, pre-existing); `test_guild_woods_melee_converter`,
`test_new_oakvale_conversion`, `test_native_position_distance` 16/16. The full suite was not run.

Observed, not mine, not fixed: this morning's regen changed `CheckFriendlyAttacks` from `xStack_a0:MsgIsHitByHero()`
to an unresolved `(**(*uStack_9c + 0x54))("SCRIPT_NAME_HERO")` (`cVar11 = nil; if cVar11 ~= 0` = always true).
That is the handoff's resume item 1 (`canonicalise_stack_objects` liveness) and it will misfire the friendly-fire
warnings in-game until fixed.

## The tattoo cards

Retail `Gameflow` stage 0 (0x00CE7670) really does `if (!IsXbox()) GiveHeroObject("OBJECT_TATTOO_CARD_*_CUSTOM_01",
-1, true)` x5 before the Oakvale intro card (the PC custom-tattoo feature); the converter and Aeon reproduce it
from the same bytes. It is invisible in retail PC (user reference: youtube IpF4n02QJhw @5:35). The one
operand-level difference: the sidecar's `GiveHeroObject` binding was 2-arg and hard-coded the third native bool
to `false`; retail passes `true` (PDB name `bUnknown`). Now `GiveHeroObject(name, amount, sol::optional<bool>)`
passing it through (`sidecar-abi-v2` `b922e6d`, patch `sidecar_patches/novi-give-hero-object.patch`; the
converter takes arity from `LuaQuestState.h`, so the converted Gameflow will emit the operand on its next regen;
Aeon's port already passes `true`). Hypothesis to test in the next run: the pickups stop showing. The adult
glimpse is stage 0 running before `Q_NewOakValeIntro` morphs the hero, a few frames later than retail's black
screen covers.

## Bundle

`local-candidate-v6` rebuilt on the regenerated Guild unit + the new DLL (5,753,344 B), preflight passed:

    python work/new-oakvale-original-fse-20260912/local-candidate-v6/local_test.py --game-dir "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters" --launch --save-dir "C:\Users\Cornelio\Documents\My Games\Fable\Saves"

Look for in the log: zero `LUA RUNTIME ERROR`; `RunTutorials` proceeding past PreMelee (apples at the markers,
the melee tutorial starting); and whether the tattoo pickups still show at New Game.

# Afternoon: the Guildmaster stall, root-caused from the bytes (runs 3-5)

Three more v6 runs (12:47, 13:0x, 13:1x). Transition, arrival cutscene, apple loop, PreMelee activation all
fine; the tattoo pickups are gone (`GiveHeroObject` third bool confirmed); barrel man fixed; bully deviation
in. The Guildmaster still stood there. `quest:Log` probes on every statement of PreMelee `Main` (bundle copy
only, manifest hash updated) showed the last line printed before `resources:TryAcquire(resource, PreMeleeMaze, 4)`
and nothing after: not a retry loop (the in-loop probe never printed), the call itself never returned.

## What `StartScriptingEntity` really does (0x89B5B0, disassembled from retail bytes)

```
thing->IsValid (slot 0x12c) else -> create null resource, return false
CThing = thing->GetPThing (slot 0x2c)
if (CThing+0x10 == 1 && current_priority(0x6d5ad0) > requested) return false      ; lower priority: refused
tc = CThing+0x20 < 0 ? Components(CThing+0x44/+0x48).LowerBound(0x1f)->second : param_2
while (byte [tc+0x18]) { if (!IsValid || DAT_013d2838[5]) return false; GSI->NewScriptFrame(); }   ; YIELDS
if (CThing && !(CThing+0x91 & 1)) { new CScriptGameResourceObjectScriptedThing(this, CThing, ScriptID, prio); *param_2 = it; return true }
```

`tc` is the thing's `CTCScriptedControl` (component 0x1f). Retail layout (the PDB's is 4 bytes off -- the list is 8
bytes in retail): `+0x0c PScriptHandleImp, +0x14 PActionList, +0x18 Locked, +0x1c ScriptAIPriority`.
`SetLocked` (0x712C20, only caller: the ScriptedThing ctor 0x9039D0, unconditional) sets `+0x18`;
`ClearLocked` (0x712C80, only caller: the ScriptedThing dtor 0x903AC0) and `InterruptControl` (0x713850) clear it.
So an equal-or-higher-priority acquire **yields the script fiber every frame until the current holder's resource
object is destroyed**, then steals; only a *lower* priority is refused. That yield happens inside a C++ binding
called from Lua, so the host never resumes the coroutine: silent, no error, game keeps running.

## Why retail does not deadlock and we did

`CNOVI`-style entity scripts release before every wait. The Maze (0x00D43DB0):

```
if (master->GuildWarningOccuring) {
    if (HasPhysicsMesh(&res)) Clear(&res);     ; bsim labels for 0xCD23B9 / 0xCD2770 = FSE InitScriptObjectHelper1/2
    while (GuildWarningOccuring) frame();      ; waits UNLOCKED
    if (HasPhysicsMesh(&res)) Clear(&res);
    while (!StartScriptingEntity(me, &res, 4)) frame();
}
```

The converter lowered 0xCD23B9 to `RESOURCE_IsAcquired`, then mapped every `RESOURCE_IsAcquired(X)` to `false`
("a freshly constructed stack resource has no handle yet") and dropped 0xCD2770 as noise -- true for the first
site, wrong for every re-check after an acquire. The Maze therefore held its lock through the warning, and
PreMelee's equal-priority acquire waited forever. The NOVI hand pipeline had this right all along
(`generate_*_resource_candidate.py` "preparation": the pair == `resources:PrepareResource`).

## Fixes

- `native_evidence_lowering.py`: `RESOURCE_RESET = {0xCD2770}` lowers to `RESOURCE_Reset`; `if (IsAcquired(R))
  Reset(R)` (assigned or inline) folds to `RESOURCE_PrepareResource(R)` -> `resources:PrepareResource(R)`; the
  IsAcquired operand regex now takes the cast in parentheses (it matched `(C3DMeshInfo *` before). A bare test with
  no reset still lowers to `false`. Guild: 13 sites in `GuildTraining.lua`, 28 in PreMelee's Guildmaster, 3 in the
  Maze; PreMelee `Main` now prepares + `TryAcquire`s all four actors as retail does.
- Sidecar (`sidecar-abi-v2` `c421e65`, patches regenerated): `TryAcquire` prepares an unprepared resource on first use
  (retail runs the helpers at every acquire site); on success it registers the handle in
  `g_controlHandlesByEntityData` (+ `g_retailResourceHandles`), unregistered by `PrepareResource` / release / destroy.
  `StartCutscene` therefore copies the held resource for `MAZE` instead of re-acquiring at VERY_HIGH -- which, by the
  bytes above, would have waited on the quest's own lock forever (second deadlock on the same path). The entity API's
  `SelectControlHandle` falls back to a registered retail-resource handle (never another VM's entity handle), so the
  Maze's `me:MoveToPosition` drives the resource it holds (retail: a method on that object). Diagnostics stay in:
  `TryAcquire enter` (thing, `tc1f`, `handleImp`, `actions`, `locked`, `prio`) and refused/granted transitions.
- Deviation to note: `LuaEntityAPI::AcquireControl` "borrows" a live registered handle instead of stealing; retail
  would wait-then-steal. Accepted for now; watch for a script that expects to evict a RetailResources holder.

Gates after the converter change: Oakvale identical; Guild draft 1 / readable 1 (baseline); Trader 2 / 2 (was 8);
Orchard draft 0 / readable 1 -- `Artefact.lua:47 nil:IsEqualTo` appeared after the Gameflow agent's 13:31 edits to
`lift_native_lua.py` / the switch files, handed to that agent to bisect. Targeted tests 16/16.

# Run 6 (13:54): Guildmaster fixed; the race apprentice parks in a second movie start

PreMelee ran end to end (intro cutscenes, Guildmaster to the dummy). Talking to `ApprenticeSpeedTest` took control
and showed no dialogue. Log: `--- Starting Movie Sequence ---` from the entity, never `--- Movie Sequence Started
Successfully ---`, no `Speak_Blocking` entry line. The Lua ran `xStack_20c = resources:StartMovie("")` (the movie
object's inlined ctor, folded) AND `quest:StartMovieSequence()` (the GSI slot 0x5c8 call the converter rebuilt
from the disassembly after Ghidra dropped it). Retail makes one call, the object is its out-parameter.

Retail `StartMovieSequence` 0x89B110 (bytes): `while (byte [GSI+0x2c]) NewScriptFrame();` then sets that byte --
it yields every frame while a sequence is already active on the interface. The second start therefore waited on
the first forever, inside the binding: same silent-park class as `StartScriptingEntity`. 98 sites across the four
units had the pair.

Fix (`native_evidence_lowering.py`, `lower_after_annotate`): a `GSI->StartMovieSequence(&name, M)` whose `M` was
folded to `RESOURCE_StartMovie` is dropped (the NOVI hand pipeline did the same by rewriting the GSI call). 98 -> 0.

A static scan of every GSI vtable target for an internal `call [vt+0x1c]` finds eight yielding natives:
`StartScriptingEntity` (0x20), `CreateCreature` (0x16c, bounded), `GiveHeroObject` (0x1e4: two frames when the
third bool is false and `DAT_013d2838` is set -- the tattoo pickup pause), `GiveHeroItemsFromContainer` (0x1f0),
`KickOffDeathScreen` (0x4d8), `KickOffCreditsScreen` (0x4dc), `StartMovieSequence` (0x5c8), `CameraDefault`
(0x680, counts `[this+0x48]` frames). Only the two condition waits can be made unbounded by our own scripts;
both are now handled. The `TryAcquire enter` snapshot's field values are garbage (wrong component-walk offsets);
the refused/granted lines are still trustworthy.

Gates after the fold: Oakvale identical; smoke Guild 1/1, Orchard 0/0, Trader 2/2, Gameflow 0/0; tests OK
(incl. `test_native_switch_tree`). v6 rebuilt.

# Run 7 (14:xx): the dummy stage -- Guildmaster `Main` died on a resource released as a movie (2026-09-19, afternoon)

Player report: Guildmaster talks, hero enters the ring, the melee dummy cannot be locked on, no `?/7` tally. Log
`local-candidate-v6/NoviCompatibility/FableScriptExtender.log` (747 lines, read while the game ran):

```
637  !!! LUA RUNTIME ERROR in Main() of 'GuildTrainingPreMelee/Entities/PreMeleeWhisper':
     ...PreMeleeWhisper.lua:75: attempt to call a nil value (global 'ABS')
689  !!! LUA RUNTIME ERROR in Main() of 'GuildTrainingPreMelee/Entities/TheRealGuildmaster': Invalid or released retail resource
692      [C]: in method 'DestroyMovie'  ...TheRealGuildmaster.lua:107
```

## Root cause (bytes)

`TheRealGuildmaster.lua:107` was `resources:DestroyMovie(movie2)` on the object created by
`movie2 = resources:NewResource(); TryAcquire(movie2, hero, 4)` -- the hero's controlled-entity resource for the
`CS_GUILD_PREMELEE_PUNCH` cutscene. Retail (0x00D52E90, site `0x00D533BF`) calls `0x007E74D0`
(`CScriptGameResourceObjectScriptedThingBase` dtor, `ecxStack -332` = the resource at `-0x14c`), not the movie dtor
`0x006E7B80`. The sidecar refuses `DestroyMovie` on a resource handle, the entity `Main` unwound, and everything after
line 107 never ran: `SetStateInt("PreMeleeMode", 1)`, `SetStateInt("DummyHits", 0)`,
`AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)` / `DisplayQuestInfo(true)` (the tally) -- and
`PreMeleeDummy.Main` (0x00D52xxx) waits `while GetStateInt("PreMeleeMode") ~= 1` before its
`EntitySetTargetable(me, true)`, so the dummy stayed untargetable. The `EntitySetTargetable` / `SetIsPushableByHero`
operands and the `AddQuestInfoCounter` / `UpdateQuestInfoCounter` lowering are correct (checked against the export:
GSI 0x51c `(&key, 7, 1.0)`, 0x53c `(counter, DummyHits, -1)`); the PC (`!IsXbox`) path carries its own copy of the
tally block (`goto LAB_00d536c0` into the Xbox branch, copied by `native_goto_scopes`), so nothing else was in the way.

Why the converter picked the wrong destructor: both dtors print under ONE bsim label
(`CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase`, 11 + 13 sites in this function);
`disambiguate_call_labels` renames the k-th printed occurrence by its export target -- but Ghidra wraps the label at
its `::` in deeply indented code (`CScriptGameResourceObjectMovieBase::` newline `~CScriptGameResourceObjectMovieBase`),
so it counted 11 printed vs 24 sites and gave up; `fold_resource_objects` then keyed the label on the last target and
`reconcile_destructor_kinds` could not settle it either because the SAME stack slot (`-0x14c`) hosts a resource in one
scope and a movie in another (`xStack_14c = NewResource()` at 0x00D53346, `xStack_14c = StartMovie("")` later).
1,954 such wrapped labels across 82 Guild functions (243 Orchard, 397 Trader, 602 Gameflow). 20 mis-kinded dtor
sites in the Guild drafts before; 0 after (the 5 the text-order check still flags in Will's Guildmaster are correct
per the export: `0x00D61331` -> `0x007E74D0` on `-0x1f0`; the text order there is a moved `goto` region).

## The Whisper `ABS`

`PreMeleeWhisper` 0x00D524A0 compares the chat marker's height with the hero's:

```
00d527ca  fld  dword ptr [ebx + 8]      ; marker pos.z
00d527cd  fsub dword ptr [eax + 8]      ; hero pos.z
00d527de  fstp dword ptr [esp + 0x28]   ; depth 184 -> entry slot -0x90
...
00d5282c  fld  dword ptr [esp + 0x24]   ; depth 180 -> -0x90
00d52830  fabs
00d52832  fcomp dword ptr [0x122ded8]   ; 1.0
00d52838  fnstsw ax
00d5283a  test ah, 0x41                 ; C0 | C3
00d5283d  jp   0xd52d56                 ; neither: |dz| > 1.0 -> skip the chat
```

Ghidra: `(ABS(fStack_94) < 1.0 == (ABS(fStack_94) == 1.0))`. Three converter defects on one line: (1) `ABS(` was
emitted verbatim (no such Lua global); (2) `canonicalise_stack_objects` folded `fStack_94` onto the resource object
`xStack_a0` (16-byte extent `-0xa0..-0x91` in the restored true-slot numbering, but Ghidra's `fStack_94` is 4 bytes
adrift: its true slot is `-0x90`, outside) -- the height became the resource handle; (3) `rename_scalar_stack_locals`
took `switch(fStack_94)` / `ABS(fStack_94)` for call arguments (hidden-return slots) and kept the Ghidra name the
lifter refuses. The position members `puVar8[2]` / `*(float *)(pCVar6 + 0x8)` were a `TODO` because the GetPos
fold only ran in `lower()` (before the annotation pass names the vtable-slot spelling).

## Generic changes

- `convert_quest_unit.py` `unwrap_statements`: a line ending in `::` joins the next line (one token).
- `native_evidence_lowering.py`:
  - `canonicalise_stack_objects`: float-typed locals (`float x;` / `fStack_*`) are never members of a resource /
    movie / map / thing object (PDB: no float members) and keep their name.
  - `rename_scalar_stack_locals`: `switch( if( while( ABS(` are not calls taking a slot's address.
  - `fold_position_reads` (was inline in `lower`, now also at the end of `lower_after_annotate`): any pointer cast
    on `GetPos`, `*(float *)(v + 4|8)` -> `.y/.z`, bare `&DAT_0143e8e0` -> zero vector; a member-wise copy
    `S._0_4_ = V.x; S._4_4_ = V.y; S._8_4_ = V.z` into the 12-byte slot the next call passes as
    `(C3DVector *)xStack_<same slot>` -> `xStack_<slot> = ENGINE_VectorCopy(V)` -> `{x = V.x, y = V.y, z = V.z}`
    (the three `CreateEffect(.., "SMASH_DUMMY_01", pos, ..)` sites now pass the dummy's position instead of an
    unassigned slot -- the "converter gap" the DLL logged).
  - x87 idiom: `(a < b) == (a == b)` -> `b < a` (the negation of the existing `!=` -> `<=` rule); `finish_lua`:
    `ABS(` -> `math.abs(`. Whisper now reads `1.0 < math.abs(getPos.z - hero:GetPos().z)`.
  - `KEYED_LOGBOOK` += `0xCBEA81: AddLogbookTutorialEntryPC` (below); `lower_after_annotate` also drops a
    `GSI->StartMovieSequence();` printed WITHOUT operands right after the movie ctor (TraderToRescue 0x00DFE0F0's
    untyped decompile: 3 remaining `quest:StartMovieSequence()` sites -> 0).
  - the `CCountedPointer::operator=(&P, &thing.Data)` handle rule: when `P` is Ghidra's own name for the Data field
    of a stack `CScriptThing` that is a call operand (`piStack_14` beside `(CScriptThing *)xStack_18`), `P` is
    `thing._4_4_` -- ScorpionHome 0x00D643A0 now spawns the beetle at `r1:GetPos()` (the furthest
    `ScorpionSpawn`) instead of `nil --[[missing]]`, and `r1 = pCVar8` is the thing copy.
- `annotate_interface_slots.py` `_annotate_things`: a stack slot copied (once) from a variable whose EVERY definition
  is a thing-valued call (`piStack_14 = pCVar8` at the loop tail) is a thing receiver before the store in text
  order too (a first, wider version keyed on any thing definition of the source broke the Oakvale gate: NOVI_Bully's
  `pCVar6` is `me` on one path and a call result on another).
- `convert_quest_unit.py` `_align` + `_receiver_printed`: a `__thiscall` vtable site whose push record is exactly
  one longer than the printed arguments AND whose first printed argument is the receiver (the head's base
  expression, or a loaded-vtable register alias `iVar1 = *this_00`) drops the EARLIEST push: VC7.1 pushes a literal
  operand of the NEXT call before making the calls whose results it also pushes (Departure `Init` 0x00D506B0:
  `push ebx` = `EntityTeleportToThing`'s bool, then `GetThingWithScriptName` / `GetHero`; record `[0, -16, -12]` for
  two real pushes), and the old `lead = 0` alignment shifted every operand one to the left (the name slot took the
  result slot: `GetThingWithScriptName(nil --[[missing]])`). ~190 GSI sites across the four exports had that shape
  (58 `piVarN` receivers, 36 + 20 `*(int **)(this + 4|0x40)`, ...). Guild `nil --[[missing]]`: 3 -> 0.
- `lift_native_lua.py`: `ENGINE_VectorCopy` / `ENGINE_ZeroVector` results have kind `vector`;
  `CALLEE_ALIASES[0xCBEA81]`.
- `convert_quest_unit.py` `sidecar_bindings`: parses the patch's added lines with the `+` stripped (a lambda head
  wrapped over two lines -- `CreateEffect` -- was invisible); a `sol::object` parameter is typed as a thing only when
  named like one (`THING_OBJECT_NAMES`); a binding whose first Lua operand is the native hidden-result slot
  (`result`, i.e. CreateEffect) is left out of the lifter manifest on purpose -- with it in, the lifter's
  hidden-result placement shuffled the operands (`CreateEffect(r1, "SMASH_DUMMY_01", 0.0, false, false)`); without
  it the call is emitted positionally from the typed export, which is right.

## Sidecar: `AddLogbookTutorialEntryPC` (0x00CBEA81)

The PC branch of the Guildmaster (`TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC`) calls `0x00CBEA81` for
`TEXT_QST_LOG_COMBAT_LOCKINGON` / `_PUNCHING` (the Xbox branch calls `0x00CBE9EE`); it was a `TODO` in the readable
output (`CSubtitleRenderer::SetText__atcbea81`). Bytes: same shape as 0xCBE9EE -- `__thiscall(const CCharString*)`,
title `<key>` + `"_TITLE"` (0x122e240), body `<key>` + `"_PC"` (0x122e248, where 0xCBE9EE passes the bare key),
`push 2` category, GSI `+0x4d0` `AddLogBookEntry`, then `call [gsi+0x1c]` (one-frame yield, bounded). Binding
`quest:AddLogbookTutorialEntryPC(key)` in `NoviUnitBindings.h` (`sidecar-abi-v2` `9ebaa27`; DLL 5,769,216 B, sha256
`ce48a9a4...`; `novi-unit-bindings.patch` regenerated over the same four files as before). The converter takes the
signature from the patch, so the PC path now emits `quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_LOCKINGON")`
(11 sites across the Guild units).

## Sweep of the other defect classes (all four units, fresh drafts)

`quest:StartMovieSequence()` beside `resources:StartMovie`: Guild 0, Orchard 0, Trader 3 -> 0, Gameflow 0.
`RESOURCE_IsAcquired` left unfolded: 1 (TraderToRescue 0x00DFE0F0, the resource slot mis-folded as a colour literal
`{R = 255, G = 0, B = 0, A = 255}` in an untyped decompile -- not on the guild path, left). `&`-prefixed operands in
emitted calls: 0 (13 Guild / 1 Trader occurrences are all inside `TODO(native)` comments). `nil --[[missing]]`:
Guild 3 -> 0, Trader 25 (untyped TraderToRescue / GiveThingBestEnemyTarget, not on the guild path), Orchard 0,
Gameflow 0.

## Gates

Oakvale draft identical (`diff -r -q` clean). Units regenerated (todo: Guild 756 -> 696, Trader 159 -> 144, Orchard
29, Gameflow 5); `shippedAsDraft` empty in all four readable summaries. Smoke: Guild draft 1 (BirdKiller `x_stk_60`, pre-existing) / readable 1 (ScorpionHome ownership
review, pre-existing); Orchard 0/0; Trader 2/2; Gameflow 0/0 -- baseline. Targeted tests 20/20. Full suite (alone, the four
stale fixture files excluded): 1598 tests, OK, 25 min.

Not done: the bundle (`local-candidate-v6`) still carries the old Lua + DLL; rebuild it (`build_novi_compat_bundle.py`,
then the unit package) before the next run. Expect: no `ABS` error, the Guildmaster proceeding past the punch
cutscene, `PreMeleeMode` = 1, the tally on screen, the dummy targetable, `SMASH_DUMMY_01` at the dummy when it is
destroyed, the PC logbook entries.

# Evening: the dropped cross-branch `goto` (the missing `?/7` counter), fixed generically

Run 7's other finding, proven in-game: after the STICK cutscene the Guildmaster's `Main` returned and the
`?/7` tally never appeared. Retail 0x00D52E90 keeps the stick-counter block (`PreMeleeMode = 2`,
`AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)`, the DummyHits loop) INSIDE the Xbox half of the
tutorial (`if (IsXbox()) { ... if (!terminating) { LAB_00d53c7e: ... } } else { PC half; goto LAB_00d53c7e; }`,
typed export + the decompile at lines 300-317 / 1115-1131 of the pre-goto statement dump). The lifter cannot
jump into a sibling's nested block, wrote `-- TODO(native): goto LAB_00d53c7e_c26`, and the PC script fell off
the end of the else branch.

## Census of the 120 residues (readable stage: Guild 103, Trader 17), by shape

Instrumented the lifter (`duplicate_sibling_tails` input/output per function, four units). At the C statement
level the Guild unit alone had **387** jumps whose label was not in an enclosing scope, before any rewrite:

| shape | count | what the previous pass did |
|---|---:|---|
| label path from the common ancestor is if/else blocks only | 379 (98%) | copied the tail (`duplicate_sibling_tails`), copies of copies: PreMelee `Main` 1,168 -> 4,338 statements, 49 jumps still dropped |
| a `while` / `do` on the label path (jump into a loop body) | 8 | nothing |
| direction | 192 forward / 195 backward | |
| label depth below the common ancestor | 1: 303, 2: 30, 3: 39, 4+: 15 | |
| jump depth below the common ancestor | 0: 39, 1: 136, 2: 79, 3+: 133 | |

Two sub-shapes inside the 379, by what the label's region does:
* **a shared continuation** (the rest of the stage: the Xbox/PC halves of every tutorial, the then/else halves of
  a check, a switch-chain case join): the region is not straight-line; only a move or a copy expresses it.
* **a cleanup ladder** (`LAB_a: release(A); goto LAB_b;` -> `LAB_b: release(B); return;`), the VC7.1 epilogue
  chain: straight-line, but its terminal is itself a jump the lifter could not make, so
  `native_cleanup_regions` never recognised the region (Melee's Guildmaster: 23 of its 24 residues).

## Transformation per shape (all generic; nothing hand-edited)

1. **Hoist the shared tail** (`native_goto_scopes.hoist_shared_tails`, new; runs in `Lifter.lift` between
   `merge_equivalent_regions` and `duplicate_sibling_tails`, unit-converter path only, and once more after the
   copies). For a jump whose label sits under a chain of if/else blocks the jump site is outside of, the tail --
   from the label to the end of each enclosing if block on the way out, skipping the else chains that
   fall-through skips -- is MOVED right after the outermost block's if/else chain; the label site becomes
   `goto L`, later segments `goto FLOW_hoist_l_k`, and `goto FLOW_past_l` / `FLOW_past_l:` guard the
   fall-through. The tail stays inside the same loops, so `break`/`continue` keep their meaning and every
   internal label keeps its name. A region that is a straight-line cleanup (`_is_epilogue`: to `return;` or to
   another such region; falling off the function does not count -- `native_cleanup_regions` refuses those) is
   left alone, so the readable `__cleanup_X(); return` style survives. Each hoist must strictly reduce the
   count of unexpressible jumps (re-parsed), else it is undone; the pass repeats to a fixpoint. Loops on the
   path are refused (the 8 sites; all of them turned out to be epilogues the next fix handles).
2. **Cleanup ladders** (`native_cleanup_regions._walk`): a region may end in `-- TODO(native): goto X`; it then
   delegates to X's region and resolves to `return` when X does. A region ending in a real `goto Y` is only
   reproduced at a site from which `::Y::` is visible (`_scopes`); empty-body goto regions are registered.
3. **`duplicate_sibling_tails`**: a copy that renamed a jump (`goto LAB_x_cN`) but terminated before copying
   the label is rejected unless the original label is visible from the copy site (MeleeApprentice's two
   `LAB_00d41a07_c14/_c15`, PreMeleeMaze's `LAB_00d44494_c2` -- pre-existing broken copies).
4. Two readable-stage bugs the new shapes exposed: `fold_goto_else` took the `end` of a nested if at the same
   indent for the branch's end and moved a `continue_N` label into the wrong loop (Skill: `no visible label`);
   `strip_redundant_parens` turned `(nil):IsAlive()` (an inlined `x = nil` staging for the experience-orb thing
   whose `CCountedPointer` assignment is still a TODO) into the syntax error `nil:IsAlive()`.

## Numbers

| | before | after |
|---|---:|---:|
| `TODO(native): goto`, readable: Guild / Trader / Orchard / Gameflow | 103 / 17 / 0 / 0 | **0** / 1 / 0 / 0 |
| draft: Guild / Trader | 104 / 19 | **0** / 1 |
| `report_goto_residue.py` total | 120 | **1** |
| PreMelee Guildmaster readable lines | 2,195 | **649** |
| draft `todo` (Guild CONVERSION_REPORT) | 696 | 415 |
| readable syntax | ok | ok (`syntaxOk: true`, `shippedAsDraft: {}` in all four) |

The one left: `TraderConflictEvil.Main` (untyped decompile, the file with 25 `nil --[[missing]]`, not on the
guild path) -- `LAB_00df77e2: bVar13 = xStack_18 == nil` (a refcount-release idiom) whose region ends in
`goto LAB_00df7957`, a `return` label inside a nested if that the jump site cannot see. Needs the same
emitted-tail resolution `native_cleanup_regions` applies to `FLOW_` labels; left.

PreMelee's stick section now reads (readable_converter): the Xbox and PC halves each end in `goto LAB_00d53c7e`
inside their `if not IsActiveThreadTerminating()` guard, `goto FLOW_past_lab_00d53c7e` skips the tail when
neither reached it, then `::LAB_00d53c7e::` `SetStateInt("PreMeleeMode", 2)` ... `AddQuestInfoCounter(...)` and
the DummyHits loop -- the block retail runs. Style residue that is not a converter gap: the `goto FLOW_past` +
`::LAB::` pair where an early-return inversion of the terminating checks would let the tail follow the chain
without a jump (the switch-tree flattener's `trailingIfsInverted` idea, not applied here).

## Gates

* Oakvale: `convert_new_oakvale.py --out <scratch>` byte-identical to `refs/script_recovery/lifted/NewOakValeIntro`
  (checked twice: after the lifter/goto change and after the cleanup-region change).
* Units regenerated (`convert_quest_unit.py` + `build_readable_unit.py`, Guild into `readable_converter`).
  Smoke: Guild draft 1 (BirdKiller `x_stk_60`, pre-existing) / readable_converter 1 (the same BirdKiller line,
  `scratchValue31`); Orchard 0 / 0; Trader 2 / 2 (the two FREE GLOBALS files, baseline); Gameflow 0 / 0.
* `test_cross_branch_goto.py` (new, 4 tests): PreMelee Guildmaster `Main` (draft AND readable) through a mock
  quest with `IsXbox() == false` and `DummyHits` held at 7: `SetStateInt("PreMeleeMode", 1)` then `2`, two
  `AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)`, the PC logbook entry before the stick stage;
  a synthetic hoist (move, not copy); loops and epilogues left alone; zero residue in the Guild unit.
* Targeted: `test_guild_woods_melee_converter test_new_oakvale_conversion test_native_position_distance
  test_native_switch_tree test_cross_branch_goto test_native_cleanup_regions test_native_goto_scopes
  test_readable_style`: 49 tests OK. Full suite (alone, the four stale fixtures excluded): **1602 tests, OK, 29 min**.

Not done: bundle not rebuilt (`local-candidate-v6` still carries the old Lua; its copies also carry `PMDIAG2`
probes), nothing launched, nothing committed. Expect in the next run: the `?/7` tally after the STICK cutscene,
`PreMeleeMode` 2, and the whole PreMelee -> Melee handoff without the Guildmaster returning early.

# Night: the woods loop that never ended (the YES/NO answer wrote to a phantom slot), 2026-09-19

In-game (run 19:56, `work/ab_runs/v6-20260919-195605/FableScriptExtender.log`): beetles killed,
`SetQuestAsCompleted Q_GuildTrainingWoodsMelee` (line 947), the hero walked back into the Guild, and the Guildmaster
was standing at `M_MeleeTeacherStand` again; talking to him replayed `CS_GUILD_PREMELEE_PUNCH` (`PREMELEE_PUNCH_10`
at line 1245, first at 692). Two separate facts hide in that log, and only one of them is a converter defect.

## 1. The native state machine of PreMelee's `TheRealGuildmaster::Main` (0x00D52E90, 11,869 bytes)

There is no state-driven outer loop. `Main` is one straight run of stages, every wait loop bailing to the cleanup
ladder (`LAB_00d55c2b` ...) when `IsActiveThreadTerminating` (0xF35B30, the `TrollWhackGroundBase::Initialise` bsim
mislabel) flips:

| stage | selected by | evidence |
|---|---|---|
| wait `GuildmasterTeleport` (quest +0x54), `SetQuestCardObjective(OBJECTIVE_01)`, `TryAcquire(me,4)`, teleport to `M_MeleeTeacherStand`, talk loop until `IsTalkedToByHero` | nothing else: unconditional | decompile lines 98-292 |
| PUNCH (`CS_GUILD_PREMELEE_PUNCH`, `PreMeleeMode = 1`, `DummyHits` loop to 7) | falls through from the talk loop | 362-520 |
| STICK (`CS_GUILD_PREMELEE_STICK`, `PreMeleeMode = 2`, second `DummyHits` loop) | falls through | 554-760 |
| XP orb (`CS_GUILD_PREMELEE_PASSED_SETUP/PASSED`, orb loop) | falls through | 784-1000 |
| ALARM (`CS_GUILD_PREMELEE_ALARM`), `GiveHeroQuestCardDirectly(KILL_BEETLES)`, activates `Q_GuildTrainingWoodsMelee`, green marker, walk to `MK_GTM_WD_GUARD` | falls through | 1000-1265 |
| **woods loop** `do { ... } while (cStack_169 != 0)` | see below | 1265-2419 |
| after the loop: `HeroSleeps = 1` (quest +0x49), `FadeScreenOut(0.5,0.5)`, `SetTimeOfDay(11)`, `ChangeHeroHealthBy(1000)`, `ResetPlayerCreatureCombatMultiplier` | loop exit | 2420-2440 |

The quest's own `Main` (0x00D51520) binds the three entities, runs the intro cutscenes, sets +0x4a and +0x54
(`GuildmasterTeleport`), then waits on +0x49 (`HeroSleeps`) and ends the quest through slot 0x464 with
`GetActiveQuestName()`. `Init` (0x00D51ED0) is `this+0x1c = this+0x1d = 0`; the entity has no `OnPersist` (vtable
0x012CF838 slot 4 = the 3-byte default 0xCDEBC0).

The woods loop, one iteration per frame (`cStack_169` = keep-looping, set 1 at the head 0x00D54956; `cStack_161` =
walk-finished, set 0 at 0x00D549DB; `this+0x1d` = end-cutscene played):

1. `if (!master.ScorpionsDestroyedCutscenePlayed) goto LAB_00d54f9c` (master +0x9d, PDB `CQ_SunnyvaleMasterData`
   157; the WoodsMelee quest sets it in `WatchForTermination` after `MissionSucceeded`).
2. flag set and `this+0x1d == 0`: `this+0x1d = 1`; `MiniMapRemoveMarker(TheRealGuildmaster)`;
   `MiniMapAddMarker(TheRealGuildmaster, "HUD_ORB_QUEST_CORE")` (the green `HUD_ORB_GREEN_SMALL` from the alarm stage
   is replaced); acquire the hero; actor map `HERO` + `GUARD` (= me); `StartMovie`; `PauseAllNonScriptedEntities(true)`;
   `FixMovieSequenceCamera(true)`; `CS_GUILD_MELEE_WOODSWON`; `GiveHeroYesNoQuestion(PREMELEE_END_QUESTION)`; wait.
   * **YES** (`MsgIsQuestionAnsweredYesOrNo == 1`): `FadeScreenOut(0.5, 0.5, black)`, `Pause(1.0)`,
     `PlayAVIMovie("Data\Video\2_guild_split_1_comp.xmv")`, **`cStack_169 = 0`** -- `mov byte ptr [esp+0x33], 0` at
     0x00D54DF0 with ESP = entry-412 (the export's `depth` at the neighbouring sites 0x00D54DEB/0x00D54DFE is 412), so
     the byte is -0x169: the loop flag. The loop exits, `HeroSleeps` ends the quest.
   * **NO**: if `GetHealth(me) > 0` `Speak(TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO)` and wait;
     `MoveToPosition(MK_GTM_WD_GUARD, 1.0, WALK, false, true)`; **`cStack_161 = 0`** (`mov byte ptr [esp+0x3b], 0` at
     0x00D54F17, -0x161). He walks back to the woods-door marker and the loop keeps running.
3. flag set, `this+0x1d` already 1 and `IsQuestActive(Q_GuildTrainingWoodsMelee)` (the hero came back while the woods
   quest is still winding down): wait until it deactivates, acquire me, `StartMovie`, `Speak(PREMELEE_END)` if alive,
   the same question with the same two outcomes (0x00D55348 / 0x00D55465).
4. `LAB_00d55480`: `IsTalkedToByHero` -> `StartMovie`, `PauseAll(true)`, `ClearCommands`; flag set ->
   `Speak(PREMELEE_END)` + the question a third time (0x00D55838 / 0x00D55955); flag clear ->
   `Speak(PRE_MELEE_BEETLES_NOT_DEAD)`.
5. `if (cStack_169)`: the idle comments (hero within 5.5, timer expired): `END_BEETLES_COMMENT_FIRST` when the flag
   is set, else `END_NO_BEETLES_COMMENT_FIRST/SECOND` alternating on `CStack_154`.
6. `if (cStack_161 == 0 && !IsPerformingScriptTask(me))`: `cStack_161 = 1`, `EntitySetFacingAngleTowardsThing(me, hero)`
   (0x00D55B9C: `[esp+0x3f], 1` under one push = -0x161).
7. `} while (cStack_169 != 0)` (0x00D55BB5 `mov al, [esp+0x33]`; `jne 0xd549e0`).

## 2. The converter defect: both flag stores were dropped

Ghidra's decompiler lost the argument purge of the untyped vtable call `(**(code **)(*piVar1 + 0x5ec))()`
(`PauseAllNonScriptedEntities(true)`, 0x00D54C30 `push 1`; the register copy `piVar1` had no type), so from
0x00D54C49 to the next merge its ESP model sat 4 bytes low. Temporaries constructed in that stretch are named 4 bytes
off (`CStack_c8` for the `CS_GUILD_MELEE_WOODSWON` literal the export places at -196 = `CStack_c4`), and the two
byte stores were printed against phantom slots: `uStack_170 = uStack_170 & 0xffffff;` (-0x16d) for the YES
`cStack_169 = 0`, `CStack_168._3_1_ = 0;` (-0x165) for the NO `cStack_161 = 0` -- three copies each. Nothing reads
those phantoms, the lifter dropped them, `cStack_169` was never written inside the loop and the readable output was
`repeat ... until false` (line 584 of the previous `TheRealGuildmaster.lua`): YES played the AVI and looped
forever, `HeroSleeps` was unreachable, the quest could never end; NO never re-armed the facing-the-hero step.

`restore_stack_operands` (convert_quest_unit.py) already re-slots *call operands* from the export's exact stack
model (`ecxStack`/`pushedStack`). The fix extends it to the byte slices:

* `_drifted_byte_slices` (new): for every `X = X & 0xffffff;`, `X = CONCAT13(v, X);`, `X._N_1_ = v;`,
  `X = (T)((uint)Y & 0xffffff);` with `Y = X;` above (Ghidra's clear-through-a-copy), and the reads `X._N_1_` /
  `(char)((uint)X >> 0x18)`, the drift is taken from the nearest temporary constructed before it (export slot minus
  Ghidra's name) and the slice's true byte is `named + N + drift`. Only when a declared one-byte local
  (`char`/`bool`/`undefined1 xStack_...`) sits exactly there is the spelling rewritten to that local; the pairing then
  re-runs on the corrected text. Guild: 6 stores in PreMelee's Guildmaster; in Will's Guildmaster (0x00D5E0C0) the
  health flag `CStack_234._3_1_ = 1` is `cStack_22d` (0x00D5FD05 `[esp+0x27], 1` under depth 596 = -0x22d, the loop
  flag's slot reused by VC7.1 for a dead-range bool) -- two `TODO(native)` gone and `if 1 ~= 0` became the real test.
* `fold_byte_split_pointers` (native_evidence_lowering.py) claimed every `CONCAT13(u3,CONCAT12(u2,CONCAT11(u1,u0)))`
  after a pointer split for that pointer, including the ones whose four bytes had since been re-keyed to literals
  (`uVar18 = 0; ... CONCAT13(...)` = a by-value `false`): Will's `DeactivateQuestLater(name, 1)` was the native 0,
  PreMelee's `Speak(..., resource ~= 0)` / `MoveToPosition(..., resource2 ~= 0, true)` were `false` / `true`. The
  split now yields to `fold_byte_literal_words`, which also takes the unwrap's `CONCAT11( u,u)` spelling.

Regenerated PreMelee Guildmaster (readable_converter): `scratchValue7 = 1` at the loop head, `scratchValue7 = 0`
after each `PlayAVIMovie`, `scratchValue6 = 0` after each walk back, `until scratchValue7 == 0`, then
`SetStateBool("HeroSleeps", true)`. Controlled diff (the same converter with both changes disabled, regenerated to
scratch): only the two Guildmaster files differ; Orchard / Trader / Gameflow drafts are byte-identical; the
Oakvale gate is byte-identical.

## 3. The replay itself is a re-run of `Main` from the top -- retail engine behaviour, not a branch selection

The log shows the mechanism: the moment the hero entered the woods every Guild entity got `OnPersist` (lines
826-843) and its `Main` was unwound (`[Terminating] NewScriptFrame(me)` ... `EXITED LUA CALL for
TheRealGuildmaster`, 909-916); on the way back the retail allocator was called again for each of them
(`[EntityAllocator<43>] Script: GuildTrainingPreMelee/Entities/TheRealGuildmaster`, 1019) and `Main` re-entered
(1198), which -- with `GuildmasterTeleport` already true -- teleports him to `M_MeleeTeacherStand`, waits to be talked
to and plays PUNCH. Retail bytes for that lifecycle: `CScriptBase::OnDeactivateEntity` 0x00CB88B0 looks the entity up
by UID in the map at +8, sets the thread's byte +5 (the flag 0xF35B30 reads) and saves persist info through
0x00CB83D0 (calls the entity vtable slot 4 `OnPersist` into a `CPersistContext`, stores it if non-empty);
`OnActivateEntity` -> the binding's allocator -> `CScriptBase::LoadOrCreatePersistInfo` 0x00CB7EE0 restores it -> `Main`
from the top. This entity persists nothing and its `Main` has no guard, so the re-run reaches PUNCH by construction --
the converter output is faithful to the bytes here. **What retail does on that return is not established by this
session** (no launch): the native `Main` (0x00D52E90) has no state that survives the unload, so either retail keeps
thread #1 alive across the woods trip (HeroGuildComplex not unloaded, the flag then fires WOODSWON in the Guild on
return with the QUEST_CORE marker on him), or the fresh `Main`'s `TryAcquire(me, 4)` (`StartScriptingEntity`
0x89B5B0 yields while the `CTCScriptedControl` is `Locked`) parks on a lock the old resource left on a persistent
`CThing`. Both are testable in the next run with the corrected Lua: answer YES in whichever place the question
appears -> the AVI, `HeroSleeps`, PreMelee ends before any replay; answer NO -> the walk back to `MK_GTM_WD_GUARD`
and a re-ask on the next talk. The 19:22 run's question at the woods door before any beetle was the
`GetMasterGameState` boolean/`== 0` lowering fixed earlier tonight (GOTCHAS 2026-09-19), not this.

## Tests and gates

* `test_cross_branch_goto.py`: `WoodsStageTests` drive draft + readable_converter `Main` past the woods
  (`ScorpionsDestroyedCutscenePlayed` true, `IsQuestActive(Q_GuildTrainingWoodsMelee)` false, `DummyHits` 7):
  YES -> one PUNCH, WOODSWON, the markers `HUD_ORB_GREEN_SMALL` then `HUD_ORB_QUEST_CORE`, `PlayAVIMovie` before
  `HeroSleeps`, no walk after the AVI; NO -> no AVI, no `HeroSleeps`, `PREMELEE_END_NO` then
  `MoveToPosition(pos, 1.0, WALK, false, true)`, a second question after the next talk; NO then YES -> one AVI and
  `HeroSleeps`. `DriftedByteSliceTests` pins the re-slotting on a synthetic frame. 8 tests OK.
* Oakvale byte-identical; Guild / Orchard / Trader / Gameflow regenerated (draft + readable), `syntaxOk`,
  `shippedAsDraft: {}`; smoke Guild 1/1 (BirdKiller, baseline), Orchard 0/0, Trader 2/2, Gameflow 0/0;
  `TODO(native): goto` 1 (TraderConflictEvil); targeted set (`test_cross_branch_goto
  test_guild_woods_melee_converter test_new_oakvale_conversion test_native_switch_tree test_maze_converter`) 35 OK.
* Not done: no bundle rebuilt, nothing launched, nothing committed. Known residue: `PlayAVIMovie("Data\\\\Video\\\\...")`
  doubles the backslashes (the Oakvale intro has the same and its AVI plays; Win32 collapses them) -- a lifter
  string-escape quirk gated behind the Oakvale byte-identity, left alone.
