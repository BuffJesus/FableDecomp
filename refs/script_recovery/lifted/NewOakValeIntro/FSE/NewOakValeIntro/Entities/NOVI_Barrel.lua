-- Generated native draft: NOVI_Barrel. Review coverage report before use.
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
end

function Main(quest, me)
    local bVar1, cVar2, pCVar3, pCVar5
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        cVar2 = quest:GetStateBool("InstructionGiven_Barrels")
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            pCVar3 = quest:GetHero()
            bVar1 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, 2.0)
            if bVar1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                cVar2 = quest:IsXbox()
                if not cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        return
                    end
                    pCVar5 = "TEXT_QST_048_INSTRUCTION_BREAK_BARRELS_PC"
                    quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_BREAK_BARRELS_PC")
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                    while not cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                        cVar2 = quest:MsgIsGameInfoClickedPast()
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        return
                    end
                    pCVar5 = "TEXT_QST_048_INSTRUCTION_BREAK_BARRELS"
                    quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_BREAK_BARRELS")
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                    while not cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                        cVar2 = quest:MsgIsGameInfoClickedPast()
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                quest:SetStateBool("InstructionGiven_Barrels", true)
            end
            cVar2 = quest:GetStateBool("InstructionGiven_Barrels")
        end
        alive = not quest:IsActiveThreadTerminating()
        cVar2 = not alive
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            cVar2 = not alive
        end
    end
end

function OnPredicateFail(quest, me)
    quest:SetStateBool("BarrelBrokenInstantaneous", true)
    quest:SetStateBool("BarrelBrokenPersistent", true)
    local puVar2 = me:GetPos()
    -- TODO(native): *(undefined4 *)(iVar1 + 0x76) = *puVar2;
    -- TODO(native): *(undefined4 *)(iVar1 + 0x7a) = puVar2[1];
    -- TODO(native): *(undefined4 *)(iVar1 + 0x7e) = puVar2[2];
end

