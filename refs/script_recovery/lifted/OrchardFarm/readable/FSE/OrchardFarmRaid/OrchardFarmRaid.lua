-- Readable native conversion: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    quest:AddEntityBinding("GuardTeamSpawn", "OrchardFarmRaid/Entities/TeamSpawn")
    quest:AddEntityBinding("BanditTeamSpawn", "OrchardFarmRaid/Entities/TeamSpawn")
    quest:AddEntityBinding("Artefact", "OrchardFarmRaid/Entities/Artefact")
    quest:AddEntityBinding("OrchardFarmWhisper", "OrchardFarmRaid/Entities/OrchardFarmWhisper")
    quest:AddEntityBinding("GuardTeamMember", "OrchardFarmRaid/Entities/CrateTeamMember")
    quest:AddEntityBinding("BanditTeamMember", "OrchardFarmRaid/Entities/CrateTeamMember")
    quest:AddEntityBinding("FarmRearEntrance", "OrchardFarmRaid/Entities/FarmRearEntrance")
    quest:AddEntityBinding("M_WhisperFarmRaidIntro", "OrchardFarmRaid/Entities/M_WhisperFarmRaidIntro")
    quest:AddEntityBinding("MK_OFI_GWLL_WHIS2", "OrchardFarmRaid/Entities/MK_OFI_GWLL_WHIS2")
    quest:FinalizeEntityBindings()
    local scratchValue
    if not quest:IsQuestActive("Q_OrchardFarm_Barricade") then
        quest:ActivateQuest("Q_OrchardFarm_Barricade")
    end
    scratchValue = quest:IsRegionLoaded("OrchardFarm")
    while true do
        if scratchValue then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateThing("Teams_1_CrateDropPos", quest:GetThingWithScriptName("BanditTeamCrateDrop"))
            quest:SetStateThing("Teams_0_CrateDropPos", quest:GetThingWithScriptName("GuardTeamCrateDrop"))
            if quest:GetStateInt("HeroTeam") == 1 then
                quest:CreateThread("ProcessGameRulesEvil")  -- native thread body 0x00DD03D0: lift it as function ProcessGameRulesEvil(quest)
            else
                quest:CreateThread("ProcessGameRulesGood")  -- native thread body 0x00DD0F60: lift it as function ProcessGameRulesGood(quest)
            end
            quest:CreateThread("DoCutsceneIfRequired")  -- native thread body 0x00DCFA60: lift it as function DoCutsceneIfRequired(quest)
            quest:CreateThread("WatchForExternalScriptDeactivation")  -- native thread body CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation: lift it as function WatchForExternalScriptDeactivation(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        scratchValue = quest:IsRegionLoaded("OrchardFarm")
    end
end

function Init(quest)
    quest:SetStateInt("CommentTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("RemindHeroOfObjectivesTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_0_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_1_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    if quest:IsQuestActive("Q_OrchardFarmRaidGood") then
        quest:AddQuestRegion("Q_OrchardFarmRaidGood", "OrchardFarm")
    else
        quest:AddQuestRegion("Q_OrchardFarmRaidEvil", "OrchardFarm")
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
    quest:SetStateInt("MissionFailed", 0)
    quest:SetStateInt("BanditWavesSpawned", 0)
    quest:SetStateBool("ShownCombatMultiplierTutorial", false)
    quest:SetStateBool("ShownCombatFlourishTutorial", false)
    quest:SetStateBool("WhisperInCutscene", false)
    quest:SetStateInt("Teams_1_EnemyTeam", 0)
    quest:SetStateThing("Teams_0_TeamCrateCarrier", nil)
    quest:SetStateThing("Teams_1_TeamCrateCarrier", nil)
    quest:SetStateString("FailReasons_0", "PROBLEM: Tell Ben problem with Orchard Farm fail reasons")
    quest:SetStateString("FailReasons_1", "TEXT_QST_051_FAILED_HERO_KILLED")
    quest:SetStateString("FailReasons_2", "TEXT_QST_051_FAILED_CRATES_STOLEN")
    quest:SetStateString("FailReasons_3", "TEXT_QST_051_FAILED_TEAM_KILLED")
    if quest:IsQuestActive("Q_OrchardFarmRaidEvil") then
        quest:SetStateInt("HeroTeam", 1)
        quest:SetStateString("TextSystemScriptCode", "TEXT_QST_051_")
        quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "", "HeroGuildComplexInside")
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false)
    else
        if not quest:IsQuestActive("Q_OrchardFarmRaidGood") then
            return
        end
        quest:SetStateInt("HeroTeam", 0)
        quest:SetStateString("TextSystemScriptCode", "TEXT_QST_052_")
        quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "", "Greatwood")
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", false)
    end
    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true)
