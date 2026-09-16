-- Native DB9B7F..DB9C87. nil means cancellation; only answer 1 accepts.
local function askTheresaAboutChocolates(quest, me, resources)
    if quest:IsActiveThreadTerminating() then return nil end
    resources:ShowTheresaChocolateQuestion()
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return nil end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if quest:IsActiveThreadTerminating() then return nil end
    if answer ~= 1 then return false end
    if quest:IsActiveThreadTerminating() then return nil end
    return true
end

return askTheresaAboutChocolates
