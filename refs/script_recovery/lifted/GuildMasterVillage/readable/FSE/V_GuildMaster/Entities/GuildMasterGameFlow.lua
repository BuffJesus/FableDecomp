-- Readable native conversion: GuildMasterGameFlow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X44, self0X4A, self0X4B, self0X4C, self0X49

-- GuildMasterGameFlow.Main (retail 0x00e90be0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local c_stk_1b1_1, c_stk_1b1_2, questionAnswer, line, this_00, scratchValue, scratchValue12
    local scratchValue13
    scratchValue13 = 0
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if quest:IsQuestActive("Q_EndGameFocalSites") then
            local resource2 = resources:NewResource()
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource2)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource2)
                resources:ReleaseResource(resource)
                return
            end
            local actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource2)
            resources:SetActor(actorMap, "GUILDDUDE", resource)
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_FOCALSITEINTRO", actorMap, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource2)
            quest:SetTeleportingAsActive(false)
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ACTIVATE_FOCAL_SITES", "Q_EndGameFocalSites", false)
            quest:SetQuestCardObjective("Q_EndGameFocalSites", "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_01", "", "")
            local teleporterResidue = quest:GetThingWithScriptName("TeleporterResidue")
            quest:CreateEffectAtPos("NEWTELEPORTER2", teleporterResidue:GetPos(), 0.0, false)
            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TeleporterResidue"), "HUD_ORB_QUEST_CORE")
            while quest:IsQuestActive("Q_EndGameFocalSites") do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                local teleporterResidue3 = quest:GetThingWithScriptName("TeleporterResidue")
                if IsDistanceFromThingToPositionUnder(hero,teleporterResidue3:GetPos(),3.0) ~= 0 then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("FocalSitesHSP"), false)
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_0 then
                        -- TODO(native): pCVar11 = GetGuildMasterSpeech(quest, me, *(this + 0x14))
                        line = nil --[[unresolved native value]]
                        me:Speak(hero, line, 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                end
                scratchValue12 = scratchValue13
                scratchValue13 = scratchValue13 | 1
                if not me:MsgIsHitByHero() then
                    scratchValue = scratchValue12 | 3
                    scratchValue13 = scratchValue
                    -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                    if nil then
                        scratchValue = scratchValue12 | 7
                        scratchValue13 = scratchValue
                        -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                        if not nil then goto LAB_00e914b3 end
                    end
                    c_stk_1b1_1 = 0
                else
                    goto LAB_00e914b3
                end
                goto FLOW_past_lab_00e914b3
                ::LAB_00e914b3::
                c_stk_1b1_1 = 1
                ::FLOW_past_lab_00e914b3::
                if scratchValue & 4 ~= 0 then
                    scratchValue = scratchValue & 0xfffffffb
                    scratchValue13 = scratchValue
                end
                if scratchValue & 2 ~= 0 then
                    scratchValue = scratchValue & 0xfffffffd
                    scratchValue13 = scratchValue
                end
                if scratchValue & 1 ~= 0 then
                    scratchValue13 = scratchValue & 0xfffffffe
                end
                if c_stk_1b1_1 ~= 0 then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_00 then
                        me:Speak(hero, "TEXT_QST_081_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie5)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        end
        scratchValue12 = scratchValue13
        scratchValue13 = scratchValue13 | 8
        if not me:MsgIsHitByHero() then
            scratchValue = scratchValue12 | 24
            scratchValue13 = scratchValue
            -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
            if nil then
                scratchValue = scratchValue12 | 56
                scratchValue13 = scratchValue
                -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                if not nil then goto LAB_00e91742 end
            end
            c_stk_1b1_2 = 0
        else
            goto LAB_00e91742
        end
        goto FLOW_past_lab_00e91742
        ::LAB_00e91742::
        c_stk_1b1_2 = 1
        ::FLOW_past_lab_00e91742::
        if scratchValue & 32 ~= 0 then
            scratchValue = scratchValue & 0xffffffdf
            scratchValue13 = scratchValue
        end
        if scratchValue & 16 ~= 0 then
            scratchValue = scratchValue & 0xffffffef
            scratchValue13 = scratchValue
        end
        if scratchValue & 8 ~= 0 then
            scratchValue13 = scratchValue & 0xfffffff7
        end
        if c_stk_1b1_2 ~= 0 then
            if not quest:IsActiveThreadTerminating() then
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                if fret_01 <= 0.0 then
                    goto LAB_00e91898
                end
                goto FLOW_past_lab_00e91898
                ::LAB_00e91898::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto LAB_00e918af
                ::FLOW_past_lab_00e91898::
                me:Speak(hero, "TEXT_QST_081_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = movie3
                        goto LAB_00e91ef9
                    end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00e91898 end
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie3
                goto LAB_00e91ef9
            end
            goto FLOW_hoist_lab_00e91ef9_1
        end
        goto FLOW_past_lab_00e91ef9
        ::LAB_00e91ef9::
        resources:DestroyMovie(this_00)
        ::FLOW_hoist_lab_00e91ef9_1::
        break
        ::FLOW_past_lab_00e91ef9::
        ::LAB_00e918af::
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if quest:GetMasterGameState("PostSavePosition") ~= 1450 then
                    if not quest:IsActiveThreadTerminating() then
                        local fret_05 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_05 then
                            -- TODO(native): pCVar11 = GetGuildMasterSpeech(quest, me, *(this + 0x14))
                            line = nil --[[unresolved native value]]
                            if not me:Speak(hero, line, 0, false, true, false) then goto LAB_00e91ccb end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e91ee8 end
                        end
                        goto LAB_00e91db4
                    end
                    goto LAB_00e91ee8
                end
                if quest:IsActiveThreadTerminating() then
                    goto LAB_00e91ccb
                else
                    local fret_02 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_02 then
                        if not me:Speak(hero, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e91ee8 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e91ccb end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_QUESTION_TAKE_CARD", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e91ee8 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e91ccb end
                    if questionAnswer ~= 1 then
                        local fret_04 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_04 then
                            if not me:Speak(hero, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e91ee8 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e91ccb end
                        end
                        goto LAB_00e91db4
                        goto FLOW_hoist_lab_00e91db4_1
                    end
                    goto FLOW_hoist_lab_00e91db4_2
                end
                goto FLOW_past_lab_00e91db4
                ::LAB_00e91db4::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00e91dcc
                ::FLOW_hoist_lab_00e91db4_1::
                goto LAB_00e91ccb
                ::FLOW_hoist_lab_00e91db4_2::
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    if not me:Speak(hero, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e91ccb end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e91ee8 end
                end
                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_WIZARD_BATTLE", "Q_WizardBattle", false)
                quest:TakeObjectFromHero("OBJECT_MAZE_JOURNAL")
                goto LAB_00e91db4
                goto LAB_00e91ee8
                ::FLOW_past_lab_00e91db4::
                goto FLOW_past_lab_00e91ee8
                ::LAB_00e91ee8::
                quest:PauseAllNonScriptedEntities(false)
                ::FLOW_past_lab_00e91ee8::
                goto FLOW_past_lab_00e91ccb
                ::LAB_00e91ccb::
                quest:PauseAllNonScriptedEntities(false)
                ::FLOW_past_lab_00e91ccb::
                this_00 = movie
                goto LAB_00e91ef9
            end
            break
        end
        ::LAB_00e91dcc::
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    until false
    resources:ReleaseResource(resource)
end

-- GuildMasterGameFlow.Init (retail 0x00e909a0)
function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

-- GuildMasterGameFlow.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- GuildMasterGameFlow.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

-- GuildMasterGameFlow.GetGuildMasterSpeech (retail 0x00e91f20)
-- E91F20: bsim names this body NScript::CV_GuildMasterScript::GetGuildMasterSpeech (a homologous script member); no PDB name
function GetGuildMasterSpeech(quest, me)
    local scratchValue, self0X, sequence, scratchValue4
    -- TODO(native): CCharString::CCharString(&xStack_c,other);
    local function ReleaseEverything()
        do return "" end
    end
    self0X = self0X44
    -- TODO(native): iVar1 = *(iVar4 + 4)
    local scratchValue3 = nil --[[unresolved native value]]
    if scratchValue3 < 901 then
        if scratchValue3 == 900 then
            scratchValue4 = "TEXT_QST_081_ARENA"
        elseif scratchValue3 < 701 then
            if scratchValue3 == 700 then
                goto LAB_00e92080
            elseif scratchValue3 < 401 then
                if scratchValue3 == 400 then
                    scratchValue4 = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"
                else
                    repeat
                        if scratchValue3 == 100 then
                            scratchValue4 = "TEXT_QST_081_TRAINING"
                            break
                        elseif scratchValue3 == 150 then
                            scratchValue4 = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"
                            break
                        elseif scratchValue3 == 200 then
                            scratchValue4 = "TEXT_QST_081_WASP_BOSS"
                            break
                        elseif scratchValue3 == 300 then
                            scratchValue4 = "TEXT_QST_081_MAZE_MEETING"
                        else
                            -- FLOW_native_label_2_c1: (native jump target)
                            do return "" end
                            goto FLOW_after_flow_native_label_2
                        end
                    until true
                end
            else
                repeat
                    if scratchValue3 == 450 then
                        scratchValue4 = "TEXT_QST_081_ORCHARD_FARM"
                        break
                    elseif scratchValue3 == 500 then
                        scratchValue4 = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"
                        break
                    elseif scratchValue3 == 550 then
                        scratchValue4 = "TEXT_QST_081_TRADER_ESCORT"
                        break
                    elseif scratchValue3 == 600 then
                        local predicateResult = quest:IsActiveThreadTerminating()
                        if quest:IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp") then
                            if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                            goto LAB_00e92080
                        end
                        if predicateResult then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                        scratchValue4 = "TEXT_QST_081_SECOND_MAZE_MEETING"
                    else
                        -- FLOW_native_label_2_c2: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2
                    end
                until true
            end
            goto FLOW_past_lab_00e92080
            ::LAB_00e92080::
            scratchValue4 = "TEXT_QST_081_BANDIT_CAMP"
            ::FLOW_past_lab_00e92080::
        else
            repeat
                if scratchValue3 == 733 then
                    scratchValue4 = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"
                    break
                elseif scratchValue3 == 766 then
                    scratchValue4 = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"
                    break
                else
                    if scratchValue3 == 800 then
                        local predicateResult2 = quest:IsActiveThreadTerminating()
                        if quest:IsQuestActive("V_TrophyDealer") then
                            if predicateResult2 then
                                -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                                do return "" end
                                goto FLOW_after_flow_native_label_2
                            end
                            goto FLOW_native_label_1
                        end
                        if predicateResult2 then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                        scratchValue4 = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"
                        break
                    elseif scratchValue3 == 850 then
                        goto FLOW_native_label_1
                    elseif scratchValue3 == 856 then
                        scratchValue4 = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"
                        break
                    elseif scratchValue3 == 865 then
                        scratchValue4 = "TEXT_QST_081_WHITE_BALVERINE_KHG"
                        break
                    elseif scratchValue3 == 870 then
                        scratchValue4 = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"
                        break
                    elseif scratchValue3 == 875 then
                        scratchValue4 = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"
                    else
                        -- FLOW_native_label_2_c4: (native jump target)
                        do return "" end
                        goto FLOW_after_flow_native_label_2
                    end
                    goto FLOW_past_flow_native_label_1
                    ::FLOW_native_label_1::
                    scratchValue4 = "TEXT_QST_081_TROPHY_DEALER"
                    break
                    ::FLOW_past_flow_native_label_1::
                end
            until true
        end
    else
        if scratchValue3 < 1501 then
            if scratchValue3 == 1500 then
                scratchValue4 = "TEXT_QST_081_WIZARD_BATTLE"
            else
                if scratchValue3 < 1217 then
                    if scratchValue3 == 1216 then
                        scratchValue4 = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"
                    else
                        repeat
                            if scratchValue3 == 1000 then
                                scratchValue4 = "TEXT_QST_081_MEET_SISTER"
                                break
                            elseif scratchValue3 == 1050 then
                                scratchValue4 = "TEXT_QST_081_WAITING_FOR_MCC"
                                break
                            elseif scratchValue3 == 1100 then
                                scratchValue4 = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"
                                break
                            elseif scratchValue3 == 1200 then
                                scratchValue4 = "TEXT_QST_081_GRAVEYARD"
                            else
                                do return "" end
                                goto FLOW_after_flow_native_label_2_243
                            end
                        until true
                    end
                else
                    repeat
                        if scratchValue3 == 1233 then
                            scratchValue4 = "TEXT_QST_081_FINALGRAVEYARD"
                            break
                        elseif scratchValue3 == 1250 then
                            scratchValue4 = "TEXT_QST_081_PRISON"
                            break
                        elseif scratchValue3 == 1300 then
                            -- TODO(native): if *(iVar4 + 0x60) == 0 then
                            if false then
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                scratchValue4 = "TEXT_QST_081_BEFORE_HOOK_COAST"
                            else
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                                scratchValue4 = "TEXT_QST_081_HOOK_COAST"
                            end
                            break
                        elseif scratchValue3 == 1450 then
                            scratchValue4 = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
                        else
                            -- FLOW_native_label_2: (native jump target)
                            return ""
                        end
                    until true
                end
                ::FLOW_after_flow_native_label_2_243::
            end
        else
            if scratchValue3 < 2301 then
                if scratchValue3 == 2300 then goto LAB_00e923f7 end
                if scratchValue3 < 1701 then
                    if scratchValue3 == 1700 then
                        scratchValue4 = "TEXT_QST_081_JACK_BOSS_FIGHT"
                    elseif scratchValue3 == 1550 then
                        scratchValue4 = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"
                    else
                        if scratchValue3 ~= 1600 then
                            do return "" end
                            goto FLOW_after_flow_native_label_2_358
                        end
                        scratchValue4 = "TEXT_QST_081_FOCAL_SITES"
                    end
                else
                    if scratchValue3 ~= 2100 then
                        do return "" end
                        goto FLOW_after_flow_native_label_2_358
                    end
                    scratchValue4 = "TEXT_QST_081_SHIP_SUMMONING"
                end
            elseif scratchValue3 == 2400 then
                goto LAB_00e923f7
            elseif scratchValue3 == 2500 then
                -- TODO(native): iVar4 = *(iVar4 + 0x70)
                self0X = nil --[[unresolved native value]]
                if self0X == 0 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto FLOW_after_flow_native_label_2_358 end
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS"
                elseif self0X == 1 then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                        do return "" end
                        goto FLOW_after_flow_native_label_2_358
                    end
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER"
                elseif self0X == 2 then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                        do return "" end
                        goto FLOW_after_flow_native_label_2_358
                    end
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA"
                elseif self0X == 3 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER"
                elseif self0X == 4 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL"
                elseif self0X == 5 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER"
                elseif self0X == 6 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR"
                else
                    if self0X ~= 7 then
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): CCharString::CCharString(in_stack_00000004,&xStack_c);
                            do return "" end
                            goto FLOW_after_flow_native_label_2_358
                        end
                        do return "" end
                        goto FLOW_after_flow_native_label_2_358
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end  -- TODO(native): goto FLOW_after_flow_native_label_2_358
                    scratchValue4 = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"
                end
            else
                if scratchValue3 ~= 2600 then
                    do return "" end
                    goto FLOW_after_flow_native_label_2_358
                end
                scratchValue4 = "TEXT_QST_081_DRAGON_FIGHT_10"
            end
            goto FLOW_past_lab_00e923f7
            ::LAB_00e923f7::
            scratchValue4 = "TEXT_QST_081_THE_ORACLE"
            ::FLOW_past_lab_00e923f7::
        end
        ::FLOW_after_flow_native_label_2_358::
    end
    ::FLOW_after_flow_native_label_2::
    sequence = self_0x50 == in_stack_fffffff0
    if not sequence then
        sequence = self_0x50 ~= nil and in_stack_fffffff0 ~= nil
        if sequence then
            -- TODO(native): if *(CVar2 + 4) == *(in_stack_fffffff0 + 4) then
            sequence = false
        end
        if sequence then
            -- TODO(native): iVar4 = CBasicString<char>::Compare(*(void **)CVar2,*(void **)in_stack_fffffff0);
            sequence = self0X == 0
        end
    end
    if sequence then
        local scratchValue6 = math.random(0, 32767) & 0x80000003
        scratchValue = scratchValue6 == 0
        if scratchValue6 < 0 then
            scratchValue = (scratchValue6 - 1 | 0xfffffffc) == 0xffffffff
        end
        if scratchValue then
            if not self0X4A then
                -- TODO(native): name field 0x4a (undefined1)
                self0X4A = true
                scratchValue4 = "TEXT_QST_081_INFO_TELEPORTERS"
            elseif not self0X4B then
                -- TODO(native): name field 0x4b (undefined1)
                self0X4B = true
                scratchValue4 = "TEXT_QST_081_INFO_WEAPONS"
            elseif not self0X4C then
                -- TODO(native): name field 0x4c (undefined1)
                self0X4C = true
                scratchValue4 = "TEXT_QST_081_INFO_LOG_BOOK"
            elseif not ((quest:GetHeroTitle() ~= 19) or self0X49) then
                -- TODO(native): name field 0x49 (undefined1)
                self0X49 = true
                scratchValue4 = "TEXT_QST_081_INFO_HERO_TITLE"
            end
        end
    end
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x50),&xStack_10);
    return scratchValue4
end

