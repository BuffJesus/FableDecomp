-- Readable native conversion: Q_TraderEscort. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    SummoningShipNoDamageBoastReward = 504,  -- 2500
    TCGKillNoBanditsCost = 508,  -- 400
    TCGKillNoBanditsReward = 512,  -- 1000
    TCGMadeTimeLimitCost = 516,  -- 400
    TE_TraderCommentDelay = 3568,  -- 5
    TE_TraderLowInfectionTime = 3576,  -- 80
    TE_TraderMediumInfectionTime = 3580,  -- 40
    TE_TraderHighInfectionTime = 3584,  -- 20
    TE_AllTradersAliveBoastCost = 3588,  -- 100
    TE_AllTradersAliveBoastReward = 3592,  -- 400
    TE_TraderGoldReward = 3620,  -- 100
}

-- Q_TraderEscort.Main (retail 0x00e00db0)
function Main(quest)
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
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_01", "Darkwood1", "Greatwood")
    quest:CreateThread("WatchForSurprisingBalverines")  -- native thread body CQ_TraderEscortScript::WatchForSurprisingBalverines: lift it as function WatchForSurprisingBalverines(quest)
    quest:CreateThread("WatchForMissionRules")  -- native thread body 0x00E06440: lift it as function WatchForMissionRules(quest)
    if quest:GetStateBool("QuestStartScreened") then goto LAB_00e015c9 end
    while true do
        if not quest:NewScriptFrame() then break end
        if quest:HeroHasExpression("EXPRESSION_PICKPOCKET") then
            if quest:IsActiveThreadTerminating() then return end
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS", 10, quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastReward), true, getActiveQuestName, 0)
            goto LAB_00e015c9
        end
        if quest:GetStateBool("QuestStartScreened") then
            return
        end
    end
    goto FLOW_past_lab_00e015c9
    ::LAB_00e015c9::
    ::FLOW_past_lab_00e015c9::
end

