-- Readable native conversion: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local banditTeamCrateDrop, guardTeamCrateDrop
    local alive = true
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
    local scratchValue = quest:IsQuestActive("Q_OrchardFarm_Barricade")
    if not scratchValue then
        quest:ActivateQuest("Q_OrchardFarm_Barricade")
    end
    scratchValue = quest:IsRegionLoaded("OrchardFarm")
    while true do
        if scratchValue then
            alive = not quest:IsActiveThreadTerminating()
            scratchValue = not alive
            if not scratchValue then
                banditTeamCrateDrop = quest:GetThingWithScriptName("BanditTeamCrateDrop")
                quest:SetStateThing("Teams_1_CrateDropPos", banditTeamCrateDrop)
                guardTeamCrateDrop = quest:GetThingWithScriptName("GuardTeamCrateDrop")
                quest:SetStateThing("Teams_0_CrateDropPos", guardTeamCrateDrop)
                if quest:GetStateInt("HeroTeam") == 1 then
                    quest:CreateThread("ProcessGameRulesEvil")  -- native thread body 0x00DD03D0: lift it as function ProcessGameRulesEvil(quest)
                else
                    quest:CreateThread("ProcessGameRulesGood")  -- native thread body 0x00DD0F60: lift it as function ProcessGameRulesGood(quest)
                end
                quest:CreateThread("DoCutsceneIfRequired")  -- native thread body 0x00DCFA60: lift it as function DoCutsceneIfRequired(quest)
                quest:CreateThread("WatchForExternalScriptDeactivation")  -- native thread body CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation: lift it as function WatchForExternalScriptDeactivation(quest)
            end
            return
        end
        alive = quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then break end
        scratchValue = quest:IsRegionLoaded("OrchardFarm")
    end
end

