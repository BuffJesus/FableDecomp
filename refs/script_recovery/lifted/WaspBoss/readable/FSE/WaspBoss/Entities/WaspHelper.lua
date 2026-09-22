-- Readable native conversion: WaspHelper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_WaspHelperCallsOutDistance = 3664,  -- 5
}

-- per-entity fields (native class members; one Lua state per entity instance)
local pleadedToHero, leadToRegion, wavedOver

-- WaspHelper.Main (retail 0x00e0f8a0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isDistanceBetweenThingsUnder, scratchValue8, addNewConversation, getPos, scratchValue
    local scratchValue13, movie, timerId
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    local waspGuardMoveToPos = quest:GetThingWithScriptName("WaspGuardMoveToPos")
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 3) do
        if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
    if not pleadedToHero then
        me:FollowThing(hero, 1.0, true)
        while not quest:IsDistanceBetweenThingsUnder(me, hero, quest:ReadGlobalGameData(SCRIPT_DEF.WB_WaspHelperCallsOutDistance)) do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        if quest:GetStateBool("MissionSucceeded") then
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
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e0fd75 end
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
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_072_HELPER_FOLLOW_ME_10", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e0fd75 end
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
        pleadedToHero = true
    end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 3) do
        if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
    if not leadToRegion then
        timerId = quest:RegisterTimer()
        me:ClearCommands()
        if not (waspGuardMoveToPos ~= nil and not waspGuardMoveToPos:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = waspGuardMoveToPos:GetPos()
        end
        me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, waspGuardMoveToPos, 2.0)
        scratchValue = 0
        scratchValue13 = 0
        while not isDistanceBetweenThingsUnder do
            if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
            local getDistanceBetweenThings = quest:GetDistanceBetweenThings(hero, waspGuardMoveToPos) ^ 2
            local getDistanceBetweenThings2 = quest:GetDistanceBetweenThings(me, waspGuardMoveToPos) ^ 2
            if quest:IsDistanceBetweenThingsUnder(me, hero, 9.0) or getDistanceBetweenThings <= getDistanceBetweenThings2 then
                if quest:IsDistanceBetweenThingsUnder(me, hero, 6.5) or getDistanceBetweenThings <= getDistanceBetweenThings2 then
                    if not scratchValue then
                        me:ClearCommands()
                        if not (waspGuardMoveToPos ~= nil and not waspGuardMoveToPos:IsNull()) then
                            getPos = {x = 0, y = 0, z = 0}
                        else
                            getPos = waspGuardMoveToPos:GetPos()
                        end
                        me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
                        scratchValue = 1
                    end
                elseif quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
                    if scratchValue ~= 0 then
                        me:ClearCommands()
                        if not (waspGuardMoveToPos ~= nil and not waspGuardMoveToPos:IsNull()) then
                            getPos = {x = 0, y = 0, z = 0}
                        else
                            getPos = waspGuardMoveToPos:GetPos()
                        end
                        me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
                        scratchValue = 0
                    end
                end
                scratchValue13 = 0
                if quest:GetTimer(timerId) == 0 then
                    if not quest:IsActiveThreadTerminating() then
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_GO_THIS_WAY", me, hero, false)
                        scratchValue8 = 8
                        goto LAB_00e10323
                    end
                    goto LAB_00e109a6
                end
            else
                me:ClearCommands()
                me:ClearAllActions()
                if not scratchValue13 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e109a6 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_OVER_HERE", me, hero, false)
                    quest:Pause(1.0)
                    scratchValue13 = 1
                end
                if quest:GetTimer(timerId) == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e109a6 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_OVER_HERE", me, hero, false)
                    me:PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, true, false, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e109a6 end
                    quest:Pause(0.8)
                    me:PlayAnimation("ST_OPINION_APPROVAL_WAVING_AT_DISTANCE", false, false, false, true, true, false, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e109a6 end
                    scratchValue8 = 5
                    goto LAB_00e10323
                end
            end
            goto FLOW_past_lab_00e10323
            ::LAB_00e10323::
            quest:SetTimer(timerId, scratchValue8)
            ::FLOW_past_lab_00e10323::
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, waspGuardMoveToPos, 2.0)
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
        quest:AddPersonToConversation(addNewConversation, hero)
        quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_THIS_WAY_10", me, hero, false)
        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        quest:EntitySetFacingAngleTowardsThing(me, waspGuardMoveToPos, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then goto LAB_00e109ab end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e109ab end
        me:PlayAnimation("ST_CALL_OVER", false, false, false, true, true, false, false)
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
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then break end
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, hero, false)
            end
        end
        if quest:GetTimer(timerId) ~= 0 then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:SetTimer(timerId, 10)
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then break end
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, hero, false)
            end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
            end
            if quest:IsActiveThreadTerminating() then break end
            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetFacingAngleTowardsThing(me, waspGuardMoveToPos, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
            end
            if quest:IsActiveThreadTerminating() then break end
            me:PlayAnimation("ST_CALL_OVER", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00e109a6 end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:NewScriptFrame(me)
        end
    end
    ::FLOW_past_lab_00e1036a::
    ::LAB_00e109a6::
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
    pleadedToHero = false
    leadToRegion = false
    wavedOver = false
end

-- WaspHelper.OnPersist (retail 0x00e12300)
function OnPersist(quest, me, context)
    quest:SetStateBool("PleadedToHero", quest:PersistTransferBool(context, "PleadedToHero", quest:GetStateBool("PleadedToHero")))
end

-- WaspHelper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

