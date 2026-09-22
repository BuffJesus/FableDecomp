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
    local bVar3, cVar4, dist, fVar14, iVar5, pCVar10, pCVar17, pCVar19, pCVar6, pCVar9, pcVar18, pvVar11, this_00, uVar15, uVar16, uVar2, uVar7, uVar8, xStack_64, xStack_74, xStack_84, xStack_8c, xStack_9c, xStack_9c_b3, xStack_a8, xStack_a8_b3, xStack_c8, x_stk_d0
    local alive = true
    xStack_8c = pCVar9
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    this_00 = (this + 0x24)
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
                    -- TODO(native): (**(code **)(*(int *)this_00 + 0x48))(&xStack_8c,0,0,1,0,true,0,0);
                else
                    if iVar5 == 1 then
                        uVar8 = quest:GetHero()
                        pCVar17 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT"
                        quest:AddLineToConversation(uVar7, pCVar17, uVar8, nil --[[missing]], false)
                        -- TODO(native): (**(code **)(*(int *)this_00 + 0x48))(xStack_9c,0,0,1,0,true,0,0);
                        uVar8 = quest:GetHero()
                        quest:AddLineToConversation(uVar7, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", uVar8, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                    if iVar5 == 2 then
                        uVar8 = quest:GetHero()
                        pCVar17 = "TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL"
                        quest:AddLineToConversation(uVar7, pCVar17, uVar8, nil --[[missing]], false)
                        -- TODO(native): (**(code **)(*(int *)this_00 + 0x48))(&xStack_a8,0,0,1,0,true,0,0);
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
        if (cVar4 ~= 0) and (not __native_entity_state:GetStateBool("GivenObject")) then
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
            -- TODO(native): pvVar11 = *pCVar19
            pvVar11 = nil --[[unresolved native value]]
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SHIT");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed002a end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLIRT");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed005f end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_HEROIC_STANCE");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed0091 end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_VICTORY_PUMP");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed00c3 end
            ::LAB_00ed00e0::
            xStack_84 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): pCVar9 = (**(*this_00 + 0x30))(xStack_84)
            pCVar9 = nil --[[unresolved native value]]
            fVar14 = quest:GetHealth(nil --[[missing]])
            cVar4 = 0.0 < fVar14
            if cVar4 then
                -- TODO(native): pCVar19 = *(this + 4)
                pCVar19 = nil --[[unresolved native value]]
                -- TODO(native): iVar5 = *this_00
                iVar5 = nil --[[unresolved native value]]
                uVar16 = 0
                uVar15 = 1
                uVar8 = 0
                uVar7 = 0
                pvVar11 = pcVar18
                -- TODO(native): uVar7 = (**(*pCVar19 + 0x118))(pvVar11,uVar7,uVar8,uVar15,uVar16)
                uVar7 = nil --[[unresolved native value]]
                -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                cVar4 = nil --[[unresolved native value]]
                while cVar4 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                        goto LAB_00ed0ae8
                    end
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(&xStack_60)
                    cVar4 = nil --[[unresolved native value]]
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
            -- TODO(native): pvVar11 = *pCVar19
            pvVar11 = nil --[[unresolved native value]]
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_PELVIC_THRUST");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if not cVar4 then
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COCK_A_DOODLE_DO");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if not cVar4 then
                    -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_CROTCH_GRAB");
                    cVar4 = not (iVar5 ~= 0)
                    -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                    if not cVar4 then
                        -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_KISS_MY_ASS");
                        cVar4 = not (iVar5 ~= 0)
                        -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                        if not cVar4 then
                            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLAMENCO");
                            cVar4 = not (iVar5 ~= 0)
                            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                            if not cVar4 then
                                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COSSACK");
                                cVar4 = not (iVar5 ~= 0)
                                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                if not cVar4 then
                                    -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_AIR_GUITAR");
                                    cVar4 = not (iVar5 ~= 0)
                                    -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                    if not cVar4 then
                                        -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_BALLET");
                                        cVar4 = not (iVar5 ~= 0)
                                        -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                        if not cVar4 then
                                            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SATURDAY_NIGHT_FEVER");
                                            cVar4 = not (iVar5 ~= 0)
                                            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                            if not cVar4 then
                                                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_TAP");
                                                cVar4 = not (iVar5 ~= 0)
                                                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                                if not cVar4 then
                                                    -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_GIGGLE");
                                                    cVar4 = not (iVar5 ~= 0)
                                                    -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
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
                    -- TODO(native): pCVar10 = (**(*this_00 + 0x30))(auStack_4c)
                    pCVar10 = nil --[[unresolved native value]]
                    fVar14 = quest:GetHealth(nil --[[missing]])
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        -- TODO(native): iVar5 = *this_00
                        iVar5 = nil --[[unresolved native value]]
                        uVar7 = quest:GetHero()
                        -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                        cVar4 = nil --[[unresolved native value]]
                        while cVar4 ~= 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00ed0a36 end
                            -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                            cVar4 = nil --[[unresolved native value]]
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
                    -- TODO(native): pCVar10 = (**(*this_00 + 0x30))(xStack_64)
                    pCVar10 = nil --[[unresolved native value]]
                    fVar14 = quest:GetHealth(uVar7)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        -- TODO(native): iVar5 = *this_00
                        iVar5 = nil --[[unresolved native value]]
                        uVar7 = quest:GetHero()
                        -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                        cVar4 = nil --[[unresolved native value]]
                        while cVar4 ~= 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00ed0a36 end
                            -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                            cVar4 = nil --[[unresolved native value]]
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
            -- TODO(native): pCVar10 = (**(*this_00 + 0x30))(auStack_34)
            pCVar10 = nil --[[unresolved native value]]
            fVar14 = quest:GetHealth(uVar7)
            cVar4 = 0.0 < fVar14
            if not cVar4 then goto LAB_00ed0666 end
            -- TODO(native): iVar5 = *this_00
            iVar5 = nil --[[unresolved native value]]
            uVar16 = 0
            uVar15 = 1
            uVar8 = 0
            uVar7 = 0
            pvVar11 = pcVar18
            uVar7 = quest:GetHero()
            -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
            -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
            cVar4 = nil --[[unresolved native value]]
            pCVar6 = pCVar19
            while cVar4 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
                    goto LAB_00ed0a62
                end
                -- TODO(native): cVar4 = (**(*this_00 + 0x68))(pvVar11)
                cVar4 = nil --[[unresolved native value]]
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
            -- TODO(native): pvVar11 = *pCVar19
            pvVar11 = nil --[[unresolved native value]]
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_PELVIC_THRUST");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed0210 end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COCK_A_DOODLE_DO");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed0245 end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_CROTCH_GRAB");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed027a end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_KISS_MY_ASS");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if cVar4 then goto LAB_00ed02ac end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLAMENCO");
            cVar4 = not (iVar5 ~= 0)
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if not cVar4 then
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COSSACK");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if cVar4 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_AIR_GUITAR");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if cVar4 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_BALLET");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if cVar4 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SATURDAY_NIGHT_FEVER");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if cVar4 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_TAP");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if cVar4 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_GIGGLE");
                cVar4 = not (iVar5 ~= 0)
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
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
        if cVar4 == 0 then
            goto LAB_00ed06d4
        else
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(1,(undefined3)xStack_9c);
            if __native_entity_state:GetStateBool("GivenObject") then goto LAB_00ed06d4 end
        end
        goto FLOW_past_lab_00ed06d4
        ::LAB_00ed06d4::
        -- TODO(native): xStack_9c = (CScriptThing *)((uint)xStack_9c & 0xffffff);
        ::FLOW_past_lab_00ed06d4::
        if xStack_9c_b3 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ed0ae8 end
            xStack_64 = resources:StartMovie("")
            pCVar6 = 0x1
            quest:PauseAllNonScriptedEntities(true)
            iVar5 = __native_entity_state:GetStateInt("TimesMadeLaugh")
            if iVar5 == 0 then
                -- TODO(native): uVar7 = (**(*this_00 + 0x30))(xStack_48)
                uVar7 = nil --[[unresolved native value]]
                fVar14 = quest:GetHealth(nil --[[missing]])
                uVar2 = ""
                -- TODO(native): xStack_a8 = (CScriptThing *)CONCAT13(1,(undefined3)xStack_a8);
                if fVar14 <= 0.0 then
                    -- TODO(native): xStack_a8 = (CScriptThing *)(uVar2 & 0xffffff);
                end
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if xStack_a8_b3 ~= 0 then
                    -- TODO(native): iVar5 = *this_00
                    iVar5 = nil --[[unresolved native value]]
                    uVar7 = quest:GetHero()
                    -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                    cVar4 = nil --[[unresolved native value]]
                    while cVar4 ~= 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0a79 end
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                        cVar4 = nil --[[unresolved native value]]
                    end
                    goto LAB_00ed0952
                end
            elseif iVar5 == 1 then
                -- TODO(native): uVar7 = (**(*this_00 + 0x30))(xStack_30)
                uVar7 = nil --[[unresolved native value]]
                fVar14 = quest:GetHealth(nil --[[missing]])
                uVar2 = xStack_a8
                -- TODO(native): xStack_a8 = (CScriptThing *)CONCAT13(1,(undefined3)xStack_a8);
                if fVar14 <= 0.0 then
                    -- TODO(native): xStack_a8 = (CScriptThing *)(uVar2 & 0xffffff);
                end
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if xStack_a8_b3 ~= 0 then
                    -- TODO(native): iVar5 = *this_00
                    iVar5 = nil --[[unresolved native value]]
                    uVar7 = quest:GetHero()
                    -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                    cVar4 = nil --[[unresolved native value]]
                    while cVar4 ~= 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0a79 end
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                        cVar4 = nil --[[unresolved native value]]
                    end
                    goto LAB_00ed0952
                end
            elseif iVar5 == 2 then
                -- TODO(native): uVar7 = (**(*this_00 + 0x30))(xStack_18)
                uVar7 = nil --[[unresolved native value]]
                fVar14 = quest:GetHealth(nil --[[missing]])
                uVar2 = xStack_a8
                -- TODO(native): xStack_a8 = (CScriptThing *)CONCAT13(1,(undefined3)xStack_a8);
                if fVar14 <= 0.0 then
                    -- TODO(native): xStack_a8 = (CScriptThing *)(uVar2 & 0xffffff);
                end
                pCVar9 = pCVar19
                pCVar19 = pCVar9
                if xStack_a8_b3 ~= 0 then
                    -- TODO(native): iVar5 = *this_00
                    iVar5 = nil --[[unresolved native value]]
                    uVar7 = quest:GetHero()
                    -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                    cVar4 = nil --[[unresolved native value]]
                    while cVar4 ~= 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ed0a79 end
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                        cVar4 = nil --[[unresolved native value]]
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
    quest:DeregisterTimer(pCVar6)
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
    local bVar1, bVar3, cVar2
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
        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_4);
        quest:GiveHeroObject(nil --[[missing]], -1, false)
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x768))
        quest:SetStateBool("LaughingWomanKilled", true)
    end
