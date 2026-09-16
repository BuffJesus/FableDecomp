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
        -- TODO(native): this_00 = &CStack_18;
    else
        quest:AddQuestRegion("Q_OrchardFarmRaidEvil", "OrchardFarm")
        -- TODO(native): this_00 = &local_1c;
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
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, predicateResult, scratchValue5
    local doneIntroduction, scratchValue6, scratchValue7, scratchValue8, scratchValue9
    local scratchValue10, scratchValue11, pMessage, pPosition, scratchValue12, scratchValue13
    local whisper, orchardFarmWhisper
    local alive = true
    local function __cleanup_LAB_00dd0b11()
        scratchValue10 = 0
        quest:PauseAllNonScriptedEntities((scratchValue10 ~= 0))
        resources:DestroyMovie(scratchValue3)
        resources:DestroyActorMap(scratchValue13)
        resources:ReleaseResource(scratchValue4)
        resources:ReleaseResource(aCStack_c4)
    end
    local function __cleanup_LAB_00dd0b1f()
        scratchValue10 = 0
        quest:PauseAllNonScriptedEntities((scratchValue10 ~= 0))
        resources:DestroyMovie(scratchValue3)
        resources:DestroyActorMap(scratchValue13)
        resources:ReleaseResource(scratchValue4)
        resources:ReleaseResource(aCStack_c4)
    end
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame()
        scratchValue5 = quest:IsActiveThreadTerminating()
        if scratchValue5 then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    scratchValue5 = not alive
    if not scratchValue5 then
        quest:DisplayQuestInfo(true)
        scratchValue7 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        scratchValue8 = scratchValue7
        alive = not quest:IsActiveThreadTerminating()
        scratchValue5 = not alive
        if not scratchValue5 then
            repeat
                quest:UpdateQuestInfoCounter(scratchValue7, 3 - quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
                    scratchValue10 = quest:GetThingWithScriptName("GuardTeamSpawn")
                    scratchValue5 = false
                    scratchValue11 = "OrchardFarmWhisper"
                    pPosition = scratchValue10:GetPos()
                    scratchValue12 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, scratchValue11)
                    quest:RemoveQuestInfoElement(scratchValue7)
                    quest:MiniMapAddMarker(scratchValue12, "HUD_ORB_RED_SMALL")
                    quest:EntityAttachToScript(scratchValue12, "Q_OrchardFarmRaid")
                    scratchValue13 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    whisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    scratchValue4 = resources:NewResource()
                    scratchValue3 = resources:NewResource()
                    resources:TryAcquire(scratchValue3, scratchValue12, 4)
                    scratchValue9 = 4
                    -- TODO(native): pppuVar9 = appuStack_bc;
                    scratchValue10 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue10, scratchValue9)
                    scratchValue = resources:NewActorMap()
                    resources:SetActor(scratchValue, "HERO", scratchValue4)
                    resources:SetActor(scratchValue, "WHISPER", scratchValue3)
                    scratchValue2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    scratchValue10 = quest:GetHero()
                    scratchValue8 = (quest:GetDistanceBetweenThings(scratchValue, scratchValue10) ^ 2)
                    scratchValue10 = quest:GetHero()
                    scratchValue7 = (quest:GetDistanceBetweenThings(scratchValue2, scratchValue10) ^ 2)
                    if scratchValue8 <= scratchValue7 then
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue5 = not alive
                        if not scratchValue5 then
                            resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", scratchValue13, false, true)
                            scratchValue11 = "CS_ORCHARD_EVIL_WHISPER_BACK"
                            goto LAB_00dd08eb
                        end
                        __cleanup_LAB_00dd0b1f(); return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue5 = not alive
                    if scratchValue5 then
                        __cleanup_LAB_00dd0b11()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", scratchValue13, false, true)
                    scratchValue11 = "CS_ORCHARD_EVIL_WHISPER_FRONT"
                    ::LAB_00dd08eb::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        scratchValue5 = quest:IsActiveThreadTerminating()
                        if scratchValue5 then __cleanup_LAB_00dd0b11(); return end
                        scratchValue5 = quest:DisplayTutorial(18)
                        if scratchValue5 then
                            alive = not quest:IsActiveThreadTerminating()
                            scratchValue5 = not alive
                            if not scratchValue5 then
                                scratchValue5 = quest:MsgIsTutorialClickedPast()
                                while not scratchValue5 do
                                    alive = quest:NewScriptFrame()
                                    scratchValue5 = quest:IsActiveThreadTerminating()
                                    if scratchValue5 then __cleanup_LAB_00dd0b11(); return end
                                    scratchValue5 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue5 = not alive
                                if not scratchValue5 then goto LAB_00dd0977 end
                            end
                            __cleanup_LAB_00dd0b1f()
                            return
                        end
                        ::LAB_00dd0977::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue2)
                    resources:DestroyActorMap(scratchValue)
                    resources:ReleaseResource(scratchValue3)
                    resources:ReleaseResource(scratchValue4)
                    scratchValue7 = scratchValue8
                end
                scratchValue10 = quest:GetHero()
                scratchValue6 = scratchValue10:MsgIsKilledBy("")
                if scratchValue6 then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
                    orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    scratchValue3 = resources:NewResource()
                    scratchValue9 = 4
                    -- TODO(native): pppuVar9 = appuStack_ac;
                    scratchValue4 = resources:NewResource()
                    scratchValue10 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue10, scratchValue9)
                    resources:TryAcquire(scratchValue4, orchardFarmWhisper, 4)
                    scratchValue13 = resources:NewActorMap()
                    resources:SetActor(scratchValue13, "HERO", scratchValue3)
                    resources:SetActor(scratchValue13, "WHISPER", scratchValue4)
                    scratchValue2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", scratchValue13, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (appuStack_9c);
                    resources:DestroyActorMap(scratchValue13)
                    quest:AddLogbookStoryEntry(85)
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(3480))
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(scratchValue7)
                    scratchValue5 = true
                    scratchValue10 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(scratchValue10, scratchValue5)
                    predicateResult = false
                    scratchValue5 = false
                    scratchValue11 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(scratchValue11, scratchValue5, predicateResult, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue5 = not alive
                    until not (not scratchValue5)
                    resources:ReleaseResource(scratchValue4)
                    resources:ReleaseResource(scratchValue3)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(scratchValue7)
                    predicateResult = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    scratchValue5 = true
                    scratchValue11 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(scratchValue11, scratchValue5, pMessage, predicateResult)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                scratchValue5 = quest:IsActiveThreadTerminating()
                if scratchValue5 then
                    return
                end
            until false
        end
    end
end

function ProcessGameRulesGood(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, predicateResult, scratchValue8, doneIntroduction, scratchValue9
    local scratchValue10, scratchValue11, scratchValue12, sequence1, scratchValue13, scratchValue14
    local scratchValue15, pMessage, pPosition, scratchValue16, scratchValue17, scratchValue18
    local whisper, whisper2, orchardFarmWhisper
    local alive = true
    local function __cleanup_LAB_00dd1728()
        scratchValue14 = 0
        quest:PauseAllNonScriptedEntities((scratchValue14 ~= 0))
        resources:DestroyMovie((whisper + 4))
        resources:DestroyActorMap(ppuStack_9c)
        resources:ReleaseResource(scratchValue4)
        resources:ReleaseResource(CStack_cc)
    end
    local function __cleanup_LAB_00dd1736()
        scratchValue14 = 0
        quest:PauseAllNonScriptedEntities((scratchValue14 ~= 0))
        resources:DestroyMovie((whisper + 4))
        resources:DestroyActorMap(ppuStack_9c)
        resources:ReleaseResource(scratchValue4)
        resources:ReleaseResource(CStack_cc)
    end
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame()
        scratchValue8 = quest:IsActiveThreadTerminating()
        if scratchValue8 then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    scratchValue8 = not alive
    if not scratchValue8 then
        quest:DisplayQuestInfo(true)
        scratchValue12 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        alive = not quest:IsActiveThreadTerminating()
        scratchValue8 = not alive
        if not scratchValue8 then
            repeat
                quest:UpdateQuestInfoCounter(scratchValue12, quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue8 = quest:IsActiveThreadTerminating()
                    if scratchValue8 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 2)
                end
                scratchValue13 = quest:GetHero()
                scratchValue14 = ""
                scratchValue = scratchValue13:MsgIsKilledBy("")
                if scratchValue then
                    scratchValue8 = quest:IsActiveThreadTerminating()
                    if scratchValue8 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if (2 < quest:GetStateInt("BanditWavesSpawned")) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue8 = quest:IsActiveThreadTerminating()
                    if scratchValue8 then
                        return
                    end
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
                    quest:RemoveQuestInfoElement(scratchValue12)
                    scratchValue13 = quest:GetThingWithScriptName("BanditTeamSpawn")
                    scratchValue8 = false
                    pPosition = scratchValue13:GetPos()
                    scratchValue18 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, "OrchardFarmWhisper")
                    quest:EntityAttachToScript(scratchValue18, "Q_OrchardFarmRaid")
                    quest:MiniMapAddMarker(scratchValue13, "HUD_ORB_RED_SMALL")
                    whisper = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    whisper2 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    scratchValue6 = resources:NewResource()
                    scratchValue16 = resources:NewResource()
                    -- TODO(native): ppuStack_80[0] = 0;
                    resources:TryAcquire(scratchValue16, scratchValue18, 4)
                    scratchValue11 = 4
                    scratchValue17 = scratchValue6
                    scratchValue13 = quest:GetHero()
                    resources:TryAcquire(scratchValue17, scratchValue13, scratchValue11)
                    scratchValue4 = resources:NewActorMap()
                    resources:SetActor(scratchValue4, "HERO", scratchValue6)
                    resources:SetActor(scratchValue4, "WHISPER", scratchValue16)
                    scratchValue5 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    scratchValue13 = quest:GetHero()
                    -- TODO(native): aCStack_1c[0] = (CCharString)(quest:GetDistanceBetweenThings(auStack_c4_p4, pCVar5) ^ 2);
                    scratchValue13 = quest:GetHero()
                    scratchValue10 = (quest:GetDistanceBetweenThings(auStack_b0 + 4, scratchValue13) ^ 2)
                    if aCStack_1c[0] <= scratchValue10 then
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue8 = not alive
                        if not scratchValue8 then
                            resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", ppuStack_9c, false, true)
                            scratchValue15 = "CS_ORCHARD_GOOD_WHISPER_BACK"
                            goto LAB_00dd14ee
                        end
                        __cleanup_LAB_00dd1736(); return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue8 = not alive
                    if scratchValue8 then
                        __cleanup_LAB_00dd1728()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", ppuStack_9c, false, true)
                    scratchValue15 = "CS_ORCHARD_GOOD_WHISPER_FRONT"
                    ::LAB_00dd14ee::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        scratchValue8 = quest:IsActiveThreadTerminating()
                        if scratchValue8 then __cleanup_LAB_00dd1728(); return end
                        scratchValue8 = quest:DisplayTutorial(18)
                        if scratchValue8 then
                            alive = not quest:IsActiveThreadTerminating()
                            scratchValue8 = not alive
                            if not scratchValue8 then
                                scratchValue8 = quest:MsgIsTutorialClickedPast()
                                while not scratchValue8 do
                                    alive = quest:NewScriptFrame()
                                    scratchValue8 = quest:IsActiveThreadTerminating()
                                    if scratchValue8 then __cleanup_LAB_00dd1728(); return end
                                    scratchValue8 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue8 = not alive
                                if not scratchValue8 then goto LAB_00dd157a end
                            end
                            __cleanup_LAB_00dd1736()
                            return
                        end
                        ::LAB_00dd157a::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue5)
                    resources:DestroyActorMap(ppuStack_9c)
                    resources:ReleaseResource(scratchValue16)
                    resources:ReleaseResource(scratchValue6)
                    scratchValue12 = i_stk_14
                end
                sequence1 = false
                if quest:GetStateInt("CrateCount") == 0 then
                    sequence1 = true
                else
                    sequence1 = false
                end
                if sequence1 then
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue8 = not alive
                    if scratchValue8 then
                        sequence1 = true
                    else
                        sequence1 = false
                    end
                end
                if sequence1 then
                    return
                end
                scratchValue14 = quest:GetHero()
                scratchValue9 = scratchValue14:MsgIsKilledBy("")
                if scratchValue9 then
                    scratchValue8 = quest:IsActiveThreadTerminating()
                    if scratchValue8 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    scratchValue8 = quest:IsActiveThreadTerminating()
                    if scratchValue8 then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
                    orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    scratchValue7 = resources:NewResource()
                    scratchValue3 = resources:NewResource()
                    scratchValue17 = scratchValue7
                    scratchValue14 = quest:GetHero()
                    resources:TryAcquire(scratchValue17, scratchValue14, 4)
                    resources:TryAcquire(scratchValue3, orchardFarmWhisper, 4)
                    scratchValue5 = resources:NewActorMap()
                    resources:SetActor(scratchValue5, "HERO", scratchValue7)
                    resources:SetActor(scratchValue5, "WHISPER", scratchValue3)
                    scratchValue2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_GOOD_OUTRO", scratchValue5, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue2)
                    resources:DestroyActorMap(scratchValue5)
                    quest:AddLogbookStoryEntry(80)
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(3484))
                    if quest:GetMasterGameState("OFBRCratesStolen") == 0 then
                        scratchValue8 = quest:IsActiveThreadTerminating()
                        if scratchValue8 then goto LAB_00dd1a93 end
                        quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                        alive = quest:NewScriptFrame()
                        scratchValue8 = quest:IsActiveThreadTerminating()
                        if scratchValue8 then goto LAB_00dd1a93 end
                    end
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
                    quest:RemoveQuestInfoElement(4)
                    scratchValue8 = true
                    scratchValue14 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(scratchValue14, scratchValue8)
                    predicateResult = false
                    scratchValue8 = false
                    scratchValue15 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(scratchValue15, scratchValue8, predicateResult, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue8 = not alive
                    until not (not scratchValue8)
                    ::LAB_00dd1a93::
                    resources:ReleaseResource(scratchValue3)
                    resources:ReleaseResource(scratchValue7)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    scratchValue8 = quest:IsActiveThreadTerminating()
                    if scratchValue8 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(scratchValue11)
                    predicateResult = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    scratchValue8 = true
                    scratchValue15 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(scratchValue15, scratchValue8, pMessage, predicateResult)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                scratchValue8 = quest:IsActiveThreadTerminating()
                if scratchValue8 then
                    return
                end
            until false
        end
    end
end

function DoCutsceneIfRequired(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, predicateResult, predicateResult2, isRegionLoaded
    local isDistanceBetweenThingsUnder, instructionDismissed, isRegionLoaded2, isRegionLoaded3
    local scratchValue9, banditTeamSpawn, guardTeamSpawn, hero, hero2, mainGates, mainGates2, hero3
    local ofFarmhouseDoor, mainGates3, mainGates4, scratchValue10, scratchValue11
    local getAllThingsWithScriptName, getNearestWithScriptName
    local alive = true
    local scratchValue12 = nil
    if quest:GetStateInt("HeroTeam") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
        if predicateResult then
            -- LAB_00dd03b8: (native jump target)
            return
        end
        banditTeamSpawn = quest:GetThingWithScriptName("BanditTeamSpawn")
        scratchValue12 = banditTeamSpawn
    else
        if quest:IsActiveThreadTerminating() then
            return
        end
        guardTeamSpawn = quest:GetThingWithScriptName("GuardTeamSpawn")
        scratchValue12 = guardTeamSpawn
    end
    -- LAB_00dcfb65: (native jump target)
    alive = not quest:IsActiveThreadTerminating()
    predicateResult2 = not alive
    repeat
        if predicateResult2 then
            scratchValue12 = nil
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
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(scratchValue12, hero, 10.0)
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
                scratchValue6 = resources:NewResource()
                scratchValue3 = resources:NewResource()
                scratchValue2 = resources:NewResource()
                scratchValue = resources:NewResource()
                resources:TryAcquire(scratchValue, getNearestWithScriptName, 4)
                resources:TryAcquire(scratchValue6, 0, 4)
                resources:TryAcquire(scratchValue3, 0 + 1, 4)

                scratchValue10 = scratchValue2
                hero2 = quest:GetHero()
                resources:TryAcquire(scratchValue10, hero2, (4))
                quest:SheatheHeroWeapons()
                scratchValue8 = resources:NewActorMap()
                resources:SetActor(scratchValue8, "HERO", scratchValue2)
                resources:SetActor(scratchValue8, "BAN1", scratchValue6)
                resources:SetActor(scratchValue8, "BAN2", scratchValue3)
                resources:SetActor(scratchValue8, "GUARD", scratchValue)
                scratchValue4 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", scratchValue8, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue4)
                resources:DestroyActorMap(scratchValue8)
                resources:ReleaseResource(scratchValue)
                resources:ReleaseResource(scratchValue2)
                resources:ReleaseResource(scratchValue3)
                resources:ReleaseResource(scratchValue6)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                mainGates = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates)
            else
                if quest:IsActiveThreadTerminating() then
                    return
                end
                mainGates2 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates2)
                scratchValue7 = resources:NewResource()

                scratchValue11 = scratchValue7
                hero3 = quest:GetHero()
                resources:TryAcquire(scratchValue11, hero3, (4))
                scratchValue9 = resources:NewActorMap()
                resources:SetActor(scratchValue9, "HERO", scratchValue7)
                scratchValue5 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", scratchValue9, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue5)
                resources:DestroyActorMap(scratchValue9)
                resources:ReleaseResource(scratchValue7)
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
    local scratchValue2 = quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return true
