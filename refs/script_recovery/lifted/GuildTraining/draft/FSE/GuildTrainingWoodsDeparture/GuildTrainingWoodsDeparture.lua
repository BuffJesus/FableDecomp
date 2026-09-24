-- Generated native draft: Q_GuildTrainingWoodsDeparture. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar5, bVar2, pPosition, r1, this_00
    local alive = true
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    bVar2 = quest:IsLevelLoaded("GuildWoods")
    CVar5 = 0
    while true do
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:AddEntityBinding("ArtifactThief", "GuildTrainingWoodsDeparture/Entities/ArtifactThief", 1)
                quest:AddEntityBinding("FinalMaze", "GuildTrainingWoodsDeparture/Entities/FinalMaze", 1)
                quest:AddEntityBinding("ScorpionHome", "GuildTrainingWoodsDeparture/Entities/ScorpionHome", 1)
                quest:FinalizeEntityBindings()
                quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Departure_Init: lift it as function WatchForTermination(quest)
                if (CVar5 & 8) ~= 0 then
                    CVar5 = CVar5 & 0xfffffff7
                end
                quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
                quest:SetStateInt("DepartureMissionPoint", 0)
                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "")
                this_00 = quest:GetThingWithScriptName("MazeCreationMarker")
                bVar2 = false
                pPosition = this_00:GetPos()
                r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_TUTORIAL", pPosition, "FinalMaze")
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        bVar2 = quest:IsLevelLoaded("GuildWoods")
        CVar5 = "FinalMaze"
    end
end

function WatchForTermination(quest)
    local bVar4, bVar6, pCVar5
    local alive = true
    local cVar1 = quest:GetStateBool("MissionFailed")
    while (not cVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:SetExperienceSpendingAsEnabled(true)
        if not quest:GetStateBool("MissionFailed") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = true
            bVar4 = false
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, bVar6, false)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = true
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pCVar5, bVar4, "", bVar6)
        end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0)
    end
end

function DoMission(quest)
    local CVar1, bVar3, pQuestName
    local alive = true
    quest:GiveHeroNewQuestObjective("first objective", 1)
    bVar3 = quest:IsLevelLoaded("GuildWoods")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsLevelLoaded("GuildWoods")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:CreateThread("WatchForLeaving")  -- native thread body CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
        quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_First: lift it as function TeleportOutHero(quest)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if (not bVar3) and (not quest:GetStateBool("MissionFailed")) then
            CVar1 = quest:GetStateBool("MissionOver")
            while not CVar1 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                CVar1 = quest:GetStateBool("MissionOver")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                bVar3 = true
                pQuestName = quest:GetActiveQuestName()
                quest:SetQuestAsCompleted(pQuestName, bVar3, true, false)
                quest:SetStateBool("MissionSucceeded", true)
            end
        end
    end
end

function WatchForLeaving(quest)
    local alive = true
    local pCVar2 = quest:GetHero()
    local bVar1 = (pCVar2 ~= nil and pCVar2:IsAlive())
    if bVar1 then
        repeat
            if (quest:GetStateBool("MissionFailed")) or (quest:GetStateBool("MissionSucceeded")) then break end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pCVar2 = quest:GetHero()
            bVar1 = (pCVar2 ~= nil and pCVar2:IsAlive())
        until not (bVar1)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if (not bVar1) and (not quest:GetStateBool("MissionSucceeded")) then
        quest:SetStateBool("MissionFailed", true)
    end
end

function TeleportOutHero(quest)
    local bVar2, fret_0, iVar5, pCVar3, pCVar4
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            return
        end
        pCVar3 = quest:GetHero()
        fret_0 = quest:GetHealth(pCVar3)
        if fret_0 < 6.0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            pCVar3 = quest:GetThingWithScriptName("GuildWoodsTeleportExitHSP")
            pCVar4 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar4, pCVar3, false)
            pCVar3 = nil
            quest:Pause(2.0)
            bVar2 = false
            pCVar3 = quest:GetHero()
            iVar5 = quest:AddNewConversation(pCVar3, bVar2, false)
            pCVar3 = quest:GetHero()
            pCVar4 = quest:GetHero()
            quest:AddLineToConversation(iVar5, "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST", pCVar4, pCVar3, false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until false
end

