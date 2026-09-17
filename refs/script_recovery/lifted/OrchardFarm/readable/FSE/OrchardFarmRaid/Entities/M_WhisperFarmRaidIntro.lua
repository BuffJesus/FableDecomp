-- Readable native conversion: M_WhisperFarmRaidIntro. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("OrchardFarmRaid.native_quest_helpers")

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    if not quest:GetStateBool("HeroMetWhisperBeforeFarm") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("HeroMetWhisperBeforeFarm", true)
        helpers.DoMultiplierCutscene(quest, me)
        return
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

