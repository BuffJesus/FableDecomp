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
    local allCreaturesOffset, allCreaturesOffset2, allCreaturesOffset3, allCreaturesOffset4
    local allCreaturesOffset5, scratchValue13, c_stk_7d_1, ctr_40, ctr_44, ctr_5c, ctr_74, ctr_78
    local f_stk_70_1, f_stk_70_2, f_stk_70_3, f_stk_70_4, addQuestInfoCounter, scratchValue16
    local scratchValue17, scratchValue18, getActiveQuestName, creaturesItem
    local getNearestWithScriptName, oakvaleVillagerMaleFakeShopkeeper, getNearestWithScriptName3
    local bsSlumBedBrown01, getDataString
    local hero = quest:GetHero()
    local function ReleaseEverything()
        local scratchValue13 = bsSlumBedBrown01 == nil
    end
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
    local scratchValue21 = "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL"
    -- TODO(native): EntitySetAsOpinionSource is not a ForgeFSE binding
    quest:EntitySetAsOpinionSource(hero, scratchValue21)
    quest:SetGuardsIgnoreCrimes(true)
    quest:EnableGuards(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"), false)
    bsSlumBedBrown01 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
    allCreaturesOffset5 = #bsSlumBedBrown01
    local scratchValue33 = allCreaturesOffset5
    scratchValue16 = 0
    if 0 < allCreaturesOffset5 then
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
            quest:SetThingAsUsable(bsSlumBedBrown01[addQuestInfoCounter / 12 + 1], false)
            scratchValue16 = scratchValue16 + 1
            addQuestInfoCounter = addQuestInfoCounter + 12
        until scratchValue16 >= allCreaturesOffset5
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
    UpdateLiveEnemies(quest)
    getActiveQuestName = "TC_IntroCSHeroPos"
    getNearestWithScriptName = quest:GetNearestWithScriptName(hero, getActiveQuestName)
    if getNearestWithScriptName == nil then
        getDataString = ""
    else
        getDataString = getNearestWithScriptName:GetDataString()
    end
    c_stk_7d_1 = getDataString == "SOUTH"
    if not c_stk_7d_1 then
        if not quest:IsActiveThreadTerminating() then
            -- TODO(native): goto LAB_00df656e
        end
    elseif not quest:IsActiveThreadTerminating() then
        local scratchValue23 = "CS_TRADERCON_EVIL_INTRO_SOUTH"
        -- LAB_00df656e: (native jump target)
        PlayHeroCutscene(quest, scratchValue23)
        if not quest:GetStateBool("QuestStartScreened") then
            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
            getActiveQuestName = quest:GetActiveQuestName()
            quest:KickOffQuestStartScreen(getActiveQuestName, true, false)
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
                allCreaturesOffset5 = ctr_78
                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), nil --[[operand lost by the decompiler]])
                quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "FACTION_MONSTERS")
                quest:MiniMapAddMarker(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "HUD_ORB_RED_SMALL")
                quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), false)
                creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):GetName()
                if creaturesItem == nil then
                    allCreaturesOffset5 = ctr_78
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):GetDefName()
                    if creaturesItem == nil then
                        allCreaturesOffset5 = ctr_78
                        c_stk_7d_1 = false
                    else
                        c_stk_7d_1 = creaturesItem == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER"
                    end
                    if c_stk_7d_1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                        getActiveQuestName = "TC_Villager"
                        oakvaleVillagerMaleFakeShopkeeper = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):GetPos(), getActiveQuestName)
                        ctr_74 = ctr_74 + 1
                        quest:SetCombatNearbyBreakOffRange(oakvaleVillagerMaleFakeShopkeeper, 10.0)
                        quest:EntitySetInFaction(oakvaleVillagerMaleFakeShopkeeper, "FACTION_MONSTERS")
                        quest:MiniMapAddMarker(oakvaleVillagerMaleFakeShopkeeper, "HUD_ORB_RED_SMALL")
                        quest:EntitySetSleepEnabled(oakvaleVillagerMaleFakeShopkeeper, false)
                        quest:EntitySetStategroupEnabled(oakvaleVillagerMaleFakeShopkeeper, "SG_MINION_SLEEP", false)
                    end
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_SELL_TO_BUYER", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_SETUP_WARES", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_MINION_SLEEP", false)
                elseif creaturesItem ~= "IsAGuard" then
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    if quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):GetDefName() == nil then
                        allCreaturesOffset5 = ctr_78
                        c_stk_7d_1 = false
                    end
                    if c_stk_7d_1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                        getActiveQuestName = "TC_Villager"
                        local oakvaleVillagerMaleFakeShopkeeper2 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):GetPos(), getActiveQuestName)
                        ctr_74 = ctr_74 + 1
                        quest:SetCombatNearbyBreakOffRange(oakvaleVillagerMaleFakeShopkeeper2, 10.0)
                        quest:EntitySetInFaction(oakvaleVillagerMaleFakeShopkeeper2, "FACTION_MONSTERS")
                        quest:MiniMapAddMarker(oakvaleVillagerMaleFakeShopkeeper2, "HUD_ORB_RED_SMALL")
                        quest:EntitySetSleepEnabled(oakvaleVillagerMaleFakeShopkeeper2, false)
                        quest:EntitySetStategroupEnabled(oakvaleVillagerMaleFakeShopkeeper2, "SG_MINION_SLEEP", false)
                    end
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_SELL_TO_BUYER", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_SETUP_WARES", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "SG_MINION_SLEEP", false)
                end
                ctr_40 = ctr_40 + 1
                ctr_78 = ctr_78 + 12
            until ctr_40 >= quest:GetStateListCount("AllCreatures")
        end
        if not quest:IsActiveThreadTerminating() then
            ctr_44 = 0
            if 0 < ctr_74 then
                -- LAB_00df6aa2: (native jump target)
                allCreaturesOffset5 = 0
                if not quest:IsActiveThreadTerminating() then
                    ctr_40 = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        repeat
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            if quest:GetStateListAt("AllCreatures", 0 / 12):GetDefName() ~= "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER" then ctr_40 = ctr_40 + 1; goto continue_1 end
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
                            ::continue_1::
                        until ctr_40 >= quest:GetStateListCount("AllCreatures")
                    end
                    goto LAB_00df6bae
                end
                goto LAB_00df75ec
            end
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
                        allCreaturesOffset5 = ctr_78
                        if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                        if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByHero() then
                            if quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                                if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByHeroSpecialAbility(14) then
                                    scratchValue13 = true
                                    goto FLOW_after_lab_00df6e53
                                end
                            end
                            scratchValue13 = false
                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), banditFollower, 10.0) then
                                scratchValue13 = true
                                goto FLOW_after_lab_00df6e53
                            end
                        else
                            -- LAB_00df6e53: (native jump target)
                            scratchValue13 = true
                        end
                        ::FLOW_after_lab_00df6e53::
                        if scratchValue13 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            f_stk_70_1 = 0
                            if quest:GetStateListCount("AllCreatures") ~= 0 then
                                ctr_74 = 0
                                repeat
                                    allCreaturesOffset = ctr_74
                                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                    if not quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), quest:GetStateListAt("AllCreatures", allCreaturesOffset / 12), 10.0) then f_stk_70_1 = (f_stk_70_1 + 1); ctr_74 = (allCreaturesOffset + 12); goto continue_3 end
                                    creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset / 12):GetName()
                                    if creaturesItem == nil then
                                        allCreaturesOffset = ctr_74
                                        allCreaturesOffset5 = ctr_78
                                    elseif creaturesItem == "IsAGuard" then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                        allCreaturesOffset = ctr_74
                                        quest:GiveThingBestEnemyTarget(hero, oakvaleVillagerMaleFakeShopkeeper)
                                        allCreaturesOffset5 = ctr_78
                                    end
                                    f_stk_70_1 = f_stk_70_1 + 1
                                    ctr_74 = allCreaturesOffset + 12
                                    ::continue_3::
                                until f_stk_70_1 >= quest:GetStateListCount("AllCreatures")
                            end
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitBy("TC_BanditFighter") then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            if quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                                if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByHeroSpecialAbility(14) then
                                    scratchValue13 = true
                                    goto FLOW_after_lab_00df706d
                                end
                            end
                            scratchValue13 = false
                        else
                            -- LAB_00df706d: (native jump target)
                            scratchValue13 = true
                        end
                        ::FLOW_after_lab_00df706d::
                        if not scratchValue13 then ctr_5c = ctr_5c + 1; ctr_78 = (allCreaturesOffset5 + 12); goto continue_9 end
                        if not quest:IsActiveThreadTerminating() then
                            local getNearestWithScriptName2 = quest:GetNearestWithScriptName(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "TC_BanditFighter")
                            ctr_74 = 0
                            if quest:GetStateListCount("AllCreatures") ~= 0 then
                                f_stk_70_2 = 0
                                repeat
                                    allCreaturesOffset2 = f_stk_70_2
                                    if quest:IsActiveThreadTerminating() then return end
                                    if not quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), quest:GetStateListAt("AllCreatures", allCreaturesOffset2 / 12), 10.0) then ctr_74 = ctr_74 + 1; f_stk_70_2 = (allCreaturesOffset2 + 12); goto continue_4 end
                                    creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset2 / 12):GetName()
                                    if creaturesItem == nil then
                                        allCreaturesOffset2 = f_stk_70_2
                                        allCreaturesOffset5 = ctr_78
                                    elseif creaturesItem == "IsAGuard" then
                                        allCreaturesOffset2 = f_stk_70_2
                                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                        quest:GiveThingBestEnemyTarget(quest:GetStateListAt("AllCreatures", f_stk_70_2 / 12), getNearestWithScriptName2)
                                        allCreaturesOffset5 = ctr_78
                                    end
                                    ctr_74 = ctr_74 + 1
                                    f_stk_70_2 = allCreaturesOffset2 + 12
                                    ::continue_4::
                                until ctr_74 >= quest:GetStateListCount("AllCreatures")
                            end
                        else
                            goto LAB_00df75ec
                        end
                        ctr_5c = ctr_5c + 1
                        ctr_78 = allCreaturesOffset5 + 12
                        ::continue_9::
                    until ctr_5c >= quest:GetStateListCount("AllCreatures")
                    UpdateLiveEnemies(quest)
                    quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
                    addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
                    ::continue_2::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
                scratchValue17 = 0
                quest:StopOverrideMusic(false)
                quest:SetStateBool("MissionSucceeded", true)
                PlayHeroCutscene(quest, "CS_TRADERCON_EVIL_OUTRO")
                quest:EntityUnsetAsOpinionSource(hero, true)
                quest:SetGuardsIgnoreCrimes(false)
                allCreaturesOffset5 = scratchValue33
                addQuestInfoCounter = 0
                if 0 < scratchValue33 then
                    repeat
                        if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                        quest:SetThingAsUsable(bsSlumBedBrown01[scratchValue17 + 1], true)
                        addQuestInfoCounter = addQuestInfoCounter + 1
                        scratchValue17 = scratchValue17 + 1
                    until addQuestInfoCounter >= scratchValue33
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
                if not quest:IsQuestActive("V_SickChild") then quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); goto LAB_00df7957 end
                if quest:IsActiveThreadTerminating() then
                    -- LAB_00df75e3: (native jump target)
                    goto LAB_00df75ec
                end
                quest:ActivateQuest("V_SickChildBarrowFields")
                quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                ::LAB_00df7957::
                return
            end
        end
    end
    ::FLOW_after_lab_00df6bdb::
    ::LAB_00df75ec::
    ::LAB_00df75f5::
    do return end
    while true do
        addQuestInfoCounter = addQuestInfoCounter + 12
        if addQuestInfoCounter == (quest:GetStateListCount("AllCreatures") * 12) then break end
        if quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):IsEqualTo(quest:GetStateListAt("AllCreatures", addQuestInfoCounter / 12)) then
            quest:StateListErase("AllCreatures", addQuestInfoCounter / 12)
            break
        end
    end
    ::FLOW_after_lab_00df6b83::
    ::LAB_00df6bae::
    if quest:IsActiveThreadTerminating() then
        if #bsSlumBedBrown01 ~= 0 then return end
        -- TODO(native): goto LAB_00df7957_c15
    end
    ctr_44 = ctr_44 + 1
    -- TODO(native): xStack_48 = (CCharString)((int)CVar5 + 0xc);
    if ctr_74 <= ctr_44 then
        if not quest:IsActiveThreadTerminating() then
            UpdateLiveEnemies(quest)
            quest:SetStateInt("InitialNumberInRegion", quest:GetStateListCount("AllCreatures"))
            quest:DisplayQuestInfo(true)
            quest:SetStateInt("CounterID", quest:AddQuestInfoCounter("HUD_QUEST_ICON_GUARD", 25, 1.0))
            quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
            local banditFollower2 = quest:GetThingWithScriptName("TC_BanditFollower")
            addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            while addQuestInfoCounter ~= -25 and -1 < addQuestInfoCounter + 25 do
                if not quest:NewScriptFrame() then goto LAB_00df7957_c16 end
                ctr_5c = 0
                if quest:GetStateListCount("AllCreatures") == 0 then UpdateLiveEnemies(quest); quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1); addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned"); goto continue_6 end
                ctr_78 = 0
                repeat
                    allCreaturesOffset5 = ctr_78
                    if quest:IsActiveThreadTerminating() then
                        ReleaseEverything(); goto LAB_00df7957_c16
                    end
                    if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByHero() then
                        if quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                            if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByHeroSpecialAbility(14) then return end  -- TODO(native): goto LAB_00df6e53_c16
                        end
                        scratchValue13 = false
                        if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), banditFollower2, 10.0) then return end  -- TODO(native): goto LAB_00df6e53_c16
                    else
                        -- LAB_00df6e53_c16: (native jump target)
                        scratchValue13 = true
                    end
                    if scratchValue13 then
                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00df75e3_c16
                        f_stk_70_3 = 0
                        if quest:GetStateListCount("AllCreatures") ~= 0 then
                            ctr_74 = 0
                            repeat
                                allCreaturesOffset3 = ctr_74
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00df75e3_c16
                                if not quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12), 10.0) then f_stk_70_3 = (f_stk_70_3 + 1); ctr_74 = (allCreaturesOffset3 + 12); goto continue_7 end
                                creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset3 / 12):GetName()
                                if creaturesItem == nil then
                                    allCreaturesOffset3 = ctr_74
                                    allCreaturesOffset5 = ctr_78
                                else
                                    if creaturesItem == "IsAGuard" then return end  -- TODO(native): goto LAB_00df6f5f_c16
                                end
                                f_stk_70_3 = f_stk_70_3 + 1
                                ctr_74 = allCreaturesOffset3 + 12
                                ::continue_7::
                            until f_stk_70_3 >= quest:GetStateListCount("AllCreatures")
                        end
                    end
                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                    if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitBy("TC_BanditFighter") then
                        -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                        if quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                            if not quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12):MsgIsHitByHeroSpecialAbility(14) then return end  -- TODO(native): goto LAB_00df706d_c16
                        end
                        scratchValue13 = false
                    else
                        -- LAB_00df706d_c16: (native jump target)
                        scratchValue13 = true
                    end
                    if not scratchValue13 then ctr_5c = ctr_5c + 1; ctr_78 = (allCreaturesOffset5 + 12); goto continue_11 end
                    if not quest:IsActiveThreadTerminating() then
                        getNearestWithScriptName3 = quest:GetNearestWithScriptName(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), "TC_BanditFighter")
                        ctr_74 = 0
                        if quest:GetStateListCount("AllCreatures") ~= 0 then
                            f_stk_70_4 = 0
                            repeat
                                allCreaturesOffset4 = f_stk_70_4
                                if quest:IsActiveThreadTerminating() then return end
                                if not quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", allCreaturesOffset5 / 12), quest:GetStateListAt("AllCreatures", allCreaturesOffset4 / 12), 10.0) then ctr_74 = ctr_74 + 1; f_stk_70_4 = (allCreaturesOffset4 + 12); goto continue_8 end
                                creaturesItem = quest:GetStateListAt("AllCreatures", allCreaturesOffset4 / 12):GetName()
                                if creaturesItem == nil then
                                    allCreaturesOffset4 = f_stk_70_4
                                    allCreaturesOffset5 = ctr_78
                                else
                                    if creaturesItem == "IsAGuard" then return end  -- TODO(native): goto LAB_00df71ab_c16
                                end
                                ctr_74 = ctr_74 + 1
                                f_stk_70_4 = allCreaturesOffset4 + 12
                                ::continue_8::
                            until ctr_74 >= quest:GetStateListCount("AllCreatures")
                        end
                    else
                        -- TODO(native): goto LAB_00df75e3_c16
                    end
                    ctr_5c = ctr_5c + 1
                    ctr_78 = allCreaturesOffset5 + 12
                    ::continue_11::
                until ctr_5c >= quest:GetStateListCount("AllCreatures")
                UpdateLiveEnemies(quest)
                quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
                addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
                ::continue_6::
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00df7957_c16 end
            quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
            scratchValue18 = 0
            quest:StopOverrideMusic(false)
            quest:SetStateBool("MissionSucceeded", true)
            PlayHeroCutscene(quest, "CS_TRADERCON_EVIL_OUTRO")
            quest:EntityUnsetAsOpinionSource(hero, true)
            quest:SetGuardsIgnoreCrimes(false)
            allCreaturesOffset5 = getActiveQuestName
            addQuestInfoCounter = 0
            if 0 < getActiveQuestName then
                repeat
                    if quest:IsActiveThreadTerminating() then
                        ReleaseEverything(); goto LAB_00df7957_c16
                    end
                    quest:SetThingAsUsable(bsSlumBedBrown01[scratchValue18 + 1], true)
                    addQuestInfoCounter = addQuestInfoCounter + 1
                    scratchValue18 = scratchValue18 + 1
                until addQuestInfoCounter >= allCreaturesOffset5
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00df7957_c16 end
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
            if not quest:IsQuestActive("V_SickChild") then quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); goto LAB_00df7957_c16 end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00df75e3_c16: (native jump target)
                goto LAB_00df75ec
            end
            quest:ActivateQuest("V_SickChildBarrowFields")
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            ::LAB_00df7957_c16::
            return
        end
        goto FLOW_after_lab_00df6bdb
    end
    -- TODO(native): goto LAB_00df6aa2
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
    while quest:GetMasterGameState("TCETimeLimitBoastTaken") == 0 do
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
    local isActiveThreadTerminating2, scratchValue, predicateResult4, allCreaturesIndex2
    local allCreaturesIndex, scratchValue7
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    allCreaturesIndex = 0
    while allCreaturesIndex ~= quest:GetStateListCount("AllCreatures") do
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetDefName() ~= "CREATURE_DEMON_DOOR_FACE_01" then
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() ~= "TC_BanditFollower" then goto LAB_00df9c5a end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
            goto FLOW_after_lab_00df9cb3
            ::LAB_00df9c5a::
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() == "TC_BanditFighter" then
                local isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating3 then
                    return isActiveThreadTerminating3
                end
                quest:StateListErase("AllCreatures", allCreaturesIndex)
                goto FLOW_after_lab_00df9cb3
            end
            local isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            allCreaturesIndex = allCreaturesIndex + 1
        else
            local isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
        end
        ::FLOW_after_lab_00df9cb3::
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

-- helper 0xDF9E00 (named after its shape)
function PlayHeroCutscene(quest, strParam1)
    quest:StartCutscene({HERO = quest:GetHero()}, {}, true)
    quest:RunCutscene(strParam1, true, false)
    quest:EndCutscene()
end

