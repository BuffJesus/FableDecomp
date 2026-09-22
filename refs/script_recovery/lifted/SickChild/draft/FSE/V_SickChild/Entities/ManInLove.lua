-- Generated native draft: ManInLove. Review coverage report before use.
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
    local __native_condition_1, bVar5, cVar6, center, fVar10, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, fret_12, iVar13, iVar14, iVar15, iVar16, pCVar7, pCVar8, pCVar9, pcVar12, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar11, uVar4, u_stk_15c, xStack_104, xStack_140, xStack_158, xStack_170, xStack_b8, xStack_d4, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_6c, x_stk_78, x_stk_84, x_stk_90, x_stk_9c, x_stk_a8, x_stk_c
    local alive = true
    local function __cleanup_LAB_00ec95d3()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_158)
        resources:ReleaseResource(xStack_170)
    end
    u_stk_15c = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_170 = resources:NewResource()
    -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x740),(int)&xStack_148);
    u_stk_15c = 1
    pCVar7 = quest:GetHero()
    bVar5 = quest:IsObjectInThingsPossession(nil --[[missing]], pCVar7)
    if not bVar5 then
        u_stk_15c = 3
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", pCVar7)
        if ((not bVar5) and (__native_entity_state:GetStateBool("GivenLetter"))) and (quest:GetStateInt("MansLoverState") ~= 3) then
            bVar5 = false
            goto LAB_00ec8400
        end
    end
    bVar5 = true
    ::LAB_00ec8400::
    if (u_stk_15c & 2) ~= 0 then
        u_stk_15c = u_stk_15c & 0xfffffffd
    end
    if (u_stk_15c & 1) ~= 0 then
        u_stk_15c = u_stk_15c & 0xfffffffe
    end
    if bVar5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            -- LAB_00ec9bea: (native jump target)
            resources:ReleaseResource(xStack_170)
            return
        end
        quest:SetThingHasInformation(me, true, true, false)
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    pCVar8 = me:GetPos()
    center = pCVar8.x
    quest:SetWanderCentrePoint(me, pCVar8)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 10.0)
    quest:SetScriptingStateGroup(me, 4)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    repeat
        if bVar5 then
            resources:ReleaseResource(xStack_170)
            return
        end
        cVar6 = me:IsTalkedToByHero()
        if cVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_170)
                return
            end
            resources:PrepareResource(xStack_170)
            bVar5 = resources:TryAcquire(xStack_170, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                bVar5 = resources:TryAcquire(xStack_170, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_170)
                return
            end
            if (not __native_entity_state:GetStateBool("GivenLetter")) and (quest:GetStateInt("MansLoverState") ~= 3) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                xStack_d4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_54 = resources:ScriptThing(xStack_170)
                pCVar9 = x_stk_54
                fret_0 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_0 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO"
                    pCVar9 = quest:GetHero()
                    r1 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_d4)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar6 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_d4)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                x_stk_3c = resources:ScriptThing(xStack_170)
                pCVar9 = x_stk_3c
                fret_00 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_00 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_50"
                    pCVar9 = quest:GetHero()
                    r2 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_d4)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar6 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_d4)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x73c),(int)&xStack_118);
                quest:GiveHeroObject(pcVar12, -1, false)
                x_stk_24 = resources:ScriptThing(xStack_170)
                pCVar9 = x_stk_24
                fret_01 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_01 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_60"
                    pCVar9 = quest:GetHero()
                    r3 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_d4)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar6 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_d4)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                x_stk_30 = resources:ScriptThing(xStack_170)
                pCVar9 = x_stk_30
                fret_02 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_02 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_70"
                    pCVar9 = quest:GetHero()
                    r4 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00ec9b00: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_d4)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar6 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00ec9b2c: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_d4)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                __native_entity_state:SetStateBool("GivenLetter", true)
                quest:ClearThingHasInformation(me)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_d4
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                xStack_158 = resources:StartMovie("")
                pCVar9 = 0x1
                quest:PauseAllNonScriptedEntities(true)
                -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x740),(int)xStack_138);
                uVar4 = xStack_158
                pCVar7 = quest:GetHero()
                bVar5 = quest:IsObjectInThingsPossession("", pCVar7)
                if bVar5 then
                    goto LAB_00ec8a86
                else
                    uVar11 = uVar4 | 0xc
                    xStack_158 = uVar11
                    pCVar7 = quest:GetHero()
                    bVar5 = quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", pCVar7)
                    if bVar5 then goto LAB_00ec8a86 end
                    goto LAB_00ec8a92
                end
                goto FLOW_past_lab_00ec8a86
                ::LAB_00ec8a86::
                -- TODO(native): xStack_170 = (int *)CONCAT13(1,(undefined3)xStack_170);
                if __native_entity_state:GetStateBool("GotReward") then goto LAB_00ec8a92 end
                ::FLOW_past_lab_00ec8a86::
                goto FLOW_past_lab_00ec8a92
                ::LAB_00ec8a92::
                ::FLOW_past_lab_00ec8a92::
                if (uVar11 & 8) ~= 0 then
                    uVar11 = uVar11 & 0xfffffff7
                    xStack_158 = uVar11
                end
                if (uVar11 & 4) ~= 0 then
                    -- TODO(native): xStack_158 = uVar11 & 0xfffffffb;
                end
                if 0 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        __cleanup_LAB_00ec95d3()
                        return
                    end
                    if not __native_entity_state:GetStateBool("GotReward") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_158)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        if quest:GetStateInt("MansLoverState") == 3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_158)
                                resources:ReleaseResource(xStack_170)
                                return
                            end
                            x_stk_18 = resources:ScriptThing(xStack_170)
                            pCVar7 = x_stk_18
                            fret_07 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_07 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED"
                                pCVar9 = quest:GetHero()
                                r5 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_158)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_158)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                            end
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)xStack_140);
                            quest:GiveHeroObject(pcVar12, xStack_140, -1)
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_110);
                            quest:RemoveItemFromContainer(me, "")
                            __native_entity_state:SetStateBool("GotReward", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_158)
                                resources:ReleaseResource(xStack_170)
                                return
                            end
                            x_stk_60 = resources:ScriptThing(xStack_170)
                            pCVar7 = x_stk_60
                            fret_08 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_08 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER"
                                pCVar9 = quest:GetHero()
                                r6 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_158)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                end
                                goto LAB_00ec96a4
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_158)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        me:ClearCommands()
                        if not __native_entity_state:GetStateBool("HelpedGuyOut") then
                            if quest:GetStateInt("MansLoverState") == 3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_158)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                x_stk_9c = resources:ScriptThing(xStack_170)
                                pCVar7 = x_stk_9c
                                fret_10 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_10 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10"
                                    pCVar9 = quest:GetHero()
                                    r7 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_158)
                                            resources:ReleaseResource(xStack_170)
                                            return
                                        end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar6 = iVar13
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_158)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_158)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                x_stk_84 = resources:ScriptThing(xStack_170)
                                pCVar7 = x_stk_84
                                fret_11 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_11 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10"
                                    pCVar9 = quest:GetHero()
                                    r8 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_158)
                                            resources:ReleaseResource(xStack_170)
                                            return
                                        end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar6 = iVar13
                                    end
                                    goto LAB_00ec96a4
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_158)
                                resources:ReleaseResource(xStack_170)
                                return
                            end
                            x_stk_a8 = resources:ScriptThing(xStack_170)
                            pCVar7 = x_stk_a8
                            fret_09 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_09 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10"
                                pCVar9 = quest:GetHero()
                                r9 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_158)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                end
                                goto LAB_00ec96a4
                            end
                        end
                    end
                    goto FLOW_past_lab_00ec96a4
                    ::LAB_00ec96a4::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_158)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                    ::FLOW_past_lab_00ec96a4::
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then __cleanup_LAB_00ec95d3(); return end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar13 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00ec95d3(); return end
                        iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then __cleanup_LAB_00ec95d3(); return end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if iVar13 == 1 then
                        if bVar5 then __cleanup_LAB_00ec95d3(); return end
                        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x740),(int)&xStack_e8);
                        quest:TakeObjectFromHero(pcVar12)
                        quest:TakeObjectFromHero("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER")
                        x_stk_c = resources:ScriptThing(xStack_170)
                        pCVar7 = x_stk_c
                        fret_03 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_03 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME"
                            pCVar9 = quest:GetHero()
                            r10 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar6 = iVar13
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00ec95d3(); return end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                        end
                        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_120);
                        quest:GiveHeroObject(pcVar12, -1, false)
                        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)xStack_dc);
                        quest:RemoveItemFromContainer(me, nil --[[missing]])
                        if quest:GetStateInt("MansLoverState") == 2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            fVar10 = quest:ReadGlobalGameDataFloat(0x758)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            fVar10 = quest:ReadGlobalGameDataFloat(0x764)
                        end
                        quest:GiveHeroMorality(fVar10)
                        __native_entity_state:SetStateBool("GotReward", true)
                        __native_entity_state:SetStateBool("HelpedGuyOut", true)
                        quest:ClearThingHasInformation(me)
                    else
                        if bVar5 then __cleanup_LAB_00ec95d3(); return end
                        x_stk_90 = resources:ScriptThing(xStack_170)
                        pCVar7 = x_stk_90
                        fret_04 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_04 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME"
                            pCVar7 = quest:GetHero()
                            r11 = me:Speak(pCVar7, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar6 = iVar13
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00ec95d3(); return end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_WOMANS_OPINION_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar13 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00ec95d3(); return end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if iVar13 == 1 then
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            x_stk_48 = resources:ScriptThing(xStack_170)
                            pCVar7 = x_stk_48
                            fret_05 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_05 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_HAPPY"
                                pCVar9 = quest:GetHero()
                                r12 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then __cleanup_LAB_00ec95d3(); return end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            end
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)xStack_104);
                            quest:GiveHeroObject(pcVar12, xStack_104, -1)
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_ec);
                            quest:RemoveItemFromContainer(me, nil --[[missing]])
                            __native_entity_state:SetStateBool("GotReward", true)
                            __native_entity_state:SetStateBool("HelpedGuyOut", true)
                            if quest:GetStateInt("MansLoverState") == 2 then
                                goto LAB_00ec91ef
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00ec95d3(); return end
                                fVar10 = quest:ReadGlobalGameDataFloat(0x764)
                            end
                        else
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            x_stk_78 = resources:ScriptThing(xStack_170)
                            pCVar7 = x_stk_78
                            fret_06 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_06 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY"
                                pCVar9 = quest:GetHero()
                                r13 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then __cleanup_LAB_00ec95d3(); return end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            end
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_fc);
                            quest:GiveHeroObject(pcVar12, -1, false)
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_124);
                            quest:RemoveItemFromContainer(me, nil --[[missing]])
                            __native_entity_state:SetStateBool("GotReward", true)
                            if quest:GetStateInt("MansLoverState") ~= 2 then goto LAB_00ec91ef end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00ec95d3(); return end
                            fVar10 = quest:ReadGlobalGameDataFloat(0x760)
                        end
                        goto FLOW_past_lab_00ec91ef
                        ::LAB_00ec91ef::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00ec9b58: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_158)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        fVar10 = quest:ReadGlobalGameDataFloat(0x758)
                        ::FLOW_past_lab_00ec91ef::
                        quest:GiveHeroMorality(fVar10)
                        quest:ClearThingHasInformation(me)
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_158
            end
            resources:ReleaseResource(this_00)
            resources:PrepareResource(xStack_170)
        end
        uVar4 = u_stk_15c
        u_stk_15c = u_stk_15c | 0x10
        cVar6 = me:MsgIsHitByHero()
        if not cVar6 then
            uVar11 = uVar4 | 0x30
            u_stk_15c = uVar11
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                uVar11 = uVar4 | 0x70
                u_stk_15c = uVar11
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00ec9774 end
            end
        else
            goto LAB_00ec9774
        end
        goto FLOW_past_lab_00ec9774
        ::LAB_00ec9774::
        ::FLOW_past_lab_00ec9774::
        if (uVar11 & 0x40) ~= 0 then
            uVar11 = uVar11 & 0xffffffbf
            u_stk_15c = uVar11
        end
        if (uVar11 & 0x20) ~= 0 then
            uVar11 = uVar11 & 0xffffffdf
            u_stk_15c = uVar11
        end
        if (uVar11 & 0x10) ~= 0 then
            u_stk_15c = uVar11 & 0xffffffef
        end
        if 1 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_170)
                return
            end
            me:ClearCommands()
            resources:PrepareResource(xStack_170)
            bVar5 = resources:TryAcquire(xStack_170, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                bVar5 = resources:TryAcquire(xStack_170, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_170)
                return
            end
            xStack_b8 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            x_stk_6c = resources:ScriptThing(xStack_170)
            pCVar9 = x_stk_6c
            fret_12 = quest:GetHealth(pCVar9)
            fVar3 = 0.0
            if fVar3 < fret_12 then
                iVar16 = 0
                iVar15 = 1
                iVar14 = 0
                iVar13 = 0
                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_ON_HIT"
                pCVar9 = quest:GetHero()
                r14 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                iVar13 = me:IsPerformingScriptTask()
                cVar6 = iVar13
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_b8)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_b8)
                    resources:ReleaseResource(xStack_170)
                    return
                end
            end
            resources:PrepareResource(xStack_170)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_b8)
        end
        __native_condition_1 = __native_entity_state:GetStateBool("GotReward")
        if __native_condition_1 then
            iVar13 = me:IsPerformingScriptTask()
            __native_condition_1 = not iVar13
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:ReleaseResource(xStack_170)
                return
            end
            r15 = quest:GetNearestWithDefName(me, "REGION_EXIT_POINT")
            resources:PrepareResource(xStack_170)
            bVar5 = resources:TryAcquire(xStack_170, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                bVar5 = resources:TryAcquire(xStack_170, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                -- LAB_00ec9bda: (native jump target)
                resources:ReleaseResource(xStack_170)
                return
            end
            bVar5 = quest:IsDistanceBetweenThingsOver(me, r15, 2.0)
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                me:MoveToThing(r15, 1.0, 0)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                quest:FadeOutAndKillEntity(me, true, 1.5, true)
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("GivenLetter", false)
    __native_entity_state:SetStateBool("GotReward", false)
    __native_entity_state:SetStateBool("HelpedGuyOut", false)
    quest:SetThingPersistent(me, true)
end

function OnPersist(quest, me, context)
    local givenLetter = quest:GetStateBool("GivenLetter") or false
    givenLetter = quest:PersistTransferBool(context, "GivenLetter", givenLetter)
    quest:SetStateBool("GivenLetter", givenLetter)
    local gotReward = quest:GetStateBool("GotReward") or false
    gotReward = quest:PersistTransferBool(context, "GotReward", gotReward)
    quest:SetStateBool("GotReward", gotReward)
end

function OnPredicateFail(quest, me)
    local bVar1, bVar3, cVar2
    bVar3 = false
    if not __native_entity_state:GetStateBool("GotReward") then
        bVar3 = true
        cVar2 = me:MsgIsKilledBy("")
        bVar1 = true
        if cVar2 then goto LAB_00ec8274 end
    end
    bVar1 = false
    ::LAB_00ec8274::
    if bVar3 then
    end
    if bVar1 then
        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_4);
        quest:GiveHeroObject(nil --[[missing]], -1, false)
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x768))
    end
end

