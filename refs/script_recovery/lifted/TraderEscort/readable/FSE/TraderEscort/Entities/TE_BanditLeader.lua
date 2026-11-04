-- Readable native conversion: TE_BanditLeader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TE_BanditLeader.Main (retail 0x00e035e0)
function Main(quest, me)
    quest:NewScriptFrame(me)
    local isDistanceBetweenThingsUnder
    if quest:IsActiveThreadTerminating() then return end
    local hero = quest:GetHero()
    isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 15.0)
    while not isDistanceBetweenThingsUnder do
        if not quest:NewScriptFrame(me) then return end
        local hero2 = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero2, 15.0)
    end
    if quest:IsActiveThreadTerminating() then return end
    local conversationID = quest:AddNewConversation(me, false, false)
    local hero3 = quest:GetHero()
    quest:AddPersonToConversation(conversationID, hero3)
    local hero4 = quest:GetHero()
    quest:AddLineToConversation(conversationID, "TEXT_QST_067_BANDIT_ATTACK", me, hero4, false)
    local hero5 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, hero5)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
end

-- TE_BanditLeader.Init (retail 0x00e035b0)
function Init(quest, me)
end

-- TE_BanditLeader.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TE_BanditLeader.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

