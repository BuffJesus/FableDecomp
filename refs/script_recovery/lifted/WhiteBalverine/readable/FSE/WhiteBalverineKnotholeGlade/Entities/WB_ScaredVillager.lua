-- Readable native conversion: WB_ScaredVillager. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- WB_ScaredVillager.Main (retail 0x00e16a80)
function Main(quest, me)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    while not quest:IsActiveThreadTerminating() do
        quest:NewScriptFrame(me)
    end
    resources:ReleaseResource(resource)
end

-- WB_ScaredVillager.Init (retail 0x00e16a40)
function Init(quest, me)
end

-- WB_ScaredVillager.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WB_ScaredVillager.OnPredicateFail (retail 0x00e16a50)
function OnPredicateFail(quest, me)
end

