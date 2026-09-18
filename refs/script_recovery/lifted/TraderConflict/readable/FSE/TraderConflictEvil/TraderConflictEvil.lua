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
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local c_stk_7d_1, ctr_40, ctr_44, ctr_5c, ctr_74, ctr_78, f_stk_70_1, f_stk_70_2, f_stk_70_3
    local f_stk_70_4, addQuestInfoCounter, scratchValue7, scratchValue8, scratchValue9
    local getActiveQuestName, creaturesItem, getNearestWithScriptName, scratchValue10
    local scratchValue11, banditFollower, getNearestWithScriptName2, banditFollower2
    local bsSlumBedBrown01, scratchValue12
    local hero = quest:GetHero()
    addQuestInfoCounter = 0
    quest:AddEntityBinding("TC_GuardSpawnPoint", "TraderConflictEvil/Entities/TC_GuardSpawnPoint")
    quest:AddEntityBinding("TC_BanditFollower", "TraderConflictEvil/Entities/TC_BanditFollower")
    quest:AddEntityBinding("TC_BanditFighter", "TraderConflictEvil/Entities/TC_BanditFighter")
    quest:AddEntityBinding("TC_Villager", "TraderConflictEvil/Entities/TC_Villager")
    quest:AddEntityBinding("IsAGuard", "TraderConflictEvil/Entities/IsAGuard")
    quest:FinalizeEntityBindings()
    scratchValue6 = quest:IsRegionLoaded("BarrowFields")
    while not scratchValue6 do
        if not quest:NewScriptFrame() then return end
        scratchValue6 = quest:IsRegionLoaded("BarrowFields")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuest("V_SickChildBarrowFields", 0)
    quest:RepopulateVillage(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"))
    -- TODO(native): EntitySetAsOpinionSource is not a ForgeFSE binding
    quest:EntitySetAsOpinionSource(hero, "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL")
    quest:SetGuardsIgnoreCrimes(true)
    quest:EnableGuards(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"), false)
    bsSlumBedBrown01 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
    scratchValue5 = #bsSlumBedBrown01
    scratchValue12 = scratchValue5
    scratchValue7 = 0
    if 0 < scratchValue5 then
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
            quest:SetThingAsUsable(bsSlumBedBrown01[addQuestInfoCounter / 12 + 1], false)
            scratchValue7 = scratchValue7 + 1
            addQuestInfoCounter = addQuestInfoCounter + 12
        until scratchValue7 >= scratchValue5
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
    UpdateLiveEnemies(quest)
    getActiveQuestName = "TC_IntroCSHeroPos"
    getNearestWithScriptName = quest:GetNearestWithScriptName(hero, getActiveQuestName)
    if getNearestWithScriptName == nil then
        -- TODO(native): CCharString::CCharString((CCharString *)&xStack_74,(CCharString *)&DAT_0143e8ec);
    else
        getNearestWithScriptName:GetDataString()
    end
    if ctr_74 == nil then
        c_stk_7d_1 = false
    else
        -- TODO(native): iVar8 = CBasicString<char>::Compare(*(void **)ctr_74,"SOUTH");
        c_stk_7d_1 = addQuestInfoCounter == 0
    end
    if not c_stk_7d_1 then
        if not quest:IsActiveThreadTerminating() then
            -- TODO(native): goto LAB_00df656e
        end
    elseif not quest:IsActiveThreadTerminating() then
        -- LAB_00df656e: (native jump target)
        PlayHeroCutscene(quest, "CS_TRADERCON_EVIL_INTRO_SOUTH")
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
                scratchValue5 = ctr_78
                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), nil --[[operand lost by the decompiler]])
                quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "FACTION_MONSTERS")
                quest:MiniMapAddMarker(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "HUD_ORB_RED_SMALL")
                quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), false)
                creaturesItem = quest:GetStateListAt("AllCreatures", scratchValue5 / 12):GetName()
                if creaturesItem == nil then
                    scratchValue5 = ctr_78
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    creaturesItem = quest:GetStateListAt("AllCreatures", scratchValue5 / 12):GetDefName()
                    if creaturesItem == nil then
                        scratchValue5 = ctr_78
                        c_stk_7d_1 = false
                    else
                        c_stk_7d_1 = creaturesItem == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER"
                    end
                    if c_stk_7d_1 then
                        getActiveQuestName = "TC_Villager"
                        scratchValue10 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", scratchValue5 / 12):GetPos(), getActiveQuestName)
                        ctr_74 = ctr_74 + 1
                        quest:SetCombatNearbyBreakOffRange(scratchValue10, 10.0)
                        quest:EntitySetInFaction(scratchValue10, "FACTION_MONSTERS")
                        quest:MiniMapAddMarker(scratchValue10, "HUD_ORB_RED_SMALL")
                        quest:EntitySetSleepEnabled(scratchValue10, false)
                        quest:EntitySetStategroupEnabled(scratchValue10, "SG_MINION_SLEEP", false)
                    end
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_SELL_TO_BUYER", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_SETUP_WARES", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_MINION_SLEEP", false)
                elseif creaturesItem ~= "IsAGuard" then
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    if quest:GetStateListAt("AllCreatures", scratchValue5 / 12):GetDefName() == nil then
                        scratchValue5 = ctr_78
                        c_stk_7d_1 = false
                    end
                    if c_stk_7d_1 then
                        getActiveQuestName = "TC_Villager"
                        scratchValue11 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", scratchValue5 / 12):GetPos(), getActiveQuestName)
                        ctr_74 = ctr_74 + 1
                        quest:SetCombatNearbyBreakOffRange(scratchValue11, 10.0)
                        quest:EntitySetInFaction(scratchValue11, "FACTION_MONSTERS")
                        quest:MiniMapAddMarker(scratchValue11, "HUD_ORB_RED_SMALL")
                        quest:EntitySetSleepEnabled(scratchValue11, false)
                        quest:EntitySetStategroupEnabled(scratchValue11, "SG_MINION_SLEEP", false)
                    end
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_SELL_TO_BUYER", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_SETUP_WARES", false)
                    quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "SG_MINION_SLEEP", false)
                end
                ctr_40 = ctr_40 + 1
                ctr_78 = ctr_78 + 12
            until ctr_40 >= quest:GetStateListCount("AllCreatures")
        end
        if not quest:IsActiveThreadTerminating() then
            ctr_44 = 0
            if 0 < ctr_74 then
                -- LAB_00df6aa2: (native jump target)
                if not quest:IsActiveThreadTerminating() then
                    ctr_40 = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        repeat
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            if ((quest:GetStateListAt("AllCreatures", 0 / 12):GetDefName() == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER") and 0 or 1) == 0 then
                                if 0 ~= (quest:GetStateListCount("AllCreatures") * 12) then
                                    if quest:GetStateListAt("AllCreatures", 0 / 12):IsEqualTo(quest:GetStateListAt("AllCreatures", 0 / 12)) then
                                        quest:StateListErase("AllCreatures", 0 / 12)
                                        break
                                    end
                                    goto FLOW_after_lab_00df6b83
                                end
                                break
                            end
                            ctr_40 = ctr_40 + 1
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
                banditFollower = quest:GetThingWithScriptName("TC_BanditFollower")
                addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
                while addQuestInfoCounter ~= -25 and -1 < addQuestInfoCounter + 25 do
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then
                        if #bsSlumBedBrown01 == 0 then
                            -- LAB_00df74f2: (native jump target)
                        end
                        goto LAB_00df7957
                    end
                    ctr_5c = 0
                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                        ctr_78 = 0
                        repeat
                            scratchValue5 = ctr_78
                            if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                            if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByHero() then
                                if quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                                    if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByHeroSpecialAbility(14) then
                                        scratchValue6 = true
                                        goto FLOW_after_lab_00df6e53
                                    end
                                end
                                scratchValue6 = false
                                if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), banditFollower, 10.0) then
                                    scratchValue6 = true
                                    goto FLOW_after_lab_00df6e53
                                end
                            else
                                -- LAB_00df6e53: (native jump target)
                                scratchValue6 = true
                            end
                            ::FLOW_after_lab_00df6e53::
                            if scratchValue6 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                f_stk_70_1 = 0
                                if quest:GetStateListCount("AllCreatures") ~= 0 then
                                    ctr_74 = 0
                                    repeat
                                        scratchValue = ctr_74
                                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                        if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), quest:GetStateListAt("AllCreatures", scratchValue / 12), 10.0) then
                                            creaturesItem = quest:GetStateListAt("AllCreatures", scratchValue / 12):GetName()
                                            if creaturesItem == nil then
                                                scratchValue = ctr_74
                                                scratchValue5 = ctr_78
                                            elseif creaturesItem == "IsAGuard" then
                                                scratchValue = ctr_74
                                                quest:GiveThingBestEnemyTarget(hero, scratchValue10)
                                                scratchValue5 = ctr_78
                                            end
                                        end
                                        f_stk_70_1 = f_stk_70_1 + 1
                                        ctr_74 = scratchValue + 12
                                    until f_stk_70_1 >= quest:GetStateListCount("AllCreatures")
                                end
                            end
                            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                            if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitBy("TC_BanditFighter") then
                                -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                                if quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                                    if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByHeroSpecialAbility(14) then
                                        scratchValue6 = true
                                        goto FLOW_after_lab_00df706d
                                    end
                                end
                                scratchValue6 = false
                            else
                                -- LAB_00df706d: (native jump target)
                                scratchValue6 = true
                            end
                            ::FLOW_after_lab_00df706d::
                            if scratchValue6 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                getNearestWithScriptName2 = quest:GetNearestWithScriptName(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), "TC_BanditFighter")
                                ctr_74 = 0
                                if quest:GetStateListCount("AllCreatures") ~= 0 then
                                    f_stk_70_2 = 0
                                    repeat
                                        scratchValue2 = f_stk_70_2
                                        if quest:IsActiveThreadTerminating() then return end
                                        if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), quest:GetStateListAt("AllCreatures", scratchValue2 / 12), 10.0) then
                                            creaturesItem = quest:GetStateListAt("AllCreatures", scratchValue2 / 12):GetName()
                                            if creaturesItem == nil then
                                                scratchValue2 = f_stk_70_2
                                                scratchValue5 = ctr_78
                                            elseif creaturesItem == "IsAGuard" then
                                                scratchValue2 = f_stk_70_2
                                                quest:GiveThingBestEnemyTarget(quest:GetStateListAt("AllCreatures", f_stk_70_2 / 12), getNearestWithScriptName2)
                                                scratchValue5 = ctr_78
                                            end
                                        end
                                        ctr_74 = ctr_74 + 1
                                        f_stk_70_2 = scratchValue2 + 12
                                    until ctr_74 >= quest:GetStateListCount("AllCreatures")
                                end
                            end
                            ctr_5c = ctr_5c + 1
                            ctr_78 = scratchValue5 + 12
                        until ctr_5c >= quest:GetStateListCount("AllCreatures")
                    end
                    UpdateLiveEnemies(quest)
                    quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
                    addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
                end
                if quest:IsActiveThreadTerminating() then
                    if #bsSlumBedBrown01 == 0 then
                        -- LAB_00df77e2: (native jump target)
                    end
                    goto LAB_00df7957
                end
                quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
                scratchValue8 = 0
                quest:StopOverrideMusic(false)
                quest:SetStateBool("MissionSucceeded", true)
                PlayHeroCutscene(quest, "CS_TRADERCON_EVIL_OUTRO")
                quest:EntityUnsetAsOpinionSource(hero, true)
                quest:SetGuardsIgnoreCrimes(false)
                addQuestInfoCounter = 0
                if 0 < scratchValue12 then
                    repeat
                        if quest:IsActiveThreadTerminating() then goto LAB_00df7957 end
                        quest:SetThingAsUsable(bsSlumBedBrown01[scratchValue8 + 1], true)
                        addQuestInfoCounter = addQuestInfoCounter + 1
                        scratchValue8 = scratchValue8 + 1
                    until addQuestInfoCounter >= scratchValue12
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
                    if quest:IsQuestActive("V_SickChild") then
                        if quest:IsActiveThreadTerminating() then
                            -- LAB_00df75e3: (native jump target)
                            goto LAB_00df75ec
                        end
                        quest:ActivateQuest("V_SickChildBarrowFields")
                    end
                    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                end
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
        if quest:GetStateListAt("AllCreatures", scratchValue5 / 12):IsEqualTo(quest:GetStateListAt("AllCreatures", addQuestInfoCounter / 12)) then
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
            banditFollower2 = quest:GetThingWithScriptName("TC_BanditFollower")
            addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            while addQuestInfoCounter ~= -25 and -1 < addQuestInfoCounter + 25 do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    if #bsSlumBedBrown01 == 0 then
                        -- LAB_00df74f2_c16: (native jump target)
                    end
                    goto LAB_00df7957_c16
                end
                ctr_5c = 0
                if quest:GetStateListCount("AllCreatures") ~= 0 then
                    ctr_78 = 0
                    repeat
                        scratchValue5 = ctr_78
                        if quest:IsActiveThreadTerminating() then
                            banditFollower2 = nil
                            -- TODO(native): goto LAB_00df74f2_c16
                        end
                        if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByHero() then
                            if quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                                if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByHeroSpecialAbility(14) then return end  -- TODO(native): goto LAB_00df6e53_c16
                            end
                            scratchValue6 = false
                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), banditFollower2, 10.0) then return end  -- TODO(native): goto LAB_00df6e53_c16
                        else
                            -- LAB_00df6e53_c16: (native jump target)
                            scratchValue6 = true
                        end
                        if scratchValue6 then
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00df75e3_c16
                            f_stk_70_3 = 0
                            if quest:GetStateListCount("AllCreatures") ~= 0 then
                                ctr_74 = 0
                                repeat
                                    scratchValue3 = ctr_74
                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00df75e3_c16
                                    if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), quest:GetStateListAt("AllCreatures", scratchValue3 / 12), 10.0) then
                                        creaturesItem = quest:GetStateListAt("AllCreatures", scratchValue3 / 12):GetName()
                                        if creaturesItem == nil then
                                            scratchValue3 = ctr_74
                                            scratchValue5 = ctr_78
                                        else
                                            if creaturesItem == "IsAGuard" then return end  -- TODO(native): goto LAB_00df6f5f_c16
                                        end
                                    end
                                    f_stk_70_3 = f_stk_70_3 + 1
                                    ctr_74 = scratchValue3 + 12
                                until f_stk_70_3 >= quest:GetStateListCount("AllCreatures")
                            end
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitBy("TC_BanditFighter") then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            if quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                                if not quest:GetStateListAt("AllCreatures", scratchValue5 / 12):MsgIsHitByHeroSpecialAbility(14) then return end  -- TODO(native): goto LAB_00df706d_c16
                            end
                            scratchValue6 = false
                        else
                            -- LAB_00df706d_c16: (native jump target)
                            scratchValue6 = true
                        end
                        if scratchValue6 then
                            if not quest:IsActiveThreadTerminating() then
                                ctr_74 = 0
                                if quest:GetStateListCount("AllCreatures") ~= 0 then
                                    f_stk_70_4 = 0
                                    repeat
                                        scratchValue4 = f_stk_70_4
                                        if quest:IsActiveThreadTerminating() then return end
                                        if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue5 / 12), quest:GetStateListAt("AllCreatures", scratchValue4 / 12), 10.0) then
                                            creaturesItem = quest:GetStateListAt("AllCreatures", scratchValue4 / 12):GetName()
                                            if creaturesItem == nil then
                                                scratchValue4 = f_stk_70_4
                                                scratchValue5 = ctr_78
                                            else
                                                if creaturesItem == "IsAGuard" then return end  -- TODO(native): goto LAB_00df71ab_c16
                                            end
                                        end
                                        ctr_74 = ctr_74 + 1
                                        f_stk_70_4 = scratchValue4 + 12
                                    until ctr_74 >= quest:GetStateListCount("AllCreatures")
                                end
                            else
                                -- TODO(native): goto LAB_00df75e3_c16
                            end
                        end
                        ctr_5c = ctr_5c + 1
                        ctr_78 = scratchValue5 + 12
                    until ctr_5c >= quest:GetStateListCount("AllCreatures")
                end
                UpdateLiveEnemies(quest)
                quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
                addQuestInfoCounter = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            end
            if quest:IsActiveThreadTerminating() then
                if #bsSlumBedBrown01 == 0 then
                    -- LAB_00df77e2_c16: (native jump target)
                end
                goto LAB_00df7957_c16
            end
            quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
            scratchValue9 = 0
            quest:StopOverrideMusic(false)
            quest:SetStateBool("MissionSucceeded", true)
            PlayHeroCutscene(quest, "CS_TRADERCON_EVIL_OUTRO")
            quest:EntityUnsetAsOpinionSource(hero, true)
            quest:SetGuardsIgnoreCrimes(false)
            scratchValue5 = getActiveQuestName
            addQuestInfoCounter = 0
            if 0 < getActiveQuestName then
                repeat
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): goto LAB_00df77e2_c16
                    end
                    quest:SetThingAsUsable(bsSlumBedBrown01[scratchValue9 + 1], true)
                    addQuestInfoCounter = addQuestInfoCounter + 1
                    scratchValue9 = scratchValue9 + 1
                until addQuestInfoCounter >= scratchValue5
            end
            if not quest:IsActiveThreadTerminating() then
                quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
                if quest:IsQuestActive("V_SickChild") then
                    if quest:IsActiveThreadTerminating() then
                        -- LAB_00df75e3_c16: (native jump target)
                        goto LAB_00df75ec
                    end
                    quest:ActivateQuest("V_SickChildBarrowFields")
                end
                quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            end
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
    local scratchValue, timerId
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    while quest:GetMasterGameState("TCETimeLimitBoastTaken") == 0 do
        if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        if quest:GetTimer(timerId) == 0 then
            quest:DeregisterTimer(timerId)
            return
        end
    end
    quest:SetTimer(timerId, quest:ReadGlobalGameData(SCRIPT_DEF.TCE_TimeLimit))
    scratchValue = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
    quest:DisplayQuestInfo(true)
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then goto LAB_00df7b03 end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveQuestInfoElement(scratchValue)
        if 0 < quest:GetTimer(timerId) then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            quest:SetMasterGameState("TCEMadeTimeLimit", true)
        end
    end
    ::LAB_00df7b03::
    quest:DeregisterTimer(timerId)
