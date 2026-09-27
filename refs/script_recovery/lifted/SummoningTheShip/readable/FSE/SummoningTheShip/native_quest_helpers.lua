-- Generated from the same native helper bodies as the quest draft.
local MakeBriarRoseComment
function MakeBriarRoseComment(quest, me, commentToMake)
    local isConversationActive, getHero
    local hero = quest:GetHero()
    isConversationActive = quest:IsConversationActive(quest:GetStateInt("ConversationIndex"))
    if not isConversationActive then
        quest:SetTimer(quest:GetStateInt("CommentaryTimer"), 15)
        local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
        if briarRose ~= nil then
            local scratchValue = briarRose ~= nil and briarRose:IsAlive()
            if scratchValue then
                local conversationID = quest:AddNewConversation(briarRose, false, false)
                quest:SetStateInt("ConversationIndex", conversationID)
                getHero = hero
                quest:AddPersonToConversation(conversationID, getHero)
                local pListener = hero
                getHero = briarRose
                isConversationActive = false
                local pLine = "TEXT_QST_B02_BRIARROSE_" .. commentToMake
                quest:AddLineToConversation(conversationID, pLine, getHero, pListener, isConversationActive)
                return true
            end
        end
    end
    return false
end

return {MakeBriarRoseComment = MakeBriarRoseComment}
