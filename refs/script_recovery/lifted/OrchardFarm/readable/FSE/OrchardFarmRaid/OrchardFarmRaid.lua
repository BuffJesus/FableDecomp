-- Readable native conversion: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local TUTORIAL_CATEGORY_COMBAT_MULTIPLIER = 9  -- ETutorialCategory (Ego_r.pdb)
local TUTORIAL_CATEGORY_FLOURISHING_MOVE = 18  -- ETutorialCategory (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    ExperienceForDefeatingWhisperInOrchardFarm = 3432,  -- 500
    OFEvilCompletionMorality = 3480,  -- -0.05999999865889549
    OFGoodCompletionMorality = 3484,  -- 0.05999999865889549
}

-- Q_OrchardFarmRaid.Main (retail 0x00dcc770)
function Main(quest)
    quest:AddEntityBinding("GuardTeamSpawn", "OrchardFarmRaid/Entities/TeamSpawn", 1)
    quest:AddEntityBinding("BanditTeamSpawn", "OrchardFarmRaid/Entities/TeamSpawn", 1)
    quest:AddEntityBinding("Artefact", "OrchardFarmRaid/Entities/Artefact", 1)
    quest:AddEntityBinding("OrchardFarmWhisper", "OrchardFarmRaid/Entities/OrchardFarmWhisper", 1)
    quest:AddEntityBinding("GuardTeamMember", "OrchardFarmRaid/Entities/CrateTeamMember", 1)
    quest:AddEntityBinding("BanditTeamMember", "OrchardFarmRaid/Entities/CrateTeamMember", 1)
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
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then return end
            local banditTeamCrateDrop = quest:GetThingWithScriptName("BanditTeamCrateDrop")
            quest:SetStateThing("Teams_1_CrateDropPos", banditTeamCrateDrop)
            local guardTeamCrateDrop = quest:GetThingWithScriptName("GuardTeamCrateDrop")
            quest:SetStateThing("Teams_0_CrateDropPos", guardTeamCrateDrop)
            if quest:GetStateInt("HeroTeam") == 1 then
                quest:CreateThread("ProcessGameRulesEvil")  -- native thread body 0x00DD03D0: lift it as function ProcessGameRulesEvil(quest)
            else
                quest:CreateThread("ProcessGameRulesGood")  -- native thread body 0x00DD0F60: lift it as function ProcessGameRulesGood(quest)
            end
            if not scratchValue then
            end
            quest:CreateThread("DoCutsceneIfRequired")  -- native thread body 0x00DCFA60: lift it as function DoCutsceneIfRequired(quest)
            if not scratchValue then
            end
            quest:CreateThread("WatchForExternalScriptDeactivation")  -- native thread body CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation: lift it as function WatchForExternalScriptDeactivation(quest)
            if not scratchValue then
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        scratchValue = quest:IsRegionLoaded("OrchardFarm")
    end
end

-- Q_OrchardFarmRaid.Init (retail 0x00dcc140)
function Init(quest)
    quest:SetStateInt("Teams_0_MemberCount", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_0_StateCounter_0", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_0_StateCounter_1", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_0_StateCounter_2", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_0_StateCounter_3", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_0_StateCounter_4", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_0_StateCounter_5", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_MemberCount", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_StateCounter_0", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_StateCounter_1", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_StateCounter_2", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_StateCounter_3", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_StateCounter_4", 0)  -- native constructor: initial value
    quest:SetStateInt("Teams_1_StateCounter_5", 0)  -- native constructor: initial value
    quest:SetStateInt("CommentTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("RemindHeroOfObjectivesTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_0_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_1_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    local string
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
        string = "GreatwoodEntrance"
    else
        if not quest:IsQuestActive("Q_OrchardFarmRaidGood") then
            return
        end
        quest:SetStateInt("HeroTeam", 0)
        quest:SetStateString("TextSystemScriptCode", "TEXT_QST_052_")
        quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "", "Greatwood")
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", false)
        string = "GreatwoodLake"
    end
    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", string, true)
