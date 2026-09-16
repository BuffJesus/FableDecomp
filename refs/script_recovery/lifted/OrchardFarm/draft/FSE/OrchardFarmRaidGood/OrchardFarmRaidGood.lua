-- Generated native draft: Q_OrchardFarmRaidGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
end

function Init(quest)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x180), quest:ReadGlobalGameData(0x184), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x188), quest:ReadGlobalGameData(0x18c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(400), quest:ReadGlobalGameData(0x194), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN", 0x11, quest:ReadGlobalGameData(0xd88), quest:ReadGlobalGameData(0xd8c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 0x13, quest:ReadGlobalGameData(0xd90), quest:ReadGlobalGameData(0xd94), false, "", 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

