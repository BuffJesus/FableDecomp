-- Generated native draft: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local pCVar5, pCVar6, piVar1, piVar3, puVar4, uVar2
    local alive = true
    local bVar8 = puVar4 == nil
    if bVar8 then
        puVar4 = 0x0
    else
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 1;
    end
    bVar8 = not bVar8
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if bVar8 then
    end
    local bVar7 = not bVar8 and bVar8
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 2
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 1;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if (bVar7 & 2) ~= 0 then
        bVar7 = bVar7 & 0xfd
    end
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 4
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 1;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if (bVar7 & 4) ~= 0 then
        bVar7 = bVar7 & 0xfb
    end
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 8
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 1;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if (bVar7 & 8) ~= 0 then
        bVar7 = bVar7 & 0xf7
    end
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 0x10
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 1;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if (bVar7 & 0x10) ~= 0 then
        bVar7 = bVar7 & 0xef
    end
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 0x20
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 1;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if (bVar7 & 0x20) ~= 0 then
        bVar7 = bVar7 & 0xdf
    end
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 0x40
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 0;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if (bVar7 & 0x40) ~= 0 then
        bVar7 = bVar7 & 0xbf
    end
    if puVar4 == nil then
        puVar4 = 0x0
    else
        bVar7 = bVar7 | 0x80
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 0;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if bVar7 < 0 then
    end
    bVar8 = puVar4 == nil
    if bVar8 then
        puVar4 = 0x0
    else
        -- TODO(native): CCharString::CCharString((CCharString *)(puVar4 + 1),&local_14);
        -- TODO(native): puVar4[2] = this;
        -- TODO(native): *(undefined1 *)(puVar4 + 5) = 1;
        -- TODO(native): puVar4[6] = 0;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,puVar4);
    if not bVar8 then
    end
    quest:FinalizeEntityBindings()
    bVar8 = quest:IsQuestActive("Q_OrchardFarm_Barricade")
    if not bVar8 then
        quest:ActivateQuest("Q_OrchardFarm_Barricade")
    end
    bVar8 = quest:IsRegionLoaded("OrchardFarm")
    while true do
        if bVar8 then
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if not bVar8 then
                pCVar5 = quest:GetThingWithScriptName("BanditTeamCrateDrop")
                piVar1 = *&*(pCVar5 + 0x8)
                uVar2 = *&*(pCVar5 + 0x4)
                piVar3 = *(this + 0xf8)
                if piVar3 ~= piVar1 then
                    if piVar3 ~= nil then
                        -- TODO(native): *piVar3 = *piVar3 + -1;
                        if **(this + 0xf8) == 0 then
                            -- TODO(native): (*(code *)(*(int **)(this + 0xf8))[1])();
                        end
                    end
                    -- TODO(native): name field 0xf4 (undefined4)
                    quest:SetStateInt("self_0xf4", uVar2)
                    -- TODO(native): *(int **)(this + 0xf8) = piVar1;
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                pCVar5 = quest:GetThingWithScriptName("GuardTeamCrateDrop")
                piVar1 = *&*(pCVar5 + 0x8)
                uVar2 = *&*(pCVar5 + 0x4)
                piVar3 = *(this + 0xb8)
                if piVar3 ~= piVar1 then
                    if piVar3 ~= nil then
                        -- TODO(native): *piVar3 = *piVar3 + -1;
                        if **(this + 0xb8) == 0 then
                            -- TODO(native): (*(code *)(*(int **)(this + 0xb8))[1])();
                        end
                    end
                    -- TODO(native): name field 0xb4 (undefined4)
                    quest:SetStateInt("self_0xb4", uVar2)
                    -- TODO(native): *(int **)(this + 0xb8) = piVar1;
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                if quest:GetStateInt("HeroTeam") == 1 then
                    quest:CreateThread("ProcessGameRulesEvil")  -- native thread body 0x00DD03D0: lift it as function ProcessGameRulesEvil(quest)
                    -- TODO(native): CCharString::CCharString((CCharString *)&local_10,&DAT_0122d70e,-1);
                    -- TODO(native): CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,pCVar6,sectionName);
                else
                    quest:CreateThread("ProcessGameRulesGood")  -- native thread body 0x00DD0F60: lift it as function ProcessGameRulesGood(quest)
                    -- TODO(native): CCharString::CCharString((CCharString *)&local_10,&DAT_0122d70e,-1);
                    -- TODO(native): CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,pCVar6,sectionName_00);
                end
                if not bVar8 then
                end
                quest:CreateThread("DoCutsceneIfRequired")  -- native thread body 0x00DCFA60: lift it as function DoCutsceneIfRequired(quest)
                -- TODO(native): CCharString::CCharString((CCharString *)&local_10,&DAT_0122d70e,-1);
                -- TODO(native): CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,pCVar6,sectionName_01);
                if not bVar8 then
                end
                -- TODO(native): bVar8 = pCVar6 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
                if bVar8 then
                    pCVar6 = 0x0
                else
                    -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(pCVar6,&local_14,0);
                    -- TODO(native): *(code **)(pCVar6 + 0x34) = CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation;
                    -- TODO(native): *(CQ_GuildTrainingScript **)(pCVar6 + 0x38) = this;
                end
                -- TODO(native): CCharString::CCharString((CCharString *)&local_10,&DAT_0122d70e,-1);
                -- TODO(native): CGuiVarTransferStruct::Add((CGuiVarTransferStruct *)this,pCVar6,sectionName_02);
                if not bVar8 then
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar8 = not alive
        if bVar8 then break end
        bVar8 = quest:IsRegionLoaded("OrchardFarm")
    end
