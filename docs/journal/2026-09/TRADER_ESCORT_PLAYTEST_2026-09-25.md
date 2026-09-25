# Trader Escort: completed hands-free on v15, 2026-09-25

Continues [2026-09-24](TRADER_ESCORT_PLAYTEST_2026-09-24.md). Bundle v15 with only
`TraderEscort/Entities/DarkwoodTrader.lua` replaced (and its manifest hash), from
`adult_trader_escort_accepted_v15_2026-09-24` via `work/trader_escort/to_post_intro.sh` + `ingame_runner.py`.

## Result

Run **te7**: Darkwood1 -> 6 -> Barrow Fields with all three traders, the camp-trader greeting, the rock troll,
the lead to `M_TradersStopHere`, the end trader's `ENDTRADER_GREETINGS` scene and
`SetQuestAsCompleted('Q_TraderEscort')` (`IsQuestCompleted` true), then creature generators re-enabled and
`QS_GuardianSisterInfo2_SisterInBanditCamp` started. **0 Lua errors** (the 5 `stack traceback` lines are the
sidecar's diagnostic stack at each `RetailMovie start`). No quest state was set by hand in te7; the last hop
(stop marker -> end marker) was sent by hand as a runner route, and the quest file's leg now points there. Checkpoint:
`Saves/adult_trader_escort_completed_v15_2026-09-25`. Boast 9 (all traders alive) was taken; no payout line was
logged, so a won boast's payout is still unseen.

## Fixes

* **3811caa** (converter): DarkwoodTrader's post-greeting `PrepareResource` named an unresolved handle. The typed
  exporter dropped EBX's thing tag at `push ebx` (a PUSH only reads) and at the `lea ebx,[ebx]` loop-head
  padding, so later `GetDataString` calls on the trader fell back to push counting and drifted 16 bytes. Fixed,
  the call prepares the trader's own resource, the heal effect is `HEAL_LEVEL2` (was `""`) and the three
  greeting lines pair speaker and listener as retail does. Verified live (te6): greeting plays, all three follow on.
* **a46c7c8** (lifter): `c_stk_169 = f_stk_20 < fVar19` was typed a number (`RE_NUMERIC_EXPR` admits `<`/`>`
  for shifts), so the native `== '\0'` stayed `== 0`, never true for a Lua boolean. The Barrow Fields traders
  then stopped and said FOLLOW_ME whenever the hero was 9+ units away, even ahead of them. Now a top-level
  compare is a boolean first.
* (lowering, this commit): WatchForMissionRules' fallback end (end trader dead) compared
  `(xStack_b0 - xStack_b0) / 0xc`: the vector's begin/end/capacity slots all reached the fold as the slot's
  earlier actor-map name. A vector minus itself is now `LOCALLIST_Count` (`#vec`).
* Every other unit regenerated with and without each converter fix: no other output changes (A/B, 0 files).

## Runner changes

* Kills by damage: `ModifyThingHealth(e, -N, true)` (the flag is "can kill"; `false` stops at 1 health,
  checked live). A fade-kill skipped the rock troll's `GetHealth(me) == 0` wait, so `TradersShouldBeScared`
  stayed true and the traders never followed again (te6, Darkwood6). A creature killed by damage still reports
  `IsAlive()`, so health 0 counts as dead (te7 counted each corpse again as it slid: 364 "kills").
* `trader_escort.json`: in Barrow Fields the traders lead, so the expected party is 0 there, and the leg is
  `M_EndTheQuestHere` (2643.89, 2167.16); the end trader starts within 3 units of it.

te6 (before the lifter fix) reached Barrow Fields only after `TradersShouldBeScared` was cleared by hand
(the troll's own post-death effects); te7 needed no injection.
