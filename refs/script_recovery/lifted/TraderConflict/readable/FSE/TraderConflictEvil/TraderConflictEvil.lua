-- Readable native conversion: Q_TraderConflictEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_TraderConflictEvil.Main (retail 0x00df6010)
function Main(quest)
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, scratchValue9, scratchValue10, scratchValue11
    local scratchValue12, getStateListAt3, getStateListAt6, getStateListAt9, getStateListAt12
    local c_stk_7d_1, ctr_40, ctr_44, ctr_5c, ctr_74, ctr_78, f_stk_70_1, f_stk_70_2, f_stk_70_3
    local f_stk_70_4, scratchValue13, scratchValue14, scratchValue15, scratchValue16, hero
    local scratchValue17, getStateListAt13, getStateListAt14, getStateListAt17, getStateListAt18
    local getStateListAt19, getStateListAt20, scratchValue22, scratchValue23, scratchValue24
    local scratchValue25, scratchValue26, scratchValue27, scratchValue28, scratchValue29
    local scratchValue30, scratchValue31, scratchValue32, scratchValue33, scratchValue34
    local scratchValue36, scratchValue37, getNearestWithScriptName, getAllThingsWithDefName
    local scratchValue40, scratchValue41
    scratchValue13 = 0
    quest:AddEntityBinding("TC_GuardSpawnPoint", "TraderConflictEvil/Entities/TC_GuardSpawnPoint")
    quest:AddEntityBinding("TC_BanditFollower", "TraderConflictEvil/Entities/TC_BanditFollower")
    quest:AddEntityBinding("TC_BanditFighter", "TraderConflictEvil/Entities/TC_BanditFighter")
    quest:AddEntityBinding("TC_Villager", "TraderConflictEvil/Entities/TC_Villager")
    quest:AddEntityBinding("IsAGuard", "TraderConflictEvil/Entities/IsAGuard")
    quest:FinalizeEntityBindings()
    scratchValue12 = quest:IsRegionLoaded("BarrowFields")
    while not scratchValue12 do
        if not quest:NewScriptFrame() then return end
        scratchValue12 = quest:IsRegionLoaded("BarrowFields")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuest("V_SickChildBarrowFields", 0)
    quest:RepopulateVillage(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"))
    hero = quest:GetHero()
    -- TODO(native): EntitySetAsOpinionSource is not a ForgeFSE binding
    quest:EntitySetAsOpinionSource(hero, "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL")
    quest:SetGuardsIgnoreCrimes(true)
    quest:EnableGuards(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"), false)
    getAllThingsWithDefName = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
    scratchValue5 = #getAllThingsWithDefName
    scratchValue41 = scratchValue5
    scratchValue14 = 0
    if 0 < scratchValue5 then
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
            quest:SetThingAsUsable(getAllThingsWithDefName[scratchValue13 / 12 + 1], false)
            scratchValue14 = scratchValue14 + 1
            scratchValue13 = scratchValue13 + 12
        until scratchValue14 >= scratchValue5
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00df75f5 end
    UpdateLiveEnemies(quest)
    scratchValue17 = "TC_IntroCSHeroPos"
    if not (xStack_30 ~= nil and not xStack_30:IsNull()) then
        -- TODO(native): CCharString::CCharString((CCharString *)&xStack_74,(CCharString *)&DAT_0143e8ec);
    else
        xStack_30:GetDataString()
    end
    if ctr_74 == nil then
        c_stk_7d_1 = false
    else
        -- TODO(native): iVar8 = CBasicString<char>::Compare(*(void **)ctr_74,"SOUTH");
        c_stk_7d_1 = scratchValue13 == 0
    end
    if not c_stk_7d_1 then
        if not quest:IsActiveThreadTerminating() then
            -- TODO(native): goto LAB_00df656e
        end
    else
        if not quest:IsActiveThreadTerminating() then
            -- LAB_00df656e: (native jump target)
            helper_DF9E00(quest)
            if not quest:GetStateBool("QuestStartScreened") then
                if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                scratchValue17 = quest:GetActiveQuestName()
                quest:KickOffQuestStartScreen(scratchValue17, true, false)
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
                    scratchValue6 = ctr_78
                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                    quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), ctr_78)
                    quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "FACTION_MONSTERS")
                    quest:MiniMapAddMarker(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "HUD_ORB_RED_SMALL")
                    quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), false)
                    getStateListAt13 = quest:GetStateListAt("AllCreatures", scratchValue6 / 12):GetName()
                    if getStateListAt13 == nil then
                        scratchValue7 = ctr_78
                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                        getStateListAt14 = quest:GetStateListAt("AllCreatures", scratchValue7 / 12):GetDefName()
                        if getStateListAt14 == nil then
                            scratchValue13 = 42
                            c_stk_7d_1 = true
                            repeat
                                scratchValue7 = ctr_78
                                if scratchValue13 == 0 then break end
                                scratchValue13 = scratchValue13 - 1
                                -- TODO(native): c_stk_7d = *pcVar14 == *pcVar11
                                c_stk_7d_1 = nil --[[unresolved native value]]
                            until not c_stk_7d_1
                        else
                            c_stk_7d_1 = getStateListAt14 == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER"
                        end
                        if c_stk_7d_1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            scratchValue17 = "TC_Villager"
                            scratchValue36 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", scratchValue7 / 12):GetPos(), scratchValue17)
                            ctr_74 = ctr_74 + 1
                            quest:SetCombatNearbyBreakOffRange(scratchValue36, 10.0)
                            quest:EntitySetInFaction(scratchValue36, "FACTION_MONSTERS")
                            quest:MiniMapAddMarker(scratchValue36, "HUD_ORB_RED_SMALL")
                            quest:EntitySetSleepEnabled(scratchValue36, false)
                            quest:EntitySetStategroupEnabled(scratchValue36, "SG_MINION_SLEEP", false)
                        end
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue7 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue7 / 12), "SG_SELL_TO_BUYER", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue7 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue7 / 12), "SG_SETUP_WARES", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue7 / 12), "SG_MINION_SLEEP", false)
                    elseif getStateListAt13 ~= "IsAGuard" then
                        if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                        if quest:GetStateListAt("AllCreatures", scratchValue6 / 12):GetDefName() == nil then
                            scratchValue13 = 42
                            c_stk_7d_1 = true
                            repeat
                                scratchValue6 = ctr_78
                                if scratchValue13 == 0 then break end
                                scratchValue13 = scratchValue13 - 1
                                -- TODO(native): c_stk_7d = *pcVar14 == *pcVar11
                                c_stk_7d_1 = nil --[[unresolved native value]]
                            until not c_stk_7d_1
                        end
                        if c_stk_7d_1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                            scratchValue17 = "TC_Villager"
                            scratchValue37 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", quest:GetStateListAt("AllCreatures", scratchValue6 / 12):GetPos(), scratchValue17)
                            ctr_74 = ctr_74 + 1
                            quest:SetCombatNearbyBreakOffRange(scratchValue37, 10.0)
                            quest:EntitySetInFaction(scratchValue37, "FACTION_MONSTERS")
                            quest:MiniMapAddMarker(scratchValue37, "HUD_ORB_RED_SMALL")
                            quest:EntitySetSleepEnabled(scratchValue37, false)
                            quest:EntitySetStategroupEnabled(scratchValue37, "SG_MINION_SLEEP", false)
                        end
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "SG_MINION_HAWKING_FROM_STALL", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "SG_SELL_TO_BUYER", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "SG_OPEN_AND_CLOSE_SHOP", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "SG_SETUP_WARES", false)
                        quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", scratchValue6 / 12), "SG_MINION_SLEEP", false)
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
                                        -- TODO(native): piVar7 = *(CVar5 + 4 + quest:GetStateListRef("AllCreatures"))
