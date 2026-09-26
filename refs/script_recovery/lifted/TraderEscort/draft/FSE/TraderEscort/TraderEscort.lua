-- Generated native draft: Q_TraderEscort. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar8, pCVar6, pQuestName, textID
    local alive = true
    quest:AddEntityBinding("DarkwoodTrader", "TraderEscort/Entities/DarkwoodTrader", 1)
    quest:AddEntityBinding("FleshEatingBalverine", "TraderEscort/Entities/FleshEatingBalverine", 1)
    quest:AddEntityBinding("InfectedBalverine", "TraderEscort/Entities/InfectedBalverine", 1)
    quest:AddEntityBinding("TraderComment", "TraderEscort/Entities/TraderComment", 1)
    quest:AddEntityBinding("DarkwoodAssassinSpawn", "TraderEscort/Entities/DarkwoodAssassinSpawn", 1)
    quest:AddEntityBinding("DarkwoodBalverineTrader", "TraderEscort/Entities/DarkwoodBalverineTrader", 1)
    quest:AddEntityBinding("RockTrollTrigger", "TraderEscort/Entities/RockTrollTrigger", 1)
    quest:AddEntityBinding("DarkwoodRockTroll", "TraderEscort/Entities/DarkwoodRockTroll", 1)
    quest:AddEntityBinding("MagicBarrier", "TraderEscort/Entities/MagicBarrier", 1)
    quest:AddEntityBinding("TE_BanditLeader", "TraderEscort/Entities/TE_BanditLeader", 1)
    quest:AddEntityBinding("BanditCronies", "TraderEscort/Entities/BanditCronies", 1)
    quest:AddEntityBinding("DW5_Balv", "TraderEscort/Entities/DW5_Balv", 1)
    quest:AddEntityBinding("DW5_Bandit", "TraderEscort/Entities/DW5_Bandit", 1)
    quest:AddEntityBinding("EndTrader", "TraderEscort/Entities/EndTrader")
    quest:FinalizeEntityBindings()
    pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_01", "Darkwood1", "Greatwood")
    quest:CreateThread("WatchForSurprisingBalverines")  -- native thread body CQ_TraderEscortScript::WatchForSurprisingBalverines: lift it as function WatchForSurprisingBalverines(quest)
    if not bVar8 then
    end
    quest:CreateThread("WatchForMissionRules")  -- native thread body 0x00E06440: lift it as function WatchForMissionRules(quest)
    if not quest:GetStateBool("QuestStartScreened") then
        while true do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if bVar8 then break end
            bVar8 = quest:HeroHasExpression("EXPRESSION_PICKPOCKET")
            if bVar8 then
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                if bVar8 then
                    return
                end
                textID = 0
                pCVar6 = quest:GetActiveQuestName()
                quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS", 10, quest:ReadGlobalGameData(0xe04), quest:ReadGlobalGameData(0xe08), true, pCVar6, textID)
                goto LAB_00e015c9
            end
            if quest:GetStateBool("QuestStartScreened") then
                alive = not quest:IsActiveThreadTerminating()
                return
            end
        end
    else
        goto LAB_00e015c9
    end
    goto FLOW_past_lab_00e015c9
    ::LAB_00e015c9::
    alive = not quest:IsActiveThreadTerminating()
    ::FLOW_past_lab_00e015c9::
end

