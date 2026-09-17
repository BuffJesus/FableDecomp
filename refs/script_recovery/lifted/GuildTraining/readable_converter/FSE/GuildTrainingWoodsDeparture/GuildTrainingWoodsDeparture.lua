-- Readable native conversion: Q_GuildTrainingWoodsDeparture. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingWoodsDeparture.Main (retail 0x00d61d20)
function Main(quest)
    local isLevelLoaded
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    while true do
        if isLevelLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:AddEntityBinding("ArtifactThief", "GuildTrainingWoodsDeparture/Entities/ArtifactThief")
            quest:AddEntityBinding("FinalMaze", "GuildTrainingWoodsDeparture/Entities/FinalMaze")
            quest:AddEntityBinding("ScorpionHome", "GuildTrainingWoodsDeparture/Entities/ScorpionHome")
            quest:FinalizeEntityBindings()
            quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Departure_Init: lift it as function WatchForTermination(quest)
            quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
            quest:SetStateInt("DepartureMissionPoint", 0)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "")
            quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_TUTORIAL", quest:GetThingWithScriptName("MazeCreationMarker"):GetPos(), "FinalMaze")
            return
        end
        if not quest:NewScriptFrame() then break end
        isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    end
end

-- Q_GuildTrainingWoodsDeparture.WatchForTermination (retail 0x00d62140)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetExperienceSpendingAsEnabled(true)
    if not quest:GetStateBool("MissionFailed") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, true, false)
    else
        if quest:IsActiveThreadTerminating() then return end
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
    end
    quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", "Q_GuildTrainingWoodsDeparture")
end

-- Q_GuildTrainingWoodsDeparture.DoMission (retail 0x00d63dc0)
function DoMission(quest)
    quest:GiveHeroNewQuestObjective("first objective", 1)
    while not quest:IsLevelLoaded("GuildWoods") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CreateThread("WatchForLeaving")  -- native thread body CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
    quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_First: lift it as function TeleportOutHero(quest)
    quest:NewScriptFrame()
    if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
        while not quest:GetStateBool("MissionOver") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, true, false)
        quest:SetStateBool("MissionSucceeded", true)
    end
end

-- Q_GuildTrainingWoodsDeparture.WatchForLeaving (retail 0x00d64000)
function WatchForLeaving(quest)
    local hero = quest:GetHero()
    while hero ~= nil and hero:IsAlive() do
        if quest:GetStateBool("MissionFailed") or quest:GetStateBool("MissionSucceeded") then break end
        if not quest:NewScriptFrame() then return end
        hero = quest:GetHero()
    end
    if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionSucceeded") then
        quest:SetStateBool("MissionFailed", true)
    end
end

-- Q_GuildTrainingWoodsDeparture.TeleportOutHero (retail 0x00d64080)
function TeleportOutHero(quest)
    local hero3, hero5
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetHealth(quest:GetHero()) < 6.0 then
            quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("GuildWoodsTeleportExitHSP"), false)
            quest:Pause(2.0)
            hero3 = quest:GetHero()
            hero5 = quest:GetHero()
            quest:AddLineToConversation(quest:AddNewConversation(quest:GetHero(), false, false), "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST", hero5, hero3, false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
        end
        quest:NewScriptFrame()
    until false
end

