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
    local ctr_84, ctr_88, scratchValue, scratchValue2, scratchValue6, scratchValue8, resource
    local resource2, willBandit
    scratchValue = 0
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    quest:SetStateBool("WhisperAnimate", false)
    quest:SetStateBool("BanditsAlive", true)
    quest:AddEntityBinding("WillWhisper", "GuildTrainingWoodsWill/Entities/WillWhisper", 1)
    quest:FinalizeEntityBindings()
    while not quest:IsLevelLoaded("GuildWoods") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "")
    quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Will_Init: lift it as function WatchForTermination(quest)
    quest:CreateThread("DoMission")  -- native thread body 0x00D68AE0: lift it as function DoMission(quest)
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
    local willWhisper = quest:GetThingWithScriptName("WillWhisper")
    local resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, willWhisper, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d685d5 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d685d5 end
    willBandit = quest:GetAllThingsWithScriptName("WillBandit")
    -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef__at7e72a0(xStack_10);
    -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)&xStack_2c,iVar8 / 0xc,iVar5);
    resources:ReleaseResource(resource)
    scratchValue8 = 0
    if #willBandit ~= 0 then
        ctr_84 = 0
        repeat
            resources:TryAcquire(0 + scratchValue, willBandit[ctr_84 + 1], 4)
            ctr_84 = ctr_84 + 1
            scratchValue8 = scratchValue8 + 1
            scratchValue = scratchValue + 16
        until scratchValue8 >= #willBandit
    end
    scratchValue2 = 0
    resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d67db1 end
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00d67db1
    else
        local actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource2)
        -- TODO(native): resources:SetActor(xStack_38, "BAN1", &0x0)
        -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
        -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
        resources:SetActor(actorMap, "WHISPER", resource4)
        local movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_WILL_WOODS_INTRO", actorMap, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource2)
        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
        ctr_88 = 0
        if #willBandit ~= 0 then
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                quest:GiveThingBestEnemyTarget(willBandit[scratchValue2 + 1], hero)
                local scratchValue5 = willBandit[scratchValue2 + 1]
                quest:ModifyThingHealth(scratchValue5, 15.0 - quest:GetHealth(scratchValue5), false)
                willBandit[scratchValue2 + 1]:SetToKillOnLevelUnload(0)
                ctr_88 = ctr_88 + 1
                scratchValue2 = scratchValue2 + 1
            until ctr_88 >= #willBandit
        end
        if not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(resource4)
            while quest:GetStateBool("BanditsAlive") do
                if not quest:NewScriptFrame() then goto LAB_00d685cc end
                while not quest:IsLevelLoaded("GuildWoods") do
                    if not quest:NewScriptFrame() then goto LAB_00d685cc end
                end
                while willBandit ~= scratchValue6 do
                    -- TODO(native): puStack_78 = puStack_78 - 3;
                end
                willBandit = quest:GetAllThingsWithScriptName("WillBandit")
                if #willBandit == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                    quest:SetStateBool("BanditsAlive", false)
                end
                if quest:GetHealth(hero) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                    local conversationId = quest:AddNewConversation(willWhisper, false, false)
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
                end
            end
            if not quest:IsActiveThreadTerminating() then
                resources:PrepareResource(resource4)
                while not resources:TryAcquire(resource4, willWhisper, 4) do
                    if not quest:NewScriptFrame() then goto LAB_00d685cc end
                end
                if not quest:IsActiveThreadTerminating() then
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, hero, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d6849b end
                    end
                    if quest:IsActiveThreadTerminating() then
                        goto LAB_00d6849b
                    else
                        local actorMap2 = resources:NewActorMap()
                        resources:SetActor(actorMap2, "HERO", resource3)
                        resources:SetActor(actorMap2, "WHISPER", resource4)
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_WOODS_OUTRO", actorMap2, false, true)
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:DestroyActorMap(actorMap2)
                        resources:ReleaseResource(resource3)
                        quest:SetStateBool("MissionSucceeded", true)
                    end
                    goto FLOW_past_lab_00d6849b
                    ::LAB_00d6849b::
                    resources:ReleaseResource(resource3)
                    ::FLOW_past_lab_00d6849b::
                end
            end
        end
    end
    goto FLOW_past_lab_00d67db1
    ::LAB_00d67db1::
    resources:ReleaseResource(resource2)
    -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
    ::FLOW_past_lab_00d67db1::
    ::LAB_00d685cc::
    ::LAB_00d685d5::
    resources:ReleaseResource(resource4)
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

