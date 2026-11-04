-- Readable native conversion: Q_TraderConflictGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TraderConflictGoodNakedBoastCost = 480,  -- 100
    TraderConflictGoodNakedBoastReward = 484,  -- 400
    TraderConflictGoodNoDamageBoastCost = 488,  -- 300
    TraderConflictGoodNoDamageBoastReward = 492,  -- 1000
    TraderConflictGoodNoWeaponsBoastCost = 496,  -- 300
    TraderConflictGoodNoWeaponsBoastReward = 500,  -- 1000
    TCGKillNoBanditsCost = 568,  -- 400
    TCGKillNoBanditsReward = 572,  -- 1000
    TCGMadeTimeLimitCost = 576,  -- 400
    TCGMadeTimeLimitReward = 580,  -- 1000
    TCG_TimeLimit = 3972,  -- 480
}

local function __native_all_dead(list)
    local down = 0
    for _, thing in ipairs(list) do
        if not thing:IsAlive() or thing:IsUnconscious() then down = down + 1 end
    end
    return down == #list
end

-- Q_TraderConflictGood.Main (retail 0x00dfa450)
function Main(quest)
    local allCreaturesIndex, scratchValue
    scratchValue = 0
    quest:AddEntityBinding("TraderToRescue", "TraderConflictGood/Entities/TraderToRescue", 1)
    quest:AddEntityBinding("TC_BanditGuard", "TraderConflictGood/Entities/TC_BanditGuard", 1)
    quest:AddEntityBinding("TC_BanditHostageKeeper", "TraderConflictGood/Entities/TC_BanditHostageKeeper", 1)
    quest:AddEntityBinding("BCMTrader", "TraderConflictGood/Entities/BCMTrader")
    quest:AddEntityBinding("BCGameMaster", "TraderConflictGood/Entities/BCGameMaster")
    quest:AddEntityBinding("BanditExtra", "TraderConflictGood/Entities/BanditExtra")
    quest:AddEntityBinding("CampHostageDoor", "TraderConflictGood/Entities/CampHostageDoor", 1)
    quest:FinalizeEntityBindings()
    while not quest:IsRegionLoaded("BanditCampEntrance") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local gate1 = quest:GetThingWithScriptName("Gate1")
    quest:OpenDoor(gate1)
    local gate2Outer = quest:GetThingWithScriptName("Gate2Outer")
    quest:OpenDoor(gate2Outer)
    helper_DFDED0(quest, "CS_TRADERCON_GOOD_INTRO")
    quest:SetStateBool("IntroDone", true)
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:KickOffQuestStartScreen(getActiveQuestName, true, false)
    quest:FadeScreenIn()
    quest:CreateThread("WatchTimeLimit")  -- native thread body CQ_TraderConflictEvilScript::WatchTimeLimit: lift it as function WatchTimeLimit(quest)
    UpdateLiveEnemies(quest)
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_01", "BanditCampCentre", "BanditCampMain")
    if quest:GetStateListCount("AllCreatures") ~= 0 then
        allCreaturesIndex = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", allCreaturesIndex), 10.0)
            quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", allCreaturesIndex), false)
            quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", allCreaturesIndex), "FACTION_BANDITS")
            scratchValue = scratchValue + 1
            allCreaturesIndex = allCreaturesIndex + 1
        until not (scratchValue < quest:GetStateListCount("AllCreatures"))
    end
    local predicateResult = quest:IsActiveThreadTerminating()
    if predicateResult then return end
    quest:CreateThread("WatchForRegionTransitions")  -- native thread body WatchForRegionTransitions: lift it as function WatchForRegionTransitions(quest)
    if not predicateResult then
    end
    quest:CreateThread("WatchForHittingEnemies")  -- native thread body WatchForHittingEnemies: lift it as function WatchForHittingEnemies(quest)
    if not predicateResult then
    end
    quest:CreateThread("WatchForTradersFreed")  -- native thread body 0x00DFCC10: lift it as function WatchForTradersFreed(quest)
    if not predicateResult then
    end
    quest:CreateThread("WatchForKilledPeople")  -- native thread body WatchForKilledPeople: lift it as function WatchForKilledPeople(quest)
    if not predicateResult then
    end
    while not quest:IsRegionLoaded("BanditCampCentre") do
        if not quest:NewScriptFrame() then return end
        if not quest:GetStateBool("FirstRegionDead") and quest:GetStateListCount("AllCreatures") == 0 then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("FirstRegionDead", true)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    local gate2Inner = quest:GetThingWithScriptName("Gate2Inner")
    quest:OpenDoor(gate2Inner)
    while not quest:GetStateBool("OutroDone") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:SetQuestAsCompleted(getActiveQuestName2, true, true, false)
    while quest:IsRegionLoaded("BanditCampEntrance") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
    local getActiveQuestName3 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName3, 0)
