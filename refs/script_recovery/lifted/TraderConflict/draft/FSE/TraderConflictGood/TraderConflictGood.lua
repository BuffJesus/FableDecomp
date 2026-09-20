-- Generated native draft: Q_TraderConflictGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar1, CVar11, bVar10, iVar9, pCVar4, pCVar5, pQuestName, uVar8
    local alive = true
    uVar8 = 0
    quest:AddEntityBinding("TraderToRescue", "TraderConflictGood/Entities/TraderToRescue")
    quest:AddEntityBinding("TC_BanditGuard", "TraderConflictGood/Entities/TC_BanditGuard")
    quest:AddEntityBinding("TC_BanditHostageKeeper", "TraderConflictGood/Entities/TC_BanditHostageKeeper")
    quest:AddEntityBinding("BCMTrader", "TraderConflictGood/Entities/BCMTrader")
    quest:AddEntityBinding("BCGameMaster", "TraderConflictGood/Entities/BCGameMaster")
    quest:AddEntityBinding("BanditExtra", "TraderConflictGood/Entities/BanditExtra")
    quest:AddEntityBinding("CampHostageDoor", "TraderConflictGood/Entities/CampHostageDoor")
    quest:FinalizeEntityBindings()
    bVar10 = quest:IsRegionLoaded("BanditCampEntrance")
    while not bVar10 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar10 = not alive
        if bVar10 then
            return
        end
        bVar10 = quest:IsRegionLoaded("BanditCampEntrance")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if not bVar10 then
        pCVar4 = quest:GetThingWithScriptName("Gate1")
        quest:OpenDoor(pCVar4)
        pCVar4 = quest:GetThingWithScriptName("Gate2Outer")
        quest:OpenDoor(pCVar4)
        helper_DFDED0(quest, "CS_TRADERCON_GOOD_INTRO")
        CVar11 = 0x0
        bVar10 = true
        quest:SetStateBool("IntroDone", true)
        pCVar5 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(pCVar5, bVar10, (CVar11 ~= 0))
        quest:FadeScreenIn()
        quest:CreateThread("WatchTimeLimit")  -- native thread body CQ_TraderConflictEvilScript::WatchTimeLimit: lift it as function WatchTimeLimit(quest)
        UpdateLiveEnemies(quest)
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_01", "BanditCampCentre", "BanditCampMain")
        if quest:GetStateListCount("AllCreatures") ~= 0 then
            iVar9 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then
                    return
                end
                quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", (iVar9) / 0xc), nil --[[operand lost by the decompiler]])
                quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", (iVar9) / 0xc), false)
                quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", (iVar9) / 0xc), "FACTION_BANDITS")
                uVar8 = uVar8 + 1
                iVar9 = iVar9 + 0xc
            until not (uVar8 < quest:GetStateListCount("AllCreatures"))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar10 = not alive
        if not bVar10 then
            quest:CreateThread("WatchForRegionTransitions")  -- native thread body WatchForRegionTransitions: lift it as function WatchForRegionTransitions(quest)
            if not bVar10 then
            end
            quest:CreateThread("WatchForHittingEnemies")  -- native thread body WatchForHittingEnemies: lift it as function WatchForHittingEnemies(quest)
            if not bVar10 then
            end
            quest:CreateThread("WatchForTradersFreed")  -- native thread body 0x00DFCC10: lift it as function WatchForTradersFreed(quest)
            if not bVar10 then
            end
            quest:CreateThread("WatchForKilledPeople")  -- native thread body WatchForKilledPeople: lift it as function WatchForKilledPeople(quest)
            if not bVar10 then
            end
            bVar10 = quest:IsRegionLoaded("BanditCampCentre")
            while not bVar10 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then
                    return
                end
                if (not quest:GetStateBool("FirstRegionDead")) and (quest:GetStateListCount("AllCreatures") == 0) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then
                        return
                    end
                    quest:SetStateBool("FirstRegionDead", true)
                end
                bVar10 = quest:IsRegionLoaded("BanditCampCentre")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if not bVar10 then
                pCVar4 = quest:GetThingWithScriptName("Gate2Inner")
                quest:OpenDoor(pCVar4)
                CVar1 = quest:GetStateBool("OutroDone")
                while not CVar1 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then
                        return
                    end
                    CVar1 = quest:GetStateBool("OutroDone")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if not bVar10 then
                    CVar11 = 0x0
                    bVar10 = true
                    pCVar5 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(pCVar5, bVar10, true, (CVar11 ~= 0))
                    bVar10 = quest:IsRegionLoaded("BanditCampEntrance")
                    if bVar10 then
                        repeat
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then
                                return
                            end
                            bVar10 = quest:IsRegionLoaded("BanditCampEntrance")
                        until not (bVar10)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if not bVar10 then
                        quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
                        uVar8 = 0
                        pCVar5 = quest:GetActiveQuestName()
                        quest:DeactivateQuestLater(pCVar5, uVar8)
                    end
                end
            end
        end
    end