function Init(quest)
    quest:SetStateInt("CommentTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:AddQuestRegion("Q_TraderEscort", "Darkwood1")
    quest:AddQuestRegion("Q_TraderEscort", "Darkwood2")
    quest:AddQuestRegion("Q_TraderEscort", "DemonDoor_DarkwoodSporeSwamp")
    quest:AddQuestRegion("Q_TraderEscort", "Darkwood3")
    quest:AddQuestRegion("Q_TraderEscort", "Darkwood4")
    quest:AddQuestRegion("Q_TraderEscort", "DarkwoodChapel")
    quest:AddQuestRegion("Q_TraderEscort", "Darkwood5")
    quest:AddQuestRegion("Q_TraderEscort", "Darkwood6")
    quest:AddQuestRegion("Q_TraderEscort", "BarrowFields")
    quest:AddQuestRegion("Q_TraderEscort", "DemonDoor_Barrowfields")
    quest:SetCreatureGeneratorsEnabled("Darkwood1", false)
    quest:SetCreatureGeneratorsEnabled("Darkwood2", false)
    quest:SetCreatureGeneratorsEnabled("Darkwood3", false)
    quest:SetCreatureGeneratorsEnabled("Darkwood5", false)
    quest:SetCreatureGeneratorsEnabled("Darkwood6", false)
    quest:SetStateInt("TraderPickpocketedCount", 0)
    quest:SetStateInt("BalverinesToSurpriseHeroNeeded", 0)
    quest:SetStateInt("TradersStillAliveCounter", 0)
    quest:SetStateBool("InfectedTraderCanGetUp", false)
    quest:SetStateBool("TradersShouldBeScared", false)
    quest:SetStateBool("IntroFinished", false)
    quest:SetStateBool("FollowInfoGiven", false)
    quest:SetStateBool("EndStarted", false)
    quest:SetStateBool("SavedInMiddle", false)
    quest:SetStateBool("SavedNearEnd", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("QuestStartScreened", false)
    quest:SetStateBool("GoneWrongWay", false)
    quest:SetStateBool("ShownBalverine", false)
    quest:SetStateBool("ShownEarthTroll", false)
    quest:SetStateInt("TimesBantered", 0)
    quest:SetStateInt("IncubationTimeLow", quest:ReadGlobalGameData(0xdf8))
    quest:SetStateInt("IncubationTimeMedium", quest:ReadGlobalGameData(0xdfc))
    quest:SetStateInt("IncubationTimeHigh", quest:ReadGlobalGameData(0xe00))
    quest:SetStateString("TraderToTalk", "")
    quest:SetMasterGameState("DarkwoodAllTradersAlive", true)
    quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x1f8), quest:ReadGlobalGameData(0x1fc), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x200), quest:ReadGlobalGameData(0x204), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_ALLTRADERSALIVE", 9, quest:ReadGlobalGameData(0xe04), quest:ReadGlobalGameData(0xe08), false, "", 0)
end

function OnPersist(quest, context)
    local introFinished = quest:GetStateBool("IntroFinished") or false
    introFinished = quest:PersistTransferBool(context, "IntroFinished", introFinished)
    quest:SetStateBool("IntroFinished", introFinished)
    local savedInMiddle = quest:GetStateBool("SavedInMiddle") or false
    savedInMiddle = quest:PersistTransferBool(context, "SavedInMiddle", savedInMiddle)
    quest:SetStateBool("SavedInMiddle", savedInMiddle)
    local savedNearEnd = quest:GetStateBool("SavedNearEnd") or false
    savedNearEnd = quest:PersistTransferBool(context, "SavedNearEnd", savedNearEnd)
    quest:SetStateBool("SavedNearEnd", savedNearEnd)
    local endStarted = quest:GetStateBool("EndStarted") or false
    endStarted = quest:PersistTransferBool(context, "EndStarted", endStarted)
    quest:SetStateBool("EndStarted", endStarted)
    local questStartScreened = quest:GetStateBool("QuestStartScreened") or false
    questStartScreened = quest:PersistTransferBool(context, "QuestStartScreened", questStartScreened)
    quest:SetStateBool("QuestStartScreened", questStartScreened)
    local shownBalverine = quest:GetStateBool("ShownBalverine") or false
    shownBalverine = quest:PersistTransferBool(context, "ShownBalverine", shownBalverine)
    quest:SetStateBool("ShownBalverine", shownBalverine)
    local shownEarthTroll = quest:GetStateBool("ShownEarthTroll") or false
    shownEarthTroll = quest:PersistTransferBool(context, "ShownEarthTroll", shownEarthTroll)
    quest:SetStateBool("ShownEarthTroll", shownEarthTroll)
end

