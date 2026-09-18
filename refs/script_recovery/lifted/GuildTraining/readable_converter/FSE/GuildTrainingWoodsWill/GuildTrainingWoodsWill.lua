-- Readable native conversion: Q_GuildTrainingWoodsWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MinHealth = 3800,  -- 6.0
}

-- Q_GuildTrainingWoodsWill.Main (retail 0x00d67890)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, ctr_84, ctr_88, scratchValue2, scratchValue3, conversationId, scratchValue6
    local scratchValue7, willWhisper, scratchValue9, resource, resource2, willBandit
    scratchValue2 = 0
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    quest:SetStateBool("WhisperAnimate", false)
    quest:SetStateBool("BanditsAlive", true)
    quest:AddEntityBinding("WillWhisper", "GuildTrainingWoodsWill/Entities/WillWhisper")
    quest:FinalizeEntityBindings()
    while not quest:IsLevelLoaded("GuildWoods") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "")
    quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Will_Init: lift it as function WatchForTermination(quest)
    if scratchValue & 2 ~= 0 then
        scratchValue = scratchValue & 0xfffffffd
    end
    quest:CreateThread("DoMission")  -- native thread body 0x00D68AE0: lift it as function DoMission(quest)
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
    willWhisper = quest:GetThingWithScriptName("WillWhisper")
    if not willWhisper:AcquireControl(4) then goto LAB_00d685d5 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d685d5 end
    willBandit = quest:GetAllThingsWithScriptName("WillBandit")
    -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef__at7e72a0(xStack_10);
    -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)&xStack_2c,iVar8 / 0xc,iVar5);
    resources:ReleaseResource(resource)
    scratchValue9 = 0
    if #willBandit ~= 0 then
        ctr_84 = 0
        repeat
            resources:TryAcquire(0 + scratchValue2, willBandit[ctr_84 + 1], 4)
            ctr_84 = ctr_84 + 1
            scratchValue9 = scratchValue9 + 1
            scratchValue2 = scratchValue2 + 16
        until scratchValue9 >= #willBandit
    end
    scratchValue3 = 0
    resource2 = resources:NewResource()
    while not resources:TryAcquire(resource2, hero, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
            goto FLOW_after_lab_00d67db1
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource2)
        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
    else
        -- TODO(native): resources:SetActor(xStack_38, "BAN1", &0x0)
        -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
        -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
        quest:StartCutscene({HERO = hero, WHISPER = willWhisper}, {}, true)
        quest:RunCutscene("CS_GUILD_WILL_WOODS_INTRO", true, false)
        quest:EndCutscene()
        resources:ReleaseResource(resource2)
        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
        ctr_88 = 0
        if #willBandit ~= 0 then
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                quest:GiveThingBestEnemyTarget(willBandit[scratchValue3 + 1], hero)
                scratchValue6 = willBandit[scratchValue3 + 1]
                quest:ModifyThingHealth(scratchValue6, 15.0 - quest:GetHealth(scratchValue6), false)
                -- TODO(native): (**(code **)(*(int *)xStack_7c[(iVar12) / 0xc + 1] + 0x118))(0);
                ctr_88 = ctr_88 + 1
                scratchValue3 = scratchValue3 + 1
            until ctr_88 >= #willBandit
        end
        if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d67db1 end
        while quest:GetStateBool("BanditsAlive") do
            if not quest:NewScriptFrame() then goto LAB_00d685cc end
            while not quest:IsLevelLoaded("GuildWoods") do
                if not quest:NewScriptFrame() then goto LAB_00d685cc end
            end
            while willBandit ~= scratchValue7 do
                -- TODO(native): puStack_78 = puStack_78 - 3;
            end
            willBandit = quest:GetAllThingsWithScriptName("WillBandit")
            if #willBandit == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                quest:SetStateBool("BanditsAlive", false)
            end
            if quest:GetHealth(hero) >= quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
            conversationId = quest:AddNewConversation(willWhisper, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", willWhisper, hero, false)
            if quest:IsXbox() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00d685cc end
                end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00d685cc end
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
            quest:SetStateBool("WhisperAnimate", true)
            quest:ChangeHeroHealthBy(1000.0, true, false)
            ::continue_1::
        end
        if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d67db1 end
        if not willWhisper:AcquireControl(4) then goto LAB_00d685cc end
        if not hero:AcquireControl(4) then goto FLOW_after_lab_00d6849b end
        if quest:IsActiveThreadTerminating() then
            hero:ReleaseControl()
        else
            quest:StartCutscene({HERO = hero, WHISPER = willWhisper}, {}, true)
            quest:RunCutscene("CS_GUILD_WILL_WOODS_OUTRO", true, false)
            quest:EndCutscene()
            hero:ReleaseControl()
            quest:SetStateBool("MissionSucceeded", true)
        end
        ::FLOW_after_lab_00d6849b::
    end
    ::FLOW_after_lab_00d67db1::
    ::LAB_00d685cc::
    ::LAB_00d685d5::
    willWhisper:ReleaseControl()
end

-- Q_GuildTrainingWoodsWill.WatchForTermination (retail 0x00d68600)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionFailed") then
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, true, false)
    else
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
    end
    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
end

-- Q_GuildTrainingWoodsWill.DoMission (retail 0x00d68ae0)
function DoMission(quest)
    quest:GiveHeroNewQuestObjective("first objective", 1)
    local isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    while true do
        if isLevelLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:CreateThread("WatchForLeaving")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
                EndMission(quest)
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    end
end

-- Q_GuildTrainingWoodsWill.WatchForLeaving (retail 0x00d68c50)
function WatchForLeaving(quest)
    local hero = quest:GetHero()
    while hero ~= nil and hero:IsAlive() do
        if quest:GetStateBool("MissionFailed") or quest:GetStateBool("MissionSucceeded") then break end
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionSucceeded") then
        quest:SetStateBool("MissionFailed", true)
    end
end

-- Q_GuildTrainingWoodsWill.EndMission (retail 0x00d68cd0)
function EndMission(quest)
    local missionOver = quest:GetStateBool("MissionOver")
    while true do
        if missionOver then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("MissionSucceeded", true)
            return
        end
        if not quest:NewScriptFrame() then break end
        missionOver = quest:GetStateBool("MissionOver")
    end
end

