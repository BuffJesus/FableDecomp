-- Readable native conversion: Q_BanditCampBossBattle. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BAC_RenownAward = 3684,  -- 50.0
    BAC_HostageMoralityGain = 3688,  -- 0.029999999329447746
    BAC_SingleHostageMoralityLoss = 3692,  -- -0.009999999776482582
    BAC_DoubleHostageMoralityLoss = 3696,  -- -0.019999999552965164
    BAC_EnemiesToKillInFirstArea = 3704,  -- 12.0
    BAC_EnemiesToKillInSecondArea = 3708,  -- 12.0
    BAC_TheresaExperienceGift = 3724,  -- 10000.0
    BAC_TwinBladeKilledMorality = 3728,  -- -0.05999999865889549
    BAC_TwinBladeSavedMorality = 3732,  -- 0.05999999865889549
}

-- Q_BanditCampBossBattle.Main (retail 0x00d038e0)
function Main(quest)
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
    quest:CreateThread("WatchForBanditCampGates")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function WatchForBanditCampGates(quest)
    quest:CreateThread("ProcessHostageCutscenes")  -- native thread body 0x00D0FE80: lift it as function ProcessHostageCutscenes(quest)
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptBase_WaitForEntityAndExecuteTwoPair: lift it as function WatchForTermination(quest)
    quest:CreateThread("CheckForFirstAreaMassacre")  -- native thread body Quest009_BanditCamp_Outer_OnLoad: lift it as function CheckForFirstAreaMassacre(quest)
    quest:CreateThread("CheckForSecondAreaMassacre")  -- native thread body Quest009_BanditCamp_Residential_OnLoad: lift it as function CheckForSecondAreaMassacre(quest)
    quest:CreateThread("CheckForSecondAreaDoorHelp")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function CheckForSecondAreaDoorHelp(quest)
    quest:CacheMusicSet(48)
    quest:CacheMusicSet(49)
    quest:CacheMusicSet(50)
end

-- Q_BanditCampBossBattle.Init (retail 0x00d03610)
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

