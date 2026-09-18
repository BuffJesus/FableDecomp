# Readable output: from "decompiled program" to "quest script"

Status: steps 1, 2, 6 LANDED 2026-09-17, steps 3/4/5 PARTLY landed the same night (`tools/script_recovery/readable_style.py`,
run by `build_readable_unit.py`; night-5 section of `docs/journal/2026-09/CONVERTER_GENERIC_UNITS_2026-09-16.md`). Orchard
readable 2109 -> 1236 lines, temporaries 270 -> 81, labels 26 -> 14, termination checks 177 -> 103, 26 frame checks, zero
per-call `require(`. Landed from 3/4/5: `state:GetInt("TeamID")` entity-state alias, init-only state reads hoisted to
`local teamId = ...` (unit-wide writer analysis), `helper_DCEC50` -> `SetMemberState` (named from the state it writes),
per-function `-- Owner.Function (retail 0x...)` headers, goto idioms: skip-the-rest -> if/else (one and two levels),
`goto` to a tail `return` -> the return, `else` + single `if` -> `elseif`, `if C then A else EXIT end` -> guard.
Still open: per-unit accessor tables for `Teams_<n>_<Field>` (needs a runtime accessor with Get/Set kinds), enum names
for MemberState values (no PDB enum — do not guess), the DoMultiplierCutscene label tangle (native_quest_helpers), the
`if X then goto L end` whose fall-through would duplicate the skipped code. Owner stage: `tools/script_recovery/build_readable_unit.py` (readable stage ONLY — the draft stays the
faithful, reversible audit trail; `smoke_run_unit.py` keeps running both stages).

## After the first increment (CrateTeamMember.Main, 350 -> 154 lines, 50 -> 9 temporaries)
```lua
function Main(quest, me)
    local predicateResult4, predicateResult5, predicateResult6, scratchValue8, getStateInt
    local getCurrentStateGroupType, p0_00, getNearestWithScriptName, thing_38, scratchValue14
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame(me) then return end
        if quest:GetStateBool("HeroAtWrongEntrance") then
            quest:RemoveThing(me, false, false)
            ...
    if __native_entity_state:GetStateInt("TeamID") == 1 and quest:GetStateInt("HeroTeam") == 1 then
        while quest:IsInCutscene() do
            if not quest:NewScriptFrame(me) then return end
        end
        helpers.MakeTeamMemberComment(quest, me, "FETCHING_03", me, 0)
    end
```
Still to come (steps 3-5): `teams[myTeam].crateCarrier` accessors instead of `quest:GetStateThing("Teams_" .. .. "_TeamCrateCarrier")`,
`local self = __native_entity_state`, `helper_DCEC50(quest, me, 2)` -> `SetMemberState(MEMBER_FETCHING)`, the three goto idioms.

## The gap (same API, different presentation)

Ours today (`refs/script_recovery/lifted/OrchardFarm/readable/FSE/OrchardFarmRaid/Entities/CrateTeamMember.lua`):
```lua
alive = not quest:IsActiveThreadTerminating()
predicateResult7 = not alive
if predicateResult7 then
    -- LAB_00dcebab: (native jump target)
    return
end
scratchValue8 = quest:IsDistanceBetweenThingsUnder(me, thing_38, 10.0)
if (quest:GetStateInt("HeroTeam") == 0) and (__native_entity_state:GetStateInt("TeamID") == 1) then
    if quest:IsActiveThreadTerminating() then return end
    scratchValue8 = true
end
isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(thing_38, quest:GetStateThing(("Teams_" .. __native_entity_state:GetStateInt("MyTeam") .. "_TeamCrateCarrier")))
```
Style oracle (Aeon, `work/aeon_lua_ports/Fisherman/FSE/Fisherman/Entities/Fisherman.lua`):
```lua
local npcStand = quest:GetNearestWithScriptName(me, "NPC_NPCStand")
if npcStand and quest:IsDistanceBetweenThingsUnder(me, npcStand, 10.0) then
    quest:EntityTeleportToThing(me, npcStand)
end
me:MoveToThing(fleeMarker, 3.0, 1) -- 1 = ENTITY_MOVE_RUN (Non-blocking)
```

## Steps, in payoff order

