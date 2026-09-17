-- Readable native conversion: Q_GuildTrainingWoodsWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingWoodsWill.Main (retail 0x00d67890)
function Main(quest)
    local resources = quest:RetailResources()
    local scratchValue, ctr_84, ctr_88, fret_0, scratchValue2, scratchValue3, conversationId
    local willWhisper, scratchValue8, scratchValue9, scratchValue10, scratchValue11, scratchValue12
    local scratchValue13, scratchValue14, scratchValue15, getAllThingsWithScriptName
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
    scratchValue15 = resources:NewResource()
    while not resources:TryAcquire(scratchValue15, willWhisper, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d685d5 end
    end
    if not quest:IsActiveThreadTerminating() then
        getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("WillBandit")
        -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef__at7e72a0(xStack_10);
        -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)&xStack_2c,iVar8 / 0xc,iVar5);
        resources:ReleaseResource(xStack_10)
        scratchValue8 = 0
        if #getAllThingsWithScriptName ~= 0 then
            ctr_84 = 0
            repeat
                resources:TryAcquire(0 + scratchValue2, ctr_84 + getAllThingsWithScriptName, 4)
                ctr_84 = ctr_84 + 12
                scratchValue8 = scratchValue8 + 1
                scratchValue2 = scratchValue2 + 16
            until scratchValue8 >= ((#getAllThingsWithScriptName * 12) / 12)
        end
        scratchValue3 = 0
        scratchValue13 = resources:NewResource()
        while not resources:TryAcquire(scratchValue13, quest:GetHero(), 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue13)
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
                goto FLOW_after_lab_00d67db1
            end
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(scratchValue13)
            -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
        else
            scratchValue11 = resources:NewActorMap()
            resources:SetActor(scratchValue11, "HERO", scratchValue13)
            -- TODO(native): resources:SetActor(xStack_38, "BAN1", &0x0)
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
            resources:SetActor(scratchValue11, "WHISPER", scratchValue15)
            scratchValue9 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_WILL_WOODS_INTRO", scratchValue11, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue9)
            resources:DestroyActorMap(scratchValue11)
            resources:ReleaseResource(scratchValue13)
            -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
            ctr_88 = 0
            if #getAllThingsWithScriptName ~= 0 then
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                    quest:GiveThingBestEnemyTarget(quest:GetHero(), willWhisper)
                    fret_0 = quest:GetHealth(nil --[[missing]])
                    quest:ModifyThingHealth(nil --[[missing]], scratchValue3 + getAllThingsWithScriptName, 15.0 - fret_0)
                    -- TODO(native): (**(code **)(*(int *)((int)xStack_7c + iVar12) + 0x118))(0);
                    ctr_88 = ctr_88 + 1
                    scratchValue3 = scratchValue3 + 12
                until ctr_88 >= ((#getAllThingsWithScriptName * 12) / 12)
            end
            if not quest:IsActiveThreadTerminating() then
                while quest:GetStateBool("BanditsAlive") do
                    if not quest:NewScriptFrame() then goto LAB_00d685cc end
                    while not quest:IsLevelLoaded("GuildWoods") do
                        if not quest:NewScriptFrame() then goto LAB_00d685cc end
                    end
                    while getAllThingsWithScriptName ~= puStack_78 do
                        -- TODO(native): puStack_78 = puStack_78 - 3;
                    end
                    getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("WillBandit")
                    if #getAllThingsWithScriptName == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                        quest:SetStateBool("BanditsAlive", false)
                    end
                    if quest:GetHealth(quest:GetHero()) < quest:ReadGlobalGameDataFloat(3800) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d685cc end
                        conversationId = quest:AddNewConversation(nil --[[missing]], false, false)
                        quest:AddPersonToConversation(conversationId, quest:GetHero())
                        quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", quest:GetHero(), nil --[[missing]], false)
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
                    while not resources:TryAcquire(scratchValue15, willWhisper, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d685cc end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue14 = resources:NewResource()
                        while not resources:TryAcquire(scratchValue14, quest:GetHero(), 4) do
                            if not quest:NewScriptFrame() then resources:ReleaseResource(scratchValue14); goto FLOW_after_lab_00d6849b end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue14)
                        else
                            scratchValue12 = resources:NewActorMap()
                            resources:SetActor(scratchValue12, "HERO", scratchValue14)
                            resources:SetActor(scratchValue12, "WHISPER", scratchValue15)
                            scratchValue10 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_WILL_WOODS_OUTRO", scratchValue12, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue10)
                            resources:DestroyActorMap(scratchValue12)
                            resources:ReleaseResource(scratchValue14)
                            quest:SetStateBool("MissionSucceeded", true)
                        end
                        ::FLOW_after_lab_00d6849b::
                    end
                end
            end
        end
        ::FLOW_after_lab_00d67db1::
        ::LAB_00d685cc::
    end
    ::LAB_00d685d5::
    resources:ReleaseResource(scratchValue15)
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
    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", "Q_GuildTrainingWoodsWill")
end

-- Q_GuildTrainingWoodsWill.DoMission (retail 0x00d68ae0)
function DoMission(quest)
    quest:GiveHeroNewQuestObjective("first objective", 1)
    local scratchValue = quest:IsLevelLoaded("GuildWoods")
    while true do
        if scratchValue then
            if quest:IsActiveThreadTerminating() then return end
            quest:CreateThread("WatchForLeaving")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
                EndMission(quest)
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        scratchValue = quest:IsLevelLoaded("GuildWoods")
    end
end

-- Q_GuildTrainingWoodsWill.WatchForLeaving (retail 0x00d68c50)
function WatchForLeaving(quest)
    local hero = quest:GetHero()
    while hero ~= nil and hero:IsAlive() do
        if quest:GetStateBool("MissionFailed") or quest:GetStateBool("MissionSucceeded") then break end
        if not quest:NewScriptFrame() then return end
        hero = quest:GetHero()
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

