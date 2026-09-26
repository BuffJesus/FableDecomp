-- Readable native conversion: WB_Villager. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local done

-- WB_Villager.Main (retail 0x00e16c40)
function Main(quest, me)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    while not quest:IsActiveThreadTerminating() do
        quest:NewScriptFrame(me)
    end
    resources:ReleaseResource(resource)
end

-- WB_Villager.Init (retail 0x00e16c00)
function Init(quest, me)
    done = false
end

-- WB_Villager.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WB_Villager.OnPredicateFail (retail 0x00e16c10)
function OnPredicateFail(quest, me)
end

