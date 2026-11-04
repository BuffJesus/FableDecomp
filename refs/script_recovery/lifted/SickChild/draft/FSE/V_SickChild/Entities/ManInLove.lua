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
    local __native_condition_1, bVar5, cVar6, center, fVar10, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, fret_12, iVar13, iVar14, iVar15, iVar16, local_manLetterDefinition, local_manReturnedItemDefinition, local_manRewardDefinitionFirst, local_manRewardDefinitionSecond, manCleanupFlags, manConversationMovie, manHasUnrewardedLetter, manHitMovie, manIntroMovie, manShouldReactToHit, manTalkMovie, pCVar7, pCVar8, pCVar9, pcVar12, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, uVar11, uVar4, xStack_110, xStack_118, xStack_120, xStack_124, xStack_148, xStack_170, xStack_e8, xStack_ec, xStack_fc, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_6c, x_stk_78, x_stk_84, x_stk_90, x_stk_9c, x_stk_a8, x_stk_c
    local alive = true
    local function __cleanup_LAB_00ec95d3()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(manTalkMovie)
        resources:ReleaseResource(xStack_170)
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_170 = resources:NewResource()
    xStack_148 = quest:ReadGlobalGameDataString(0x740)
    pCVar7 = quest:GetHero()
    bVar5 = quest:IsObjectInThingsPossession(xStack_148, pCVar7)
    if not bVar5 then
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", pCVar7)
        if ((not bVar5) and (__native_entity_state:GetStateBool("GivenLetter"))) and (quest:GetStateInt("MansLoverState") ~= 3) then
            bVar5 = false
            goto LAB_00ec8400
        end
    end
    bVar5 = true
    ::LAB_00ec8400::
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
                manIntroMovie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                pCVar9, x_stk_54 = nil, nil
                if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO"
                    pCVar9 = quest:GetHero()
                    r1 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    if not r1 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
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
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                pCVar9, x_stk_3c = nil, nil
                if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_50"
                    pCVar9 = quest:GetHero()
                    r2 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    if not r2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
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
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                xStack_118 = quest:ReadGlobalGameDataString(0x73c)
                quest:GiveHeroObject(xStack_118, -1, false)
                pCVar9, x_stk_24 = nil, nil
                if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_60"
                    pCVar9 = quest:GetHero()
                    r3 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    if not r3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
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
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                pCVar9, x_stk_30 = nil, nil
                if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_70"
                    pCVar9 = quest:GetHero()
                    r4 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    if not r4 then
                        -- LAB_00ec9b00: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar6 = iVar13
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- LAB_00ec9b00: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
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
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(xStack_170)
                        return
                    end
                end
                __native_entity_state:SetStateBool("GivenLetter", true)
                quest:ClearThingHasInformation(me)
                quest:PauseAllNonScriptedEntities(false)
                manConversationMovie = manIntroMovie
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_170)
                    return
                end
                manTalkMovie = resources:StartMovie("")
                pCVar9 = 0x1
                quest:PauseAllNonScriptedEntities(true)
                local_manLetterDefinition = quest:ReadGlobalGameDataString(0x740)
                pCVar7 = quest:GetHero()
                bVar5 = quest:IsObjectInThingsPossession(local_manLetterDefinition, pCVar7)
                if bVar5 then
                    goto LAB_00ec8a86
                else
                    pCVar7 = quest:GetHero()
                    bVar5 = quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", pCVar7)
                    if bVar5 then goto LAB_00ec8a86 end
                    goto LAB_00ec8a92
                end
                goto FLOW_past_lab_00ec8a86
                ::LAB_00ec8a86::
                manHasUnrewardedLetter = true
                if __native_entity_state:GetStateBool("GotReward") then goto LAB_00ec8a92 end
                ::FLOW_past_lab_00ec8a86::
                goto FLOW_past_lab_00ec8a92
                ::LAB_00ec8a92::
                manHasUnrewardedLetter = false
                ::FLOW_past_lab_00ec8a92::
                if not manHasUnrewardedLetter then
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
                            resources:DestroyMovie(manTalkMovie)
                            resources:ReleaseResource(xStack_170)
                            return
                        end
                        if quest:GetStateInt("MansLoverState") == 3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(manTalkMovie)
                                resources:ReleaseResource(xStack_170)
                                return
                            end
                            pCVar7, x_stk_18 = nil, nil
                            if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED"
                                pCVar9 = quest:GetHero()
                                r5 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                if not r5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
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
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                            end
                            local_manRewardDefinitionFirst = quest:ReadGlobalGameDataString(0x738)
                            quest:GiveHeroObject(local_manRewardDefinitionFirst, -1, false)
                            xStack_110 = quest:ReadGlobalGameDataString(0x738)
                            quest:RemoveItemFromContainer(me, xStack_110)
                            __native_entity_state:SetStateBool("GotReward", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(manTalkMovie)
                                resources:ReleaseResource(xStack_170)
                                return
                            end
                            pCVar7, x_stk_60 = nil, nil
                            if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER"
                                pCVar9 = quest:GetHero()
                                r6 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                if not r6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
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
                            resources:DestroyMovie(manTalkMovie)
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
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                pCVar7, x_stk_9c = nil, nil
                                if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10"
                                    pCVar9 = quest:GetHero()
                                    r7 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    if not r7 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(manTalkMovie)
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
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                pCVar7, x_stk_84 = nil, nil
                                if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10"
                                    pCVar9 = quest:GetHero()
                                    r8 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    if not r8 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(xStack_170)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar6 = iVar13
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(manTalkMovie)
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
                                resources:DestroyMovie(manTalkMovie)
                                resources:ReleaseResource(xStack_170)
                                return
                            end
                            pCVar7, x_stk_a8 = nil, nil
                            if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10"
                                pCVar9 = quest:GetHero()
                                r9 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                if not r9 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(xStack_170)
                                    return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar6 = iVar13
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
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
                        resources:DestroyMovie(manTalkMovie)
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
                        xStack_e8 = quest:ReadGlobalGameDataString(0x740)
                        quest:TakeObjectFromHero(xStack_e8)
                        quest:TakeObjectFromHero("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER")
                        pCVar7, x_stk_c = nil, nil
                        if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME"
                            pCVar9 = quest:GetHero()
                            r10 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            if not r10 then
                                __cleanup_LAB_00ec95d3(); return
                            end
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
                        xStack_120 = quest:ReadGlobalGameDataString(0x738)
                        quest:GiveHeroObject(xStack_120, -1, false)
                        local_manReturnedItemDefinition = quest:ReadGlobalGameDataString(0x738)
                        quest:RemoveItemFromContainer(me, local_manReturnedItemDefinition)
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
                        pCVar7, x_stk_90 = nil, nil
                        if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME"
                            pCVar7 = quest:GetHero()
                            r11 = me:Speak(pCVar7, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            if not r11 then
                                __cleanup_LAB_00ec95d3(); return
                            end
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
                            pCVar7, x_stk_48 = nil, nil
                            if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_HAPPY"
                                pCVar9 = quest:GetHero()
                                r12 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                if not r12 then
                                    __cleanup_LAB_00ec95d3(); return
                                end
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
                            local_manRewardDefinitionSecond = quest:ReadGlobalGameDataString(0x738)
                            quest:GiveHeroObject(local_manRewardDefinitionSecond, -1, false)
                            xStack_ec = quest:ReadGlobalGameDataString(0x738)
                            quest:RemoveItemFromContainer(me, xStack_ec)
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
                            pCVar7, x_stk_78 = nil, nil
                            if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY"
                                pCVar9 = quest:GetHero()
                                r13 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                if not r13 then
                                    __cleanup_LAB_00ec95d3(); return
                                end
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
                            xStack_fc = quest:ReadGlobalGameDataString(0x738)
                            quest:GiveHeroObject(xStack_fc, -1, false)
                            xStack_124 = quest:ReadGlobalGameDataString(0x738)
                            quest:RemoveItemFromContainer(me, xStack_124)
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
                            resources:DestroyMovie(manTalkMovie)
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
                manConversationMovie = manTalkMovie
            end
            resources:DestroyMovie(manConversationMovie)
            resources:PrepareResource(xStack_170)
        end
        cVar6 = me:MsgIsHitByHero()
        if not cVar6 then
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00ec9774 end
            end
            manShouldReactToHit = false
        else
            goto LAB_00ec9774
        end
        goto FLOW_past_lab_00ec9774
        ::LAB_00ec9774::
        manShouldReactToHit = true
        ::FLOW_past_lab_00ec9774::
        if manShouldReactToHit then
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
            manHitMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            pCVar9, x_stk_6c = nil, nil
            if resources:ResourceThingHealthAbove(xStack_170, 0.0) then
                iVar16 = 0
                iVar15 = 1
                iVar14 = 0
                iVar13 = 0
                pcVar12 = "TEXT_QST_B10_MAN_IN_LOVE_ON_HIT"
                pCVar9 = quest:GetHero()
                r14 = me:Speak(pCVar9, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                if not r14 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(manHitMovie)
                    resources:ReleaseResource(xStack_170)
                    return
                end
                iVar13 = me:IsPerformingScriptTask()
                cVar6 = iVar13
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manHitMovie)
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
                    resources:DestroyMovie(manHitMovie)
                    resources:ReleaseResource(xStack_170)
                    return
                end
            end
            resources:PrepareResource(xStack_170)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(manHitMovie)
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
    local givenLetter = __native_entity_state:GetStateBool("GivenLetter") or false
    givenLetter = quest:PersistTransferBool(context, "GivenLetter", givenLetter)
    __native_entity_state:SetStateBool("GivenLetter", givenLetter)
    local gotReward = __native_entity_state:GetStateBool("GotReward") or false
    gotReward = quest:PersistTransferBool(context, "GotReward", gotReward)
    __native_entity_state:SetStateBool("GotReward", gotReward)
end

function OnPredicateFail(quest, me)
    local bVar1, bVar3, cVar2, xStack_4
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
        xStack_4 = quest:ReadGlobalGameDataString(0x738)
        quest:GiveHeroObject(xStack_4, -1, false)
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x768))
    end
end