function Init(quest)
    quest:SetStateInt("CommentTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("RemindHeroOfObjectivesTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_0_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_1_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    local isQuestActive = quest:IsQuestActive("Q_OrchardFarmRaidGood")
    if isQuestActive then
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
    isQuestActive = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
    if isQuestActive then
        quest:SetStateInt("HeroTeam", 1)
        quest:SetStateString("TextSystemScriptCode", "TEXT_QST_051_")
        quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "", "HeroGuildComplexInside")
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false)
    else
        isQuestActive = quest:IsQuestActive("Q_OrchardFarmRaidGood")
        if not isQuestActive then
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
    local b3, predicateResult, scratchValue, doneIntroduction, scratchValue2, scratchValue3
    local scratchValue4, scratchValue5, scratchValue6, scratchValue7, pMessage, pPosition, pppuVar9
    local scratchValue8, scratchValue9, whisper, orchardFarmWhisper, scratchValue10, scratchValue11
    local scratchValue12, scratchValue13
    local alive = true
    local function __cleanup_LAB_00dd0b11()
        scratchValue6 = 0
        quest:PauseAllNonScriptedEntities((scratchValue6 ~= 0))
        resources:DestroyMovie(scratchValue11)
        resources:DestroyActorMap(scratchValue9)
        resources:ReleaseResource(scratchValue12)
        resources:ReleaseResource(scratchValue13)
    end
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    scratchValue = not alive
    if not scratchValue then
        quest:DisplayQuestInfo(true)
        scratchValue3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        scratchValue4 = scratchValue3
        alive = not quest:IsActiveThreadTerminating()
        scratchValue = not alive
        if not scratchValue then
            repeat
                quest:UpdateQuestInfoCounter(scratchValue3, 3 - quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
                    scratchValue6 = quest:GetThingWithScriptName("GuardTeamSpawn")
                    scratchValue = false
                    scratchValue7 = aC_stk_30
                    pPosition = scratchValue6:GetPos()
                    scratchValue8 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, scratchValue7)
                    quest:RemoveQuestInfoElement(scratchValue3)
                    quest:MiniMapAddMarker(scratchValue8, "HUD_ORB_RED_SMALL")
                    quest:EntityAttachToScript(scratchValue8, "Q_OrchardFarmRaid")
                    scratchValue9 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    whisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    scratchValue13 = resources:NewResource()
                    scratchValue12 = resources:NewResource()
                    resources:TryAcquire(scratchValue12, scratchValue8, 4)
                    scratchValue5 = 4
                    pppuVar9 = scratchValue13
                    scratchValue6 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue6, scratchValue5)
                    scratchValue10 = resources:NewActorMap()
                    resources:SetActor(scratchValue10, "HERO", scratchValue13)
                    resources:SetActor(scratchValue10, "WHISPER", scratchValue12)
                    scratchValue11 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    scratchValue6 = quest:GetHero()
                    scratchValue4 = (quest:GetDistanceBetweenThings(whisper, scratchValue6) ^ 2)
                    scratchValue6 = quest:GetHero()
                    scratchValue3 = (quest:GetDistanceBetweenThings(scratchValue9, scratchValue6) ^ 2)
                    if scratchValue4 <= scratchValue3 then
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue = not alive
                        if not scratchValue then
                            resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", scratchValue9, false, true)
                            scratchValue7 = aC_stk_30
                            goto LAB_00dd08eb
                        end
                        __cleanup_LAB_00dd0b11(); return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue = not alive
                    if scratchValue then
                        __cleanup_LAB_00dd0b11()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", scratchValue9, false, true)
                    scratchValue7 = (scratchValue8 + 8)
                    ::LAB_00dd08eb::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then __cleanup_LAB_00dd0b11(); return end
                        scratchValue = quest:DisplayTutorial(18)
                        if scratchValue then
                            alive = not quest:IsActiveThreadTerminating()
                            scratchValue = not alive
                            if not scratchValue then
                                scratchValue = quest:MsgIsTutorialClickedPast()
                                while not scratchValue do
                                    alive = quest:NewScriptFrame()
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then __cleanup_LAB_00dd0b11(); return end
                                    scratchValue = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue = not alive
                                if not scratchValue then goto LAB_00dd0977 end
                            end
                            -- LAB_00dd0b1f: (native jump target)
                            scratchValue6 = 0
                            quest:PauseAllNonScriptedEntities((scratchValue6 ~= 0))
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
                scratchValue2 = scratchValue6:MsgIsKilledBy("")
                if scratchValue2 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
                    orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    scratchValue12 = resources:NewResource()
                    scratchValue5 = 4
                    pppuVar9 = scratchValue12
                    scratchValue13 = resources:NewResource()
                    scratchValue6 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue6, scratchValue5)
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
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(3480))
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(scratchValue3)
                    scratchValue = true
                    scratchValue6 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(scratchValue6, scratchValue)
                    b3 = false
                    predicateResult = false
                    scratchValue = false
                    scratchValue7 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(scratchValue7, scratchValue, predicateResult, b3)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue = not alive
                    until not (not scratchValue)
                    resources:ReleaseResource(scratchValue13)
                    resources:ReleaseResource(scratchValue12)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(scratchValue3)
                    predicateResult = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    scratchValue = true
                    scratchValue7 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(scratchValue7, scratchValue, pMessage, predicateResult)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    return
                end
            until false
        end
    end
end

