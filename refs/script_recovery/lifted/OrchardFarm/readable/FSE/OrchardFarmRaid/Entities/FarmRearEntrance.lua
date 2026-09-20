-- Readable native conversion: FarmRearEntrance. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- FarmRearEntrance.Main (retail 0x00dcf4c0)
function Main(quest, me)
    quest:NewScriptFrame(me)
    if not ((not quest:IsActiveThreadTerminating()) and quest:GetStateBool("DoneIntroduction")) then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveThing(me, true, true)
end

-- FarmRearEntrance.Init (retail 0x00dcf480)
function Init(quest, me)
    quest:EntitySetTargetable(me, false)
end

-- FarmRearEntrance.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- FarmRearEntrance.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

