-- Generated from the same native helper bodies as the quest draft.
local MakeBriarRoseComment
function MakeBriarRoseComment(quest, me, native_arg_comment_to_make)
    local bVar2, cVar3, conversationID, pCVar4, pLine, pListener, r1
    bVar2 = quest:IsConversationActive(quest:GetStateInt("ConversationIndex"))
    if not bVar2 then
        quest:SetTimer(quest:GetStateInt("CommentaryTimer"), 0xf)
        r1 = quest:GetThingWithScriptName("STS_BriarRose")
        if (r1 ~= nil and not r1:IsNull()) then
            cVar3 = (r1 ~= nil and r1:IsAlive())
            if cVar3 then
                conversationID = quest:AddNewConversation(r1, false, false)
                quest:SetStateInt("ConversationIndex", conversationID)
                pCVar4 = quest:GetHero()
                quest:AddPersonToConversation(conversationID, pCVar4)
                pListener = quest:GetHero()
                pCVar4 = r1
                bVar2 = false
                pLine = ("TEXT_QST_B02_BRIARROSE_" .. native_arg_comment_to_make)
                quest:AddLineToConversation(conversationID, pLine, pCVar4, pListener, bVar2)
                return true
            end
        end
    end
    return false
end

return {MakeBriarRoseComment = MakeBriarRoseComment}
