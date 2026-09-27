-- Generated from the same native helper bodies as the quest draft.
local AddGossip
-- E55C60: bsim names this body NScript::CV_BookCollectingScript::AddGossip (a homologous script member); no PDB name
function AddGossip(quest, me, strParam1)
    quest:AddRumourCategory(strParam1)
    quest:AddNewRumourToCategory(strParam1, nil --[[missing]])
    quest:AddGossipVillage(strParam1, nil --[[missing]])
    quest:AddGossipFactionToCategory(strParam1, nil --[[missing]])
end

return {AddGossip = AddGossip}
