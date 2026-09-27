-- Generated from the same native helper bodies as the quest draft.
local AddGossip
-- E55C60: bsim names this body NScript::CV_BookCollectingScript::AddGossip (a homologous script member); no PDB name
function AddGossip(quest, me, strParam1, strParam2, strParam3, strParam4)
    quest:AddRumourCategory(strParam1)
    quest:AddNewRumourToCategory(strParam1, strParam2)
    quest:AddGossipVillage(strParam1, strParam3)
    quest:AddGossipFactionToCategory(strParam1, strParam4)
end

return {AddGossip = AddGossip}
