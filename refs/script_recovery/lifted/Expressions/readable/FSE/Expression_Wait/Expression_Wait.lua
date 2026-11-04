-- Readable native conversion: Expression_Wait. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Wait.Main (retail 0x00eea460)
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
        quest:KillSpawnedFunction("FollowThread", 0)
        quest:DeactivateQuest("Expression_Follow", 0)
        quest:CameraDefault()
        quest:Pause(1.0)
        quest:FadeScreenIn()
        local pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end)
end

-- Expression_Wait.Init (retail 0x00eea450)
function Init(quest)
end

-- Expression_Wait.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

