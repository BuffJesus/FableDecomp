-- Generated native draft: BordelloEntrance. Review coverage report before use.
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
    local pCVar4
    local alive = true
    quest:EntitySetAsLocked(me, true)
    local bVar3 = quest:IsQuestCompleted("Q_TraderEscort")
    repeat
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                if not quest:GetStateBool("BecomeNunnery") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    pCVar4 = quest:GetThingWithScriptName("BordelloSign")
                    quest:SetReadableObjectTextTag(pCVar4, "TEXT_QST_B13_BORDELLO_OPEN")
                    quest:EntitySetAsLocked(me, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    pCVar4 = quest:GetThingWithScriptName("BordelloSign")
                    quest:SetReadableObjectTextTag(pCVar4, "TEXT_QST_B13_BORDELLO_REFUGE")
                    quest:EntitySetAsLocked(me, false)
                end
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                until not (not bVar3)
            end
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = me:MsgIsUsedByHero()
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            quest:DisplayGameInfo("TEXT_QST_B13_BORDELLO_CLOSED")
            bVar3 = quest:MsgIsGameInfoClickedPast()
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                bVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
        end
        bVar3 = quest:IsQuestCompleted("Q_TraderEscort")
    until false
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

