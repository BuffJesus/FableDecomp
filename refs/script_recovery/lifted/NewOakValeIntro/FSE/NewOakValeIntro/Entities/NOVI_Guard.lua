-- Generated native draft: NOVI_Guard. Review coverage report before use.
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
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    local r1 = me:GetHomePos()
    quest:SetWanderCentrePoint(me, r1)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 6.0)
    quest:SetScriptingStateGroup(me, 4)
    quest:EntitySheatheWeapons(me, false)
end

function Main(quest, me)
    local bVar6, cVar7, fVar20, fVar24, fVar4, iVar2, iVar9, pCVar1, pCVar10, pCVar14, pCVar21, pCVar22, pCVar23, paVar17, pcVar12, ppVar13, ppVar16, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r3, r4, r5, r6, r7, r8, r9, uVar11, uVar19, uVar5, uVar8
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    cVar7 = not alive
    repeat
        if cVar7 then
            -- LAB_00dadd6c: (native jump target)
            return
        end
        iVar9 = quest:GetStateInt("BadDeedsPerformed")
        iVar2 = quest:GetStateInt("GuardsDealtWithBadDeeds")
        if iVar9 ~= iVar2 and -1 < iVar9 - iVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                iVar9 = quest:GetStateInt("BadDeedsPerformed")
                if 2 < iVar9 then
                    iVar9 = 3
                end
                -- TODO(native): fVar24 = *(float *)(&DAT_013ac844 + iVar9 * 4);
                pCVar10 = quest:GetHero()
                bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, fVar24)
                if not bVar6 then goto LAB_00dad894 end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dade3b end
                r1 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(r1, nil --[[missing]])
                uVar11 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(uVar11, nil --[[missing]])
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea8);
                if bVar6 then
                end
                cVar7 = me:AcquireControl(4)
                while not cVar7 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dade3b end
                    cVar7 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dade3b end
                -- TODO(native): fVar24 = DAT_013ac840;
                pCVar10 = quest:GetHero()
                bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, fVar24)
                if not bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dade3b end
                    pCVar23 = 0x1
                    pcVar12 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(pcVar12, nil --[[missing]])
                    ppVar13 = quest:AddNewConversation(me, false, false)
                    uVar11 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar13, uVar11)
                    uVar11 = quest:GetHero()
                    -- TODO(native): (**(code **)(iStack_140 + 0x5b8))(ppVar13,aCStack_128,0,pCVar14,uVar11);
                    pCVar10 = quest:GetHero()
                end
                fVar24 = 1.4013e-45
                quest:EntitySetCutsceneBehaviour(pCVar10, pCVar23)
                pCVar10 = quest:GetHero()
                bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, 0x0)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dade3b end
                    -- TODO(native): fVar24 = DAT_013ac840;
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, 0)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dade3b end
                quest:EntitySetCutsceneBehaviour(uVar11, nil --[[missing]])
                iVar9 = quest:GetStateInt("BadDeedsPerformed")
                iVar2 = quest:GetStateInt("GuardsDealtWithBadDeeds")
                if iVar9 == iVar2 or iVar9 - iVar2 < 0 then
                    -- LAB_00dad87e: (native jump target)
                    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_148);
                    if bVar6 then
                    end
                    goto LAB_00dad894
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dade3b end
                quest:SetStateInt("GuardsDealtWithBadDeeds", quest:GetStateInt("BadDeedsPerformed"))
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_148);
                if bVar6 then
                end
                cVar7 = me:AcquireControl(4)
                while not cVar7 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dade3b end
                    cVar7 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dade3b end
                cVar7 = quest:IsHeroControlledByPlayer()
                while not cVar7 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dade3b end
                    cVar7 = quest:IsHeroControlledByPlayer()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dade3b end
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_138);
                pCVar23 = ""
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                uVar11 = quest:GetHero()
                -- TODO(native): (**(code **)(iStack_12c + 0x76c))(uVar11);
                if quest:GetStateBool("GuardsSpokenOnce") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00dadd90
                    -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffea0);
                    fVar20 = quest:GetHealth(nil --[[missing]])
                    fVar4 = 0.0
                    if fVar4 < fVar20 then
                        bVar6 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar23 = 0x0
                        pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN"
                        pCVar14 = quest:GetHero()
                        r2 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                        bVar6 = me:IsPerformingScriptTask()
                        if bVar6 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daddb8
                                bVar6 = me:IsPerformingScriptTask()
                            until not (bVar6)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dadda5
                    end
                    if quest:GetStateBool("WhichBadDeedsPerformed_0") then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                            fVar20 = quest:GetHealth(nil --[[missing]])
                            fVar4 = 0.0
                            if fVar4 < fVar20 then
                                bVar6 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar23 = 0x0
                                pcVar12 = "TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING"
                                pCVar14 = quest:GetHero()
                                r3 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daddb8
                            end
                            goto LAB_00dad4a3
                        end
                        -- TODO(native): goto LAB_00daddb8
                    end
                    ::LAB_00dad4a3::
                    if quest:GetStateBool("WhichBadDeedsPerformed_1") then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                            fVar20 = quest:GetHealth(nil --[[missing]])
                            fVar4 = 0.0
                            if fVar4 < fVar20 then
                                bVar6 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar23 = 0x0
                                pcVar12 = "TEXT_QST_048_GUARD_CRIME_DERELICTION_OF_DUTY"
                                pCVar14 = quest:GetHero()
                                r4 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00daddb8
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00dadda5
                            end
                            goto LAB_00dad568
                        end
                        -- TODO(native): goto LAB_00dadda5
                    end
                    ::LAB_00dad568::
                    if quest:GetStateBool("WhichBadDeedsPerformed_2") then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                            fVar20 = quest:GetHealth(nil --[[missing]])
                            fVar4 = 0.0
                            if fVar4 < fVar20 then
                                bVar6 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar23 = 0x0
                                pcVar12 = "TEXT_QST_048_GUARD_CRIME_VIOLENCE"
                                pCVar14 = quest:GetHero()
                                r5 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daddb8
                            end
                            goto LAB_00dad633
                        end
                        -- TODO(native): goto LAB_00daddb8
                    end
                    ::LAB_00dad633::
                    if quest:GetStateBool("WhichBadDeedsPerformed_3") then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                            fVar20 = quest:GetHealth(nil --[[missing]])
                            fVar4 = 0.0
                            if fVar4 < fVar20 then
                                bVar6 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar23 = 0x0
                                pcVar12 = "TEXT_QST_048_GUARD_CRIME_TEDDY_TO_BULLY"
                                pCVar14 = quest:GetHero()
                                r6 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                                bVar6 = me:IsPerformingScriptTask()
                                if bVar6 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00daddb8
                                        bVar6 = me:IsPerformingScriptTask()
                                    until not (bVar6)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00dadda5
                            end
                            goto LAB_00dad6f8
                        end
                        -- TODO(native): goto LAB_00dadda5
                    end
                    ::LAB_00dad6f8::
                    uVar8 = uVar11
                    if quest:GetStateBool("WhichBadDeedsPerformed_4") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00daddb8
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        uVar8 = uVar11
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CRIME_CONCEALED_AFFAIR"
                            pCVar14 = quest:GetHero()
                            r7 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            uVar8 = uVar11
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                    uVar8 = uVar11
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daddb8
                        end
                    end
                    -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                    fVar20 = quest:GetHealth(nil --[[missing]])
                    fVar4 = 0.0
                    if fVar4 < fVar20 then
                        bVar6 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar23 = 0x0
                        pcVar12 = "TEXT_QST_048_GUARD_AFTER_READ_LIST"
                        pCVar14 = quest:GetHero()
                        r8 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                        bVar6 = me:IsPerformingScriptTask()
                        if bVar6 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                bVar6 = me:IsPerformingScriptTask()
                            until not (bVar6)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00daddb8
                    end
                    -- LAB_00dad868: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00dad87e
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffea0);
                    fVar20 = quest:GetHealth(nil --[[missing]])
                    fVar4 = 0.0
                    if fVar4 < fVar20 then
                        bVar6 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar23 = 0x0
                        pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_10"
                        pCVar14 = quest:GetHero()
                        r9 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                        bVar6 = me:IsPerformingScriptTask()
                        if bVar6 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00daddb8 end
                                bVar6 = me:IsPerformingScriptTask()
                            until not (bVar6)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dadda5
                    end
                    -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                    fVar20 = quest:GetHealth(nil --[[missing]])
                    fVar4 = 0.0
                    if fVar4 < fVar20 then
                        bVar6 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar23 = 0x0
                        pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_20"
                        pCVar14 = quest:GetHero()
                        r10 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                        bVar6 = me:IsPerformingScriptTask()
                        if bVar6 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00daddb8 end
                                bVar6 = me:IsPerformingScriptTask()
                            until not (bVar6)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dadda5
                    end
                    if quest:GetStateBool("WhichBadDeedsPerformed_0") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00daddb8 end
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING"
                            pCVar14 = quest:GetHero()
                            r11 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                    end
                    if quest:GetStateBool("WhichBadDeedsPerformed_1") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dadda5
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CRIME_DERELICTION_OF_DUTY"
                            pCVar14 = quest:GetHero()
                            r12 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00daddb8 end
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dadda5
                        end
                    end
                    if quest:GetStateBool("WhichBadDeedsPerformed_2") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00daddb8 end
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CRIME_VIOLENCE"
                            pCVar14 = quest:GetHero()
                            r13 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                    end
                    if quest:GetStateBool("WhichBadDeedsPerformed_3") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00dadda5: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_138;
                            -- TODO(native): goto LAB_00dade32
                        end
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CRIME_TEDDY_TO_BULLY"
                            pCVar14 = quest:GetHero()
                            r14 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00daddb8 end
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dadda5
                        end
                    end
                    if not quest:GetStateBool("WhichBadDeedsPerformed_4") then
                        -- LAB_00dad078: (native jump target)
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_30"
                            pCVar14 = quest:GetHero()
                            r15 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_40"
                            pCVar14 = quest:GetHero()
                            r16 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        uVar8 = uVar11
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_50"
                            pCVar14 = quest:GetHero()
                            r17 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            uVar8 = uVar11
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                    uVar8 = uVar11
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                        -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CAUGHT_YOU_60"
                            pCVar14 = quest:GetHero()
                            r18 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                        quest:SetStateBool("GuardsSpokenOnce", true)
                        -- TODO(native): goto LAB_00dad868
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): uVar15 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        fVar4 = 0.0
                        if fVar4 < fVar20 then
                            bVar6 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar23 = 0x0
                            pcVar12 = "TEXT_QST_048_GUARD_CRIME_CONCEALED_AFFAIR"
                            pCVar14 = quest:GetHero()
                            r19 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dadda5
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daddb8 end
                        end
                        -- TODO(native): goto LAB_00dad078
                    end
                    ::LAB_00daddb8::
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_138;
                else
                    -- LAB_00dadd90: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_138;
                end
                -- LAB_00dade32: (native jump target)
                ::LAB_00dade3b::
                return
            end
            return
        end
        ::LAB_00dad894::
        cVar7 = me:IsTalkedToByHero()
        if cVar7 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            r20 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(r20, nil --[[missing]])
            ppVar16 = quest:AddNewConversation(me, false, false)
            uVar11 = quest:GetHero()
            quest:AddPersonToConversation(ppVar16, uVar11)
            iVar9 = quest:GetStateInt("BadDeedsPerformed")
            ppVar13 = ppVar16
            if (iVar9 == 0) and (0 < quest:GetStateInt("GoodDeedsPerformed")) then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dade3b
                r21 = quest:GetHero()
                quest:AddLineToConversation(ppVar16, "TEXT_QST_048_GUARD_ON_TALK_GOOD", r21, nil --[[missing]])
            elseif iVar9 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dade3b
                r22 = quest:GetHero()
                quest:AddLineToConversation(ppVar16, "TEXT_QST_048_GUARD_ON_TALK_NEUTRAL", r22, nil --[[missing]])
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dade3b
                r23 = quest:GetHero()
                quest:AddLineToConversation(ppVar16, "TEXT_QST_048_GUARD_ON_TALK_BAD", r23, nil --[[missing]])
            end
            cVar7 = quest:IsConversationActive(ppVar16)
            while cVar7 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dade3b
                cVar7 = quest:IsConversationActive(0x0)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00dade3b
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_148);
            if bVar6 then
            end
        end
        uVar19 = uStack_124 | 1
        cVar7 = me:MsgIsHitByHero()
        if not cVar7 then
            uVar19 = uVar5 | 3
            cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar7 then
                uVar19 = uVar5 | 7
                cVar7 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if not cVar7 then return end  -- TODO(native): goto LAB_00dadaf4
            end
            -- TODO(native): cStack_125 = '\0';
        else
            -- LAB_00dadaf4: (native jump target)
            -- TODO(native): cStack_125 = '\x01';
        end
        if (uVar19 & 4) ~= 0 then
            uVar19 = uVar19 & 0xfffffffb
        end
        if (uVar19 & 2) ~= 0 then
            uVar19 = uVar19 & 0xfffffffd
        end
        if (uVar19 & 1) ~= 0 then
            -- TODO(native): uStack_124 = uVar19 & 0xfffffffe;
        end
        if cStack_125 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00dade3b
            r24 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r24, nil --[[missing]])
            uVar11 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(uVar11, nil --[[missing]])
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea8);
            if bVar6 then
            end
            cVar7 = me:AcquireControl(4)
            while not cVar7 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dade3b
                cVar7 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00dade3b
            -- TODO(native): auStack_e8[0] = 0;
            pCVar23 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
            fVar20 = quest:GetHealth(nil --[[missing]])
            fVar4 = 0.0
            if fVar4 < fVar20 then
                bVar6 = false
                pCVar22 = 0x1
                pCVar21 = 0x0
                pCVar23 = 0x0
                pcVar12 = "TEXT_QST_048_GUARD_ON_HIT"
                pCVar14 = quest:GetHero()
                r25 = me:Speak(pCVar14, pcVar12, pCVar23, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar6)
                bVar6 = me:IsPerformingScriptTask()
                if bVar6 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00dade2e
                        end
                        bVar6 = me:IsPerformingScriptTask()
                    until not (bVar6)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00dade2e: (native jump target)
                    -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)appuStack_f4;
                    -- TODO(native): goto LAB_00dade32
                end
            end
            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea8);
            if bVar6 then
            end
            quest:PauseAllNonScriptedEntities(false)
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar7 = not alive
    until false
end