end

-- Q_OrchardFarmRaid.OnPersist (retail 0x00dcc720)
function OnPersist(quest, context)
    local shownCombatMultiplierTutorial = quest:GetStateBool("ShownCombatMultiplierTutorial") or false
    shownCombatMultiplierTutorial = quest:PersistTransferBool(context, "ShownCombatMultiplierTutorial", shownCombatMultiplierTutorial)
    quest:SetStateBool("ShownCombatMultiplierTutorial", shownCombatMultiplierTutorial)
    local heroMetWhisperBeforeFarm = quest:GetStateBool("HeroMetWhisperBeforeFarm") or false
    heroMetWhisperBeforeFarm = quest:PersistTransferBool(context, "HeroMetWhisperBeforeFarm", heroMetWhisperBeforeFarm)
    quest:SetStateBool("HeroMetWhisperBeforeFarm", heroMetWhisperBeforeFarm)
end

-- Q_OrchardFarmRaid.ProcessGameRulesEvil (retail 0x00dd03d0)
function ProcessGameRulesEvil(quest)
    local resources = quest:RetailResources()
    local addQuestInfoCounter, pppuVar, actorMap, movie, resource, resource2
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource2)
    end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayQuestInfo(true)
    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
    local infoCounter = addQuestInfoCounter
    if quest:IsActiveThreadTerminating() then return end
    repeat
        quest:UpdateQuestInfoCounter(addQuestInfoCounter, 3 - quest:GetStateInt("CrateCount"), -1)
        if quest:GetStateInt("CrateCount") == 0 and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("WhisperInCutscene", true)
            quest:SetStateBool("WhisperSpawned", true)
            quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
            local pPosition = quest:GetThingWithScriptName("GuardTeamSpawn"):GetPos()
            local rivalHeroWhisperOrchardFarm = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, "OrchardFarmWhisper", false)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:MiniMapAddMarker(rivalHeroWhisperOrchardFarm, "HUD_ORB_RED_SMALL")
            quest:EntityAttachToScript(rivalHeroWhisperOrchardFarm, "Q_OrchardFarmRaid")
            local mkOfwbWhisper = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
            local mkOfwfWhisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
            resource2 = resources:NewResource()
            resource = resources:NewResource()
            resources:TryAcquire(resource, rivalHeroWhisperOrchardFarm, 4)
            pppuVar = resource2
            local hero = quest:GetHero()
            resources:TryAcquire(pppuVar, hero, 4)
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource2)
            resources:SetActor(actorMap, "WHISPER", resource)
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            local hero2 = quest:GetHero()
            local scratchValue = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(mkOfwfWhisper, hero2)))
            local hero3 = quest:GetHero()
            if scratchValue <= resources:DistanceBetweenThings(mkOfwbWhisper, hero3) then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", actorMap, false, true)
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", actorMap, false, true)
            end
            quest:FixMovieSequenceCamera(false)
            if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if quest:DisplayTutorial(TUTORIAL_CATEGORY_FLOURISHING_MOVE) then
                    if not quest:IsActiveThreadTerminating() then
                        while not quest:MsgIsTutorialClickedPast() do
                            if not quest:NewScriptFrame() then ReleaseEverything(); return end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00dd0977 end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource2)
                    return
                end
                ::LAB_00dd0977::
                quest:SetStateBool("ShownCombatFlourishTutorial", true)
            end
            quest:SetStateBool("WhisperInCutscene", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            resources:ReleaseResource(resource2)
            addQuestInfoCounter = infoCounter
        end
        local hero4 = quest:GetHero()
        if hero4 ~= nil and hero4:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.ExperienceForDefeatingWhisperInOrchardFarm))
            local orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
            resource = resources:NewResource()
            pppuVar = resource
            resource2 = resources:NewResource()
            local hero5 = quest:GetHero()
            resources:TryAcquire(pppuVar, hero5, 4)
            resources:TryAcquire(resource2, orchardFarmWhisper, 4)
            local actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", resource)
            resources:SetActor(actorMap2, "WHISPER", resource2)
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", actorMap2, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap2)
            quest:AddLogbookStoryEntry(85)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.OFEvilCompletionMorality))
            quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            local ofFarmhouseDoor = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(ofFarmhouseDoor, true)
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
            quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetStateInt("MissionFailed") ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            ReplaceQuestCards(quest)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            local pMessage = quest:GetStateString("FailReasons_" .. quest:GetStateInt("MissionFailed"))
            local getActiveQuestName2 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(getActiveQuestName2, true, pMessage, true)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- Q_OrchardFarmRaid.ProcessGameRulesGood (retail 0x00dd0f60)
