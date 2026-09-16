"""Native EC42F0 control flow, with explicit pending capability boundaries."""
BODY='''    quest:RegisterBoundAliveCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateBool("RockTrollTriggered") then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local spawnMkr = resources:NewThingFromScriptName("M_RTFERockTrollSpawnPos")
        local function heroIsNear()
            local proximity = quest:GetRockTrollTriggerProximity()
            local hero = quest:GetHero()
            return quest:IsDistanceBetweenThingsUnder(hero, me, proximity)
        end
        while not heroIsNear() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                resources:DestroyThing(spawnMkr)
                return
            end
        end
        if quest:IsActiveThreadTerminating() then
            resources:DestroyThing(spawnMkr)
            return
        end
        resources:CreateCreatureAtThingPosition(spawnMkr, "CREATURE_ROCK_TROLL_START_STANDING", "RTFE_RockTroll", false)
        quest:SetStateBool("RockTrollTriggered", true)
        quest:RemoveThing(me, false, true)
        resources:DestroyThing(spawnMkr)
    end)
'''