end

function OnPersist(quest, context)
end

function ProcessGameRulesEvil(quest)
    local resources = quest:RetailResources()
    local scratchValue3, scratchValue4, scratchValue6, scratchValue8, scratchValue9, whisper
    local orchardFarmWhisper, scratchValue10, scratchValue11, scratchValue12, scratchValue13
    local function __cleanup_LAB_00dd0b11()
        scratchValue6 = 0
        quest:PauseAllNonScriptedEntities(scratchValue6 ~= 0)
        resources:DestroyMovie(scratchValue11)
        resources:DestroyActorMap(scratchValue9)
        resources:ReleaseResource(scratchValue12)
        resources:ReleaseResource(scratchValue13)
    end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayQuestInfo(true)
    scratchValue3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        quest:UpdateQuestInfoCounter(scratchValue3, 3 - quest:GetStateInt("CrateCount"), -1)
        if quest:GetStateInt("CrateCount") == 0 and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("WhisperInCutscene", true)
            quest:SetStateBool("WhisperSpawned", true)
            quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
            scratchValue6 = quest:GetThingWithScriptName("GuardTeamSpawn")
            scratchValue8 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", scratchValue6:GetPos(), "OrchardFarmWhisper")
            quest:RemoveQuestInfoElement(scratchValue3)
            quest:MiniMapAddMarker(scratchValue8, "HUD_ORB_RED_SMALL")
            quest:EntityAttachToScript(scratchValue8, "Q_OrchardFarmRaid")
            scratchValue9 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
            whisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
            scratchValue13 = resources:NewResource()
            scratchValue12 = resources:NewResource()
            resources:TryAcquire(scratchValue12, scratchValue8, 4)
            scratchValue6 = quest:GetHero()
            resources:TryAcquire(scratchValue13, scratchValue6, 4)
            scratchValue10 = resources:NewActorMap()
            resources:SetActor(scratchValue10, "HERO", scratchValue13)
            resources:SetActor(scratchValue10, "WHISPER", scratchValue12)
            scratchValue11 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            scratchValue6 = quest:GetHero()
            scratchValue4 = quest:GetDistanceBetweenThings(whisper, scratchValue6) ^ 2
            scratchValue6 = quest:GetHero()
            if scratchValue4 <= (quest:GetDistanceBetweenThings(scratchValue9, scratchValue6) ^ 2) then
                if not quest:IsActiveThreadTerminating() then
                    resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", scratchValue9, false, true)
                    goto LAB_00dd08eb
                end
                __cleanup_LAB_00dd0b11(); return
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd0b11(); return end
            resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", scratchValue9, false, true)
            ::LAB_00dd08eb::
            quest:FixMovieSequenceCamera(false)
            if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd0b11(); return end
                if quest:DisplayTutorial(18) then
                    if not quest:IsActiveThreadTerminating() then
                        while not quest:MsgIsTutorialClickedPast() do
                            quest:NewScriptFrame()
                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd0b11(); return end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00dd0977 end
                    end
                    scratchValue6 = 0
                    quest:PauseAllNonScriptedEntities(scratchValue6 ~= 0)
                    resources:DestroyMovie(scratchValue11)
                    resources:DestroyActorMap(scratchValue9)
                    resources:ReleaseResource(scratchValue12)
                    resources:ReleaseResource(scratchValue13)
                    return
                end
                ::LAB_00dd0977::
                quest:SetStateBool("ShownCombatFlourishTutorial", true)
            end
            quest:SetStateBool("WhisperInCutscene", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue11)
            resources:DestroyActorMap(scratchValue10)
            resources:ReleaseResource(scratchValue12)
            resources:ReleaseResource(scratchValue13)
            scratchValue3 = scratchValue4
        end
        scratchValue6 = quest:GetHero()
        if scratchValue6:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
            orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
            scratchValue12 = resources:NewResource()
            scratchValue13 = resources:NewResource()
            scratchValue6 = quest:GetHero()
            resources:TryAcquire(scratchValue12, scratchValue6, 4)
            resources:TryAcquire(scratchValue13, orchardFarmWhisper, 4)
            scratchValue9 = resources:NewActorMap()
            resources:SetActor(scratchValue9, "HERO", scratchValue12)
            resources:SetActor(scratchValue9, "WHISPER", scratchValue13)
            scratchValue11 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", scratchValue9, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue11)
            resources:DestroyActorMap(scratchValue9)
            quest:AddLogbookStoryEntry(85)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(3480))
            quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
            quest:RemoveQuestInfoElement(scratchValue3)
            scratchValue6 = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(scratchValue6, true)
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            resources:ReleaseResource(scratchValue13)
            resources:ReleaseResource(scratchValue12)
            return
        end
        if quest:GetStateInt("MissionFailed") ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            ReplaceQuestCards(quest)
            quest:RemoveQuestInfoElement(scratchValue3)
            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, quest:GetStateString("FailReasons_" .. quest:GetStateInt("MissionFailed")), true)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
        end
        if not quest:NewScriptFrame() then return end
    until false
