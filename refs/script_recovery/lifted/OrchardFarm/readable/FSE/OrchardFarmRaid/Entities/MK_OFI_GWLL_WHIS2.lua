-- Readable native conversion: MK_OFI_GWLL_WHIS2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("OrchardFarmRaid.native_quest_helpers")

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- MK_OFI_GWLL_WHIS2.Main (retail 0x00dd1eb0)
function Main(quest, me)
    if not quest:GetStateBool("HeroMetWhisperBeforeFarm") and quest:GetStateInt("HeroTeam") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("HeroMetWhisperBeforeFarm", true)
        helpers.DoMultiplierCutscene(quest, me)
        return
    end
end

-- MK_OFI_GWLL_WHIS2.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- MK_OFI_GWLL_WHIS2.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- MK_OFI_GWLL_WHIS2.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

