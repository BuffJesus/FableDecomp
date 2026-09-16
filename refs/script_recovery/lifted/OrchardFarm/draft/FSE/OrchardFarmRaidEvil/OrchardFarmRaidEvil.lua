-- Generated native draft: Q_OrchardFarmRaidEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
end

function Init(quest)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_4,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, *(DAT_0143e90c + 0x168), *(DAT_0143e90c + 0x16c), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, *(DAT_0143e90c + 0x170), *(DAT_0143e90c + 0x174), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, *(DAT_0143e90c + 0x178), *(DAT_0143e90c + 0x17c), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS", 8, *(DAT_0143e90c + 0xd78), *(DAT_0143e90c + 0xd7c), false, nil --[[missing]], 0)
    -- TODO(native): CCharString::CCharString((CCharString *)&local_8,&DAT_0122d70e,-1);
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 0x12, *(DAT_0143e90c + 0xd80), *(DAT_0143e90c + 0xd84), true, nil --[[missing]], 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

