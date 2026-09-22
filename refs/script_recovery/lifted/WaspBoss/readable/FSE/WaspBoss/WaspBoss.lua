-- Readable native conversion: Q_WaspBoss. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)
local TUTORIAL_CATEGORY_CAMERA = 6  -- ETutorialCategory (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_LowHealth = 3636,  -- 4
    WB_ExperienceReward = 3672,  -- 800
}

-- Q_WaspBoss.Main (retail 0x00e0ea40)
function Main(quest)
    quest:SetTimer(quest:GetStateInt("ReachedWaspHelper"), 120)
    while not quest:IsRegionLoaded("LookoutPoint") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:GiveHeroTutorial(TUTORIAL_CATEGORY_CAMERA)
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
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01", "", "HeroGuildComplexInside")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_028_GUILDSEAL_WASP_MAP", "", true, true)
    while not quest:IsLevelLoaded("PicnicArea") do
        if not quest:NewScriptFrame() then return end
    end
    local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then return end
    quest:OverrideMusic(23, false, false)
    quest:CreateThread("WatchForTermination")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function WatchForTermination(quest)
    quest:CreateThread("DoMission")  -- native thread body CQ_WaspBossScript::DoMission: lift it as function DoMission(quest)
    -- TODO(native): bVar2 = pCVar4 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
    if not isActiveThreadTerminating then
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(pCVar4,&xStack_8,0);
        -- TODO(native): *(code **)(pCVar4 + 0x34) = CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetCutscene;
        -- TODO(native): *(CQ_GuildTrainingScript **)(pCVar4 + 0x38) = this;
    end
end

-- Q_WaspBoss.Init (retail 0x00e0e820)
function Init(quest)
    quest:SetStateInt("ReachedWaspHelper", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:AddQuestRegion("Q_WaspBoss", "PicnicArea")
    quest:SetQuestWorldMapOffset("Q_WaspBoss", 10, nil --[[missing]])
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

-- Q_WaspBoss.OnPersist (retail 0x00e0e9a0)
function OnPersist(quest, context)
    quest:SetStateBool("QuestStartScreened", quest:PersistTransferBool(context, "QuestStartScreened", quest:GetStateBool("QuestStartScreened")))
    quest:SetStateInt("SavedVillagerCount", quest:PersistTransferInt(context, "SavedVillagerCount", quest:GetStateInt("SavedVillagerCount") or 0))
end

-- Q_WaspBoss.WatchForTermination (retail 0x00e0f1d0)
function WatchForTermination(quest)
    local missionFailed = quest:GetStateBool("MissionFailed")
    while not missionFailed and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not missionFailed then
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.WB_ExperienceReward))
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
        quest:StopOverrideMusic(false)
        quest:FadeScreenIn()
        while quest:IsLevelLoaded("PicnicArea") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FIRST", "", true, true)
    else
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
        quest:StopOverrideMusic(false)
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_WaspBoss.DoMission (retail 0x00e12580)
function DoMission(quest)
    local scratchValue, scratchValue2, getThingWithScriptName, this_00, this_01, waspAttacker
    local x_stk_30_1
    quest:AddRumourCategory("Post waspboss killed")
    quest:AddNewRumourToCategory("Post waspboss killed", "TEXT_AI_GOSSIP_WASPBOSS_KILLED")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_PICNIC_AREA")
    x_stk_30_1 = nil
    scratchValue2 = 1
    repeat
        if quest:IsActiveThreadTerminating() then goto LAB_00e12a9c end
        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0xe3c),(int)&xStack_44);
        quest:GetThingWithScriptName("QueenDepositPos" .. tostring(scratchValue2))
        -- TODO(native): this_00 = *(this + 0x40)
        this_00 = nil --[[unresolved native value]]
        -- TODO(native): xStack_40 = *this_00;
        -- TODO(native): pCVar10 = (**(*pCVar9 + 0x18))(pCVar9)
