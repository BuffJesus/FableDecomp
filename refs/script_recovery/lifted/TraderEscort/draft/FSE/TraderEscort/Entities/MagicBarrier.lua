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
    local __native_condition_1, __native_condition_2, bVar10, bVar4, cVar5, fVar8, fVar9, f_stk_4c, f_stk_54, f_stk_58, f_stk_60, pCVar6, pCVar7, r1, r2, r3, r4, r5, r6
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
        cVar5 = (r2 ~= nil and r2:IsAlive())
        __native_condition_1 = not cVar5
        if __native_condition_1 then
            cVar5 = (r3 ~= nil and r3:IsAlive())
            __native_condition_1 = not cVar5
        end
        if __native_condition_1 then
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
            r2 = pCVar7
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
            r3 = pCVar7
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
            cVar5 = (r2 ~= nil and r2:IsAlive())
            __native_condition_2 = not cVar5
            if __native_condition_2 then
                cVar5 = (r3 ~= nil and r3:IsAlive())
                __native_condition_2 = not cVar5
            end
            if __native_condition_2 then
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
                r2 = pCVar7
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
                r3 = pCVar7
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

