-- Generated native draft: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local pCVar5
    local alive = true
    quest:AddEntityBinding("GuardTeamSpawn", "OrchardFarmRaid/Entities/TeamSpawn")
    local bVar7 = not bVar8 and bVar8
    quest:AddEntityBinding("BanditTeamSpawn", "OrchardFarmRaid/Entities/TeamSpawn")
    quest:AddEntityBinding("Artefact", "OrchardFarmRaid/Entities/Artefact")
    quest:AddEntityBinding("OrchardFarmWhisper", "OrchardFarmRaid/Entities/OrchardFarmWhisper")
    quest:AddEntityBinding("GuardTeamMember", "OrchardFarmRaid/Entities/CrateTeamMember")
    quest:AddEntityBinding("BanditTeamMember", "OrchardFarmRaid/Entities/CrateTeamMember")
    quest:AddEntityBinding("FarmRearEntrance", "OrchardFarmRaid/Entities/FarmRearEntrance")
    quest:AddEntityBinding("M_WhisperFarmRaidIntro", "OrchardFarmRaid/Entities/M_WhisperFarmRaidIntro")
    quest:AddEntityBinding("MK_OFI_GWLL_WHIS2", "OrchardFarmRaid/Entities/MK_OFI_GWLL_WHIS2")
    quest:FinalizeEntityBindings()
    local bVar8 = quest:IsQuestActive("Q_OrchardFarm_Barricade")
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
                quest:SetStateThing("Teams_1_CrateDropPos", pCVar5)
                pCVar5 = quest:GetThingWithScriptName("GuardTeamCrateDrop")
                quest:SetStateThing("Teams_0_CrateDropPos", pCVar5)
                if quest:GetStateInt("HeroTeam") == 1 then
                    quest:CreateThread("ProcessGameRulesEvil")  -- native thread body 0x00DD03D0: lift it as function ProcessGameRulesEvil(quest)
                else
                    quest:CreateThread("ProcessGameRulesGood")  -- native thread body 0x00DD0F60: lift it as function ProcessGameRulesGood(quest)
                end
                if not bVar8 then
                end
                quest:CreateThread("DoCutsceneIfRequired")  -- native thread body 0x00DCFA60: lift it as function DoCutsceneIfRequired(quest)
                if not bVar8 then
                end
                quest:CreateThread("WatchForExternalScriptDeactivation")  -- native thread body CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation: lift it as function WatchForExternalScriptDeactivation(quest)
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
    quest:SetStateThing("Teams_0_TeamCrateCarrier", nil)
    quest:SetStateThing("Teams_1_TeamCrateCarrier", nil)
    -- TODO(native): CCharString__AssignFromWide(this + 0x68,0x12db0c8);
    -- TODO(native): CCharString__AssignFromWide(this + 0x6c,0x12db088);
    -- TODO(native): CCharString__AssignFromWide(this + 0x70,0x12db040);
    -- TODO(native): CCharString__AssignFromWide(this + 0x74,0x12db000);
    local bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
    if bVar2 then
        quest:SetStateInt("HeroTeam", 1)
        -- TODO(native): name field 0x84 (CCharString)
        quest:SetStateString("self_0x84", "TEXT_QST_051_")
        -- TODO(native): CCharString::CCharString((CCharString *)&local_1c,&DAT_0122d70e,-1);
        quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "HeroGuildComplexInside", nil --[[missing]])
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false)
    else
        bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
        if not bVar2 then
            return
        end
        quest:SetStateInt("HeroTeam", 0)
        -- TODO(native): name field 0x84 (CCharString)
        quest:SetStateString("self_0x84", "TEXT_QST_052_")
        -- TODO(native): CCharString::CCharString((CCharString *)&CStack_20,&DAT_0122d70e,-1);
        quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "Greatwood", "GreatwoodEntrance")
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", false)
    end
    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true)
end

function OnPersist(quest, context)
end