end

function Init(quest)
    quest:SetStateInt("TradersFollowing", 0)
    quest:SetStateInt("TradersReachedTeleporter", 0)
    quest:SetStateInt("BanditSecurityLinesSaid", 0)
    quest:SetStateBool("IntroDone", false)
    quest:SetStateBool("OutroStart", false)
    quest:SetStateBool("OutroDone", false)
    quest:SetStateBool("OpenedCage", false)
    quest:SetStateBool("FirstRegionDead", false)
    quest:SetStateBool("EnteredNewRegion", false)
    quest:SetStateBool("CommentedOnBanditCostume", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:AddQuestRegion("Q_TraderConflictGood", "BanditCampEntrance")
    quest:AddQuestRegion("Q_TraderConflictGood", "BanditCampCentre")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x1e0), quest:ReadGlobalGameData(0x1e4), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x1e8), quest:ReadGlobalGameData(0x1ec), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(0x1f0), quest:ReadGlobalGameData(0x1f4), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE", 0x22, quest:ReadGlobalGameData(0x238), quest:ReadGlobalGameData(0x23c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT", 0x23, quest:ReadGlobalGameData(0x240), quest:ReadGlobalGameData(0x244), false, "", 0)
    quest:SetMasterGameState("TCGKillNoBandits", true)
    quest:SetMasterGameState("TCGMadeTimeLimit", false)
end

function WatchTimeLimit(quest)
    local CVar2, bVar3, cVar1, iVar4, xStack_8
    local alive = true
    xStack_8 = quest:RegisterTimer()
    quest:SetTimer(xStack_8, 5)
    cVar1 = quest:GetMasterGameState("TCGTimeLimitBoastTaken")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(xStack_8)
            return
        end
        iVar4 = quest:GetTimer(xStack_8)
        if iVar4 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            quest:DeregisterTimer(xStack_8)
            return
        end
        cVar1 = quest:GetMasterGameState("TCGTimeLimitBoastTaken")
    end
    quest:SetTimer(xStack_8, quest:ReadGlobalGameData(0xf84))
    iVar4 = quest:AddQuestInfoTimer(xStack_8, "HUD_CLOCK_ICON", 1.0)
    quest:DisplayQuestInfo(true)
    CVar2 = quest:GetStateBool("MissionSucceeded")
    while not CVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00dfb033 end
        CVar2 = quest:GetStateBool("MissionSucceeded")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:RemoveQuestInfoElement(iVar4)
        iVar4 = quest:GetTimer(xStack_8)
        if 0 < iVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(xStack_8)
                return
            end
            quest:SetMasterGameState("TCGMadeTimeLimit", true)
        end
    end
    ::LAB_00dfb033::
    quest:DeregisterTimer(xStack_8)
end

function WatchForRegionTransitions(quest)
    local CVar1, bVar3, iVar5, uVar4
    local alive = true
    CVar1 = quest:GetStateBool("OutroDone")
    while not CVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00dfca72 end
        bVar3 = quest:MsgOnRegionLoaded()
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00dfca72 end
            UpdateLiveEnemies(quest)
            uVar4 = 0
            if quest:GetStateListCount("AllCreatures") ~= 0 then
                iVar5 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00dfca72 end
                    quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", (iVar5) / 0xc), nil --[[operand lost by the decompiler]])
                    quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", (iVar5) / 0xc), false)
                    quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", (iVar5) / 0xc), "FACTION_BANDITS")
                    uVar4 = uVar4 + 1
                    iVar5 = iVar5 + 0xc
                until not (uVar4 < quest:GetStateListCount("AllCreatures"))
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00dfca72 end
            quest:SetStateBool("EnteredNewRegion", true)
        end
        CVar1 = quest:GetStateBool("OutroDone")
    end
    alive = not quest:IsActiveThreadTerminating()
    ::LAB_00dfca72::
