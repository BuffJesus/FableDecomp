# 2026-09-19 — GuildTrainingWoodsMelee dies on entering Guild Woods (converter), and what it uncovered

Aeon (Discord, 07:53): "neither me or my klankers are able to understand why GuildTrainingWoodsMelee is
crashing when entering Guild Woods". His port and ours are different files, but our converter's
`Q_GuildTrainingWoodsMelee` had a hard failure at *exactly* that seam, in both the draft and the shipped
readable (`work/AeonShare-2026-09-18.zip`, the v5 bundle). Offline session; no in-game run (user away).

## The seam, natively

`Q_GuildTrainingWoodsMelee` is activated by the quest card (`GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_
TRAINING_KILL_BEETLES", ...)` in PreMelee's TheRealGuildmaster). Its `Main` (0xD66620) sets four state bools,
then spins on `IsLevelLoaded("GuildWoods")`. Past that gate — i.e. the moment the hero enters Guild Woods —
it binds `ScorpionHome`, `FinalizeEntityBindings`, spawns `WatchForTermination` and `DoMission`, and waits
on `ScorpionsAlive`. `ScorpionHome.Main` (0xD67270) then registers a timer, adds `HUD_BEETLE_ICON` counter
(`GUI_MeleeBeetles` = 10.0 from script.bin), and spawns `CREATURE_GUILD_STAG_BEETLE` at the furthest
`ScorpionSpawn` while fewer than 3 `GuildScorpions` are alive. All of those bindings exist in ForgeFSE;
`ReadGlobalGameDataFloat` is a documented sidecar binding.

## Bug 1 — `nil & 2` in `Main`, right after `FinalizeEntityBindings`

The readable had:

    local scratchValue
    ...
    quest:CreateThread("WatchForTermination")
    if scratchValue & 2 ~= 0 then scratchValue = scratchValue & 0xfffffffd end
    quest:CreateThread("DoMission")

Lua 5.4: "attempt to perform bitwise operation on a nil value" (proved with lupa). `Main` dies before
`DoMission` is ever created. That is VC7.1's exception-state flag (which stack temporaries are constructed,
for the unwinder). `drop_eh_state_flags` already removes that flag when it is a plain `uint` register, but the
typed export spells it through the *stack slot a string temporary reuses afterwards*:

    xStack_4 = (CCharString)0x0;          // the flag, in the slot "DoMission" is later constructed in
    CVar4 = xStack_4;
    if (((uint)xStack_4 & 1) != 0) { CVar4 = (CCharString)((uint)xStack_4 & 0xfffffffe); ~CCharString(&xStack_4); }
    CVar4 = (CCharString)((uint)CVar4 | 2);
    CCharString::CCharString(&xStack_4,"DoMission",-1);

None of that matched the flag shapes (casts, `CCharString` declaration, the slot→register copy). The pass
now: drops the casts for names that take part in bit ops (ONLY those — a `(CCharString)0x0` movie-handle
init in BirdKiller must stay, the refcount-release fold depends on it), migrates the slot's flag-phase lines
(everything naming it before its first `&slot`) onto the register, accepts `CCharString` in the declaration
shape and `= 1` as an init. Then the existing machinery deletes the register wholesale. Same fix covers
`DoMission` (`CVar3 = 0x0 ... if (0x0 & 1)`) and GuildTrainingWoodsDeparture.

## Bug 2 — `DoMission` re-polled `IsLevelLoaded("")`

The wait loop's tail re-check had lost its operand. Native has `"GuildWoods"` at both sites (0xD66CDC,
0xD66D29). Cause: `_text_order_sites` (the exact callOrder pairing of printed calls to export sites) could
not find `NScript::CQ_CinemaTestScript::EndMission` in the text, because Ghidra prints a member of the
enclosing namespace as `CQ_CinemaTestScript::EndMission(`. 28 heads ≠ 29 sites → the whole function fell
back to address-order pairing, and the seven `CCharString` constructors were paired with the wrong sites
(the two `aCStack_4` empty-string temporaries and the rotated-loop tail shuffled). Added the progressive
namespace-strip fallback. Across the three units the pairing now fails for 2 functions instead of 3; the
remaining two are decompiler-side (0xD52E90 prints 260 vtable heads for 291 sites).

