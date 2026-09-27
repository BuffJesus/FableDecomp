-- Readable native conversion: BordelloEntrance. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- BordelloEntrance.Main (retail 0x00e3b4a0)
function Main(quest, me)
    quest:EntitySetAsLocked(me, true)
    local isQuestCompleted = quest:IsQuestCompleted("Q_TraderEscort")
    repeat
        if isQuestCompleted then
            if quest:IsActiveThreadTerminating() then return end
            if not quest:GetStateBool("BecomeNunnery") then
                quest:SetReadableObjectTextTag(quest:GetThingWithScriptName("BordelloSign"), "TEXT_QST_B13_BORDELLO_OPEN")
                quest:EntitySetAsLocked(me, false)
            else
                quest:SetReadableObjectTextTag(quest:GetThingWithScriptName("BordelloSign"), "TEXT_QST_B13_BORDELLO_REFUGE")
                quest:EntitySetAsLocked(me, false)
            end
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
            return
        end
        if not quest:NewScriptFrame(me) then return end
        if not me:MsgIsUsedByHero() then
            isQuestCompleted = quest:IsQuestCompleted("Q_TraderEscort")
        else
            quest:DisplayGameInfo("TEXT_QST_B13_BORDELLO_CLOSED")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            if not quest:NewScriptFrame(me) then return end
            isQuestCompleted = quest:IsQuestCompleted("Q_TraderEscort")
        end
    until false
end

-- BordelloEntrance.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- BordelloEntrance.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BordelloEntrance.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