end

function WatchForHittingEnemies(quest)
    local bVar3, bVar4, bVar6, cVar5, iVar9, i_stk_4, pCVar7, pThing1, uVar8, u_stk_8
    local alive = true
    iVar9 = quest:GetStateInt("TradersFollowing")
    bVar3 = false
    bVar6 = false
    repeat
        if 0 < iVar9 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                iVar9 = quest:GetStateInt("TradersReachedTeleporter")
                while iVar9 < 3 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    UpdateLiveEnemies(quest)
                    iVar9 = quest:GetStateInt("TradersReachedTeleporter")
                end
                alive = not quest:IsActiveThreadTerminating()
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        u_stk_8 = 0
        if quest:GetStateListCount("AllCreatures") ~= 0 then
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                cVar5 = quest:GetStateListAt("AllCreatures", (0) / 0xc):MsgIsHitBy("")
                if not cVar5 then
                    cVar5 = quest:GetStateListAt("AllCreatures", (0) / 0xc):MsgIsHitByAnySpecialAbilityFrom("")
                    if cVar5 then
                        bVar3 = true
                        bVar6 = true
                        cVar5 = quest:GetStateListAt("AllCreatures", (0) / 0xc):MsgIsHitByHeroSpecialAbility(0xe)
                        if not cVar5 then goto LAB_00dfc735 end
                    end
                    bVar3 = true
                    bVar4 = false
                else
                    goto LAB_00dfc735
                end
                goto FLOW_past_lab_00dfc735
                ::LAB_00dfc735::
                bVar4 = true
                ::FLOW_past_lab_00dfc735::
                if bVar6 then
                    bVar6 = false
                end
                if bVar3 then
                    bVar3 = false
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    uVar8 = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        iVar9 = 0
                        repeat
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                return
                            end
                            bVar4 = quest:IsDistanceBetweenThingsUnder((quest:GetStateListAt("AllCreatures", (0) / 0xc)), (quest:GetStateListAt("AllCreatures", (iVar9) / 0xc)), 20.0)
                            if bVar4 then
                                goto LAB_00dfc811
                            else
                                pCVar7 = quest:GetStateListAt("AllCreatures", (iVar9) / 0xc)
                                pThing1 = quest:GetHero()
                                bVar4 = quest:IsThingAwareOfOtherThingInAnyWay(pThing1, nil --[[missing]])
                                if bVar4 then goto LAB_00dfc811 end
                            end
                            goto FLOW_past_lab_00dfc811
                            ::LAB_00dfc811::
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                return
                            end
                            i_stk_4 = quest:GetStateListRef("AllCreatures")
                            pCVar7 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(pCVar7, nil --[[missing]])
                            ::FLOW_past_lab_00dfc811::
                            uVar8 = uVar8 + 1
                            iVar9 = iVar9 + 0xc
                        until not (uVar8 < quest:GetStateListCount("AllCreatures"))
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                end
                u_stk_8 = u_stk_8 + 1
                -- TODO(native): iStack_18 = iStack_18 + 0xc;
            until not (u_stk_8 < quest:GetStateListCount("AllCreatures"))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        UpdateLiveEnemies(quest)
        iVar9 = quest:GetStateInt("TradersFollowing")
    until false
end

