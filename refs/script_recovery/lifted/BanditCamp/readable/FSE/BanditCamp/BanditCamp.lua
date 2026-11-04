-- Readable native conversion: Q_BanditCamp. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BanditCampNoDamageBoastCost = 136,  -- 500
    BanditCampNoDamageBoastReward = 140,  -- 3000
    BanditCampKillNoBanditsBoastCost = 1012,  -- 400
    BanditCampKillNoBanditsBoastReward = 1016,  -- 1200
    BanditCampKillManyBanditsBoastCost = 1020,  -- 200
    BanditCampKillManyBanditsBoastReward = 1024,  -- 400
    BAC_BoastBanditKill = 3720,  -- 24.0
}

-- Q_BanditCamp.Main (retail 0x00d00af0)
function Main(quest)
    local isActiveThreadTerminating
    quest:AddEntityBinding("Gate1GuardOuter", "BanditCamp/Entities/Gate1GuardOuter")
    quest:AddEntityBinding("Gate1GuardInner", "BanditCamp/Entities/Gate1GuardInner")
    quest:FinalizeEntityBindings()
    if not quest:GetStateBool("ActivatedBanditRaid") then
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return
        end
        quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_01", "", "")
        quest:ActivateQuest("Q_OakValeBanditRaid")
        quest:SetStateBool("ActivatedBanditRaid", true)
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptBase_WaitForEntityAndExecuteOnePair: lift it as function WatchForTermination(quest)
    if not isActiveThreadTerminating then
    end
    quest:CreateThread("WatchForCostume")  -- native thread body WatchForCostume: lift it as function WatchForCostume(quest)
    if not isActiveThreadTerminating then
    end
    quest:CreateThread("CheckAnyBanditsKilled")  -- native thread body CheckAnyBanditsKilled: lift it as function CheckAnyBanditsKilled(quest)
    if not isActiveThreadTerminating then
    end
    quest:CreateThread("WatchForEndOfScript")  -- native thread body 0x00D013A0: lift it as function WatchForEndOfScript(quest)
    if not isActiveThreadTerminating then
    end
    if not quest:GetStateBool("Gate1Open") then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:GetStateBool("Gate1Open") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:AutoSaveCheckPoint()
    end
    local hero = quest:GetHero()
    if quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero) then
        if quest:IsActiveThreadTerminating() then return end
        quest:TakeObjectFromHero("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
    end
    hero = quest:GetHero()
    if quest:IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", hero) and not quest:IsActiveThreadTerminating() then
        quest:TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
    end
end

-- Q_BanditCamp.Init (retail 0x00d006d0)
function Init(quest)
    quest:AddQuestRegion("Q_BanditCamp", "BanditCampPathEntrance")
    quest:AddQuestRegion("Q_BanditCamp", "BanditCampEntrance")
    quest:AddQuestRegion("Q_BanditCamp", "BanditCampPath1")
    quest:AddQuestRegion("Q_BanditCamp", "BanditCampCentre")
    quest:AddQuestRegion("Q_BanditCamp", "BanditCampBoss")
    quest:AddQuestRegion("Q_BanditCamp", "DemonDoor_BanditCampPath")
    quest:SetStateBool("AttackedOuterGateGuards", false)
    quest:SetStateBool("Gate1Open", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("BoastBegun", false)
    quest:SetStateBool("ActivatedBanditRaid", false)
    quest:SetMasterGameState("BanditCampKillManyBandits", false)
    quest:SetMasterGameState("BanditCampKillNoBandits", true)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.BanditCampNoDamageBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.BanditCampNoDamageBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNOBANDITS", 24, quest:ReadGlobalGameData(SCRIPT_DEF.BanditCampKillNoBanditsBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.BanditCampKillNoBanditsBoastReward), true, "", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLMANYBANDITS", 27, quest:ReadGlobalGameData(SCRIPT_DEF.BanditCampKillManyBanditsBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.BanditCampKillManyBanditsBoastReward), true, "", 1)
end

-- Q_BanditCamp.OnPersist (retail 0x00d00a90)
function OnPersist(quest, context)
    local gate1Open = quest:GetStateBool("Gate1Open") or false
    gate1Open = quest:PersistTransferBool(context, "Gate1Open", gate1Open)
    quest:SetStateBool("Gate1Open", gate1Open)
    local boastBegun = quest:GetStateBool("BoastBegun") or false
    boastBegun = quest:PersistTransferBool(context, "BoastBegun", boastBegun)
    quest:SetStateBool("BoastBegun", boastBegun)
    local activatedBanditRaid = quest:GetStateBool("ActivatedBanditRaid") or false
    activatedBanditRaid = quest:PersistTransferBool(context, "ActivatedBanditRaid", activatedBanditRaid)
    quest:SetStateBool("ActivatedBanditRaid", activatedBanditRaid)
end

-- Q_BanditCamp.WatchForTermination (retail 0x00d01290)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionSucceeded") then
        if quest:IsActiveThreadTerminating() then return end
        local pQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pQuestName, true, "", true)
    else
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName, true, false, false)
    end
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName2, 0)
end

