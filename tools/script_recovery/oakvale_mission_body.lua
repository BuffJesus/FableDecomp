function DoMission(quest)
    while not quest:IsRegionLoaded("StartOakVale") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end

    if not quest:GetStateBool("AttackOver") then
        if quest:IsActiveThreadTerminating() then return end
        quest:FadeScreenOutUntilNextCallToFadeScreenIn(2.0, 0.0)
        quest:WithRetailResources(function(resources)
            resources:TurnOakvaleHeroIntoChild()
        end)
        quest:CreateThread("WatchBarrels")
        quest:CreateThread("WatchForGotGold")
        quest:CreateThread("ManageQuestCoreMarkers")
        quest:CacheMusicSet(46)
        quest:ActivateQuest("Q_NewOakValeIntro_PreAttack")
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end

        quest:WithRetailResources(function(resources)
            resources:SetOakvaleHeroKillable(false)
        end)
        quest:SetTimeAsStopped(true)
        quest:SetTimeOfDay(12.0)
        quest:SetHeroSleepingAsEnabled(false)
        quest:DisplayMoneyBag(true)
        quest:WithRetailResources(function(resources)
            resources:PrepareOakvaleHouseAndStartScreen()
        end)
        while not quest:GetStateBool("AttackOver") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
    end

    if quest:IsActiveThreadTerminating() then return end
    AttackStuff(quest)
    PostAttackStuff(quest)
    -- Retail continues this tail when PostAttackStuff returns after cancellation.
    quest:FadeScreenOutUntilNextCallToFadeScreenIn(0.5, 0.0)
    quest:WithRetailResources(function(resources)
        resources:SetOakvaleHeroKillable(true)
    end)
    quest:SetHeroSleepingAsEnabled(true)
    quest:WithRetailResources(function(resources)
        resources:FinishOakvaleActiveQuest(false)
        resources:FinishOakvaleActiveQuest(true)
    end)
end