--[[unresolved native value]]
        -- TODO(native): pCVar9 = (**(xStack_40 + 0x16c))(this_00,xStack_18,&xStack_44,pCVar10,"HornetDrone",bVar6)
        getThingWithScriptName = nil --[[unresolved native value]]
        -- TODO(native): piVar2 = *(pCVar9 + 0x8)
    --[[unresolved native value]]
        -- TODO(native): uVar3 = *(pCVar9 + 0x4)
    --[[unresolved native value]]
        if x_stk_30_1 ~= nil then
            x_stk_30_1 = nil
            if nil ~= nil then
                -- TODO(native): *piVar2 = *piVar2 + 1;
            end
        end
        scratchValue2 = scratchValue2 + 1
    until scratchValue2 >= 6
    if quest:IsActiveThreadTerminating() then return end
    waspAttacker = quest:GetAllThingsWithScriptName("WaspAttacker")
    WaspIntro(quest)
    -- TODO(native): iVar12 = AreAllThingsInVectorDead(&xStack_24)
    --[[unresolved native value]]
    scratchValue = nil
    while true do
        if scratchValue ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            quest:Pause(4.0)
            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0xe40),(int)&xStack_44);
            quest:CreateCreature("QueenHornet", quest:GetThingWithScriptName("MK_WQ_STARTING"):GetPos(), waspAttacker)
            quest:SetStateBool("QueenHornetAttacks", true)
            if this_01 ~= nil then
                -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(this_01,&xStack_40,0);
                -- TODO(native): *(code **)(this_01 + 0x34) = CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetDialogue;
                -- TODO(native): *(CQ_WaspBossScript **)(this_01 + 0x38) = this;
            end
            -- TODO(native): CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,this_01,sectionName);
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
                helper_E13310(quest)
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        -- TODO(native): iVar12 = AreAllThingsInVectorDead(&xStack_24)
    --[[unresolved native value]]
        scratchValue = nil
    end
    ::LAB_00e12a9c::
end

