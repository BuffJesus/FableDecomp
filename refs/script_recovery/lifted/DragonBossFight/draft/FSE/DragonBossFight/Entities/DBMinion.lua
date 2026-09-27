-- Generated native draft: DBMinion. Review coverage report before use.
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
    local iVar1, pTarget
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar2 = not alive
    if not bVar2 then
        pTarget = quest:GetHero()
        quest:GiveThingBestEnemyTarget(me, pTarget)
        iVar1 = quest:GetStateInt("DragonState")
        while iVar1 ~= 4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            iVar1 = quest:GetStateInt("DragonState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:RemoveThing(me, true, true)
        end
    end
end

function Init(quest, me)
    quest:SetStateInt("NumMinions", quest:GetStateInt("NumMinions") + 1)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt("NumMinions", quest:GetStateInt("NumMinions") + -1)
    local iVar1 = quest:GetTimer(quest:GetStateInt("MinionSpawnDelay"))
    quest:SetTimer(quest:GetStateInt("MinionSpawnDelay"), iVar1 + 0x14)
end