--[[unresolved native value]]
                                        -- TODO(native): bVar13 = (**(*piVar7 + 0x138))(piVar7,*(iVar8 + 4))
                                        scratchValue12 = nil --[[unresolved native value]]
                                        if scratchValue12 then
                                            -- TODO(native): std__vector__pop_back(quest:GetStateListRef("AllCreatures"),iVar8);
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
                    scratchValue13 = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
                    while scratchValue13 ~= -25 and -1 < scratchValue13 + 25 do
                        quest:NewScriptFrame()
                        if quest:IsActiveThreadTerminating() then
                            scratchValue23 = getAllThingsWithDefName
                            if getAllThingsWithDefName == pu_stk_14 then
                                -- LAB_00df74f2: (native jump target)
                            else
                                repeat
                                    -- TODO(native): (**(code **)*puVar12)();
                                    scratchValue23 = scratchValue23 + 3
                                until scratchValue23 == scratchValue27
                            end
                            goto LAB_00df7957
                        end
                        ctr_5c = 0
                        if quest:GetStateListCount("AllCreatures") ~= 0 then
                            ctr_78 = 0
                            repeat
                                scratchValue8 = ctr_78
                                if quest:IsActiveThreadTerminating() then
                                    scratchValue28 = getAllThingsWithDefName
                                    while scratchValue28 ~= scratchValue22 do
                                        -- TODO(native): (**(code **)*puVar3)();
                                        scratchValue28 = scratchValue28 + 3
                                    end
                                    goto LAB_00df7957
                                end
                                if not quest:GetStateListAt("AllCreatures", scratchValue8 / 12):MsgIsHitByHero() then
                                    if quest:GetStateListAt("AllCreatures", scratchValue8 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                                        getStateListAt3 = quest:GetStateListAt("AllCreatures", scratchValue8 / 12):MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                        if not getStateListAt3 then
                                            scratchValue12 = true
                                            goto FLOW_after_lab_00df6e53
                                        end
                                    end
                                    scratchValue12 = false
                                    if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue8 / 12), scratchValue40, 10.0) then
                                        scratchValue12 = true
                                        goto FLOW_after_lab_00df6e53
                                    end
                                else
                                    -- LAB_00df6e53: (native jump target)
                                    scratchValue12 = true
                                end
                                ::FLOW_after_lab_00df6e53::
                                if scratchValue12 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                    f_stk_70_1 = 0
                                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                                        ctr_74 = 0
                                        repeat
                                            scratchValue = ctr_74
                                            if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue8 / 12), quest:GetStateListAt("AllCreatures", scratchValue / 12), 10.0) then
                                                getStateListAt17 = quest:GetStateListAt("AllCreatures", scratchValue / 12):GetName()
                                                if getStateListAt17 == nil then
                                                    scratchValue = ctr_74
                                                    scratchValue8 = ctr_78
                                                elseif getStateListAt17 == "IsAGuard" then
                                                    scratchValue = ctr_74
                                                    quest:GiveThingBestEnemyTarget(quest:GetHero(), scratchValue36)
                                                    scratchValue8 = ctr_78
                                                end
                                            end
                                            f_stk_70_1 = f_stk_70_1 + 1
                                            ctr_74 = scratchValue + 12
                                        until f_stk_70_1 >= quest:GetStateListCount("AllCreatures")
                                    end
                                end
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if not quest:GetStateListAt("AllCreatures", scratchValue8 / 12):MsgIsHitBy("TC_BanditFighter") then
                                    -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                                    if quest:GetStateListAt("AllCreatures", scratchValue8 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                                        getStateListAt6 = quest:GetStateListAt("AllCreatures", scratchValue8 / 12):MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                        if not getStateListAt6 then
                                            scratchValue12 = true
                                            goto FLOW_after_lab_00df706d
                                        end
                                    end
                                    scratchValue12 = false
                                else
                                    -- LAB_00df706d: (native jump target)
                                    scratchValue12 = true
                                end
                                ::FLOW_after_lab_00df706d::
                                if scratchValue12 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00df75ec end
                                    getNearestWithScriptName = quest:GetNearestWithScriptName(quest:GetStateListAt("AllCreatures", scratchValue8 / 12), "TC_BanditFighter")
                                    ctr_74 = 0
                                    if quest:GetStateListCount("AllCreatures") ~= 0 then
                                        f_stk_70_2 = 0
                                        repeat
                                            scratchValue2 = f_stk_70_2
                                            if quest:IsActiveThreadTerminating() then return end
                                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue8 / 12), quest:GetStateListAt("AllCreatures", scratchValue2 / 12), 10.0) then
                                                getStateListAt18 = quest:GetStateListAt("AllCreatures", scratchValue2 / 12):GetName()
                                                if getStateListAt18 == nil then
                                                    scratchValue2 = f_stk_70_2
                                                    scratchValue8 = ctr_78
                                                elseif getStateListAt18 == "IsAGuard" then
                                                    scratchValue2 = f_stk_70_2
                                                    quest:GiveThingBestEnemyTarget(quest:GetStateListAt("AllCreatures", f_stk_70_2 / 12), getNearestWithScriptName)
                                                    scratchValue8 = ctr_78
                                                end
                                            end
                                            ctr_74 = ctr_74 + 1
                                            f_stk_70_2 = scratchValue2 + 12
                                        until ctr_74 >= quest:GetStateListCount("AllCreatures")
                                    end
                                end
                                ctr_5c = ctr_5c + 1
                                ctr_78 = scratchValue8 + 12
                            until ctr_5c >= quest:GetStateListCount("AllCreatures")
                        end
                        UpdateLiveEnemies(quest)
                        quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
                        scratchValue13 = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
                    end
                    if quest:IsActiveThreadTerminating() then
                        scratchValue24 = getAllThingsWithDefName
                        if getAllThingsWithDefName == pu_stk_14 then
                            -- LAB_00df77e2: (native jump target)
                        else
                            repeat
                                -- TODO(native): (**(code **)*puVar12)();
                                scratchValue24 = scratchValue24 + 3
                            until scratchValue24 == scratchValue27
                        end
                        goto LAB_00df7957
                    end
                    quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
                    scratchValue15 = 0
                    quest:StopOverrideMusic(false)
                    quest:SetStateBool("MissionSucceeded", true)
                    helper_DF9E00(quest, "CS_TRADERCON_EVIL_OUTRO")
                    quest:EntityUnsetAsOpinionSource(quest:GetHero(), true)
                    quest:SetGuardsIgnoreCrimes(false)
                    scratchValue9 = scratchValue41
                    scratchValue13 = 0
                    if 0 < scratchValue41 then
                        repeat
                            if quest:IsActiveThreadTerminating() then
                                scratchValue29 = getAllThingsWithDefName
                                while scratchValue29 ~= scratchValue22 do
                                    -- TODO(native): (**(code **)*puVar3)();
                                    scratchValue29 = scratchValue29 + 3
                                end
                                goto LAB_00df7957
                            end
                            quest:SetThingAsUsable(getAllThingsWithDefName[scratchValue15 + 1], true)
                            scratchValue13 = scratchValue13 + 1
                            scratchValue15 = scratchValue15 + 1
                        until scratchValue13 >= scratchValue9
                    end
                    if quest:IsActiveThreadTerminating() then
                        scratchValue30 = getAllThingsWithDefName
                        while scratchValue30 ~= scratchValue22 do
                            -- TODO(native): (**(code **)*puVar3)();
                            scratchValue30 = scratchValue30 + 3
                        end
                    else
                        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
                        if quest:IsQuestActive("V_SickChild") then
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00df75e3: (native jump target)
                                goto LAB_00df75ec
                            end
                            quest:ActivateQuest("V_SickChildBarrowFields")
                        end
                        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                        scratchValue31 = getAllThingsWithDefName
                        while scratchValue31 ~= scratchValue22 do
                            -- TODO(native): (**(code **)*puVar3)();
                            scratchValue31 = scratchValue31 + 3
                        end
                    end
                    ::LAB_00df7957::
                    return
                end
            end
        end
    end
    ::FLOW_after_lab_00df6bdb::
    ::LAB_00df75ec::
    ::LAB_00df75f5::
    do return end
    while true do
        scratchValue13 = scratchValue13 + 12
        if scratchValue13 == (quest:GetStateListCount("AllCreatures") * 12) then break end
        -- TODO(native): piVar7 = *(CVar5 + 4 + quest:GetStateListRef("AllCreatures"))
