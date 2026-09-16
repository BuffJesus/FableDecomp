-- Native DBE960..DBEAD4 after snapshot count and the preceding termination query.
-- Caller owns the retained barrel snapshot. Reward adapters own temporary Things.
local function processBarrelBreaks(quest, resources, total, addBadDeed)
    quest:SetStateBool("BarrelBrokenInstantaneous", false)
    local broken = 0
    if quest:IsActiveThreadTerminating() then return end
    while not quest:GetStateBool("AttackOver") do
        if quest:GetStateBool("BarrelBrokenInstantaneous") then
            broken = (broken + 1) % 4294967296
            if broken >= 2147483648 then broken = broken - 4294967296 end
            quest:SetStateBool("BarrelBrokenInstantaneous", false)
            if broken == 1 then
                addBadDeed(0)
            elseif broken == total - 1 then
                -- Retail puts the gold pickup into the final surviving barrel.
                local last_barrel = nil
                if quest.GetThingWithScriptName then
                    last_barrel = quest:GetThingWithScriptName("NOVI_Barrel")
                end
                if last_barrel ~= nil then
                    quest:AddItemToContainer(last_barrel, "OBJECT_GOLD_1")
                end
            elseif broken > total - 4 then
                resources:SpawnBarrelBeetle({
                    x = quest:GetStateFloat("BarrelBrokenPos_x"),
                    y = quest:GetStateFloat("BarrelBrokenPos_y"),
                    z = quest:GetStateFloat("BarrelBrokenPos_z"),
                })
            end
        end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
end

return processBarrelBreaks