end

-- Q_TraderConflictGood.Init (retail 0x00dfa0e0)
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictGoodNakedBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictGoodNakedBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictGoodNoDamageBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictGoodNoDamageBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictGoodNoWeaponsBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictGoodNoWeaponsBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE", 34, quest:ReadGlobalGameData(SCRIPT_DEF.TCGKillNoBanditsCost), quest:ReadGlobalGameData(SCRIPT_DEF.TCGKillNoBanditsReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT", 35, quest:ReadGlobalGameData(SCRIPT_DEF.TCGMadeTimeLimitCost), quest:ReadGlobalGameData(SCRIPT_DEF.TCGMadeTimeLimitReward), false, "", 0)
    quest:SetMasterGameState("TCGKillNoBandits", true)
    quest:SetMasterGameState("TCGMadeTimeLimit", false)
end

-- Q_TraderConflictGood.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Q_TraderConflictGood.WatchTimeLimit (retail 0x00dfaeb0)
function WatchTimeLimit(quest)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    while not quest:GetMasterGameState("TCGTimeLimitBoastTaken") do
        if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        if quest:GetTimer(timerId) == 0 then
            quest:IsActiveThreadTerminating()
            quest:DeregisterTimer(timerId)
            do return end
        end
    end
    quest:SetTimer(timerId, quest:ReadGlobalGameData(SCRIPT_DEF.TCG_TimeLimit))
    local infoElement = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
    quest:DisplayQuestInfo(true)
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then goto LAB_00dfb033 end
    end
    if not (quest:IsActiveThreadTerminating()) then
        quest:RemoveQuestInfoElement(infoElement)
        if not (0 < quest:GetTimer(timerId)) then goto LAB_00dfb033 end
        if not quest:IsActiveThreadTerminating() then quest:SetMasterGameState("TCGMadeTimeLimit", true); goto LAB_00dfb033 end
        quest:DeregisterTimer(timerId)
        do return end
        quest:SetMasterGameState("TCGMadeTimeLimit", true)
    end
    ::LAB_00dfb033::
    quest:DeregisterTimer(timerId)
end

-- Q_TraderConflictGood.WatchForRegionTransitions (retail 0x00dfc920)
function WatchForRegionTransitions(quest)
    local allCreaturesIndex, scratchValue
    while not quest:GetStateBool("OutroDone") do
        if not quest:NewScriptFrame() then return end
        if quest:MsgOnRegionLoaded() then
            if quest:IsActiveThreadTerminating() then return end
            UpdateLiveEnemies(quest)
            scratchValue = 0
            if quest:GetStateListCount("AllCreatures") ~= 0 then
                allCreaturesIndex = 0
                repeat
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", allCreaturesIndex), 10.0)
                    quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", allCreaturesIndex), false)
                    quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", allCreaturesIndex), "FACTION_BANDITS")
                    scratchValue = scratchValue + 1
                    allCreaturesIndex = allCreaturesIndex + 1
                until not (scratchValue < quest:GetStateListCount("AllCreatures"))
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("EnteredNewRegion", true)
        end
    end
    quest:IsActiveThreadTerminating()
end

