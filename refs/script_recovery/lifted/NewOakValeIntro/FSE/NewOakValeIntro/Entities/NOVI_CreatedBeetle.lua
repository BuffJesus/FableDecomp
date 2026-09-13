-- Generated native draft: NOVI_CreatedBeetle. Review coverage report before use.
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
    local iVar2, timerId
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 5)
        iVar2 = quest:GetTimer(timerId)
        while iVar2 ~= 0 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:DeregisterTimer(timerId)
                return
            end
            iVar2 = quest:GetTimer(timerId)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:RemoveThing(me, true, true)
        end
        quest:DeregisterTimer(timerId)
    end
end