end

function ProcessGameRulesGood(quest)
    local resources = quest:RetailResources()
    local ePriority, f_xStack_18, scratchValue5, scratchValue6, scratchValue7, scratchValue8
    local scratchValue9, whisper, orchardFarmWhisper, scratchValue10, scratchValue11, scratchValue12
    local scratchValue13
    local function __cleanup_LAB_00dd1728()
        scratchValue7 = 0
        quest:PauseAllNonScriptedEntities(scratchValue7 ~= 0)
        resources:DestroyMovie(scratchValue12)
        resources:DestroyActorMap(scratchValue11)
        resources:ReleaseResource(scratchValue10)
        resources:ReleaseResource(scratchValue13)
    end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayQuestInfo(true)
    scratchValue5 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
    scratchValue6 = scratchValue5
    if quest:IsActiveThreadTerminating() then return end
    repeat
        quest:UpdateQuestInfoCounter(scratchValue5, quest:GetStateInt("CrateCount"), -1)
        if quest:GetStateInt("CrateCount") == 0 and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 2)
        end
        scratchValue7 = quest:GetHero()
        if scratchValue7:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if 2 < quest:GetStateInt("BanditWavesSpawned") and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("WhisperSpawned", true)
            quest:SetStateBool("WhisperInCutscene", true)
            quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
            quest:RemoveQuestInfoElement(scratchValue5)
            scratchValue7 = quest:GetThingWithScriptName("BanditTeamSpawn")
            scratchValue8 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", scratchValue7:GetPos(), "OrchardFarmWhisper")
            quest:EntityAttachToScript(scratchValue8, "Q_OrchardFarmRaid")
            quest:MiniMapAddMarker(scratchValue7, "HUD_ORB_RED_SMALL")
            scratchValue9 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
            whisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
            scratchValue13 = resources:NewResource()
            scratchValue10 = resources:NewResource()
            resources:TryAcquire(scratchValue10, scratchValue8, 4)
            scratchValue7 = quest:GetHero()
            resources:TryAcquire(scratchValue13, scratchValue7, 4)
            scratchValue11 = resources:NewActorMap()
            resources:SetActor(scratchValue11, "HERO", scratchValue13)
            resources:SetActor(scratchValue11, "WHISPER", scratchValue10)
            scratchValue12 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            scratchValue7 = quest:GetHero()
            f_xStack_18 = quest:GetDistanceBetweenThings(whisper, scratchValue7) ^ 2
            scratchValue7 = quest:GetHero()
            if f_xStack_18 <= (quest:GetDistanceBetweenThings(scratchValue9, scratchValue7) ^ 2) then
                if not quest:IsActiveThreadTerminating() then
                    resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", scratchValue11, false, true)
                    goto LAB_00dd14ee
                end
                __cleanup_LAB_00dd1728(); return
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd1728(); return end
            resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", scratchValue11, false, true)
            ::LAB_00dd14ee::
            quest:FixMovieSequenceCamera(false)
            if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd1728(); return end
                if quest:DisplayTutorial(18) then
                    if not quest:IsActiveThreadTerminating() then
                        while not quest:MsgIsTutorialClickedPast() do
                            quest:NewScriptFrame()
                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd1728(); return end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00dd157a end
                    end
                    scratchValue7 = 0
                    quest:PauseAllNonScriptedEntities(scratchValue7 ~= 0)
                    resources:DestroyMovie(scratchValue12)
                    resources:DestroyActorMap(scratchValue11)
                    resources:ReleaseResource(scratchValue10)
                    resources:ReleaseResource(scratchValue13)
                    return
                end
                ::LAB_00dd157a::
                quest:SetStateBool("ShownCombatFlourishTutorial", true)
            end
            quest:SetStateBool("WhisperInCutscene", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue12)
            resources:DestroyActorMap(scratchValue11)
            resources:ReleaseResource(scratchValue10)
            resources:ReleaseResource(scratchValue13)
            scratchValue5 = scratchValue6
        end
        if quest:GetStateInt("CrateCount") == 0 and quest:IsActiveThreadTerminating() then return end
        scratchValue7 = quest:GetHero()
        if scratchValue7:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
            orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
            scratchValue13 = resources:NewResource()
            scratchValue12 = resources:NewResource()
            scratchValue7 = quest:GetHero()
            resources:TryAcquire(scratchValue13, scratchValue7, 4)
            resources:TryAcquire(scratchValue12, orchardFarmWhisper, 4)
            scratchValue9 = resources:NewActorMap()
            resources:SetActor(scratchValue9, "HERO", scratchValue13)
            resources:SetActor(scratchValue9, "WHISPER", scratchValue12)
            scratchValue10 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_ORCHARD_GOOD_OUTRO", scratchValue9, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue10)
            resources:DestroyActorMap(scratchValue9)
            quest:AddLogbookStoryEntry(80)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(3484))
            if quest:GetMasterGameState("OFBRCratesStolen") == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1a93 end
                quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                if not quest:NewScriptFrame() then goto LAB_00dd1a93 end
            end
            quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
            quest:RemoveQuestInfoElement(scratchValue5)
            scratchValue7 = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(scratchValue7, true)
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            ::LAB_00dd1a93::
            resources:ReleaseResource(scratchValue12)
            resources:ReleaseResource(scratchValue13)
            return
        end
        if quest:GetStateInt("MissionFailed") ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            ReplaceQuestCards(quest)
            quest:RemoveQuestInfoElement(ePriority)
            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, quest:GetStateString("FailReasons_" .. quest:GetStateInt("MissionFailed")), true)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
        end
        if not quest:NewScriptFrame() then return end
    until false
