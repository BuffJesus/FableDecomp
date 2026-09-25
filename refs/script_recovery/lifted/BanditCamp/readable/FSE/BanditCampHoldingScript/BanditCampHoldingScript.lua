-- Readable native conversion: Q_BanditCampHoldingScript. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_BanditCampHoldingScript.Main (retail 0x00d129c0)
function Main(quest)
    local isRegionLoaded
    if not quest:GetStateBool("PathReached") then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsLevelLoaded("BanditCampPath_1") do
            if not quest:NewScriptFrame() then return end
        end
        quest:SetStateBool("PathReached", true)
    end
    isRegionLoaded = quest:IsRegionLoaded("BanditCampPathEntrance")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            local oakValeBanditRaidGate = quest:GetThingWithScriptName("OakValeBanditRaidGate")
            if (oakValeBanditRaidGate == nil or not (oakValeBanditRaidGate ~= nil and oakValeBanditRaidGate:IsOpenDoor())) and not quest:IsActiveThreadTerminating() then
                quest:OpenDoor(oakValeBanditRaidGate)
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("BanditCampPathEntrance")
    end
end

-- Q_BanditCampHoldingScript.Init (retail 0x00d129b0)
function Init(quest)
    quest:SetStateBool("PathReached", false)
end

-- Q_BanditCampHoldingScript.OnPersist (retail 0x00d12c20)
function OnPersist(quest, context)
    quest:SetStateBool("PathReached", quest:PersistTransferBool(context, "PathReached", quest:GetStateBool("PathReached")))
end

