-- DISABLED CANDIDATE: resource-aware NOVI_AffairMan generated from the converter draft
-- plus the verified native resource map (native_affair_man_resources_witness.json).
-- Not registered, not installed. Requires the unapplied LuaRetailResources extension:
-- MoveToPosition, ClearCommands, ClearAllActions, ThingIsDistanceFromPositionOver, IsHitByHeroExceptAbility, ThingsAreWithinDistance, FaceThing, AddConversationPerson, AddConversationLine, InitializeAffairManActor, PlayAffairManAnimation.
-- One retail resource local (man_resource) spans Main: constructed after the entry
-- termination check, prepared/acquired at the three native sites, used by every
-- speech, task query, animation, move and clear call. Normal exits release at the
-- native join; errors use scope Close, including movie destruction and unpause.
-- Speech is the native non-waiting wrapper followed by the retail task poll.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("BadgerIndex", 0)
    __native_entity_state:SetStateBool("EncounterOver", false)
    __native_entity_state:SetStateBool("SaidFirstRangedComment", false)
    __native_entity_state:SetStateBool("HeroAgreedToKeepQuiet", false)
    __native_entity_state:SetStateBool("HeroSaidHeWouldReportMan", false)
    quest:WithRetailResources(function(resources)
        resources:InitializeAffairManActor(me)
    end)
end

