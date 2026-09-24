-- Generated native draft: MagicBarrier. Review coverage report before use.
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
    local bVar10, bVar4, cVar5, fVar8, fVar9, f_stk_4c, f_stk_54, f_stk_58, f_stk_60, native_arg_sequence_1, native_arg_sequence_2, pCVar6, pCVar7, piVar1, r1, r2, r3, r4, r5, r6, uVar2
    local alive = true
    r1 = quest:GetThingWithScriptName("BarrierFX")
    if not (r1 ~= nil and not r1:IsNull()) then
        pCVar6 = {x = 0, y = 0, z = 0}
    else
        fVar8 = r1:GetAngleXY()
        f_stk_60 = fVar8
        if not (r1 ~= nil and not r1:IsNull()) then
            pCVar6 = {x = 0, y = 0, z = 0}
        else
            pCVar6 = r1:GetPos()
        end
    end
    r2 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", pCVar6, f_stk_60, false)
    if not (r1 ~= nil and not r1:IsNull()) then
        pCVar6 = {x = 0, y = 0, z = 0}
    else
        fVar8 = r1:GetAngleXY()
        f_stk_58 = fVar8
        if not (r1 ~= nil and not r1:IsNull()) then
            pCVar6 = {x = 0, y = 0, z = 0}
        else
            pCVar6 = r1:GetPos()
        end
    end
    r3 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", pCVar6, f_stk_58, false)
    cVar5 = quest:GetStateBool("TradersShouldBeScared")
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e046bf end
        -- TODO(native): cVar5 = (**(r2._0_4_ + 0x12c))()
        cVar5 = nil --[[unresolved native value]]
        native_arg_sequence_1 = false
        if not cVar5 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            -- TODO(native): cVar5 = (**(r3._0_4_ + 0x12c))()
            cVar5 = nil --[[unresolved native value]]
            if not cVar5 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e046bf end
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                fVar8 = r1:GetAngleXY()
                f_stk_58 = fVar8
                if not (r1 ~= nil and not r1:IsNull()) then
                    pCVar6 = {x = 0, y = 0, z = 0}
                else
                    pCVar6 = r1:GetPos()
                end
            end
            pCVar7 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", pCVar6, f_stk_58, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
            piVar1 = nil --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
            uVar2 = nil --[[unresolved native value]]
            if r2._8_4_ ~= piVar1 then
                -- TODO(native): xStack_30._4_4_ = uVar2;
                -- TODO(native): xStack_30._8_4_ = piVar1;
                if piVar1 ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            pCVar7 = nil
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                fVar8 = r1:GetAngleXY()
                f_stk_54 = fVar8
                if not (r1 ~= nil and not r1:IsNull()) then
                    pCVar6 = {x = 0, y = 0, z = 0}
                else
                    pCVar6 = r1:GetPos()
                end
            end
            pCVar7 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", pCVar6, f_stk_54, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
            piVar1 = nil --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
            uVar2 = nil --[[unresolved native value]]
            if r3._8_4_ ~= piVar1 then
                -- TODO(native): xStack_3c._4_4_ = uVar2;
                -- TODO(native): xStack_3c._8_4_ = piVar1;
                if piVar1 ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            pCVar7 = nil
        end
        cVar5 = quest:GetStateBool("TradersShouldBeScared")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        r4 = quest:GetThingWithScriptName("DarkwoodRockTroll")
        cVar5 = quest:GetStateBool("TradersShouldBeScared")
        while cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e046b6 end
            -- TODO(native): cVar5 = (**(r2._0_4_ + 0x12c))()
            cVar5 = nil --[[unresolved native value]]
            native_arg_sequence_2 = false
            if not cVar5 then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if native_arg_sequence_2 then
                -- TODO(native): cVar5 = (**(r3._0_4_ + 0x12c))()
                cVar5 = nil --[[unresolved native value]]
                if not cVar5 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
            end
            if native_arg_sequence_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e046b6 end
                if not (r1 ~= nil and not r1:IsNull()) then
                    pCVar6 = {x = 0, y = 0, z = 0}
                else
                    fVar8 = r1:GetAngleXY()
                    f_stk_4c = fVar8
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pCVar6 = {x = 0, y = 0, z = 0}
                    else
                        pCVar6 = r1:GetPos()
                    end
                end
                pCVar7 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01", pCVar6, f_stk_4c, false)
                -- TODO(native): piVar1 = *(pCVar7 + 0x8)
                piVar1 = nil --[[unresolved native value]]
                -- TODO(native): uVar2 = *(pCVar7 + 0x4)
                uVar2 = nil --[[unresolved native value]]
                if r2._8_4_ ~= piVar1 then
                    -- TODO(native): xStack_30._4_4_ = uVar2;
                    -- TODO(native): xStack_30._8_4_ = piVar1;
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                if not (r1 ~= nil and not r1:IsNull()) then
                    pCVar6 = {x = 0, y = 0, z = 0}
                else
                    fVar8 = r1:GetAngleXY()
                    f_stk_58 = fVar8
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pCVar6 = {x = 0, y = 0, z = 0}
                    else
                        pCVar6 = r1:GetPos()
                    end
                end
                pCVar7 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02", pCVar6, f_stk_58, false)
                -- TODO(native): piVar1 = *(pCVar7 + 0x8)
                piVar1 = nil --[[unresolved native value]]
                -- TODO(native): uVar2 = *(pCVar7 + 0x4)
                uVar2 = nil --[[unresolved native value]]
                if r3._8_4_ ~= piVar1 then
                    -- TODO(native): xStack_3c._4_4_ = uVar2;
                    -- TODO(native): xStack_3c._8_4_ = piVar1;
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
            end
            cVar5 = quest:GetStateBool("TradersShouldBeScared")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            quest:RemoveThing(r2, false, true)
            quest:RemoveThing(r3, false, true)
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                pCVar6 = r1:GetPos()
            end
            bVar10 = false
            bVar4 = false
            fVar9 = r1:GetAngleXY()
            r5 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_01_OFF", pCVar6, fVar9, bVar4)
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                pCVar6 = r1:GetPos()
            end
            bVar10 = false
            bVar4 = false
            fVar9 = r1:GetAngleXY()
            r6 = quest:CreateEffectAtPos("NEW_RED_FORCEFIELD_IDLE_02_OFF", pCVar6, fVar9, bVar4)
            quest:RemoveThing(me, false, true)
        end
        ::LAB_00e046b6::
    end
    ::LAB_00e046bf::
end

function Init(quest, me)
    quest:EntitySetAsDrawable(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