1. **Termination boilerplate** (~30% of lines). Collapse `alive = not X; p = not alive; if p then return end` to one line;
   drop the check entirely when it is the statement right after a blocking call and its only effect is `return` (the FSE
   host unwinds a terminating coroutine at the next blocking call — the Oakvale bundle already relies on this). Keep it (one
   line) when it runs cleanup (`__cleanup_X(); return`). Retry loops become `while not cond do quest:NewScriptFrame(me) end`
   with a single `if not quest:NewScriptFrame(me) then return end` like Aeon.
2. **Inline single-use temporaries.** `scratchValueN` / `predicateResultN` / `getStateThingN` assigned once, used once before
   reassignment, with no call in between (pure): substitute at the use. Multi-use temps: name from call + argument
   (`heroNearCrate`, not `isDistanceBetweenThingsUnder`).
3. **Named state and things.** Hoist unchanging `GetStateX("Name")` reads into `local name = ...` at block top; alias entity
   state (`local self = __native_entity_state`, or DLL: `me.state:GetInt("TeamID")`); generate per-unit accessor tables for
   PDB-named containers (`teams[myTeam].crateCarrier` instead of `quest:GetStateThing("Teams_" .. t .. "_TeamCrateCarrier")`).
4. **Structure.** Recognise the three goto idioms on the readable stage: retry loop → `while`; early-exit cleanup → the
   existing `__cleanup_X()` helper (drop the `-- LAB_x: (native jump target)` comment); shared tails (already duplicated).
   No `::label::` survives; an unstructurable jump becomes a numbered comment at most. Builds on native_goto_scopes.py.
5. **Constants and comments.** `helper_DCEC50(quest, me, 2)` → PDB name (`SetMemberState`) + enum constant
   (`MEMBER_FETCHING`, from the unit evidence enum fields; FSE enum tables for move types etc. as inline comments); one-line
   header per function from the PDB name/role (Main, per-frame thread, helper).
6. **Cosmetics.** Strip `(4.0)`, `(true)`, `(x ~= false)`; single blank lines; `local helpers = require(...)` once per file.

## Status 2026-09-17 (night) — measured against Aeon's Fisherman / NewOakValeIntro ports

Landed in `readable_style.py` / `readable_lua.py`: `local hero = quest:GetHero()` hoisted once per function; copy
propagation (aliases, loop-end reloads); uint fixups; cleanup closures tidied; free `name2` suffixes dropped; the
cutscene boilerplate folded to `quest:StartCutscene({HERO = hero, ...}, {}, fixCamera)` / `RunCutscene` / `EndCutscene`
(exactly LuaQuestState::StartCutscene's native calls); helpers named by shape (`PlayHeroCutscene`); temporaries named
after the script/def name they look up or create (`guildScorpions`, `scorpionSpawn`, `guildStagBeetle`, `count`,
`infoCounter`). Orchard readable 1140 lines / 69 temporaries (was 2110 / 270 before the style work).

Still different from Aeon's hand style, in payoff order:
1. Named constants for `quest:ReadGlobalGameDataFloat(<offset>)` — needs an offset -> main.def field-name table
   (`OVI_MoralityChangePerDeed` etc.); none on disk yet. Candidate source: the CGlobalGameData layout in the PDB types.
2. `if x then` instead of `x ~= nil and not x:IsNull()` right after a lookup — ForgeFSE's `WrapScriptThingOutput` already
   returns nil for null things, so the IsNull is redundant at that point (keep it for things held across frames).
3. `quest:Log` breadcrumbs at phase starts; section comments (`-- TASK 1: ...`).
4. Local helper functions for blocks repeated inside one file (Aeon's `RunAwaySequence`).
5. Entity `while not TryAcquire` retry loops -> `me:AcquireControl()`; `or 0` defaults on state reads.

## Measuring it
Add to READABLE_REPORT.json per function: lines, temps, labels, `IsActiveThreadTerminating` count, `require(` count, gotos.
Drive them down; semantics stay honest through `smoke_run_unit.py --stage readable` (0 problems) and the byte-identical
draft gate (`gate.sh` → `OAKVALE GATE: identical`). Aeon's Fisherman / Orchard ports are the style target (audit table in
`docs/scripts/AEON_LUA_PORTS.md`).

## Resume commands
```
python tools/script_recovery/build_readable_unit.py --unit orchard_farm
python tools/script_recovery/smoke_run_unit.py --unit orchard_farm --stage readable
python tools/script_recovery/build_unit_playtest_package.py        # -> local-candidate-v5 (then local_test.py preflight)
```
