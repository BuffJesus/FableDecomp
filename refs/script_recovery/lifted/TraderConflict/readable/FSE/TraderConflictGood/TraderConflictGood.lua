-- Readable native conversion: Q_TraderConflictGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WhiteBalvNoDamageBoastReward = 480,  -- 2500
    WhiteBalvNoWeaponsBoastCost = 484,  -- 500
    WhiteBalvNoWeaponsBoastReward = 488,  -- 4000
    SummoningShipNakedBoastCost = 492,  -- 800
    SummoningShipNakedBoastReward = 496,  -- 1800
    SummoningShipNoDamageBoastCost = 500,  -- 500
    RansomVictimHaveVictimKilledReward = 568,  -- 1000
    MinionCampBriarNoDamageCost = 572,  -- 400
    MinionCampBriarNoDamageReward = 576,  -- 2000
    AmbushScamTricksterApproachProximityLow = 580,  -- 5.0
    TCG_TimeLimit = 3972,  -- 480
}

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
    quest:OpenDoor(quest:GetThingWithScriptName("Gate1"))
    quest:OpenDoor(quest:GetThingWithScriptName("Gate2Outer"))
    helper_DFDED0(quest, "CS_TRADERCON_GOOD_INTRO")
    quest:SetStateBool("IntroDone", true)
    quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
    quest:FadeScreenIn()
    quest:CreateThread("WatchTimeLimit")  -- native thread body CQ_TraderConflictEvilScript::WatchTimeLimit: lift it as function WatchTimeLimit(quest)
    UpdateLiveEnemies(quest)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_01", "BanditCampCentre", "BanditCampMain")
    if quest:GetStateListCount("AllCreatures") ~= 0 then
        allCreaturesIndex = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", allCreaturesIndex), nil --[[operand lost by the decompiler]])
            quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", allCreaturesIndex), false)
            quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", allCreaturesIndex), "FACTION_BANDITS")
            scratchValue = scratchValue + 1
            allCreaturesIndex = allCreaturesIndex + 1
        until scratchValue >= quest:GetStateListCount("AllCreatures")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CreateThread("WatchForRegionTransitions")  -- native thread body WatchForRegionTransitions: lift it as function WatchForRegionTransitions(quest)
    quest:CreateThread("WatchForHittingEnemies")  -- native thread body WatchForHittingEnemies: lift it as function WatchForHittingEnemies(quest)
    quest:CreateThread("WatchForTradersFreed")  -- native thread body 0x00DFCC10: lift it as function WatchForTradersFreed(quest)
    quest:CreateThread("WatchForKilledPeople")  -- native thread body WatchForKilledPeople: lift it as function WatchForKilledPeople(quest)
    while not quest:IsRegionLoaded("BanditCampCentre") do
        if not quest:NewScriptFrame() then return end
        if not quest:GetStateBool("FirstRegionDead") and quest:GetStateListCount("AllCreatures") == 0 then
            quest:SetStateBool("FirstRegionDead", true)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:OpenDoor(quest:GetThingWithScriptName("Gate2Inner"))
    while not quest:GetStateBool("OutroDone") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
    while quest:IsRegionLoaded("BanditCampEntrance") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNoDamageBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNoWeaponsBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNoWeaponsBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.SummoningShipNakedBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(SCRIPT_DEF.SummoningShipNakedBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.SummoningShipNoDamageBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE", 34, quest:ReadGlobalGameData(SCRIPT_DEF.RansomVictimHaveVictimKilledReward), quest:ReadGlobalGameData(SCRIPT_DEF.MinionCampBriarNoDamageCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT", 35, quest:ReadGlobalGameData(SCRIPT_DEF.MinionCampBriarNoDamageReward), quest:ReadGlobalGameData(SCRIPT_DEF.AmbushScamTricksterApproachProximityLow), false, "", 0)
    quest:SetMasterGameState("TCGKillNoBandits", true)
    quest:SetMasterGameState("TCGMadeTimeLimit", false)
end

-- Q_TraderConflictGood.WatchTimeLimit (retail 0x00dfaeb0)
function WatchTimeLimit(quest)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    while not quest:GetMasterGameState("TCGTimeLimitBoastTaken") do
        if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        if quest:GetTimer(timerId) == 0 then
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
    if quest:IsActiveThreadTerminating() then goto LAB_00dfb033 end
    quest:RemoveQuestInfoElement(infoElement)
    if 0 >= quest:GetTimer(timerId) then goto LAB_00dfb033 end
    if not quest:IsActiveThreadTerminating() then quest:SetMasterGameState("TCGMadeTimeLimit", true); goto LAB_00dfb033 end
    quest:DeregisterTimer(timerId)
    do return end
    quest:SetMasterGameState("TCGMadeTimeLimit", true)
    ::LAB_00dfb033::
    quest:DeregisterTimer(timerId)
