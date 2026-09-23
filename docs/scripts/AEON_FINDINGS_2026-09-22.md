# Notes for Aeon — 2026-09-22

Since the 2026-09-20 share. Ordered by what is most likely to matter to you.

---

## 1. A real bug in LUAGameflow: `CoreQuestWaiting` is a ulong, not a bool

**This one bites saves, so it is first.**

`LUAGameflow.OnPersist` does:

```lua
local coreQuestWaiting = Quest:GetStateBool("CoreQuestWaiting") or false
coreQuestWaiting = Quest:PersistTransferBool(context, "CoreQuestWaiting", coreQuestWaiting)
```

Retail persists that field as a **ulong**. Two independent sources:

* Retail's own `Gameflow::OnPersist` at **0x00CEF8E0** calls four different `Transfer<T>` instantiations.
  The `CoreQuestWaiting` site (0x00CEF921) targets **0x004106F0**, which ForgeFSE itself binds as
  `CPersistContext_Transfer_uint` (`FableAPI.cpp:531`; bool is 0x4045C0, int 0x410BE0). Careful: the bsim
  label on all of them reads `CPersistContext::Transfer<signed_char>` — that name is propagated across the
  byte-identical 27-byte family and is wrong.
* The PDB types the member directly: `CGameflowScript::CoreQuestWaiting` is `ulong`
  (`ghidra_out/struct_layouts_egor.tsv:54364`).

**What it costs:** a save written by a bool-persisting Gameflow cannot be read by a ulong-persisting one.
Loading a v6-written save under our converted Gameflow killed the process *inside* the transfer, twice,
with no log line after `Transferring uint 'CoreQuestWaiting'`. Proven both directions: patching ours back
to bool loads that save fine, and unmodified ours loads a **retail-born** save (one no Lua Gameflow ever
touched) cleanly. So retail's schema is the ulong one and the bool is the deviation.

Retail also transfers two more fields your port does not: `SavedScriptNames` and `SavedCardDefNames`
(both `vector<CCharString>`, slot 0x0049B8D0, at `this + 0x4c` and `this + 0x58`).

## 2. ForgeFSE binding gaps we hit (all three are yours to judge)

| binding | status |
|---|---|
| `MsgIsRegionUnloaded` | The API pointer is already resolved (`GameInterface.cpp`, `pVTable[21]`, typedef takes `const CCharString*`) but it is never registered on the Lua usertype. One-line addition. `V_TourGuide` needs it |
| `IsDistanceFromThingToPositionUnder` | Genuinely absent — only `IsDistanceFromPositionOver` is bound. `not Over(d)` is **not** `Under(d)` at the boundary (retail's Over is `dist > d`, Under is `dist < d`, both false at equality), so it needs a real binding. `V_GuildMaster` needs it |
| `EntitySetPersonalityOverride` | NOT a gap — you bind it as `...ByInt` / `...ByString`. This was our converter looking up the retail name and giving up; fixed on our side |

## 3. Guild Training plays start to finish on converted Lua

The whole chain — woods → melee → Skill (static + moving) → Will → adulthood → the final woods test →
the ceremony → the experience orb → `SetQuestAsCompleted Q_GuildTraining` — runs hands-free in one
launch: **58/58 steps, zero Lua runtime errors**. Also covered since: the apple girl end to end, and the
melee secret trainer's own test.

Two behaviours that look like bugs and are not:

* **The Will test's HUD counter is an arrow.** Retail's own Will Guildmaster (0x00D5E0C0) pushes
  `"HUD_ICON_ARROW"` at 0x00D5F275, then `"HUD_CLOCK_ICON"` for the timer; `WillApprentice` (0x00D4EFE0) does the same at 0x00D4FBF6. Lionhead reused the arrow icon rather than
  authoring a Will one.
* **The secret trainers refuse mid-test.** `CombatApprentice`, `SkillApprentice` and `WillApprentice` all
  check `GetMasterGameState("HeroTakingGuildTest")` and answer with a brush-off line while the
  Guildmaster's own test is running. Their tests are additionally gated on
  `Global{Melee,Skill,Will}Grade` being non-zero — grade 0 is the "come back later" branch.

## 4. New conversions since the last share

| unit | functions | missing | notes |
|---|---|---|---|
| `Q_WaspBoss` | 46 | 0 | all nine entity classes (QueenHornet, HornetDrone, WaspChaser, WaspChaseWoman, WaspAttacker, WaspVictim, WaspHelper, FleeingWoman, GratefulVillagerSpawn) |
| `QS_GuardianSisterInfo` (+ `…SisterInBanditCamp`) | 14 | 0 | tiny; all the content is the `MazeAtTavern` entity |
| `V_TourGuide`, `V_GuildMaster`, `GameflowAssistance`, + 7 ambient `V_*` | ~200 | 0–2 | converted EVIDENCE, not finished scripts — the village family runs ~5–15 unconverted statements per function vs ~0.1 for quest scripts |

**The wasp one is worth cross-referencing against your hand port** — you did that quest, so any
disagreement between the two is evidence about one side or the other. That is exactly how item 1 above
surfaced.

## 5. The order between Guild Training and Orchard Farm, from retail's Gameflow

Straight out of the converted `Gameflow` (stages 0 → 400), in case it helps you sequence:

* stage 100 — waits on `Q_GuildTraining`, then `AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss")`
  plus three dummy cards; activates `GameflowAssistance`, `CS_OakValeRevisited`, `V_BeggarAndChild`,
  `V_GuildMaster`, `Q_OrchardFarm_Barricade`, `V_SickChild`, `V_BookCollecting`, `V_ChickenKicking`,
  `V_Bordello`
* stage 200 — waits on `Q_WaspBoss`, hands `QS_GuardianSisterInfo` directly, activates `V_TourGuide`
* stage 300 — waits on `QS_GuardianSisterInfo`, then adds **both** Orchard Farm cards
* stage 400 — polls `IsQuestActive` on either; accepting one removes the other

`Q_OrchardFarm_Barricade` has no script class — it is a resource-section quest the raid activates.

## 6. Harness, if you want it

The autopilot drives a run hands-free off the exec channel: checklists of `do` lines (channel Lua, or real
keyboard/mouse through the game window) with an `expect` regex and a `forbid` (`LUA RUNTIME ERROR`). Two
hard-won rules are in there:

* never teleport the hero ONTO an NPC you then want to talk to — standing inside him means the game cannot
  highlight him, so there is no talk target and `IsTalkedToByHero()` never fires. Stand beside and face him.
* region changes need `GoToMapSlotRetailTransition`, armed once per crossing.
