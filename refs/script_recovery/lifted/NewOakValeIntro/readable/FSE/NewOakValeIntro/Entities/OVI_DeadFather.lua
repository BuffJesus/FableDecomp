-- Disabled complete DeadFather lifecycle; no quest registration.
function DeadFatherInit(quest, me)
    quest:WithRetailResources(function(resources)
        resources:InitializeDeadFatherActor(me)
    end)
end

function DeadFatherMain(quest, me)
    quest:RegisterBoundAliveCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:PlaceDeadFatherAtMarker(me)
        resources:PlayDeadFatherPose(control)
        while not quest:GetStateBool("DadFound") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:RemoveDeadFatherMarker(me)
        repeat quest:NewScriptFrame() until quest:IsActiveThreadTerminating()
    end)
end

function DeadFatherOnPredicateFail(quest, me)
end

function Init(quest, me) DeadFatherInit(quest, me) end
function Main(quest, me) DeadFatherMain(quest, me) end
function OnPredicateFail(quest, me) DeadFatherOnPredicateFail(quest, me) end
