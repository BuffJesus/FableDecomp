-- Generated native draft: ChickenSign. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- CChickenSign::TextKeys (+0x1c): filled once by native Init from .rdata literals, never written again
local TextKeys = {"TEXT_QST_B17_SIGN_WON_NOTHING", "TEXT_QST_B17_SIGN_WON_EXPR", "TEXT_QST_B17_SIGN_WON_KEY", "TEXT_QST_B17_SIGN_WON_KEY_EXPR", "TEXT_QST_B17_SIGN_WON_HAT", "TEXT_QST_B17_SIGN_WON_HAT_EXPR", "TEXT_QST_B17_SIGN_WON_HAT_KEY", "TEXT_QST_B17_SIGN_WON_HAT_KEY_EXPR"}

function Main(quest, me)
    local bVar3
    local alive = true
    quest:SetThingAsUsable(me, true)
    local pCVar5 = TextKeys[quest:GetStateInt("PrizesWon") + 1]
    local pCVar4 = quest:GetThingWithScriptName("ChickenSign")
    quest:SetReadableObjectText(pCVar4, pCVar5)
    pCVar4 = nil
    local iVar2 = quest:GetStateInt("PrizesWon")
    while true do
        if iVar2 == 7 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        if iVar2 ~= quest:GetStateInt("PrizesWon") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            pCVar5 = TextKeys[quest:GetStateInt("PrizesWon") + 1]
            pCVar4 = quest:GetThingWithScriptName("ChickenSign")
            quest:SetReadableObjectText(pCVar4, pCVar5)
            pCVar4 = nil
            iVar2 = quest:GetStateInt("PrizesWon")
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

