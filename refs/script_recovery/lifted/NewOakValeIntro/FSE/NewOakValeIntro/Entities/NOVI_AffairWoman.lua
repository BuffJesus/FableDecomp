-- Generated native draft: NOVI_AffairWoman. Review coverage report before use.
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
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

function Main(quest, me)
    local __native_condition_1, __native_condition_2, bVar5, cVar6, fVar14, fVar21, fVar4, pCVar1, pCVar16, pCVar17, pCVar18, pCVar7, pCVar8, pcVar15, ppVar20, puVar10, r1, r2, r3, r4, r5, r6, uVar12, uVar13, uVar19
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_ac);
    if bVar5 then
    end
    -- TODO(native): pppuVar23 = &ppuStack_ac;
    -- TODO(native): ppVar22 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar1;
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db2986 end
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db2986 end
    r1 = quest:GetThingWithScriptName("NOVI_AffairWife")
    r2 = quest:GetThingWithScriptName("NOVI_AffairMan")
    alive = not quest:IsActiveThreadTerminating()
    uVar12 = 0
    cVar6 = extraout_AL_02
    while cVar6 == 0 do
        fVar21 = 0.1
        pCVar7 = me:GetHomePos()
        bVar5 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar7, fVar21))
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            me:GetHomePos()
            while true do
                fVar21 = 2.0
                -- TODO(native): pCVar8 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff48);
                -- TODO(native): bVar5 = IsDistanceFromThingToPositionOver(pCVar8,aCStack_40,fVar21);
                bVar5 = nil --[[unresolved native result]]
                if not bVar5 then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db2974 end
                me:MoveToPosition(pCVar7, 0x0, 0x0, false, true)
                bVar5 = me:IsPerformingScriptTask()
                if bVar5 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db2974 end
                        bVar5 = me:IsPerformingScriptTask()
                    until not (bVar5)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db2974 end
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
        end
        uVar13 = uVar12 | 1
        cVar6 = me:MsgIsHitByHero()
        if not cVar6 then
            uVar13 = uVar12 | 3
            cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar6 then
                uVar13 = uVar12 | 7
                cVar6 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if not cVar6 then return end  -- TODO(native): goto LAB_00db2203
            end
            -- TODO(native): cStack_a5 = '\0';
        else
            -- LAB_00db2203: (native jump target)
            -- TODO(native): cStack_a5 = '\x01';
        end
        if (uVar13 & 4) ~= 0 then
            uVar13 = uVar13 & 0xfffffffb
        end
        if (uVar13 & 2) ~= 0 then
            uVar13 = uVar13 & 0xfffffffd
        end
        if (uVar13 & 1) ~= 0 then
            -- TODO(native): uStack_88 = uVar13 & 0xfffffffe;
        end
        if cStack_a5 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            quest:SetStateBool("TalkingToWoman", true)
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_34);
            pCVar16 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff3c);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while true do
                uVar19 = SUB41(me,0)
                if not (not cVar6) then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db26f8 end
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00db2715: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                break
            end
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff30);
            fVar14 = quest:GetHealth(r2)
            fVar4 = _DAT_0122dedc
            if fVar4 < fVar14 then
                bVar5 = false
                pCVar18 = 0x1
                pCVar17 = 0x0
                pCVar16 = 0x0
                pcVar15 = "TEXT_QST_048_AFFAIRWOMAN_ON_HIT"
                pCVar8 = quest:GetHero()
                r3 = me:Speak(pCVar8, pcVar15, pCVar16, (pCVar17 ~= 0), (pCVar18 ~= 0), bVar5)
                bVar5 = me:IsPerformingScriptTask()
                if bVar5 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db26f8 end
                        bVar5 = me:IsPerformingScriptTask()
                    until not (bVar5)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db2715
            end
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            quest:SetStateBool("TalkingToWoman", false)
            quest:PauseAllNonScriptedEntities((fVar21 ~= 0))
        end
        cVar6 = me:IsTalkedToByHero()
        if cVar6 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            quest:SetStateBool("TalkingToWoman", true)
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_68);
            pCVar8 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            r4 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(r4, r1)
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff30);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while true do
                uVar19 = SUB41(me,0)
                if not (not cVar6) then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db2732 end
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00db274c: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                break
            end
            -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff24);
            fVar14 = quest:GetHealth(nil --[[missing]])
            fVar4 = _DAT_0122dedc
            if fVar4 < fVar14 then
                bVar5 = false
                pCVar18 = 0x1
                pCVar17 = 0x0
                pCVar16 = 0x0
                pcVar15 = "TEXT_QST_048_AFFAIRWOMAN_BUSY"
                pCVar8 = quest:GetHero()
                r5 = me:Speak(pCVar8, pcVar15, pCVar16, (pCVar17 ~= 0), (pCVar18 ~= 0), bVar5)
                bVar5 = me:IsPerformingScriptTask()
                if bVar5 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db2732 end
                        bVar5 = me:IsPerformingScriptTask()
                    until not (bVar5)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db274c
            end
            quest:EntitySetFacingAngleTowardsThing(me, nil --[[missing]], false)
            quest:SetStateBool("TalkingToWoman", false)
            quest:PauseAllNonScriptedEntities(false)
        end
        __native_condition_1 = quest:GetStateBool("ReceiveKiss")
        if __native_condition_1 then
            bVar5 = me:IsPerformingScriptTask()
            __native_condition_1 = not bVar5
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            me:PlayAnimation("RECEIVE_KISS", false, true, false, true, DAT_01375748, false)
            quest:SetStateBool("ReceiveKiss", false)
        end
        __native_condition_2 = quest:GetStateBool("ReceiveHug")
        if __native_condition_2 then
            bVar5 = me:IsPerformingScriptTask()
            __native_condition_2 = not bVar5
        end
        if __native_condition_2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            me:PlayAnimation("RECEIVE_HUG", false, true, false, true, DAT_01375748, false)
            quest:SetStateBool("ReceiveHug", false)
        end
        bVar5 = quest:IsDistanceBetweenThingsUnder(me, nil --[[missing]], apStack_98)
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                r6 = quest:GetThingWithScriptName("AffairWomanRunOffPoint")
                quest:EntitySetAsUseMovementInActions(r6, false)
                quest:SetIsPushableByHero(nil --[[missing]], nil --[[missing]])
                if piStack_7c == nil then
                else
                    puVar10 = (**(*piStack_7c + 0x18))()
                end
                -- TODO(native): uStack_4c = *puVar10;
                -- TODO(native): uStack_48 = puVar10[1];
                fVar21 = 2.0
                -- TODO(native): uStack_44 = puVar10[2];
                -- TODO(native): pCVar8 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff48);
                -- TODO(native): bVar5 = IsDistanceFromThingToPositionOver(pCVar8,(C3DVector *)&uStack_4c,fVar21);
                bVar5 = nil --[[unresolved native result]]
                -- TODO(native): goto joined_r0x00db282e;
            end
            break
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar6 = extraout_AL_20
    end
    goto LAB_00db2974
    ::LAB_00db26f8::
    quest:PauseAllNonScriptedEntities((fVar21 ~= 0))
    goto LAB_00db2974
    ::LAB_00db2732::
    quest:PauseAllNonScriptedEntities(false)
    goto LAB_00db2974
    -- TODO(native): joined_r0x00db282e:
    if not bVar5 then goto LAB_00db28de end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db296b end
    me:MoveToPosition(nil --[[missing]], 0x0, 0x1, false, true)
    bVar5 = me:IsPerformingScriptTask()
    if bVar5 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db296b end
            bVar5 = me:IsPerformingScriptTask()
        until not (bVar5)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db296b end
    fVar21 = 2.0
    -- TODO(native): pCVar8 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff48);
    -- TODO(native): bVar5 = IsDistanceFromThingToPositionOver(pCVar8,(C3DVector *)&uStack_4c,fVar21);
    bVar5 = nil --[[unresolved native result]]
    -- TODO(native): goto joined_r0x00db282e;
    ::LAB_00db28de::
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff48);
        if bVar5 then
        end
        me:GetPos()
        cVar6 = quest:IsCameraPosOnScreen(fVar21)
        while cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db296b end
            me:GetPos()
            cVar6 = quest:IsCameraPosOnScreen(nil --[[missing]])
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:RemoveThing(nil --[[missing]])
        end
    end
    ::LAB_00db296b::
    ::LAB_00db2974::
    ::LAB_00db2986::
end