function ProcessGameRulesGood(quest)
    local resources = quest:RetailResources()
    local b3, predicateResult, scratchValue, doneIntroduction, scratchValue2, ePriority
    local scratchValue3, f_xStack_18, scratchValue4, scratchValue5, scratchValue6, sequence1
    local scratchValue7, scratchValue8, pMessage, pPosition, pppuVar9, scratchValue9, scratchValue10
    local whisper, orchardFarmWhisper, scratchValue11, scratchValue12, scratchValue13
    local scratchValue14
    local alive = true
    local function __cleanup_LAB_00dd1728()
        scratchValue7 = 0
        quest:PauseAllNonScriptedEntities((scratchValue7 ~= 0))
        resources:DestroyMovie(scratchValue13)
        resources:DestroyActorMap(scratchValue12)
        resources:ReleaseResource(scratchValue11)
        resources:ReleaseResource(scratchValue14)
    end
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    scratchValue = not alive
    if not scratchValue then
        quest:DisplayQuestInfo(true)
        scratchValue5 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        scratchValue6 = scratchValue5
        alive = not quest:IsActiveThreadTerminating()
        scratchValue = not alive
        if not scratchValue then
            repeat
                quest:UpdateQuestInfoCounter(scratchValue5, quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 2)
                end
                scratchValue7 = quest:GetHero()
                scratchValue2 = scratchValue7:MsgIsKilledBy("")
                if scratchValue2 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if (2 < quest:GetStateInt("BanditWavesSpawned")) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
                    quest:RemoveQuestInfoElement(scratchValue5)
                    scratchValue7 = quest:GetThingWithScriptName("BanditTeamSpawn")
                    scratchValue = false
                    pPosition = scratchValue7:GetPos()
                    scratchValue9 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, "OrchardFarmWhisper")
                    quest:EntityAttachToScript(scratchValue9, "Q_OrchardFarmRaid")
                    quest:MiniMapAddMarker(scratchValue7, "HUD_ORB_RED_SMALL")
                    scratchValue10 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    whisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    scratchValue14 = resources:NewResource()
                    scratchValue11 = resources:NewResource()
                    resources:TryAcquire(scratchValue11, scratchValue9, 4)
                    scratchValue4 = 4
                    pppuVar9 = scratchValue14
                    scratchValue7 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue7, scratchValue4)
                    scratchValue12 = resources:NewActorMap()
                    resources:SetActor(scratchValue12, "HERO", scratchValue14)
                    resources:SetActor(scratchValue12, "WHISPER", scratchValue11)
                    scratchValue13 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    scratchValue7 = quest:GetHero()
                    f_xStack_18 = (quest:GetDistanceBetweenThings(whisper, scratchValue7) ^ 2)
                    scratchValue7 = quest:GetHero()
                    scratchValue3 = (quest:GetDistanceBetweenThings(scratchValue10, scratchValue7) ^ 2)
                    if f_xStack_18 <= scratchValue3 then
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue = not alive
                        if not scratchValue then
                            resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", scratchValue12, false, true)
                            scratchValue8 = aC_stk_30
                            goto LAB_00dd14ee
                        end
                        __cleanup_LAB_00dd1728(); return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue = not alive
                    if scratchValue then
                        __cleanup_LAB_00dd1728()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", scratchValue12, false, true)
                    scratchValue8 = aC_stk_48
                    ::LAB_00dd14ee::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then __cleanup_LAB_00dd1728(); return end
                        scratchValue = quest:DisplayTutorial(18)
                        if scratchValue then
                            alive = not quest:IsActiveThreadTerminating()
                            scratchValue = not alive
                            if not scratchValue then
                                scratchValue = quest:MsgIsTutorialClickedPast()
                                while not scratchValue do
                                    alive = quest:NewScriptFrame()
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then __cleanup_LAB_00dd1728(); return end
                                    scratchValue = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue = not alive
                                if not scratchValue then goto LAB_00dd157a end
                            end
                            -- LAB_00dd1736: (native jump target)
                            scratchValue7 = 0
                            quest:PauseAllNonScriptedEntities((scratchValue7 ~= 0))
                            resources:DestroyMovie(scratchValue13)
                            resources:DestroyActorMap(scratchValue12)
                            resources:ReleaseResource(scratchValue11)
                            resources:ReleaseResource(scratchValue14)
                            return
                        end
                        ::LAB_00dd157a::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue13)
                    resources:DestroyActorMap(scratchValue12)
                    resources:ReleaseResource(scratchValue11)
                    resources:ReleaseResource(scratchValue14)
                    scratchValue5 = scratchValue6
                end
                sequence1 = false
                if quest:GetStateInt("CrateCount") == 0 then
                    sequence1 = true
                else
                    sequence1 = false
                end
                if sequence1 then
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue = not alive
                    if scratchValue then
                        sequence1 = true
                    else
                        sequence1 = false
                    end
                end
                if sequence1 then
                    return
                end
                scratchValue7 = quest:GetHero()
                scratchValue2 = scratchValue7:MsgIsKilledBy("")
                if scratchValue2 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
                    orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    scratchValue14 = resources:NewResource()
                    scratchValue13 = resources:NewResource()
                    ePriority = 4
                    pppuVar9 = scratchValue14
                    scratchValue7 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue7, ePriority)
                    resources:TryAcquire(scratchValue13, orchardFarmWhisper, 4)
                    scratchValue10 = resources:NewActorMap()
                    resources:SetActor(scratchValue10, "HERO", scratchValue14)
                    resources:SetActor(scratchValue10, "WHISPER", scratchValue13)
                    scratchValue11 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_GOOD_OUTRO", scratchValue10, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue11)
                    resources:DestroyActorMap(scratchValue10)
                    quest:AddLogbookStoryEntry(80)
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(3484))
                    if quest:GetMasterGameState("OFBRCratesStolen") == 0 then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00dd1a93 end
                        quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                        alive = quest:NewScriptFrame()
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00dd1a93 end
                    end
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
                    quest:RemoveQuestInfoElement(scratchValue5)
                    scratchValue = true
                    scratchValue7 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(scratchValue7, scratchValue)
                    b3 = false
                    predicateResult = false
                    scratchValue = false
                    scratchValue8 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(scratchValue8, scratchValue, predicateResult, b3)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue = not alive
                    until not (not scratchValue)
                    ::LAB_00dd1a93::
                    resources:ReleaseResource(scratchValue13)
                    resources:ReleaseResource(scratchValue14)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(ePriority)
                    predicateResult = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    scratchValue = true
                    scratchValue8 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(scratchValue8, scratchValue, pMessage, predicateResult)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    return
                end
            until false
        end
    end
