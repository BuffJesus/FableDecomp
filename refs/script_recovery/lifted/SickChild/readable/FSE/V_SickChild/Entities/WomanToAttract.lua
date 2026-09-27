-- Readable native conversion: WomanToAttract. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local givenObject, timesMadeLaugh_

-- WomanToAttract.Main (retail 0x00ecf7f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult17, predicateResult18, predicateResult19
    local predicateResult20, scratchValue19, scratchValue20, scratchValue21, scratchValue22
    local scratchValue23, scratchValue24, health, getTimer, timesMadeLaugh, scratchValue35, getHero
    local scratchValue37, scratchValue38, scratchValue39, scratchValue40, conversationId, movie
    local movie2, movie3, scratchValue44, scratchValue46, scratchValue47
    if not quest:NewScriptFrame(me) then return end
    local this_00 = resources:MemberResource("seh_me", me)
    resources:PrepareResource(this_00)
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then return end
        scratchValue47 = this_00
    end
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateBool("LaughingWomanKilled") then
        quest:RemoveThing(me, false, true)
    end
    -- TODO(native): xStack_a8 = *(CScriptThing **)(this + 0xc);
    -- TODO(native): xStack_c8 = *(C3DMeshInfo **)(this + 0x10);
    if scratchValue47 ~= nil then
        -- TODO(native): *(int *)xStack_c8 = *(int *)xStack_c8 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    if not givenObject then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetThingHasInformation(me, false, false, false)
    end
    local x_stk_d0_2 = quest:RegisterTimer()
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(x_stk_d0_2)
            return
        end
        getTimer = quest:GetTimer(x_stk_d0_2)
        if getTimer == 0 then
            getHero = hero
            if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(hero); return end
                conversationId = quest:AddNewConversation(hero, unaff_EBP, false)
                quest:AddPersonToConversation(conversationId, hero)
                getTimer = timesMadeLaugh_
                if getTimer == 0 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED", hero, nil --[[missing]], false)
                    -- TODO(native): (**(code **)(*(int *)this_00 + 0x48))(&xStack_8c,0,0,1,0,true,0,0);
                else
                    if getTimer == 1 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT", hero, nil --[[missing]], false)
                        -- TODO(native): (**(code **)(*(int *)this_00 + 0x48))(xStack_9c,0,0,1,0,true,0,0);
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", hero, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                    if getTimer == 2 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL", hero, nil --[[missing]], false)
                        -- TODO(native): (**(code **)(*(int *)this_00 + 0x48))(&xStack_a8,0,0,1,0,true,0,0);
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", hero, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                end
                ::LAB_00ecfbad::
                quest:SetTimer(20, 15.0)
            end
        end
        -- TODO(native): cVar4 = (**(*pCVar9 + 0x74))("")
    --[[unresolved native value]]
        if nil and not givenObject then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            if scratchValue35 == nil then
                goto LAB_00ecff84
            end
            goto FLOW_past_lab_00ecff84
            ::LAB_00ecff84::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            if scratchValue35 == nil then
                if false then
                    goto LAB_00ed002a
                elseif false then
                    goto LAB_00ed005f
                elseif false then
                    goto LAB_00ed0091
                else
                    goto LAB_00ed00e0
                    goto LAB_00ed00c3
                end
                goto FLOW_hoist_lab_00ed00c3_3
            end
            goto FLOW_past_lab_00ed00c3
            ::LAB_00ed00c3::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed00c3_3::
            goto FLOW_hoist_lab_00ed0091_3
            ::FLOW_past_lab_00ed00c3::
            goto FLOW_past_lab_00ed0091
            ::LAB_00ed0091::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed0091_3::
            goto FLOW_hoist_lab_00ed005f_2
            ::FLOW_past_lab_00ed0091::
            goto FLOW_past_lab_00ed005f
            ::LAB_00ed005f::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_FLIRT"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed005f_2::
            goto FLOW_hoist_lab_00ed002a_1
            ::FLOW_past_lab_00ed005f::
            goto FLOW_past_lab_00ed002a
            ::LAB_00ed002a::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_RUDE"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed002a_1::
            goto FLOW_past_lab_00ed00db
            ::LAB_00ed00db::
            goto LAB_00ed00e0
            ::FLOW_past_lab_00ed00db::
            quest:DeregisterTimer(getHero)
            do return end
            ::FLOW_past_lab_00ed002a::
            -- TODO(native): pvVar11 = *pCVar19
            scratchValue40 = nil --[[unresolved native value]]
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SHIT");
            predicateResult17 = getTimer == 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if predicateResult17 then goto LAB_00ed002a end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLIRT");
            predicateResult18 = getTimer == 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if predicateResult18 then goto LAB_00ed005f end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_HEROIC_STANCE");
            predicateResult19 = getTimer == 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if predicateResult19 then goto LAB_00ed0091 end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_VICTORY_PUMP");
            predicateResult20 = getTimer == 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if predicateResult20 then goto LAB_00ed00c3 end
            ::LAB_00ed00e0::
            movie3 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): pCVar9 = (**(*this_00 + 0x30))(xStack_84)
            scratchValue37 = nil --[[unresolved native value]]
            health = quest:GetHealth(nil --[[missing]])
            if 0.0 < health then
                -- TODO(native): pCVar19 = *(this + 4)
                scratchValue35 = nil --[[unresolved native value]]
                -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                -- TODO(native): uVar7 = (**(*pCVar19 + 0x118))(pvVar11,uVar7,uVar8,uVar15,uVar16)
                conversationId = nil --[[unresolved native value]]
                -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                while nil --[[unresolved native value]] do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                        quest:DeregisterTimer(getHero)
                        return
                    end
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                    quest:DeregisterTimer(getHero)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
            ::LAB_00ed0694::
            -- TODO(native): unaff_EBP = pCVar9;
            goto LAB_00ed069d
            ::FLOW_past_lab_00ecff84::
            -- TODO(native): pvVar11 = *pCVar19
            scratchValue40 = nil --[[unresolved native value]]
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_PELVIC_THRUST");
            local scratchValue3 = getTimer == 0 and 1 or 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if not scratchValue3 then
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COCK_A_DOODLE_DO");
                local scratchValue4 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if not scratchValue4 then
                    -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_CROTCH_GRAB");
                    local scratchValue5 = getTimer == 0 and 1 or 0
                    -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                    if not scratchValue5 then
                        -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_KISS_MY_ASS");
                        local scratchValue6 = getTimer == 0 and 1 or 0
                        -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                        if not scratchValue6 then
                            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLAMENCO");
                            local scratchValue7 = getTimer == 0 and 1 or 0
                            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                            if not scratchValue7 then
                                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COSSACK");
                                local scratchValue8 = getTimer == 0 and 1 or 0
                                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                if not scratchValue8 then
                                    -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_AIR_GUITAR");
                                    local scratchValue9 = getTimer == 0 and 1 or 0
                                    -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                    if not scratchValue9 then
                                        -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_BALLET");
                                        local scratchValue = getTimer == 0 and 1 or 0
                                        -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                        if not scratchValue then
                                            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SATURDAY_NIGHT_FEVER");
                                            local scratchValue11 = getTimer == 0 and 1 or 0
                                            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                            if not scratchValue11 then
                                                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_TAP");
                                                local scratchValue12 = getTimer == 0 and 1 or 0
                                                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                                if not scratchValue12 then
                                                    -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_GIGGLE");
                                                    local scratchValue13 = getTimer == 0 and 1 or 0
                                                    -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                                                    if not scratchValue13 then goto LAB_00ecff84 end
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
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            timesMadeLaugh_ = timesMadeLaugh_ + 1
            if scratchValue35 == nil then
                if false then
                    goto LAB_00ed0245
                elseif false then
                    goto LAB_00ed027a
                else
                    goto LAB_00ed02ff
                    goto LAB_00ed04c2
                    goto FLOW_hoist_lab_00ed04c2_6
                    goto FLOW_hoist_lab_00ed04c2_7
                end
                goto FLOW_hoist_lab_00ed04c2_9
                goto FLOW_hoist_lab_00ed04c2_10
            end
            goto FLOW_past_lab_00ed04c2
            ::LAB_00ed04c2::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE"
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
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed02ac_2::
            goto FLOW_hoist_lab_00ed027a_2
            ::FLOW_hoist_lab_00ed02ac_3::
            goto FLOW_hoist_lab_00ed027a_3
            ::FLOW_past_lab_00ed02ac::
            goto FLOW_past_lab_00ed027a
            ::LAB_00ed027a::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed027a_2::
            goto FLOW_hoist_lab_00ed0245_1
            ::FLOW_hoist_lab_00ed027a_3::
            goto FLOW_hoist_lab_00ed0245_2
            ::FLOW_past_lab_00ed027a::
            goto FLOW_past_lab_00ed0245
            ::LAB_00ed0245::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed0245_1::
            quest:DeregisterTimer(getHero)
            do return end
            ::FLOW_hoist_lab_00ed0245_2::
            goto LAB_00ed0210
            ::FLOW_past_lab_00ed0245::
            goto FLOW_past_lab_00ed0210
            ::LAB_00ed0210::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE"
            ::LAB_00ed02fa::
            ::LAB_00ed02ff::
            movie2 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): pCVar9 = *(this + 4)
    --[[unresolved native value]]
            -- TODO(native): (**(code **)(*(int *)pCVar9 + 0x5ec))(1);
            timesMadeLaugh = timesMadeLaugh_
            getHero = nil
            if timesMadeLaugh ~= 1 then
                if timesMadeLaugh == 2 then
                    -- TODO(native): pCVar10 = (**(*this_00 + 0x30))(auStack_4c)
