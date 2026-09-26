-- Readable native conversion: Q_BanditCamp. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BanditCampNoDamageBoastCost = 136,  -- 500
    BanditCampNoDamageBoastReward = 140,  -- 3000
    BAC_BoastBanditKill = 3720,  -- 24.0
}

-- Q_BanditCamp.Main (retail 0x00d00af0)
function Main(quest)
    local hero = quest:GetHero()
    quest:AddEntityBinding("Gate1GuardOuter", "BanditCamp/Entities/Gate1GuardOuter")
    quest:AddEntityBinding("Gate1GuardInner", "BanditCamp/Entities/Gate1GuardInner")
    quest:FinalizeEntityBindings()
    if not quest:GetStateBool("ActivatedBanditRaid") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_01", "", "")
        quest:ActivateQuest("Q_OakValeBanditRaid")
        quest:SetStateBool("ActivatedBanditRaid", true)
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptBase_WaitForEntityAndExecuteOnePair: lift it as function WatchForTermination(quest)
    quest:CreateThread("WatchForCostume")  -- native thread body WatchForCostume: lift it as function WatchForCostume(quest)
    quest:CreateThread("CheckAnyBanditsKilled")  -- native thread body CheckAnyBanditsKilled: lift it as function CheckAnyBanditsKilled(quest)
    quest:CreateThread("WatchForEndOfScript")  -- native thread body 0x00D013A0: lift it as function WatchForEndOfScript(quest)
    if not quest:GetStateBool("Gate1Open") then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:GetStateBool("Gate1Open") do
            if not quest:NewScriptFrame() then return end
        end
        quest:AutoSaveCheckPoint()
    end
    if quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero) then
        if quest:IsActiveThreadTerminating() then return end
        quest:TakeObjectFromHero("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
    end
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNOBANDITS", 24, quest:ReadGlobalGameData(1012), quest:ReadGlobalGameData(1016), true, "", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLMANYBANDITS", 27, quest:ReadGlobalGameData(1020), quest:ReadGlobalGameData(1024), true, "", 1)
end

-- Q_BanditCamp.OnPersist (retail 0x00d00a90)
function OnPersist(quest, context)
    quest:SetStateBool("Gate1Open", quest:PersistTransferBool(context, "Gate1Open", quest:GetStateBool("Gate1Open")))
    quest:SetStateBool("BoastBegun", quest:PersistTransferBool(context, "BoastBegun", quest:GetStateBool("BoastBegun")))
    quest:SetStateBool("ActivatedBanditRaid", quest:PersistTransferBool(context, "ActivatedBanditRaid", quest:GetStateBool("ActivatedBanditRaid")))
end

-- Q_BanditCamp.WatchForTermination (retail 0x00d01290)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionSucceeded") then
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
    else
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_BanditCamp.WatchForCostume (retail 0x00d00fd0)
function WatchForCostume(quest)
    local count
    local hero = quest:GetHero()
    while not quest:IsLevelLoaded("BanditCampPath_1") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local id = quest:AddQuestInfoCounter("HUD_BANDIT_CLOTHES_ICON", 5, 1.0)
    quest:DisplayQuestInfo(true)
    repeat
        if not quest:NewScriptFrame() then return end
        count = 0
        if quest:IsObjectInThingsPossession("OBJECT_HERO_HAT_BANDITCAMP", hero) then
            count = 1
        end
        if quest:IsObjectInThingsPossession("OBJECT_HERO_GLOVES_BANDITCAMP", hero) then
            count = count + 1
        end
        if quest:IsObjectInThingsPossession("OBJECT_HERO_BOOTS_BANDITCAMP", hero) then
            count = count + 1
        end
        if quest:IsObjectInThingsPossession("OBJECT_HERO_SHIRT_BANDITCAMP", hero) then
            count = count + 1
        end
        if not quest:IsObjectInThingsPossession("OBJECT_HERO_TROUSERS_BANDITCAMP", hero) then
            quest:UpdateQuestInfoCounter(id, count, -1)
        else
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
    local msgGetThingsKilledGroups, addQuestInfoCounter, sequence, sequence13, scratchValue
    local scratchValue7, scratchValue8
    scratchValue7 = 0
    if not quest:GetStateBool("BoastBegun") then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("BanditCampPathEntrance") do
            if not quest:NewScriptFrame() then return end
        end
    end
    quest:SetStateBool("BoastBegun", true)
    addQuestInfoCounter = -1
    if quest:GetMasterGameState("BanditCampKillManyBandits") then
        if quest:IsActiveThreadTerminating() then return end
        addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_BANDIT", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill))), 1.0)
    end
    if quest:IsActiveThreadTerminating() then return end
    while true do
        -- TODO(native): ::__EH_epilog3(auStack_4,(int)&uStack_15,0);
        msgGetThingsKilledGroups = quest:GetHero():MsgGetThingsKilledGroups()
        if #msgGetThingsKilledGroups ~= 0 then break end
        if quest:GetMasterGameState("BanditCampKillNoBandits") and false then
            if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
            quest:SetMasterGameState("BanditCampKillNoBandits", false)
        end
        sequence = not quest:GetMasterGameState("BanditCampKillManyBandits")
        if sequence then
            scratchValue8 = 0
            sequence = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) <= 0
        end
        if sequence then
            if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
            quest:SetMasterGameState("BanditCampKillManyBandits", true)
            quest:UpdateQuestInfoCounter(addQuestInfoCounter, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) - scratchValue8)), -1)
        end
        if not quest:NewScriptFrame() then return end
    end
    ::FLOW_after_lab_00d034ad::
    if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
    scratchValue = 0
    if #msgGetThingsKilledGroups ~= 0 then
        repeat
            if msgGetThingsKilledGroups[scratchValue + 1] & 4 == 0 then
                scratchValue = scratchValue + 1
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
                scratchValue7 = scratchValue7 + 1
                scratchValue = scratchValue + 1
            end
        until scratchValue >= #msgGetThingsKilledGroups
    end
    msgGetThingsKilledGroups = {}
    if quest:GetMasterGameState("BanditCampKillNoBandits") and scratchValue7 ~= 0 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
        quest:SetMasterGameState("BanditCampKillNoBandits", false)
    end
    sequence13 = not quest:GetMasterGameState("BanditCampKillManyBandits")
    if sequence13 then
        scratchValue8 = scratchValue7
        sequence13 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) <= scratchValue8
    end
    if sequence13 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
        quest:SetMasterGameState("BanditCampKillManyBandits", true)
        quest:UpdateQuestInfoCounter(addQuestInfoCounter, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) - scratchValue8)), -1)
    end
    if not quest:NewScriptFrame() then return end
    goto FLOW_after_lab_00d034ad
    ::LAB_00d03564::
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
    quest:SetStateBool("MissionSucceeded", true)
end

