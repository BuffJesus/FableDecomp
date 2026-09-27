-- Generated native draft: Q_SummoningTheShip. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar2
    quest:AddEntityBinding("STS_BriarRose", "SummoningTheShip/Entities/STS_BriarRose")
    quest:AddEntityBinding("SummonerAttacker", "SummoningTheShip/Entities/SummonerAttacker")
    quest:AddEntityBinding("SummonerMinion", "SummoningTheShip/Entities/SummonerMinion")
    quest:AddEntityBinding("FireHeartHolder", "SummoningTheShip/Entities/FireHeartHolder")
    quest:AddEntityBinding("FireHeart", "SummoningTheShip/Entities/FireHeart")
    quest:AddEntityBinding("M_ActivateLighthouse", "SummoningTheShip/Entities/M_ActivateLighthouse")
    quest:FinalizeEntityBindings()
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
    if not bVar2 then
    end
end

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

function DoMission(quest)
    local CVar1, CVar9, b2, bVar2, conversationID, delay, iVar7, pCVar3, pCVar4, pCVar5, pCVar6, pThingToMove, r1
    local alive = true
    pCVar3 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_01", "HookCoast", "HookCoast")
    bVar2 = quest:IsLevelLoaded("HookCoast")
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsLevelLoaded("HookCoast")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    quest:SetSummonerDeathExplosionAffectsHero(false)
    quest:SetWeaponOutCrimeEnabled(false)
    CVar9 = 0x0
    bVar2 = false
    pCVar4 = quest:GetThingWithScriptName("LightHouseDoor")
    quest:RemoveThing(pCVar4, bVar2, (CVar9 ~= 0))
    CVar9 = 0x0
    bVar2 = false
    pCVar4 = quest:GetThingWithScriptName("HookCoastShip1")
    quest:RemoveThing(pCVar4, bVar2, (CVar9 ~= 0))
    CVar9 = 0x0
    bVar2 = false
    pCVar4 = quest:GetThingWithScriptName("HookCoastShip2")
    quest:RemoveThing(pCVar4, bVar2, (CVar9 ~= 0))
    CVar9 = 0x1
    pCVar4 = quest:GetThingWithScriptName("HookCoastVillage")
    quest:SetVillageLimbo(pCVar4, (CVar9 ~= 0))
    iVar7 = extraout_ECX
    helper_DF3E50(quest, iVar7)
    CVar9 = 0x0
    bVar2 = true
    pCVar5 = quest:GetActiveQuestName()
    quest:KickOffQuestStartScreen(pCVar5, bVar2, (CVar9 ~= 0))
    quest:FadeScreenIn()
    CVar1 = quest:GetStateBool("LighthouseStarted")
    while not CVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        CVar1 = quest:GetStateBool("LighthouseStarted")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    quest:OverrideMusic(0x17, false, false)
    pCVar3 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_02", "HookCoast", "HookCoast")
    iVar7 = extraout_ECX_00
    helper_DF3E50(quest, iVar7)
    quest:SetStateBool("SummonerAttacksStarted", true)
    quest:OverrideMusic(0x17, false, false)
    iVar7 = quest:GetStateInt("CurrentAttackWave")
    while (iVar7 < 2 and (not quest:GetStateBool("MissionFailed"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar7 = quest:GetStateInt("CurrentAttackWave")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    if not quest:GetStateBool("MissionFailed") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar7 = extraout_ECX_02
        helper_DF3E50(quest, iVar7)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar7 = extraout_ECX_01
        helper_DF3E50(quest, iVar7)
        CVar9 = 0x1
        bVar2 = true
        pCVar6 = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pCVar6, bVar2, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", (CVar9 ~= 0))
    end
    quest:SetSummonerDeathExplosionAffectsHero(true)
    iVar7 = quest:GetStateInt("SummonersAlive")
    while (iVar7 ~= 0 and (not quest:GetStateBool("MissionFailed"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar7 = quest:GetStateInt("SummonersAlive")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    r1 = quest:GetThingWithScriptName("STS_BriarRose")
    iVar7 = (r1 ~= nil and r1:IsAlive())
    if iVar7 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00df3e3a end
        conversationID = quest:AddNewConversation(r1, false, false)
        quest:SetStateInt("ConversationIndex", conversationID)
        pCVar4 = quest:GetHero()
        quest:AddPersonToConversation(conversationID, pCVar4)
        pCVar4 = quest:GetHero()
        quest:AddLineToConversation(conversationID, "TEXT_QST_B02_BRIARROSE_FIREHEART_POWERED", r1, pCVar4, false)
    end
    quest:StopOverrideMusic(false)
    if quest:GetStateInt("SummonersAlive") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00df3e3a end
        quest:Pause(3.0)
        iVar7 = extraout_ECX_03
        helper_DF3E50(quest, iVar7)
        quest:PlayAVIMovie("Data\\\\Video\\\\Summoning_The_Ship.xmv")
        CVar9 = 0x0
        b2 = false
        bVar2 = true
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar5, bVar2, b2, (CVar9 ~= 0))
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00df3e3a end
        iVar7 = extraout_ECX_04
        helper_DF3E50(quest, iVar7)
        CVar9 = 0x1
        bVar2 = true
        pCVar6 = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pCVar6, bVar2, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", (CVar9 ~= 0))
    end
    CVar9 = 0x0
    pCVar4 = quest:GetThingWithScriptName("HookCoastVillage")
    quest:SetVillageLimbo(pCVar4, (CVar9 ~= 0))
    quest:SetWeaponOutCrimeEnabled(true)
    CVar9 = 0x1
    bVar2 = false
    pCVar4 = quest:GetThingWithScriptName("STS_BriarRose")
    quest:RemoveThing(pCVar4, bVar2, (CVar9 ~= 0))
    quest:StopOverrideMusic(false)
    CVar9 = 0x0
    pCVar4 = quest:GetThingWithScriptName("LostBayHSP")
    pThingToMove = quest:GetHero()
    quest:EntityTeleportToThing(pThingToMove, pCVar4, (CVar9 ~= 0))
    delay = 0
    pCVar5 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar5, delay)
    ::LAB_00df3e3a::
end

function MakeBriarRoseComment(quest, native_arg_comment_to_make)
    local bVar2, cVar3, conversationID, pCVar4, pLine, pListener, r1
    bVar2 = quest:IsConversationActive(quest:GetStateInt("ConversationIndex"))
    if not bVar2 then
        quest:SetTimer(quest:GetStateInt("CommentaryTimer"), 0xf)
        r1 = quest:GetThingWithScriptName("STS_BriarRose")
        if (r1 ~= nil and not r1:IsNull()) then
            cVar3 = (r1 ~= nil and r1:IsAlive())
            if cVar3 then
                conversationID = quest:AddNewConversation(r1, false, false)
                quest:SetStateInt("ConversationIndex", conversationID)
                pCVar4 = quest:GetHero()
                quest:AddPersonToConversation(conversationID, pCVar4)
                pListener = quest:GetHero()
                pCVar4 = r1
                bVar2 = false
                pLine = ("TEXT_QST_B02_BRIARROSE_" .. native_arg_comment_to_make)
                quest:AddLineToConversation(conversationID, pLine, pCVar4, pListener, bVar2)
                return true
            end
        end
    end
    return false
end

function helper_DF3E50(quest, native_arg_param_1)
    local resources = quest:RetailResources()
    local xStack_20 = resources:NewResource()
    local pScriptObject = xStack_20
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_2c = resources:NewActorMap()
    resources:SetActor(xStack_2c, "HERO", xStack_20)
    local xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(native_arg_param_1, xStack_2c, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_2c)
    resources:ReleaseResource(xStack_20)
end

