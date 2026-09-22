-- Generated native draft: QS_GuardianSisterInfo2_SisterInBanditCamp. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local b2, b3, bVar5, cVar3, delay, pCVar4, pQuestName, r1
    local alive = true
    quest:AddEntityBinding("MazeAtTavern", "QS_GuardianSisterInfo2_SisterInBanditCamp/Entities/MazeAtTavern")
    quest:FinalizeEntityBindings()
    pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01", "", "OakBay")
    cVar3 = quest:GetStateBool("GuardianSpokeToHero")
    while not cVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        cVar3 = quest:GetStateBool("GuardianSpokeToHero")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        r1 = quest:GetThingWithScriptName("MazeAtTavern")
        cVar3 = (r1 ~= nil and r1:IsAlive())
        while cVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e26aca end
            cVar3 = (r1 ~= nil and r1:IsAlive())
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            b3 = false
            b2 = false
            bVar5 = false
            pCVar4 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar4, bVar5, b2, b3)
            delay = 0
            pCVar4 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar4, delay)
        end
        ::LAB_00e26aca::
    end
end

function Init(quest)
    quest:SetStateBool("GuardianSpokeToHero", false)
end

function OnPersist(quest, context)
    local guardianSpokeToHero = quest:GetStateBool("GuardianSpokeToHero") or false
    guardianSpokeToHero = quest:PersistTransferBool(context, "GuardianSpokeToHero", guardianSpokeToHero)
    quest:SetStateBool("GuardianSpokeToHero", guardianSpokeToHero)
end

