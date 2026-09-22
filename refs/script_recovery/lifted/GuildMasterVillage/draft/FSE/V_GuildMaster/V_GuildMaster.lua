-- Generated native draft: V_GuildMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    quest:AddEntityBinding("GuildMasterGameFlow", "V_GuildMaster/Entities/GuildMasterGameFlow", 1)
    quest:FinalizeEntityBindings()
end

function Init(quest)
    -- TODO(native): *(undefined4 *)__element("GuildMasterDialogue", 0) = 0;
    -- TODO(native): *(undefined1 *)__element("GuildMasterDialogue", 1) = 0;
end