-- Q_TraderConflictGood.WatchForHittingEnemies (retail 0x00dfc630)
function WatchForHittingEnemies(quest)
    local predicateResult, tradersFollowing, allCreaturesIndex2, allCreaturesIndex, hero
    local scratchValue2, scratchValue3
    tradersFollowing = quest:GetStateInt("TradersFollowing")
    repeat
        if 0 < tradersFollowing then
            if quest:IsActiveThreadTerminating() then return end
            while quest:GetStateInt("TradersReachedTeleporter") < 3 do
                if not quest:NewScriptFrame() then return end
                UpdateLiveEnemies(quest)
            end
            quest:IsActiveThreadTerminating()
            return
        end
        if not quest:NewScriptFrame() then return end
        scratchValue3 = 0
        if quest:GetStateListCount("AllCreatures") ~= 0 then
            allCreaturesIndex = 0
            repeat
                if quest:IsActiveThreadTerminating() then return end
                if not (quest:GetStateListAt("AllCreatures", allCreaturesIndex):MsgIsHitBy("")) then
                    if quest:GetStateListAt("AllCreatures", allCreaturesIndex):MsgIsHitByAnySpecialAbilityFrom("") and not quest:GetStateListAt("AllCreatures", allCreaturesIndex):MsgIsHitByHeroSpecialAbility(14) then goto LAB_00dfc735 end
                    predicateResult = false
                    goto FLOW_past_lab_00dfc735
                end
                ::LAB_00dfc735::
                predicateResult = true
                ::FLOW_past_lab_00dfc735::
                if not predicateResult then scratchValue3 = scratchValue3 + 1; allCreaturesIndex = allCreaturesIndex + 1; goto continue_1 end
                if quest:IsActiveThreadTerminating() then return end
                scratchValue2 = 0
                if quest:GetStateListCount("AllCreatures") ~= 0 then
                    allCreaturesIndex2 = 0
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesIndex), quest:GetStateListAt("AllCreatures", allCreaturesIndex2), 20.0) then
                            goto LAB_00dfc811
                        else
                            local creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesIndex2)
                            local pThing1 = quest:GetHero()
                            if quest:IsThingAwareOfOtherThingInAnyWay(pThing1, creaturesItem) then goto LAB_00dfc811 end
                        end
                        goto FLOW_past_lab_00dfc811
                        ::LAB_00dfc811::
                        if quest:IsActiveThreadTerminating() then return end
                        hero = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(quest:GetStateListAt("AllCreatures", allCreaturesIndex2), hero)
                        ::FLOW_past_lab_00dfc811::
                        scratchValue2 = scratchValue2 + 1
                        allCreaturesIndex2 = allCreaturesIndex2 + 1
                    until not (scratchValue2 < quest:GetStateListCount("AllCreatures"))
                end
                if quest:IsActiveThreadTerminating() then return end
                scratchValue3 = scratchValue3 + 1
                allCreaturesIndex = allCreaturesIndex + 1
                ::continue_1::
            until not (scratchValue3 < quest:GetStateListCount("AllCreatures"))
        end
        if quest:IsActiveThreadTerminating() then return end
        UpdateLiveEnemies(quest)
        tradersFollowing = quest:GetStateInt("TradersFollowing")
    until false
end

