-- Readable native conversion: SpeedFriend. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- SpeedFriend.Main (retail 0x00d408b0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue
    if not quest:NewScriptFrame(me) then return end
    scratchValue = resources:NewResource()
    while not resources:TryAcquire(scratchValue, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d409c5 end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:SetIsPushableByHero(me, false)
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
    end
    ::LAB_00d409c5::
    resources:ReleaseResource(scratchValue)
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

