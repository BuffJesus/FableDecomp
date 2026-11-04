-- Readable native conversion: Q_GuildTrainingWoodsMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingWoodsMelee.Main (retail 0x00d66620)
function Main(quest)
    quest:SetStateBool("ScorpionsAlive", true)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    while not quest:IsLevelLoaded("GuildWoods") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:AddEntityBinding("ScorpionHome", "GuildTrainingWoodsMelee/Entities/ScorpionHome", 1)
    quest:FinalizeEntityBindings()
    quest:CreateThread("WatchForTermination")  -- native thread body CQ_HobbeCaveScript::WatchForTermination: lift it as function WatchForTermination(quest)
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
    while quest:GetStateBool("ScorpionsAlive") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("MissionSucceeded", true)
end

-- Q_GuildTrainingWoodsMelee.WatchForTermination (retail 0x00d66880)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionFailed") then
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName, true, false, false)
    else
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName2 = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(getActiveQuestName2, true, "", true)
    end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02", "HeroGuildComplexInside", "")
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    local hero = quest:GetHero()
    local conversationID = quest:AddNewConversation(hero, false, false)
    local hero2 = quest:GetHero()
    local pSpeaker = quest:GetHero()
    quest:AddLineToConversation(conversationID, "TEXT_QST_028_GUILDSEAL_COME_BACK", pSpeaker, hero2, false)
    if quest:GetStateBool("MissionSucceeded") then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", true)
    end
    while not quest:IsLevelLoaded("HeroGuildComplex") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
end

-- Q_GuildTrainingWoodsMelee.DoMission (retail 0x00d66ca0)
function DoMission(quest)
    quest:GiveHeroNewQuestObjective("first objective", 1)
    local isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    while true do
        if isLevelLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:CreateThread("WatchForLeaving")  -- native thread body CV_AmbushScamScript::WatchForQuestFinished: lift it as function WatchForLeaving(quest)
            quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_Additional: lift it as function TeleportOutHero(quest)
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
                EndMission(quest)
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    end
end

-- Q_GuildTrainingWoodsMelee.WatchForLeaving (retail 0x00d66e90)
function WatchForLeaving(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionSucceeded") then
        quest:SetStateBool("MissionFailed", true)
    end
end

-- Q_GuildTrainingWoodsMelee.TeleportOutHero (retail 0x00d66f50)
function TeleportOutHero(quest)
    local guildWoodsTeleportExitHSP
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local hero = quest:GetHero()
        if not (quest:GetHealth(hero) < 6.0) then
            quest:NewScriptFrame()
        else
            if quest:IsActiveThreadTerminating() then return end
            guildWoodsTeleportExitHSP = quest:GetThingWithScriptName("GuildWoodsTeleportExitHSP")
            local hero4 = quest:GetHero()
            quest:EntityTeleportToThing(hero4, guildWoodsTeleportExitHSP, false)
            guildWoodsTeleportExitHSP = nil
            quest:Pause(2.0)
            local hero2 = quest:GetHero()
            local conversationId = quest:AddNewConversation(hero2, false, false)
            local hero3 = quest:GetHero()
            local hero5 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST", hero5, hero3, false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:NewScriptFrame()
        end
    until false
end

-- Q_GuildTrainingWoodsMelee.EndMission (retail 0x00d66ee0)
-- D66EE0: bsim names this body NScript::CQ_CinemaTestScript::EndMission (a homologous script member); no PDB name
function EndMission(quest)
    local missionOver = quest:GetStateBool("MissionOver")
    while true do
        if missionOver then
            if quest:IsActiveThreadTerminating() then return end
            local pQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pQuestName, true, false, false)
            quest:SetStateBool("MissionSucceeded", true)
            return
        end
        if not quest:NewScriptFrame() then break end
        missionOver = quest:GetStateBool("MissionOver")
    end
end

