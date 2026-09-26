-- Generated native draft: QS_GuardianTrophyDealerInfo. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar2
    quest:AddEntityBinding("GTDI_Maze", "QS_GuardianTrophyDealerInfo/Entities/GTDI_Maze")
    quest:FinalizeEntityBindings()
    quest:CreateThread("WaitForPieceOver")  -- native thread body NScript::CQ_GuildTrainingScript::WatchForSparrowKilled: lift it as function WaitForPieceOver(quest)
    if not bVar2 then
    end
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01", "HeroGuildComplexInside", "HeroGuildComplexInside")
end

function Init(quest)
    quest:SetStateBool("PieceOver", false)
end

function OnPersist(quest, context)
    local pieceOver = quest:GetStateBool("PieceOver") or false
    pieceOver = quest:PersistTransferBool(context, "PieceOver", pieceOver)
    quest:SetStateBool("PieceOver", pieceOver)
end

function WaitForPieceOver(quest)
    local bVar3, pQuestName
    local alive = true
    local CVar1 = quest:GetStateBool("PieceOver")
    while not CVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        CVar1 = quest:GetStateBool("PieceOver")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    local r1 = quest:GetThingWithScriptName("GTDI_Maze")
    local cVar4
    while true do
        local __native_condition_1 = (r1 ~= nil and not r1:IsNull())
        if __native_condition_1 then
            cVar4 = (r1 ~= nil and r1:IsAlive())
            __native_condition_1 = cVar4
        end
        if not __native_condition_1 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00e27b57: (native jump target)
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
        return
    end
    return
end

