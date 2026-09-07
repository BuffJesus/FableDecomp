function Init(quest, me)
    quest:Log("WaspVictim: Init() started. Setting scared state.")
    quest:EntitySetAsScared(me, true)
end

function Main(quest, me)
    quest:Log("WaspVictim: Main() started. Waiting for QuestStartScreened...")

    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    me:AcquireControl(4) -- SCRIPT_AI_PRIORITY_HIGHEST

    local curHealth = quest:GetHealth(me)
    if curHealth > 0 then
        quest:Log("WaspVictim: Adjusting health to 6.0.")
        quest:ModifyThingHealth(me, 6.0 - curHealth)
    end

    if quest:GetStateBool("QueenHornetAttacks") then
        quest:Log("WaspVictim: Queen Hornet already attacking. Removing entity.")
        quest:RemoveThing(me)
        me:ReleaseControl()
        return
    end

    local waspAttacker = quest:GetThingWithScriptName("WaspAttacker")
    local hero = quest:GetHero()
    local panicTimer = quest:RegisterTimer()
    quest:SetTimer(panicTimer, 0)

    quest:Log("WaspVictim: Panicking while under attack by WaspAttacker...")
    while waspAttacker and waspAttacker:IsAlive() and quest:GetHealth(waspAttacker) > 0 do
        if quest:GetTimer(panicTimer) <= 0 then
            -- WB_ScreamingVillagerScreamsDistance = 10
            if hero and quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) then
                quest:SetTimer(panicTimer, 7) -- WB_WaspChaseWomanPanicTime = 7
                quest:Log("WaspVictim: Screaming for help: TEXT_QST_072_VILLAGER_MALE_SCREAMS_10")
                local convoID = quest:StartAmbientConversation(me, hero, false, false)
                quest:AddLineToConversation(convoID, "TEXT_QST_072_VILLAGER_MALE_SCREAMS_10", me, hero, false)
            end
        end

        if not quest:NewScriptFrame(me) then
            quest:DeregisterTimer(panicTimer)
            me:ReleaseControl()
            return
        end
    end

    quest:DeregisterTimer(panicTimer)

    -- Saved by Hero
    quest:Log("WaspVictim: WaspAttacker killed! Speaking saved dialogue...")
    if hero then
        local convoID = quest:StartAmbientConversation(me, hero, false, false)
        quest:AddLineToConversation(convoID, "TEXT_QST_072_VILLAGER_MALE_ON_SAVED_10", me, hero, false)
    end
    quest:EntitySetAsScared(me, false)

    local savedCount = quest:GetStateInt("SavedVillagerCount") or 0
    if savedCount < 2 then
        quest:Log("WaspVictim: Awarding morality (0.01 WB_MoralityGain) and incrementing SavedVillagerCount.")
        quest:GiveHeroMorality(0.01)
        quest:SetStateInt("SavedVillagerCount", savedCount + 1)
    end

    local victimExit = quest:GetThingWithScriptName("VillagerEscapePos")
    if victimExit then
        quest:Log("WaspVictim: Running to VillagerEscapePos...")
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

    quest:Log("WaspVictim: Fading out (2.5s WB_ScreamingVillagerFadeOutTime) and killing entity.")
    quest:FadeOutAndKillEntity(me, true, 2.5, true)
    me:ReleaseControl()
end
