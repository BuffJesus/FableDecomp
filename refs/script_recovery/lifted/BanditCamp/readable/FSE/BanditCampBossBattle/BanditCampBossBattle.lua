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
    local scratchValue
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
    if not scratchValue then
    end
    quest:CreateThread("WatchForBanditCampGates")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function WatchForBanditCampGates(quest)
    quest:CreateThread("ProcessHostageCutscenes")  -- native thread body 0x00D0FE80: lift it as function ProcessHostageCutscenes(quest)
    if not scratchValue then
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptBase_WaitForEntityAndExecuteTwoPair: lift it as function WatchForTermination(quest)
    if not scratchValue then
    end
    quest:CreateThread("CheckForFirstAreaMassacre")  -- native thread body Quest009_BanditCamp_Outer_OnLoad: lift it as function CheckForFirstAreaMassacre(quest)
    if not scratchValue then
    end
    quest:CreateThread("CheckForSecondAreaMassacre")  -- native thread body Quest009_BanditCamp_Residential_OnLoad: lift it as function CheckForSecondAreaMassacre(quest)
    if not scratchValue then
    end
    quest:CreateThread("CheckForSecondAreaDoorHelp")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function CheckForSecondAreaDoorHelp(quest)
    if not scratchValue then
    end
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
    local resources = quest:RetailResources()
    local readGlobalGameDataFloat, scratchValue4, scratchValue6
    while (1 - (quest:IsLevelLoaded("BanditCampBoss") and 1 or 0)) ~= 0 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CancelUsingAbility(15)
    quest:CancelUsingAbility(10)
    quest:SetAbilityAvailability(15, false)
    quest:SetAbilityAvailability(10, false)
    quest:AutoSaveCheckPoint()
    local gate3Inner = quest:GetThingWithScriptName("Gate3Inner")
    quest:SetThingAsUsable(gate3Inner, false)
    quest:SetTeleportingAsActive(false)
    quest:OverrideMusic(1, false, false)
    local position = quest:GetThingWithScriptName("BanditKingSpawn"):GetPos()
    local bossBanditKing = quest:CreateCreature("CREATURE_BOSS_BANDIT_KING", position, "BanditKing", false)
    local resource5 = resources:NewResource()
    local resource6 = resources:NewResource()
    resources:TryAcquire(resource5, bossBanditKing, 4)
    local pThing = quest:GetHero()
    resources:TryAcquire(resource6, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource6)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:SetAllowScreenFadingIfAlreadyFaded(true)
    resources:RunMacro("CS_BANDITKING_INTRO_BLACK", actorMap, false, false)
    quest:SetAllowScreenFadingIfAlreadyFaded(false)
    resources:SetActor(actorMap, "HERO", resource6)
    resources:SetActor(actorMap, "KING", resource5)
    resources:RunMacro("CS_BANDITKING_INTRO", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource6)
    resources:ReleaseResource(resource5)
    local fret_0 = quest:GetHealth(bossBanditKing)
    local C_stk_4c_1 = math.tointeger(math.modf(fret_0 * 0.25))
    quest:ActivateQuest("Q_BanditCamp_Barriers")
    local invisibleWall = quest:GetAllThingsWithScriptName("InvisibleWall")
    if #invisibleWall ~= 0 then
        scratchValue4 = 0
        scratchValue6 = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            quest:EntitySetAsDrawable(invisibleWall[scratchValue4 + 1], false)
            scratchValue6 = scratchValue6 + 1
            scratchValue4 = scratchValue4 + 1
        until not (scratchValue6 < (#invisibleWall))
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
    local hero = quest:GetHero()
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
        local pColour = {R = 255, G = 0, B = 0, A = 255}
        local pColour1 = {R = 255, G = 0, B = 0, A = 255}
        local fret_00 = quest:GetHealth(bossBanditKing)
        local infoElement = quest:AddQuestInfoBar(fret_00, 0.0, pColour1, pColour, "HUD_QUEST_ICON_TWINBLADE", "", 1.0)
        while C_stk_4c_1 < quest:GetHealth(bossBanditKing) do
            if not quest:NewScriptFrame() then return end
            local fret_02 = quest:GetHealth(bossBanditKing)
            quest:UpdateQuestInfoBar(infoElement, fret_02, -1.0, -1.0)
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuest("Q_BanditCamp_Barriers", 0)
        quest:DisplayQuestInfo(false)
        quest:RemoveQuestInfoElement(infoElement)
        quest:StopOverrideMusic(false)
        local fret_04 = quest:GetHealth(bossBanditKing)
        quest:ModifyThingHealth(bossBanditKing, string.unpack("<f", string.pack("<f", C_stk_4c_1 - fret_04)), false)
        if bossBanditKing ~= nil and bossBanditKing:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("BanditKingFightEnded", true)
            quest:SetMasterGameState("BodyGuardsMustStandAndWait", false)
            local position2 = quest:GetThingWithScriptName("BanditKingSpawn"):GetPos()
            local sister = quest:CreateCreature("CREATURE_SISTER", position2, "HeroSister", false)
            quest:SetStateBool("BanditsNeededForCutscene", true)
            local resource = resources:NewResource()
            local resource3 = resources:NewResource()
            local resource4 = resources:NewResource()
            resources:TryAcquire(resource, bossBanditKing, 4)
            resources:TryAcquire(resource4, sister, 4)
            local hero2 = quest:GetHero()
            resources:TryAcquire(resource3, hero2, 4)
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
            local gate3Inner2 = quest:GetThingWithScriptName("Gate3Inner")
            local crowdbandits = quest:GetNearestWithScriptName(gate3Inner2, "CROWDBANDITS")
            quest:RemoveThing(crowdbandits, false, true)
            local gate3Inner3 = quest:GetThingWithScriptName("Gate3Inner")
            local crowdbandits2 = quest:GetNearestWithScriptName(gate3Inner3, "CROWDBANDITS")
            quest:RemoveThing(crowdbandits2, false, true)
            quest:ResetPlayerCreatureCombatMultiplier()
            local scratchValue = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_TheresaExperienceGift)))
            quest:GiveHeroExperience(scratchValue)
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
        local gate3Inner4 = quest:GetThingWithScriptName("Gate3Inner")
        quest:OpenDoor(gate3Inner4)
        quest:SetAbilityAvailability(15, true)
        quest:SetAbilityAvailability(10, true)
        while (1 - (quest:IsLevelLoaded("BanditCampResidential") and 1 or 0)) ~= 0 do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        if not quest:GetStateBool("TwinBladeKilled") then
            if quest:IsActiveThreadTerminating() then return end
            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_TwinBladeSavedMorality)
        else
            if quest:IsActiveThreadTerminating() then return end
            local scratchValue3 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_RenownAward)))
            quest:GiveHeroRenownPoints(scratchValue3)
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
            if quest:IsActiveThreadTerminating() then return end
            local gate2Inner = quest:GetThingWithScriptName("Gate2Inner")
            quest:OpenDoor(gate2Inner)
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
    local resources = quest:RetailResources()
    local predicateResult2, amount, predicateResult3, scratchValue, ePriority, scratchValue4
    local sequence, resource, resource2, resource3, hero3, gate3Guard3, campHostage2, campHostage
    local scratchValue7, resource6, actorMap, movie, defensiveGuardBandit
    scratchValue4 = 0
    while not quest:IsRegionLoaded("BanditCampCentre") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    campHostage2 = quest:GetThingWithScriptName("CampHostage2")
    campHostage = quest:GetThingWithScriptName("CampHostage")
    predicateResult3 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult3 then
            campHostage = nil
            campHostage2 = nil
            return
        end
        if quest:IsRegionLoaded("BanditCampCentre") then
            if quest:IsActiveThreadTerminating() then return end
            local predicateResult = not (campHostage2 ~= nil and not campHostage2:IsNull()) or not (campHostage2 ~= nil and campHostage2:MsgIsKilledBy(""))
            if not (not predicateResult) then
                sequence = (campHostage ~= nil and not campHostage:IsNull()) and (campHostage ~= nil and campHostage:MsgIsKilledBy(""))
                if sequence then goto LAB_00d10034 end
                scratchValue = 0
                goto FLOW_past_lab_00d10034
            end
            ::LAB_00d10034::
            scratchValue = 1
            ::FLOW_past_lab_00d10034::
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
                local hero = quest:GetHero()
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
                local gate3Guard = quest:GetThingWithScriptName("Gate3Guard")
                quest:RemoveThing(gate3Guard, false, true)
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
                resource2 = resource5
                local hero2 = quest:GetHero()
                resources:TryAcquire(resource5, hero2, 4)
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
                local gate3Guard2 = quest:GetThingWithScriptName("Gate3Guard")
                quest:RemoveThing(gate3Guard2, false, true)
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
        if not quest:GetStateBool("HostageKilled") then
            quest:NewScriptFrame();
            predicateResult3 = quest:IsActiveThreadTerminating();
        else
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
            resource6 = resources:NewResource()
            ePriority = 4
            resource3 = resource6
            hero3 = quest:GetHero()
            resources:TryAcquire(resource6, hero3, 4)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            actorMap = resources:NewActorMap()
            resources:RunMacro("CS_BANDITCAMP_HOSTAGEKILL", actorMap, false, true)
            gate3Guard3 = quest:GetThingWithScriptName("Gate3Guard")
            quest:RemoveThing(gate3Guard3, false, true)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource6)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            defensiveGuardBandit = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
            scratchValue7 = 0
            if #defensiveGuardBandit ~= 0 then break end
            goto LAB_00d108eb
            predicateResult3 = quest:IsActiveThreadTerminating()
        end
        ::continue_1::
    until false
    while true do
        if quest:IsActiveThreadTerminating() then goto LAB_00d108f6 end
        local scratchValue8 = defensiveGuardBandit[scratchValue4 + 1]
        local hero4 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(scratchValue8, hero4)
        scratchValue7 = scratchValue7 + 1
        scratchValue4 = scratchValue4 + 1
        if (#defensiveGuardBandit) <= scratchValue7 then break end
    end
    ::LAB_00d108eb::
    if quest:IsActiveThreadTerminating() then
        goto LAB_00d108f6
    else
        local defensiveGuardLeader = quest:GetThingWithScriptName("DefensiveGuardLeader")
        local pPosition = defensiveGuardLeader:GetPos()
        local banditLeaderLevel = quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", pPosition, "", false)
        local hero5 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(banditLeaderLevel, hero5)
        quest:SetStateBool("Gate3Open", true)
        return
    end
    ::LAB_00d108f6::
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
        if quest:IsActiveThreadTerminating() then return end
        local pQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pQuestName, false, "", true)
    else
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
        quest:SetPrizeTavernTable(true)
        quest:Pause(1.0)
        quest:FadeScreenIn()
    end
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName2, 0)
end

