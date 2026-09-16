-- Caller registers the bound-conscious condition. Original entry resumes DB6CB0.
local function runBarrelThugMainAfterCondition(quest, me, resources, state)
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    local control = resources:NewResource()
    local ok, failure = pcall(function()
        if quest:IsActiveThreadTerminating() then return end
        while true do
            if not state:GetStateBool("DoneIntro") then
                if not introduceBarrelThug(quest, me, resources, control, state) then return end
            end
            if resources:WasVillagerTalkedTo(me) then
                if not runBarrelThugConversation(quest, me, resources, control) then return end
            end
            if not updateBarrelThugTimedRemarks(quest, me, resources, control, state) then return end
            if resources:IsHitByHeroExceptAbility(me, 14) then
                if not handleBarrelThugHit(quest, me, resources, control) then return end
            end
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return end
        end
    end)
    local released, releaseError = pcall(resources.ReleaseResource, resources, control)
    if not ok then error(failure, 0) end
    if not released then error(releaseError, 0) end
end

return runBarrelThugMainAfterCondition