-- Q_TraderConflictGood.WatchForTradersFreed (retail 0x00dfcc10)
function WatchForTradersFreed(quest)
    local predicateResult
    while quest:GetStateInt("TradersFollowing") < 1 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
    quest:ActivateQuest("Q_TraderConflictGood_Extras")
    predicateResult = false
    while quest:GetStateInt("TradersReachedTeleporter") < 3 do
        if not quest:NewScriptFrame() then return end
        if quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing") then
            if quest:IsActiveThreadTerminating() then return end
            local getActiveQuestName2 = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
            predicateResult = false
            AttackPeople(quest)
            if not __native_all_dead(quest:GetStateListCopy("AllCreatures")) then goto continue_1 end
            if quest:IsActiveThreadTerminating() then return end
            while not quest:GetStateBool("EnteredNewRegion") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("EnteredNewRegion", false)
        elseif not predicateResult then
            if quest:IsActiveThreadTerminating() then return end
            local traderToRescue = quest:GetAllThingsWithScriptName("TraderToRescue")
            if quest:IsRegionLoaded("BanditCampEntrance") then
                if quest:IsActiveThreadTerminating() then return end
                local count = #traderToRescue
                if count == 1 then
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName3 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName3, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                    else
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName4 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName4, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                    end
                elseif count == 2 then
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName5 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName5, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName6 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName6, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                    end
                else
                    if count ~= 3 then goto LAB_00dfd5c2 end
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName7 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName7, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName8 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName8, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    end
                end
            elseif quest:IsRegionLoaded("BanditCampCentre") then
                if quest:IsActiveThreadTerminating() then return end
                local count2 = #traderToRescue
                if count2 == 1 then
                    if quest:IsActiveThreadTerminating() then return end
                    local getActiveQuestName9 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName9, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                elseif count2 < 2 then
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName10 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName10, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        if quest:IsActiveThreadTerminating() then return end
                        local getActiveQuestName11 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(getActiveQuestName11, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                    end
                else
                    if quest:IsActiveThreadTerminating() then return end
                    local getActiveQuestName12 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName12, "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                end
            end
            ::LAB_00dfd5c2::
            predicateResult = true
        end
        ::continue_1::
    end
    quest:IsActiveThreadTerminating()
end

-- Q_TraderConflictGood.WatchForKilledPeople (retail 0x00dfc290)
function WatchForKilledPeople(quest)
    while quest:GetStateInt("TradersReachedTeleporter") < 3 do
        if not quest:NewScriptFrame() then return end
        if quest:GetHero():MsgGetThingsKilled() then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetMasterGameState("TCGKillNoBandits", false)
        end
    end
    quest:IsActiveThreadTerminating()
end

-- Q_TraderConflictGood.UpdateLiveEnemies (retail 0x00dfc320)
function UpdateLiveEnemies(quest)
    local isActiveThreadTerminating2, isActiveThreadTerminating3, scratchValue, p0
    local allCreaturesIndex, scratchValue9
    quest:StateListClear("AllCreatures")
    local lst_AllCreatures = quest:GetAllCreaturesExcludingHero()
    quest:StateListSet("AllCreatures", lst_AllCreatures)
    allCreaturesIndex = 0
    while allCreaturesIndex ~= quest:GetStateListCount("AllCreatures") do
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if not (quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetDefName() == "CREATURE_NEW_CHICKEN_04") then
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "TraderToRescue" then
                goto LAB_00dfc3b5
            end
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "BodyGuard" then
                goto LAB_00dfc3b5
            end
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "RingFighter" then
                goto LAB_00dfc3b5
            end
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "FisticuffsMember" then
                goto LAB_00dfc3b5
            end
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "Tyler" then
                goto LAB_00dfc3b5
            end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            allCreaturesIndex = allCreaturesIndex + 1
            goto FLOW_past_lab_00dfc3b5
        end
        ::LAB_00dfc3b5::
        isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating3 then
            return isActiveThreadTerminating3
        end
        quest:StateListErase("AllCreatures", allCreaturesIndex)
        ::FLOW_past_lab_00dfc3b5::
    end
    local predicateResult = quest:IsActiveThreadTerminating()
    scratchValue = predicateResult
    if not predicateResult then
        scratchValue9 = 0
        local pTarget = quest:GetHero()
        local followers = quest:GetFollowingEntityList(pTarget)
        if #followers ~= 0 then
            scratchValue = 4
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00dfc618 end
                p0 = 0
                while p0 ~= quest:GetStateListCount("AllCreatures") do
                    if not followers[(scratchValue - 4) / 12 + 1]:IsEqualTo(quest:GetStateListAt("AllCreatures", p0)) then
                        p0 = p0 + 1
                    else
                        quest:StateListErase("AllCreatures", p0)
                        break
                        p0 = p0 + 1
                    end
                end
                scratchValue9 = scratchValue9 + 1
                scratchValue = scratchValue + 12
            until not (scratchValue9 < (#followers))
        end
        quest:IsActiveThreadTerminating()
        ::LAB_00dfc618::
    end
    return scratchValue
end

-- Q_TraderConflictGood.AttackPeople (retail 0x00dfd600)
function AttackPeople(quest)
    return quest:WithRetailResources(function(resources)
        local traderOwner = 0
        local scratchValue, ctr_38, scratchValue3, addNewConversation, timerId, sequence1, sequence
        local scratchValue4, getHero, scratchValue5, traderToRescue, scratchValue6
        local function ReleaseEverything()
            resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
        end
        UpdateLiveEnemies(quest)
        local allCreatures = quest:GetStateListCopy("AllCreatures")
        scratchValue3 = quest:RegisterTimer()
        timerId = scratchValue3
        if not quest:GetStateBool("EnteredNewRegion") then
            goto LAB_00dfd643
        end
        goto FLOW_past_lab_00dfd643
        ::LAB_00dfd643::
        if (#allCreatures ~= 0) and quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing") then
            quest:NewScriptFrame()
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then
                resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                goto LAB_00dfde13
            end
            ctr_38 = 0
            if #allCreatures ~= 0 then
                addNewConversation = 0
                repeat
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                        goto LAB_00dfde13
                    end
                    traderToRescue = quest:GetNearestWithScriptName(allCreatures[addNewConversation / 12 + 1], "TraderToRescue")
                    traderOwner = resources:TrackThingOutput(traderToRescue)
                    scratchValue = quest:IsEntityFollowingHero(traderToRescue)
                    if not scratchValue then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then ReleaseEverything(); return end
                        if traderOwner ~= 0 then resources:AssignThingOutput(traderOwner, nil) end
                    end
                    local dist = 15.0
                    scratchValue4 = allCreatures[addNewConversation / 12 + 1]
                    getHero = quest:GetHero()
                    scratchValue = quest:IsDistanceBetweenThingsUnder(scratchValue4, getHero, dist)
                    sequence1 = false
                    sequence1 = scratchValue
                    if not sequence1 then
                        scratchValue = quest:IsDistanceBetweenThingsUnder(allCreatures[addNewConversation / 12 + 1], traderToRescue, 15.0)
                        sequence1 = scratchValue
                    end
                    if sequence1 then
                        goto LAB_00dfd841
                    end
                    goto FLOW_past_lab_00dfd841
                    ::LAB_00dfd841::
                    scratchValue = quest:IsActiveThreadTerminating()
                    scratchValue4 = ctr_38
                    if scratchValue then
                        resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                        return
                    end
                    scratchValue = quest:IsDistanceBetweenThingsUnder(allCreatures[ctr_38 + 1], traderToRescue, 15.0)
                    sequence = false
                    sequence = scratchValue
                    if not sequence then
                        scratchValue = quest:IsThingAwareOfOtherThingInAnyWay(allCreatures[scratchValue4 + 1], traderToRescue)
                        sequence = scratchValue and traderToRescue ~= nil
                        if sequence then
                            local scratchValue2 = traderToRescue ~= nil and traderToRescue:IsAlive()
                            sequence = scratchValue2
                        end
                    end
                    if sequence then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then ReleaseEverything(); return end
                        quest:GiveThingBestEnemyTarget(allCreatures[scratchValue4 + 1], traderToRescue)
                        scratchValue3 = quest:GetTimer(timerId)
                        if scratchValue3 == 0 then
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then
                                resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                                return
                            end
                            addNewConversation = quest:AddNewConversation(allCreatures[scratchValue4 + 1], false, false)
                            getHero = quest:GetHero()
                            quest:AddPersonToConversation(addNewConversation, getHero)
                            scratchValue6 = allCreatures[scratchValue4 + 1]
                            getHero = quest:GetHero()
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", scratchValue6, getHero, false)
                            goto LAB_00dfda32
                        end
                    else
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then ReleaseEverything(); return end
                        getHero = allCreatures[scratchValue4 + 1]
                        local pTarget = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(getHero, pTarget)
                        scratchValue3 = quest:GetTimer(timerId)
                        if scratchValue3 == 0 then
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then
                                resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                                return
                            end
                            addNewConversation = quest:AddNewConversation(allCreatures[scratchValue4 + 1], false, false)
                            getHero = quest:GetHero()
                            quest:AddPersonToConversation(addNewConversation, getHero)
                            scratchValue6 = allCreatures[scratchValue4 + 1]
                            getHero = quest:GetHero()
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_B11_BANDIT_ATTACK_HERO", scratchValue6, getHero, false)
                            goto LAB_00dfda32
                        end
                    end
                    goto FLOW_past_lab_00dfda32
                    ::LAB_00dfda32::
                    quest:SetTimer(timerId, 4)
                    ::FLOW_past_lab_00dfda32::
                    quest:MiniMapAddMarker(allCreatures[scratchValue4 + 1], "HUD_ORB_RED_SMALL")
                    scratchValue5 = 0
                    if #allCreatures ~= 0 then goto LAB_00dfda90 end
                    goto LAB_00dfdabb
                    ::FLOW_past_lab_00dfd841::
                    scratchValue4 = allCreatures[addNewConversation / 12 + 1]
                    getHero = quest:GetHero()
                    scratchValue = quest:IsThingAwareOfOtherThingInAnyWay(scratchValue4, getHero)
                    if scratchValue then goto LAB_00dfd841 end
                    scratchValue = quest:IsThingAwareOfOtherThingInAnyWay(allCreatures[addNewConversation / 12 + 1], traderToRescue)
                    if scratchValue then
                        local p0 = "TC_BanditGuard"
                        local elem_1 = allCreatures[addNewConversation / 12 + 1]
                        local this_00 = elem_1:GetName()
                        scratchValue3 = this_00 ~= p0 and 1 or 0
                        if scratchValue3 ~= 0 then goto LAB_00dfd841 end
                    end
                    resources:RetireThingOutput(traderOwner)
                    traderOwner = 0
                    ctr_38 = ctr_38 + 1
                    addNewConversation = addNewConversation + 12
                    scratchValue3 = timerId
                until not (ctr_38 < (#allCreatures))
            end
            goto LAB_00dfdaff
        end
        ::FLOW_past_lab_00dfd643::
        ::LAB_00dfdb8c::
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
        else
            if quest:GetStateBool("EnteredNewRegion") then
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                    goto LAB_00dfde13
                end
                quest:SetStateBool("EnteredNewRegion", false)
            end
            resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
        end
        ::LAB_00dfde13::
        do return end
        ::LAB_00dfda90::
        while true do
            scratchValue = allCreatures[scratchValue4 + 1]:IsEqualTo(allCreatures[scratchValue5 + 1])
            if scratchValue then
                table.remove(allCreatures, scratchValue5 + 1)
                break
            end
            scratchValue5 = scratchValue5 + 1
            if not (scratchValue5 ~= #allCreatures) then break end
        end
        ::LAB_00dfdabb::
        resources:RetireThingOutput(traderOwner)
        traderOwner = 0
        traderToRescue = nil
        scratchValue3 = timerId
        ::LAB_00dfdaff::
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
        else
            quest:NewScriptFrame()
            scratchValue = quest:IsActiveThreadTerminating()
            if not scratchValue then
                quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    ReleaseEverything()
                    return
                end
                quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId); return end
                quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                    return
                end
                quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if not scratchValue then
                    if quest:GetStateBool("EnteredNewRegion") then goto LAB_00dfdb8c end
                    goto LAB_00dfd643
                end
                resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
                return
            end
            resources:RetireThingOutput(traderOwner); traderOwner = 0; quest:DeregisterTimer(timerId)
        end
        goto LAB_00dfde13
    end)
end

-- Q_TraderConflictGood.helper_DFDED0 (retail 0x00dfded0)
function helper_DFDED0(quest, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(strParam1, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

