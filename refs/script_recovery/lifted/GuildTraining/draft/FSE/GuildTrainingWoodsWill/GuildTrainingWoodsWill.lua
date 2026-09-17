-- Generated native draft: Q_GuildTrainingWoodsWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local CVar10, bVar3, cVar1, fret_0, fret_00, iVar12, iVar5, iVar8, pCVar6, pTarget, pppuVar13, pvVar9, r1, r2, r3, uVar11, xStack_20, xStack_38, xStack_48, xStack_60
    local alive = true
    iVar12 = 0
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    quest:SetStateBool("WhisperAnimate", false)
    quest:SetStateBool("BanditsAlive", true)
    quest:AddEntityBinding("WillWhisper", "GuildTrainingWoodsWill/Entities/WillWhisper")
    quest:FinalizeEntityBindings()
    bVar3 = quest:IsLevelLoaded("GuildWoods")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsLevelLoaded("GuildWoods")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "")
        quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Will_Init: lift it as function WatchForTermination(quest)
        if (CVar10 & 2) ~= 0 then
            CVar10 = (CVar10 & 0xfffffffd)
        end
        quest:CreateThread("DoMission")  -- native thread body 0x00D68AE0: lift it as function DoMission(quest)
        if (CVar10 & 4) ~= 0 then
        end
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
        r1 = quest:GetThingWithScriptName("WillWhisper")
        xStack_60 = resources:NewResource()
        bVar3 = false
        if bVar3 ~= 0 then
        end
        bVar3 = resources:TryAcquire(xStack_60, r1, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d685d5 end
            bVar3 = resources:TryAcquire(xStack_60, r1, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            r2 = quest:GetAllThingsWithScriptName("WillBandit")
            iVar8 = 0x0 - 0x0
            -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef__at7e72a0(xStack_10);
            -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)&xStack_2c,iVar8 / 0xc,iVar5);
            resources:ReleaseResource(xStack_10)
            iVar5 = 0x0 - 0x0 >> 0x1f
            uVar11 = 0
            if (0x0 - 0x0) / 0xc + iVar5 ~= iVar5 then
                repeat
                    resources:TryAcquire((0x0 + iVar12), (0x0 + 0x0), 4)
                    -- TODO(native): xStack_84 = (CCharString)((int)xStack_84 + 0xc);
                    uVar11 = uVar11 + 1
                    iVar12 = iVar12 + 0x10
                until not (uVar11 < ((0x0 - 0x0) / 0xc))
            end
            iVar12 = 0
            xStack_48 = resources:NewResource()
            bVar3 = false
            if bVar3 ~= 0 then
            end
            iVar8 = 4
            pppuVar13 = xStack_48
            pCVar6 = quest:GetHero()
            bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar8)
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(xStack_48)
                    -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
                    goto FLOW_after_lab_00d67db1
                end
                iVar8 = 4
                pppuVar13 = xStack_48
                pCVar6 = quest:GetHero()
                bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar8)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d67db1: (native jump target)
                resources:ReleaseResource(xStack_48)
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
            else
                xStack_38 = resources:NewActorMap()
                resources:SetActor(xStack_38, "HERO", xStack_48)
                resources:SetActor(xStack_38, "BAN1", &0x0)
                pvVar9 = (0x0 + 0x10)
                -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
                pvVar9 = (0x0 + 0x20)
                -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](xStack_38,&xStack_88);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pvVar9);
                resources:SetActor(xStack_38, "WHISPER", xStack_60)
                xStack_20 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_WOODS_INTRO", xStack_38, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_20)
                resources:DestroyActorMap(xStack_38)
                resources:ReleaseResource(xStack_48)
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_2c);
                iVar5 = 0x0 - 0x0 >> 0x1f
                if (0x0 - 0x0) / 0xc + iVar5 ~= iVar5 then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d685cc end
                        pCVar6 = (iVar12 + 0x0)
                        pTarget = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(pTarget, r1)
                        pCVar6 = (iVar12 + 0x0)
                        bVar3 = false
                        fret_0 = quest:GetHealth(nil --[[missing]])
                        quest:ModifyThingHealth(nil --[[missing]], pCVar6, (15.0 - fret_0))
                        -- TODO(native): (**(code **)(*(int *)((int)xStack_7c + iVar12) + 0x118))(0);
                        -- TODO(native): xStack_88 = (CCharString)((int)xStack_88 + 1);
                        iVar12 = iVar12 + 0xc
                    until not (xStack_88 < ((0x0 - 0x0) / 0xc))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    bVar3 = false
                    if bVar3 ~= 0 then
                    end
                    cVar1 = quest:GetStateBool("BanditsAlive")
                    while cVar1 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d685cc end
                        bVar3 = quest:IsLevelLoaded("GuildWoods")
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            bVar3 = quest:IsLevelLoaded("GuildWoods")
                            while not bVar3 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d685cc end
                                bVar3 = quest:IsLevelLoaded("GuildWoods")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                        end
                        if nil ~= nil then
                            repeat
                                -- TODO(native): puStack_78 = puStack_78 + -3;
                                -- TODO(native): (**(code **)*puStack_78)(0);
                            until not (0x0 ~= puStack_78)
                        end
                        r3 = quest:GetAllThingsWithScriptName("WillBandit")
                        iVar12 = puStack_78 - 0x0 >> 0x1f
                        if (puStack_78 - 0x0) / 0xc + iVar12 == iVar12 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            quest:SetStateBool("BanditsAlive", false)
                        end
                        pCVar6 = quest:GetHero()
                        fret_00 = quest:GetHealth(pCVar6)
                        if fret_00 < quest:ReadGlobalGameData(0xed8) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            iVar5 = quest:AddNewConversation(nil --[[missing]], false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar5, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar5, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", pCVar6, nil --[[missing]], false)
                            bVar3 = quest:IsXbox()
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d685cc end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP")
                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                while not bVar3 do
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d685cc end
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d685cc end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC")
                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                while not bVar3 do
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d685cc end
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                end
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            quest:SetStateBool("WhisperAnimate", true)
                            quest:ChangeHeroHealthBy(1000.0, true, false)
                        end
                        cVar1 = quest:GetStateBool("BanditsAlive")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        bVar3 = false
                        if bVar3 ~= 0 then
                        end
                        bVar3 = resources:TryAcquire(xStack_60, r1, 4)
                        while not bVar3 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            bVar3 = resources:TryAcquire(xStack_60, r1, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            xStack_48 = resources:NewResource()
                            bVar3 = false
                            if bVar3 ~= 0 then
                            end
                            iVar5 = 4
                            pppuVar13 = xStack_48
                            pCVar6 = quest:GetHero()
                            bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar5)
                            while not bVar3 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    resources:ReleaseResource(xStack_48)
                                    goto FLOW_after_lab_00d6849b
                                end
                                iVar5 = 4
                                pppuVar13 = xStack_48
                                pCVar6 = quest:GetHero()
                                bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar5)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d6849b: (native jump target)
                                resources:ReleaseResource(xStack_48)
                            else
                                xStack_38 = resources:NewActorMap()
                                resources:SetActor(xStack_38, "HERO", xStack_48)
                                resources:SetActor(xStack_38, "WHISPER", xStack_60)
                                xStack_20 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_WILL_WOODS_OUTRO", xStack_38, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20)
                                resources:DestroyActorMap(xStack_38)
                                resources:ReleaseResource(xStack_48)
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
        resources:ReleaseResource(xStack_60)
    end
