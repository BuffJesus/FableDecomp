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
local regulateBanterComment_, incubationTime_, regulateFollowStateComment_, brainState_, leadToCamp
local traderHealthID, inSafeZone, greetedBuddy, infected, currentAIState, previousAIState

-- DarkwoodTrader.Main (retail 0x00e07640)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue5, scratchValue8, scratchValue, questionAnswer
    local local_cutscene_furthest, local_cutscene_nearest, sequence, sequence3, hero, getPos, hero2
    local scratchValue30, scratchValue32, hero11, hero12, actorMap, actorMap2, dataString9, getPos2
    local getPos3, getPos4, getPos5, getPos6, getPos7, thing, mkDtbeCutscenetrigger, scratchValue40
    local localCutsceneNearestControl, actorMap3, newResource, localCutsceneFurthestControl
    local regulateBanterComment = regulateBanterComment_
    local commentTimer = quest:GetStateInt("CommentTimer")
    local incubationTime = incubationTime_
    local regulateFollowStateComment = regulateFollowStateComment_
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    while not quest:GetStateBool("IntroFinished") do
        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
        if me:IsTalkedToByHero() and quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
    if not (brainState_ ~= 1) then
        goto FLOW_past_lab_00e083cd
    end
    ::LAB_00e083cd::
    SetBrainState(quest, me, 2)
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + 1)
    quest:SetTimer(regulateBanterComment, 30)
    if not quest:IsActiveThreadTerminating() then
        repeat
            if not (leadToCamp or (not quest:IsRegionLoaded("BarrowFields"))) then
                if quest:IsActiveThreadTerminating() then break end
                quest:EntityStopFollowing(me)
                hero = quest:GetHero()
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, false)
                quest:SetTimer(commentTimer, 0)
                helpers.MakeTraderComment(quest, me, "IN_BARROW_FIELD", me, 0)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                end
                if quest:IsActiveThreadTerminating() then break end
                local tradersStopHere = quest:GetThingWithScriptName("M_TradersStopHere")
                scratchValue2 = false
                scratchValue8 = 0
                me:ClearCommands()
                if not (tradersStopHere ~= nil and not tradersStopHere:IsNull()) then
                    getPos2 = {x = 0, y = 0, z = 0}
                else
                    getPos2 = tradersStopHere:GetPos()
                end
                me:MoveToPosition(getPos2, 1.0, ENTITY_MOVE_RUN, false, true)
                while not quest:IsDistanceBetweenThingsUnder(me, tradersStopHere, 2.0) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                    local scratchValue7 = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(me, tradersStopHere)))
                    local hero3 = quest:GetHero()
                    scratchValue5 = scratchValue7 < resources:DistanceBetweenThings(hero3, tradersStopHere)
                    local hero4 = quest:GetHero()
                    if quest:IsDistanceBetweenThingsUnder(me, hero4, 9.0) or not scratchValue5 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                        local hero5 = quest:GetHero()
                        if quest:IsDistanceBetweenThingsUnder(me, hero5, 5.0) or not scratchValue5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                            if scratchValue8 == 0 then scratchValue2 = false; goto continue_1 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                            me:ClearCommands()
                            if not (tradersStopHere ~= nil and not tradersStopHere:IsNull()) then
                                getPos3 = {x = 0, y = 0, z = 0}
                            else
                                getPos3 = tradersStopHere:GetPos()
                            end
                            me:MoveToPosition(getPos3, 1.0, ENTITY_MOVE_RUN, false, true)
                            scratchValue8 = 0
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                            if scratchValue8 ~= 1 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                                me:ClearCommands()
                                if not (tradersStopHere ~= nil and not tradersStopHere:IsNull()) then
                                    getPos4 = {x = 0, y = 0, z = 0}
                                else
                                    getPos4 = tradersStopHere:GetPos()
                                end
                                me:MoveToPosition(getPos4, 1.0, ENTITY_MOVE_WALK, false, true)
                                scratchValue8 = 1
                            end
                        end
                        scratchValue2 = false
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                        me:ClearCommands()
                        me:ClearAllActions()
                        if not scratchValue2 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                            local hero6 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, hero6, false)
                            local conversationId = quest:AddNewConversation(me, false, false)
                            local hero7 = quest:GetHero()
                            quest:AddPersonToConversation(conversationId, hero7)
                            hero = quest:GetHero()
                            local scratchValue18 = ("TEXT_QST_067_" .. me:GetDataString()) .. "_FOLLOW_ME"
                            quest:AddLineToConversation(conversationId, scratchValue18, me, hero, false)
                            quest:Pause(1.0)
                            scratchValue2 = true
                            scratchValue8 = 2
                        end
                    end
                    ::continue_1::
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                hero2 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero2, false)
                helpers.MakeTraderComment(quest, me, "THANKS_10", me, 0)
                leadToCamp = true
            end
            if quest:GetStateBool("EndStarted") then
                if quest:IsActiveThreadTerminating() then break end
                quest:RemoveQuestInfoElement(traderHealthID)
                quest:MiniMapRemoveMarker(me)
                quest:EntityStopFollowing(me)
                hero = quest:GetHero()
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, false)
                local fret_02 = quest:GetHealth(me)
                quest:ModifyThingHealth(me, string.unpack("<f", string.pack("<f", 30.0 - fret_02)), false)
                resources:PrepareResource(resource)
                scratchValue2 = resources:TryAcquire(resource, me, 4)
                goto LAB_00e09fd9
            end
            if quest:GetStateBool("TradersShouldBeScared") then
                if quest:IsActiveThreadTerminating() then break end
                local runOffPos = quest:GetThingWithScriptName("DTE_RunOffPos")
                quest:EntityStopFollowing(me)
                hero = quest:GetHero()
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                if not (runOffPos ~= nil and not runOffPos:IsNull()) then
                    getPos5 = {x = 0, y = 0, z = 0}
                else
                    getPos5 = runOffPos:GetPos()
                end
                newResource = {x = getPos5.x, y = getPos5.y, z = getPos5.z}
                local scratchValue29 = resources:ScriptThing(resource)
                if scratchValue29 ~= nil and scratchValue29:IsDistanceFromPositionOver(newResource, 3.0) then
                    repeat
                        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                        me:MoveToPosition(newResource, 1.0, ENTITY_MOVE_RUN, false, true)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                        scratchValue30 = resources:ScriptThing(resource)
                    until not (scratchValue30 ~= nil and scratchValue30:IsDistanceFromPositionOver(newResource, 3.0))
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                while quest:GetStateBool("TradersShouldBeScared") do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                resources:PrepareResource(resource)
                local hero8 = quest:GetHero()
                quest:EntityFollowThing(me, hero8, 3.0, true)
                hero = quest:GetHero()
                quest:SetEntityAsRegionFollowing(hero, me, true)
                local hero9 = quest:GetHero()
                scratchValue2 = quest:IsDistanceBetweenThingsUnder(me, hero9, 20.0)
                while not scratchValue2 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                    local hero10 = quest:GetHero()
                    scratchValue2 = quest:IsDistanceBetweenThingsUnder(me, hero10, 20.0)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                hero2 = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero2)
                hero = quest:GetHero()
                local scratchValue20 = ("TEXT_QST_067_" .. me:GetDataString()) .. "_KILLED_EARTH_TROLL"
                quest:AddLineToConversation(conversationId2, scratchValue20, me, hero, false)
            end
            if not (inSafeZone or (not quest:IsRegionLoaded("Darkwood4"))) then
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
            if not (greetedBuddy or (not quest:IsRegionLoaded("Darkwood4"))) then
                if quest:IsActiveThreadTerminating() then break end
                hero2 = quest:GetHero()
                local dataString = quest:GetNearestWithScriptName(hero2, "DarkwoodTrader"):GetDataString()
                quest:SetStateString("TraderToTalk", dataString)
                thing = quest:GetThingWithScriptName("TE_CampTrader_A")
                if thing ~= nil and thing:IsAlive() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                    quest:EntityStopFollowing(me)
                    hero = quest:GetHero()
                    quest:SetEntityAsRegionFollowing(hero, me, false)
                    resources:PrepareResource(resource)
                    while not resources:TryAcquire(resource, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                    if not (thing ~= nil and not thing:IsNull()) then
                        getPos6 = {x = 0, y = 0, z = 0}
                    else
                        getPos6 = thing:GetPos()
                    end
                    localCutsceneNearestControl = {x = getPos6.x, y = getPos6.y, z = getPos6.z}
                    local scratchValue31 = resources:ScriptThing(resource)
                    if scratchValue31 ~= nil and scratchValue31:IsDistanceFromPositionOver(localCutsceneNearestControl, 5.0) then
                        repeat
                            if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                            me:MoveToPosition(localCutsceneNearestControl, 3.0, ENTITY_MOVE_WALK, false, true)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                            scratchValue32 = resources:ScriptThing(resource)
                        until not (scratchValue32 ~= nil and scratchValue32:IsDistanceFromPositionOver(localCutsceneNearestControl, 5.0))
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                    getPos = me:GetPos()
                    local scratchValue39 = quest:CreateEffectAtPos("HEAL_LEVEL2", getPos, 0.0, false)
                    quest:ModifyThingHealth(me, 1000.0, false)
                    if me:GetDataString() ~= quest:GetStateString("TraderToTalk") then
                        if not quest:IsActiveThreadTerminating() then
                            resources:PlayAnimationWithNativeArgument5(resource, "STANDARD_WAVE", false, false, false, true, false, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e09493 end
                            end
                            if not quest:IsActiveThreadTerminating() then quest:Pause(2.0); goto LAB_00e09493 end
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        local conversationId3 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId3, thing)
                        local scratchValue33 = thing
                        local scratchValue22 = ("TEXT_QST_067_" .. me:GetDataString()) .. "_GREET_ALLY_10"
                        quest:AddLineToConversation(conversationId3, scratchValue22, me, scratchValue33, false)
                        local scratchValue34 = thing
                        local scratchValue24 = ("TEXT_QST_067_" .. me:GetDataString()) .. "_GREET_ALLY_RESPONSE"
                        quest:AddLineToConversation(conversationId3, scratchValue24, scratchValue34, me, false)
                        local scratchValue35 = thing
                        local scratchValue26 = ("TEXT_QST_067_" .. me:GetDataString()) .. "_GREET_ALLY_20"
                        quest:AddLineToConversation(conversationId3, scratchValue26, me, scratchValue35, false)
                        quest:Pause(2.0)
                        quest:RemoveThing(scratchValue39, false, true)
                        while quest:IsConversationActive(conversationId3) do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e09493 end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e09493 end
                    end
                    goto FLOW_past_lab_00e09493
                    ::LAB_00e09493::
                    greetedBuddy = true
                    if scratchValue39 ~= nil and scratchValue39:IsAlive() then
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e09493 end
                        quest:RemoveThing(scratchValue39, false, true)
                    end
                    resources:PrepareResource(resource)
                    hero2 = quest:GetHero()
                    quest:EntityFollowThing(me, hero2, 3.0, true)
                    hero = quest:GetHero()
                    quest:SetEntityAsRegionFollowing(hero, me, true)
                    goto LAB_00e09511
                    ::FLOW_past_lab_00e09493::
                    resources:ReleaseResource(resource)
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
                    resources:PrepareResource(resource)
                    quest:CreateThread("TurnToBalv", {args = {me}})  -- native parent-quest worker TurnToBalv, the entity's own thing captured
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                end
                break
            end
            local incubationTimeHigh = quest:GetStateInt("IncubationTimeHigh")
            sequence = quest:GetTimer(incubationTime) < incubationTimeHigh
            if sequence then
                scratchValue40 = 4
                sequence = infected < 4
            end
            if sequence then
                if quest:IsActiveThreadTerminating() then break end
                scratchValue2 = helpers.MakeTraderComment(quest, me, "INFECTED_HIGH", me, 0)
                goto LAB_00e096c7
            else
                local incubationTimeMedium = quest:GetStateInt("IncubationTimeMedium")
                sequence3 = quest:GetTimer(incubationTime) < incubationTimeMedium
                if sequence3 then
                    scratchValue40 = 3
                    sequence3 = infected < 3
                end
                if sequence3 then
                    if not quest:IsActiveThreadTerminating() then scratchValue2 = helpers.MakeTraderComment(quest, me, "INFECTED_MEDIUM", me, 0); goto LAB_00e096c7 end
                    break
                end
                local incubationTimeLow = quest:GetStateInt("IncubationTimeLow")
                if quest:GetTimer(incubationTime) < incubationTimeLow and infected < 2 then
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
                infected = scratchValue40
            end
            ::FLOW_past_lab_00e096c7::
            if me:MsgIsHitBy("") then
                goto LAB_00e09848
            else
                if me:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00e09848 end
                goto LAB_00e09886
            end
            goto FLOW_past_lab_00e09848
            ::LAB_00e09848::
            scratchValue2 = true
            if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e09886 end
            ::FLOW_past_lab_00e09848::
            goto FLOW_past_lab_00e09886
            ::LAB_00e09886::
            scratchValue2 = false
            ::FLOW_past_lab_00e09886::
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetTimer(regulateFollowStateComment, 5)
                if quest:GetHealth(me) <= 5.0 then
                    goto LAB_00e099dc
                else
                    if me:MsgIsHitByHero() then goto LAB_00e099dc end
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e099dc end
                    scratchValue2 = true
                end
                goto FLOW_past_lab_00e099dc
                ::LAB_00e099dc::
                scratchValue2 = false
                ::FLOW_past_lab_00e099dc::
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then break end
                    helpers.MakeTraderComment(quest, me, "UNDER_ATTACK", me, 0)
                    scratchValue = 3
                else
                    if quest:GetHealth(me) <= 5.0 then
                        goto LAB_00e09b4f
                    else
                        if not me:MsgIsHitByHero() then
                            if not (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL))) then
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
                    scratchValue = 2
                end
                quest:SetTimer(commentTimer, scratchValue)
            end
            ::LAB_00e09c13::
            if currentAIState == previousAIState then
                if quest:GetTimer(regulateBanterComment) == 0 and not leadToCamp then
                    if quest:IsActiveThreadTerminating() then break end
                    if quest:GetStateInt("TradersStillAliveCounter") == 1 then
                        if quest:IsActiveThreadTerminating() then break end
                        scratchValue5 = helpers.MakeTraderComment(quest, me, "BANTER_SOLO", me, 0)
                    else
                        if quest:IsActiveThreadTerminating() then break end
                        local switch1 = quest:GetStateInt("TimesBantered")
                        repeat
                            if switch1 == 0 then
                                scratchValue5 = helpers.MakeTraderComment(quest, me, "BANTER_FIRST", me, 1)
                                break
                            elseif switch1 == 1 then
                                scratchValue5 = helpers.MakeTraderComment(quest, me, "BANTER_SECOND", me, 1)
                                break
                            elseif switch1 == 2 then
                                scratchValue5 = helpers.MakeTraderComment(quest, me, "BANTER_THIRD", me, 1)
                                break
                            elseif switch1 == 3 then
                                scratchValue5 = helpers.MakeTraderComment(quest, me, "BANTER_FOURTH", me, 1)
                                break
                            else
                                scratchValue5 = helpers.MakeTraderComment(quest, me, "BANTER", me, 0)
                            end
                        until true
                    end
                    if not scratchValue5 then
                        if quest:IsActiveThreadTerminating() then break end
                        local randomChoice = quest:RetailRandModulo(32768)
                        quest:SetTimer(regulateBanterComment, randomChoice % 15 + 25)
                    else
                        if quest:IsActiveThreadTerminating() then break end
                        local randomChoice2 = quest:RetailRandModulo(32768)
                        quest:SetTimer(regulateBanterComment, randomChoice2 % 20 + 30)
                        quest:SetStateInt("TimesBantered", quest:GetStateInt("TimesBantered") + 1)
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then break end
                if quest:GetTimer(regulateFollowStateComment) == 0 and currentAIState == 2 then
                    if quest:IsActiveThreadTerminating() then break end
                    helpers.MakeTraderComment(quest, me, "FOLLOWING", me, 0)
                    quest:SetTimer(regulateFollowStateComment, 10)
                end
            end
            previousAIState = currentAIState
            local getCurrentStateGroupType = me:GetCurrentStateGroupType()
            currentAIState = getCurrentStateGroupType
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                do return end
            end
        until false
    end
    resources:ReleaseResource(resource)
    do return end
    ::FLOW_past_lab_00e083cd::
    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
    quest:SetThingAsConscious(me, false, "")
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    while not quest:GetStateBool("InfectedTraderCanGetUp") do
        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
    quest:EntitySetAsDamageable(me, true)
    quest:EntitySetTargetable(me, true)
    quest:SetThingAsConscious(me, true, "")
    mkDtbeCutscenetrigger = quest:GetThingWithScriptName("MK_DTBE_CUTSCENETRIGGER")
    hero11 = quest:GetHero()
    if quest:IsDistanceBetweenThingsOver(mkDtbeCutscenetrigger, hero11, 5.0) then
        repeat
            if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
            if not quest:GetStateBool("MissionFailed") then
                hero12 = quest:GetHero()
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                local mkDtebInfexit = quest:GetThingWithScriptName("MK_DTEB_INFEXIT")
                if mkDtebInfexit == nil then
                    getPos7 = {x = 0, y = 0, z = 0}
                else
                    getPos7 = mkDtebInfexit:GetPos()
                end
                thing = {x = getPos7.x, y = getPos7.y, z = getPos7.z}
                local scratchValue36 = resources:ScriptThing(resource)
                scratchValue5 = scratchValue36 ~= nil and scratchValue36:IsDistanceFromPositionOver(thing, 2.0)
                while scratchValue5 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                    me:MoveToPosition(thing, 0, ENTITY_MOVE_RUN, false, true)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                    local scratchValue37 = resources:ScriptThing(resource)
                    scratchValue5 = scratchValue37 ~= nil and scratchValue37:IsDistanceFromPositionOver(thing, 2.0)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                quest:FadeOutAndKillEntity(me, true, 2.0, true)
                hero12 = quest:GetHero()
            end
        until not quest:IsDistanceBetweenThingsOver(mkDtbeCutscenetrigger, hero12, 5.0)
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue5 = 0
        if me:IsDead() or quest:GetHealth(me) <= 0.0 then
            while true do
                if quest:IsActiveThreadTerminating() then break end
                quest:NewScriptFrame(me)
            end
        else
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local_cutscene_nearest = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
            local_cutscene_furthest = quest:GetFurthestWithScriptName(me, "DarkwoodTrader")
            newResource = resources:NewResource()
            localCutsceneNearestControl = resources:NewResource()
            localCutsceneFurthestControl = resources:NewResource()
            resources:PrepareResource(newResource)
            getPos = newResource
            hero2 = quest:GetHero()
            while not resources:TryAcquire(getPos, hero2, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e07db9 end
                getPos = newResource
                hero2 = quest:GetHero()
            end
            if not quest:IsActiveThreadTerminating() then
                if local_cutscene_nearest ~= nil and local_cutscene_nearest:IsEqualTo(local_cutscene_furthest) then
                    if not quest:IsActiveThreadTerminating() then
                        local_cutscene_furthest = nil
                    else
                        resources:ReleaseResource(localCutsceneFurthestControl)
                        resources:ReleaseResource(localCutsceneNearestControl)
                        resources:ReleaseResource(newResource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                    end
                end
                if local_cutscene_nearest:GetDataString() == "SCARED" then
                    goto LAB_00e07cac
                else
                    scratchValue2 = true
                    if not (local_cutscene_furthest ~= nil and local_cutscene_furthest:IsAlive()) then goto LAB_00e07cac end
                end
                goto FLOW_past_lab_00e07cac
                ::LAB_00e07cac::
                scratchValue2 = false
                ::FLOW_past_lab_00e07cac::
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e07db9 end
                    local native_cutscene_swap = local_cutscene_nearest
                    local_cutscene_nearest = local_cutscene_furthest
                    local_cutscene_furthest = native_cutscene_swap
                end
                quest:FixMovieSequenceCamera(true)
                actorMap3 = resources:NewActorMap()
                resources:SetActor(actorMap3, "HERO", newResource)
                resources:SetActor(actorMap3, "TRADERI", resource)
                if not (local_cutscene_nearest ~= nil and local_cutscene_nearest:IsAlive()) then
                    goto LAB_00e081e9
                elseif not quest:IsActiveThreadTerminating() then
                    if local_cutscene_furthest ~= nil and local_cutscene_furthest:IsAlive() then
                        if not quest:IsActiveThreadTerminating() then
                            resources:PrepareResource(localCutsceneNearestControl)
                            while not resources:TryAcquire(localCutsceneNearestControl, local_cutscene_nearest, 4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00e07e30 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e081e9 end
                            resources:PrepareResource(localCutsceneFurthestControl)
                            while not resources:TryAcquire(localCutsceneFurthestControl, local_cutscene_furthest, 4) do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e081e9 end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                resources:SetActor(actorMap3, "TRADERS", localCutsceneNearestControl)
                                resources:SetActor(actorMap3, "TRADERN", localCutsceneFurthestControl)
                                actorMap = "CS_DARKWOOD_TRADER_INFECTED_BOTH"
                                goto LAB_00e081c2
                            end
                        end
                        ::LAB_00e07e30::
                        resources:DestroyActorMap(actorMap3)
                        goto LAB_00e07db9
                    end
                    dataString9 = local_cutscene_nearest:GetDataString()
                    if not (dataString9 ~= nil and dataString9 == "SCARED") then
                        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap3); goto LAB_00e07db9 end
                        resources:PrepareResource(localCutsceneFurthestControl)
                        while not resources:TryAcquire(localCutsceneFurthestControl, local_cutscene_nearest, 4) do
                            if not quest:NewScriptFrame(me) then resources:DestroyActorMap(actorMap3); goto LAB_00e07db9 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap3, "TRADERN", localCutsceneFurthestControl)
                            actorMap = "CS_DARKWOOD_TRADER_INFECTED_NORMAL"
                            goto LAB_00e081c2
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(localCutsceneNearestControl)
                        while not resources:TryAcquire(localCutsceneNearestControl, local_cutscene_nearest, 4) do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e081e9 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap3, "TRADERS", localCutsceneNearestControl)
                            actorMap = "CS_DARKWOOD_TRADER_INFECTED_SCARED"
                            goto LAB_00e081c2
                        end
                        goto FLOW_hoist_lab_00e081c2_1
                    end
                    goto FLOW_past_lab_00e081c2
                    ::LAB_00e081c2::
                    resources:RunMacro(actorMap, actorMap3, false, true)
                    goto LAB_00e081e9
                    ::FLOW_hoist_lab_00e081c2_1::
                    resources:DestroyActorMap(actorMap3)
                    goto LAB_00e07db9
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
                        resources:DestroyActorMap(actorMap3)
                        goto LAB_00e07db9
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if scratchValue2 then
                            resources:DestroyActorMap(actorMap3)
                            goto LAB_00e07db9
                        end
                        scratchValue5 = 1
                        actorMap2 = "CS_DARKWOOD_TRADER_INFECTED_JOINS"
                    else
                        if scratchValue2 then goto FLOW_past_lab_00e081e9 end
                        actorMap2 = "CS_DARKWOOD_TRADER_INFECTED_LEAVES"
                    end
                    resources:RunMacro(actorMap2, actorMap3, false, true)
                    quest:FixMovieSequenceCamera(false)
                    resources:DestroyActorMap(actorMap3)
                    resources:ReleaseResource(localCutsceneFurthestControl)
                    resources:ReleaseResource(localCutsceneNearestControl)
                    resources:ReleaseResource(newResource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    if scratchValue5 == 0 then
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TraderEscortLeaveTraderMorality))
                            quest:RemoveThing(me, false, true)
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(resource)
                        quest:ClearThingHasInformation(me)
                        goto LAB_00e083cd
                    end
                    goto LAB_00e085f8
                end
                ::FLOW_past_lab_00e081e9::
                resources:DestroyActorMap(actorMap3)
            end
            ::LAB_00e07db9::
            resources:ReleaseResource(localCutsceneFurthestControl)
            resources:ReleaseResource(localCutsceneNearestControl)
            resources:ReleaseResource(newResource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
    end
    ::LAB_00e085f8::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e09fd9::
    if not scratchValue2 then
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        scratchValue2 = resources:TryAcquire(resource, me, 4)
        goto LAB_00e09fd9
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    while quest:IsDistanceBetweenThingsOver(me, actorMap3, 7.0) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:MoveToThing(hero2, 4.0, ENTITY_MOVE_WALK)
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:PrepareResource(resource)
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            localCutsceneFurthestControl = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scriptThing = resources:ScriptThing(resource)
            local fret_03 = quest:GetHealth(scriptThing)
            if 0.0 < fret_03 then
                local getDataString = ("TEXT_QST_067_" .. me:GetDataString()) .. "_THANKS"
                local hero13 = quest:GetHero()
                if not me:Speak(hero13, getDataString, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e0a33a end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e0a33a end
                    goto FLOW_past_lab_00e0a33a
                end
                ::LAB_00e0a33a::
                resources:DestroyMovie(localCutsceneFurthestControl)
                break
                ::FLOW_past_lab_00e0a33a::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(localCutsceneFurthestControl)
            resources:PrepareResource(resource)
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    until false
    resources:ReleaseResource(resource)
end

-- DarkwoodTrader.Init (retail 0x00e04bd0)
function Init(quest, me)
    incubationTime_ = quest:RegisterTimer()  -- native constructor: CTimer member
    regulateFollowStateComment_ = quest:RegisterTimer()  -- native constructor: CTimer member
    regulateBanterComment_ = quest:RegisterTimer()  -- native constructor: CTimer member
    local scratchValue
    brainState_ = 0
    currentAIState = 0
    previousAIState = 0
    if me:GetDataString() == "INFECTED" then
        brainState_ = 1
    end
    infected = 0
    quest:SetTimer(incubationTime_, 0)
    greetedBuddy = false
    leadToCamp = false
    inSafeZone = false
    quest:SetTimer(regulateFollowStateComment_, 0)
    quest:SetTimer(regulateBanterComment_, 60)
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
    local brainState = brainState_ or 0
    brainState_ = quest:PersistTransferInt(context, "BrainState", brainState)
end

-- DarkwoodTrader.OnPredicateFail (retail 0x00e01760)
function OnPredicateFail(quest, me)
    local scratchValue
    if quest:GetTimer(incubationTime_) < 1 then
        return
    end
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") - 1)
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
    if not me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        if quest:GetStateInt("TradersStillAliveCounter") == 2 then
            scratchValue = 1
        else
            if quest:GetStateInt("TradersStillAliveCounter") ~= 1 then quest:SetMasterGameState("DarkwoodAllTradersAlive", false); return end
            scratchValue = 0
        end
        helpers.MakeTraderComment(quest, me, "LAST_TRADER", me, scratchValue)
    else
        local darkwoodTrader = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
        helpers.MakeTraderComment(quest, me, "HERO_KILLED_TRADER", darkwoodTrader, 0)
    end
    quest:SetMasterGameState("DarkwoodAllTradersAlive", false)
end

-- DarkwoodTrader.SetBrainState (retail 0x00e0a510)
function SetBrainState(quest, me, brainState)
    local incubationTime = incubationTime_
    brainState_ = brainState
    local setTimer
    if brainState ~= 2 then return end
    if quest:IsActiveThreadTerminating() then return end
    local hero = quest:GetHero()
    quest:EntityFollowThing(me, hero, 3.0, true)
    local pFollower_00 = quest:GetHero()
    quest:SetEntityAsRegionFollowing(pFollower_00, me, true)
    quest:DisplayQuestInfo(true)
    local dataString = me:GetDataString()
    if not (dataString ~= nil and dataString == "INFECTED") then
        local dataString2 = me:GetDataString()
        if not (dataString2 ~= nil and dataString2 == "SCARED") then
            local dataString3 = me:GetDataString()
            if not (dataString3 ~= nil and dataString3 == "FRIENDLY") then return end
            if quest:IsActiveThreadTerminating() then return end
            traderHealthID = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER_HAT_02", 1.0)
        else
            if quest:IsActiveThreadTerminating() then return end
            traderHealthID = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER_HAT_01", 1.0)
            quest:EntitySetAsScared(me, true)
            return
        end
    else
        if quest:IsActiveThreadTerminating() then return end
        traderHealthID = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER", 1.0)
        infected = 1
        if not quest:GetStateBool("SavedNearEnd") then
            if quest:IsActiveThreadTerminating() then return end
            setTimer = quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderToBalverineDelay))
            return
        else
            if quest:IsActiveThreadTerminating() then return end
            setTimer = quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderLowInfectionTime))
            return
        end
    end
end

