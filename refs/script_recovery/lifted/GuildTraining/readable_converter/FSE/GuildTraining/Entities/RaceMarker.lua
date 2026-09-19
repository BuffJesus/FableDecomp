-- Readable native conversion: RaceMarker. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- RaceMarker.Main (retail 0x00d40aa0)
function Main(quest, me)
    while not quest:IsActiveThreadTerminating() do
        if not quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 3.0) then
            quest:NewScriptFrame(me)
        else
            quest:SetStateBool("ReachedPlatform", true)
            quest:MiniMapRemoveMarker(me)
            quest:NewScriptFrame(me)
        end
    end
end

-- RaceMarker.Init (retail 0x00d40a90)
function Init(quest, me)
end

-- RaceMarker.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- RaceMarker.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