end

function Init(quest)
    -- TODO(native): CStack_20._3_1_ = GSI->IsQuestActive(&local_1c);
    if CStack_20._3_1_ == 0 then
        quest:AddQuestRegion("Q_OrchardFarmRaidEvil", "OrchardFarm")
        -- TODO(native): this_00 = &local_1c;
    else
        quest:AddQuestRegion("Q_OrchardFarmRaidGood", "OrchardFarm")
        -- TODO(native): this_00 = &CStack_18;
    end
    quest:SetMasterGameState("OrchardFarmGuardKilled", false)
    quest:SetMasterGameState("OrchardFarmBanditKilled", false)
    quest:SetStateBool("HeroMetWhisperBeforeFarm", false)
    quest:SetStateInt("CrateCount", 0)
    quest:SetMasterGameState("OFBRCratesStolen", false)
    quest:SetMasterGameState("OFBR_NoCratesWereStolen", false)
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
    quest:SetStateInt("Teams_0_EnemyTeam", 1)
    quest:SetStateBool("DoneIntroduction", false)
    quest:SetStateBool("HeroAtWrongEntrance", false)
    quest:SetStateBool("WhisperSpawned", false)
    quest:SetStateBool("MissionSucceeded", false)
    -- TODO(native): *(undefined4 *)(this + 100) = 0;
    quest:SetStateInt("BanditWavesSpawned", 0)
    quest:SetStateBool("ShownCombatMultiplierTutorial", false)
    quest:SetStateBool("ShownCombatFlourishTutorial", false)
    quest:SetStateBool("WhisperInCutscene", false)
    quest:SetStateInt("Teams_1_EnemyTeam", 0)
    -- TODO(native): quest:SetStateThing("Teams_0_TeamCrateCarrier", nil);
    -- TODO(native): quest:SetStateThing("Teams_1_TeamCrateCarrier", nil);
    -- TODO(native): CCharString__AssignFromWide(L"PROBLEM: Tell Ben problem with Orchard Farm fail reasons");
    -- TODO(native): CCharString__AssignFromWide(L"TEXT_QST_051_FAILED_HERO_KILLED");
    -- TODO(native): CCharString__AssignFromWide(L"TEXT_QST_051_FAILED_CRATES_STOLEN");
    -- TODO(native): CCharString__AssignFromWide(L"TEXT_QST_051_FAILED_TEAM_KILLED");
    local bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
    if bVar2 then
        quest:SetStateInt("HeroTeam", 1)
        -- TODO(native): name field 0x84 (CCharString)
        quest:SetStateString("self_0x84", "TEXT_QST_051_")
        -- TODO(native): CCharString::CCharString((CCharString *)&local_1c,&DAT_0122d70e,-1);
        quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "HeroGuildComplexInside", nil --[[missing]])
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false)
        -- TODO(native): string = "GreatwoodEntrance";
    else
        bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
        if not bVar2 then
            return
        end
        quest:SetStateInt("HeroTeam", 0)
        -- TODO(native): name field 0x84 (CCharString)
        quest:SetStateString("self_0x84", "TEXT_QST_052_")
        -- TODO(native): CCharString::CCharString((CCharString *)&CStack_20,&DAT_0122d70e,-1);
        quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "Greatwood", nil --[[missing]])
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", false)
        -- TODO(native): string = "GreatwoodLake";
    end
    -- TODO(native): CCharString::CCharString((CCharString *)&local_1c,string,-1);
    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", nil --[[missing]], true)
