-- Generated native draft: Q_GuildTrainingWoodsMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar1, CVar4, pBinding
    local alive = true
    quest:SetStateBool("ScorpionsAlive", true)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    local bVar2 = quest:IsLevelLoaded("GuildWoods")
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsLevelLoaded("GuildWoods")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        if pBinding == nil then
            pBinding = 0x0
        else
            -- TODO(native): CCharString::CCharString((CCharString *)(pBinding + 1),&xStack_8);
            -- TODO(native): pBinding[2] = this;
            -- TODO(native): *(undefined1 *)(pBinding + 5) = 1;
            -- TODO(native): pBinding[6] = 1;
        end
        -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,pBinding);
        CVar4 = 0x1
        if (0x1 & 1) ~= 0 then
            CVar4 = (0x1 & 0xfffffffe)
        end
        quest:FinalizeEntityBindings()
        quest:CreateThread("WatchForTermination")  -- native thread body CQ_HobbeCaveScript::WatchForTermination: lift it as function WatchForTermination(quest)
        if (CVar4 & 2) ~= 0 then
            CVar4 = (CVar4 & 0xfffffffd)
        end
        quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
        if (CVar4 & 4) ~= 0 then
        end
        CVar1 = quest:GetStateBool("ScorpionsAlive")
        while CVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            CVar1 = quest:GetStateBool("ScorpionsAlive")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:SetStateBool("MissionSucceeded", true)
        end
    end
end

function WatchForTermination(quest)
    local bVar4, bVar7, conversationID, pCVar5, pCVar6, pSpeaker
    local alive = true
    local CVar1 = quest:GetStateBool("MissionFailed")
    while (not CVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        CVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        if not quest:GetStateBool("MissionFailed") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar7 = false
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, bVar7, false)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar7 = true
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pCVar5, bVar4, "", bVar7)
        end
        quest:SetQuestCardObjective("TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02", "HeroGuildComplexInside", "", "Q_GuildTraining")
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            bVar7 = false
                            bVar4 = false
                            pCVar6 = quest:GetHero()
                            conversationID = quest:AddNewConversation(pCVar6, bVar4, bVar7)
                            pCVar6 = quest:GetHero()
                            pSpeaker = quest:GetHero()
                            quest:AddLineToConversation(conversationID, "TEXT_QST_028_GUILDSEAL_COME_BACK", pSpeaker, pCVar6, false)
                            if quest:GetStateBool("MissionSucceeded") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    return
                                end
                                quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", true)
                            end
                            bVar4 = quest:IsLevelLoaded("HeroGuildComplex")
                            while not bVar4 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    return
                                end
                                bVar4 = quest:IsLevelLoaded("HeroGuildComplex")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
                            end
                        end
                    end
                end
            end
        end
    end
end

function DoMission(quest)
    local CVar3
    local alive = true
    quest:GiveHeroNewQuestObjective("first objective", 1)
    local bVar1 = quest:IsLevelLoaded("GuildWoods")
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:CreateThread("WatchForLeaving")  -- native thread body CV_AmbushScamScript::WatchForQuestFinished: lift it as function WatchForLeaving(quest)
                CVar3 = 0x0
                if (0x0 & 1) ~= 0 then
                    CVar3 = (0x0 & 0xfffffffe)
                end
                quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_Additional: lift it as function TeleportOutHero(quest)
                if (CVar3 & 2) ~= 0 then
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if (not bVar1) and (not quest:GetStateBool("MissionFailed")) then
                    -- TODO(native): CQ_CinemaTestScript::EndMission((CQ_CinemaTestScript *)this);
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsLevelLoaded("")
    end
end

function WatchForLeaving(quest)
    local bVar2
    local alive = true
    local CVar1 = quest:GetStateBool("MissionFailed")
    while (not CVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        CVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if (not bVar2) and (not quest:GetStateBool("MissionSucceeded")) then
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

function helper_D66EE0(quest)
    local bVar3, pQuestName
    local alive = true
    local CVar1 = quest:GetStateBool("MissionOver")
    while true do
        if CVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                bVar3 = true
                pQuestName = quest:GetActiveQuestName()
                quest:SetQuestAsCompleted(pQuestName, bVar3, false, false)
                quest:SetStateBool("MissionSucceeded", true)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        CVar1 = quest:GetStateBool("MissionOver")
    end
end