end

function DoCutsceneIfRequired(quest)
    local resources = quest:RetailResources()
    local getNearestWithScriptName, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, scratchValue9, scratchValue10, getAllThingsWithScriptName
    local scratchValue11
    local scratchValue12
    if quest:GetStateInt("HeroTeam") == 1 then
        if quest:IsActiveThreadTerminating() then return end
        scratchValue12 = quest:GetThingWithScriptName("BanditTeamSpawn")
    else
        if quest:IsActiveThreadTerminating() then return end
        scratchValue12 = quest:GetThingWithScriptName("GuardTeamSpawn")
    end
    repeat
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("OrchardFarm") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsDistanceBetweenThingsUnder(scratchValue12, quest:GetHero(), 10.0) then
            if quest:GetStateInt("HeroTeam") == 1 then
                getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("BanditTeamMember")
                getNearestWithScriptName = quest:GetNearestWithScriptName(scratchValue12, "GuardTeamMember")
                scratchValue8 = resources:NewResource()
                scratchValue5 = resources:NewResource()
                scratchValue4 = resources:NewResource()
                scratchValue3 = resources:NewResource()
                resources:TryAcquire(scratchValue3, getNearestWithScriptName, 4)
                resources:TryAcquire(scratchValue8, getAllThingsWithScriptName[0 + 1], 4)
                resources:TryAcquire(scratchValue5, getAllThingsWithScriptName[1 + 1], 4)
                resources:TryAcquire(scratchValue4, quest:GetHero(), 4)
                quest:SheatheHeroWeapons()
                scratchValue10 = resources:NewActorMap()
                resources:SetActor(scratchValue10, "HERO", scratchValue4)
                resources:SetActor(scratchValue10, "BAN1", scratchValue8)
                resources:SetActor(scratchValue10, "BAN2", scratchValue5)
                resources:SetActor(scratchValue10, "GUARD", scratchValue3)
                scratchValue6 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", scratchValue10, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue6)
                resources:DestroyActorMap(scratchValue10)
                resources:ReleaseResource(scratchValue3)
                resources:ReleaseResource(scratchValue4)
                resources:ReleaseResource(scratchValue5)
                resources:ReleaseResource(scratchValue8)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                quest:OpenDoor(quest:GetThingWithScriptName("OF_MainGates"))
            else
                quest:OpenDoor(quest:GetThingWithScriptName("OF_MainGates"))
                scratchValue9 = resources:NewResource()
                resources:TryAcquire(scratchValue9, quest:GetHero(), 4)
                scratchValue11 = resources:NewActorMap()
                resources:SetActor(scratchValue11[0 + 1], "HERO", scratchValue9)
                scratchValue7 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", scratchValue11[0 + 1], false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue7)
                resources:DestroyActorMap(scratchValue11[0 + 1])
                resources:ReleaseResource(scratchValue9)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", true, false)
            end
            quest:DeactivateQuest("Q_OrchardFarm_Barricade", 0)
            quest:Pause(1.0)
            quest:SetStateBool("DoneIntroduction", true)
            quest:SetThingAsUsable(quest:GetThingWithScriptName("OFFarmhouseDoor"), false)
            return
        end
        quest:SetStateBool("HeroAtWrongEntrance", true)
        quest:CloseDoor(quest:GetThingWithScriptName("OF_MainGates"))
        quest:SetThingAsUsable(quest:GetThingWithScriptName("OF_MainGates"), false)
        quest:Pause(1.0)
        quest:DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE")
        while not quest:MsgIsGameInfoClickedPast() do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("OrchardFarm") do
            if not quest:NewScriptFrame() then return end
        end
        quest:SetStateBool("HeroAtWrongEntrance", false)
        quest:NewScriptFrame()
    until false
