-- Generated native draft: CampHostageDoor. Review coverage report before use.
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
    local alive = true
    quest:CloseDoor(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if not bVar1 then
        while true do
            bVar1 = me:MsgIsUsedByHero()
            if bVar1 then break end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:OpenDoor(me)
            quest:SetStateBool("OpenedCage", true)
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

