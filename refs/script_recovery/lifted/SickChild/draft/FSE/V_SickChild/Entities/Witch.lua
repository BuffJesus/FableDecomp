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
    local bVar4, bVar6, bVar7, bVar8, cVar5, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar10, native_arg_byte_uStack_a8_b3, native_arg_sequence_1, pCVar11, pCVar17, pCVar9, pThing, pcVar14, pvVar12, r1, r2, r3, r4, r5, r6, stack0xffffff44, this_00, this_01, uVar13, witchAfterCureHitMovie, witchBeforeCureHitMovie, witchCureMovie, witchHitDialogueIndex, witchTalkMovie, xStack_58, xStack_74, xStack_7c, xStack_a0, x_stk_34
    local alive = true
    local function __cleanup_LAB_00ecf292()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(witchTalkMovie)
    end
    local function __cleanup_LAB_00ecf7c9()
        this_01 = witchAfterCureHitMovie
        resources:DestroyMovie(this_01)
    end
    bVar7 = false
    bVar8 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        this_00 = resources:MemberResource("seh_Witch")
        resources:PrepareResource(this_00)
        bVar4 = resources:TryAcquire(resources:MemberResource("seh_Witch"), me, 4)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = resources:TryAcquire(resources:MemberResource("seh_Witch"), me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            iVar10 = __native_entity_state:GetStateInt("ThingsForPotionGot")
            witchHitDialogueIndex = 0
            while iVar10 < 4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    witchTalkMovie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if not __native_entity_state:GetStateBool("IntroDone") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __cleanup_LAB_00ecf292(); return end
                        require("V_SickChild.native_quest_helpers").helper_ECE460(quest, me, "CS_SICK_WITCH", "NULL")
                        pCVar9 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pCVar9, "TEXT_QUEST_SICK_CHILD_OBJECTIVE_02", "", "")
                        __native_entity_state:SetStateBool("IntroDone", true)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            return
                        end
                        xStack_58 = quest:ReadGlobalGameDataString(0x738)
                        pCVar11 = quest:GetHero()
                        bVar4 = quest:IsObjectInThingsPossession(xStack_58, pCVar11)
                        if (bVar4) and (__native_entity_state:GetStateInt("ThingsForPotionGot") < 4) then
                            bVar4 = true
                        else
                            bVar4 = false
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar4 then
                            if bVar6 then
                                __cleanup_LAB_00ecf292()
                                return
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B10_QUESTION_GIVE_MUSHROOM", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar10 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    return
                                end
                                iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if iVar10 == 1 then
                                if bVar4 then
                                    -- LAB_00ecf3e5: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    return
                                end
                                while true do
                                    xStack_74 = quest:ReadGlobalGameDataString(0x738)
                                    pCVar11 = quest:GetHero()
                                    bVar4 = quest:IsObjectInThingsPossession(xStack_74, pCVar11)
                                    if not bVar4 then break end
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(witchTalkMovie)
                                        return
                                    end
                                    xStack_7c = quest:ReadGlobalGameDataString(0x738)
                                    quest:TakeObjectFromHero(xStack_7c)
                                    __native_entity_state:SetStateInt("ThingsForPotionGot", ((__native_entity_state:GetStateInt("ThingsForPotionGot") + 1 + 2147483648) % 4294967296 - 2147483648))
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    return
                                end
                                if 3 < __native_entity_state:GetStateInt("ThingsForPotionGot") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar8 = not alive
                                    if bVar8 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(witchTalkMovie)
                                        return
                                    end
                                    stack0xffffff44 = quest:ReadGlobalGameDataString(0x744)
                                    require("V_SickChild.native_quest_helpers").helper_ECE460(quest, me, "CS_SICK_POTION", stack0xffffff44)
                                    pCVar17 = "TEXT_QUEST_SICK_CHILD_OBJECTIVE_03"
                                    pCVar9 = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(pCVar9, pCVar17, "", "")
                                    quest:ClearThingHasInformation(me)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    break
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    return
                                end
                                pCVar11 = nil
                                if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                                    pcVar14 = "TEXT_QST_B10_WITCH_NEED_MORE_CURE_10"
                                    pCVar11 = quest:GetHero()
                                    r1 = me:Speak(pCVar11, "TEXT_QST_B10_WITCH_NEED_MORE_CURE_10", 0, false, true, false)
                                    if not r1 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(witchTalkMovie)
                                        return
                                    end
                                    cVar5 = me:IsPerformingScriptTask()
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(witchTalkMovie)
                                            return
                                        end
                                        cVar5 = me:IsPerformingScriptTask()
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(witchTalkMovie)
                                        return
                                    end
                                end
                            else
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    return
                                end
                                pCVar11 = nil
                                if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                                    pcVar14 = "TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10"
                                    pCVar11 = quest:GetHero()
                                    r2 = me:Speak(pCVar11, "TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10", 0, false, true, false)
                                    if not r2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(witchTalkMovie)
                                        return
                                    end
                                    cVar5 = me:IsPerformingScriptTask()
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(witchTalkMovie)
                                            return
                                        end
                                        cVar5 = me:IsPerformingScriptTask()
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(witchTalkMovie)
                                        return
                                    end
                                end
                            end
                        else
                            if bVar6 then __cleanup_LAB_00ecf292(); return end
                            pCVar11 = nil
                            if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                                pcVar14 = "TEXT_QST_B10_WITCH_WAITING_FOR_CURE"
                                pCVar11 = quest:GetHero()
                                r3 = me:Speak(pCVar11, "TEXT_QST_B10_WITCH_WAITING_FOR_CURE", 0, false, true, false)
                                if not r3 then
                                    __cleanup_LAB_00ecf292(); return
                                end
                                cVar5 = me:IsPerformingScriptTask()
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then __cleanup_LAB_00ecf292(); return end
                                    cVar5 = me:IsPerformingScriptTask()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then __cleanup_LAB_00ecf292(); return end
                            end
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(witchTalkMovie)
                end
                native_arg_byte_uStack_a8_b3 = resources:IsHitByHeroExceptAbility(me, 14) and 1 or 0
                bVar7 = false
                bVar8 = false
                if native_arg_byte_uStack_a8_b3 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    iVar10 = witchHitDialogueIndex
                    if bVar4 then
                        return
                    end
                    witchHitDialogueIndex = witchHitDialogueIndex + 1
                    xStack_a0 = ""
                    if iVar10 == 0 then
                        pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_10"
                        goto LAB_00ecf00d
                    else
                        if iVar10 == 1 then
                            pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_20"
                            goto LAB_00ecf00d
                        end
                        if iVar10 == 2 then
                            xStack_a0 = "TEXT_QST_B10_WITCH_ONHIT_30"
                            witchHitDialogueIndex = 0
                        else
                            xStack_a0 = "TEXT_QST_B10_WITCH_ONHIT_20"
                            witchHitDialogueIndex = 0
                        end
                    end
                    goto FLOW_past_lab_00ecf00d
                    ::LAB_00ecf00d::
                    xStack_a0 = pcVar14
                    ::FLOW_past_lab_00ecf00d::
                    witchBeforeCureHitMovie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar11 = nil
                    fret_02 = resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0)
                    x_stk_34 = nil
                    x_stk_34 = 0
                    if fret_02 then
                        -- reviewed native member speech table: xStack_60 = *(int *)(*(int *)(this + 0x14) + 0x58);
                        pvVar12 = xStack_a0
                        pCVar11 = quest:GetHero()
                        r4 = me:Speak(pCVar11, pvVar12, 0, false, true, false)
                        if not r4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchBeforeCureHitMovie)
                            return
                        end
                        cVar5 = me:IsPerformingScriptTask()
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchBeforeCureHitMovie)
                                return
                            end
                            cVar5 = me:IsPerformingScriptTask()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            this_01 = witchBeforeCureHitMovie
                            resources:DestroyMovie(this_01)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(witchBeforeCureHitMovie)
                end
                iVar10 = __native_entity_state:GetStateInt("ThingsForPotionGot")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            native_arg_sequence_1 = false
            if not bVar8 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                if not bVar8 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                repeat
                    pCVar11 = "SCRIPT_NAME_HERO"
                    cVar5 = me:IsTalkedToByHero()
                    if cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then
                            return
                        end
                        witchCureMovie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        pThing = nil
                        if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                            pcVar14 = "TEXT_QST_B10_WITCH_GOT_CURE"
                            pCVar11 = quest:GetHero()
                            r5 = me:Speak(pCVar11, "TEXT_QST_B10_WITCH_GOT_CURE", 2, false, true, false)
                            if not r5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchCureMovie)
                                return
                            end
                            cVar5 = me:IsPerformingScriptTask()
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchCureMovie)
                                    return
                                end
                                cVar5 = me:IsPerformingScriptTask()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchCureMovie)
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(witchCureMovie)
                    end
                    native_arg_byte_uStack_a8_b3 = resources:IsHitByHeroExceptAbility(me, 14) and 1 or 0
                    if native_arg_byte_uStack_a8_b3 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        iVar10 = witchHitDialogueIndex
                        if bVar8 then
                            return
                        end
                        witchHitDialogueIndex = witchHitDialogueIndex + 1
                        if iVar10 == 0 then
                            pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_10"
                            goto LAB_00ecf622
                        else
                            if iVar10 == 1 then
                                pcVar14 = "TEXT_QST_B10_WITCH_ONHIT_20"
                                goto LAB_00ecf622
                            end
                            if iVar10 == 2 then
                                xStack_a0 = "TEXT_QST_B10_WITCH_ONHIT_30"
                                witchHitDialogueIndex = 0
                            else
                                xStack_a0 = "TEXT_QST_B10_WITCH_ONHIT_20"
                                witchHitDialogueIndex = 0
                            end
                        end
                        goto FLOW_past_lab_00ecf622
                        ::LAB_00ecf622::
                        xStack_a0 = pcVar14
                        ::FLOW_past_lab_00ecf622::
                        witchAfterCureHitMovie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        pCVar11 = nil
                        if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                            pvVar12 = xStack_a0
                            pCVar11 = quest:GetHero()
                            r6 = me:Speak(pCVar11, pvVar12, 0, false, true, false)
                            if not r6 then
                                quest:PauseAllNonScriptedEntities(false)
                                __cleanup_LAB_00ecf7c9(); return
                            end
                            cVar5 = me:IsPerformingScriptTask()
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    __cleanup_LAB_00ecf7c9(); return
                                end
                                cVar5 = me:IsPerformingScriptTask()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                quest:PauseAllNonScriptedEntities(false)
                                __cleanup_LAB_00ecf7c9()
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(witchAfterCureHitMovie)
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
    local introDone = __native_entity_state:GetStateBool("IntroDone") or false
    introDone = quest:PersistTransferBool(context, "IntroDone", introDone)
    __native_entity_state:SetStateBool("IntroDone", introDone)
    local thingsForPotionGot = __native_entity_state:GetStateInt("ThingsForPotionGot") or 0
    thingsForPotionGot = quest:PersistTransferInt(context, "ThingsForPotionGot", thingsForPotionGot)
    __native_entity_state:SetStateInt("ThingsForPotionGot", thingsForPotionGot)
end

function OnPredicateFail(quest, me)
end