function ProcessGameRulesGood(quest)
    local resources = quest:RetailResources()
    local addQuestInfoCounter, ofFarmhouseDoor, getActiveQuestName, pppuVar, resource, actorMap
    local resource2, resource3
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(resource2)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource3)
    end
    while not quest:GetStateBool("DoneIntroduction") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayQuestInfo(true)
    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
    local infoCounter = addQuestInfoCounter
    if quest:IsActiveThreadTerminating() then return end
    repeat
        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("CrateCount"), -1)
        if quest:GetStateInt("CrateCount") == 0 and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 2)
        end
        local hero = quest:GetHero()
        if hero ~= nil and hero:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if 2 < quest:GetStateInt("BanditWavesSpawned") and not quest:GetStateBool("WhisperSpawned") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("WhisperSpawned", true)
            quest:SetStateBool("WhisperInCutscene", true)
            quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            local pPosition = quest:GetThingWithScriptName("BanditTeamSpawn"):GetPos()
            local rivalHeroWhisperOrchardFarm = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, "OrchardFarmWhisper", false)
            quest:EntityAttachToScript(rivalHeroWhisperOrchardFarm, "Q_OrchardFarmRaid")
            quest:MiniMapAddMarker(rivalHeroWhisperOrchardFarm, "HUD_ORB_RED_SMALL")
            local mkOfwbWhisper = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
            local mkOfwfWhisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
            resource3 = resources:NewResource()
            resource = resources:NewResource()
            resources:TryAcquire(resource, rivalHeroWhisperOrchardFarm, 4)
            pppuVar = resource3
            local hero2 = quest:GetHero()
            resources:TryAcquire(pppuVar, hero2, 4)
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource3)
            resources:SetActor(actorMap, "WHISPER", resource)
            resource2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            local hero3 = quest:GetHero()
            local scratchValue = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(mkOfwfWhisper, hero3)))
            local hero4 = quest:GetHero()
            if scratchValue <= resources:DistanceBetweenThings(mkOfwbWhisper, hero4) then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", actorMap, false, true)
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", actorMap, false, true)
            end
            quest:FixMovieSequenceCamera(false)
            if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if quest:DisplayTutorial(TUTORIAL_CATEGORY_FLOURISHING_MOVE) then
                    if not quest:IsActiveThreadTerminating() then
                        while not quest:MsgIsTutorialClickedPast() do
                            if not quest:NewScriptFrame() then ReleaseEverything(); return end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00dd157a end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(resource2)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource3)
                    return
                end
                ::LAB_00dd157a::
                quest:SetStateBool("ShownCombatFlourishTutorial", true)
            end
            quest:SetStateBool("WhisperInCutscene", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource2)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            resources:ReleaseResource(resource3)
            addQuestInfoCounter = infoCounter
        end
        if quest:GetStateInt("CrateCount") == 0 and quest:IsActiveThreadTerminating() then return end
        local hero5 = quest:GetHero()
        if hero5 ~= nil and hero5:MsgIsKilledBy("") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("MissionFailed", 1)
        end
        if quest:GetStateBool("MissionSucceeded") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.ExperienceForDefeatingWhisperInOrchardFarm))
            local orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
            resource3 = resources:NewResource()
            resource2 = resources:NewResource()
            pppuVar = resource3
            local hero6 = quest:GetHero()
            resources:TryAcquire(pppuVar, hero6, 4)
            resources:TryAcquire(resource2, orchardFarmWhisper, 4)
            local actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", resource3)
            resources:SetActor(actorMap2, "WHISPER", resource2)
            resource = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_ORCHARD_GOOD_OUTRO", actorMap2, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource)
            resources:DestroyActorMap(actorMap2)
            quest:AddLogbookStoryEntry(80)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.OFGoodCompletionMorality))
            if not quest:GetMasterGameState("OFBRCratesStolen") then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1a93 end
                quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                if not quest:NewScriptFrame() then goto LAB_00dd1a93 end
            end
            quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            ofFarmhouseDoor = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(ofFarmhouseDoor, true)
            getActiveQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
            quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            ::LAB_00dd1a93::
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource3)
            return
        end
        if quest:GetStateInt("MissionFailed") ~= 0 then
            if quest:IsActiveThreadTerminating() then return end
            ReplaceQuestCards(quest)
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            local pMessage = quest:GetStateString("FailReasons_" .. quest:GetStateInt("MissionFailed"))
            local getActiveQuestName2 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(getActiveQuestName2, true, pMessage, true)
            quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- Q_OrchardFarmRaid.DoCutsceneIfRequired (retail 0x00dcfa60)
