-- Structured PostAttackStuff; atomic lookup adapters retain native temporary scopes.
local function runPostAttack(quest, resources)
    while not resources:PostAttackStartIsAlive() do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CacheMusicSet(45)
    resources:TeleportToPostAttackStart()
    resources:SetPostAttackVillageLimbo(true)
    quest:DisplayMoneyBag(false)
    quest:TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE")
    quest:AddLogbookStoryEntry(20)
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:CameraResetToViewBehindHero(0.0)
    quest:CameraDefault()
    quest:CacheMusicSet(57)
    quest:FadeScreenIn()
    local trigger = resources:NewThingFromScriptName("MK_OVI_DADTRIGGER")
    local function finishPostAttack()
        while not resources:PostAttackHeroNearTrigger(trigger) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        playPostAttackDadCutscene(quest, resources)
        resources:SetPostAttackVillageLimbo(false)
        quest:SetTimeAsStopped(false)
        quest:DeactivateQuest("Q__OakValeIntro_PostAttack", 0)
        quest:ResetToDefaultTheme(0)
        quest:StopOverrideMusic(false)
    end
    finishPostAttack()
    resources:DestroyThing(trigger)
end

function PostAttackStuff(quest)
    quest:WithRetailResources(function(resources) runPostAttack(quest, resources) end)
end

return runPostAttack
