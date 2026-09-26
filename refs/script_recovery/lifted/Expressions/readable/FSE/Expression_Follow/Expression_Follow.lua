-- Readable native conversion: Expression_Follow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Follow.Main (retail 0x00eea020)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame() then return end
    if not hero:AcquireControl(4) then hero:ReleaseControl(); return end
    local movie = resources:StartMovie("")
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:CreateThread("FollowThread")  -- native thread body CExpression_FollowScript__FollowThread: lift it as function FollowThread(quest)
    quest:CameraDefault()
    quest:Pause(1.0)
    quest:FadeScreenIn()
    resources:DestroyMovie(movie)
    hero:ReleaseControl()
end

-- Expression_Follow.Init (retail 0x00ee9f70)
function Init(quest)
end

-- Expression_Follow.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Expression_Follow.FollowThread (retail 0x00eea240)
function FollowThread(quest)
    local sequence, scratchValue
    local getHeroTargetedThing = quest:GetHeroTargetedThing()
    sequence = scratchValue == nil
    if not sequence then
        -- TODO(native): cVar2 = (**(*piStack_18 + 0x12c))()
    --[[unresolved native value]]
        sequence = not nil
    end
    if sequence then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        return
    else
        if quest:IsActiveThreadTerminating() then return end
        getHeroTargetedThing:AcquireControl(4)
        while not quest:IsActiveThreadTerminating() do
            quest:NewScriptFrame()
        end
        getHeroTargetedThing:ReleaseControl()
        return
    end
end

