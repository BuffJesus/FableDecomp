-- Readable native conversion: Q_TraderConflictEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TraderEscortNoDamageBoastReward = 456,  -- 1000
    TraderEscortNoWeaponsBoastCost = 460,  -- 200
    TraderEscortNoWeaponsBoastReward = 464,  -- 600
    WhiteBalvNakedBoastCost = 468,  -- 800
    WhiteBalvNakedBoastReward = 472,  -- 1800
    WhiteBalvNoDamageBoastCost = 476,  -- 500
    AmbushScamTricksterApproachProximityHigh = 584,  -- 7.0
    AmbushScamTricksterHeroProximity = 588,  -- 5.0
    AmbushScamTricksterRunAwayProximity = 592,  -- 2.0
    AmbushScamAmbushTriggerProximity = 596,  -- 4.0
    TCE_TimeLimit = 3968,  -- 240
}

-- Q_TraderConflictEvil.Main (retail 0x00df6010)
function Main(quest)
    local allCreaturesOffset, allCreaturesOffset2, allCreaturesOffset3, predicateResult, c_stk_7d_2
    local ctr_40, ctr_44, ctr_5c, ctr_74, ctr_78, f_stk_70_1, f_stk_70_2, addQuestInfoCounter
    local scratchValue11, scratchValue12, scratchValue17, creaturesItem, getNearestWithScriptName
    local oakvaleVillagerMaleFakeShopkeeper, getDataString
    local hero = quest:GetHero()
    addQuestInfoCounter = 0
    quest:AddEntityBinding("TC_GuardSpawnPoint", "TraderConflictEvil/Entities/TC_GuardSpawnPoint")
    quest:AddEntityBinding("TC_BanditFollower", "TraderConflictEvil/Entities/TC_BanditFollower")
    quest:AddEntityBinding("TC_BanditFighter", "TraderConflictEvil/Entities/TC_BanditFighter")
    quest:AddEntityBinding("TC_Villager", "TraderConflictEvil/Entities/TC_Villager")
    quest:AddEntityBinding("IsAGuard", "TraderConflictEvil/Entities/IsAGuard")
    quest:FinalizeEntityBindings()
    while not quest:IsRegionLoaded("BarrowFields") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuest("V_SickChildBarrowFields", 0)
    quest:RepopulateVillage(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"))
    quest:EntitySetAsOpinionSource(hero, "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL")
    quest:SetGuardsIgnoreCrimes(true)
    quest:EnableGuards(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"), false)
    local bsSlumBedBrown01 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
    allCreaturesOffset3 = #bsSlumBedBrown01
    local scratchValue23 = allCreaturesOffset3
    scratchValue11 = 0
    if 0 < allCreaturesOffset3 then
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
            quest:SetThingAsUsable(bsSlumBedBrown01[addQuestInfoCounter / 12 + 1], false)
            scratchValue11 = scratchValue11 + 1
            addQuestInfoCounter = addQuestInfoCounter + 12
        until scratchValue11 >= allCreaturesOffset3
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
    UpdateLiveEnemies(quest)
    getNearestWithScriptName = quest:GetNearestWithScriptName(hero, "TC_IntroCSHeroPos")
    if getNearestWithScriptName == nil then
        getDataString = ""
    else
        getDataString = getNearestWithScriptName:GetDataString()
    end
    if getDataString ~= "SOUTH" then
        if not quest:IsActiveThreadTerminating() then scratchValue17 = "CS_TRADERCON_EVIL_INTRO_NORTH"; goto LAB_00df656e end
    elseif not quest:IsActiveThreadTerminating() then
        scratchValue17 = "CS_TRADERCON_EVIL_INTRO_SOUTH"
        goto LAB_00df656e
    end
    goto FLOW_past_lab_00df656e
    ::LAB_00df656e::
    helper_DF9E00(quest, scratchValue17)
    if not quest:GetStateBool("QuestStartScreened") then
        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    quest:FadeScreenIn()
    quest:OverrideMusic(23, false, false)
    quest:CreateThread("WatchTimeLimit")  -- native thread body NScript::CQ_TraderConflictEvilScript::WatchTimeLimit: lift it as function WatchTimeLimit(quest)
    ctr_74 = 0
    ctr_40 = 0
    if quest:GetStateListCount("AllCreatures") ~= 0 then
        ctr_78 = 0
        repeat
            allCreaturesOffset3 = ctr_78
            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
            quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), nil --[[operand lost by the decompiler]])
            quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "FACTION_MONSTERS")
            quest:MiniMapAddMarker(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "HUD_ORB_RED_SMALL")
            quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), false)
            creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):GetName()
            if creaturesItem == nil then
                allCreaturesOffset3 = ctr_78
                goto LAB_00df6794
            else
                if creaturesItem ~= "IsAGuard" then goto LAB_00df6794 end
            end
            goto FLOW_past_lab_00df6794
            ::LAB_00df6794::
            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
            creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):GetDefName()
            if creaturesItem == nil then
                allCreaturesOffset3 = ctr_78
                c_stk_7d_2 = false
            else
                c_stk_7d_2 = creaturesItem == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER"
            end
            if c_stk_7d_2 then
                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                oakvaleVillagerMaleFakeShopkeeper = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):GetPos(), "TC_Villager")
                ctr_74 = ctr_74 + 1
                quest:SetCombatNearbyBreakOffRange(oakvaleVillagerMaleFakeShopkeeper, 10.0)
                quest:EntitySetInFaction(oakvaleVillagerMaleFakeShopkeeper, "FACTION_MONSTERS")
                quest:MiniMapAddMarker(oakvaleVillagerMaleFakeShopkeeper, "HUD_ORB_RED_SMALL")
                quest:EntitySetSleepEnabled(oakvaleVillagerMaleFakeShopkeeper, false)
                quest:EntitySetStategroupEnabled(oakvaleVillagerMaleFakeShopkeeper, "SG_MINION_SLEEP", false)
            end
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "SG_SELL_TO_BUYER", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "SG_SETUP_WARES", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "SG_MINION_SLEEP", false)
            ::FLOW_past_lab_00df6794::
            ctr_40 = ctr_40 + 1
            ctr_78 = ctr_78 + 12
        until ctr_40 >= quest:GetStateListCount("AllCreatures")
    end
    if not quest:IsActiveThreadTerminating() then
        ctr_44 = 0
        if 0 < ctr_74 then
            goto LAB_00df6aa2
        end
        goto FLOW_hoist_lab_00df6aa2_1
    end
    goto FLOW_past_lab_00df6aa2
    ::LAB_00df6aa2::
    allCreaturesOffset3 = 0
    if not quest:IsActiveThreadTerminating() then
        ctr_40 = 0
        if quest:GetStateListCount("AllCreatures") ~= 0 then
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                if quest:GetStateListAt("AllCreatures", 0 / 12):GetDefName() ~= "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER" then
                    ctr_40 = ctr_40 + 1
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    addQuestInfoCounter = 0
                    if 0 ~= (quest:GetStateListCount("AllCreatures") * 12) then
                        if quest:GetStateListAt("AllCreatures", 0 / 12):IsEqualTo(quest:GetStateListAt("AllCreatures", 0 / 12)) then
                            quest:StateListErase("AllCreatures", 0 / 12)
                            break
                        end
                        goto FLOW_after_lab_00df6b83
                    end
                    break
                    ctr_40 = ctr_40 + 1
                end
            until ctr_40 >= quest:GetStateListCount("AllCreatures")
        end
        goto LAB_00df6bae
    end
    goto LAB_00df75ec
    ::FLOW_hoist_lab_00df6aa2_1::
    goto LAB_00df6bdb
    ::FLOW_past_lab_00df6aa2::
    goto FLOW_past_lab_00df6bdb
    ::LAB_00df6bdb::
    if not quest:IsActiveThreadTerminating() then
        UpdateLiveEnemies(quest)
        quest:SetStateInt("InitialNumberInRegion", quest:GetStateListCount("AllCreatures"))
        quest:DisplayQuestInfo(true)
        quest:SetStateInt("CounterID", quest:AddQuestInfoCounter("HUD_QUEST_ICON_GUARD", 25, 1.0))
        quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
        local banditFollower = quest:GetThingWithScriptName("TC_BanditFollower")
        addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
        while addQuestInfoCounter ~= -25 and -1 < addQuestInfoCounter + 25 do
            if not quest:NewScriptFrame() then goto LAB_00df7957 end
            ctr_5c = 0
            if quest:GetStateListCount("AllCreatures") == 0 then UpdateLiveEnemies(quest); quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1); addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned"); goto continue_2 end
            ctr_78 = 0
            repeat
                allCreaturesOffset3 = ctr_78
                if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                if quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):MsgIsHitByHero() then goto LAB_00df6e53 end
                if quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                    if not quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):MsgIsHitByHeroSpecialAbility(14) then goto LAB_00df6e53 end
                end
                predicateResult = false
                if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), banditFollower, 10.0) then goto LAB_00df6e53 end
                goto FLOW_past_lab_00df6e53
                ::LAB_00df6e53::
                predicateResult = true
                ::FLOW_past_lab_00df6e53::
                if predicateResult then
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    f_stk_70_1 = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        ctr_74 = 0
                        repeat
                            allCreaturesOffset = ctr_74
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            if not quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), quest:GetStateListAt("AllCreatures", allCreaturesOffset / 12), 10.0) then f_stk_70_1 = (f_stk_70_1 + 1); ctr_74 = (allCreaturesOffset + 12); goto continue_3 end
                            creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset / 12):GetName()
                            if creaturesItem == nil then
                                allCreaturesOffset = ctr_74
                                allCreaturesOffset3 = ctr_78
                            else
                                if creaturesItem == "IsAGuard" then goto LAB_00df6f5f end
                            end
                            goto FLOW_past_lab_00df6f5f
                            ::LAB_00df6f5f::
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            allCreaturesOffset = ctr_74
                            quest:GiveThingBestEnemyTarget(hero, oakvaleVillagerMaleFakeShopkeeper)
                            allCreaturesOffset3 = ctr_78
                            ::FLOW_past_lab_00df6f5f::
                            f_stk_70_1 = f_stk_70_1 + 1
                            ctr_74 = allCreaturesOffset + 12
                            ::continue_3::
                        until f_stk_70_1 >= quest:GetStateListCount("AllCreatures")
                    end
                end
                if quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):MsgIsHitBy("TC_BanditFighter") then goto LAB_00df706d end
                if quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                    if not quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):MsgIsHitByHeroSpecialAbility(14) then goto LAB_00df706d end
                end
                predicateResult = false
                goto FLOW_past_lab_00df706d
                ::LAB_00df706d::
                predicateResult = true
                ::FLOW_past_lab_00df706d::
                if not predicateResult then ctr_5c = ctr_5c + 1; ctr_78 = (allCreaturesOffset3 + 12); goto continue_5 end
                if not quest:IsActiveThreadTerminating() then
                    local getNearestWithScriptName2 = quest:GetNearestWithScriptName(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), "TC_BanditFighter")
                    ctr_74 = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        f_stk_70_2 = 0
                        repeat
                            allCreaturesOffset2 = f_stk_70_2
                            if quest:IsActiveThreadTerminating() then return end
                            if not quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), quest:GetStateListAt("AllCreatures", allCreaturesOffset2 / 12), 10.0) then ctr_74 = ctr_74 + 1; f_stk_70_2 = (allCreaturesOffset2 + 12); goto continue_4 end
                            creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset2 / 12):GetName()
                            if creaturesItem == nil then
                                allCreaturesOffset2 = f_stk_70_2
                                allCreaturesOffset3 = ctr_78
                            else
                                if creaturesItem == "IsAGuard" then goto LAB_00df71ab end
                            end
                            goto FLOW_past_lab_00df71ab
                            ::LAB_00df71ab::
                            allCreaturesOffset2 = f_stk_70_2
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            quest:GiveThingBestEnemyTarget(quest:GetStateListAt("AllCreatures", f_stk_70_2 / 12), getNearestWithScriptName2)
                            allCreaturesOffset3 = ctr_78
                            ::FLOW_past_lab_00df71ab::
                            ctr_74 = ctr_74 + 1
                            f_stk_70_2 = allCreaturesOffset2 + 12
                            ::continue_4::
                        until ctr_74 >= quest:GetStateListCount("AllCreatures")
                    end
                else
                    goto LAB_00df75ec
                end
                ctr_5c = ctr_5c + 1
                ctr_78 = allCreaturesOffset3 + 12
                ::continue_5::
            until ctr_5c >= quest:GetStateListCount("AllCreatures")
            UpdateLiveEnemies(quest)
            quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
            addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            ::continue_2::
        end
        if quest:IsActiveThreadTerminating() then
            if #bsSlumBedBrown01 == 0 then
                -- LAB_00df77e2: (native jump target)
            end
            goto LAB_00df7957
        end
        quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
        scratchValue12 = 0
        quest:StopOverrideMusic(false)
        quest:SetStateBool("MissionSucceeded", true)
        helper_DF9E00(quest, "CS_TRADERCON_EVIL_OUTRO")
        quest:EntityUnsetAsOpinionSource(hero, true)
        quest:SetGuardsIgnoreCrimes(false)
        allCreaturesOffset3 = scratchValue23
        addQuestInfoCounter = 0
        if 0 < scratchValue23 then
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                quest:SetThingAsUsable(bsSlumBedBrown01[scratchValue12 + 1], true)
                addQuestInfoCounter = addQuestInfoCounter + 1
                scratchValue12 = scratchValue12 + 1
            until addQuestInfoCounter >= scratchValue23
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
        if not quest:IsQuestActive("V_SickChild") then quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); goto LAB_00df7957 end
        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
        quest:ActivateQuest("V_SickChildBarrowFields")
        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        ::LAB_00df7957::
        return
    end
    ::FLOW_past_lab_00df6bdb::
    ::FLOW_past_lab_00df656e::
    ::LAB_00df75ec::
    ::LAB_00df75f5::
    do return end
    while true do
        addQuestInfoCounter = addQuestInfoCounter + 12
        if addQuestInfoCounter == (quest:GetStateListCount("AllCreatures") * 12) then break end
        if quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):IsEqualTo(quest:GetStateListAt("AllCreatures", addQuestInfoCounter / 12)) then
            quest:StateListErase("AllCreatures", addQuestInfoCounter / 12)
            break
        end
    end
    ::FLOW_after_lab_00df6b83::
    ::LAB_00df6bae::
    if quest:IsActiveThreadTerminating() then
        if #bsSlumBedBrown01 ~= 0 then return end
        -- TODO(native): goto LAB_00df77e2
    end
    ctr_44 = ctr_44 + 1
    -- TODO(native): xStack_48 = (CCharString)((int)CVar5 + 0xc);
    if ctr_74 <= ctr_44 then goto LAB_00df6bdb end
    goto LAB_00df6aa2
