-- Readable native conversion: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    ExperienceForDefeatingWhisperInOrchardFarm = 3432,  -- 500
    OFEvilCompletionMorality = 3480,  -- -0.05999999865889549
    OFGoodCompletionMorality = 3484,  -- 0.05999999865889549
}

-- Q_OrchardFarmRaid.Main (retail 0x00dcc770)
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

-- Q_OrchardFarmRaid.Init (retail 0x00dcc140)
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

-- Q_OrchardFarmRaid.OnPersist (retail 0x00dcc720)
function OnPersist(quest, context)
end

-- Q_OrchardFarmRaid.ProcessGameRulesEvil (retail 0x00dd03d0)
function ProcessGameRulesEvil(quest)
    local hero = quest:GetHero()
    local addQuestInfoCounter, scratchValue, rivalHeroWhisperOrchardFarm, scratchValue6
    local mkOfwfWhisper
    local function __cleanup_LAB_00dd0b11()
        quest:EndCutscene()
    end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayQuestInfo(true)
    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        quest:UpdateQuestInfoCounter(addQuestInfoCounter, 3 - quest:GetStateInt("CrateCount"), -1)
        if quest:GetStateInt("CrateCount") == 0 and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("WhisperInCutscene", true)
            quest:SetStateBool("WhisperSpawned", true)
            quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
            rivalHeroWhisperOrchardFarm = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", quest:GetThingWithScriptName("GuardTeamSpawn"):GetPos(), "OrchardFarmWhisper")
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:MiniMapAddMarker(rivalHeroWhisperOrchardFarm, "HUD_ORB_RED_SMALL")
            quest:EntityAttachToScript(rivalHeroWhisperOrchardFarm, "Q_OrchardFarmRaid")
            scratchValue6 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
            mkOfwfWhisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
            quest:StartCutscene({HERO = hero, WHISPER = rivalHeroWhisperOrchardFarm}, {}, true)
            scratchValue = quest:GetDistanceBetweenThings(mkOfwfWhisper, hero) ^ 2
            if scratchValue <= (quest:GetDistanceBetweenThings(scratchValue6, hero) ^ 2) then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd0b11(); return end
                quest:RunCutscene("CS_ORCHARD_EVIL_WHISPER_BACK", true, false)
            else
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd0b11(); return end
                quest:RunCutscene("CS_ORCHARD_EVIL_WHISPER_FRONT", true, false)
            end
            quest:FixMovieSequenceCamera(false)
            if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd0b11(); return end
                if quest:DisplayTutorial(18) then
                    if quest:IsActiveThreadTerminating() then quest:EndCutscene(); return end
                    while not quest:MsgIsTutorialClickedPast() do
                        if not quest:NewScriptFrame() then __cleanup_LAB_00dd0b11(); return end
                    end
                end
                quest:SetStateBool("ShownCombatFlourishTutorial", true)
            end
            quest:SetStateBool("WhisperInCutscene", false)
            quest:EndCutscene()
            addQuestInfoCounter = scratchValue
        end
        if hero:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.ExperienceForDefeatingWhisperInOrchardFarm))
            quest:StartCutscene({HERO = hero, WHISPER = quest:GetThingWithScriptName("OrchardFarmWhisper")}, {}, false)
            quest:RunCutscene("CS_ORCHARD_EVIL_OUTRO", true, false)
            quest:EndCutscene()
            quest:AddLogbookStoryEntry(85)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.OFEvilCompletionMorality))
            quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:SetThingAsUsable(quest:GetThingWithScriptName("OFFarmhouseDoor"), true)
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            quest:EndCutscene()
            return
        end
        if quest:GetStateInt("MissionFailed") ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            ReplaceQuestCards(quest)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, quest:GetStateString("FailReasons_" .. quest:GetStateInt("MissionFailed")), true)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- Q_OrchardFarmRaid.ProcessGameRulesGood (retail 0x00dd0f60)
