-- DISABLED resource-aware BookTrader candidate; see candidate report for remaining gaps.
-- Generated native draft: NOVI_BookTrader. Review coverage report before use.
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
    __native_entity_state:SetStateBool("DoneIntro", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

local function __resource_main(quest, me, resources)
    local book_resource, book_thing, book_movie
    local function release_book()
        assert(book_movie == nil and book_thing == nil, "BookTrader cleanup order changed")
        resources:ReleaseResource(book_resource)
        book_resource = nil
    end
    local outsideDistance, outsideDistance2, taskRunning, taskRunning2, isHitByHeroExceptAbility
    local taskRunning3, taskRunning4, taskRunning5, taskRunning6, taskRunning7, taskRunning8
    local taskRunning9, taskRunning10, taskRunning11, taskRunning12, taskRunning13, taskRunning14
    local taskRunning15, taskRunning16, isDistanceBetweenThingsUnder, predicateResult
    local controlAcquired, controlAcquired2, talkedToByHero, controlAcquired3, controlAcquired4
    local health, health2, health3, health4, health5, health6, health7, questionAnswer, getHeroGold
    local bookAlly, bookHasHealth1, bookHasHealth2, bookHasHealth3, bookHasHealth4, bookHasHealth5
    local bookHasHealth6, bookHasHealth7, bookHome, bookInitialHome, hero, hero2, hero3, hero4
    local hero5, hero6, hero7, hero8, getActiveQuestName, speechResult, speechResult2, speechResult3
    local speechResult4, speechResult5, speechResult6, speechResult7, hero9, conversationId
    local alive = true
    quest:RegisterBoundConsciousCondition()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        book_resource = resources:NewResource()
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive

        local function prepareAndReturnHome()
            if predicateResult then
                return false
            end
            resources:PrepareResource(book_resource)

            controlAcquired = resources:TryAcquire(book_resource, me, 3)
            while not controlAcquired do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return false
                end
                controlAcquired = resources:TryAcquire(book_resource, me, 3)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end

            bookInitialHome = me:GetHomePos()
            outsideDistance = (me ~= nil and me:IsDistanceFromPositionOver(bookInitialHome, (0.1)))
            if outsideDistance then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    bookHome = me:GetHomePos()
                    while true do

                        book_thing = resources:NewThingFromResource(book_resource)
                        outsideDistance2 = resources:ThingIsDistanceFromPositionOver(book_thing, bookHome, (2.0))
                        resources:DestroyThing(book_thing); book_thing = nil
                        if not outsideDistance2 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return false end
                        resources:MoveToPosition(book_resource, bookHome, 0.0, 0, false, true)
                        taskRunning = resources:IsPerformingScriptTask(book_resource)
                        if taskRunning then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return false end
                                taskRunning2 = resources:IsPerformingScriptTask(book_resource)
                            until not (taskRunning2)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return false
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:FaceThingByScriptName(me, "NOVI_Theresa", false)
                        return true
                    end
                end
                return false
            end
            return true
        end
        local function finishTrade()
            quest:FaceThingByScriptName(me, "NOVI_Theresa", false)
            resources:Pause(false)
            resources:DestroyMovie(book_movie); book_movie = nil
            return true
        end
        local function processInteraction()
            isHitByHeroExceptAbility = resources:IsHitByHeroExceptAbility(me, 14)
            if isHitByHeroExceptAbility then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    bookAlly = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, bookAlly)
                    hero9 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(hero9, me)
                    require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, 2)
                    resources:PrepareResource(book_resource)
                    controlAcquired2 = resources:TryAcquire(book_resource, me, 4)
                    while not controlAcquired2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return false end
                        controlAcquired2 = resources:TryAcquire(book_resource, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then

                        book_movie = resources:StartMovie("")
                        resources:Pause(true)
                        book_thing = resources:NewThingFromResource(book_resource)
                        health = resources:ThingHealth(book_thing)
                        resources:DestroyThing(book_thing); book_thing = nil
                        bookHasHealth1 = health > 0.0
                        if bookHasHealth1 then





                            hero = quest:GetHero()
                            speechResult = resources:Speak(book_resource, hero, ("TEXT_QST_048_TRADER_ON_HIT"), (0x0), false, true, (false))
                            taskRunning3 = resources:IsPerformingScriptTask(book_resource)
                            if taskRunning3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        return false
                                    end
                                    taskRunning4 = resources:IsPerformingScriptTask(book_resource)
                                until not (taskRunning4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                -- LAB_00db4f51: (native jump target)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                return false
                            end
                        end
                        quest:FaceThingByScriptName(me, "NOVI_Theresa", true)
                        resources:Pause(false)
                        -- LAB_00db4ce1: (native jump target)
                        resources:DestroyMovie(book_movie); book_movie = nil
                        return true
                    end
                end
                return false
            end
            talkedToByHero = me:IsTalkedToByHero()
            if talkedToByHero then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    resources:PrepareResource(book_resource)
                    controlAcquired3 = resources:TryAcquire(book_resource, me, 4)
                    while not controlAcquired3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return false end
                        controlAcquired3 = resources:TryAcquire(book_resource, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then

                        book_movie = resources:StartMovie("")

                        resources:Pause(true)

                        if quest:GetStateBool("GivenSweets") then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                book_thing = resources:NewThingFromResource(book_resource)
                                health2 = resources:ThingHealth(book_thing)
                                resources:DestroyThing(book_thing); book_thing = nil
                                bookHasHealth2 = health2 > 0.0
                                if bookHasHealth2 then





                                    hero2 = quest:GetHero()
                                    speechResult2 = resources:Speak(book_resource, hero2, ("TEXT_QST_048_TRADER_INTRO_10"), (0x0), false, true, (false))
                                    taskRunning5 = resources:IsPerformingScriptTask(book_resource)
                                    if taskRunning5 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(book_movie); book_movie = nil
                                                return false
                                            end
                                            taskRunning6 = resources:IsPerformingScriptTask(book_resource)
                                        until not (taskRunning6)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        return false
                                    end
                                end
                                return finishTrade()
                            end
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            return false
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00db4f39: (native jump target)
                            -- LAB_00db4f45: (native jump target)
                            resources:Pause(false)
                            -- LAB_00db4f4d: (native jump target)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            return false
                        end
                        if not __native_entity_state:GetStateBool("DoneIntro") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                return false
                            end
                            book_thing = resources:NewThingFromResource(book_resource)
                            health3 = resources:ThingHealth(book_thing)
                            resources:DestroyThing(book_thing); book_thing = nil
                            bookHasHealth3 = health3 > 0.0
                            if bookHasHealth3 then





                                hero3 = quest:GetHero()
                                speechResult3 = resources:Speak(book_resource, hero3, ("TEXT_QST_048_TRADER_INTRO"), (0x0), false, true, (false))
                                taskRunning7 = resources:IsPerformingScriptTask(book_resource)
                                if taskRunning7 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            return false
                                        end
                                        taskRunning8 = resources:IsPerformingScriptTask(book_resource)
                                    until not (taskRunning8)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(book_movie); book_movie = nil
                                    return false
                                end
                            end
                            __native_entity_state:SetStateBool("DoneIntro", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                return false
                            end
                            book_thing = resources:NewThingFromResource(book_resource)
                            health4 = resources:ThingHealth(book_thing)
                            resources:DestroyThing(book_thing); book_thing = nil
                            bookHasHealth4 = health4 > 0.0
                            if bookHasHealth4 then





                                hero4 = quest:GetHero()
                                speechResult4 = resources:Speak(book_resource, hero4, ("TEXT_QST_048_TRADER_STILL_GOT"), (0x0), false, true, (false))
                                taskRunning9 = resources:IsPerformingScriptTask(book_resource)
                                if taskRunning9 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            return false
                                        end
                                        taskRunning10 = resources:IsPerformingScriptTask(book_resource)
                                    until not (taskRunning10)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(book_movie); book_movie = nil
                                    return false
                                end
                            end
                        end

                        quest:GiveHeroYesNoQuestion(("TEXT_QST_048_TRADER_BUY_SWEETS"), "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while true do

                            if not (questionAnswer < 0) then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                return false
                            end
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            return false
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if alive then
                                getHeroGold = quest:GetHeroGold()
                                if getHeroGold < 3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        return false
                                    end
                                    book_thing = resources:NewThingFromResource(book_resource)
                                    health5 = resources:ThingHealth(book_thing)
                                    resources:DestroyThing(book_thing); book_thing = nil
                                    bookHasHealth5 = health5 > 0.0
                                    if bookHasHealth5 then





                                        hero5 = quest:GetHero()
                                        speechResult5 = resources:Speak(book_resource, hero5, ("TEXT_QST_048_TRADER_NOT_ENOUGH_CASH"), (0x0), false, true, (false))
                                        taskRunning11 = resources:IsPerformingScriptTask(book_resource)
                                        if taskRunning11 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(book_movie); book_movie = nil
                                                    return false
                                                end
                                                taskRunning12 = resources:IsPerformingScriptTask(book_resource)
                                            until not (taskRunning12)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            -- LAB_00db4bc5: (native jump target)
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            return false
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        return false
                                    end
                                    book_thing = resources:NewThingFromResource(book_resource)
                                    health6 = resources:ThingHealth(book_thing)
                                    resources:DestroyThing(book_thing); book_thing = nil
                                    bookHasHealth6 = health6 > 0.0
                                    if bookHasHealth6 then





                                        hero6 = quest:GetHero()
                                        speechResult6 = resources:Speak(book_resource, hero6, ("TEXT_QST_048_TRADER_GIVES_SWEETS"), (0x0), false, true, (false))
                                        taskRunning13 = resources:IsPerformingScriptTask(book_resource)
                                        if taskRunning13 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(book_movie); book_movie = nil
                                                    return false
                                                end
                                                taskRunning14 = resources:IsPerformingScriptTask(book_resource)
                                            until not (taskRunning14)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            return false
                                        end
                                    end

                                    quest:GiveHeroObject(("OBJECT_CHOCOLATE_BOX_UNGIVEABLE"), -1)
                                    quest:GiveHeroGold(-3)
                                    getActiveQuestName = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04", "", "")
                                    quest:SetStateBool("GivenSweets", true)
                                    quest:ClearThingHasInformation(me)
                                end
                                return finishTrade()
                            end
                            -- LAB_00db46da: (native jump target)
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            return false
                        end
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            return false
                        end
                        book_thing = resources:NewThingFromResource(book_resource)
                        health7 = resources:ThingHealth(book_thing)
                        resources:DestroyThing(book_thing); book_thing = nil
                        bookHasHealth7 = health7 > 0.0
                        if bookHasHealth7 then





                            hero7 = quest:GetHero()
                            speechResult7 = resources:Speak(book_resource, hero7, ("TEXT_QST_048_TRADER_BUY_LATER"), (0x0), false, true, (false))
                            taskRunning15 = resources:IsPerformingScriptTask(book_resource)
                            if taskRunning15 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        return false
                                    end
                                    taskRunning16 = resources:IsPerformingScriptTask(book_resource)
                                until not (taskRunning16)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then return finishTrade() end
                            -- LAB_00db4f3f: (native jump target)
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            return false
                        end
                        return finishTrade()
                    end
                end
                return false
            end
            return true
        end
        local function tryIntermittentLine()
            if quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer")) == 0
                and quest:RetailRandModulo(200) == 0 then

                hero8 = quest:GetHero()
                isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero8, (20.0))
                if isDistanceBetweenThingsUnder then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
                        conversationId = quest:StartConversationWithHero(me, false, false)
                        resources:PrepareResource(book_resource)
                        controlAcquired4 = resources:TryAcquire(book_resource, me, 4)
                        while not controlAcquired4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return false end
                            controlAcquired4 = resources:TryAcquire(book_resource, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            resources:PlayAnimation(book_resource, "ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, resources:ReadAnimationArgument5(), false)
                            quest:AddConversationLineToHero(conversationId, "TEXT_QST_048_TRADER_ROLL_UP", me, false)
                            return true
                        end
                    end
                    return false
                end
            end
            return true
        end
        while true do
            if not prepareAndReturnHome() then break end
            if not processInteraction() then break end
            if not tryIntermittentLine() then break end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            predicateResult = not alive
        end
        release_book()
    end
end


function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
