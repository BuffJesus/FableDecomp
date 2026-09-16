-- Retail Guild race marker. ReachedPlatform is parent quest state.
function Init(quest, me)
end

function Main(quest, me)
    while not quest:IsActiveThreadTerminating() do
        local hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(hero, me, 3.0) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("ReachedPlatform", true)
            quest:MiniMapRemoveMarker(me)
        end
        quest:NewScriptFrame(me)
    end
end
