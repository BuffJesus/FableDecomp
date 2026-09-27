-- Readable native conversion: SickChildFishingSpot. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- SickChildFishingSpot.Main (retail 0x00ecd530)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    while true do
        if not quest:IsFishingSpotEnabled(me) then break end
        if not quest:NewScriptFrame(me) then return end
    end
    quest:SetStateBool("GotFishingSpotMushroom", true)
end

-- SickChildFishingSpot.Init (retail 0x00ecd500)
function Init(quest, me)
end

-- SickChildFishingSpot.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SickChildFishingSpot.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