--[[unresolved native value]]
        -- TODO(native): bVar13 = (**(*piVar7 + 0x138))(piVar7,*(iVar8 + 4))
        scratchValue12 = nil --[[unresolved native value]]
        if scratchValue12 then
            -- TODO(native): std__vector__pop_back(quest:GetStateListRef("AllCreatures"),iVar8);
            break
        end
    end
    ::FLOW_after_lab_00df6b83::
    ::LAB_00df6bae::
    if quest:IsActiveThreadTerminating() then
        scratchValue22 = getAllThingsWithDefName
        if getAllThingsWithDefName ~= pu_stk_14 then goto LAB_00df6d70 end
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
            scratchValue13 = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            while scratchValue13 ~= -25 and -1 < scratchValue13 + 25 do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    scratchValue25 = getAllThingsWithDefName
                    if getAllThingsWithDefName == pu_stk_14 then
                        -- LAB_00df74f2_c16: (native jump target)
                    else
                        repeat
                            -- TODO(native): (**(code **)*puVar12)();
                            scratchValue25 = scratchValue25 + 3
                        until scratchValue25 == scratchValue27
                    end
                    goto LAB_00df7957_c16
                end
                ctr_5c = 0
                if quest:GetStateListCount("AllCreatures") ~= 0 then
                    ctr_78 = 0
                    repeat
                        scratchValue10 = ctr_78
                        if quest:IsActiveThreadTerminating() then
                            scratchValue40 = nil
                            scratchValue27 = getAllThingsWithDefName
                            while scratchValue27 ~= scratchValue22 do
                                -- TODO(native): (**(code **)*puVar3)();
                                scratchValue27 = scratchValue27 + 3
                            end
                            -- TODO(native): goto LAB_00df74f2_c16
                        end
                        if not quest:GetStateListAt("AllCreatures", scratchValue10 / 12):MsgIsHitByHero() then
                            if quest:GetStateListAt("AllCreatures", scratchValue10 / 12):MsgIsHitByAnySpecialAbilityFromHero() then
                                getStateListAt9 = quest:GetStateListAt("AllCreatures", scratchValue10 / 12):MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not getStateListAt9 then return end  -- TODO(native): goto LAB_00df6e53_c16
                            end
                            scratchValue12 = false
                            if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue10 / 12), scratchValue40, 10.0) then return end  -- TODO(native): goto LAB_00df6e53_c16
                        else
                            -- LAB_00df6e53_c16: (native jump target)
                            scratchValue12 = true
                        end
                        if scratchValue12 then
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00df75e3_c16
                            f_stk_70_3 = 0
                            if quest:GetStateListCount("AllCreatures") ~= 0 then
                                ctr_74 = 0
                                repeat
                                    scratchValue3 = ctr_74
                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00df75e3_c16
                                    if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue10 / 12), quest:GetStateListAt("AllCreatures", scratchValue3 / 12), 10.0) then
                                        getStateListAt19 = quest:GetStateListAt("AllCreatures", scratchValue3 / 12):GetName()
                                        if getStateListAt19 == nil then
                                            scratchValue3 = ctr_74
                                            scratchValue10 = ctr_78
                                        else
                                            if getStateListAt19 == "IsAGuard" then return end  -- TODO(native): goto LAB_00df6f5f_c16
                                        end
                                    end
                                    f_stk_70_3 = f_stk_70_3 + 1
                                    ctr_74 = scratchValue3 + 12
                                until f_stk_70_3 >= quest:GetStateListCount("AllCreatures")
                            end
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        if not quest:GetStateListAt("AllCreatures", scratchValue10 / 12):MsgIsHitBy("TC_BanditFighter") then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            if quest:GetStateListAt("AllCreatures", scratchValue10 / 12):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") then
                                getStateListAt12 = quest:GetStateListAt("AllCreatures", scratchValue10 / 12):MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not getStateListAt12 then return end  -- TODO(native): goto LAB_00df706d_c16
                            end
                            scratchValue12 = false
                        else
                            -- LAB_00df706d_c16: (native jump target)
                            scratchValue12 = true
                        end
                        if scratchValue12 then
                            if not quest:IsActiveThreadTerminating() then
                                ctr_74 = 0
                                if quest:GetStateListCount("AllCreatures") ~= 0 then
                                    f_stk_70_4 = 0
                                    repeat
                                        scratchValue4 = f_stk_70_4
                                        if quest:IsActiveThreadTerminating() then return end
                                        if quest:IsDistanceBetweenThingsUnder(quest:GetStateListAt("AllCreatures", scratchValue10 / 12), quest:GetStateListAt("AllCreatures", scratchValue4 / 12), 10.0) then
                                            getStateListAt20 = quest:GetStateListAt("AllCreatures", scratchValue4 / 12):GetName()
                                            if getStateListAt20 == nil then
                                                scratchValue4 = f_stk_70_4
                                                scratchValue10 = ctr_78
                                            else
                                                if getStateListAt20 == "IsAGuard" then return end  -- TODO(native): goto LAB_00df71ab_c16
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
                        ctr_78 = scratchValue10 + 12
                    until ctr_5c >= quest:GetStateListCount("AllCreatures")
                end
                UpdateLiveEnemies(quest)
                quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25, -1)
                scratchValue13 = (quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            end
            if quest:IsActiveThreadTerminating() then
                scratchValue26 = getAllThingsWithDefName
                if getAllThingsWithDefName == pu_stk_14 then
                    -- LAB_00df77e2_c16: (native jump target)
                else
                    repeat
                        -- TODO(native): (**(code **)*puVar12)();
                        scratchValue26 = scratchValue26 + 3
                    until scratchValue26 == scratchValue27
                end
                goto LAB_00df7957_c16
            end
            quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
            scratchValue16 = 0
            quest:StopOverrideMusic(false)
            quest:SetStateBool("MissionSucceeded", true)
            helper_DF9E00(quest, "CS_TRADERCON_EVIL_OUTRO")
            quest:EntityUnsetAsOpinionSource(quest:GetHero(), true)
            quest:SetGuardsIgnoreCrimes(false)
            scratchValue11 = scratchValue17
            scratchValue13 = 0
            if 0 < scratchValue17 then
                repeat
                    if quest:IsActiveThreadTerminating() then
                        scratchValue32 = getAllThingsWithDefName
                        while scratchValue32 ~= scratchValue22 do
                            -- TODO(native): (**(code **)*puVar3)();
                            scratchValue32 = scratchValue32 + 3
                        end
                        -- TODO(native): goto LAB_00df77e2_c16
                    end
                    quest:SetThingAsUsable(getAllThingsWithDefName[scratchValue16 + 1], true)
                    scratchValue13 = scratchValue13 + 1
                    scratchValue16 = scratchValue16 + 1
                until scratchValue13 >= scratchValue11
            end
            if quest:IsActiveThreadTerminating() then
                scratchValue33 = getAllThingsWithDefName
                while scratchValue33 ~= scratchValue22 do
                    -- TODO(native): (**(code **)*puVar3)();
                    scratchValue33 = scratchValue33 + 3
                end
            else
                quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
                if quest:IsQuestActive("V_SickChild") then
                    if quest:IsActiveThreadTerminating() then
                        -- LAB_00df75e3_c16: (native jump target)
                        goto LAB_00df75ec
                    end
                    quest:ActivateQuest("V_SickChildBarrowFields")
                end
                quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                scratchValue34 = getAllThingsWithDefName
                while scratchValue34 ~= scratchValue22 do
                    -- TODO(native): (**(code **)*puVar3)();
                    scratchValue34 = scratchValue34 + 3
                end
            end
            ::LAB_00df7957_c16::
            return
        end
        goto FLOW_after_lab_00df6bdb
    end
    -- TODO(native): goto LAB_00df6aa2
    ::LAB_00df6d70::
    repeat
        -- TODO(native): (**(code **)*puVar12)();
        scratchValue22 = scratchValue22 + 3
    until scratchValue22 == scratchValue27
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(456), quest:ReadGlobalGameData(460), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(464), quest:ReadGlobalGameData(468), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(472), quest:ReadGlobalGameData(476), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE", 36, quest:ReadGlobalGameData(584), quest:ReadGlobalGameData(588), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT", 37, quest:ReadGlobalGameData(592), quest:ReadGlobalGameData(596), false, "", 0)
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
    quest:SetTimer(timerId, quest:ReadGlobalGameData(3968))
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
    local isActiveThreadTerminating4, isActiveThreadTerminating5, predicateResult, scratchValue5
    local scratchValue6, scratchValue7, scratchValue8, scratchValue9, getFollowingEntityList
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    scratchValue8 = 0
    while scratchValue8 ~= quest:GetStateListCount("AllCreatures") do
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if ((quest:GetStateListAt("AllCreatures", scratchValue8):GetDefName() == "CREATURE_DEMON_DOOR_FACE_01") and 0 or 1) ~= 0 then
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "TC_BanditFollower" then goto LAB_00df9c5a end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00df9cb3
            ::LAB_00df9c5a::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() == "TC_BanditFighter" then
                isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating3 then
                    return isActiveThreadTerminating3
                end
                quest:StateListErase("AllCreatures", scratchValue8)
                goto FLOW_after_lab_00df9cb3
            end
            isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            scratchValue8 = scratchValue8 + 1
        else
            isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", scratchValue8)
        end
        ::FLOW_after_lab_00df9cb3::
    end
    scratchValue9 = 0
    getFollowingEntityList = quest:GetFollowingEntityList(quest:GetHero())
    if #getFollowingEntityList ~= 0 then
        scratchValue5 = 4
        repeat
            predicateResult = quest:IsActiveThreadTerminating()
            scratchValue6 = predicateResult
            if predicateResult then goto LAB_00df9dc0 end
            scratchValue7 = 0
            while scratchValue7 ~= quest:GetStateListCount("AllCreatures") do
                if getFollowingEntityList[(scratchValue5 - 4) / 12 + 1]:IsEqualTo(quest:GetStateListAt("AllCreatures", scratchValue7)) then
                    quest:StateListErase("AllCreatures", scratchValue7)
                    break
                end
                scratchValue7 = scratchValue7 + 1
            end
            scratchValue9 = scratchValue9 + 1
            scratchValue5 = scratchValue5 + 12
        until scratchValue9 >= #getFollowingEntityList
    end
    scratchValue6 = quest:IsActiveThreadTerminating()
    ::LAB_00df9dea::
    do return scratchValue6 end
    ::LAB_00df9dc0::
    goto LAB_00df9dea
end

-- Q_TraderConflictEvil.helper_DF9E00 (retail 0x00df9e00)
function helper_DF9E00(quest, strParam1)
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