function WatchForSurprisingBalverines(quest)
    local bVar5, cVar6, iVar9, pCVar7, pCVar8, r1, uVar10, xStack_30
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        -- LAB_00e06394: (native jump target)
    else
        goto LAB_00e06074
    end
    ::FLOW_after_lab_00e06394::
    goto FLOW_past_lab_00e06074
    ::LAB_00e06074::
    if quest:GetStateInt("BalverinesToSurpriseHeroNeeded") < 1 then goto LAB_00e06373 end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        quest:Pause(2.0)
        -- TODO(native): Std_Vector_Erase_Range(&xStack_30,(int)xStack_30,(int)x_stk_2c);
        xStack_30 = quest:GetAllThingsWithScriptName("M_BalverineSurprise")
        uVar10 = 0
        if #xStack_30 ~= 0 then
            iVar9 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e063db end
                if 0 < quest:GetStateInt("BalverinesToSurpriseHeroNeeded") then
                    pCVar7 = xStack_30[(iVar9) / 0xc + 1]:GetPos()
                    bVar5 = quest:IsCameraPosOnScreen(pCVar7)
                    if not bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        bVar5 = true
                        pCVar7 = xStack_30[uVar10 + 1]:GetPos()
                        r1 = quest:CreateCreature("CREATURE_BALVERINE_EASY", pCVar7, "SurpriseBalverine")
                        quest:EntitySetCutsceneBehaviour(r1, 1)
                        bVar5 = false
                        pCVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r1, pCVar8, bVar5)
                        quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                        iVar9 = 0
                        pCVar8 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
                        MakeTraderComment(quest, "BALVERINE_SURPRISE", pCVar8, iVar9)
                        iVar9 = math.random(0, 32767)
                        if iVar9 % 5 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00e063f5(); return end
                            pCVar8 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
                            quest:GiveThingBestEnemyTarget(r1, pCVar8)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00e063f5(); return end
                            pCVar8 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(r1, pCVar8)
                        end
                        cVar6 = (r1 ~= nil and r1:IsAlive())
                        goto LAB_00e06319
                    end
                end
                uVar10 = uVar10 + 1
                iVar9 = iVar9 + 0xc
            until not (uVar10 < (#xStack_30))
        end
        goto LAB_00e06364
    end
    ::FLOW_past_lab_00e06074::
    ::LAB_00e0642a::
    do return end
    ::LAB_00e063db::
    goto LAB_00e0642a
    ::LAB_00e06319::
    if not cVar6 then goto LAB_00e06349 end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then __cleanup_LAB_00e063f5(); return end
    cVar6 = (r1 ~= nil and r1:IsAlive())
    goto LAB_00e06319
    ::LAB_00e06349::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        __cleanup_LAB_00e063f5()
        return
    end
    quest:SetStateInt("BalverinesToSurpriseHeroNeeded", quest:GetStateInt("BalverinesToSurpriseHeroNeeded") + -1)
    ::LAB_00e06364::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then goto LAB_00e06415 end
    ::LAB_00e06373::
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        goto FLOW_after_lab_00e06394
    end
    goto LAB_00e06074
    ::LAB_00e06415::
    goto LAB_00e0642a
end

function WatchForMissionRules(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, __native_condition_2, __native_condition_3, b3, bVar14, bVar2, cVar3, c_stk_c1, dist, ePriority, iVar8, native_arg_sequence_1, pCVar4, pCVar6, pCVar7, pppuVar12, r1, r2, r3, uVar15, xStack_1c, xStack_2c, xStack_3c, xStack_50, xStack_60, xStack_b0, xStack_c0, xStack_dc, x_stk_d0
    local alive = true
    if not quest:GetStateBool("IntroFinished") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        quest:SetStateBool("SavedInMiddle", false)
        bVar2 = quest:IsRegionLoaded("Darkwood1")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
            bVar2 = quest:IsRegionLoaded("Darkwood1")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        pCVar4 = quest:GetThingWithScriptName("MK_DTE_INTRO_TRADER1")
        r1 = quest:GetNearestWithScriptName(pCVar4, "DarkwoodTrader")
        r2 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
        xStack_60 = resources:NewResource()
        xStack_1c = resources:NewResource()
        ePriority = 4
        xStack_2c = resources:NewResource()
        pppuVar12 = xStack_2c
        pCVar4 = quest:GetHero()
        resources:TryAcquire(pppuVar12, pCVar4, ePriority)
        resources:TryAcquire(xStack_60, r1, 4)
        resources:TryAcquire(xStack_1c, r2, 4)
        xStack_b0 = resources:NewActorMap()
        resources:SetActor(xStack_b0, "HERO", xStack_2c)
        resources:SetActor(xStack_b0, "TRADER1", xStack_60)
        resources:SetActor(xStack_b0, "TRADER2", xStack_1c)
        xStack_50 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_DARKWOOD_TRADER_INTRO", xStack_b0, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_50)
        resources:DestroyActorMap(xStack_b0)
        resources:ReleaseResource(xStack_2c)
        resources:ReleaseResource(xStack_1c)
        resources:ReleaseResource(xStack_60)
    end
    pCVar6 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar6, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_02", "Darkwood4", "Greatwood")
    if not quest:GetStateBool("QuestStartScreened") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        bVar14 = false
        bVar2 = true
        quest:SetStateBool("IntroFinished", true)
        pCVar7 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(pCVar7, bVar2, bVar14)
        quest:SetStateBool("QuestStartScreened", true)
    end
    if not quest:GetStateBool("FollowInfoGiven") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        bVar2 = quest:IsXbox()
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT")
            bVar2 = quest:MsgIsGameInfoClickedPast()
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e07620 end
                bVar2 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT_PC")
            bVar2 = quest:MsgIsGameInfoClickedPast()
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e07620 end
                bVar2 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
        end
        quest:SetStateBool("FollowInfoGiven", true)
    end
    iVar8 = quest:GetStateInt("TradersStillAliveCounter")
    while iVar8 < 1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        iVar8 = quest:GetStateInt("TradersStillAliveCounter")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e07620 end
    x_stk_d0 = nil
    xStack_dc = nil
    if quest:GetStateBool("SavedInMiddle") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e0760e end
        pCVar6 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar6, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while not bVar2 do
        if not quest:GetStateBool("GoneWrongWay") then
            bVar2 = quest:IsRegionLoaded("GreatwoodCaves")
            if not bVar2 then goto LAB_00e06b57 end
            bVar2 = true
        else
            goto LAB_00e06b57
        end
        goto FLOW_past_lab_00e06b57
        ::LAB_00e06b57::
        bVar2 = false
        ::FLOW_past_lab_00e06b57::
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_GONE_WRONG_WAY")
            bVar2 = quest:MsgIsGameInfoClickedPast()
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e0760e end
                bVar2 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("GoneWrongWay", true)
        end
        if not quest:GetStateBool("ShownBalverine") then
            bVar2 = quest:IsRegionLoaded("Darkwood2")
            if not bVar2 then goto LAB_00e06c33 end
            bVar2 = true
        else
            goto LAB_00e06c33
        end
        goto FLOW_past_lab_00e06c33
        ::LAB_00e06c33::
        bVar2 = false
        ::FLOW_past_lab_00e06c33::
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            xStack_3c = resources:NewActorMap()
            xStack_60 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_DARKWOOD_TRADER_BALVERINE", xStack_3c, false, true)
            quest:SetStateBool("ShownBalverine", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_60)
            resources:DestroyActorMap(xStack_3c)
        end
        if not quest:GetStateBool("EndStarted") then
            bVar2 = quest:IsRegionLoaded("BarrowFields")
            native_arg_sequence_1 = false
            if not bVar2 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                cVar3 = (xStack_dc ~= nil and xStack_dc:IsAlive())
                if cVar3 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00e06d6c end
            bVar2 = true
        else
            goto LAB_00e06d6c
        end
        goto FLOW_past_lab_00e06d6c
        ::LAB_00e06d6c::
        bVar2 = false
        ::FLOW_past_lab_00e06d6c::
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                pCVar4 = quest:GetThingWithScriptName("EndTrader")
                xStack_dc = pCVar4
                pCVar4 = quest:GetThingWithScriptName("M_EndTheQuestHere")
                x_stk_d0 = pCVar4
                cVar3 = (xStack_dc ~= nil and xStack_dc:IsAlive())
                if cVar3 then goto LAB_00e06fb5 end
                if not quest:GetStateBool("EndStarted") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        ::LAB_00e06e73::
                        if not quest:GetStateBool("EndStarted") then
                            bVar2 = quest:IsRegionLoaded("BarrowFields")
                            if not bVar2 then goto LAB_00e06eb0 end
                            bVar2 = true
                        else
                            goto LAB_00e06eb0
                        end
                        goto FLOW_past_lab_00e06eb0
                        ::LAB_00e06eb0::
                        bVar2 = false
                        ::FLOW_past_lab_00e06eb0::
                        if bVar2 then
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then break end
                            dist = 4.0
                            pCVar4 = quest:GetHero()
                            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar4, x_stk_d0, dist)
                            if bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then break end
                                xStack_b0 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
                                if #xStack_b0 == quest:GetStateInt("TradersStillAliveCounter") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        break
                                    end
                                    quest:SetStateBool("EndStarted", true)
                                end
                            end
                            goto LAB_00e06e73
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then goto LAB_00e06fb5 end
                    end
                else
                    goto LAB_00e073cf
                end
            end
            goto FLOW_hoist_lab_00e073cf_2
        end
        goto FLOW_past_lab_00e073cf
        ::LAB_00e073cf::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            xStack_c0 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
            quest:GiveHeroGold((#xStack_c0) * quest:ReadGlobalGameData(0xe24))
            b3 = false
            bVar14 = false
            bVar2 = true
            pCVar7 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar7, bVar2, bVar14, b3)
            quest:SetCreatureGeneratorsEnabled("Darkwood1", true)
            quest:SetCreatureGeneratorsEnabled(pCVar7, true)
            quest:SetCreatureGeneratorsEnabled("Darkwood2", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood3", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood5", true)
            bVar2 = quest:IsRegionLoaded("BarrowFields")
            goto LAB_00e07574
        end
        ::FLOW_hoist_lab_00e073cf_2::
        break
        ::FLOW_past_lab_00e073cf::
        ::LAB_00e06fb5::
        if quest:GetStateBool("EndStarted") then goto LAB_00e073cf end
        if quest:GetStateInt("TradersStillAliveCounter") == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetCreatureGeneratorsEnabled("Darkwood1", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood2", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood3", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood5", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood6", true)
            bVar14 = true
            bVar2 = true
            pCVar7 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pCVar7, bVar2, "TEXT_QST_067_QUEST_FAILED_TRADERS_DIED", bVar14)
            quest:SetStateBool("MissionFailed", true)
            r3 = quest:GetThingWithScriptName(xStack_c0)
            bVar2 = quest:IsRegionLoaded("Darkwood1")
            __native_condition_1 = bVar2
            if __native_condition_1 then
                iVar8 = xStack_c0:IsAlive()
                __native_condition_1 = iVar8
            end
            if __native_condition_1 then
                bVar2 = true
            else
                bVar2 = false
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    cVar3 = xStack_c0[0 + 1]:IsAlive()
                    while cVar3 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e07609 end
                        cVar3 = xStack_c0[0 + 1]:IsAlive()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then goto LAB_00e07208 end
                end
                ::LAB_00e07609::
                break
            end
            ::LAB_00e07208::
            uVar15 = 0
            pCVar7 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar7, uVar15)
        end
        __native_condition_2 = not quest:GetStateBool("SavedInMiddle")
        if __native_condition_2 then
            bVar2 = quest:MsgOnRegionLoaded()
            __native_condition_2 = bVar2
        end
        if __native_condition_2 then
            if "" == nil then
                bVar2 = false
                if bVar2 then
                    goto LAB_00e07299
                end
            else
                iVar8 = (("" == "Darkwood4") and 0 or 1)
                c_stk_c1 = not (iVar8 ~= 0)
                if c_stk_c1 then goto LAB_00e07299 end
            end
            goto FLOW_past_lab_00e07299
            ::LAB_00e07299::
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("SavedInMiddle", true)
            pCVar6 = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pCVar6, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
            quest:AutoSaveCheckPoint()
            ::FLOW_past_lab_00e07299::
        end
        __native_condition_3 = not quest:GetStateBool("SavedNearEnd")
        if __native_condition_3 then
            bVar2 = quest:MsgOnRegionLoaded()
            __native_condition_3 = bVar2
        end
        if __native_condition_3 then
            if "" == nil then
                bVar2 = false
                if bVar2 then
                    goto LAB_00e07385
                end
            else
                iVar8 = (("" == "Darkwood6") and 0 or 1)
                c_stk_c1 = not (iVar8 ~= 0)
                if c_stk_c1 then goto LAB_00e07385 end
            end
            goto FLOW_past_lab_00e07385
            ::LAB_00e07385::
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("SavedNearEnd", true)
            quest:AutoSaveCheckPoint()
            ::FLOW_past_lab_00e07385::
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
    goto LAB_00e0760e
    ::LAB_00e07574::
    if not bVar2 then goto LAB_00e075b8 end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e075fa end
    bVar2 = quest:IsRegionLoaded("BarrowFields")
    goto LAB_00e07574
    ::LAB_00e075b8::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        uVar15 = 0
        pCVar7 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar7, uVar15)
        repeat
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        until not (not bVar2)
    end
    ::LAB_00e075fa::
    ::LAB_00e0760e::
    ::LAB_00e07620::
