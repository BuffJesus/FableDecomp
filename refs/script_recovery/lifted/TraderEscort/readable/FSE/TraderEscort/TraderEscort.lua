-- Readable native conversion: Q_TraderEscort. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TraderEscortNakedBoastCost = 504,  -- 200
    TraderEscortNakedBoastReward = 508,  -- 400
    TraderEscortNoDamageBoastCost = 512,  -- 200
    TraderEscortNoDamageBoastReward = 516,  -- 1000
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
    local scratchValue
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
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_01", "Darkwood1", "Greatwood")
    quest:CreateThread("WatchForSurprisingBalverines")  -- native thread body CQ_TraderEscortScript::WatchForSurprisingBalverines: lift it as function WatchForSurprisingBalverines(quest)
    if not scratchValue then
    end
    quest:CreateThread("WatchForMissionRules")  -- native thread body 0x00E06440: lift it as function WatchForMissionRules(quest)
    if quest:GetStateBool("QuestStartScreened") then quest:IsActiveThreadTerminating(); return end
    while true do
        if not quest:NewScriptFrame() then break end
        if quest:HeroHasExpression("EXPRESSION_PICKPOCKET") then
            if quest:IsActiveThreadTerminating() then return end
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS", 10, quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastReward), true, getActiveQuestName, 0)
            quest:IsActiveThreadTerminating()
            return
        end
        if quest:GetStateBool("QuestStartScreened") then
            quest:IsActiveThreadTerminating()
            do return end
        end
    end
    do return end
    quest:IsActiveThreadTerminating()
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNakedBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNakedBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNoDamageBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNoDamageBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_ALLTRADERSALIVE", 9, quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastReward), false, "", 0)
end

-- Q_TraderEscort.OnPersist (retail 0x00e00ce0)
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