--[[unresolved native value]]
                    local health2 = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health2 then
                        -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                        conversationId = hero
                        -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                        while nil --[[unresolved native value]] do
                            if not quest:NewScriptFrame(me) then goto LAB_00ed0a36 end
                            -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ed0411 end
                    end
                elseif timesMadeLaugh == 3 then
                    DoLeavingCutscene(quest, me)
                    givenObject = true
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(1908))
                else
                    -- TODO(native): pCVar10 = (**(*this_00 + 0x30))(xStack_64)
--[[unresolved native value]]
                    if 0.0 < quest:GetHealth(conversationId) then
                        -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                        conversationId = hero
                        -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                        while nil --[[unresolved native value]] do
                            if not quest:NewScriptFrame(me) then goto LAB_00ed0a36 end
                            -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ed0411 end
                    end
                end
                goto FLOW_past_lab_00ed0411
                ::LAB_00ed0411::
                -- TODO(native): iVar5 = *pCVar9
--[[unresolved native value]]
                goto LAB_00ed0a5a
                ::FLOW_past_lab_00ed0411::
                goto LAB_00ed0666
            end
            goto FLOW_past_lab_00ed0666
            ::LAB_00ed0666::
            quest:SetTimer(0x1238c8c, 20)
            -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
            resources:DestroyMovie(movie3)
            goto LAB_00ed0694
            ::FLOW_past_lab_00ed0666::
            -- TODO(native): pCVar10 = (**(*this_00 + 0x30))(auStack_34)
