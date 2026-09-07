# Fable TLC Quest-Card System — definitive model (decompile-backed)

The quest-card API is a contiguous `CGameScriptInterface` vtable group (slots
292–299). A card lives in **two places**: the **Guild "available cards" list**
(what the Logbook renders) and, transiently, as a **card *thing*** coupled to
quest activation. The two give-functions target different halves — which is the
whole story behind "card gives ok=true but never shows."

## The two give paths (the crux)

### `AddQuestCard(cardObjName, questName, replayable, skipGMmsg)` — vtable[292] @0x008913F0
The **register-by-name** path. Calls `FUN_004b1670`:
- reads the `CQuestManager` guild **available-quest-cards `std::list`** (`this+0x58`),
- finds `cardObjName`; **if absent, appends `{cardObjName, questName, replayable}`
  and sets the dirty byte (+0x8F)**.
- **No card thing created, no quest activation, no kill.** Pure list insert.
The Logbook renders from this list, resolving `cardObjName → OBJECT def → its inline
`<CQuestCardDef>` component` (QuestName/QuestSummary/RegionName TextIDs) at draw
time. **This is why it reliably displays.**

### `GiveHeroQuestCardDirectly(cardObj, questName, flag)` — vtable[295] @0x008968C0
The **quest-activation-coupled** path — NOT a Logbook add. It:
1. `CThingObject::Create(cardObjectDefIndex, …)` — makes a **transient** card thing.
2. sets the card's script-quest name to `questName` (`(tc+0x28) = param_3`).
3. `CQuestManager::ActivateQuestCard(card, true)` @0x004b4aa0.
4. **If ActivateQuestCard returns false → `CThing::Kill(card)` — the card is destroyed.**

`ActivateQuestCard` succeeds only when `FUN_004b4a10(card+0x28, …)` →
`CQuestManager::ActivateMultipleQuests` can activate a quest **whose name equals the
card's RegionName field**, and `IsActiveQuestBetweenQuestStartAndFinishScreensInRegion`
gates it. So this is the **"start the quest via its card"** path (a real quest-start
card). If the named quest isn't an activatable card-region, it **silently kills the
card** → invisible. The Lua `ok=true` only means the pcall didn't throw; the engine
fn returns void and self-destructs the card on failure.

**So:** we called `GiveHeroQuestCardDirectly` with `"FSE_Master"` (not an activatable
card-region) → card created, activation failed, card Killed → nothing shown. **Wrong
function for "show a card"; it's for "activate this quest through its card."**

The first argument is an `OBJECT_QUEST_CARD_*` definition name. ForgeFSE formerly
called it `textDBEntry` in its C++ header, but retail resolves that string to an
object-definition index before calling `CThingObject::Create`; passing `TEXT_*`
there is therefore incorrect.

## The rest of the group
| Slot | Fn | Role |
|---|---|---|
| 292 `0x490` | `AddQuestCard` | register available card → Guild/Logbook |
| 293 `0x494` | `RemoveQuestCardFromGuild` | remove from guild list |
| 294 `0x498` | `RemoveQuestCardFromHero` | remove from hero's held |
| 295 `0x49C` | `GiveHeroQuestCardDirectly` | create card thing + activate quest (or kill) |
| 296 `0x4A0` | `SetQuestCardObjective(quest, objText, r1, r2)` | objective line on the card |
| 297 `0x4A4` | `SetQuestCardGoldReward(quest, amt)` | displayed gold |
| 298 `0x4A8` | `SetQuestCardRenownReward(quest, amt)` | displayed renown |
| 299 `0x4AC` | `RemoveAllAvailableQuestCardsFromGuild` | clear guild board |
| 544 `0x880` | `SetHeroGuideToShowQuestCardsWhenSpokenTo` | guide-shows-cards toggle |

## The working recipe (validated, deployed)
```lua
Quest:AddQuestCard("OBJECT_DUMMY_QUEST_CARD_DEFEAT_SNOW_TROLL", "FSE_Master", false, false)
```
- arg1 = card OBJECT def name (must exist in game.bin — our overwrite provides it; def **needs no field change** to appear — it's a real Create-able card).
- arg2 = host quest name (**stored, not validated at add-time**; the MASTER quest FSE_Master is fine — no `S_ALWAYS_ACTIVE` helper quest required, that shape is only needed if the host's `Main()` doesn't reliably run).
- arg3 = replayable flag (cosmetic, stored in the list node).
- arg4 = `false` → also flashes the "new quest" GM HUD popup (`TEXT_QST_078_GM_MSG_NEW_QUEST`); `true` → silent.
No ForgeFSE DLL change needed — `AddQuestCard` is already bound (vtable[292]).

For a quest with runtime objective/reward text, registration and population are
two separate phases. The setters resolve the **active** `CTCQuestCard` by script
name, so calling them immediately after `AddQuestCard` while the card is merely in
the Guild list is a no-op:

```lua
-- Registration phase: makes the compiled card asset available.
Quest:AddQuestCard("OBJECT_QUEST_CARD_MY_CUSTOM_QUEST", "MyCustomQuest", false, true)

-- Activation phase: creates/resolves the runtime card thing.
Quest:ActivateQuest("MyCustomQuest")
Quest:Pause(0.1)
if not Quest:NewScriptFrame() then return end

-- Runtime-population phase: these now have an active card to update.
Quest:SetQuestCardObjective("MyCustomQuest", "TEXT_MY_CUSTOM_QUEST_OBJECTIVE",
                            "Oakvale", "")
Quest:SetQuestGoldReward("MyCustomQuest", 500)
Quest:SetQuestRenownReward("MyCustomQuest", 100)
Quest:KickOffQuestStartScreen("MyCustomQuest", false, false)
```

Use `Quest:` methods in Lua; these functions are methods on the quest-state object,
not bare global functions.

## Facts worth remembering
- **No `DUMMY_`-prefix hide filter exists** — the prefix is naming convention only.
- `AddQuestCard` associates card↔quest **purely by name string**; the OBJECT needs no back-link to the quest.
- `FUN_004b1670` **de-dups by card name** — if a prior run already added it to a persisted save's guild list, re-adding is a no-op (still displays, no second popup).
- If a card still doesn't appear after `AddQuestCard`, the next suspect is the card's `<CQuestCardDef>` `RegionName` / `IsCoreQuest` / `InventoryCategory` (which Logbook tab it files under: Current vs Available vs Completed) — **not** the give path.
- A full custom quest presents as: `AddQuestCard` (register) → `ActivateQuest`
  (create/resolve runtime card) → `SetQuestCardObjective` + reward setters (fill)
  → `AddQuestInfoBar/Counter/Timer` (live HUD tracking) →
  `TellHeroQuestObjectiveCompleted` (close).

Evidence: Ghidra headless of retail Fable.exe funcs 0x008913F0 / 0x004b1670 /
0x008968C0 / 0x004b4aa0 / 0x004b4a10; ForgeFSE bindings GameInterface.cpp:1223-1226,
LuaQuestState.cpp:269,278; official FSE `MyFirstQuest.lua` (`AddQuestCard` usage);
def schema chocolatebox_def.xml:2936.