end

-- Q_TraderConflictGood.WatchForRegionTransitions (retail 0x00dfc920)
function WatchForRegionTransitions(quest)
    local allCreaturesIndex, scratchValue
    while not quest:GetStateBool("OutroDone") do
        if not quest:NewScriptFrame() then goto LAB_00dfca72 end
        if quest:MsgOnRegionLoaded() then
            UpdateLiveEnemies(quest)
            scratchValue = 0
            if quest:GetStateListCount("AllCreatures") ~= 0 then
                allCreaturesIndex = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00dfca72 end
                    quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", allCreaturesIndex), nil --[[operand lost by the decompiler]])
                    quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", allCreaturesIndex), false)
                    quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", allCreaturesIndex), "FACTION_BANDITS")
                    scratchValue = scratchValue + 1
                    allCreaturesIndex = allCreaturesIndex + 1
                until scratchValue >= quest:GetStateListCount("AllCreatures")
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00dfca72 end
            quest:SetStateBool("EnteredNewRegion", true)
        end
    end
    ::LAB_00dfca72::
end

-- Q_TraderConflictGood.WatchForHittingEnemies (retail 0x00dfc630)
function WatchForHittingEnemies(quest)
    local predicateResult, tradersFollowing, allCreaturesIndex, scratchValue2, scratchValue3
    local hero = quest:GetHero()
    tradersFollowing = quest:GetStateInt("TradersFollowing")
    repeat
        if 0 < tradersFollowing then
            if quest:IsActiveThreadTerminating() then return end
            while quest:GetStateInt("TradersReachedTeleporter") < 3 do
                if not quest:NewScriptFrame() then return end
                UpdateLiveEnemies(quest)
            end
            return
        end
        if not quest:NewScriptFrame() then return end
        scratchValue3 = 0
        if quest:GetStateListCount("AllCreatures") ~= 0 then
            repeat
                if quest:IsActiveThreadTerminating() then return end
                if quest:GetStateListAt("AllCreatures", 0 / 12):MsgIsHitBy("") then goto LAB_00dfc735 end
                if quest:GetStateListAt("AllCreatures", 0 / 12):MsgIsHitByAnySpecialAbilityFrom("") then
                    if not quest:GetStateListAt("AllCreatures", 0 / 12):MsgIsHitByHeroSpecialAbility(14) then goto LAB_00dfc735 end
                end
                predicateResult = false
                goto FLOW_past_lab_00dfc735
                ::LAB_00dfc735::
                predicateResult = true
                ::FLOW_past_lab_00dfc735::
                if predicateResult then
                    scratchValue2 = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        allCreaturesIndex = 0
                        repeat
                            if quest:IsActiveThreadTerminating() then return end
                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", 0 / 12), quest:GetStateListAt("AllCreatures", allCreaturesIndex), 20.0) then
                                goto LAB_00dfc811
                            else
                                local isThingAwareOfOtherThingInAnyWay = quest:IsThingAwareOfOtherThingInAnyWay(hero, nil --[[missing]])
                                if isThingAwareOfOtherThingInAnyWay then goto LAB_00dfc811 end
                            end
                            goto FLOW_past_lab_00dfc811
                            ::LAB_00dfc811::
                            quest:GiveThingBestEnemyTarget(hero, nil --[[missing]])
                            ::FLOW_past_lab_00dfc811::
                            scratchValue2 = scratchValue2 + 1
                            allCreaturesIndex = allCreaturesIndex + 1
                        until scratchValue2 >= quest:GetStateListCount("AllCreatures")
                    end
                    if quest:IsActiveThreadTerminating() then return end
                end
                scratchValue3 = scratchValue3 + 1
                -- TODO(native): iStack_18 = iStack_18 + 0xc;
            until scratchValue3 >= quest:GetStateListCount("AllCreatures")
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
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
    quest:ActivateQuest("Q_TraderConflictGood_Extras")
    predicateResult = false
    while quest:GetStateInt("TradersReachedTeleporter") < 3 do
        if not quest:NewScriptFrame() then return end
        if quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing") then
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
            predicateResult = false
            AttackPeople(quest)
            if AreAllThingsInVectorDead(this + 72) == 0 then goto continue_1 end
            if quest:IsActiveThreadTerminating() then return end
            while not quest:GetStateBool("EnteredNewRegion") do
                if not quest:NewScriptFrame() then return end
            end
            quest:SetStateBool("EnteredNewRegion", false)
        elseif not predicateResult then
            local traderToRescue = quest:GetAllThingsWithScriptName("TraderToRescue")
            if quest:IsRegionLoaded("BanditCampEntrance") then
                local count = #traderToRescue
                if count == 1 then
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                    end
                elseif count == 2 then
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                    end
                else
                    if count ~= 3 then goto LAB_00dfd5c2 end
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    end
                end
            elseif quest:IsRegionLoaded("BanditCampCentre") then
                local count2 = #traderToRescue
                if count2 == 1 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                elseif count2 < 2 then
                    if quest:GetStateInt("TradersReachedTeleporter") < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                    end
                else
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                end
            end
            ::LAB_00dfd5c2::
            predicateResult = true
        end
        ::continue_1::
    end
