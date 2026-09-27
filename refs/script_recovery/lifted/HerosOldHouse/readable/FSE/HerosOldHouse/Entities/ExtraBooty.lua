-- Readable native conversion: ExtraBooty. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- ExtraBooty.Main (retail 0x00d8a8a0)
function Main(quest, me)
    while quest:IsDiggingSpotEnabled(me) do
        if not quest:NewScriptFrame(me) then return end
    end
    local predicateResult = not quest:IsActiveThreadTerminating()
    if predicateResult then
        quest:SetStateBool("BootyDugUp", true)
        predicateResult = not quest:GetStateBool("WifeAttacked")
    end
    if predicateResult and not quest:IsActiveThreadTerminating() then
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
    end
end

-- ExtraBooty.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- ExtraBooty.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ExtraBooty.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

