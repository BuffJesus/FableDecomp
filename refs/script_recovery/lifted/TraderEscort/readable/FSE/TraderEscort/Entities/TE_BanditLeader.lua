-- Readable native conversion: TE_BanditLeader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TE_BanditLeader.Main (retail 0x00e035e0)
function Main(quest, me)
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    while not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
        if not quest:NewScriptFrame(me) then return end
    end
    local conversationID = quest:AddNewConversation(me, false, false)
    quest:AddPersonToConversation(conversationID, hero)
    quest:AddLineToConversation(conversationID, "TEXT_QST_067_BANDIT_ATTACK", me, hero, false)
    quest:GiveThingBestEnemyTarget(me, hero)
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

