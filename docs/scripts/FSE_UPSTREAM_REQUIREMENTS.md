# What stock FSE needs to run converter-generated retail quests

Audience: Aeon / FableScriptExtender maintainers. Status 2026-09-16.

Our quest Lua is **lifted from the retail native scripts** (Ghidra decompile of `Fable.exe`, names
and layouts from the debug PDB, FSE's own `GameInterface.h` typedefs for the vtable ABI). The output
mirrors the native logic 1:1 — same state fields, same cutscene/resource lifetimes, same RNG use —
so it needs a few *generic* runtime primitives that stock FSE (upstream master, checked 2026-09-12)
does not expose. Everything below has a working implementation in our compatibility DLL
(`ForgeFSE-retail-shadow` + scalar-ABI patch), from which PRs can be lifted. Nothing is quest-specific.

New Oakvale intro (`Q_NewOakValeIntro`, 51/51 functions) plays through childhood on this API.
Orchard Farm (`Q_OrchardFarmRaid` + Evil/Good variants) is in progress on the same API.

## 1. Replace a native quest by name (required)

Stock FSE only **adds** quests from `quests.lua`. A Lua `Q_OrchardFarmRaid` registered that way
runs beside the native one (double execution, duplicated entity bindings). Retail-parity scripts
must *replace* the native quest under its own name so gameflow, quest cards, section quests
(`Q_OrchardFarm_Barricade`), persistence and `MsgOnQuestCompleted("Q_OrchardFarmRaid")` keep working.

Implementation we use: detour `CScriptManager::AddScript` (`0x00CB5C90`, prologue
`8B 44 24 08 83 EC 18`), and when `scriptInfo->Name` matches an entry of an opt-in
`retail_override.lua`, forward the same `CScriptInfo` with `pAllocFunc` swapped for the Lua host's
allocator (identity preserved). `dllmain.cpp: AddScript_RetailOverrideDetour / InstallRetailOverrideHook`.
Config shape:

```lua
RetailOverrides = {
  enabled = true,
  entries = {
    { nativeName = "Q_OrchardFarmRaid", file = "OrchardFarmRaid/OrchardFarmRaid", mode = "override",
      entity_scripts = { { name = "GuardTeamSpawn", file = "OrchardFarmRaid/Entities/TeamSpawn", id = 200 }, ... } },
  },
}
```

## 2. Retail resource objects with retail lifetime semantics (required)

Retail scripts control entities through `CScriptGameResourceObjectScriptedThingBase` objects and
cutscenes through actor maps; they nest control by **priority** and release in a specific order.
Stock `AcquireControl` returns true for an existing handle regardless of priority, and
`RunCutsceneWithSetup` builds its own movie object and pause; the Whisper / raid cutscenes in
Orchard Farm and the Bully / Father scenes in Oakvale need the retail shapes:

| Lua (`resources` object) | Native |
|---|---|
| `NewResource()` / `TryAcquire(id, actor, priority)` / `ReleaseResource(id)` | `CBaseObject_Construct` 0x99A380 + vtable 0x127094C; GSI `StartScriptingEntity(thing, res, prio)` slot 0x20; dtor 0x7E74D0 |
| `NewActorMap()` / `SetActor(map, key, thing_or_resource)` / `DestroyActorMap(map)` | `std::map<CCharString, CCountedPointer<…>>`: ctor (inline `malloc(0x24)` or 0xCDBF70), `operator[]` 0xCD3D2E, dtor 0xCDBFB0 |
| `RunMacro(name, map, setupCondition, skippable)` | `RunCutsceneMacro_Func` 0xCBFB7D (`__fastcall`, 6 args) |
| `StartMovie("")` / `DestroyMovie(id)` | movie object ctor 0x6E7B40/60, vtable 0x1260EF4, dtor 0x6E7B80 |
| `Pause(bool)` | `PauseAllNonScriptedEntities` |

Also needed: a way to hold the `resources` object for the quest's lifetime
(`quest:RetailResources()`); the closure form `WithRetailResources(function(resources) … end)`
works but forces every generated function to be wrapped.

## 3. State kinds (required)

| Binding | Why |
|---|---|
| `quest:GetStateFloat/SetStateFloat` | float members exist (positions, timers); missing upstream |
| `quest:GetStateThing/SetStateThing(name, thing)` | `CScriptThing` quest members (Orchard: `Teams[i].CrateDropPos`, `Teams[i].TeamCrateCarrier`) shared between the quest and its entities |
| `quest:GetStateListCount/GetStateListAt/StateListPush(name, …)` | `std::vector<CScriptThing>` members (Orchard `CrateList`; Guild `DummyWizardsVector`) |
| `quest:StateListClear(name)` / `StateListSet(name, table)` / `GetStateListCopy(name)` | member vector `erase(begin,end)`; a GSI out-argument fill (`GetAllCreaturesExcludingHero(&m_AllCreatures)`, TraderConflict); the by-value copy constructor (`std::vector<CScriptThing> local(m_AllCreatures)`, TraderConflict `AttackPeople`) — sidecar patch `novi-unit-bindings.patch` |
| entity-state `Thing` kind | entity `CScriptThing` members (`ThingToPatrolTo`, `OtherSpawnPoint`) |

## 4. Small bindings (required for the quests we have lifted)

| Binding | Native | Note |
|---|---|---|
| `quest:IsActiveThreadTerminating()` | `CScriptBase::IsActiveThreadTerminating` 0xCB7940 (quest) / 0xF35B30 (entity) | retail polls it after every frame; must be the real predicate |
| `quest:RetailRandModulo(n)` | native RNG | `math.random` changes retail RNG consumption |
| `quest:IsDistanceBetweenThingsOver(a, b, d)` | 0xCBE3EA | pair of `…Under` 0xCBE2FF |
| `me:IsDistanceFromPositionOver(pos, d)` | entity API | |
| `quest:AddNewConversation(thing, b1, b2)` / `AddPersonToConversation(id, thing)` | GSI slots | raw conversation API; the combined helpers change ordering |
| `quest:ReadGlobalGameData(offset)` | `*(DAT_0143e90c + offset)` | boast renown/gold in `AddBoast` come from this table |
| `me:GetName()` | `CScriptThing` vtable slot 4 | entities derive their team from their own script name |
| `me:SetDataString(s)` / `me:GetCurrentStateGroupType()` / `me:IsBeingCarriedBy(scriptName)` | `CScriptThing` slots / GSI | |
| `quest:PersistTransfer{Bool,Int,Float,String}(context, name, default)` | `CPersistContext::Transfer<T>` | explicit default; current value as default differs on first load |
| `quest:IsPlayerHoldingFireRangedWeaponButton()` | GSI slot 67 (0x10C) `?IsPlayerHoldingFireRangedWeaponButton@CGameScriptInterface@@UBE_NXZ` | GuildTraining skill tutorial polls it (5 sites, 2026-09-17) |
| `me:MsgIsHitBy(scriptName)` | `?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z` | the DLL only exposes `MsgIsHitByHero`; guild dummies test a named attacker (6 sites) |
| `quest:AddQuestInfoTickByText(text, state, scale)` | GSI slot 331 (0x52C) | already bound (`AddQuestInfoTickByAction` = slot 330); the converter now emits the `ByText` name for CCharString operands |

## 5. Already upstream (thanks) — for reference

`GetCurrentStateGroupType`, `MoveToPosition_NonBlocking`, `MsgIsHitBy`, … (the 12 bindings added in
FSE 6.9.26 after the first Aeon-port audit), `AcquireControl`, `RunCutsceneWithSetup`, persistence,
frame return values, `OnPredicateFail` dispatch.

## Evidence / where to look in our tree

- Compatibility DLL source: `D:\Code\ForgeFSE-retail-shadow` (+ `work/oakvale_entity_scalar_abi_integration/*.patch`).
- Vtable ABI spec generated from FSE's typedefs: `refs/script_recovery/typing/gsi_prototypes.json`.
- Per-quest binding gaps: `refs/script_recovery/orchard_farm/RUNTIME_API_GAPS.md`,
  `refs/script_recovery/lifted/NewOakValeIntro/UPSTREAM_FSE_COMPATIBILITY.json`.
- Upstream audit notes: `docs/journal/2026-09/NEW_OAKVALE_ORIGINAL_FSE_COMPATIBILITY_2026-09-12.md`.
