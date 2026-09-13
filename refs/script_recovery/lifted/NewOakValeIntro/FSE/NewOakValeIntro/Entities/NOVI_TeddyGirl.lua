-- Generated native draft: NOVI_TeddyGirl. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("DoneIntro", false)
    __native_entity_state:SetStateBool("FoundTeddy", false)
    __native_entity_state:SetStateBool("SpokeAboutFindingTeddy", false)
    __native_entity_state:SetStateBool("HeroHitMe", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
end

function Main(quest, me)
    local CVar22, CVar24, __native_condition_1, bVar6, cVar5, fVar18, fVar25, fVar3, iVar9, pCVar1, pCVar13, pCVar14, pCVar20, pCVar21, pCVar23, pCVar26, pCVar8, pcVar19, piVar16, piVar2, ppVar12, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r2, r3, r4, r5, r6, r7, r8, r9, uVar10, uVar4, uVar7
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    r1 = quest:GetThingWithScriptName("NOVI_Bully")
    alive = not quest:IsActiveThreadTerminating()
    cVar5 = not alive
    repeat
        if cVar5 then
            return
        end
        uVar4 = auStack_13c[0]
        pCVar26 = "SCRIPT_NAME_HERO"
        -- TODO(native): auStack_13c[0] = auStack_13c[0] | 1;
        cVar5 = me:IsTalkedToByHero()
        if not cVar5 then
            -- LAB_00daf1a4: (native jump target)
            -- TODO(native): uStack_150 = (int *)((uint)uStack_150 & 0xffffff);
        else
            -- TODO(native): piStack_140 = (int *)(uVar4 | 3);
            r2 = quest:GetHero()
            -- TODO(native): IsObjectInThingsPossession is not a ForgeFSE binding
            quest:IsObjectInThingsPossession()
            -- TODO(native): uStack_150 = (int *)CONCAT13(1,(undefined3)uStack_150);
            if not cVar5 then return end  -- TODO(native): goto LAB_00daf1a4
        end
        if (piStack_140 & 2) ~= 0 then
            -- TODO(native): piStack_140 = (int *)((uint)piStack_140 & 0xfffffffd);
        end
        if (piStack_140 & 1) ~= 0 then
            -- TODO(native): piStack_140 = (int *)((uint)piStack_140 & 0xfffffffe);
        end
        if uStack_150._3_1_ ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db05da end
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea0);
            if bVar6 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db05da end
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db05da end
            pCVar20 = ""
            quest:StartMovieSequence()
            CVar24 = 0x1
            quest:PauseAllNonScriptedEntities((CVar24 ~= 0))
            if not __native_entity_state:GetStateBool("DoneIntro") then
                if not __native_entity_state:GetStateBool("HeroHitMe") then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                        fVar18 = quest:GetHealth(r2)
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            bVar6 = false
                            pCVar23 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY"
                            pCVar8 = quest:GetHero()
                            r3 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00daf38a
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db04ea end
                        end
                        -- TODO(native): goto LAB_00daf457
                    end
                    ::LAB_00db04ea::
                    quest:PauseAllNonScriptedEntities(false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                        fVar18 = quest:GetHealth(r1)
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            bVar6 = false
                            pCVar23 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN"
                            pCVar8 = quest:GetHero()
                            r4 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00db04ea
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daf38a end
                        end
                        -- LAB_00daf457: (native jump target)
                        __native_entity_state:SetStateBool("DoneIntro", true)
                        -- TODO(native): goto LAB_00daf45b
                    end
                    ::LAB_00daf38a::
                    quest:PauseAllNonScriptedEntities(false)
                end
            else
                -- LAB_00daf45b: (native jump target)
                pCVar20 = "TEXT_QST_048_GIVE_TEDDY_TO_GIRL"
                quest:GiveHeroYesNoQuestion(pCVar20, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_QST_048_GIVE_TEDDY_TO_GIRL")
                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                uVar7 = SUB41(pCVar20,0)
                while iVar9 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db04fe
                    iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                    uVar7 = SUB41(pCVar20,0)
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    alive = not quest:IsActiveThreadTerminating()
                    if iVar9 == 1 then
                        if not alive then
                            -- LAB_00db04fe: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00db05d5
                        end
                        -- TODO(native): uVar10 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe80);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            bVar6 = false
                            pCVar23 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY"
                            pCVar8 = quest:GetHero()
                            r5 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00db0513 end
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00db04fe
                        end
                        GivenTeddy(quest, me)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        if not alive then goto LAB_00db0513 end
                        -- TODO(native): uVar10 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe80);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            bVar6 = false
                            pCVar23 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA"
                            pCVar8 = quest:GetHero()
                            r6 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00db04fe
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db0513 end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                    end
                    -- TODO(native): goto LAB_00dafa96
                end
                ::LAB_00db0513::
                quest:PauseAllNonScriptedEntities(false)
            end
            -- LAB_00db05d5: (native jump target)
            ::LAB_00db05da::
            return
        end
        cVar5 = me:MsgIsPresentedWithItem()
        if not cVar5 then
            -- LAB_00daf720: (native jump target)
            cVar5 = me:MsgIsPresentedWithItem()
            __native_condition_1 = cVar5
            if __native_condition_1 then
                cVar5 = g_PresentedItemName ~= "OBJECT_TEDDY_BEAR_UNGIVEABLE"
                __native_condition_1 = cVar5
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05da
                if not __native_entity_state:GetStateBool("FoundTeddy") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db05da
                    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea0);
                    if bVar6 then
                    end
                    cVar5 = me:AcquireControl(4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db05da
                        cVar5 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db05da
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_ac);
                    pCVar20 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                    fVar18 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar18 then
                        CVar22 = 0x0
                        pCVar23 = 0x1
                        pCVar21 = 0x0
                        pCVar20 = 0x0
                        pcVar19 = "TEXT_QST_048_TEDDYGIRL_DONT_WANT"
                        pCVar8 = quest:GetHero()
                        r7 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), (CVar22 ~= 0))
                        bVar6 = me:IsPerformingScriptTask()
                        if bVar6 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities((CVar22 ~= 0))
                                    -- TODO(native): goto LAB_00db05d5
                                end
                                bVar6 = me:IsPerformingScriptTask()
                            until not (bVar6)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00db05d5
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00dafa96
                end
            end
        else
            if unaff_EDI ~= nil then
                cVar5 = '\x01' - (lVar11 ~= 0)
                -- TODO(native): uStack_150 = (int *)CONCAT13(cVar5,(undefined3)uStack_150);
                if cVar5 ~= 0 then goto LAB_00daf87a end
                -- TODO(native): goto LAB_00daf720
            end
            iVar9 = 28
            bVar6 = false
            if not bVar6 then return end  -- TODO(native): goto LAB_00daf720
            ::LAB_00daf87a::
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db05da
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea0);
            if bVar6 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05da
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db05da
            __native_entity_state:SetStateBool("DoneIntro", true)
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_cc);
            pCVar20 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities((iVar9 ~= 0))
            -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
            fVar18 = quest:GetHealth(nil --[[missing]])
            fVar3 = _DAT_0122dedc
            if fVar3 < fVar18 then
                CVar22 = 0x0
                pCVar23 = 0x1
                pCVar21 = 0x0
                pCVar20 = 0x0
                pcVar19 = "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY"
                pCVar8 = quest:GetHero()
                r8 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), (CVar22 ~= 0))
                bVar6 = me:IsPerformingScriptTask()
                if bVar6 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities((CVar22 ~= 0))
                            -- TODO(native): goto LAB_00db05d5
                        end
                        bVar6 = me:IsPerformingScriptTask()
                    until not (bVar6)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00db05d5
                end
            end
            GivenTeddy(quest, me)
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- LAB_00dafa96: (native jump target)
        end
        if quest:GetStateBool("SpokeAboutFindingTeddy") then
            bVar6 = quest:IsDistanceBetweenThingsUnder(me, r1, 10.0)
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05da
                ppVar12 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                r9 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r9)
                uVar10 = quest:GetHero()
                quest:AddLineToConversation(ppVar12, "TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED", me, uVar10, false)
                quest:SetMasterGameState("TeddySolution", "C")
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffe78);
                if bVar6 then
                end
                cVar5 = me:AcquireControl(4)
                while not cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db05da
                    cVar5 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05da
                r10 = quest:GetThingWithScriptName("NOVI_AffairWife")
                me:MoveToThing(r10, 0x40400000, 0x1)
                me:GetPos()
                cVar5 = quest:IsCameraPosOnScreen(nil --[[missing]])
                while cVar5 do
                    fVar25 = 20.0
                    pCVar13 = quest:GetHero()
                    bVar6 = IsDistanceBetweenThingsOver(me,pCVar13,fVar25)
                    if bVar6 then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db0582
                    me:GetPos()
                    cVar5 = quest:IsCameraPosOnScreen(fVar25)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00db0582: (native jump target)
                    -- TODO(native): goto LAB_00db05da
                end
                quest:RemoveThing(pCVar13)
            end
        end
        cVar5 = me:IsTalkedToByHero()
        if cVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db05da
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea0);
            if bVar6 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05da
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db05da
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)auStack_13c);
            pCVar20 = ""
            quest:StartMovieSequence()
            CVar24 = 0x1
            quest:PauseAllNonScriptedEntities((CVar24 ~= 0))
            if (not __native_entity_state:GetStateBool("FoundTeddy")) and (__native_entity_state:GetStateBool("HeroHitMe")) then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05a2
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                fVar18 = quest:GetHealth(nil --[[missing]])
                fVar3 = _DAT_0122dedc
                if fVar3 < fVar18 then
                    bVar6 = false
                    pCVar23 = 0x1
                    pCVar21 = 0x0
                    pCVar20 = 0x0
                    pcVar19 = "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN"
                    pCVar8 = quest:GetHero()
                    r11 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                    bVar6 = me:IsPerformingScriptTask()
                    if bVar6 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00db0590
                            bVar6 = me:IsPerformingScriptTask()
                        until not (bVar6)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db05a2
                end
                __native_entity_state:SetStateBool("DoneIntro", true)
            else
                if __native_entity_state:GetStateBool("DoneIntro") then
                    if not __native_entity_state:GetStateBool("FoundTeddy") then
                        if quest:GetStateBool("TeddyRuined") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00db0590
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                            fVar18 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar18 then
                                bVar6 = false
                                pCVar23 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TEDDYGIRL_BAD_FEELING"
                                pCVar8 = quest:GetHero()
                                r12 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db05a2 end
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db0590
                            end
                            quest:ClearThingHasInformation(nil --[[missing]])
                            goto LAB_00db0179
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                            fVar18 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar18 then
                                bVar6 = false
                                pCVar23 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA"
                                pCVar8 = quest:GetHero()
                                r13 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00db0590
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                -- TODO(native): goto LAB_00db016a
                            end
                            goto LAB_00db0179
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                            fVar18 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar18 then
                                bVar6 = false
                                pCVar23 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TEDDYGIRL_REPEAT_FOUND"
                                pCVar8 = quest:GetHero()
                                r14 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00db0590
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                -- LAB_00db016a: (native jump target)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db05a2 end
                            end
                            goto LAB_00db0179
                        end
                    end
                    ::LAB_00db05a2::
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): goto LAB_00db05d5
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00db0590: (native jump target)
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): goto LAB_00db05d5
                end
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
                fVar18 = quest:GetHealth(nil --[[missing]])
                fVar3 = _DAT_0122dedc
                if fVar3 < fVar18 then
                    bVar6 = false
                    pCVar23 = 0x1
                    pCVar21 = 0x0
                    pCVar20 = 0x0
                    pcVar19 = "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY"
                    pCVar8 = quest:GetHero()
                    r15 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), bVar6)
                    bVar6 = me:IsPerformingScriptTask()
                    if bVar6 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00db05a2
                            bVar6 = me:IsPerformingScriptTask()
                        until not (bVar6)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db0590
                end
                __native_entity_state:SetStateBool("DoneIntro", true)
            end
            ::LAB_00db0179::
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
        end
        piVar16 = (piStack_140 | 4)
        cVar5 = me:MsgIsHitByHero()
        if not cVar5 then
            piVar16 = (piVar2 | 0xc)
            cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar5 then
                piVar16 = (piVar2 | 0x1c)
                cVar5 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if not cVar5 then return end  -- TODO(native): goto LAB_00db0220
            end
            -- TODO(native): uStack_150 = (int *)((uint)uStack_150 & 0xffffff);
        else
            -- LAB_00db0220: (native jump target)
            -- TODO(native): uStack_150 = (int *)CONCAT13(1,(undefined3)uStack_150);
        end
        if (piVar16 & 0x10) ~= 0 then
            piVar16 = (piVar16 & 0xffffffef)
        end
        if (piVar16 & 8) ~= 0 then
            piVar16 = (piVar16 & 0xfffffff7)
        end
        if (piVar16 & 4) ~= 0 then
            -- TODO(native): piStack_140 = (int *)((uint)piVar16 & 0xfffffffb);
        end
        if uStack_150._3_1_ ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db05da
            r16 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r16, nil --[[missing]])
            r17 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r17, nil --[[missing]])
            __native_entity_state:SetStateBool("HeroHitMe", true)
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffe90);
            if bVar6 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db05da
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db05da
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_bc);
            pCVar20 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe94);
            fVar18 = quest:GetHealth(nil --[[missing]])
            fVar3 = _DAT_0122dedc
            if fVar3 < fVar18 then
                CVar22 = 0x0
                pCVar23 = 0x1
                pCVar21 = 0x0
                pCVar20 = 0x0
                pcVar19 = "TEXT_QST_048_TEDDYGIRL_DONT_HIT"
                pCVar8 = quest:GetHero()
                r18 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar23 ~= 0), (CVar22 ~= 0))
                bVar6 = me:IsPerformingScriptTask()
                if bVar6 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities((CVar22 ~= 0))
                            -- TODO(native): goto LAB_00db05ce
                        end
                        bVar6 = me:IsPerformingScriptTask()
                    until not (bVar6)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- LAB_00db05ce: (native jump target)
                    -- TODO(native): goto LAB_00db05d5
                end
            end
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
        end
        -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea0);
        if bVar6 then
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
    until false
end

function GivenTeddy(quest, me)
    quest:TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE")
    __native_entity_state:SetStateBool("FoundTeddy", true)
    require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
    quest:ClearThingHasInformation(me)
    quest:SetMasterGameState("TeddySolution", "B")
end

