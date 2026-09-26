-- Generated native draft: Q_BanditCampBossBattle. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar4
    quest:AddEntityBinding("Gate2Guard1", "BanditCampBossBattle/Entities/Gate2Guard1", 1)
    quest:AddEntityBinding("BCGameMaster", "BanditCampBossBattle/Entities/BCGameMaster")
    quest:AddEntityBinding("AssassinMarker", "BanditCampBossBattle/Entities/AssassinMarker", 1)
    quest:AddEntityBinding("Assassin1", "BanditCampBossBattle/Entities/Assassin1", 1)
    quest:AddEntityBinding("Assassin2", "BanditCampBossBattle/Entities/Assassin2", 1)
    quest:AddEntityBinding("Assassin3", "BanditCampBossBattle/Entities/Assassin3", 1)
    quest:AddEntityBinding("Gate3Guard", "BanditCampBossBattle/Entities/Gate3Guard", 1)
    quest:AddEntityBinding("CampHostage", "BanditCampBossBattle/Entities/CampHostage", 1)
    quest:AddEntityBinding("CampHostage2", "BanditCampBossBattle/Entities/CampHostage2", 1)
    quest:AddEntityBinding("CampHostageDoor", "BanditCampBossBattle/Entities/CampHostageDoor", 1)
    quest:AddEntityBinding("CampHostageGuard", "BanditCampBossBattle/Entities/CampHostageGuard", 1)
    quest:AddEntityBinding("BanditForger", "BanditCampBossBattle/Entities/BanditForger", 1)
    quest:AddEntityBinding("BanditKing", "BanditCampBossBattle/Entities/BanditKing", 1)
    quest:AddEntityBinding("CROWDBANDITS", "BanditCampBossBattle/Entities/CROWDBANDITS", 1)
    quest:FinalizeEntityBindings()
    quest:CreateThread("BanditKingMissionProcess")  -- native thread body 0x00D109F0: lift it as function BanditKingMissionProcess(quest)
    if not bVar4 then
    end
    quest:CreateThread("WatchForBanditCampGates")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function WatchForBanditCampGates(quest)
    quest:CreateThread("ProcessHostageCutscenes")  -- native thread body 0x00D0FE80: lift it as function ProcessHostageCutscenes(quest)
    if not bVar4 then
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptBase_WaitForEntityAndExecuteTwoPair: lift it as function WatchForTermination(quest)
    if not bVar4 then
    end
    quest:CreateThread("CheckForFirstAreaMassacre")  -- native thread body Quest009_BanditCamp_Outer_OnLoad: lift it as function CheckForFirstAreaMassacre(quest)
    if not bVar4 then
    end
    quest:CreateThread("CheckForSecondAreaMassacre")  -- native thread body Quest009_BanditCamp_Residential_OnLoad: lift it as function CheckForSecondAreaMassacre(quest)
    if not bVar4 then
    end
    quest:CreateThread("CheckForSecondAreaDoorHelp")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function CheckForSecondAreaDoorHelp(quest)
    if not bVar4 then
    end
    quest:CacheMusicSet(0x30)
    quest:CacheMusicSet(0x31)
    quest:CacheMusicSet(0x32)
end

