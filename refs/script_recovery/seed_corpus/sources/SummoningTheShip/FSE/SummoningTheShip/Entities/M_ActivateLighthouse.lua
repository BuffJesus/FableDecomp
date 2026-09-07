-- M_ActivateLighthouse.lua
-- Entity script stub for M_ActivateLighthouse marker
-- Functional parity port of NScript::CQ_SummoningTheShipScript::CM_ActivateLighthouse

function Init(quest, me)
end

function Main(quest, me)
    while true do
        if not quest:NewScriptFrame(me) then break end
    end
end