end

function DoCutsceneIfRequired(quest)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult2, isRegionLoaded, isDistanceBetweenThingsUnder
    local instructionDismissed, isRegionLoaded2, isRegionLoaded3, scratchValue, hero, hero2
    local mainGates, mainGates2, hero3, ofFarmhouseDoor, mainGates3, mainGates4, scratchValue2
    local scratchValue3, getAllThingsWithScriptName, getNearestWithScriptName, scratchValue4
    local scratchValue5, scratchValue6, scratchValue7, scratchValue8, scratchValue9, scratchValue10
    local scratchValue11, scratchValue12
    local alive = true
    if quest:GetStateInt("HeroTeam") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
        if predicateResult then
            -- LAB_00dd03b8: (native jump target)
            return
        end
        scratchValue = quest:GetThingWithScriptName("BanditTeamSpawn")
    else
        if quest:IsActiveThreadTerminating() then
            return
        end
        scratchValue = quest:GetThingWithScriptName("GuardTeamSpawn")
    end
    -- LAB_00dcfb65: (native jump target)
    alive = not quest:IsActiveThreadTerminating()
    predicateResult2 = not alive
    repeat
        if predicateResult2 then
            -- LAB_00dcfe47: (native jump target)
            return
        end
        isRegionLoaded = quest:IsRegionLoaded("OrchardFarm")
        while not isRegionLoaded do
            alive = quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
            isRegionLoaded = quest:IsRegionLoaded("OrchardFarm")
        end
        if quest:IsActiveThreadTerminating() then
            return
        end
        hero = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(xStack_94, hero, 10.0)
        if isDistanceBetweenThingsUnder then
            if quest:IsActiveThreadTerminating() then
                return
            end
            if quest:GetStateInt("HeroTeam") == 1 then
                if quest:IsActiveThreadTerminating() then
                    return
                end
                getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("BanditTeamMember")
                getNearestWithScriptName = quest:GetNearestWithScriptName(hero, "GuardTeamMember")
                scratchValue9 = resources:NewResource()
                scratchValue6 = resources:NewResource()
                scratchValue5 = resources:NewResource()
                scratchValue4 = resources:NewResource()
                resources:TryAcquire(scratchValue4, getNearestWithScriptName, 4)
                resources:TryAcquire(scratchValue9, scratchValue12, 4)
                resources:TryAcquire(scratchValue6, scratchValue12 + 1, 4)

                scratchValue2 = scratchValue5
                hero2 = quest:GetHero()
                resources:TryAcquire(scratchValue2, hero2, (4))
                quest:SheatheHeroWeapons()
                scratchValue11 = resources:NewActorMap()
                resources:SetActor(scratchValue11, "HERO", scratchValue5)
                resources:SetActor(scratchValue11, "BAN1", scratchValue9)
                resources:SetActor(scratchValue11, "BAN2", scratchValue6)
                resources:SetActor(scratchValue11, "GUARD", scratchValue4)
                scratchValue7 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", scratchValue11, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue7)
                resources:DestroyActorMap(scratchValue11)
                resources:ReleaseResource(scratchValue4)
                resources:ReleaseResource(scratchValue5)
                resources:ReleaseResource(scratchValue6)
                resources:ReleaseResource(scratchValue9)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                mainGates = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates)
            else
                if quest:IsActiveThreadTerminating() then
                    return
                end
                mainGates2 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates2)
                scratchValue10 = resources:NewResource()

                scratchValue3 = scratchValue10
                hero3 = quest:GetHero()
                resources:TryAcquire(scratchValue3, hero3, (4))
                scratchValue12 = resources:NewActorMap()
                resources:SetActor(scratchValue12, "HERO", scratchValue10)
                scratchValue8 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", scratchValue12, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue8)
                resources:DestroyActorMap(scratchValue12)
                resources:ReleaseResource(scratchValue10)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", true, false)
            end
            quest:DeactivateQuest("Q_OrchardFarm_Barricade", 0)
            quest:Pause(1.0)
            quest:SetStateBool("DoneIntroduction", true)

            ofFarmhouseDoor = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(ofFarmhouseDoor, (false))
            return
        end
        if quest:IsActiveThreadTerminating() then
            return
        end
        quest:SetStateBool("HeroAtWrongEntrance", true)
        mainGates3 = quest:GetThingWithScriptName("OF_MainGates")
        quest:CloseDoor(mainGates3)

        mainGates4 = quest:GetThingWithScriptName("OF_MainGates")
        quest:SetThingAsUsable(mainGates4, (false))
        quest:Pause(1.0)
        quest:DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE")
        instructionDismissed = quest:MsgIsGameInfoClickedPast()
        while not instructionDismissed do
            alive = quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                return
            end
            instructionDismissed = quest:MsgIsGameInfoClickedPast()
        end
        if quest:IsActiveThreadTerminating() then
            return
        end
        isRegionLoaded2 = quest:IsRegionLoaded("OrchardFarm")
        if isRegionLoaded2 then
            repeat
                alive = quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    return
                end
                isRegionLoaded3 = quest:IsRegionLoaded("OrchardFarm")
            until not (isRegionLoaded3)
        end
        if quest:IsActiveThreadTerminating() then
            return
        end
        quest:SetStateBool("HeroAtWrongEntrance", false)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        predicateResult2 = not alive
    until false
