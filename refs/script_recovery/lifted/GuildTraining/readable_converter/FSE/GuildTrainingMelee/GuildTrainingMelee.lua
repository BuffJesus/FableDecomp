-- Readable native conversion: Q_GuildTrainingMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local TUTORIAL_CATEGORY_MOVEMENT = 26  -- ETutorialCategory (Ego_r.pdb)

-- Q_GuildTrainingMelee.Main (retail 0x00d55e90)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("MeleeOpponent", "GuildTrainingMelee/Entities/MeleeOpponent")
    quest:AddEntityBinding("MeleeThunder", "GuildTrainingMelee/Entities/MeleeThunder")
    quest:FinalizeEntityBindings()
    quest:EntityTeleportToThing(quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetThingWithScriptName("M_MeleeTeacherStand"), false)
    local rivalHeroWhisperTeenApprentice = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("MK_GTWU_WHISPER"):GetPos(), "MeleeOpponent")
    quest:EntitySetInFaction(rivalHeroWhisperTeenApprentice, "FACTION_HERO")
    if rivalHeroWhisperTeenApprentice ~= nil and not rivalHeroWhisperTeenApprentice:IsNull() then
        rivalHeroWhisperTeenApprentice:SetFriendsWithEverythingFlag(1)
    end
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, rivalHeroWhisperTeenApprentice, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySetAllowBossPhaseChanges(rivalHeroWhisperTeenApprentice, false)
    if not hero:AcquireControl(4) then goto LAB_00d56509 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d56509 end
    quest:StartCutscene({HERO = hero, WHISPER = rivalHeroWhisperTeenApprentice}, {}, true)
    quest:RunCutscene("CS_GUILD_MELEE_INTRO", true, false)
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("TalkedToWhisper", true)
    quest:EndCutscene()
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "")
    quest:AddLogbookStoryEntry(40)
    if not (quest:DisplayTutorial(TUTORIAL_CATEGORY_MOVEMENT) and not quest:IsActiveThreadTerminating()) then goto LAB_00d56509 end
    while not quest:MsgIsTutorialClickedPast() do
        if not quest:NewScriptFrame() then goto LAB_00d56509 end
    end
    ::LAB_00d56509::
    hero:ReleaseControl()
    resources:ReleaseResource(resource)
end

-- Q_GuildTrainingMelee.Init (retail 0x00d55da0)
function Init(quest)
    quest:SetStateInt("GenericTutorialCounter", 0)
    quest:SetStateInt("TutorialState", 1)
    quest:SetStateBool("TalkedToWhisper", false)
    quest:SetStateBool("EarlyHitWhisper", false)
    quest:SetStateBool("WhisperArrived", false)
    quest:SetStateBool("WhisperStopWalking", false)
    quest:SetStateBool("MeleeRepeating", true)
    quest:SetStateBool("MeleeRepeatKnown", false)
    quest:SetStateBool("MeleeOpponentReset", false)
end