Latent in practice (Main gates on the level first), but the loop would never have re-armed.

## What the fix shook loose (each checked against the C)

- **The EH-flag copy fold was lossy.** `g = flag | k; ...; flag = g;` deletes the set line and re-emits it at
  the copy back — but when the window holds no copy back (a chain of sets, or a goto past the window) the
  bit is simply lost. Harmless when the flag is then deleted wholesale; when the shape check fails it left
  TC_BanditFighter reading `CVar6 & 8` on a path where `CVar6` was never assigned. The casts used to hide
  those lines from the fold; normalising exposed it. Now: track `lossy`, and on a failed shape check revert
  to the unfolded text only if the fold was lossy (FinalMaze's non-lossy fold keeps its pristine shape;
  TraderToRescue gets +15 flag-noise lines next to a pre-existing `unaff_EBP | 3` nil read).
- **Int counters in string-typed slots.** The existing renamer (`X = (CCharString)((int)X + k)` → `ctr_NN`)
  did not know the round-robin form `X = (CCharString)(((int)X + 1) % 5)`, and never touched the counter's
  own `== (CCharString)0x0` compare, which lifted to `== nil`. MeleeOpponent's block-help alternation was
  `if nil == nil` (always true) with four unlifted `TODO(native)` updates; it is now a real `ctr_c8` with
  `(ctr_c8 + 1) % 5` and `if ctr_c8 == 0`. AppleGirl's `ctr_64 == nil` → `== 0`. The compare rewrite is
  positional (nearest preceding event on that slot suffix must be a counter assignment), because the same
  slot is a string in another phase (TraderConflictEvil `_74`: `GetDataString(&xStack_74)` then the null
  test the string rules fold; MeleeOpponent `_c8`: a script-name temporary earlier in the function).
- The heredoc-eats-backslashes gotcha bit three times tonight. Write/Edit only for regex-heavy Python.

## Gates

Oakvale draft **identical** (FSE tree byte-identical vs pristine tools; the committed CONVERSION_REPORT was
already stale from a21faaa). Smoke Orchard 0 / Guild 1 / TraderConflict 8 (unchanged). Readable errors: none
in all three units (MeleeOpponent briefly regressed to a Lua syntax error when `if 0 == 0` reached the
epilogue-hoisting pass — fixed by lifting the counter instead of folding the compare). New test
`test_guild_woods_melee_converter.py` runs the converter's WoodsMelee `Main` through the level gate on both
stages and checks `DoMission` polls `GuildWoods` twice; fails 4/4 on the committed files. Draft diff across
units: 11 files, the rest cosmetic (`0x0` → `0`, parens).

## For Aeon

His crash is his own port, but the native facts at that seam are the evidence list to diff against: Main's
four state bools → level gate → bind ScorpionHome → FinalizeEntityBindings → threads WatchForTermination
(waits MissionFailed/MissionSucceeded, then SetQuestAsCompleted/Failed, `TEXT_QST_028_GUILDSEAL_COME_BACK`
after five frames, DeactivateQuestLater once HeroGuildComplex loads) and DoMission (GiveHeroNewQuestObjective
"first objective", WatchForLeaving + TeleportOutHero, EndMission on MissionOver); ScorpionHome: RegisterTimer +
SetTimer 5, `AddQuestInfoCounter("HUD_BEETLE_ICON", 10, 1.0)`, DisplayQuestInfo, stick nag
`TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT` every 8 s without OBJECT_HERO_STICK, spawn at
`GetFurthestWithScriptName(hero, "ScorpionSpawn")` (zero vector if none), `SetToKillOnLevelUnload(0)`,
`EntityAttachToScript(beetle, "Q_GuildTrainingWoodsMelee")`, `ScorpionsDestroyed` master state when the last
one dies. And the crash class he named (quit mid-quest): every early exit deregisters the timer and removes
the quest-info element.
