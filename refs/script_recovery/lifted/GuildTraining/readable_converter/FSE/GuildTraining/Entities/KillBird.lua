-- Readable native conversion: KillBird. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- KillBird.Main (retail 0x00d43190)
function Main(quest, me)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    while true do
        if quest:IsActiveThreadTerminating() then break end
        quest:NewScriptFrame(me)
    end
    resources:ReleaseResource(resource)
end

-- KillBird.Init (retail 0x00d430f0)
function Init(quest, me)
    quest:EntitySetThingAsEnemyOfThing(me, quest:GetHero())
end

-- KillBird.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- KillBird.OnPredicateFail (retail 0x00d43120)
function OnPredicateFail(quest, me)
    if not me:MsgIsKilledBy("") then return end
    quest:SetStateBool("DisplayBirdKilledMessage", true)
    quest:SetStateInt("CurrentBirdsKilled", quest:GetStateInt("CurrentBirdsKilled") + 1)
end

