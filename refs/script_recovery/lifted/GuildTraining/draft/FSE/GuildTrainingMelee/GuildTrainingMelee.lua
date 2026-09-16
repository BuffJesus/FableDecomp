-- Generated native draft: Q_GuildTrainingMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar15, aVar14, bVar7, cVar2, iVar1, native_arg_sequence_1, pCVar11, piVar5, ppVar13, ppVar4, pppuStack_c0, r1, r2, uStack_78, uStack_98, uVar12
    local alive = true
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("MeleeOpponent", "GuildTrainingMelee/Entities/MeleeOpponent")
    quest:AddEntityBinding("MeleeThunder", "GuildTrainingMelee/Entities/MeleeThunder")
    quest:FinalizeEntityBindings()
    iVar1 = *piVar5
    -- TODO(native): puStack_74 = auStack_2c;
    uStack_78 = quest:GetThingWithScriptName("TheRealGuildmaster")
    ppVar4 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(ppVar4, uStack_78)
    -- TODO(native): pppuVar17 = &ppuStack_48;
    piVar5 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE")
    uStack_98 = piVar5:GetPos()
    r1 = quest:CreateCreature("MeleeOpponent", uStack_98, "MK_GTWU_WHISPER")
    quest:EntitySetInFaction(r1, "FACTION_HERO")
    if ppVar4 ~= nil then
        -- TODO(native): (**(code **)(*(int *)ppVar4 + 0x10c))();
    end
    -- TODO(native): bVar7 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffffa0);
    if bVar7 then
    end
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    cVar2 = nil --[[unresolved native result]]
    while cVar2 == 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if bVar7 then
            if PTR__scalar_deleting_destructor__0127094c == nil then
                -- TODO(native): (*(code *)PTR__Validate_CScriptGameResourceObjectScriptedThingBase__MBEXXZ_01270950)();
            end
            if (pppuVar17 ~= nil) and (*pppuVar17 = (*pppuVar17 + -1), *pppuVar17 == nil) then
            end
            return
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar2 = nil --[[unresolved native result]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar7 = not alive
    if not bVar7 then
        quest:EntitySetAllowBossPhaseChanges(piVar5, false)
        -- TODO(native): bVar7 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff9c);
        if bVar7 then
        end
        r2 = quest:GetHero()
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar2 = nil --[[unresolved native result]]
        while cVar2 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if bVar7 then goto LAB_00d56509 end
            pppuStack_c0 = quest:GetHero()
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar2 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if not bVar7 then
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_78,(CCharString *)&puStack_9c);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar16);
            -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_78,(CCharString *)&puStack_9c);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar16);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            CVar15 = 0x0
            aVar14 = 0x0
            ppVar13 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            uVar12 = 0
            quest:FixMovieSequenceCamera((uVar12 ~= 0))
            quest:SetStateBool("TalkedToWhisper", true)
            pCVar11 = 0x0
            quest:PauseAllNonScriptedEntities((pCVar11 ~= 0))
            -- TODO(native): StdMap_Destroy_API();
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "")
            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)0x28,(CFontBank *)ppVar4,pCVar8,pCVar9,pCVar10,pCVar11,uVar12, (float)ppVar13,(bool)aVar14,(bool)CVar15);
            cVar2 = quest:DisplayTutorial(0x1a)
            native_arg_sequence_1 = false
            if cVar2 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar7 = not alive
                if not bVar7 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                cVar2 = quest:MsgIsTutorialClickedPast()
                while not cVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar7 = not alive
                    if bVar7 then goto LAB_00d56509 end
                    cVar2 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
            end
        end
        ::LAB_00d56509::
    end
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

