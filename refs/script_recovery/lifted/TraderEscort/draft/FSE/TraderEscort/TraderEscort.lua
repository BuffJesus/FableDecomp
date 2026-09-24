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
    local bVar5, cVar6, iVar9, pCVar7, pCVar8, r1, uVar10, xStack_2c, xStack_30
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
        -- TODO(native): Std_Vector_Erase_Range(&xStack_30,(int)xStack_30,(int)xStack_2c);
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
                        -- TODO(native): pCVar7 = (**(xStack_30[uVar10 * 3] + 0x18))()
                        pCVar7 = nil --[[unresolved native value]]
                        r1 = quest:CreateCreature(pCVar7, nil --[[missing]], "SurpriseBalverine")
                        quest:EntitySetCutsceneBehaviour(r1, 1)
                        bVar5 = false
                        pCVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r1, pCVar8, bVar5)
                        quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                        iVar9 = 0
                        xStack_30 = quest:GetNearestWithScriptName(r1, "BALVERINE_SURPRISE")
                        pCVar8 = #xStack_30
                        -- TODO(native): MakeTraderComment(quest, &xStack_34, pCVar8, iVar9)
                        iVar9 = math.random(0, 32767)
                        if iVar9 % 5 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00e063f5(); return end
                            xStack_2c = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
                            pCVar8 = #xStack_2c
                            quest:GiveThingBestEnemyTarget(r1, xStack_2c)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00e063f5(); return end
                            pCVar8 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(r1, pCVar8)
                        end
                        -- TODO(native): cVar6 = (**(r1._0_4_ + 0x12c))()
                        cVar6 = nil --[[unresolved native value]]
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
    if cVar6 == 0 then goto LAB_00e06349 end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then __cleanup_LAB_00e063f5(); return end
    -- TODO(native): cVar6 = (**(r1._0_4_ + 0x12c))()
    cVar6 = nil --[[unresolved native value]]
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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar2, cVar3, dist, iVar5, native_arg_sequence_1, pCVar6, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, xStack_1c, xStack_2c, xStack_3c, xStack_50, xStack_60_2, xStack_b0, xStack_d0, xStack_dc, x_stk_60
    local alive = true
    if not quest:GetStateBool("IntroFinished") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        quest:SetStateBool("SavedInMiddle", false)
        cVar3 = quest:IsRegionLoaded("Darkwood1")
        while not cVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
            cVar3 = quest:IsRegionLoaded("Darkwood1")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        r1 = quest:GetThingWithScriptName("MK_DTE_INTRO_TRADER1")
        r2 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
        r3 = quest:GetNearestWithScriptName(r2, "DarkwoodTrader")
        x_stk_60 = resources:NewResource()
        xStack_1c = resources:NewResource()
        xStack_2c = resources:NewResource()
        r4 = quest:GetHero()
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: &uStack_74
        xStack_b0 = resources:NewActorMap()
        -- TODO(native): resources:SetActor(xStack_b0, "HERO", &auStack_70)
        -- TODO(native): resources:SetActor(xStack_b0, "TRADER1", &auStack_a4)
        resources:SetActor(xStack_b0, "TRADER2", xStack_60_2)
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
        resources:DestroyMovie(xStack_60_2)
    end
    r5 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(r5, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_02", "Darkwood4", "Greatwood")
    if not quest:GetStateBool("QuestStartScreened") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        quest:SetStateBool("IntroFinished", true)
        r6 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(r6, nil --[[missing]], nil --[[missing]])
        quest:SetStateBool("QuestStartScreened", true)
    end
    if not quest:GetStateBool("FollowInfoGiven") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        cVar3 = quest:IsXbox()
        if not cVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT_PC")
            cVar3 = quest:MsgIsGameInfoClickedPast()
            while not cVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e07620 end
                cVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e07620 end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT")
            cVar3 = quest:MsgIsGameInfoClickedPast()
            while not cVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e07620 end
                cVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        end
        quest:SetStateBool("FollowInfoGiven", true)
    end
    iVar5 = quest:GetStateInt("TradersStillAliveCounter")
    while iVar5 < 1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e07620 end
        iVar5 = quest:GetStateInt("TradersStillAliveCounter")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e07620 end
    xStack_d0 = nil
    xStack_dc = nil
    if quest:GetStateBool("SavedInMiddle") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e0760e end
        r7 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(r7, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while not bVar2 do
        if not quest:GetStateBool("GoneWrongWay") then
            cVar3 = quest:IsRegionLoaded("GreatwoodCaves")
            if not cVar3 then goto LAB_00e06b57 end
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
            cVar3 = quest:MsgIsGameInfoClickedPast()
            while not cVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e0760e end
                cVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("GoneWrongWay", true)
        end
        if not quest:GetStateBool("ShownBalverine") then
            cVar3 = quest:IsRegionLoaded("Darkwood2")
            if not cVar3 then goto LAB_00e06c33 end
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
            xStack_60_2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            resources:RunMacro("CS_DARKWOOD_TRADER_BALVERINE", xStack_3c, false, true)
            quest:SetStateBool("ShownBalverine", true)
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            resources:DestroyMovie(xStack_60_2)
            resources:DestroyActorMap(xStack_3c)
        end
        if not quest:GetStateBool("EndStarted") then
            cVar3 = quest:IsRegionLoaded("BarrowFields")
            native_arg_sequence_1 = false
            if not cVar3 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                -- TODO(native): cVar3 = (*xStack_dc[0x4b])()
                cVar3 = nil --[[unresolved native value]]
                if cVar3 ~= 0 then
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
                iVar5 = quest:GetThingWithScriptName("EndTrader")
                -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_dc,iVar5);
                iVar5 = quest:GetThingWithScriptName("M_EndTheQuestHere")
                -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_d0,iVar5);
                -- TODO(native): cVar3 = (**(xStack_8c + 0x12c))()
                cVar3 = nil --[[unresolved native value]]
                if cVar3 ~= 0 then goto LAB_00e06fb5 end
                if not quest:GetStateBool("EndStarted") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        ::LAB_00e06e73::
                        if not quest:GetStateBool("EndStarted") then
                            cVar3 = quest:IsRegionLoaded("BarrowFields")
                            if not cVar3 then goto LAB_00e06eb0 end
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
                            pCVar6 = quest:GetHero()
                            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, xStack_d0, dist)
                            if bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then break end
                                r8 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
                                if (0 - 0) / 0xc == quest:GetStateInt("TradersStillAliveCounter") then
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
            r9 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
            quest:GiveHeroGold(0)
            r10 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(r10, false, false, false)
            quest:SetCreatureGeneratorsEnabled("Darkwood1", (dist ~= 0))
            quest:SetCreatureGeneratorsEnabled("Darkwood2", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood3", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood5", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood6", true)
            cVar3 = quest:IsRegionLoaded("BarrowFields")
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
            quest:SetCreatureGeneratorsEnabled("Darkwood1", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood2", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood3", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood5", nil --[[missing]])
            quest:SetCreatureGeneratorsEnabled("Darkwood6", nil --[[missing]])
            -- TODO(native): CCharString__AssignFromWide(xStack_d0 + 4,0x12df8d8);
            r11 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(r11, nil --[[missing]], r9, nil --[[missing]])
            quest:SetStateBool("MissionFailed", true)
            r12 = quest:GetThingWithScriptName("DarkwoodTrader")
            cVar3 = quest:IsRegionLoaded("Darkwood1")
            __native_condition_1 = not cVar3
            if not __native_condition_1 then
                -- TODO(native): iVar5 = &iStack_c0:IsAlive()
                iVar5 = nil --[[unresolved native value]]
                __native_condition_1 = not iVar5
            end
            if __native_condition_1 then
                bVar2 = false
            else
                bVar2 = true
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    -- TODO(native): cVar3 = (**(iStack_c0 + 0x12c))()
                    cVar3 = nil --[[unresolved native value]]
                    while cVar3 ~= 0 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e07609 end
                        -- TODO(native): cVar3 = (**(iStack_c0 + 0x12c))()
                        cVar3 = nil --[[unresolved native value]]
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then goto LAB_00e07208 end
                end
                ::LAB_00e07609::
                break
            end
            ::LAB_00e07208::
            r13 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(r13, nil --[[missing]])
        end
        __native_condition_2 = not quest:GetStateBool("SavedInMiddle")
        if __native_condition_2 then
            cVar3 = quest:MsgOnRegionLoaded()
            __native_condition_2 = cVar3 ~= 0
        end
        if __native_condition_2 then
            if "" == nil then
                bVar2 = false
                if bVar2 then
                    goto LAB_00e07299
                end
            else
                -- TODO(native): iVar5 = CBasicString<char>::Compare((void *)*xStack_f4,"Darkwood4");
                cVar3 = not (iVar5 ~= 0)
                -- TODO(native): xStack_e4 = CONCAT13(cVar3,(undefined3)xStack_e4);
                if cVar3 then goto LAB_00e07299 end
            end
            goto FLOW_past_lab_00e07299
            ::LAB_00e07299::
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("SavedInMiddle", true)
            r14 = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(r14, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", r8)
            quest:AutoSaveCheckPoint()
            ::FLOW_past_lab_00e07299::
        end
        __native_condition_3 = not quest:GetStateBool("SavedNearEnd")
        if __native_condition_3 then
            cVar3 = quest:MsgOnRegionLoaded()
            __native_condition_3 = cVar3 ~= 0
        end
        if __native_condition_3 then
            if "" == nil then
                bVar2 = false
                if bVar2 then
                    goto LAB_00e07385
                end
            else
                -- TODO(native): iVar5 = CBasicString<char>::Compare((void *)*xStack_f4,"Darkwood6");
                cVar3 = not (iVar5 ~= 0)
                -- TODO(native): xStack_e4 = CONCAT13(cVar3,(undefined3)xStack_e4);
                if cVar3 then goto LAB_00e07385 end
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
    if not cVar3 then goto LAB_00e075b8 end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e075fa end
    cVar3 = quest:IsRegionLoaded("BarrowFields")
    goto LAB_00e07574
    ::LAB_00e075b8::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        r15 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(r15, nil --[[missing]])
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
    if native_arg_Me._4_4_ == 0 then
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
            -- TODO(native): if (native_arg_Me._8_4_ ~= 0) and (*native_arg_Me._8_4_ = *native_arg_Me._8_4_ + -1, *native_arg_Me._8_4_ == 0) then
            if false then
                -- TODO(native): (**(code **)(native_arg_Me._8_4_ + 4))();
            end
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
            -- TODO(native): if (native_arg_Trader._8_4_ ~= 0) and (*native_arg_Trader._8_4_ = *native_arg_Trader._8_4_ + -1, *native_arg_Trader._8_4_ == 0) then
            if false then
                -- TODO(native): (**(code **)(native_arg_Trader._8_4_ + 4))();
                -- LAB_00e05100: (native jump target)
            end
            -- LAB_00e05108: (native jump target)
            return
        end
        bVar1 = quest:MsgOnHeroPickedPocket()
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                cVar2 = (xStack_c ~= nil and xStack_c:IsEqualTo(native_arg_Trader._4_4_))
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
            goto LAB_00e050a5
        end
        goto FLOW_past_lab_00e050a5
        ::LAB_00e050a5::
        xStack_c = nil
        native_arg_Trader = nil
        -- TODO(native): if (native_arg_Trader._8_4_ == 0) or (*native_arg_Trader._8_4_ = *native_arg_Trader._8_4_ + -1, *native_arg_Trader._8_4_ ~= 0) then
        if false then
            return
        end
        -- TODO(native): (**(code **)(native_arg_Trader._8_4_ + 4))();
        do __cleanup_LAB_00e05100(); return end
        ::FLOW_past_lab_00e050a5::
        ::LAB_00e04f80::
        if native_arg_Trader._4_4_ == 0 then
            cVar2 = 0
        else
            cVar2 = (native_arg_Trader ~= nil and native_arg_Trader:MsgIsKilledBy(""))
        end
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 0)
            end
            goto LAB_00e050a5
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function MakeTraderComment(quest, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local CVar1, CVar8, __native_condition_1, bVar3, cVar2, iVar4, local_28, p0, pCVar6, pCVar7, pcVar14, piVar13, r1, r10, r11, r12, r13, r14, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar10, uVar12, uVar5, u_stk_84, xStack_18, xStack_24, xStack_34, xStack_3c
    local alive = true
    -- TODO(native): local_2c = *(CCharString *)(native_arg_speaker + 0x4);
    -- TODO(native): local_28 = *(CCharString *)(native_arg_speaker + 0x8);
    xStack_24 = nil
    if local_28 ~= nil then
        -- TODO(native): *(int *)local_28 = *(int *)local_28 + 1;
    end
    -- TODO(native): cVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)()
    cVar2 = nil --[[unresolved native value]]
    if cVar2 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return false
        end
        r1 = quest:GetHero()
        iVar4 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
        -- TODO(native): CVar8 = *(iVar4 + 8)
        CVar8 = nil --[[unresolved native value]]
        -- TODO(native): CVar1 = *(iVar4 + 4)
        CVar1 = nil --[[unresolved native value]]
        if local_28 ~= CVar8 then
            if CVar8 ~= nil then
                -- TODO(native): *(int *)CVar8 = *(int *)CVar8 + 1;
            end
        end
    end
    -- TODO(native): cVar2 = (*xStack_24[0x4b])()
    cVar2 = nil --[[unresolved native value]]
    if cVar2 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    if native_arg_comment_type ~= 2 then
        iVar4 = quest:GetTimer(quest:GetStateInt("CommentTimer"))
        if 0 < iVar4 then
            alive = not quest:IsActiveThreadTerminating()
            return false
        end
        r2 = quest:AddNewConversation(nil --[[missing]], false, nil --[[missing]])
        r3 = quest:GetHero()
        quest:AddPersonToConversation(nil --[[missing]], r3)
        if xStack_34 == nil then
            -- TODO(native): CCharString::CCharString((CCharString *)xStack_24,(CCharString *)&DAT_0143e8ec);
        else
            -- TODO(native): (**(code **)(*(int *)xStack_34 + 0xc))();
        end
        r4 = quest:GetHero()
        piVar13 = "_"
        pCVar7 = ("TEXT_QST_067_" .. xStack_24)
        pCVar7 = (pCVar7 .. piVar13)
        (pCVar7 .. CVar8)
        quest:AddLineToConversation(nil --[[missing]], piVar13, r4, nil --[[missing]])
        if native_arg_comment_type ~= 1 then goto LAB_00e02355 end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e01aca end
        r5 = quest:GetNearestWithScriptName(nil --[[missing]], "DarkwoodTrader")
        -- TODO(native): iVar4 = &xStack_18:IsAlive()
        iVar4 = nil --[[unresolved native value]]
        __native_condition_1 = not iVar4
        if not __native_condition_1 then
            cVar2 = xStack_c:IsEqualTo(nil --[[missing]])
            __native_condition_1 = cVar2
        end
        if __native_condition_1 then
            goto LAB_00e02213
        else
            pcVar14 = "_RESPONSE"
            piVar13 = "_"
            -- TODO(native): pCVar7 = &xStack_18:GetDataString()
            pCVar7 = nil --[[unresolved native value]]
            pCVar7 = ("TEXT_QST_067_" .. pCVar7)
            pCVar7 = (pCVar7 .. piVar13)
            pCVar7 = (pCVar7 .. CVar8)
            (pCVar7 .. pcVar14)
            cVar2 = quest:TextEntryExists()
            native_arg_speaker = CONCAT31(native_arg_speaker._1_3_,1)
            if not cVar2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        CVar8 = 31
        if (31 & 0x10) ~= 0 then
            CVar8 = 31 & 0xffffffef
        end
        if (CVar8 & 8) ~= 0 then
            CVar8 = CVar8 & 0xfffffff7
        end
        if (CVar8 & 4) ~= 0 then
            CVar8 = CVar8 & 0xfffffffb
        end
        if (CVar8 & 2) ~= 0 then
            CVar8 = CVar8 & 0xfffffffd
        end
        if native_arg_speaker ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return false
            end
            quest:AddPersonToConversation(31, r5)
            r6 = quest:GetHero()
            pcVar14 = "_RESPONSE"
            piVar13 = "_"
            pCVar7 = 31:GetDataString()
            pCVar7 = ("TEXT_QST_067_" .. pCVar7)
            pCVar7 = (pCVar7 .. piVar13)
            pCVar7 = (pCVar7 .. xStack_3c)
            (pCVar7 .. pcVar14)
            quest:AddLineToConversation(nil --[[missing]], piVar13, r6, nil --[[missing]])
        end
        ::LAB_00e02355::
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        return true
    end
    ::FLOW_after_lab_00e02355::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e01aca end
    r7 = quest:GetNearestWithScriptName(nil --[[missing]], "DarkwoodTrader")
    r8 = quest:GetFurthestWithScriptName(r7, "DarkwoodTrader")
    cVar2 = xStack_c:IsEqualTo(nil --[[missing]])
    if not cVar2 then
        goto LAB_00e01afb
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            -- TODO(native): iVar4 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_18);
            -- TODO(native): CScriptThing::operator=((CScriptThing *)&xStack_c,iVar4);
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    pcVar14 = "SCARED"
    this_00 = xStack_c:GetDataString()
    iVar4 = ((this_00 ~= pcVar14) and 1 or 0)
    if iVar4 ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e01ab8 end
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_c,(int)&xStack_18);
        xStack_18 = xStack_24
    end
    pCVar7 = native_arg_comment_to_make
    if native_arg_comment_to_make == nil then
        bVar3 = false
        if bVar3 then
            goto LAB_00e01ba2
        end
    else
        iVar4 = ((native_arg_comment_to_make == "ROCK_TROLL_CLOSE") and 0 or 1)
        cVar2 = not (iVar4 ~= 0)
        native_arg_comment_to_make = CONCAT31(native_arg_comment_to_make._1_3_,cVar2)
        if cVar2 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e01ab8 end
    p0 = quest:GetThingWithScriptName("RockTrollTrigger")
    r9 = quest:PlayCriteriaSoundOnThing(p0, "EARTH_TROLL_OFFSCREEN_ROAR")
    quest:Pause(nil --[[missing]])
    ::FLOW_past_lab_00e01ba2::
    uVar5 = quest:AddNewConversation(r8, nil --[[missing]], nil --[[missing]])
    r10 = quest:GetHero()
    quest:AddPersonToConversation(nil --[[missing]], r10)
    iVar4 = xStack_c:IsAlive()
    if not iVar4 then
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
        -- TODO(native): iVar4 = &xStack_40:IsAlive()
        iVar4 = nil --[[unresolved native value]]
        if not iVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                r11 = quest:GetHero()
                piVar13 = "_"
                pCVar6 = xStack_24:GetDataString()
                pCVar6 = ("TEXT_QST_067_" .. pCVar6)
                pCVar6 = (pCVar6 .. piVar13)
                (pCVar6 .. pCVar7)
                quest:AddLineToConversation(nil --[[missing]], piVar13, r11, nil --[[missing]])
                r12 = quest:GetHero()
                pcVar14 = "_INTRO_RESPONSE_NO"
                -- TODO(native): pCVar7 = &xStack_38:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                (pCVar7 .. pcVar14)
                quest:AddLineToConversation(nil --[[missing]], pcVar14, r12, nil --[[missing]])
                uVar12 = quest:GetHero()
                uVar10 = 0
                pcVar14 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = this_01:GetDataString()
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, uVar12, nil --[[missing]], (uVar10 ~= 0))
                -- LAB_00e01ff9_c2: (native jump target)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
                r13 = quest:GetHero()
                piVar13 = "_"
                -- TODO(native): pCVar6 = &xStack_38:GetDataString()
                pCVar6 = nil --[[unresolved native value]]
                pCVar6 = ("TEXT_QST_067_" .. pCVar6)
                pCVar6 = (pCVar6 .. piVar13)
                (pCVar6 .. pCVar7)
                quest:AddLineToConversation(nil --[[missing]], piVar13, r13, nil --[[missing]])
                r14 = quest:GetHero()
                pcVar14 = "_INTRO_RESPONSE_NO"
                -- TODO(native): pCVar7 = &xStack_34:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                (pCVar7 .. pcVar14)
                quest:AddLineToConversation(nil --[[missing]], pcVar14, r14, nil --[[missing]])
                u_stk_84 = quest:GetHero()
                uVar12 = 0
                pcVar14 = "_INTRO_RESPONSE_OATH"
                -- TODO(native): pCVar7 = &stack0xffffffb8:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, u_stk_84, nil --[[missing]], (uVar12 ~= 0))
                uVar12 = quest:GetHero()
                uVar10 = 0
                pcVar14 = "_INTRO_RESPONSE_BAD_NEWS"
                -- TODO(native): pCVar7 = &stack0xffffff98:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, uVar12, nil --[[missing]], (uVar10 ~= 0))
                uVar12 = quest:GetHero()
                -- LAB_00e01fb0: (native jump target)
                uVar10 = 0
                pcVar14 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = this_01:GetDataString()
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, uVar12, nil --[[missing]], (uVar10 ~= 0))
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

