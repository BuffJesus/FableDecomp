-- Generated native draft: RaceMarker. Review coverage report before use.
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
    local bVar1, pCVar2
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        repeat
            pCVar2 = quest:GetHero()
            bVar1 = quest:IsDistanceBetweenThingsUnder(pCVar2, me, 3.0)
            if bVar1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                quest:SetStateBool("ReachedPlatform", true)
                quest:MiniMapRemoveMarker(me)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
        until not (alive)
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

