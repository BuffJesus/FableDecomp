-- Generated native draft: Q_GuildTrainingWoodsWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar3, cVar1, ctr_84, ctr_88, fret_0, fret_00, iVar12, iVar5, iVar8, pCVar6, pTarget, pppuVar13, r1, uVar11, xStack_10, xStack_20, xStack_2c, xStack_38, xStack_48, xStack_60, xStack_7c
    local alive = true
    iVar12 = 0
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    quest:SetStateBool("WhisperAnimate", false)
    quest:SetStateBool("BanditsAlive", true)
    quest:AddEntityBinding("WillWhisper", "GuildTrainingWoodsWill/Entities/WillWhisper", 1)
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
        quest:CreateThread("DoMission")  -- native thread body 0x00D68AE0: lift it as function DoMission(quest)
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
        r1 = quest:GetThingWithScriptName("WillWhisper")
        xStack_60 = resources:NewResource()
        resources:PrepareResource(xStack_60)
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
            xStack_7c = quest:GetAllThingsWithScriptName("WillBandit")
            iVar8 = #xStack_7c * 0xc
            xStack_10 = resources:NewResource()
            xStack_2c = (function(n) local t = {} for i = 1, n do t[i] = resources:NewResource() end return t end)(iVar8 / 0xc)
            resources:ReleaseResource(xStack_10)
            uVar11 = 0
            if #xStack_7c ~= 0 then
                ctr_84 = 0
                repeat
                    resources:TryAcquire(xStack_2c[(iVar12) / 0x10 + 1], xStack_7c[(ctr_84) / 0xc + 1], 4)
                    ctr_84 = ctr_84 + 0xc
                    uVar11 = uVar11 + 1
                    iVar12 = iVar12 + 0x10
                until not (uVar11 < (#xStack_7c))
            end
            iVar12 = 0
            xStack_48 = resources:NewResource()
            resources:PrepareResource(xStack_48)
            iVar8 = 4
            pppuVar13 = xStack_48
            pCVar6 = quest:GetHero()
            bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar8)
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d67db1 end
                iVar8 = 4
                pppuVar13 = xStack_48
                pCVar6 = quest:GetHero()
                bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar8)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                goto LAB_00d67db1
            else
                xStack_38 = resources:NewActorMap()
                resources:SetActor(xStack_38, "HERO", xStack_48)
                resources:SetActor(xStack_38, "BAN1", xStack_2c[0 + 1])
                resources:SetActor(xStack_38, "BAN2", xStack_2c[1 + 1])
                resources:SetActor(xStack_38, "BAN3", xStack_2c[2 + 1])
                resources:SetActor(xStack_38, "WHISPER", xStack_60)
                xStack_20 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_WOODS_INTRO", xStack_38, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_20)
                resources:DestroyActorMap(xStack_38)
                resources:ReleaseResource(xStack_48)
                for _, r in ipairs(xStack_2c) do resources:ReleaseResource(r) end
                ctr_88 = 0
                if #xStack_7c ~= 0 then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d685cc end
                        pCVar6 = xStack_7c[(iVar12) / 0xc + 1]
                        pTarget = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(pCVar6, pTarget)
                        pCVar6 = xStack_7c[(iVar12) / 0xc + 1]
                        bVar3 = false
                        fret_0 = quest:GetHealth(pCVar6)
                        quest:ModifyThingHealth(pCVar6, (15.0 - fret_0), bVar3)
                        xStack_7c[(iVar12) / 0xc + 1]:SetToKillOnLevelUnload(false)
                        ctr_88 = ctr_88 + 1
                        iVar12 = iVar12 + 0xc
                    until not (ctr_88 < (#xStack_7c))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    resources:PrepareResource(xStack_60)
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
                        xStack_7c = quest:GetAllThingsWithScriptName("WillBandit")
                        if #xStack_7c == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            quest:SetStateBool("BanditsAlive", false)
                        end
                        pCVar6 = quest:GetHero()
                        fret_00 = quest:GetHealth(pCVar6)
                        if fret_00 < quest:ReadGlobalGameDataFloat(0xed8) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d685cc end
                            iVar5 = quest:AddNewConversation(r1, false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar5, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar5, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", r1, pCVar6, false)
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
                        resources:PrepareResource(xStack_60)
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
                            resources:PrepareResource(xStack_48)
                            iVar5 = 4
                            pppuVar13 = xStack_48
                            pCVar6 = quest:GetHero()
                            bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar5)
                            while not bVar3 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d6849b end
                                iVar5 = 4
                                pppuVar13 = xStack_48
                                pCVar6 = quest:GetHero()
                                bVar3 = resources:TryAcquire(pppuVar13, pCVar6, iVar5)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                goto LAB_00d6849b
                            else
                                xStack_38 = resources:NewActorMap()
                                resources:SetActor(xStack_38, "HERO", xStack_48)
                                resources:SetActor(xStack_38, "WHISPER", xStack_60)
                                xStack_20 = resources:StartMovie("")
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
                            goto FLOW_past_lab_00d6849b
                            ::LAB_00d6849b::
                            resources:ReleaseResource(xStack_48)
                            ::FLOW_past_lab_00d6849b::
                        end
                    end
                end
            end
            goto FLOW_past_lab_00d67db1
            ::LAB_00d67db1::
            resources:ReleaseResource(xStack_48)
            for _, r in ipairs(xStack_2c) do resources:ReleaseResource(r) end
            ::FLOW_past_lab_00d67db1::
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
            bVar6 = true
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pCVar5, bVar4, "", bVar6)
        end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
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

