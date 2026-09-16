-- Native DB73A0..DB7924. Timer reads are intentionally repeated per threshold.
local function updateBarrelThugTimedRemarks(quest, me, resources, control, state)
    local function remaining()
        return resources:GetBarrelWatchTimer(quest:GetStateInt("WatchTimer"))
    end
    if quest:GetStateBool("BarrelManSpokenToHeroOnReturn") or remaining() <= 0 then
        if quest:IsActiveThreadTerminating() then return false end
        resources:ResetResource(control)
        return true
    end
    if quest:IsActiveThreadTerminating() then return false end
    local broken = quest:GetStateBool("BarrelBrokenPersistent")
    if quest:IsActiveThreadTerminating() then return false end
    local conversation = resources:NewBarrelThugRemarkConversation(me)
    local thresholds = broken and {10, 25, 35, 45} or {10, 20, 25, 30, 34, 38, 45}
    local prefix = broken and "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_" or "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_"
    for index, threshold in ipairs(thresholds) do
        if remaining() < threshold and state:GetStateInt("LastTimeSpoken") > threshold then
            if quest:IsActiveThreadTerminating() then return false end
            resources:AddBarrelThugRemark(conversation, me, prefix .. tostring(index * 10))
            state:SetStateInt("LastTimeSpoken", remaining())
            break
        end
    end
    return true
end

return updateBarrelThugTimedRemarks
