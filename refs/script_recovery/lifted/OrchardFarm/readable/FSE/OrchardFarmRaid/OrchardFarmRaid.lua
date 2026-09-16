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
    -- TODO(native): CCharString__AssignFromWide(this + 0x68,0x12db0c8);
    -- TODO(native): CCharString__AssignFromWide(this + 0x6c,0x12db088);
    -- TODO(native): CCharString__AssignFromWide(this + 0x70,0x12db040);
    -- TODO(native): CCharString__AssignFromWide(this + 0x74,0x12db000);
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
    local scratchValue, scratchValue2, scratchValue3, predicateResult, scratchValue4
    local doneIntroduction, scratchValue5, scratchValue6, scratchValue7, scratchValue8
    local scratchValue9, scratchValue10, pMessage, pPosition, scratchValue11, scratchValue12
    local whisper, orchardFarmWhisper
    local alive = true
    local function __cleanup_LAB_00dd0b11()
        scratchValue9 = 0
        quest:PauseAllNonScriptedEntities((scratchValue9 ~= 0))
        resources:ReleaseResource(scratchValue2)
        resources:DestroyActorMap(scratchValue12)
        resources:ReleaseResource(scratchValue3)
        resources:ReleaseResource(aC_stk_c4)
    end
    local function __cleanup_LAB_00dd0b2b()
        resources:ReleaseResource(scratchValue2)
        resources:DestroyActorMap(scratchValue12)
        resources:ReleaseResource(scratchValue3)
        resources:ReleaseResource(aC_stk_c4)
    end
    doneIntroduction = quest:GetStateBool("DoneIntroduction")
    while not doneIntroduction do
        alive = quest:NewScriptFrame()
        scratchValue4 = quest:IsActiveThreadTerminating()
        if scratchValue4 then
            return
        end
        doneIntroduction = quest:GetStateBool("DoneIntroduction")
    end
    alive = not quest:IsActiveThreadTerminating()
    scratchValue4 = not alive
    if not scratchValue4 then
        quest:DisplayQuestInfo(true)
        scratchValue6 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        scratchValue7 = scratchValue6
        alive = not quest:IsActiveThreadTerminating()
        scratchValue4 = not alive
        if not scratchValue4 then
            repeat
                quest:UpdateQuestInfoCounter(scratchValue6, 3 - quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if scratchValue4 then
                        return
                    end
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
                    scratchValue9 = quest:GetThingWithScriptName("GuardTeamSpawn")
                    scratchValue4 = false
                    scratchValue10 = "OrchardFarmWhisper"
                    pPosition = scratchValue9:GetPos()
                    scratchValue11 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, scratchValue10)
                    quest:RemoveQuestInfoElement(scratchValue6)
                    quest:MiniMapAddMarker(scratchValue11, "HUD_ORB_RED_SMALL")
                    quest:EntityAttachToScript(scratchValue11, "Q_OrchardFarmRaid")
                    scratchValue12 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    whisper = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    scratchValue3 = resources:NewResource()
                    scratchValue2 = resources:NewResource()
                    resources:TryAcquire(scratchValue2, scratchValue11, 4)
                    scratchValue8 = 4
                    -- TODO(native): pppuVar9 = appuStack_bc;
                    scratchValue9 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue9, scratchValue8)
                    scratchValue12 = resources:NewActorMap()
                    resources:SetActor(scratchValue12, "HERO", scratchValue3)
                    resources:SetActor(scratchValue12, "WHISPER", scratchValue2)
                    scratchValue = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    scratchValue9 = quest:GetHero()
                    scratchValue7 = (quest:GetDistanceBetweenThings(whisper, scratchValue9) ^ 2)
                    scratchValue9 = quest:GetHero()
                    scratchValue6 = (quest:GetDistanceBetweenThings(scratchValue, scratchValue9) ^ 2)
                    if scratchValue7 <= scratchValue6 then
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue4 = not alive
                        if not scratchValue4 then
                            resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", scratchValue12, false, true)
                            scratchValue10 = "CS_ORCHARD_EVIL_WHISPER_BACK"
                            goto LAB_00dd08eb
                        end
                        -- TODO(native): goto LAB_00dd0b1f
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue4 = not alive
                    if scratchValue4 then
                        __cleanup_LAB_00dd0b11()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", scratchValue12, false, true)
                    scratchValue10 = "CS_ORCHARD_EVIL_WHISPER_FRONT"
                    ::LAB_00dd08eb::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        scratchValue4 = quest:IsActiveThreadTerminating()
                        if scratchValue4 then __cleanup_LAB_00dd0b11(); return end
                        scratchValue4 = quest:DisplayTutorial(18)
                        if scratchValue4 then
                            alive = not quest:IsActiveThreadTerminating()
                            scratchValue4 = not alive
                            if not scratchValue4 then
                                scratchValue4 = quest:MsgIsTutorialClickedPast()
                                while not scratchValue4 do
                                    alive = quest:NewScriptFrame()
                                    scratchValue4 = quest:IsActiveThreadTerminating()
                                    if scratchValue4 then __cleanup_LAB_00dd0b11(); return end
                                    scratchValue4 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue4 = not alive
                                if not scratchValue4 then goto LAB_00dd0977 end
                            end
                            -- LAB_00dd0b1f: (native jump target)
                            scratchValue9 = 0
                            quest:PauseAllNonScriptedEntities((scratchValue9 ~= 0))
                            __cleanup_LAB_00dd0b2b(); return
                        end
                        ::LAB_00dd0977::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue)
                    resources:DestroyActorMap(scratchValue12)
                    resources:ReleaseResource(scratchValue2)
                    resources:ReleaseResource(scratchValue3)
                    scratchValue6 = scratchValue7
                end
                scratchValue9 = quest:GetHero()
                scratchValue5 = scratchValue9:MsgIsKilledBy("")
                if scratchValue5 then
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if scratchValue4 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if scratchValue4 then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(3432))
                    orchardFarmWhisper = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    scratchValue2 = resources:NewResource()
                    scratchValue8 = 4
                    -- TODO(native): pppuVar9 = appuStack_ac;
                    scratchValue3 = resources:NewResource()
                    scratchValue9 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, scratchValue9, scratchValue8)
                    resources:TryAcquire(scratchValue3, orchardFarmWhisper, 4)
                    scratchValue12 = resources:NewActorMap()
                    resources:SetActor(scratchValue12, "HERO", scratchValue2)
                    resources:SetActor(scratchValue12, "WHISPER", scratchValue3)
                    scratchValue = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", scratchValue12, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (appuStack_9c);
                    resources:DestroyActorMap(scratchValue12)
                    -- TODO(native): CSubtitleRenderer::SetText(0x55);
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(3480))
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(scratchValue6)
                    scratchValue4 = true
                    scratchValue9 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(scratchValue9, scratchValue4)
                    predicateResult = false
                    scratchValue4 = false
                    scratchValue10 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(scratchValue10, scratchValue4, predicateResult, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue4 = not alive
                    until not (not scratchValue4)
                    resources:ReleaseResource(scratchValue3)
                    resources:ReleaseResource(scratchValue2)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if scratchValue4 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(scratchValue6)
                    predicateResult = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    scratchValue4 = true
                    scratchValue10 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(scratchValue10, scratchValue4, pMessage, predicateResult)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                end
                alive = quest:NewScriptFrame()
                scratchValue4 = quest:IsActiveThreadTerminating()
                if scratchValue4 then
                    return
                end
            until false
        end
    end
end

function ProcessGameRulesGood(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, predicateResult, scratchValue5
    local doneIntroduction, scratchValue6, scratchValue7, scratchValue8, scratchValue9, sequence1
    local scratchValue10, scratchValue11, scratchValue12, pMessage, pOther_00, pPosition
    local scratchValue13, scratchValue14, scratchValue15, scratchValue16, whisper, whisper2
    local orchardFarmWhisper
    local alive = true
    local function __cleanup_LAB_00dd1728()
        scratchValue11 = 0
        quest:PauseAllNonScriptedEntities((scratchValue11 ~= 0))
        resources:ReleaseResource((whisper + 4))
        resources:DestroyActorMap(scratchValue14)
        resources:ReleaseResource(scratchValue2)
        resources:ReleaseResource(CStack_cc)
    end
    local function __cleanup_LAB_00dd1742()
        resources:ReleaseResource((whisper + 4))
        resources:DestroyActorMap(scratchValue14)
        resources:ReleaseResource(scratchValue2)
        resources:ReleaseResource(CStack_cc)
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
        scratchValue9 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        scratchValue4 = scratchValue9
        alive = not quest:IsActiveThreadTerminating()
        scratchValue5 = not alive
        if not scratchValue5 then
            repeat
                quest:UpdateQuestInfoCounter(scratchValue9, quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 2)
                end
                scratchValue10 = quest:GetHero()
                scratchValue11 = ""
                -- TODO(native): CStack_cc._3_1_ = CScriptThing::MsgIsKilledBy(pCVar5);
                if CStack_cc._3_1_ ~= 0 then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if (2 < quest:GetStateInt("BanditWavesSpawned")) and (not quest:GetStateBool("WhisperSpawned")) then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
                    quest:RemoveQuestInfoElement(scratchValue9)
                    scratchValue10 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM")
                    scratchValue5 = false
                    pPosition = scratchValue10:GetPos()
                    scratchValue16 = quest:CreateCreature("OrchardFarmWhisper", pPosition, "BanditTeamSpawn")
                    quest:EntityAttachToScript(scratchValue16, "Q_OrchardFarmRaid")
                    quest:MiniMapAddMarker(scratchValue10, "HUD_ORB_RED_SMALL")
                    whisper = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    whisper2 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    scratchValue3 = resources:NewResource()
                    scratchValue2 = resources:NewResource()
                    -- TODO(native): ppuStack_80[0] = 0;
                    resources:TryAcquire(scratchValue2, scratchValue16, 4)
                    scratchValue8 = 4
                    scratchValue15 = scratchValue3
                    scratchValue10 = quest:GetHero()
                    resources:TryAcquire(scratchValue15, scratchValue10, scratchValue8)
                    scratchValue = resources:NewActorMap()
                    resources:SetActor(scratchValue, "HERO", scratchValue3)
                    resources:SetActor(scratchValue, "WHISPER", scratchValue2)
                    scratchValue14 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    scratchValue10 = quest:GetHero()
                    -- TODO(native): aCStack_1c[0] = (CCharString)(quest:GetDistanceBetweenThings(auStack_c4, pCVar5) ^ 2);
                    scratchValue10 = quest:GetHero()
                    scratchValue7 = (quest:GetDistanceBetweenThings(auStack_b0 + 4, scratchValue10) ^ 2)
                    if aCStack_1c[0] <= scratchValue7 then
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue5 = not alive
                        if not scratchValue5 then
                            resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", scratchValue14, false, true)
                            scratchValue12 = "CS_ORCHARD_GOOD_WHISPER_BACK"
                            goto LAB_00dd14ee
                        end
                        -- TODO(native): goto LAB_00dd1736
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue5 = not alive
                    if scratchValue5 then
                        __cleanup_LAB_00dd1728()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", scratchValue14, false, true)
                    scratchValue12 = "CS_ORCHARD_GOOD_WHISPER_FRONT"
                    ::LAB_00dd14ee::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        scratchValue5 = quest:IsActiveThreadTerminating()
                        if scratchValue5 then __cleanup_LAB_00dd1728(); return end
                        scratchValue5 = quest:DisplayTutorial(18)
                        if scratchValue5 then
                            alive = not quest:IsActiveThreadTerminating()
                            scratchValue5 = not alive
                            if not scratchValue5 then
                                scratchValue5 = quest:MsgIsTutorialClickedPast()
                                while not scratchValue5 do
                                    alive = quest:NewScriptFrame()
                                    scratchValue5 = quest:IsActiveThreadTerminating()
                                    if scratchValue5 then __cleanup_LAB_00dd1728(); return end
                                    scratchValue5 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                scratchValue5 = not alive
                                if not scratchValue5 then goto LAB_00dd157a end
                            end
                            -- LAB_00dd1736: (native jump target)
                            scratchValue11 = 0
                            quest:PauseAllNonScriptedEntities((scratchValue11 ~= 0))
                            __cleanup_LAB_00dd1742(); return
                        end
                        ::LAB_00dd157a::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue14)
                    resources:DestroyActorMap(scratchValue)
                    resources:ReleaseResource(scratchValue2)
                    resources:ReleaseResource(scratchValue3)
                    scratchValue9 = i_stk_14
                end
                sequence1 = false
                if quest:GetStateInt("CrateCount") == 0 then
                    sequence1 = true
                else
                    sequence1 = false
                end
                if sequence1 then
                    alive = not quest:IsActiveThreadTerminating()
                    scratchValue5 = not alive
                    if scratchValue5 then
                        sequence1 = true
                    else
                        sequence1 = false
                    end
                end
                if sequence1 then
                    return
                end
                scratchValue11 = quest:GetHero()
                scratchValue6 = scratchValue11:MsgIsKilledBy("")
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
                    scratchValue3 = resources:StartMovie("")
                    scratchValue14 = resources:StartMovie("")
                    scratchValue15 = scratchValue3 + 4
                    scratchValue11 = quest:GetHero()
                    resources:TryAcquire(scratchValue15, scratchValue11, 4)
                    resources:TryAcquire(scratchValue14, orchardFarmWhisper, 4)
                    -- TODO(native): StdMap_Construct_API(auStack_a8 + 4);
                    scratchValue15 = scratchValue3 + 4
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(auStack_a8 + 4),&CStack_cc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,puVar10);
                    pOther_00 = scratchValue14
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(auStack_a8 + 4),&CStack_cc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pOther_00);
                    scratchValue13 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): RunCutsceneMacro_Func(&CStack_cc,auStack_a8 + 4,(void *)0x0,(void *)0x0,false,true);
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue13)
                    -- TODO(native): StdMap_Destroy_API(auStack_a8 + 4);
                    -- TODO(native): CSubtitleRenderer::SetText(0x50);
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(3484))
                    if quest:GetMasterGameState("OFBRCratesStolen") == 0 then
                        scratchValue5 = quest:IsActiveThreadTerminating()
                        if scratchValue5 then goto LAB_00dd1a93 end
                        quest:SetMasterGameState("OFBR_NoCratesWereStolen", true)
                        alive = quest:NewScriptFrame()
                        scratchValue5 = quest:IsActiveThreadTerminating()
                        if scratchValue5 then goto LAB_00dd1a93 end
                    end
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 2)
                    quest:RemoveQuestInfoElement(4)
                    scratchValue5 = true
                    scratchValue11 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(scratchValue11, scratchValue5)
                    predicateResult = false
                    scratchValue5 = false
                    scratchValue12 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(scratchValue12, scratchValue5, predicateResult, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        scratchValue5 = not alive
                    until not (not scratchValue5)
                    ::LAB_00dd1a93::
                    resources:ReleaseResource(scratchValue14)
                    resources:ReleaseResource(scratchValue3)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    scratchValue5 = quest:IsActiveThreadTerminating()
                    if scratchValue5 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(scratchValue8)
                    predicateResult = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    scratchValue5 = true
                    scratchValue12 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(scratchValue12, scratchValue5, pMessage, predicateResult)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
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

function DoCutsceneIfRequired(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, aC_stk_40_1, aC_stk_40_2, am_stk_5c_1
    local am_stk_5c_2, am_stk_5c_3, predicateResult, isRegionLoaded, isDistanceBetweenThingsUnder
    local instructionDismissed, isRegionLoaded2, isRegionLoaded3, scratchValue4, banditTeamSpawn
    local guardTeamSpawn, hero, hero2, mainGates, mainGates2, hero3, ofFarmhouseDoor, mainGates3
    local mainGates4, scratchValue5, scratchValue6, getAllThingsWithScriptName
    local getNearestWithScriptName
    local alive = true
    local scratchValue7 = nil
    if quest:GetStateInt("HeroTeam") == 1 then
        if quest:IsActiveThreadTerminating() then
            return
        end
        banditTeamSpawn = quest:GetThingWithScriptName("BanditTeamSpawn")
        scratchValue7 = banditTeamSpawn
    else
        if quest:IsActiveThreadTerminating() then return end
        guardTeamSpawn = quest:GetThingWithScriptName("GuardTeamSpawn")
        scratchValue7 = guardTeamSpawn
    end
    -- LAB_00dcfb65: (native jump target)
    alive = not quest:IsActiveThreadTerminating()
    predicateResult = not alive
    repeat
        if predicateResult then
            return
        end
        isRegionLoaded = quest:IsRegionLoaded("OrchardFarm")
        while not isRegionLoaded do
            alive = quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
            isRegionLoaded = quest:IsRegionLoaded("OrchardFarm")
        end
        if quest:IsActiveThreadTerminating() then return end
        hero = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(scratchValue7, hero, 10.0)
        if isDistanceBetweenThingsUnder then
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetStateInt("HeroTeam") == 1 then
                if quest:IsActiveThreadTerminating() then return end
                getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("BanditTeamMember")
                getNearestWithScriptName = quest:GetNearestWithScriptName(hero, "GuardTeamMember")
                am_stk_5c_1 = resources:StartMovie("")
                scratchValue3 = resources:StartMovie("")
                scratchValue2 = resources:StartMovie("")
                scratchValue = resources:StartMovie("")
                resources:TryAcquire(scratchValue, getNearestWithScriptName, 4)
                resources:TryAcquire(am_stk_5c_1, 0, 4)
                resources:TryAcquire(scratchValue3, 0 + 1, 4)

                scratchValue5 = scratchValue2
                hero2 = quest:GetHero()
                resources:TryAcquire(scratchValue5, hero2, (4))
                quest:SheatheHeroWeapons()
                am_stk_5c_2 = resources:NewActorMap()
                resources:SetActor(am_stk_5c_2, "HERO", scratchValue2)
                resources:SetActor(am_stk_5c_2, "BAN1", am_stk_5c_2)
                resources:SetActor(am_stk_5c_2, "BAN2", scratchValue3)
                resources:SetActor(am_stk_5c_2, "GUARD", scratchValue)
                aC_stk_40_1 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", am_stk_5c_2, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:ReleaseResource(aC_stk_40_1)
                resources:DestroyActorMap(am_stk_5c_2)
                resources:ReleaseResource(scratchValue)
                resources:ReleaseResource(scratchValue2)
                resources:ReleaseResource(scratchValue3)
                resources:ReleaseResource(am_stk_5c_2)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                mainGates = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates)
            else
                if quest:IsActiveThreadTerminating() then return end
                mainGates2 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(mainGates2)
                am_stk_5c_3 = resources:StartMovie("")

                scratchValue6 = am_stk_5c_3
                hero3 = quest:GetHero()
                resources:TryAcquire(scratchValue6, hero3, (4))
                scratchValue4 = resources:NewActorMap()
                resources:SetActor(scratchValue4, "HERO", am_stk_5c_3)
                aC_stk_40_2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", scratchValue4, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:ReleaseResource(aC_stk_40_2)
                resources:DestroyActorMap(scratchValue4)
                resources:ReleaseResource(am_stk_5c_3)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", true, false)
            end
            quest:DeactivateQuest("Q_OrchardFarm_Barricade", 0)
            quest:Pause(1.0)
            quest:SetStateBool("DoneIntroduction", true)

            ofFarmhouseDoor = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(ofFarmhouseDoor, (false))
            return
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("HeroAtWrongEntrance", true)
        mainGates3 = quest:GetThingWithScriptName("OF_MainGates")
        -- TODO(native): CloseDoor is not a ForgeFSE binding
        quest:CloseDoor(mainGates3)

        mainGates4 = quest:GetThingWithScriptName("OF_MainGates")
        quest:SetThingAsUsable(mainGates4, (false))
        quest:Pause(1.0)
        quest:DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE")
        instructionDismissed = quest:MsgIsGameInfoClickedPast()
        while not instructionDismissed do
            alive = quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
            instructionDismissed = quest:MsgIsGameInfoClickedPast()
        end
        if quest:IsActiveThreadTerminating() then return end
        isRegionLoaded2 = quest:IsRegionLoaded("OrchardFarm")
        if isRegionLoaded2 then
            repeat
                alive = quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return end
                isRegionLoaded3 = quest:IsRegionLoaded("OrchardFarm")
            until not (isRegionLoaded3)
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("HeroAtWrongEntrance", false)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
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
            if isQuestActive2 then return end  -- TODO(native): goto LAB_00dccfa3
        end
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
    local scratchValue, scratchValue2, scratchValue3, sequence1, sequence2, scratchValue4
    local pScriptObject, scratchValue5, scratchValue6
    local alive = true
    local function __region_LAB_00dd1e3d()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(scratchValue)
    end
    local function __region_LAB_00dd1e53()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(scratchValue)
    end
    scratchValue6 = 0
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
            scratchValue6 = piVar3
            scratchValue6 = quest:GetHero()
            scratchValue4 = quest:GetThingWithScriptName("MK_OFI_GWL")
            scratchValue2 = quest:IsDistanceBetweenThingsUnder(scratchValue4, scratchValue6, 20.0)
            if scratchValue2 then
                alive = not quest:IsActiveThreadTerminating()
                scratchValue2 = not alive
                if scratchValue2 then return end  -- TODO(native): goto LAB_00dd1d15
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
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", scratchValue4, false, true)
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
    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (appuStack_20);
    ::LAB_00dd1e95::
    resources:DestroyActorMap(scratchValue4)
    resources:ReleaseResource(scratchValue3)
    return
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
    return
end

