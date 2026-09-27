-- Readable native conversion: Q_SummoningTheShip. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_SummoningTheShip.Main (retail 0x00df13a0)
function Main(quest)
    quest:AddEntityBinding("STS_BriarRose", "SummoningTheShip/Entities/STS_BriarRose")
    quest:AddEntityBinding("SummonerAttacker", "SummoningTheShip/Entities/SummonerAttacker")
    quest:AddEntityBinding("SummonerMinion", "SummoningTheShip/Entities/SummonerMinion")
    quest:AddEntityBinding("FireHeartHolder", "SummoningTheShip/Entities/FireHeartHolder")
    quest:AddEntityBinding("FireHeart", "SummoningTheShip/Entities/FireHeart")
    quest:AddEntityBinding("M_ActivateLighthouse", "SummoningTheShip/Entities/M_ActivateLighthouse")
    quest:FinalizeEntityBindings()
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
end

-- Q_SummoningTheShip.Init (retail 0x00df1230)
function Init(quest)
    quest:SetStateInt("CommentaryTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:AddQuestRegion("Q_SummoningTheShip", "HookCoast")
    quest:AddQuestRegion("Q_SummoningTheShip", "LostBay")
    quest:SetStateInt("SummonersAlive", 0)
    quest:SetStateInt("CurrentAttackWave", 1)
    quest:SetStateBool("LighthouseStarted", false)
    quest:SetStateBool("SummonerAttacksStarted", false)
    quest:SetStateBool("MissionFailed", false)
end

-- Q_SummoningTheShip.DoMission (retail 0x00df36e0)
function DoMission(quest)
    local scratchValue, currentAttackWave
    local hero = quest:GetHero()
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_01", "HookCoast", "HookCoast")
    while not quest:IsLevelLoaded("HookCoast") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetSummonerDeathExplosionAffectsHero(false)
    quest:SetWeaponOutCrimeEnabled(false)
    quest:RemoveThing(quest:GetThingWithScriptName("LightHouseDoor"), false, false)
    quest:RemoveThing(quest:GetThingWithScriptName("HookCoastShip1"), false, false)
    quest:RemoveThing(quest:GetThingWithScriptName("HookCoastShip2"), false, false)
    quest:SetVillageLimbo(quest:GetThingWithScriptName("HookCoastVillage"), true)
    helper_DF3E50(quest, scratchValue)
    quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
    quest:FadeScreenIn()
    while not quest:GetStateBool("LighthouseStarted") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:OverrideMusic(23, false, false)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_02", "HookCoast", "HookCoast")
    helper_DF3E50(quest, scratchValue)
    quest:SetStateBool("SummonerAttacksStarted", true)
    quest:OverrideMusic(23, false, false)
    currentAttackWave = quest:GetStateInt("CurrentAttackWave")
    while currentAttackWave < 2 and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then return end
        currentAttackWave = quest:GetStateInt("CurrentAttackWave")
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionFailed") then
        helper_DF3E50(quest, currentAttackWave)
    else
        helper_DF3E50(quest, currentAttackWave)
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
    end
    quest:SetSummonerDeathExplosionAffectsHero(true)
    while quest:GetStateInt("SummonersAlive") ~= 0 and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
    local scratchValue2 = briarRose ~= nil and briarRose:IsAlive()
    if scratchValue2 then
        local conversationID = quest:AddNewConversation(briarRose, false, false)
        quest:SetStateInt("ConversationIndex", conversationID)
        quest:AddPersonToConversation(conversationID, hero)
        quest:AddLineToConversation(conversationID, "TEXT_QST_B02_BRIARROSE_FIREHEART_POWERED", briarRose, hero, false)
    end
    quest:StopOverrideMusic(false)
    if quest:GetStateInt("SummonersAlive") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:Pause(3.0)
        helper_DF3E50(quest, scratchValue2)
        quest:PlayAVIMovie("Data\\\\Video\\\\Summoning_The_Ship.xmv")
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
    else
        if quest:IsActiveThreadTerminating() then return end
        helper_DF3E50(quest, scratchValue2)
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
    end
    quest:SetVillageLimbo(quest:GetThingWithScriptName("HookCoastVillage"), false)
    quest:SetWeaponOutCrimeEnabled(true)
    quest:RemoveThing(quest:GetThingWithScriptName("STS_BriarRose"), false, true)
    quest:StopOverrideMusic(false)
    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("LostBayHSP"), false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_SummoningTheShip.MakeBriarRoseComment (retail 0x00df20f0)
function MakeBriarRoseComment(quest, commentToMake)
    local hero = quest:GetHero()
    if not quest:IsConversationActive(quest:GetStateInt("ConversationIndex")) then
        quest:SetTimer(quest:GetStateInt("CommentaryTimer"), 15)
        local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
        if briarRose ~= nil then
            if briarRose ~= nil and briarRose:IsAlive() then
                local conversationID = quest:AddNewConversation(briarRose, false, false)
                quest:SetStateInt("ConversationIndex", conversationID)
                quest:AddPersonToConversation(conversationID, hero)
                quest:AddLineToConversation(conversationID, "TEXT_QST_B02_BRIARROSE_" .. commentToMake, briarRose, hero, false)
                return true
            end
        end
    end
    return false
end

-- Q_SummoningTheShip.helper_DF3E50 (retail 0x00df3e50)
function helper_DF3E50(quest, param1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(param1, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

