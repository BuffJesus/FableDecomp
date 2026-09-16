-- Native deed helpers share control flow between quest and entity callers.
local function deedFrame(quest, me)
    if me ~= nil then quest:NewScriptFrame(me) else quest:NewScriptFrame() end
end

local function incrementDeed(quest, key)
    local MAX_SIGNED_INT32 = 2147483647
    local MIN_SIGNED_INT32 = -2147483648
    local count = quest:GetStateInt(key)
    -- Match the native counter's overflow without obscuring the increment.
    if count == MAX_SIGNED_INT32 then
        count = MIN_SIGNED_INT32
    else
        count = count + 1
    end
    quest:SetStateInt(key, count)
end

local function waitForDeedMessage(quest, me)
    while not quest:MsgIsGameInfoClickedPast() do
        deedFrame(quest, me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

local function addGoodDeed(quest, me)
    quest:WithRetailResources(function(resources)
        incrementDeed(quest, "GoodDeedsPerformed")
        resources:ApplyOakvaleDeedMorality(true)
        local first = quest:GetStateInt("GoodDeedsPerformed") == 1 and quest:GetStateInt("BadDeedsPerformed") == 0
        if quest:IsActiveThreadTerminating() then return end
        resources:DisplayRawGameInfo(first and "TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED" or "TEXT_QST_048_SCRMSG_DID_GOOD_DEED")
        if not waitForDeedMessage(quest, me) then return end
        if first then quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP") end
        if quest:GetHeroGold() < 3 and not quest:GetStateBool("GivenSweets") then
            if quest:IsActiveThreadTerminating() then return end
            resources:SetOakvaleDeedObjective()
        end
        quest:UpdateQuestInfoCounter(quest:GetStateInt("GUIGoodDeedCounter"), quest:GetStateInt("GoodDeedsPerformed"), -1)
    end)
end

local function addBadDeed(quest, me, deed)
    quest:WithRetailResources(function(resources)
        incrementDeed(quest, "BadDeedsPerformed")
        resources:ApplyOakvaleDeedMorality(false)
        local first = quest:GetStateInt("BadDeedsPerformed") == 1 and quest:GetStateInt("GoodDeedsPerformed") == 0
        if quest:IsActiveThreadTerminating() then return end
        if first then
            resources:DisplayRawGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED")
            if not waitForDeedMessage(quest, me) then return end
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP")
        elseif not quest:GetStateBool("WhichBadDeedsPerformed_" .. deed) then
            if quest:IsActiveThreadTerminating() then return end
            resources:DisplayRawGameInfo("TEXT_QST_048_SCRMSG_DID_BAD_DEED")
            if not waitForDeedMessage(quest, me) then return end
        end
        quest:SetStateBool("WhichBadDeedsPerformed_" .. deed, true)
    end)
end

return { good = addGoodDeed, bad = addBadDeed }