-- Q_BanditCamp.WatchForCostume (retail 0x00d00fd0)
function WatchForCostume(quest)
    local count
    while not quest:IsLevelLoaded("BanditCampPath_1") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local id = quest:AddQuestInfoCounter("HUD_BANDIT_CLOTHES_ICON", 5, 1.0)
    quest:DisplayQuestInfo(true)
    repeat
        if not quest:NewScriptFrame() then return end
        count = 0
        local hero = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_HERO_HAT_BANDITCAMP", hero) then
            if quest:IsActiveThreadTerminating() then return end
            count = 1
        end
        local hero2 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_HERO_GLOVES_BANDITCAMP", hero2) then
            if quest:IsActiveThreadTerminating() then return end
            count = count + 1
        end
        local hero3 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_HERO_BOOTS_BANDITCAMP", hero3) then
            if quest:IsActiveThreadTerminating() then return end
            count = count + 1
        end
        local hero4 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_HERO_SHIRT_BANDITCAMP", hero4) then
            if quest:IsActiveThreadTerminating() then return end
            count = count + 1
        end
        local hero5 = quest:GetHero()
        if not quest:IsObjectInThingsPossession("OBJECT_HERO_TROUSERS_BANDITCAMP", hero5) then
            quest:UpdateQuestInfoCounter(id, count, -1)
        else
            if quest:IsActiveThreadTerminating() then return end
            count = count + 1
            quest:UpdateQuestInfoCounter(id, count, -1)
        end
    until count == 5
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(id)
    quest:DisplayQuestInfo(false)
end

-- Q_BanditCamp.CheckAnyBanditsKilled (retail 0x00d032f0)
function CheckAnyBanditsKilled(quest)
    local addQuestInfoCounter, sequence, scratchValue6, scratchValue7, scratchValue8
    scratchValue7 = 0
    if not quest:GetStateBool("BoastBegun") then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("BanditCampPathEntrance") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:SetStateBool("BoastBegun", true)
    addQuestInfoCounter = -1
    if quest:GetMasterGameState("BanditCampKillManyBandits") then
        if quest:IsActiveThreadTerminating() then return end
        local scratchValue4 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill)))
        addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_BANDIT", scratchValue4, 1.0)
    end
    if quest:IsActiveThreadTerminating() then return end
    ::FLOW_loop_head_lab_00d034ad::
    local hero = quest:GetHero():MsgGetThingsKilledGroups()
    if #hero ~= 0 then goto FLOW_loop_exit_lab_00d034ad end
    ::LAB_00d034ad::
    if quest:GetMasterGameState("BanditCampKillNoBandits") and scratchValue7 ~= 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetMasterGameState("BanditCampKillNoBandits", false)
    end
    sequence = not quest:GetMasterGameState("BanditCampKillManyBandits")
    if sequence then
        scratchValue8 = scratchValue7
        sequence = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) <= scratchValue8
    end
    if sequence then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetMasterGameState("BanditCampKillManyBandits", true)
        local scratchValue5 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) - scratchValue8))
        quest:UpdateQuestInfoCounter(addQuestInfoCounter, scratchValue5, -1)
    end
    if not quest:NewScriptFrame() then return end
    goto FLOW_loop_head_lab_00d034ad
    ::FLOW_loop_exit_lab_00d034ad::
    if quest:IsActiveThreadTerminating() then return end
    scratchValue6 = 0
    if #hero ~= 0 then
        repeat
            if (hero[scratchValue6 + 1] & 4) == 0 then
                scratchValue6 = scratchValue6 + 1
            else
                if quest:IsActiveThreadTerminating() then return end
                scratchValue7 = scratchValue7 + 1
                scratchValue6 = scratchValue6 + 1
            end
        until not (scratchValue6 < (#hero))
    end
    goto LAB_00d034ad
end

-- Q_BanditCamp.WatchForEndOfScript (retail 0x00d013a0)
function WatchForEndOfScript(quest)
    if not quest:IsRegionLoaded("BanditCampBoss") then
        if quest:IsActiveThreadTerminating() then return end
        quest:ActivateQuest("Q_BanditCampBossBattle")
    end
    if not quest:NewScriptFrame() then return end
    while quest:IsQuestActive("Q_BanditCampBossBattle") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("MissionSucceeded", true)
end