end

function WatchForTermination(quest)
    local bVar4, bVar6, pCVar5
    local alive = true
    local cVar1 = quest:GetStateBool("MissionFailed")
    while (not cVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        if not quest:GetStateBool("MissionFailed") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = true
            bVar4 = false
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, bVar6, false)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            -- TODO(native): CCharString__AssignFromWide(&xStack_8,0x122d70c);
            bVar6 = true
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pCVar5, bVar4, nil --[[missing]], pMessage)
        end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", "Q_GuildTrainingWoodsWill")
    end
end

function DoMission(quest)
    local alive = true
    quest:GiveHeroNewQuestObjective("first objective", 1)
    local bVar1 = quest:IsLevelLoaded("GuildWoods")
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:CreateThread("WatchForLeaving")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
                if (0 & 1) ~= 0 then
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if (not bVar1) and (not quest:GetStateBool("MissionFailed")) then
                    EndMission(quest)
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsLevelLoaded("GuildWoods")
    end
end

function WatchForLeaving(quest)
    local alive = true
    local pCVar2 = quest:GetHero()
    local bVar1 = (pCVar2 ~= nil and pCVar2:IsAlive())
    if bVar1 then
        repeat
            if (quest:GetStateBool("MissionFailed")) or (quest:GetStateBool("MissionSucceeded")) then break end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pCVar2 = quest:GetHero()
            bVar1 = (pCVar2 ~= nil and pCVar2:IsAlive())
        until not (bVar1)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if (not bVar1) and (not quest:GetStateBool("MissionSucceeded")) then
        quest:SetStateBool("MissionFailed", true)
    end
end

function EndMission(quest)
    local bVar2
    local alive = true
    local CVar1 = quest:GetStateBool("MissionOver")
    while true do
        if CVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:SetStateBool("MissionSucceeded", true)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        CVar1 = quest:GetStateBool("MissionOver")
    end
end

