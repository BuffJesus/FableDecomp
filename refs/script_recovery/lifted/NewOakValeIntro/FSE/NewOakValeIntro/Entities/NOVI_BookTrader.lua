-- Generated native draft: NOVI_BookTrader. Review coverage report before use.
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
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

function Main(quest, me)
    local __native_condition_1, bVar4, cVar5, fVar18, fVar23, fVar3, iVar11, native_arg_book_ally, native_arg_book_listener, pCVar1, pCVar13, pCVar20, pCVar21, pCVar22, pCVar24, pCVar7, pCVar8, pcVar19, piVar25, ppVar10, r1, r2, r3, r4, r5, r6, r7, uVar12, uVar16, uVar17, uVar6, uVar9
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
        uVar16 = 0
        while true do
            if cVar5 then
                return
            end
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_124);
            if bVar4 then
            end
            piVar25 = 0x3
            cVar5 = me:AcquireControl(3)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                cVar5 = me:AcquireControl(3)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            fVar23 = 0.1
            pCVar7 = me:GetHomePos()
            bVar4 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar7, fVar23))
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    me:GetHomePos()
                    while true do
                        fVar23 = 2.0
                        -- TODO(native): pCVar8 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffed0);
                        -- TODO(native): bVar4 = IsDistanceFromThingToPositionOver(pCVar8,aCStack_9c,fVar23);
                        bVar4 = nil --[[unresolved native result]]
                        if not bVar4 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db4f5a end
                        me:MoveToPosition(pCVar7, 0x0, 0x0, false, true)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db4f5a end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        uVar9 = quest:GetThingWithScriptName("NOVI_Theresa")
                        quest:EntitySetFacingAngleTowardsThing(me, uVar9)
                        goto LAB_00db4234
                    end
                end
                break
            end
            ::LAB_00db4234::
            uVar17 = uVar16 | 1
            cVar5 = me:MsgIsHitByHero()
            if not cVar5 then
                uVar17 = uVar16 | 3
                cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar5 then
                    uVar17 = uVar16 | 7
                    cVar5 = me:MsgIsHitByHeroSpecialAbility(14)
                    bVar4 = not cVar5
                else
                    bVar4 = false
                end
            else
                bVar4 = true
            end
            if (uVar17 & 4) ~= 0 then
                uVar17 = uVar17 & 0xfffffffb
            end
            if (uVar17 & 2) ~= 0 then
                uVar17 = uVar17 & 0xfffffffd
            end
            if (uVar17 & 1) ~= 0 then
                -- TODO(native): uStack_120 = uVar17 & 0xfffffffe;
            end
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    native_arg_book_ally = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, native_arg_book_ally)
                    uVar9 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(uVar9, me)
                    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffec0);
                    if bVar4 then
                    end
                    cVar5 = me:AcquireControl(4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db4f5a end
                        cVar5 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pCVar20 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                        fVar18 = quest:GetHealth(me)
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TRADER_ON_HIT"
                            pCVar8 = quest:GetHero()
                            r1 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db4f5a
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00db4f51: (native jump target)
                                quest:EndMovieSequence()
                                break
                            end
                        end
                        uVar9 = quest:GetThingWithScriptName("NOVI_Theresa")
                        quest:EntitySetFacingAngleTowardsThing(me, uVar9)
                        quest:PauseAllNonScriptedEntities(false)
                        -- LAB_00db4ce1: (native jump target)
                        quest:EndMovieSequence()
                        goto LAB_00db4ce6
                    end
                end
                break
            end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffed0);
                    if bVar4 then
                    end
                    cVar5 = me:AcquireControl(4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db4f5a end
                        cVar5 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pCVar20 = ""
                        quest:StartMovieSequence()
                        uVar9 = 1
                        quest:PauseAllNonScriptedEntities(true)
                        uVar6 = uVar9
                        if quest:GetStateBool("GivenSweets") then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                                fVar18 = quest:GetHealth(me)
                                fVar3 = _DAT_0122dedc
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_TRADER_INTRO_10"
                                    pCVar8 = quest:GetHero()
                                    r2 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(false)
                                                quest:EndMovieSequence()
                                                goto LAB_00db4f5a
                                            end
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db4f5a
                                    end
                                end
                                goto LAB_00db4c85
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db4f5a
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00db4f39: (native jump target)
                            -- LAB_00db4f45: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            -- LAB_00db4f4d: (native jump target)
                            quest:EndMovieSequence()
                            goto LAB_00db4f5a
                        end
                        if not __native_entity_state:GetStateBool("DoneIntro") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db4f5a
                            end
                            -- TODO(native): uVar12 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                            fVar18 = quest:GetHealth(me)
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar18 then
                                bVar4 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TRADER_INTRO"
                                pCVar8 = quest:GetHero()
                                r3 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                bVar4 = me:IsPerformingScriptTask()
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:EndMovieSequence()
                                            goto LAB_00db4f5a
                                        end
                                        bVar4 = me:IsPerformingScriptTask()
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:EndMovieSequence()
                                    goto LAB_00db4f5a
                                end
                            end
                            __native_entity_state:SetStateBool("DoneIntro", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db4f5a
                            end
                            -- TODO(native): uVar12 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                            fVar18 = quest:GetHealth(me)
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar18 then
                                bVar4 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TRADER_STILL_GOT"
                                pCVar8 = quest:GetHero()
                                r4 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                bVar4 = me:IsPerformingScriptTask()
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:EndMovieSequence()
                                            goto LAB_00db4f5a
                                        end
                                        bVar4 = me:IsPerformingScriptTask()
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:EndMovieSequence()
                                    goto LAB_00db4f5a
                                end
                            end
                        end
                        pCVar20 = "TEXT_QST_048_TRADER_BUY_SWEETS"
                        quest:GiveHeroYesNoQuestion(pCVar20, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar11 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while true do
                            uVar6 = uVar9
                            if not (iVar11 < 0) then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db4f5a
                            end
                            iVar11 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db4f5a
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if iVar11 == 1 then
                            if alive then
                                iVar11 = quest:GetHeroGold()
                                if iVar11 < 3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db4f5a
                                    end
                                    -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                                    fVar18 = quest:GetHealth(me)
                                    fVar3 = _DAT_0122dedc
                                    if fVar3 < fVar18 then
                                        bVar4 = false
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_TRADER_NOT_ENOUGH_CASH"
                                        pCVar8 = quest:GetHero()
                                        r5 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                        bVar4 = me:IsPerformingScriptTask()
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    quest:EndMovieSequence()
                                                    goto LAB_00db4f5a
                                                end
                                                bVar4 = me:IsPerformingScriptTask()
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            -- LAB_00db4bc5: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:EndMovieSequence()
                                            goto LAB_00db4f5a
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db4f5a
                                    end
                                    -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                                    fVar18 = quest:GetHealth(me)
                                    fVar3 = _DAT_0122dedc
                                    if fVar3 < fVar18 then
                                        bVar4 = false
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_TRADER_GIVES_SWEETS"
                                        pCVar8 = quest:GetHero()
                                        r6 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                        bVar4 = me:IsPerformingScriptTask()
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    quest:EndMovieSequence()
                                                    goto LAB_00db4f5a
                                                end
                                                bVar4 = me:IsPerformingScriptTask()
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:EndMovieSequence()
                                            goto LAB_00db4f5a
                                        end
                                    end
                                    pCVar20 = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
                                    quest:GiveHeroObject(pCVar20, -1)
                                    quest:GiveHeroGold(-3)
                                    ppVar10 = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(ppVar10, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04", "", "")
                                    quest:SetStateBool("GivenSweets", true)
                                    quest:ClearThingHasInformation(me)
                                end
                                goto LAB_00db4c85
                            end
                            -- LAB_00db46da: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db4f5a
                        end
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db4f5a
                        end
                        -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec4);
                        fVar18 = quest:GetHealth(me)
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TRADER_BUY_LATER"
                            pCVar8 = quest:GetHero()
                            r7 = me:Speak(pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db4f5a
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then goto LAB_00db4c85 end
                            -- LAB_00db4f3f: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db4f5a
                        end
                        ::LAB_00db4c85::
                        uVar9 = quest:GetThingWithScriptName("NOVI_Theresa")
                        quest:EntitySetFacingAngleTowardsThing(me, uVar9)
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db4ce6
                    end
                end
                break
            end
            ::LAB_00db4ce6::
            iVar11 = quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer"))
            __native_condition_1 = iVar11 == 0
            if __native_condition_1 then
                iVar11 = quest:RetailRandModulo(200)
                __native_condition_1 = iVar11 == 0
            end
            if __native_condition_1 then
                fVar23 = 20.0
                pCVar8 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar23)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
                        uVar9 = quest:AddNewConversation(me, false, false)
                        uVar12 = quest:GetHero()
                        quest:AddPersonToConversation(uVar9, uVar12)
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffeb4);
                        if bVar4 then
                        end
                        cVar5 = me:AcquireControl(4)
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db4f5a end
                            cVar5 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            me:PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, DAT_01375748, false)
                            native_arg_book_listener = quest:GetHero()
                            quest:AddLineToConversation(uVar9, "TEXT_QST_048_TRADER_ROLL_UP", me, native_arg_book_listener, false)
                            goto LAB_00db4e64
                        end
                    end
                    break
                end
            end
            ::LAB_00db4e64::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            cVar5 = not alive
        end
        ::LAB_00db4f5a::
    end
end

