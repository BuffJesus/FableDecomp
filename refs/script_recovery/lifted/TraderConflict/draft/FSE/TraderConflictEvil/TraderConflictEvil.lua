-- Generated native draft: Q_TraderConflictEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar10, CVar15, CVar5, bVar13, bVar2, cVar1, c_stk_7d, ctr_40, ctr_44, ctr_5c, ctr_74, ctr_78, fVar16, f_stk_70, iVar8, iVar9, pCVar4, pCVar6, pPosition, pcVar14, piVar7, r1, r2, r3, r4, uVar17, xStack_18, xStack_34, xStack_38, xStack_74
    local alive = true
    local function __region_LAB_00df77e2()
        bVar13 = xStack_18 == nil
    end
    iVar8 = 0
    quest:AddEntityBinding("TC_GuardSpawnPoint", "TraderConflictEvil/Entities/TC_GuardSpawnPoint")
    quest:AddEntityBinding("TC_BanditFollower", "TraderConflictEvil/Entities/TC_BanditFollower")
    quest:AddEntityBinding("TC_BanditFighter", "TraderConflictEvil/Entities/TC_BanditFighter")
    quest:AddEntityBinding("TC_Villager", "TraderConflictEvil/Entities/TC_Villager")
    quest:AddEntityBinding("IsAGuard", "TraderConflictEvil/Entities/IsAGuard")
    quest:FinalizeEntityBindings()
    bVar13 = quest:IsRegionLoaded("BarrowFields")
    while not bVar13 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar13 = not alive
        if bVar13 then
            return
        end
        bVar13 = quest:IsRegionLoaded("BarrowFields")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar13 = not alive
    if bVar13 then
        return
    end
    quest:DeactivateQuest("V_SickChildBarrowFields", 0)
    pCVar4 = quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS")
    quest:RepopulateVillage(pCVar4)
    pCVar6 = "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL"
    pCVar4 = quest:GetHero()
    quest:EntitySetAsOpinionSource(pCVar4, iVar8)
    quest:SetGuardsIgnoreCrimes(true)
    CVar15 = 0x0
    pCVar4 = quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS")
    quest:EnableGuards(pCVar4, (CVar15 ~= 0))
    xStack_18 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
    CVar5 = #xStack_18
    xStack_34 = CVar5
    iVar9 = 0
    if 0 < CVar5 then
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar13 = not alive
            if bVar13 then goto LAB_00df75f5 end
            quest:SetThingAsUsable(xStack_18[(iVar8) / 0xc + 1], false)
            iVar9 = iVar9 + 1
            iVar8 = iVar8 + 0xc
        until not (iVar9 < CVar5)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar13 = not alive
    if bVar13 then goto LAB_00df75f5 end
    UpdateLiveEnemies(quest)
    pCVar6 = "TC_IntroCSHeroPos"
    pCVar4 = quest:GetHero()
    r1 = quest:GetNearestWithScriptName(pCVar4, pCVar6)
    if not (r1 ~= nil and not r1:IsNull()) then
        xStack_74 = ""
    else
        xStack_74 = r1:GetDataString()
    end
    iVar8 = ((xStack_74 == "SOUTH") and 0 or 1)
    c_stk_7d = not (iVar8 ~= 0)
    if not c_stk_7d then
        alive = not quest:IsActiveThreadTerminating()
        bVar13 = not alive
        if not bVar13 then
            pcVar14 = "CS_TRADERCON_EVIL_INTRO_NORTH"
            goto LAB_00df656e
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar13 = not alive
        if not bVar13 then
            pcVar14 = "CS_TRADERCON_EVIL_INTRO_SOUTH"
            goto LAB_00df656e
        end
    end
    goto FLOW_past_lab_00df656e
    ::LAB_00df656e::
    helper_DF9E00(quest, pcVar14)
    if not quest:GetStateBool("QuestStartScreened") then
        alive = not quest:IsActiveThreadTerminating()
        bVar13 = not alive
        if bVar13 then goto LAB_00df75ec end
        CVar15 = 0x0
        bVar13 = true
        pCVar6 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(pCVar6, bVar13, (CVar15 ~= 0))
        quest:SetStateBool("QuestStartScreened", true)
    end
    quest:FadeScreenIn()
    quest:OverrideMusic(0x17, false, false)
    quest:CreateThread("WatchTimeLimit")  -- native thread body NScript::CQ_TraderConflictEvilScript::WatchTimeLimit: lift it as function WatchTimeLimit(quest)
    ctr_74 = 0
    ctr_40 = 0
    if quest:GetStateListCount("AllCreatures") ~= 0 then
        ctr_78 = 0
        repeat
            CVar5 = ctr_78
            alive = not quest:IsActiveThreadTerminating()
            bVar13 = not alive
            if bVar13 then goto LAB_00df75ec end
            quest:SetCombatNearbyBreakOffRange(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), nil --[[operand lost by the decompiler]])
            quest:EntitySetInFaction(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "FACTION_MONSTERS")
            quest:MiniMapAddMarker(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "HUD_ORB_RED_SMALL")
            quest:EntitySetSleepEnabled(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), false)
            piVar7 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):GetName()
            if piVar7 == nil then
                bVar13 = false
                CVar5 = ctr_78
                if not bVar13 then
                    goto LAB_00df6794
                end
            else
                iVar8 = ((piVar7 == "IsAGuard") and 0 or 1)
                if iVar8 ~= 0 then goto LAB_00df6794 end
            end
            goto FLOW_past_lab_00df6794
            ::LAB_00df6794::
            alive = not quest:IsActiveThreadTerminating()
            bVar13 = not alive
            if bVar13 then goto LAB_00df75ec end
            piVar7 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):GetDefName()
            if piVar7 == nil then
                CVar5 = ctr_78
                c_stk_7d = false
            else
                iVar8 = ((piVar7 == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER") and 0 or 1)
                c_stk_7d = not (iVar8 ~= 0)
            end
            if c_stk_7d then
                alive = not quest:IsActiveThreadTerminating()
                bVar13 = not alive
                if bVar13 then goto LAB_00df75ec end
                CVar15 = 0x0
                pCVar6 = "TC_Villager"
                pPosition = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):GetPos()
                pCVar4 = "CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER"
                r2 = quest:CreateCreature(pCVar4, pPosition, pCVar6)
                fVar16 = 10.0
                ctr_74 = ctr_74 + 1
                quest:SetCombatNearbyBreakOffRange(r2, fVar16)
                quest:EntitySetInFaction(r2, "FACTION_MONSTERS")
                quest:MiniMapAddMarker(r2, "HUD_ORB_RED_SMALL")
                quest:EntitySetSleepEnabled(r2, false)
                quest:EntitySetStategroupEnabled(r2, "SG_MINION_SLEEP", false)
            end
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "SG_MINION_HAWKING_FROM_STALL", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "SG_SELL_TO_BUYER", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "SG_OPEN_AND_CLOSE_SHOP", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "SG_SETUP_WARES", false)
            quest:EntitySetStategroupEnabled(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "SG_MINION_SLEEP", false)
            ::FLOW_past_lab_00df6794::
            ctr_40 = ctr_40 + 1
            ctr_78 = ctr_78 + 0xc
        until not (ctr_40 < (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar13 = not alive
    if not bVar13 then
        ctr_44 = 0
        if 0 < ctr_74 then
            goto LAB_00df6aa2
        end
        goto FLOW_hoist_lab_00df6aa2_1
    end
    goto FLOW_past_lab_00df6aa2
    ::LAB_00df6aa2::
    CVar5 = 0x0
    alive = not quest:IsActiveThreadTerminating()
    bVar13 = not alive
    if not bVar13 then
        ctr_40 = 0
        if quest:GetStateListCount("AllCreatures") ~= 0 then
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar13 = not alive
                if bVar13 then goto LAB_00df75ec end
                piVar7 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):GetDefName()
                iVar8 = ((piVar7 == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER") and 0 or 1)
                c_stk_7d = not (iVar8 ~= 0)
                if c_stk_7d then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar13 = not alive
                    if bVar13 then goto LAB_00df75ec end
                    iVar8 = 0
                    if iVar8 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
                        piVar7 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc)
                        bVar13 = piVar7:IsEqualTo(quest:GetStateListAt("AllCreatures", (iVar8) / 0xc))
                        if bVar13 then
                            quest:StateListErase("AllCreatures", (iVar8) / 0xc)
                            break
                        end
                        goto FLOW_after_lab_00df6b83
                    end
                    break
                end
                ctr_40 = ctr_40 + 1
            until not (ctr_40 < (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc))
        end
        goto LAB_00df6bae
    end
    goto LAB_00df75ec
    ::FLOW_hoist_lab_00df6aa2_1::
    goto LAB_00df6bdb
    ::FLOW_past_lab_00df6aa2::
    goto FLOW_past_lab_00df6bdb
    ::LAB_00df6bdb::
    alive = not quest:IsActiveThreadTerminating()
    bVar13 = not alive
    if not bVar13 then
        UpdateLiveEnemies(quest)
        quest:SetStateInt("InitialNumberInRegion", ((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc)
        quest:DisplayQuestInfo(true)
        iVar8 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_GUARD", 0x19, 1.0)
        quest:SetStateInt("CounterID", iVar8)
        quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 0x19, -1)
        r3 = quest:GetThingWithScriptName("TC_BanditFollower")
        iVar8 = (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
        if iVar8 ~= -0x19 and -1 < iVar8 + 0x19 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar13 = not alive
                if bVar13 then
                    r3 = nil
                    r1 = nil
                    if #xStack_18 == 0 then
                        -- LAB_00df74f2: (native jump target)
                        bVar13 = xStack_18 == nil
                    else
                        bVar13 = xStack_18 == nil
                    end
                    goto LAB_00df7957
                end
                ctr_5c = 0
                if quest:GetStateListCount("AllCreatures") ~= 0 then
                    ctr_78 = 0
                    repeat
                        CVar5 = ctr_78
                        alive = not quest:IsActiveThreadTerminating()
                        bVar13 = not alive
                        if bVar13 then
                            r3 = nil
                            r1 = nil
                            bVar13 = xStack_18 == nil
                            goto LAB_00df7957
                        end
                        cVar1 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):MsgIsHitByHero()
                        if not cVar1 then
                            cVar1 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):MsgIsHitByAnySpecialAbilityFromHero()
                            if cVar1 then
                                cVar1 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):MsgIsHitByHeroSpecialAbility(0xe)
                                if not cVar1 then goto LAB_00df6e53 end
                            end
                            bVar2 = quest:IsDistanceBetweenThingsUnder((quest:GetStateListAt("AllCreatures", (CVar5) / 0xc)), r3, 10.0)
                            bVar13 = false
                            if bVar2 then goto LAB_00df6e53 end
                        else
                            goto LAB_00df6e53
                        end
                        goto FLOW_past_lab_00df6e53
                        ::LAB_00df6e53::
                        bVar13 = true
                        ::FLOW_past_lab_00df6e53::
                        if bVar13 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar13 = not alive
                            if bVar13 then
                                goto LAB_00df75ec
                            end
                            f_stk_70 = 0x0
                            if quest:GetStateListCount("AllCreatures") ~= 0 then
                                ctr_74 = 0
                                repeat
                                    CVar10 = ctr_74
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar13 = not alive
                                    if bVar13 then
                                        goto LAB_00df75ec
                                    end
                                    bVar13 = quest:IsDistanceBetweenThingsUnder((quest:GetStateListAt("AllCreatures", (CVar5) / 0xc)), (quest:GetStateListAt("AllCreatures", (CVar10) / 0xc)), 10.0)
                                    if bVar13 then
                                        piVar7 = quest:GetStateListAt("AllCreatures", (CVar10) / 0xc):GetName()
                                        if piVar7 == nil then
                                            bVar13 = false
                                            CVar10 = ctr_74
                                            CVar5 = ctr_78
                                            if bVar13 then
                                                goto LAB_00df6f5f
                                            end
                                        else
                                            iVar8 = ((piVar7 == "IsAGuard") and 0 or 1)
                                            if iVar8 == 0 then goto LAB_00df6f5f end
                                        end
                                        goto FLOW_past_lab_00df6f5f
                                        ::LAB_00df6f5f::
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar13 = not alive
                                        if bVar13 then
                                            goto LAB_00df75ec
                                        end
                                        xStack_38 = quest:GetStateListRef("AllCreatures")
                                        pCVar4 = quest:GetHero()
                                        CVar10 = ctr_74
                                        quest:GiveThingBestEnemyTarget(pCVar4, r2)
                                        CVar5 = ctr_78
                                        ::FLOW_past_lab_00df6f5f::
                                    end
                                    f_stk_70 = (f_stk_70 + 1)
                                    ctr_74 = (CVar10 + 0xc)
                                until not (f_stk_70 < (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc))
                            end
                        end
                        cVar1 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):MsgIsHitBy("TC_BanditFighter")
                        if not cVar1 then
                            cVar1 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter")
                            if cVar1 then
                                cVar1 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc):MsgIsHitByHeroSpecialAbility(0xe)
                                if not cVar1 then goto LAB_00df706d end
                            end
                            bVar13 = false
                        else
                            goto LAB_00df706d
                        end
                        goto FLOW_past_lab_00df706d
                        ::LAB_00df706d::
                        bVar13 = true
                        ::FLOW_past_lab_00df706d::
                        if bVar13 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar13 = not alive
                            if not bVar13 then
                                r4 = quest:GetNearestWithScriptName(quest:GetStateListAt("AllCreatures", (CVar5) / 0xc), "TC_BanditFighter")
                                ctr_74 = 0
                                if quest:GetStateListCount("AllCreatures") ~= 0 then
                                    f_stk_70 = 0x0
                                    repeat
                                        CVar10 = f_stk_70
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar13 = not alive
                                        if bVar13 then
                                            r4 = nil
                                            r3 = nil
                                            return
                                        end
                                        bVar13 = quest:IsDistanceBetweenThingsUnder((quest:GetStateListAt("AllCreatures", (CVar5) / 0xc)), (quest:GetStateListAt("AllCreatures", (CVar10) / 0xc)), 10.0)
                                        if bVar13 then
                                            piVar7 = quest:GetStateListAt("AllCreatures", (CVar10) / 0xc):GetName()
                                            if piVar7 == nil then
                                                bVar13 = false
                                                CVar10 = f_stk_70
                                                CVar5 = ctr_78
                                                if bVar13 then
                                                    goto LAB_00df71ab
                                                end
                                            else
                                                iVar8 = ((piVar7 == "IsAGuard") and 0 or 1)
                                                if iVar8 == 0 then goto LAB_00df71ab end
                                            end
                                            goto FLOW_past_lab_00df71ab
                                            ::LAB_00df71ab::
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar13 = not alive
                                            CVar10 = f_stk_70
                                            if bVar13 then
                                                goto LAB_00df75ec
                                            end
                                            quest:GiveThingBestEnemyTarget(quest:GetStateListAt("AllCreatures", (f_stk_70) / 0xc), r4)
                                            CVar5 = ctr_78
                                            ::FLOW_past_lab_00df71ab::
                                        end
                                        ctr_74 = ctr_74 + 1
                                        f_stk_70 = (CVar10 + 0xc)
                                    until not (ctr_74 < (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc))
                                end
                                r4 = nil
                                goto LAB_00df7264
                            end
                            goto LAB_00df75ec
                        end
                        ::LAB_00df7264::
                        ctr_5c = ctr_5c + 1
                        ctr_78 = (CVar5 + 0xc)
                    until not (ctr_5c < (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc))
                end
                UpdateLiveEnemies(quest)
                quest:UpdateQuestInfoCounter(quest:GetStateInt("CounterID"), ((((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 0x19, -1)
                iVar8 = (((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")
            until not (iVar8 ~= -0x19 and -1 < iVar8 + 0x19)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar13 = not alive
        if bVar13 then
            r3 = nil
            r1 = nil
            if #xStack_18 == 0 then
                -- LAB_00df77e2: (native jump target)
                bVar13 = xStack_18 == nil
            else
                bVar13 = xStack_18 == nil
            end
            goto LAB_00df7957
        end
        quest:RemoveQuestInfoElement(quest:GetStateInt("CounterID"))
        iVar9 = 0
        quest:StopOverrideMusic(false)
        quest:SetStateBool("MissionSucceeded", true)
        helper_DF9E00(quest, "CS_TRADERCON_EVIL_OUTRO")
        CVar15 = 0x1
        pCVar4 = quest:GetHero()
        quest:EntityUnsetAsOpinionSource(pCVar4, (CVar15 ~= 0))
        quest:SetGuardsIgnoreCrimes(false)
        CVar5 = xStack_34
        iVar8 = 0
        if 0 < xStack_34 then
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar13 = not alive
                if bVar13 then
                    r3 = nil
                    r1 = nil
                    bVar13 = xStack_18 == nil
                    goto LAB_00df7957
                end
                quest:SetThingAsUsable(xStack_18[(iVar9) / 0xc + 1], true)
                iVar8 = iVar8 + 1
                iVar9 = iVar9 + 0xc
            until not (iVar8 < CVar5)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar13 = not alive
        if bVar13 then
            r3 = nil
            r1 = nil
        else
            CVar15 = 0x0
            bVar2 = true
            bVar13 = true
            pCVar6 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar6, bVar13, bVar2, (CVar15 ~= 0))
            bVar13 = quest:IsQuestActive("V_SickChild")
            if bVar13 then
                alive = not quest:IsActiveThreadTerminating()
                bVar13 = not alive
                if bVar13 then
                    -- LAB_00df75e3: (native jump target)
                    goto LAB_00df75ec
                end
                quest:ActivateQuest("V_SickChildBarrowFields")
            end
            uVar17 = 0
            pCVar6 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar6, uVar17)
            r3 = nil
            r1 = nil
        end
        bVar13 = xStack_18 == nil
        ::LAB_00df7957::
        return
    end
    ::FLOW_past_lab_00df6bdb::
    ::FLOW_past_lab_00df656e::
    ::LAB_00df75ec::
    ::LAB_00df75f5::
    do return end
    while true do
        iVar8 = iVar8 + 0xc
        if not (iVar8 ~= (quest:GetStateListCount("AllCreatures") * 0xc)) then break end
        -- LAB_00df6b83: (native jump target)
        piVar7 = quest:GetStateListAt("AllCreatures", (CVar5) / 0xc)
        bVar13 = piVar7:IsEqualTo(quest:GetStateListAt("AllCreatures", (iVar8) / 0xc))
        if bVar13 then
            quest:StateListErase("AllCreatures", (iVar8) / 0xc)
            break
        end
    end
    ::FLOW_after_lab_00df6b83::
    ::LAB_00df6bae::
    alive = not quest:IsActiveThreadTerminating()
    bVar13 = not alive
    if bVar13 then
        r1 = nil
        if #xStack_18 ~= 0 then goto LAB_00df6d70 end
        -- TODO(native): goto LAB_00df77e2
    end
    ctr_44 = ctr_44 + 1
    -- TODO(native): xStack_48 = (CCharString)((int)CVar5 + 0xc);
    if ctr_74 <= ctr_44 then goto LAB_00df6bdb end
    goto LAB_00df6aa2
    ::LAB_00df6d70::
    bVar13 = xStack_18 == nil
    return
end

function Init(quest)
    quest:SetStateInt("ScreamOutTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("CounterID", 0)
    quest:SetStateInt("NumberSpawned", 0)
    quest:SetStateInt("InitialNumberInRegion", 0)
    quest:SetStateBool("QuestStartScreened", false)
    quest:SetStateInt("NextTimeToSpawnGuards", 0x18)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("HeroAttackedBandit", false)
    quest:SetTimer(quest:GetStateInt("ScreamOutTimer"), 0xf)
    quest:SetStateBool("PlayerEngaged", false)
    quest:AddQuestRegion("Q_TraderConflictEvil", "BarrowFields")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x1c8), quest:ReadGlobalGameData(0x1cc), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x1d0), quest:ReadGlobalGameData(0x1d4), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(0x1d8), quest:ReadGlobalGameData(0x1dc), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE", 0x24, quest:ReadGlobalGameData(0x248), quest:ReadGlobalGameData(0x24c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT", 0x25, quest:ReadGlobalGameData(0x250), quest:ReadGlobalGameData(0x254), false, "", 0)
    quest:SetMasterGameState("TCEKeepBanditFollowerAlive", true)
    quest:SetMasterGameState("TCEMadeTimeLimit", false)
end

function WatchTimeLimit(quest)
    local CVar2, bVar3, cVar1, iVar4, xStack_8
    local alive = true
    xStack_8 = quest:RegisterTimer()
    quest:SetTimer(xStack_8, 5)
    cVar1 = quest:GetMasterGameState("TCETimeLimitBoastTaken")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(xStack_8)
            return
        end
        iVar4 = quest:GetTimer(xStack_8)
        if iVar4 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            quest:DeregisterTimer(xStack_8)
            return
        end
        cVar1 = quest:GetMasterGameState("TCETimeLimitBoastTaken")
    end
    quest:SetTimer(xStack_8, quest:ReadGlobalGameData(0xf80))
    iVar4 = quest:AddQuestInfoTimer(xStack_8, "HUD_CLOCK_ICON", 1.0)
    quest:DisplayQuestInfo(true)
    CVar2 = quest:GetStateBool("MissionSucceeded")
    while not CVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00df7b03 end
        CVar2 = quest:GetStateBool("MissionSucceeded")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:RemoveQuestInfoElement(iVar4)
        iVar4 = quest:GetTimer(xStack_8)
        if 0 < iVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(xStack_8)
                return
            end
            quest:SetMasterGameState("TCEMadeTimeLimit", true)
        end
    end
    ::LAB_00df7b03::
    quest:DeregisterTimer(xStack_8)
end

function UpdateLiveEnemies(quest)
    local bVar3, cVar11, iVar5, iVar9, lst_AllCreatures, pTarget, piVar4, piVar6, uVar7, xStack_c
    local alive = true
    quest:StateListClear("AllCreatures")
    lst_AllCreatures = quest:GetAllCreaturesExcludingHero()
    quest:StateListSet("AllCreatures", lst_AllCreatures)
    piVar6 = 0
    if piVar6 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return bVar3
            end
            piVar4 = quest:GetStateListAt("AllCreatures", (piVar6) / 0xc):GetDefName()
            iVar5 = ((piVar4 == "CREATURE_DEMON_DOOR_FACE_01") and 0 or 1)
            cVar11 = not (iVar5 ~= 0)
            if not cVar11 then
                piVar4 = quest:GetStateListAt("AllCreatures", (piVar6) / 0xc):GetName()
                iVar5 = ((piVar4 == "TC_BanditFollower") and 0 or 1)
                if iVar5 ~= 0 then goto LAB_00df9c5a end
                goto LAB_00df9cb3
                ::LAB_00df9c5a::
                piVar4 = quest:GetStateListAt("AllCreatures", (piVar6) / 0xc):GetName()
                iVar5 = ((piVar4 == "TC_BanditFighter") and 0 or 1)
                if iVar5 == 0 then goto LAB_00df9cb3 end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return bVar3
                end
                piVar6 = piVar6 + 0xc
            else
                goto LAB_00df9cb3
            end
            goto FLOW_past_lab_00df9cb3
            ::LAB_00df9cb3::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return bVar3
            end
            quest:StateListErase("AllCreatures", (piVar6) / 0xc)
            ::FLOW_past_lab_00df9cb3::
        until not (piVar6 ~= (quest:GetStateListCount("AllCreatures") * 0xc))
    end
    uVar7 = 0
    pTarget = quest:GetHero()
    xStack_c = quest:GetFollowingEntityList(pTarget)
    if #xStack_c ~= 0 then
        iVar5 = 4
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            iVar9 = bVar3
            if bVar3 then goto LAB_00df9dc0 end
            iVar9 = 0
            if iVar9 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
                repeat
                    cVar11 = xStack_c[(iVar5 - 4) / 0xc + 1]:IsEqualTo(quest:GetStateListAt("AllCreatures", (iVar9) / 0xc))
                    if cVar11 then
                        quest:StateListErase("AllCreatures", (iVar9) / 0xc)
                        break
                    end
                    iVar9 = iVar9 + 0xc
                until not (iVar9 ~= (quest:GetStateListCount("AllCreatures") * 0xc))
            end
            uVar7 = uVar7 + 1
            iVar5 = iVar5 + 0xc
        until not (uVar7 < (#xStack_c))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    iVar9 = bVar3
    ::LAB_00df9dea::
    do return iVar9 end
    ::LAB_00df9dc0::
    goto LAB_00df9dea
end

function helper_DF9E00(quest, native_arg_strParam_1)
    local resources = quest:RetailResources()
    local xStack_20 = resources:NewResource()
    local pScriptObject = xStack_20
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_2c = resources:NewActorMap()
    resources:SetActor(xStack_2c, "HERO", xStack_20)
    local xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(native_arg_strParam_1, xStack_2c, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_2c)
    resources:ReleaseResource(xStack_20)
end