function WatchForTradersFreed(quest)
    local bVar3, bVar4, cVar1, iStack_c4, iVar6, pCVar5, uVar2
    local alive = true
    iVar6 = quest:GetStateInt("TradersFollowing")
    while iVar6 < 1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        iVar6 = quest:GetStateInt("TradersFollowing")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
        quest:ActivateQuest("Q_TraderConflictGood_Extras")
        bVar3 = false
        iVar6 = quest:GetStateInt("TradersReachedTeleporter")
        while iVar6 < 3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            if quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                pCVar5 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
                bVar3 = false
                AttackPeople(quest)
                iVar6 = AreAllThingsInVectorDead((this + 0x48))
                if iVar6 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    cVar1 = quest:GetStateBool("EnteredNewRegion")
                    while not cVar1 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            return
                        end
                        cVar1 = quest:GetStateBool("EnteredNewRegion")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:SetStateBool("EnteredNewRegion", false)
                end
            elseif not bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                iStack_c4 = quest:GetAllThingsWithScriptName("TraderToRescue")
                bVar3 = quest:IsRegionLoaded("BanditCampEntrance")
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    iVar6 = #iStack_c4
                    if iVar6 == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                        end
                    elseif iVar6 == 2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                        end
                    else
                        if iVar6 ~= 3 then goto LAB_00dfd5c2 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                        end
                    end
                else
                    bVar3 = quest:IsRegionLoaded("BanditCampCentre")
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then return end
                        uVar2 = #iStack_c4
                        if uVar2 == 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then return end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                        elseif uVar2 < 2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then return end
                            if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end
                                pCVar5 = quest:GetActiveQuestName()
                                quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end
                                pCVar5 = quest:GetActiveQuestName()
                                quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then return end
                            pCVar5 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                        end
                        goto LAB_00dfd5bd
                    end
                end
                ::LAB_00dfd5bd::
                ::LAB_00dfd5c2::
                bVar3 = true
            end
            iVar6 = quest:GetStateInt("TradersReachedTeleporter")
        end
        alive = not quest:IsActiveThreadTerminating()
    end
end

function WatchForKilledPeople(quest)
    local bVar2, iVar1, this_00
    local alive = true
    iVar1 = quest:GetStateInt("TradersReachedTeleporter")
    while iVar1 < 3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00dfc302 end
        this_00 = quest:GetHero()
        -- TODO(native): MsgGetThingsKilled is not a ForgeFSE binding
        -- TODO(native): bVar2 = this_00:MsgGetThingsKilled(&0x0)
        bVar2 = nil --[[unresolved native value]]
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00dfc302 end
            quest:SetMasterGameState("TCGKillNoBandits", false)
        end
        iVar1 = quest:GetStateInt("TradersReachedTeleporter")
    end
    alive = not quest:IsActiveThreadTerminating()
    ::LAB_00dfc302::
    if nil ~= nil then
        -- TODO(native): free(xStack_c);
    end
end

