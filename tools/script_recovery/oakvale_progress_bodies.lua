function AttackStuff(quest)
    quest:ActivateQuest("Q__OakValeIntro_PostAttack")
    quest:DeactivateQuest("Q_NewOakValeIntro_PreAttack", 0)
    quest:SetTimeOfDay(23.0)
    quest:TransitionToTheme("ENVIRONMENT_OV_POSTATTACK", 0.0)
    quest:WithRetailResources(function(resources)
        resources:SetOakvaleProgressObjective(false)
    end)
end

function WatchForGotGold(quest)
    while quest:GetHeroGold() <= 2 do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        resources:SetOakvaleProgressObjective(true)
    end)
end
