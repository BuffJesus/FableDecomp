"""Retail Sparrow Main EC46B0: condition, acquisition and TrollAwake wait only."""
BODY='''    quest:RegisterBoundAliveCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local seh_me = resources:NewResource()
        resources:PrepareResource(seh_me)
        while not resources:TryAcquire(seh_me, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        while not quest:GetStateBool("TrollAwake") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        quest:IsActiveThreadTerminating()
    end)
'''
