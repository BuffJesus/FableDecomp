-- Generated native draft: Q_GuildTrainingWoodsWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar1, bVar5, cVar4, fVar14, iVar11, iVar2, pCVar12, pCVar20, ppVar10, ppVar15, puVar18, puVar19, r1, r2, r3, r4, r5, r6, r7, r8, r9, uVar13, uVar8
    local alive = true
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    quest:SetStateBool("WhisperAnimate", false)
    quest:SetStateBool("BanditsAlive", true)
    quest:AddEntityBinding("WillWhisper", "GuildTrainingWoodsWill/Entities/WillWhisper")
    quest:FinalizeEntityBindings()
    cVar4 = quest:IsLevelLoaded("GuildWoods")
    while not cVar4 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        cVar4 = quest:IsLevelLoaded("GuildWoods")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        pCVar20 = "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02"
        quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "")
        quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Will_Init: lift it as function WatchForTermination(quest)
        if (uVar13 & 2) ~= 0 then
            uVar13 = uVar13 & 0xfffffffd
        end
        quest:CreateThread("DoMission")  -- native thread body 0x00D68AE0: lift it as function DoMission(quest)
        if (uVar13 & 4) ~= 0 then
        end
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", nil --[[missing]])
        r1 = quest:GetThingWithScriptName("WillWhisper")
        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_88);
        if bVar5 then
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
        while cVar4 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d685d5 end
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar4 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            puVar18 = 0x0
            puVar19 = 0x0
            r2 = quest:GetAllThingsWithScriptName("WillBandit")
            iVar11 = puVar17 - ppVar10
            -- TODO(native): apCStack_60[0] = (CScriptGameResourceObjectScriptedThingBase *)0x0;
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_4c);
            -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)&uStack_68,(pair<long,long> *)(iVar11 / 0xc));
            iVar11 = puVar17 - ppVar10 >> 0x1f
            uVar13 = 0
            if (puVar17 - ppVar10) / 0xc + iVar11 ~= iVar11 then
                iVar11 = 0
                repeat
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: ppVar10 + iVar11
                    iVar11 = iVar11 + 0xc
                    uVar13 = uVar13 + 1
                until not (uVar13 < ((puVar17 - ppVar10) / 0xc))
            end
            iVar11 = 0
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_84);
            if bVar5 then
            end
            uVar8 = quest:GetHero()
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: uVar8
            cVar4 = nil --[[unresolved native result]]
            while cVar4 == 0 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then return end  -- TODO(native): goto LAB_00d67db1
                r3 = quest:GetHero()
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                cVar4 = nil --[[unresolved native result]]
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                -- LAB_00d67db1: (native jump target)
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)apCStack_60);
            else
                -- TODO(native): StdMap_Construct_API();
                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff44);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                pCVar12 = apCStack_60[0]
                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff44);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                pCVar12 = apCStack_60[0] + 0x10
                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff44);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                pCVar12 = apCStack_60[0] + 0x20
                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff44);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff44);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_54);
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(false)
                quest:FixMovieSequenceCamera(false)
                ppVar15 = 0x0
                -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): StdMap_Destroy_API();
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)apCStack_60);
                iVar2 = puVar19 - puVar18 >> 0x1f
                uVar13 = 0
                if (puVar19 - puVar18) / 0xc + iVar2 ~= iVar2 then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d685cc end
                        r4 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(r4, r3)
                        ppVar15 = ppVar10 + iVar11
                        fVar14 = quest:GetHealth(uVar8)
                        quest:ModifyThingHealth(r1, ppVar15, (_DAT_0124e640 - fVar14))
                        -- TODO(native): (**(code **)(*(int *)((int)&ppuStack_88 + iVar11) + 0x118))(0);
                        uVar13 = uVar13 + 1
                        iVar11 = iVar11 + 0xc
                    until not (uVar13 < ((puVar19 - puVar18) / 0xc))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff6c);
                    if bVar5 then
                    end
                    CVar1 = quest:GetStateBool("BanditsAlive")
                    while CVar1 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d685cc end
                        cVar4 = quest:IsLevelLoaded("GuildWoods")
                        if not cVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d685cc end
                            cVar4 = quest:IsLevelLoaded("GuildWoods")
                            while not cVar4 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00d685cc end
                                cVar4 = quest:IsLevelLoaded("GuildWoods")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d685cc end
                        end
                        if puVar18 ~= puVar19 then
                            repeat
                                puVar19 = puVar19 + -3
                                -- TODO(native): (**(code **)*puVar19)();
                            until not (puVar18 ~= puVar19)
                        end
                        r5 = quest:GetAllThingsWithScriptName("WillBandit")
                        iVar11 = puVar19 - puVar18 >> 0x1f
                        if (puVar19 - puVar18) / 0xc + iVar11 == iVar11 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d685cc end
                            quest:SetStateBool("BanditsAlive", false)
                        end
                        -- TODO(native): iVar2 = DAT_0143e90c;
                        r6 = quest:GetHero()
                        fVar14 = quest:GetHealth(r6)
                        if fVar14 < *(iVar2 + 0xed8) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d685cc end
                            ppVar10 = quest:AddNewConversation(nil --[[missing]], false, false)
                            r7 = quest:GetHero()
                            quest:AddPersonToConversation(ppVar10, r7)
                            uVar8 = quest:GetHero()
                            quest:AddLineToConversation(ppVar10, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", uVar8, nil --[[missing]], false)
                            cVar4 = quest:IsXbox()
                            if not cVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00d685cc end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC")
                                cVar4 = quest:MsgIsGameInfoClickedPast()
                                while not cVar4 do
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00d685cc end
                                    cVar4 = quest:MsgIsGameInfoClickedPast()
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00d685cc end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP")
                                cVar4 = quest:MsgIsGameInfoClickedPast()
                                while not cVar4 do
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00d685cc end
                                    cVar4 = quest:MsgIsGameInfoClickedPast()
                                end
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d685cc end
                            quest:SetStateBool("WhisperAnimate", true)
                            quest:ChangeHeroHealthBy(puVar18, 4, false)
                        end
                        CVar1 = quest:GetStateBool("BanditsAlive")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff6c);
                        if bVar5 then
                        end
                        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                        cVar4 = nil --[[unresolved native result]]
                        while cVar4 == 0 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00d685cc end
                            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                            cVar4 = nil --[[unresolved native result]]
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&uStack_7c);
                            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&uStack_7c);
                            if bVar5 then
                            end
                            r8 = quest:GetHero()
                            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                            cVar4 = nil --[[unresolved native result]]
                            while cVar4 == 0 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then return end  -- TODO(native): goto LAB_00d6849b
                                r9 = quest:GetHero()
                                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                                cVar4 = nil --[[unresolved native result]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00d6849b: (native jump target)
                            else
                                -- TODO(native): StdMap_Construct_API();
                                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff4c);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                                -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_6c,(CCharString *)&stack0xffffff4c);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar12);
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_54);
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                quest:FixMovieSequenceCamera(nil --[[missing]])
                                ppVar10 = 0x0
                                -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): StdMap_Destroy_API();
                                quest:SetStateBool("MissionSucceeded", true)
                            end
                        end
                    end
                end
            end
            ::LAB_00d685cc::
        end
        ::LAB_00d685d5::
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
            -- TODO(native): puStack_18 = auStack_8;
            ppVar4 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(ppVar4, true, nil --[[missing]], true)
        end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
    end
end

function DoMission(quest)
    local bVar2
    local alive = true
    local pCVar4 = "first objective"
    quest:GiveHeroNewQuestObjective("first objective", 0)
    local cVar1 = quest:IsLevelLoaded("GuildWoods")
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:CreateThread("WatchForLeaving")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
                if (unaff_ESI & 1) ~= 0 then
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if (not bVar2) and (not quest:GetStateBool("MissionFailed")) then
                    EndMission(quest)
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

function EndMission(quest)
    local bVar2
    local alive = true
    local CVar1 = quest:GetStateBool("MissionOver")
    while true do
        if CVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:SetStateBool("MissionSucceeded", true)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        CVar1 = quest:GetStateBool("MissionOver")
    end
end

