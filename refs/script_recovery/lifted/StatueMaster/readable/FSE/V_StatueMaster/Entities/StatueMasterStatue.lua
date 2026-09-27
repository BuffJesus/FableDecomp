-- Readable native conversion: StatueMasterStatue. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)

local helpers = require("V_StatueMaster.native_quest_helpers")

-- StatueMasterStatue.Main (retail 0x00ed41d0)
function Main(quest, me)
    local f_stk_10_2, fret_0
    quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        f_stk_10_2 = helpers.HasCameraMode(quest, me) + 0.5
        quest:EntitySetFacingAngle(me, fret_0, true)
        local scratchValue = helpers.GetStatuePointingPosition(quest, me)
        if scratchValue == 0 then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then return end
            if scratchValue == 1 then
                quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_BOWERSTONE")
            elseif scratchValue == 3 then
                quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_GREATWOOD")
            else
                quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_NOWHERE")
            end
            quest:NewScriptFrame(me)
        end
    until false
end

-- StatueMasterStatue.Init (retail 0x00ed4170)
function Init(quest, me)
    quest:SetReadableObjectTextTag(me, "TEXT_QST_061_STATUE_NOWHERE")
end

-- StatueMasterStatue.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- StatueMasterStatue.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

