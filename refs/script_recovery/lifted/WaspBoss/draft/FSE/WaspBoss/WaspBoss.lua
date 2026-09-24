-- Generated native draft: Q_WaspBoss. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local function __native_all_dead(list)
    local down = 0
    for _, thing in ipairs(list) do
        if (not thing:IsAlive()) or thing:IsUnconscious() then down = down + 1 end
    end
    return down == #list
end

function Main(quest)
    local pQuestName, r1
    local alive = true
    quest:SetTimer(quest:GetStateInt("ReachedWaspHelper"), 0x78)
    local bVar2 = quest:IsRegionLoaded("LookoutPoint")
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsRegionLoaded("LookoutPoint")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:Pause(1.0)
        r1 = quest:GiveHeroTutorial(6)
        quest:AddEntityBinding("GratefulVillagerSpawn", "WaspBoss/Entities/GratefulVillagerSpawn", 1)
        quest:AddEntityBinding("WaspChaser", "WaspBoss/Entities/WaspChaser", 1)
        quest:AddEntityBinding("WaspChaseWoman", "WaspBoss/Entities/WaspChaseWoman", 1)
        quest:AddEntityBinding("WaspAttacker", "WaspBoss/Entities/WaspAttacker", 1)
        quest:AddEntityBinding("WaspVictim", "WaspBoss/Entities/WaspVictim", 1)
        quest:AddEntityBinding("FleeingWoman", "WaspBoss/Entities/FleeingWoman", 1)
        quest:AddEntityBinding("WaspHelper", "WaspBoss/Entities/WaspHelper", 1)
        quest:AddEntityBinding("QueenHornet", "WaspBoss/Entities/QueenHornet", 1)
        quest:AddEntityBinding("HornetDrone", "WaspBoss/Entities/HornetDrone", 1)
        quest:FinalizeEntityBindings()
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01", "", "HeroGuildComplexInside")
        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_028_GUILDSEAL_WASP_MAP", "", true, true)
        bVar2 = quest:IsLevelLoaded("PicnicArea")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:IsLevelLoaded("PicnicArea")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:OverrideMusic(0x17, false, false)
            quest:CreateThread("WatchForTermination")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function WatchForTermination(quest)
            if not bVar2 then
            end
            quest:CreateThread("DoMission")  -- native thread body CQ_WaspBossScript::DoMission: lift it as function DoMission(quest)
            if not bVar2 then
            end
            quest:CreateThread("WatchForCutscene")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetCutscene: lift it as function WatchForCutscene(quest)
            if not bVar2 then
            end
        end
    end
end

