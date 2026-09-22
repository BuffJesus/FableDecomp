-- Readable native conversion: WaspChaseWoman. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_MoralityGain = 3632,  -- 0.009999999776482582
    WB_ScreamingVillagerFadeOutTime = 3660,  -- 2.5
}

-- WaspChaseWoman.Main (retail 0x00e10e60)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isDistanceBetweenThingsOver, scratchValue, waspChaser, getThingWithScriptName
    local villagerEscapePos, timerId
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e11369 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e11369 end
    if quest:GetStateBool("QueenHornetAttacks") then
        quest:RemoveThing(me, false, true)
    end
    while not quest:GetStateBool("StartChase") do
        if not quest:NewScriptFrame(me) then goto LAB_00e11369 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e11369 end
    waspChaser = quest:GetThingWithScriptName("WaspChaser")
    scratchValue = 2
    getThingWithScriptName = quest:GetThingWithScriptName("ChasedWomanNav" .. tostring(3))
    me:FollowPreCalculatedRoute(quest:GetThingWithScriptName("ChasedWomanNav" .. tostring(2)), 1, false, true)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 11)
    while not quest:IsActiveThreadTerminating() do
        local taskRunning = me:IsPerformingScriptTask()
        if not taskRunning or quest:IsDistanceBetweenThingsUnder(me, getThingWithScriptName, 2.0) then
            scratchValue = (scratchValue + 1) % 6
            getThingWithScriptName = quest:GetThingWithScriptName("ChasedWomanNav" .. tostring((scratchValue + 1) % 6))
            me:ClearCommands()
            me:FollowPreCalculatedRoute(quest:GetThingWithScriptName("ChasedWomanNav" .. tostring(scratchValue)), 1, false, true)
        end
        if waspChaser ~= nil and waspChaser:IsAlive() then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e1134e end
            quest:EntitySetAsScared(me, false)
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10", me, hero, false)
            villagerEscapePos = quest:GetThingWithScriptName("VillagerEscapePos")
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e11345 end
                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.WB_MoralityGain))
                quest:SetStateInt("SavedVillagerCount", quest:GetStateInt("SavedVillagerCount") + 1)
            end
            isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(me, villagerEscapePos, 2.0)
            goto LAB_00e112c4
            quest:NewScriptFrame(me)
        end
    end
    quest:DeregisterTimer(timerId)
    goto LAB_00e11357
    ::LAB_00e112c4::
    if isDistanceBetweenThingsOver then
        if not quest:NewScriptFrame(me) then goto LAB_00e11345 end
        if not me:IsPerformingScriptTask() then
            me:MoveToThing(villagerEscapePos, 1.0, ENTITY_MOVE_RUN)
        end
        isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(me, villagerEscapePos, 2.0)
        goto LAB_00e112c4
    end
    if not quest:IsActiveThreadTerminating() then
        quest:FadeOutAndKillEntity(me, true, quest:ReadGlobalGameDataFloat(SCRIPT_DEF.WB_ScreamingVillagerFadeOutTime), true)
    end
    ::LAB_00e11345::
    ::LAB_00e1134e::
    quest:DeregisterTimer(timerId)
    ::LAB_00e11357::
    ::LAB_00e11369::
    resources:ReleaseResource(resource)
end

-- WaspChaseWoman.Init (retail 0x00e10e20)
function Init(quest, me)
    quest:EntitySetAsScared(me, true)
end

-- WaspChaseWoman.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WaspChaseWoman.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

