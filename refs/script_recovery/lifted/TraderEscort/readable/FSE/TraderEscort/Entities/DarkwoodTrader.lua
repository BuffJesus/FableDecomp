-- Readable native conversion: DarkwoodTrader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_TraderToBalverineDelay = 3572,  -- 140
    TE_TraderLowInfectionTime = 3576,  -- 80
    TraderEscortLeaveTraderMorality = 3628,  -- -0.019999999552965164
}

local helpers = require("TraderEscort.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local regulateBanterComment, incubationTime, regulateFollowStateComment, brainState_, leadToCamp
local traderHealthID, inSafeZone, greetedBuddy, self0X14, infected, currentAIState, previousAIState

-- DarkwoodTrader.Main (retail 0x00e07640)
function Main(quest, me)
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue, scratchValue11, scratchValue14, isPerformingScriptTask
    local scratchValue16, questionAnswer, sequence, sequence3, hero9, scratchValue38, scratchValue40
    local actorMap, getDataString, getPos, furthest, actorMap2, resource, mkDtbeCutscenetrigger
    local movie, getCurrentStateGroupType, scratchValue45, resource2, scratchValue46, scriptThing
    local newResource
    if not quest:NewScriptFrame(me) then return end
    local resource3 = resources:NewResource()
    while not quest:GetStateBool("IntroFinished") do
        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
        if me:IsTalkedToByHero() and quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    end
    if brainState_ ~= 1 then
        goto LAB_00e083cd
    end
    goto FLOW_past_lab_00e083cd
    ::LAB_00e083cd::
    SetBrainState(quest, me, 2)
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + 1)
    quest:SetTimer(regulateBanterComment, 30)
    if not quest:IsActiveThreadTerminating() then
        repeat
            if leadToCamp then goto LAB_00e0843a end
            scratchValue2 = true
            if not quest:IsRegionLoaded("BarrowFields") then goto LAB_00e0843a end
            goto FLOW_past_lab_00e0843a
            ::LAB_00e0843a::
            scratchValue2 = false
            ::FLOW_past_lab_00e0843a::
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:EntityStopFollowing(me)
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, false)
                quest:SetTimer(commentTimer, 0)
                helpers.MakeTraderComment(quest, me, "IN_BARROW_FIELD", me, 0)
                resources:PrepareResource(resource3)
                while not resources:TryAcquire(resource3, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                if quest:IsActiveThreadTerminating() then break end
                local tradersStopHere = quest:GetThingWithScriptName("M_TradersStopHere")
                scratchValue2 = false
                scratchValue14 = 0
                me:ClearCommands()
                if not (tradersStopHere ~= nil and not tradersStopHere:IsNull()) then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = tradersStopHere:GetPos()
                end
                me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
                while not quest:IsDistanceBetweenThingsUnder(me, tradersStopHere, 2.0) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    scratchValue = (quest:GetDistanceBetweenThings(me, tradersStopHere) ^ 2) < (quest:GetDistanceBetweenThings(hero, tradersStopHere) ^ 2)
                    if quest:IsDistanceBetweenThingsUnder(me, hero, 9.0) or scratchValue == 0 then
                        if quest:IsDistanceBetweenThingsUnder(me, hero, 5.0) or scratchValue == 0 then
                            if scratchValue14 == 0 then scratchValue2 = false; goto continue_1 end
                            me:ClearCommands()
                            if not (tradersStopHere ~= nil and not tradersStopHere:IsNull()) then
                                getPos = {x = 0, y = 0, z = 0}
                            else
                                getPos = tradersStopHere:GetPos()
                            end
                            me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
                            scratchValue14 = 0
                        elseif scratchValue14 ~= 1 then
                            me:ClearCommands()
                            if not (tradersStopHere ~= nil and not tradersStopHere:IsNull()) then
                                getPos = {x = 0, y = 0, z = 0}
                            else
                                getPos = tradersStopHere:GetPos()
                            end
                            me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
                            scratchValue14 = 1
                        end
                        scratchValue2 = false
                    else
                        me:ClearCommands()
                        me:ClearAllActions()
                        if not scratchValue2 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                            local conversationId = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId, hero)
                            quest:AddLineToConversation(conversationId, ("TEXT_QST_067_" .. me:GetDataString()) .. "_FOLLOW_ME", me, hero, false)
                            quest:Pause(1.0)
                            scratchValue2 = true
                            scratchValue14 = 2
                        end
                    end
                    ::continue_1::
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
                hero9 = hero
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                helpers.MakeTraderComment(quest, me, "THANKS_10", me, 0)
                leadToCamp = true
            end
            if quest:GetStateBool("EndStarted") then
                if quest:IsActiveThreadTerminating() then break end
                quest:RemoveQuestInfoElement(traderHealthID)
                quest:MiniMapRemoveMarker(me)
                quest:EntityStopFollowing(me)
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, false)
                quest:ModifyThingHealth(me, 30.0 - quest:GetHealth(me), false)
                resources:PrepareResource(resource3)
                scratchValue2 = resources:TryAcquire(resource3, me, 4)
                goto LAB_00e09fd9
            end
            if quest:GetStateBool("TradersShouldBeScared") then
                if quest:IsActiveThreadTerminating() then break end
                local runOffPos = quest:GetThingWithScriptName("DTE_RunOffPos")
                quest:EntityStopFollowing(me)
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, true)
                resources:PrepareResource(resource3)
                while not resources:TryAcquire(resource3, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
                if not (runOffPos ~= nil and not runOffPos:IsNull()) then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = runOffPos:GetPos()
                end
                newResource = {x = getPos.x, y = getPos.y, z = getPos.z}
                local scratchValue37 = resources:ScriptThing(resource3)
                if scratchValue37 ~= nil and scratchValue37:IsDistanceFromPositionOver(newResource, 3.0) then
                    repeat
                        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                        me:MoveToPosition(newResource, 1.0, ENTITY_MOVE_RUN, false, true)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                        scratchValue38 = resources:ScriptThing(resource3)
                    until not (scratchValue38 ~= nil and scratchValue38:IsDistanceFromPositionOver(newResource, 3.0))
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                while quest:GetStateBool("TradersShouldBeScared") do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                resources:PrepareResource(resource3)
                quest:EntityFollowThing(me, hero, 3.0, true)
                quest:SetEntityAsRegionFollowing(hero, me, true)
                while not quest:IsDistanceBetweenThingsUnder(me, hero, 20.0) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                hero9 = hero
                quest:AddPersonToConversation(conversationId2, hero)
                quest:AddLineToConversation(conversationId2, ("TEXT_QST_067_" .. me:GetDataString()) .. "_KILLED_EARTH_TROLL", me, hero, false)
            end
            if inSafeZone then goto LAB_00e08d39 end
            scratchValue2 = true
            if not quest:IsRegionLoaded("Darkwood4") then goto LAB_00e08d39 end
            goto FLOW_past_lab_00e08d39
            ::LAB_00e08d39::
            scratchValue2 = false
            ::FLOW_past_lab_00e08d39::
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:EntitySetAsScared(me, false)
                inSafeZone = true
            else
                if not inSafeZone then
                    goto LAB_00e08dc8
                else
                    scratchValue2 = true
                    if quest:IsRegionLoaded("Darkwood4") then goto LAB_00e08dc8 end
                end
                goto FLOW_past_lab_00e08dc8
                ::LAB_00e08dc8::
                scratchValue2 = false
                ::FLOW_past_lab_00e08dc8::
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then break end
                    quest:EntitySetAsScared(me, true)
                    inSafeZone = false
                end
            end
            if greetedBuddy then goto LAB_00e08e52 end
            scratchValue2 = true
            if not quest:IsRegionLoaded("Darkwood4") then goto LAB_00e08e52 end
            goto FLOW_past_lab_00e08e52
            ::LAB_00e08e52::
            scratchValue2 = false
            ::FLOW_past_lab_00e08e52::
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then break end
                hero9 = hero
                local darkwoodTrader = quest:GetNearestWithScriptName(hero, "DarkwoodTrader")
                quest:SetStateString("TraderToTalk", darkwoodTrader:GetDataString())
                actorMap2 = quest:GetThingWithScriptName("TE_CampTrader_A")
                isPerformingScriptTask = actorMap2 ~= nil and actorMap2:IsAlive()
                if isPerformingScriptTask then
                    quest:EntityStopFollowing(me)
                    quest:SetEntityAsRegionFollowing(hero, me, false)
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                    if not (actorMap2 ~= nil and not actorMap2:IsNull()) then
                        getPos = {x = 0, y = 0, z = 0}
                    else
                        getPos = actorMap2:GetPos()
                    end
                    resource2 = {x = getPos.x, y = getPos.y, z = getPos.z}
                    local scratchValue39 = resources:ScriptThing(resource3)
                    if scratchValue39 ~= nil and scratchValue39:IsDistanceFromPositionOver(resource2, 5.0) then
                        repeat
                            if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                            me:MoveToPosition(resource2, 3.0, ENTITY_MOVE_WALK, false, true)
                            isPerformingScriptTask = me:IsPerformingScriptTask()
                            while isPerformingScriptTask do
                                if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                            scratchValue40 = resources:ScriptThing(resource3)
                        until not (scratchValue40 ~= nil and scratchValue40:IsDistanceFromPositionOver(resource2, 5.0))
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                    resource = quest:CreateEffectAtPos("", me:GetPos(), 0.0, false, false)
                    quest:ModifyThingHealth(me, 1000.0, false)
                    -- TODO(native): puVar7 = *piVar14
                    getPos = nil --[[unresolved native value]]
                    -- TODO(native): puVar1 = *(self0X14 + 0x78)
    --[[unresolved native value]]
                    if nil == getPos then
                        scratchValue11 = 1
                    elseif nil == nil or getPos == nil then
                        scratchValue11 = 0
                    elseif (nil)[1] == getPos.y then
                        -- TODO(native): iVar8 = CBasicString<char>::Compare((void *)*puVar1,(void *)puVar7.x);
                        scratchValue11 = isPerformingScriptTask == 0 and 1 or 0
                    else
                        scratchValue11 = 0
                    end
                    if scratchValue11 == 0 then
                        if not quest:IsActiveThreadTerminating() then
                            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e0a380 end
                            end
                            if not quest:IsActiveThreadTerminating() then quest:Pause(2.0); goto LAB_00e09493 end
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        local conversationId3 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId3, actorMap2)
                        local scratchValue41 = actorMap2
                        quest:AddLineToConversation(conversationId3, ("TEXT_QST_067_" .. me:GetDataString()) .. "_GREET_ALLY_10", me, resource, true)
                        quest:AddLineToConversation(conversationId3, ("TEXT_QST_067_" .. me:GetDataString()) .. "_GREET_ALLY_RESPONSE", actorMap2, darkwoodTrader, true)
                        -- TODO(native): uVar19 = SUB41(&xStack_158,0)
                        scratchValue45 = nil --[[unresolved native value]]
                        quest:AddLineToConversation(conversationId3, ("TEXT_QST_067_" .. me:GetDataString()) .. "_GREET_ALLY_20", me, scratchValue41)
                        quest:Pause(2.0)
                        quest:RemoveThing(resource, false, true)
                        while quest:IsConversationActive(conversationId3) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e0a380 end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e09493 end
                    end
                    goto FLOW_past_lab_00e09493
                    ::LAB_00e09493::
                    greetedBuddy = true
                    if resource ~= nil and resource:IsAlive() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e0a380 end
                        quest:RemoveThing(resource, false, true)
                    end
                    resources:PrepareResource(scratchValue46)
                    hero9 = hero
                    quest:EntityFollowThing(me, hero, 3.0, true)
                    quest:SetEntityAsRegionFollowing(hero, me, true)
                    goto LAB_00e09511
                    ::FLOW_past_lab_00e09493::
                    ::LAB_00e0a380::
                    resources:ReleaseResource(resource3)
                    return
                end
                ::LAB_00e09511::
            end
            if 0 < quest:GetStateInt("BalverinesToSurpriseHeroNeeded") and not inSafeZone then
                if quest:IsActiveThreadTerminating() then break end
                quest:EntitySetAsScared(me, true)
            end
            if infected == 0 or not quest:GetStateBool("SavedInMiddle") then
                goto LAB_00e095cf
            else
                if quest:IsRegionLoaded("Darkwood4") then goto LAB_00e095cf end
                scratchValue2 = false
                if quest:IsRegionLoaded("BarrowFields") then goto LAB_00e095cf end
            end
            goto FLOW_past_lab_00e095cf
            ::LAB_00e095cf::
            scratchValue2 = true
            ::FLOW_past_lab_00e095cf::
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderToBalverineDelay))
            end
            if quest:GetTimer(incubationTime) == 0 then
                if not quest:IsActiveThreadTerminating() then
                    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") - 1)
                    resources:PrepareResource(resource3)
                    quest:CreateThread("TurnToBalv", {args = {me}})  -- native parent-quest worker TurnToBalv, the entity's own thing captured
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                end
                break
            end
            sequence = quest:GetTimer(incubationTime) < quest:GetStateInt("IncubationTimeHigh")
            if sequence then
                getCurrentStateGroupType = 4
                sequence = infected < 4
            end
            if sequence then
                if quest:IsActiveThreadTerminating() then break end
                scratchValue2 = helpers.MakeTraderComment(quest, me, "INFECTED_HIGH", me, 0)
                goto LAB_00e096c7
            else
                sequence3 = quest:GetTimer(incubationTime) < quest:GetStateInt("IncubationTimeMedium")
                if sequence3 then
                    getCurrentStateGroupType = 3
                    sequence3 = infected < 3
                end
                if sequence3 then
                    if not quest:IsActiveThreadTerminating() then scratchValue2 = helpers.MakeTraderComment(quest, me, "INFECTED_MEDIUM", me, 0); goto LAB_00e096c7 end
                    break
                end
                if quest:GetTimer(incubationTime) < quest:GetStateInt("IncubationTimeLow") and infected < 2 then
                    if quest:IsActiveThreadTerminating() then break end
                    if helpers.MakeTraderComment(quest, me, "INFECTED_LOW", me, 0) then
                        if quest:IsActiveThreadTerminating() then break end
                        infected = 2
                    end
                end
            end
            goto FLOW_past_lab_00e096c7
            ::LAB_00e096c7::
            if scratchValue2 ~= false then
                if quest:IsActiveThreadTerminating() then break end
                infected = getCurrentStateGroupType
            end
            ::FLOW_past_lab_00e096c7::
            if me:MsgIsHitBy("") then goto LAB_00e09848 end
            if me:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00e09848 end
            goto LAB_00e09886
            goto FLOW_past_lab_00e09886
            ::LAB_00e09886::
            scratchValue2 = false
            ::FLOW_past_lab_00e09886::
            goto FLOW_past_lab_00e09848
            ::LAB_00e09848::
            scratchValue2 = true
            if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e09886 end
            ::FLOW_past_lab_00e09848::
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetTimer(regulateFollowStateComment, 5)
                if quest:GetHealth(me) <= 5.0 then
                    goto LAB_00e099dc
                else
                    if me:MsgIsHitByHero() then goto LAB_00e099dc end
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e099dc end
                    end
                    scratchValue2 = true
                end
                goto FLOW_past_lab_00e099dc
                ::LAB_00e099dc::
                scratchValue2 = false
                ::FLOW_past_lab_00e099dc::
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then break end
                    helpers.MakeTraderComment(quest, me, "UNDER_ATTACK", me, 0)
                    scratchValue16 = 3
                else
                    if quest:GetHealth(me) <= 5.0 then
                        goto LAB_00e09b4f
                    else
                        if not me:MsgIsHitByHero() then
                            if not (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)) then
                                goto LAB_00e09b4f
                            end
                        end
                        scratchValue2 = true
                    end
                    goto FLOW_past_lab_00e09b4f
                    ::LAB_00e09b4f::
                    scratchValue2 = false
                    ::FLOW_past_lab_00e09b4f::
                    if not scratchValue2 then goto LAB_00e09c13 end
                    if quest:IsActiveThreadTerminating() then break end
                    helpers.MakeTraderComment(quest, me, "HERO_HIT_ME", me, 0)
                    scratchValue16 = 2
                end
                quest:SetTimer(commentTimer, scratchValue16)
            end
            ::LAB_00e09c13::
            if currentAIState == previousAIState then
                if quest:GetTimer(regulateBanterComment) == 0 and not leadToCamp then
                    if quest:IsActiveThreadTerminating() then break end
                    if quest:GetStateInt("TradersStillAliveCounter") == 1 then
                        scratchValue = helpers.MakeTraderComment(quest, me, "BANTER_SOLO", me, 0)
                    else
                        local switch1 = quest:GetStateInt("TimesBantered")
                        repeat
                            if switch1 == 0 then
                                scratchValue = helpers.MakeTraderComment(quest, me, "BANTER_FIRST", me, 1)
                                break
                            elseif switch1 == 1 then
                                scratchValue = helpers.MakeTraderComment(quest, me, "BANTER_SECOND", me, 1)
                                break
                            elseif switch1 == 2 then
                                scratchValue = helpers.MakeTraderComment(quest, me, "BANTER_THIRD", me, 1)
                                break
                            elseif switch1 == 3 then
                                scratchValue = helpers.MakeTraderComment(quest, me, "BANTER_FOURTH", me, 1)
                                break
                            else
                                scratchValue = helpers.MakeTraderComment(quest, me, "BANTER", me, 0)
                            end
                        until true
                    end
                    if not scratchValue then
                        if quest:IsActiveThreadTerminating() then break end
                        local scratchValue17 = math.random(0, 32767)
                        quest:SetTimer(regulateBanterComment, scratchValue17 % 15 + 25)
                    else
                        if quest:IsActiveThreadTerminating() then break end
                        local scratchValue18 = math.random(0, 32767)
                        quest:SetTimer(regulateBanterComment, scratchValue18 % 20 + 30)
                        quest:SetStateInt("TimesBantered", quest:GetStateInt("TimesBantered") + 1)
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then break end
                if quest:GetTimer(regulateFollowStateComment) == 0 and currentAIState == 2 then
                    helpers.MakeTraderComment(quest, me, "FOLLOWING", me, 0)
                    quest:SetTimer(regulateFollowStateComment, 10)
                end
            end
            previousAIState = currentAIState
            getCurrentStateGroupType = me:GetCurrentStateGroupType()
            currentAIState = getCurrentStateGroupType
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource3)
                do return end
            end
        until false
    end
    resources:ReleaseResource(resource3)
    do return end
    ::FLOW_past_lab_00e083cd::
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    quest:SetThingAsConscious(me, false, "")
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    while not quest:GetStateBool("InfectedTraderCanGetUp") do
        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    quest:EntitySetAsDamageable(me, true)
    quest:EntitySetTargetable(me, true)
    quest:SetThingAsConscious(me, true, "")
    mkDtbeCutscenetrigger = quest:GetThingWithScriptName("MK_DTBE_CUTSCENETRIGGER")
    while quest:IsDistanceBetweenThingsOver(mkDtbeCutscenetrigger, hero, 5.0) do
        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
        if quest:GetStateBool("MissionFailed") then
            movie = quest:GetThingWithScriptName("MK_DTEB_INFEXIT")
            if movie == nil then
                getPos = {x = 0, y = 0, z = 0}
            else
                getPos = movie:GetPos()
            end
            actorMap2 = {x = getPos.x, y = getPos.y, z = getPos.z}
            local scratchValue42 = resources:ScriptThing(resource3)
            scratchValue = scratchValue42 ~= nil and scratchValue42:IsDistanceFromPositionOver(actorMap2, 2.0)
            while scratchValue do
                if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                me:MoveToPosition(actorMap2, 0, ENTITY_MOVE_RUN, false, true)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                local scratchValue43 = resources:ScriptThing(resource3)
                scratchValue = scratchValue43 ~= nil and scratchValue43:IsDistanceFromPositionOver(actorMap2, 2.0)
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
            quest:FadeOutAndKillEntity(me, true, 2.0, true)
        end
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if not scratchValue2 then
        scratchValue = scratchValue2
        if not me:IsDead() and 0.0 < quest:GetHealth(me) then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local darkwoodTrader2 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
            furthest = quest:GetFurthestWithScriptName(me, "DarkwoodTrader")
            newResource = resources:NewResource()
            resource2 = resources:NewResource()
            resource = resources:NewResource()
            resources:PrepareResource(newResource)
            hero9 = hero
            while not resources:TryAcquire(newResource, hero9, 4) do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    hero9 = hero
                else
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource2)
                    resources:ReleaseResource(newResource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e085f8
                    hero9 = hero
                end
            end
            if not quest:IsActiveThreadTerminating() then
                if darkwoodTrader2 ~= nil and darkwoodTrader2:IsEqualTo(furthest._4_4_) then
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource)
                        resources:ReleaseResource(resource2)
                        resources:ReleaseResource(newResource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                    end
                end
                if darkwoodTrader2:GetDataString() == "SCARED" then
                    goto LAB_00e07cac
                else
                    scratchValue2 = true
                    if not (furthest ~= nil and furthest:IsAlive()) then goto LAB_00e07cac end
                end
                goto FLOW_past_lab_00e07cac
                ::LAB_00e07cac::
                scratchValue2 = false
                ::FLOW_past_lab_00e07cac::
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e07db9 end
                    -- TODO(native): xStack_144 = xStack_12c;
                    furthest = scriptThing
                end
                quest:FixMovieSequenceCamera(true)
                actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2, "HERO", newResource)
                resources:SetActor(actorMap2, "TRADERI", resource3)
                if not (darkwoodTrader2 ~= nil and darkwoodTrader2:IsAlive()) then
                    goto LAB_00e081e9
                elseif not quest:IsActiveThreadTerminating() then
                    if furthest ~= nil and furthest:IsAlive() then
                        if not quest:IsActiveThreadTerminating() then
                            resources:PrepareResource(resource2)
                            while not resources:TryAcquire(resource2, darkwoodTrader2, 4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00e07e30 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e07db0 end
                            resources:PrepareResource(resource)
                            while not resources:TryAcquire(resource, furthest, 4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00e07db0 end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                resources:SetActor(actorMap2, "TRADERS", resource2)
                                resources:SetActor(actorMap2, "TRADERN", resource)
                                actorMap = "CS_DARKWOOD_TRADER_INFECTED_BOTH"
                                goto LAB_00e081c2
                            end
                        end
                        ::LAB_00e07e30::
                        resources:DestroyActorMap(actorMap2)
                        resources:ReleaseResource(resource)
                        resources:ReleaseResource(resource2)
                        resources:ReleaseResource(newResource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                    end
                    getDataString = darkwoodTrader2:GetDataString()
                    if not (getDataString ~= nil and getDataString == "SCARED") then
                        if quest:IsActiveThreadTerminating() then
                            resources:DestroyActorMap(actorMap2)
                            resources:ReleaseResource(resource)
                            resources:ReleaseResource(resource2)
                            resources:ReleaseResource(newResource)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e085f8
                        end
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, darkwoodTrader2, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:DestroyActorMap(actorMap2)
                                resources:ReleaseResource(resource)
                                resources:ReleaseResource(resource2)
                                resources:ReleaseResource(newResource)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00e085f8
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap2, "TRADERN", resource)
                            actorMap = "CS_DARKWOOD_TRADER_INFECTED_NORMAL"
                            goto LAB_00e081c2
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(resource2)
                        while not resources:TryAcquire(resource2, darkwoodTrader2, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e07db0 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap2, "TRADERS", resource2)
                            actorMap = "CS_DARKWOOD_TRADER_INFECTED_SCARED"
                            goto LAB_00e081c2
                        end
                        goto FLOW_hoist_lab_00e081c2_1
                    end
                    goto FLOW_past_lab_00e081c2
                    ::LAB_00e081c2::
                    resources:RunMacro(actorMap, actorMap2, false, true)
                    goto LAB_00e081e9
                    ::FLOW_hoist_lab_00e081c2_1::
                    resources:DestroyActorMap(actorMap2)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource2)
                    resources:ReleaseResource(newResource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e085f8
                    ::FLOW_past_lab_00e081c2::
                end
                goto FLOW_past_lab_00e081e9
                ::LAB_00e081e9::
                quest:GiveHeroYesNoQuestion("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        resources:DestroyActorMap(actorMap2)
                        resources:ReleaseResource(resource)
                        resources:ReleaseResource(resource2)
                        resources:ReleaseResource(newResource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if scratchValue2 then
                            resources:DestroyActorMap(actorMap2)
                            resources:ReleaseResource(resource)
                            resources:ReleaseResource(resource2)
                            resources:ReleaseResource(newResource)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e085f8
                        end
                        scratchValue = 1
                        actorMap = "CS_DARKWOOD_TRADER_INFECTED_JOINS"
                    else
                        if scratchValue2 then goto LAB_00e07db0 end
                        actorMap = "CS_DARKWOOD_TRADER_INFECTED_LEAVES"
                    end
                    resources:RunMacro(actorMap, actorMap2, false, true)
                    quest:FixMovieSequenceCamera(false)
                    resources:DestroyActorMap(actorMap2)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource2)
                    resources:ReleaseResource(newResource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    if scratchValue == 0 then
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TraderEscortLeaveTraderMorality))
                            quest:RemoveThing(me, false, true)
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(resource3)
                        quest:ClearThingHasInformation(me)
                        goto LAB_00e083cd
                    end
                    goto LAB_00e085f8
                end
                ::FLOW_past_lab_00e081e9::
                ::LAB_00e07db0::
                resources:DestroyActorMap(actorMap2)
            end
            ::LAB_00e07db9::
            resources:ReleaseResource(resource)
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(newResource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        else
            while true do
                if quest:IsActiveThreadTerminating() then break end
                quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00e085f8::
    ::LAB_00e08601::
    resources:ReleaseResource(resource3)
    do return end
    ::LAB_00e09fd9::
    if not scratchValue2 then
        if not quest:NewScriptFrame(me) then goto LAB_00e0a346 end
        scratchValue2 = resources:TryAcquire(resource3, me, 4)
        goto LAB_00e09fd9
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0a346 end
    while quest:IsDistanceBetweenThingsOver(me, actorMap2, 7.0) do
        if not quest:NewScriptFrame(me) then goto LAB_00e0a346 end
        if not me:IsPerformingScriptTask() then
            me:MoveToThing(hero9, 4.0, ENTITY_MOVE_WALK)
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0a346 end
    resources:PrepareResource(resource3)
    if quest:IsActiveThreadTerminating() then goto LAB_00e0a346 end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource3)
            while not resources:TryAcquire(resource3, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e0a346 end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local fret_03 = quest:GetHealth(resources:ScriptThing(resource3))
            if 0.0 < fret_03 then
                me:Speak(hero, ("TEXT_QST_067_" .. me:GetDataString()) .. "_THANKS", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e0a33a end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e0a33a end
                goto FLOW_past_lab_00e0a33a
                ::LAB_00e0a33a::
                resources:ReleaseResource(newResource)
                break
                ::FLOW_past_lab_00e0a33a::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(newResource)
            resources:PrepareResource("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW")
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW")
            do return end
        end
    until false
    ::LAB_00e0a346::
    resources:ReleaseResource(resource3)
end

-- DarkwoodTrader.Init (retail 0x00e04bd0)
function Init(quest, me)
    incubationTime = quest:RegisterTimer()  -- native constructor: CTimer member
    regulateFollowStateComment = quest:RegisterTimer()  -- native constructor: CTimer member
    regulateBanterComment = quest:RegisterTimer()  -- native constructor: CTimer member
    local scratchValue
    brainState_ = 0
    currentAIState = 0
    previousAIState = 0
    if me:GetDataString() == "INFECTED" then
        brainState_ = 1
    end
    infected = 0
    quest:SetTimer(incubationTime, 0)
    greetedBuddy = false
    leadToCamp = false
    inSafeZone = false
    quest:SetTimer(regulateFollowStateComment, 0)
    quest:SetTimer(regulateBanterComment, 60)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:SetThingPersistent(me, true)
    quest:SetEntityAsFollowingHeroThroughTeleporters(me, false)
    if me:GetDataString() ~= "INFECTED" then
        if me:GetDataString() ~= "SCARED" then
            if me:GetDataString() ~= "FRIENDLY" then goto LAB_00e04e13 end
            scratchValue = "FRIENDLY_TRADER"
        else
            scratchValue = "SCARED_TRADER"
        end
    else
        scratchValue = "INFECTED_TRADER"
    end
    quest:EntitySetWillBeUsingNarrator(me, scratchValue)
    ::LAB_00e04e13::
    quest:CreateThread("WatchForPickpocketing", {args = {me}})  -- native parent-quest worker WatchForPickpocketing, the entity's own thing captured
end

-- DarkwoodTrader.OnPersist (retail 0x00e05330)
function OnPersist(quest, me, context)
    quest:SetStateInt("BrainState", quest:PersistTransferInt(context, "BrainState", quest:GetStateInt("BrainState") or 0))
end

-- DarkwoodTrader.OnPredicateFail (retail 0x00e01760)
function OnPredicateFail(quest, me)
    local scratchValue
    if quest:GetTimer(incubationTime) < 1 then
        return
    end
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") - 1)
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
    if not me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        if quest:GetStateInt("TradersStillAliveCounter") == 2 then
            scratchValue = 1
        else
            if quest:GetStateInt("TradersStillAliveCounter") ~= 1 then goto LAB_00e018e4 end
            scratchValue = 0
        end
        helpers.MakeTraderComment(quest, me, "LAST_TRADER", me, scratchValue)
    else
        helpers.MakeTraderComment(quest, me, "HERO_KILLED_TRADER", quest:GetNearestWithScriptName(me, "DarkwoodTrader"), 0)
    end
    ::LAB_00e018e4::
    quest:SetMasterGameState("DarkwoodAllTradersAlive", false)
end

-- DarkwoodTrader.SetBrainState (retail 0x00e0a510)
function SetBrainState(quest, me, brainState)
    local hero = quest:GetHero()
    brainState_ = brainState
    local addQuestInfoBarHealth = brainState
    if brainState == 2 then
        local predicateResult = quest:IsActiveThreadTerminating()
        addQuestInfoBarHealth = predicateResult
        if not predicateResult then
            quest:EntityFollowThing(me, hero, 3.0, true)
            quest:SetEntityAsRegionFollowing(hero, me, true)
            quest:DisplayQuestInfo(true)
            local getDataString = me:GetDataString()
            if not (getDataString ~= nil and getDataString == "INFECTED") then
                local getDataString2 = me:GetDataString()
                if not (getDataString2 ~= nil and getDataString2 == "SCARED") then
                    local getDataString3 = me:GetDataString()
                    local brainStateFlag = getDataString3 ~= nil and getDataString3 == "FRIENDLY"
                    addQuestInfoBarHealth = brainStateFlag
                    if brainStateFlag then
                        local predicateResult2 = quest:IsActiveThreadTerminating()
                        addQuestInfoBarHealth = predicateResult2
                        if not predicateResult2 then
                            addQuestInfoBarHealth = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER_HAT_02", 1.0)
                            traderHealthID = addQuestInfoBarHealth
                        end
                    end
                else
                    local predicateResult3 = quest:IsActiveThreadTerminating()
                    addQuestInfoBarHealth = predicateResult3
                    if not predicateResult3 then
                        traderHealthID = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER_HAT_01", 1.0)
                        return quest:EntitySetAsScared(me, true)
                    end
                end
            else
                local predicateResult4 = quest:IsActiveThreadTerminating()
                addQuestInfoBarHealth = predicateResult4
                if not predicateResult4 then
                    traderHealthID = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER", 1.0)
                    infected = 1
                    if not quest:GetStateBool("SavedNearEnd") then
                        local predicateResult5 = quest:IsActiveThreadTerminating()
                        addQuestInfoBarHealth = predicateResult5
                        if not predicateResult5 then
                            return quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderToBalverineDelay))
                        end
                    else
                        local predicateResult6 = quest:IsActiveThreadTerminating()
                        addQuestInfoBarHealth = predicateResult6
                        if not predicateResult6 then
                            return quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderLowInfectionTime))
                        end
                    end
                end
            end
        end
    end
    return addQuestInfoBarHealth
end

