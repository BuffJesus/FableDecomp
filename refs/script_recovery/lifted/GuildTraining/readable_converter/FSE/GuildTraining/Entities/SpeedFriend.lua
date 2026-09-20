-- Readable native conversion: SpeedFriend. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- SpeedFriend.Main (retail 0x00d408b0)
function Main(quest, me)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d409c5 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d409c5 end
    quest:SetIsPushableByHero(me, false)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::LAB_00d409c5::
    resources:ReleaseResource(resource)
end

-- SpeedFriend.Init (retail 0x00d40870)
function Init(quest, me)
end

-- SpeedFriend.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SpeedFriend.OnPredicateFail (retail 0x00d40880)
function OnPredicateFail(quest, me)
end