end

function WatchForExternalScriptDeactivation(quest)
    local predicateResult, isQuestActive, predicateResult2, isQuestActive2
    if quest:IsActiveThreadTerminating() then
        return
    end
    predicateResult = false
    repeat
        isQuestActive = quest:IsQuestActive("Q_OrchardFarmRaidGood")
        if isQuestActive then
            -- LAB_00dccfa3: (native jump target)
            predicateResult2 = false
        else
            predicateResult = true
            isQuestActive2 = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
            predicateResult2 = true
            if isQuestActive2 then
                predicateResult2 = false
                goto FLOW_after_lab_00dccfa3
            end
        end
        ::FLOW_after_lab_00dccfa3::
        if predicateResult then
            predicateResult = false
        end
        if predicateResult2 then
            if quest:IsActiveThreadTerminating() then
                return
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
            quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
            return
        end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            return
        end
    until false
end

function MakeTeamMemberComment(quest, commentToMake, speaker, commentType)
    local timeRemaining = quest:GetTimer(quest:GetStateInt("CommentTimer"))
    local pSpeaker = speaker
    if 0 < timeRemaining then
        return false
    end
    local conversationID = quest:AddNewConversation(speaker, false, false)
    local hero = quest:GetHero()
    quest:AddPersonToConversation(conversationID, hero)
    hero = quest:GetHero()
    local scratchValue = pSpeaker:GetDataString()
    scratchValue = (quest:GetStateString("TextSystemScriptCode") .. scratchValue)
    scratchValue = (scratchValue .. "_")
    scratchValue = (scratchValue .. commentToMake)
    quest:AddLineToConversation(conversationID, scratchValue, pSpeaker, hero, false)
    return true
end