-- Q_WaspBoss.WatchForCutscene (retail 0x00e12330)
function WatchForCutscene(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local queenHornetAttacks = quest:GetStateBool("QueenHornetAttacks")
    while true do
        if queenHornetAttacks then
            if quest:IsActiveThreadTerminating() then return end
            quest:EntitySetCutsceneBehaviour(quest:GetThingWithScriptName("QueenHornet"), CUTSCENE_BEHAVIOUR_NOT_PAUSED)
            local resource = resources:NewResource()
            resources:TryAcquire(resource, hero, 4)
            local actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_WASPBOSS_QUEEN", actorMap, false, true)
            quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]], true)
            quest:CameraDefault()
            quest:SetStateBool("CutsceneFinished", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        if not quest:NewScriptFrame() then break end
        queenHornetAttacks = quest:GetStateBool("QueenHornetAttacks")
    end
end

-- Q_WaspBoss.GuildmasterHelp (retail 0x00e12b30)
function GuildmasterHelp(quest)
    local fret_0, fret_00
    while not quest:GetStateBool("CutsceneFinished") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local queenHornet = quest:GetThingWithScriptName("QueenHornet")
    if not quest:NewScriptFrame() then goto LAB_00e12f02 end
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_10", "", true, true)
    while quest:EntityGetBossPhase(queenHornet) ~= 3 do
        if not quest:NewScriptFrame() then goto LAB_00e12f02 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e12f02 end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_20", "", true, true)
    while quest:EntityGetBossPhase(queenHornet) ~= 4 do
        if not quest:NewScriptFrame() then goto LAB_00e12f02 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e12f02 end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_30", "", true, true)
    while quest:EntityGetBossPhase(queenHornet) ~= 6 do
        if not quest:NewScriptFrame() then goto LAB_00e12f02 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e12f02 end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_40", "", true, true)
    fret_0 = quest:GetHealth(queenHornet)
    if quest:ReadGlobalGameData(SCRIPT_DEF.WB_LowHealth) ~= fret_0 then
        repeat
            if not quest:NewScriptFrame() then goto LAB_00e12f02 end
            fret_00 = quest:GetHealth(queenHornet)
        until quest:ReadGlobalGameData(SCRIPT_DEF.WB_LowHealth) == fret_00
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e12f02 end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_50", "", true, true)
    ::LAB_00e12f02::
end

-- Q_WaspBoss.WaspIntro (retail 0x00e12f20)
-- E12F20: bsim names this body NScript::CQ_WaspBossScript::WaspIntro (a homologous script member); no PDB name
function WaspIntro(quest)
    local resources = quest:RetailResources()
    quest:SetStateBool("StartChase", true)
    local waspVictim = quest:GetThingWithScriptName("WaspVictim")
    resources:NewResource()
    resources:NewResource()
    resources:TryAcquire(0, quest:GetHero(), 4)
    waspVictim:AcquireControl(4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", 0)
    resources:SetActor(actorMap, "VICTIM", 0)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_WASPBOSS_INTRO", actorMap, false, true)
    if not quest:GetStateBool("QuestStartScreened") then
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(0)
            resources:ReleaseResource(0)
            return
        end
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(0)
    resources:ReleaseResource(0)
end

-- Q_WaspBoss.helper_E13310 (retail 0x00e13310)
function helper_E13310(quest)
    local predicateResult, scratchValue, healthBar, scratchValue2
    scratchValue2 = nil
    -- TODO(native): piVar1 = *(pCVar5 + 0x8)
    --[[unresolved native value]]
    -- TODO(native): piVar2 = *(pCVar5 + 0x4)
    --[[unresolved native value]]
    if nil ~= nil then
        scratchValue2 = nil
        if nil ~= nil then
            -- TODO(native): *piVar1 = *piVar1 + 1;
        end
    end
    quest:DisplayQuestInfo(true)
    while not quest:GetStateBool("QueenHornetAttacks") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e135bd end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WASP_MENACE_OBJECTIVE_02", "", "HeroGuildComplexInside")
    healthBar = quest:AddQuestInfoBarHealth(scratchValue2, {R = 255, G = 255, B = 0, A = 255}, "HUD_ICON_WASP_HEAD", 1.0)
    while true do
        while true do
            if not quest:NewScriptFrame() then goto LAB_00e135bd end
            predicateResult = false
            if not quest:IsLevelLoaded("PicnicArea") then break end
            if not (scratchValue2 ~= nil and not scratchValue2:IsNull()) then
                scratchValue = 0
            else
                scratchValue = scratchValue2 ~= nil and scratchValue2:MsgIsKilledBy("")
            end
            if scratchValue then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(healthBar)
                    helper_E137B0(quest)
                    quest:SetStateBool("MissionSucceeded", true)
                end
                goto LAB_00e135bd
            end
        end
        if predicateResult then break end
        while not quest:IsLevelLoaded("PicnicArea") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        -- TODO(native): piVar1 = *(pCVar5 + 0x8)
    --[[unresolved native value]]
        -- TODO(native): piVar2 = *(pCVar5 + 0x4)
    --[[unresolved native value]]
        if scratchValue2 ~= nil then
            scratchValue2 = nil
            if nil ~= nil then
                -- TODO(native): *piVar1 = *piVar1 + 1;
            end
        end
        quest:DisplayQuestInfo(true)
        healthBar = quest:AddQuestInfoBarHealth(nil --[[missing]], {R = 255, G = 255, B = 0, A = 255}, scratchValue2, 1.0)
    end
    ::LAB_00e135bd::
end

-- Q_WaspBoss.helper_E137B0 (retail 0x00e137b0)
function helper_E137B0(quest)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame() then return end
    quest:SetStateBool("PanickedVillagersScene", true)
    resources:TryAcquire(resources:NewResource(), quest:GetHero(), 4)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:Pause(3.0)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(0.5)
    local getNearestWithDefName = quest:GetNearestWithDefName(quest:GetThingWithScriptName("QueenDepositPos1"), "CREATURE_HORNET_QUEEN_01")
    if getNearestWithDefName ~= nil then
        if getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive() then
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(0)
                return
            end
            quest:RemoveThing(getNearestWithDefName, false, true)
        end
    end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", 0)
    resources:RunMacro("CS_WASPBOSS_OUTRO", actorMap, false, true)
    resources:DestroyActorMap(actorMap)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(0)
    quest:EntitySetInFaction(quest:GetThingWithScriptName("VILL1"), "FACTION_PICNIC_AREA")
    quest:EntitySetInFaction(quest:GetThingWithScriptName("VILL2"), "FACTION_PICNIC_AREA")
    quest:EntitySetInFaction(quest:GetThingWithScriptName("VILL3"), "FACTION_PICNIC_AREA")
end

