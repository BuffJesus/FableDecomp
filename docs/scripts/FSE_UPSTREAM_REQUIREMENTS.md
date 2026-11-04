# What stock FSE needs to run converter-generated retail quests

Audience: Aeon / FableScriptExtender maintainers. Status 2026-09-16.

Our quest Lua is **lifted from the retail native scripts** (Ghidra decompile of `Fable.exe`, names
and layouts from the debug PDB, FSE's own `GameInterface.h` typedefs for the vtable ABI). The output
aims to preserve native state, call order, resource lifetimes and RNG use. Converter diagnostics, synthetic boundaries and unverified runtime behavior remain; the output is not certified 1:1. It needs a few *generic* runtime primitives that stock FSE (upstream master, checked 2026-09-12)
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
| `quest:StateListClear(name)` / `StateListSet(name, table)` / `GetStateListCopy(name)` | member vector `erase(begin,end)`; a GSI out-argument fill (`GetAllCreaturesExcludingHero(&m_AllCreatures)`, TraderConflict); the by-value copy constructor (`std::vector<CScriptThing> local(m_AllCreatures)`, TraderConflict `AttackPeople`) — sidecar patch `novi-unit-bindings.patch`; **sidecar: bound 2026-09-19** (source landed + DLL rebuilt) |
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
| `quest:IsPlayerHoldingFireRangedWeaponButton()` | GSI slot 67 (0x10C) `?IsPlayerHoldingFireRangedWeaponButton@CGameScriptInterface@@UBE_NXZ` | GuildTraining skill tutorial polls it (5 sites, 2026-09-17) — **sidecar: bound 2026-09-19** |
| `me:MsgIsHitBy(scriptName)` | `?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z` | the DLL only exposed `MsgIsHitByHero`; guild dummies test a named attacker (6 sites); an empty name = hit by anything this frame (0x8D0FB0) — **sidecar: bound 2026-09-19** |
| `quest:AddQuestInfoTickByText(text, state, scale)` | GSI slot 331 (0x52C) | already bound (`AddQuestInfoTickByAction` = slot 330); the converter now emits the `ByText` name for CCharString operands |
| `me:MsgIsHitByAnySpecialAbilityFrom(scriptName)` | `?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z` (slot 0xA8) | TraderConflict polls it per creature with `"TC_BanditFighter"` (4 sites); only the `...FromHero` form was bound — **sidecar: bound 2026-09-19** |
| `me:MsgIsHitByHeroSpecialAbility(ability)` (int `EHeroAbility`) | slot 0xA4 `?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z` with `SCRIPT_NAME_HERO` | TraderConflict passes ability 14 (the recovered immediate); the binding must take the enum value |
| `quest:IsPlayerHoldingLockTargetButton()` | GSI slot 66 (0x108) | TraderConflict Good/Evil poll it in the intro (3 sites) — **sidecar: bound 2026-09-19** |
| `quest:TextEntryExists(key)` | GSI slot 0x598 | TraderToRescue builds `"TEXT_QST_B11_" .. name .. "_ONTALK_" .. n` and probes it (2 sites); the readable output still drops the key operand and the result store (converter gap: `quest:TextEntryExists()`) — **sidecar: bound 2026-09-19** |
| ~~`me:MsgExpressionPerformedTo(scriptName)`~~ EXISTS: `me:MsgExpressionPerformedTo()` returns the expression name or nil (LuaEntityAPI.cpp; the native out-parameter is the result, the bool is its presence — the lifter emits `name = me:MsgExpressionPerformedTo(); fired = name ~= nil`) | slot 0x74 `?MsgExpressionPerformedTo@CScriptThing@@UBE_NAAVCCharString@@@Z` | TraderToRescue (EXPRESSION_FOLLOW / EXPRESSION_WAIT checks) |
| `quest:EntitySetAsOpinionSource(thing, ...)` | GSI slots 0x9B4 / 0x9B8 | TraderConflict Evil Main (1 site) + GuildTraining (1 site); both pass a string (0x9B8), the binding dispatches string/int like the native overload pair — **sidecar: bound 2026-09-19** |
| `quest:IsHeroInProjectileWeaponMode()` | GSI slot 69 (0x114) `?IsHeroInProjectileWeaponMode@CGameScriptInterface@@UBE_NXZ` | GuildTraining ranged tutorial (2 sites) — **sidecar: bound 2026-09-19** |
| `hero:MsgHitFriendWithMeleeWeapon()` / `MsgHitFriendWithRangedWeapon()` / `MsgHitFriendWithBareHands()` | CScriptThing slots 0xBC / 0xC4 / 0xCC (`@@UBE_NXZ`, 0x4AADE0 / 0x4AAE20 / 0x4AAE60) | GuildTraining friendly-fire checks on the hero (1 site each) — **sidecar: bound 2026-09-19** |
| `quest:CreateEffect(result, name, {x,y,z}, scriptName, angle, indep, always)` / `(result, name, thing, bone, scriptName, indep, always)` | GSI slots 0x190 / 0x194, the native overload pair with the hidden result first | GuildTraining PreMelee `SMASH_DUMMY_01` (3 sites, at-position form); the readable output loses the position operand (passes an actor map / thing where retail copies a `C3DVector`), the binding logs and creates nothing there — **sidecar: bound 2026-09-19** |
| `quest:PersistTransferStringList(context, name, table)` (returns the table) | `CPersistContext::Transfer<std::vector<CCharString>>` 0x49B8D0 (`__thiscall`, 2 stack operands: name, &vector; no default) | Gameflow `OnPersist` transfers `SavedScriptNames` (this + 0x4c) and `SavedCardDefNames` (this + 0x58) this way; retail `Main` never reads either. **The converter now emits the call** (2026-09-19 pm): `local savedScriptNames = quest:PersistTransferStringList(context, "SavedScriptNames", {})` — the binding must transfer a Lua sequence of strings under `name` exactly like `Transfer<vector<CCharString>>` (count then each string) and return the (loaded) table; an empty table goes out because nothing in the script writes the member. **sidecar: bound 2026-09-19 (`b2b4697`)** — `LuaQuestState::PersistTransferStringList` builds a retail `std::vector<CCharString>` with push_back 0x44BFF0, calls 0x49B8D0, reads the loaded elements back into a new table, destroys the vector with 0x414EA0. `PostSavePosition` goes through `PersistTransferInt` (callee 0xCDCA70 is the `Transfer<int>` body shape for the `EGameflowPosition` enum) and `CoreQuestWaiting` through the existing `PersistTransferUInt` (0x4106F0) |

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


## Arena round definitions (2026-09-26)

`quest:InitialiseArenaRounds()` replaces the reviewed definition-vector copy at
retail `0xF25854` (`0xF25980`, quest +0x98 from global definitions +0x1044).
The implementation copies the script-visible round/wave/creature fields into
owned values, then writes `Rounds_<round>_Waves_<wave>_Creatures_<group>_*`
through the existing namespaced Int/Bool/String state methods. Indices are zero
based. Declared counts and vector lengths remain distinct; no Lua table or native
pointer is shared between callback states. Opaque inherited definition fields
are not script state and are not copied into Lua. Arena's native OnPersist does
not serialize this round-definition vector.

Source: `tools/script_recovery/runtime_bindings/NoviArenaRounds.h` and
`arena_round_snapshot.h`; installable source patch:
`tools/script_recovery/sidecar_patches/novi-zzzzzzzzzzzzz-arena-rounds.patch`.
**Historical deployment status (2026-09-26):** the x86 C++ snapshot executable and binding compile probe
pass against the ABI-v13 headers. A separate Release x86 candidate DLL builds
and links with all six subsequent patches (things-killed through Arena rounds).
It is not installed or in-game validated; smoke reports the method as pending.
The candidate and its patch/hash manifest are in
`work/readability_marathon_20260926_round10/sidecar_candidate/`. Preserve the other
patches when preparing a later candidate; do not drop existing bindings.

2026-10-18 source-profile clarification (W198): the current owned movie-reentry
SDK already includes and calls `RegisterNoviArenaRounds`. Its unchanged full
registration compiles, and synthetic snapshot checks pass. Smoke now follows
called registration helpers in local quoted headers, so this method is known
when that source profile is selected. Older profiles can still report it pending.
Static source recognition does not establish DLL identity, installation, actual
Arena execution, or C++ conditional-compilation/liveness semantics.

Reproduce without changing the base checkout or game installation:

```powershell
python -X utf8 -m tools.script_recovery.run_arena_round_checks --forge-root work/new-oakvale-original-fse-20260912/sidecar-abi-v13 --output work/arena-round-checks-new
```

The output directory must be new. The executable checks nested vectors, owned
strings, signed counts, empty vectors, declared counts, and malformed extents.
The compile probe instantiates the real sol registration with the actual FSE
headers; it does not link or run engine code. Generated Lua and the retired
container bodies remain traceable through `runtimeBoundaries` in the conversion
report. The original native evidence is retained in the translation unit.

## Script-member resources and string maps (2026-09-27)

Retail scripts keep resource objects (PDB `seh_*`, `CScriptGameResourceObjectScriptedThingBase`)
and cutscene-argument string maps (`csargs`, `std::map<CCharString,CCharString>`) as script
members: constructed with the script and destroyed by its destructor (Bordello `0x00E46A50`).
Entities write their quest's members (BordelloGuard `0x00E40420`: `parent->seh_Guard = res`,
`parent->csargs["$DIALOGUE"] = ...`) and the quest's cutscene helper reads them (`0x00E3E720`).

