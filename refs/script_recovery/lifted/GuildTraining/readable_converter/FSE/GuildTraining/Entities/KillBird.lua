-- Readable native conversion: KillBird. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- KillBird.Main (retail 0x00d43190)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue
    if not quest:NewScriptFrame(me) then return end
    scratchValue = resources:NewResource()
    while not resources:TryAcquire(scratchValue, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d43263 end
    end
    while true do
        if quest:IsActiveThreadTerminating() then break end
        quest:NewScriptFrame(me)
    end
    ::LAB_00d43263::
    resources:ReleaseResource(scratchValue)
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
    if me:MsgIsKilledBy("") then
        quest:SetStateBool("DisplayBirdKilledMessage", true)
        -- TODO(native): *piVar1 = *piVar1 + 1;
    end
end

