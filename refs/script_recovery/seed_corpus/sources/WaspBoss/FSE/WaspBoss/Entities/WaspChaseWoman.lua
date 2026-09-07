function Init(quest, me)
    quest:Log("WaspChaseWoman: Init() started. Setting scared state.")
    quest:EntitySetAsScared(me, true)
end

function Main(quest, me)
    quest:Log("WaspChaseWoman: Main() started.")
    me:AcquireControl(4) -- SCRIPT_AI_PRIORITY_HIGHEST

    if quest:GetStateBool("QueenHornetAttacks") then
        quest:Log("WaspChaseWoman: Queen Hornet already attacking. Removing entity.")
        quest:RemoveThing(me)
        me:ReleaseControl()
        return
    end

    quest:Log("WaspChaseWoman: Waiting for StartChase signal...")
    while not quest:GetStateBool("StartChase") do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    local waspChaser = quest:GetThingWithScriptName("WaspChaser")
    local navIndex = 2

    local navPoints = {}
    for i = 0, 5 do
        navPoints[i] = quest:GetThingWithScriptName("ChasedWomanNav" .. i)
    end

    local destNav = navPoints[3]
    local startNav = navPoints[2]
    if startNav then
        quest:Log("WaspChaseWoman: Starting route from ChasedWomanNav2 (avoidObstacles=false, ignorePathPref=true).")
        me:FollowPreCalculatedRoute(startNav, 1, false, true)
    end

    quest:Log("WaspChaseWoman: Running route while chased by WaspChaser...")
    local debugFrame = 0
    while waspChaser and waspChaser:IsAlive() and quest:GetHealth(waspChaser) > 0 do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end

        debugFrame = debugFrame + 1
        local taskActive = me:IsPerformingScriptTask()
        local reachedDest = destNav and quest:IsDistanceBetweenThingsUnder(me, destNav, 2.0)

        if debugFrame % 30 == 0 then
            quest:Log(string.format("WaspChaseWoman: Frame %d, taskActive=%s, reachedDest=%s, navIndex=%d", debugFrame, tostring(taskActive), tostring(reachedDest), navIndex))
        end

        if not taskActive or reachedDest then
            navIndex = (navIndex + 1) % 6
            local nextIndex = (navIndex + 1) % 6
            destNav = navPoints[nextIndex]
            local currentNav = navPoints[navIndex]
            me:ClearCommands()
            if currentNav then
                me:FollowPreCalculatedRoute(currentNav, 1, false, true)
            end
        end
    end

    -- Saved by Hero
    quest:Log("WaspChaseWoman: WaspChaser killed! Speaking saved dialogue...")
    quest:EntitySetAsScared(me, false)
    local hero = quest:GetHero()
    if hero then
        local convoID = quest:StartAmbientConversation(me, hero, false, false)
        quest:AddLineToConversation(convoID, "TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10", me, hero, false)
    end

    local savedCount = quest:GetStateInt("SavedVillagerCount") or 0
    if savedCount < 2 then
        quest:Log("WaspChaseWoman: Awarding morality (0.01 WB_MoralityGain) and incrementing SavedVillagerCount.")
        quest:GiveHeroMorality(0.01)
        quest:SetStateInt("SavedVillagerCount", savedCount + 1)
    end

    local victimExit = quest:GetThingWithScriptName("VillagerEscapePos")
    if victimExit then
        quest:Log("WaspChaseWoman: Running to VillagerEscapePos...")
        while not quest:IsDistanceBetweenThingsUnder(me, victimExit, 2.0) do
            if not me:IsPerformingScriptTask() then
                me:MoveToThing(victimExit, 1.0, 1)
            end
            if not quest:NewScriptFrame(me) then
                me:ReleaseControl()
                return
            end
        end
    end

    quest:Log("WaspChaseWoman: Fading out (2.5s WB_ScreamingVillagerFadeOutTime) and killing entity.")
    quest:FadeOutAndKillEntity(me, true, 2.5, true)
    me:ReleaseControl()
end