end

function WatchForExternalScriptDeactivation(quest)
    local predicateResult2
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:IsQuestActive("Q_OrchardFarmRaidGood") then
            predicateResult2 = false
        else
            predicateResult2 = true
            if quest:IsQuestActive("Q_OrchardFarmRaidEvil") then
                predicateResult2 = false
                goto FLOW_after_lab_00dccfa3
            end
        end
        ::FLOW_after_lab_00dccfa3::
        if predicateResult2 then
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            return
        end
        if not quest:NewScriptFrame() then return end
    until false
end

function MakeTeamMemberComment(quest, commentToMake, speaker, commentType)
    local pSpeaker = speaker
    if 0 < quest:GetTimer(quest:GetStateInt("CommentTimer")) then
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    quest:AddPersonToConversation(conversationID, quest:GetHero())
    quest:AddLineToConversation(conversationID, ((quest:GetStateString("TextSystemScriptCode") .. pSpeaker:GetDataString()) .. "_") .. commentToMake, pSpeaker, quest:GetHero(), false)
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return true
end

function DoMultiplierCutscene(quest)
    local resources = quest:RetailResources()
    local scratchValue, sequence12, sequence21, sequence24, pScriptObject, string, scratchValue8
    local scratchValue9, scratchValue10
    scratchValue8 = resources:NewResource()
    resources:TryAcquire(pScriptObject, quest:GetHero(), 4)
    scratchValue10 = resources:NewActorMap()
    resources:SetActor(scratchValue10, "HERO", scratchValue8)
    scratchValue9 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    scratchValue = quest:IsRegionLoaded("GreatwoodLake")
    sequence12 = not scratchValue
    if not sequence12 then
        scratchValue = true
        sequence12 = quest:GetStateInt("HeroTeam") ~= 0
    end
    if sequence12 then
        scratchValue = false
    end
    if scratchValue then
        if not quest:IsActiveThreadTerminating() then
            resources:RunMacro("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL", scratchValue10, false, true)
            quest:FixMovieSequenceCamera(false)
            sequence21 = false
            if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                sequence21 = true
            end
            if sequence21 or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
            if quest:DisplayTutorial(9) then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00dd1e70_c1 end
                end
            end
            quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            ::LAB_00dd1e70_c1::
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00dd1d98
        end
        quest:PauseAllNonScriptedEntities(false)
    else
        if not quest:IsActiveThreadTerminating() then
            if quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("MK_OFI_GWL"), quest:GetHero(), 20.0) then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d98
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    if not quest:IsActiveThreadTerminating() then
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                        goto LAB_00dd1d98
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue9)
                    goto LAB_00dd1e95
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue9)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                ::LAB_00dd1d98::
                resources:RunMacro(string, scratchValue10, false, true)
                quest:FixMovieSequenceCamera(false)
                if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                if quest:DisplayTutorial(9) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    while not quest:MsgIsTutorialClickedPast() do
                        if not quest:NewScriptFrame() then goto LAB_00dd1e70 end
                    end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                if not quest:IsActiveThreadTerminating() then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                    end
                    resources:RunMacro(string, scratchValue10, false, true)
                    quest:FixMovieSequenceCamera(false)
                    sequence24 = false
                    if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                        sequence24 = true
                    end
                    if sequence24 or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    if quest:DisplayTutorial(9) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        while not quest:MsgIsTutorialClickedPast() do
                            if not quest:NewScriptFrame() then goto LAB_00dd1e70 end
                        end
                    end
                    quest:SetStateBool("ShownCombatMultiplierTutorial", true)
                    goto FLOW_after_lab_00dd1d98_125
                end
            end
            ::FLOW_after_lab_00dd1d98_125::
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d98::
    resources:DestroyMovie(scratchValue9)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue10)
    resources:ReleaseResource(scratchValue8)
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
end

