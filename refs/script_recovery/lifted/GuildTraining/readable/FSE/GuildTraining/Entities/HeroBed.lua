-- Reviewed native slice for Q_GuildTraining.HeroBed.
-- Retail marks the quest binding and every matching bed persistent and unusable.

function Init(quest, me)
end

function Main(quest, me)
    quest:SetThingAsUsable(me, false)
    quest:SetThingPersistent(me, true)

    local function markBeds(definition)
        local beds = quest:GetAllThingsWithDefName(definition)
        for _, bed in ipairs(beds) do
            if quest:IsActiveThreadTerminating() then return false end
            quest:SetThingAsUsable(bed, false)
            quest:SetThingPersistent(bed, true)
        end
        return true
    end

    if not markBeds("OBJECT_GUILD_BED_FLOOR_PALLET_01") then return end
    if not markBeds("OBJECT_GUILD_BED_APPRENTICE_01") then return end
    markBeds("OBJECT_BS_SLUM_BED_BROWN_01")
end
