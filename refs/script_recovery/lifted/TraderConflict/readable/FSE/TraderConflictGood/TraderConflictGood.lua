-- Readable native conversion: Q_TraderConflictGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_TraderConflictGood.Main (retail 0x00dfa450)
function Main(quest)
    local scratchValue, scratchValue2, scratchValue3, sequence12, scratchValue4
    scratchValue4 = 0
    quest:AddEntityBinding("TraderToRescue", "TraderConflictGood/Entities/TraderToRescue")
    quest:AddEntityBinding("TC_BanditGuard", "TraderConflictGood/Entities/TC_BanditGuard")
    quest:AddEntityBinding("TC_BanditHostageKeeper", "TraderConflictGood/Entities/TC_BanditHostageKeeper")
    quest:AddEntityBinding("BCMTrader", "TraderConflictGood/Entities/BCMTrader")
    quest:AddEntityBinding("BCGameMaster", "TraderConflictGood/Entities/BCGameMaster")
    quest:AddEntityBinding("BanditExtra", "TraderConflictGood/Entities/BanditExtra")
    quest:AddEntityBinding("CampHostageDoor", "TraderConflictGood/Entities/CampHostageDoor")
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
    scratchValue = (quest:GetStateListCount("AllCreatures") * 12) >> 31
    if quest:GetStateListCount("AllCreatures") + scratchValue ~= scratchValue then
        scratchValue2 = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", scratchValue2 / 12), scratchValue2)
            quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", scratchValue2 / 12), false)
            quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", scratchValue2 / 12), "FACTION_BANDITS")
            scratchValue4 = scratchValue4 + 1
            scratchValue2 = scratchValue2 + 12
        until scratchValue4 >= quest:GetStateListCount("AllCreatures")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CreateThread("WatchForRegionTransitions")  -- native thread body WatchForRegionTransitions: lift it as function WatchForRegionTransitions(quest)
    quest:CreateThread("WatchForHittingEnemies")  -- native thread body WatchForHittingEnemies: lift it as function WatchForHittingEnemies(quest)
    quest:CreateThread("WatchForTradersFreed")  -- native thread body 0x00DFCC10: lift it as function WatchForTradersFreed(quest)
    quest:CreateThread("WatchForKilledPeople")  -- native thread body WatchForKilledPeople: lift it as function WatchForKilledPeople(quest)
    while not quest:IsRegionLoaded("BanditCampCentre") do
        if not quest:NewScriptFrame() then return end
        sequence12 = not quest:GetStateBool("FirstRegionDead")
        if sequence12 then
            scratchValue3 = (quest:GetStateListCount("AllCreatures") * 12) >> 31
            sequence12 = quest:GetStateListCount("AllCreatures") + scratchValue3 == scratchValue3
        end
        if sequence12 then
            quest:SetStateBool("FirstRegionDead", true)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:OpenDoor(quest:GetThingWithScriptName("BanditCampMain"))
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(480), quest:ReadGlobalGameData(484), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(488), quest:ReadGlobalGameData(492), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(496), quest:ReadGlobalGameData(500), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE", 34, quest:ReadGlobalGameData(568), quest:ReadGlobalGameData(572), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT", 35, quest:ReadGlobalGameData(576), quest:ReadGlobalGameData(580), false, "", 0)
    quest:SetMasterGameState("TCGKillNoBandits", true)
    quest:SetMasterGameState("TCGMadeTimeLimit", false)
end

-- Q_TraderConflictGood.WatchTimeLimit (retail 0x00dfaeb0)
function WatchTimeLimit(quest)
    local scratchValue, timerId
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    while quest:GetMasterGameState("TCGTimeLimitBoastTaken") == 0 do
        if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        if quest:GetTimer(timerId) == 0 then
            quest:DeregisterTimer(timerId)
            return
        end
    end
    quest:SetTimer(timerId, quest:ReadGlobalGameData(3972))
    scratchValue = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
    quest:DisplayQuestInfo(true)
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then goto LAB_00dfb033 end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveQuestInfoElement(scratchValue)
        if 0 < quest:GetTimer(timerId) then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            quest:SetMasterGameState("TCGMadeTimeLimit", true)
        end
    end
    ::LAB_00dfb033::
    quest:DeregisterTimer(timerId)
