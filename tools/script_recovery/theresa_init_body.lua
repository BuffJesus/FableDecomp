-- Complete native Init DAC4F0: entity flags reset before the first engine call.
local function initializeTheresa(quest, me, resources, state)
    state:SetStateBool("DoneIntro", false)
    state:SetStateBool("AskedForPresent", false)
    resources:InitializeTheresaActor(me)
end

return initializeTheresa
