-- Readable native conversion: MeleeThunder. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- MeleeThunder.Main (retail 0x00d58080)
function Main(quest, me)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    while quest:GetStateInt("TutorialState") ~= 4 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d582ed end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d582ed end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d582ed end
    while quest:GetStateInt("TutorialState") == 4 do
        if not quest:NewScriptFrame(me) then goto LAB_00d582ed end
    end
    resources:PrepareResource(resource)
    while quest:GetStateInt("TutorialState") ~= 6 do
        if not quest:NewScriptFrame(me) then goto LAB_00d58297 end
    end
    if not quest:IsActiveThreadTerminating() then
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00d58297 end
        end
        if not quest:IsActiveThreadTerminating() then
            while quest:GetStateInt("TutorialState") == 6 do
                if not quest:NewScriptFrame(me) then goto LAB_00d58297 end
            end
            if not quest:IsActiveThreadTerminating() then
                resources:PrepareResource(resource)
                repeat
                    quest:NewScriptFrame(me)
                until quest:IsActiveThreadTerminating()
            end
        end
    end
    ::LAB_00d58297::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d582ed::
    resources:ReleaseResource(resource)
end

-- MeleeThunder.Init (retail 0x00d58050)
function Init(quest, me)
end

-- MeleeThunder.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- MeleeThunder.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

