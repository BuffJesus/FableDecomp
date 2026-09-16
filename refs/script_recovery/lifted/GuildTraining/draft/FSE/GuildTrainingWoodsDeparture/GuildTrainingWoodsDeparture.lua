-- Generated native draft: Q_GuildTrainingWoodsDeparture. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar3, piVar6, r1, uVar7, uVar8
    local alive = true
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    local cVar2 = quest:IsLevelLoaded("GuildWoods")
    uVar8 = 0
    while true do
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:AddEntityBinding("ArtifactThief", "GuildTrainingWoodsDeparture/Entities/ArtifactThief")
                quest:AddEntityBinding("FinalMaze", "GuildTrainingWoodsDeparture/Entities/FinalMaze")
                quest:AddEntityBinding("ScorpionHome", "GuildTrainingWoodsDeparture/Entities/ScorpionHome")
                quest:FinalizeEntityBindings()
                quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Departure_Init: lift it as function WatchForTermination(quest)
                if (uVar8 & 8) ~= 0 then
                    uVar8 = uVar8 & 0xfffffff7
                end
                quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
                if (uVar8 & 0x10) ~= 0 then
                end
                quest:SetStateInt("DepartureMissionPoint", 0)
                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12")
                piVar6 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_MAZE_TUTORIAL")
                uVar7 = piVar6:GetPos()
                r1 = quest:CreateCreature("MazeCreationMarker", uVar7, "FinalMaze")
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        cVar2 = quest:IsLevelLoaded("GuildWoods")
    end
end

function WatchForTermination(quest)
    local bVar3, ppVar4
    local alive = true
    local CVar1 = quest:GetStateBool("MissionFailed")
    while (not CVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        CVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:SetExperienceSpendingAsEnabled(true)
        if not quest:GetStateBool("MissionFailed") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            ppVar4 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(ppVar4, false, true, false)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            -- TODO(native): CCharString__AssignFromWide();
            ppVar4 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(ppVar4, true, nil --[[missing]], true)
        end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0)
    end
end

function DoMission(quest)
    local CVar1, bVar4, ppVar6
    local alive = true
    local pCVar7 = "first objective"
    quest:GiveHeroNewQuestObjective("first objective", 0)
    local cVar3 = quest:IsLevelLoaded("GuildWoods")
    while not cVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar3 = quest:IsLevelLoaded("GuildWoods")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:CreateThread("WatchForLeaving")  -- native thread body CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
        if (unaff_ESI & 1) ~= 0 then
            -- TODO(native): unaff_ESI = unaff_ESI & 0xfffffffe;
        end
        quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_First: lift it as function TeleportOutHero(quest)
        if (unaff_ESI & 2) ~= 0 then
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if (not bVar4) and (not quest:GetStateBool("MissionFailed")) then
            CVar1 = quest:GetStateBool("MissionOver")
            while not CVar1 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                CVar1 = quest:GetStateBool("MissionOver")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                ppVar6 = quest:GetActiveQuestName()
                quest:SetQuestAsCompleted(ppVar6, nil --[[missing]], nil --[[missing]], nil --[[missing]])
                quest:SetStateBool("MissionSucceeded", true)
            end
        end
    end
end

function WatchForLeaving(quest)
    local bVar2
    local alive = true
    local piVar3 = quest:GetHero()
    local cVar1 = (piVar3 ~= nil and piVar3:IsAlive())
    while ((cVar1 and (not quest:GetStateBool("MissionFailed"))) and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        piVar3 = quest:GetHero()
        cVar1 = (piVar3 ~= nil and piVar3:IsAlive())
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if (not bVar2) and (not quest:GetStateBool("MissionSucceeded")) then
        quest:SetStateBool("MissionFailed", true)
    end
end

function TeleportOutHero(quest)
    local fVar6, iVar1, ppVar4, r1, r2, r3, r4, uVar5
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar3 = not alive
    repeat
        if bVar3 then
            return
        end
        r1 = quest:GetHero()
        fVar6 = quest:GetHealth(r1)
        if fVar6 < _DAT_0125a2a0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            iVar1 = *piVar2
            r2 = quest:GetThingWithScriptName("GuildWoodsTeleportExitHSP")
            ppVar4 = quest:GetHero()
            quest:EntityTeleportToThing(ppVar4, r2)
            -- TODO(native): unaff_ESI = (int *)0x0;
            quest:Pause(nil --[[missing]])
            r3 = quest:GetHero()
            ppVar4 = quest:AddNewConversation(r3, nil --[[missing]], nil --[[missing]])
            iVar1 = *piVar2
            r4 = quest:GetHero()
            uVar5 = quest:GetHero()
            quest:AddLineToConversation(ppVar4, "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_TELEPORT_OUT_FIRST", uVar5, r4, false)
            quest:ChangeHeroHealthBy(0x447a0000, true, false)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
end