| Binding | Retail | Notes |
| --- | --- | --- |
| `resources:MemberResource(name[, owner])` | member resource object | one persistent entry per PDB member name in the quest's long-lived `RetailResources` scope (shared by the quest and its entity VMs); an entity's own member is keyed by its thing too |
| `resources:MemberStringMap(name[, owner])` | `std::map<CCharString,CCharString>` member | persistent; filled with the existing `SetString`, passed to `RunMacroWithStrings` |
| `resources:AssignResource(dst, src)` | `CScriptGameResourceObjectScriptedThingBase::operator=` `0x8ABD10` (= `CBaseObject_Assign_API`) | counted copy; the copy takes over the control-handle registry entry |
| `resources:ClearStringMap(id)` | `std::map<CCharString,CCharString>::clear` `0x9AACE0` | disasm: the destructor `0x9AC310` without freeing the head, so destroy + construct in place |

Patch: `tools/script_recovery/sidecar_patches/novi-zzzzzzzzzzzzzz-member-resources.patch` (on top of
the six round-10 patches). **Deployment pending:** the Release x86 candidate builds
(`work/readability_marathon_20260927_round12/sidecar_candidate/`); not installed or in-game validated.
Known gap: the quest scope is never closed, so members outlive a finished quest (retail destroys
them with the script object).

