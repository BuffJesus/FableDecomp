-- Readable native conversion: GuildMasterGameFlow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_GuildMaster.native_quest_helpers")

-- GuildMasterGameFlow.Main (retail 0x00e90be0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local controlAcquired, questionAnswer, position, pppuVar, line, this_00
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
            if quest:IsActiveThreadTerminating() then break end
            local resource2 = resources:NewResource()
            resources:PrepareResource(resource2)
            pppuVar = resource2
            local hero3 = quest:GetHero()
            controlAcquired = resources:TryAcquire(pppuVar, hero3, 4)
            while not controlAcquired do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource2)
                    resources:ReleaseResource(resource)
                    return
                end
                pppuVar = resource2
                local hero4 = quest:GetHero()
                controlAcquired = resources:TryAcquire(pppuVar, hero4, 4)
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
            position = teleporterResidue:GetPos()
            quest:CreateEffectAtPos("NEWTELEPORTER2", position, 0.0, false)
            local teleporterResidue2 = quest:GetThingWithScriptName("TeleporterResidue")
            quest:MiniMapAddMarker(teleporterResidue2, "HUD_ORB_QUEST_CORE")
            while quest:IsQuestActive("Q_EndGameFocalSites") do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                local teleporterResidue3 = quest:GetThingWithScriptName("TeleporterResidue")
                position = teleporterResidue3:GetPos()
                local hero5 = quest:GetHero()
                if hero5 ~= nil and hero5:IsDistanceFromPositionUnder(position, 3.0) then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local focalSitesHSP = quest:GetThingWithScriptName("FocalSitesHSP")
                    local hero = quest:GetHero()
                    quest:EntityTeleportToThing(hero, focalSitesHSP, false)
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue15 = resources:ScriptThing(resource)
                    local fret_0 = quest:GetHealth(scratchValue15)
                    if 0.0 < fret_0 then
                        line = helpers.GetGuildMasterSpeech(quest, me)
                        local hero6 = quest:GetHero()
                        if not me:Speak(hero6, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            resources:ReleaseResource(resource)
                            return
                        end
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
                if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue16 = resources:ScriptThing(resource)
                    local fret_00 = quest:GetHealth(scratchValue16)
                    if 0.0 < fret_00 then
                        local hero7 = quest:GetHero()
                        if not me:Speak(hero7, "TEXT_QST_081_ATTACKED", GROUP_SELECT_FIRST, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            resources:ReleaseResource(resource)
                            return
                        end
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
        if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if not quest:IsActiveThreadTerminating() then
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue18 = resources:ScriptThing(resource)
                local fret_01 = quest:GetHealth(scratchValue18)
                if not (fret_01 <= 0.0) then
                    goto FLOW_past_lab_00e91898
                end
                ::LAB_00e91898::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto FLOW_past_lab_00e91ef9
                ::FLOW_past_lab_00e91898::
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, "TEXT_QST_081_ATTACKED", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie3
                    goto LAB_00e91ef9
                end
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
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if quest:GetMasterGameState("PostSavePosition") ~= 1450 then
                    if not quest:IsActiveThreadTerminating() then
                        local scratchValue19 = resources:ScriptThing(resource)
                        local fret_05 = quest:GetHealth(scratchValue19)
                        if 0.0 < fret_05 then
                            line = helpers.GetGuildMasterSpeech(quest, me)
                            local hero9 = quest:GetHero()
                            if not me:Speak(hero9, line, 0, false, true, false) then
                                goto LAB_00e91ccb
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e91ccb end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e91ee8 end
                        end
                        goto LAB_00e91db4
                    end
                    goto LAB_00e91ee8
                end
                if quest:IsActiveThreadTerminating() then
                    goto LAB_00e91ccb
                else
                    local scratchValue20 = resources:ScriptThing(resource)
                    local fret_02 = quest:GetHealth(scratchValue20)
                    if 0.0 < fret_02 then
                        local hero2 = quest:GetHero()
                        if not me:Speak(hero2, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00e91ee8
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e91ee8 end
                        end
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
                        do
                            local scratchValue17 = resources:ScriptThing(resource)
                            local fret_04 = quest:GetHealth(scratchValue17)
                            if 0.0 < fret_04 then
                                local hero10 = quest:GetHero()
                                if not me:Speak(hero10, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO", GROUP_SELECT_FIRST, false, true, false) then
                                    goto LAB_00e91ee8
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e91ee8 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e91ccb end
                            end
                            goto LAB_00e91db4
                        end
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
                do
                    local scratchValue = resources:ScriptThing(resource)
                    local fret_03 = quest:GetHealth(scratchValue)
                    if 0.0 < fret_03 then
                        local hero11 = quest:GetHero()
                        if not me:Speak(hero11, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00e91ccb
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e91ccb end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e91ee8 end
                    end
                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_WIZARD_BATTLE", "Q_WizardBattle", false)
                    quest:TakeObjectFromHero("OBJECT_MAZE_JOURNAL")
                    goto LAB_00e91db4
                end
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
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
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