function Init(quest)
    quest:AddQuestRegion("Q_BanditCampBossBattle", "BanditCampPathEntrance")
    quest:AddQuestRegion("Q_BanditCampBossBattle", "BanditCampEntrance")
    quest:AddQuestRegion("Q_BanditCampBossBattle", "BanditCampPath1")
    quest:AddQuestRegion("Q_BanditCampBossBattle", "BanditCampCentre")
    quest:AddQuestRegion("Q_BanditCampBossBattle", "BanditCampBoss")
    quest:AddQuestRegion("Q_BanditCampBossBattle", "DemonDoor_BanditCampPath")
    quest:SetStateBool("AttackedGate2OuterGuard", false)
    quest:SetStateBool("AttackedGate3OuterGuard", false)
    quest:SetStateBool("Gate2Open", false)
    quest:SetStateBool("Gate3Open", false)
    quest:SetStateBool("HostagesRescued", false)
    quest:SetStateBool("HostageKilled", false)
    quest:SetStateBool("ItsAllOver", false)
    quest:SetStateBool("BanditKingCrowdSpawn", false)
    quest:SetStateBool("BanditKingFightStarted", false)
    quest:SetStateBool("BanditKingFightEnded", false)
    quest:SetStateBool("TheresaFrescoShown", false)
    quest:SetStateInt("KingHealth", 0)
    quest:SetStateBool("SpokenToSecondGuard", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("Gate1Passed", false)
    quest:SetStateBool("AssassinsUnderAttack", false)
    quest:SetStateBool("TalkedToAssassin", false)
    quest:SetStateBool("AssassinCutsceneTriggered", false)
    quest:SetStateBool("PlayGuardTooCloseCutscene", false)
    quest:SetStateBool("TwinBladeKilled", false)
    quest:SetStateBool("TwinBladeAttacked", false)
    quest:SetStateInt("AngryBanditNeeded", 0)
    quest:SetStateBool("BanditsNeededForCutscene", false)
    quest:SetStateBool("PubGameChatted", false)
end

function OnPersist(quest, context)
end

function BanditKingMissionProcess(quest)
    local resources = quest:RetailResources()
    local C_stk_4c, bVar2, c_stk_99, fVar13, fVar8, f_stk_a4, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iStack_78, iVar12, iVar6, pCVar3, pCVar4, pColour1, pColour2, pScriptObject, pThing, r1, uVar10, uVar11, uVar7, xStack_10, xStack_20, xStack_48, xStack_5c, xStack_6c, xStack_84, xStack_90
    local alive = true
    bVar2 = quest:IsLevelLoaded("BanditCampBoss")
    c_stk_99 = (1 - (bVar2 and 1 or 0))
    while c_stk_99 ~= 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsLevelLoaded("BanditCampBoss")
        c_stk_99 = (1 - (bVar2 and 1 or 0))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    quest:CancelUsingAbility(0xf)
    quest:CancelUsingAbility(10)
    quest:SetAbilityAvailability(0xf, false)
    quest:SetAbilityAvailability(10, false)
    quest:AutoSaveCheckPoint()
    bVar2 = false
    pCVar3 = quest:GetThingWithScriptName("Gate3Inner")
    quest:SetThingAsUsable(pCVar3, bVar2)
    quest:SetTeleportingAsActive(false)
    xStack_90 = nil
    quest:OverrideMusic(1, false, false)
    pCVar3 = quest:GetThingWithScriptName("BanditKingSpawn")
    bVar2 = false
    pCVar4 = pCVar3:GetPos()
    pCVar3 = quest:CreateCreature("CREATURE_BOSS_BANDIT_KING", pCVar4, "BanditKing")
    xStack_90 = pCVar3
    xStack_5c = resources:NewResource()
    xStack_6c = resources:NewResource()
    resources:TryAcquire(xStack_5c, xStack_90, 4)
    iVar12 = 4
    pCVar3 = xStack_6c
    pThing = quest:GetHero()
    resources:TryAcquire(pCVar3, pThing, iVar12)
    xStack_84 = resources:NewActorMap()
    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_84,&xStack_a0);
    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator=__at8ab1e0((CCountedPointer<CDiskFileWin32> *)(pCVar5 + 8), (int)&((CScriptThing *)xStack_6c)->field_0x8);
    xStack_48 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:SetAllowScreenFadingIfAlreadyFaded(true)
    resources:RunMacro("CS_BANDITKING_INTRO_BLACK", xStack_84, false, false)
    quest:SetAllowScreenFadingIfAlreadyFaded(false)
    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_84,&xStack_a0);
    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator=__at8ab1e0((CCountedPointer<CDiskFileWin32> *)(pCVar5 + 8), (int)&((CScriptThing *)xStack_6c)->field_0x8);
    resources:SetActor(xStack_84, "KING", xStack_5c)
    resources:RunMacro("CS_BANDITKING_INTRO", xStack_84, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_48)
    resources:DestroyActorMap(xStack_84)
    resources:ReleaseResource(xStack_6c)
    resources:ReleaseResource(xStack_5c)
    fret_0 = quest:GetHealth(xStack_90)
    C_stk_4c = math.tointeger(math.modf(fret_0 * 0.25))
    quest:ActivateQuest("Q_BanditCamp_Barriers")
    iStack_78 = quest:GetAllThingsWithScriptName("InvisibleWall")
    if #iStack_78 ~= 0 then
        iVar6 = 0
        uVar7 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d11905 end
            quest:EntitySetAsDrawable(iStack_78[(iVar6) / 0xc + 1], false)
            uVar7 = uVar7 + 1
            iVar6 = iVar6 + 0xc
        until not (uVar7 < (#iStack_78))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:EntitySetAsKillable(xStack_90, false, true)
        quest:SetStateBool("BanditKingFightStarted", true)
        quest:SetMasterGameState("BodyGuardsMustStandAndWait", true)
        quest:OverrideMusic(0x17, false, false)
        xStack_6c = resources:NewResource()
        resources:TryAcquire(xStack_6c, xStack_90, 4)
        iVar12 = quest:RegisterTimer()
        quest:SetTimer(iVar12, 5)
        bVar2 = false
        pCVar3 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(xStack_90, pCVar3, bVar2)
        iVar6 = quest:GetTimer(iVar12)
        while 0 < iVar6 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(iVar12)
                resources:ReleaseResource(xStack_6c)
                goto LAB_00d11905
            end
            iVar6 = quest:GetTimer(iVar12)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(iVar12)
            resources:ReleaseResource(xStack_6c)
        else
            resources:PrepareResource(xStack_6c)
            quest:DeregisterTimer(iVar12)
            resources:ReleaseResource(xStack_6c)
            quest:DisplayQuestInfo(true)
            fVar13 = 1.0
            pColour2 = {R = 255, G = 0, B = 0, A = 255}
            pColour1 = {R = 255, G = 0, B = 0, A = 255}
            fVar8 = 0.0
            fret_00 = quest:GetHealth(xStack_90)
            iVar6 = quest:AddQuestInfoBar(fret_00, fVar8, pColour1, pColour2, "HUD_QUEST_ICON_TWINBLADE", "", fVar13)
            f_stk_a4 = C_stk_4c
            C_stk_4c = f_stk_a4
            fret_01 = quest:GetHealth(xStack_90)
            if f_stk_a4 < fret_01 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d11905 end
                    fVar13 = -1.0
                    fVar8 = -1.0
                    fret_02 = quest:GetHealth(xStack_90)
                    quest:UpdateQuestInfoBar(iVar6, fret_02, fVar8, fVar13)
                    fret_03 = quest:GetHealth(xStack_90)
                until not (f_stk_a4 < fret_03)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:DeactivateQuest("Q_BanditCamp_Barriers", 0)
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(iVar6)
                quest:StopOverrideMusic(false)
                bVar2 = false
                fret_04 = quest:GetHealth(xStack_90)
                quest:ModifyThingHealth(xStack_90, (C_stk_4c - fret_04), bVar2)
                iVar6 = (xStack_90 ~= nil and xStack_90:IsAlive())
                if iVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d11905 end
                    quest:SetStateBool("BanditKingFightEnded", true)
                    quest:SetMasterGameState("BodyGuardsMustStandAndWait", false)
                    pCVar3 = quest:GetThingWithScriptName("BanditKingSpawn")
                    bVar2 = false
                    pCVar4 = pCVar3:GetPos()
                    r1 = quest:CreateCreature("CREATURE_SISTER", pCVar4, "HeroSister")
                    quest:SetStateBool("BanditsNeededForCutscene", true)
                    xStack_10 = resources:NewResource()
                    xStack_20 = resources:NewResource()
                    xStack_48 = resources:NewResource()
                    resources:TryAcquire(xStack_10, xStack_90, 4)
                    resources:TryAcquire(xStack_48, r1, 4)
                    iVar12 = 4
                    pScriptObject = xStack_20
                    pCVar3 = quest:GetHero()
                    resources:TryAcquire(pScriptObject, pCVar3, iVar12)
                    xStack_84 = resources:NewActorMap()
                    resources:SetActor(xStack_84, "KING", xStack_10)
                    resources:SetActor(xStack_84, "HERO", xStack_20)
                    resources:SetActor(xStack_84, "SISTER", xStack_48)
                    xStack_5c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_BANDITKING_THERESA", xStack_84, false, true)
                    quest:PlayAVIMovie("Data\\\\Video\\\\3_theresa_flashback_comp.xmv")
                    quest:SetStateBool("TheresaFrescoShown", true)
                    resources:SetActor(xStack_84, "KING", xStack_10)
                    resources:SetActor(xStack_84, "HERO", xStack_20)
                    resources:SetActor(xStack_84, "SISTER", xStack_48)
                    resources:RunMacro("CS_BANDITKING_THERESA_END", xStack_84, false, true)
                    quest:FixMovieSequenceCamera(false)
                    uVar11 = true
                    uVar10 = false
                    pCVar3 = quest:GetThingWithScriptName("Gate3Inner")
                    pCVar3 = quest:GetNearestWithScriptName(pCVar3, "CROWDBANDITS")
                    quest:RemoveThing(pCVar3, uVar10, uVar11)
                    uVar11 = true
                    uVar10 = false
                    pCVar3 = quest:GetThingWithScriptName("Gate3Inner")
                    pCVar3 = quest:GetNearestWithScriptName(pCVar3, "CROWDBANDITS")
                    quest:RemoveThing(pCVar3, uVar10, uVar11)
                    quest:ResetPlayerCreatureCombatMultiplier()
                    iVar12 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe8c)))
                    quest:GiveHeroExperience(iVar12)
                    quest:SetStateBool("BanditsNeededForCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_5c)
                    resources:DestroyActorMap(xStack_84)
                    resources:ReleaseResource(xStack_48)
                    resources:ReleaseResource(xStack_20)
                    resources:ReleaseResource(xStack_10)
                    quest:SetStateBool("ItsAllOver", true)
                end
                quest:Pause(0.2)
                quest:FadeScreenIn()
                pCVar3 = quest:GetThingWithScriptName("Gate3Inner")
                quest:OpenDoor(pCVar3)
                quest:SetAbilityAvailability(0xf, true)
                quest:SetAbilityAvailability(10, true)
                bVar2 = quest:IsLevelLoaded("BanditCampResidential")
                c_stk_99 = (1 - (bVar2 and 1 or 0))
                while c_stk_99 ~= 0 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d11905 end
                    bVar2 = quest:IsLevelLoaded("BanditCampResidential")
                    c_stk_99 = (1 - (bVar2 and 1 or 0))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    if not quest:GetStateBool("TwinBladeKilled") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d11905 end
                        fVar8 = quest:ReadGlobalGameDataFloat(0xe94)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d11905 end
                        iVar12 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe64)))
                        quest:GiveHeroRenownPoints(iVar12)
                        fVar8 = quest:ReadGlobalGameDataFloat(0xe90)
                    end
                    quest:GiveHeroMorality(fVar8)
                    quest:SetTeleportingAsActive(true)
                    quest:SetStateBool("MissionSucceeded", true)
                end
            end
        end
    end
    ::LAB_00d11905::
