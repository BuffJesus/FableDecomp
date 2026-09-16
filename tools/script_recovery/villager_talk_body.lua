-- Native Main DAE3F3..DAE607. Caller has checked talk result and termination.
-- The outer resource scope owns control; this function owns only the suffix.
local function handleVillagerConversation(quest, me, resources, control, state)
    local suffix = resources:NewText()
    local function conversationBody()
        local male = quest:EntityGetSex(me) == 1
        if quest:IsActiveThreadTerminating() then return false end
        resources:AssignVillagerSuffix(suffix, male)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local conversation = resources:StartVillagerTalkConversation(me)
        local heroHitVillager = state:GetStateBool("HeroDidHitMe")
        if quest:IsActiveThreadTerminating() then return false end
        resources:AddVillagerTalkLine(conversation, suffix, me, heroHitVillager)
        while quest:IsConversationActive(conversation) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        resources:PrepareResource(control)
        return true
    end
    local ok, result = pcall(conversationBody)
    -- Release the retained suffix before the caller releases the outer locals.
    local released, releaseError = pcall(resources.DestroyText, resources, suffix)
    if not ok then error(result, 0) end
    if not released then error(releaseError, 0) end
    return result
end

return handleVillagerConversation
