-- Readable native conversion: QS_GuardianTrophyDealerInfo. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- QS_GuardianTrophyDealerInfo.Main (retail 0x00e27920)
function Main(quest)
    quest:AddEntityBinding("GTDI_Maze", "QS_GuardianTrophyDealerInfo/Entities/GTDI_Maze")
    quest:FinalizeEntityBindings()
    quest:CreateThread("WaitForPieceOver")  -- native thread body NScript::CQ_GuildTrainingScript::WatchForSparrowKilled: lift it as function WaitForPieceOver(quest)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01", "HeroGuildComplexInside", "HeroGuildComplexInside")
end

-- QS_GuardianTrophyDealerInfo.Init (retail 0x00e27870)
function Init(quest)
    quest:SetStateBool("PieceOver", false)
end

-- QS_GuardianTrophyDealerInfo.OnPersist (retail 0x00e27e60)
function OnPersist(quest, context)
    quest:SetStateBool("PieceOver", quest:PersistTransferBool(context, "PieceOver", quest:GetStateBool("PieceOver")))
end

-- QS_GuardianTrophyDealerInfo.WaitForPieceOver (retail 0x00e27ac0)
function WaitForPieceOver(quest)
    while not quest:GetStateBool("PieceOver") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local maze = quest:GetThingWithScriptName("GTDI_Maze")
    while true do
        local scratchValue = (maze ~= nil and not maze:IsNull()) and (maze ~= nil and maze:IsAlive())
        if not scratchValue then break end
        if not quest:NewScriptFrame() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    do return end
end