## Member thing lists and global conversations (2026-09-27, afternoon)

| Binding | Retail | Notes |
| --- | --- | --- |
| `quest:StateListResize(name, n)` | `std::vector<CScriptThing>::resize(n, CScriptThing())` 0xD34AC0 | truncate or pad with empty things (CS_OakValeRevisited OakValeFire sizes `Fires`) |
| `quest:StateListSetAt(name, i, thing)` | inlined `CScriptThing::operator=` into element i (0-based) | stores a copy; an empty/nil thing stores an empty handle |
| `quest:GlobalConversations(offset)` | `std::vector<CConversation>` at `*(0x0143E90C) + offset` | array of `{Lines, Speaker, Dialogue, Animation, AnimLoop}` (CConversation::Copy 0x00E54CA0 layout, 0x5C bytes); V_BookCollecting reads BookReactions (+0x4C8) through it |

Patches: `novi-zzzzzzzzzzzzzzz-state-list-resize.patch`, `novi-zzzzzzzzzzzzzzzz-global-conversations.patch`
(after the member-resources patch). The Release x86 candidate in
`work/readability_marathon_20260927_round12/sidecar_candidate/` builds; not installed or in-game validated.
`quest:RetailFlags(name)` (already bound) now also serves member `map<CCharString,bool>` flag maps.

## Indexed definition strings (2026-09-27)

`quest:ReadGlobalGameDataStringAt(offset, index)` returns the zero-based element of
`vector<CCharString>` at the global definition object's byte offset. BookReaction
(0x00E566F0) reads boy/girl markers at +0x4B0/+0x4BC; native loads and PDB
BookBoyMarker/BookGirlMarker types support this ABI. Invalid negative indices,
null definitions and absent/out-of-range elements return an empty string.
Patch: `tools/script_recovery/sidecar_patches/novi-zzzzzzzzzzzzzzzzzz-global-string-at.patch`.
The round-12 Release x86 sidecar candidate builds; this binding is pending in the
installed sidecar and has not been verified in-game. Do not treat mock support as deployment.

## Thing-to-position Under test (2026-09-28)

`thing:IsDistanceFromPositionUnder(position, distance)` forwards to retail 0x00CBE4B7
(`__fastcall bool(CScriptThing*, const C3DVector*, float)`: alive, and squared distance strictly below
`distance^2`), the twin of the already-bound `IsDistanceFromPositionOver` (0x00CBE45C). Callers:
V_GuildMaster GuildMasterGameFlow Main (hero within 3.0 of TeleporterResidue) and V_ChickenKicking
ChickenMaster Main (its vector operand is still unrecovered). Patch
`novi-zzzzzzzzzzzzzzzzzzz-position-under.patch`; the round-12 Release x86 candidate builds with it;
not installed or in-game validated.


## Scoped Thing outputs and direct implementation equality (2026-10-05)

