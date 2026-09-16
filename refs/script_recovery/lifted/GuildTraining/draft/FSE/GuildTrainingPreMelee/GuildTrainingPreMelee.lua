-- Generated native draft: Q_GuildTrainingPreMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar1, bVar8, cVar4, iVar7, pCVar10, pCVar11, ppVar9, r1, r2, r3, r4, r5, r6, uVar3
    local alive = true
    -- TODO(native): appuStack_84[0] = (undefined **)0x0;
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingPreMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("PreMeleeDummy", "GuildTrainingPreMelee/Entities/PreMeleeDummy")
    quest:AddEntityBinding("PreMeleeWhisper", "GuildTrainingPreMelee/Entities/PreMeleeWhisper")
    quest:FinalizeEntityBindings()
    quest:SetMasterGameState("GuildWarningOccuring", true)
    r1 = quest:GetThingWithScriptName("PreMeleeMaze")
    r2 = quest:GetThingWithScriptName("PreMeleeWhisper")
    -- TODO(native): ppVar12 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)appuStack_84;
    r3 = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    quest:SetStateInt("PreMeleeMode", 0)
    -- TODO(native): bVar8 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_78);
    if bVar8 then
    end
    -- TODO(native): ppVar12 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)appuStack_84;
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    cVar4 = nil --[[unresolved native result]]
    while cVar4 == 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar8 = not alive
        if bVar8 then
            if (ppVar9 ~= nil) and (*ppVar9 = *ppVar9 + -1, *ppVar9 == 0) then
                -- TODO(native): (**(code **)(ppVar9 + 4))();
            end
            return
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar8 = not alive
    if not bVar8 then
        -- TODO(native): bVar8 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_74);
        if bVar8 then
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
        while cVar4 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if bVar8 then goto LAB_00d51951 end
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar4 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar8 = not alive
        if not bVar8 then
            -- TODO(native): bVar8 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_64);
            if bVar8 then
            end
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar4 = nil --[[unresolved native result]]
            while cVar4 == 0 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                if bVar8 then goto LAB_00d51948 end
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                cVar4 = nil --[[unresolved native result]]
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if not bVar8 then
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_48);
                -- TODO(native): bVar8 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_48);
                if bVar8 then
                end
                r4 = quest:GetHero()
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                cVar4 = nil --[[unresolved native result]]
                while cVar4 == 0 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then goto LAB_00d5193f end
                    r5 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    cVar4 = nil --[[unresolved native result]]
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                if not bVar8 then
                    -- TODO(native): StdMap_Construct_API();
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_54,(CCharString *)&stack0xffffff50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6, (CScriptGameResourceObjectScriptedThingBase *)pCVar10);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_54,(CCharString *)&stack0xffffff50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar11);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_54,(CCharString *)&stack0xffffff50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar11);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_54,(CCharString *)&stack0xffffff50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar11);
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_38);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    quest:FixMovieSequenceCamera(false)
                    ppVar9 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    ppVar9 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    ppVar9 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    quest:FixMovieSequenceCamera(false)
                    quest:ChangeHeroHealthBy(0x447a0000, true, false)
                    quest:ResetPlayerCreatureCombatMultiplier()
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): StdMap_Destroy_API();
                    quest:SetStateBool("WhisperCutsceneFinished", true)
                    quest:SetStateBool("GuildmasterTeleport", true)
                    quest:SetMasterGameState("GuildWarningOccuring", false)
                    -- TODO(native): CTimer::CTimer((CTimer *)&uStack_104);
                    quest:SetTimer(uStack_104, 5)
                    iVar7 = quest:GetTimer(ppVar9)
                    while 0 < iVar7 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then goto LAB_00d51dd9 end
                        iVar7 = quest:GetTimer(0)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if not bVar8 then
                        quest:GiveHeroExpression("EXPRESSION_FART", 0)
                        quest:GiveHeroExpression("EXPRESSION_BELCH", 0x0)
                        quest:GiveHeroExpression("EXPRESSION_GIGGLE", 0x0)
                        CVar1 = quest:GetStateBool("HeroSleeps")
                        while not CVar1 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then goto LAB_00d51dd9 end
                            CVar1 = quest:GetStateBool("HeroSleeps")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if not bVar8 then
                            r6 = quest:GetActiveQuestName()
                            quest:DeactivateQuestLater(r6, 0)
                        end
                    end
                    ::LAB_00d51dd9::
                    goto LAB_00d51de2
                end
                ::LAB_00d5193f::
            end
            ::LAB_00d51948::
        end
        ::LAB_00d51951::
    end
    ::LAB_00d51de2::
end

function Init(quest)
    quest:SetStateBool("HeroSleeps", false)
    quest:SetStateBool("WhisperCutsceneFinished", false)
    quest:SetStateBool("GuildmasterTeleport", false)
    quest:SetStateBool("WhisperStopFollowing", false)
end