end

function TurnToBalv(quest, native_arg_Me)
    local resources = quest:RetailResources()
    local bVar3, ePriority, pPosition, pThing, pppuVar5, r1, xStack_10, xStack_20, xStack_30, xStack_40, xStack_4c
    local alive = true
    xStack_30 = resources:NewResource()
    xStack_40 = resources:NewResource()
    if not (native_arg_Me ~= nil and not native_arg_Me:IsNull()) then
        pPosition = {x = 0, y = 0, z = 0}
    else
        pPosition = native_arg_Me:GetPos()
    end
    r1 = quest:CreateCreature("CREATURE_BALVERINE_01", pPosition, "DarkwoodBalverineTrader")
    quest:EntitySetAsDrawable(r1, false)
    resources:PrepareResource(xStack_40)
    bVar3 = resources:TryAcquire(xStack_40, r1, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            r1 = nil
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            native_arg_Me = nil
            return
        end
        bVar3 = resources:TryAcquire(xStack_40, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        resources:PrepareResource(xStack_30)
        -- TODO(native): bVar3 = resources:TryAcquire(xStack_30, &native_arg_Me, 4)
        bVar3 = nil --[[unresolved native value]]
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e0ad0a end
            -- TODO(native): bVar3 = resources:TryAcquire(xStack_30, &native_arg_Me, 4)
            bVar3 = nil --[[unresolved native value]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_10 = resources:NewResource()
            ePriority = 4
            pppuVar5 = xStack_10
            pThing = quest:GetHero()
            resources:TryAcquire(pppuVar5, pThing, ePriority)
            xStack_4c = resources:NewActorMap()
            resources:SetActor(xStack_4c, "HERO", xStack_10)
            resources:SetActor(xStack_4c, "BALV", xStack_40)
            resources:SetActor(xStack_4c, "TRADER", xStack_30)
            xStack_20 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_DARKWOOD_TRADER_TRANSFORM", xStack_4c, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_20)
            resources:DestroyActorMap(xStack_4c)
            resources:ReleaseResource(xStack_10)
            quest:Pause(0.3)
            resources:PrepareResource(xStack_40)
            repeat
                xStack_4c = nil
                bVar3 = MakeTraderComment(quest, "TRANSFORMED", xStack_4c, 1)
                xStack_4c = nil
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    break
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            until not (not bVar3)
        end
    end
    ::LAB_00e0ad0a::
    resources:ReleaseResource(xStack_40)
    resources:ReleaseResource(xStack_30)
end

function WatchForPickpocketing(quest, native_arg_Trader)
    local bVar1, cVar2, native_arg_sequence_1, xStack_c
    local alive = true
    xStack_c = nil
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    repeat
        if bVar1 then
            xStack_c = nil
            native_arg_Trader = nil
            -- LAB_00e05100: (native jump target)
            -- LAB_00e05108: (native jump target)
            return
        end
        xStack_c = quest:MsgOnHeroPickedPocket()
        bVar1 = (xStack_c ~= nil)
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                cVar2 = (xStack_c ~= nil and xStack_c:IsEqualTo(native_arg_Trader))
                if cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        quest:SetStateInt("TraderPickpocketedCount", quest:GetStateInt("TraderPickpocketedCount") + 1)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        native_arg_sequence_1 = false
                        if not bVar1 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if native_arg_sequence_1 then
                            if 2 < quest:GetStateInt("TraderPickpocketedCount") then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if native_arg_sequence_1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if not bVar1 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if native_arg_sequence_1 then
                            quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 2)
                        end
                    end
                    return
                end
                goto LAB_00e04f80
            end
            -- LAB_00e050a5: (native jump target)
            xStack_c = nil
            native_arg_Trader = nil
            return
        end
        ::LAB_00e04f80::
        if not (native_arg_Trader ~= nil and not native_arg_Trader:IsNull()) then
            cVar2 = false
        else
            cVar2 = (native_arg_Trader ~= nil and native_arg_Trader:MsgIsKilledBy(""))
        end
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 0)
            end
            xStack_c = nil
            native_arg_Trader = nil
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function MakeTraderComment(quest, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local __native_condition_1, bVar2, cVar3, iVar5, iVar6, pCVar10, pCVar4, pCVar7, pCVar8, pCVar9, pcVar15, piVar1, piVar13, r1, r2, r3, r4, this_00, uVar11, uVar14, xStack_30
    local alive = true
    -- TODO(native): xStack_30._4_4_ = *(undefined4 *)(native_arg_speaker + 0x4);
    -- TODO(native): xStack_30 = *(int **)(native_arg_speaker + 0x8);
    xStack_30 = nil
    if xStack_30 ~= nil then
        -- TODO(native): *xStack_30 = *xStack_30 + 1;
    end
    -- TODO(native): bVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)(xStack_30)
    bVar2 = nil --[[unresolved native value]]
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return false
        end
        pCVar4 = quest:GetHero()
        pCVar4 = quest:GetNearestWithScriptName(pCVar4, "DarkwoodTrader")
        -- TODO(native): piVar13 = *(pCVar4 + 0x8)
        piVar13 = nil --[[unresolved native value]]
        -- TODO(native): piVar1 = *(pCVar4 + 0x4)
        piVar1 = nil --[[unresolved native value]]
        if xStack_30 ~= piVar13 then
            -- TODO(native): xStack_30._4_4_ = piVar1;
            if piVar13 ~= nil then
                -- TODO(native): *piVar13 = *piVar13 + 1;
            end
        end
    end
    -- TODO(native): bVar2 = (**(xStack_30._0_4_ + 0x12c))(xStack_30)
    bVar2 = nil --[[unresolved native value]]
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    if native_arg_comment_type ~= 2 then
        iVar5 = quest:GetTimer(quest:GetStateInt("CommentTimer"))
        if 0 < iVar5 then
            alive = not quest:IsActiveThreadTerminating()
            return false
        end
        iVar6 = quest:AddNewConversation(xStack_30, false, false)
        pCVar4 = quest:GetHero()
        quest:AddPersonToConversation(iVar6, pCVar4)
        if not (xStack_30 ~= nil and not xStack_30:IsNull()) then
            native_arg_speaker = ""
        else
            xStack_30:GetDataString()
        end
        pCVar7 = quest:GetHero()
        pCVar9 = native_arg_comment_to_make
        pCVar4 = xStack_30
        uVar14 = false
        piVar13 = "_"
        pCVar8 = native_arg_comment_to_make
        pCVar10 = ("TEXT_QST_067_" .. native_arg_speaker)
        pCVar10 = (pCVar10 .. "_")
        pCVar8 = (pCVar10 .. pCVar8)
        quest:AddLineToConversation(iVar6, pCVar8, pCVar4, pCVar7, uVar14)
        if native_arg_comment_type ~= 1 then goto LAB_00e02355 end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e01aca end
        r1 = quest:GetNearestWithScriptName(xStack_30, "DarkwoodTrader")
        iVar5 = (r1 ~= nil and r1:IsAlive())
        __native_condition_1 = not iVar5
        if not __native_condition_1 then
            cVar3 = (r1 ~= nil and r1:IsEqualTo(xStack_30._4_4_))
            __native_condition_1 = cVar3
        end
        if __native_condition_1 then
            goto LAB_00e02213
        else
            pcVar15 = "_RESPONSE"
            piVar13 = "_"
            pCVar8 = pCVar9
            pCVar10 = r1:GetDataString()
            pCVar10 = ("TEXT_QST_067_" .. pCVar10)
            pCVar10 = (pCVar10 .. "_")
            pCVar8 = (pCVar10 .. pCVar8)
            pCVar8 = (pCVar8 .. pcVar15)
            bVar2 = quest:TextEntryExists(pCVar8)
            native_arg_speaker = CONCAT31(native_arg_speaker._1_3_,1)
            if not bVar2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        uVar11 = 0x1f
        if (0x1f & 0x10) ~= 0 then
            uVar11 = 0x1f & 0xffffffef
        end
        if (uVar11 & 8) ~= 0 then
            uVar11 = uVar11 & 0xfffffff7
        end
        if (uVar11 & 4) ~= 0 then
            uVar11 = uVar11 & 0xfffffffb
        end
        if (uVar11 & 2) ~= 0 then
            uVar11 = uVar11 & 0xfffffffd
        end
        if native_arg_speaker ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return false
            end
            quest:AddPersonToConversation(iVar6, r1)
            pCVar7 = quest:GetHero()
            pCVar4 = r1
            uVar14 = false
            pcVar15 = "_RESPONSE"
            piVar13 = "_"
            pCVar8 = r1:GetDataString()
            pCVar8 = ("TEXT_QST_067_" .. pCVar8)
            pCVar8 = (pCVar8 .. "_")
            pCVar9 = (pCVar8 .. pCVar9)
            pCVar9 = (pCVar9 .. pcVar15)
            quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
        end
        ::LAB_00e02355::
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        return true
    end
    ::FLOW_after_lab_00e02355::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e01aca end
    r2 = quest:GetNearestWithScriptName(xStack_30, "DarkwoodTrader")
    r3 = quest:GetFurthestWithScriptName(xStack_30, "DarkwoodTrader")
    cVar3 = (r2 ~= nil and r2:IsEqualTo(r3._4_4_))
    if not cVar3 then
        goto LAB_00e01afb
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_c);
            -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,iVar5);
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    pcVar15 = "SCARED"
    this_00 = r2:GetDataString()
    iVar5 = ((this_00 ~= pcVar15) and 1 or 0)
    if iVar5 ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e01ab8 end
        -- TODO(native): xStack_24 = xStack_18;
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,(int)&xStack_c);
    end
    pCVar9 = native_arg_comment_to_make
    if native_arg_comment_to_make == nil then
        bVar2 = false
        if bVar2 then
            goto LAB_00e01ba2
        end
    else
        iVar5 = ((native_arg_comment_to_make == "ROCK_TROLL_CLOSE") and 0 or 1)
        cVar3 = not (iVar5 ~= 0)
        native_arg_comment_to_make = CONCAT31(native_arg_comment_to_make._1_3_,cVar3)
        if cVar3 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e01ab8 end
    pCVar4 = quest:GetThingWithScriptName("RockTrollTrigger")
    r4 = quest:PlayCriteriaSoundOnThing(pCVar4, "EARTH_TROLL_OFFSCREEN_ROAR")
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    iVar6 = quest:AddNewConversation(r3, false, false)
    pCVar4 = quest:GetHero()
    quest:AddPersonToConversation(iVar6, pCVar4)
    iVar5 = (r2 ~= nil and r2:IsAlive())
    if not iVar5 then
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:AddPersonToConversation(iVar6, r2)
        iVar5 = (r3 ~= nil and r3:IsAlive())
        if not iVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                pCVar7 = quest:GetHero()
                pCVar4 = xStack_30
                uVar14 = false
                piVar13 = "_"
                pCVar8 = xStack_30:GetDataString()
                pCVar8 = ("TEXT_QST_067_" .. pCVar8)
                pCVar8 = (pCVar8 .. "_")
                pCVar9 = (pCVar8 .. pCVar9)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r2
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_NO"
                pCVar9 = r2:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar4 = quest:GetHero()
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = xStack_30
                pCVar9 = pCVar7:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar7, pCVar4, uVar14)
                -- LAB_00e01ff9_c2: (native jump target)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:AddPersonToConversation(iVar6, r3)
                pCVar7 = quest:GetHero()
                pCVar4 = xStack_30
                uVar14 = false
                piVar13 = "_"
                pCVar8 = xStack_30:GetDataString()
                pCVar8 = ("TEXT_QST_067_" .. pCVar8)
                pCVar8 = (pCVar8 .. "_")
                pCVar9 = (pCVar8 .. pCVar9)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r2
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_NO"
                pCVar9 = r2:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r3
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_OATH"
                pCVar9 = r3:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r2
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_BAD_NEWS"
                pCVar9 = r2:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar4 = quest:GetHero()
                -- LAB_00e01fb0: (native jump target)
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = xStack_30
                pCVar9 = pCVar7:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar7, pCVar4, uVar14)
                -- LAB_00e01ff9: (native jump target)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
                return true
            end
        end
        ::FLOW_after_lab_00e01fb0::
    end
    ::FLOW_past_lab_00e01afb::
    ::LAB_00e01ab8::
    ::LAB_00e01aca::
    return false
end