Retail `CScriptThing` locals are 12-byte wrappers with independent counted references.
Lua aliases to one wrapper do not replace native copy construction, assignment, or
destruction. Generated helpers use `WithRetailResources` to retain those local
lifetimes and explicit retirement at the corresponding native boundary.

| Binding | Contract |
| --- | --- |
| `resources:CopyThingOutput(thing)` | Native copy into a new owned wrapper, registered in this scope. Nil constructs an owned empty wrapper. Returns Thing userdata, not a numeric resource token. |
| `resources:TrackThingOutput(thing)` | Claims an API-owned output and returns its token. Repeated tracking in the same scope returns that token without taking another native reference. Nil returns zero. Unowned, retired, or cross-scope outputs are rejected. |
| `resources:AssignThingOutput(token, source)` | Calls retail assignment `0x008AB980` on the live scoped destination. Nil source is an empty native wrapper. Assignment updates the existing wrapper, so its Lua aliases observe the change. |
| `resources:RetireThingOutput(token)` | Deterministically releases that local's native reference. Zero is a no-op in an open scope. Remaining Lua aliases become empty Things; their later GC frees metadata/storage without releasing the native reference twice. |
| `thing:RetailImplementationIsEqualTo(other)` | Direct concrete implementation dispatch using the wrappers' `Data` pointers, matching native helper sites `0x00E01A98` and `0x00E02195`. This is distinct from the existing nil-safe wrapper `IsEqualTo`. Both arguments must be actual wrappers; an empty implementation can retain the native access violation. Do not substitute false for that native fault. |

Scope closure retires remaining owned outputs after the protected callback returns,
including a protected Lua error. Suspension must retain the C++ scope and its Lua
stack until the callback resumes or the production host drains it. Deleting a
suspended fiber is not equivalent to unwinding that callback. Native termination
queries remain at their reviewed ownership boundaries even when their boolean
result is unused.

The cumulative committed implementation is preserved in
[`novi-scoped-thing-outputs-direct-equality.patch`](../../tools/script_recovery/sidecar_patches/novi-scoped-thing-outputs-direct-equality.patch):
base `07774e98c5675fb66aba7c23b4f1e4453b0434c0`, candidate
`161c39c4ad167ed23b63704b46afe60e8dd63c35`, patch SHA-256
`b885fe7f8cd9b36d08d74ddcf1c5fe276fd16ab907d6d56893032909e392dc70`.
It includes prerequisite Thing-output ownership and the earlier reviewed
camera/position/CreateCreature ABI changes; it is not an independent patch against
an arbitrary upstream checkout. Reviewed lowering evidence remains in
[`Q_TraderEscort.json`](../../refs/script_recovery/trader_escort/units/Q_TraderEscort.json).

Validation distinguishes original native oracle execution, actual SDK/sol probes,
standalone Win32 fiber suspension, installed DLL identity, and live host evidence.
The standalone probes model engine/copy/destructor boundaries and do not establish
production quest teardown or full escort behavior. Current results and remaining
gates are recorded in the [Lua lane journal](../journal/2026-10/LUA_RECOVERY_2026-10-05.md).


## Health-inclusive hero-hit predicate (offline candidate, 2026-10-22)

`resources:IsHitByHeroExceptAbilityAndHealthAbove(actor, excludedAbility, minimumHealth)`
combines the three native hero-hit queries and conditional health read while keeping
one or three native CString temporaries alive through the health call. The result is
an ordered `GetHealth_API(game, actor) > minimumHealth`; NaN is false. TalkingTrader1
uses ability14 and minimum0.0. A query-only helper followed by a separate health call
releases the native strings too early for this reviewed scope.

W290 owned SDK source/W292 x86 build DLL1549f1e4 and W295 generic lowering are verified
offline; W296 carries that DLL and exact tested Lua. Original651B linked-helper and
46B health API execution cover896 cases, both generated stages1792 bridged cases,
and exact-method SDK-header/sol fixture896 valid calls. The standalone sol fixture
substitutes a ResourceProbe receiver and does not exercise actual resource-host
construction/destruction or full DLL loading/registration.

The contract requires a valid CScriptThing userdata, numeric ability/threshold,
normal child ABI, disjoint storage and default FP environment. Wrong Lua actor types
can fault and malformed numbers can convert; the existing query-only helper reproduces
actor/ability handling. Nil actor, closed resource, null actor vtable and missing
query/health APIs reject cleanly in the standalone source fixture. No claim of
malformed-input safety, native heap/ownership, exception/reentry or gameplay proof.
The SDK remains uncommitted per the user constraint; W295 exports its actual owned
working-tree diff against unchanged HEAD, explicitly including preserved prior edits.
It is not a committed upstream integration patch. No install or game launch occurred.
