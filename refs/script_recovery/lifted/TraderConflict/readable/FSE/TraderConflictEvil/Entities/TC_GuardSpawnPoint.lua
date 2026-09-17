-- Readable native conversion: TC_GuardSpawnPoint. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_GuardSpawnPoint.Main (retail 0x00df7bf0)
function Main(quest, me)
    local self_0x14
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    if not quest:IsActiveThreadTerminating() and not quest:IsActiveThreadTerminating() then
        while true do
            if not (25 - quest:GetStateInt("InitialNumberInRegion") ~= quest:GetStateInt("NumberSpawned")) then break end
            -- TODO(native): if ((((*(self_0x14 + 0x4c) - *(self_0x14 + 0x48)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 0x19 < quest:GetStateInt("NextTimeToSpawnGuards")) or (((*(self_0x14 + 0x4c) - *(self_0x14 + 0x48)) / 0xc) < 7) then
            if not quest:NewScriptFrame(me) then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        return
    end
end

-- TC_GuardSpawnPoint.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_GuardSpawnPoint.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_GuardSpawnPoint.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