function ProcessGameRulesGood(quest)
    local hero = quest:GetHero()
    local ePriority, addQuestInfoCounter, rivalHeroWhisperOrchardFarm, scratchValue, mkOfwfWhisper
    local function __cleanup_LAB_00dd1728()
        quest:EndCutscene()
    end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayQuestInfo(true)
    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("CrateCount"), -1)
        if quest:GetStateInt("CrateCount") == 0 and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 2)
        end
        if hero:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if 2 < quest:GetStateInt("BanditWavesSpawned") and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("WhisperSpawned", true)
            quest:SetStateBool("WhisperInCutscene", true)
            quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            rivalHeroWhisperOrchardFarm = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", quest:GetThingWithScriptName("BanditTeamSpawn"):GetPos(), "OrchardFarmWhisper")
            quest:EntityAttachToScript(rivalHeroWhisperOrchardFarm, "Q_OrchardFarmRaid")
            quest:MiniMapAddMarker(rivalHeroWhisperOrchardFarm, "HUD_ORB_RED_SMALL")
            scratchValue = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
            mkOfwfWhisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
            quest:StartCutscene({HERO = hero, WHISPER = rivalHeroWhisperOrchardFarm}, {}, true)
            if (quest:GetDistanceBetweenThings(mkOfwfWhisper, hero) ^ 2) <= (quest:GetDistanceBetweenThings(scratchValue, hero) ^ 2) then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd1728(); return end
                quest:RunCutscene("CS_ORCHARD_GOOD_WHISPER_BACK", true, false)
            else
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd1728(); return end
                quest:RunCutscene("CS_ORCHARD_GOOD_WHISPER_FRONT", true, false)
            end
            quest:FixMovieSequenceCamera(false)
            if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dd1728(); return end
                if quest:DisplayTutorial(18) then
                    if quest:IsActiveThreadTerminating() then quest:EndCutscene(); return end
                    while not quest:MsgIsTutorialClickedPast() do
                        if not quest:NewScriptFrame() then __cleanup_LAB_00dd1728(); return end
                    end
                end
                quest:SetStateBool("ShownCombatFlourishTutorial", true)
            end
            quest:SetStateBool("WhisperInCutscene", false)
            quest:EndCutscene()
        end
        if quest:GetStateInt("CrateCount") == 0 and quest:IsActiveThreadTerminating() then return end
        if hero:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.ExperienceForDefeatingWhisperInOrchardFarm))
            quest:StartCutscene({HERO = hero, WHISPER = quest:GetThingWithScriptName("OrchardFarmWhisper")}, {}, false)
            quest:RunCutscene("CS_ORCHARD_GOOD_OUTRO", true, false)
            quest:EndCutscene()
            quest:AddLogbookStoryEntry(80)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.OFGoodCompletionMorality))
            if quest:GetMasterGameState("OFBRCratesStolen") == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1a93 end
                quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                if not quest:NewScriptFrame() then goto LAB_00dd1a93 end
            end
            quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:SetThingAsUsable(quest:GetThingWithScriptName("OFFarmhouseDoor"), true)
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            ::LAB_00dd1a93::
            quest:EndCutscene()
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

-- Q_OrchardFarmRaid.DoCutsceneIfRequired (retail 0x00dcfa60)
function DoCutsceneIfRequired(quest)
    local heroTeam = quest:GetStateInt("HeroTeam")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local guardTeamMember, resource, resource4, resource5, movie, movie2, resource6, resource7
    local actorMap, banditTeamMember, actorMap2
    local scratchValue
    if heroTeam == 1 then
        if quest:IsActiveThreadTerminating() then return end
        scratchValue = quest:GetThingWithScriptName("BanditTeamSpawn")
    else
        if quest:IsActiveThreadTerminating() then return end
        scratchValue = quest:GetThingWithScriptName("GuardTeamSpawn")
    end
    repeat
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("OrchardFarm") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsDistanceBetweenThingsUnder(scratchValue, hero, 10.0) then
            if heroTeam == 1 then
                banditTeamMember = quest:GetAllThingsWithScriptName("BanditTeamMember")
                guardTeamMember = quest:GetNearestWithScriptName(scratchValue, "GuardTeamMember")
                resource6 = resources:NewResource()
                resource5 = resources:NewResource()
                resource4 = resources:NewResource()
                resource = resources:NewResource()
                resources:TryAcquire(resource, guardTeamMember, 4)
                resources:TryAcquire(resource6, banditTeamMember[0 + 1], 4)
                resources:TryAcquire(resource5, banditTeamMember[1 + 1], 4)
                resources:TryAcquire(resource4, hero, 4)
                quest:SheatheHeroWeapons()
                actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "HERO", resource4)
                resources:SetActor(actorMap, "BAN1", resource6)
                resources:SetActor(actorMap, "BAN2", resource5)
                resources:SetActor(actorMap, "GUARD", resource)
                movie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", actorMap, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource)
                resources:ReleaseResource(resource4)
                resources:ReleaseResource(resource5)
                resources:ReleaseResource(resource6)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                quest:OpenDoor(quest:GetThingWithScriptName("OF_MainGates"))
            else
                quest:OpenDoor(quest:GetThingWithScriptName("OF_MainGates"))
                resource7 = resources:NewResource()
                resources:TryAcquire(resource7, hero, 4)
                actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2[0 + 1], "HERO", resource7)
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", actorMap2[0 + 1], false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap2[0 + 1])
                resources:ReleaseResource(resource7)
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

