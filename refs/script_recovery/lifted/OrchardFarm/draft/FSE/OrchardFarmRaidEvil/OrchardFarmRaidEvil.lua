-- Generated native draft: Q_OrchardFarmRaidEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
end

function Init(quest)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x168), quest:ReadGlobalGameData(0x16c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x170), quest:ReadGlobalGameData(0x174), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(0x178), quest:ReadGlobalGameData(0x17c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS", 8, quest:ReadGlobalGameData(0xd78), quest:ReadGlobalGameData(0xd7c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 0x12, quest:ReadGlobalGameData(0xd80), quest:ReadGlobalGameData(0xd84), true, "", 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

