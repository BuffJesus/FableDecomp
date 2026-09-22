-- Readable native conversion: WaspAttacker. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- WaspAttacker.Main (retail 0x00e11450)
function Main(quest, me)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    local waspVictim = quest:GetThingWithScriptName("WaspVictim")
    quest:GiveThingBestEnemyTarget(me, waspVictim)
    while waspVictim ~= nil and waspVictim:IsAlive() do
        if not quest:NewScriptFrame(me) then goto LAB_00e1155e end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    end
    ::LAB_00e1155e::
    resources:ReleaseResource(resource)
end

-- WaspAttacker.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- WaspAttacker.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WaspAttacker.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