end

function WatchForBanditCampGates(quest)
    local r1
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if bVar1 then
        return
    end
    repeat
        bVar1 = quest:IsRegionLoaded("BanditCampCentre")
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            r1 = quest:GetThingWithScriptName("Gate2Inner")
            quest:OpenDoor(r1)
            bVar1 = quest:IsRegionLoaded("BanditCampCentre")
            if bVar1 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    bVar1 = quest:IsRegionLoaded("BanditCampCentre")
                until not (bVar1)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                -- LAB_00d048a7: (native jump target)
                return
            end
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
    until false
end

function ProcessHostageCutscenes(quest)
    local resources = quest:RetailResources()
    local CVar2, CVar8, __native_condition_1, __native_condition_2, amount, bVar12, bVar3, cVar4, c_stk_79, ePriority, iVar10, iVar6, native_arg_sequence_1, pCVar11, pCVar7, pPosition, r1, r2, r3, uVar9, xStack_1c, xStack_2c, xStack_3c, xStack_4c, xStack_5c, xStack_78
    local alive = true
    iVar10 = 0
    bVar3 = quest:IsRegionLoaded("BanditCampCentre")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsRegionLoaded("BanditCampCentre")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    r1 = quest:GetThingWithScriptName("CampHostage2")
    r2 = quest:GetThingWithScriptName("CampHostage")
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            r2 = nil
            r1 = nil
            return
        end
        bVar3 = quest:IsRegionLoaded("BanditCampCentre")
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d109cc end
            CVar2 = 0
            -- TODO(native): xStack_78 = xStack_78 | 1;
            __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
            if not __native_condition_1 then
                cVar4 = (r1 ~= nil and r1:MsgIsKilledBy(""))
                __native_condition_1 = not cVar4
            end
            if __native_condition_1 then
                CVar8 = CVar2 | 3
                xStack_78 = CVar8
                native_arg_sequence_1 = false
                if (r2 ~= nil and not r2:IsNull()) then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    cVar4 = (r2 ~= nil and r2:MsgIsKilledBy(""))
                    if cVar4 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00d10034 end
                c_stk_79 = 0
            else
                goto LAB_00d10034
            end
            goto FLOW_past_lab_00d10034
            ::LAB_00d10034::
            c_stk_79 = 1
            ::FLOW_past_lab_00d10034::
            if (CVar8 & 2) ~= 0 then
                CVar8 = CVar8 & 0xfffffffd
                xStack_78 = CVar8
            end
            if (CVar8 & 1) ~= 0 then
                -- TODO(native): xStack_78 = CVar8 & 0xfffffffe;
            end
            if c_stk_79 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d109cc end
                quest:SetStateBool("HostageKilled", true)
            end
        end
        if quest:GetStateBool("HostagesRescued") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                if not quest:GetStateBool("Gate3Open") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe68))
                        xStack_2c = resources:NewResource()
                        xStack_3c = resources:NewResource()
                        xStack_1c = resources:NewResource()
                        iVar6 = 4
                        pCVar11 = xStack_2c
                        pCVar7 = quest:GetHero()
                        resources:TryAcquire(pCVar11, pCVar7, iVar6)
                        resources:TryAcquire(xStack_3c, r1, 4)
                        resources:TryAcquire(xStack_1c, r2, 4)
                        xStack_5c = resources:NewActorMap()
                        resources:SetActor(xStack_5c, "HERO", xStack_2c)
                        resources:SetActor(xStack_5c, "HOST1", xStack_3c)
                        resources:SetActor(xStack_5c, "HOST2", xStack_1c)
                        xStack_4c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_BANDITCAMP_HOSTAGEFREE", xStack_5c, false, true)
                        quest:FixMovieSequenceCamera(false)
                        bVar12 = true
                        bVar3 = false
                        pCVar7 = quest:GetThingWithScriptName("Gate3Guard")
                        quest:RemoveThing(pCVar7, bVar3, bVar12)
                        quest:SetStateBool("Gate3Open", true)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_4c)
                        resources:DestroyActorMap(xStack_5c)
                        resources:ReleaseResource(xStack_1c)
                        resources:ReleaseResource(xStack_3c)
                        resources:ReleaseResource(pCVar11)
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe68))
                        xStack_1c = resources:NewResource()
                        xStack_2c = resources:NewResource()
                        xStack_3c = resources:NewResource()
                        iVar6 = 4
                        pCVar11 = xStack_1c
                        pCVar7 = quest:GetHero()
                        resources:TryAcquire(pCVar11, pCVar7, iVar6)
                        resources:TryAcquire(xStack_2c, r1, 4)
                        resources:TryAcquire(xStack_3c, r2, 4)
                        xStack_5c = resources:NewActorMap()
                        resources:SetActor(xStack_5c, "HERO", xStack_1c)
                        resources:SetActor(xStack_5c, "HOST1", xStack_2c)
                        resources:SetActor(xStack_5c, "HOST2", xStack_3c)
                        xStack_4c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_BANDITCAMP_HOSTAGEFREE_NOTOPEN", xStack_5c, false, true)
                        quest:FixMovieSequenceCamera(false)
                        bVar12 = true
                        bVar3 = false
                        pCVar7 = quest:GetThingWithScriptName("Gate3Guard")
                        quest:RemoveThing(pCVar7, bVar3, bVar12)
                        quest:SetStateBool("Gate3Open", true)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_4c)
                        resources:DestroyActorMap(xStack_5c)
                        resources:ReleaseResource(xStack_3c)
                        resources:ReleaseResource(xStack_2c)
                        resources:ReleaseResource(xStack_1c)
                    end
                end
            end
            goto LAB_00d109cc
        end
        if quest:GetStateBool("HostageKilled") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d109cc end
            iVar6 = (r1 ~= nil and r1:IsAlive())
            __native_condition_2 = not iVar6
            if __native_condition_2 then
                iVar6 = (r2 ~= nil and r2:IsAlive())
                __native_condition_2 = not iVar6
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d109cc end
                amount = quest:ReadGlobalGameDataFloat(0xe70)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d109cc end
                amount = quest:ReadGlobalGameDataFloat(0xe6c)
            end
            quest:GiveHeroMorality(amount)
            if quest:GetStateBool("Gate3Open") then goto LAB_00d109c8 end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d109cc end
            xStack_3c = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            xStack_1c = resources:NewResource()
            ePriority = 4
            pCVar11 = xStack_1c
            pCVar7 = quest:GetHero()
            resources:TryAcquire(pCVar11, pCVar7, ePriority)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            xStack_2c = resources:NewActorMap()
            resources:RunMacro("CS_BANDITCAMP_HOSTAGEKILL", xStack_2c, false, true)
            bVar12 = true
            bVar3 = false
            pCVar7 = quest:GetThingWithScriptName("Gate3Guard")
            quest:RemoveThing(pCVar7, bVar3, bVar12)
            resources:DestroyActorMap(xStack_2c)
            resources:ReleaseResource(xStack_1c)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_3c)
            xStack_5c = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
            iVar6 = xStack_5c - xStack_5c >> 0x1f
            uVar9 = 0
            if #xStack_5c + iVar6 ~= iVar6 then break end
            goto LAB_00d108eb
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
    while true do
        -- TODO(native): xStack_84 = (CCharString)(xStack_5c + iVar10);
        pCVar7 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(pCVar7, nil --[[missing]])
        uVar9 = uVar9 + 1
        iVar10 = iVar10 + 0xc
        if (#xStack_5c) <= uVar9 then break end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d108f6 end
    end
    ::LAB_00d108eb::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        goto LAB_00d108f6
    else
        pCVar7 = quest:GetThingWithScriptName("DefensiveGuardLeader")
        bVar3 = false
        pPosition = pCVar7:GetPos()
        r3 = quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", pPosition, "")
        pCVar7 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(r3, pCVar7)
        goto LAB_00d109c8
    end
    goto FLOW_past_lab_00d108f6
    ::LAB_00d108f6::
    ::FLOW_past_lab_00d108f6::
    goto FLOW_past_lab_00d109c8
    ::LAB_00d109c8::
    quest:SetStateBool("Gate3Open", true)
    ::FLOW_past_lab_00d109c8::
    ::LAB_00d109cc::
end

function WatchForTermination(quest)
    local bVar4, bVar6, pCVar5, pQuestName
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
        if not quest:GetStateBool("MissionSucceeded") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = true
            bVar4 = false
            pQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pQuestName, bVar4, "", bVar6)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar6 = false
            bVar4 = false
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, bVar6, false)
            quest:SetPrizeTavernTable(true)
            quest:Pause(1.0)
            quest:FadeScreenIn()
        end
        pCVar5 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar5, 0)
    end
