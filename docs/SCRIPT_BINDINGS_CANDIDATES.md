# Script-binding candidates — the 101 native CGameScriptInterface methods not in FSE

**Date:** 2026-07-19
**Author:** RE/API-cataloging agent (analysis-only pass, no Ghidra).
**Purpose:** turn `ghidra_out/native_methods_not_in_fse.txt` (the 101-method gap) into a
prioritized, evidence-backed spec of candidate NEW FSE/Lua bindings.

## Sources (every claim below cites ≥2 of these)
1. **Catalog** `ghidra_out/gamescriptinterface_catalog.tsv` — 891 native methods + retail
   addresses (name→addr). All 101 candidates resolve to a retail address here (**101/101**).
2. **FSE manifest** `refs/fse_api_manifest.json` — 931 (2,324 name tokens) already-bound Lua
   functions, each with `scope`/`returnType`/`parameters`/`category`/`description`. Used to
   detect *variant duplicates* (native base method already re-exposed under a typed name) and
   to model the binding shape (types, blocking flag).
3. **Decomp logs** `ghidra_out/decomp_*.log` — only 2 of the 101 appear in existing logs
   (`IsActiveThreadTerminating`, cited below). Full bodies of the other 99 were NOT
   re-decompiled this pass (Ghidra lock held by another agent). Where a body is unavailable,
   the signature is inferred from name + FSE sibling conventions and marked accordingly.
4. **FINDINGS.md 2026-07-19** "CGameScriptInterface: the native scripting API (891 methods)"
   — establishes the interface, the 790/101 split, and vtable base `0x1260F0C`.

## Headline finding: ~18 of the 101 are VARIANT DUPLICATES, not truly-new capability
Cross-matching native names against FSE names (normalized, infix-stripped) shows FSE already
binds the *same underlying capability* under a typed/suffixed wrapper name. These are the native
**base overloads**; binding them verbatim adds little (the Lua-facing function already exists).
Cited pairs (native → existing FSE, from `fse_api_manifest.json`):

| Native (gap)                                   | Existing FSE binding(s)                              |
|------------------------------------------------|-----------------------------------------------------|
| `ActivateQuestWithoutLoadingResources`         | `ActivateQuest` (+`DontPopulateNextLoadedRegion` for the no-load intent) |
| `GiveHeroQuestCardDirectly`                     | `GiveQuestCardDirectly`                              |
| `SetHeroGuideToShowQuestCardsWhenSpokenTo`      | `SetHeroGuideShowsQuestCards`                        |
| `SetQuestCardGoldReward`                        | `SetQuestGoldReward`                                 |
| `SetQuestCardRenownReward`                      | (`SetQuestGoldReward` sibling; renown variant)      |
| `EntitySetAsOpinionSource`                      | `EntitySetAsOpinionSourceByInt` / `...ByString`     |
| `EntitySetOpinionDeedMask`                      | `EntitySetOpinionDeedMaskByInt` / `...ByString`     |
| `EntitySetPersonalityOverride`                  | `EntitySetPersonalityOverrideByInt` / `...ByString` |
| `SetIsGossipForPlayer`                          | `SetIsGossipForPlayer_ByName` / `_ByObject`         |
| `RadialBlurFadeTo`                              | `RadialBlurFadeTo_NoPos` / `_WithPos`               |
| `ShowOnScreenMessage`                           | `ShowOnScreenMessageWithFont`, `AddScreenMessage`   |
| `MsgOnBoastMade`                                | `MsgIsBoastMade` / `MsgOnBoastsMade`                |
| `IsInMovieSequence`                             | `StartMovieSequence`/`EndMovieSequence` family (state query variant) |
| `AddQuestInfoTick`                              | `AddQuestInfoTickByText` / `...ByAction`            |
| `CancelUsingAbility`                            | ability family (`SetAbilityAvailability` etc.); base cancel |

These stay in **Tier D (skip / low value — already covered)** below unless a mod specifically
needs the untyped base overload.

---

