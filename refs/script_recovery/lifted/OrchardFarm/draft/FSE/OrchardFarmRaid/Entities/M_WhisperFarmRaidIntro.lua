-- Generated native draft: M_WhisperFarmRaidIntro. Review coverage report before use.
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
    local bVar1
    local alive = true
    if not quest:GetStateBool("HeroMetWhisperBeforeFarm") then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetStateBool("HeroMetWhisperBeforeFarm", true)
            require("OrchardFarmRaid.native_quest_helpers").DoMultiplierCutscene(quest, me)
            return
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