end

-- Q_TraderConflictEvil.Init (retail 0x00df5cd0)
function Init(quest)
    quest:SetStateInt("ScreamOutTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("CounterID", 0)
    quest:SetStateInt("NumberSpawned", 0)
    quest:SetStateInt("InitialNumberInRegion", 0)
    quest:SetStateBool("QuestStartScreened", false)
    quest:SetStateInt("NextTimeToSpawnGuards", 24)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("HeroAttackedBandit", false)
    quest:SetTimer(quest:GetStateInt("ScreamOutTimer"), 15)
    quest:SetStateBool("PlayerEngaged", false)
    quest:AddQuestRegion("Q_TraderConflictEvil", "BarrowFields")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNoDamageBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNoWeaponsBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.TraderEscortNoWeaponsBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNakedBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNakedBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNoDamageBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE", 36, quest:ReadGlobalGameData(SCRIPT_DEF.AmbushScamTricksterApproachProximityHigh), quest:ReadGlobalGameData(SCRIPT_DEF.AmbushScamTricksterHeroProximity), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT", 37, quest:ReadGlobalGameData(SCRIPT_DEF.AmbushScamTricksterRunAwayProximity), quest:ReadGlobalGameData(SCRIPT_DEF.AmbushScamAmbushTriggerProximity), false, "", 0)
    quest:SetMasterGameState("TCEKeepBanditFollowerAlive", true)
    quest:SetMasterGameState("TCEMadeTimeLimit", false)
