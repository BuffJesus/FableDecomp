-- Readable native conversion: Global_GiveHeroItemsFromRewardChest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local TUTORIAL_CATEGORY_CREATURE_DROP = 10  -- ETutorialCategory (Ego_r.pdb)

-- Global_GiveHeroItemsFromRewardChest.Main (retail 0x00eec410)
function Main(quest)
    local predicateResult, delay, getActiveQuestName2, getDefName
    quest:GetHero()
    if not quest:NewScriptFrame() then return end
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(getActiveQuestName, false)
    predicateResult = false
    local getMostRecentValidUsedTarget = quest:GetMostRecentValidUsedTarget()
    local scratchValue = getMostRecentValidUsedTarget ~= nil and (getMostRecentValidUsedTarget ~= nil and getMostRecentValidUsedTarget:IsAlive())
    if scratchValue then
        if quest:IsActiveThreadTerminating() then return end
        if not (getMostRecentValidUsedTarget ~= nil and not getMostRecentValidUsedTarget:IsNull()) then
            getDefName = ""
        else
            getDefName = getMostRecentValidUsedTarget:GetDefName()
        end
        if getDefName == "OBJECT_CHEST_REWARD_ON_DEATH" then
            if quest:IsActiveThreadTerminating() then return end
            predicateResult = true
        end
        quest:GiveHeroItemsFromContainer(getMostRecentValidUsedTarget, false)
        quest:RemoveThing(getMostRecentValidUsedTarget, false, true)
    end
    while not quest:IsHeroControlledByPlayer() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not predicateResult then delay = 0; getActiveQuestName2 = quest:GetActiveQuestName(); quest:DeactivateQuestLater(getActiveQuestName2, 0); goto LAB_00eec676 end
    if quest:IsActiveThreadTerminating() then return end
    if quest:IsHeroChild() then delay = 0; getActiveQuestName2 = quest:GetActiveQuestName(); quest:DeactivateQuestLater(getActiveQuestName2, 0); goto LAB_00eec676 end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:DisplayTutorial(TUTORIAL_CATEGORY_CREATURE_DROP) then delay = 0; getActiveQuestName2 = quest:GetActiveQuestName(); quest:DeactivateQuestLater(getActiveQuestName2, 0); goto LAB_00eec676 end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:MsgIsTutorialClickedPast() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    delay = 0
    getActiveQuestName2 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName2, 0)
    ::LAB_00eec676::
end

-- Global_GiveHeroItemsFromRewardChest.Init (retail 0x00eec400)
function Init(quest)
end

-- Global_GiveHeroItemsFromRewardChest.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

