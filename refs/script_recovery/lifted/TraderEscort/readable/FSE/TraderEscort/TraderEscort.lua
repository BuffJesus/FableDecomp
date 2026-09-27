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
    if quest:GetStateBool("QuestStartScreened") then return end
    while true do
        if not quest:NewScriptFrame() then break end
        if quest:HeroHasExpression("EXPRESSION_PICKPOCKET") then
            if quest:IsActiveThreadTerminating() then return end
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS", 10, quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TE_AllTradersAliveBoastReward), true, getActiveQuestName, 0)
            return
        end
        if quest:GetStateBool("QuestStartScreened") then
            return
        end
    end
    do return end
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
    local scratchValue, scratchValue2, balverineEasy, scratchValue6
    local hero = quest:GetHero()
    if not quest:IsActiveThreadTerminating() then goto LAB_00e06074 end
    ::FLOW_after_lab_00e06394::
    goto FLOW_past_lab_00e06074
    ::LAB_00e06074::
    if quest:GetStateInt("BalverinesToSurpriseHeroNeeded") < 1 then goto LAB_00e06373 end
    if not quest:IsActiveThreadTerminating() then
        quest:Pause(2.0)
        -- TODO(native): Std_Vector_Erase_Range(&xStack_30,(int)xStack_30,(int)x_stk_2c);
        local balverineSurprise = quest:GetAllThingsWithScriptName("M_BalverineSurprise")
        scratchValue6 = 0
        if #balverineSurprise ~= 0 then
            scratchValue2 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00e063db end
                if 0 >= quest:GetStateInt("BalverinesToSurpriseHeroNeeded") then scratchValue6 = scratchValue6 + 1; scratchValue2 = scratchValue2 + 1; goto continue_1 end
                if not quest:IsCameraPosOnScreen(balverineSurprise[scratchValue2 + 1]:GetPos()) then
                    if quest:IsActiveThreadTerminating() then return end
                    balverineEasy = quest:CreateCreature("CREATURE_BALVERINE_EASY", balverineSurprise[scratchValue6 + 1]:GetPos(), "SurpriseBalverine")
                    quest:EntitySetCutsceneBehaviour(balverineEasy, CUTSCENE_BEHAVIOUR_PAUSED)
                    quest:EntitySetFacingAngleTowardsThing(balverineEasy, hero, false)
                    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                    MakeTraderComment(quest, "BALVERINE_SURPRISE", quest:GetNearestWithScriptName(balverineEasy, "DarkwoodTrader"), 0)
                    if math.random(0, 32767) % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00e063f5(); return end
                        quest:GiveThingBestEnemyTarget(balverineEasy, quest:GetNearestWithScriptName(balverineEasy, "DarkwoodTrader"))
                    else
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00e063f5(); return end
                        quest:GiveThingBestEnemyTarget(balverineEasy, hero)
                    end
                    scratchValue = balverineEasy ~= nil and balverineEasy:IsAlive()
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
    if scratchValue then
        if not quest:NewScriptFrame() then __cleanup_LAB_00e063f5(); return end
        scratchValue = balverineEasy ~= nil and balverineEasy:IsAlive()
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
    local isRegionLoaded, darkwoodTrader4, endTrader
    if not quest:GetStateBool("IntroFinished") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("SavedInMiddle", false)
        while not quest:IsRegionLoaded("Darkwood1") do
            if not quest:NewScriptFrame() then return end
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
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("IntroFinished", true)
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    if not quest:GetStateBool("FollowInfoGiven") then
        if quest:IsActiveThreadTerminating() then return end
        if quest:IsXbox() then
            quest:DisplayGameInfo("TEXT_QST_067_INFO_USING_FOLLOW_WAIT")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        else
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
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    end
    while not quest:IsActiveThreadTerminating() do
        if not (quest:GetStateBool("GoneWrongWay") or (not quest:IsRegionLoaded("GreatwoodCaves"))) then
            quest:DisplayGameInfo("TEXT_QST_067_INFO_GONE_WRONG_WAY")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:SetStateBool("GoneWrongWay", true)
        end
        if not (quest:GetStateBool("ShownBalverine") or (not quest:IsRegionLoaded("Darkwood2"))) then
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
                if endTrader ~= nil and endTrader:IsAlive() then goto LAB_00e06fb5 end
                if not quest:GetStateBool("EndStarted") then
                    if not quest:IsActiveThreadTerminating() then
                        ::LAB_00e06e73::
                        if not (quest:GetStateBool("EndStarted") or (not quest:IsRegionLoaded("BarrowFields"))) then
                            if not quest:NewScriptFrame() then break end
                            if quest:IsDistanceBetweenThingsUnder(hero, x_stk_d0_2, 4.0) then
                                if #quest:GetAllThingsWithScriptName("DarkwoodTrader") == quest:GetStateInt("TradersStillAliveCounter") then
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
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
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
        if not (not quest:GetStateBool("SavedNearEnd") and quest:MsgOnRegionLoaded()) then
            quest:NewScriptFrame()
        elseif not ("" ~= nil and "" == "Darkwood6") then
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
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    repeat
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
end

