-- Source-only adapter for reviewed root/callback operations, no registration.
local A = {}
function A.new(quest, resources)
    local h = {}
    function h:spoke() return quest:GetStateBool("GuardianSpokeToHero") end
    function h:set_spoke(v) quest:SetStateBool("GuardianSpokeToHero", v) end
    function h:persist_bool(...) return quest:PersistTransferBool(...) end
    function h:bind(name, flags)
        assert(name == "MazeAtTavern")
        quest:AddEntityBinding(name, "GuardianSisterInfo/Entities/MazeAtTavern", flags)
    end
    function h:finalize() quest:FinalizeEntityBindings() end
    function h:active() return quest:GetActiveQuestName() end
    function h:objective(...) quest:SetQuestCardObjective(...) end
    function h:region_loaded(name) return quest:IsRegionLoaded(name) end
    function h:frame() resources:Frame() end
    function h:terminating() return quest:IsActiveThreadTerminating() end
    function h:time(value) quest:SetTimeOfDay(value) end
    function h:deactivate(...) quest:DeactivateQuestLater(...) end
    return h
end
function A.run(reference, entry, quest)
    quest:WithRetailResources(function(resources) reference[entry](A.new(quest, resources)) end)
end
return A