-- Q_BanditCampBossBattle.CheckForFirstAreaMassacre (retail 0x00d0ee70)
function CheckForFirstAreaMassacre(quest)
    local resources = quest:RetailResources()
    local scratchValue5, scratchValue6, scratchValue7, killsXStack, gate1, scratchValue8
    local scratchValue9, scratchValue10, defensiveGuardBandit
    while true do
        gate1 = quest:GetThingWithScriptName("Gate1")
        local scratchValue = gate1 ~= nil and gate1:IsOpenDoor()
        gate1 = nil
        if scratchValue then break end
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    killsXStack = 0
    repeat
        if not quest:IsLevelLoaded("BanditCampMain") then
            if quest:IsActiveThreadTerminating() then return end
            while not quest:IsLevelLoaded("BanditCampMain") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        end
        local hero2 = quest:GetHero():MsgGetThingsKilledGroups()
        if #hero2 ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            killsXStack = killsXStack + #hero2
        end
        local hero = quest:GetHero()
        local followers = quest:GetFollowingEntityList(hero)
        scratchValue8 = 0
        if #followers ~= 0 then
            scratchValue5 = 0
            repeat
                if quest:IsActiveThreadTerminating() then return end
                local scratchValue13 = followers[scratchValue5 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue13 ~= 0 then
                    if quest:IsActiveThreadTerminating() then return end
                    killsXStack = killsXStack + #scratchValue13
                end
                scratchValue8 = scratchValue8 + 1
                scratchValue5 = scratchValue5 + 1
            until not (scratchValue8 < (#followers))
        end
        if quest:IsActiveThreadTerminating() then return end
        local getHeroSummonedCreaturesList = quest:GetHeroSummonedCreaturesList()
        scratchValue9 = 0
        if #getHeroSummonedCreaturesList ~= 0 then
            scratchValue6 = 0
            repeat
                if quest:IsActiveThreadTerminating() then return end
                local scratchValue14 = getHeroSummonedCreaturesList[scratchValue6 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue14 ~= 0 then
                    if quest:IsActiveThreadTerminating() then return end
                    killsXStack = killsXStack + #scratchValue14
                end
                scratchValue9 = scratchValue9 + 1
                scratchValue6 = scratchValue6 + 1
            until not (scratchValue9 < (#getHeroSummonedCreaturesList))
        end
        scratchValue7 = 0
        if quest:IsActiveThreadTerminating() then break end
        if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_EnemiesToKillInFirstArea) <= killsXStack then
            if quest:IsActiveThreadTerminating() then return end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local hero3 = quest:GetHero()
            hero3:AcquireControl(4)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            local actorMap = resources:NewActorMap()
            resources:RunMacro("CS_BANDITCAMP_ALARM_OUTER", actorMap, false, true)
            local gate2Guard1 = quest:GetThingWithScriptName("Gate2Guard1")
            quest:RemoveThing(gate2Guard1, false, true)
            resources:DestroyActorMap(actorMap)
            hero3:ReleaseControl()
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
        local scratchValue15 = defensiveGuardBandit[scratchValue7 + 1]
        local hero4 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(scratchValue15, hero4)
        scratchValue10 = scratchValue10 + 1
        scratchValue7 = scratchValue7 + 1
        if (#defensiveGuardBandit) <= scratchValue10 then break end
    end
    ::LAB_00d0f52f::
    if quest:IsActiveThreadTerminating() then return end
    local pPosition = quest:GetThingWithScriptName("DefensiveGuardLeader"):GetPos()
    local banditLeaderLevel = quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", pPosition, "", false)
    local hero5 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(banditLeaderLevel, hero5)
end

-- Q_BanditCampBossBattle.CheckForSecondAreaMassacre (retail 0x00d0f640)
function CheckForSecondAreaMassacre(quest)
    local resources = quest:RetailResources()
    local scratchValue4, scratchValue5, scratchValue6, killsXStack, banditLeaderLevel, scratchValue7
    local scratchValue8, scratchValue9
    killsXStack = 0
    if quest:IsActiveThreadTerminating() then return end
    while true do
        if not quest:IsLevelLoaded("BanditCampResidential") then
            if quest:IsActiveThreadTerminating() then return end
            while not quest:IsLevelLoaded("BanditCampResidential") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:GetStateBool("Gate3Open") then
            quest:IsActiveThreadTerminating()
            return
        end
        local hero2 = quest:GetHero():MsgGetThingsKilledGroups()
        if #hero2 ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0fe39 end
            killsXStack = killsXStack + #hero2
        end
        local hero = quest:GetHero()
        local followers = quest:GetFollowingEntityList(hero)
        scratchValue7 = 0
        if #followers ~= 0 then
            scratchValue4 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d0f9d8 end
                local scratchValue12 = followers[scratchValue4 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue12 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0fe39 end
                    killsXStack = killsXStack + #scratchValue12
                end
                scratchValue7 = scratchValue7 + 1
                scratchValue4 = scratchValue4 + 1
            until not (scratchValue7 < (#followers))
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00d0fa12 end
        local getHeroSummonedCreaturesList = quest:GetHeroSummonedCreaturesList()
        scratchValue8 = 0
        if #getHeroSummonedCreaturesList ~= 0 then
            scratchValue5 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d0fa46 end
                local scratchValue13 = getHeroSummonedCreaturesList[scratchValue5 + 1]:MsgGetThingsKilledGroups()
                if #scratchValue13 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0fe39 end
                    killsXStack = killsXStack + #scratchValue13
                end
                scratchValue8 = scratchValue8 + 1
                scratchValue5 = scratchValue5 + 1
            until not (scratchValue8 < (#getHeroSummonedCreaturesList))
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00d0fa7a end
        if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_EnemiesToKillInSecondArea) <= killsXStack then break end
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() then
        local movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        local hero3 = quest:GetHero()
        hero3:AcquireControl(4)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        local actorMap = resources:NewActorMap()
        resources:RunMacro("CS_BANDITCAMP_ALARM_INNER", actorMap, false, true)
        local gate3Guard = quest:GetThingWithScriptName("Gate3Guard")
        quest:RemoveThing(gate3Guard, false, true)
        resources:DestroyActorMap(actorMap)
        hero3:ReleaseControl()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:SetStateBool("Gate3Open", true)
        local defensiveGuardBandit = quest:GetAllThingsWithScriptName("DefensiveGuardBandit")
        scratchValue9 = 0
        if #defensiveGuardBandit ~= 0 then
            scratchValue6 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d0fe39 end
                local scratchValue14 = defensiveGuardBandit[scratchValue6 + 1]
                local hero4 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(scratchValue14, hero4)
                scratchValue9 = scratchValue9 + 1
                scratchValue6 = scratchValue6 + 1
            until not (scratchValue9 < (#defensiveGuardBandit))
        end
        if not quest:IsActiveThreadTerminating() then
            local pPosition = quest:GetThingWithScriptName("DefensiveGuardLeader"):GetPos()
            banditLeaderLevel = quest:CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", pPosition, "", false)
            local hero5 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(banditLeaderLevel, hero5)
            banditLeaderLevel = nil
        end
    end
    ::LAB_00d0fe39::
    do return end
    ::LAB_00d0f9d8::
    goto LAB_00d0fe39
    ::LAB_00d0fa12::
    goto LAB_00d0fe39
    ::LAB_00d0fa46::
    goto LAB_00d0fe39
    ::LAB_00d0fa7a::
    goto LAB_00d0fe39
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
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
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
    local pDoor = quest:GetThingWithScriptName(doorName)
    quest:OpenDoor(pDoor)
    pDoor = nil
end

