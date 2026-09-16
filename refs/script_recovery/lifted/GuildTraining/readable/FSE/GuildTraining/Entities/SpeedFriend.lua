-- Reviewed native slice for Q_GuildTraining.SpeedFriend.
-- The temporary mesh/resource probe in retail has no script-visible result.

function Init(quest, me)
end

function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    if quest:IsActiveThreadTerminating() then return end

    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then return end
        if quest:IsActiveThreadTerminating() then return end
    end

    if quest:IsActiveThreadTerminating() then return end
    quest:SetIsPushableByHero(me, false)
    while quest:NewScriptFrame(me) do
        if quest:IsActiveThreadTerminating() then return end
    end
end
