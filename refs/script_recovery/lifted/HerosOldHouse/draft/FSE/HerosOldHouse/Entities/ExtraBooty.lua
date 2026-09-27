-- Generated native draft: ExtraBooty. Review coverage report before use.
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
    local bVar2, pQuestName
    local alive = true
    bVar2 = quest:IsDiggingSpotEnabled(me)
    if bVar2 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:IsDiggingSpotEnabled(me)
        until not (bVar2)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    local __native_condition_2 = not bVar2
    if __native_condition_2 then
        quest:SetStateBool("BootyDugUp", true)
        __native_condition_2 = not quest:GetStateBool("WifeAttacked")
    end
    local __native_condition_1 = __native_condition_2
    if __native_condition_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        __native_condition_1 = not bVar2
    end
    if __native_condition_1 then
        bVar2 = false
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pQuestName, bVar2, false, false)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