-- Q_TraderEscort.WatchForSurprisingBalverines (retail 0x00e06050)
function WatchForSurprisingBalverines(quest)
    quest:WithRetailResources(function(resources)
        local surprisePositions = resources:NewThingPositionList()
        local scratchValue, scratchValue2, retailRandModulo, position, scratchValue3, balverineEasy
        local scratchValue4
        local surpriseActorOwner, surpriseNearestOwner = 0, 0
        scratchValue = quest:IsActiveThreadTerminating()
        if not scratchValue then
            goto LAB_00e06074
        end
        ::FLOW_after_lab_00e06394::
        goto FLOW_past_lab_00e06074
        ::LAB_00e06074::
        if quest:GetStateInt("BalverinesToSurpriseHeroNeeded") < 1 then goto LAB_00e06373 end
        scratchValue = quest:IsActiveThreadTerminating()
        if not scratchValue then
            quest:Pause(2.0)
            surprisePositions:Clear()
            surprisePositions:FillScriptName("M_BalverineSurprise")
            scratchValue4 = 0
            if surprisePositions:Count() ~= 0 then
                retailRandModulo = 0
                repeat
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00e063db end
                    if not (0 < quest:GetStateInt("BalverinesToSurpriseHeroNeeded")) then scratchValue4 = scratchValue4 + 1; retailRandModulo = retailRandModulo + 12; goto continue_1 end
                    position = surprisePositions:GetPos(retailRandModulo / 12)
                    scratchValue = quest:IsCameraPosOnScreen(position)
                    if not scratchValue then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then
                            return
                        end
                        scratchValue = true
                        position = surprisePositions:GetPos(scratchValue4)
                        balverineEasy = quest:CreateCreature("CREATURE_BALVERINE_EASY", position, "SurpriseBalverine", scratchValue)
                        surpriseActorOwner = resources:TrackThingOutput(balverineEasy)
                        quest:EntitySetCutsceneBehaviour(balverineEasy, CUTSCENE_BEHAVIOUR_PAUSED)
                        scratchValue = false
                        scratchValue3 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(balverineEasy, scratchValue3, scratchValue)
                        quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                        retailRandModulo = 0
                        scratchValue3 = quest:GetNearestWithScriptName(balverineEasy, "DarkwoodTrader")
                        surpriseNearestOwner = resources:TrackThingOutput(scratchValue3)
                        MakeTraderComment(quest, "BALVERINE_SURPRISE", scratchValue3, retailRandModulo)
                        resources:RetireThingOutput(surpriseNearestOwner)
                        surpriseNearestOwner = 0
                        retailRandModulo = quest:RetailRandModulo(32768)
                        if retailRandModulo % 5 == 0 then
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then return end
                            scratchValue3 = quest:GetNearestWithScriptName(balverineEasy, "DarkwoodTrader")
                            surpriseNearestOwner = resources:TrackThingOutput(scratchValue3)
                            quest:GiveThingBestEnemyTarget(balverineEasy, scratchValue3)
                            resources:RetireThingOutput(surpriseNearestOwner)
                            surpriseNearestOwner = 0
                        else
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then return end
                            scratchValue3 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(balverineEasy, scratchValue3)
                        end
                        scratchValue2 = balverineEasy ~= nil and balverineEasy:IsAlive()
                        goto LAB_00e06319
                    end
                    scratchValue4 = scratchValue4 + 1
                    retailRandModulo = retailRandModulo + 12
                    ::continue_1::
                until not (scratchValue4 < surprisePositions:Count())
            end
            goto LAB_00e06364
        end
        ::FLOW_past_lab_00e06074::
        ::LAB_00e0642a::
        do return end
        ::LAB_00e063db::
        goto LAB_00e0642a
        ::LAB_00e06319::
        if scratchValue2 then
            quest:NewScriptFrame()
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then return end
            scratchValue2 = balverineEasy ~= nil and balverineEasy:IsAlive()
            goto LAB_00e06319
        end
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            return
        end
        quest:SetStateInt("BalverinesToSurpriseHeroNeeded", quest:GetStateInt("BalverinesToSurpriseHeroNeeded") - 1)
        resources:RetireThingOutput(surpriseActorOwner)
        surpriseActorOwner = 0
        ::LAB_00e06364::
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then goto LAB_00e06415 end
        ::LAB_00e06373::
        quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            goto FLOW_after_lab_00e06394
        end
        goto LAB_00e06074
        ::LAB_00e06415::
        goto LAB_00e0642a
    end)
end

