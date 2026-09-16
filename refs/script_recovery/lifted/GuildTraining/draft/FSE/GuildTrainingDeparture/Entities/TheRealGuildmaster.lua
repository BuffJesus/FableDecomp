-- Generated native draft: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local bVar5, cVar6, fVar11, fVar4, pCVar1, pCVar10, pCVar13, pCVar14, pCVar15, pCVar18, pcVar12, piVar19, ppVar8, ppuStack_a8, r1, r2
    local alive = true
    quest:FadeScreenOut(0x3f000000, 0)
    quest:SetThingHasInformation(me)
    quest:EntitySetAsKillable(me, false)
    -- TODO(native): ppuStack_90 = *(undefined ***)(this + 0xc);
    -- TODO(native): ppuStack_8c = *(undefined ***)(this + 0x10);
    if ppuStack_8c ~= nil then
        -- TODO(native): *ppuStack_8c = (undefined *)((int)*ppuStack_8c + 1);
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    -- TODO(native): SetHeroGuideToShowQuestCardsWhenSpokenTo is not a ForgeFSE binding
    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo()
    ppuStack_a8 = quest:GetThingWithScriptName("M_DepartureTeacherStand")
    quest:EntityTeleportToThing(ppuStack_a8, nil --[[missing]])
    if (pCStack_84 ~= nil) and (*pCStack_84 = *pCStack_84 + -1, *pCStack_84 == 0) then
        -- TODO(native): (**(code **)(pCStack_84 + 4))();
    end
    -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_80);
    if bVar5 then
    end
    -- TODO(native): ppVar17 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar1;
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        cVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        if not cVar6 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d5134c end
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_7c);
            if bVar5 then
            end
            ppVar8 = quest:GetHero()
            cVar6 = me:AcquireControl(4)
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d50ebd
                r1 = quest:GetHero()
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d50ebd: (native jump target)
                return
            end
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&ppCStack_a4,(CCharString *)&pppuStack_b4);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar16);
            -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&ppCStack_a4,(CCharString *)&pppuStack_b4);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar9,pCVar16);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&pCStack_78);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            quest:FixMovieSequenceCamera(false)
            ppVar8 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_09", "GuildWoods", "")
            quest:ActivateQuest("Q_GuildTrainingWoodsDeparture")
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsDeparture", false)
            quest:SetMasterGameState("HeroTakingGuildTest", true)
        end
        cVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        while cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d5134c end
            cVar6 = me:IsTalkedToByHero()
            if cVar6 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d5134c end
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_98);
                fVar11 = quest:GetHealth(r1)
                fVar4 = _DAT_0122dedc
                piVar19 = 0x0
                if fVar4 < fVar11 then
                    bVar5 = false
                    pCVar15 = 0x1
                    pCVar14 = 0x0
                    pCVar13 = 0x0
                    pcVar12 = "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_MOAN"
                    pCVar10 = quest:GetHero()
                    r2 = me:Speak(pCVar10, pcVar12, pCVar13, (pCVar14 ~= 0), (pCVar15 ~= 0), bVar5)
                    bVar5 = me:IsPerformingScriptTask()
                    if bVar5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities((piVar19 ~= 0))
                                return
                            end
                            bVar5 = me:IsPerformingScriptTask()
                        until not (bVar5)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
            end
            cVar6 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:ClearThingHasInformation(nil --[[missing]])
        end
    end
    ::LAB_00d5134c::
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HeroSpokenToMe", false)
    __native_entity_state:SetStateBool("TeleportToWoods", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