-- Q_TraderEscort.Init (retail 0x00e006d0)
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
    quest:SetStateInt("IncubationTimeLow", quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderLowInfectionTime))
    quest:SetStateInt("IncubationTimeMedium", quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderMediumInfectionTime))
    quest:SetStateInt("IncubationTimeHigh", quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderHighInfectionTime))
    quest:SetStateString("TraderToTalk", "")
    quest:SetMasterGameState("DarkwoodAllTradersAlive", true)
    quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.SummoningShipNoDamageBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.TCGKillNoBanditsCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.TCGKillNoBanditsReward), quest:ReadGlobalGameData(SCRIPT_DEF.TCGMadeTimeLimitCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_ALLTRADERSALIVE", 9, quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastReward), false, "", 0)
end

-- Q_TraderEscort.OnPersist (retail 0x00e00ce0)
function OnPersist(quest, context)
    quest:SetStateBool("IntroFinished", quest:PersistTransferBool(context, "IntroFinished", quest:GetStateBool("IntroFinished")))
    quest:SetStateBool("SavedInMiddle", quest:PersistTransferBool(context, "SavedInMiddle", quest:GetStateBool("SavedInMiddle")))
    quest:SetStateBool("SavedNearEnd", quest:PersistTransferBool(context, "SavedNearEnd", quest:GetStateBool("SavedNearEnd")))
    quest:SetStateBool("EndStarted", quest:PersistTransferBool(context, "EndStarted", quest:GetStateBool("EndStarted")))
    quest:SetStateBool("QuestStartScreened", quest:PersistTransferBool(context, "QuestStartScreened", quest:GetStateBool("QuestStartScreened")))
    quest:SetStateBool("ShownBalverine", quest:PersistTransferBool(context, "ShownBalverine", quest:GetStateBool("ShownBalverine")))
    quest:SetStateBool("ShownEarthTroll", quest:PersistTransferBool(context, "ShownEarthTroll", quest:GetStateBool("ShownEarthTroll")))
end

-- Q_TraderEscort.WatchForSurprisingBalverines (retail 0x00e06050)
function WatchForSurprisingBalverines(quest)
    local scratchValue, scratchValue2, scratchValue6
    local hero = quest:GetHero()
    if not quest:IsActiveThreadTerminating() then goto LAB_00e06074 end
    ::FLOW_after_lab_00e06394::
    goto FLOW_past_lab_00e06074
    ::LAB_00e06074::
    if quest:GetStateInt("BalverinesToSurpriseHeroNeeded") < 1 then goto LAB_00e06373 end
    if not quest:IsActiveThreadTerminating() then
        quest:Pause(2.0)
        -- TODO(native): Std_Vector_Erase_Range(&xStack_30,(int)xStack_30,(int)xStack_2c);
        local balverineSurprise = quest:GetAllThingsWithScriptName("M_BalverineSurprise")
        scratchValue6 = 0
        if #balverineSurprise ~= 0 then
            scratchValue2 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00e063db end
                if 0 >= quest:GetStateInt("BalverinesToSurpriseHeroNeeded") then scratchValue6 = scratchValue6 + 1; scratchValue2 = scratchValue2 + 1; goto continue_1 end
                if not quest:IsCameraPosOnScreen(balverineSurprise[scratchValue2 + 1]:GetPos()) then
                    if quest:IsActiveThreadTerminating() then return end
                    -- TODO(native): pCVar7 = (**(xStack_30[uVar10 * 3] + 0x18))()
    --[[unresolved native value]]
                    local scratchValue5 = quest:CreateCreature(nil, nil --[[missing]], "SurpriseBalverine")
                    quest:EntitySetCutsceneBehaviour(scratchValue5, CUTSCENE_BEHAVIOUR_PAUSED)
                    quest:EntitySetFacingAngleTowardsThing(scratchValue5, hero, false)
                    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                    -- TODO(native): MakeTraderComment(quest, &xStack_34, pCVar8, iVar9)
                    if math.random(0, 32767) % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00e063f5(); return end
                        local darkwoodTrader = quest:GetNearestWithScriptName(scratchValue5, "DarkwoodTrader")
                        quest:GiveThingBestEnemyTarget(scratchValue5, darkwoodTrader)
                    else
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00e063f5(); return end
                        quest:GiveThingBestEnemyTarget(scratchValue5, hero)
                    end
                    -- TODO(native): cVar6 = (**(r1._0_4_ + 0x12c))()
                    scratchValue = nil --[[unresolved native value]]
                    goto LAB_00e06319
                end
                scratchValue6 = scratchValue6 + 1
                scratchValue2 = scratchValue2 + 1
                ::continue_1::
            until scratchValue6 >= #balverineSurprise
        end
        goto LAB_00e06364
    end
    ::FLOW_past_lab_00e06074::
    ::LAB_00e0642a::
    do return end
    ::LAB_00e063db::
    goto LAB_00e0642a
    ::LAB_00e06319::
    if scratchValue ~= 0 then
        if not quest:NewScriptFrame() then __cleanup_LAB_00e063f5(); return end
        -- TODO(native): cVar6 = (**(r1._0_4_ + 0x12c))()
        scratchValue = nil --[[unresolved native value]]
        goto LAB_00e06319
    end
    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00e063f5(); return end
    quest:SetStateInt("BalverinesToSurpriseHeroNeeded", quest:GetStateInt("BalverinesToSurpriseHeroNeeded") - 1)
    ::LAB_00e06364::
    if quest:IsActiveThreadTerminating() then goto LAB_00e06415 end
    ::LAB_00e06373::
    if not quest:NewScriptFrame() then goto FLOW_after_lab_00e06394 end
    goto LAB_00e06074
    ::LAB_00e06415::
    goto LAB_00e0642a
end

-- Q_TraderEscort.WatchForMissionRules (retail 0x00e06440)
function WatchForMissionRules(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isRegionLoaded2, predicateResult, predicateResult3, predicateResult4, predicateResult7
    local isRegionLoaded6, sequence, darkwoodTrader4
    if not quest:GetStateBool("IntroFinished") then
        if quest:IsActiveThreadTerminating() then goto LAB_00e07620 end
        quest:SetStateBool("SavedInMiddle", false)
        while not quest:IsRegionLoaded("Darkwood1") do
            if not quest:NewScriptFrame() then goto LAB_00e07620 end
        end
        local darkwoodTrader = quest:GetNearestWithScriptName(quest:GetThingWithScriptName("MK_DTE_INTRO_TRADER1"), "DarkwoodTrader")
        local darkwoodTrader2 = quest:GetNearestWithScriptName(darkwoodTrader, "DarkwoodTrader")
        local resource3 = resources:NewResource()
        local resource = resources:NewResource()
        local resource2 = resources:NewResource()
        resources:TryAcquire(resource2, hero, 4)
        resources:TryAcquire(resource3, darkwoodTrader, 4)
        resources:TryAcquire(resource, darkwoodTrader2, 4)
        local actorMap2 = resources:NewActorMap()
        resources:SetActor(actorMap2, "HERO", resource2)
        resources:SetActor(actorMap2, "TRADER1", resource3)
        resources:SetActor(actorMap2, "TRADER2", resource)
        local movie = resources:StartMovie("")
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_DARKWOOD_TRADER_INTRO", actorMap2, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource3)
    end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_02", "Darkwood4", "Greatwood")
    if not quest:GetStateBool("QuestStartScreened") then
        if quest:IsActiveThreadTerminating() then goto LAB_00e07620 end
        quest:SetStateBool("IntroFinished", true)
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    if not quest:GetStateBool("FollowInfoGiven") then
        if quest:IsActiveThreadTerminating() then goto LAB_00e07620 end
        if quest:IsXbox() then
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00e07620 end
            end
            if quest:IsActiveThreadTerminating() then return end
        else
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00e07620 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e07620 end
        end
        quest:SetStateBool("FollowInfoGiven", true)
    end
    while quest:GetStateInt("TradersStillAliveCounter") < 1 do
        if not quest:NewScriptFrame() then goto LAB_00e07620 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e07620 end
    if quest:GetStateBool("SavedInMiddle") then
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    end
    while not quest:IsActiveThreadTerminating() do
        if quest:GetStateBool("GoneWrongWay") then goto LAB_00e06b57 end
        if not quest:IsRegionLoaded("GreatwoodCaves") then goto LAB_00e06b57 end
        predicateResult = true
        goto FLOW_past_lab_00e06b57
        ::LAB_00e06b57::
        predicateResult = false
        ::FLOW_past_lab_00e06b57::
        if predicateResult then
            quest:DisplayGameInfo("TEXT_QST_067_INFO_GONE_WRONG_WAY")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00e0760e end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:SetStateBool("GoneWrongWay", true)
        end
        if quest:GetStateBool("ShownBalverine") then goto LAB_00e06c33 end
        if not quest:IsRegionLoaded("Darkwood2") then goto LAB_00e06c33 end
        predicateResult3 = true
        goto FLOW_past_lab_00e06c33
        ::LAB_00e06c33::
        predicateResult3 = false
        ::FLOW_past_lab_00e06c33::
        if predicateResult3 then
            local actorMap = resources:NewActorMap()
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_DARKWOOD_TRADER_BALVERINE", actorMap, false, true)
            quest:SetStateBool("ShownBalverine", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap)
        end
        if not quest:GetStateBool("EndStarted") then
            local isRegionLoaded = quest:IsRegionLoaded("BarrowFields")
            sequence = not isRegionLoaded
            if not sequence then
                -- TODO(native): cVar3 = (*xStack_dc[0x4b])()
    --[[unresolved native value]]
                sequence = nil ~= 0
            end
            if sequence then goto LAB_00e06d6c end
            predicateResult4 = true
        else
            goto LAB_00e06d6c
        end
        goto FLOW_past_lab_00e06d6c
        ::LAB_00e06d6c::
        predicateResult4 = false
        ::FLOW_past_lab_00e06d6c::
        if predicateResult4 then
            if not quest:IsActiveThreadTerminating() then
                local x_stk_d0_2 = quest:GetThingWithScriptName("M_EndTheQuestHere")
                -- TODO(native): cVar3 = (*xStack_dc[0x4b])()
    --[[unresolved native value]]
                if nil ~= 0 then goto LAB_00e06fb5 end
                if not quest:GetStateBool("EndStarted") then
                    if not quest:IsActiveThreadTerminating() then
                        ::LAB_00e06e73::
                        if quest:GetStateBool("EndStarted") then goto LAB_00e06eb0 end
                        if not quest:IsRegionLoaded("BarrowFields") then goto LAB_00e06eb0 end
                        predicateResult7 = true
                        goto FLOW_past_lab_00e06eb0
                        ::LAB_00e06eb0::
                        predicateResult7 = false
                        ::FLOW_past_lab_00e06eb0::
                        if predicateResult7 then
                            if not quest:NewScriptFrame() then break end
                            if quest:IsDistanceBetweenThingsUnder(hero, x_stk_d0_2, 4.0) then
                                local darkwoodTrader3 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
                                if (darkwoodTrader3 - darkwoodTrader3) / 12 == quest:GetStateInt("TradersStillAliveCounter") then
                                    quest:SetStateBool("EndStarted", true)
                                end
                            end
                            goto LAB_00e06e73
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e06fb5 end
                    end
                else
                    goto LAB_00e073cf
                end
            end
            goto FLOW_hoist_lab_00e073cf_2
        end
        goto FLOW_past_lab_00e073cf
        ::LAB_00e073cf::
        if not quest:IsActiveThreadTerminating() then
            quest:GiveHeroGold(#quest:GetAllThingsWithScriptName("DarkwoodTrader") * quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderGoldReward))
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName, true, false, false)
            quest:SetCreatureGeneratorsEnabled("Darkwood1", true)
            quest:SetCreatureGeneratorsEnabled(getActiveQuestName, true)
            quest:SetCreatureGeneratorsEnabled("Darkwood2", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood3", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood5", true)
            isRegionLoaded6 = quest:IsRegionLoaded("BarrowFields")
            goto LAB_00e07574
        end
        ::FLOW_hoist_lab_00e073cf_2::
        break
        ::FLOW_past_lab_00e073cf::
        ::LAB_00e06fb5::
        if quest:GetStateBool("EndStarted") then goto LAB_00e073cf end
        if quest:GetStateInt("TradersStillAliveCounter") == 0 then
            if quest:IsActiveThreadTerminating() then break end
            quest:SetCreatureGeneratorsEnabled("Darkwood1", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood2", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood3", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood5", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood6", true)
            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "TEXT_QST_067_QUEST_FAILED_TRADERS_DIED", true)
            quest:SetStateBool("MissionFailed", true)
            isRegionLoaded2 = quest:IsRegionLoaded("Darkwood1")
            if isRegionLoaded2 then
                -- TODO(native): iVar8 = &xStack_c0:IsAlive()
    --[[unresolved native value]]
                isRegionLoaded2 = nil
            end
            if isRegionLoaded2 then
                if not quest:IsActiveThreadTerminating() then
                    while darkwoodTrader4[0 + 1]:IsAlive() do
                        if not quest:NewScriptFrame() then goto LAB_00e07609 end
                    end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e07208 end
                end
                ::LAB_00e07609::
                break
            end
            ::LAB_00e07208::
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        end
        if not quest:GetStateBool("SavedInMiddle") and quest:MsgOnRegionLoaded() then
            if "" ~= nil and "" == "Darkwood4" then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetStateBool("SavedInMiddle", true)
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
                quest:AutoSaveCheckPoint()
            end
        end
        if not quest:GetStateBool("SavedNearEnd") and quest:MsgOnRegionLoaded() then
            if "" ~= nil and "" == "Darkwood6" then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetStateBool("SavedNearEnd", true)
                quest:AutoSaveCheckPoint()
            end
        end
        quest:NewScriptFrame()
    end
    goto LAB_00e0760e
    ::LAB_00e07574::
    if isRegionLoaded6 then
        if not quest:NewScriptFrame() then goto LAB_00e075fa end
        isRegionLoaded6 = quest:IsRegionLoaded("BarrowFields")
        goto LAB_00e07574
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e075fa end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    repeat
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
    ::LAB_00e075fa::
    ::LAB_00e0760e::
    ::LAB_00e07620::
end

-- Q_TraderEscort.TurnToBalv (retail 0x00e0a820)
function TurnToBalv(quest, me)
    local resources = quest:RetailResources()
    local pPosition, resource, movie, actorMap
    local resource2 = resources:NewResource()
    local resource3 = resources:NewResource()
    if me._4_4_ == 0 then
        pPosition = {x = 0, y = 0, z = 0}
    else
        pPosition = me:GetPos()
    end
    local balverine01 = quest:CreateCreature("CREATURE_BALVERINE_01", pPosition, "DarkwoodBalverineTrader")
    quest:EntitySetAsDrawable(balverine01, false)
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, balverine01, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource2)
            me = nil
            -- TODO(native): if (native_arg_Me._8_4_ ~= 0) and (*native_arg_Me._8_4_ = *native_arg_Me._8_4_ - 1, *native_arg_Me._8_4_ == 0) then
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0ad0a end
    resources:PrepareResource(resource2)
    -- TODO(native): bVar3 = resources:TryAcquire(xStack_30, &native_arg_Me, 4)
    while not nil --[[unresolved native value]] do
        if not quest:NewScriptFrame() then goto LAB_00e0ad0a end
        -- TODO(native): bVar3 = resources:TryAcquire(xStack_30, &native_arg_Me, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0ad0a end
    resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BALV", resource3)
    resources:SetActor(actorMap, "TRADER", resource2)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_DARKWOOD_TRADER_TRANSFORM", actorMap, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    quest:Pause(0.3)
    resources:PrepareResource(resource3)
    repeat
        local scratchValue = MakeTraderComment(quest, "TRANSFORMED", nil, 1)
        if scratchValue then
            break
        end
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
    ::LAB_00e0ad0a::
    resources:ReleaseResource(resource3)
    resources:ReleaseResource(resource2)
end

-- Q_TraderEscort.WatchForPickpocketing (retail 0x00e04f10)
function WatchForPickpocketing(quest, trader)
    local predicateResult, scratchValue
    local scratchValue4 = nil
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            trader = nil
            -- TODO(native): if (native_arg_Trader._8_4_ ~= 0) and (*native_arg_Trader._8_4_ = *native_arg_Trader._8_4_ - 1, *native_arg_Trader._8_4_ == 0) then
            return
        end
        if quest:MsgOnHeroPickedPocket() then
            if not quest:IsActiveThreadTerminating() then
                if scratchValue4 ~= nil and scratchValue4:IsEqualTo(trader._4_4_) then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateInt("TraderPickpocketedCount", quest:GetStateInt("TraderPickpocketedCount") + 1)
                    local predicateResult4 = quest:IsActiveThreadTerminating()
                    local sequence = not predicateResult4 and 2 < quest:GetStateInt("TraderPickpocketedCount") and not quest:IsActiveThreadTerminating()
                    if sequence then
                        quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 2)
                    end
                    return
                end
                goto LAB_00e04f80
            end
            goto LAB_00e050a5
        end
        goto FLOW_past_lab_00e050a5
        ::LAB_00e050a5::
        trader = nil
        -- TODO(native): if (native_arg_Trader._8_4_ == 0) or (*native_arg_Trader._8_4_ = *native_arg_Trader._8_4_ - 1, *native_arg_Trader._8_4_ ~= 0) then
        -- TODO(native): (**(code **)(native_arg_Trader._8_4_ + 4))();
        do __cleanup_LAB_00e05100(); return end
        ::FLOW_past_lab_00e050a5::
        ::LAB_00e04f80::
        if trader._4_4_ == 0 then
            scratchValue = 0
        else
            scratchValue = trader ~= nil and trader:MsgIsKilledBy("")
        end
        if not scratchValue then quest:NewScriptFrame(); predicateResult = quest:IsActiveThreadTerminating(); goto continue_1 end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 0)
        end
        goto LAB_00e050a5
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
end