end

-- Q_TraderConflictGood.WatchForKilledPeople (retail 0x00dfc290)
function WatchForKilledPeople(quest)
    local this_00
    while quest:GetStateInt("TradersReachedTeleporter") < 3 do
        if not quest:NewScriptFrame() then goto LAB_00dfc302 end
        -- TODO(native): MsgGetThingsKilled is not a ForgeFSE binding
        -- TODO(native): bVar2 = this_00:MsgGetThingsKilled(&0x0)
    --[[unresolved native value]]
        if nil then
            if quest:IsActiveThreadTerminating() then goto LAB_00dfc302 end
            quest:SetMasterGameState("TCGKillNoBandits", false)
        end
    end
    ::LAB_00dfc302::
    if nil ~= nil then
        -- TODO(native): free(xStack_c);
    end
end

-- Q_TraderConflictGood.UpdateLiveEnemies (retail 0x00dfc320)
function UpdateLiveEnemies(quest)
    local isActiveThreadTerminating2, isActiveThreadTerminating3, scratchValue, p0
    local allCreaturesIndex, scratchValue8
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    allCreaturesIndex = 0
    while allCreaturesIndex ~= quest:GetStateListCount("AllCreatures") do
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetDefName() == "CREATURE_NEW_CHICKEN_04" then goto LAB_00dfc3b5 end
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
        scratchValue8 = 0
        local followers = quest:GetFollowingEntityList(quest:GetHero())
        if #followers ~= 0 then
            scratchValue = 4
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00dfc618 end
                p0 = 0
                while p0 ~= quest:GetStateListCount("AllCreatures") do
                    -- TODO(native): cVar8 = (**(**(iVar3 + iStack_c) + 0x138))(quest:GetStateListAt("AllCreatures", p0))
    --[[unresolved native value]]
                    if nil == 0 then
                        p0 = p0 + 1
                    else
                        quest:StateListErase("AllCreatures", p0)
                        break
                        p0 = p0 + 1
                    end
                end
                scratchValue8 = scratchValue8 + 1
                scratchValue = scratchValue + 12
            until scratchValue8 >= #followers
        end
        ::LAB_00dfc618::
    end
    return scratchValue
end

