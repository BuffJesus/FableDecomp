-- Readable native conversion: PreMeleeMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local predicateResult5, predicateResult6, predicateResult13, scratchValue7, p0, r1_1
    local scratchValue11, scratchValue12, scratchValue13, scratchValue14
    predicateResult13 = false
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    r1_1 = quest:GetThingWithScriptName("PreMeleeMazeTargetMarker")
    scratchValue14 = resources:NewResource()
    while not resources:TryAcquire(scratchValue14, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(scratchValue14)
            -- TODO(native): xStack_48[0] = (int *)0x0;
            return
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(scratchValue14); return end
    predicateResult5 = false
    while true do
        if predicateResult5 then
            resources:ReleaseResource(scratchValue14)
            -- TODO(native): xStack_48[0] = (int *)0x0;
            return
        end
        if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            while quest:GetMasterGameState("GuildWarningOccuring") ~= 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00d444a1 end
            end
            while not resources:TryAcquire(scratchValue14, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d444a1 end
            end
        end
        if quest:IsDistanceBetweenThingsOver(me, r1_1, 4.0) and not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            if r1_1 ~= nil and not r1_1:IsNull() then
                p0 = r1_1:GetPos()
            end
            me:MoveToPosition(p0, 3.0, 0, false, true)
        end
        if me:IsTalkedToByHero() then break end
        if me:MsgIsHitByHero() then
            predicateResult6 = true
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                predicateResult13 = true
                if not me:MsgIsHitByHeroSpecialAbility(me) then
                    predicateResult6 = true
                    goto FLOW_after_lab_00d44243
                end
            end
            predicateResult13 = true
            predicateResult6 = false
        end
        ::FLOW_after_lab_00d44243::
        predicateResult13 = predicateResult13 and false
        if predicateResult6 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            me:ClearCommands()
            scratchValue11 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(predicateResult13)
            scratchValue7 = 0.0
            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue14)) then
                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_HIT", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_10;
                        goto LAB_00d44498
                    end
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
            resources:DestroyMovie(scratchValue11)
        end
        quest:NewScriptFrame(me)
        predicateResult5 = quest:IsActiveThreadTerminating()
    end
    ::FLOW_after_lab_00d441bb::
    if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
    me:ClearCommands()
    scratchValue13 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(scratchValue7 ~= 0)
    scratchValue7 = 0.0
    if quest:GetHealth(resources:ScriptThing(scratchValue14)) <= 0.0 then
        -- LAB_00d441a3: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue13)
        predicateResult13 = predicateResult13 and false
        if me:MsgIsHitByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d444a1 end
            me:ClearCommands()
            scratchValue12 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(predicateResult13)
            scratchValue7 = 0.0
            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue14)) then
                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_HIT", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00d44494_c3
                    end
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
            resources:DestroyMovie(scratchValue12)
        end
        quest:NewScriptFrame(me)
        goto FLOW_after_lab_00d441bb
    end
    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_LEAVE_ME", 0, false, true, false)
    while me:IsPerformingScriptTask() do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20;
            goto LAB_00d44498
        end
    end
    if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d441a3
    quest:PauseAllNonScriptedEntities(false)
    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20;
    ::LAB_00d44498::
    resources:DestroyMovie(this_00)
    ::LAB_00d444a1::
    resources:ReleaseResource(scratchValue14)
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