end

function DoMultiplierCutscene(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, hero, sequence1, sequence2, scratchValue4
    local pScriptObject, scratchValue5
    local alive = true
    local function __region_LAB_00dd1e3d()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue)
    end
    local function __region_LAB_00dd1e53()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue)
    end
    scratchValue3 = resources:NewResource()
    scratchValue4 = quest:GetHero()
    resources:TryAcquire(pScriptObject, scratchValue4, 4)
    scratchValue5 = resources:NewActorMap()
    resources:SetActor(scratchValue5, "HERO", scratchValue3)
    scratchValue = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    scratchValue2 = quest:IsRegionLoaded("GreatwoodLake")
    sequence1 = false
    if not scratchValue2 then
        sequence1 = true
    else
        sequence1 = false
    end
    if not sequence1 then
        scratchValue2 = true
        if quest:GetStateInt("HeroTeam") ~= 0 then
            sequence1 = true
        else
            sequence1 = false
        end
    end
    if sequence1 then
        scratchValue2 = false
    end
    if scratchValue2 then
        alive = not quest:IsActiveThreadTerminating()
        scratchValue2 = not alive
        if not scratchValue2 then
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        scratchValue2 = not alive
        if not scratchValue2 then
            -- TODO(native): local_3c = (CScriptThing *)piVar3;
            hero = quest:GetHero()
            scratchValue4 = quest:GetThingWithScriptName("MK_OFI_GWL")
            scratchValue2 = quest:IsDistanceBetweenThingsUnder(scratchValue4, hero, 20.0)
            if scratchValue2 then
                alive = not quest:IsActiveThreadTerminating()
                scratchValue2 = not alive
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d15
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue2 = not alive
                    if not scratchValue2 then
                        -- TODO(native): goto LAB_00dd1d98
                    end
                    __region_LAB_00dd1e3d()
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                scratchValue2 = not alive
                if scratchValue2 then
                    __region_LAB_00dd1e53()
                    goto LAB_00dd1e95
                end
                -- LAB_00dd1d98: (native jump target)
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", scratchValue5, false, true)
                quest:FixMovieSequenceCamera(false)
                sequence2 = false
                if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                    sequence2 = true
                else
                    sequence2 = false
                end
                if not sequence2 then
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue2 = not alive
                    if scratchValue2 then
                        sequence2 = true
                    else
                        sequence2 = false
                    end
                end
                if sequence2 then goto LAB_00dd1e70 end
                scratchValue2 = quest:DisplayTutorial(9)
                if scratchValue2 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then __region_LAB_00dd1e53(); goto LAB_00dd1e95 end
                    scratchValue2 = quest:MsgIsTutorialClickedPast()
                    while not scratchValue2 do
                        alive = quest:NewScriptFrame()
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then __region_LAB_00dd1e53(); goto LAB_00dd1e95 end
                        scratchValue2 = quest:MsgIsTutorialClickedPast()
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then __region_LAB_00dd1e3d(); goto LAB_00dd1e95 end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                scratchValue2 = not alive
                if not scratchValue2 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then __region_LAB_00dd1e3d(); goto LAB_00dd1e95 end
                    else
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then __region_LAB_00dd1e53(); goto LAB_00dd1e95 end
                    end
                    -- TODO(native): goto LAB_00dd1d98
                end
            end
        end
        ::LAB_00dd1e70::
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00dd1d15::
    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (appuStack_20);
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue5)
    resources:ReleaseResource(scratchValue3)
    return
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
    return
end

