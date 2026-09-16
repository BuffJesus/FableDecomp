-- Reviewed native Main setup for Q_GuildTrainingWoodsMelee.
-- DoMission, WatchForLeaving, and TeleportOutHero are reviewed from native x86.
-- WatchForTermination remains pending because the shared body carries a nested
-- worker state layout that is not yet mapped to this quest's parent fields.

function Main(questObject)
    Quest = questObject
    Quest:SetStateBool("ScorpionsAlive", true)
    Quest:SetStateBool("MissionSucceeded", false)
    Quest:SetStateBool("MissionFailed", false)
    Quest:SetStateBool("MissionOver", false)
    while not Quest:IsLevelLoaded("GuildWoods") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    Quest:AddEntityBinding("ScorpionHome", "GuildTrainingWoodsMelee/Entities/ScorpionHome")
    Quest:FinalizeEntityBindings()
    Quest:CreateThread("WatchForTermination")
    Quest:CreateThread("DoMission")
    while Quest:GetStateBool("ScorpionsAlive") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    if Quest:IsActiveThreadTerminating() then return end
    Quest:SetStateBool("MissionSucceeded", true)
end

function DoMission(questObject)
    Quest = questObject
    Quest:GiveHeroNewQuestObjective("first objective", 0)
    while not Quest:IsLevelLoaded("GuildWoods") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    Quest:CreateThread("WatchForLeaving")
    Quest:CreateThread("TeleportOutHero")
    if not Quest:NewScriptFrame() then return end
    if Quest:IsActiveThreadTerminating() or Quest:GetStateBool("MissionFailed") then return end
    Quest:EndMission()
end

function TeleportOutHero(questObject)
    Quest = questObject
    while not Quest:IsActiveThreadTerminating() do
        local hero = Quest:GetHero()
        if Quest:GetHealth(hero) < 6.0 then
            if Quest:IsActiveThreadTerminating() then return end
            local exitMarker = Quest:GetThingWithScriptName("GuildWoodsTeleportExitHSP")
            Quest:EntityTeleportToThing(hero, exitMarker)
            Quest:Pause(false)
            local conversation = Quest:AddNewConversation(hero, false, false)
            Quest:AddLineToConversation(conversation,
                "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST",
                hero, hero, false)
            Quest:ChangeHeroHealthBy(1000.0, true, false)
        end
        if not Quest:NewScriptFrame() then return end
    end
end

function WatchForLeaving(questObject)
    Quest = questObject
    local hero = Quest:GetHero()
    while hero ~= nil and hero:IsAlive()
            and not Quest:GetStateBool("MissionFailed")
            and not Quest:GetStateBool("MissionSucceeded") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
        hero = Quest:GetHero()
    end
    if not Quest:GetStateBool("MissionSucceeded") then
        Quest:SetStateBool("MissionFailed", true)
    end
end