end

-- Q_TraderConflictGood.WatchForRegionTransitions (retail 0x00dfc920)
function WatchForRegionTransitions(quest)
    local scratchValue2, scratchValue3, scratchValue4
    while not quest:GetStateBool("OutroDone") do
        if not quest:NewScriptFrame() then goto LAB_00dfca72 end
        if quest:MsgOnRegionLoaded() then
            UpdateLiveEnemies(quest)
            scratchValue2 = (quest:GetStateListCount("AllCreatures") * 12) >> 31
            scratchValue4 = 0
            if quest:GetStateListCount("AllCreatures") + scratchValue2 ~= scratchValue2 then
                scratchValue3 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00dfca72 end
                    quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", scratchValue3 / 12), scratchValue3)
                    quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", scratchValue3 / 12), false)
                    quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", scratchValue3 / 12), "FACTION_BANDITS")
                    scratchValue4 = scratchValue4 + 1
                    scratchValue3 = scratchValue3 + 12
                until scratchValue4 >= quest:GetStateListCount("AllCreatures")
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00dfca72 end
            quest:SetStateBool("EnteredNewRegion", true)
        end
    end
    ::LAB_00dfca72::
end

-- Q_TraderConflictGood.WatchForHittingEnemies (retail 0x00dfc630)
function WatchForHittingEnemies(quest)
    local predicateResult2, isThingAwareOfOtherThingInAnyWay, tradersFollowing, scratchValue
    local scratchValue2, scratchValue3, scratchValue4, scratchValue5
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
        scratchValue = (quest:GetStateListCount("AllCreatures") * 12) >> 31
        scratchValue5 = 0
        if quest:GetStateListCount("AllCreatures") + scratchValue ~= scratchValue then
            repeat
                if quest:IsActiveThreadTerminating() then return end
                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                if not quest:GetStateListAt("AllCreatures", 0 / 12):MsgIsHitBy("") then
                    -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                    if quest:GetStateListAt("AllCreatures", 0 / 12):MsgIsHitByAnySpecialAbilityFrom("") then
                        if not quest:GetStateListAt("AllCreatures", 0 / 12):MsgIsHitByHeroSpecialAbility(14) then
                            predicateResult2 = true
                            goto FLOW_after_lab_00dfc735
                        end
                    end
                    predicateResult2 = false
                else
                    predicateResult2 = true
                end
                ::FLOW_after_lab_00dfc735::
                if predicateResult2 then
                    if quest:IsActiveThreadTerminating() then return end
                    scratchValue2 = (quest:GetStateListCount("AllCreatures") * 12) >> 31
                    scratchValue4 = 0
                    if quest:GetStateListCount("AllCreatures") + scratchValue2 ~= scratchValue2 then
                        scratchValue3 = 0
                        repeat
                            if quest:IsActiveThreadTerminating() then return end
                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", 0 / 12), quest:GetStateListAt("AllCreatures", scratchValue3), 20.0) then
                                quest:GiveThingBestEnemyTarget(quest:GetHero(), nil --[[missing]])
                            else
                                isThingAwareOfOtherThingInAnyWay = quest:IsThingAwareOfOtherThingInAnyWay(quest:GetHero(), nil --[[missing]])
                                if isThingAwareOfOtherThingInAnyWay then
                                    quest:GiveThingBestEnemyTarget(quest:GetHero(), nil --[[missing]])
                                end
                            end
                            scratchValue4 = scratchValue4 + 1
                            scratchValue3 = scratchValue3 + 1
                        until scratchValue4 >= quest:GetStateListCount("AllCreatures")
                    end
                    if quest:IsActiveThreadTerminating() then return end
                end
                scratchValue5 = scratchValue5 + 1
                -- TODO(native): iStack_18 = iStack_18 + 0xc;
            until scratchValue5 >= quest:GetStateListCount("AllCreatures")
        end
        if quest:IsActiveThreadTerminating() then return end
        UpdateLiveEnemies(quest)
        tradersFollowing = quest:GetStateInt("TradersFollowing")
    until false
end

