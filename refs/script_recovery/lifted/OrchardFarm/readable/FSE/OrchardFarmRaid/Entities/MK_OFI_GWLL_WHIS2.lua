-- Readable native conversion: MK_OFI_GWLL_WHIS2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("OrchardFarmRaid.native_quest_helpers")

-- MK_OFI_GWLL_WHIS2.Main (retail 0x00dd1eb0)
function Main(quest, me)
    if not ((not quest:GetStateBool("HeroMetWhisperBeforeFarm")) and (quest:GetStateInt("HeroTeam") == 0)) then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("HeroMetWhisperBeforeFarm", true)
    helpers.DoMultiplierCutscene(quest, me)
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

