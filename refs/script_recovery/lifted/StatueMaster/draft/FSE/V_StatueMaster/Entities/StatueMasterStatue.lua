-- Generated native draft: StatueMasterStatue. Review coverage report before use.
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
    local CVar2, CVar4, bVar3, f_stk_10, fret_0, xStack_24
    local alive = true
    quest:EntitySetCutsceneBehaviour(me, 2)
    local r1 = quest:GetThingWithScriptName("StatueAltar")
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            r1 = nil
            return
        end
        f_stk_10 = require("V_StatueMaster.native_quest_helpers").helper_ED43D0(quest, me)
        f_stk_10 = f_stk_10 + 0.5
        bVar3 = true
        quest:EntitySetFacingAngle(me, fret_0, bVar3)
        CVar2 = 0x0
        CVar4 = require("V_StatueMaster.native_quest_helpers").GetStatuePointingPosition(quest, me)
        xStack_24 = CVar4
        if CVar4 ~= CVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                r1 = nil
                return
            end
            if CVar4 == 0x1 then
                quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_BOWERSTONE")
            elseif CVar4 == 0x3 then
                quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_GREATWOOD")
            else
                quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_NOWHERE")
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
end

function Init(quest, me)
    quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_NOWHERE")
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

