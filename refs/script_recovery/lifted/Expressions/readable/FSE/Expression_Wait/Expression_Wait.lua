-- Readable native conversion: Expression_Wait. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Wait.Main (retail 0x00eea460)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame() then return end
    if not hero:AcquireControl(4) then return end
    local movie = resources:StartMovie("")
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    -- TODO(native): CScriptBase::KillSpawnedFunction((CScriptBase *)this,(int)&xStack_34,0);
    quest:DeactivateQuest("Expression_Follow", 0)
    quest:CameraDefault()
    quest:Pause(1.0)
    quest:FadeScreenIn()
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    resources:DestroyMovie(movie)
    hero:ReleaseControl()
end

-- Expression_Wait.Init (retail 0x00eea450)
function Init(quest)
end

-- Expression_Wait.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

