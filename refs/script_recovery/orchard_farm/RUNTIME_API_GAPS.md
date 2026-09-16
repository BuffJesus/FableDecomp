# Orchard Farm draft: runtime bindings not yet in the NoviCompatibility DLL (2026-09-16)

Generated Lua (refs/script_recovery/lifted/OrchardFarm/draft) calls these; add them to the sidecar
(ForgeFSE-retail-shadow + scalar-ABI patch tree) before the package can run.

| Lua call | Meaning | Native evidence |
|---|---|---|
| `quest:GetStateThing(name)` / `SetStateThing(name, thing)` | Thing-valued quest state (Teams[i].CrateDropPos / TeamCrateCarrier, entity ThingToPatrolTo via entity state) | PDB CScriptThing members, counted-pointer assign/release folds |
| `quest:GetStateListCount/GetStateListAt/StateListPush(name, ...)` | `vector<CScriptThing>` CrateList | begin/end stride-12 iteration, `Vector_PushBack_ScriptThing` 0x8ADF90 |
| `quest:RetailResources()` | long-lived `resources` handle (draft functions declare `local resources = quest:RetailResources()`) instead of `WithRetailResources` closures | — |
| `quest:ReadGlobalGameData(offset)` | ints read from the global game-data table `*(DAT_0143e90c + off)` (boast renown/gold) | `AddBoast` operands in Q_OrchardFarmRaidEvil/Good Init |
| `me:GetName()` | entity's own script name (TeamID derivation) | CScriptThing vtable slot 4 |
| `me:SetDataString(s)` | | CScriptThing slot (`SetDataString`) |
| `me:IsBeingCarriedBy(scriptName)` | Artefact carried check | GSI slot |
| `me:GetCurrentStateGroupType()` | | CScriptThing slot |

`resources:` methods used (already in the sidecar tree): NewResource, TryAcquire, ReleaseResource,
NewActorMap, SetActor, DestroyActorMap, RunMacro, StartMovie, DestroyMovie.
