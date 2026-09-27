-- Readable native conversion: GhostFisherman. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local ghostGoing, haveTalked

-- GhostFisherman.Main (retail 0x00d8abf0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local questionAnswer, questionAnswer2, thing
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local hiddenBooty = quest:GetThingWithScriptName("HiddenBooty")
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    if not quest:GetStateBool("WifeAttacked") then
        while not ghostGoing do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
            if not haveTalked then
                if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    local conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_032_GHOST_FISHERMAN_HELP", me, hero, false)
                    quest:SetTimer(timerId, 15)
                end
            end
            if not quest:GetStateBool("Helping") and not quest:IsDiggingSpotEnabled(hiddenBooty) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                quest:SetStateBool("MissionAborted", true)
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                local resource = resources:NewResource()
                thing = hero
                resources:TryAcquire(resource, hero, 4)
                local actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "HERO", resource)
                resources:SetActor(actorMap, "GHOST", resource2)
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_SETUP", actorMap, false, false)
                if not haveTalked then
                    if not quest:IsActiveThreadTerminating() then
                        haveTalked = true
                        resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_1", actorMap, false, true)
                        quest:GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:DestroyActorMap(actorMap)
                                resources:ReleaseResource(resource)
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(resource2)
                                do return end
                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local predicateResult8 = quest:IsActiveThreadTerminating()
                            if questionAnswer == 1 then
                                if predicateResult8 then goto LAB_00d8bc59 end
                                quest:SetStateBool("Helping", true)
                                resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_2", actorMap, false, true)
                                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", quest:GetActiveQuestName(), false)
                                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay")
                                quest:MiniMapRemoveMarker(me)
                                thing = quest:GetThingWithScriptName("FishermansWife")
                                quest:MiniMapAddMarker(thing, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if predicateResult8 then goto LAB_00d8bb8e end
                                resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_REFUSAL", actorMap, false, true)
                            end
                            goto LAB_00d8bac9
                        end
                    end
                    goto LAB_00d8bb8e
                else
                    if not quest:IsActiveThreadTerminating() then
                        if quest:GetStateBool("Helping") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d8bb8e end
                            if quest:IsDiggingSpotEnabled(hiddenBooty) then
                                thing = resources:ScriptThing(resource2)
                                local fret_0 = quest:GetHealth(thing)
                                if 0.0 < fret_0 then
                                    thing = hero
                                    me:Speak(hero, "TEXT_QST_032_GHOST_FISHERMAN_WHERE", GROUP_SELECT_FIRST, false, true, false)
                                    while me:IsPerformingScriptTask() do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie)
                                            resources:DestroyActorMap(actorMap)
                                            resources:ReleaseResource(resource)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(resource2)
                                            do return end
                                        end
                                    end
                                    goto LAB_00d8bab8
                                end
                                goto FLOW_hoist_lab_00d8bab8_1
                            end
                            goto FLOW_hoist_lab_00d8bab8_2
                        end
                        goto FLOW_hoist_lab_00d8bab8_3
                    end
                    goto FLOW_hoist_lab_00d8bab8_4
                end
                goto FLOW_past_lab_00d8bab8
                ::LAB_00d8bab8::
                if quest:IsActiveThreadTerminating() then goto LAB_00d8bc59 end
                ::FLOW_hoist_lab_00d8bab8_1::
                goto LAB_00d8bac9
                ::FLOW_hoist_lab_00d8bab8_2::
                if quest:GetStateBool("Helped") then
                    resources:RunMacro("CS_GHOSTFISH_FISH_SUCCESS", actorMap, false, true)
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_04", "", "OakBay")
                    ghostGoing = true
                    goto LAB_00d8bac9
                end
                thing = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(thing) then
                    thing = hero
                    if not me:Speak(hero, "TEXT_QST_032_GHOST_FISHERMAN_COMPLAINT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d8bc59 end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d8bc64 end
                end
                goto LAB_00d8bac9
                ::FLOW_hoist_lab_00d8bab8_3::
                goto FLOW_hoist_lab_00d8bac9_1
                ::FLOW_hoist_lab_00d8bab8_4::
                goto FLOW_hoist_lab_00d8bac9_2
                ::FLOW_past_lab_00d8bab8::
                goto FLOW_past_lab_00d8bb8e
                ::LAB_00d8bb8e::
                quest:PauseAllNonScriptedEntities(false)
                ::FLOW_past_lab_00d8bb8e::
                goto FLOW_past_lab_00d8bac9
                ::LAB_00d8bac9::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource)
                goto LAB_00d8bb7b
                ::FLOW_hoist_lab_00d8bac9_1::
                if not quest:IsActiveThreadTerminating() then
                    quest:GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer2 < 0 do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        else
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource2)
                            do return end
                            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult = quest:IsActiveThreadTerminating()
                        if questionAnswer2 == 1 then
                            if predicateResult then goto LAB_00d8bb8e end
                            quest:SetStateBool("Helping", true)
                            resources:RunMacro("CS_GHOSTFISH_FISH_INTRO_2", actorMap, false, true)
                            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", quest:GetActiveQuestName(), false)
                            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay")
                            quest:MiniMapRemoveMarker(me)
                            thing = quest:GetThingWithScriptName("FishermansWife")
                            quest:MiniMapAddMarker(thing, "HUD_ORB_QUEST_VIGNETTE")
                        else
                            if predicateResult then goto LAB_00d8bc59 end
                            thing = resources:ScriptThing(resource2)
                            local fret_01 = quest:GetHealth(thing)
                            if 0.0 < fret_01 then
                                thing = hero
                                if not me:Speak(hero, "TEXT_QST_032_GHOST_FISHERMAN_REFUSAL", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d8bc59 end
                                goto LAB_00d8bab8
                            end
                        end
                        goto LAB_00d8bac9
                    end
                end
                ::FLOW_hoist_lab_00d8bac9_2::
                goto LAB_00d8bc59
                ::FLOW_past_lab_00d8bac9::
                goto FLOW_past_lab_00d8bc59
                ::LAB_00d8bc59::
                quest:PauseAllNonScriptedEntities(false)
                ::FLOW_past_lab_00d8bc59::
                ::LAB_00d8bc64::
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource2)
                return
            end
            ::LAB_00d8bb7b::
            if quest:GetStateBool("WifeAttacked") then break end
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    if not quest:GetStateBool("WifeAttacked") then
        if not quest:IsActiveThreadTerminating() then
            quest:RemoveThing(me, false, true)
        end
    else
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
        quest:CreateEffectAtPos("Ghost_Appear_01", thing:GetPos(), 0.0, false)
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource2)
end

-- GhostFisherman.Init (retail 0x00d8a560)
function Init(quest, me)
    haveTalked = false
    ghostGoing = false
    quest:EntitySetAsAbleToWalkThroughSolidObjects(me, true)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsThingForcePushable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetTargetingType(me, 2)
    quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_INANIMATE_EVIL_HIGH")
    quest:SetThingHasInformation(me, false, true, false)
    if not quest:GetStateBool("Helping") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
end

-- GhostFisherman.OnPersist (retail 0x00d8ab70)
function OnPersist(quest, me, context)
    quest:SetStateBool("HaveTalked", quest:PersistTransferBool(context, "HaveTalked", quest:GetStateBool("HaveTalked")))
end

-- GhostFisherman.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

