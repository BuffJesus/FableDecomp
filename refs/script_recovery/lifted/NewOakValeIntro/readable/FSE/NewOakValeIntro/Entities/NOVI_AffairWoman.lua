-- DISABLED resource-aware woman candidate; native/runtime parity checks remain pending.
-- Generated native draft: NOVI_AffairWoman. Review coverage report before use.
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
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

local function __resource_main(quest, me, resources)
    local woman_resource, woman_thing, woman_movie, woman_home
    local function finish_movie()
        resources:Pause(false)
        resources:DestroyMovie(woman_movie); woman_movie = nil
    end
    local __native_condition_1, __native_condition_2, outsideDistance, outsideDistance2, taskRunning
    local taskRunning2, isHitByHeroExceptAbility, taskRunning3, taskRunning4, taskRunning5
    local taskRunning6, taskRunning7, taskRunning8, thingsWithinDistance, outsideDistance3
    local taskRunning9, taskRunning10, controlAcquired, predicateResult, controlAcquired2
    local talkedToByHero, controlAcquired3, isCameraPosOnScreen, health, health2, runoffPosition
    local homePosition, hero, hero2, affairWife, affairMan, speechResult, hero3, speechResult2
    local affairWomanRunOffPoint
    local alive = true
    quest:RegisterBoundConsciousCondition()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    local function runOffAndWaitForCamera()
        while outsideDistance3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end
            resources:MoveToPosition(woman_resource, runoffPosition, 0.0, 1, false, true)
            taskRunning9 = resources:IsPerformingScriptTask(woman_resource)
            if taskRunning9 then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end
                    taskRunning10 = resources:IsPerformingScriptTask(woman_resource)
                until not (taskRunning10)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end

            woman_thing = resources:NewThingFromResource(woman_resource)
            outsideDistance3 = resources:ThingIsDistanceFromPositionOver(woman_thing, runoffPosition, 2.0)
            resources:DestroyThing(woman_thing); woman_thing = nil
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            resources:PrepareResource(woman_resource)
            isCameraPosOnScreen = quest:IsCameraPosOnScreen(me:GetPos())
            while isCameraPosOnScreen do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end
                isCameraPosOnScreen = quest:IsCameraPosOnScreen(me:GetPos())
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                quest:RemoveThing(me, false, true)
            end
        end
    end
    local function runInteractions()
        alive = not quest:IsActiveThreadTerminating()

        predicateResult = not alive
        while not predicateResult do

            homePosition = me:GetHomePos()
            outsideDistance = (me ~= nil and me:IsDistanceFromPositionOver(homePosition, (0.1)))
            if outsideDistance then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                woman_home = me:GetHomePos()
                while true do

                    woman_thing = resources:NewThingFromResource(woman_resource)
                    outsideDistance2 = resources:ThingIsDistanceFromPositionOver(woman_thing, woman_home, 2.0)
                    resources:DestroyThing(woman_thing); woman_thing = nil
                    if not outsideDistance2 then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end
                    resources:MoveToPosition(woman_resource, woman_home, 0.0, 0, false, true)
                    taskRunning = resources:IsPerformingScriptTask(woman_resource)
                    if taskRunning then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end
                            taskRunning2 = resources:IsPerformingScriptTask(woman_resource)
                        until not (taskRunning2)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
            end
            isHitByHeroExceptAbility = resources:IsHitByHeroExceptAbility(me, 14)
            if isHitByHeroExceptAbility then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                quest:SetStateBool("TalkingToWoman", true)

                woman_movie = resources:StartMovie("")
                resources:Pause(true)
                resources:PrepareResource(woman_resource)
                controlAcquired2 = resources:TryAcquire(woman_resource, me, 4)
                while true do
                    if not (not controlAcquired2) then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then finish_movie(); return end
                    controlAcquired2 = resources:TryAcquire(woman_resource, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00db2715: (native jump target)
                    finish_movie()
                    break
                end
                woman_thing = resources:NewThingFromResource(woman_resource)
                health = resources:ThingHealth(woman_thing)
                resources:DestroyThing(woman_thing); woman_thing = nil

                if (0.0) < health then





                    hero = quest:GetHero()
                    speechResult = resources:Speak(woman_resource, hero, "TEXT_QST_048_AFFAIRWOMAN_ON_HIT", 0, false, true, false)
                    taskRunning3 = resources:IsPerformingScriptTask(woman_resource)
                    if taskRunning3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then finish_movie(); return end
                            taskRunning4 = resources:IsPerformingScriptTask(woman_resource)
                        until not (taskRunning4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        finish_movie()
                        return
                    end
                end
                require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                quest:SetStateBool("TalkingToWoman", false)
                finish_movie()
            end
            talkedToByHero = me:IsTalkedToByHero()
            if talkedToByHero then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                quest:SetStateBool("TalkingToWoman", true)
                resources:ClearAllActions(woman_resource)
                resources:ClearCommands(woman_resource)

                woman_movie = resources:StartMovie("")
                resources:Pause(true)
                hero3 = quest:GetHero()
                resources:FaceThing(affairMan, hero3, false)
                resources:PrepareResource(woman_resource)
                controlAcquired3 = resources:TryAcquire(woman_resource, me, 4)
                while true do
                    if not (not controlAcquired3) then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then finish_movie(); return end
                    controlAcquired3 = resources:TryAcquire(woman_resource, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00db274c: (native jump target)
                    finish_movie()
                    break
                end
                woman_thing = resources:NewThingFromResource(woman_resource)
                health2 = resources:ThingHealth(woman_thing)
                resources:DestroyThing(woman_thing); woman_thing = nil

                if (0.0) < health2 then





                    hero2 = quest:GetHero()
                    speechResult2 = resources:Speak(woman_resource, hero2, "TEXT_QST_048_AFFAIRWOMAN_BUSY", 0, false, true, false)
                    taskRunning5 = resources:IsPerformingScriptTask(woman_resource)
                    if taskRunning5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then finish_movie(); return end
                            taskRunning6 = resources:IsPerformingScriptTask(woman_resource)
                        until not (taskRunning6)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        finish_movie()
                        return
                    end
                end
                resources:FaceThing(affairMan, me, false)
                quest:SetStateBool("TalkingToWoman", false)
                finish_movie()
            end
            __native_condition_1 = quest:GetStateBool("ReceiveKiss")
            if __native_condition_1 then
                taskRunning7 = resources:IsPerformingScriptTask(woman_resource)
                __native_condition_1 = not taskRunning7
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                resources:PlayAnimation(woman_resource, "RECEIVE_KISS", false, true, false, true, resources:ReadAnimationArgument5(), false)
                quest:SetStateBool("ReceiveKiss", false)
            end
            __native_condition_2 = quest:GetStateBool("ReceiveHug")
            if __native_condition_2 then
                taskRunning8 = resources:IsPerformingScriptTask(woman_resource)
                __native_condition_2 = not taskRunning8
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                resources:PlayAnimation(woman_resource, "RECEIVE_HUG", false, true, false, true, resources:ReadAnimationArgument5(), false)
                quest:SetStateBool("ReceiveHug", false)
            end
            thingsWithinDistance = resources:ThingsAreWithinDistance(me, affairWife, 5.0)
            if thingsWithinDistance then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    affairWomanRunOffPoint = resources:NewThingFromScriptName("AffairWomanRunOffPoint")
                    quest:EntitySetAsUseMovementInActions(me, true)
                    quest:SetIsPushableByHero(me, true)
                    runoffPosition = resources:ThingPosition(affairWomanRunOffPoint)

                    woman_thing = resources:NewThingFromResource(woman_resource)
                    outsideDistance3 = resources:ThingIsDistanceFromPositionOver(woman_thing, runoffPosition, 2.0)
                    resources:DestroyThing(woman_thing); woman_thing = nil
                    runOffAndWaitForCamera()
                    resources:DestroyThing(affairWomanRunOffPoint); affairWomanRunOffPoint = nil
                    return
                end
                break
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            predicateResult = not alive
        end
    end
    local function acquireAndRun()
        resources:PrepareResource(woman_resource)
        controlAcquired = resources:TryAcquire(woman_resource, me, 4)
        while not controlAcquired do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end
            controlAcquired = resources:TryAcquire(woman_resource, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end
        affairWife = resources:NewThingFromScriptName("NOVI_AffairWife")
        affairMan = resources:NewThingFromScriptName("NOVI_AffairMan")
        runInteractions()
        resources:DestroyThing(affairMan); affairMan = nil
        resources:DestroyThing(affairWife); affairWife = nil
    end
    woman_resource = resources:NewResource()
    acquireAndRun()
    resources:ReleaseResource(woman_resource); woman_resource = nil
end


function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