-- Q_TraderEscort.MakeTraderComment (retail 0x00e01900)
function MakeTraderComment(quest, commentToMake, speaker, commentType)
    local scratchValue19, darkwoodTrader, scratchValue52
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    -- TODO(native): xStack_30._4_4_ = *(undefined4 *)(native_arg_speaker + 0x4);
    -- TODO(native): xStack_30 = *(int **)(native_arg_speaker + 0x8);
    if nil ~= nil then
        -- TODO(native): *xStack_30 = *xStack_30 + 1;
    end
    -- TODO(native): bVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)(xStack_30)
    --[[unresolved native value]]
    if not nil then
        if quest:IsActiveThreadTerminating() then
            return false
        end
        -- TODO(native): piVar13 = *(pCVar4 + 0x8)
    --[[unresolved native value]]
        -- TODO(native): piVar1 = *(pCVar4 + 0x4)
--[[unresolved native value]]
        if nil ~= nil then
            -- TODO(native): xStack_30._4_4_ = piVar1;
            if nil ~= nil then
                -- TODO(native): *piVar13 = *piVar13 + 1;
            end
        end
    end
    -- TODO(native): bVar2 = (**(xStack_30._0_4_ + 0x12c))(xStack_30)
    --[[unresolved native value]]
    if not nil then
        return false
    end
    if commentType ~= 2 then
        if 0 < quest:GetTimer(commentTimer) then
            return false
        end
        local conversationId = quest:AddNewConversation(nil, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        if not (nil ~= nil and not (nil):IsNull()) then
            speaker = ""
        else
            (nil):GetDataString()
        end
        local scratchValue33 = commentToMake
        quest:AddLineToConversation(conversationId, (("TEXT_QST_067_" .. speaker) .. "_") .. commentToMake, nil, hero, false)
        if commentType ~= 1 then goto LAB_00e02355 end
        if quest:IsActiveThreadTerminating() then return false end
        darkwoodTrader = quest:GetNearestWithScriptName(nil, "DarkwoodTrader")
        if not (darkwoodTrader ~= nil and darkwoodTrader:IsAlive()) or (darkwoodTrader ~= nil and darkwoodTrader:IsEqualTo((nil)._4_4_)) then
            goto LAB_00e02213
        else
            local scratchValue2 = quest:TextEntryExists(((("TEXT_QST_067_" .. darkwoodTrader:GetDataString()) .. "_") .. scratchValue33) .. "_RESPONSE")
            speaker = CONCAT31(speaker._1_3_,1)
            if not scratchValue2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        scratchValue52 = 31
        if 31 & 16 ~= 0 then
            scratchValue52 = 31 & 0xffffffef
        end
        if scratchValue52 & 8 ~= 0 then
            scratchValue52 = scratchValue52 & 0xfffffff7
        end
        if scratchValue52 & 4 ~= 0 then
            scratchValue52 = scratchValue52 & 0xfffffffb
        end
        if speaker ~= 0 then
            if quest:IsActiveThreadTerminating() then
                return false
            end
            quest:AddPersonToConversation(conversationId, darkwoodTrader)
            quest:AddLineToConversation(conversationId, ((("TEXT_QST_067_" .. darkwoodTrader:GetDataString()) .. "_") .. scratchValue33) .. "_RESPONSE", darkwoodTrader, hero, false)
        end
        ::LAB_00e02355::
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        return true
    end
    ::FLOW_after_lab_00e02355::
    if quest:IsActiveThreadTerminating() then return false end
    local darkwoodTrader8 = quest:GetNearestWithScriptName(nil, "DarkwoodTrader")
    local darkwoodTrader9 = quest:GetFurthestWithScriptName(nil, "DarkwoodTrader")
    if not (darkwoodTrader8 ~= nil and darkwoodTrader8:IsEqualTo(darkwoodTrader9._4_4_)) then
        goto LAB_00e01afb
    elseif not quest:IsActiveThreadTerminating() then
        -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_c);
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,iVar5);
        goto LAB_00e01afb
    end
    do return false end
    ::LAB_00e01afb::
    if darkwoodTrader8:GetDataString() ~= "SCARED" then
        if quest:IsActiveThreadTerminating() then return false end
        -- TODO(native): xStack_24 = xStack_18;
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,(int)&xStack_c);
    end
    local scratchValue36 = commentToMake
    if commentToMake ~= nil then
        local predicateResult = commentToMake == "ROCK_TROLL_CLOSE"
        commentToMake = CONCAT31(commentToMake._1_3_,predicateResult)
        if predicateResult then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    if quest:IsActiveThreadTerminating() then return false end
    quest:PlayCriteriaSoundOnThing(quest:GetThingWithScriptName("EARTH_TROLL_OFFSCREEN_ROAR"), scratchValue19)
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    local conversationId2 = quest:AddNewConversation(darkwoodTrader9, false, false)
    quest:AddPersonToConversation(conversationId2, hero)
    if not (darkwoodTrader8 ~= nil and darkwoodTrader8:IsAlive()) then
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    if not quest:IsActiveThreadTerminating() then
        quest:AddPersonToConversation(conversationId2, darkwoodTrader8)
        if not (darkwoodTrader9 ~= nil and darkwoodTrader9:IsAlive()) then
            if not quest:IsActiveThreadTerminating() then
                quest:AddLineToConversation(conversationId2, (("TEXT_QST_067_" .. (nil):GetDataString()) .. "_") .. scratchValue36, nil, hero, false)
                quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. darkwoodTrader8:GetDataString()) .. "_INTRO_RESPONSE_NO", darkwoodTrader8, hero, false)
                local scratchValue = nil
                quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. scratchValue:GetDataString()) .. "_INTRO_RESPONSE_REWARD", scratchValue, hero, false)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        elseif not quest:IsActiveThreadTerminating() then
            quest:AddPersonToConversation(conversationId2, darkwoodTrader9)
            quest:AddLineToConversation(conversationId2, (("TEXT_QST_067_" .. (nil):GetDataString()) .. "_") .. scratchValue36, nil, hero, false)
            quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. darkwoodTrader8:GetDataString()) .. "_INTRO_RESPONSE_NO", darkwoodTrader8, hero, false)
            quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. darkwoodTrader9:GetDataString()) .. "_INTRO_RESPONSE_OATH", darkwoodTrader9, hero, false)
            quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. darkwoodTrader8:GetDataString()) .. "_INTRO_RESPONSE_BAD_NEWS", darkwoodTrader8, hero, false)
            local scratchValue18 = nil
            quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. scratchValue18:GetDataString()) .. "_INTRO_RESPONSE_REWARD", scratchValue18, hero, false)
            quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
            return true
        end
        ::FLOW_after_lab_00e01fb0::
    end
    return false
end

