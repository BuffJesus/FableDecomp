# 2026-09-22 -- run 14 (v7) dies in Gameflow's OnPersist; TraderToRescue's hidden vtable receivers

## Run 14: v7 cannot load a v6-written save (converter is right, Aeon's port deviates)

`autopilot.py run v7 ... --save f645456fds` reaches "save loaded: first quest host is up" and the game is then
GONE (no window, no process). The FSE log stops mid-transfer, deterministically, on two runs:

    [PERSIST] Transferring int 'PostSavePosition'...
    [PERSIST] ...Value is now: 100
    [PERSIST] Transferring uint 'CoreQuestWaiting'...      <- last line ever written

v6 (Aeon's LUAGameflow) transfers the same key as a **bool**. Which one is retail?

* retail `Gameflow::OnPersist` 0x00CEF8E0 calls four different Transfer instantiations; the CoreQuestWaiting
  site (0x00CEF921) targets **0x004106F0**, which FSE binds as `CPersistContext_Transfer_uint`
  (`FableAPI.cpp:531`; bool is 0x4045C0, int 0x410BE0). The bsim label on all of them
  (`CPersistContext::Transfer<signed_char>`) is propagated across the byte-identical 27-byte family -- do not
  trust it.
* PDB: `CGameflowScript::CoreQuestWaiting` is a **ulong** (`ghidra_out/struct_layouts_egor.tsv:54364`).

Two independent sources: the converter's `PersistTransferUInt` is faithful and Aeon's `PersistTransferBool`
is the deviation. The crash is a **save-compatibility** failure, not a converter bug: the staged save was
written by v6, so the record under that key is one byte and the ulong read runs off it.

**Probe (proves it):** `local-candidate-v7probe` = v7 with that one line switched to `PersistTransferBool`.
The same save now loads, both `SavedScriptNames` / `SavedCardDefNames` string lists transfer (0 entries), and
the woods checklist plays with `host Gameflow/Gameflow` live and no Lua runtime errors. So the rest of the
converter's Gameflow is fine in-game; only the persist schema is incompatible with a v6-born save.

**Counter-probe (decisive):** the unmodified v7 (uint) loading a **retail-born save** (profile `25`, July,
never touched by an FSE Gameflow) transfers cleanly -- `Transferring uint 'CoreQuestWaiting' ... Value is now: 0`
and both string lists. So v7 is compatible with retail's own persist schema and it is the v6 record that is
malformed for it. (That run's checklist then failed, as expected: profile 25 is at a different story point.)

**Consequence for the A/B:** v6 and v7 cannot share a save. Run 14 has to start from a **v7-born** save
(play the chain once on v7 from a new game, or re-save under v7 before the compare), otherwise every v7 run
dies at load. Do not "fix" the converter to bool -- that would be a deliberate deviation from retail.

## TraderToRescue 0x00DFE0F0: vtable calls whose receiver is never printed

Two liveness tests in `Main` came out as `TODO(native): cVar3 = (**(xStack_148 + 0x12c))()`. The receiver is
inside the code-pointer expression, where no operand pass reaches it, and Ghidra's own spelling there
(`iStack_140`) is a drifted name four slots off -- so the object fold read them as calls on the *resource*.

The typed export knows better: each indirect site carries `ecxStack`, the receiver's true slot (here -0x134
and -0x130, the second drifted by the thing's +4). `restore_stack_operands` now, for a vtable site whose
receiver is not printed as an argument, respells the head as a thing receiver on the object living at that
slot (`*(int *)xStack_134`), with the same two rules the argument pass already uses: a slot at which nothing
is known takes the object whose extent covers it, and one Ghidra head name spread over several slots that
never hold a construction is ONE object, living at its earliest site. Both calls now lift to

    cVar3 = (r1 ~= nil and r1:IsAlive())          -- r1 = GetNearestWithScriptName(me, "TC_BanditHostageKeeper")

which is the retail reading: the hostage trader waits while its keeper lives.

Also generic: `fold_inline_constructors` accepted a member zero-store only as `X[0] = 0x0`, so the resource
ctor's `auStack_13c[0] = 0;` survived as a `TODO(native): xStack_148[0] = 0;` (`RE_INLINE_CTOR` now takes
plain `0` and the `._N_4_` spelling too).

**Gates:** TraderConflict draft TODO 34 -> 31; guild_training / orchard_farm / gameflow regenerate
byte-identical; smoke 0 problems on all four units (trader was 1); `test_skill_target_and_friendly_actor` +
`test_cross_branch_goto` 25/25.

## Open

* Run 14 proper: a v7-born save, then `ab_playtest.py compare v6 v7`.
* TraderToRescue still has 5 TODOs: an `(iVar11 + 0)` element vcall at 0x138, two `CCharString + 0xa` string
  slices, one `+ 1`, and a helper call printed with no operands.

## The readable pass folded a loop-exiting `goto` into a frame-less spin

Rebuilding the readable stages (they were stale against the draft) made `smoke_run_unit.py --unit
trader_conflict --stage readable` report `WatchForKilledPeople: call trace overflow (loop without frames?)`.
It is pre-existing, not from today's converter work: the file is byte-identical to HEAD's.

`fold_goto_else` rewrites `if C then goto L end; Y; ::L::` as `if not C then Y end` and, looking for the
label, skips over any `end` lines in between. Skipping a **loop's** `end` changes what the jump does --
retail's `if terminating then goto <function end> end` inside a `while` became

    while quest:GetStateInt("TradersReachedTeleporter") < 3 do
        if quest:NewScriptFrame() then ... end

i.e. the script keeps looping, without ever yielding a frame, exactly when the thread is being torn down.
`_block_openers` now names the head each `end` closes and the fold stops at a `while` / `for` / `repeat`.
Blast radius across the units: that one function (8 lines); Gameflow's readable output is unchanged, so the
deployed v7 / v7probe bundles are still current. `test_readable_loop_exit_goto.py` pins it.

## The A/B, and two side findings

**`v7probe` ran the whole chain: 58/58 steps, 0 failed, zero Lua runtime errors**, ending at
`SetQuestAsCompleted Q_GuildTraining` -- the converter's Gameflow drives the Guild path exactly as Aeon's port
did in run 13 (`work/ab_runs/v7probe-20260922-100933` vs `v6-20260921-212900`). `ab_playtest.py compare` shows
the only substantive one-side events are the two quests themselves (`LUAGameflow/LUAGameflow` vs
`Gameflow/Gameflow`, plus Aeon's two "campaign loop at stage 100" log lines, which our port simply does not
print) and the converter's two extra persist transfers (`SavedScriptNames` / `SavedCardDefNames`, which retail
does and Aeon's port does not). The rest of both one-side lists is run-to-run noise: thread pointers, cutscene
tick numbers, `skipQueryTrue` callers. The `override Q_TraderConflict*` lines missing on the v7probe side are a
one-off of that bundle build, not a behaviour difference -- rebuilding the same command registers them.

**The Will test's HUD counter is an arrow in RETAIL** (user report). Not ours: the Will Guildmaster 0x00D5E0C0
pushes `"HUD_ICON_ARROW"` at 0x00D5F275, then `"HUD_CLOCK_ICON"` for the timer, and WillApprentice 0x00D4EFE0
does the same at 0x00D4FBF6. Lionhead reused the arrow counter icon rather than authoring a Will one (Skill uses
`HUD_ICON_MULTI_ARROW` + `HUD_ICON_ARROW`, the PreMelee/Skill Guildmasters `HUD_QUEST_ICON_TARGET_DUMMY`, Melee
`HUD_WHISPER_ICON`). Changing it would be a deliberate deviation, so it is in GOTCHAS.

**TraderToRescue's last operand-less call was real lost behaviour.** At 0x00E00589 the bytes are
`push 0x12df01c ("CS_TRADERCON_GOOD_OUTRO"); call <CCharString ctor>; mov ecx,[ebp+0x14]; call 0xdfded0`:
Ghidra printed the receiver and dropped the by-value string, so the good-ending outro cutscene never reached
the Lua. `name_by_value_string_parameters` now matches the wrapped constructor line, a stack temp deeper than
`&stack0xffffff..`, and a receiver reached through a field. Unit todo 34 -> 29.

## Nothing was testing the side quests or the secret trainers

The user asked whether the apple girl, the gulls, the race and the three apprentice trainers had been
covered. They had not. In the v7probe chain log every one of those entities binds, its script loads and
some enter `Main` (`KillBird.lua` is allocated seven times, `RaceMarker` runs its loop, `MeleeApprentice`
even speaks `TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT`), but no checklist ever *talks* to them, so
every conversation, question, grading and reward branch was unexercised.

Two new checklists:

* `checklists/guild_side_quests.json` (childhood, after `guild_woods_return`) -- AppleGirl
  (`APPLEGIRL_CHAT` -> question -> the counting conversation, `CurrentApples` staged the way the melee
  checklist stages `DummyHits`), BirdKiller (`BIRD_KILLER_GREET` -> question -> the `CS_GUILD_GULLS_INTRO`
  macro -> the `CurrentBirdsKilled * GUI_GoldPerBird` payout) and ApprenticeSpeedTest
  (`FAST_APPRENTICE_BOAST` -> question -> `FAST_APPRENTICE_RUN` -> the `ReachedPlatform` outcome branches).
* `checklists/guild_secret_trainers.json` (adult, after `guild_departure_stage`) -- CombatApprentice,
  SkillApprentice and WillApprentice: the hello/question lines and the `CS_GUILD_DEPARTURE_*_TEST_*`
  grade macros (END / A / APLUS / APLUS_PRIZE).

`test_checklists.py` now validates all of them statically -- regexes compile, every `input:` verb is one
`send_input` implements with its arity, channel lines are `<Quest>: <lua>` or a console command, ids are
unique, and any step that drives Lua forbids `LUA RUNTIME ERROR`. That last check found a real gap:
`guild_skill_stage.json`'s `skill_move_state` had no `forbid`, so a runtime error during the moving-dummy
read-back would have passed silently.

## The last Guild TODO: a fourth counted-release spelling

`MeleeApprentice` 0x00D40CF0's termination path kept `TODO(native): (**(code **)((int)xStack_e8 + 4))();`
inside a dead `if false then`. It is the same inlined counted-pointer release as the other three variants,
but Ghidra typed the slot `undefined1 [4]`, the `&&` wrapped across lines, and by the time the fold runs the
earlier passes have dropped the cast entirely (`if ((xStack_e8 != 0x0) && (...))`). The null test in
`RE_LOCAL_COUNTED_RELEASE3` now takes an optional cast and whitespace around `&&`.

**The whole GuildTraining unit is down to two structural TODOs**: a `*xStack_23c` read in the Melee
Guildmaster, and `CreateEffect` -- which is a missing ForgeFSE binding, not a converter gap.

## Driving the side quests and the secret trainers (runs 21-26)

**Run 21 (side quests, from the Guild-arrival save): the apple girl plays through** --
`applegirl_talk` -> `applegirl_accept` -> `applegirl_apples` all PASS, zero Lua errors. That whole script had
never been executed before today. `birdkiller_talk` then failed with the reason in the log:
`GetThingWithScriptName 'BirdKiller'` returns `pImp.Data = 0x0`, an empty thing -- its region is not loaded at
arrival. The same name resolves to a live thing (`WrapperPos = (4708.36,3674.84,19.8)`) only at line 13132 of
the full-chain log, in the adulthood transition where `SkillApprentice` is allocated, so the gulls and the race
moved into the departure-window checklist.

**Runs 22-26 (the chain with `guild_secret_trainers` inserted between the Will stage and Departure).** Each run
went one step further, and every stop had its reason in the script rather than a guess:

* run 22, **50/51 PASS**: all three trainers' conversations driven for the first time. The melee one answered
  `TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE` -- CombatApprentice, SkillApprentice and WillApprentice all
  refuse their own test while `GetMasterGameState("HeroTakingGuildTest")` is true, which is exactly what the
  Guildmaster's departure test sets. Each test step now clears the flag and re-talks.
* run 23, **51/52**: `melee_trainer_test` PASSES -- the melee secret trainer's own test ran. The skill failure
  was the checklist, not the game: the apprentice was speaking `..._SKILL_EARLY_COMMENT`, a line the expect list
  did not name. All three talk steps now take every key their script can speak.
* run 24, **52/53**: `skill_trainer_talk` PASSES. The test is gated on
  `GetMasterGameState("GlobalSkillGrade")` -- 0 is the `EARLY_COMMENT` branch, no test on offer -- so each test
  step stages its own grade (`GlobalMeleeGrade` / `GlobalSkillGrade` / `GlobalWillGrade`).
* run 25: the grade landed (the apprentice moved from `EARLY_COMMENT` to `NOT_APLUS_COMMENT`) but still no
  offer. The offer sits behind `IsTalkedToByHero() and not me:IsPerformingScriptTask()`, and this apprentice
  walks to `SkillApprenticeTargetMarker` in its idle loop, so the hero now waits on that marker.
* run 26, **52/53**, unchanged: still no offer.

**Where it stands.** In run 26's log the only trainer question issued anywhere is the melee one
(`TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION`); the skill apprentice never calls
`GiveHeroYesNoQuestion` and only ever speaks its ambient `NOT_APLUS_COMMENT` (eight times). So its state is
right and `IsTalkedToByHero()` simply never fires for **that** entity -- TAB talks to the nearest NPC and the
archery area is crowded, while the melee trainer stands alone, which is why it worked. Zero Lua runtime errors
in every one of these runs.

**Next (one change, one run):** teleport the *apprentice* to the hero rather than the hero to the apprentice,
so it is unambiguously the nearest thing when TAB goes in; the will trainer's steps have not been reached yet,
and the gulls and race steps sit behind them in the same file.