end

function OnPersist(quest, context)
    local shownCombatMultiplierTutorial = quest:GetStateBool("ShownCombatMultiplierTutorial") or false
    shownCombatMultiplierTutorial = quest:PersistTransferBool(context, "ShownCombatMultiplierTutorial", shownCombatMultiplierTutorial)
    quest:SetStateBool("ShownCombatMultiplierTutorial", shownCombatMultiplierTutorial)
    local heroMetWhisperBeforeFarm = quest:GetStateBool("HeroMetWhisperBeforeFarm") or false
    heroMetWhisperBeforeFarm = quest:PersistTransferBool(context, "HeroMetWhisperBeforeFarm", heroMetWhisperBeforeFarm)
    quest:SetStateBool("HeroMetWhisperBeforeFarm", heroMetWhisperBeforeFarm)
end

function ProcessGameRulesEvil(quest)
    local bVar4, cStack_c1, cVar1, fVar5, pCVar6, pCVar8, pPosition, r1, r2, r3, r4, uStack_138, uStack_f0
    local alive = true
    cVar1 = quest:GetStateBool("DoneIntroduction")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar1 = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:DisplayQuestInfo(true)
        fVar5 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            repeat
                quest:UpdateQuestInfoCounter(fVar5, 3 - quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetStateBool("WhisperSpawned", true)
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_54,&DAT_0122d70e,-1);
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "Greatwood", nil --[[missing]])
                    pCVar6 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM")
                    bVar4 = false
                    pCVar8 = "OrchardFarmWhisper"
                    pPosition = pCVar6:GetPos()
                    r1 = quest:CreateCreature(pCVar8, pPosition, "GuardTeamSpawn")
                    quest:RemoveQuestInfoElement(fVar5)
                    quest:MiniMapAddMarker(r1, "HUD_ORB_RED_SMALL")
                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                    r2 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    r3 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: (CScriptThing *)auStack_64,appuStack_ac,4
                    -- TODO(native): ePriority = 4;
                    -- TODO(native): pppuVar10 = &ppuStack_bc;
                    pCVar6 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: pCVar6,pppuVar10,ePriority
                    -- TODO(native): StdMap_Construct_API(amStack_80);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_50,&DAT_01255174,-1);
                    -- TODO(native): pppuVar10 = &ppuStack_bc;
                    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_80,&CStack_50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pppuVar10);
                    -- TODO(native): pppuVar10 = appuStack_ac;
                    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_80,&CStack_4c);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,pppuVar10);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_24,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    pCVar6 = (**(*this_00 + 0x118))(this_00)
                    -- TODO(native): fStack_14 = GetSquaredDistanceBetweenThings(aCStack_78,(CScriptThing *)pCVar6);
                    pCVar6 = (**(*this_00 + 0x118))(this_00)
                    fVar5 = GetSquaredDistanceBetweenThings(&uStack_90,pCVar6)
                    if fStack_14 <= fVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            -- TODO(native): RunCutsceneMacro_Func(aCStack_30,&CStack_8c.field_0x8,(void *)0x0,(void *)0x0,false,true);
                            pCVar8 = "CS_ORCHARD_EVIL_WHISPER_BACK"
                            goto LAB_00dd08eb
                        end
                        -- TODO(native): goto LAB_00dd0b1f
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        -- LAB_00dd0b11: (native jump target)
                        pCVar6 = 0x0
                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                        -- LAB_00dd0b2b: (native jump target)
                        -- TODO(native): StdMap_Destroy_API(&CStack_8c.field_0x4);
                        return
                    end
                    -- TODO(native): CCharString::CCharString((CCharString *)(auStack_64 + 8),"CS_ORCHARD_EVIL_WHISPER_FRONT",-1);
                    -- TODO(native): RunCutsceneMacro_Func((CCharString *)(auStack_64 + 8),&CStack_8c.field_0x8,(void *)0x0,(void *)0x0 ,false,true);
                    pCVar8 = (r1 + 8)
                    ::LAB_00dd08eb::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd0b11
                        bVar4 = quest:DisplayTutorial(0x12)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                bVar4 = quest:MsgIsTutorialClickedPast()
                                while not bVar4 do
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then return end  -- TODO(native): goto LAB_00dd0b11
                                    bVar4 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then goto LAB_00dd0977 end
                            end
                            -- LAB_00dd0b1f: (native jump target)
                            pCVar6 = 0x0
                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                            -- TODO(native): goto LAB_00dd0b2b
                        end
                        ::LAB_00dd0977::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): StdMap_Destroy_API(amStack_80);
                end
                -- TODO(native): CCharString::CCharString(aCStack_3c,&DAT_0122d70e,-1);
                pCVar6 = quest:GetHero()
                cStack_c1 = pCVar6:MsgIsKilledBy("CS_ORCHARD_EVIL_WHISPER_BACK")
                if cStack_c1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    -- TODO(native): *(undefined4 *)(this + 100) = 1;
                end
                if quest:GetStateBool("MissionSucceeded") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:GiveHeroExperience(0)
                    r4 = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    -- TODO(native): appuStack_ac[0] = (undefined **)0x0;
                    uStack_f0 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): StdMap_Construct_API(&uStack_b0);
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff14,&DAT_01255174,-1);
                    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_b0,(CCharString *)&stack0xffffff14);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,puVar9);
                    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_b0,(CCharString *)&stack0xffffff14);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar7,puVar9);
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff14,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): RunCutsceneMacro_Func(&CStack_f8,&ppuStack_bc,(void *)0x0,(void *)0x0,false,true);
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (&CStack_c8);
                    -- TODO(native): StdMap_Destroy_API(&CStack_c0);
                    -- TODO(native): CSubtitleRenderer::SetText(0x55);
                    quest:GiveHeroMorality(4)
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(0x0)
                    pCVar6 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar6, false)
                    uStack_138 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(uStack_138, false, false, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, nil --[[missing]])
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                    until not (not bVar4)
                    return
                end
                if *(this + 100) ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    -- TODO(native): b2 = true;
                    -- TODO(native): pMessage = (CWideString *)(this + *(int *)(this + 100) * 4 + 0x68);
                    bVar4 = true
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar8, bVar4, "Q_OrchardFarmRaid", pMessage)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
            until false
        end
    end