end

-- Q_TraderConflictEvil.UpdateLiveEnemies (retail 0x00df9b80)
function UpdateLiveEnemies(quest)
    local isActiveThreadTerminating, isActiveThreadTerminating2, isActiveThreadTerminating3
    local isActiveThreadTerminating4, isActiveThreadTerminating5, predicateResult, scratchValue
    local predicateResult4, scratchValue6, scratchValue7, scratchValue8, followers
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    scratchValue7 = 0
    while scratchValue7 ~= quest:GetStateListCount("AllCreatures") do
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if ((quest:GetStateListAt("AllCreatures", scratchValue7):GetDefName() == "CREATURE_DEMON_DOOR_FACE_01") and 0 or 1) ~= 0 then
            if quest:GetStateListAt("AllCreatures", scratchValue7):GetName() ~= "TC_BanditFollower" then goto LAB_00df9c5a end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", scratchValue7)
            goto FLOW_after_lab_00df9cb3
            ::LAB_00df9c5a::
            if quest:GetStateListAt("AllCreatures", scratchValue7):GetName() == "TC_BanditFighter" then
                isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating3 then
                    return isActiveThreadTerminating3
                end
                quest:StateListErase("AllCreatures", scratchValue7)
                goto FLOW_after_lab_00df9cb3
            end
            isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            scratchValue7 = scratchValue7 + 1
        else
            isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", scratchValue7)
        end
        ::FLOW_after_lab_00df9cb3::
    end
    scratchValue8 = 0
    followers = quest:GetFollowingEntityList(quest:GetHero())
    if #followers ~= 0 then
        scratchValue = 4
        repeat
            predicateResult = quest:IsActiveThreadTerminating()
            predicateResult4 = predicateResult
            if predicateResult then goto LAB_00df9dc0 end
            scratchValue6 = 0
            while scratchValue6 ~= quest:GetStateListCount("AllCreatures") do
                if followers[(scratchValue - 4) / 12 + 1]:IsEqualTo(quest:GetStateListAt("AllCreatures", scratchValue6)) then
                    quest:StateListErase("AllCreatures", scratchValue6)
                    break
                end
                scratchValue6 = scratchValue6 + 1
            end
            scratchValue8 = scratchValue8 + 1
            scratchValue = scratchValue + 12
        until scratchValue8 >= #followers
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