-- Q_BanditCampBossBattle.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Q_BanditCampBossBattle.BanditKingMissionProcess (retail 0x00d109f0)
function BanditKingMissionProcess(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local readGlobalGameDataFloat, scratchValue, scratchValue6
    while (1 - (quest:IsLevelLoaded("BanditCampBoss") and 1 or 0)) ~= 0 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CancelUsingAbility(15)
    quest:CancelUsingAbility(10)
    quest:SetAbilityAvailability(15, false)
    quest:SetAbilityAvailability(10, false)
    quest:AutoSaveCheckPoint()
    quest:SetThingAsUsable(quest:GetThingWithScriptName("Gate3Inner"), false)
    quest:SetTeleportingAsActive(false)
    quest:OverrideMusic(1, false, false)
    local bossBanditKing = quest:CreateCreature("CREATURE_BOSS_BANDIT_KING", quest:GetThingWithScriptName("BanditKingSpawn"):GetPos(), "BanditKing")
    local resource5 = resources:NewResource()
    resources:TryAcquire(resource5, bossBanditKing, 4)
    hero:AcquireControl(4)
    local actorMap = resources:NewActorMap()
    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_84,&xStack_a0);
    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator=__at8ab1e0((CCountedPointer<CDiskFileWin32> *)(pCVar5 + 8), (int)&((CScriptThing *)xStack_6c)->field_0x8);
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:SetAllowScreenFadingIfAlreadyFaded(true)
    resources:RunMacro("CS_BANDITKING_INTRO_BLACK", actorMap, false, false)
    quest:SetAllowScreenFadingIfAlreadyFaded(false)
    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_84,&xStack_a0);
    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator=__at8ab1e0((CCountedPointer<CDiskFileWin32> *)(pCVar5 + 8), (int)&((CScriptThing *)xStack_6c)->field_0x8);
    resources:SetActor(actorMap, "KING", resource5)
    resources:RunMacro("CS_BANDITKING_INTRO", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    hero:ReleaseControl()
    resources:ReleaseResource(resource5)
    local C_stk_4c_1 = math.tointeger(math.modf(quest:GetHealth(bossBanditKing) * 0.25))
    quest:ActivateQuest("Q_BanditCamp_Barriers")
    local invisibleWall = quest:GetAllThingsWithScriptName("InvisibleWall")
    if #invisibleWall ~= 0 then
        scratchValue = 0
        scratchValue6 = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            quest:EntitySetAsDrawable(invisibleWall[scratchValue + 1], false)
            scratchValue6 = scratchValue6 + 1
            scratchValue = scratchValue + 1
        until scratchValue6 >= #invisibleWall
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetAsKillable(bossBanditKing, false, true)
    quest:SetStateBool("BanditKingFightStarted", true)
    quest:SetMasterGameState("BodyGuardsMustStandAndWait", true)
    quest:OverrideMusic(23, false, false)
    local resource7 = resources:NewResource()
    resources:TryAcquire(resource7, bossBanditKing, 4)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    quest:EntitySetFacingAngleTowardsThing(bossBanditKing, hero, false)
    while 0 < quest:GetTimer(timerId) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource7)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource7)
    else
        resources:PrepareResource(resource7)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource7)
        quest:DisplayQuestInfo(true)
        local infoElement = quest:AddQuestInfoBar(quest:GetHealth(bossBanditKing), 0.0, {R = 255, G = 0, B = 0, A = 255}, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TWINBLADE", "", 1.0)
        while C_stk_4c_1 < quest:GetHealth(bossBanditKing) do
            if not quest:NewScriptFrame() then return end
            quest:UpdateQuestInfoBar(infoElement, quest:GetHealth(bossBanditKing), -1.0, -1.0)
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuest("Q_BanditCamp_Barriers", 0)
        quest:DisplayQuestInfo(false)
        quest:RemoveQuestInfoElement(infoElement)
        quest:StopOverrideMusic(false)
        quest:ModifyThingHealth(bossBanditKing, C_stk_4c_1 - quest:GetHealth(bossBanditKing), false)
        if bossBanditKing ~= nil and bossBanditKing:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("BanditKingFightEnded", true)
            quest:SetMasterGameState("BodyGuardsMustStandAndWait", false)
            local sister = quest:CreateCreature("CREATURE_SISTER", quest:GetThingWithScriptName("BanditKingSpawn"):GetPos(), "HeroSister")
            quest:SetStateBool("BanditsNeededForCutscene", true)
            local resource = resources:NewResource()
            local resource3 = resources:NewResource()
            local resource4 = resources:NewResource()
            resources:TryAcquire(resource, bossBanditKing, 4)
            resources:TryAcquire(resource4, sister, 4)
            resources:TryAcquire(resource3, hero, 4)
            local actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "KING", resource)
            resources:SetActor(actorMap2, "HERO", resource3)
            resources:SetActor(actorMap2, "SISTER", resource4)
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_BANDITKING_THERESA", actorMap2, false, true)
            quest:PlayAVIMovie("Data\\\\Video\\\\3_theresa_flashback_comp.xmv")
            quest:SetStateBool("TheresaFrescoShown", true)
            resources:SetActor(actorMap2, "KING", resource)
            resources:SetActor(actorMap2, "HERO", resource3)
            resources:SetActor(actorMap2, "SISTER", resource4)
            resources:RunMacro("CS_BANDITKING_THERESA_END", actorMap2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:RemoveThing(quest:GetNearestWithScriptName(quest:GetThingWithScriptName("Gate3Inner"), "CROWDBANDITS"), false, true)
            quest:RemoveThing(quest:GetNearestWithScriptName(quest:GetThingWithScriptName("Gate3Inner"), "CROWDBANDITS"), false, true)
            quest:ResetPlayerCreatureCombatMultiplier()
            quest:GiveHeroExperience(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_TheresaExperienceGift))))
            quest:SetStateBool("BanditsNeededForCutscene", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource4)
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource)
            quest:SetStateBool("ItsAllOver", true)
        end
        quest:Pause(0.2)
        quest:FadeScreenIn()
        quest:OpenDoor(quest:GetThingWithScriptName("Gate3Inner"))
        quest:SetAbilityAvailability(15, true)
        quest:SetAbilityAvailability(10, true)
        while (1 - (quest:IsLevelLoaded("BanditCampResidential") and 1 or 0)) ~= 0 do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        if not quest:GetStateBool("TwinBladeKilled") then
            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_TwinBladeSavedMorality)
        else
            quest:GiveHeroRenownPoints(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_RenownAward))))
            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_TwinBladeKilledMorality)
        end
        quest:GiveHeroMorality(readGlobalGameDataFloat)
        quest:SetTeleportingAsActive(true)
        quest:SetStateBool("MissionSucceeded", true)
    end