end

function ProcessGameRulesGood(quest)
    local bVar3, cStack_c5, cVar1, fVar9, iVar11, iVar4, native_arg_sequence_1, pCVar5, pCVar7, pCVar8, pPosition, puVar10, r1, r2, r3, r4, r5, uStack_13c
    local alive = true
    cVar1 = quest:GetStateBool("DoneIntroduction")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar1 = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:DisplayQuestInfo(true)
        iVar4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        -- TODO(native): CStack_10._0_4_ = iVar4;
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            repeat
                quest:UpdateQuestInfoCounter(iVar4, quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    -- TODO(native): *(undefined4 *)(this + 100) = 2;
                end
                -- TODO(native): CCharString::CCharString((CCharString *)auStack_c4,&DAT_0122d70e,-1);
                pCVar5 = quest:GetHero()
                pCVar7 = "HUD_QUEST_ICON_SMALL_CRATE"
                -- TODO(native): CStack_cc._3_1_ = CScriptThing::MsgIsKilledBy(pCVar5);
                if CStack_cc._3_1_ ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    -- TODO(native): *(undefined4 *)(this + 100) = 1;
                end
                if (2 < quest:GetStateInt("BanditWavesSpawned")) and (not quest:GetStateBool("WhisperSpawned")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetStateBool("WhisperInCutscene", true)
                    -- TODO(native): CCharString::CCharString((CCharString *)aCStack_4c,&DAT_0122d70e,-1);
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "Greatwood", "HUD_QUEST_ICON_SMALL_CRATE")
                    quest:RemoveQuestInfoElement(iVar4)
                    pCVar5 = quest:GetThingWithScriptName("BanditTeamSpawn")
                    bVar3 = false
                    pPosition = pCVar5:GetPos()
                    r1 = quest:CreateCreature("OrchardFarmWhisper", pPosition, "CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM")
                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                    quest:MiniMapAddMarker(pCVar5, "HUD_ORB_RED_SMALL")
                    r2 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    r3 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    -- TODO(native): auStack_74[0] = 0;
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: &CStack_6c,appuStack_80,4
                    iVar11 = 4
                    pCVar5 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: pCVar5,puVar10,iVar11
                    -- TODO(native): StdMap_Construct_API(amStack_8c);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_38,&DAT_01255174,-1);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_8c,&CStack_38);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,puVar10);
                    -- TODO(native): pOther_00 = appuStack_80;
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_8c,&CStack_50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pOther_00);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_60,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    pCVar5 = (**(*this_00 + 0x118))(this_00)
                    -- TODO(native): aCStack_1c[0] = (CCharString) GetSquaredDistanceBetweenThings((CScriptThing *)&uStack_b8,(CScriptThing *)pCVar5);
                    pCVar5 = (**(*this_00 + 0x118))(this_00)
                    fVar9 = GetSquaredDistanceBetweenThings(aCStack_ac,pCVar5)
                    if aCStack_1c[0] <= fVar9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            -- TODO(native): RunCutsceneMacro_Func(aCStack_34,&uStack_90,(void *)0x0,(void *)0x0,false,true);
                            pCVar8 = "CS_ORCHARD_GOOD_WHISPER_BACK"
                            goto LAB_00dd14ee
                        end
                        -- TODO(native): goto LAB_00dd1736
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00dd1728: (native jump target)
                        pCVar7 = 0x0
                        quest:PauseAllNonScriptedEntities((pCVar7 ~= 0))
                        -- LAB_00dd1742: (native jump target)
                        -- TODO(native): StdMap_Destroy_API(&uStack_94);
                        return
                    end
                    -- TODO(native): RunCutsceneMacro_Func(aCStack_4c,&uStack_90,(void *)0x0,(void *)0x0,false,true);
                    pCVar8 = "CS_ORCHARD_GOOD_WHISPER_FRONT"
                    ::LAB_00dd14ee::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then return end  -- TODO(native): goto LAB_00dd1728
                        bVar3 = quest:DisplayTutorial(0x12)
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                bVar3 = quest:MsgIsTutorialClickedPast()
                                while not bVar3 do
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then return end  -- TODO(native): goto LAB_00dd1728
                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then goto LAB_00dd157a end
                            end
                            -- LAB_00dd1736: (native jump target)
                            pCVar7 = 0x0
                            quest:PauseAllNonScriptedEntities((pCVar7 ~= 0))
                            -- TODO(native): goto LAB_00dd1742
                        end
                        ::LAB_00dd157a::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): StdMap_Destroy_API(amStack_8c);
                end
                native_arg_sequence_1 = false
                if quest:GetStateInt("CrateCount") == 0 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    return
                end
                -- TODO(native): CCharString::CCharString(aCStack_40,&DAT_0122d70e,-1);
                pCVar7 = quest:GetHero()
                cStack_c5 = pCVar7:MsgIsKilledBy("CS_ORCHARD_GOOD_WHISPER_FRONT")
                if cStack_c5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    -- TODO(native): *(undefined4 *)(this + 100) = 1;
                end
                if quest:GetStateBool("MissionSucceeded") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:GiveHeroExperience(iVar11)
                    r4 = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_cc);
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)(auStack_a8 + 4));
                    r5 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): CStack_fc = (CCharString)((int)auStack_b4 + 4);
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): StdMap_Construct_API(amStack_c8);
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff10,&DAT_01255174,-1);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_c8,(CCharString *)&stack0xffffff10);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,puVar10);
                    -- TODO(native): pOther = &uStack_bc;
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_c8,(CCharString *)&stack0xffffff10);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pOther);
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a0);
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff10,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(4)
                    -- TODO(native): RunCutsceneMacro_Func(&CStack_fc,&stack0xffffff2c,(void *)0x0,(void *)0x0,false,true);
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): StdMap_Destroy_API(&stack0xffffff28);
                    -- TODO(native): CSubtitleRenderer::SetText(0x50);
                    -- TODO(native): pCVar7 = *(CScriptThing **)(DAT_0143e90c + 0xd9c);
                    quest:GiveHeroMorality(0)
                    if quest:GetMasterGameState("OFBRCratesStolen") == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00dd1a93 end
                        quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00dd1a93 end
                    end
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
                    quest:RemoveQuestInfoElement(0)
                    pCVar5 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar5, nil --[[missing]])
                    uStack_13c = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(uStack_13c, false, false, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, nil --[[missing]], nil --[[missing]])
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                    until not (not bVar3)
                    ::LAB_00dd1a93::
                    return
                end
                if *(this + 100) ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    bVar3 = true
                    -- TODO(native): pMessage = (CWideString *)(this + *(int *)(this + 100) * 4 + 0x68);
                    -- TODO(native): b1 = (C3DClothPrimitive)0x1;
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar8, b1, "CS_ORCHARD_GOOD_OUTRO", pMessage)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
            until false
        end
    end