-- Q_TraderEscort.TurnToBalv (retail 0x00e0a820)
function TurnToBalv(quest, me)
    local resources = quest:RetailResources()
    local pPosition
    local resource2 = resources:NewResource()
    local resource3 = resources:NewResource()
    if not (me ~= nil and not me:IsNull()) then
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
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); resources:ReleaseResource(resource2); return end
    resources:PrepareResource(resource2)
    -- TODO(native): bVar3 = resources:TryAcquire(xStack_30, &native_arg_Me, 4)
    while not nil --[[unresolved native value]] do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource3); resources:ReleaseResource(resource2); return end
        -- TODO(native): bVar3 = resources:TryAcquire(xStack_30, &native_arg_Me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap = resources:NewActorMap()
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
        local scratchValue = MakeTraderComment(quest, "TRANSFORMED", nil, 1)
        if scratchValue then
            break
        end
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
    resources:ReleaseResource(resource3)
    resources:ReleaseResource(resource2)
end

-- Q_TraderEscort.WatchForPickpocketing (retail 0x00e04f10)
function WatchForPickpocketing(quest, trader)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local scratchValue = quest:MsgOnHeroPickedPocket()
        if scratchValue ~= nil then
            if scratchValue ~= nil and scratchValue:IsEqualTo(trader) then
                quest:SetStateInt("TraderPickpocketedCount", quest:GetStateInt("TraderPickpocketedCount") + 1)
                if 2 < quest:GetStateInt("TraderPickpocketedCount") and not quest:IsActiveThreadTerminating() then
                    quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 2)
                end
                return
            end
            goto LAB_00e04f80
            return
        end
        ::LAB_00e04f80::
        if not ((trader ~= nil and not trader:IsNull()) and trader ~= nil and trader:MsgIsKilledBy("")) then
            quest:NewScriptFrame()
        else
            quest:SetMasterGameState("DarkwoodPickpocketedAllTraders", 0)
            do return end
            quest:NewScriptFrame()
        end
    until false
end

-- Q_TraderEscort.MakeTraderComment (retail 0x00e01900)
function MakeTraderComment(quest, commentToMake, speaker, commentType)
    local darkwoodTrader, scratchValue51
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
        local scratchValue32 = commentToMake
        quest:AddLineToConversation(conversationId, (("TEXT_QST_067_" .. speaker) .. "_") .. commentToMake, nil, hero, false)
        if commentType ~= 1 then goto LAB_00e02355 end
        if quest:IsActiveThreadTerminating() then return false end
        darkwoodTrader = quest:GetNearestWithScriptName(nil, "DarkwoodTrader")
        if not (darkwoodTrader ~= nil and darkwoodTrader:IsAlive()) or (darkwoodTrader ~= nil and darkwoodTrader:IsEqualTo((nil)._4_4_)) then
            goto LAB_00e02213
        else
            local scratchValue2 = quest:TextEntryExists(((("TEXT_QST_067_" .. darkwoodTrader:GetDataString()) .. "_") .. scratchValue32) .. "_RESPONSE")
            speaker = CONCAT31(speaker._1_3_,1)
            if not scratchValue2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        scratchValue51 = 31
        if 31 & 16 ~= 0 then
            scratchValue51 = 31 & 0xffffffef
        end
        if scratchValue51 & 8 ~= 0 then
            scratchValue51 = scratchValue51 & 0xfffffff7
        end
        if scratchValue51 & 4 ~= 0 then
            scratchValue51 = scratchValue51 & 0xfffffffb
        end
        if speaker ~= 0 then
            if quest:IsActiveThreadTerminating() then
                return false
            end
            quest:AddPersonToConversation(conversationId, darkwoodTrader)
            quest:AddLineToConversation(conversationId, ((("TEXT_QST_067_" .. darkwoodTrader:GetDataString()) .. "_") .. scratchValue32) .. "_RESPONSE", darkwoodTrader, hero, false)
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
    local scratchValue35 = commentToMake
    if commentToMake ~= nil then
        local predicateResult = commentToMake == "ROCK_TROLL_CLOSE"
        commentToMake = CONCAT31(commentToMake._1_3_,predicateResult)
        if predicateResult then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    if quest:IsActiveThreadTerminating() then return false end
    quest:PlayCriteriaSoundOnThing(quest:GetThingWithScriptName("RockTrollTrigger"), "EARTH_TROLL_OFFSCREEN_ROAR")
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
                quest:AddLineToConversation(conversationId2, (("TEXT_QST_067_" .. (nil):GetDataString()) .. "_") .. scratchValue35, nil, hero, false)
                quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. darkwoodTrader8:GetDataString()) .. "_INTRO_RESPONSE_NO", darkwoodTrader8, hero, false)
                local scratchValue = nil
                quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. scratchValue:GetDataString()) .. "_INTRO_RESPONSE_REWARD", scratchValue, hero, false)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        elseif not quest:IsActiveThreadTerminating() then
            quest:AddPersonToConversation(conversationId2, darkwoodTrader9)
            quest:AddLineToConversation(conversationId2, (("TEXT_QST_067_" .. (nil):GetDataString()) .. "_") .. scratchValue35, nil, hero, false)
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

