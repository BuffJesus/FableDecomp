-- Generated native draft: Global_GiveHeroItemsFromRewardChest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local __native_condition_1, bVar2, bVar4, cVar3, delay, iVar6, p0, pCVar5, r1, r2, xStack_20
    local alive = true
    p0 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    bVar2 = false
    pCVar5 = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(pCVar5, bVar2)
    bVar2 = false
    r1 = quest:GetMostRecentValidUsedTarget()
    __native_condition_1 = (r1 ~= nil and not r1:IsNull())
    if __native_condition_1 then
        cVar3 = (r1 ~= nil and r1:IsAlive())
        __native_condition_1 = cVar3
    end
    if __native_condition_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00eec676 end
        if not (r1 ~= nil and not r1:IsNull()) then
            pCVar5 = ""
        else
            xStack_20 = r1:GetDefName()
        end
        iVar6 = ((pCVar5 == "OBJECT_CHEST_REWARD_ON_DEATH") and 0 or 1)
        cVar3 = not (iVar6 ~= 0)
        if cVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00eec676 end
            bVar2 = true
        end
        r2 = quest:GiveHeroItemsFromContainer(r1, false)
        quest:RemoveThing(r1, false, true)
    end
    bVar4 = quest:IsHeroControlledByPlayer()
    while not bVar4 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00eec676 end
        bVar4 = quest:IsHeroControlledByPlayer()
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00eec676 end
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00eec676 end
        bVar2 = quest:IsHeroChild()
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00eec676 end
            bVar2 = quest:DisplayTutorial(10)
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00eec676 end
                bVar2 = quest:MsgIsTutorialClickedPast()
                while not bVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00eec676 end
                    bVar2 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00eec676 end
            end
        end
    end
    delay = 0
    pCVar5 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar5, delay)
    ::LAB_00eec676::
end

function Init(quest)
end

function OnPersist(quest, context)
end

