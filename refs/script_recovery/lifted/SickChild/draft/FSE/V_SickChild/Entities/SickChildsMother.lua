-- Generated native draft: SickChildsMother. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local bFlag, bVar10, bVar5, bVar6, bVar7, bVar8, cVar9, dist, fVar4, f_stk_84, fret_0, fret_00, fret_01, iVar13, pCVar1, pCVar11, pCVar12, pCVar17, pObjectiveText, pQuestName, pcVar14, uVar15, uVar16, xStack_34, xStack_4c, xStack_60, xStack_94, xStack_98, xStack_9c, xStack_a8
    local alive = true
    local function __cleanup_LAB_00ece3f7()
        quest:PauseAllNonScriptedEntities(resources:MemberResource("seh_Mother"))
        resources:DestroyMovie(xStack_94)
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar8 = not alive
    if not bVar8 then
        pCVar1 = resources:MemberResource("seh_Mother")
        resources:PrepareResource(pCVar1)
        bVar8 = resources:TryAcquire(resources:MemberResource("seh_Mother"), me, 4)
        while not bVar8 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if bVar8 then
                return
            end
            bVar8 = resources:TryAcquire(resources:MemberResource("seh_Mother"), me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar8 = not alive
        if not bVar8 then
            xStack_9c = quest:ReadGlobalGameDataString(0x744)
            pCVar11 = quest:GetHero()
            bVar8 = quest:IsObjectInThingsPossession(xStack_9c, pCVar11)
            if (bVar8) or (not quest:GetStateBool("MotherIntroDone")) then
                bVar8 = true
            end
            bVar7 = false
            bVar6 = false
            bVar5 = false
            if bVar8 then
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                if bVar8 then
                    return
                end
                quest:SetThingHasInformation(me, false, true, false)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if not bVar8 then
                repeat
                    if not quest:GetStateBool("MotherIntroDone") then
                        dist = 7.0
                        pCVar11 = quest:GetHero()
                        bVar8 = quest:IsDistanceBetweenThingsUnder(me, pCVar11, dist)
                        if bVar8 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                return
                            end
                            xStack_34 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            require("V_SickChild.native_quest_helpers").helper_ECE460(quest, me)
                            bFlag = 0x0
                            pCVar12 = quest:GetActiveQuestName()
                            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SICK_CHILD", pCVar12, (bFlag ~= 0))
                            pObjectiveText = "TEXT_QUEST_SICK_CHILD_OBJECTIVE_01"
                            pQuestName = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pQuestName, pObjectiveText, "", "")
                            quest:SetStateBool("MotherIntroDone", true)
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_34)
                        end
                    end
                    if not quest:GetStateBool("MotherIntroDone") then
                        goto LAB_00ecdd11
                    else
                        bVar5 = true
                        cVar9 = me:IsTalkedToByHero()
                        bVar8 = true
                        if not cVar9 then goto LAB_00ecdd11 end
                    end
                    goto FLOW_past_lab_00ecdd11
                    ::LAB_00ecdd11::
                    bVar8 = false
                    ::FLOW_past_lab_00ecdd11::
                    if bVar5 then
                        bVar5 = false
                    end
                    if bVar8 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then
                            return
                        end
                        xStack_94 = resources:StartMovie("")
                        f_stk_84 = 0x0
                        pCVar17 = 0x1
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x744),(int)xStack_70);
                        pCVar11 = quest:GetHero()
                        bVar8 = quest:IsObjectInThingsPossession("", pCVar11)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar8 then
                            if bVar10 then
                                __cleanup_LAB_00ece3f7()
                                return
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MOTHER_CURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar13 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_94)
                                    return
                                end
                                iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                -- LAB_00ece3b6: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_94)
                                return
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if iVar13 == 1 then
                                if not bVar8 then
                                    require("V_SickChild.native_quest_helpers").helper_ECE460(quest, me)
                                    xStack_98 = quest:ReadGlobalGameDataString(0x744)
                                    quest:TakeObjectFromHero(xStack_98)
                                    quest:ClearThingHasInformation(me)
                                    quest:SetStateBool("FinishedQuest", true)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_94)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar8 = not alive
                                    if bVar8 then
                                        return
                                    end
                                    pCVar1 = resources:MemberResource("seh_Mother")
                                    resources:PrepareResource(pCVar1)
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar8 = not alive
                                    until not (not bVar8)
                                    return
                                end
                                -- LAB_00ecdf97: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_94)
                                return
                            end
                            if bVar8 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_94)
                                return
                            end
                            -- TODO(native): pCVar11 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x30))()
                            pCVar11 = nil --[[unresolved native value]]
                            fret_0 = quest:GetHealth(nil --[[missing]])
                            fVar4 = 0.0
                            if fVar4 < fret_0 then
                                -- TODO(native): iVar13 = *(__native_entity_state:GetStateInt("self_0x14") + 0x48)
                                iVar13 = nil --[[unresolved native value]]
                                uVar16 = 0
                                uVar15 = 2
                                pcVar14 = "TEXT_QST_B10_MOTHER_ASK_CURE"
                                pCVar17 = quest:GetHero()
                                -- TODO(native): (**(code **)(iVar13 + 0x34))(pCVar17,pcVar14,uVar15,uVar16);
                                -- TODO(native): cVar9 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x68))(pCVar17)
                                cVar9 = nil --[[unresolved native value]]
                                while cVar9 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar8 = not alive
                                    if bVar8 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_94)
                                        return
                                    end
                                    -- TODO(native): cVar9 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x68))()
                                    cVar9 = nil --[[unresolved native value]]
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_94)
                                    return
                                end
                            end
                        else
                            if bVar10 then __cleanup_LAB_00ece3f7(); return end
                            -- TODO(native): pCVar11 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x30))()
                            pCVar11 = nil --[[unresolved native value]]
                            fret_00 = quest:GetHealth(pCVar17)
                            fVar4 = 0.0
                            if fVar4 < fret_00 then
                                -- TODO(native): iVar13 = *(__native_entity_state:GetStateInt("self_0x14") + 0x48)
                                iVar13 = nil --[[unresolved native value]]
                                uVar16 = 0
                                uVar15 = 0
                                pcVar14 = "TEXT_QST_B10_MOTHER_CURE_REMIND_10"
                                pCVar17 = quest:GetHero()
                                -- TODO(native): (**(code **)(iVar13 + 0x34))(pCVar17,pcVar14,uVar15,uVar16);
                                -- TODO(native): cVar9 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x68))(pCVar17)
                                cVar9 = nil --[[unresolved native value]]
                                while cVar9 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar8 = not alive
                                    if bVar8 then
                                        quest:PauseAllNonScriptedEntities(resources:MemberResource("seh_Mother"))
                                        resources:DestroyMovie(xStack_a8)
                                        return
                                    end
                                    -- TODO(native): cVar9 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x68))()
                                    cVar9 = nil --[[unresolved native value]]
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then __cleanup_LAB_00ece3f7(); return end
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_94)
                    end
                    cVar9 = me:MsgIsHitByHero()
                    if not cVar9 then
                        cVar9 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar9 then
                            bVar7 = true
                            bVar6 = true
                            cVar9 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar9 then goto LAB_00ece11a end
                        end
                        bVar7 = true
                        bVar8 = false
                    else
                        goto LAB_00ece11a
                    end
                    goto FLOW_past_lab_00ece11a
                    ::LAB_00ece11a::
                    bVar8 = true
                    ::FLOW_past_lab_00ece11a::
                    if bVar6 then
                        bVar6 = false
                    end
                    if bVar7 then
                        bVar7 = false
                    end
                    if bVar8 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then
                            return
                        end
                        xStack_4c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): pCVar17 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x30))()
                        pCVar17 = nil --[[unresolved native value]]
                        fret_01 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fret_01 then
                            -- TODO(native): iVar13 = *(__native_entity_state:GetStateInt("self_0x14") + 0x48)
                            iVar13 = nil --[[unresolved native value]]
                            uVar15 = 0
                            pcVar14 = "TEXT_QST_B10_MOTHER_HIT"
                            pCVar17 = quest:GetHero()
                            -- TODO(native): (**(code **)(iVar13 + 0x34))(pCVar17,pcVar14,uVar15);
                            -- TODO(native): cVar9 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x68))(pCVar17)
                            cVar9 = nil --[[unresolved native value]]
                            while cVar9 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_60)
                                    return
                                end
                                -- TODO(native): cVar9 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x48) + 0x68))()
                                cVar9 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_60)
                                return
                            end
                        end
                        pCVar11 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(me, pCVar11)
                        pCVar11 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(pCVar11, me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_60)
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then
                        return
                    end
                until false
            end
        end
    end
end

function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