end

function DoCutsceneIfRequired(quest)
    local CStack_bc, CStack_e8, bVar2, pCVar3, piVar6, r1, r2, uVar5
    local alive = true
    if quest:GetStateInt("HeroTeam") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00dd03b8: (native jump target)
            return
        end
        pCVar3 = quest:GetThingWithScriptName("BanditTeamSpawn")
        piVar6 = *&*(pCVar3 + 0x8)
        uVar5 = *&*(pCVar3 + 0x4)
        if 0x0 == piVar6 then goto LAB_00dcfb65 end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
        pCVar3 = quest:GetThingWithScriptName("GuardTeamSpawn")
        piVar6 = *&*(pCVar3 + 0x8)
        uVar5 = *&*(pCVar3 + 0x4)
        if 0x0 == piVar6 then goto LAB_00dcfb65 end
    end
    if piVar6 ~= nil then
        -- TODO(native): *piVar6 = *piVar6 + 1;
    end
    ::LAB_00dcfb65::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            -- LAB_00dcfe47: (native jump target)
            return
        end
        bVar2 = quest:IsRegionLoaded("OrchardFarm")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                if true then return end  -- TODO(native): goto LAB_00dcfe47
                -- TODO(native): goto LAB_00dcfe3f
            end
            bVar2 = quest:IsRegionLoaded("OrchardFarm")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
        -- TODO(native): dist = 10.0;
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar3, nil --[[missing]], auStack_90)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
            if quest:GetStateInt("HeroTeam") == 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
                r1 = quest:GetAllThingsWithScriptName("BanditTeamMember")
                r2 = quest:GetNearestWithScriptName(nil --[[missing]], "GuardTeamMember")
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_64);
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_44);
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_34);
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_24);
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                -- TODO(native): pCStack_e4 = aCStack_58;
                CStack_e8 = quest:GetHero()
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                quest:SheatheHeroWeapons()
                -- TODO(native): StdMap_Construct_API(&stack0xffffff60);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff28,&DAT_01255174,-1);
                -- TODO(native): pCVar4 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff60,(CCharString *)&stack0xffffff28);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar4,pCVar7);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff28,&DAT_012d1f04,-1);
                -- TODO(native): pCVar4 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff60,(CCharString *)&stack0xffffff28);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar4,pCVar7);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff28,&DAT_012d1efc,-1);
                -- TODO(native): pOther = &CStack_7c.field_0x8;
                -- TODO(native): pCVar4 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff60,(CCharString *)&stack0xffffff28);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar4,pOther);
                -- TODO(native): pOther_00 = auStack_54;
                -- TODO(native): pCVar4 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff60,(CCharString *)&stack0xffffff28);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar4,pOther_00);
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_84);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff28,&DAT_0122d70e,-1);
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(false)
                quest:FixMovieSequenceCamera(4)
                -- TODO(native): RunCutsceneMacro_Func(&CStack_e8,&stack0xffffff50,(void *)0x0,(void *)0x0,false,true);
                quest:FixMovieSequenceCamera(0x4)
                pCVar3 = 0x0
                quest:PauseAllNonScriptedEntities((pCVar3 ~= 0))
                -- TODO(native): StdMap_Destroy_API(&pCStack_b8);
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", false, false)
                pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(pCVar3)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
                pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(pCVar3)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_5c);
                CStack_bc = quest:GetHero()
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                -- TODO(native): StdMap_Construct_API(&piStack_88);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff54,&DAT_01255174,-1);
                -- TODO(native): pOther_01 = &uStack_68;
                -- TODO(native): pCVar4 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&piStack_88,(CCharString *)&stack0xffffff54);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar4,pOther_01);
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_58);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff54,&DAT_0122d70e,-1);
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                -- TODO(native): RunCutsceneMacro_Func(&CStack_bc,&stack0xffffff68,(void *)0x0,(void *)0x0,false,true);
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): StdMap_Destroy_API(&stack0xffffff60);
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", false, nil --[[missing]])
            end
            quest:DeactivateQuest("Q_OrchardFarm_Barricade", 0)
            quest:Pause(0x3f800000)
            quest:SetStateBool("DoneIntroduction", true)
            uVar5 = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(uVar5, nil --[[missing]])
            -- TODO(native): goto LAB_00dd03b8
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
        quest:SetStateBool("HeroAtWrongEntrance", true)
        pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
        -- TODO(native): CloseDoor is not a ForgeFSE binding
        quest:CloseDoor(pCVar3)
        bVar2 = false
        pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
        quest:SetThingAsUsable(pCVar3, bVar2)
        quest:Pause(1.0)
        quest:DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE")
        bVar2 = quest:MsgIsGameInfoClickedPast()
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
            bVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
        bVar2 = quest:IsRegionLoaded("OrchardFarm")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
                bVar2 = quest:IsRegionLoaded("OrchardFarm")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
        quest:SetStateBool("HeroAtWrongEntrance", false)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until false
