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
    local ppu_stk_c
    local bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
    if bVar2 then
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
    ppu_stk_c = nil
    quest:SetStateThing("Teams_0_TeamCrateCarrier", nil)
    ppu_stk_c = nil
    ppu_stk_c = 0
    ppu_stk_c = nil
    quest:SetStateThing("Teams_1_TeamCrateCarrier", nil)
    ppu_stk_c = nil
    ppu_stk_c = 0
    -- TODO(native): CCharString__AssignFromWide(this + 0x68,0x12db0c8);
    -- TODO(native): CCharString__AssignFromWide(this + 0x6c,0x12db088);
    -- TODO(native): CCharString__AssignFromWide(this + 0x70,0x12db040);
    -- TODO(native): CCharString__AssignFromWide(this + 0x74,0x12db000);
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
    local appuStack_9c, appuStack_ac, appuStack_bc, bVar10, bVar4, cVar1, c_stk_c1, fVar5, f_stk_14, iVar11, pCVar6, pCVar8, pMessage, pPosition, r1, r2, r3, r4
    local alive = true
    local function __cleanup_LAB_00dd0b11()
        pCVar6 = 0x0
        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
        resources:ReleaseResource(appuStack_ac)
        resources:DestroyActorMap(r2)
        resources:ReleaseResource(appuStack_bc)
        resources:ReleaseResource(aCStack_c4)
    end
    local function __cleanup_LAB_00dd0b2b()
        resources:ReleaseResource(appuStack_ac)
        resources:DestroyActorMap(r2)
        resources:ReleaseResource(appuStack_bc)
        resources:ReleaseResource(aCStack_c4)
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
                    appuStack_bc = resources:NewResource()
                    appuStack_ac = resources:NewResource()
                    resources:TryAcquire(appuStack_ac, r1, 4)
                    iVar11 = 4
                    -- TODO(native): pppuVar9 = appuStack_bc;
                    pCVar6 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, pCVar6, iVar11)
                    r2 = resources:NewActorMap()
                    resources:SetActor(r2, "HERO", appuStack_bc)
                    resources:SetActor(r2, "WHISPER", appuStack_ac)
                    appuStack_9c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    pCVar6 = quest:GetHero()
                    f_stk_14 = (quest:GetDistanceBetweenThings(r3, pCVar6) ^ 2)
                    pCVar6 = quest:GetHero()
                    fVar5 = (quest:GetDistanceBetweenThings(appuStack_9c, pCVar6) ^ 2)
                    if f_stk_14 <= fVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_BACK", r2, false, true)
                            pCVar8 = "CS_ORCHARD_EVIL_WHISPER_BACK"
                            goto LAB_00dd08eb
                        end
                        -- TODO(native): goto LAB_00dd0b1f
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        __cleanup_LAB_00dd0b11()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_EVIL_WHISPER_FRONT", r2, false, true)
                    pCVar8 = "CS_ORCHARD_EVIL_WHISPER_FRONT"
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
                            __cleanup_LAB_00dd0b2b(); return
                        end
                        ::LAB_00dd0977::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(appuStack_9c)
                    resources:DestroyActorMap(r2)
                    resources:ReleaseResource(appuStack_ac)
                    resources:ReleaseResource(appuStack_bc)
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
                    appuStack_ac = resources:NewResource()
                    iVar11 = 4
                    -- TODO(native): pppuVar9 = appuStack_ac;
                    appuStack_bc = resources:NewResource()
                    pCVar6 = quest:GetHero()
                    resources:TryAcquire(pppuVar9, pCVar6, iVar11)
                    resources:TryAcquire(appuStack_bc, r4, 4)
                    r2 = resources:NewActorMap()
                    resources:SetActor(r2, "HERO", appuStack_ac)
                    resources:SetActor(r2, "WHISPER", appuStack_bc)
                    appuStack_9c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_ORCHARD_EVIL_OUTRO", r2, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (appuStack_9c);
                    resources:DestroyActorMap(r2)
                    -- TODO(native): CSubtitleRenderer::SetText(0x55);
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(0xd98))
                    quest:SetMasterGameState("OrchardFarmRaidLastCompleted", 1)
                    quest:RemoveQuestInfoElement(fVar5)
                    bVar4 = true
                    pCVar6 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar6, bVar4)
                    bVar10 = false
                    bVar4 = false
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(pCVar8, bVar4, bVar10, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidEvil", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidEvil", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                    until not (not bVar4)
                    resources:ReleaseResource(appuStack_bc)
                    resources:ReleaseResource(appuStack_ac)
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
    local aCStack_98, amStack_8c, auStack_c4, bVar12, bVar3, cVar1, c_stk_c5, fVar9, iVar11, iVar4, native_arg_sequence_1, pCVar5, pCVar7, pCVar8, pMessage, pOther_00, pPosition, ppuStack_80, ppuStack_9c, puVar10, r1, r2, r3, r4
    local alive = true
    local function __cleanup_LAB_00dd1728()
        pCVar7 = 0x0
        quest:PauseAllNonScriptedEntities((pCVar7 ~= 0))
        resources:ReleaseResource((r2 + 4))
        resources:DestroyActorMap(ppuStack_9c)
        resources:ReleaseResource(amStack_8c)
        resources:ReleaseResource(CStack_cc)
    end
    local function __cleanup_LAB_00dd1742()
        resources:ReleaseResource((r2 + 4))
        resources:DestroyActorMap(ppuStack_9c)
        resources:ReleaseResource(amStack_8c)
        resources:ReleaseResource(CStack_cc)
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
                pCVar7 = ""
                -- TODO(native): CStack_cc._3_1_ = CScriptThing::MsgIsKilledBy(pCVar5);
                if CStack_cc._3_1_ ~= 0 then
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
                    auStack_c4 = resources:NewResource()
                    amStack_8c = resources:NewResource()
                    -- TODO(native): ppuStack_80[0] = 0;
                    resources:TryAcquire(amStack_8c, r1, 4)
                    iVar11 = 4
                    puVar10 = auStack_c4
                    pCVar5 = quest:GetHero()
                    resources:TryAcquire(puVar10, pCVar5, iVar11)
                    aCStack_98 = resources:NewActorMap()
                    resources:SetActor(aCStack_98, "HERO", auStack_c4)
                    resources:SetActor(aCStack_98, "WHISPER", amStack_8c)
                    ppuStack_9c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    -- TODO(native): this_00 = *(int **)(this + 0x40);
                    pCVar5 = quest:GetHero()
                    -- TODO(native): aCStack_1c[0] = (CCharString)(quest:GetDistanceBetweenThings(auStack_c4, pCVar5) ^ 2);
                    pCVar5 = quest:GetHero()
                    fVar9 = (quest:GetDistanceBetweenThings(auStack_b0 + 4, pCVar5) ^ 2)
                    if aCStack_1c[0] <= fVar9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_BACK", ppuStack_9c, false, true)
                            pCVar8 = "CS_ORCHARD_GOOD_WHISPER_BACK"
                            goto LAB_00dd14ee
                        end
                        -- TODO(native): goto LAB_00dd1736
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        __cleanup_LAB_00dd1728()
                        return
                    end
                    resources:RunMacro("CS_ORCHARD_GOOD_WHISPER_FRONT", ppuStack_9c, false, true)
                    pCVar8 = "CS_ORCHARD_GOOD_WHISPER_FRONT"
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
                            pCVar7 = 0x0
                            quest:PauseAllNonScriptedEntities((pCVar7 ~= 0))
                            __cleanup_LAB_00dd1742(); return
                        end
                        ::LAB_00dd157a::
                        quest:SetStateBool("ShownCombatFlourishTutorial", true)
                    end
                    quest:SetStateBool("WhisperInCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(ppuStack_9c)
                    resources:DestroyActorMap(aCStack_98)
                    resources:ReleaseResource(amStack_8c)
                    resources:ReleaseResource(auStack_c4)
                    iVar4 = i_stk_14
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
                pCVar7 = quest:GetHero()
                c_stk_c5 = pCVar7:MsgIsKilledBy("")
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
                    auStack_c4 = resources:StartMovie("")
                    ppuStack_9c = resources:StartMovie("")
                    puVar10 = auStack_c4 + 4
                    pCVar7 = quest:GetHero()
                    resources:TryAcquire(puVar10, pCVar7, 4)
                    resources:TryAcquire(ppuStack_9c, r4, 4)
                    -- TODO(native): StdMap_Construct_API(auStack_a8 + 4);
                    puVar10 = auStack_c4 + 4
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(auStack_a8 + 4),&CStack_cc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,puVar10);
                    pOther_00 = ppuStack_9c
                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(auStack_a8 + 4),&CStack_cc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(pCVar6,pOther_00);
                    ppuStack_80 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): RunCutsceneMacro_Func(&CStack_cc,auStack_a8 + 4,(void *)0x0,(void *)0x0,false,true);
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(ppuStack_80)
                    -- TODO(native): StdMap_Destroy_API(auStack_a8 + 4);
                    -- TODO(native): CSubtitleRenderer::SetText(0x50);
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(0xd9c))
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
                    quest:RemoveQuestInfoElement(4)
                    bVar3 = true
                    pCVar7 = quest:GetThingWithScriptName("OFFarmhouseDoor")
                    quest:SetThingAsUsable(pCVar7, bVar3)
                    bVar12 = false
                    bVar3 = false
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsCompleted(pCVar8, bVar3, bVar12, false)
                    quest:SetQuestAsCompleted("Q_OrchardFarmRaidGood", true, false, false)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaidGood", 0)
                    quest:DeactivateQuestLater("Q_OrchardFarmRaid", 0)
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                    until not (not bVar3)
                    ::LAB_00dd1a93::
                    resources:ReleaseResource(ppuStack_9c)
                    resources:ReleaseResource(auStack_c4)
                    return
                end
                if quest:GetStateInt("MissionFailed") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    ReplaceQuestCards(quest)
                    quest:RemoveQuestInfoElement(iVar11)
                    bVar12 = true
                    pMessage = quest:GetStateString(("FailReasons_" .. quest:GetStateInt("MissionFailed")))
                    bVar3 = true
                    pCVar8 = quest:GetActiveQuestName()
                    quest:SetQuestAsFailed(pCVar8, bVar3, pMessage, bVar12)
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
    local aCStack_10, aCStack_20, aCStack_30, aCStack_40, amStack_5c, bVar2, iVar8, pCStack_70, pCVar3, pCVar7, r1, r2
    local alive = true
    local au_stk_90 = nil
    if quest:GetStateInt("HeroTeam") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        pCVar3 = quest:GetThingWithScriptName("BanditTeamSpawn")
        au_stk_90 = pCVar3
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end
        pCVar3 = quest:GetThingWithScriptName("GuardTeamSpawn")
        au_stk_90 = pCVar3
    end
    -- LAB_00dcfb65: (native jump target)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            au_stk_90 = nil
            return
        end
        bVar2 = quest:IsRegionLoaded("OrchardFarm")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                au_stk_90 = nil
                return
            end
            bVar2 = quest:IsRegionLoaded("OrchardFarm")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(au_stk_90, pCVar3, 10.0)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then return end
            if quest:GetStateInt("HeroTeam") == 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then return end
                r1 = quest:GetAllThingsWithScriptName("BanditTeamMember")
                r2 = quest:GetNearestWithScriptName(pCVar3, "GuardTeamMember")
                amStack_5c = resources:StartMovie("")
                aCStack_30 = resources:StartMovie("")
                aCStack_20 = resources:StartMovie("")
                aCStack_10 = resources:StartMovie("")
                resources:TryAcquire(aCStack_10, r2, 4)
                resources:TryAcquire(amStack_5c, 0, 4)
                resources:TryAcquire(aCStack_30, 0 + 1, 4)
                iVar8 = 4
                pCVar7 = aCStack_20
                pCVar3 = quest:GetHero()
                resources:TryAcquire(pCVar7, pCVar3, iVar8)
                quest:SheatheHeroWeapons()
                amStack_5c = resources:NewActorMap()
                resources:SetActor(amStack_5c, "HERO", aCStack_20)
                resources:SetActor(amStack_5c, "BAN1", amStack_5c)
                resources:SetActor(amStack_5c, "BAN2", aCStack_30)
                resources:SetActor(amStack_5c, "GUARD", aCStack_10)
                aCStack_40 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_EVIL_INTRO", amStack_5c, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:ReleaseResource(aCStack_40)
                resources:DestroyActorMap(amStack_5c)
                resources:ReleaseResource(aCStack_10)
                resources:ReleaseResource(aCStack_20)
                resources:ReleaseResource(aCStack_30)
                resources:ReleaseResource(amStack_5c)
                quest:KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false)
                pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(pCVar3)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then return end
                pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
                quest:OpenDoor(pCVar3)
                amStack_5c = resources:StartMovie("")
                iVar8 = 4
                pCVar7 = amStack_5c
                pCVar3 = quest:GetHero()
                resources:TryAcquire(pCVar7, pCVar3, iVar8)
                pCStack_70 = resources:NewActorMap()
                resources:SetActor(pCStack_70, "HERO", amStack_5c)
                aCStack_40 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_ORCHARD_GOOD_INTRO", pCStack_70, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:ReleaseResource(aCStack_40)
                resources:DestroyActorMap(pCStack_70)
                resources:ReleaseResource(amStack_5c)
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
        if bVar2 then return end
        quest:SetStateBool("HeroAtWrongEntrance", true)
        pCVar3 = quest:GetThingWithScriptName("OF_MainGates")
        -- TODO(native): CloseDoor is not a ForgeFSE binding
        quest:CloseDoor(pCVar3)
        bVar2 = false
        pCVar3 = quest:GetThingWithScriptName(r1)
        quest:SetThingAsUsable(pCVar3, bVar2)
        quest:Pause(1.0)
        quest:DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE")
        bVar2 = quest:MsgIsGameInfoClickedPast()
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then return end
            bVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end
        bVar2 = quest:IsRegionLoaded("OrchardFarm")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then return end
                bVar2 = quest:IsRegionLoaded("OrchardFarm")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then return end
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
            if bVar3 then return end  -- TODO(native): goto LAB_00dccfa3
        end
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
    local appuStack_20, bVar4, local_10, local_3c, native_arg_sequence_1, native_arg_sequence_2, pCVar5, pScriptObject, puStack_38
    local alive = true
    local function __region_LAB_00dd1e3d()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(appuStack_20)
    end
    local function __region_LAB_00dd1e53()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(appuStack_20)
    end
    local_10 = resources:NewResource()
    pCVar5 = quest:GetHero()
    resources:TryAcquire(pScriptObject, pCVar5, 4)
    puStack_38 = resources:NewActorMap()
    resources:SetActor(puStack_38, "HERO", local_10)
    appuStack_20 = resources:StartMovie("")
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
            -- TODO(native): goto LAB_00dd1d98
        end
        -- LAB_00dd1d15: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): local_3c = (CScriptThing *)piVar3;
            local_3c = quest:GetHero()
            pCVar5 = quest:GetThingWithScriptName("MK_OFI_GWL")
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar5, local_3c, 20.0)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00dd1d15
                if quest:GetStateInt("HeroTeam") ~= 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): goto LAB_00dd1d98
                    end
                    __region_LAB_00dd1e3d()
                    goto LAB_00dd1e95
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    __region_LAB_00dd1e53()
                    goto LAB_00dd1e95
                end
                -- LAB_00dd1d98: (native jump target)
                resources:RunMacro("CS_ORCHARD_EVIL_WHISPERINTRO_GWL", pCVar5, false, true)
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
                    if bVar4 then __region_LAB_00dd1e53(); goto LAB_00dd1e95 end
                    bVar4 = quest:MsgIsTutorialClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __region_LAB_00dd1e53(); goto LAB_00dd1e95 end
                        bVar4 = quest:MsgIsTutorialClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __region_LAB_00dd1e3d(); goto LAB_00dd1e95 end
                end
                quest:SetStateBool("ShownCombatMultiplierTutorial", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    if quest:GetStateInt("HeroTeam") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __region_LAB_00dd1e3d(); goto LAB_00dd1e95 end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __region_LAB_00dd1e53(); goto LAB_00dd1e95 end
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
    resources:DestroyActorMap(pCVar5)
    resources:ReleaseResource(local_10)
    return extraout_EAX
end

function ReplaceQuestCards(quest)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
    return extraout_EAX
end