function DoCutsceneIfRequired(quest)
    local resources = quest:RetailResources()
    local predicateResult
    local heroTeam = quest:GetStateInt("HeroTeam")
    local thing = nil
    if heroTeam == 1 then
        if quest:IsActiveThreadTerminating() then return end
        thing = quest:GetThingWithScriptName("BanditTeamSpawn")
    else
        if quest:IsActiveThreadTerminating() then return end
        thing = quest:GetThingWithScriptName("GuardTeamSpawn")
    end
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            thing = nil
            return
        end
        while not quest:IsRegionLoaded("OrchardFarm") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                thing = nil
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        local hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(thing, hero, 10.0) then
            if quest:IsActiveThreadTerminating() then return end
            if heroTeam == 1 then
                if quest:IsActiveThreadTerminating() then return end
                local banditTeamMember = quest:GetAllThingsWithScriptName("BanditTeamMember")
                local guardTeamMember = quest:GetNearestWithScriptName(thing, "GuardTeamMember")
                local resource6 = resources:NewResource()
                local resource5 = resources:NewResource()
                local resource4 = resources:NewResource()
                local resource = resources:NewResource()
                resources:TryAcquire(resource, guardTeamMember, 4)
                resources:TryAcquire(resource6, banditTeamMember[0 + 1], 4)
                resources:TryAcquire(resource5, banditTeamMember[1 + 1], 4)
                local hero2 = quest:GetHero()
                resources:TryAcquire(resource4, hero2, 4)
                quest:SheatheHeroWeapons()
                local actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "HERO", resource4)
                resources:SetActor(actorMap, "BAN1", resource6)
                resources:SetActor(actorMap, "BAN2", resource5)
                resources:SetActor(actorMap, "GUARD", resource)
                local movie = resources:StartMovie("")
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
                local mainGates = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates)
            else
                if quest:IsActiveThreadTerminating() then return end
                local mainGates2 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates2)
                local resource7 = resources:NewResource()
                local hero3 = quest:GetHero()
                resources:TryAcquire(resource7, hero3, 4)
                local actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2, "HERO", resource7)
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", actorMap2, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap2)
                resources:ReleaseResource(resource7)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", true, false)
            end
            quest:DeactivateQuest("Q_OrchardFarm_Barricade", 0)
            quest:Pause(1.0)
            quest:SetStateBool("DoneIntroduction", true)
            local ofFarmhouseDoor = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(ofFarmhouseDoor, false)
            return
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("HeroAtWrongEntrance", true)
        local mainGates3 = quest:GetThingWithScriptName("OF_MainGates")
        quest:CloseDoor(mainGates3)
        local mainGates4 = quest:GetThingWithScriptName("OF_MainGates")
        quest:SetThingAsUsable(mainGates4, false)
        quest:Pause(1.0)
        quest:DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE")
        while not quest:MsgIsGameInfoClickedPast() do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("OrchardFarm") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("HeroAtWrongEntrance", false)
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- Q_OrchardFarmRaid.WatchForExternalScriptDeactivation (retail 0x00dccf30)
function WatchForExternalScriptDeactivation(quest)
    local predicateResult
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:IsQuestActive("Q_OrchardFarmRaidGood") then
            goto LAB_00dccfa3
        else
            predicateResult = true
            if quest:IsQuestActive("Q_OrchardFarmRaidEvil") then goto LAB_00dccfa3 end
        end
        goto FLOW_past_lab_00dccfa3
        ::LAB_00dccfa3::
        predicateResult = false
        ::FLOW_past_lab_00dccfa3::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then return end
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
    local timeRemaining = quest:GetTimer(commentTimer)
    local pSpeaker = speaker
    if 0 < timeRemaining then
        quest:IsActiveThreadTerminating()
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    local hero = quest:GetHero()
    quest:AddPersonToConversation(conversationID, hero)
    hero = quest:GetHero()
    local getDataString = pSpeaker:GetDataString()
    getDataString = ((quest:GetStateString("TextSystemScriptCode") .. getDataString) .. "_") .. commentToMake
    quest:AddLineToConversation(conversationID, getDataString, pSpeaker, hero, false)
    quest:SetTimer(commentTimer, 5)
    return true
