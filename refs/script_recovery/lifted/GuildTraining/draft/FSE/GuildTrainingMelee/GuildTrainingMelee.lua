-- Generated native draft: Q_GuildTrainingMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bUnknown, bVar6, iVar8, native_arg_sequence_1, pCVar4, pPosition, pThingToMove, pppuVar7, r1, xStack_10, xStack_20
    local alive = true
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("MeleeOpponent", "GuildTrainingMelee/Entities/MeleeOpponent")
    quest:AddEntityBinding("MeleeThunder", "GuildTrainingMelee/Entities/MeleeThunder")
    quest:FinalizeEntityBindings()
    bUnknown = 0
    pCVar4 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    pThingToMove = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:EntityTeleportToThing(pThingToMove, pCVar4, (bUnknown ~= 0))
    pThingToMove = nil
    pCVar4 = nil
    pCVar4 = quest:GetThingWithScriptName("MK_GTWU_WHISPER")
    bVar6 = false
    pPosition = pCVar4:GetPos()
    r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pPosition, "MeleeOpponent")
    pCVar4 = nil
    quest:EntitySetInFaction(r1, "FACTION_HERO")
    if (r1 ~= nil and not r1:IsNull()) then
        r1:SetFriendsWithEverythingFlag(1)
    end
    xStack_20 = resources:NewResource()
    bVar6 = false
    if bVar6 ~= 0 then
    end
    bVar6 = resources:TryAcquire(xStack_20, r1, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            r1 = nil
            return
        end
        bVar6 = resources:TryAcquire(pThingToMove, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        quest:EntitySetAllowBossPhaseChanges(nil --[[missing]], (r1 ~= 0))
        xStack_10 = resources:NewResource()
        bVar6 = false
        if bVar6 ~= 0 then
        end
        iVar8 = 4
        pppuVar7 = xStack_10
        pCVar4 = quest:GetHero()
        bVar6 = resources:TryAcquire(pppuVar7, pCVar4, iVar8)
        while not bVar6 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d56509 end
            iVar8 = 4
            pppuVar7 = xStack_10
            pCVar4 = quest:GetHero()
            bVar6 = resources:TryAcquire(pppuVar7, pCVar4, iVar8)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            pCVar4 = resources:NewActorMap()
            resources:SetActor(pCVar4, "HERO", xStack_10)
            resources:SetActor(pCVar4, "WHISPER", pThingToMove)
            pThingToMove = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_INTRO", pCVar4, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("TalkedToWhisper", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(0)
            resources:DestroyActorMap(pCVar4)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "")
            quest:AddLogbookStoryEntry(40)
            bVar6 = quest:DisplayTutorial(0x1a)
            native_arg_sequence_1 = false
            if bVar6 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                bVar6 = quest:MsgIsTutorialClickedPast()
                while not bVar6 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d56509 end
                    bVar6 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
            end
        end
        ::LAB_00d56509::
        resources:ReleaseResource(xStack_10)
    end
    resources:ReleaseResource(0)
end

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

