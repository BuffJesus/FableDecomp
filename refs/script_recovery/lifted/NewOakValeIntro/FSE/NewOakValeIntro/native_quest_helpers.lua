-- Generated from the same native helper bodies as the quest draft.
local AddBadDeed, AddGoodDeed
function AddBadDeed(quest, me, native_arg_param_1)
    local bVar1, cVar2
    local alive = true
    quest:SetStateInt("BadDeedsPerformed", quest:GetStateInt("BadDeedsPerformed") + 1)
    quest:GiveHeroMorality(-0.0010000000474974513)
    if (quest:GetStateInt("BadDeedsPerformed") == 1) and (quest:GetStateInt("GoodDeedsPerformed") == 0) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED")
            cVar2 = quest:MsgIsGameInfoClickedPast()
            while not cVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                cVar2 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP")
                quest:SetStateBool("WhichBadDeedsPerformed_" .. (native_arg_param_1), true)
                return
            end
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            if not quest:GetStateBool("WhichBadDeedsPerformed_" .. (native_arg_param_1)) then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_BAD_DEED")
                cVar2 = quest:MsgIsGameInfoClickedPast()
                while not cVar2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
            end
            quest:SetStateBool("WhichBadDeedsPerformed_" .. (native_arg_param_1), true)
        end
    end
end

function AddGoodDeed(quest, me)
    local bVar1, cVar2, uVar4
    local alive = true
    quest:SetStateInt("GoodDeedsPerformed", quest:GetStateInt("GoodDeedsPerformed") + 1)
    quest:GiveHeroMorality(0.0010000000474974513)
    if (quest:GetStateInt("GoodDeedsPerformed") == 1) and (quest:GetStateInt("BadDeedsPerformed") == 0) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED")
        cVar2 = quest:MsgIsGameInfoClickedPast()
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            cVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP")
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_GOOD_DEED")
        cVar2 = quest:MsgIsGameInfoClickedPast()
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            cVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
    end
    local iVar3 = quest:GetHeroGold()
    if (iVar3 < 3) and (not quest:GetStateBool("GivenSweets")) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        uVar4 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(uVar4, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02", "", "")
    end
    quest:UpdateQuestInfoCounter(quest:GetStateInt("GUIGoodDeedCounter"), quest:GetStateInt("GoodDeedsPerformed"), -1)
end

return {AddBadDeed = AddBadDeed, AddGoodDeed = AddGoodDeed}
