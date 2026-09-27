-- Generated native draft: Dragon. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar2, bVar3, fret_0, fret_00, fret_01, iVar1, p0, this_00, uVar4
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        bVar2 = false
        iVar1 = quest:GetStateInt("DragonState")
        while iVar1 ~= 4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar2 then
                if bVar3 then
                    return
                end
                bVar3 = quest:IsCreatureFlying(me)
                if not bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    bVar2 = false
                end
            else
                if bVar3 then
                    return
                end
                bVar3 = quest:IsCreatureFlying(me)
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    require("DragonBossFight.native_quest_helpers").helper_D25C20(quest, me)
                    bVar2 = true
                end
            end
            __native_condition_1 = quest:GetStateInt("DragonState") == 0
            if __native_condition_1 then
                fret_0 = quest:GetHealth(me)
                __native_condition_1 = fret_0 < __native_entity_state:GetStateFloat("MediumHealth") ~= (fret_0 == __native_entity_state:GetStateFloat("MediumHealth"))
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                quest:SetStateInt("DragonState", 1)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbc4))
                    uVar4 = quest:ReadGlobalGameData(0xbd4)
                    goto LAB_00d25bfd
                end
                -- TODO(native): } else {
                __native_condition_2 = quest:GetStateInt("DragonState") == 1
                if __native_condition_2 then
                    fret_00 = quest:GetHealth(me)
                    __native_condition_2 = fret_00 < __native_entity_state:GetStateFloat("LowHealth") ~= (fret_00 == __native_entity_state:GetStateFloat("LowHealth"))
                end
                if __native_condition_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:SetStateInt("DragonState", 2)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbc8))
                        uVar4 = quest:ReadGlobalGameData(0xbd8)
                        goto LAB_00d25bfd
                    end
                    -- TODO(native): } else {
                    __native_condition_3 = quest:GetStateInt("DragonState") == 2
                    if __native_condition_3 then
                        fret_01 = quest:GetHealth(me)
                        __native_condition_3 = fret_01 < __native_entity_state:GetStateFloat("VeryLowHealth") ~= (fret_01 == __native_entity_state:GetStateFloat("VeryLowHealth"))
                    end
                    if __native_condition_3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        quest:SetStateInt("DragonState", 3)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbcc))
                            uVar4 = quest:ReadGlobalGameData(0xbdc)
                            goto LAB_00d25bfd
                        end
                    end
                end
            end
            goto FLOW_past_lab_00d25bfd
            ::LAB_00d25bfd::
            -- TODO(native): *(undefined4 *)(this_00 + 0x6c) = uVar4;
            ::FLOW_past_lab_00d25bfd::
            iVar1 = quest:GetStateInt("DragonState")
        end
        alive = not quest:IsActiveThreadTerminating()
    end
end

function Init(quest, me)
    -- TODO(native): helper_D258C0(quest, me, *(this + 0x14), 0)
    __native_entity_state:SetStateFloat("MediumHealth", quest:ReadGlobalGameData(0xbb4))
    __native_entity_state:SetStateFloat("LowHealth", quest:ReadGlobalGameData(0xbb8))
    __native_entity_state:SetStateFloat("VeryLowHealth", quest:ReadGlobalGameData(0xbbc))
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if cVar1 then
        quest:SetStateInt("DragonState", 4)
    end
end

function helper_D258C0(quest, me, native_arg_param_1)
    local bVar1
    local alive = true
    -- TODO(native): name field 0x48 (int)
    __native_entity_state:SetStateInt("self_0x48", native_arg_param_1)
    if native_arg_param_1 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            -- TODO(native): name field 0x68 (undefined4)
            __native_entity_state:SetStateInt("self_0x68", quest:ReadGlobalGameData(0xbc0))
            -- TODO(native): name field 0x6c (undefined4)
            __native_entity_state:SetStateInt("self_0x6c", quest:ReadGlobalGameData(0xbd0))
            return
        end
    elseif native_arg_param_1 == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            -- TODO(native): name field 0x68 (undefined4)
            __native_entity_state:SetStateInt("self_0x68", quest:ReadGlobalGameData(0xbc4))
            -- TODO(native): name field 0x6c (undefined4)
            __native_entity_state:SetStateInt("self_0x6c", quest:ReadGlobalGameData(0xbd4))
            return
        end
    elseif native_arg_param_1 == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            -- TODO(native): name field 0x68 (undefined4)
            __native_entity_state:SetStateInt("self_0x68", quest:ReadGlobalGameData(0xbc8))
            -- TODO(native): name field 0x6c (undefined4)
            __native_entity_state:SetStateInt("self_0x6c", quest:ReadGlobalGameData(0xbd8))
            return
        end
    elseif native_arg_param_1 == 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            -- TODO(native): name field 0x68 (undefined4)
            __native_entity_state:SetStateInt("self_0x68", quest:ReadGlobalGameData(0xbcc))
            -- TODO(native): name field 0x6c (undefined4)
            __native_entity_state:SetStateInt("self_0x6c", quest:ReadGlobalGameData(0xbdc))
        end
    end
end

