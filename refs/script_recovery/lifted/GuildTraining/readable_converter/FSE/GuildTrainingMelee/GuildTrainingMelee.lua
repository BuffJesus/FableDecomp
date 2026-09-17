-- Readable native conversion: Q_GuildTrainingMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingMelee.Main (retail 0x00d55e90)
function Main(quest)
    local resources = quest:RetailResources()
    local scratchValue3, pThingToMove, scratchValue4, scratchValue5, scratchValue6
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("MeleeOpponent", "GuildTrainingMelee/Entities/MeleeOpponent")
    quest:AddEntityBinding("MeleeThunder", "GuildTrainingMelee/Entities/MeleeThunder")
    quest:FinalizeEntityBindings()
    quest:EntityTeleportToThing(quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetThingWithScriptName("M_MeleeTeacherStand"), false)
    scratchValue4 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("MK_GTWU_WHISPER"):GetPos(), "MeleeOpponent")
    quest:EntitySetInFaction(scratchValue4, "FACTION_HERO")
    if scratchValue4 ~= nil and not scratchValue4:IsNull() then
        scratchValue4:SetFriendsWithEverythingFlag(1)
    end
    scratchValue6 = resources:NewResource()
    while not resources:TryAcquire(scratchValue6, scratchValue4, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(scratchValue6); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAllowBossPhaseChanges(scratchValue4, false)
        scratchValue5 = resources:NewResource()
        while not resources:TryAcquire(scratchValue5, quest:GetHero(), 4) do
            if not quest:NewScriptFrame() then goto LAB_00d56509 end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue3 = resources:NewActorMap()
            resources:SetActor(scratchValue3, "HERO", scratchValue5)
            resources:SetActor(scratchValue3, "WHISPER", scratchValue6)
            pThingToMove = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_INTRO", scratchValue3, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("TalkedToWhisper", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(pThingToMove)
            resources:DestroyActorMap(scratchValue3)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "")
            quest:AddLogbookStoryEntry(40)
            if quest:DisplayTutorial(26) and not quest:IsActiveThreadTerminating() then
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00d56509 end
                end
            end
        end
        ::LAB_00d56509::
        resources:ReleaseResource(scratchValue5)
    end
    resources:ReleaseResource(scratchValue6)
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