end

-- Q_TraderConflictEvil.WatchTimeLimit (retail 0x00df7980)
function WatchTimeLimit(quest)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    while not quest:GetMasterGameState("TCETimeLimitBoastTaken") do
        if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        if quest:GetTimer(timerId) == 0 then
            quest:DeregisterTimer(timerId)
            do return end
        end
    end
    quest:SetTimer(timerId, quest:ReadGlobalGameData(SCRIPT_DEF.TCE_TimeLimit))
    local infoElement = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
    quest:DisplayQuestInfo(true)
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then goto LAB_00df7b03 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00df7b03 end
    quest:RemoveQuestInfoElement(infoElement)
    if 0 >= quest:GetTimer(timerId) then goto LAB_00df7b03 end
    if not quest:IsActiveThreadTerminating() then quest:SetMasterGameState("TCEMadeTimeLimit", true); goto LAB_00df7b03 end
    quest:DeregisterTimer(timerId)
    do return end
    quest:SetMasterGameState("TCEMadeTimeLimit", true)
    ::LAB_00df7b03::
    quest:DeregisterTimer(timerId)
end

-- Q_TraderConflictEvil.UpdateLiveEnemies (retail 0x00df9b80)
function UpdateLiveEnemies(quest)
    local isActiveThreadTerminating2, isActiveThreadTerminating3, scratchValue, predicateResult4
    local allCreaturesIndex2, allCreaturesIndex, scratchValue7
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    allCreaturesIndex = 0
    while allCreaturesIndex ~= quest:GetStateListCount("AllCreatures") do
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetDefName() == "CREATURE_DEMON_DOOR_FACE_01" then goto LAB_00df9cb3 end
        if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "TC_BanditFollower" then
            goto LAB_00df9cb3
        end
        if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "TC_BanditFighter" then goto LAB_00df9cb3 end
        isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating2 then
            return isActiveThreadTerminating2
        end
        allCreaturesIndex = allCreaturesIndex + 1
        goto FLOW_past_lab_00df9cb3
        ::LAB_00df9cb3::
        isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating3 then
            return isActiveThreadTerminating3
        end
        quest:StateListErase("AllCreatures", allCreaturesIndex)
        ::FLOW_past_lab_00df9cb3::
    end
    scratchValue7 = 0
    local followers = quest:GetFollowingEntityList(quest:GetHero())
    if #followers ~= 0 then
        scratchValue = 4
        repeat
            local predicateResult = quest:IsActiveThreadTerminating()
            predicateResult4 = predicateResult
            if predicateResult then goto LAB_00df9dc0 end
            allCreaturesIndex2 = 0
            while allCreaturesIndex2 ~= quest:GetStateListCount("AllCreatures") do
                if not followers[(scratchValue - 4) / 12 + 1]:IsEqualTo(quest:GetStateListAt("AllCreatures", allCreaturesIndex2)) then
                    allCreaturesIndex2 = allCreaturesIndex2 + 1
                else
                    quest:StateListErase("AllCreatures", allCreaturesIndex2)
                    break
                    allCreaturesIndex2 = allCreaturesIndex2 + 1
                end
            end
            scratchValue7 = scratchValue7 + 1
            scratchValue = scratchValue + 12
        until scratchValue7 >= #followers
    end
    predicateResult4 = quest:IsActiveThreadTerminating()
    ::LAB_00df9dea::
    do return predicateResult4 end
    ::LAB_00df9dc0::
    goto LAB_00df9dea
end

-- Q_TraderConflictEvil.helper_DF9E00 (retail 0x00df9e00)
function helper_DF9E00(quest, strParam1)
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

