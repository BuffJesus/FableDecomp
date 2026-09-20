# 2026-09-20 — the crash on the return from Guild Woods (one sidecar bug, one converter bug), v6/v7 rebuilt

Morning. Aeon's Discord thread was answered first (his WoodsMelee crash = `GetPos` in `Init()` before the
entities exist; ours had been the `nil & 2` EH-flag lift — different bugs at the same seam; retail
`Q_GuildTrainingWoodsMelee::Main` 0xD66620 touches no entity before its `IsLevelLoaded("GuildWoods")` gate,
so "nothing in Init" is exactly what the bytes do). Then v6 and v7 were rebuilt on the 20:42 regenerated units
(the bundles still carried the pre-woods-loop-fix Lua) and v6 launched through `ab_playtest.py`.

## The run (`work/ab_runs/v6-20260920-080006`, 08:00-08:05, 1250 lines)

Beetles killed, `SetQuestAsCompleted Q_GuildTrainingWoodsMelee` (line 958), back into the Guild, every Guild
entity's `Main` re-entered from the top (1182-1219; the retail lifecycle predicted last night), the Guildmaster
teleported to `M_MeleeTeacherStand` again, the hero talked to him (`PREMELEE_COMMENT_FIRST` + the wave), and
**Fable.exe died at the first cutscene command of PUNCH**: the log ends at

    [CutsceneCommandDiag] macro=<none> tick=836500 command=TEACHER.LookAtNothing TRUE
        [QuestThreadFlag] GuildTraining/GuildTraining thread=0 terminating=-1 at TEACHER.LookAtNothing TRUE

No WER record (no `Fable.exe.*.dmp` for today; the sidecar installs no exception filter) — the log is the
evidence, and it is enough.

## 1. Sidecar: `LogQuestThreadFlags` read a freed quest host (the crash)

Yesterday's log at the same command printed THREE `[QuestThreadFlag]` lines — an empty-name entry with
`thread=0`, GuildTraining, GuildTrainingPreMelee — and continued into PUNCH_10. Today: one line, then death.
Twenty lines earlier, both days: `[LuaQuestHost::Destructor] Tearing down quest 'GuildTrainingWoodsMelee'
(bDelete=1)` (`DeactivateQuestLater` → `operator delete(this)`).

`LogQuestThreadFlags` (sidecar `b2b4697`, run on every cutscene command) iterates `m_entityScriptDataMap`
and dereferences `host->m_pParentHost->base + 0x2c` and `GetScriptName()`. ScorpionHome's entity host
(`[EntityAllocator<51>]`, line 929) was registered with the WoodsMelee host as its parent, and the whole run
has **zero** `Unregistered entity script data` lines — the engine frees entity hosts through their own refcount
block later, not with the quest. `~LuaQuestHost` detaches its entity hosts (`UnregisterQuestEntityScripts`)
only inside the `NewOakValeIntro` lifetime branch; every retail-override quest skipped it, so the ScorpionHome
entry kept a dangling parent. Yesterday the freed block still read as an empty name / null thread (the
empty-name line); today the block had been reused → access violation.

Fix: `sidecar-abi-v2` `e1ed740` — detach for every lifetime, before the VM is closed. DLL rebuilt (08:11,
`build-detach.log`, 0 errors), `tools/script_recovery/sidecar_patches/novi-unit-bindings.patch` regenerated
as a pure `git diff 6e19dfd HEAD` over its file set (only this hunk changed).

## 2. Converter: `CheckFriendlyAttacks` (0x00D45060) died on every return to the Guild

Same log, line 1179 — the only Lua error of the run, at the moment of re-entering HeroGuildComplex:

    LUA RUNTIME ERROR in thread 'CheckFriendlyAttacks': GuildTraining.lua:645: attempt to perform arithmetic on a
    sol.sol::d::u<CScriptThing> value (local 'preMeleeMaze')

The readable had `local scratchValue = preMeleeMaze - creatures2 >> 31` after the second
`GetAllCreaturesExcludingHero()` fill, and — worse but silent — four `if nil == "CREATURE_..."` compares in the
first loop, which made it set EVERY creature friends-with-everything + unkillable instead of skipping the
sparrows / Whisper apprentices / Maze. Two converter defects, traced with `CONVERT_DUMP=CheckFriendlyAttacks`
and a monkeypatch that dumps the text before/after `fold_local_thing_vectors`:

* **`canonicalise_stack_objects` folded the vector's end slot onto the PreMeleeMaze thing.** Ghidra's
  `puStack_90` (the end; true slot -0x80, its Ghidra number is 0x10 adrift) lands on 0x90 = the thing's base
  (`auStack_a0` → true -0x90, `QUESTTHING_Empty()`). The pass already keeps assigned names before the
  constructor line as their own objects ("own"); after it, everything inside the extent was renamed. A rule
  wide enough to keep any assigned name broke the Trader helper_DFDED0 actor map (`iStack_28 = 0` is the
  map's size member and must fold — smoke 2 → 3). The precise discriminator: a name assigned in the post
  range AND used in pointer arithmetic / a compare against a stack slot OUTSIDE the extent
  (`(int)puStack_90 - (int)puStack_94`) is its own object; a member is never subtracted from a sibling of
  another object.
* **`at_vcall_local` only accepted a bare index.** The `((int)V + i)` element spelling is rewritten to
  `LOCALLIST_At(V, (i) / 0xc)` first, and the vcall fold's regex wanted `LOCALLIST_At(V, \w+)`; the `(V + i)`
  spelling went through `elem` and resolved. Accepting `(\w+) / 0xc` resolved the four `GetDefName` (+8) and
  the `SetFriendsWithEverythingFlag(1)` (+0x10c) vcalls, and `SetToKillOnLevelUnload(0)` in WoodsWill.

The readable then drops the `IsActiveThreadTerminating` guard between `GetDefName` and the flag call —
`prune_dead_termination_checks`, by design: `GetDefName` is pure, no scheduler advance since the loop head.

## Gates

* Oakvale draft byte-identical (`convert_new_oakvale.py --out <scratch>` vs `refs/.../NewOakValeIntro`).
* Units regenerated (Guild into `readable_converter`), smoke: Guild 1/1 (BirdKiller, baseline), Orchard 0/0,
  Trader 2/2, Gameflow 0/0. Only Guild Lua changed (GuildTraining.lua, GuildTrainingWoodsWill.lua).
* New `test_check_friendly_attacks_converter.py` (draft + readable through lupa with five creatures across a
  level reload: the guard and the apprentice are made friendly on both passes, the sparrow / Maze never; no
  `nil == "CREATURE_`, no `>> 31`). Targeted set 39 OK; full suite: see the log `work/converter_suite_20260920.log`.
* v6 + v7 rebuilt on the regenerated units + the `e1ed740` DLL, both preflight PASSED. `work/AeonShare-2026-09-20.zip`
  built (notes updated: all three Orchard quests, Guild played through the beetles, Gameflow unit added, bundle v6).

## Next run (user)

    python tools/script_recovery/ab_playtest.py launch v6      # profile f645456fds (woods-entry autosave) or the post-beetles save
    python tools/script_recovery/ab_playtest.py launch v7      # same save, OUR Gameflow
    python tools/script_recovery/ab_playtest.py compare v6 v7

Expect on the return from the woods: no crash at `TEACHER.LookAtNothing`, no `CheckFriendlyAttacks` error, and the
open question from last night answered — whether the re-run Guildmaster `Main` replays PUNCH (retail lifecycle,
then the woods loop's WOODSWON / YES-NO comes only from a surviving thread) or parks on `TryAcquire`.

## 3. Ultracode audit round: the spawner-literal rotation (silent, on the next stage)

A 15-agent residue audit over the four regenerated units (`converter-residue-audit`, 4 finders + adversarial
verify against the typed C) confirmed two findings, both in `RunTutorials` (0x00D45DD0), both live on the path
the next run takes: `CreateObject("PreMeleeDummy", GetThingWithScriptName("OBJECT_STRAW_DUMMY_01"):GetPos(),
"PreMeleeDummyMarker")` (native: def `OBJECT_STRAW_DUMMY_01`, marker `PreMeleeDummyMarker`, script name
`PreMeleeDummy` — a def name looked up as a script name is nil, `:GetPos()` raises; masked so far only because
the TNG dummy is alive and the guard skips it) and the same rotation on `CreateCreature("MeleeApprentice",
GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"):GetPos(), "M_MeleeOpponentStand")` at
GameState 5 — the first thing after the woods. Yesterday's journal quoted the CreateObject line as the fixed
result; it was wrong, and so were all nine Guild `CreateCreature` sites, two `AddLineToConversation` speaker /
key swaps, an `EntityTeleportToThing` operand swap, Orchard-Good's actor map passed as `actorMap2[0 + 1]` (nil)
to SetActor / RunMacro / DestroyActorMap, and PreMelee's `TryAcquire(resource4, appleMarker[0 + 1], 4)` for the
Guildmaster. One cause, traced through `CONVERT_DUMP` and the pre-fold dump:

* **`fold_local_thing_vectors` treated every bare `(CScriptThing *)V` as element 0 of the vector V.** Ghidra
  reuses the AppleMarker vector's slot -0x54 as the hidden by-value result of fifteen `GetThingWithScriptName`
  calls before and after the vector's life, and as the acquired thing of the Guildmaster `TryAcquire`. Rewritten
  to `LOCALLIST_At(xStack_54, 0)`, the lifter no longer saw a hidden-result slot, shed the real string operand as
  a "prototype extra", back-filled the string slot from its LIFO literal pool, and the rotation cascaded into the
  two calls after it. Fix: the bare-name rules apply only inside the vector's live range (fill → `~vector` /
  `free`); a by-value fill with no visible destructor keeps the whole-text behaviour.
* **The lifter's `slot_results` survived a fresh definition of the slot**: after the fix the vector fill
  `xStack_54 = GetAllThingsWithScriptName("AppleMarker")` was still rewritten to the stale hidden result
  (`if #hero ~= 0`). An interface-call target now drops its `slot_results` entry.
* `_receiver_printed` compared the vtable-alias line with the printed receiver cast-sensitively
  (`iVar9 = **(int **)((int)this + 0x40)` vs `*(void **)((int)this + 0x40)`) and on CRLF text; fixed, so those
  sites now pair — which exposed that a truncated push record on a site with a vcall between the pushes must
  NOT be padded into an alignment: at 0x00D46A73 the export charged `GetPos` a 4-byte purge it does not make
  (bytes: `push eax` of the pre-pushed script operand survives the call; depth 176 recorded, 180 real), so
  every recorded slot was 4 too high while 0x00D464AE in the same function is exact. Those sites stay
  unaligned; the pool is right once the hidden slot is visible.

Gates after: Oakvale byte-identical; smoke baseline; changed Lua = Guild (GuildTraining.lua, WoodsWill) and
Orchard (the actor map). Targeted 41 OK; full suite rerun clean in `work/converter_suite_20260920b.log`
(the first run, `..._20260920.log`, was invalidated by a `git stash` during it). v6 / v7 rebuilt + preflighted;
`work/AeonShare-2026-09-20.zip` rebuilt on the fixed units.

## 4. Second v6 run (09:22): no crash, no Lua error — and the PUNCH replay root-caused from the bytes

`work/ab_runs/v6-20260920-092255`: the sidecar fix holds (the `[QuestThreadFlag]` dump at `TEACHER.LookAtNothing`
prints cleanly after the WoodsMelee teardown), `CheckFriendlyAttacks` survives the return, and the Guildmaster
replays PUNCH (1242). The retail oracle (the user's walkthrough link, 26:34-26:58): the hero walks out of the
woods and the WOODSWON close-up fires at once — no talk, no re-teleport — then the split AVI. That is the woods
loop of a **surviving** thread. Ours was unwound on woods entry by the retail flag (`0xF35B30` on the entity host =
`GetParentScript()->IsActiveThreadTerminating()`), so retail must keep the script alive through the unload. It does:

* `CScriptBase::OnScriptedEntityDeactivated` (0xCB88B0, from `CQuestManager::OnDeactivateEntity` 0x4AFB00 with
  `true` from `SaveThingsOnLevelReadyForUnloading` 0x5252B0): `if (bUnload && (script->Flags & 1)) {
  EnableNavigator(true); thing->vslot(0x124)(); return; }` — otherwise `script+5 = 1` (terminating) + `SaveEntityScript`.
  `+0x3c` = `CActiveEntityScriptBase::Flags` (PDB).
* The flag comes from the binding: retail quest `Main` builds each `CEntityScriptBinding` as
  `{vtable, name, this, alloc, 1, flags}` (`puVar2[6] = 1;` before `AddEntityScriptBinding`); the factory
  `Script_CreateActiveEntityScriptBase` (0xE7ED60) passes `binding+0x18` into the `CActiveEntityScriptBase` ctor.
  Survey over the four units: every Guild binding is 1, Orchard's `MK_OFI_GWLL_WHIS2` and all Trader bindings are 0.
* The sidecar already forwarded an optional third `flags` argument (`CEntityScriptBindingBase::unknown_zero` at
  +0x18) — the converter never emitted it. `RE_BINDING` now captures the store and unit-mode drafts emit
  `AddEntityBinding(name, path, 1)` (Oakvale gate untouched). `UpdateSpawnedFunction` (0xCB7950) sets `quest+0x2c`
  only while a thread is being updated, so `thread=0` in the dumps was never a defect.

## 5. Second audit round (32 agents): 20 confirmed, the Guild-path ones fixed

Melee Guildmaster (0x00D58490): `infoCounter & 1` on nil (the EH flag in an `int *` slot reused as the counter handle
— `drop_eh_state_flags` now splits a flag phase off a slot with no register copy), the `AddQuestInfoBarHealth` colour
(address loaded BEFORE the byte stores — `fold_stack_colours` follows the pointer temp), the melee grade computed
into a CCharString-typed slot (float phase → `f_stk_1d4`). Will's Guildmaster (0x00D5E0C0) acquired an earlier
resource (byte split with a literal low byte — `RE_BYTE_SPLIT` accepts `u0 = 0;`). RunTutorials' leftover-apple
`RemoveThing(V + i)` (int-typed begin slot — indexed now). TraderConflict's `MsgIsHitBySpecialAbilityFrom(p0, 0xe,
HERO)` lost the enum to the receiver alias (`strip_receiver_arguments` strips `p0 = (CScriptThing *)(this + 8)`
aliases — 24 `SetFriendsWithEverythingFlag(me)` sites became `(true)` too). Still open from the round: BirdKiller's
dropped `AddLineToConversation` text key (0x00D4DEA0), the Trader `"TEXT_QST_B11_" .. GetDataString() .. "_SUFFIX"`
idiom (3 sites + a speaker/listener swap at 0x00DFF312), TraderToRescue's `IsRegionLoaded("")` (array-typed string
local), Orchard's branch-selected literals (`GreatwoodEntrance`/`GreatwoodLake`, `FACTION_GUARDS_ENEMY`/`FACTION_BANDITS`)
and the Good rules' counter handle (`ePriority`); the Evil counter one is a Ghidra ESP mis-track (skip).

Gates: Oakvale identical; smoke baseline; `test_binding_flags_and_melee_stage.py` + targeted set 51 OK; full suite
`work/converter_suite_20260920c.log` = 1616 run, green except the four known stale-fixture modules (322+78+44+1).
v6 / v7 rebuilt + preflighted; zip rebuilt. **Next run:** same command; expect WOODSWON at the woods door on
return, the YES/NO, the AVI — then the Melee stage (Whisper fight, grades) as new ground.

## 6. Late afternoon (user out running): the rest of the second audit, generically

* Orchard: per-branch literals consumed by a `CCharString(&slot, var, -1)` constructor are now real locals
  (`string = "GreatwoodEntrance"` / `"GreatwoodLake"` in `Init`, `FACTION_GUARDS_ENEMY` / `FACTION_BANDITS` in
  OrchardFarmWhisper — both were hard-coded to the Good variant). `ProcessGameRulesGood`'s
  `RemoveQuestInfoElement(ePriority)`: `isolate_gsi_vtable_temps` no longer leaves a vtable load under the
  register's name when the same name has other uses in scope (Ghidra merged two lifetimes: the counter handle
  reload and the vtable on the Whisper branch); the vcalls take the alias, the other uses keep the register.
* Trader: the early-pushed `"_SUFFIX"` literal of `AppendCString` (`name_append_literals` now accepts the
  `(int *)pCVar4` operand spelling — a case bug, `p[ci]Var` vs `pCVar`); the push-rebuild charges the two
  `__fastcall` string helpers (0x99F600 / 0x99F690) their one stack operand when the export recorded none, so
  `AddLineToConversation` no longer swallows the suffix push (`(id, text, "_THREATEN", 0, me)` → `(id, text, me,
  hero, false)` at all five push-[mem] sites); a doubled `((this + 8))` from the register trace is unwrapped
  (which also fixed three `AddNewConversation(nil, ((me)), false)`); `fold_stack_colours` treats the array-spelled
  constructor `CCharString(aCStack_14c, ...)` as the slot's redefinition (`IsRegionLoaded("")`); the sign test of
  a byte slice (`CVar6._0_1_ < '\0'`) is the flag's bit 0x80, not a fresh scalar.
* BirdKiller's dropped text key turned out to be already fixed by the vtable-alias change.

Gates: Oakvale identical; smoke baseline; targeted 51 OK. Third audit round launched on the regenerated tree.
Not chased: Orchard `ProcessGameRulesEvil`'s counter slot (a Ghidra ESP mis-track at the export level).
