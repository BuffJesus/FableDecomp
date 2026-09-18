-- Readable native conversion: PreMeleeMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- PreMeleeMaze.Main (retail 0x00d43db0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, p0, preMeleeMazeTargetMarker, movie, movie2, movie3, resource
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    preMeleeMazeTargetMarker = quest:GetThingWithScriptName("PreMeleeMazeTargetMarker")
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            -- TODO(native): xStack_48[0] = (int *)0x0;
            return
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    predicateResult = false
    while true do
        if predicateResult then
            resources:ReleaseResource(resource)
            -- TODO(native): xStack_48[0] = (int *)0x0;
            return
        end
        if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            while quest:GetMasterGameState("GuildWarningOccuring") ~= 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00d444a1 end
            end
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d444a1 end
            end
        end
        if quest:IsDistanceBetweenThingsOver(me, preMeleeMazeTargetMarker, 4.0) and not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            if preMeleeMazeTargetMarker ~= nil and not preMeleeMazeTargetMarker:IsNull() then
                p0 = preMeleeMazeTargetMarker:GetPos()
            end
            me:MoveToPosition(p0, 3.0, 0, false, true)
        end
        if me:IsTalkedToByHero() then break end
        if me:MsgIsHitByHero() or me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(me) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            me:ClearCommands()
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_028_MAZE_HIT", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                    goto LAB_00d44498
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d44494: (native jump target)
                    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                    goto LAB_00d44498
                end
            end
            me:SetFriendsWithEverythingFlag(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    end
    ::FLOW_after_lab_00d441bb::
    if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
    me:ClearCommands()
    movie3 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
        -- LAB_00d441a3: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        if me:MsgIsHitByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            me:ClearCommands()
            movie2 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_028_MAZE_HIT", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00d44494_c3
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d44494_c3: (native jump target)
                    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                    goto LAB_00d44498
                end
            end
            me:SetFriendsWithEverythingFlag(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        quest:NewScriptFrame(me)
        goto FLOW_after_lab_00d441bb
    end
    if not me:Speak(hero, "TEXT_QST_028_MAZE_LEAVE_ME", GROUP_SELECT_FIRST, false, true, false) then
        quest:PauseAllNonScriptedEntities(false)
        -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20;
        goto LAB_00d44498
    end
    if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d441a3
    quest:PauseAllNonScriptedEntities(false)
    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20;
    ::LAB_00d44498::
    resources:DestroyMovie(this_00)
    ::LAB_00d444a1::
    resources:ReleaseResource(resource)
end

-- PreMeleeMaze.Init (retail 0x00d43d80)
function Init(quest, me)
end

-- PreMeleeMaze.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- PreMeleeMaze.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