local function __resource_main(quest, me, resources)
    local man_resource, man_thing, man_movie, taskRunning, taskRunning2, thingsWithinDistance
    local taskRunning3, taskRunning4, taskRunning5, taskRunning6, taskRunning7, taskRunning8
    local taskRunning9, taskRunning10, taskRunning11, taskRunning12, taskRunning13, taskRunning14
    local taskRunning15, taskRunning16, thingAlive, thingAlive2, taskRunning17
    local thingsWithinDistance2, outsideDistance, outsideDistance2, taskRunning18, taskRunning19
    local outsideDistance3, thingsWithinDistance3, isDistanceBetweenThingsUnder
    local isDistanceBetweenThingsUnder2, controlAcquired, controlAcquired2, talkedToByHero
    local controlAcquired3, isConversationActive, health, health2, health3, health4, health5
    local health6, health7, health8, questionAnswer, randomChoice, randomChoice2, badgerIndex
    local badgerIndex2, manConversationId, manHit, manHomePosition, manLineIndex
    local manLineTerminating1, manLineTerminating2, manNearKey, manRangedIndex, manRangedKey
    local manTalkTerminating, sequence12, womanNearKey, womanRangedKey, homePosition, hero, hero2
    local hero3, hero4, hero5, hero6, hero7, hero8, hero9, hero10, affairWoman, affairWife
    local randomChoice3, conversationId, conversationId2, hero11
    local alive = true
    quest:RegisterBoundConsciousCondition()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        man_resource = resources:NewResource()
        local function acquireAndRunInteractions()
            resources:PrepareResource(man_resource)
            controlAcquired = resources:TryAcquire(man_resource, me, 4)
            while not controlAcquired do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end
                controlAcquired = resources:TryAcquire(man_resource, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                affairWoman = resources:NewThingFromScriptName("NOVI_AffairWoman")
                affairWife = resources:NewThingFromScriptName("NOVI_AffairWife")
                local function runInteractions()
                    manConversationId = 0
                    alive = not quest:IsActiveThreadTerminating()

                    if alive then
                        local function finishConversationMovie()
                            resources:Pause(false)
                            resources:DestroyMovie(man_movie)
                            return true
                        end
                        local function facePartnerAndFinishConversation()
                            thingAlive = resources:ThingAlive(affairWoman)
                            if thingAlive then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return false
                                end
                                resources:FaceThing(affairWoman, me, false)
                            else
                                thingAlive2 = resources:ThingAlive(affairWife)
                                if thingAlive2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        resources:FaceThing(affairWife, me, false)
                                        return finishConversationMovie()
                                    end
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return false
                                end
                            end
                            return finishConversationMovie()
                        end
                        local function processInteraction()
                            manHit = resources:IsHitByHeroExceptAbility(me, 14)
                            if manHit then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end

                                man_movie = resources:StartMovie("")
                                resources:Pause(true)
                                resources:PrepareResource(man_resource)
                                controlAcquired2 = resources:TryAcquire(man_resource, me, 4)
                                while not controlAcquired2 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        return false
                                    end
                                    controlAcquired2 = resources:TryAcquire(man_resource, me, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    man_thing = resources:NewThingFromResource(man_resource)
                                    health = resources:ThingHealth(man_thing)
                                    resources:DestroyThing(man_thing)

                                    if (0.0) < health then





                                        hero = quest:GetHero()
                                        resources:Speak(man_resource, hero, ("TEXT_QST_048_AFFAIRMAN_ON_HIT"), (0x0), false, true, (false))
                                        taskRunning = resources:IsPerformingScriptTask(man_resource)
                                        if taskRunning then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    return false
                                                end
                                                taskRunning2 = resources:IsPerformingScriptTask(man_resource)
                                            until not (taskRunning2)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(man_movie)
                                            return false
                                        end
                                    end
                                    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return true
                                end
                                resources:Pause(false)
                                resources:DestroyMovie(man_movie)
                                return false
                            end
                            talkedToByHero = me:IsTalkedToByHero()
                            if talkedToByHero then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end

                                man_movie = resources:StartMovie("")

                                resources:Pause(true)
                                resources:PrepareResource(man_resource)
                                controlAcquired3 = resources:TryAcquire(man_resource, me, 4)
                                while not controlAcquired3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        return false
                                    end
                                    controlAcquired3 = resources:TryAcquire(man_resource, me, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return false
                                end
                                resources:ClearAllActions(man_resource)
                                resources:ClearCommands(man_resource)
                                thingsWithinDistance = resources:ThingsAreWithinDistance(me, affairWife, 5.0)
                                if thingsWithinDistance then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        return false
                                    end
                                    if not __native_entity_state:GetStateBool("HeroAgreedToKeepQuiet") then
                                        if __native_entity_state:GetStateBool("HeroSaidHeWouldReportMan") then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if alive then
                                                man_thing = resources:NewThingFromResource(man_resource)
                                                health2 = resources:ThingHealth(man_thing)
                                                resources:DestroyThing(man_thing)

                                                if (0.0) < health2 then





                                                    hero2 = quest:GetHero()
                                                    resources:Speak(man_resource, hero2, ("TEXT_QST_048_AFFAIRMAN_SOME_NERVE"), (0x0), false, true, (false))
                                                    taskRunning3 = resources:IsPerformingScriptTask(man_resource)
                                                    if taskRunning3 then
                                                        repeat
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then
                                                                resources:Pause(false)
                                                                resources:DestroyMovie(man_movie)
                                                                return false
                                                            end
                                                            taskRunning4 = resources:IsPerformingScriptTask(man_resource)
                                                        until not (taskRunning4)
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then
                                                        resources:Pause(false)
                                                        resources:DestroyMovie(man_movie)
                                                        return false
                                                    end
                                                end
                                                quest:ClearThingHasInformation(me)
                                                return facePartnerAndFinishConversation()
                                            end
                                            -- LAB_00db1085: (native jump target)
                                            resources:Pause(false)
                                            resources:DestroyMovie(man_movie)
                                            return false
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            man_thing = resources:NewThingFromResource(man_resource)
                                            health3 = resources:ThingHealth(man_thing)
                                            resources:DestroyThing(man_thing)

                                            if (0.0) < health3 then





                                                hero3 = quest:GetHero()
                                                resources:Speak(man_resource, hero3, ("TEXT_QST_048_AFFAIRMAN_HOW_FIND_OUT"), (0x0), false, true, (false))
                                                taskRunning5 = resources:IsPerformingScriptTask(man_resource)
                                                if taskRunning5 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then
                                                            resources:Pause(false)
                                                            resources:DestroyMovie(man_movie)
                                                            return false
                                                        end
                                                        taskRunning6 = resources:IsPerformingScriptTask(man_resource)
                                                    until not (taskRunning6)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                manTalkTerminating = not alive
                                                if manTalkTerminating then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    return false
                                                end
                                                quest:ClearThingHasInformation(me)
                                                return facePartnerAndFinishConversation()
                                            end
                                            quest:ClearThingHasInformation(me)
                                            return facePartnerAndFinishConversation()
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            man_thing = resources:NewThingFromResource(man_resource)
                                            health4 = resources:ThingHealth(man_thing)
                                            resources:DestroyThing(man_thing)

                                            if (0.0) < health4 then





                                                hero4 = quest:GetHero()
                                                resources:Speak(man_resource, hero4, ("TEXT_QST_048_AFFAIRMAN_HAD_A_DEAL"), (0x0), false, true, (false))
                                                taskRunning7 = resources:IsPerformingScriptTask(man_resource)
                                                if taskRunning7 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then
                                                            resources:Pause(false)
                                                            resources:DestroyMovie(man_movie)
                                                            return false
                                                        end
                                                        taskRunning8 = resources:IsPerformingScriptTask(man_resource)
                                                    until not (taskRunning8)
                                                end
                                                -- LAB_00db1144: (native jump target)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    return false
                                                end
                                            end
                                            -- LAB_00db1153: (native jump target)
                                            quest:ClearThingHasInformation(me)
                                            return facePartnerAndFinishConversation()
                                        end
                                    end
                                    -- LAB_00db1cf3: (native jump target)
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return false
                                end
                                if not __native_entity_state:GetStateBool("EncounterOver") then
                                    alive = not quest:IsActiveThreadTerminating()

                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        return false
                                    end
                                    hero11 = quest:GetHero()
                                    resources:FaceThing(affairWoman, hero11, false)
                                    man_thing = resources:NewThingFromResource(man_resource)
                                    health5 = resources:ThingHealth(man_thing)
                                    resources:DestroyThing(man_thing)

                                    if (0.0) < health5 then





                                        hero5 = quest:GetHero()
                                        resources:Speak(man_resource, hero5, ("TEXT_QST_048_AFFAIRMAN_INTRO"), (0x0), false, true, (false))
                                        taskRunning9 = resources:IsPerformingScriptTask(man_resource)
                                        if taskRunning9 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    return false
                                                end
                                                taskRunning10 = resources:IsPerformingScriptTask(man_resource)
                                            until not (taskRunning10)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(man_movie)
                                            return false
                                        end
                                    end
                                    quest:SetStateBool("HeroDiscoveredInfidelity", true)

                                    quest:GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()

                                    while questionAnswer < 0 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(man_movie)
                                            return false
                                        end
                                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()

                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if questionAnswer == 1 then
                                            if not alive then
                                                -- LAB_00db1d23: (native jump target)
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                return false
                                            end
                                            man_thing = resources:NewThingFromResource(man_resource)
                                            health6 = resources:ThingHealth(man_thing)
                                            resources:DestroyThing(man_thing)

                                            if (0.0) < health6 then





                                                hero6 = quest:GetHero()
                                                resources:Speak(man_resource, hero6, ("TEXT_QST_048_AFFAIRMAN_GOOD_LAD"), (0x0), false, true, (false))
                                                taskRunning11 = resources:IsPerformingScriptTask(man_resource)
                                                if taskRunning11 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then
                                                            resources:Pause(false)
                                                            resources:DestroyMovie(man_movie)
                                                            return false
                                                        end
                                                        taskRunning12 = resources:IsPerformingScriptTask(man_resource)
                                                    until not (taskRunning12)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    return false
                                                end
                                            end
                                            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 4)
                                            quest:GiveHeroGold(1)
                                            __native_entity_state:SetStateBool("HeroAgreedToKeepQuiet", true)
                                        else
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                return false
                                            end
                                            man_thing = resources:NewThingFromResource(man_resource)
                                            health7 = resources:ThingHealth(man_thing)
                                            resources:DestroyThing(man_thing)

                                            if (0.0) < health7 then





                                                hero7 = quest:GetHero()
                                                resources:Speak(man_resource, hero7, ("TEXT_QST_048_AFFAIRMAN_DONT_TELL"), (0x0), false, true, (false))
                                                taskRunning13 = resources:IsPerformingScriptTask(man_resource)
                                                if taskRunning13 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then
                                                            resources:Pause(false)
                                                            resources:DestroyMovie(man_movie)
                                                            return false
                                                        end
                                                        taskRunning14 = resources:IsPerformingScriptTask(man_resource)
                                                    until not (taskRunning14)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(man_movie)
                                                    return false
                                                end
                                            end
                                            __native_entity_state:SetStateBool("HeroSaidHeWouldReportMan", true)
                                        end
                                        quest:ClearThingHasInformation(me)
                                        __native_entity_state:SetStateBool("EncounterOver", true)
                                        return facePartnerAndFinishConversation()
                                    end
                                    -- LAB_00db1d0a: (native jump target)
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return false
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(man_movie)
                                    return false
                                end
                                man_thing = resources:NewThingFromResource(man_resource)
                                health8 = resources:ThingHealth(man_thing)
                                resources:DestroyThing(man_thing)

                                if (0.0) < health8 then





                                    hero8 = quest:GetHero()
                                    resources:Speak(man_resource, hero8, ("TEXT_QST_048_AFFAIRMAN_SHOO"), (0x0), false, true, (false))
                                    taskRunning15 = resources:IsPerformingScriptTask(man_resource)
                                    if taskRunning15 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(man_movie)
                                                return false
                                            end
                                            taskRunning16 = resources:IsPerformingScriptTask(man_resource)
                                        until not (taskRunning16)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(man_movie)
                                        return false
                                    end
                                end
                                return facePartnerAndFinishConversation()
                            end
                            taskRunning17 = resources:IsPerformingScriptTask(man_resource)
                            if taskRunning17 then return true end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return false end
                            thingsWithinDistance2 = resources:ThingsAreWithinDistance(me, affairWife, 5.0)
                            if thingsWithinDistance2 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                resources:FaceThing(me, affairWife, false)
                                randomChoice = quest:RetailRandModulo(50)
                                if randomChoice ~= 0 then return true end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                resources:PlayAffairManAnimation(man_resource, "ST_OPINION_FEAR_IDLE_COWERING", false)
                            else

                                homePosition = me:GetHomePos()
                                outsideDistance = (me ~= nil and me:IsDistanceFromPositionOver(homePosition, (0.1)))
                                if outsideDistance then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return false end
                                    manHomePosition = me:GetHomePos()

                                    man_thing = resources:NewThingFromResource(man_resource)
                                    outsideDistance2 = resources:ThingIsDistanceFromPositionOver(man_thing, manHomePosition, 2.0)
                                    resources:DestroyThing(man_thing)
                                    if outsideDistance2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return false end
                                            resources:MoveToPosition(man_resource, manHomePosition, 0.0, 0, false, true)
                                            taskRunning18 = resources:IsPerformingScriptTask(man_resource)
                                            if taskRunning18 then
                                                repeat
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then return false end
                                                    taskRunning19 = resources:IsPerformingScriptTask(man_resource)
                                                until not (taskRunning19)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return false end

                                            man_thing = resources:NewThingFromResource(man_resource)
                                            outsideDistance3 = resources:ThingIsDistanceFromPositionOver(man_thing, manHomePosition, 2.0)
                                            resources:DestroyThing(man_thing)
                                        until not (outsideDistance3)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return false end
                                    return true
                                end
                                isConversationActive = quest:IsConversationActive(manConversationId)

                                if isConversationActive then
                                    sequence12 = true
                                else
                                    sequence12 = false
                                end
                                if not sequence12 then
                                    if __native_entity_state:GetStateBool("SaidFirstRangedComment") then
                                        sequence12 = true
                                    else
                                        sequence12 = false
                                    end
                                    if sequence12 then
                                        randomChoice2 = quest:RetailRandModulo(100)
                                        if randomChoice2 ~= 0 then
                                            sequence12 = true
                                        else
                                            sequence12 = false
                                        end
                                    end
                                    if not sequence12 then
                                        thingsWithinDistance3 = resources:ThingsAreWithinDistance(me, affairWoman, 2.0)
                                        if not thingsWithinDistance3 then
                                            sequence12 = true
                                        else
                                            sequence12 = false
                                        end
                                    end
                                end
                                if sequence12 then return true end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end

                                hero9 = quest:GetHero()
                                isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero9, 5.0)
                                if isDistanceBetweenThingsUnder then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return false end
                                    conversationId = quest:AddNewConversation(me, false, false)
                                    manConversationId = conversationId
                                    resources:AddConversationPerson(conversationId, affairWoman)
                                    badgerIndex = __native_entity_state:GetStateInt("BadgerIndex")
                                    __native_entity_state:SetStateInt("BadgerIndex", badgerIndex + 10)
                                    if 0x32 < badgerIndex + 10 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return false end
                                        __native_entity_state:SetStateInt("BadgerIndex", 10)
                                    end
                                    manLineIndex = __native_entity_state:GetStateInt("BadgerIndex")
                                    manNearKey = "TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_" .. tostring(manLineIndex)
                                    womanNearKey = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_" .. tostring(manLineIndex)
                                    if manLineIndex == 10 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        manLineTerminating1 = not alive
                                        if manLineTerminating1 then return false end
                                        resources:AddConversationLine(conversationId, womanNearKey, affairWoman, me, false)
                                        resources:AddConversationLine(conversationId, manNearKey, me, affairWoman, false)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        manLineTerminating2 = not alive
                                        if manLineTerminating2 then return false end
                                        resources:AddConversationLine(conversationId, manNearKey, me, affairWoman, false)
                                        resources:AddConversationLine(conversationId, womanNearKey, affairWoman, me, false)
                                    end
                                    quest:SetStateBool("HeroDiscoveredInfidelity", true)
                                else

                                    hero10 = quest:GetHero()
                                    isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(me, hero10, 13.0)
                                    if isDistanceBetweenThingsUnder2 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return false end
                                        __native_entity_state:SetStateBool("SaidFirstRangedComment", true)
                                        conversationId2 = quest:AddNewConversation(me, false, false)
                                        manConversationId = conversationId2
                                        resources:AddConversationPerson(conversationId2, affairWoman)
                                        badgerIndex2 = __native_entity_state:GetStateInt("BadgerIndex")
                                        __native_entity_state:SetStateInt("BadgerIndex", badgerIndex2 + 10)
                                        if 0x32 < badgerIndex2 + 10 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return false end
                                            __native_entity_state:SetStateInt("BadgerIndex", 10)
                                        end
                                        manRangedIndex = __native_entity_state:GetStateInt("BadgerIndex")
                                        manRangedKey = "TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_" .. tostring(manRangedIndex)
                                        womanRangedKey = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_" .. tostring(manRangedIndex)
                                        resources:AddConversationLine(conversationId2, manRangedKey, me, affairWoman, false)
                                        resources:AddConversationLine(conversationId2, womanRangedKey, affairWoman, me, false)
                                    end
                                end
                                if quest:GetStateBool("TalkingToWoman") then return true end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                quest:Pause(0.4000000059604645)
                                resources:FaceThing(me, affairWoman, true)
                                resources:FaceThing(affairWoman, me, true)
                                randomChoice3 = quest:RetailRandModulo(2)
                                if randomChoice3 == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        quest:SetStateBool("ReceiveKiss", true)
                                        resources:PlayAffairManAnimation(man_resource, "GIVE_KISS", true)
                                        return true
                                    end
                                    return false
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                quest:SetStateBool("ReceiveHug", true)
                                resources:PlayAffairManAnimation(man_resource, "GIVE_HUG", true)
                            end
                            return true
                        end
                        while true do
                            if not processInteraction() then return end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end
                        end
                    end
                end
                runInteractions()
                resources:DestroyThing(affairWife)
                resources:DestroyThing(affairWoman)
            end
        end
        acquireAndRunInteractions()
        resources:ReleaseResource(man_resource)
    end
end

function Main(quest, me)
    -- Normal exits release at DB1D92's join; Close is a fallback for Lua errors.
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
