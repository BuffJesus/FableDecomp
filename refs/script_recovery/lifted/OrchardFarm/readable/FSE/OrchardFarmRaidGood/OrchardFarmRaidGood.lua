-- Readable native conversion: Q_OrchardFarmRaidGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_OrchardFarmRaidGood.Main (retail 0x00dd2390)
function Main(quest)
end

-- Q_OrchardFarmRaidGood.Init (retail 0x00dd23a0)
function Init(quest)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(384), quest:ReadGlobalGameData(388), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(392), quest:ReadGlobalGameData(396), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(400), quest:ReadGlobalGameData(404), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN", 17, quest:ReadGlobalGameData(3464), quest:ReadGlobalGameData(3468), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 19, quest:ReadGlobalGameData(3472), quest:ReadGlobalGameData(3476), false, "", 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