-- Q_TraderConflictGood.AttackPeople (retail 0x00dfd600)
function AttackPeople(quest)
    local scratchValue4, sequence, scratchValue5, scratchValue7, scratchValue8, scratchValue10
    local scratchValue, timerId
    local hero = quest:GetHero()
    local function DeregisterTimers()
        quest:DeregisterTimer(timerId)
    end
    UpdateLiveEnemies(quest)
    local allCreatures = quest:GetStateListCopy("AllCreatures")
    timerId = quest:RegisterTimer()
    if not quest:GetStateBool("EnteredNewRegion") then
        goto LAB_00dfd643
    end
    goto FLOW_past_lab_00dfd643
    ::LAB_00dfd643::
    if #allCreatures ~= 0 and quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing") then
        if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); goto LAB_00dfde13 end
        if #allCreatures ~= 0 then
            scratchValue4 = 0
            repeat
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); goto LAB_00dfde13 end
                local traderToRescue = quest:GetNearestWithScriptName(allCreatures[scratchValue4 + 1], "TraderToRescue")
                if not quest:IsEntityFollowingHero(traderToRescue) then
                    if scratchValue10 ~= nil then
                        -- TODO(native): *xStack_10 = *xStack_10 - 1;
                        -- TODO(native): if *xStack_10 == 0 then
                    end
                end
                if quest:IsDistanceBetweenThingsUnder(allCreatures[scratchValue4 + 1], hero, 15.0) or quest:IsDistanceBetweenThingsUnder(allCreatures[scratchValue4 + 1], traderToRescue, 15.0) then
                    goto LAB_00dfd841
                end
                goto FLOW_past_lab_00dfd841
                ::LAB_00dfd841::
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                sequence = quest:IsDistanceBetweenThingsUnder(allCreatures[0 + 1], traderToRescue, 15.0)
                if not sequence then
                    sequence = quest:IsThingAwareOfOtherThingInAnyWay(allCreatures[0 + 1], traderToRescue) and scratchValue7 ~= nil
                    if sequence then
                        -- TODO(native): cVar3 = (**(*piStack_14 + 0x12c))()
                        local scratchValue2 = nil --[[unresolved native value]]
                        sequence = scratchValue2 ~= 0
                    end
                end
                if sequence then
                    if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
                    quest:GiveThingBestEnemyTarget(allCreatures[0 + 1], traderToRescue)
                    if quest:GetTimer(timerId) == 0 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                        local conversationId = quest:AddNewConversation(allCreatures[0 + 1], false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        scratchValue = allCreatures[0 + 1]
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", scratchValue, hero, false)
                        goto LAB_00dfda32
                    end
                else
                    if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
                    quest:GiveThingBestEnemyTarget(allCreatures[0 + 1], hero)
                    if quest:GetTimer(timerId) == 0 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                        local conversationId2 = quest:AddNewConversation(allCreatures[0 + 1], false, false)
                        quest:AddPersonToConversation(conversationId2, hero)
                        scratchValue = allCreatures[0 + 1]
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_ATTACK_HERO", scratchValue, hero, false)
                        goto LAB_00dfda32
                    end
                end
                goto FLOW_past_lab_00dfda32
                ::LAB_00dfda32::
                quest:SetTimer(timerId, 4)
                ::FLOW_past_lab_00dfda32::
                quest:MiniMapAddMarker(allCreatures[0 + 1], "HUD_ORB_RED_SMALL")
                if #allCreatures ~= 0 then
                    if allCreatures[0 + 1]:IsEqualTo(allCreatures[0 + 1]) then
                        table.remove(allCreatures, 0 + 1)
                        break
                    end
                    goto FLOW_after_lab_00dfda90
                end
                goto LAB_00dfdabb
                ::FLOW_past_lab_00dfd841::
                if quest:IsThingAwareOfOtherThingInAnyWay(allCreatures[scratchValue4 + 1], hero) then goto LAB_00dfd841 end
                if quest:IsThingAwareOfOtherThingInAnyWay(allCreatures[scratchValue4 + 1], traderToRescue) then
                    if allCreatures[scratchValue4 + 1]:GetName() ~= "TC_BanditGuard" then goto LAB_00dfd841 end
                end
                -- TODO(native): xStack_38 = (CScriptThing *)&*(int *)(xStack_38 + 0x1);
                scratchValue4 = scratchValue4 + 1
            until scratchValue >= #allCreatures
        end
        goto LAB_00dfdaff
    end
    ::FLOW_past_lab_00dfd643::
    ::LAB_00dfdb8c::
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
    else
        if quest:GetStateBool("EnteredNewRegion") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); goto LAB_00dfde13 end
            quest:SetStateBool("EnteredNewRegion", false)
        end
        quest:DeregisterTimer(timerId)
    end
    ::LAB_00dfde13::
    do return end
    while true do
        scratchValue8 = scratchValue8 + 1
        if scratchValue8 == #allCreatures then break end
        if allCreatures[scratchValue5 + 1]:IsEqualTo(allCreatures[scratchValue8 + 1]) then
            table.remove(allCreatures, scratchValue8 + 1)
            break
        end
    end
    ::FLOW_after_lab_00dfda90::
    ::LAB_00dfdabb::
    ::LAB_00dfdaff::
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
    else
        quest:NewScriptFrame()
        if not quest:IsActiveThreadTerminating() then
            if not quest:NewScriptFrame() then DeregisterTimers(); return end
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() then
                if quest:GetStateBool("EnteredNewRegion") then goto LAB_00dfdb8c end
                goto LAB_00dfd643
            end
            quest:DeregisterTimer(timerId)
            return
        end
        quest:DeregisterTimer(timerId)
    end
    goto LAB_00dfde13
end

-- Q_TraderConflictGood.helper_DFDED0 (retail 0x00dfded0)
function helper_DFDED0(quest, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
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