end

-- Q_OrchardFarmRaid.DoMultiplierCutscene (retail 0x00dd1af0)
function DoMultiplierCutscene(quest)
    local resources = quest:RetailResources()
    local string
    local heroTeam = quest:GetStateInt("HeroTeam")
    local resource = resources:NewResource()
    local hero2 = quest:GetHero()
    resources:TryAcquire(resource, hero2, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    if quest:IsRegionLoaded("GreatwoodLake") and heroTeam == 0 then
        if not quest:IsActiveThreadTerminating() then string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL"; goto LAB_00dd1d98 end
        goto LAB_00dd1d15
    else
        if not quest:IsActiveThreadTerminating() then
            local hero = quest:GetHero()
            local mkOfiGwl = quest:GetThingWithScriptName("MK_OFI_GWL")
            if quest:IsDistanceBetweenThingsUnder(mkOfiGwl, hero, 20.0) then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1d15 end
                if heroTeam ~= 1 then
                    if not quest:IsActiveThreadTerminating() then string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"; goto LAB_00dd1d98 end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    return
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    return
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                goto LAB_00dd1d98
            elseif not quest:IsActiveThreadTerminating() then
                if heroTeam == 1 then
                    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00dd1d98_2 end
                    string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                else
                    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00dd1d98_2 end
                    string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                end
                goto LAB_00dd1d98
            end
        end
        goto FLOW_hoist_lab_00dd1d98_2
    end
    goto FLOW_past_lab_00dd1d15
    ::LAB_00dd1d15::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00dd1d15::
    goto FLOW_past_lab_00dd1d98
    ::LAB_00dd1d98::
    resources:RunMacro(string, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    if quest:GetStateBool("ShownCombatMultiplierTutorial") or quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00dd1d98_2 end
    if not quest:DisplayTutorial(TUTORIAL_CATEGORY_COMBAT_MULTIPLIER) then quest:SetStateBool("ShownCombatMultiplierTutorial", true); goto FLOW_hoist_lab_00dd1d98_2 end
    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00dd1d98_2 end
    while not quest:MsgIsTutorialClickedPast() do
        if not quest:NewScriptFrame() then goto FLOW_hoist_lab_00dd1d98_2 end
    end
    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00dd1d98_2 end
    quest:SetStateBool("ShownCombatMultiplierTutorial", true)
    ::FLOW_hoist_lab_00dd1d98_2::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00dd1d98::
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

-- Q_OrchardFarmRaid.ReplaceQuestCards (retail 0x00dd0eb0)
function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
end

