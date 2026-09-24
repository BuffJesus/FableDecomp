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
    local bVar10, bVar4, cVar5, fVar8, fVar9, f_stk_4c, f_stk_54, f_stk_58, f_stk_60, native_arg_sequence_1, native_arg_sequence_2, pCVar6, piVar1, r1, r2, uVar2, xStack_18, xStack_24, xStack_30, xStack_3c
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
    -- TODO(native): CreateEffect is not a ForgeFSE binding
    quest:CreateEffect(xStack_30, "NEW_RED_FORCEFIELD_IDLE_01", pCVar6, "", f_stk_60, false, false)
    if not (r1 ~= nil and not r1:IsNull()) then
        pCVar6 = {x = 0, y = 0, z = 0}
    else
        fVar8 = r1:GetAngleXY()
        -- TODO(native): xStack_58 = (CCharString)(float)fVar8;
        if not (r1 ~= nil and not r1:IsNull()) then
            pCVar6 = {x = 0, y = 0, z = 0}
        else
            pCVar6 = r1:GetPos()
        end
    end
    -- TODO(native): CreateEffect is not a ForgeFSE binding
    quest:CreateEffect(xStack_3c, "NEW_RED_FORCEFIELD_IDLE_02", pCVar6, "", f_stk_58, false, false)
    cVar5 = quest:GetStateBool("TradersShouldBeScared")
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e046bf end
        -- TODO(native): cVar5 = (**(xStack_30._0_4_ + 0x12c))()
        cVar5 = nil --[[unresolved native value]]
        native_arg_sequence_1 = false
        if cVar5 == 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            -- TODO(native): cVar5 = (**(xStack_3c._0_4_ + 0x12c))()
            cVar5 = nil --[[unresolved native value]]
            if cVar5 == 0 then
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
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(xStack_24, "NEW_RED_FORCEFIELD_IDLE_01", pCVar6, "", f_stk_58, false, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
            piVar1 = nil --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
            uVar2 = nil --[[unresolved native value]]
            if xStack_30._8_4_ ~= piVar1 then
                -- TODO(native): xStack_30._4_4_ = uVar2;
                -- TODO(native): xStack_30._8_4_ = piVar1;
                if piVar1 ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            xStack_24 = nil
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                fVar8 = r1:GetAngleXY()
                -- TODO(native): xStack_54 = (CCharString)(float)fVar8;
                if not (r1 ~= nil and not r1:IsNull()) then
                    pCVar6 = {x = 0, y = 0, z = 0}
                else
                    pCVar6 = r1:GetPos()
                end
            end
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(xStack_18, "NEW_RED_FORCEFIELD_IDLE_02", pCVar6, "", f_stk_54, false, false)
            -- TODO(native): piVar1 = *(pCVar7 + 0x8)
            piVar1 = nil --[[unresolved native value]]
            -- TODO(native): uVar2 = *(pCVar7 + 0x4)
            uVar2 = nil --[[unresolved native value]]
            if xStack_3c._8_4_ ~= piVar1 then
                -- TODO(native): xStack_3c._4_4_ = uVar2;
                -- TODO(native): xStack_3c._8_4_ = piVar1;
                if piVar1 ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            xStack_18 = nil
        end
        cVar5 = quest:GetStateBool("TradersShouldBeScared")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        r2 = quest:GetThingWithScriptName("DarkwoodRockTroll")
        cVar5 = quest:GetStateBool("TradersShouldBeScared")
        while cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e046b6 end
            -- TODO(native): cVar5 = (**(xStack_30._0_4_ + 0x12c))()
            cVar5 = nil --[[unresolved native value]]
            native_arg_sequence_2 = false
            if cVar5 == 0 then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if native_arg_sequence_2 then
                -- TODO(native): cVar5 = (**(xStack_3c._0_4_ + 0x12c))()
                cVar5 = nil --[[unresolved native value]]
                if cVar5 == 0 then
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
                    -- TODO(native): xStack_4c = (CCharString)(float)fVar8;
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pCVar6 = {x = 0, y = 0, z = 0}
                    else
                        pCVar6 = r1:GetPos()
                    end
                end
                -- TODO(native): CreateEffect is not a ForgeFSE binding
                quest:CreateEffect(xStack_18, "NEW_RED_FORCEFIELD_IDLE_01", pCVar6, "", f_stk_4c, false, false)
                -- TODO(native): piVar1 = *(pCVar7 + 0x8)
                piVar1 = nil --[[unresolved native value]]
                -- TODO(native): uVar2 = *(pCVar7 + 0x4)
                uVar2 = nil --[[unresolved native value]]
                if xStack_30._8_4_ ~= piVar1 then
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
                -- TODO(native): CreateEffect is not a ForgeFSE binding
                quest:CreateEffect(xStack_24, "NEW_RED_FORCEFIELD_IDLE_02", pCVar6, "", f_stk_58, false, false)
                -- TODO(native): piVar1 = *(pCVar7 + 0x8)
                piVar1 = nil --[[unresolved native value]]
                -- TODO(native): uVar2 = *(pCVar7 + 0x4)
                uVar2 = nil --[[unresolved native value]]
                if xStack_3c._8_4_ ~= piVar1 then
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
            quest:RemoveThing(r2, xStack_30, false)
            quest:RemoveThing(r1, xStack_3c, false)
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                pCVar6 = r1:GetPos()
            end
            bVar10 = false
            bVar4 = false
            fVar9 = r1:GetAngleXY()
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(xStack_18, "NEW_RED_FORCEFIELD_IDLE_01_OFF", pCVar6, "", fVar9, bVar4, bVar10)
            if not (r1 ~= nil and not r1:IsNull()) then
                pCVar6 = {x = 0, y = 0, z = 0}
            else
                pCVar6 = r1:GetPos()
            end
            bVar10 = false
            bVar4 = false
            fVar9 = r1:GetAngleXY()
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect(xStack_18, "NEW_RED_FORCEFIELD_IDLE_02_OFF", pCVar6, "", fVar9, bVar4, bVar10)
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

