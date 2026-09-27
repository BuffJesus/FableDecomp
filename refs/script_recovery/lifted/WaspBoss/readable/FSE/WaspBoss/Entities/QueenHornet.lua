-- Readable native conversion: QueenHornet. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local droneDeadCount

-- QueenHornet.Main (retail 0x00e11bb0)
function Main(quest, me)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    quest:Pause(2.0)
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 0) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    resources:ReleaseResource(resource)
end

-- QueenHornet.Init (retail 0x00e11b80)
function Init(quest, me)
    droneDeadCount = 0
end

-- QueenHornet.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- QueenHornet.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

