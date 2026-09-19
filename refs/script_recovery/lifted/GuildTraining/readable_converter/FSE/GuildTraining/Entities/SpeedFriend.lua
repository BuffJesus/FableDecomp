-- Readable native conversion: SpeedFriend. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- SpeedFriend.Main (retail 0x00d408b0)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    if not me:AcquireControl(4) then goto LAB_00d409c5 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d409c5 end
    quest:SetIsPushableByHero(me, false)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::LAB_00d409c5::
    me:ReleaseControl()
end

-- SpeedFriend.Init (retail 0x00d40870)
function Init(quest, me)
end

-- SpeedFriend.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- SpeedFriend.OnPredicateFail (retail 0x00d40880)
function OnPredicateFail(quest, me)
end

