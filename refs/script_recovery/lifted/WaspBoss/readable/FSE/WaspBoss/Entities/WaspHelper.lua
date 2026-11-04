-- Readable native conversion: WaspHelper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_WaspHelperCallsOutDistance = 3664,  -- 5
}

-- per-entity fields (native class members; one Lua state per entity instance)
local pleadedToHero_, leadToRegion, wavedOver

-- WaspHelper.Main (retail 0x00e0f8a0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isDistanceBetweenThingsUnder, isDistanceBetweenThingsUnder2, scratchValue8
    local addNewConversation, line, getPos, predicateResult, predicateResult6, movie, timerId
    local scriptThing
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    local waspGuardMoveToPos = quest:GetThingWithScriptName("WaspGuardMoveToPos")
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 3) do
        if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
    if not pleadedToHero_ then
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        local hero = quest:GetHero()
        me:FollowThing(hero, 1.0, true)
        local scratchValue4 = quest:ReadGlobalGameData(SCRIPT_DEF.WB_WaspHelperCallsOutDistance)
        local hero2 = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero2, scratchValue4)
        while not isDistanceBetweenThingsUnder do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
            local scratchValue5 = quest:ReadGlobalGameData(SCRIPT_DEF.WB_WaspHelperCallsOutDistance)
            local hero3 = quest:GetHero()
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero3, scratchValue5)
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
            quest:RemoveThing(me, false, true)
        end
        if quest:GetTimer(quest:GetStateInt("ReachedWaspHelper")) == 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
            me:ClearCommands()
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scriptThing = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scriptThing) then
                line = "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10"
                local hero4 = quest:GetHero()
                if not me:Speak(hero4, line, 0, false, true, false) then
                    goto LAB_00e0fd75
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e0fd75 end
                end
                goto LAB_00e0fbf4
            end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
            me:ClearCommands()
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scriptThing = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scriptThing) then
                line = "TEXT_QST_072_HELPER_FOLLOW_ME_10"
                local hero5 = quest:GetHero()
                if not me:Speak(hero5, line, 0, false, true, false) then
                    goto LAB_00e0fd75
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e0fd75 end
                end
                goto LAB_00e0fbf4
            end
        end
        goto FLOW_past_lab_00e0fbf4
        ::LAB_00e0fbf4::
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            goto LAB_00e109ab
        end
        ::FLOW_past_lab_00e0fbf4::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        pleadedToHero_ = true
    end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 3) do
        if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
    if not leadToRegion then
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        timerId = quest:RegisterTimer()
        me:ClearCommands()
        if not (waspGuardMoveToPos ~= nil and not waspGuardMoveToPos:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = waspGuardMoveToPos:GetPos()
        end
        me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
        isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(me, waspGuardMoveToPos, 2.0)
        predicateResult = false
        predicateResult6 = false
        while not isDistanceBetweenThingsUnder2 do
            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
            local scratchValue = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(me, waspGuardMoveToPos)))
            local hero23 = quest:GetHero()
            local scratchValue2 = resources:DistanceBetweenThings(hero23, waspGuardMoveToPos)
            local scratchValue6 = scratchValue
            local hero24 = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero24, 9.0) or scratchValue2 <= scratchValue6 then
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                local hero25 = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, hero25, 6.5) or scratchValue2 <= scratchValue6 then
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                    if not predicateResult then
                        me:ClearCommands()
                        if not (waspGuardMoveToPos ~= nil and not waspGuardMoveToPos:IsNull()) then
                            getPos = {x = 0, y = 0, z = 0}
                        else
                            getPos = waspGuardMoveToPos:GetPos()
                        end
                        me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
                        predicateResult = true
                    end
                else
                    local hero26 = quest:GetHero()
                    if quest:IsDistanceBetweenThingsUnder(me, hero26, 8.0) then
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                        if predicateResult then
                            me:ClearCommands()
                            if not (waspGuardMoveToPos ~= nil and not waspGuardMoveToPos:IsNull()) then
                                getPos = {x = 0, y = 0, z = 0}
                            else
                                getPos = waspGuardMoveToPos:GetPos()
                            end
                            me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
                            predicateResult = false
                        end
                    end
                end
                predicateResult6 = false
                if quest:GetTimer(timerId) == 0 then
                    if not quest:IsActiveThreadTerminating() then
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        local hero6 = quest:GetHero()
                        quest:AddPersonToConversation(addNewConversation, hero6)
                        local getHero = quest:GetHero()
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_GO_THIS_WAY", me, getHero, false)
                        scratchValue8 = 8
                        goto LAB_00e10323
                    end
                    goto FLOW_past_lab_00e1036a
                end
            else
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                me:ClearCommands()
                me:ClearAllActions()
                if not predicateResult6 then
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                    local hero7 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero7, false)
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero8 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero8)
                    local hero27 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_OVER_HERE", me, hero27, false)
                    quest:Pause(1.0)
                    predicateResult6 = true
                end
                if quest:GetTimer(timerId) == 0 then
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                    local hero9 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero9, false)
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero10 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero10)
                    local hero11 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_OVER_HERE", me, hero11, false)
                    resources:PlayAnimationWithNativeArgument5(resource, "ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, false, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                    quest:Pause(0.8)
                    resources:PlayAnimationWithNativeArgument5(resource, "ST_OPINION_APPROVAL_WAVING_AT_DISTANCE", false, false, false, true, false, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e1036a end
                    scratchValue8 = 5
                    goto LAB_00e10323
                end
            end
            goto FLOW_past_lab_00e10323
            ::LAB_00e10323::
            quest:SetTimer(timerId, scratchValue8)
            ::FLOW_past_lab_00e10323::
            isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(me, waspGuardMoveToPos, 2.0)
        end
        if not quest:IsActiveThreadTerminating() then
            leadToRegion = true
            quest:DeregisterTimer(timerId)
            goto LAB_00e1036a
        end
    else
        goto LAB_00e1036a
    end
    goto FLOW_past_lab_00e1036a
    ::LAB_00e1036a::
    me:ClearCommands()
    quest:SetIsPushableByHero(me, false)
    if not wavedOver then
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        addNewConversation = quest:AddNewConversation(me, false, false)
        local hero12 = quest:GetHero()
        quest:AddPersonToConversation(addNewConversation, hero12)
        local hero13 = quest:GetHero()
        quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_THIS_WAY_10", me, hero13, false)
        local hero14 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, hero14, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        resources:PlayAnimationWithNativeArgument5(resource, "STANDARD_WAVE", false, false, false, true, false, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        quest:EntitySetFacingAngleTowardsThing(me, waspGuardMoveToPos, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        resources:PlayAnimationWithNativeArgument5(resource, "ST_CALL_OVER", false, false, false, true, false, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        wavedOver = true
    end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 10)
    while not quest:IsActiveThreadTerminating() do
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            addNewConversation = quest:AddNewConversation(me, false, false)
            local hero15 = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, hero15)
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                if quest:IsActiveThreadTerminating() then break end
                local hero16 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, hero16, false)
            else
                if quest:IsActiveThreadTerminating() then break end
                local hero17 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, hero17, false)
            end
        end
        if quest:GetTimer(timerId) ~= 0 then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then break end
            local hero18 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero18, false)
            quest:SetTimer(timerId, 10)
            addNewConversation = quest:AddNewConversation(me, false, false)
            local hero19 = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, hero19)
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                if quest:IsActiveThreadTerminating() then break end
                local hero20 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, hero20, false)
            else
                if quest:IsActiveThreadTerminating() then break end
                local hero21 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, hero21, false)
            end
            local hero22 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero22, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:PlayAnimationWithNativeArgument5(resource, "STANDARD_WAVE", false, false, false, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetFacingAngleTowardsThing(me, waspGuardMoveToPos, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:PlayAnimationWithNativeArgument5(resource, "ST_CALL_OVER", false, false, false, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e1036a end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:NewScriptFrame(me)
        end
    end
    ::FLOW_past_lab_00e1036a::
    quest:DeregisterTimer(timerId)
    ::LAB_00e109ab::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e0fd75::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    goto LAB_00e109ab
end

-- WaspHelper.Init (retail 0x00e0f870)
function Init(quest, me)
    pleadedToHero_ = false
    leadToRegion = false
    wavedOver = false
end

-- WaspHelper.OnPersist (retail 0x00e12300)
function OnPersist(quest, me, context)
    local pleadedToHero = pleadedToHero_ or false
    pleadedToHero_ = quest:PersistTransferBool(context, "PleadedToHero", pleadedToHero)
end

-- WaspHelper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

