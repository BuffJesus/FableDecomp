-- Readable native conversion: BCMTrader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- BCMTrader.Main (retail 0x00dfbda0)
function Main(quest, me)
    quest:RemoveThing(me, false, false)
end

-- BCMTrader.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- BCMTrader.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- BCMTrader.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

