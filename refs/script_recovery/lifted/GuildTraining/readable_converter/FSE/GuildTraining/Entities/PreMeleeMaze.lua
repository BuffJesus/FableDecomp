-- Readable native conversion: PreMeleeMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- PreMeleeMaze.Main (retail 0x00d43db0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult4, predicateResult5, msgIsHitByHero, predicateResult13
    local fret_0, fret_00, p0, this_00, movie, movie3
    predicateResult13 = false
    predicateResult = false
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(true)
    local preMeleeMazeTargetMarker = quest:GetThingWithScriptName("PreMeleeMazeTargetMarker")
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    predicateResult4 = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult4 then
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetMasterGameState("GuildWarningOccuring") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            resources:PrepareResource(resource)
            while quest:GetMasterGameState("GuildWarningOccuring") do
                if not quest:NewScriptFrame(me) then goto LAB_00d444a1 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d444a1 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
        end
        local isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(me, preMeleeMazeTargetMarker, 4.0) and not me:IsPerformingScriptTask()
        if isDistanceBetweenThingsOver then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            if not (preMeleeMazeTargetMarker ~= nil and not preMeleeMazeTargetMarker:IsNull()) then
                p0 = {x = 0, y = 0, z = 0}
            else
                p0 = preMeleeMazeTargetMarker:GetPos()
            end
            me:MoveToPosition(p0, 3.0, ENTITY_MOVE_WALK, false, true)
        end
        if me:IsTalkedToByHero() then break end
        if me:MsgIsHitByHero() then
            goto LAB_00d44243
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                predicateResult13 = true
                predicateResult = true
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d44243 end
            end
            predicateResult13 = true
            predicateResult5 = false
        end
        goto FLOW_past_lab_00d44243
        ::LAB_00d44243::
        predicateResult5 = true
        ::FLOW_past_lab_00d44243::
        predicateResult = predicateResult and false
        predicateResult13 = predicateResult13 and false
        if not predicateResult5 then quest:NewScriptFrame(me); predicateResult4 = quest:IsActiveThreadTerminating(); goto continue_2 end
        if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
        me:ClearCommands()
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        fret_00 = quest:GetHealth(resources:ScriptThing(resource))
        if 0.0 < fret_00 then
            me:Speak(hero, "TEXT_QST_028_MAZE_HIT", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d44494 end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d44494 end
            goto FLOW_past_lab_00d44494
            ::LAB_00d44494::
            this_00 = movie
            goto LAB_00d44498
            ::FLOW_past_lab_00d44494::
        end
        me:SetFriendsWithEverythingFlag(true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:NewScriptFrame(me)
        predicateResult4 = quest:IsActiveThreadTerminating()
        ::continue_2::
    end
    ::FLOW_after_lab_00d441bb::
    if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
    me:ClearCommands()
    movie3 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    fret_0 = quest:GetHealth(resources:ScriptThing(resource))
    if fret_0 <= 0.0 then
        goto LAB_00d441a3
    end
    goto FLOW_past_lab_00d441a3
    ::LAB_00d441a3::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    msgIsHitByHero = me:MsgIsHitByHero()
    predicateResult = predicateResult and false
    predicateResult13 = predicateResult13 and false
    if msgIsHitByHero then
        if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
        me:ClearCommands()
        local movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        fret_00 = quest:GetHealth(resources:ScriptThing(resource))
        if 0.0 < fret_00 then
            me:Speak(hero, "TEXT_QST_028_MAZE_HIT", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d44494_c1 end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d44494_c1 end
            goto FLOW_past_lab_00d44494_c1
            ::LAB_00d44494_c1::
            this_00 = movie2
            goto LAB_00d44498
            ::FLOW_past_lab_00d44494_c1::
        end
        me:SetFriendsWithEverythingFlag(true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
    end
    quest:NewScriptFrame(me)
    goto FLOW_after_lab_00d441bb
    ::FLOW_past_lab_00d441a3::
    me:Speak(hero, "TEXT_QST_028_MAZE_LEAVE_ME", GROUP_SELECT_FIRST, false, true, false)
    while me:IsPerformingScriptTask() do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            this_00 = movie3
            goto LAB_00d44498
        end
    end
    if not quest:IsActiveThreadTerminating() then goto LAB_00d441a3 end
    quest:PauseAllNonScriptedEntities(false)
    this_00 = movie3
    ::LAB_00d44498::
    resources:DestroyMovie(this_00)
    ::LAB_00d444a1::
    resources:ReleaseResource(resource)
end

-- PreMeleeMaze.Init (retail 0x00d43d80)
function Init(quest, me)
end

-- PreMeleeMaze.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- PreMeleeMaze.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

