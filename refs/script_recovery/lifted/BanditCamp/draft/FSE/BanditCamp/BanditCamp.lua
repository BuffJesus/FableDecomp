-- Generated native draft: Q_BanditCamp. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar1, bVar6, native_arg_sequence_1
    local alive = true
    quest:AddEntityBinding("Gate1GuardOuter", "BanditCamp/Entities/Gate1GuardOuter")
    quest:AddEntityBinding("Gate1GuardInner", "BanditCamp/Entities/Gate1GuardInner")
    quest:FinalizeEntityBindings()
    if not quest:GetStateBool("ActivatedBanditRaid") then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_01", "", "")
        quest:ActivateQuest("Q_OakValeBanditRaid")
        quest:SetStateBool("ActivatedBanditRaid", true)
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptBase_WaitForEntityAndExecuteOnePair: lift it as function WatchForTermination(quest)
    if not bVar6 then
    end
    quest:CreateThread("WatchForCostume")  -- native thread body WatchForCostume: lift it as function WatchForCostume(quest)
    if not bVar6 then
    end
    quest:CreateThread("CheckAnyBanditsKilled")  -- native thread body CheckAnyBanditsKilled: lift it as function CheckAnyBanditsKilled(quest)
    if not bVar6 then
    end
    quest:CreateThread("WatchForEndOfScript")  -- native thread body 0x00D013A0: lift it as function WatchForEndOfScript(quest)
    if not bVar6 then
    end
    if not quest:GetStateBool("Gate1Open") then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        CVar1 = quest:GetStateBool("Gate1Open")
        while not CVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            CVar1 = quest:GetStateBool("Gate1Open")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        quest:AutoSaveCheckPoint()
    end
    local pCVar5 = quest:GetHero()
    bVar6 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
    if bVar6 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        quest:TakeObjectFromHero("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
    end
    pCVar5 = quest:GetHero()
    bVar6 = quest:IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", pCVar5)
    native_arg_sequence_1 = false
    if bVar6 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if native_arg_sequence_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        quest:TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
    end
end

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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x88), quest:ReadGlobalGameData(0x8c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNOBANDITS", 0x18, quest:ReadGlobalGameData(0x3f4), quest:ReadGlobalGameData(0x3f8), true, "", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLMANYBANDITS", 0x1b, quest:ReadGlobalGameData(0x3fc), quest:ReadGlobalGameData(0x400), true, "", 1)
end

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

function WatchForTermination(quest)
    local bVar4, bVar6, pCVar5, pQuestName
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
        if not quest:GetStateBool("MissionSucceeded") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = true
            bVar4 = true
            pQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pQuestName, bVar4, "", bVar6)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = false
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, bVar6, false)
        end
        pCVar5 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar5, 0)
    end
end

function WatchForCostume(quest)
    local count, id, pCVar4
    local alive = true
    local bVar3 = quest:IsLevelLoaded("BanditCampPath_1")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsLevelLoaded("BanditCampPath_1")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        id = quest:AddQuestInfoCounter("HUD_BANDIT_CLOTHES_ICON", 5, 1.0)
        quest:DisplayQuestInfo(true)
        repeat
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            count = 0
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_HERO_HAT_BANDITCAMP", pCVar4)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                count = 1
            end
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_HERO_GLOVES_BANDITCAMP", pCVar4)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                count = count + 1
            end
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_HERO_BOOTS_BANDITCAMP", pCVar4)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                count = count + 1
            end
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_HERO_SHIRT_BANDITCAMP", pCVar4)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                count = count + 1
            end
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_HERO_TROUSERS_BANDITCAMP", pCVar4)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                count = count + 1
            end
            quest:UpdateQuestInfoCounter(id, count, -1)
        until not (count ~= 5)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:RemoveQuestInfoElement(id)
            quest:DisplayQuestInfo(false)
        end
    end
end

function CheckAnyBanditsKilled(quest)
    local bVar2, iStack_c, iVar3, iVar4, iVar5, max, native_arg_sequence_1, scale, this_00, uVar6, v_stk_10, xStack_14
    local alive = true
    v_stk_10 = 0
    if not quest:GetStateBool("BoastBegun") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsRegionLoaded("BanditCampPathEntrance")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:IsRegionLoaded("BanditCampPathEntrance")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
    end
    quest:SetStateBool("BoastBegun", true)
    iVar5 = -1
    if quest:GetMasterGameState("BanditCampKillManyBandits") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        scale = 1.0
        iVar3 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe88)))
        iVar5 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_BANDIT", iVar3, scale)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        while true do
            iStack_c = {}
            -- TODO(native): ::__EH_epilog3(auStack_4,(int)&uStack_15,0);
            this_00 = quest:GetHero()
            iStack_c = this_00:MsgGetThingsKilledGroups()
            bVar2 = #iStack_c ~= 0
            if bVar2 then break end
            -- LAB_00d034ad: (native jump target)
            if (quest:GetMasterGameState("BanditCampKillNoBandits")) and (v_stk_10 ~= 0) then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d03564 end
                quest:SetMasterGameState("BanditCampKillNoBandits", false)
            end
            native_arg_sequence_1 = false
            if not quest:GetMasterGameState("BanditCampKillManyBandits") then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                xStack_14 = v_stk_10
                if quest:ReadGlobalGameDataFloat(0xe88) <= xStack_14 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d03564 end
                quest:SetMasterGameState("BanditCampKillManyBandits", true)
                max = -1
                iVar4 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe88) - xStack_14))
                quest:UpdateQuestInfoCounter(iVar5, iVar4, max)
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        end
        ::FLOW_after_lab_00d034ad::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            uVar6 = 0
            if #iStack_c ~= 0 then
                repeat
                    if (iStack_c[(uVar6) + 1] & 4) ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d03564 end
                        v_stk_10 = v_stk_10 + 1
                    end
                    uVar6 = uVar6 + 1
                until not (uVar6 < (#iStack_c))
            end
            iStack_c = {}
            if (quest:GetMasterGameState("BanditCampKillNoBandits")) and (v_stk_10 ~= 0) then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d03564 end
                quest:SetMasterGameState("BanditCampKillNoBandits", false)
            end
            native_arg_sequence_1 = false
            if not quest:GetMasterGameState("BanditCampKillManyBandits") then
                native_arg_sequence_1 = true
            end
            if native_arg_sequence_1 then
                xStack_14 = v_stk_10
                if quest:ReadGlobalGameDataFloat(0xe88) <= xStack_14 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d03564 end
                quest:SetMasterGameState("BanditCampKillManyBandits", true)
                max = -1
                iVar4 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe88) - xStack_14))
                quest:UpdateQuestInfoCounter(iVar5, iVar4, max)
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            goto FLOW_after_lab_00d034ad
        end
        ::LAB_00d03564::
    end
end

function WatchForEndOfScript(quest)
    local alive = true
    local bVar1 = quest:IsRegionLoaded("BanditCampBoss")
    if not bVar1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:ActivateQuest("Q_BanditCampBossBattle")
    end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        bVar1 = quest:IsQuestActive("Q_BanditCampBossBattle")
        if bVar1 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                bVar1 = quest:IsQuestActive("Q_BanditCampBossBattle")
            until not (bVar1)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetStateBool("MissionSucceeded", true)
        end
    end
end

