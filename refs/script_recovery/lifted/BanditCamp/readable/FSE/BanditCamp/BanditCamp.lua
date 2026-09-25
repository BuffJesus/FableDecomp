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
    local scratchValue3, scratchValue4, scratchValue6, scratchValue7, scratchValue8, this_00
    local scratchValue9
    if not quest:GetStateBool("BoastBegun") then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("BanditCampPathEntrance") do
            if not quest:NewScriptFrame() then return end
        end
    end
    quest:SetStateBool("BoastBegun", true)
    if quest:GetMasterGameState("BanditCampKillManyBandits") then
        if quest:IsActiveThreadTerminating() then return end
        math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill)))
        -- TODO(native): iVar6 = (**(iVar6 + 0x51c))(piVar7,"HUD_QUEST_ICON_BANDIT",iVar4,fVar10)
--[[unresolved native value]]
    end
    scratchValue7 = 0
    if quest:IsActiveThreadTerminating() then return end
    while true do
        -- TODO(native): piVar8 = *piVar7
        scratchValue8 = nil --[[unresolved native value]]
        -- TODO(native): pcVar2 = *(*piVar1 + 0x118)
--[[unresolved native value]]
        -- TODO(native): *piVar7 = 0xd03439;
        -- TODO(native): this_00 = (*pcVar2)(piVar1)
        this_00 = nil --[[unresolved native value]]
        -- TODO(native): iVar4 = *this_00
        scratchValue3 = nil --[[unresolved native value]]
        -- TODO(native): *piVar7 = (int)(piVar7 + 8);
        -- TODO(native): pcVar2 = *(iVar4 + 0xdc)
--[[unresolved native value]]
        -- TODO(native): piVar7[-1] = 0xd03448;
        -- TODO(native): bVar3 = (*pcVar2)(this_00,*piVar7)
    --[[unresolved native value]]
        if nil then break end
        if quest:GetMasterGameState("BanditCampKillNoBandits") and scratchValue7[7] ~= scratchValue8 then
            -- TODO(native): *piVar7 = 0xd034c7;
            if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
            quest:SetMasterGameState("BanditCampKillNoBandits", false)
        end
        if not quest:GetMasterGameState("BanditCampKillManyBandits") and scratchValue7[6] = scratchValue7[7], quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) <= scratchValue7[7] then
            -- TODO(native): *piVar7 = 0xd03506;
            if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
            quest:SetMasterGameState("BanditCampKillManyBandits", true)
            local scratchValue = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill)
            -- TODO(native): *piVar7 = -1;
            -- TODO(native): piVar7[-1] = 0xd03530;
            math.tointeger(math.modf(scratchValue - scratchValue7[6]))
            -- TODO(native): piVar7[-1] = iVar5;
            -- TODO(native): piVar7[-2] = iVar6;
            -- TODO(native): pcVar2 = *(code **)(DAT_0143e90c + 0x53c);
            -- TODO(native): piVar7[-3] = 0xd0353a;
            -- TODO(native): (*pcVar2)(piVar1,piVar7[-2],piVar7[-1],*piVar7);
            scratchValue8 = 0
        end
        -- TODO(native): *piVar7 = 0xd03545;
        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(piVar7 + 8));
        -- TODO(native): pcVar2 = *(*piVar1 + 0x1c)
--[[unresolved native value]]
        -- TODO(native): *piVar7 = 0xd0354d;
        -- TODO(native): (*pcVar2)(piVar1);
        -- TODO(native): *piVar7 = 0xd03554;
        scratchValue7 = scratchValue8
        if quest:IsActiveThreadTerminating() then return end
    end
    ::FLOW_after_lab_00d034ad::
    -- TODO(native): *piVar7 = 0xd03453;
    if not quest:IsActiveThreadTerminating() then
        scratchValue4 = scratchValue7[9]
        scratchValue6 = scratchValue7[8]
        scratchValue9 = 0
        if scratchValue4 - scratchValue6 >> 2 ~= 0 then
            repeat
                -- TODO(native): if (*(iVar5 + uVar9 * 4) & 4) ~= 0 then
                scratchValue9 = scratchValue9 + 1; goto continue_1
                -- TODO(native): *piVar7 = 0xd0347d;
                if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
                scratchValue4 = scratchValue7[9]
                -- TODO(native): piVar7[7] = piVar7[7] + 1;
                scratchValue6 = scratchValue7[8]
                scratchValue9 = scratchValue9 + 1
                ::continue_1::
            until scratchValue9 >= (scratchValue4 - scratchValue6 >> 2)
        end
        -- TODO(native): *piVar7 = iVar4;
        -- TODO(native): piVar7[-1] = iVar5;
        -- TODO(native): piVar7[-2] = 0xd034ad;
        -- TODO(native): std_vector_push_copy_element(piVar7 + 8,piVar7[-1],*piVar7);
        if quest:GetMasterGameState("BanditCampKillNoBandits") and scratchValue7[7] ~= scratchValue8 then
            -- TODO(native): *piVar7 = 0xd034c7;
            if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
            quest:SetMasterGameState("BanditCampKillNoBandits", false)
        end
        if not quest:GetMasterGameState("BanditCampKillManyBandits") and scratchValue7[6] = scratchValue7[7], quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill) <= scratchValue7[7] then
            -- TODO(native): *piVar7 = 0xd03506;
            if quest:IsActiveThreadTerminating() then goto LAB_00d03564 end
            quest:SetMasterGameState("BanditCampKillManyBandits", true)
            local scratchValue2 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_BoastBanditKill)
            -- TODO(native): *piVar7 = -1;
            -- TODO(native): piVar7[-1] = 0xd03530;
            math.tointeger(math.modf(scratchValue2 - scratchValue7[6]))
            -- TODO(native): piVar7[-1] = iVar5;
            -- TODO(native): piVar7[-2] = iVar6;
            -- TODO(native): pcVar2 = *(code **)(DAT_0143e90c + 0x53c);
            -- TODO(native): piVar7[-3] = 0xd0353a;
            -- TODO(native): (*pcVar2)(piVar1,piVar7[-2],piVar7[-1],*piVar7);
            scratchValue8 = 0
        end
        -- TODO(native): *piVar7 = 0xd03545;
        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(piVar7 + 8));
        -- TODO(native): pcVar2 = *(*piVar1 + 0x1c)
--[[unresolved native value]]
        -- TODO(native): *piVar7 = 0xd0354d;
        -- TODO(native): (*pcVar2)(piVar1);
        -- TODO(native): *piVar7 = 0xd03554;
        scratchValue7 = scratchValue8
        if quest:IsActiveThreadTerminating() then return end
        goto FLOW_after_lab_00d034ad
    end
    ::LAB_00d03564::
    -- TODO(native): *piVar7 = 0xd0356d;
    -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(piVar7 + 8));
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

