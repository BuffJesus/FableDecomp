-- ============================================================================
-- WaspHelper.lua
-- Main Quest: Q_WaspBoss
-- Functional recreation of NScript::CQ_WaspBossScript::CWaspHelper
-- ============================================================================

local ENTITY_MOVE_WALK = 0
local ENTITY_MOVE_RUN  = 1
local SCRIPT_AI_PRIORITY_HIGH = 3

local pleadedToHero = false
local leadToRegion  = false
local wavedOver     = false

function Init(quest, me)
    quest:Log("WaspHelper: Init() started.")
    pleadedToHero = false
    leadToRegion  = false
    wavedOver     = false
    quest:Log("WaspHelper: Init() completed. States reset.")
end

function OnPersist(quest, me, context)
    quest:Log("WaspHelper: OnPersist() triggered.")
    pleadedToHero = quest:PersistTransferBool(context, "PleadedToHero", pleadedToHero)
    leadToRegion  = quest:PersistTransferBool(context, "LeadToRegion", leadToRegion)
    wavedOver     = quest:PersistTransferBool(context, "WavedOver", wavedOver)
    quest:Log("WaspHelper: OnPersist() completed. pleadedToHero=" .. tostring(pleadedToHero) .. ", leadToRegion=" .. tostring(leadToRegion) .. ", wavedOver=" .. tostring(wavedOver))
end

