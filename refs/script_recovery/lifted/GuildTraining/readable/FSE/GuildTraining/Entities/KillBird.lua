-- Reviewed native slice for Q_GuildTraining.KillBird.

function Init(quest, me)
    quest:EntitySetThingAsEnemyOfThing(me, quest:GetHero())
end

function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    if quest:IsActiveThreadTerminating() then return end
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then return end
        if quest:IsActiveThreadTerminating() then return end
    end
    while quest:NewScriptFrame(me) do
        if quest:IsActiveThreadTerminating() then return end
    end
end