end

-- Q_BanditCampBossBattle.WatchForBanditCampGates (retail 0x00d04770)
function WatchForBanditCampGates(quest)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:IsRegionLoaded("BanditCampCentre") then
            quest:OpenDoor(quest:GetThingWithScriptName("Gate2Inner"))
            while quest:IsRegionLoaded("BanditCampCentre") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- Q_BanditCampBossBattle.ProcessHostageCutscenes (retail 0x00d0fe80)
function ProcessHostageCutscenes(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, amount, predicateResult3, scratchValue, scratchValue7, sequence
    local resource, scratchValue8, actorMap, movie, defensiveGuardBandit
    while not quest:IsRegionLoaded("BanditCampCentre") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local campHostage2 = quest:GetThingWithScriptName("CampHostage2")
    local campHostage = quest:GetThingWithScriptName("CampHostage")
    predicateResult3 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult3 then
            return
        end
        if quest:IsRegionLoaded("BanditCampCentre") then
            if quest:IsActiveThreadTerminating() then return end
            -- TODO(native): xStack_78 = xStack_78 | 1;
            local predicateResult = not (campHostage2 ~= nil and not campHostage2:IsNull()) or not (campHostage2 ~= nil and campHostage2:MsgIsKilledBy(""))
            if not predicateResult then goto LAB_00d10034 end
            sequence = (campHostage ~= nil and not campHostage:IsNull()) and (campHostage ~= nil and campHostage:MsgIsKilledBy(""))
            if sequence then goto LAB_00d10034 end
            scratchValue = 0
            goto FLOW_past_lab_00d10034
            ::LAB_00d10034::
            scratchValue = 1
            ::FLOW_past_lab_00d10034::
                -- TODO(native): xStack_78 = CVar8 & 0xfffffffe;
            if scratchValue ~= 0 then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("HostageKilled", true)
            end
        end
        if quest:GetStateBool("HostagesRescued") then
            if quest:IsActiveThreadTerminating() then return end
            if not quest:GetStateBool("Gate3Open") then
                if quest:IsActiveThreadTerminating() then return end
                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_HostageMoralityGain))
                local resource7 = resources:NewResource()
                local resource9 = resources:NewResource()
                local resource4 = resources:NewResource()
                resource = resource7
                resources:TryAcquire(resource7, hero, 4)
                resources:TryAcquire(resource9, campHostage2, 4)
                resources:TryAcquire(resource4, campHostage, 4)
                local actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2, "HERO", resource7)
                resources:SetActor(actorMap2, "HOST1", resource9)
                resources:SetActor(actorMap2, "HOST2", resource4)
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_BANDITCAMP_HOSTAGEFREE", actorMap2, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:RemoveThing(quest:GetThingWithScriptName("Gate3Guard"), false, true)
                quest:SetStateBool("Gate3Open", true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap2)
                resources:ReleaseResource(resource4)
                resources:ReleaseResource(resource9)
                resources:ReleaseResource(resource7)
            elseif not quest:IsActiveThreadTerminating() then
                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_HostageMoralityGain))
                local resource5 = resources:NewResource()
                local resource8 = resources:NewResource()
                local resource10 = resources:NewResource()
                resources:TryAcquire(resource5, hero, 4)
                resources:TryAcquire(resource8, campHostage2, 4)
                resources:TryAcquire(resource10, campHostage, 4)
                local actorMap3 = resources:NewActorMap()
                resources:SetActor(actorMap3, "HERO", resource5)
                resources:SetActor(actorMap3, "HOST1", resource8)
                resources:SetActor(actorMap3, "HOST2", resource10)
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_BANDITCAMP_HOSTAGEFREE_NOTOPEN", actorMap3, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:RemoveThing(quest:GetThingWithScriptName("Gate3Guard"), false, true)
                quest:SetStateBool("Gate3Open", true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                resources:DestroyActorMap(actorMap3)
                resources:ReleaseResource(resource10)
                resources:ReleaseResource(resource8)
                resources:ReleaseResource(resource5)
            end
            return
        end
        if not quest:GetStateBool("HostageKilled") then quest:NewScriptFrame(); predicateResult3 = quest:IsActiveThreadTerminating(); goto continue_1 end
        if quest:IsActiveThreadTerminating() then return end
        predicateResult2 = not (campHostage2 ~= nil and campHostage2:IsAlive()) and not (campHostage ~= nil and campHostage:IsAlive())
        if predicateResult2 then
            if quest:IsActiveThreadTerminating() then return end
            amount = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_DoubleHostageMoralityLoss)
        else
            if quest:IsActiveThreadTerminating() then return end
            amount = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_SingleHostageMoralityLoss)
        end
        quest:GiveHeroMorality(amount)
        if quest:GetStateBool("Gate3Open") then quest:SetStateBool("Gate3Open", true); return end
        if quest:IsActiveThreadTerminating() then return end
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        hero:AcquireControl(4)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        actorMap = resources:NewActorMap()
        resources:RunMacro("CS_BANDITCAMP_HOSTAGEKILL", actorMap, false, true)
        quest:RemoveThing(quest:GetThingWithScriptName("Gate3Guard"), false, true)
        resources:DestroyActorMap(actorMap)
        hero:ReleaseControl()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        defensiveGuardBandit = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
        scratchValue7 = defensiveGuardBandit - defensiveGuardBandit >> 31
        scratchValue8 = 0
        if #defensiveGuardBandit + scratchValue7 ~= scratchValue7 then break end
        goto LAB_00d108eb
        quest:NewScriptFrame()
        predicateResult3 = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
    while true do
        -- TODO(native): xStack_84 = (CCharString)(xStack_5c + iVar10);
        quest:GiveThingBestEnemyTarget(hero, nil --[[missing]])
        scratchValue8 = scratchValue8 + 1
        if #defensiveGuardBandit <= scratchValue8 then break end
        if quest:IsActiveThreadTerminating() then goto LAB_00d108f6 end
    end
    ::LAB_00d108eb::
    if quest:IsActiveThreadTerminating() then
        goto LAB_00d108f6
    else
        local defensiveGuardLeader = quest:GetThingWithScriptName("DefensiveGuardLeader")
        quest:GiveThingBestEnemyTarget(quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", defensiveGuardLeader:GetPos(), ""), hero)
        quest:SetStateBool("Gate3Open", true)
        return
    end
    goto FLOW_past_lab_00d108f6
    ::LAB_00d108f6::
    ::FLOW_past_lab_00d108f6::
    do return end
    quest:SetStateBool("Gate3Open", true)
end

-- Q_BanditCampBossBattle.WatchForTermination (retail 0x00d04260)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionSucceeded") then
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), false, "", true)
    else
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
        quest:SetPrizeTavernTable(true)
        quest:Pause(1.0)
        quest:FadeScreenIn()
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_BanditCampBossBattle.CheckForFirstAreaMassacre (retail 0x00d0ee70)
function CheckForFirstAreaMassacre(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue5, scratchValue6, scratchValue7, killsXStack, scratchValue8, scratchValue9
    local scratchValue10, defensiveGuardBandit
    while true do
        local gate1 = quest:GetThingWithScriptName("Gate1")
        local scratchValue = gate1 ~= nil and gate1:IsOpenDoor()
        if scratchValue then break end
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    killsXStack = 0
    repeat
        while not quest:IsLevelLoaded("BanditCampMain") do
            if not quest:NewScriptFrame() then return end
        end
        local scratchValue12 = hero:MsgGetThingsKilledGroups()
        if #scratchValue12 ~= 0 then
            killsXStack = killsXStack + #scratchValue12
        end
        local followers = quest:GetFollowingEntityList(hero)
        scratchValue8 = 0
        if #followers ~= 0 then
            scratchValue5 = 0
            repeat
                local scratchValue13 = followers[scratchValue5 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue13 ~= 0 then
                    killsXStack = killsXStack + #scratchValue13
                end
                scratchValue8 = scratchValue8 + 1
                scratchValue5 = scratchValue5 + 1
            until scratchValue8 >= #followers
        end
        local getHeroSummonedCreaturesList = quest:GetHeroSummonedCreaturesList()
        scratchValue9 = 0
        if #getHeroSummonedCreaturesList ~= 0 then
            scratchValue6 = 0
            repeat
                local scratchValue14 = getHeroSummonedCreaturesList[scratchValue6 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue14 ~= 0 then
                    killsXStack = killsXStack + #scratchValue14
                end
                scratchValue9 = scratchValue9 + 1
                scratchValue6 = scratchValue6 + 1
            until scratchValue9 >= #getHeroSummonedCreaturesList
        end
        scratchValue7 = 0
        if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_EnemiesToKillInFirstArea) <= killsXStack then
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            hero:AcquireControl(4)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            local actorMap = resources:NewActorMap()
            resources:RunMacro("CS_BANDITCAMP_ALARM_OUTER", actorMap, false, true)
            quest:RemoveThing(quest:GetThingWithScriptName("Gate2Guard1"), false, true)
            resources:DestroyActorMap(actorMap)
            hero:ReleaseControl()
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:SetStateBool("Gate2Open", true)
            defensiveGuardBandit = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
            scratchValue10 = 0
            if #defensiveGuardBandit == 0 then goto LAB_00d0f52f end
            goto LAB_00d0f4d7
        end
        if not quest:NewScriptFrame() then return end
    until false
    do return end
    ::LAB_00d0f4d7::
    while true do
        if quest:IsActiveThreadTerminating() then return end
        quest:GiveThingBestEnemyTarget(defensiveGuardBandit[scratchValue7 + 1], hero)
        scratchValue10 = scratchValue10 + 1
        scratchValue7 = scratchValue7 + 1
        if #defensiveGuardBandit <= scratchValue10 then break end
    end
    ::LAB_00d0f52f::
    if quest:IsActiveThreadTerminating() then return end
    quest:GiveThingBestEnemyTarget(quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", quest:GetThingWithScriptName("DefensiveGuardLeader"):GetPos(), ""), hero)
end

-- Q_BanditCampBossBattle.CheckForSecondAreaMassacre (retail 0x00d0f640)
function CheckForSecondAreaMassacre(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue4, scratchValue5, killsXStack, scratchValue7, scratchValue8, scratchValue9
    local actorMap
    killsXStack = 0
    if quest:IsActiveThreadTerminating() then return end
    while true do
        while not quest:IsLevelLoaded("BanditCampResidential") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:GetStateBool("Gate3Open") then
            return
        end
        local scratchValue = hero:MsgGetThingsKilledGroups()
        if #scratchValue ~= 0 then
            killsXStack = killsXStack + #scratchValue
        end
        local followers = quest:GetFollowingEntityList(hero)
        scratchValue7 = 0
        if #followers ~= 0 then
            scratchValue4 = 0
            repeat
                local scratchValue12 = followers[scratchValue4 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue12 ~= 0 then
                    killsXStack = killsXStack + #scratchValue12
                end
                scratchValue7 = scratchValue7 + 1
                scratchValue4 = scratchValue4 + 1
            until scratchValue7 >= #followers
        end
        local getHeroSummonedCreaturesList = quest:GetHeroSummonedCreaturesList()
        scratchValue8 = 0
        if #getHeroSummonedCreaturesList ~= 0 then
            scratchValue5 = 0
            repeat
                local scratchValue13 = getHeroSummonedCreaturesList[scratchValue5 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue13 ~= 0 then
                    killsXStack = killsXStack + #scratchValue13
                end
                scratchValue8 = scratchValue8 + 1
                scratchValue5 = scratchValue5 + 1
            until scratchValue8 >= #getHeroSummonedCreaturesList
        end
        if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_EnemiesToKillInSecondArea) <= killsXStack then break end
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() then
        local movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        hero:AcquireControl(4)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        -- TODO(native): xStack_38._0_4_ = malloc(0x24);
        -- TODO(native): *(undefined1 *)xStack_38._0_4_ = 0;
        -- TODO(native): *(undefined **)(xStack_38._0_4_ + 4) = (undefined *)0x0;
        -- TODO(native): *(undefined4 *)(xStack_38._0_4_ + 8) = xStack_38._0_4_;
        -- TODO(native): *(undefined4 *)(xStack_38._0_4_ + 0xc) = xStack_38._0_4_;
        resources:RunMacro("CS_BANDITCAMP_ALARM_INNER", actorMap, false, true)
        quest:RemoveThing(quest:GetThingWithScriptName("Gate3Guard"), false, true)
        resources:DestroyActorMap(actorMap)
        hero:ReleaseControl()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:SetStateBool("Gate3Open", true)
        local defensiveGuardBandit = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
        scratchValue9 = 0
        if #defensiveGuardBandit ~= 0 then
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d0fe48 end
                -- TODO(native): xStack_6c = (CCharString)(iStack_50 + iVar6);
                quest:GiveThingBestEnemyTarget(hero, nil --[[missing]])
                scratchValue9 = scratchValue9 + 1
            until scratchValue9 >= #defensiveGuardBandit
        end
        if not quest:IsActiveThreadTerminating() then
            quest:GiveThingBestEnemyTarget(quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", quest:GetThingWithScriptName("DefensiveGuardLeader"):GetPos(), ""), hero)
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

-- Q_BanditCampBossBattle.CheckForSecondAreaDoorHelp (retail 0x00d043a0)
function CheckForSecondAreaDoorHelp(quest)
    local predicateResult, switch1, scratchValue
    while not quest:GetStateBool("Gate3Open") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    scratchValue = 0
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(timerId)
            return
        end
        while not quest:IsLevelLoaded("BanditCampResidential") do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
            if quest:IsLevelLoaded("BanditCampBoss") then
                if not quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                quest:DeregisterTimer(timerId)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        if 0 < quest:GetTimer(timerId) then goto LAB_00d046b8 end
        switch1 = scratchValue
        repeat
            if switch1 == 0 then
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10", "", true, true)
                scratchValue = 1
                break
            elseif switch1 == 1 then
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_20", "", true, true)
                scratchValue = 2
                break
            else
                if switch1 == 2 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_30", "", true, true)
                    goto LAB_00d0469d
                elseif switch1 == 3 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_40", "", true, true)
                    scratchValue = 4
                    break
                elseif switch1 == 4 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_50", "", true, true)
                    goto LAB_00d0469d
                end
                goto FLOW_past_lab_00d0469d
                ::LAB_00d0469d::
                scratchValue = 3
                ::FLOW_past_lab_00d0469d::
            end
        until true
        quest:SetTimer(timerId, 20)
        ::LAB_00d046b8::
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- Q_BanditCampBossBattle.OpenGate (retail 0x00d0ebc0)
function OpenGate(quest, timeDelay, doorName)
    quest:Pause(timeDelay)
    quest:OpenDoor(quest:GetThingWithScriptName(doorName))
end