--[[unresolved native value]]
            if 0.0 >= quest:GetHealth(conversationId) then goto LAB_00ed0666 end
            -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
            conversationId = hero
            -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
            -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
            scratchValue19 = nil --[[unresolved native value]]
            getHero = scratchValue35
            while scratchValue19 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(getHero)
                    return
                end
                -- TODO(native): cVar4 = (**(*this_00 + 0x68))(pvVar11)
                scratchValue19 = nil --[[unresolved native value]]
            end
            scratchValue35 = getHero
            if not quest:IsActiveThreadTerminating() then goto LAB_00ed0666 end
            -- TODO(native): iVar5 = *pCVar6
--[[unresolved native value]]
            ::LAB_00ed0a5a::
            -- TODO(native): (**(code **)(iVar5 + 0x5ec))(0);
            resources:DestroyMovie(movie2)
            quest:DeregisterTimer(getHero)
            do return end
            ::FLOW_past_lab_00ed0210::
            -- TODO(native): pvVar11 = *pCVar19
            scratchValue40 = nil --[[unresolved native value]]
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_PELVIC_THRUST");
            scratchValue20 = getTimer == 0 and 1 or 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if scratchValue20 then goto LAB_00ed0210 end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COCK_A_DOODLE_DO");
            scratchValue21 = getTimer == 0 and 1 or 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if scratchValue21 then goto LAB_00ed0245 end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_CROTCH_GRAB");
            scratchValue22 = getTimer == 0 and 1 or 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if scratchValue22 then goto LAB_00ed027a end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_KISS_MY_ASS");
            scratchValue23 = getTimer == 0 and 1 or 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if scratchValue23 then goto LAB_00ed02ac end
            -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLAMENCO");
            scratchValue24 = getTimer == 0 and 1 or 0
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
            if not scratchValue24 then
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_COSSACK");
                local scratchValue25 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if scratchValue25 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_AIR_GUITAR");
                local scratchValue26 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if scratchValue26 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_BALLET");
                local scratchValue27 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if scratchValue27 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_SATURDAY_NIGHT_FEVER");
                local scratchValue28 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if scratchValue28 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_TAP");
                local scratchValue29 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if scratchValue29 then goto LAB_00ed02e2 end
                -- TODO(native): iVar5 = CBasicString<char>::Compare(pvVar11,"EXPRESSION_GIGGLE");
                local scratchValue30 = getTimer == 0 and 1 or 0
                -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(cVar4,(undefined3)xStack_9c);
                if scratchValue30 then goto LAB_00ed04c2 end
                goto LAB_00ed02ff
            end
            ::LAB_00ed02e2::
            if not quest:IsActiveThreadTerminating() then scratchValue39 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE"; goto LAB_00ed02fa end
            quest:DeregisterTimer(getHero)
            return
        end
        ::LAB_00ed069d::
        -- TODO(native): cVar4 = (**(*pCVar9 + 0x6c))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
        if not nil then
            goto LAB_00ed06d4
        else
            -- TODO(native): xStack_9c = (CScriptThing *)CONCAT13(1,(undefined3)xStack_9c);
            if givenObject then goto LAB_00ed06d4 end
        end
        goto FLOW_past_lab_00ed06d4
        ::LAB_00ed06d4::
        -- TODO(native): xStack_9c = (CScriptThing *)((uint)xStack_9c & 0xffffff);
        ::FLOW_past_lab_00ed06d4::
        if scratchValue44 ~= 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            movie = resources:StartMovie("")
            getHero = 1
            quest:PauseAllNonScriptedEntities(true)
            local timesMadeLaugh2 = timesMadeLaugh_
            if timesMadeLaugh2 == 0 then
                -- TODO(native): uVar7 = (**(*this_00 + 0x30))(xStack_48)
                conversationId = nil --[[unresolved native value]]
                local health5 = quest:GetHealth(nil --[[missing]])
                -- TODO(native): xStack_a8 = (CScriptThing *)CONCAT13(1,(undefined3)xStack_a8);
                if health5 <= 0.0 then
                    -- TODO(native): xStack_a8 = (CScriptThing *)(uVar2 & 0xffffff);
                end
                scratchValue38 = scratchValue35
                scratchValue35 = scratchValue38
                if scratchValue46 ~= 0 then
                    -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                    conversationId = hero
                    -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                    while nil --[[unresolved native value]] do
                        if not quest:NewScriptFrame(me) then goto LAB_00ed0a79 end
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                    end
                    goto LAB_00ed0952
                end
            elseif timesMadeLaugh2 == 1 then
                -- TODO(native): uVar7 = (**(*this_00 + 0x30))(xStack_30)
                conversationId = nil --[[unresolved native value]]
                local health6 = quest:GetHealth(nil --[[missing]])
                -- TODO(native): xStack_a8 = (CScriptThing *)CONCAT13(1,(undefined3)xStack_a8);
                if health6 <= 0.0 then
                    -- TODO(native): xStack_a8 = (CScriptThing *)(uVar2 & 0xffffff);
                end
                scratchValue38 = scratchValue35
                scratchValue35 = scratchValue38
                if scratchValue46 ~= 0 then
                    -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                    conversationId = hero
                    -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                    while nil --[[unresolved native value]] do
                        if not quest:NewScriptFrame(me) then goto LAB_00ed0a79 end
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                    end
                    goto LAB_00ed0952
                end
            elseif timesMadeLaugh2 == 2 then
                -- TODO(native): uVar7 = (**(*this_00 + 0x30))(xStack_18)
                conversationId = nil --[[unresolved native value]]
                local health7 = quest:GetHealth(nil --[[missing]])
                -- TODO(native): xStack_a8 = (CScriptThing *)CONCAT13(1,(undefined3)xStack_a8);
                if health7 <= 0.0 then
                    -- TODO(native): xStack_a8 = (CScriptThing *)(uVar2 & 0xffffff);
                end
                scratchValue38 = scratchValue35
                scratchValue35 = scratchValue38
                if scratchValue46 ~= 0 then
                    -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                    conversationId = hero
                    -- TODO(native): (**(code **)(iVar5 + 0x34))(uVar7);
                    -- TODO(native): cVar4 = (**(*this_00 + 0x68))(uVar7)
                    while nil --[[unresolved native value]] do
                        if not quest:NewScriptFrame(me) then goto LAB_00ed0a79 end
                        -- TODO(native): cVar4 = (**(*this_00 + 0x68))()
                    end
                    goto LAB_00ed0952
                end
            end
            goto FLOW_past_lab_00ed0952
            ::LAB_00ed0952::
            scratchValue35 = scratchValue38
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DeregisterTimer(1)
                return
            end
            ::FLOW_past_lab_00ed0952::
            quest:SetTimer(x_stk_d0_2, quest:GetTimer(x_stk_d0_2) + 5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        if givenObject then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(getHero); return end
            resources:PrepareResource(this_00)
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveThing(me, false, true)
            end
            quest:DeregisterTimer(getHero)
            return
        end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
    ::LAB_00ed0a79::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    quest:DeregisterTimer(getHero)
    do return end
    ::LAB_00ed0a36::
    -- TODO(native): (**(code **)(*(int *)pCVar9 + 0x5ec))(0);
    resources:DestroyMovie(movie2)
    quest:DeregisterTimer(nil)
end

-- WomanToAttract.Init (retail 0x00ecd380)
function Init(quest, me)
    givenObject = false
    timesMadeLaugh_ = 0
end

-- WomanToAttract.OnPersist (retail 0x00ecd9b0)
function OnPersist(quest, me, context)
    quest:SetStateBool("GivenObject", quest:PersistTransferBool(context, "GivenObject", quest:GetStateBool("GivenObject")))
end

-- WomanToAttract.OnPredicateFail (retail 0x00ecd390)
function OnPredicateFail(quest, me)
    local predicateResult
    if not givenObject then
        predicateResult = true
        if me:MsgIsKilledBy("") then goto LAB_00ecd3d4 end
    end
    predicateResult = false
    ::LAB_00ecd3d4::
    if not predicateResult then return end
    quest:GiveHeroObject(quest:ReadGlobalGameDataString(1848), -1, false)
    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1896))
    quest:SetStateBool("LaughingWomanKilled", true)
end

-- WomanToAttract.DoLeavingCutscene (retail 0x00ed0b10)
-- ED0B10: bsim names this body NScript::CV_SickChildScript::CWomanToAttract::DoLeavingCutscene (a homologous script member); no PDB name
function DoLeavingCutscene(quest, me)
    local resources = quest:RetailResources()
    local pOther, actorMap
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "HERO", resource)
    resources:SetActor(actorMap2, "WOMAN", resources:MemberResource("seh_me", me))
    -- TODO(native): xStack_1c = malloc(0x18);
    -- TODO(native): *xStack_1c = 0;
    -- TODO(native): *(undefined4 *)(xStack_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(xStack_1c + 8) = xStack_1c;
    -- TODO(native): *(undefined1 **)(xStack_1c + 0xc) = xStack_1c;
    quest:ReadGlobalGameDataString(1848)
    resources:SetString(actorMap, "$ITEM", pOther)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_SICK_LAUGHING_WOMAN_LEAVES", actorMap2, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    if actorMap ~= nil then
        -- TODO(native): free(xStack_1c);
    end
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource)
end