function UpdateLiveEnemies(quest)
    local bVar1, cVar8, iStack_c, iVar3, lst_AllCreatures, p0, pTarget, piVar2, piVar4, uVar5
    local alive = true
    quest:StateListClear("AllCreatures")
    lst_AllCreatures = quest:GetAllCreaturesExcludingHero()
    quest:StateListSet("AllCreatures", lst_AllCreatures)
    piVar4 = 0
    if piVar4 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return bVar1
            end
            piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetDefName()
            iVar3 = ((piVar2 == "CREATURE_NEW_CHICKEN_04") and 0 or 1)
            cVar8 = not (iVar3 ~= 0)
            if not cVar8 then
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "TraderToRescue") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc413 end
                goto LAB_00dfc3b5
                ::LAB_00dfc413::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "BodyGuard") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc45c end
                goto LAB_00dfc3b5
                ::LAB_00dfc45c::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "RingFighter") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc4a5 end
                goto LAB_00dfc3b5
                ::LAB_00dfc4a5::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "FisticuffsMember") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc4ee end
                goto LAB_00dfc3b5
                ::LAB_00dfc4ee::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "Tyler") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc537 end
                goto LAB_00dfc3b5
                ::LAB_00dfc537::
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                piVar4 = piVar4 + 0xc
            else
                goto LAB_00dfc3b5
            end
            goto FLOW_past_lab_00dfc3b5
            ::LAB_00dfc3b5::
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return bVar1
            end
            quest:StateListErase("AllCreatures", (piVar4) / 0xc)
            ::FLOW_past_lab_00dfc3b5::
        until not (piVar4 ~= (quest:GetStateListCount("AllCreatures") * 0xc))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    iVar3 = bVar1
    if not bVar1 then
        uVar5 = 0
        pTarget = quest:GetHero()
        iStack_c = quest:GetFollowingEntityList(pTarget)
        if #iStack_c ~= 0 then
            iVar3 = 4
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then goto LAB_00dfc618 end
                p0 = 0
                if p0 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
                    repeat
                        -- TODO(native): cVar8 = (**(**(iVar3 + iStack_c) + 0x138))(quest:GetStateListAt("AllCreatures", (p0) / 0xc))
                        cVar8 = nil --[[unresolved native value]]
                        if cVar8 ~= 0 then
                            quest:StateListErase("AllCreatures", (p0) / 0xc)
                            break
                        end
                        p0 = p0 + 0xc
                    until not (p0 ~= (quest:GetStateListCount("AllCreatures") * 0xc))
                end
                uVar5 = uVar5 + 1
                iVar3 = iVar3 + 0xc
            until not (uVar5 < (#iStack_c))
        end
        alive = not quest:IsActiveThreadTerminating()
        ::LAB_00dfc618::
    end
    return iVar3
end

function AttackPeople(quest)
    local bVar2, cVar3, dist, elem_1, iVar4, iVar7, native_arg_sequence_1, native_arg_sequence_2, p0, pCVar1, pCVar5, pTarget, piStack_14, puVar6, r1, this_00, xStack_10, xStack_24, xStack_38, xStack_3c, x_stk_c
    local alive = true
    local function __cleanup_LAB_00dfdd34()
        quest:DeregisterTimer(xStack_3c)
    end
    UpdateLiveEnemies(quest)
    xStack_24 = quest:GetStateListCopy("AllCreatures")
    iVar4 = quest:RegisterTimer()
    xStack_3c = iVar4
    if not quest:GetStateBool("EnteredNewRegion") then
        goto LAB_00dfd643
    end
    goto FLOW_past_lab_00dfd643
    ::LAB_00dfd643::
    if (#xStack_24 ~= 0) and (quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing")) then
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(xStack_3c)
            goto LAB_00dfde13
        end
        if #xStack_24 ~= 0 then
            iVar7 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:DeregisterTimer(xStack_3c)
                    goto LAB_00dfde13
                end
                r1 = quest:GetNearestWithScriptName(xStack_24[(iVar7) / 0xc + 1], "TraderToRescue")
                bVar2 = quest:IsEntityFollowingHero(r1)
                if not bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then __cleanup_LAB_00dfdd34(); return end
                    x_stk_c = nil
                    if xStack_10 ~= nil then
                        -- TODO(native): *xStack_10 = *xStack_10 + -1;
                        -- TODO(native): if *xStack_10 == 0 then
                        if false then
                        end
                    end
                end
                dist = 15.0
                pCVar1 = xStack_24[(iVar7) / 0xc + 1]
                pCVar5 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar1, pCVar5, dist)
                native_arg_sequence_1 = false
                if bVar2 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    bVar2 = quest:IsDistanceBetweenThingsUnder(xStack_24[(iVar7) / 0xc + 1], r1, 15.0)
                    if bVar2 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    goto LAB_00dfd841
                end
                goto FLOW_past_lab_00dfd841
                ::LAB_00dfd841::
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                pCVar1 = 0x0
                if bVar2 then
                    -- LAB_00dfdc54: (native jump target)
                    quest:DeregisterTimer(xStack_3c)
                    return
                end
                bVar2 = quest:IsDistanceBetweenThingsUnder(xStack_24[0x0 + 1], r1, 15.0)
                native_arg_sequence_2 = false
                if bVar2 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if not native_arg_sequence_2 then
                    bVar2 = quest:IsThingAwareOfOtherThingInAnyWay(xStack_24[pCVar1 + 1], r1)
                    if bVar2 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                    if native_arg_sequence_2 then
                        if piStack_14 ~= nil then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                    if native_arg_sequence_2 then
                        -- TODO(native): cVar3 = (**(*piStack_14 + 0x12c))()
                        cVar3 = nil --[[unresolved native value]]
                        if cVar3 ~= 0 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                end
                if native_arg_sequence_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then __cleanup_LAB_00dfdd34(); return end
                    quest:GiveThingBestEnemyTarget(xStack_24[pCVar1 + 1], r1)
                    iVar4 = quest:GetTimer(xStack_3c)
                    if iVar4 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:DeregisterTimer(xStack_3c)
                            return
                        end
                        iVar7 = quest:AddNewConversation(xStack_24[pCVar1 + 1], false, false)
                        pCVar5 = quest:GetHero()
                        quest:AddPersonToConversation(iVar7, pCVar5)
                        xStack_38 = xStack_24[pCVar1 + 1]
                        pCVar5 = quest:GetHero()
                        quest:AddLineToConversation(iVar7, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", xStack_38, pCVar5, false)
                        goto LAB_00dfda32
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then __cleanup_LAB_00dfdd34(); return end
                    pCVar5 = xStack_24[pCVar1 + 1]
                    pTarget = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(pCVar5, pTarget)
                    iVar4 = quest:GetTimer(xStack_3c)
                    if iVar4 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            -- LAB_00dfdc8a: (native jump target)
                            quest:DeregisterTimer(xStack_3c)
                            return
                        end
                        iVar7 = quest:AddNewConversation(xStack_24[pCVar1 + 1], false, false)
                        pCVar5 = quest:GetHero()
                        quest:AddPersonToConversation(iVar7, pCVar5)
                        xStack_38 = xStack_24[pCVar1 + 1]
                        pCVar5 = quest:GetHero()
                        quest:AddLineToConversation(iVar7, "TEXT_QST_B11_BANDIT_ATTACK_HERO", xStack_38, pCVar5, false)
                        goto LAB_00dfda32
                    end
                end
                goto FLOW_past_lab_00dfda32
                ::LAB_00dfda32::
                quest:SetTimer(xStack_3c, 4)
                ::FLOW_past_lab_00dfda32::
                quest:MiniMapAddMarker(xStack_24[pCVar1 + 1], "HUD_ORB_RED_SMALL")
                puVar6 = 0
                if #xStack_24 ~= 0 then
                    bVar2 = xStack_24[pCVar1 + 1]:IsEqualTo(xStack_24[puVar6 + 1])
                    if bVar2 then
                        table.remove(xStack_24, puVar6 + 1)
                        break
                    end
                    goto FLOW_after_lab_00dfda90
                end
                goto LAB_00dfdabb
                ::FLOW_past_lab_00dfd841::
                pCVar1 = xStack_24[(iVar7) / 0xc + 1]
                pCVar5 = quest:GetHero()
                bVar2 = quest:IsThingAwareOfOtherThingInAnyWay(pCVar1, pCVar5)
                if bVar2 then goto LAB_00dfd841 end
                bVar2 = quest:IsThingAwareOfOtherThingInAnyWay(xStack_24[(iVar7) / 0xc + 1], r1)
                if bVar2 then
                    p0 = "TC_BanditGuard"
                    elem_1 = xStack_24[(iVar7) / 0xc + 1]
                    this_00 = elem_1:GetName()
                    iVar4 = ((this_00 ~= p0) and 1 or 0)
                    if iVar4 ~= 0 then goto LAB_00dfd841 end
                end
                -- TODO(native): xStack_38 = (CScriptThing *)&*(int *)(xStack_38 + 0x1);
                iVar7 = iVar7 + 0xc
            until not (xStack_38 < (#xStack_24))
        end
        goto LAB_00dfdaff
    end
    ::FLOW_past_lab_00dfd643::
    ::LAB_00dfdb8c::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:DeregisterTimer(xStack_3c)
    else
        if quest:GetStateBool("EnteredNewRegion") then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(xStack_3c)
                goto LAB_00dfde13
            end
            quest:SetStateBool("EnteredNewRegion", false)
        end
        quest:DeregisterTimer(xStack_3c)
    end
    ::LAB_00dfde13::
    do return end
    while true do
        puVar6 = puVar6 + 1
        if not (puVar6 ~= #xStack_24) then break end
        -- LAB_00dfda90: (native jump target)
        bVar2 = xStack_24[pCVar1 + 1]:IsEqualTo(xStack_24[puVar6 + 1])
        if bVar2 then
            table.remove(xStack_24, puVar6 + 1)
            break
        end
    end
    ::FLOW_after_lab_00dfda90::
    ::LAB_00dfdabb::
    r1 = nil
    ::LAB_00dfdaff::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:DeregisterTimer(xStack_3c)
    else
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                __cleanup_LAB_00dfdd34()
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:DeregisterTimer(xStack_3c)
                    return
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    if quest:GetStateBool("EnteredNewRegion") then goto LAB_00dfdb8c end
                    goto LAB_00dfd643
                end
            end
            quest:DeregisterTimer(xStack_3c)
            return
        end
        quest:DeregisterTimer(xStack_3c)
    end
    goto LAB_00dfde13
end

function helper_DFDED0(quest, native_arg_strParam_1)
    local resources = quest:RetailResources()
    local xStack_20 = resources:NewResource()
    local pScriptObject = xStack_20
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_2c = resources:NewActorMap()
    resources:SetActor(xStack_2c, "HERO", xStack_20)
    local xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(native_arg_strParam_1, xStack_2c, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_2c)
    resources:ReleaseResource(xStack_20)
end

