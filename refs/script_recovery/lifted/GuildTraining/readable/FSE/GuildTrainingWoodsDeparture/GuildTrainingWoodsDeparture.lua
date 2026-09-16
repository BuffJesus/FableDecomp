-- Reviewed native Main setup for Q_GuildTrainingWoodsDeparture.
-- Worker thread bodies remain pending; registration is intentionally disabled.

function Main(questObject)
    Quest = questObject
    Quest:SetStateBool("MissionSucceeded", false)
    Quest:SetStateBool("MissionFailed", false)
    Quest:SetStateBool("MissionOver", false)
    Quest:SetStateInt("DepartureMissionPoint", 0)
    while not Quest:IsLevelLoaded("GuildWoods") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    Quest:AddEntityBinding("ArtifactThief", "GuildTrainingWoodsDeparture/Entities/ArtifactThief")
    Quest:AddEntityBinding("FinalMaze", "GuildTrainingWoodsDeparture/Entities/FinalMaze")
    Quest:AddEntityBinding("ScorpionHome", "GuildTrainingWoodsDeparture/Entities/ScorpionHome")
    Quest:FinalizeEntityBindings()
    Quest:CreateThread("WatchForTermination")
    Quest:CreateThread("DoMission")
    Quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12")
    local marker = Quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_MAZE_TUTORIAL")
    Quest:CreateCreature("MazeCreationMarker", marker:GetPos(), "FinalMaze")
end

function WatchForTermination(questObject)
    Quest = questObject
    while not Quest:GetStateBool("MissionFailed") and not Quest:GetStateBool("MissionSucceeded") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    Quest:SetExperienceSpendingAsEnabled(true)
    local activeQuest = Quest:GetActiveQuestName()
    if Quest:GetStateBool("MissionFailed") then
        Quest:SetQuestAsFailed(activeQuest, true, "", true)
    else
        Quest:SetQuestAsCompleted(activeQuest, false, true, false)
    end
    Quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0)
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
    while not Quest:GetStateBool("MissionOver") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    if Quest:IsActiveThreadTerminating() then return end
    Quest:SetQuestAsCompleted(Quest:GetActiveQuestName(), true, true, false)
    Quest:SetStateBool("MissionSucceeded", true)
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
