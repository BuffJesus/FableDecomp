-- Native DBA004..DBA10F/DBA31A, inside the later offer movie.
local function askTheresaOfferChoice(quest, me, resources)
    resources:ShowTheresaChocolateQuestion()
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return nil end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if quest:IsActiveThreadTerminating() then return nil end
    if quest:IsActiveThreadTerminating() then return nil end
    return answer == 1
end

return askTheresaOfferChoice
