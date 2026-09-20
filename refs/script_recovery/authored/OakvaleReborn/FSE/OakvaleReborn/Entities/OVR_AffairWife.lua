-- DISABLED Wife candidate; native phase/adapter proofs do not establish gameplay integration.
-- Generated native draft: NOVI_AffairWife. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("GoingForHusband", false)
    __native_entity_state:SetStateBool("ForceFirstTimeSpeak", true)
    quest:WithRetailResources(function(resources)
        resources:InitializeWifeActor(me)
        __native_entity_state:SetStateBool("SaidRunningLine", false)
        resources:SetWifeDeedReactionsDisabled(me)
    end)
end

local function __resource_main(quest, me, resources)
    local wife_resource, wife_thing, wife_movie
    local wifeLineCounter, wifeReplyRemainder, conversationId2, affairMan
    local alive = true
    quest:RegisterBoundConsciousCondition()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    wife_resource = resources:NewResource()
    local function waitUntilNearHusband()
        local outsideDistance, thingsWithinDistance, homePosition, conversationId
        while true do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end
            if not __native_entity_state:GetStateBool("SaidRunningLine") then

                homePosition = me:GetHomePos()
                outsideDistance = (me ~= nil and me:IsDistanceFromPositionOver(homePosition, (10.0)))
                if outsideDistance then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return false end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY", me, nil, false)
                    __native_entity_state:SetStateBool("SaidRunningLine", true)
                end
            end
            thingsWithinDistance = resources:ThingsAreWithinDistance(me, affairMan, 3.0)
            if thingsWithinDistance then break end
        end
        return true
    end
    local function processHeroInteraction()
        local taskRunning, taskRunning2, taskRunning3, taskRunning4, taskRunning5, taskRunning6
        local taskRunning7, taskRunning8, controlAcquired, talkedToByHero, controlAcquired2, health
        local health2, health3, health4, questionAnswer, wifeAllyHero, wifeHit, wifeReverseHero
        local hero, hero2, hero3, hero4, speechResult, speechResult2, speechResult3, speechResult4
        wifeHit = resources:IsHitByHeroExceptAbility(me, 14)
        if wifeHit then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end
            wifeAllyHero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, wifeAllyHero)
            wifeReverseHero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(wifeReverseHero, me)
            require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, 2)
            resources:PrepareResource(wife_resource)
            controlAcquired = resources:TryAcquire(wife_resource, me, 4)
            while not controlAcquired do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return false end
                controlAcquired = resources:TryAcquire(wife_resource, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end

            wife_movie = resources:StartMovie("")
            resources:Pause(true)
            wife_thing = resources:NewThingFromResource(wife_resource)
            health = resources:ThingHealth(wife_thing)
            resources:DestroyThing(wife_thing); wife_thing = nil

            if not (health > (0.0)) then
                -- LAB_00db2e83: (native jump target)
                resources:Pause(false)
                -- LAB_00db32a5: (native jump target)
                resources:DestroyMovie(wife_movie); wife_movie = nil
                return true
            end





            hero = quest:GetHero()
            speechResult = resources:Speak(wife_resource, hero, ("TEXT_QST_048_AFFAIR_WIFE_ON_HIT"), (0x0), false, true, false)
            taskRunning = resources:IsPerformingScriptTask(wife_resource)
            if taskRunning then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        resources:Pause(false)
                        resources:DestroyMovie(wife_movie); wife_movie = nil
                        return false
                    end
                    taskRunning2 = resources:IsPerformingScriptTask(wife_resource)
                until not (taskRunning2)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                resources:Pause(false)
                resources:DestroyMovie(wife_movie); wife_movie = nil
                return true
            end
            resources:Pause(false)
            resources:DestroyMovie(wife_movie); wife_movie = nil
            return false
        end
        talkedToByHero = me:IsTalkedToByHero()
        if talkedToByHero then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end
            wife_movie = resources:StartMovie("")
            resources:Pause(true)
            resources:PrepareResource(wife_resource)
            controlAcquired2 = resources:TryAcquire(wife_resource, me, 4)
            while not controlAcquired2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    resources:Pause(false)
                    resources:DestroyMovie(wife_movie); wife_movie = nil
                    return false
                end
                controlAcquired2 = resources:TryAcquire(wife_resource, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                wife_thing = resources:NewThingFromResource(wife_resource)
                health2 = resources:ThingHealth(wife_thing)
                resources:DestroyThing(wife_thing); wife_thing = nil

                if (0.0) < health2 then





                    hero4 = quest:GetHero()
                    speechResult2 = resources:Speak(wife_resource, hero4, ("TEXT_QST_048_AFFAIR_WIFE_LAYABOUT"), (0x0), false, true, (false))
                    taskRunning3 = resources:IsPerformingScriptTask(wife_resource)
                    if taskRunning3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(wife_movie); wife_movie = nil
                                return false
                            end
                            taskRunning4 = resources:IsPerformingScriptTask(wife_resource)
                        until not (taskRunning4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        resources:Pause(false)
                        resources:DestroyMovie(wife_movie); wife_movie = nil
                        return false
                    end
                end
                if not quest:GetStateBool("HeroDiscoveredInfidelity") then
                    resources:Pause(false)
                    resources:DestroyMovie(wife_movie); wife_movie = nil
                    return true
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    quest:GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIR_WIFE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(wife_movie); wife_movie = nil
                            return false
                        end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        resources:Pause(false)
                        resources:DestroyMovie(wife_movie); wife_movie = nil
                        return false
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if not alive then
                            -- LAB_00db3def: (native jump target)
                            resources:Pause(false)
                            resources:DestroyMovie(wife_movie); wife_movie = nil
                            return false
                        end
                        wife_thing = resources:NewThingFromResource(wife_resource)
                        health3 = resources:ThingHealth(wife_thing)
                        resources:DestroyThing(wife_thing); wife_thing = nil

                        if (0.0) < health3 then





                            hero2 = quest:GetHero()
                            speechResult3 = resources:Speak(wife_resource, hero2, ("TEXT_QST_048_AFFAIR_WIFE_THANKYOU"), (0x0), false, true, (false))
                            taskRunning5 = resources:IsPerformingScriptTask(wife_resource)
                            if taskRunning5 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(wife_movie); wife_movie = nil
                                        return false
                                    end
                                    taskRunning6 = resources:IsPerformingScriptTask(wife_resource)
                                until not (taskRunning6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(wife_movie); wife_movie = nil
                                return false
                            end
                        end
                        require("OakvaleReborn.native_quest_helpers").AddGoodDeed(quest, me)
                        __native_entity_state:SetStateBool("GoingForHusband", true)
                    else
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(wife_movie); wife_movie = nil
                            return false
                        end
                        wife_thing = resources:NewThingFromResource(wife_resource)
                        health4 = resources:ThingHealth(wife_thing)
                        resources:DestroyThing(wife_thing); wife_thing = nil

                        if (0.0) < health4 then





                            hero3 = quest:GetHero()
                            speechResult4 = resources:Speak(wife_resource, hero3, ("TEXT_QST_048_AFFAIR_WIFE_PLEA"), (0x0), false, true, (false))
                            taskRunning7 = resources:IsPerformingScriptTask(wife_resource)
                            if taskRunning7 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(wife_movie); wife_movie = nil
                                        return false
                                    end
                                    taskRunning8 = resources:IsPerformingScriptTask(wife_resource)
                                until not (taskRunning8)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(wife_movie); wife_movie = nil
                                return false
                            end
                        end
                    end
                    -- LAB_00db3293: (native jump target)
                    resources:Pause(false)
                    resources:DestroyMovie(wife_movie); wife_movie = nil
                    return true
                end
                -- LAB_00db3dff: (native jump target)
                resources:Pause(false)
                resources:DestroyMovie(wife_movie); wife_movie = nil
                return false
            end
            -- LAB_00db3de0: (native jump target)
            resources:Pause(false)
            resources:DestroyMovie(wife_movie); wife_movie = nil
            return false
        end
        return true
    end
    local function runBody()
        local thingsWithinDistance, isDistanceBetweenThingsUnder, isDistanceBetweenThingsUnder2
        local taskRunning, isHitByHeroExceptAbility, taskRunning2, taskRunning3
        local taskRunning4, taskRunning5, taskRunning6, predicateResult, controlAcquired
        local controlAcquired2, talkedToByHero, controlAcquired3, isConversationActive, health
        local health2, timeRemaining, randomChoice, sequence12, wifeAllyHero, wifeAnimationRemainder
        local wifeArgumentId, wifeParticipant, hero, scratchValue2, hero2, hero3, hero4
        local scratchValue3, speechResult, speechResult2, conversationId, hero5, hero6
    wifeArgumentId = 0
    alive = not quest:IsActiveThreadTerminating()
    predicateResult = not alive

    repeat
        if predicateResult then
            -- LAB_00db33f3: (native jump target)
            return
        end
        resources:PrepareResource(wife_resource)
        controlAcquired = resources:TryAcquire(wife_resource, me, 3)
        while not controlAcquired do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            controlAcquired = resources:TryAcquire(wife_resource, me, 3)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end
        if __native_entity_state:GetStateBool("GoingForHusband") then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end

            affairMan = resources:NewThingFromScriptName("NOVI_AffairMan")

            quest:EntitySetAsUseMovementInActions(me, true)
            scratchValue2 = resources:ThingPosition(affairMan)
            resources:MoveToPosition(wife_resource, scratchValue2, 2.0, 1, false, true)
            quest:ClearThingHasInformation(me)
            thingsWithinDistance = resources:ThingsAreWithinDistance(me, affairMan, 3.0)
            if not thingsWithinDistance then
                if not waitUntilNearHusband() then return end
            end
            break
        end
        if not processHeroInteraction() then return end
        timeRemaining = quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer"))

        if timeRemaining == 0 then
            sequence12 = true
        else
            sequence12 = false
        end
        if sequence12 then
            if __native_entity_state:GetStateBool("ForceFirstTimeSpeak") then
                sequence12 = true
            else
                sequence12 = false
            end
            if not sequence12 then
                randomChoice = quest:RetailRandModulo(500)
                if randomChoice == 0 then
                    sequence12 = true
                else
                    sequence12 = false
                end
            end
        end
        if sequence12 then

            hero = quest:GetHero()
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 5.0)
            if isDistanceBetweenThingsUnder then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end
                quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
                __native_entity_state:SetStateBool("ForceFirstTimeSpeak", false)
                resources:AddWifeWhereHusbandConversation(me)
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive

    until false
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00db3d90: (native jump target)
        return
    end
    resources:ClearCommands(wife_resource)

    wifeLineCounter = 0
    quest:EntitySetAsUseMovementInActions(me, (wifeLineCounter ~= 0))
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00db3d6a: (native jump target)
        return
    end
    repeat
        hero2 = quest:GetHero()
        isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(me, hero2, 15.0)
        if isDistanceBetweenThingsUnder2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end
            wifeAnimationRemainder = quest:RetailRandModulo(2)
            if wifeAnimationRemainder == 0 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end
                resources:PlayWifeArgumentAnimation(wife_resource, "ST_ARGUING_POINT_AWAY")
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end
                resources:PlayWifeArgumentAnimation(wife_resource, "ST_ARGUING_POINT_AT")
            end
            taskRunning = resources:IsPerformingScriptTask(wife_resource)
            if taskRunning then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end
                    resources:FaceThing(me, affairMan, false)
                    isHitByHeroExceptAbility = resources:IsHitByHeroExceptAbility(me, 14)
                    if isHitByHeroExceptAbility then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end
                        wifeAllyHero = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(me, wifeAllyHero)
                        hero5 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(hero5, me)
                        require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, 2)
                        resources:PrepareResource(wife_resource)
                        controlAcquired2 = resources:TryAcquire(wife_resource, me, 4)
                        while not controlAcquired2 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end
                            controlAcquired2 = resources:TryAcquire(wife_resource, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end

                        wife_movie = resources:StartMovie("")
                        resources:Pause(true)
                        wife_thing = resources:NewThingFromResource(wife_resource)
                        health = resources:ThingHealth(wife_thing)
                        resources:DestroyThing(wife_thing); wife_thing = nil

                        if (0.0) < health then





                            hero3 = quest:GetHero()
                            speechResult = resources:Speak(wife_resource, hero3, ("TEXT_QST_048_AFFAIR_WIFE_ON_HIT"), (0x0), false, true, (false))
                            taskRunning2 = resources:IsPerformingScriptTask(wife_resource)
                            if taskRunning2 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(wife_movie); wife_movie = nil
                                        return
                                    end
                                    taskRunning3 = resources:IsPerformingScriptTask(wife_resource)
                                until not (taskRunning3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(wife_movie); wife_movie = nil
                                return
                            end
                        end
                        resources:FaceThing(me, affairMan, true)
                        resources:Pause(false)
                        resources:DestroyMovie(wife_movie); wife_movie = nil
                    end
                    talkedToByHero = me:IsTalkedToByHero()
                    if talkedToByHero then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end
                        resources:PrepareResource(wife_resource)
                        controlAcquired3 = resources:TryAcquire(wife_resource, me, 4)
                        while not controlAcquired3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end
                            controlAcquired3 = resources:TryAcquire(wife_resource, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end
                        resources:ClearAllActions(wife_resource)
                        resources:ClearCommands(wife_resource)
                        wife_movie = resources:StartMovie("")
                        resources:Pause(true)
                        wife_thing = resources:NewThingFromResource(wife_resource)
                        health2 = resources:ThingHealth(wife_thing)
                        resources:DestroyThing(wife_thing); wife_thing = nil

                        if (0.0) < health2 then





                            hero4 = quest:GetHero()
                            speechResult2 = resources:Speak(wife_resource, hero4, ("TEXT_QST_048_AFFAIR_WIFE_THANKYOU_SINGLE"), (0x0), false, true, (false))
                            taskRunning4 = resources:IsPerformingScriptTask(wife_resource)
                            if taskRunning4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(wife_movie); wife_movie = nil
                                        return
                                    end
                                    taskRunning5 = resources:IsPerformingScriptTask(wife_resource)
                                until not (taskRunning5)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                -- LAB_00db3d61: (native jump target)
                                resources:DestroyMovie(wife_movie); wife_movie = nil
                                return
                            end
                        end
                        resources:FaceThing(me, affairMan, true)
                        resources:Pause(false)
                        resources:DestroyMovie(wife_movie); wife_movie = nil
                    end
                    isConversationActive = quest:IsConversationActive(wifeArgumentId)
                    if not isConversationActive then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        scratchValue3 = conversationId2
                        wifeArgumentId = conversationId2
                        resources:AddConversationPerson(conversationId2, affairMan)
                        wifeLineCounter = wifeLineCounter + 10
                        local continueArgument = resources:WithArgumentKey(wifeLineCounter, function(argumentKey)
                            if not argumentKey:Exists() then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                wifeLineCounter = 10
                                argumentKey:ResetToFirst()
                            end
                            resources:AddArgumentKeyLine(argumentKey, conversationId2, me, affairMan)
                            wifeReplyRemainder = quest:RetailRandModulo(2)
                            if wifeReplyRemainder == 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                resources:AddConversationLine(conversationId2, "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE", affairMan, me, false)
                            end
                            return true
                        end)
                        if not continueArgument then return end
                    end
                    taskRunning6 = resources:IsPerformingScriptTask(wife_resource)
                until not (taskRunning6)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end
        end

        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
    until false
    -- LAB_00db3dd9: (native jump target)
    -- LAB_00db3e05: (native jump target)
    resources:Pause(false)
    -- LAB_00db3e0d: (native jump target)
    -- LAB_00db3e11: (native jump target)
    resources:DestroyMovie(wife_movie); wife_movie = nil
    end
    runBody()
    assert(wife_movie == nil, "wife movie cleanup was bypassed")
    if affairMan ~= nil then resources:DestroyThing(affairMan); affairMan = nil end
    resources:ReleaseResource(wife_resource); wife_resource = nil
end


function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
