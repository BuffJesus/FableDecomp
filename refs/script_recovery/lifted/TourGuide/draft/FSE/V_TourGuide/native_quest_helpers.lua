-- Generated from the same native helper bodies as the quest draft.
local helper_EE6850
function helper_EE6850(quest, me, native_arg_param_1, native_arg_param_2)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar4, cVar5, iVar1, iVar9, native_arg_sequence_1, pCVar6, pCVar7, puVar8, uVar10, uVar11, uVar12, uVar13, xStack_18
    local alive = true
    bVar4 = (native_arg_param_1 ~= nil and native_arg_param_1:IsAlive())
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            uVar13 = 1
            uVar12 = 0
            uVar11 = 0
            uVar10 = 1.0
            pCVar6 = native_arg_param_1:GetPos()
            resources:MoveToPosition(native_arg_param_2, pCVar6, uVar10, uVar11, (uVar12 ~= 0), (uVar13 ~= 0))
            return
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 4);
            -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 8);
            xStack_18 = nil
            if xStack_18 ~= nil then
                -- TODO(native): *xStack_18 = *xStack_18 + 1;
            end
            iVar1 = quest:GetStateInt("WaypointCounter")
            while true do
                __native_condition_1 = xStack_18 == nil
                if not __native_condition_1 then
                    cVar5 = (xStack_18 ~= nil and xStack_18:IsAlive())
                    __native_condition_1 = not cVar5
                end
                if not __native_condition_1 then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00ee6a24 end
                iVar9 = quest:GetStateInt("WaypointCounter") + 1
                quest:SetStateInt("WaypointCounter", iVar9)
                if iVar9 < 0x12 then
                    native_arg_sequence_1 = false
                    if iVar9 == iVar1 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then goto LAB_00ee6a24 end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6a24 end
                    quest:SetStateInt("WaypointCounter", 0)
                end
                pCVar7 = quest:GetThingWithScriptName(quest:GetStateString(("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker")))
                xStack_18 = pCVar7
                pCVar7 = nil
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                if xStack_18 == nil then
                    puVar8 = {x = 0, y = 0, z = 0}
                else
                    puVar8 = xStack_18:GetPos()
                end
                resources:MoveToPosition(native_arg_param_2, puVar8, 1.0, 0, false, true)
            end
            ::LAB_00ee6a24::
            return
        end
    end
end

return {helper_EE6850 = helper_EE6850}