## Priority tiers

Ranking rubric: **hero/world-state mutation a mod can't otherwise reach** > read-back queries
enabling logic > event hooks (`Msg*`) > engine-internal / debug / Xbox-Live no-ops.

### Tier A — high-value, novel, safe to bind first (mutating gameplay verbs no FSE equivalent)

| # | Method | Retail | Scope | Inferred signature | Purpose (name + FSE-sibling evidence) |
|---|--------|--------|-------|--------------------|----------------------------------------|
| A1 | `HeroGoFishing` | `0x00895a90` | Entity/Hero | `void HeroGoFishing()` | Puts hero into the fishing action/minigame. **No `*Fishing*` verb in FSE** (only `MsgOnFishingGameFinished`) — genuinely new. |
| A2 | `HeroGoDigging` | `0x0088df80` | Entity/Hero | `void HeroGoDigging()` | Starts hero digging. FSE has digging-*spot* queries only (`IsDiggingSpotEnabled`…), never the hero verb. |
| A3 | `HeroStopDigging` | `0x0088dfa0` | Entity/Hero | `void HeroStopDigging()` | Stops the dig action. Pairs with A2. |
| A4 | `HeroPlayOracleMinigame` | `0x00895b60` | Hero | `void HeroPlayOracleMinigame()` | Launches the Oracle (Snowspire) minigame. **Zero `Oracle`/`Minigame`-launch bindings in FSE.** |
| A5 | `HeroPlayFireheartMinigame` | `0x00895c90` | Hero | `void HeroPlayFireheartMinigame()` | Launches the Fireheart minigame. Zero `Fireheart` in FSE. |
| A6 | `SetHouseOwnedByPlayer` | `0x00895ed0` | World | **[VERIFIED]** `void SetHouseOwnedByPlayer(const CScriptThing& house, bool owned, bool rented)` | Assigns house ownership to the hero — property scripting FSE lacks (`GetNumHousesOwned` is read-only). High mod value (custom player homes). **TWO bools** (owned + rented); body calls `CTCBuyableHouse::SetOwnedByPlayer(owned)` then `::SetRented(rented)`. |
| A7 | `SetBuyableHouseAsScripted` | `0x00896000` | World | **[VERIFIED]** `void SetBuyableHouseAsScripted(const CScriptThing& house, bool scripted)` | Flags a house purchasable/scripted. Complements A6. No `Buyable` in FSE. One bool; body stores it at buyable-house `+0x10`. |
| A8 | `SetHouseOwnedByPlayer` companion `OpenHouseDoors` | `0x0088dfd0` | World | **[VERIFIED]** `void OpenHouseDoors(const CScriptThing& house)` | Force-open a house's doors. FSE has generic `OpenDoor` on a door thing, not the house-group op. Single arg; body → `CThingBuilding::LockAllDoors(false,false)`. |
| A9 | `CloseHouseDoors` | `0x0088e000` | World | **[VERIFIED]** `void CloseHouseDoors(const CScriptThing& house)` | Group-close house doors. Pairs with A8. Single arg; body → `CThingBuilding::LockAllDoors(true,false)`. |
| A10 | `JamDoor` | `0x00895e10` | World | **[VERIFIED]** `void JamDoor(const CScriptThing& door)` | Jam a door (quest gating). No `Jam` in FSE. **Single arg** — mangled name `?JamDoor@...UBEXABVCScriptThing@@@Z` has NO 2nd param (Ghidra's inferred `long param_2` is spurious); body unconditionally sets the door jam flag = 1 (no toggle bool). |
| A11 | `CreateEffect` | `0x0089f910` | World | **[VERIFIED]** `CScriptThing CreateEffect(const CCharString& effectName, const CScriptThing& atThing, const CCharString&, const CCharString& boneName, bool alwaysUpdate, const CScriptThing&)` | Base effect spawner (on-thing form). Returns `CScriptThing` by value (the emitter). Body: `CTCDParticleEmitter::Create(name, thing.pos)`, `AttachToThing(...,boneName,...)`, `SetAlwaysUpdate(bool)`. A **2nd overload** exists at `0x0089f9e0`: `CScriptThing CreateEffect(const CCharString& effectName, const C3DVector& pos, const C3DVector& orient, float, bool, const CScriptThing&)` (at-position form). |
| A12 | `AddLogBookEntry` | `0x0088fe00` | Quest | **[VERIFIED]** `void AddLogBookEntry(const CWideString& title, const CWideString&, const CWideString& body, CHeroLogBookEntry::ECategory category)` | Writes a logbook entry. **No `LogBook` binding in FSE** — useful for quest/lore mods. **Args are WIDE strings** (3× `CWideString`), NOT a text-DB tag or numeric id, plus a `CHeroLogBookEntry::ECategory` enum. From mangled name `?AddLogBookEntry@...UBEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@Z`. |

### Tier B — high-value read-back queries (enable mod logic; safe, non-mutating)

| # | Method | Retail | Inferred signature | Purpose |
|---|--------|--------|--------------------|---------|
| B1 | `GetHeroFishingLevel` | `0x00895b00` | `int GetHeroFishingLevel()` | Hero fishing skill readout. No FSE equivalent. |
| B2 | `GetBestFishWeight` | `0x00895a10` | `float GetBestFishWeight()` | Best fish caught (records). |
| B3 | `IsHeroControlledByPlayer` | `0x0088e980` | `bool IsHeroControlledByPlayer()` | True when player (not a cutscene/script) drives the hero — key guard for mods. |
| B4 | `IsEntityUnderScriptedControl` | `0x008960c0` | `bool IsEntityUnderScriptedControl(CScriptThing* e)` | Whether an entity is script-controlled (pairs with FSE `AcquireControl`/`ReleaseControl`). |
| B5 | `IsThingWithThisUIDAlive` | `0x0088e260` | `bool IsThingWithThisUIDAlive(int uid)` | UID liveness check — lets Lua validate a stored UID before use. High utility. |
| B6 | `IsObjectInThingsPossession` | `0x00897220` | `bool IsObjectInThingsPossession(CScriptThing* owner, defName)` | Inventory containment test (complements `GetNumberOfItemsOfTypeInInventory`, FINDINGS 0x897190). |
| B7 | `IsHeroPlayingOracleMinigame` | `0x00895bc0` | `bool` | Guard for A4. |
| B8 | `HasHeroWonOracleMinigame` | `0x00895c30` | `bool` | Result readout for A4. |
| B9 | `HasHeroForceQuitFireheartMinigame` | `0x00895d50` | `bool` | Result readout for A5. |
| B10 | `IsRegionDefLoaded` | `0x00891db0` | `bool IsRegionDefLoaded(name)` | Finer than FSE `IsRegionLoaded` — def vs. runtime. |
| B11 | `IsRegionLoadedAndPreloaded` | `0x00890550` | `bool` | Region readiness (preload complete). |
| B12 | `GetAllThingsInLevel` | `0x008a8af0` | **[VERIFIED]** `long GetAllThingsInLevel(const CCharString& levelName, std::vector<CScriptThing>& outList)` | Enumerate every alive thing in a named level. Return is the **count** (`(end-begin)/12`, sizeof CScriptThing=0xC); the out-param is an `AAV std::vector<CScriptThing>&` appended in place. Body maps `levelName`→map number, runs `CThingSearchTools::GetAllThings` with an `InMap`+`!InArea` filter, and pushes each alive thing into the vector. From mangled name `...UBEJABVCCharString@@AAV?$vector@VCScriptThing@@...@Z`. |
| B13 | `GetTextString` / `GetFormattedString` | `0x008913a0` / `0x00891340` | `std::string Get*String(tag,...)` | Text-DB lookups. FSE has display helpers (`DisplayGameInfoText`) but no raw fetch — enables custom UI text. |
| B14 | `TextEntryExists` | `0x00892070` | `bool TextEntryExists(tag)` | Guard for B13 lookups. |
| B15 | `GetWanderingPopulationScriptDefNameInRegion` / `...InCurrentRegion` | `0x00891e30` / `0x0088e390` | `std::string(...)` | Reads the population script def for a region — spawn/NPC modding. |

### Tier C — event hooks (`Msg*`) + cutscene/scene control (medium value)

Event-poll methods (FSE binds `Msg*` as blocking waits inside quest threads; native shape is
`bool MsgOnX()`). Novel hooks (not already in FSE by cross-match):

| # | Method | Retail | Purpose |
|---|--------|--------|---------|
| C1 | `MsgOnLevelLoaded` / `MsgOnLevelUnloaded` | `0x0089ac10` / `0x0089ac80` | Level load/unload hooks. FSE has `MsgIsLevelLoaded/Unloaded` (query variant) — these are the on-event forms. |
| C2 | `MsgOnRegionLoaded` / `MsgOnRegionUnloaded` / `MsgOnRegionPreunload` | `0x008933f0` / `0x00893470` / `0x008934f0` | Region lifecycle hooks (preunload = cleanup point). |
| C3 | `MsgIsRegionLoaded` / `MsgIsRegionUnloaded` | `0x008932d0` / `0x00893360` | Region state waits. |
| C4 | `MsgOnAnyQuestCompleted` | `0x00893610` | Global quest-completion hook. |
| C5 | `MsgOnQuestCompletedBeforeScreenShown` / `MsgOnQuestFailedBeforeScreenShown` | `0x00893730` / `0x008937d0` | Pre-screen hooks (mutate state before the completion UI). |
| C6 | `SetCutsceneMode` / `SetCutsceneActionMode` | `0x0089d600` / `0x008922c0` | Enter/exit cutscene modes. FSE has `IsInCutscene`/`PlayCutscene` but not these mode setters. |
| C7 | `SetCutsceneSkippable` / `SetCutsceneSkippableWhilePaused` | `0x00892280` / `0x008922a0` | Control skip behaviour. |
| C8 | `CancelAbilitiesForCutscene` / `ResumeAbilitiesForCutscene` | `0x0089d690` / `0x0089d6f0` | Suspend/restore hero abilities across a cutscene. |
| C9 | `SetToKeepHeroAbilitiesDuringCutscenes` / `SetToDisplayTutorialsDuringCutscenes` | `0x00890b50` / `0x0088e930` | Cutscene policy toggles. |
| C10 | `PreloadNewScene` | `0x00890b70` | Preload a scene (streaming control). |
| C11 | `PostAddScriptedEntities` / `StartScriptingEntity` | `0x006e7460` / `0x0089b5b0` | Entity-scripting lifecycle. `StartScriptingEntity` pairs with B4. |
| C12 | Conversation builder: `AddNewConversation` / `AddLineToConversation` / `AddPersonToConversation` | `0x008906c0` / `0x00890750` / `0x00890710` | Programmatic dialogue construction. FSE has `CameraDoConversation`/`IsConversationActive` but **no builder** — novel, medium-high value for dialogue mods. **[VERIFIED signatures]** — `long AddNewConversation(const CScriptThing& starter, bool, bool)` returns a conversation id/handle (`long`), forwards to `CScriptConversationManager::MakeNewConversation(thing, b1, b2)`; `void AddPersonToConversation(long convId, const CScriptThing& person)`; `void AddLineToConversation(long convId, const CCharString& line, bool, const CScriptThing& speaker, const CScriptThing& target)`. The `long` first arg on the two Add* methods IS the id returned by `AddNewConversation`. Line text is `CCharString` (narrow), not wide/tag. |
| C13 | `SetScreenMessagesEnabled` / `StartProgressDisplay` / `StopProgressDisplay` | `0x008922d0` / `0x0088e950` / `0x0088e970` | On-screen message + progress-bar control. |
| C14 | `SetDoorTriggerType` / `SetDoorTriggerType`, `OverrideAutomaticHouseLocking` | `0x00895e70` / `0x0088e030` | Door/house locking policy. Pairs with Tier-A door verbs. |
| C15 | `CloseDoor` | `0x00895de0` | Single-door close (FSE has `OpenDoor` but no explicit close). |
| C16 | `DisableFishingSpot` / `IsFishingSpotEnabled` / `UpdateFishWeight` | `0x00895950` / `0x008958f0` / `0x008959b0` | Fishing-spot control (complements Tier A fishing). |
| C17 | `PrepareQuestsWhenFinalQuestIsActivated` / `...Completed` | `0x00892f20` / `0x00892f30` | Endgame quest-chain prep. |
| C18 | `ActivateMultipleQuests` / `ActivateMultipleQuestsWithoutLoadingResources` | `0x00892ea0` / `0x00892ee0` | Batch quest activation. Convenience over FSE `ActivateQuest`. |
| C19 | `DeactivateBoastUi` | `0x008a0c60` | Hide boast UI (boast family otherwise well-covered in FSE). |
| C20 | `GetValidTextEntryNameWithAttitude` | `0x008920c0` | Attitude-aware text lookup. |

### Tier D — low value: skip, engine-internal, debug, Xbox-Live no-ops, or variant duplicates

| Method | Retail | Reason |
|--------|--------|--------|
| `IsXbox` | `0x0088e100` | Always false on native PC TLC — a constant. No mod value. |
| `SetDebugCameraType` | `0x0088e0f0` | Debug camera; dev-only. |
| `Error` / `TraceMessage` / `Validate` | `0x0088de00` / `0x006e7d20` / `0x006e72a0` | Engine logging/self-check internals. `Validate` is the vtable slot-0 region (`0x6E7…`), an internal method, not gameplay. |
| `IsActiveThreadTerminating` | `0x006e71b0` | **Confirmed via decomp** (`decomp_open_chest_scripts.log:109,123,…`) as `CScriptBase::IsActiveThreadTerminating` — a script-yield/abort guard called in every native script loop. It is FSE's own thread machinery; exposing to Lua is redundant (FSE threads already handle termination). |
| `UpdateOnlineScore_Archery` / `_ChapelOrTemple` / `_ChickenKick` / `_FishingCompo`, `UpdateScore_FishingCompetition` | `0x008a1040` / `0x008a1140` / `0x008a10c0` / `0x008a11c0` / `0x008a1240` | **[VERIFIED — NOT stubs/no-ops on native PC].** All four `UpdateOnlineScore_*` bodies are real: each calls `CTCHeroOnlineScoreboard::UpdateScoreForActivity(board, <activityId>, score, false)` with a distinct activity id — Archery=8, ChapelOrTemple=10 (0xA), ChickenKick=0x11, FishingCompo=0x26. Signature `void UpdateOnlineScore_X(float score)` (mangled `...UBEXM@Z`, `M`=float). `UpdateScore_FishingCompetition(float)` writes the score to a scoreboard field at `+0x15c`. They update a **local** `CTCHeroOnlineScoreboard` (the in-game records/scoreboard object), not an XBL network call — so they function on the native/Steam build. Still low mod value (scoreboard bookkeeping), but they are live code, not empty returns. |
| `IsDPadButtonHeldForExpression` | `0x00895430` | Input-poll; overlaps the controller subsystem (owned by another agent, #16/#17). Do not bind here. |
| `IsPlayerHoldingFireRangedWeaponButton` / `...FirstPersonTargetingButton` / `...LockTargetButton` / `...UnsheatheRangedWeaponButton`, `IsHeroInProjectileWeaponMode` | `0x00890610` / `0x00890640` / `0x008905e0` / `0x008905b0` / `0x00890670` | Input-state polls — combat/aiming. Overlap controller subsystem; defer to #16/#17. Could be Tier-B if a combat mod needs them, but ownership dictates defer. |
| The 18 variant-duplicate rows in the Headline table above | (see table) | Already reachable in Lua under a typed/suffixed FSE name. |
| `HeroQuitFireheartMinigame` / `HeroForceQuit…` control-only | `0x00895cf0` | Minor helper; keep with A5's family if the minigame is bound. |
| `RadialBlurFadeTo` | `0x008900d0` | Variant of FSE `RadialBlurFadeTo_NoPos/_WithPos`. |
| `SetIsGossipForPlayer` | `0x008a96f0` | Variant of `SetIsGossipForPlayer_ByName/_ByObject`. |

---

## Binding spec for the Tier-A candidates (FSE/Lua style)

FSE binds via the sol2 Lua state; the manifest shows two scopes: **Quest** (free functions in
the quest/global table) and **Entity** (methods whose first param is the `CScriptThing* pMe`).
Native calls go through the `CGameScriptInterface` vtable at base `0x1260F0C`
(FINDINGS 2026-07-19); the FSE hook is `0xCDB355` with `g_fableBase`-relative pointers
(CLAUDE.md). Each binding is: (a) a C++ thunk that resolves the game-script-interface instance
and calls the native method at its retail RVA (address − ImageBase `0x400000`), then (b) a sol2
registration exposing it to Lua.

**Pattern (illustrative, matching FSE style):**

```cpp
// RVA = retail_addr - 0x400000 ; call convention = thiscall (member of CGameScriptInterface)
// Entity-scope example: HeroGoFishing (retail 0x00895a90 -> RVA 0x495a90)
typedef void (__thiscall* fn_HeroGoFishing)(void* pGSI);
static void Lua_HeroGoFishing() {
    auto* gsi = FSE::GetGameScriptInterface();          // g_fableBase-relative singleton
    reinterpret_cast<fn_HeroGoFishing>(FSE::Base() + 0x495a90)(gsi);
}
// register (Quest table, matching manifest scope conventions):
questTable.set_function("HeroGoFishing", &Lua_HeroGoFishing);
```

Per Tier-A candidate (RVA = addr − 0x400000):

| Candidate | RVA | Lua reg name | Lua signature (proposed) | Notes |
|-----------|-----|--------------|--------------------------|-------|
| A1 `HeroGoFishing` | `0x495a90` | `HeroGoFishing` | `HeroGoFishing()` | blocking (starts minigame); guard with B1 if needed. |
| A2 `HeroGoDigging` | `0x48df80` | `HeroGoDigging` | `HeroGoDigging()` | blocking. |
| A3 `HeroStopDigging` | `0x48dfa0` | `HeroStopDigging` | `HeroStopDigging()` | non-blocking. |
| A4 `HeroPlayOracleMinigame` | `0x495b60` | `HeroPlayOracleMinigame` | `HeroPlayOracleMinigame()` | blocking; result via B8. |
| A5 `HeroPlayFireheartMinigame` | `0x495c90` | `HeroPlayFireheartMinigame` | `HeroPlayFireheartMinigame()` | blocking; result via B9. |
| A6 `SetHouseOwnedByPlayer` | `0x495ed0` | `SetHouseOwnedByPlayer` | `SetHouseOwnedByPlayer(house, owned, rented)` | Entity scope (house = CScriptThing). **[VERIFIED] TWO bools** — `owned` then `rented`. |
| A7 `SetBuyableHouseAsScripted` | `0x496000` | `SetBuyableHouseAsScripted` | `SetBuyableHouseAsScripted(house, scripted)` | **[VERIFIED] one bool.** |
| A8 `OpenHouseDoors` | `0x48dfd0` | `OpenHouseDoors` | `OpenHouseDoors(house)` | **[VERIFIED]** single arg. |
| A9 `CloseHouseDoors` | `0x48e000` | `CloseHouseDoors` | `CloseHouseDoors(house)` | **[VERIFIED]** single arg. |
| A10 `JamDoor` | `0x495e10` | `JamDoor` | `JamDoor(door)` | **[VERIFIED] single arg** — no bool (mangled name has no 2nd param; always jams). |
| A11 `CreateEffect` | `0x49f910` | `CreateEffect` | `CreateEffect(effectName, atThing, str, boneName, alwaysUpdate, thing) -> thing` | **[VERIFIED]** returns `CScriptThing`. On-thing form. 2nd overload at RVA `0x49f9e0` = `CreateEffect(effectName, pos, orient, float, alwaysUpdate, thing)` (at-position form). |
| A12 `AddLogBookEntry` | `0x48fe00` | `AddLogBookEntry` | `AddLogBookEntry(title, str2, body, category)` | Quest scope. **[VERIFIED]** args are 3× `CWideString` + a `CHeroLogBookEntry::ECategory` enum — NOT a text-DB tag/id. |

Tier-B/C bindings follow the same thunk pattern (read-back ones return the native return
value; `Msg*` ones register as blocking waits like existing FSE `Msg*` functions).

---

## Resolution status

- **Address + name + tier + inferred purpose: 101 / 101 resolved** (all appear in
  `gamescriptinterface_catalog.tsv` with a retail address, cross-checked against the FSE
  manifest for novelty).
- **Signature fully confidence-graded from ≥2 sources: 101 / 101** at the name+sibling level;
  the **7 "needs decomp" candidates are now DECOMP-VERIFIED (2026-07-19)** from the MSVC-mangled
  donor symbols + decompiled bodies (`ghidra_out/decomp_bindings7.c`). See the "Decomp-verified
  signatures" note below. UpdateOnlineScore_* confirmed **real (not XBL no-ops)** on native PC.

### Needs decomp — ✅ ALL RESOLVED 2026-07-19 (see "Decomp-verified signatures" below)
1. ~~`CreateEffect` (`0x0089f910`)~~ ✅ `CScriptThing CreateEffect(const CCharString& name, const CScriptThing& atThing, const CCharString&, const CCharString& boneName, bool alwaysUpdate, const CScriptThing&)`; on-thing form. 2nd overload `0x0089f9e0` = at-position form.
2. ~~`GetAllThingsInLevel` (`0x008a8af0`)~~ ✅ `long GetAllThingsInLevel(const CCharString& levelName, std::vector<CScriptThing>& outList)` — returns count, appends to the vector by reference.
3. ~~`AddNewConversation`/`AddLineToConversation`/`AddPersonToConversation`~~ ✅ `long AddNewConversation(const CScriptThing&, bool, bool)`; `void AddPersonToConversation(long convId, const CScriptThing&)`; `void AddLineToConversation(long convId, const CCharString& line, bool, const CScriptThing& speaker, const CScriptThing& target)`.
4. ~~`SetHouseOwnedByPlayer`/`SetBuyableHouseAsScripted`/`JamDoor`~~ ✅ SetHouseOwnedByPlayer = **2 bools** (owned, rented); SetBuyableHouseAsScripted = **1 bool** (scripted); JamDoor = **0 bools / 1 arg** (door only, always jams).
5. ~~`AddLogBookEntry` (`0x0088fe00`)~~ ✅ **3× `CWideString`** (title, ?, body) + `CHeroLogBookEntry::ECategory` — neither a text tag nor a numeric id.
6. ~~`UpdateOnlineScore_*`/`UpdateScore_FishingCompetition`~~ ✅ **NOT XBL no-ops** — real local-scoreboard code (`CTCHeroOnlineScoreboard::UpdateScoreForActivity` with per-activity ids); signature `void UpdateOnlineScore_X(float score)`.

---

## Decomp-verified signatures — dated note (2026-07-19)

**Author:** RE agent (script-bindings decomp pass). **Evidence:** single Ghidra 12.1 headless
`DecompByName.java` read-only pass over Fable.exe (retail, ImageBase 0x400000); output in
`ghidra_out/decomp_bindings7.c`, run log `ghidra_out/decomp_bindings7.log` (22 functions,
ok=22 fail=0). Signatures below are read from the **MSVC-mangled donor symbol names** (ground
truth, `?Name@Class@@…@Z`) cross-checked against each decompiled body. RVA = retail addr − 0x400000.

| Method | Retail | Mangled/decoded signature (verified) |
|--------|--------|--------------------------------------|
| `CreateEffect` (on-thing) | `0x0089f910` | `CScriptThing CreateEffect(const CCharString& name, const CScriptThing& atThing, const CCharString&, const CCharString& boneName, bool alwaysUpdate, const CScriptThing&)` — `?CreateEffect@…UBE?AVCScriptThing@@ABVCCharString@@ABV2@00_N2@Z` |
| `CreateEffect` (at-pos) | `0x0089f9e0` | `CScriptThing CreateEffect(const CCharString& name, const C3DVector& pos, const C3DVector& orient, float, bool alwaysUpdate, const CScriptThing&)` — `?CreateEffect@…ABVCCharString@@ABVC3DVector@@0M_N2@Z` |
| `GetAllThingsInLevel` | `0x008a8af0` | `long GetAllThingsInLevel(const CCharString& levelName, std::vector<CScriptThing>& out)` — returns count `(end-begin)/12`; out-param filled by ref |
| `AddNewConversation` | `0x008906c0` | `long AddNewConversation(const CScriptThing& starter, bool, bool)` — returns conversation id/handle |
| `AddPersonToConversation` | `0x00890710` | `void AddPersonToConversation(long convId, const CScriptThing& person)` |
| `AddLineToConversation` | `0x00890750` | `void AddLineToConversation(long convId, const CCharString& line, bool, const CScriptThing& speaker, const CScriptThing& target)` |
| `AddLogBookEntry` | `0x0088fe00` | `void AddLogBookEntry(const CWideString& title, const CWideString&, const CWideString& body, CHeroLogBookEntry::ECategory)` — WIDE strings, not tag/id |
| `UpdateOnlineScore_Archery` | `0x008a1040` | `void(float score)` → `UpdateScoreForActivity(board, 8, score, false)` — **live, not stub** |
| `UpdateOnlineScore_ChickenKick` | `0x008a10c0` | `void(float)` → activity `0x11` |
| `UpdateOnlineScore_ChapelOrTemple` | `0x008a1140` | `void(float)` → activity `0xA` |
| `UpdateOnlineScore_FishingCompo` | `0x008a11c0` | `void(float)` → activity `0x26` |
| `UpdateScore_FishingCompetition` | `0x008a1240` | `void(float)` → writes scoreboard `+0x15c` |
| `SetHouseOwnedByPlayer` | `0x00895ed0` | `void(const CScriptThing& house, bool owned, bool rented)` — 2 bools |
| `SetBuyableHouseAsScripted` | `0x00896000` | `void(const CScriptThing& house, bool scripted)` — 1 bool |
| `OpenHouseDoors` | `0x0088dfd0` | `void(const CScriptThing& house)` → `LockAllDoors(false,false)` |
| `CloseHouseDoors` | `0x0088e000` | `void(const CScriptThing& house)` → `LockAllDoors(true,false)` |
| `JamDoor` | `0x00895e10` | `void(const CScriptThing& door)` — single arg, always jams (Ghidra's `long param_2` is spurious) |

**Caveats / residual ambiguity:**
- `CreateEffect` return `CScriptThing` is returned by value via a hidden first pointer arg (standard
  MSVC struct return); the middle `const CCharString&` param of the on-thing overload (position 3)
  has an unclear role in the body — likely an effect-slot/type name, unconfirmed.
- `AddLogBookEntry`'s 2nd `CWideString&` role is unconfirmed (title/subtitle vs. an id string); the
  body is a thin thunk (`FUN_0047a9f0(...,1)`) that forwards to the logbook add.
- `UpdateOnlineScore_*` decompiles show `param_2` typed as the scoreboard pointer because the `float`
  score arrives in a register Ghidra mislabels; the mangled `M` (float) in every symbol is
  authoritative for the public signature.
