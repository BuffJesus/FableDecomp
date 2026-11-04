-- Generated native draft: WomanToAttract. Review coverage report before use.
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
    local au_stk_34, au_stk_4c, au_stk_64, au_stk_7c, bVar3, cVar4, dist, fVar14, hb_stk_9c, hb_stk_a8, iVar5, local_womanExpressionName, pCVar10, pCVar17, pCVar19, pCVar6, pCVar9, pcVar18, pvVar11, r1, r2, r3, r4, r5, r6, r7, response_stk_c4, response_stk_c8, this_00, uVar15, uVar16, uVar7, uVar7_pushed, uVar8, womanFunnyMovie, womanNotFunnyMovie, womanSelfPointer, womanTalkMovie, womanTimerId, xStack_9c, xStack_a8, xStack_c8, x_stk_18, x_stk_30, x_stk_48, x_stk_c4
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    this_00 = resources:MemberResource("seh_me", me)
    resources:PrepareResource(this_00)
    xStack_9c = pCVar9
    cVar4 = me:AcquireControl(4)
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        x_stk_c4 = 4
        xStack_a8 = pCVar9
        xStack_c8 = this_00
        cVar4 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    if quest:GetStateBool("LaughingWomanKilled") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        x_stk_c4 = 1
        xStack_a8 = me
        quest:RemoveThing(me, false, true)
    end
    x_stk_c4 = 0
    if xStack_c8 ~= nil then
    end
    quest:SetIsPushableByHero(me, false)
    if not __native_entity_state:GetStateBool("GivenObject") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:SetThingHasInformation(me, false, false, false)
    end
    womanTimerId = quest:RegisterTimer()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            quest:DeregisterTimer(womanTimerId)
            return
        end
        iVar5 = quest:GetTimer(womanTimerId)
        if iVar5 == 0 then
            dist = 15.0
            pCVar6 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, dist)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00ed0ae8 end
                uVar7 = quest:AddNewConversation(me, false, false)
                uVar8 = quest:GetHero()
                quest:AddPersonToConversation(uVar7, uVar8)
                iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
                if iVar5 == 0 then
                    uVar8 = quest:GetHero()
                    quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED", me, uVar8, false)
                    me:PlayAnimation("STANDARD_TALK_SADNESS", false, false, true, false, true, false, false)
                else
                    if iVar5 == 1 then
                        uVar8 = quest:GetHero()
                        pCVar17 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT"
                        quest:AddLineToConversation(uVar7, pCVar17, me, uVar8, false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_SNIGGER", false, false, true, false, true, false, false)
                        uVar8 = quest:GetHero()
                        quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", me, uVar8, false)
                        goto LAB_00ecfbad
                    end
                    if iVar5 == 2 then
                        uVar8 = quest:GetHero()
                        pCVar17 = "TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL"
                        quest:AddLineToConversation(uVar7, pCVar17, me, uVar8, false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_POINT_AND_LAUGH", false, false, true, false, true, false, false)
                        uVar8 = quest:GetHero()
                        quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", me, uVar8, false)
                        goto LAB_00ecfbad
                    end
                end
                ::LAB_00ecfbad::
                quest:SetTimer(womanTimerId, 0x14)
            end
        end
        local_womanExpressionName = me:MsgExpressionPerformedTo()
        cVar4 = local_womanExpressionName ~= nil
        if (cVar4) and (not __native_entity_state:GetStateBool("GivenObject")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0ae8 end
            if local_womanExpressionName == nil then
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                bVar3 = false
                goto LAB_00ecff84
            end
            goto FLOW_past_lab_00ecff84
            ::LAB_00ecff84::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0ae8 end
            response_stk_c4 = "TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY"
            if local_womanExpressionName == nil then
                bVar3 = false
                if bVar3 then
                    goto LAB_00ed002a
                else
                    bVar3 = false
                    if bVar3 then
                        goto LAB_00ed005f
                    else
                        bVar3 = false
                        if bVar3 then
                            goto LAB_00ed0091
                        else
                            bVar3 = false
                            if not bVar3 then goto LAB_00ed00e0 end
                            goto LAB_00ed00c3
                        end
                    end
                end
                goto FLOW_hoist_lab_00ed00c3_3
            end
            goto FLOW_past_lab_00ed00c3
            ::LAB_00ed00c3::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP"
                goto LAB_00ed00db
            end
            ::FLOW_hoist_lab_00ed00c3_3::
            goto FLOW_hoist_lab_00ed0091_3
            ::FLOW_past_lab_00ed00c3::
            goto FLOW_past_lab_00ed0091
            ::LAB_00ed0091::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE"
                goto LAB_00ed00db
            end
            ::FLOW_hoist_lab_00ed0091_3::
            goto FLOW_hoist_lab_00ed005f_2
            ::FLOW_past_lab_00ed0091::
            goto FLOW_past_lab_00ed005f
            ::LAB_00ed005f::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_FLIRT"
                goto LAB_00ed00db
            end
            ::FLOW_hoist_lab_00ed005f_2::
            goto FLOW_hoist_lab_00ed002a_1
            ::FLOW_past_lab_00ed005f::
            goto FLOW_past_lab_00ed002a
            ::LAB_00ed002a::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_RUDE"
                goto LAB_00ed00db
            end
            ::FLOW_hoist_lab_00ed002a_1::
            goto FLOW_past_lab_00ed00db
            ::LAB_00ed00db::
            response_stk_c4 = pcVar18
            goto LAB_00ed00e0
            ::FLOW_past_lab_00ed00db::
            goto LAB_00ed0a6f
            ::FLOW_past_lab_00ed002a::
            pvVar11 = nil --[[reviewed native staging]]
            iVar5 = ((local_womanExpressionName == "EXPRESSION_SHIT") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed002a end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_FLIRT") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed005f end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_HEROIC_STANCE") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed0091 end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_VICTORY_PUMP") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed00c3 end
            ::LAB_00ed00e0::
            womanNotFunnyMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            au_stk_7c, pCVar9 = nil, nil
            cVar4 = resources:ResourceThingHealthAbove(this_00, 0.0)
            if cVar4 then
                pCVar19 = nil --[[reviewed native staging]]
                uVar16 = 0
                uVar15 = 1
                uVar8 = 0
                uVar7 = 0
                pvVar11 = response_stk_c4
                uVar7_pushed = uVar7
                uVar7 = quest:GetHero()
                r1 = me:Speak(uVar7, pvVar11, uVar7_pushed, (uVar8 ~= 0), (uVar15 ~= 0), (uVar16 ~= 0))
                if not r1 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(womanNotFunnyMovie)
                    goto LAB_00ed0ae8
                end
                cVar4 = me:IsPerformingScriptTask()
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(womanNotFunnyMovie)
                        goto LAB_00ed0ae8
                    end
                    cVar4 = me:IsPerformingScriptTask()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(womanNotFunnyMovie)
                    goto LAB_00ed0ae8
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanNotFunnyMovie)
            ::LAB_00ed0694::
            goto LAB_00ed069d
            ::FLOW_past_lab_00ecff84::
            pvVar11 = nil --[[reviewed native staging]]
            iVar5 = ((local_womanExpressionName == "EXPRESSION_PELVIC_THRUST") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if not cVar4 then
                iVar5 = ((local_womanExpressionName == "EXPRESSION_COCK_A_DOODLE_DO") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if not cVar4 then
                    iVar5 = ((local_womanExpressionName == "EXPRESSION_CROTCH_GRAB") and 0 or 1)
                    cVar4 = not (iVar5 ~= 0)
                    hb_stk_9c = cVar4
                    if not cVar4 then
                        iVar5 = ((local_womanExpressionName == "EXPRESSION_KISS_MY_ASS") and 0 or 1)
                        cVar4 = not (iVar5 ~= 0)
                        hb_stk_9c = cVar4
                        if not cVar4 then
                            iVar5 = ((local_womanExpressionName == "EXPRESSION_FLAMENCO") and 0 or 1)
                            cVar4 = not (iVar5 ~= 0)
                            hb_stk_9c = cVar4
                            if not cVar4 then
                                iVar5 = ((local_womanExpressionName == "EXPRESSION_COSSACK") and 0 or 1)
                                cVar4 = not (iVar5 ~= 0)
                                hb_stk_9c = cVar4
                                if not cVar4 then
                                    iVar5 = ((local_womanExpressionName == "EXPRESSION_AIR_GUITAR") and 0 or 1)
                                    cVar4 = not (iVar5 ~= 0)
                                    hb_stk_9c = cVar4
                                    if not cVar4 then
                                        iVar5 = ((local_womanExpressionName == "EXPRESSION_BALLET") and 0 or 1)
                                        cVar4 = not (iVar5 ~= 0)
                                        hb_stk_9c = cVar4
                                        if not cVar4 then
                                            iVar5 = ((local_womanExpressionName == "EXPRESSION_SATURDAY_NIGHT_FEVER") and 0 or 1)
                                            cVar4 = not (iVar5 ~= 0)
                                            hb_stk_9c = cVar4
                                            if not cVar4 then
                                                iVar5 = ((local_womanExpressionName == "EXPRESSION_TAP") and 0 or 1)
                                                cVar4 = not (iVar5 ~= 0)
                                                hb_stk_9c = cVar4
                                                if not cVar4 then
                                                    iVar5 = ((local_womanExpressionName == "EXPRESSION_GIGGLE") and 0 or 1)
                                                    cVar4 = not (iVar5 ~= 0)
                                                    hb_stk_9c = cVar4
                                                    if not cVar4 then goto LAB_00ecff84 end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
            -- LAB_00ecfd04: (native jump target)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0ae8 end
            __native_entity_state:SetStateInt("TimesMadeLaugh", __native_entity_state:GetStateInt("TimesMadeLaugh") + 1)
            response_stk_c8 = "TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY"
            if local_womanExpressionName == nil then
                bVar3 = false
                if not bVar3 then
                    bVar3 = false
                    if bVar3 then
                        goto LAB_00ed0245
                    else
                        bVar3 = false
                        if bVar3 then
                            goto LAB_00ed027a
                        else
                            bVar3 = false
                            if not bVar3 then
                                bVar3 = false
                                if not bVar3 then
                                    bVar3 = false
                                    if not bVar3 then
                                        bVar3 = false
                                        if not bVar3 then
                                            bVar3 = false
                                            if not bVar3 then
                                                bVar3 = false
                                                if not bVar3 then
                                                    bVar3 = false
                                                    if not bVar3 then
                                                        bVar3 = false
                                                        if not bVar3 then goto LAB_00ed02ff end
                                                        goto LAB_00ed04c2
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                                goto FLOW_hoist_lab_00ed04c2_6
                            end
                            goto FLOW_hoist_lab_00ed04c2_7
                        end
                    end
                    goto FLOW_hoist_lab_00ed04c2_9
                end
                goto FLOW_hoist_lab_00ed04c2_10
            end
            goto FLOW_past_lab_00ed04c2
            ::LAB_00ed04c2::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0a6f end
            pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE"
            goto LAB_00ed02fa
            ::FLOW_hoist_lab_00ed04c2_6::
            goto LAB_00ed02e2
            ::FLOW_hoist_lab_00ed04c2_7::
            goto LAB_00ed02ac
            ::FLOW_hoist_lab_00ed04c2_9::
            goto FLOW_hoist_lab_00ed02ac_2
            ::FLOW_hoist_lab_00ed04c2_10::
            goto FLOW_hoist_lab_00ed02ac_3
            ::FLOW_past_lab_00ed04c2::
            goto FLOW_past_lab_00ed02ac
            ::LAB_00ed02ac::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE"
                goto LAB_00ed02fa
            end
            ::FLOW_hoist_lab_00ed02ac_2::
            goto FLOW_hoist_lab_00ed027a_2
            ::FLOW_hoist_lab_00ed02ac_3::
            goto FLOW_hoist_lab_00ed027a_3
            ::FLOW_past_lab_00ed02ac::
            goto FLOW_past_lab_00ed027a
            ::LAB_00ed027a::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE"
                goto LAB_00ed02fa
            end
            ::FLOW_hoist_lab_00ed027a_2::
            goto FLOW_hoist_lab_00ed0245_1
            ::FLOW_hoist_lab_00ed027a_3::
            goto FLOW_hoist_lab_00ed0245_2
            ::FLOW_past_lab_00ed027a::
            goto FLOW_past_lab_00ed0245
            ::LAB_00ed0245::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE"
                goto LAB_00ed02fa
            end
            ::FLOW_hoist_lab_00ed0245_1::
            goto LAB_00ed0a6f
            ::FLOW_hoist_lab_00ed0245_2::
            goto LAB_00ed0210
            ::FLOW_past_lab_00ed0245::
            goto FLOW_past_lab_00ed0210
            ::LAB_00ed0210::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0a6f end
            pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE"
            ::LAB_00ed02fa::
            response_stk_c8 = pcVar18
            ::LAB_00ed02ff::
            womanFunnyMovie = resources:StartMovie("")
            pCVar9 = nil --[[reviewed native staging]]
            quest:PauseAllNonScriptedEntities(true)
            iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
            pCVar6 = pCVar9
            if iVar5 ~= 1 then
                if iVar5 == 2 then
                    au_stk_4c, pCVar10 = nil, nil
                    cVar4 = resources:ResourceThingHealthAbove(this_00, 0.0)
                    if cVar4 then
                        uVar7 = quest:GetHero()
                        r2 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", 0, false, true, false)
                        if not r2 then
                            goto LAB_00ed0a36
                        end
                        cVar4 = me:IsPerformingScriptTask()
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00ed0a36 end
                            cVar4 = me:IsPerformingScriptTask()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            goto LAB_00ed0411
                        end
                    end
                elseif iVar5 == 3 then
                    helper_ED0B10(quest, me)
                    __native_entity_state:SetStateBool("GivenObject", true)
                    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x774))
                else
                    au_stk_64, pCVar10 = nil, nil
                    cVar4 = resources:ResourceThingHealthAbove(this_00, 0.0)
                    if cVar4 then
                        uVar7 = quest:GetHero()
                        r3 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", 0, false, true, false)
                        if not r3 then
                            goto LAB_00ed0a36
                        end
                        cVar4 = me:IsPerformingScriptTask()
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00ed0a36 end
                            cVar4 = me:IsPerformingScriptTask()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0411 end
                    end
                end
                goto FLOW_past_lab_00ed0411
                ::LAB_00ed0411::
                iVar5 = nil --[[reviewed native staging]]
                goto LAB_00ed0a36
                ::FLOW_past_lab_00ed0411::
                goto LAB_00ed0666
            end
            goto FLOW_past_lab_00ed0666
            ::LAB_00ed0666::
            quest:SetTimer(womanTimerId, 0x14)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanFunnyMovie)
            goto LAB_00ed0694
            ::FLOW_past_lab_00ed0666::
            au_stk_34, pCVar10 = nil, nil
            cVar4 = resources:ResourceThingHealthAbove(this_00, 0.0)
            if not cVar4 then goto LAB_00ed0666 end
            uVar16 = 0
            uVar15 = 1
            uVar8 = 0
            uVar7 = 0
            pvVar11 = response_stk_c8
            uVar7_pushed = uVar7
            uVar7 = quest:GetHero()
            r4 = me:Speak(uVar7, pvVar11, uVar7_pushed, (uVar8 ~= 0), (uVar15 ~= 0), (uVar16 ~= 0))
            cVar4 = me:IsPerformingScriptTask()
            pCVar6 = pCVar19
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00ed0a62
                end
                cVar4 = me:IsPerformingScriptTask()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            pCVar19 = pCVar6
            if not bVar3 then goto LAB_00ed0666 end
            iVar5 = nil --[[reviewed native staging]]
            -- LAB_00ed0a5a: (native jump target)
            quest:PauseAllNonScriptedEntities(false)
            goto LAB_00ed0a62
            ::FLOW_past_lab_00ed0210::
            pvVar11 = nil --[[reviewed native staging]]
            iVar5 = ((local_womanExpressionName == "EXPRESSION_PELVIC_THRUST") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed0210 end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_COCK_A_DOODLE_DO") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed0245 end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_CROTCH_GRAB") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed027a end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_KISS_MY_ASS") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed02ac end
            iVar5 = ((local_womanExpressionName == "EXPRESSION_FLAMENCO") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if not cVar4 then
                iVar5 = ((local_womanExpressionName == "EXPRESSION_COSSACK") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((local_womanExpressionName == "EXPRESSION_AIR_GUITAR") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((local_womanExpressionName == "EXPRESSION_BALLET") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((local_womanExpressionName == "EXPRESSION_SATURDAY_NIGHT_FEVER") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((local_womanExpressionName == "EXPRESSION_TAP") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((local_womanExpressionName == "EXPRESSION_GIGGLE") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed04c2 end
                goto LAB_00ed02ff
            end
            ::LAB_00ed02e2::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pcVar18 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE"
                goto LAB_00ed02fa
            end
            goto LAB_00ed0a6f
        end
        ::LAB_00ed069d::
        cVar4 = me:IsTalkedToByHero()
        if not cVar4 then
            goto LAB_00ed06d4
        else
            hb_stk_9c = true
            if __native_entity_state:GetStateBool("GivenObject") then goto LAB_00ed06d4 end
        end
        goto FLOW_past_lab_00ed06d4
        ::LAB_00ed06d4::
        hb_stk_9c = false
        ::FLOW_past_lab_00ed06d4::
        if hb_stk_9c then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0ae8 end
            womanTalkMovie = resources:StartMovie("")
            pCVar6 = 0x1
            quest:PauseAllNonScriptedEntities(true)
            iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
            if iVar5 == 0 then
                x_stk_48, uVar7 = nil, nil
                hb_stk_a8 = resources:ResourceThingHealthAbove(this_00, 0.0)
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if hb_stk_a8 then
                    iVar5 = nil --[[reviewed native staging]]
                    uVar7 = quest:GetHero()
                    r5 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_INTRO", 0, false, true, false)
                    if not r5 then
                        goto LAB_00ed0a79
                    end
                    cVar4 = me:IsPerformingScriptTask()
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0a79 end
                        cVar4 = me:IsPerformingScriptTask()
                    end
                    goto LAB_00ed0952
                end
            elseif iVar5 == 1 then
                x_stk_30, uVar7 = nil, nil
                hb_stk_a8 = resources:ResourceThingHealthAbove(this_00, 0.0)
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if hb_stk_a8 then
                    iVar5 = nil --[[reviewed native staging]]
                    uVar7 = quest:GetHero()
                    r6 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE", 0, false, true, false)
                    if not r6 then
                        goto LAB_00ed0a79
                    end
                    cVar4 = me:IsPerformingScriptTask()
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0a79 end
                        cVar4 = me:IsPerformingScriptTask()
                    end
                    goto LAB_00ed0952
                end
            elseif iVar5 == 2 then
                x_stk_18, uVar7 = nil, nil
                hb_stk_a8 = resources:ResourceThingHealthAbove(this_00, 0.0)
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if hb_stk_a8 then
                    uVar7 = quest:GetHero()
                    r7 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE", 0, false, true, false)
                    if not r7 then
                        goto LAB_00ed0a79
                    end
                    cVar4 = me:IsPerformingScriptTask()
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0a79 end
                        cVar4 = me:IsPerformingScriptTask()
                    end
                    goto LAB_00ed0952
                end
            end
            goto FLOW_past_lab_00ed0952
            ::LAB_00ed0952::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            pCVar19 = pCVar9
            if bVar3 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(womanTalkMovie)
                goto LAB_00ed0ae8
            end
            ::FLOW_past_lab_00ed0952::
            iVar5 = quest:GetTimer(womanTimerId)
            quest:SetTimer(womanTimerId, iVar5 + 5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanTalkMovie)
        end
        if __native_entity_state:GetStateBool("GivenObject") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                resources:PrepareResource(this_00)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:RemoveThing(me, false, true)
                end
            end
            goto LAB_00ed0ae8
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
    ::LAB_00ed0a79::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(womanTalkMovie)
    goto LAB_00ed0ae8
    ::LAB_00ed0a36::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00ed0a62::
    resources:DestroyMovie(womanFunnyMovie)
    ::LAB_00ed0a6f::
    ::LAB_00ed0ae8::
    quest:DeregisterTimer(womanTimerId)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("GivenObject", false)
    __native_entity_state:SetStateInt("TimesMadeLaugh", 0)
end

function OnPersist(quest, me, context)
    local givenObject = __native_entity_state:GetStateBool("GivenObject") or false
    givenObject = quest:PersistTransferBool(context, "GivenObject", givenObject)
    __native_entity_state:SetStateBool("GivenObject", givenObject)
end

function OnPredicateFail(quest, me)
    local bVar1, bVar3, cVar2, xStack_4
    bVar3 = false
    if not __native_entity_state:GetStateBool("GivenObject") then
        bVar3 = true
        cVar2 = me:MsgIsKilledBy("")
        bVar1 = true
        if cVar2 then goto LAB_00ecd3d4 end
    end
    bVar1 = false
    ::LAB_00ecd3d4::
    if bVar3 then
    end
    if bVar1 then
        xStack_4 = quest:ReadGlobalGameDataString(0x738)
        quest:GiveHeroObject(xStack_4, -1, false)
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x768))
        quest:SetStateBool("LaughingWomanKilled", true)
    end
end

function helper_ED0B10(quest, me)
    local resources = quest:RetailResources()
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_28 = resources:NewActorMap()
    resources:SetActor(xStack_28, "HERO", xStack_10)
    resources:SetActor(xStack_28, "WOMAN", resources:MemberResource("seh_me", (me)))
    local xStack_1c = resources:NewStringMap()
    local xStack_2c = quest:ReadGlobalGameDataString(0x738)
    local pOther = (xStack_2c or "")
    resources:SetString(xStack_1c, "$ITEM", pOther)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_SICK_LAUGHING_WOMAN_LEAVES", xStack_28, xStack_1c, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:DestroyStringMap(xStack_1c)
    resources:DestroyActorMap(xStack_28)
    resources:ReleaseResource(xStack_10)
end

