-- Native Main DAE6BC..DAE988. Conversation and retained key already exist.
-- speechLists must expose the quest's owned CString vectors, not copied Lua strings.
local function addVillagerAmbientLine(quest, me, resources, speechLists, key, conversation, getSpeechIndex)
    local badDeeds = quest:GetStateInt("BadDeedsPerformed")
    local category
    if badDeeds == 0 and quest:GetStateInt("GoodDeedsPerformed") > 0 then
        category = "good"
    elseif badDeeds > 0 and quest:GetStateInt("GoodDeedsPerformed") == 0 then
        category = "bad"
    elseif badDeeds > 0 and quest:GetStateInt("GoodDeedsPerformed") > 0 then
        category = "both"
    else
        category = "none"
    end
    if quest:IsActiveThreadTerminating() then return false end
    local male = quest:EntityGetSex(me) == 1
    if quest:IsActiveThreadTerminating() then return false end
    local index = getSpeechIndex(quest, me, speechLists:Count(category, male))
    -- The index helper can yield. Read the selected vector again at assignment.
    resources:AssignVillagerSpeechText(key, speechLists, category, male, index)
    resources:AddVillagerAmbientText(conversation, key, me)
    return true
end

return addVillagerAmbientLine