function DoMultiplierCutscene(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, predicateResult, predicateResult2, scratchValue3
    local scratchValue4, predicateResult3, isDistanceBetweenThingsUnder, predicateResult4
    local predicateResult5, predicateResult6, predicateResult7, scratchValue5, scratchValue6
    local predicateResult8, predicateResult9, scratchValue7, scratchValue8, dist, ePriority
    local scratchValue9, hero, sequence12, sequence21, sequence23, sequence24, hero2, gwl
    local pScriptObject, string, scratchValue10
    local alive = true
    ePriority = 4
    scratchValue9 = resources:NewResource()
    hero2 = quest:GetHero()
    resources:TryAcquire(pScriptObject, hero2, ePriority)
    scratchValue10 = resources:NewActorMap()
    resources:SetActor(scratchValue10, "HERO", scratchValue9)
    scratchValue = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    scratchValue2 = quest:IsRegionLoaded("GreatwoodLake")

    if not scratchValue2 then
        sequence12 = true
    else
        sequence12 = false
    end
    if not sequence12 then
        scratchValue2 = true
        if quest:GetStateInt("HeroTeam") ~= 0 then
            sequence12 = true
        else
            sequence12 = false
        end
    end
    if sequence12 then
        scratchValue2 = false
    end
    if scratchValue2 then
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
        if not predicateResult then
            string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL"
            -- LAB_00dd1d98_c1: (native jump target)
            resources:RunMacro(string, scratchValue10, false, true)
            quest:FixMovieSequenceCamera(false)
            sequence21 = false
            if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                sequence21 = true
            end
            if not sequence21 then
                alive = not quest:IsActiveThreadTerminating()
                predicateResult2 = not alive
                if predicateResult2 then
                    sequence21 = true
                else
                    sequence21 = false
                end
            end
            if sequence21 then goto LAB_00dd1e70_c1 end
            scratchValue3 = quest:DisplayTutorial(9)
            if scratchValue3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
                scratchValue4 = quest:MsgIsTutorialClickedPast()
                while not scratchValue4 do
                    alive = quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
                    scratchValue4 = quest:MsgIsTutorialClickedPast()
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70_c1 end
            end
            quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            ::LAB_00dd1e70_c1::
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        predicateResult3 = not alive
        if not predicateResult3 then
            dist = 20.0
            hero = quest:GetHero()
            gwl = quest:GetThingWithScriptName("MK_OFI_GWL")
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(gwl, hero, dist)
            if isDistanceBetweenThingsUnder then
                alive = not quest:IsActiveThreadTerminating()
                predicateResult4 = not alive
                if predicateResult4 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d98
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult5 = not alive
                    if not predicateResult5 then
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                        goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_3c)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                predicateResult6 = not alive
                if predicateResult6 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_3c)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                ::LAB_00dd1d98::
                resources:RunMacro(string, scratchValue10, false, true)
                quest:FixMovieSequenceCamera(false)

                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    sequence23 = true
                else
                    sequence23 = false
                end
                if not sequence23 then
                    alive = not quest:IsActiveThreadTerminating()
                    predicateResult7 = not alive
                    if predicateResult7 then
                        sequence23 = true
                    else
                        sequence23 = false
                    end
                end
                if sequence23 then goto LAB_00dd1e70 end
                scratchValue5 = quest:DisplayTutorial(9)
                if scratchValue5 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    scratchValue6 = quest:MsgIsTutorialClickedPast()
                    while not scratchValue6 do
                        alive = quest:NewScriptFrame()
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        scratchValue6 = quest:MsgIsTutorialClickedPast()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                predicateResult8 = not alive
                if not predicateResult8 then
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
                    if not sequence24 then
                        alive = not quest:IsActiveThreadTerminating()
                        predicateResult9 = not alive
                        if predicateResult9 then
                            sequence24 = true
                        else
                            sequence24 = false
                        end
                    end
                    if sequence24 then goto LAB_00dd1e70 end
                    scratchValue7 = quest:DisplayTutorial(9)
                    if scratchValue7 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                        scratchValue8 = quest:MsgIsTutorialClickedPast()
                        while not scratchValue8 do
                            alive = quest:NewScriptFrame()
                            if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                            scratchValue8 = quest:MsgIsTutorialClickedPast()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00dd1e70 end
                    end
                    quest:SetStateBool("ShownCombatMultiplierTutorial", true)
                    goto FLOW_after_lab_00dd1d98_127
                end
            end
            ::FLOW_after_lab_00dd1d98_127::
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d98::
    resources:DestroyMovie(xStack_3c)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue10)
    resources:ReleaseResource(xStack_2c)
    return
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
    return
end

