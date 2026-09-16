-- Native DB98E4..DB99CA. Control is already acquired by the caller.
-- Requires explicit-resource PlayTheresaSkip and raw-Hero proximity adapters.
local function waitForHeroToApproachTheresa(quest, me, resources, control)
    while true do
        if quest:IsActiveThreadTerminating() then return false end
        resources:PlayTheresaSkip(control)
        while not resources:IsTheresaNearHero(me, 5.0) do
            if not resources:IsPerformingScriptTask(control) then break end
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        if resources:IsTheresaNearHero(me, 5.0) then return true end
        quest:NewScriptFrame(me)
    end
end

return waitForHeroToApproachTheresa
