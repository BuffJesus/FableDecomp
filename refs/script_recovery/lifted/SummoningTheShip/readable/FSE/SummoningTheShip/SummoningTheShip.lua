-- Readable native conversion: Q_SummoningTheShip. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_SummoningTheShip.Main (retail 0x00df13a0)
function Main(quest)
    local scratchValue
    quest:AddEntityBinding("STS_BriarRose", "SummoningTheShip/Entities/STS_BriarRose")
    quest:AddEntityBinding("SummonerAttacker", "SummoningTheShip/Entities/SummonerAttacker")
    quest:AddEntityBinding("SummonerMinion", "SummoningTheShip/Entities/SummonerMinion")
    quest:AddEntityBinding("FireHeartHolder", "SummoningTheShip/Entities/FireHeartHolder")
    quest:AddEntityBinding("FireHeart", "SummoningTheShip/Entities/FireHeart")
    quest:AddEntityBinding("M_ActivateLighthouse", "SummoningTheShip/Entities/M_ActivateLighthouse")
    quest:FinalizeEntityBindings()
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
    if not scratchValue then
    end
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
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_01", "HookCoast", "HookCoast")
    while not quest:IsLevelLoaded("HookCoast") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetSummonerDeathExplosionAffectsHero(false)
    quest:SetWeaponOutCrimeEnabled(false)
    local lightHouseDoor = quest:GetThingWithScriptName("LightHouseDoor")
    quest:RemoveThing(lightHouseDoor, false, false)
    local hookCoastShip1 = quest:GetThingWithScriptName("HookCoastShip1")
    quest:RemoveThing(hookCoastShip1, false, false)
    local hookCoastShip = quest:GetThingWithScriptName("HookCoastShip2")
    quest:RemoveThing(hookCoastShip, false, false)
    local hookCoastVillage = quest:GetThingWithScriptName("HookCoastVillage")
    quest:SetVillageLimbo(hookCoastVillage, true)
    helper_DF3E50(quest, "CS_SUMSHIP_PREINTRO")
    local getActiveQuestName3 = quest:GetActiveQuestName()
    quest:KickOffQuestStartScreen(getActiveQuestName3, true, false)
    quest:FadeScreenIn()
    while not quest:GetStateBool("LighthouseStarted") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:OverrideMusic(23, false, false)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_02", "HookCoast", "HookCoast")
    helper_DF3E50(quest, "CS_SUMSHIP_INTRO")
    quest:SetStateBool("SummonerAttacksStarted", true)
    quest:OverrideMusic(23, false, false)
    while quest:GetStateInt("CurrentAttackWave") < 2 and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionFailed") then
        if quest:IsActiveThreadTerminating() then return end
        helper_DF3E50(quest, "CS_SUMSHIP_WAVE2")
    else
        if quest:IsActiveThreadTerminating() then return end
        helper_DF3E50(quest, "CS_SUMSHIP_OUTRO_FAILED")
        local getActiveQuestName6 = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(getActiveQuestName6, true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
    end
    quest:SetSummonerDeathExplosionAffectsHero(true)
    while quest:GetStateInt("SummonersAlive") ~= 0 and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local briarRose2 = quest:GetThingWithScriptName("STS_BriarRose")
    if briarRose2 ~= nil and briarRose2:IsAlive() then
        if quest:IsActiveThreadTerminating() then return end
        local conversationID = quest:AddNewConversation(briarRose2, false, false)
        quest:SetStateInt("ConversationIndex", conversationID)
        local hero = quest:GetHero()
        quest:AddPersonToConversation(conversationID, hero)
        local hero2 = quest:GetHero()
        quest:AddLineToConversation(conversationID, "TEXT_QST_B02_BRIARROSE_FIREHEART_POWERED", briarRose2, hero2, false)
    end
    quest:StopOverrideMusic(false)
    if quest:GetStateInt("SummonersAlive") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:Pause(3.0)
        helper_DF3E50(quest, "CS_SUMSHIP_OUTRO")
        quest:PlayAVIMovie("Data\\\\Video\\\\Summoning_The_Ship.xmv")
        local getActiveQuestName4 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName4, true, false, false)
    else
        if quest:IsActiveThreadTerminating() then return end
        helper_DF3E50(quest, "CS_SUMSHIP_OUTRO_FAILED")
        local getActiveQuestName7 = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(getActiveQuestName7, true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
    end
    local hookCoastVillage2 = quest:GetThingWithScriptName("HookCoastVillage")
    quest:SetVillageLimbo(hookCoastVillage2, false)
    quest:SetWeaponOutCrimeEnabled(true)
    local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
    quest:RemoveThing(briarRose, false, true)
    quest:StopOverrideMusic(false)
    local lostBayHSP = quest:GetThingWithScriptName("LostBayHSP")
    local pThingToMove = quest:GetHero()
    quest:EntityTeleportToThing(pThingToMove, lostBayHSP, false)
    local getActiveQuestName5 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName5, 0)
end

-- Q_SummoningTheShip.MakeBriarRoseComment (retail 0x00df20f0)
function MakeBriarRoseComment(quest, commentToMake)
    if not quest:IsConversationActive(quest:GetStateInt("ConversationIndex")) then
        quest:SetTimer(quest:GetStateInt("CommentaryTimer"), 15)
        local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
        if briarRose ~= nil then
            if briarRose ~= nil and briarRose:IsAlive() then
                local conversationID = quest:AddNewConversation(briarRose, false, false)
                quest:SetStateInt("ConversationIndex", conversationID)
                local hero = quest:GetHero()
                quest:AddPersonToConversation(conversationID, hero)
                local pListener = quest:GetHero()
                local pLine = "TEXT_QST_B02_BRIARROSE_" .. commentToMake
                quest:AddLineToConversation(conversationID, pLine, briarRose, pListener, false)
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
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
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

