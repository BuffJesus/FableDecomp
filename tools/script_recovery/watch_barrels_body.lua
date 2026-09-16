-- Native DBE890..DBEB17. One retained vector spans every refresh and the loop.
local function watchBarrelsWithSnapshot(quest, resources, addBadDeed)
    local barrels = resources:NewBarrelWatchSnapshot()
    local ok, failure = pcall(function()
        while barrels:Refresh() <= 0 do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        processBarrelBreaks(quest, resources, barrels:Count(), addBadDeed)
    end)
    local closed, closeError = pcall(barrels.Close, barrels)
    if not ok then error(failure, 0) end
    if not closed then error(closeError, 0) end
end

return watchBarrelsWithSnapshot