end

function WatchForExternalScriptDeactivation(quest)
    local bVar1, bVar2, bVar3
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    bVar1 = false
    repeat
        bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
        if bVar2 then
            -- LAB_00dccfa3: (native jump target)
            bVar2 = false
        else
            bVar1 = true
            bVar3 = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
            bVar2 = true
            if bVar3 then return end  -- TODO(native): goto LAB_00dccfa3
        end
        if bVar1 then
            bVar1 = false
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", bVar2, nil --[[missing]])
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", nil --[[missing]], nil --[[missing]])
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
    until false
end

function MakeTeamMemberComment(quest, native_arg_param_1, native_arg_param_2, native_arg_param_3)
    local alive = true
    local uVar3 = quest:GetStateInt("CommentTimer")
    local iVar1 = quest:GetTimer(nil --[[missing]])
    if 0 < iVar1 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    local uVar2 = quest:AddNewConversation(nil --[[missing]], false, false)
    local uStack_30 = quest:GetHero()
    quest:AddPersonToConversation(nil --[[missing]], uStack_30)
    local uStack_38 = quest:GetHero()
    -- TODO(native): uVar3 = (**(code **)(*(int *)native_arg_param_1 + 0xc))(puVar4,&DAT_01244db4,uVar3,0);
    -- TODO(native): CCharString__AppendData(uVar3);
    -- TODO(native): CCharString__AppendCString(puVar4);
    uVar3 = CCharString__AppendData(puVar5)
    quest:AddLineToConversation(uVar2, uVar3, uStack_38, nil --[[missing]])
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return true
end

