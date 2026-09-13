-- Generated native draft: NOVI_Bully. Review coverage report before use.
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
    __native_entity_state:SetStateInt("HitsTaken", 0)
    __native_entity_state:SetStateInt("InitialHealth", 4)
    __native_entity_state:SetStateBool("SpokenOnFirstProximity", false)
    quest:SetStateBool("SpokeAboutFindingTeddy", false)
    __native_entity_state:SetStateBool("SaidPieceAboutAttackingVictim", false)
    __native_entity_state:SetStateInt("IntimidateSpeechLoop", 10)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:SetIsPushableByHero(me, false)
end

function Main(quest, me)
    local aVar26, aVar5, bVar3, cVar4, fVar18, fVar2, fVar28, iVar8, native_arg_sequence_1, native_arg_sequence_2, pCVar10, pCVar22, pCVar24, pCVar25, pCVar29, pCVar6, paVar14, pcVar17, pcVar21, ppVar11, ppVar20, ppVar23, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, uVar12, uVar16, uVar19, uVar27, uVar7
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_13c);
    if bVar3 then
    end
    -- TODO(native): pppuVar31 = &ppuStack_13c;
    -- TODO(native): ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar6;
    cVar4 = me:AcquireControl(4)
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00dbcceb end
        cVar4 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00dbcceb end
    me:GetHomePos()
    while true do
        fVar28 = 2.0
        -- TODO(native): pCVar6 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeb8);
        -- TODO(native): bVar3 = IsDistanceFromThingToPositionOver(pCVar6,aCStack_f4,fVar28);
        bVar3 = nil --[[unresolved native result]]
        if not bVar3 then break end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00dbcceb end
        me:MoveToPosition(nil --[[missing]], 0x0, 0x0, false, true)
        bVar3 = me:IsPerformingScriptTask()
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                bVar3 = me:IsPerformingScriptTask()
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00dbcceb end
    r1 = quest:GetThingWithScriptName("NOVI_Victim")
    alive = not quest:IsActiveThreadTerminating()
    cVar4 = extraout_AL_06
    while cVar4 == 0 do
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffeb8);
        if bVar3 then
        end
        cVar4 = me:AcquireControl(4)
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00dbcce2 end
            cVar4 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then break end
        if __native_entity_state:GetStateBool("DoneIntro") then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                uVar16 = uStack_120 | 1
                cVar4 = (**(*pCVar6 + 0x6c))()
                if cVar4 == 0 then
                    -- LAB_00dbb684: (native jump target)
                    -- TODO(native): uStack_134 = uStack_134 & 0xffffff;
                else
                    uVar16 = uVar12 | 3
                    r2 = quest:GetHero()
                    -- TODO(native): IsObjectInThingsPossession is not a ForgeFSE binding
                    quest:IsObjectInThingsPossession()
                    -- TODO(native): uStack_134 = CONCAT13(1,(undefined3)uStack_134);
                    if cVar4 == 0 then return end  -- TODO(native): goto LAB_00dbb684
                end
                if (uVar16 & 2) ~= 0 then
                    uVar16 = uVar16 & 0xfffffffd
                end
                if (uVar16 & 1) ~= 0 then
                    -- TODO(native): uStack_120 = uVar16 & 0xfffffffe;
                end
                if uStack_134._3_1_ == 0 then
                    cVar4 = (**(*pCVar6 + 0x8c))()
                    if cVar4 ~= 0 then
                        if "" ~= nil then
                            -- TODO(native): lVar9 = CBasicString<char>::Compare ((char *)*puStack_130,"OBJECT_TEDDY_BEAR_UNGIVEABLE");
                            cVar4 = '\x01' - (lVar9 ~= 0)
                            -- TODO(native): uStack_134 = CONCAT13(cVar4,(undefined3)uStack_134);
                            pCVar6 = pCVar6
                            if cVar4 == 0 then goto LAB_00dbba25 end
                            -- LAB_00dbbb73: (native jump target)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbc848 end
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_90);
                            pCVar6 = ""
                            quest:StartMovieSequence()
                            uVar7 = 1
                            quest:PauseAllNonScriptedEntities((uVar7 ~= 0))
                            uVar27 = uVar7
                            quest:SetStateBool("VictimComplainsAboutLosingTeddy", true)
                            cVar4 = quest:GetStateBool("VictimComplainsAboutLosingTeddy")
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                uVar27 = uVar7
                                if not alive then goto LAB_00dbc7cd end
                                cVar4 = quest:GetStateBool("VictimComplainsAboutLosingTeddy")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                                fVar18 = quest:GetHealth(r2)
                                fVar2 = _DAT_0122dedc
                                if fVar2 < fVar18 then
                                    aVar5 = 0x0
                                    pCVar25 = 0x1
                                    pCVar24 = 0x0
                                    pCVar22 = 0x0
                                    pcVar21 = "TEXT_QST_048_BULLY_FOUND_TEDDY_TWO"
                                    pCVar6 = quest:GetHero()
                                    r3 = me:Speak(pCVar6, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                                    bVar3 = me:IsPerformingScriptTask()
                                    if bVar3 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00dbc7cd end
                                            bVar3 = me:IsPerformingScriptTask()
                                        until not (bVar3)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00dbc7ee end
                                end
                                GivenTeddy(quest, me)
                                quest:ClearThingHasInformation(pCVar6)
                                quest:PauseAllNonScriptedEntities((aVar5 ~= 0))
                                -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_90;
                                -- TODO(native): goto LAB_00dbbd7e
                            end
                            ::LAB_00dbc7ee::
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00dbccdd
                        end
                        iVar8 = 0x1d
                        bVar3 = true
                        pcVar21 = "OBJECT_TEDDY_BEAR_UNGIVEABLE"
                        pcVar17 = ""
                        repeat
                            if iVar8 == 0 then break end
                            iVar8 = iVar8 + -1
                            bVar3 = *pcVar21 == *pcVar17
                            pcVar21 = pcVar21 + 1
                            pcVar17 = pcVar17 + 1
                        until not (bVar3)
                        if bVar3 then return end  -- TODO(native): goto LAB_00dbbb73
                    end
                    ::LAB_00dbba25::
                    cVar4 = (**(*pCVar6 + 0x8c))()
                    native_arg_sequence_1 = false
                    if cVar4 == 0 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if not native_arg_sequence_1 then
                        cVar4 = CCharString__NotEqual()
                        if cVar4 == 0 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        -- LAB_00dbbd83: (native jump target)
                        goto LAB_00dbbd8c
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffeb8);
                        if bVar3 then
                        end
                        cVar4 = me:AcquireControl(4)
                        while not cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbc848 end
                            cVar4 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_68);
                            pCVar22 = ""
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): aVar5 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                            fVar18 = quest:GetHealth(r1)
                            fVar2 = _DAT_0122dedc
                            if fVar2 < fVar18 then
                                aVar26 = 0x0
                                pCVar25 = 0x1
                                pCVar24 = 0x0
                                pCVar22 = 0x0
                                pcVar21 = "TEXT_QST_048_BULLY_DONT_WANT"
                                pCVar6 = quest:GetHero()
                                r4 = me:Speak(pCVar6, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar26 ~= 0))
                                bVar3 = me:IsPerformingScriptTask()
                                if bVar3 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities((aVar26 ~= 0))
                                            goto LAB_00dbccdd
                                        end
                                        bVar3 = me:IsPerformingScriptTask()
                                    until not (bVar3)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00dbc848
                                end
                            end
                            quest:PauseAllNonScriptedEntities((fVar28 ~= 0))
                            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_68;
                            -- TODO(native): goto LAB_00dbbd7e
                        end
                    end
                    ::LAB_00dbc848::
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00dbc848
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_78);
                    pCVar22 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                    fVar18 = quest:GetHealth(nil --[[missing]])
                    fVar2 = _DAT_0122dedc
                    if fVar18 <= fVar2 then
                        -- LAB_00dbb7c8: (native jump target)
                        pCVar22 = "TEXT_QST_048_GIVE_TEDDY_TO_BULLY"
                        quest:GiveHeroYesNoQuestion(pCVar22, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                        iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar8 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbc7ac
                            iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00dbc78b end
                        if iVar8 == 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                quest:SetStateBool("VictimComplainsAboutLosingTeddy", true)
                                cVar4 = quest:GetStateBool("VictimComplainsAboutLosingTeddy")
                                while true do
                                    aVar5 = SUB41(uVar7,0)
                                    if not (cVar4) then break end
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00dbc78b end
                                    cVar4 = quest:GetStateBool("VictimComplainsAboutLosingTeddy")
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffea8);
                                    fVar18 = quest:GetHealth(nil --[[missing]])
                                    fVar2 = _DAT_0122dedc
                                    if fVar2 < fVar18 then
                                        aVar26 = 0x0
                                        pCVar25 = 0x1
                                        pCVar24 = 0x0
                                        pCVar22 = 0x0
                                        pcVar21 = "TEXT_QST_048_BULLY_FOUND_TEDDY_TWO"
                                        pCVar6 = quest:GetHero()
                                        r5 = me:Speak(pCVar6, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar26 ~= 0))
                                        bVar3 = me:IsPerformingScriptTask()
                                        if bVar3 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then goto LAB_00dbc78b end
                                                bVar3 = me:IsPerformingScriptTask()
                                            until not (bVar3)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00dbc7ac end
                                    end
                                    GivenTeddy(quest, me)
                                    quest:ClearThingHasInformation(nil --[[missing]])
                                    goto LAB_00dbb9d1
                                end
                            end
                            ::LAB_00dbc7ac::
                            quest:PauseAllNonScriptedEntities((aVar26 ~= 0))
                            goto LAB_00dbccdd
                        end
                        ::LAB_00dbb9d1::
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_78;
                        -- LAB_00dbbd7e: (native jump target)
                        -- TODO(native): goto LAB_00dbbd83
                    end
                    aVar5 = 0x0
                    pCVar25 = 0x1
                    pCVar24 = 0x0
                    pCVar22 = 0x0
                    pcVar21 = "TEXT_QST_048_BULLY_FOUND_TEDDY_ONE"
                    pCVar6 = quest:GetHero()
                    r6 = me:Speak(pCVar6, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                    bVar3 = me:IsPerformingScriptTask()
                    if bVar3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbc7ac
                            bVar3 = me:IsPerformingScriptTask()
                        until not (bVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then return end  -- TODO(native): goto LAB_00dbb7c8
                    ::LAB_00dbc78b::
                    quest:PauseAllNonScriptedEntities((aVar5 ~= 0))
                end
                goto LAB_00dbccdd
            end
            break
        end
        ::LAB_00dbbd8c::
        cVar4 = (**(*pCStack_108 + 0x6c))()
        if cVar4 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffeb8);
            if bVar3 then
            end
            cVar4 = me:AcquireControl(4)
            while not cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dbcce2 end
                cVar4 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_11c);
            pCVar22 = ""
            quest:StartMovieSequence()
            uVar27 = 1
            quest:PauseAllNonScriptedEntities((uVar27 ~= 0))
            if __native_entity_state:GetStateBool("DoneIntro") then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    if quest:GetStateBool("HeroAttackedVictim") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                        if not __native_entity_state:GetStateBool("SaidPieceAboutAttackingVictim") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbc851 end
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                            fVar18 = quest:GetHealth(nil --[[missing]])
                            fVar2 = _DAT_0122dedc
                            if fVar2 < fVar18 then
                                aVar5 = 0x0
                                pCVar25 = 0x1
                                pCVar24 = 0x0
                                pCVar22 = 0x0
                                pcVar21 = "TEXT_QST_048_BULLY_IN_COMMON"
                                pCVar10 = quest:GetHero()
                                r7 = me:Speak(pCVar10, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                                bVar3 = me:IsPerformingScriptTask()
                                if bVar3 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                                        bVar3 = me:IsPerformingScriptTask()
                                    until not (bVar3)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00dbc851 end
                            end
                            __native_entity_state:SetStateBool("SaidPieceAboutAttackingVictim", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                            if __native_entity_state:GetStateInt("HitsTaken") < 3 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00dbc851 end
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                                fVar18 = quest:GetHealth(nil --[[missing]])
                                fVar2 = _DAT_0122dedc
                                if fVar2 < fVar18 then
                                    aVar5 = 0x0
                                    pCVar25 = 0x1
                                    pCVar24 = 0x0
                                    pCVar22 = 0x0
                                    pcVar21 = "TEXT_QST_048_BULLY_NASTY_STREAK"
                                    pCVar10 = quest:GetHero()
                                    r8 = me:Speak(pCVar10, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                                    bVar3 = me:IsPerformingScriptTask()
                                    if bVar3 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                                            bVar3 = me:IsPerformingScriptTask()
                                        until not (bVar3)
                                    end
                                    -- TODO(native): goto LAB_00dbc27a
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                                fVar18 = quest:GetHealth(nil --[[missing]])
                                fVar2 = _DAT_0122dedc
                                if fVar2 < fVar18 then
                                    aVar5 = 0x0
                                    pCVar25 = 0x1
                                    pCVar24 = 0x0
                                    pCVar22 = 0x0
                                    pcVar21 = "TEXT_QST_048_BULLY_DONT_HIT_ME"
                                    pCVar10 = quest:GetHero()
                                    r9 = me:Speak(pCVar10, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                                    bVar3 = me:IsPerformingScriptTask()
                                    if bVar3 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00dbc851 end
                                            bVar3 = me:IsPerformingScriptTask()
                                        until not (bVar3)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                                end
                            end
                        end
                        goto LAB_00dbc289
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        fVar2 = _DAT_0122dedc
                        if fVar2 < fVar18 then
                            aVar5 = 0x0
                            pCVar25 = 0x1
                            pCVar24 = 0x0
                            pCVar22 = 0x0
                            pcVar21 = "TEXT_QST_048_BULLY_BADGERING"
                            pCVar10 = quest:GetHero()
                            r10 = me:Speak(pCVar10, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                            bVar3 = me:IsPerformingScriptTask()
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
                                    bVar3 = me:IsPerformingScriptTask()
                                until not (bVar3)
                            end
                            -- LAB_00dbc27a: (native jump target)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbc851 end
                        end
                        goto LAB_00dbc289
                    end
                end
                ::LAB_00dbc851::
                quest:PauseAllNonScriptedEntities((aVar5 ~= 0))
                break
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00dbc1c3: (native jump target)
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                break
            end
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeac);
            fVar18 = quest:GetHealth(nil --[[missing]])
            fVar2 = _DAT_0122dedc
            if fVar2 < fVar18 then
                aVar5 = 0x0
                pCVar25 = 0x1
                pCVar24 = 0x0
                pCVar22 = 0x0
                pcVar21 = "TEXT_QST_048_BULLY_GET_LOST"
                pCVar10 = quest:GetHero()
                r11 = me:Speak(pCVar10, pcVar21, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar5 ~= 0))
                bVar3 = me:IsPerformingScriptTask()
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dbc851
                        bVar3 = me:IsPerformingScriptTask()
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dbc1c3
            end
            __native_entity_state:SetStateBool("DoneIntro", true)
            ::LAB_00dbc289::
            quest:PauseAllNonScriptedEntities((aVar5 ~= 0))
        end
        uVar16 = uStack_120 | 4
        cVar4 = (**(*pCVar6 + 0x54))()
        if cVar4 == 0 then
            uVar16 = uVar12 | 0xc
            cVar4 = (**(*pCVar6 + 0xa8))()
            if cVar4 ~= 0 then
                uVar16 = uVar12 | 0x1c
                cVar4 = (**(*pCVar6 + 0xa4))()
                if cVar4 == 0 then return end  -- TODO(native): goto LAB_00dbc338
            end
            -- TODO(native): uStack_134 = uStack_134 & 0xffffff;
        else
            -- LAB_00dbc338: (native jump target)
            -- TODO(native): uStack_134 = CONCAT13(1,(undefined3)uStack_134);
        end
        if (uVar16 & 0x10) ~= 0 then
            uVar16 = uVar16 & 0xffffffef
        end
        if (uVar16 & 8) ~= 0 then
            uVar16 = uVar16 & 0xfffffff7
        end
        if (uVar16 & 4) ~= 0 then
            -- TODO(native): uStack_120 = uVar16 & 0xfffffffb;
        end
        if uStack_134._3_1_ ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            if *(__native_entity_state:GetStateInt("self_0x14") + 100) == -999 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                uVar19 = quest:AddQuestInfoBar(__native_entity_state:GetStateInt("InitialHealth"), 0, 0xff, 0x0, "HUD_QUEST_ICON_GRANDSON", "", 0xff)
                -- TODO(native): *(int *)(*(int *)(param_1 + 0x14) + 100) = (int)uVar19;
            end
            r12 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r12, nil --[[missing]])
            r13 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r13, nil --[[missing]])
            iVar8 = __native_entity_state:GetStateInt("HitsTaken")
            __native_entity_state:SetStateInt("HitsTaken", iVar8 + 1)
            if __native_entity_state:GetStateInt("InitialHealth") <= iVar8 + 1 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    quest:SetStateBool("BullySubdued", true)
                    quest:RemoveQuestInfoElement(0)
                    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffeb4);
                    if bVar3 then
                    end
                    cVar4 = me:AcquireControl(4)
                    -- TODO(native): goto joined_r0x00dbc8b9;
                end
                break
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            ppVar11 = quest:AddNewConversation(nil --[[missing]], 0xff, false)
            quest:AddPersonToConversation(0, nil --[[missing]])
            ppVar20 = ppVar11
            quest:AddLineToConversation(ppVar11, "TEXT_QST_048_BULLY_SCRMSG_GET_OFF", nil --[[missing]], nil --[[missing]], false)
            quest:AddLineToConversation(ppVar11, "TEXT_QST_048_VICTIM_REVENGE", nil --[[missing]], nil --[[missing]], false)
            -- TODO(native): iStack_94 = *(int *)(param_1 + 0x1c) - *(int *)(param_1 + 0x20);
            quest:UpdateQuestInfoBar(*(__native_entity_state:GetStateInt("self_0x14") + 100), iStack_94, 0xbf800000, 0xbf800000)
        end
        quest:EntitySetFacingAngleTowardsThing(nil --[[missing]], nil --[[missing]])
        iVar8 = quest:GetTimer(0xff)
        native_arg_sequence_2 = false
        if iVar8 == 0 then
            native_arg_sequence_2 = true
        else
            native_arg_sequence_2 = false
        end
        if native_arg_sequence_2 then
            if not __native_entity_state:GetStateBool("SpokenOnFirstProximity") then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if not native_arg_sequence_2 then
                iVar8 = rand()
                if iVar8 % DAT_013ac860 == 0 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
            end
        end
        if native_arg_sequence_2 then
            -- TODO(native): fVar28 = DAT_013ac85c;
            pCVar10 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar10, nil --[[missing]], nil --[[missing]])
            if (bVar3) and (__native_entity_state:GetStateInt("HitsTaken") == 0) then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                __native_entity_state:SetStateBool("SpokenOnFirstProximity", true)
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                quest:SetStateBool("VictimShake", true)
                uVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                quest:AddPersonToConversation(uVar7, nil --[[missing]])
                -- TODO(native): CCharString__Format_vsnprintf (&uStack_120,"TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d", *(undefined4 *)(param_1 + 0x28));
                quest:AddLineToConversation(uVar7, pCVar6, nil --[[missing]], nil --[[missing]], false)
                iVar8 = __native_entity_state:GetStateInt("IntimidateSpeechLoop")
                __native_entity_state:SetStateInt("IntimidateSpeechLoop", iVar8 + 10)
                if 0x28 < iVar8 + 10 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        __native_entity_state:SetStateInt("IntimidateSpeechLoop", 10)
                        goto LAB_00dbc6ba
                    end
                    -- LAB_00dbccd9: (native jump target)
                    -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_104;
                    goto LAB_00dbccdd
                end
                ::LAB_00dbc6ba::
                uVar12 = rand()
                uVar12 = uVar12 & 0x80000001
                bVar3 = uVar12 == 0
                if uVar12 < 0 then
                    bVar3 = (uVar12 - 1 | 0xfffffffe) == 0xffffffff
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00dbccd9
                    me:PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST", false, false, false, true, DAT_01375748, false)
                    -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_7c;
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00dbccd9
                    me:PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT", false, false, false, true, DAT_01375748, false)
                    -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_138;
                end
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar4 = extraout_AL_57
    end
    goto LAB_00dbcce2
    -- TODO(native): joined_r0x00dbc8b9:
    if cVar4 ~= 0 then goto LAB_00dbc8eb end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00dbcce2 end
    cVar4 = me:AcquireControl(4)
    -- TODO(native): goto joined_r0x00dbc8b9;
    ::LAB_00dbc8eb::
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_11c);
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_11c);
        if bVar3 then
        end
        r14 = quest:GetHero()
        cVar4 = me:AcquireControl(4)
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00dbcc33 end
            r15 = quest:GetHero()
            cVar4 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aaStack_e4);
            -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aaStack_e4);
            if bVar3 then
            end
            cVar4 = me:AcquireControl(4)
            while not cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00dbcc2a end
                cVar4 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                -- TODO(native): StdMap_Construct_API();
                -- TODO(native): pCVar13 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_f4,aCStack_138);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar13, (CScriptGameResourceObjectScriptedThingBase *)pCVar29);
                -- TODO(native): pCVar13 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_f4,aCStack_138);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar13, (CScriptGameResourceObjectScriptedThingBase *)paVar14);
                -- TODO(native): pCVar13 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_f4,aCStack_138);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar13,pCVar30);
                -- TODO(native): Std_Deque_Construct();
                if not quest:GetStateBool("HeroAttackedVictim") then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pcVar21 = "TEXT_QST_048_VICTIM_THANKS"
                        -- TODO(native): goto LAB_00dbcaf5
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pcVar21 = "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT"
                        -- LAB_00dbcaf5: (native jump target)
                        -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_100,aCStack_138);
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_68);
                        pCVar22 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        quest:FixMovieSequenceCamera(nil --[[missing]])
                        ppVar23 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func(0,aCStack_110,0);
                        if not quest:GetStateBool("GivenHeroTeddy") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                -- LAB_00dbcc00: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00dbcc18
                            end
                            ppVar23 = 0x0
                            -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                            quest:ClearThingHasInformation(r15)
                            quest:SetStateBool("GivenHeroTeddy", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbcc00
                            -- TODO(native): ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0xdbcc66;
                            -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                        end
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): LTextTreeWalkThrough__Dtor();
                        -- TODO(native): StdMap_Destroy_API();
                        quest:SetStateBool("BullyRanOff", true)
                        require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
                        quest:RemoveThing(r14, false, true)
                        goto LAB_00dbcce2
                    end
                end
                ::LAB_00dbcc18::
                -- TODO(native): LTextTreeWalkThrough__Dtor();
                -- TODO(native): StdMap_Destroy_API();
            end
            ::LAB_00dbcc2a::
        end
        ::LAB_00dbcc33::
    end
    goto LAB_00dbcce2
    ::LAB_00dbc7cd::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    ::LAB_00dbccdd::
    ::LAB_00dbcce2::
    ::LAB_00dbcceb::
end

function GivenTeddy(quest, me)
    quest:GiveHeroGold(1)
    quest:TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE")
    quest:SetStateBool("SpokeAboutFindingTeddy", true)
    quest:SetStateBool("TeddyRuined", true)
    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 3)
end

