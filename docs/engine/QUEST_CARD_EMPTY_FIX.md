# Custom quest card shows EMPTY values — root cause + fix

*From engine decomp analysis 2026-07-21 (`quest_card_decomp.txt`, 137 fns). HIGH confidence.*

## ROOT CAUSE — runtime-population gap, NOT a def/schema bug
The engine sources OBJECTIVE, GOLD and RENOWN from the **runtime per-quest card THING**
(CTCQuestCard: objective at +0x1c, gold at +0x2c) + the manager's runtime quest list
(DAT_013b89fc+0x6c) — written ONLY by the script setters **SetQuestCardObjective /
SetQuestCardGoldReward / SetQuestCardRenownReward**. Those first call
**GetActiveQuestCardFromScriptName** (resolves a card only if an ACTIVE card THING exists for
that quest script name, matched via card+0x28). `forge quest card` emits only the static
`CQuestCardDef` + OBJECT clone and **no companion quest/script**, so at display time no active
card is populated and `ConstructQuestList` (param_3==1) renders blank objective/reward.

**TITLE and SUMMARY DO come from the static def** (GetQuestName reads def+0x30, GetQuestSummary
def+0x2c) — so forge's QuestName/QuestSummary fields ARE load-bearing and render — UNLESS a
**zero/out-of-range TextID** was passed: `forge` writes `--quest-name/--quest-summary` verbatim
with no zero-guard and no symbol resolver, so `--quest-name 0` silently bakes a blank title.

## Fix
1. Do NOT change the def-write logic — the engine cannot read objective/gold/renown from the
   static def. The def+OBJECT `author()` emits is correct (title/summary/IsCore render from it).
2. Add a RUNTIME-REQUIRED note to `questcard.hpp` (l.28-36): baked objective/reward do NOT drive
   the Current-quests screen; they show only after a quest script calls the setters against an
   ACTIVE card.
3. Emit (or docs mandate) a **companion quest script**: AddQuestRegion -> AddQuestCard(
   objectName='OBJECT_QUEST_CARD_<new>', questScriptName,...) -> activate (ActivateQuestCard, or a
   quest between start/finish screens in its region so GetActiveQuestCardFromScriptName resolves
   non-null) -> SetQuestCardObjective/GoldReward/RenownReward(questScriptName, ...).
4. Guard the numeric TextIDs: in `apps/forge/main.cpp` asI32 for --quest-name/--quest-summary/
   --success-summary, reject 0/invalid ("TextID 0 renders blank; omit to inherit donor text or
   pass a valid text.big id"). Better: add a TEXT_* symbol resolver (CRC-hash -> text.big TOC).
5. Document that these flags take a NUMERIC text.big TextID (forge has no resolver yet); the safe
   default is to OMIT them and inherit the donor's text.

## Tooling status (2026-07-22)

- FQT's normal start-screen generator now registers with `AddQuestCard`, activates
  the quest, then reapplies objective/gold/renown to the active runtime card.
- The generator no longer automatically follows registration with
  `GiveQuestCardDirectly`; that path creates an activation-coupled transient card
  and can kill it on failure.
- ForgeFSE now names the direct-delivery argument `questCardObjectName` in its
  header and generated manifest. The former `textDBEntry` name was misleading;
  retail resolves an object definition there.

## Validation (byte-level, no in-game run)
`forge quest card <root> <schema> TESTCARD --donor OBJECT_QUEST_CARD_WASP_MENACE --gold 500
--renown 200 --core 1` then `forge defs decode <out> def_schema.json <idx>` and assert: clean
decode (leftover 0), GoldReward==500, RenownReward==200, IsCoreQuest==true, QuestName/Summary =
the donor's valid ids (non-zero). Confirms the def is written right; the emptiness is the runtime
gap, fixed by the companion script.
