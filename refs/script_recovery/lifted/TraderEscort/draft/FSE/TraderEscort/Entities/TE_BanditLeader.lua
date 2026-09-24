-- Generated native draft: TE_BanditLeader. Review coverage report before use.
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
    local conversationID, fVar5, pCVar4
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar3 = not alive
    if not bVar3 then
        fVar5 = 15.0
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, fVar5)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            fVar5 = 15.0
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, fVar5)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            conversationID = quest:AddNewConversation(me, false, false)
            pCVar4 = quest:GetHero()
            quest:AddPersonToConversation(conversationID, pCVar4)
            pCVar4 = quest:GetHero()
            quest:AddLineToConversation(conversationID, "TEXT_QST_067_BANDIT_ATTACK", me, pCVar4, false)
            pCVar4 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, pCVar4)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            until not (not bVar3)
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