function Init(quest)
    quest:SetStateInt("ReachedWaspHelper", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:AddQuestRegion("Q_WaspBoss", "PicnicArea")
    quest:SetQuestWorldMapOffset("Q_WaspBoss", 0xa, nil --[[missing]])
    quest:SetStateBool("PanickedVillagersScene", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("DronesCreated", false)
    quest:SetStateBool("SaidLineOnce", false)
    quest:SetStateBool("QueenHornetAttacks", false)
    quest:SetStateBool("CutsceneFinished", false)
    quest:SetStateBool("StartChase", false)
    quest:SetStateBool("QuestStartScreened", false)
    quest:SetStateInt("SavedVillagerCount", 0)
    quest:SetTimer(quest:GetStateInt("ReachedWaspHelper"), 0)
end

function OnPersist(quest, context)
    local questStartScreened = quest:GetStateBool("QuestStartScreened") or false
    questStartScreened = quest:PersistTransferBool(context, "QuestStartScreened", questStartScreened)
    quest:SetStateBool("QuestStartScreened", questStartScreened)
    local savedVillagerCount = quest:GetStateInt("SavedVillagerCount") or 0
    savedVillagerCount = quest:PersistTransferInt(context, "SavedVillagerCount", savedVillagerCount)
    quest:SetStateInt("SavedVillagerCount", savedVillagerCount)
end

function WatchForTermination(quest)
    local bVar4, bVar6, pCVar5
    local alive = true
    local CVar1 = quest:GetStateBool("MissionFailed")
    while (not CVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        CVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    if not quest:GetStateBool("MissionFailed") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:GiveHeroExperience(quest:ReadGlobalGameData(0xe58))
        bVar6 = false
        bVar4 = true
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar5, bVar4, bVar6, false)
        quest:StopOverrideMusic(false)
        quest:FadeScreenIn()
        bVar4 = quest:IsLevelLoaded("PicnicArea")
        if bVar4 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                bVar4 = quest:IsLevelLoaded("PicnicArea")
            until not (bVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FIRST", "", true, true)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        bVar6 = true
        bVar4 = true
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pCVar5, bVar4, "", bVar6)
        quest:StopOverrideMusic(false)
    end
    pCVar5 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar5, 0)
end

function DoMission(quest)
    local append_xStack_24_0, append_xStack_24_1, bVar6, cVar7, iVar12, pCVar10, pCVar8, pCVar9, r1, v_stk_34, xStack_24, xStack_44, x_stk_30
    local alive = true
    v_stk_34 = 0
    quest:AddRumourCategory("Post waspboss killed")
    quest:AddNewRumourToCategory("Post waspboss killed", "TEXT_AI_GOSSIP_WASPBOSS_KILLED")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_PICNIC_AREA")
    x_stk_30 = nil
    iVar12 = 1
    repeat
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            x_stk_30 = nil
            goto LAB_00e12a9c
        end
        xStack_44 = quest:ReadGlobalGameDataString(0xe3c)
        pCVar8 = tostring(iVar12)
        pCVar8 = ("QueenDepositPos" .. pCVar8)
        pCVar9 = quest:GetThingWithScriptName(pCVar8)
        bVar6 = false
        pCVar10 = pCVar9:GetPos()
        pCVar9 = quest:CreateCreature(xStack_44, pCVar10, "HornetDrone")
        x_stk_30 = pCVar9
        pCVar9 = nil
        pCVar9 = nil
        iVar12 = iVar12 + 1
    until not (iVar12 < 6)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        x_stk_30 = nil
        x_stk_30 = 0
        return
    end
    xStack_24 = quest:GetAllThingsWithScriptName("HornetDrone")
    append_xStack_24_0 = quest:GetAllThingsWithScriptName("WaspChaser")
    for _, appendedThing in ipairs(append_xStack_24_0) do xStack_24[#xStack_24 + 1] = appendedThing end
    append_xStack_24_1 = quest:GetAllThingsWithScriptName("WaspAttacker")
    for _, appendedThing in ipairs(append_xStack_24_1) do xStack_24[#xStack_24 + 1] = appendedThing end
    helper_E12F20(quest)
    iVar12 = __native_all_dead(xStack_24)
    cVar7 = iVar12
    while true do
        if cVar7 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:Pause(4.0)
                xStack_44 = quest:ReadGlobalGameDataString(0xe40)
                pCVar9 = quest:GetThingWithScriptName("MK_WQ_STARTING")
                bVar6 = false
                pCVar10 = pCVar9:GetPos()
                r1 = quest:CreateCreature(xStack_44, pCVar10, "QueenHornet")
                quest:SetStateBool("QueenHornetAttacks", true)
                quest:CreateThread("GuildmasterHelp")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetDialogue: lift it as function GuildmasterHelp(quest)
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if (not bVar6) and (not quest:GetStateBool("MissionFailed")) then
                    helper_E13310(quest)
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then break end
        iVar12 = __native_all_dead(xStack_24)
        cVar7 = iVar12
    end
    x_stk_30 = nil
    ::LAB_00e12a9c::
    x_stk_30 = 0
end

function WatchForCutscene(quest)
    local resources = quest:RetailResources()
    local bVar5, pCVar6, pScriptObject, pThing, r1, xStack_10, xStack_20, xStack_38
    local alive = true
    local cVar1 = quest:GetStateBool("QueenHornetAttacks")
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                r1 = quest:GetThingWithScriptName("QueenHornet")
                quest:EntitySetCutsceneBehaviour(r1, 2)
                xStack_20 = resources:NewResource()
                pScriptObject = xStack_20
                pCVar6 = quest:GetHero()
                resources:TryAcquire(pScriptObject, pCVar6, 4)
                xStack_38 = resources:NewActorMap()
                resources:SetActor(xStack_38, "HERO", xStack_20)
                xStack_10 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:RunMacro("CS_WASPBOSS_QUEEN", xStack_38, false, true)
                bVar5 = true
                pCVar6 = r1
                pThing = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(pThing, pCVar6, bVar5)
                quest:CameraDefault()
                quest:SetStateBool("CutsceneFinished", true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_10)
                resources:DestroyActorMap(xStack_38)
                resources:ReleaseResource(xStack_20)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then break end
        cVar1 = quest:GetStateBool("QueenHornetAttacks")
    end
end

function GuildmasterHelp(quest)
    local bVar2, cVar1, fret_0, fret_00, iVar3, r1
    local alive = true
    cVar1 = quest:GetStateBool("CutsceneFinished")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        cVar1 = quest:GetStateBool("CutsceneFinished")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        r1 = quest:GetThingWithScriptName("QueenHornet")
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_10", "", true, true)
            iVar3 = quest:EntityGetBossPhase(r1)
            while iVar3 ~= 3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e12f02 end
                iVar3 = quest:EntityGetBossPhase(r1)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:Pause(1.0)
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_20", "", true, true)
                iVar3 = quest:EntityGetBossPhase(r1)
                while iVar3 ~= 4 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e12f02 end
                    iVar3 = quest:EntityGetBossPhase(r1)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:Pause(1.0)
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_30", "", true, true)
                    iVar3 = quest:EntityGetBossPhase(r1)
                    while iVar3 ~= 6 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e12f02 end
                        iVar3 = quest:EntityGetBossPhase(r1)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        quest:Pause(1.0)
                        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_40", "", true, true)
                        fret_0 = quest:GetHealth(r1)
                        if quest:ReadGlobalGameData(0xe34) ~= fret_0 then
                            repeat
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e12f02 end
                                fret_00 = quest:GetHealth(r1)
                            until not (quest:ReadGlobalGameData(0xe34) ~= fret_00)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            quest:Pause(1.0)
                            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_50", "", true, true)
                        end
                    end
                end
            end
        end
        ::LAB_00e12f02::
    end
end

function helper_E12F20(quest)
    local resources = quest:RetailResources()
    local bVar4, pQuestName
    local alive = true
    quest:SetStateBool("StartChase", true)
    local r1 = quest:GetThingWithScriptName("WaspVictim")
    local xStack_20 = resources:NewResource()
    local pScriptObject = xStack_20
    local xStack_30 = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    resources:TryAcquire(xStack_30, r1, 4)
    local xStack_48 = resources:NewActorMap()
    resources:SetActor(xStack_48, "HERO", xStack_20)
    resources:SetActor(xStack_48, "VICTIM", xStack_30)
    local xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_WASPBOSS_INTRO", xStack_48, false, true)
    if not quest:GetStateBool("QuestStartScreened") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_48)
            resources:ReleaseResource(xStack_30)
            resources:ReleaseResource(xStack_20)
            return
        end
        bVar4 = true
        pQuestName = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(pQuestName, bVar4, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_48)
    resources:ReleaseResource(xStack_30)
    resources:ReleaseResource(xStack_20)
    r1 = nil
end

function helper_E13310(quest)
    local bVar3, bVar4, cVar8, iVar6, pCVar5, pQuestName, xStack_18
    local alive = true
    xStack_18 = nil
    pCVar5 = quest:GetThingWithScriptName("QueenHornet")
    xStack_18 = pCVar5
    pCVar5 = nil
    quest:DisplayQuestInfo(true)
    cVar8 = quest:GetStateBool("QueenHornetAttacks")
    while not cVar8 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00e13769: (native jump target)
            xStack_18 = nil
            return
        end
        cVar8 = quest:GetStateBool("QueenHornetAttacks")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_WASP_MENACE_OBJECTIVE_02", "", "HeroGuildComplexInside")
        iVar6 = quest:AddQuestInfoBarHealth(xStack_18, {R = 255, G = 255, B = 0, A = 255}, "HUD_ICON_WASP_HEAD", 1.0)
        while true do
            while true do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e135bd end
                bVar3 = quest:IsLevelLoaded("PicnicArea")
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar3 then break end
                if bVar4 then goto LAB_00e135bd end
                if not (xStack_18 ~= nil and not xStack_18:IsNull()) then
                    cVar8 = false
                else
                    cVar8 = (xStack_18 ~= nil and xStack_18:MsgIsKilledBy(""))
                end
                if cVar8 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:DisplayQuestInfo(false)
                        quest:RemoveQuestInfoElement(iVar6)
                        helper_E137B0(quest)
                        quest:SetStateBool("MissionSucceeded", true)
                    end
                    goto LAB_00e135bd
                end
            end
            if bVar4 then break end
            bVar3 = quest:IsLevelLoaded("PicnicArea")
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    xStack_18 = nil
                    return
                end
                bVar3 = quest:IsLevelLoaded("PicnicArea")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then break end
            pCVar5 = quest:GetThingWithScriptName("QueenHornet")
            xStack_18 = pCVar5
            pCVar5 = nil
            quest:DisplayQuestInfo(true)
            iVar6 = quest:AddQuestInfoBarHealth(xStack_18, {R = 255, G = 255, B = 0, A = 255}, "HUD_ICON_WASP_HEAD", 1.0)
        end
    end
    ::LAB_00e135bd::
end

function helper_E137B0(quest)
    local resources = quest:RetailResources()
    local cVar4, pCVar5, pScriptObject, r1, xStack_10, xStack_20, xStack_44
    local alive = true
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    local bVar3 = not alive
    if not bVar3 then
        quest:SetStateBool("PanickedVillagersScene", true)
        xStack_10 = resources:NewResource()
        pScriptObject = xStack_10
        pCVar5 = quest:GetHero()
        resources:TryAcquire(pScriptObject, pCVar5, 4)
        xStack_20 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:Pause(3.0)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(0.5)
        pCVar5 = quest:GetThingWithScriptName("QueenDepositPos1")
        r1 = quest:GetNearestWithDefName(pCVar5, "CREATURE_HORNET_QUEEN_01")
        pCVar5 = nil
        if (r1 ~= nil and not r1:IsNull()) then
            cVar4 = (r1 ~= nil and r1:IsAlive())
            if cVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                    resources:ReleaseResource(xStack_10)
                    return
                end
                quest:RemoveThing(r1, false, true)
            end
        end
        xStack_44 = resources:NewActorMap()
        resources:SetActor(xStack_44, "HERO", xStack_10)
        resources:RunMacro("CS_WASPBOSS_OUTRO", xStack_44, false, true)
        resources:DestroyActorMap(xStack_44)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_20)
        resources:ReleaseResource(xStack_10)
        pCVar5 = quest:GetThingWithScriptName("VILL1")
        quest:EntitySetInFaction(pCVar5, "FACTION_PICNIC_AREA")
        pCVar5 = quest:GetThingWithScriptName("VILL2")
        quest:EntitySetInFaction(pCVar5, "FACTION_PICNIC_AREA")
        pCVar5 = quest:GetThingWithScriptName("VILL3")
        quest:EntitySetInFaction(pCVar5, "FACTION_PICNIC_AREA")
    end
end

