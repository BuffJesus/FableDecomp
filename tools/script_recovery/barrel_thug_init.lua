-- Native DB6BF0..DB6C31. Entity state stores precede all actor calls.
local function initializeBarrelThug(quest, me, resources, state)
    state:SetStateBool("DoneIntro", false)
    state:SetStateInt("LastTimeSpoken", 9999)
    resources:InitializeBarrelThugActor(me)
end

return initializeBarrelThug
