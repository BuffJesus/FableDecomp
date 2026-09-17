-- Generated native draft: Q_OrchardFarmRaid. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local pCVar5
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
    quest:SetStateInt("CommentTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("RemindHeroOfObjectivesTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_0_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("Teams_1_TeamReinforcementsTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    local x_stk_c
    local bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
    if bVar2 then
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
    x_stk_c = nil
    quest:SetStateThing("Teams_0_TeamCrateCarrier", nil)
    x_stk_c = nil
    x_stk_c = 0
    x_stk_c = nil
    quest:SetStateThing("Teams_1_TeamCrateCarrier", nil)
    x_stk_c = nil
    x_stk_c = 0
    quest:SetStateString("FailReasons_0", "PROBLEM: Tell Ben problem with Orchard Farm fail reasons")
    quest:SetStateString("FailReasons_1", "TEXT_QST_051_FAILED_HERO_KILLED")
    quest:SetStateString("FailReasons_2", "TEXT_QST_051_FAILED_CRATES_STOLEN")
    quest:SetStateString("FailReasons_3", "TEXT_QST_051_FAILED_TEAM_KILLED")
    bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
    if bVar2 then
        quest:SetStateInt("HeroTeam", 1)
        quest:SetStateString("TextSystemScriptCode", "TEXT_QST_051_")
        quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "", "HeroGuildComplexInside")
        quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false)
    else
        bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
        if not bVar2 then
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
    local b3, bVar10, bVar4, cVar1, c_stk_c1, fVar5, f_stk_14, iVar11, pCVar6, pCVar8, pMessage, pPosition, pppuVar9, r1, r2, r3, r4, xStack_80, xStack_9c, xStack_ac, xStack_bc
    local alive = true
    local function __cleanup_LAB_00dd0b11()
        pCVar6 = 0x0
        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
        resources:DestroyMovie(xStack_9c)
        resources:DestroyActorMap(r2)
        resources:ReleaseResource(xStack_ac)
        resources:ReleaseResource(xStack_bc)
    end
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
        fVar5 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_SMALL_CRATE", 3, 1.0)
        f_stk_14 = fVar5
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            repeat
                quest:UpdateQuestInfoCounter(fVar5, 3 - quest:GetStateInt("CrateCount"), -1)
                if (quest:GetStateInt("CrateCount") == 0) and (not quest:GetStateBool("WhisperSpawned")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_02", "", "Greatwood")
                    pCVar6 = quest:GetThingWithScriptName("GuardTeamSpawn")
                    bVar4 = false
                    pCVar8 = "OrchardFarmWhisper"
                    pPosition = pCVar6:GetPos()
                    r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, pCVar8)
                    quest:RemoveQuestInfoElement(fVar5)
                    quest:MiniMapAddMarker(r1, "HUD_ORB_RED_SMALL")
                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                    r2 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    r3 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    xStack_bc = resources:NewResource()
                    xStack_ac = resources:NewResource()
                    resources:TryAcquire(xStack_ac, r1, 4)
                    iVar11 = 4
                    pppuVar9 = xStack_bc
                    pCVar6 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, pCVar6, iVar11)
                    xStack_80 = resources:NewActorMap()
                    resources:SetActor(xStack_80, "HERO", xStack_bc)
                    resources:SetActor(xStack_80, "WHISPER", xStack_ac)
                    xStack_9c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    pCVar6 = quest:GetHero()
                    f_stk_14 = (quest:GetDistanceBetweenThings(r3, pCVar6) ^ 2)
                    pCVar6 = quest:GetHero()
                    fVar5 = (quest:GetDistanceBetweenThings(r2, pCVar6) ^ 2)
                    if f_stk_14 <= fVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", r2, false, true)
                            goto LAB_00dd08eb
                        end
                        __cleanup_LAB_00dd0b11(); return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        __cleanup_LAB_00dd0b11()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", r2, false, true)
                    pCVar8 = (r1 + 8)
                    ::LAB_00dd08eb::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __cleanup_LAB_00dd0b11(); return end
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
                                    if bVar4 then __cleanup_LAB_00dd0b11(); return end
                                    bVar4 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then goto LAB_00dd0977 end
                            end
                            -- LAB_00dd0b1f: (native jump target)
                            pCVar6 = 0x0
                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                            resources:DestroyMovie(xStack_9c)
                            resources:DestroyActorMap(r2)
                            resources:ReleaseResource(xStack_ac)
                            resources:ReleaseResource(xStack_bc)
                            return
                        end
                        ::LAB_00dd0977::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_9c)
                    resources:DestroyActorMap(xStack_80)
                    resources:ReleaseResource(xStack_ac)
                    resources:ReleaseResource(xStack_bc)
                    fVar5 = f_stk_14
                end
                pCVar6 = quest:GetHero()
                c_stk_c1 = pCVar6:MsgIsKilledBy("")
                if c_stk_c1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(0xd68))
                    r4 = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    xStack_ac = resources:NewResource()
                    iVar11 = 4
                    pppuVar9 = xStack_ac
                    xStack_bc = resources:NewResource()
                    pCVar6 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, pCVar6, iVar11)
                    resources:TryAcquire(xStack_bc, r4, 4)
                    r2 = resources:NewActorMap()
                    resources:SetActor(r2, "HERO", xStack_ac)
                    resources:SetActor(r2, "WHISPER", xStack_bc)
                    xStack_9c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", r2, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_9c)
                    resources:DestroyActorMap(r2)
                    quest:AddLogbookStoryEntry(85)
                    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xd98))
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(fVar5)
                    bVar4 = true
                    pCVar6 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar6, bVar4)
                    b3 = false
                    bVar10 = false
                    bVar4 = false
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(pCVar8, bVar4, bVar10, b3)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                    until not (not bVar4)
                    resources:ReleaseResource(xStack_bc)
                    resources:ReleaseResource(xStack_ac)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(fVar5)
                    bVar10 = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    bVar4 = true
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar8, bVar4, pMessage, bVar10)
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
    local b3, bVar10, bVar3, cVar1, c_stk_c5, ePriority, fVar8, f_xStack_18, iVar11, iVar4, i_stk_10, native_arg_sequence_1, pCVar5, pCVar7, pMessage, pPosition, pppuVar9, r1, r2, r3, r4, xStack_7c, xStack_88, xStack_98, xStack_c0
    local alive = true
    local function __cleanup_LAB_00dd1728()
        pCVar5 = 0x0
        quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
        resources:DestroyMovie(xStack_98)
        resources:DestroyActorMap(xStack_88)
        resources:ReleaseResource(xStack_7c)
        resources:ReleaseResource(xStack_c0)
    end
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
        i_stk_10 = iVar4
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
                    quest:SetStateInt("MissionFailed", 2)
                end
                pCVar5 = quest:GetHero()
                c_stk_c5 = pCVar5:MsgIsKilledBy("")
                if c_stk_c5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if (2 < quest:GetStateInt("BanditWavesSpawned")) and (not quest:GetStateBool("WhisperSpawned")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:SetStateBool("WhisperSpawned", true)
                    quest:SetStateBool("WhisperInCutscene", true)
                    quest:SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_02", "", "Greatwood")
                    quest:RemoveQuestInfoElement(iVar4)
                    pCVar5 = quest:GetThingWithScriptName("BanditTeamSpawn")
                    bVar3 = false
                    pPosition = pCVar5:GetPos()
                    r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ORCHARD_FARM", pPosition, "OrchardFarmWhisper")
                    quest:EntityAttachToScript(r1, "Q_OrchardFarmRaid")
                    quest:MiniMapAddMarker(pCVar5, "HUD_ORB_RED_SMALL")
                    r2 = quest:GetThingWithScriptName("MK_OFWB_WHISPER")
                    r3 = quest:GetThingWithScriptName("MK_OFWF_WHISPER")
                    xStack_c0 = resources:NewResource()
                    xStack_7c = resources:NewResource()
                    resources:TryAcquire(xStack_7c, r1, 4)
                    iVar11 = 4
                    pppuVar9 = xStack_c0
                    pCVar5 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, pCVar5, iVar11)
                    xStack_88 = resources:NewActorMap()
                    resources:SetActor(xStack_88, "HERO", xStack_c0)
                    resources:SetActor(xStack_88, "WHISPER", xStack_7c)
                    xStack_98 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    pCVar5 = quest:GetHero()
                    f_xStack_18 = (quest:GetDistanceBetweenThings(r3, pCVar5) ^ 2)
                    pCVar5 = quest:GetHero()
                    fVar8 = (quest:GetDistanceBetweenThings(r2, pCVar5) ^ 2)
                    if f_xStack_18 <= fVar8 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", xStack_88, false, true)
                            goto LAB_00dd14ee
                        end
                        __cleanup_LAB_00dd1728(); return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        __cleanup_LAB_00dd1728()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", xStack_88, false, true)
                    ::LAB_00dd14ee::
                    quest:FixMovieSequenceCamera(false)
                    if not quest:GetStateBool("ShownCombatFlourishTutorial") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00dd1728(); return end
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
                                    if bVar3 then __cleanup_LAB_00dd1728(); return end
                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then goto LAB_00dd157a end
                            end
                            -- LAB_00dd1736: (native jump target)
                            pCVar5 = 0x0
                            quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
                            resources:DestroyMovie(xStack_98)
                            resources:DestroyActorMap(xStack_88)
                            resources:ReleaseResource(xStack_7c)
                            resources:ReleaseResource(xStack_c0)
                            return
                        end
                        ::LAB_00dd157a::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_98)
                    resources:DestroyActorMap(xStack_88)
                    resources:ReleaseResource(xStack_7c)
                    resources:ReleaseResource(xStack_c0)
                    iVar4 = i_stk_10
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
                pCVar5 = quest:GetHero()
                c_stk_c5 = pCVar5:MsgIsKilledBy("")
                if c_stk_c5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:SetStateInt("MissionFailed", 1)
                end
                if quest:GetStateBool("MissionSucceeded") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(0xd68))
                    r4 = quest:GetThingWithScriptName("OrchardFarmWhisper")
                    xStack_c0 = resources:NewResource()
                    xStack_98 = resources:NewResource()
                    ePriority = 4
                    pppuVar9 = xStack_c0
                    pCVar5 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, pCVar5, ePriority)
                    resources:TryAcquire(xStack_98, r4, 4)
                    r2 = resources:NewActorMap()
                    resources:SetActor(r2, "HERO", xStack_c0)
                    resources:SetActor(r2, "WHISPER", xStack_98)
                    xStack_7c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_GOOD_OUTRO", r2, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_7c)
                    resources:DestroyActorMap(r2)
                    quest:AddLogbookStoryEntry(80)
                    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xd9c))
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
                    quest:RemoveQuestInfoElement(iVar4)
                    bVar3 = true
                    pCVar5 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar5, bVar3)
                    b3 = false
                    bVar10 = false
                    bVar3 = false
                    pCVar7 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(pCVar7, bVar3, bVar10, b3)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                    until not (not bVar3)
                    ::LAB_00dd1a93::
                    resources:ReleaseResource(xStack_98)
                    resources:ReleaseResource(xStack_c0)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(ePriority)
                    bVar10 = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    bVar3 = true
                    pCVar7 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar7, bVar3, pMessage, bVar10)
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
    local bVar2, iVar8, pCVar3, pCVar7, r1, xStack_10, xStack_20, xStack_30, xStack_40, xStack_50, xStack_5c, xStack_70
    local alive = true
    local xStack_90 = nil
    if quest:GetStateInt("HeroTeam") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00dd03b8: (native jump target)
            return
        end
        pCVar3 = quest:GetThingWithScriptName("BanditTeamSpawn")
        xStack_90 = pCVar3
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        pCVar3 = quest:GetThingWithScriptName("GuardTeamSpawn")
        xStack_90 = pCVar3
    end
    -- LAB_00dcfb65: (native jump target)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            xStack_90 = nil
            -- LAB_00dcfe47: (native jump target)
            return
        end
        bVar2 = quest:IsRegionLoaded("OrchardFarm")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                xStack_90 = nil
                return
            end
            bVar2 = quest:IsRegionLoaded("OrchardFarm")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(xStack_90, pCVar3, 10.0)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            if quest:GetStateInt("HeroTeam") == 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                xStack_70 = quest:GetAllThingsWithScriptName("BanditTeamMember")
                r1 = quest:GetNearestWithScriptName(xStack_90, "GuardTeamMember")
                xStack_50 = resources:NewResource()
                xStack_30 = resources:NewResource()
                xStack_20 = resources:NewResource()
                xStack_10 = resources:NewResource()
                resources:TryAcquire(xStack_10, r1, 4)
                resources:TryAcquire(xStack_50, xStack_70[0 + 1], 4)
                resources:TryAcquire(xStack_30, xStack_70[1 + 1], 4)
                iVar8 = 4
                pCVar7 = xStack_20
                pCVar3 = quest:GetHero()
                resources:TryAcquire(pCVar7, pCVar3, iVar8)
                quest:SheatheHeroWeapons()
                xStack_5c = resources:NewActorMap()
                resources:SetActor(xStack_5c, "HERO", xStack_20)
                resources:SetActor(xStack_5c, "BAN1", xStack_50)
                resources:SetActor(xStack_5c, "BAN2", xStack_30)
                resources:SetActor(xStack_5c, "GUARD", xStack_10)
                xStack_40 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", xStack_5c, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_40)
                resources:DestroyActorMap(xStack_5c)
                resources:ReleaseResource(xStack_10)
                resources:ReleaseResource(xStack_20)
                resources:ReleaseResource(xStack_30)
                resources:ReleaseResource(xStack_50)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(pCVar3)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(pCVar3)
                xStack_50 = resources:NewResource()
                iVar8 = 4
                pCVar7 = xStack_50
                pCVar3 = quest:GetHero()
                resources:TryAcquire(pCVar7, pCVar3, iVar8)
                xStack_70 = resources:NewActorMap()
                resources:SetActor(xStack_70[0 + 1], "HERO", xStack_50)
                xStack_40 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", xStack_70[0 + 1], false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_40)
                resources:DestroyActorMap(xStack_70[0 + 1])
                resources:ReleaseResource(xStack_50)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidGood", true, false)
            end
            quest:DeactivateQuest("Q_OrchardFarm_Barricade", 0)
            quest:Pause(1.0)
            quest:SetStateBool("DoneIntroduction", true)
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("OFFarmhouseDoor")
            quest:SetThingAsUsable(pCVar3, bVar2)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        quest:SetStateBool("HeroAtWrongEntrance", true)
        pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
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
            if bVar2 then
                return
            end
            bVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsRegionLoaded("OrchardFarm")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsRegionLoaded("OrchardFarm")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
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
            if bVar3 then
                bVar2 = false
                goto FLOW_after_lab_00dccfa3
            end
        end
        ::FLOW_after_lab_00dccfa3::
        if bVar1 then
            bVar1 = false
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
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
    local iVar1 = quest:GetTimer(quest:GetStateInt("CommentTimer"))
    local pSpeaker = native_arg_speaker
    if 0 < iVar1 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    local conversationID = quest:AddNewConversation(native_arg_speaker, false, false)
    local pCVar2 = quest:GetHero()
    quest:AddPersonToConversation(conversationID, pCVar2)
    pCVar2 = quest:GetHero()
    local pCVar3 = pSpeaker:GetDataString()
    pCVar3 = (quest:GetStateString("TextSystemScriptCode") .. pCVar3)
    pCVar3 = (pCVar3 .. "_")
    pCVar3 = (pCVar3 .. native_arg_comment_to_make)
    quest:AddLineToConversation(conversationID, pCVar3, pSpeaker, pCVar2, false)
    local uVar4 = quest:SetTimer(quest:GetStateInt("CommentTimer"), 5)
    return true
