-- Readable native conversion: WaspVictim. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_MoralityGain = 3632,  -- 0.009999999776482582
    WB_ScreamingVillagerScreamsDistance = 3656,  -- 10
    WB_ScreamingVillagerFadeOutTime = 3660,  -- 2.5
    WB_WaspChaseWomanPanicTime = 3668,  -- 7
}

-- WaspVictim.Main (retail 0x00e11670)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local timerId
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
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
    quest:ModifyThingHealth(me, 6.0 - quest:GetHealth(me), false)
    if quest:GetStateBool("QueenHornetAttacks") then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:RemoveThing(me, false, true)
    end
    local waspAttacker = quest:GetThingWithScriptName("WaspAttacker")
    local timerId2 = quest:RegisterTimer()
    timerId = timerId2
    while true do
        if not ((waspAttacker ~= nil and not waspAttacker:IsNull()) and (waspAttacker ~= nil and waspAttacker:IsAlive())) then break end
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
        if quest:GetTimer(timerId) == 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            if quest:IsDistanceBetweenThingsUnder(me, hero, quest:ReadGlobalGameData(SCRIPT_DEF.WB_ScreamingVillagerScreamsDistance)) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                quest:SetTimer(timerId, quest:ReadGlobalGameData(SCRIPT_DEF.WB_WaspChaseWomanPanicTime))
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                quest:AddLineToConversation(conversationId, "TEXT_QST_072_VILLAGER_MALE_SCREAMS_10", me, hero, false)
                timerId = timerId2
            end
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
    local conversationId2 = quest:AddNewConversation(me, false, false)
    quest:AddPersonToConversation(conversationId2, hero)
    quest:AddLineToConversation(conversationId2, "TEXT_QST_072_VILLAGER_MALE_ON_SAVED_10", me, hero, false)
    quest:EntitySetAsScared(me, false)
    local villagerEscapePos = quest:GetThingWithScriptName("VillagerEscapePos")
    if quest:GetStateInt("SavedVillagerCount") >= 2 then goto LAB_00e11a0a end
    if not quest:IsActiveThreadTerminating() then
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.WB_MoralityGain))
        quest:SetStateInt("SavedVillagerCount", quest:GetStateInt("SavedVillagerCount") + 1)
        goto LAB_00e11a0a
    end
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e11a0a::
    while quest:IsDistanceBetweenThingsOver(me, villagerEscapePos, 2.0) do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
        if not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            me:MoveToThing(villagerEscapePos, 1.0, ENTITY_MOVE_RUN)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:FadeOutAndKillEntity(me, true, quest:ReadGlobalGameDataFloat(SCRIPT_DEF.WB_ScreamingVillagerFadeOutTime), true)
    end
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(resource)
end

-- WaspVictim.Init (retail 0x00e11630)
function Init(quest, me)
    quest:EntitySetAsScared(me, true)
end

-- WaspVictim.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WaspVictim.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

