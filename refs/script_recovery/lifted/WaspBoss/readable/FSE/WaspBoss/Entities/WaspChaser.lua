-- Readable native conversion: WaspChaser. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- WaspChaser.Main (retail 0x00e10bf0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local waspChaseWoman
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e10d61 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e10d61 end
    waspChaseWoman = quest:GetThingWithScriptName("WaspChaseWoman")
    me:FollowThing(waspChaseWoman, 1.0, true)
    while not quest:IsActiveThreadTerminating() do
        if waspChaseWoman ~= nil and waspChaseWoman:IsAlive() then
            quest:NewScriptFrame(me)
        else
            if false ~= 0 then
                resources:PrepareResource(resource)
            end
            break
            quest:NewScriptFrame(me)
        end
    end
    ::LAB_00e10d61::
    resources:ReleaseResource(resource)
end

-- WaspChaser.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- WaspChaser.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WaspChaser.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