function DoMultiplierCutscene(quest)
    local bVar4, local_3c, native_arg_sequence_1, native_arg_sequence_2, pCVar5, piVar2, piVar3
    local alive = true
    -- TODO(native): ePriority = 4;
    -- TODO(native): pScriptObject = local_10;
    pCVar5 = quest:GetHero()
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: pCVar5,pScriptObject,ePriority
    -- TODO(native): puStack_38 = malloc(0x24);
    -- TODO(native): *puStack_38 = 0;
    -- TODO(native): *(undefined4 *)(puStack_38 + 4) = 0;
    -- TODO(native): *(undefined1 **)(puStack_38 + 8) = puStack_38;
    -- TODO(native): *(undefined1 **)(puStack_38 + 0xc) = puStack_38;
    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,&DAT_01255174,-1);
    -- TODO(native): this_00 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&puStack_38,&CStack_40);
    -- TODO(native): CFourierAnalysis::CFourierAnalysis(this_00);
    piVar3 = 0x0
    piVar2 = *(this_00 + 0xc)
    if piVar2 ~= nil then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(this_00 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(this_00 + 0xc))[1])();
            end
        end
        -- TODO(native): *(CScriptThing **)(this_00 + 8) = local_3c;
        -- TODO(native): *(int **)(this_00 + 0xc) = piVar3;
        if piVar3 ~= nil then
            -- TODO(native): *piVar3 = *piVar3 + 1;
        end
    end
    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,&DAT_0122d70e,-1);
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(false)
    quest:FixMovieSequenceCamera(true)
    bVar4 = quest:IsRegionLoaded("GreatwoodLake")
    native_arg_sequence_1 = false
    if not bVar4 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if not native_arg_sequence_1 then
        bVar4 = true
        if quest:GetStateInt("HeroTeam") ~= 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        bVar4 = false
    end
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL";
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): dist = 20.0;
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, dist)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00dd1d15
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL";
                        -- TODO(native): goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00dd1e95
                end
                -- TODO(native): string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL";
                -- LAB_00dd1d98: (native jump target)
                -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,string,-1);
                -- TODO(native): RunCutsceneMacro_Func(&CStack_40,&puStack_38,(void *)0x0,(void *)0x0,false,true);
                quest:FixMovieSequenceCamera(false)
                native_arg_sequence_2 = false
                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if not native_arg_sequence_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if native_arg_sequence_2 then goto LAB_00dd1e70 end
                bVar4 = quest:DisplayTutorial(9)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                    bVar4 = quest:MsgIsTutorialClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                        bVar4 = quest:MsgIsTutorialClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00dd1e3d
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e3d
                        -- TODO(native): string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP";
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
                        -- TODO(native): string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP";
                    end
                    -- TODO(native): goto LAB_00dd1d98
                end
            end
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (&uStack_18);
    ::LAB_00dd1e95::
    -- TODO(native): StdMap_Destroy_API(&puStack_38);
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
end