function ProcessGameRulesEvil(quest)
    local resources = quest:RetailResources()
    local bVar4, cStack_c1, cVar1, id, pCVar5, pCVar7, pMessage, pPosition, r1, r2, r3, r4, r5, r6, uStack_138, uStack_b0, uStack_f0
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
        id = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            repeat
                quest:UpdateQuestInfoCounter(id, 3 - quest:GetStateInt("CrateCount"), -1)
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
                    pCVar5 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM")
                    bVar4 = false
                    pCVar7 = "OrchardFarmWhisper"
                    pPosition = pCVar5:GetPos()
                    r1 = quest:CreateCreature(pCVar7, pPosition, "GuardTeamSpawn")
                    quest:RemoveQuestInfoElement(id)
                    quest:MiniMapAddMarker(r1, "HUD_ORB_RED_SMALL")
                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                    r2 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    r3 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: (CScriptThing *)auStack_64,appuStack_ac,4
                    -- TODO(native): pppuVar9 = &ppuStack_bc;
                    pCVar5 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: pCVar5,pppuVar9,ePriority
                    -- TODO(native): StdMap_Construct_API(amStack_80);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_50,&DAT_01255174,-1);
                    -- TODO(native): pppuVar9 = &ppuStack_bc;
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_80,&CStack_50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pppuVar9);
                    -- TODO(native): pppuVar9 = appuStack_ac;
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_80,&CStack_4c);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pppuVar9);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_24,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    r4 = quest:GetHero()
                    -- TODO(native): GetSquaredDistanceBetweenThings(auStack_78);
                    -- TODO(native): fStack_14 = (float)extraout_ST0;
                    r5 = quest:GetHero()
                    -- TODO(native): GetSquaredDistanceBetweenThings(&uStack_90);
                    if fStack_14 <= extraout_ST0_00 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            -- TODO(native): RunCutsceneMacro_Func(aCStack_30,&CStack_8c.field_0x8,(void *)0x0,(void *)0x0,false,true);
                            pCVar7 = "CS_ORCHARD_EVIL_WHISPER_BACK"
                            goto LAB_00dd08eb
                        end
                        -- TODO(native): goto LAB_00dd0b1f
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        -- LAB_00dd0b11: (native jump target)
                        pCVar5 = 0x0
                        quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
                        -- LAB_00dd0b2b: (native jump target)
                        -- TODO(native): StdMap_Destroy_API(&CStack_8c.field_0x4);
                        return
                    end
                    -- TODO(native): CCharString::CCharString((CCharString *)(auStack_64 + 8),"CS_ORCHARD_EVIL_WHISPER_FRONT",-1);
                    -- TODO(native): RunCutsceneMacro_Func((CCharString *)(auStack_64 + 8),&CStack_8c.field_0x8,(void *)0x0,(void *)0x0 ,false,true);
                    pCVar7 = (r1 + 8)
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
                            pCVar5 = 0x0
                            quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
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
                pCVar5 = quest:GetHero()
                cStack_c1 = pCVar5:MsgIsKilledBy("CS_ORCHARD_EVIL_WHISPER_BACK")
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
                    quest:GiveHeroExperience(4)
                    r6 = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    -- TODO(native): appuStack_ac[0] = (undefined **)0x0;
                    uStack_f0 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    uStack_b0 = resources:NewActorMap()
                    resources:SetActor(uStack_b0, "HERO", &stack0xffffff30)
                    resources:SetActor(uStack_b0, "WHISPER", &stack0xffffff20)
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff14,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", 0x0, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (&CStack_c8);
                    resources:DestroyActorMap(CStack_c0)
                    -- TODO(native): CSubtitleRenderer::SetText(0x55);
                    quest:GiveHeroMorality(4)
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(0x0)
                    pCVar5 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar5, false)
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
                    pMessage = (this + *(this + 100) * 4 + 0x68)
                    bVar4 = true
                    pCVar7 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar7, bVar4, "Q_OrchardFarmRaid", (pMessage ~= 0))
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
    local resources = quest:RetailResources()
    local bVar3, cStack_c5, cVar1, iVar10, iVar4, native_arg_sequence_1, pCVar5, pCVar7, pCVar8, pMessage, pOther_00, pPosition, puVar9, r1, r2, r3, r4, r5, r6, r7, uStack_13c
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
                    iVar10 = 4
                    pCVar5 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: pCVar5,puVar9,iVar10
                    -- TODO(native): StdMap_Construct_API(amStack_8c);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_38,&DAT_01255174,-1);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_8c,&CStack_38);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,puVar9);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_8c,&CStack_50);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pOther_00);
                    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_60,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    r4 = quest:GetHero()
                    -- TODO(native): GetSquaredDistanceBetweenThings(&uStack_b8);
                    -- TODO(native): aCStack_1c[0] = (CCharString)(float)extraout_ST0;
                    r5 = quest:GetHero()
                    -- TODO(native): GetSquaredDistanceBetweenThings(auStack_ac);
                    if aCStack_1c[0] <= extraout_ST0_00 then
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
                        resources:DestroyActorMap(0)
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
                    quest:GiveHeroExperience(iVar10)
                    r6 = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_cc);
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)(auStack_a8 + 4));
                    r7 = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): CStack_fc = (CCharString)((int)auStack_b4 + 4);
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    -- TODO(native): StdMap_Construct_API(amStack_c8);
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff10,&DAT_01255174,-1);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_c8,(CCharString *)&stack0xffffff10);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,puVar9);
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_c8,(CCharString *)&stack0xffffff10);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pOther);
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a0);
                    -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff10,&DAT_0122d70e,-1);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:RunMacro("CS_ORCHARD_GOOD_OUTRO", stack0xffffff2c, false, true)
                    quest:PauseAllNonScriptedEntities(4)
                    resources:DestroyActorMap(stack0xffffff28)
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
                    quest:SetThingAsUsable(pCVar5, false)
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
                    pMessage = (this + *(this + 100) * 4 + 0x68)
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar8, true, "WHISPER", (pMessage ~= 0))
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
    local resources = quest:RetailResources()
    local CStack_bc, CStack_e8, bVar2, pCVar3, piStack_88, piVar6, r1, r2, stack0xffffff60, uVar5
    local alive = true
    if quest:GetStateInt("HeroTeam") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00dd03b8: (native jump target)
            return
        end
        pCVar3 = quest:GetThingWithScriptName("BanditTeamSpawn")
        piVar6 = *(pCVar3 + 0x8)
        uVar5 = *(pCVar3 + 0x4)
        if 0x0 == piVar6 then goto LAB_00dcfb65 end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end  -- TODO(native): goto LAB_00dd03b8
        pCVar3 = quest:GetThingWithScriptName("GuardTeamSpawn")
        piVar6 = *(pCVar3 + 0x8)
        uVar5 = *(pCVar3 + 0x4)
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
                stack0xffffff60 = resources:NewActorMap()
                resources:SetActor(stack0xffffff60, "HERO", &CStack_64)
                resources:SetActor(stack0xffffff60, "BAN1", &CStack_94)
                resources:SetActor(stack0xffffff60, "BAN2", pCVar3.field_0x8)
                resources:SetActor(stack0xffffff60, "GUARD", &auStack_54)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&CStack_84);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff28,&DAT_0122d70e,-1);
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(false)
                quest:FixMovieSequenceCamera(4)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", stack0xffffff50, false, true)
                quest:FixMovieSequenceCamera(0x4)
                pCVar3 = 0x0
                quest:PauseAllNonScriptedEntities((pCVar3 ~= 0))
                resources:DestroyActorMap(pCStack_b8)
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
                piStack_88 = resources:NewActorMap()
                resources:SetActor(piStack_88, "HERO", &uStack_68)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_58);
                -- TODO(native): CCharString::CCharString((CCharString *)&stack0xffffff54,&DAT_0122d70e,-1);
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                -- TODO(native): RunCutsceneMacro_Func(&CStack_bc,&stack0xffffff68,(void *)0x0,(void *)0x0,false,true);
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyActorMap(stack0xffffff60)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", nil --[[missing]], nil --[[missing]])
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

