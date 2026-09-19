-- Readable native conversion: M_WhisperFarmRaidIntro. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("OrchardFarmRaid.native_quest_helpers")

-- M_WhisperFarmRaidIntro.Main (retail 0x00dd1ac0)
function Main(quest, me)
    if quest:GetStateBool("HeroMetWhisperBeforeFarm") then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("HeroMetWhisperBeforeFarm", true)
    helpers.DoMultiplierCutscene(quest, me)
end

-- M_WhisperFarmRaidIntro.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- M_WhisperFarmRaidIntro.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- M_WhisperFarmRaidIntro.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

