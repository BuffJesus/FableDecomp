-- Readable native conversion: GuildMasterGameFlow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local helpers = require("V_GuildMaster.native_quest_helpers")

-- GuildMasterGameFlow.Main (retail 0x00e90be0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local c_stk_1b1_1, c_stk_1b1_2, questionAnswer, this_00, scratchValue, scratchValue12
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
                if hero ~= nil and hero:IsDistanceFromPositionUnder(teleporterResidue3:GetPos(), 3.0) then
                    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("FocalSitesHSP"), false)
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_0 then
                        me:Speak(hero, helpers.GetGuildMasterSpeech(quest, me), 0, false, true, false)
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
                            if not me:Speak(hero, helpers.GetGuildMasterSpeech(quest, me), 0, false, true, false) then goto LAB_00e91ccb end
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

