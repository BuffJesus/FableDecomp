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
    local au_stk_34, au_stk_4c, au_stk_64, bVar3, cVar4, dist, fVar14, hb_stk_9c, hb_stk_a8, iVar5, pCVar10, pCVar17, pCVar19, pCVar6, pCVar9, pcVar18, pvVar11, r1, r2, r3, r4, r5, r6, r7, this_00, uVar15, uVar16, uVar7, uVar7_pushed, uVar8, xStack_64, xStack_74, xStack_84, xStack_8c, xStack_9c, xStack_a8, xStack_c8, x_stk_18, x_stk_30, x_stk_48, x_stk_d0
    local alive = true
    xStack_8c = pCVar9
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    this_00 = resources:MemberResource("seh_me", me)
    resources:PrepareResource(this_00)
    xStack_9c = pCVar9
    x_stk_d0 = this_00
    cVar4 = me:AcquireControl(4)
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
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
        xStack_a8 = pCVar9
        quest:RemoveThing(me, false, true)
    end
    -- TODO(native): xStack_a8 = *(CScriptThing **)(this + 0xc);
    -- TODO(native): xStack_c8 = *(C3DMeshInfo **)(this + 0x10);
    if xStack_c8 ~= nil then
        -- TODO(native): *(int *)xStack_c8 = *(int *)xStack_c8 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    if not __native_entity_state:GetStateBool("GivenObject") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:SetThingHasInformation(me, false, false, false)
    end
    x_stk_d0 = quest:RegisterTimer()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            quest:DeregisterTimer(x_stk_d0)
            return
        end
        iVar5 = quest:GetTimer(x_stk_d0)
        if iVar5 == 0 then
            dist = 15.0
            pCVar6 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, dist)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00ed0ae8 end
                uVar7 = quest:AddNewConversation(pCVar6, unaff_EBP, false)
                uVar8 = quest:GetHero()
                quest:AddPersonToConversation(uVar7, uVar8)
                iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
                if iVar5 == 0 then
                    uVar8 = quest:GetHero()
                    quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED", uVar8, nil --[[missing]], false)
                    me:PlayAnimation("STANDARD_TALK_SADNESS", false, false, true, false, true, false, false)
                else
                    if iVar5 == 1 then
                        uVar8 = quest:GetHero()
                        pCVar17 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT"
                        quest:AddLineToConversation(uVar7, pCVar17, uVar8, nil --[[missing]], false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_SNIGGER", false, false, true, false, true, false, false)
                        uVar8 = quest:GetHero()
                        quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", uVar8, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                    if iVar5 == 2 then
                        uVar8 = quest:GetHero()
                        pCVar17 = "TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL"
                        quest:AddLineToConversation(uVar7, pCVar17, uVar8, nil --[[missing]], false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_POINT_AND_LAUGH", false, false, true, false, true, false, false)
                        uVar8 = quest:GetHero()
                        quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", uVar8, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                end
                ::LAB_00ecfbad::
                quest:SetTimer(0x14, dist)
                pCVar9 = unaff_EBP
            end
        end
        -- TODO(native): cVar4 = (**(*pCVar9 + 0x74))("")
        cVar4 = nil --[[unresolved native value]]
        if (cVar4) and (not __native_entity_state:GetStateBool("GivenObject")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0ae8 end
            if pCVar19 == nil then
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
            if pCVar19 == nil then
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
            goto LAB_00ed00e0
            ::FLOW_past_lab_00ed00db::
            goto LAB_00ed0a6f
            ::FLOW_past_lab_00ed002a::
            iVar5 = ((pCVar19 == "EXPRESSION_SHIT") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed002a end
            iVar5 = ((pCVar19 == "EXPRESSION_FLIRT") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed005f end
            iVar5 = ((pCVar19 == "EXPRESSION_HEROIC_STANCE") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed0091 end
            iVar5 = ((pCVar19 == "EXPRESSION_VICTORY_PUMP") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed00c3 end
            ::LAB_00ed00e0::
            xStack_84 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            xStack_84 = resources:ScriptThing(this_00)
            pCVar9 = xStack_84
            fVar14 = quest:GetHealth(pCVar9)
            cVar4 = 0.0 < fVar14
            if cVar4 then
                -- TODO(native): pCVar19 = *(this + 4)
                pCVar19 = nil --[[unresolved native value]]
                uVar16 = 0
                uVar15 = 1
                uVar8 = 0
                uVar7 = 0
                pvVar11 = pcVar18
                uVar7_pushed = uVar7
                uVar7 = quest:GetHero()
                r1 = me:Speak(uVar7, pvVar11, uVar7_pushed, (uVar8 ~= 0), (uVar15 ~= 0), (uVar16 ~= 0))
                cVar4 = me:IsPerformingScriptTask()
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                        goto LAB_00ed0ae8
                    end
                    cVar4 = me:IsPerformingScriptTask()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                    goto LAB_00ed0ae8
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
            pCVar9 = unaff_EBP
            ::LAB_00ed0694::
            -- TODO(native): unaff_EBP = pCVar9;
            goto LAB_00ed069d
            ::FLOW_past_lab_00ecff84::
            iVar5 = ((pCVar19 == "EXPRESSION_PELVIC_THRUST") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if not cVar4 then
                iVar5 = ((pCVar19 == "EXPRESSION_COCK_A_DOODLE_DO") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if not cVar4 then
                    iVar5 = ((pCVar19 == "EXPRESSION_CROTCH_GRAB") and 0 or 1)
                    cVar4 = not (iVar5 ~= 0)
                    hb_stk_9c = cVar4
                    if not cVar4 then
                        iVar5 = ((pCVar19 == "EXPRESSION_KISS_MY_ASS") and 0 or 1)
                        cVar4 = not (iVar5 ~= 0)
                        hb_stk_9c = cVar4
                        if not cVar4 then
                            iVar5 = ((pCVar19 == "EXPRESSION_FLAMENCO") and 0 or 1)
                            cVar4 = not (iVar5 ~= 0)
                            hb_stk_9c = cVar4
                            if not cVar4 then
                                iVar5 = ((pCVar19 == "EXPRESSION_COSSACK") and 0 or 1)
                                cVar4 = not (iVar5 ~= 0)
                                hb_stk_9c = cVar4
                                if not cVar4 then
                                    iVar5 = ((pCVar19 == "EXPRESSION_AIR_GUITAR") and 0 or 1)
                                    cVar4 = not (iVar5 ~= 0)
                                    hb_stk_9c = cVar4
                                    if not cVar4 then
                                        iVar5 = ((pCVar19 == "EXPRESSION_BALLET") and 0 or 1)
                                        cVar4 = not (iVar5 ~= 0)
                                        hb_stk_9c = cVar4
                                        if not cVar4 then
                                            iVar5 = ((pCVar19 == "EXPRESSION_SATURDAY_NIGHT_FEVER") and 0 or 1)
                                            cVar4 = not (iVar5 ~= 0)
                                            hb_stk_9c = cVar4
                                            if not cVar4 then
                                                iVar5 = ((pCVar19 == "EXPRESSION_TAP") and 0 or 1)
                                                cVar4 = not (iVar5 ~= 0)
                                                hb_stk_9c = cVar4
                                                if not cVar4 then
                                                    iVar5 = ((pCVar19 == "EXPRESSION_GIGGLE") and 0 or 1)
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
            if pCVar19 == nil then
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
            ::LAB_00ed02ff::
            xStack_74 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): pCVar9 = *(this + 4)
            pCVar9 = nil --[[unresolved native value]]
            -- TODO(native): (**(code **)(*(int *)pCVar9 + 0x5ec))(1);
            iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
            pCVar6 = pCVar9
            if iVar5 ~= 1 then
                if iVar5 == 2 then
                    au_stk_4c = resources:ScriptThing(this_00)
                    pCVar10 = au_stk_4c
                    fVar14 = quest:GetHealth(pCVar10)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        uVar7 = quest:GetHero()
                        r2 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", 0, false, true, false)
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
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(0x774))
                else
                    au_stk_64 = resources:ScriptThing(this_00)
                    pCVar10 = au_stk_64
                    fVar14 = quest:GetHealth(pCVar10)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        uVar7 = quest:GetHero()
                        r3 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", 0, false, true, false)
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
                -- TODO(native): iVar5 = *pCVar9
                iVar5 = nil --[[unresolved native value]]
                goto LAB_00ed0a5a
                ::FLOW_past_lab_00ed0411::
                goto LAB_00ed0666
            end
            goto FLOW_past_lab_00ed0666
            ::LAB_00ed0666::
            quest:SetTimer(0x1238c8c, 0x14)
            -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
            resources:DestroyMovie(xStack_84)
            goto LAB_00ed0694
            ::FLOW_past_lab_00ed0666::
            au_stk_34 = resources:ScriptThing(this_00)
            pCVar10 = au_stk_34
            fVar14 = quest:GetHealth(pCVar10)
            cVar4 = 0.0 < fVar14
            if not cVar4 then goto LAB_00ed0666 end
            uVar16 = 0
            uVar15 = 1
            uVar8 = 0
            uVar7 = 0
            pvVar11 = pcVar18
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
                    -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
                    goto LAB_00ed0a62
                end
                cVar4 = me:IsPerformingScriptTask()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            pCVar19 = pCVar6
            if not bVar3 then goto LAB_00ed0666 end
            -- TODO(native): iVar5 = *pCVar6
            iVar5 = nil --[[unresolved native value]]
            ::LAB_00ed0a5a::
            -- TODO(native): (**(code **)(iVar5 + 0x5ec))(0);
            goto LAB_00ed0a62
            ::FLOW_past_lab_00ed0210::
            iVar5 = ((pCVar19 == "EXPRESSION_PELVIC_THRUST") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed0210 end
            iVar5 = ((pCVar19 == "EXPRESSION_COCK_A_DOODLE_DO") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed0245 end
            iVar5 = ((pCVar19 == "EXPRESSION_CROTCH_GRAB") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed027a end
            iVar5 = ((pCVar19 == "EXPRESSION_KISS_MY_ASS") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if cVar4 then goto LAB_00ed02ac end
            iVar5 = ((pCVar19 == "EXPRESSION_FLAMENCO") and 0 or 1)
            cVar4 = not (iVar5 ~= 0)
            hb_stk_9c = cVar4
            if not cVar4 then
                iVar5 = ((pCVar19 == "EXPRESSION_COSSACK") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((pCVar19 == "EXPRESSION_AIR_GUITAR") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((pCVar19 == "EXPRESSION_BALLET") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((pCVar19 == "EXPRESSION_SATURDAY_NIGHT_FEVER") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((pCVar19 == "EXPRESSION_TAP") and 0 or 1)
                cVar4 = not (iVar5 ~= 0)
                hb_stk_9c = cVar4
                if cVar4 then goto LAB_00ed02e2 end
                iVar5 = ((pCVar19 == "EXPRESSION_GIGGLE") and 0 or 1)
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
        -- TODO(native): cVar4 = (**(*pCVar9 + 0x6c))("SCRIPT_NAME_HERO")
        cVar4 = nil --[[unresolved native value]]
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
            xStack_64 = resources:StartMovie("")
            pCVar6 = 0x1
            quest:PauseAllNonScriptedEntities(true)
            iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
            if iVar5 == 0 then
                x_stk_48 = resources:ScriptThing(this_00)
                uVar7 = x_stk_48
                fVar14 = quest:GetHealth(uVar7)
                hb_stk_a8 = true
                if fVar14 <= 0.0 then
                    hb_stk_a8 = false
                end
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if hb_stk_a8 then
                    -- TODO(native): iVar5 = *this_00
                    iVar5 = nil --[[unresolved native value]]
                    uVar7 = quest:GetHero()
                    r5 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_INTRO", 0, false, true, false)
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
                x_stk_30 = resources:ScriptThing(this_00)
                uVar7 = x_stk_30
                fVar14 = quest:GetHealth(uVar7)
                hb_stk_a8 = true
                if fVar14 <= 0.0 then
                    hb_stk_a8 = false
                end
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if hb_stk_a8 then
                    -- TODO(native): iVar5 = *this_00
                    iVar5 = nil --[[unresolved native value]]
                    uVar7 = quest:GetHero()
                    r6 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE", 0, false, true, false)
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
                x_stk_18 = resources:ScriptThing(this_00)
                uVar7 = x_stk_18
                fVar14 = quest:GetHealth(uVar7)
                hb_stk_a8 = true
                if fVar14 <= 0.0 then
                    hb_stk_a8 = false
                end
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if hb_stk_a8 then
                    uVar7 = quest:GetHero()
                    r7 = me:Speak(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE", 0, false, true, false)
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
                resources:DestroyMovie(xStack_64)
                goto LAB_00ed0ae8
            end
            ::FLOW_past_lab_00ed0952::
            iVar5 = quest:GetTimer(x_stk_d0)
            quest:SetTimer(x_stk_d0, iVar5 + 5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_64)
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
    resources:DestroyMovie(xStack_64)
    goto LAB_00ed0ae8
    ::LAB_00ed0a36::
    -- TODO(native): (**(code **)(*(int *)pCVar9 + 0x5ec))(0);
    ::LAB_00ed0a62::
    resources:DestroyMovie(xStack_74)
    ::LAB_00ed0a6f::
    ::LAB_00ed0ae8::
    quest:DeregisterTimer(hb_stk_a8)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("GivenObject", false)
    __native_entity_state:SetStateInt("TimesMadeLaugh", 0)
end

function OnPersist(quest, me, context)
    local givenObject = quest:GetStateBool("GivenObject") or false
    givenObject = quest:PersistTransferBool(context, "GivenObject", givenObject)
    quest:SetStateBool("GivenObject", givenObject)
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
    local i_stk_18, pOther, xStack_1c
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_28 = resources:NewActorMap()
    resources:SetActor(xStack_28, "HERO", xStack_10)
    resources:SetActor(xStack_28, "WOMAN", resources:MemberResource("seh_me", (me)))
    -- TODO(native): xStack_1c = malloc(0x18);
    i_stk_18 = 0
    -- TODO(native): *xStack_1c = 0;
    -- TODO(native): *(undefined4 *)(xStack_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(xStack_1c + 8) = xStack_1c;
    -- TODO(native): *(undefined1 **)(xStack_1c + 0xc) = xStack_1c;
    local xStack_2c = quest:ReadGlobalGameDataString(0x738)
    resources:SetString(xStack_1c, "$ITEM", pOther)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_SICK_LAUGHING_WOMAN_LEAVES", xStack_28, xStack_1c, false, true)
    quest:FixMovieSequenceCamera(false)
    if i_stk_18 ~= 0 then
        -- TODO(native): LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray((LTextTreeWalkThrough *)&xStack_1c,*(int *)(xStack_1c + 4));
        -- TODO(native): *(undefined1 **)(xStack_1c + 8) = xStack_1c;
        -- TODO(native): *(undefined4 *)(xStack_1c + 4) = 0;
        -- TODO(native): *(undefined1 **)(xStack_1c + 0xc) = xStack_1c;
        i_stk_18 = 0
    end
    if xStack_1c ~= nil then
        -- TODO(native): free(xStack_1c);
    end
    resources:DestroyActorMap(xStack_28)
    resources:ReleaseResource(xStack_10)
end