end

function DoMultiplierCutscene(quest)
    local resources = quest:RetailResources()
    local bVar4, dist, ePriority, local_3c, native_arg_sequence_1, native_arg_sequence_2, pCVar5, pScriptObject, string, xStack_10, xStack_20, xStack_38
    local alive = true
    ePriority = 4
    xStack_10 = resources:NewResource()
    pCVar5 = quest:GetHero()
    resources:TryAcquire(pScriptObject, pCVar5, ePriority)
    xStack_38 = resources:NewActorMap()
    resources:SetActor(xStack_38, "HERO", xStack_10)
    xStack_20 = resources:StartMovie("")
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
            string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWLL"
            -- LAB_00dd1d98_c1: (native jump target)
            resources:RunMacro(string, xStack_38, false, true)
            quest:FixMovieSequenceCamera(false)
            native_arg_sequence_2 = false
            if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                native_arg_sequence_2 = true
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
            if native_arg_sequence_2 then goto LAB_00dd1e70_c1 end
            bVar4 = quest:DisplayTutorial(9)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00dd1e70_c1 end
                bVar4 = quest:MsgIsTutorialClickedPast()
                while not bVar4 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00dd1e70_c1 end
                    bVar4 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00dd1e70_c1 end
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
        bVar4 = not alive
        if not bVar4 then
            dist = 20.0
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, dist)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00dd1d98
                end
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_GWL"
                        goto LAB_00dd1d98
                    end
                    -- LAB_00dd1e3d: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00dd1e53: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                    goto LAB_00dd1e95
                end
                string = "CS_ORCHARD_EVIL_WHISPERINTRO_GWL"
                ::LAB_00dd1d98::
                resources:RunMacro(string, xStack_38, false, true)
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
                    if bVar4 then goto LAB_00dd1e70 end
                    bVar4 = quest:MsgIsTutorialClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        bVar4 = quest:MsgIsTutorialClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00dd1e70 end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_EVIL_WHISPERINTRO_LOP"
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
                        string = "CS_ORCHARD_GOOD_WHISPERINTRO_LOP"
                    end
                    resources:RunMacro(string, xStack_38, false, true)
                    quest:FixMovieSequenceCamera(false)
                    native_arg_sequence_2 = false
                    if quest:GetStateBool("ShownCombatMultiplierTutorial") then
                        native_arg_sequence_2 = true
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
                        if bVar4 then goto LAB_00dd1e70 end
                        bVar4 = quest:MsgIsTutorialClickedPast()
                        while not bVar4 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00dd1e70 end
                            bVar4 = quest:MsgIsTutorialClickedPast()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00dd1e70 end
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
    resources:DestroyMovie(xStack_20)
    ::LAB_00dd1e95::
    resources:DestroyActorMap(xStack_38)
    resources:ReleaseResource(xStack_10)
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
end

