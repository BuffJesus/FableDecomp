-- DISABLED REVIEW CANDIDATE: structured Main; staged runtime adapters and Init/lifecycle validation remain.
-- Generated native draft: NOVI_BarrelMan. Review coverage report before use.
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
    local barrelWatchTimer = quest:GetStateInt("WatchTimer")
    quest:WithRetailResources(function(resources)
        resources:ResetBarrelWatchTimer(barrelWatchTimer)
    end)
    __native_entity_state:SetStateBool("ComplainedAboutStock", false)
    __native_entity_state:SetStateInt("MyPhase", 0)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    local homePosition = me:GetHomePos()
    quest:SetStateFloat("WarehouseMeetPoint_x", homePosition.x)
    quest:SetStateFloat("WarehouseMeetPoint_y", homePosition.y)
    quest:SetStateFloat("WarehouseMeetPoint_z", homePosition.z)
    __native_entity_state:SetStateBool("HeroLetMeDown", false)
    __native_entity_state:SetStateBool("OverheardYet", false)
    quest:EntitySetSightRadius(me, 10.0)
end

local function __resource_main(quest, me, resources)
    local function showWarehouseFailure()
        local movie = resources:StartMovie("")
        resources:Pause(true)
        quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED")
        local dismissed = quest:MsgIsGameInfoClickedPast()
        while not dismissed do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:Pause(false)
                resources:DestroyMovie(movie)
                return false
            end
            dismissed = quest:MsgIsGameInfoClickedPast()
        end
        if quest:IsActiveThreadTerminating() then
            resources:Pause(false)
            resources:DestroyMovie(movie)
            return false
        end
        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 1)
        resources:Pause(false)
        resources:DestroyMovie(movie)
        return true
    end
    local barrel_resource, barrel_interaction_movie
    local warehouseStartMarker, warehouseGuardMarker
    local function controlled_health()
        local thing = resources:NewThingFromResource(barrel_resource)
        local health = resources:ThingHealth(thing)
        resources:DestroyThing(thing)
        return health
    end
    local function controlled_distance(position, distance)
        local thing = resources:NewThingFromResource(barrel_resource)
        local isOver = resources:ThingIsDistanceFromPositionOver(thing, position, distance)
        resources:DestroyThing(thing)
        return isOver
    end
    local function finishSpeechMovie(movie)
        resources:Pause(false)
        resources:DestroyMovie(movie)
    end
    local function waitForBarrelSpeech()
        while resources:IsPerformingScriptTask(barrel_resource) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end
    local function playThanksMovie()
        local movie = resources:StartMovie("")
        resources:Pause(true)
        if controlled_health() > 0.0 then
            resources:Speak(barrel_resource, quest:GetHero(), "TEXT_QST_048_BARRELMAN_THANKS", 0, false, true, false)
            if not waitForBarrelSpeech() then
                finishSpeechMovie(movie)
                return false
            end
        end
        require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        finishSpeechMovie(movie)
        return true
    end
    local function playCarefulMovie()
        local movie = resources:StartMovie("")
        resources:Pause(true)
        if controlled_health() > 0.0 then
            resources:Speak(barrel_resource, quest:GetHero(), "TEXT_QST_048_BARRELMAN_CAREFUL", 0, false, true, false)
            if not waitForBarrelSpeech() then
                finishSpeechMovie(movie)
                return false
            end
        end
        finishSpeechMovie(movie)
        return true
    end
    local function playReturnInteraction(movie, phase)
        local function cancel()
            finishSpeechMovie(movie)
            return false
        end
        local line
        if phase < 4 then
            if quest:IsActiveThreadTerminating() then return cancel() end
            line = "TEXT_QST_048_BARRELMAN_NOT_LARKING"
        elseif phase == 5 then
            if quest:IsActiveThreadTerminating() then return cancel() end
            if not __native_entity_state:GetStateBool("HeroLetMeDown") then
                if quest:IsActiveThreadTerminating() then return cancel() end
                line = "TEXT_QST_048_BARRELMAN_NO_TIME"
            else
                if quest:IsActiveThreadTerminating() then return cancel() end
                local broken = quest:GetStateBool("BarrelBrokenPersistent")
                if quest:IsActiveThreadTerminating() then return cancel() end
                if broken then
                    line = "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN"
                else
                    line = "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE"
                end
            end
        end
        if line and controlled_health() > 0.0 then
            resources:Speak(barrel_resource, quest:GetHero(), line, 0, false, true, false)
            if not waitForBarrelSpeech() then return cancel() end
        end
        resources:PrepareResource(barrel_resource)
        finishSpeechMovie(movie)
        return true
    end
    local function playInitialInteraction(movie)
        if quest:IsActiveThreadTerminating() then
            finishSpeechMovie(movie)
            return false
        end
        local continued = resources:WithTimer(function(timer)
            while resources:IsBarrelManFarFromHero(me) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
                resources:MoveBarrelManToHero(barrel_resource)
                timer:Set(2)
                while resources:IsPerformingScriptTask(barrel_resource) do
                    if timer:Get() <= 0 then break end
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then return false end
                end
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
            if controlled_health() > 0.0 then
                resources:Speak(barrel_resource, quest:GetHero(), "TEXT_QST_048_BARRELMAN_FAVOUR", 0, false, true, false)
                if not waitForBarrelSpeech() then return false end
            end
            quest:FadeScreenOut(1.0, 1.0)
            quest:Pause(2.0)
            resources:TeleportBarrelDepartureActors(me)
            __native_entity_state:SetStateInt("MyPhase", 2)
            quest:ClearThingHasInformation(me)
            quest:FadeScreenIn()
            resources:SetBarrelWatchTimer(quest:GetStateInt("WatchTimer"))
            quest:SetStateBool("BarrelManLeftHeroInCharge", true)
            return true
        end)
        if continued then resources:PrepareResource(barrel_resource) end
        finishSpeechMovie(movie)
        return continued
    end
    local function walkOffFromWarehouse()
        local marker = resources:NewThingFromScriptName("M_BarrelManWalkOff")
        local position = resources:ThingPosition(marker)
        local function moveUntilNear()
            while controlled_distance(position, 2.0) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
                resources:MoveToPosition(barrel_resource, position, 0.0, 1, false, false)
                if not waitForBarrelSpeech() then return false end
            end
            return not quest:IsActiveThreadTerminating()
        end
        local continued = moveUntilNear()
        if continued then __native_entity_state:SetStateInt("MyPhase", 2) end
        resources:DestroyThing(marker)
        return continued
    end
    local function teleportWalkOff()
        while resources:GetBarrelWatchTimer(quest:GetStateInt("WatchTimer")) ~= 15 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local primary = resources:NewThingFromScriptName("M_BarrelManWalkOff")
        local alternate = resources:NewThingFromScriptName("M_BarrelManWalkOffAlt")
        local visible = resources:IsOwnedThingPositionOnScreen(primary)
        local continued = not quest:IsActiveThreadTerminating()
        if continued then
            resources:TeleportActorToOwnedThing(me, visible and alternate or primary)
            __native_entity_state:SetStateInt("MyPhase", 3)
        end
        resources:DestroyThing(alternate)
        resources:DestroyThing(primary)
        return continued
    end
    local function returnToWarehouse(marker)
        local position = resources:ThingPosition(marker)
        while controlled_distance(position, 2.0) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
            resources:MoveToPosition(barrel_resource, position, 0.0, 1, false, false)
            if not waitForBarrelSpeech() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        __native_entity_state:SetStateInt("MyPhase", 4)
        return true
    end
    local function acquireBarrelControl()
        resources:PrepareResource(barrel_resource)
        while not resources:TryAcquire(barrel_resource, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end
    local function advanceBarrelPhase()
        if __native_entity_state:GetStateInt("MyPhase") == 0 then return true end
        if quest:IsActiveThreadTerminating() then return false end
        local phase = __native_entity_state:GetStateInt("MyPhase")
        if phase == 1 then
            resources:SetBarrelWatchTimer(quest:GetStateInt("WatchTimer"))
            quest:EntitySetTargetable(me, false)
            if not acquireBarrelControl() then return false end
            return walkOffFromWarehouse()
        elseif phase == 2 then
            return teleportWalkOff()
        elseif phase == 3 then
            if not acquireBarrelControl() then return false end
            quest:EntitySetCutsceneBehaviour(me, 1)
            quest:EntitySetTargetable(me, false)
            return returnToWarehouse(warehouseStartMarker)
        elseif phase == 4 then
            quest:SetStateBool("BarrelManSpokenToHeroOnReturn", true)
            resources:FaceBarrelManTowardsHero(me)
            quest:EntitySetCutsceneBehaviour(me, 2)
            quest:EntitySetTargetable(me, true)
            local stockBroken = quest:GetStateBool("BarrelBrokenPersistent")
            if resources:ShouldBarrelManThankHero(me) and not stockBroken then
                if quest:IsActiveThreadTerminating() then return false end
                if not acquireBarrelControl() then return false end
                if not playThanksMovie() then return false end
            elseif stockBroken then
                if quest:IsActiveThreadTerminating() then return false end
                if not acquireBarrelControl() then return false end
                __native_entity_state:SetStateBool("HeroLetMeDown", true)
                local brokenMovie = resources:StartMovie("")
                resources:Pause(true)
                if not playReturnInteraction(brokenMovie, 5) then return false end
            else
                if quest:IsActiveThreadTerminating() then return false end
                resources:AddBarrelConversation(me, "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE")
                __native_entity_state:SetStateBool("HeroLetMeDown", true)
                if not showWarehouseFailure() then return false end
            end
            quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
            __native_entity_state:SetStateInt("MyPhase", 5)
        end
        return true
    end
    local function handleBarrelInteraction()
        if resources:IsHitByHeroExceptAbility(me, 14) then
            if quest:IsActiveThreadTerminating() then return false end
            resources:SetBarrelManHeroAllies(me)
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            if not acquireBarrelControl() then return false end
            return playCarefulMovie()
        end
        local approached = false
        if __native_entity_state:GetStateInt("MyPhase") == 0 then
            approached = resources:IsHeroWithinBarrelApproachDistance(me)
        end
        if not approached then approached = me:IsTalkedToByHero() end
        if approached then
            if quest:IsActiveThreadTerminating() then return false end
            if not acquireBarrelControl() then return false end
            barrel_interaction_movie = resources:StartMovie("")
            resources:Pause(true)
            local phase = __native_entity_state:GetStateInt("MyPhase")
            local continued
            if phase ~= 0 then
                continued = playReturnInteraction(barrel_interaction_movie, phase)
            else
                continued = playInitialInteraction(barrel_interaction_movie)
            end
            barrel_interaction_movie = nil
            return continued
        end
        if __native_entity_state:GetStateInt("MyPhase") == 0
            and resources:ShouldBarrelOverhear(me, __native_entity_state:GetStateBool("OverheardYet")) then
            if quest:IsActiveThreadTerminating() then return false end
            __native_entity_state:SetStateBool("OverheardYet", true)
            resources:AddBarrelConversation(me, "TEXT_QST_048_BARRELMAN_OVERHEAR")
        end
        return true
    end
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    barrel_resource = resources:NewResource()
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local barrelHomePosition = me:GetHomePos()
    quest:SetWanderCentrePoint(me, barrelHomePosition)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 1.0)
    quest:SetScriptingStateGroup(me, 4)
    warehouseStartMarker = resources:NewThingFromScriptName("M_WHouse_ManStart")
    warehouseGuardMarker = resources:NewThingFromScriptName("M_WHouse_GuardPoint")
    while not quest:IsActiveThreadTerminating() do
        if not advanceBarrelPhase() then break end
        if not handleBarrelInteraction() then break end
        quest:NewScriptFrame(me)
    end
    resources:DestroyThing(warehouseGuardMarker); warehouseGuardMarker = nil
    resources:DestroyThing(warehouseStartMarker); warehouseStartMarker = nil
    resources:ReleaseResource(barrel_resource); barrel_resource = nil
end


function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