-- Q_TraderConflictGood.WatchForTradersFreed (retail 0x00dfcc10)
function WatchForTradersFreed(quest)
    local predicateResult2, getAllThingsWithScriptName, scratchValue2, scratchValue3
    local tradersFollowing = quest:GetStateInt("TradersFollowing")
    local tradersReachedTeleporter = quest:GetStateInt("TradersReachedTeleporter")
    while tradersFollowing < 1 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
    quest:ActivateQuest("Q_TraderConflictGood_Extras")
    predicateResult2 = false
    while tradersReachedTeleporter < 3 do
        if not quest:NewScriptFrame() then return end
        if tradersReachedTeleporter < tradersFollowing then
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_02", "BanditCampEntrance", "BanditCampEntrance")
            predicateResult2 = false
            AttackPeople(quest)
            if AreAllThingsInVectorDead(this + 72) ~= 0 then
                if quest:IsActiveThreadTerminating() then return end
                while not quest:GetStateBool("EnteredNewRegion") do
                    if not quest:NewScriptFrame() then return end
                end
                quest:SetStateBool("EnteredNewRegion", false)
            end
        elseif not predicateResult2 then
            getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("TraderToRescue")
            if quest:IsRegionLoaded("BanditCampEntrance") then
                scratchValue2 = #getAllThingsWithScriptName
                if scratchValue2 == 1 then
                    if tradersReachedTeleporter < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                    end
                elseif scratchValue2 == 2 then
                    if tradersReachedTeleporter < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                    end
                else
                    if scratchValue2 ~= 3 then goto LAB_00dfd5c2 end
                    if tradersReachedTeleporter < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    end
                end
            elseif quest:IsRegionLoaded("BanditCampCentre") then
                scratchValue3 = #getAllThingsWithScriptName
                if scratchValue3 == 1 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_SINGLE", "BanditCampCentre", "BanditCampEntrance")
                elseif scratchValue3 < 2 then
                    if tradersReachedTeleporter < 2 then
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_SINGLE", "BanditCampEntrance", "BanditCampEntrance")
                    else
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03A_PLURAL", "BanditCampEntrance", "BanditCampEntrance")
                    end
                else
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_03B_PLURAL", "BanditCampCentre", "BanditCampEntrance")
                end
            end
            ::LAB_00dfd5c2::
            predicateResult2 = true
        end
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
    local isActiveThreadTerminating, isActiveThreadTerminating2, isActiveThreadTerminating3
    local isActiveThreadTerminating4, isActiveThreadTerminating5, isActiveThreadTerminating6
    local isActiveThreadTerminating7, isActiveThreadTerminating8, predicateResult, scratchValue7, p0
    local scratchValue8, scratchValue9
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    scratchValue8 = 0
    while scratchValue8 ~= quest:GetStateListCount("AllCreatures") do
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if ((quest:GetStateListAt("AllCreatures", scratchValue8):GetDefName() == "CREATURE_NEW_CHICKEN_04") and 0 or 1) ~= 0 then
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "TraderToRescue" then goto LAB_00dfc413 end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc413::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "BodyGuard" then goto LAB_00dfc45c end
            isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating3 then
                return isActiveThreadTerminating3
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc45c::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "RingFighter" then goto LAB_00dfc4a5 end
            isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4a5::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "FisticuffsMember" then goto LAB_00dfc4ee end
            isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4ee::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "Tyler" then goto LAB_00dfc537 end
            isActiveThreadTerminating6 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating6 then
                return isActiveThreadTerminating6
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc537::
            isActiveThreadTerminating7 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating7 then
                return isActiveThreadTerminating7
            end
            scratchValue8 = scratchValue8 + 1
        else
            isActiveThreadTerminating8 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating8 then
                return isActiveThreadTerminating8
            end
            quest:StateListErase("AllCreatures", scratchValue8)
        end
        ::FLOW_after_lab_00dfc3b5::
    end
    predicateResult = quest:IsActiveThreadTerminating()
    scratchValue7 = predicateResult
    if not predicateResult then
        scratchValue9 = 0
        scratchValue7 = 0 - 0 >> 31
        if (0 - 0) / 12 + scratchValue7 ~= scratchValue7 then
            scratchValue7 = 4
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00dfc618 end
                p0 = 0
                while p0 ~= quest:GetStateListCount("AllCreatures") do
                    -- TODO(native): cVar8 = (**(**(iVar3 + 0) + 0x138))(quest:GetStateListAt("AllCreatures", p0))
    --[[unresolved native value]]
                    if nil ~= 0 then
                        quest:StateListErase("AllCreatures", p0)
                        break
                    end
                    p0 = p0 + 1
                end
                scratchValue9 = scratchValue9 + 1
                scratchValue7 = scratchValue7 + 12
            until scratchValue9 >= ((0 - 0) / 12)
        end
        ::LAB_00dfc618::
    end
    return scratchValue7
