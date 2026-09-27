-- Generated native draft: StatueMasterCellarDoors. Review coverage report before use.
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
    local bVar1, bVar2, bVar3, bVar4, iVar5, pCVar6
    local alive = true
    bVar1 = false
    bVar4 = false
    iVar5 = require("V_StatueMaster.native_quest_helpers").GetStatuePointingPosition(quest, me)
    if iVar5 == 1 then
        bVar1 = true
        bVar4 = true
        pCVar6 = quest:GetThingWithScriptName("TraderToEscort")
        bVar3 = (pCVar6 ~= nil and pCVar6:IsAlive())
        bVar2 = true
        if not bVar3 then goto LAB_00ed45ff end
    end
    bVar2 = false
    ::LAB_00ed45ff::
    if bVar4 then
        pCVar6 = nil
    end
    if bVar1 then
    end
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            quest:EntitySetAsLocked(me, false)
            return
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            quest:EntitySetAsLocked(me, true)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            while not bVar4 do
                bVar4 = me:MsgIsUsedByHero()
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:DisplayGameInfo("TEXT_QST_061_CELLAR_LOCKED")
                    bVar4 = quest:MsgIsGameInfoClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            return
                        end
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
            end
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