function MakeTeamMemberComment(quest, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local alive = true
    -- TODO(native): uStack_14 = *(undefined4 *)(this + 0x4c);
    local iVar1 = quest:GetTimer(nil --[[missing]])
    if 0 < iVar1 then
        alive = not quest:IsActiveThreadTerminating()
        return extraout_var << 8
    end
    local uVar2 = quest:AddNewConversation(nil --[[missing]], false, false)
    local uStack_30 = quest:GetHero()
    quest:AddPersonToConversation(nil --[[missing]], uStack_30)
    local uStack_38 = quest:GetHero()
    -- TODO(native): iVar3 = (**(code **)(*(int *)native_arg_comment_to_make + 0xc))(p0,&DAT_01244db4,uStack_14,0);
    -- TODO(native): CCharString__AppendData(&uStack_24,iVar3);
    -- TODO(native): CCharString__AppendCString(&uStack_28,(int)p0);
    local iVar3 = CCharString__AppendData(&uStack_14,p0_00)
    quest:AddLineToConversation(uVar2, iVar3, uStack_38, nil --[[missing]])
    uVar2 = quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return CONCAT31((int3)(uVar2 >> 8),1)
end

function DoMultiplierCutscene(quest)
    local resources = quest:RetailResources()
    local bVar4, local_3c, native_arg_sequence_1, native_arg_sequence_2, pCVar5, pScriptObject, puStack_38
    local alive = true
    pCVar5 = quest:GetHero()
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: pCVar5,pScriptObject,ePriority
    puStack_38 = resources:NewActorMap()
    resources:SetActor(puStack_38, "HERO", &local_c)
    -- TODO(native): CCharString::CCharString((CCharString *)&CStack_40,&DAT_0122d70e,-1);
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
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
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): local_3c = (CScriptThing *)piVar3;
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, 20.0)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00dd1d15
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
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
                -- LAB_00dd1d98: (native jump target)
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", puStack_38, false, true)
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
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00dd1e53
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
    resources:DestroyActorMap(puStack_38)
    return extraout_EAX
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
    return extraout_EAX
end