-- Q_TraderEscort.WatchForMissionRules (retail 0x00e06440)
function WatchForMissionRules(quest)
    local resources = quest:RetailResources()
    local isRegionLoaded, darkwoodTrader4, endTrader
    if not quest:GetStateBool("IntroFinished") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("SavedInMiddle", false)
        while not quest:IsRegionLoaded("Darkwood1") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        local mkDteIntroTrader1 = quest:GetThingWithScriptName("MK_DTE_INTRO_TRADER1")
        local darkwoodTrader = quest:GetNearestWithScriptName(mkDteIntroTrader1, "DarkwoodTrader")
        local darkwoodTrader2 = quest:GetNearestWithScriptName(darkwoodTrader, "DarkwoodTrader")
        local resource3 = resources:NewResource()
        local resource = resources:NewResource()
        local resource2 = resources:NewResource()
        local hero = quest:GetHero()
        resources:TryAcquire(resource2, hero, 4)
        resources:TryAcquire(resource3, darkwoodTrader, 4)
        resources:TryAcquire(resource, darkwoodTrader2, 4)
        local actorMap2 = resources:NewActorMap()
        resources:SetActor(actorMap2, "HERO", resource2)
        resources:SetActor(actorMap2, "TRADER1", resource3)
        resources:SetActor(actorMap2, "TRADER2", resource)
        local movie = resources:StartMovie("")
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
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_02", "Darkwood4", "Greatwood")
    if not quest:GetStateBool("QuestStartScreened") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("IntroFinished", true)
        local getActiveQuestName4 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(getActiveQuestName4, true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    if not quest:GetStateBool("FollowInfoGiven") then
        if quest:IsActiveThreadTerminating() then return end
        if quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then return end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        else
            if quest:IsActiveThreadTerminating() then return end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        end
        quest:SetStateBool("FollowInfoGiven", true)
    end
    while quest:GetStateInt("TradersStillAliveCounter") < 1 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    endTrader = nil
    if quest:GetStateBool("SavedInMiddle") then
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName2 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    end
    while not quest:IsActiveThreadTerminating() do
        if not (quest:GetStateBool("GoneWrongWay") or (not quest:IsRegionLoaded("GreatwoodCaves"))) then
            if quest:IsActiveThreadTerminating() then break end
            quest:DisplayGameInfo("TEXT_QST_067_INFO_GONE_WRONG_WAY")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:SetStateBool("GoneWrongWay", true)
        end
        if not (quest:GetStateBool("ShownBalverine") or (not quest:IsRegionLoaded("Darkwood2"))) then
            if quest:IsActiveThreadTerminating() then break end
            local actorMap = resources:NewActorMap()
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_DARKWOOD_TRADER_BALVERINE", actorMap, false, true)
            quest:SetStateBool("ShownBalverine", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap)
        end
        if not (quest:GetStateBool("EndStarted") or (not quest:IsRegionLoaded("BarrowFields") or (endTrader ~= nil and endTrader:IsAlive()))) then
            if not quest:IsActiveThreadTerminating() then
                endTrader = quest:GetThingWithScriptName("EndTrader")
                local x_stk_d0_2 = quest:GetThingWithScriptName("M_EndTheQuestHere")
                if endTrader ~= nil and endTrader:IsAlive() then goto FLOW_past_lab_00e073cf end
                if not quest:GetStateBool("EndStarted") then
                    if not quest:IsActiveThreadTerminating() then
                        ::LAB_00e06e73::
                        if not (quest:GetStateBool("EndStarted") or (not quest:IsRegionLoaded("BarrowFields"))) then
                            if not quest:NewScriptFrame() then break end
                            local hero2 = quest:GetHero()
                            if quest:IsDistanceBetweenThingsUnder(hero2, x_stk_d0_2, 4.0) then
                                if quest:IsActiveThreadTerminating() then break end
                                if #quest:GetAllThingsWithScriptName("DarkwoodTrader") == quest:GetStateInt("TradersStillAliveCounter") then
                                    if quest:IsActiveThreadTerminating() then break end
                                    quest:SetStateBool("EndStarted", true)
                                end
                            end
                            goto LAB_00e06e73
                        end
                        if not quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e073cf end
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
            darkwoodTrader4 = quest:GetAllThingsWithScriptName("DarkwoodTrader")
            quest:GiveHeroGold((#darkwoodTrader4) * quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderGoldReward))
            local getActiveQuestName5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName5, true, false, false)
            quest:SetCreatureGeneratorsEnabled("Darkwood1", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood2", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood3", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood5", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood6", true)
            isRegionLoaded = quest:IsRegionLoaded("BarrowFields")
            goto LAB_00e07574
        end
        ::FLOW_hoist_lab_00e073cf_2::
        break
        ::FLOW_past_lab_00e073cf::
        if quest:GetStateBool("EndStarted") then goto LAB_00e073cf end
        if quest:GetStateInt("TradersStillAliveCounter") == 0 then
            if quest:IsActiveThreadTerminating() then break end
            quest:SetCreatureGeneratorsEnabled("Darkwood1", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood2", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood3", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood5", true)
            quest:SetCreatureGeneratorsEnabled("Darkwood6", true)
            local getActiveQuestName6 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(getActiveQuestName6, true, "TEXT_QST_067_QUEST_FAILED_TRADERS_DIED", true)
            quest:SetStateBool("MissionFailed", true)
            if quest:IsRegionLoaded("Darkwood1") and darkwoodTrader4:IsAlive() then
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
            local getActiveQuestName7 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(getActiveQuestName7, 0)
        end
        if not quest:GetStateBool("SavedInMiddle") and quest:MsgOnRegionLoaded() then
            if "" ~= nil and "" == "Darkwood4" then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetStateBool("SavedInMiddle", true)
                local getActiveQuestName3 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(getActiveQuestName3, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
                quest:AutoSaveCheckPoint()
            end
        end
        if not (not quest:GetStateBool("SavedNearEnd") and quest:MsgOnRegionLoaded()) then
            quest:NewScriptFrame()
        elseif not (("" ~= nil) and ("" == "Darkwood6")) then
            quest:NewScriptFrame()
        else
            if quest:IsActiveThreadTerminating() then break end
            quest:SetStateBool("SavedNearEnd", true)
            quest:AutoSaveCheckPoint()
            quest:NewScriptFrame()
        end
    end
    do return end
    ::LAB_00e07574::
    if isRegionLoaded then
        if not quest:NewScriptFrame() then return end
        isRegionLoaded = quest:IsRegionLoaded("BarrowFields")
        goto LAB_00e07574
    end
    if quest:IsActiveThreadTerminating() then return end
    local getActiveQuestName8 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName8, 0)
    repeat
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
end

-- Q_TraderEscort.TurnToBalv (retail 0x00e0a820)
function TurnToBalv(quest, me)
    local resources = quest:RetailResources()
    local pPosition, balverine01, actorMap
    local resource2 = resources:NewResource()
    local resource3 = resources:NewResource()
    if not (me ~= nil and not me:IsNull()) then
        pPosition = {x = 0, y = 0, z = 0}
    else
        pPosition = me:GetPos()
    end
    balverine01 = quest:CreateCreature("CREATURE_BALVERINE_01", pPosition, "DarkwoodBalverineTrader", false)
    quest:EntitySetAsDrawable(balverine01, false)
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, balverine01, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            balverine01 = nil
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); resources:ReleaseResource(resource2); return end
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource3); resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BALV", resource3)
    resources:SetActor(actorMap, "TRADER", resource2)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_DARKWOOD_TRADER_TRANSFORM", actorMap, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    quest:Pause(0.3)
    resources:PrepareResource(resource3)
    repeat
        actorMap = nil
        local scratchValue = MakeTraderComment(quest, "TRANSFORMED", actorMap, 1)
        actorMap = nil
        if not scratchValue then
            quest:NewScriptFrame()
        else
            quest:IsActiveThreadTerminating()
            break
            quest:NewScriptFrame()
        end
    until quest:IsActiveThreadTerminating()
    resources:ReleaseResource(resource3)
    resources:ReleaseResource(resource2)
end

-- Q_TraderEscort.WatchForPickpocketing (retail 0x00e04f10)
function WatchForPickpocketing(quest, trader)
    local predicateResult, msgOnHeroPickedPocket
    msgOnHeroPickedPocket = nil
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            msgOnHeroPickedPocket = nil
            return
        end
        msgOnHeroPickedPocket = quest:MsgOnHeroPickedPocket()
        if msgOnHeroPickedPocket ~= nil then
            if quest:IsActiveThreadTerminating() then msgOnHeroPickedPocket = nil; return end
            if msgOnHeroPickedPocket ~= nil and msgOnHeroPickedPocket:IsEqualTo(trader) then
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
            msgOnHeroPickedPocket = nil
            return
        end
        ::LAB_00e04f80::
        local scratchValue = (trader ~= nil and not trader:IsNull()) and trader ~= nil and trader:MsgIsKilledBy("")
        if not scratchValue then
            quest:NewScriptFrame();
            predicateResult = quest:IsActiveThreadTerminating();
        else
            if not quest:IsActiveThreadTerminating() then
                quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 0)
            end
            msgOnHeroPickedPocket = nil
            do return end
            quest:NewScriptFrame()
            predicateResult = quest:IsActiveThreadTerminating()
        end
        ::continue_1::
    until false