end

function helper_ED0B10(quest, me)
    local resources = quest:RetailResources()
    local CVar3, i_stk_18, pCVar6, pOther, xStack_1c, xStack_2c
    local xStack_10 = resources:NewResource()
    local pScriptObject = 0x0
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_28 = resources:NewActorMap()
    resources:SetActor(xStack_28, "HERO", 0x0)
    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_28,&xStack_30);
    -- TODO(native): xStack_2c = *(CCharString *)(this + 0x30);
    -- TODO(native): local CVar3 = *(pCVar6 + 0xc)
    local uVar4 = __native_entity_state:GetStateInt("self_0x2c")
    if CVar3 ~= xStack_2c then
        if CVar3 ~= nil then
            -- TODO(native): *(int *)CVar3 = *(int *)CVar3 + -1;
            if **(pCVar6 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar6 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar6 + 8) = uVar4;
        -- TODO(native): *(CCharString *)(pCVar6 + 0xc) = xStack_2c;
        if xStack_2c ~= nil then
            -- TODO(native): *(int *)xStack_2c = *(int *)xStack_2c + 1;
        end
    end
    -- TODO(native): xStack_1c = malloc(0x18);
    i_stk_18 = 0
    -- TODO(native): *xStack_1c = 0;
    -- TODO(native): *(undefined4 *)(xStack_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(xStack_1c + 8) = xStack_1c;
    -- TODO(native): *(undefined1 **)(xStack_1c + 0xc) = xStack_1c;
    -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_2c);
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
    resources:ReleaseResource(0x0)
end

