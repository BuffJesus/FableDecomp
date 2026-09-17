-- Readable native conversion: Q_GuildTrainingWoodsMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingWoodsMelee.Main (retail 0x00d66620)
function Main(quest)
    local pBinding
    quest:SetStateBool("ScorpionsAlive", true)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    while not quest:IsLevelLoaded("GuildWoods") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if pBinding ~= nil then
        -- TODO(native): CCharString::CCharString((CCharString *)(pBinding + 1),&xStack_8);
        -- TODO(native): pBinding[2] = this;
        -- TODO(native): *(undefined1 *)(pBinding + 5) = 1;
        -- TODO(native): pBinding[6] = 1;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,pBinding);
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
    local hero2, pSpeaker
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionFailed") then
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
    else
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
    end
    quest:SetQuestCardObjective("TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02", "HeroGuildComplexInside", "", "Q_GuildTraining")
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    if not quest:NewScriptFrame() then return end
    hero2 = quest:GetHero()
    pSpeaker = quest:GetHero()
    quest:AddLineToConversation(quest:AddNewConversation(quest:GetHero(), false, false), "TEXT_QST_028_GUILDSEAL_COME_BACK", pSpeaker, hero2, false)
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
    local scratchValue2 = quest:IsLevelLoaded("GuildWoods")
    while true do
        if scratchValue2 then
            if quest:IsActiveThreadTerminating() then return end
            quest:CreateThread("WatchForLeaving")  -- native thread body CV_AmbushScamScript::WatchForQuestFinished: lift it as function WatchForLeaving(quest)
            quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_Additional: lift it as function TeleportOutHero(quest)
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
                -- TODO(native): CQ_CinemaTestScript::EndMission((CQ_CinemaTestScript *)this);
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        scratchValue2 = quest:IsLevelLoaded("")
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

-- Q_GuildTrainingWoodsMelee.helper_D66EE0 (retail 0x00d66ee0)
function helper_D66EE0(quest)
    local missionOver = quest:GetStateBool("MissionOver")
    while true do
        if missionOver then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
            quest:SetStateBool("MissionSucceeded", true)
            return
        end
        if not quest:NewScriptFrame() then break end
        missionOver = quest:GetStateBool("MissionOver")
    end
end

