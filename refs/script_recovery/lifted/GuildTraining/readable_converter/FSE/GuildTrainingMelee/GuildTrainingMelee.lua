-- Readable native conversion: Q_GuildTrainingMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingMelee.Main (retail 0x00d55e90)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local actorMap, pThingToMove, rivalHeroWhisperTeenApprentice, resource, resource2
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("MeleeOpponent", "GuildTrainingMelee/Entities/MeleeOpponent")
    quest:AddEntityBinding("MeleeThunder", "GuildTrainingMelee/Entities/MeleeThunder")
    quest:FinalizeEntityBindings()
    quest:EntityTeleportToThing(quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetThingWithScriptName("M_MeleeTeacherStand"), false)
    rivalHeroWhisperTeenApprentice = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("MK_GTWU_WHISPER"):GetPos(), "MeleeOpponent")
    quest:EntitySetInFaction(rivalHeroWhisperTeenApprentice, "FACTION_HERO")
    if rivalHeroWhisperTeenApprentice ~= nil and not rivalHeroWhisperTeenApprentice:IsNull() then
        rivalHeroWhisperTeenApprentice:SetFriendsWithEverythingFlag(1)
    end
    resource2 = resources:NewResource()
    while not resources:TryAcquire(resource2, rivalHeroWhisperTeenApprentice, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource2); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAllowBossPhaseChanges(rivalHeroWhisperTeenApprentice, false)
        resource = resources:NewResource()
        while not resources:TryAcquire(resource, hero, 4) do
            if not quest:NewScriptFrame() then goto LAB_00d56509 end
        end
        if not quest:IsActiveThreadTerminating() then
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource)
            resources:SetActor(actorMap, "WHISPER", resource2)
            pThingToMove = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_INTRO", actorMap, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("TalkedToWhisper", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(pThingToMove)
            resources:DestroyActorMap(actorMap)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "")
            quest:AddLogbookStoryEntry(40)
            if quest:DisplayTutorial(26) and not quest:IsActiveThreadTerminating() then
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00d56509 end
                end
            end
        end
        ::LAB_00d56509::
        resources:ReleaseResource(resource)
    end
    resources:ReleaseResource(resource2)
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

