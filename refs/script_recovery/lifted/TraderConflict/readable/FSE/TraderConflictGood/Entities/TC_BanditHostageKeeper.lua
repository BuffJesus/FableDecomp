-- Readable native conversion: TC_BanditHostageKeeper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TC_BanditHostageKeeper.Main (retail 0x00dfba60)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isDistanceBetweenThingsUnder
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 2) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local hero = quest:GetHero()
    isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 15.0)
    while not isDistanceBetweenThingsUnder do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        local hero2 = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero2, 15.0)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if quest:RetailRandModulo(32768) % 5 == 0 then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        local conversationID = quest:AddNewConversation(me, false, false)
        local hero3 = quest:GetHero()
        quest:AddPersonToConversation(conversationID, hero3)
        if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
            local hero4 = quest:GetHero()
            quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, hero4, false)
        elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
            local hero5 = quest:GetHero()
            quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero5, false)
        else
            local hero6 = quest:GetHero()
            quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero6, false)
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

