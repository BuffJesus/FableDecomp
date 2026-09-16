-- Generated native draft: Q_GuildTrainingWoodsMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar1, bVar3, pCVar4, uVar6
    local alive = true
    quest:SetStateBool("ScorpionsAlive", true)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    local cVar2 = quest:IsLevelLoaded("GuildWoods")
    while not cVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar2 = quest:IsLevelLoaded("GuildWoods")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        if pCVar4 == nil then
            pCVar4 = 0x0
        else
            -- TODO(native): CCharString::CCharString((CCharString *)(pCVar4 + 4),(CCharString *)&stack0xfffffff4);
            -- TODO(native): *(CQ_GuildTrainingWoodsMeleeScript **)(pCVar4 + 8) = this;
            -- TODO(native): pCVar4[0x14] = (CEntityScriptBindingBase)0x1;
            -- TODO(native): *(undefined4 *)(pCVar4 + 0x18) = 1;
        end
        -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,pCVar4);
        uVar6 = 1
        if (1 & 1) ~= 0 then
            uVar6 = 1 & 0xfffffffe
        end
        quest:FinalizeEntityBindings()
        quest:CreateThread("WatchForTermination")  -- native thread body CQ_HobbeCaveScript::WatchForTermination: lift it as function WatchForTermination(quest)
        if (uVar6 & 2) ~= 0 then
            uVar6 = uVar6 & 0xfffffffd
        end
        quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
        if (uVar6 & 4) ~= 0 then
        end
        CVar1 = quest:GetStateBool("ScorpionsAlive")
        while CVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            CVar1 = quest:GetStateBool("ScorpionsAlive")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:SetStateBool("MissionSucceeded", true)
        end
    end
end

function WatchForTermination(quest)
    local bVar4, cVar5, iVar2, ppVar6, r1, r2, r3
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
            -- TODO(native): puStack_28 = auStack_4;
            ppVar6 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(ppVar6, true, false, false)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            -- TODO(native): CCharString__AssignFromWide();
            -- TODO(native): puStack_20 = auStack_10;
            -- TODO(native): puStack_28 = auStack_4;
            ppVar6 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(ppVar6, true, nil --[[missing]], true)
        end
        quest:SetQuestCardObjective("Q_GuildTraining", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02", "HeroGuildComplexInside")
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
                            r1 = quest:GetHero()
                            ppVar6 = quest:AddNewConversation(r1, false, false)
                            iVar2 = *piVar3
                            r2 = quest:GetHero()
                            r3 = quest:GetHero()
                            quest:AddLineToConversation(ppVar6, "TEXT_QST_028_GUILDSEAL_COME_BACK", r3, r2, false)
                            if quest:GetStateBool("MissionSucceeded") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    return
                                end
                                quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", true)
                            end
                            cVar5 = quest:IsLevelLoaded("HeroGuildComplex")
                            while not cVar5 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    return
                                end
                                cVar5 = quest:IsLevelLoaded("HeroGuildComplex")
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
    local bVar2
    local alive = true
    local pCVar5 = "first objective"
    quest:GiveHeroNewQuestObjective("first objective", 0)
    local cVar1 = quest:IsLevelLoaded("GuildWoods")
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:CreateThread("WatchForLeaving")  -- native thread body CV_AmbushScamScript::WatchForQuestFinished: lift it as function WatchForLeaving(quest)
                if (unaff_ESI & 1) ~= 0 then
                    -- TODO(native): unaff_ESI = unaff_ESI & 0xfffffffe;
                end
                quest:CreateThread("TeleportOutHero")  -- native thread body Quest_GuildWoods_Teleport_Exit_Additional: lift it as function TeleportOutHero(quest)
                if (unaff_ESI & 2) ~= 0 then
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if (not bVar2) and (not quest:GetStateBool("MissionFailed")) then
                    -- TODO(native): CQ_CinemaTestScript::EndMission((CQ_CinemaTestScript *)this);
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        cVar1 = quest:IsLevelLoaded("GuildWoods")
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

function helper_D66EE0(quest)
    local bVar3, ppVar4
    local alive = true
    local CVar1 = quest:GetStateBool("MissionOver")
    -- TODO(native): pCStack_4 = this;
    while true do
        if CVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                ppVar4 = quest:GetActiveQuestName()
                quest:SetQuestAsCompleted(ppVar4, true, false, false)
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

