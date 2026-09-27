-- Readable native conversion: TC_BanditHostageKeeper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TC_BanditHostageKeeper.Main (retail 0x00dfba60)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 2) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    while not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if math.random(0, 32767) % 5 == 0 then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        local conversationID = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationID, hero)
        if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
            quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, hero, false)
        elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
            quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
        else
            quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
        end
        if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
            quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
        else
            quest:SetStateInt("BanditSecurityLinesSaid", 0)
        end
    end
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    resources:ReleaseResource(resource)
end

-- TC_BanditHostageKeeper.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_BanditHostageKeeper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TC_BanditHostageKeeper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

