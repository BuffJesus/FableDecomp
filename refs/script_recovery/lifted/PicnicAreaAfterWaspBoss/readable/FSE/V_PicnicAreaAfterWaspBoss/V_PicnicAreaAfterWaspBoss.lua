-- Readable native conversion: V_PicnicAreaAfterWaspBoss. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_PicnicAreaAfterWaspBoss.Main (retail 0x00ec12e0)
function Main(quest)
    quest:AddEntityBinding("PAAWB_Villager", "V_PicnicAreaAfterWaspBoss/Entities/PAAWB_Villager")
    quest:AddEntityBinding("PAAWB_Guard", "V_PicnicAreaAfterWaspBoss/Entities/PAAWB_Guard")
    quest:FinalizeEntityBindings()
end