-- Q_OrchardFarmRaid.WatchForExternalScriptDeactivation (retail 0x00dccf30)
function WatchForExternalScriptDeactivation(quest)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if not quest:IsQuestActive("Q_OrchardFarmRaidGood") and not quest:IsQuestActive("Q_OrchardFarmRaidEvil") then
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            return
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- Q_OrchardFarmRaid.MakeTeamMemberComment (retail 0x00dcda80)
function MakeTeamMemberComment(quest, commentToMake, speaker, commentType)
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    if 0 < quest:GetTimer(commentTimer) then
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    quest:AddPersonToConversation(conversationID, hero)
    quest:AddLineToConversation(conversationID, ((quest:GetStateString("TextSystemScriptCode") .. speaker:GetDataString()) .. "_") .. commentToMake, speaker, hero, false)
    quest:SetTimer(commentTimer, 5)
    return true
end

-- Q_OrchardFarmRaid.DoMultiplierCutscene (retail 0x00dd1af0)
function DoMultiplierCutscene(quest)
    local heroTeam = quest:GetStateInt("HeroTeam")
    local hero = quest:GetHero()
    local string
    quest:StartCutscene({HERO = hero}, {}, true)
    if quest:IsRegionLoaded("GreatwoodLake") and heroTeam == 0 then
        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00dd1d98 end
        quest:RunCutscene("CS_ORCHARD_GOOD_WHISPERINTRO_GWLL", true, false)
        quest:FixMovieSequenceCamera(false)
        if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
        if quest:DisplayTutorial(9) then
            if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
            while not quest:MsgIsTutorialClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00dd1e70_c1 end
            end
        end
        quest:SetStateBool("ShownCombatMultiplierTutorial", true)
        ::LAB_00dd1e70_c1::
        quest:EndCutscene()
        goto FLOW_after_lab_00dd1d98
        quest:EndCutscene()
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
        if quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("MK_OFI_GWL"), hero, 20.0) then
            if heroTeam ~= 1 then
                if not quest:IsActiveThreadTerminating() then
                    string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                else
                    quest:EndCutscene()
                    goto LAB_00dd1e95
                end
            else
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
            end
            quest:RunCutscene(string, true, false)
            quest:FixMovieSequenceCamera(false)
            if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
            if not quest:DisplayTutorial(9) then quest:SetStateBool("ShownCombatMultiplierTutorial", true); goto FLOW_after_lab_00dd1d98_109 end
            if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
            while not quest:MsgIsTutorialClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00dd1e70 end
            end
            quest:SetStateBool("ShownCombatMultiplierTutorial", true)
        else
            if heroTeam == 1 then
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
            else
                string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
            end
            quest:RunCutscene(string, true, false)
            quest:FixMovieSequenceCamera(false)
            if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
            if quest:DisplayTutorial(9) then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00dd1e70 end
                end
            end
            quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            goto FLOW_after_lab_00dd1d98_109
        end
        ::FLOW_after_lab_00dd1d98_109::
        ::LAB_00dd1e70::
        quest:EndCutscene()
    end
    ::FLOW_after_lab_00dd1d98::
    ::LAB_00dd1e95::
    quest:EndCutscene()
end

-- Q_OrchardFarmRaid.ReplaceQuestCards (retail 0x00dd0eb0)
function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
end