end

-- Q_TraderEscort.MakeTraderComment (retail 0x00e01900)
function MakeTraderComment(quest, commentToMake, speaker, commentType)
    local commentTimer = quest:GetStateInt("CommentTimer")
    return quest:WithRetailResources(function(resources)
    local speakerOwner, nearestOwner, furthestOwner, fallbackOwner, swapOwner, triggerOwner = 0, 0, 0, 0, 0, 0
    local scratchValue, scratchValue2, retailImplementationIsEqualTo, getTimer, conversationId
    local local_furthest_handle, native_reply_available, native_rock_troll_match, getDataString
    local scratchValue3, getHero, getDataString2, getDataString3, scratchValue4, scratchValue5
    local predicateResult
    local native_conversation_speaker = speaker
    local scratchValue7 = resources:CopyThingOutput(speaker)
    speakerOwner = resources:TrackThingOutput(scratchValue7)
    scratchValue2 = scratchValue7 ~= nil and scratchValue7:IsAlive()
    if not scratchValue2 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            return false
        end
        scratchValue3 = quest:GetHero()
        scratchValue3 = quest:GetNearestWithScriptName(scratchValue3, "DarkwoodTrader")
        fallbackOwner = resources:TrackThingOutput(scratchValue3)
        resources:AssignThingOutput(speakerOwner, scratchValue3)
        resources:RetireThingOutput(fallbackOwner)
    end
    scratchValue2 = scratchValue7 ~= nil and scratchValue7:IsAlive()
    if not scratchValue2 then
        quest:IsActiveThreadTerminating()
        return false
    end
    if commentType ~= 2 then
        getTimer = quest:GetTimer(commentTimer)
        if 0 < getTimer then
            quest:IsActiveThreadTerminating()
            return false
        end
        conversationId = quest:AddNewConversation(scratchValue7, false, false)
        scratchValue3 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, scratchValue3)
        if not (scratchValue7 ~= nil and not scratchValue7:IsNull()) then
            speaker = ""
        else
            speaker = scratchValue7:GetDataString()
        end
        getHero = quest:GetHero()
        getDataString3 = commentToMake
        scratchValue3 = scratchValue7
        predicateResult = false
        getDataString2 = commentToMake
        getDataString = "TEXT_QST_067_" .. speaker
        getDataString = getDataString .. "_"
        getDataString2 = getDataString .. getDataString2
        quest:AddLineToConversation(conversationId, getDataString2, scratchValue3, getHero, predicateResult)
        if commentType == 1 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then return false end
            speaker = "DarkwoodTrader"
            local_furthest_handle = quest:GetNearestWithScriptName(scratchValue7, speaker)
            if local_furthest_handle == nil then local_furthest_handle = resources:CopyThingOutput(nil) end
            furthestOwner = resources:TrackThingOutput(local_furthest_handle)
            getTimer = local_furthest_handle ~= nil and local_furthest_handle:IsAlive()
            scratchValue = not getTimer
            if not scratchValue then
                retailImplementationIsEqualTo = local_furthest_handle:RetailImplementationIsEqualTo(scratchValue7)
                scratchValue = retailImplementationIsEqualTo
            end
            if scratchValue then
                native_reply_available = 0
            else
                scratchValue4 = "_RESPONSE"
                getDataString2 = getDataString3
                getDataString = local_furthest_handle:GetDataString()
                getDataString = "TEXT_QST_067_" .. getDataString
                getDataString = getDataString .. "_"
                getDataString2 = getDataString .. getDataString2
                getDataString2 = getDataString2 .. scratchValue4
                scratchValue2 = quest:TextEntryExists(getDataString2)
                native_reply_available = 1
                if not scratchValue2 then native_reply_available = 0 end
            end
            if native_reply_available ~= 0 then
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    return false
                end
                quest:AddPersonToConversation(conversationId, local_furthest_handle)
                getHero = quest:GetHero()
                scratchValue3 = local_furthest_handle
                predicateResult = false
                scratchValue4 = "_RESPONSE"
                getDataString2 = local_furthest_handle:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
            end
        end
        resources:RetireThingOutput(furthestOwner)
        furthestOwner = 0
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        return true
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then return false end
    commentType = "DarkwoodTrader"
    local local_nearest_handle = quest:GetNearestWithScriptName(scratchValue7, commentType)
    if local_nearest_handle == nil then local_nearest_handle = resources:CopyThingOutput(nil) end
    nearestOwner = resources:TrackThingOutput(local_nearest_handle)
    commentType = "DarkwoodTrader"
    local_furthest_handle = quest:GetFurthestWithScriptName(scratchValue7, commentType)
    if local_furthest_handle == nil then local_furthest_handle = resources:CopyThingOutput(nil) end
    furthestOwner = resources:TrackThingOutput(local_furthest_handle)
    retailImplementationIsEqualTo = local_nearest_handle:RetailImplementationIsEqualTo(local_furthest_handle)
    if retailImplementationIsEqualTo then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then return false end
        resources:AssignThingOutput(furthestOwner, nil)
    end
    scratchValue4 = "SCARED"
    local this_00 = local_nearest_handle:GetDataString()
    getTimer = this_00 ~= scratchValue4 and 1 or 0
    if getTimer ~= 0 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then return false end
        local native_swap_handle = resources:CopyThingOutput(local_nearest_handle)
        swapOwner = resources:TrackThingOutput(native_swap_handle)
        resources:AssignThingOutput(nearestOwner, local_furthest_handle)
        resources:AssignThingOutput(furthestOwner, native_swap_handle)
        resources:RetireThingOutput(swapOwner)
    end
    getDataString3 = commentToMake
    if commentToMake == nil then
        scratchValue2 = false
        if scratchValue2 then
            goto LAB_00e01ba2
        end
    else
        getTimer = commentToMake == "ROCK_TROLL_CLOSE" and 0 or 1
        retailImplementationIsEqualTo = getTimer == 0
        native_rock_troll_match = retailImplementationIsEqualTo
        if retailImplementationIsEqualTo then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then return false end
    commentType = "EARTH_TROLL_OFFSCREEN_ROAR"
    commentToMake = "RockTrollTrigger"
    scratchValue3 = quest:GetThingWithScriptName(commentToMake)
    triggerOwner = resources:TrackThingOutput(scratchValue3)
    scratchValue5 = quest:PlayCriteriaSoundOnThing(scratchValue3, commentType)
    resources:RetireThingOutput(triggerOwner)
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    conversationId = quest:AddNewConversation(native_conversation_speaker, false, false)
    scratchValue3 = quest:GetHero()
    quest:AddPersonToConversation(conversationId, scratchValue3)
    getTimer = local_nearest_handle ~= nil and local_nearest_handle:IsAlive()
    if not getTimer then
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        do return true end
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if not scratchValue2 then
        quest:AddPersonToConversation(conversationId, local_nearest_handle)
        getTimer = local_furthest_handle ~= nil and local_furthest_handle:IsAlive()
        if not getTimer then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                getHero = quest:GetHero()
                scratchValue3 = scratchValue7
                predicateResult = false
                getDataString2 = scratchValue7:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_nearest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_NO"
                getDataString3 = local_nearest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                scratchValue3 = quest:GetHero()
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_REWARD"
                getHero = scratchValue7
                getDataString3 = getHero:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, getHero, scratchValue3, predicateResult)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                do return true end
            end
        else
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                quest:AddPersonToConversation(conversationId, local_furthest_handle)
                getHero = quest:GetHero()
                scratchValue3 = scratchValue7
                predicateResult = false
                getDataString2 = scratchValue7:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_nearest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_NO"
                getDataString3 = local_nearest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_furthest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_OATH"
                getDataString3 = local_furthest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_nearest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_BAD_NEWS"
                getDataString3 = local_nearest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                scratchValue3 = quest:GetHero()
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_REWARD"
                getHero = scratchValue7
                getDataString3 = getHero:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, getHero, scratchValue3, predicateResult)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                return true
            end
        end
    end
    return false
    end)
end

