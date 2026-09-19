-- Readable native conversion: KillBird. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- KillBird.Main (retail 0x00d43190)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    if me:AcquireControl(4) then
        while true do
            if quest:IsActiveThreadTerminating() then break end
            quest:NewScriptFrame(me)
    end
    end
    me:ReleaseControl()
end

-- KillBird.Init (retail 0x00d430f0)
function Init(quest, me)
    quest:EntitySetThingAsEnemyOfThing(me, quest:GetHero())
end

-- KillBird.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- KillBird.OnPredicateFail (retail 0x00d43120)
function OnPredicateFail(quest, me)
    if not me:MsgIsKilledBy("") then return end
    quest:SetStateBool("DisplayBirdKilledMessage", true)
    quest:SetStateInt("CurrentBirdsKilled", quest:GetStateInt("CurrentBirdsKilled") + 1)
end

