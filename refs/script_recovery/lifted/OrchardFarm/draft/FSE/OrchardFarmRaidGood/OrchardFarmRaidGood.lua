-- Generated native draft: Q_OrchardFarmRaidGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
end

function Init(quest)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_4,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, *(DAT_0143e90c + 0x180), *(DAT_0143e90c + 0x184), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, *(DAT_0143e90c + 0x188), *(DAT_0143e90c + 0x18c), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, *(DAT_0143e90c + 400), *(DAT_0143e90c + 0x194), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN", 0x11, *(DAT_0143e90c + 0xd88), *(DAT_0143e90c + 0xd8c), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 0x13, *(DAT_0143e90c + 0xd90), *(DAT_0143e90c + 0xd94), false, nil --[[missing]], 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

