-- Readable native conversion: V_GuildMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_GuildMaster.Main (retail 0x00e90830)
function Main(quest)
    quest:AddEntityBinding("GuildMasterGameFlow", "V_GuildMaster/Entities/GuildMasterGameFlow", 1)
    quest:FinalizeEntityBindings()
end

-- V_GuildMaster.Init (retail 0x00e90780)
function Init(quest)
    quest:SetStateBool("GuildMasterDialogue_0", false)
    quest:SetStateBool("GuildMasterDialogue_1", false)
    quest:SetStateBool("GuildMasterDialogue_2", false)
    quest:SetStateBool("GuildMasterDialogue_3", false)
    quest:SetStateBool("GuildMasterDialogue_4", false)
end