function Main(quest, me)
    quest:Log("WaspHelper: Main() started.")

    -- 1. Entity lookups outside loops (memory leak prevention)
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: WaspHelper Main - could not get Hero!")
        return
    end

    local moveToPos = quest:GetThingWithScriptName("WaspGuardMoveToPos")
    if not moveToPos then
        quest:Log("!!! WARNING: WaspGuardMoveToPos not found in world!")
    end

    -- 2. Initial Control Acquisition at SCRIPT_AI_PRIORITY_HIGH (3)
    quest:Log("WaspHelper: [CRITICAL SECTION] Acquiring script control (priority HIGH=3)...")
    me:AcquireControl(SCRIPT_AI_PRIORITY_HIGH)
    quest:Log("WaspHelper: [CRITICAL SECTION] Script control acquired.")
    me:MakeBehavioral()
    quest:Log("WaspHelper: [CRITICAL SECTION] MakeBehavioral called, entity now behavioral.")

    local targetPos = moveToPos and moveToPos:GetPos() or nil
    local sayTimer = quest:RegisterTimer()
    quest:SetTimer(sayTimer, 0)
    local idleTimer = quest:RegisterTimer()
    quest:SetTimer(idleTimer, 10)

    -- ========================================================================
    -- PHASE 1: APPROACHING & PLEADING TO HERO (CWaspHelper_Main.c:692-849)
    -- ========================================================================
    if not pleadedToHero then
        quest:Log("WaspHelper: [PHASE 1] Starting approach to Hero with FollowThing...")
        me:FollowThing(hero, 1.0, true)

        local debugFrame = 0
        while not quest:IsDistanceBetweenThingsUnder(me, hero, 5.0) do
            debugFrame = debugFrame + 1
            if debugFrame % 30 == 0 then
                local d = quest:GetDistanceBetweenThings(me, hero)
                quest:Log(string.format("WaspHelper [APPROACHING] Frame %d: distToHero=%.2f", debugFrame, d))
            end

            -- Hit reaction
            if me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero()) then
                me:SetFriendsWithEverythingFlag(true)
            end

            -- If talked to early, break to plead
            if me:IsTalkedToByHero() then
                quest:Log("WaspHelper: Talked to by Hero while approaching.")
                break
            end

            if not quest:NewScriptFrame(me) then
                quest:Log("WaspHelper: NewScriptFrame returned false during approach. Exiting.")
                quest:DeregisterTimer(sayTimer)
                quest:DeregisterTimer(idleTimer)
                me:ReleaseControl()
                return
            end
        end

        quest:Log("WaspHelper: [PHASE 1] Met Hero. Clearing commands for plea...")
        me:ClearCommands()

        -- Check ReachedWaspHelper timer
        local timerID = quest:GetStateInt("ReachedWaspHelperTimerID")
        local timerExpired = false
        if timerID and timerID > 0 then
            timerExpired = (quest:GetTimer(timerID) <= 0)
        else
            timerExpired = (quest:GetStateInt("ReachedWaspHelperTimer") <= 0)
        end

        if timerExpired then
            quest:Log("WaspHelper: Speaking TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10 (Timer <= 0)")
            me:SpeakAndWait("TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10")
        else
            quest:Log("WaspHelper: Speaking TEXT_QST_072_HELPER_FOLLOW_ME_10 (Timer > 0)")
            me:SpeakAndWait("TEXT_QST_072_HELPER_FOLLOW_ME_10")
        end

        pleadedToHero = true
        quest:Log("WaspHelper: [PHASE 1] Plea completed. Transitioning to LEADING.")
    end

    -- ========================================================================
    -- PHASE 2: LEADING HERO TO PICNIC AREA GATE (CWaspHelper_Main.c:172-401)
    -- ========================================================================
    if not leadToRegion then
        quest:Log("WaspHelper: [PHASE 2] Starting lead path to WaspGuardMoveToPos...")
        local bLeadHeroToPointLatch = false
        local currentlyRunning = false

        if targetPos then
            me:ClearCommands()
            me:MoveToPosition(targetPos, 1.0, ENTITY_MOVE_RUN)
            currentlyRunning = true
            quest:Log("WaspHelper: [PHASE 2] Initial MoveToPosition (RUN) issued.")
        end

        quest:SetTimer(sayTimer, 0)
        local debugLeadFrame = 0

        while not quest:IsDistanceBetweenThingsUnder(me, moveToPos, 2.0) do
            if not quest:NewScriptFrame(me) then
                quest:Log("WaspHelper: NewScriptFrame returned false during LEADING loop. Exiting.")
                quest:DeregisterTimer(sayTimer)
                quest:DeregisterTimer(idleTimer)
                me:ReleaseControl()
                return
            end

            debugLeadFrame = debugLeadFrame + 1

            -- Hit reaction
            if me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero()) then
                me:SetFriendsWithEverythingFlag(true)
            end

            local wasTalkedTo = me:IsTalkedToByHero()

            -- Distance pacing calculations (matching CWaspHelper_Main.c:190-199)
            local distMeToGoal = quest:GetDistanceBetweenThings(me, moveToPos)
            local distHeroToGoal = quest:GetDistanceBetweenThings(hero, moveToPos)
            local isHeroFurther = (distHeroToGoal > distMeToGoal)
            local heroUnder9 = quest:IsDistanceBetweenThingsUnder(me, hero, 9.0)

            if debugLeadFrame % 30 == 0 then
                local distMeHero = quest:GetDistanceBetweenThings(me, hero)
                quest:Log(string.format("WaspHelper [LEADING] Frame %d: dMeHero=%.2f, dMeGoal=%.2f, dHeroGoal=%.2f, isFurther=%s, running=%s, taskRunning=%s",
                    debugLeadFrame, distMeHero, distMeToGoal, distHeroToGoal, tostring(isHeroFurther), tostring(currentlyRunning), tostring(me:IsPerformingScriptTask())))
            end

            -- Talked to while leading -> immediate ambient reminder
            if wasTalkedTo then
                quest:Log("WaspHelper: Hero talked to WaspHelper while leading. Playing ambient reminder.")
                local convoID = quest:StartAmbientConversation(me, hero, false, false)
                quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_GO_THIS_WAY", me, hero, false)
                quest:SetTimer(sayTimer, 8)
            -- HERO IS WITHIN 9m OR AHEAD OF ME (CWaspHelper_Main.c:199-275)
            elseif heroUnder9 or not isHeroFurther then
                local heroUnder65 = quest:IsDistanceBetweenThingsUnder(me, hero, 6.5)

                if heroUnder65 or not isHeroFurther then
                    -- Very close (<6.5m) or ahead -> RUN
                    if not currentlyRunning then
                        quest:Log("WaspHelper: Hero within 6.5m or ahead. Switching to RUN.")
                        if targetPos then
                            me:ClearCommands()
                            me:MoveToPosition(targetPos, 1.0, ENTITY_MOVE_RUN)
                        end
                        currentlyRunning = true
                    end
                elseif quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
                    -- 6.5m - 8.0m -> WALK
                    if currentlyRunning then
                        quest:Log("WaspHelper: Hero 6.5m-8.0m behind. Switching to WALK.")
                        if targetPos then
                            me:ClearCommands()
                            me:MoveToPosition(targetPos, 1.0, ENTITY_MOVE_WALK)
                        end
                        currentlyRunning = false
                    end
                end

                bLeadHeroToPointLatch = false

                -- Periodic guidance reminder every 8s while moving (CWaspHelper_Main.c:250-275)
                if quest:GetTimer(sayTimer) <= 0 then
                    quest:Log("WaspHelper: sayTimer expired while moving. Speaking TEXT_QST_072_HELPER_GO_THIS_WAY.")
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_GO_THIS_WAY", me, hero, false)
                    quest:SetTimer(sayTimer, 8)
                end
            -- HERO IS BEHIND > 9m (HERO STRAYED! CWaspHelper_Main.c:276-391)
            else
                me:ClearCommands()
                me:ClearAllActions()
                currentlyRunning = false

                -- First frame hero strayed: face hero, shout "Over here!", pause 1.0s (CWaspHelper_Main.c:282-314)
                if not bLeadHeroToPointLatch then
                    quest:Log("WaspHelper: Hero strayed (>9m behind). Shouting Over Here and pausing 1.0s...")
                    quest:EntitySetFacingAngleTowardsThing(me, hero)
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_OVER_HERE", me, hero, false)
                    quest:Pause(1.0)
                    quest:SetTimer(sayTimer, 5)
                    bLeadHeroToPointLatch = true
                -- Subsequent periodic shout & wave animations every 5s (CWaspHelper_Main.c:315-391)
                elseif quest:GetTimer(sayTimer) <= 0 then
                    quest:Log("WaspHelper: sayTimer expired while waiting. Calling hero over with shout and wave animations...")
                    quest:EntitySetFacingAngleTowardsThing(me, hero)
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_OVER_HERE", me, hero, false)

                    me:PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED")
                    if not quest:NewScriptFrame(me) then break end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then break end
                    end

                    quest:Pause(0.8)

                    me:PlayAnimation("ST_OPINION_APPROVAL_WAVING_AT_DISTANCE")
                    if not quest:NewScriptFrame(me) then break end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then break end
                    end

                    quest:SetTimer(sayTimer, 5)
                    quest:Log("WaspHelper: Stray animations completed. sayTimer set to 5s.")
                end
            end
        end

        leadToRegion = true
        quest:Log("WaspHelper: [PHASE 2] Reached gate! leadToRegion set to true.")
    end

    -- ========================================================================
    -- PHASE 3: ARRIVAL AT GATE (CWaspHelper_Main.c:402-511)
    -- ========================================================================
    me:ClearCommands()
    quest:SetIsPushableByHero(me, false)

    if not wavedOver then
        quest:Log("WaspHelper: [PHASE 3] Playing arrival wave and call over sequence...")
        local convoID = quest:StartAmbientConversation(me, hero, false, false)
        quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_THIS_WAY_10", me, hero, false)

        quest:EntitySetFacingAngleTowardsThing(me, hero)
        if not quest:NewScriptFrame(me) then return end
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then return end
        end

        me:PlayAnimation("STANDARD_WAVE")
        if not quest:NewScriptFrame(me) then return end
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then return end
        end

        if moveToPos then
            quest:EntitySetFacingAngleTowardsThing(me, moveToPos)
            if not quest:NewScriptFrame(me) then return end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then return end
            end
        end

        me:PlayAnimation("ST_CALL_OVER")
        if not quest:NewScriptFrame(me) then return end
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then return end
        end

        wavedOver = true
        quest:Log("WaspHelper: [PHASE 3] Arrival sequence complete. wavedOver set to true.")
    end

    -- ========================================================================
    -- PHASE 4: IDLE AT ENTRANCE (CWaspHelper_Main.c:512-689)
    -- ========================================================================
    quest:Log("WaspHelper: [PHASE 4] Entering IDLE_ENTRANCE loop...")
    quest:SetTimer(idleTimer, 10)

    while true do
        if not quest:NewScriptFrame(me) then
            quest:Log("WaspHelper: Script frame returned false in IDLE_ENTRANCE loop. Exiting.")
            break
        end

        if quest:GetStateBool("MissionSucceeded") then
            quest:Log("WaspHelper: Mission already succeeded. Removing helper.")
            quest:RemoveThing(me)
            break
        end

        if me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero()) then
            me:SetFriendsWithEverythingFlag(true)
        end

        local wasTalkedTo = me:IsTalkedToByHero()

        -- Talked to by hero at entrance -> SpeakAndWait
        if wasTalkedTo then
            quest:Log("WaspHelper: Hero talked to WaspHelper at entrance.")
            if quest:GetStateInt("SavedVillagerCount") >= 2 then
                me:SpeakAndWait("TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10")
            else
                me:SpeakAndWait("TEXT_QST_072_HELPER_NOT_MOVED_10")
            end
            quest:SetTimer(idleTimer, 10)
        -- Periodic idle reminder every 10s
        elseif quest:GetTimer(idleTimer) <= 0 then
            quest:Log("WaspHelper: idleTimer expired (10s). Playing periodic reminder sequence...")
            quest:EntitySetFacingAngleTowardsThing(me, hero)
            quest:SetTimer(idleTimer, 10)

            local convoID = quest:StartAmbientConversation(me, hero, false, false)
            if quest:GetStateInt("SavedVillagerCount") >= 2 then
                quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, hero, false)
            else
                quest:AddLineToConversation(convoID, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, hero, false)
            end

            quest:EntitySetFacingAngleTowardsThing(me, hero)
            if not quest:NewScriptFrame(me) then break end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then break end
            end

            me:PlayAnimation("STANDARD_WAVE")
            if not quest:NewScriptFrame(me) then break end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then break end
            end

            if moveToPos then
                quest:EntitySetFacingAngleTowardsThing(me, moveToPos)
                if not quest:NewScriptFrame(me) then break end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then break end
                end
            end

            me:PlayAnimation("ST_CALL_OVER")
            if not quest:NewScriptFrame(me) then break end
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then break end
            end
        end
    end

    quest:DeregisterTimer(sayTimer)
    quest:DeregisterTimer(idleTimer)
    me:ReleaseControl()
    quest:Log("WaspHelper: Main() ended.")
end
