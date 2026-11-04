-- Readable native conversion: Expression_Follow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Follow.Main (retail 0x00eea020)
function Main(quest)
    return quest:WithRetailResources(function(resources)
        local controlAcquired
        local hero = quest:GetHero()
        quest:RegisterQuestConsciousCondition(hero)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
        local resource = resources:NewResource()
        local hero2 = quest:GetHero()
        controlAcquired = resources:TryAcquire(resource, hero2, 4)
        while not controlAcquired do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local hero3 = quest:GetHero()
            controlAcquired = resources:TryAcquire(resource, hero3, 4)
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        local movie = resources:StartMovie("")
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:CreateThread("FollowThread")  -- native thread body CExpression_FollowScript__FollowThread: lift it as function FollowThread(quest)
        quest:CameraDefault()
        quest:Pause(1.0)
        quest:FadeScreenIn()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end)
end

-- Expression_Follow.Init (retail 0x00ee9f70)
function Init(quest)
end

-- Expression_Follow.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Expression_Follow.FollowThread (retail 0x00eea240)
function FollowThread(quest)
    return quest:WithRetailResources(function(resources)
        local target = quest:GetHeroTargetedThing()
        local followTargetOwner = resources:TrackThingOutput(target)
        if target == nil or not (target ~= nil and target:IsAlive()) then
            if not quest:IsActiveThreadTerminating() then
                local pQuestName = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pQuestName, 0)
                resources:RetireThingOutput(followTargetOwner); followTargetOwner = 0
                return
            end
        elseif not quest:IsActiveThreadTerminating() then
            local resource = resources:NewResource()
            resources:TryAcquire(resource, target, 4)
            local hero = quest:GetHero()
            resources:FollowThing(resource, hero, 1.0, true)
            while not quest:IsActiveThreadTerminating() do
                quest:NewScriptFrame()
            end
            resources:ReleaseResource(resource)
            resources:RetireThingOutput(followTargetOwner); followTargetOwner = 0
            return
        end
        resources:RetireThingOutput(followTargetOwner); followTargetOwner = 0
    end)
end