end

function CheckForFirstAreaMassacre(quest)
    local resources = quest:RetailResources()
    local b2, bVar4, cVar5, ePriority, elem_1, elem_2, iVar7, native_arg_kills_xStack_74, pCVar6, pPosition, pScriptObject, r1, uVar8, xStack_10, xStack_20, xStack_44, xStack_50, xStack_5c, xStack_68, xStack_6c
    local alive = true
    while true do
        pCVar6 = quest:GetThingWithScriptName("Gate1")
        bVar4 = (pCVar6 ~= nil and pCVar6:IsOpenDoor())
        pCVar6 = nil
        if bVar4 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    native_arg_kills_xStack_74 = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    repeat
        bVar4 = quest:IsLevelLoaded("BanditCampMain")
        if not bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = quest:IsLevelLoaded("BanditCampMain")
            while not bVar4 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                bVar4 = quest:IsLevelLoaded("BanditCampMain")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
        end
        xStack_5c = {}
        pCVar6 = quest:GetHero()
        xStack_5c = pCVar6:MsgGetThingsKilledGroups()
        bVar4 = #xStack_5c ~= 0
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + #xStack_5c
            xStack_5c = {}
        end
        pCVar6 = quest:GetHero()
        xStack_68 = quest:GetFollowingEntityList(pCVar6)
        uVar8 = 0
        if #xStack_68 ~= 0 then
            iVar7 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d0f264(); return end
                elem_1 = xStack_68[(iVar7) / 0xc + 1]
                xStack_5c = elem_1:MsgGetThingsKilledGroups()
                cVar5 = #xStack_5c ~= 0
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d0f264(); return end
                    native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + #xStack_5c
                    xStack_5c = {}
                end
                uVar8 = uVar8 + 1
                iVar7 = iVar7 + 0xc
            until not (uVar8 < (#xStack_68))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            __cleanup_LAB_00d0f264()
            return
        end
        xStack_68 = quest:GetHeroSummonedCreaturesList()
        uVar8 = 0
        if #xStack_68 ~= 0 then
            iVar7 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                elem_2 = xStack_68[(iVar7) / 0xc + 1]
                xStack_5c = elem_2:MsgGetThingsKilledGroups()
                cVar5 = #xStack_5c ~= 0
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d0f264(); return end
                    native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + #xStack_5c
                    xStack_5c = {}
                end
                uVar8 = uVar8 + 1
                iVar7 = iVar7 + 0xc
            until not (uVar8 < (#xStack_68))
        end
        iVar7 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then break end
        if quest:ReadGlobalGameDataFloat(0xe78) <= native_arg_kills_xStack_74 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d0f5f9 end
            xStack_20 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            ePriority = 4
            xStack_10 = resources:NewResource()
            pScriptObject = xStack_10
            pCVar6 = quest:GetHero()
            resources:TryAcquire(pScriptObject, pCVar6, ePriority)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            xStack_44 = resources:NewActorMap()
            resources:RunMacro("CS_BANDITCAMP_ALARM_OUTER", xStack_44, false, true)
            b2 = true
            bVar4 = false
            pCVar6 = quest:GetThingWithScriptName("Gate2Guard1")
            quest:RemoveThing(pCVar6, bVar4, b2)
            resources:DestroyActorMap(xStack_44)
            resources:ReleaseResource(xStack_10)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_20)
            quest:SetStateBool("Gate2Open", true)
            xStack_50 = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
            uVar8 = 0
            if #xStack_50 == 0 then goto LAB_00d0f52f end
            goto LAB_00d0f4d7
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
    until false
    goto LAB_00d0f608
    ::LAB_00d0f4d7::
    while true do
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            goto LAB_00d0f608
        end
        xStack_6c = xStack_50[(iVar7) / 0xc + 1]
        pCVar6 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(xStack_6c, pCVar6)
        uVar8 = uVar8 + 1
        iVar7 = iVar7 + 0xc
        if (#xStack_50) <= uVar8 then break end
    end
    ::LAB_00d0f52f::
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        pCVar6 = quest:GetThingWithScriptName("DefensiveGuardLeader")
        bVar4 = false
        pPosition = pCVar6:GetPos()
        r1 = quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", pPosition, "")
        pCVar6 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(r1, pCVar6)
    end
    ::LAB_00d0f5f9::
    ::LAB_00d0f608::
end

function CheckForSecondAreaMassacre(quest)
    local resources = quest:RetailResources()
    local b2, bVar3, cVar4, elem_1, elem_2, iStack_50, iVar6, iVar8, native_arg_kills_xStack_74, pCVar5, pPosition, pScriptObject, r1, uVar7, xStack_10, xStack_20, xStack_38, xStack_5c, xStack_68
    local alive = true
    native_arg_kills_xStack_74 = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    while true do
        bVar3 = quest:IsLevelLoaded("BanditCampResidential")
        if not bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            bVar3 = quest:IsLevelLoaded("BanditCampResidential")
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                bVar3 = quest:IsLevelLoaded("BanditCampResidential")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
        end
        if quest:GetStateBool("Gate3Open") then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        pCVar5 = quest:GetHero()
        xStack_5c = pCVar5:MsgGetThingsKilledGroups()
        bVar3 = #xStack_5c ~= 0
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d0fe48 end
            native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + #xStack_5c
            xStack_5c = {}
        end
        pCVar5 = quest:GetHero()
        xStack_68 = quest:GetFollowingEntityList(pCVar5)
        uVar7 = 0
        if #xStack_68 ~= 0 then
            iVar6 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d0f9d8 end
                elem_1 = xStack_68[(iVar6) / 0xc + 1]
                xStack_5c = elem_1:MsgGetThingsKilledGroups()
                cVar4 = #xStack_5c ~= 0
                if cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0fe39 end
                    native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + #xStack_5c
                    xStack_5c = {}
                end
                uVar7 = uVar7 + 1
                iVar6 = iVar6 + 0xc
            until not (uVar7 < (#xStack_68))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d0fa12 end
        xStack_68 = quest:GetHeroSummonedCreaturesList()
        uVar7 = 0
        if #xStack_68 ~= 0 then
            iVar6 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d0fa46 end
                elem_2 = xStack_68[(iVar6) / 0xc + 1]
                xStack_5c = elem_2:MsgGetThingsKilledGroups()
                cVar4 = #xStack_5c ~= 0
                if cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0fe39 end
                    native_arg_kills_xStack_74 = native_arg_kills_xStack_74 + #xStack_5c
                    xStack_5c = {}
                end
                uVar7 = uVar7 + 1
                iVar6 = iVar6 + 0xc
            until not (uVar7 < (#xStack_68))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d0fa7a end
        if quest:ReadGlobalGameDataFloat(0xe7c) <= native_arg_kills_xStack_74 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_20 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        iVar8 = 4
        xStack_10 = resources:NewResource()
        pScriptObject = xStack_10
        pCVar5 = quest:GetHero()
        resources:TryAcquire(pScriptObject, pCVar5, iVar8)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        -- TODO(native): xStack_38._0_4_ = malloc(0x24);
        -- TODO(native): *(undefined1 *)xStack_38._0_4_ = 0;
        -- TODO(native): *(undefined **)(xStack_38._0_4_ + 4) = (undefined *)0x0;
        -- TODO(native): *(undefined4 *)(xStack_38._0_4_ + 8) = xStack_38._0_4_;
        -- TODO(native): *(undefined4 *)(xStack_38._0_4_ + 0xc) = xStack_38._0_4_;
        resources:RunMacro("CS_BANDITCAMP_ALARM_INNER", xStack_38, false, true)
        b2 = true
        bVar3 = false
        pCVar5 = quest:GetThingWithScriptName("Gate3Guard")
        quest:RemoveThing(pCVar5, bVar3, b2)
        resources:DestroyActorMap(xStack_38)
        resources:ReleaseResource(xStack_10)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_20)
        quest:SetStateBool("Gate3Open", true)
        iStack_50 = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
        uVar7 = 0
        if #iStack_50 ~= 0 then
            iVar6 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    goto LAB_00d0fe48
                end
                -- TODO(native): xStack_6c = (CCharString)(iStack_50 + iVar6);
                pCVar5 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(pCVar5, nil --[[missing]])
                uVar7 = uVar7 + 1
                iVar6 = iVar6 + 0xc
            until not (uVar7 < (#iStack_50))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            pCVar5 = quest:GetThingWithScriptName("DefensiveGuardLeader")
            bVar3 = false
            pPosition = pCVar5:GetPos()
            r1 = quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", pPosition, "")
            pCVar5 = nil
            pCVar5 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(r1, pCVar5)
            r1 = nil
        end
    end
    ::LAB_00d0fe39::
    ::LAB_00d0fe48::
    do return end
    ::LAB_00d0f9d8::
    goto LAB_00d0fe48
    ::LAB_00d0fa12::
    goto LAB_00d0fe48
    ::LAB_00d0fa46::
    goto LAB_00d0fe48
    ::LAB_00d0fa7a::
    goto LAB_00d0fe48
end

function CheckForSecondAreaDoorHelp(quest)
    local CVar1, bVar2, iVar3, i_stk_4, native_arg_switch_1, timerId, uVar4
    local alive = true
    CVar1 = quest:GetStateBool("Gate3Open")
    while not CVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        CVar1 = quest:GetStateBool("Gate3Open")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    timerId = quest:RegisterTimer()
    i_stk_4 = timerId
    quest:SetTimer(i_stk_4, 5)
    uVar4 = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            quest:DeregisterTimer(i_stk_4)
            return
        end
        bVar2 = quest:IsLevelLoaded("BanditCampResidential")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_4)
                return
            end
            bVar2 = quest:IsLevelLoaded("BanditCampBoss")
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:DeregisterTimer(i_stk_4)
                    return
                end
                quest:DeregisterTimer(i_stk_4)
                return
            end
            bVar2 = quest:IsLevelLoaded("BanditCampResidential")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00d0470f: (native jump target)
            quest:DeregisterTimer(i_stk_4)
            return
        end
        iVar3 = quest:GetTimer(i_stk_4)
        if 0 < iVar3 then goto LAB_00d046b8 end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_4)
            return
        end
        native_arg_switch_1 = uVar4
        repeat
            if native_arg_switch_1 == 0 then
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10", "", true, true)
                uVar4 = 1
                break
            else
                if native_arg_switch_1 == 1 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_20", "", true, true)
                    uVar4 = 2
                    break
                else
                    if native_arg_switch_1 == 2 then
                        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_30", "", true, true)
                        goto LAB_00d0469d
                    else
                        if native_arg_switch_1 == 3 then
                            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_40", "", true, true)
                            uVar4 = 4
                            break
                        else
                            if native_arg_switch_1 == 4 then
                                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_50", "", true, true)
                                goto LAB_00d0469d
                            end
                        end
                    end
                    goto FLOW_past_lab_00d0469d
                    ::LAB_00d0469d::
                    uVar4 = 3
                    ::FLOW_past_lab_00d0469d::
                end
            end
        until not (false)
        quest:SetTimer(i_stk_4, 0x14)
        ::LAB_00d046b8::
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until false
end

function OpenGate(quest, native_arg_time_delay, native_arg_door_name)
    quest:Pause(native_arg_time_delay)
    local pDoor = quest:GetThingWithScriptName(native_arg_door_name)
    quest:OpenDoor(pDoor)
    pDoor = nil
end

