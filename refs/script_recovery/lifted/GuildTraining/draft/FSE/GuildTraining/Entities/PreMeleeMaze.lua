-- Generated native draft: PreMeleeMaze. Review coverage report before use.
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
    local __native_condition_1, bVar5, cVar6, fVar12, fVar4, pCVar1, pCVar14, pCVar15, pCVar16, pCVar18, pCVar8, pcVar13, ppVar21, puVar10, puVar11, puVar22, r1, r2, r3, uVar23
    local alive = true
    -- TODO(native): local_70[0] = (int *)0x0;
    quest:EntitySetAsKillable(me, false)
    uVar23 = 1
    me:SetFriendsWithEverythingFlag(nil --[[missing]])
    r1 = quest:GetThingWithScriptName("PreMeleeMazeTargetMarker")
    -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_48);
    if bVar5 then
    end
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            if (local_70[0] ~= nil) and (*local_70[0] = *local_70[0] + -1, *local_70[0] == 0) then
            end
            -- TODO(native): local_70[0] = (int *)0x0;
            return
        end
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    cVar6 = not alive
    while true do
        if cVar6 then
            if (local_70[0] ~= nil) and (*local_70[0] = *local_70[0] + -1, *local_70[0] == 0) then
            end
            -- TODO(native): local_70[0] = (int *)0x0;
            return
        end
        if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d444a1 end
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_54);
            if bVar5 then
            end
            cVar6 = quest:GetMasterGameState("GuildWarningOccuring")
            while cVar6 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d444a1 end
                cVar6 = quest:GetMasterGameState("GuildWarningOccuring")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d444a1 end
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_54);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d444a1 end
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d444a1 end
        end
        bVar5 = IsDistanceBetweenThingsOver(me,&stack0xffffff88,4.0)
        __native_condition_1 = bVar5
        if __native_condition_1 then
            bVar5 = me:IsPerformingScriptTask()
            __native_condition_1 = not bVar5
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d444a1 end
            if piStack_74 == nil then
            else
                pCVar8 = (**(*piStack_74 + 0x18))()
            end
            me:MoveToPosition(nil --[[missing]], pCVar8, 0x40400000, false, false)
        end
        cVar6 = me:IsTalkedToByHero()
        if cVar6 then break end
        -- LAB_00d441bb: (native jump target)
        puVar10 = (puVar22 | 1)
        puVar11 = puVar10
        cVar6 = me:MsgIsHitByHero()
        if not cVar6 then
            puVar10 = (puVar22 | 3)
            puVar11 = puVar10
            cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar6 then
                puVar10 = (puVar22 | 7)
                puVar11 = puVar10
                cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not cVar6 then return end  -- TODO(native): goto LAB_00d44243
            end
            -- TODO(native): ppVar21 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) ((uint)ppVar21 & 0xffffff);
        else
            -- LAB_00d44243: (native jump target)
            -- TODO(native): ppVar21 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)CONCAT13(1,(int3)ppVar21);
        end
        if (puVar10 & 4) ~= 0 then
            puVar10 = (puVar10 & 0xfffffffb)
            puVar11 = puVar10
        end
        if (puVar10 & 2) ~= 0 then
            puVar10 = (puVar10 & 0xfffffffd)
            puVar11 = puVar10
        end
        if (puVar10 & 1) ~= 0 then
            puVar11 = (puVar10 & 0xfffffffe)
        end
        if (ppVar21 >> 0x18) ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d444a1 end
            me:ClearCommands()
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_34);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_60);
            fVar12 = quest:GetHealth(r1)
            fVar4 = _DAT_0122dedc
            if fVar4 < fVar12 then
                bVar5 = false
                pCVar16 = 0x1
                pCVar15 = 0x0
                pCVar14 = 0x0
                pcVar13 = "TEXT_QST_028_MAZE_HIT"
                pCVar8 = quest:GetHero()
                r2 = me:Speak(pCVar8, pcVar13, pCVar14, (pCVar15 ~= 0), (pCVar16 ~= 0), bVar5)
                bVar5 = me:IsPerformingScriptTask()
                if bVar5 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00d44494
                        end
                        bVar5 = me:IsPerformingScriptTask()
                    until not (bVar5)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d44494: (native jump target)
                    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)aCStack_34;
                    goto LAB_00d44498
                end
            end
            me:SetFriendsWithEverythingFlag(1)
            quest:PauseAllNonScriptedEntities(false)
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        puVar22 = puVar11
        cVar6 = extraout_AL_14
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00d444a1 end
    me:ClearCommands()
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_60);
    fVar12 = quest:GetHealth(nil --[[missing]])
    fVar4 = _DAT_0122dedc
    if fVar12 <= fVar4 then
        -- LAB_00d441a3: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        -- TODO(native): goto LAB_00d441bb
    end
    bVar5 = false
    pCVar16 = 0x1
    pCVar15 = 0x0
    pCVar14 = 0x0
    pcVar13 = "TEXT_QST_028_MAZE_LEAVE_ME"
    pCVar8 = quest:GetHero()
    r3 = me:Speak(pCVar8, pcVar13, pCVar14, (pCVar15 ~= 0), (pCVar16 ~= 0), bVar5)
    bVar5 = me:IsPerformingScriptTask()
    if bVar5 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)&ppuStack_44;
                goto LAB_00d44498
            end
            bVar5 = me:IsPerformingScriptTask()
        until not (bVar5)
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then return end  -- TODO(native): goto LAB_00d441a3
    quest:PauseAllNonScriptedEntities(false)
    -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)&ppuStack_44;
    ::LAB_00d44498::
    ::LAB_00d444a1::
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

