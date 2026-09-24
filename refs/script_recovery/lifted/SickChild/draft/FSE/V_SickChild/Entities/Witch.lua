-- Generated native draft: Witch. Review coverage report before use.
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
    local CVar5, bVar6, bVar8, cVar7, ctr_94_2, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar10, native_arg_sequence_1, pCVar12, pCVar15, pCVar20, pCVar9, pcVar14, piVar1, pvVar11, stack0xffffff3c, this_00, this_01, uVar13, uVar16, uVar17, uVar4, xStack_58, xStack_70, xStack_70_2, xStack_74, xStack_7c, xStack_8c, xStack_8c_2, xStack_90_2, xStack_a0, x_stk_34
    local alive = true
    local function __cleanup_LAB_00ecf292()
        resources:DestroyMovie(xStack_8c)
    end
    local function __cleanup_LAB_00ecf3e5()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_8c)
    end
    local function __cleanup_LAB_00ecf7c9()
        this_01 = (xStack_90_2 + 4)
        resources:DestroyMovie(this_01)
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        this_00 = (__native_entity_state:GetStateInt("self_0x14") + 0x58)
        resources:PrepareResource(this_00)
        bVar6 = resources:TryAcquire((__native_entity_state:GetStateInt("self_0x14") + 0x58), me, 4)
        while not bVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            bVar6 = resources:TryAcquire((__native_entity_state:GetStateInt("self_0x14") + 0x58), me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            iVar10 = __native_entity_state:GetStateInt("ThingsForPotionGot")
            ctr_94_2 = 0
            while iVar10 < 4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                cVar7 = me:IsTalkedToByHero()
                if cVar7 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    xStack_8c = resources:StartMovie("")
                    pCVar20 = 0x1
                    quest:PauseAllNonScriptedEntities(true)
                    if not __native_entity_state:GetStateBool("IntroDone") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then __cleanup_LAB_00ecf292(); return end
                        require("V_SickChild.native_quest_helpers").helper_ECE460(quest, me)
                        pCVar15 = "TEXT_QUEST_SICK_CHILD_OBJECTIVE_02"
                        pCVar9 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pCVar9, pCVar15, "", "")
                        __native_entity_state:SetStateBool("IntroDone", true)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                            resources:DestroyMovie(xStack_8c)
                            return
                        end
                        xStack_58 = quest:ReadGlobalGameDataString(0x738)
                        pCVar12 = quest:GetHero()
                        bVar6 = quest:IsObjectInThingsPossession(xStack_58, pCVar12)
                        if (bVar6) and (__native_entity_state:GetStateInt("ThingsForPotionGot") < 4) then
                            bVar6 = true
                        else
                            bVar6 = false
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar6 then
                            if bVar8 then
                                __cleanup_LAB_00ecf292()
                                return
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B10_QUESTION_GIVE_MUSHROOM", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar10 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_8c)
                                    return
                                end
                                iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                resources:DestroyMovie(xStack_8c)
                                return
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if iVar10 == 1 then
                                if bVar6 then
                                    __cleanup_LAB_00ecf3e5()
                                    return
                                end
                                while true do
                                    xStack_74 = quest:ReadGlobalGameDataString(0x738)
                                    pCVar12 = quest:GetHero()
                                    bVar6 = quest:IsObjectInThingsPossession(xStack_74, pCVar12)
                                    if not bVar6 then break end
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                        resources:DestroyMovie(xStack_8c)
                                        return
                                    end
                                    xStack_7c = quest:ReadGlobalGameDataString(0x738)
                                    quest:TakeObjectFromHero(xStack_7c)
                                    __native_entity_state:SetStateInt("ThingsForPotionGot", __native_entity_state:GetStateInt("ThingsForPotionGot") + 1)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_8c)
                                    return
                                end
                                if 3 < __native_entity_state:GetStateInt("ThingsForPotionGot") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_8c)
                                        return
                                    end
                                    stack0xffffff3c = quest:ReadGlobalGameDataString(0x744)
                                    require("V_SickChild.native_quest_helpers").helper_ECE460(quest, me)
                                    pCVar9 = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(pCVar9, "TEXT_QUEST_SICK_CHILD_OBJECTIVE_03", "", "")
                                    quest:ClearThingHasInformation(me)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_8c_2)
                                    break
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                    resources:DestroyMovie(xStack_8c)
                                    return
                                end
                                -- TODO(native): pCVar12 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x30))()
                                pCVar12 = nil --[[unresolved native value]]
                                fret_0 = quest:GetHealth(nil --[[missing]])
                                fVar3 = 0.0
                                if fVar3 < fret_0 then
                                    -- TODO(native): iVar10 = *(__native_entity_state:GetStateInt("self_0x14") + 0x58)
                                    iVar10 = nil --[[unresolved native value]]
                                    uVar17 = 0
                                    uVar16 = 0
                                    pcVar14 = "TEXT_QST_B10_WITCH_NEED_MORE_CURE_10"
                                    pCVar20 = quest:GetHero()
                                    -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16,uVar17);
                                    -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))(pCVar20)
                                    cVar7 = nil --[[unresolved native value]]
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_8c)
                                            return
                                        end
                                        -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))()
                                        cVar7 = nil --[[unresolved native value]]
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                        resources:DestroyMovie(xStack_8c)
                                        return
                                    end
                                end
                            else
                                if bVar6 then
                                    -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                    resources:DestroyMovie(xStack_8c)
                                    return
                                end
                                -- TODO(native): pCVar12 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x30))()
                                pCVar12 = nil --[[unresolved native value]]
                                fret_00 = quest:GetHealth(pCVar20)
                                fVar3 = 0.0
                                if fVar3 < fret_00 then
                                    -- TODO(native): iVar10 = *(__native_entity_state:GetStateInt("self_0x14") + 0x58)
                                    iVar10 = nil --[[unresolved native value]]
                                    uVar17 = 0
                                    uVar16 = 0
                                    pcVar14 = "TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10"
                                    pCVar20 = quest:GetHero()
                                    -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16,uVar17);
                                    -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))(pCVar20)
                                    cVar7 = nil --[[unresolved native value]]
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then __cleanup_LAB_00ecf3e5(); return end
                                        -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))()
                                        cVar7 = nil --[[unresolved native value]]
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                        resources:DestroyMovie(xStack_8c)
                                        return
                                    end
                                end
                            end
                        else
                            if bVar8 then __cleanup_LAB_00ecf3e5(); return end
                            -- TODO(native): pCVar12 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x30))()
                            pCVar12 = nil --[[unresolved native value]]
                            fret_01 = quest:GetHealth(pCVar20)
                            fVar3 = 0.0
                            if fVar3 < fret_01 then
                                -- TODO(native): iVar10 = *(__native_entity_state:GetStateInt("self_0x14") + 0x58)
                                iVar10 = nil --[[unresolved native value]]
                                uVar17 = 0
                                uVar16 = 0
                                pcVar14 = "TEXT_QST_B10_WITCH_WAITING_FOR_CURE"
                                pCVar20 = quest:GetHero()
                                -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16,uVar17);
                                -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))(pCVar20)
                                cVar7 = nil --[[unresolved native value]]
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then __cleanup_LAB_00ecf292(); return end
                                    -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))()
                                    cVar7 = nil --[[unresolved native value]]
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00ecf3e5(); return end
                            end
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_8c)
                end
                -- TODO(native): xStack_a0 = xStack_a0 | 2;
                cVar7 = me:MsgIsHitByHero()
                if not cVar7 then
                    uVar13 = uVar4 | 6
                    -- TODO(native): bVar6 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
                    bVar6 = nil --[[unresolved native value]]
                    if bVar6 then
                        uVar13 = uVar4 | 0xe
                        -- TODO(native): bVar6 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
                        bVar6 = nil --[[unresolved native value]]
                        if not bVar6 then goto LAB_00ecef61 end
                    end
                else
                    goto LAB_00ecef61
                end
                goto FLOW_past_lab_00ecef61
                ::LAB_00ecef61::
                ::FLOW_past_lab_00ecef61::
                if (uVar13 & 8) ~= 0 then
                    uVar13 = uVar13 & 0xfffffff7
                end
                if (uVar13 & 4) ~= 0 then
                    uVar13 = uVar13 & 0xfffffffb
                end
                if 1 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    CVar5 = ctr_94_2
                    if bVar6 then
                        return
                    end
                    ctr_94_2 = ctr_94_2 + 1
                    if CVar5 == nil then
                        pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_10"
                        goto LAB_00ecf00d
                    else
                        if CVar5 == 0x1 then
                            pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_20"
                            goto LAB_00ecf00d
                        end
                        if CVar5 == 0x2 then
                            -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_30");
                            ctr_94_2 = 0
                        else
                            -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_20");
                            ctr_94_2 = 0
                        end
                    end
                    goto FLOW_past_lab_00ecf00d
                    ::LAB_00ecf00d::
                    xStack_a0 = pcVar14
                    ::FLOW_past_lab_00ecf00d::
                    xStack_70 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): pCVar20 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x30))()
                    pCVar20 = nil --[[unresolved native value]]
                    fret_02 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    x_stk_34 = nil
                    if fVar3 < fret_02 then
                        -- TODO(native): xStack_58 = *(int *)(*(int *)(this + 0x14) + 0x58);
                        pvVar11 = xStack_a0
                        pCVar20 = quest:GetHero()
                        -- TODO(native): (**(code **)((int)xStack_58 + 0x34))(pCVar20,pvVar11);
                        -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))(pCVar20)
                        cVar7 = nil --[[unresolved native value]]
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_8c)
                                return
                            end
                            -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))()
                            cVar7 = nil --[[unresolved native value]]
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            this_01 = xStack_8c
                            resources:DestroyMovie(this_01)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_8c)
                end
                iVar10 = __native_entity_state:GetStateInt("ThingsForPotionGot")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            native_arg_sequence_1 = false
            if not bVar6 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                repeat
                    cVar7 = me:IsTalkedToByHero()
                    if cVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            return
                        end
                        xStack_70_2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): pCVar12 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x30))()
                        pCVar12 = nil --[[unresolved native value]]
                        fret_03 = quest:GetHealth(pCVar20)
                        fVar3 = 0.0
                        if fVar3 < fret_03 then
                            -- TODO(native): iVar10 = *(__native_entity_state:GetStateInt("self_0x14") + 0x58)
                            iVar10 = nil --[[unresolved native value]]
                            uVar16 = 2
                            pcVar14 = "TEXT_QST_B10_WITCH_GOT_CURE"
                            pCVar20 = quest:GetHero()
                            -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16);
                            -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))(pCVar20)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_8c)
                                    return
                                end
                                -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_8c)
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_8c)
                    end
                    uVar4 = xStack_a0
                    -- TODO(native): xStack_a0 = xStack_a0 | 0x10;
                    cVar7 = me:MsgIsHitByHero()
                    if not cVar7 then
                        uVar13 = uVar4 | 0x30
                        -- TODO(native): bVar6 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
                        bVar6 = nil --[[unresolved native value]]
                        if bVar6 then
                            uVar13 = uVar4 | 0x70
                            -- TODO(native): bVar6 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
                            bVar6 = nil --[[unresolved native value]]
                            if not bVar6 then goto LAB_00ecf576 end
                        end
                    else
                        goto LAB_00ecf576
                    end
                    goto FLOW_past_lab_00ecf576
                    ::LAB_00ecf576::
                    ::FLOW_past_lab_00ecf576::
                    if (uVar13 & 0x40) ~= 0 then
                        uVar13 = uVar13 & 0xffffffbf
                    end
                    if (uVar13 & 0x20) ~= 0 then
                        uVar13 = uVar13 & 0xffffffdf
                    end
                    if 1 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        CVar5 = ctr_94_2
                        if bVar6 then
                            return
                        end
                        ctr_94_2 = ctr_94_2 + 1
                        if CVar5 == nil then
                            pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_10"
                            goto LAB_00ecf622
                        else
                            if CVar5 == 0x1 then
                                pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_20"
                                goto LAB_00ecf622
                            end
                            if CVar5 == 0x2 then
                                -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_30");
                                ctr_94_2 = 0
                            else
                                -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_20");
                                ctr_94_2 = 0
                            end
                        end
                        goto FLOW_past_lab_00ecf622
                        ::LAB_00ecf622::
                        xStack_a0 = pcVar14
                        ::FLOW_past_lab_00ecf622::
                        xStack_8c_2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): pCVar12 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x30))()
                        pCVar12 = nil --[[unresolved native value]]
                        fret_04 = quest:GetHealth(pCVar20)
                        fVar3 = 0.0
                        if fVar3 < fret_04 then
                            -- TODO(native): iVar10 = *(__native_entity_state:GetStateInt("self_0x14") + 0x58)
                            iVar10 = nil --[[unresolved native value]]
                            uVar17 = 0
                            uVar16 = 0
                            pvVar11 = xStack_a0
                            pCVar20 = quest:GetHero()
                            -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pvVar11,uVar16,uVar17);
                            -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))(pCVar20)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    __cleanup_LAB_00ecf7c9(); return
                                end
                                -- TODO(native): cVar7 = (**(*(__native_entity_state:GetStateInt("self_0x14") + 0x58) + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                __cleanup_LAB_00ecf7c9()
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_a0)
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                until false
            end
        end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("IntroDone", false)
    __native_entity_state:SetStateInt("ThingsForPotionGot", 0)
    if not quest:GetStateBool("MotherIntroDone") then
        quest:RemoveThing(me, false, true)
        return
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

function OnPersist(quest, me, context)
    local introDone = quest:GetStateBool("IntroDone") or false
    introDone = quest:PersistTransferBool(context, "IntroDone", introDone)
    quest:SetStateBool("IntroDone", introDone)
    local thingsForPotionGot = quest:GetStateInt("ThingsForPotionGot") or 0
    thingsForPotionGot = quest:PersistTransferInt(context, "ThingsForPotionGot", thingsForPotionGot)
    quest:SetStateInt("ThingsForPotionGot", thingsForPotionGot)
end

function OnPredicateFail(quest, me)
end

