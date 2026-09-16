-- Readable native conversion: Q_OrchardFarmRaidEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
end

function Init(quest)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(360), quest:ReadGlobalGameData(364), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(368), quest:ReadGlobalGameData(372), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(376), quest:ReadGlobalGameData(380), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS", 8, quest:ReadGlobalGameData(3448), quest:ReadGlobalGameData(3452), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 18, quest:ReadGlobalGameData(3456), quest:ReadGlobalGameData(3460), true, "", 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