end

-- Q_TraderConflictGood.AttackPeople (retail 0x00dfd600)
function AttackPeople(quest)
    local scratchValue, dist, scratchValue3, scratchValue4, p0, scratchValue7, scratchValue8
    local getStateListCopy, scratchValue9, scratchValue10
    local function __cleanup_LAB_00dfdc54()
        quest:DeregisterTimer(scratchValue10)
    end
    local function __cleanup_LAB_00dfdc8a()
        quest:DeregisterTimer(scratchValue10)
    end
    UpdateLiveEnemies(quest)
    getStateListCopy = quest:GetStateListCopy("AllCreatures")
    scratchValue3 = quest:RegisterTimer()
    scratchValue10 = scratchValue3
    if not quest:GetStateBool("EnteredNewRegion") then
        -- LAB_00dfd643: (native jump target)
        if #getStateListCopy ~= 0 and quest:GetStateInt("TradersReachedTeleporter") < quest:GetStateInt("TradersFollowing") then
            if not quest:NewScriptFrame() then scratchValue3 = quest:DeregisterTimer(scratchValue3); goto LAB_00dfde13 end
            if #getStateListCopy ~= 0 then
                scratchValue4 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue10); goto LAB_00dfde13 end
                    scratchValue8 = quest:GetNearestWithScriptName(getStateListCopy[scratchValue4 / 12 + 1], "TraderToRescue")
                    if not quest:IsEntityFollowingHero(scratchValue8) then
                        if scratchValue8 ~= nil then
                            -- TODO(native): *xStack_18 = *xStack_18 - 1;
                            -- TODO(native): if *r1 == 0 then
                        end
                    end
                    dist = 15.0
                    if quest:IsDistanceBetweenThingsUnder(getStateListCopy[scratchValue4 / 12 + 1], quest:GetHero(), 15.0) or quest:IsDistanceBetweenThingsUnder(getStateListCopy[scratchValue4 / 12 + 1], scratchValue8, 15.0) then
                        -- LAB_00dfd841: (native jump target)
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dfdc54(); return end
                        if quest:IsDistanceBetweenThingsUnder(getStateListCopy[0 + 1], scratchValue8, 15.0) or (quest:IsThingAwareOfOtherThingInAnyWay(getStateListCopy[0 + 1], scratchValue8) and (scratchValue8 ~= nil and not scratchValue8:IsNull()) and (scratchValue8 ~= nil and scratchValue8:IsAlive())) then
                            quest:GiveThingBestEnemyTarget(getStateListCopy[0 + 1], scratchValue8)
                            if quest:GetTimer(scratchValue10) == 0 then
                                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dfdc8a(); return end
                                scratchValue4 = quest:AddNewConversation(getStateListCopy[0 + 1], false, false)
                                quest:AddPersonToConversation(scratchValue4, quest:GetHero())
                                quest:AddLineToConversation(scratchValue4, "TEXT_QST_B11_BANDIT_ATTACK_TRADER", getStateListCopy[0 + 1], quest:GetHero(), false)
                                quest:SetTimer(scratchValue10, 4)
                                goto FLOW_after_lab_00dfda32
                            end
                        else
                            quest:GiveThingBestEnemyTarget(getStateListCopy[0 + 1], quest:GetHero())
                            if quest:GetTimer(scratchValue10) == 0 then
                                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dfdc8a(); return end
                                scratchValue4 = quest:AddNewConversation(getStateListCopy[0 + 1], false, false)
                                quest:AddPersonToConversation(scratchValue4, quest:GetHero())
                                quest:AddLineToConversation(scratchValue4, "TEXT_QST_B11_BANDIT_ATTACK_HERO", getStateListCopy[0 + 1], quest:GetHero(), false)
                                quest:SetTimer(scratchValue10, 4)
                            end
                        end
                        ::FLOW_after_lab_00dfda32::
                        quest:MiniMapAddMarker(getStateListCopy[0 + 1], "HUD_ORB_RED_SMALL")
                        if getStateListCopy ~= pu_stk_20 then
                            -- TODO(native): bVar2 = (**(*xStack_24[pCVar1 * 3 + 1] + 0x138))(xStack_24[pCVar1 * 3 + 1],puVar6[1])
                            scratchValue = nil --[[unresolved native value]]
                            if scratchValue then
                                -- TODO(native): std__vector__pop_back(&xStack_24,(int)puVar6);
                                break
                            end
                            goto FLOW_after_lab_00dfda90
                        end
                        goto LAB_00dfdabb
                    end
                    if quest:IsThingAwareOfOtherThingInAnyWay(getStateListCopy[scratchValue4 / 12 + 1], quest:GetHero()) then return end  -- TODO(native): goto LAB_00dfd841
                    if quest:IsThingAwareOfOtherThingInAnyWay(getStateListCopy[scratchValue4 / 12 + 1], scratchValue8) then
                        p0 = "TC_BanditGuard"
                        -- TODO(native): this_00 = (void *)(**(code **)(*(int *)(iVar7 + (int)xStack_24) + 4))();
                        if CCharString__NotEqual(this_00,p0) ~= 0 then return end  -- TODO(native): goto LAB_00dfd841
                    end
                    -- TODO(native): xStack_38 = (CScriptThing *)&*(int *)(xStack_38 + 0x1);
                    scratchValue4 = scratchValue4 + 12
                until scratchValue9 >= #getStateListCopy
            end
            goto LAB_00dfdaff
        end
    end
    ::LAB_00dfdb8c::
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(dist)
    else
        if quest:GetStateBool("EnteredNewRegion") then
            if quest:IsActiveThreadTerminating() then scratchValue3 = quest:DeregisterTimer(nil --[[missing]]); goto LAB_00dfde13 end
            quest:SetStateBool("EnteredNewRegion", false)
        end
        scratchValue3 = quest:DeregisterTimer(nil --[[missing]])
    end
    ::LAB_00dfde13::
    do return end
    while true do
        scratchValue7 = scratchValue7 + 3
        if scratchValue7 == pu_stk_20 then break end
        -- TODO(native): bVar2 = (**(*xStack_24[pCVar1 * 3 + 1] + 0x138))(xStack_24[pCVar1 * 3 + 1],puVar6[1])
        scratchValue = nil --[[unresolved native value]]
        if scratchValue then
            -- TODO(native): std__vector__pop_back(&xStack_24,(int)puVar6);
            break
        end
    end
    ::FLOW_after_lab_00dfda90::
    ::LAB_00dfdabb::
    ::LAB_00dfdaff::
    if quest:IsActiveThreadTerminating() then
        scratchValue3 = quest:DeregisterTimer(nil --[[missing]])
    else
        quest:NewScriptFrame()
        if not quest:IsActiveThreadTerminating() then
            if not quest:NewScriptFrame() then quest:DeregisterTimer(nil --[[missing]]); return end
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() then
                if not quest:NewScriptFrame() then quest:DeregisterTimer(nil --[[missing]]); return end
                quest:NewScriptFrame()
                if not quest:IsActiveThreadTerminating() then
                    if quest:GetStateBool("EnteredNewRegion") then goto LAB_00dfdb8c end
                    -- TODO(native): goto LAB_00dfd643
                end
            end
            quest:DeregisterTimer(nil --[[missing]])
            return
        end
        scratchValue3 = quest:DeregisterTimer(nil --[[missing]])
    end
    goto LAB_00dfde13
end

-- Q_TraderConflictGood.helper_DFDED0 (retail 0x00dfded0)
function helper_DFDED0(quest, strParam1)
    local resources = quest:RetailResources()
    local scratchValue = resources:NewResource()
    resources:TryAcquire(scratchValue, quest:GetHero(), 4)
    local scratchValue2 = resources:NewActorMap()
    resources:SetActor(scratchValue2, "HERO", scratchValue)
    local scratchValue3 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(strParam1, scratchValue2, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue3)
    resources:DestroyActorMap(scratchValue2)
    resources:ReleaseResource(scratchValue)
end

