-- Generated native draft: Q_HerosOldHouse. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar3, bVar4, cVar1, delay, pCVar6, pQuestName, r1
    local alive = true
    bVar3 = quest:IsRegionLoaded("OakBay")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsRegionLoaded("OakBay")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:AddEntityBinding("GhostFisherman", "HerosOldHouse/Entities/GhostFisherman", 1)
        quest:AddEntityBinding("FishermansWife", "HerosOldHouse/Entities/FishermansWife", 1)
        quest:AddEntityBinding("ExtraBooty", "HerosOldHouse/Entities/ExtraBooty", 1)
        quest:FinalizeEntityBindings()
        quest:CreateThread("WatchForBooty")  -- native thread body NScript::CQ_HerosOldHouseScript::WatchForBooty: lift it as function WatchForBooty(quest)
        quest:SetStateBool("MissionAborted", false)
        r1 = quest:GetThingWithScriptName("FishermansWife")
        bVar3 = false
        cVar1 = quest:GetStateBool("BootyDugUp")
        while not cVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d8a220 end
            if bVar3 then
                goto LAB_00d8a156
            else
                bVar4 = quest:IsRegionLoaded("OakBay")
                if (not bVar4) or (not quest:GetStateBool("WifeAttacked")) then goto LAB_00d8a156 end
            end
            goto FLOW_past_lab_00d8a156
            ::LAB_00d8a156::
            bVar4 = false
            ::FLOW_past_lab_00d8a156::
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d8a220 end
                if quest:GetStateBool("Helping") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d8a220 end
                    bVar4 = true
                    bVar3 = false
                    pQuestName = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pQuestName, bVar3, "", bVar4)
                    bVar3 = true
                    quest:SetStateBool("Helping", false)
                end
            end
            cVar1 = quest:GetStateBool("BootyDugUp")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            delay = 0
            pCVar6 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar6, delay)
        end
        ::LAB_00d8a220::
    end
end

function Init(quest)
    quest:SetStateBool("Helping", false)
    quest:SetStateBool("Helped", false)
    quest:SetStateBool("WifeAttacked", false)
    quest:SetStateBool("WifeAttackedAndLeft", false)
    quest:SetStateBool("BootyDugUp", false)
end

function OnPersist(quest, context)
    local helping = quest:GetStateBool("Helping") or false
    helping = quest:PersistTransferBool(context, "Helping", helping)
    quest:SetStateBool("Helping", helping)
    local helped = quest:GetStateBool("Helped") or false
    helped = quest:PersistTransferBool(context, "Helped", helped)
    quest:SetStateBool("Helped", helped)
    local bootyDugUp = quest:GetStateBool("BootyDugUp") or false
    bootyDugUp = quest:PersistTransferBool(context, "BootyDugUp", bootyDugUp)
    quest:SetStateBool("BootyDugUp", bootyDugUp)
end

function WatchForBooty(quest)
    local pQuestName
    local alive = true
    local r1 = quest:GetThingWithScriptName("HiddenBooty")
    alive = not quest:IsActiveThreadTerminating()
    local bVar2 = not alive
    repeat
        if bVar2 then
            r1 = nil
            -- LAB_00d8a3df: (native jump target)
            return
        end
        bVar2 = quest:IsRegionLoaded("OakBay")
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                r1 = nil
                return
            end
            bVar2 = quest:IsRegionLoaded("OakBay")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsRegionLoaded("OakBay")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        end
        bVar2 = quest:IsDiggingSpotEnabled(r1)
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                pQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02", "", "OakBay")
                return
            end
            -- LAB_00d8a401: (native jump target)
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until false
end

