-- Native DBA490..DBA4F5/DBA5A4/DBA8AB, with string compare at DBA588.
-- The caller owns one output across both polls; each poll may replace its text.
local function classifyTheresaPresentedItem(resources, presented)
    local chocolates = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
    if resources:PollPresentedItem(presented) and resources:PresentedItemMatches(presented, chocolates) then
        return "chocolates"
    end
    if resources:PollPresentedItem(presented) and not resources:PresentedItemMatches(presented, chocolates) then
        return "other-present"
    end
    return "none"
end

return classifyTheresaPresentedItem
