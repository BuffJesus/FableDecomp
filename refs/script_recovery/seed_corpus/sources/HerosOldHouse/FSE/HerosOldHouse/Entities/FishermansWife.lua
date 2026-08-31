local MY_SCRIPT_NAME = "FishermansWife"

local initial_angle = 0.0
local talkedTo = false
local leavingHappy = false

function Init(quest, me)
    initial_angle = me:GetAngleXY()
    talkedTo = false
    leavingHappy = false
    quest:EntitySetTargetingType(me, 2)
    quest:SetThingHasInformation(me, false, false, false)

    if quest:GetStateBool("Helping") and not quest:GetStateBool("Helped") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
end

function OnPersist(quest, me, context)
    talkedTo = quest:PersistTransferBool(context, "TalkedTo", talkedTo)
    leavingHappy = quest:PersistTransferBool(context, "LeavingHappy", leavingHappy)
end

function OnPredicateFail(quest, me)
    quest:Log("FishermansWife: OnPredicateFail called.")
    if me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        quest:GiveHeroMorality(-0.02)
        quest:SetStateBool("WifeAttackedAndLeft", true)
        quest:Log("FishermansWife: Killed by Hero! Morality deducted.")
    end
end

function Main(quest, me)
    quest:Log("FishermansWife: Main() started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: FishermansWife Main - could not get Hero.")
        return
    end

    local bootyMarker = quest:GetThingWithScriptName("HiddenBooty")
    local leaveMarker = quest:GetThingWithScriptName("FisherWifeLeaveMarker")

    me:AcquireControl()
    quest:EntitySetAsKillable(me, false)
    me:SetIsPushableByHero(false)
    quest:EntitySetTargetingType(me, 2)
    quest:SetThingHasInformation(me, false, false, false)

    local wifeTimer = quest:RegisterTimer()
    quest:SetTimer(wifeTimer, 5)

    while true do
        -- If leaving happily after receiving gold
        if leavingHappy then
            if leaveMarker then
                me:MoveToPosition(leaveMarker:GetPos(), 3.0, 0) -- ENTITY_MOVE_WALK
            end

            while true do
                -- Periodic ambient aside chatter (does not trigger cinematic movie mode)
                if quest:GetTimer(wifeTimer) <= 0 and quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) then
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    quest:AddLineToConversation(convoID, "TEXT_QST_032_FISHERWIFE_SHOPPING_ASIDE", me, hero, false)
                    quest:SetTimer(wifeTimer, 15)
                end

                -- Talked to while leaving
                if me:IsTalkedToByHero() then
                    me:SpeakAndWait("TEXT_QST_032_FISHERWIFE_SHOPPING")
                    if leaveMarker then
                        me:MoveToPosition(leaveMarker:GetPos(), 3.0, 0)
                    end
                end

                -- Hit by Hero while leaving
                local wasHit = me:MsgIsHitByHero()
                    or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

                if wasHit then
                    me:SpeakAndWait("TEXT_QST_032_FISHERWIFE_ATTACKED_AFTER")
                    quest:EntitySetAsKillable(me, true)
                    quest:SetStateBool("WifeAttacked", true)
                    if leaveMarker then
                        me:MoveToPosition(leaveMarker:GetPos(), 3.0, 1) -- ENTITY_MOVE_RUN
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then
                                quest:DeregisterTimer(wifeTimer)
                                me:ReleaseControl()
                                return
                            end
                        end
                    end
                    quest:SetStateBool("WifeAttackedAndLeft", true)
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                    quest:DeregisterTimer(wifeTimer)
                    me:ReleaseControl()
                    return
                end

                -- Finished moving to leave marker
                if not me:IsPerformingScriptTask() then
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                    quest:DeregisterTimer(wifeTimer)
                    me:ReleaseControl()
                    return
                end

                if not quest:NewScriptFrame(me) then
                    quest:DeregisterTimer(wifeTimer)
                    me:ReleaseControl()
                    return
                end
            end
        end

        -- Normal state (before leaving)
        -- Periodic ambient crying (does not trigger cinematic movie mode)
        if quest:GetTimer(wifeTimer) <= 0 and quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) then
            local convoID = quest:StartAmbientConversation(me, hero, false, false)
            quest:AddLineToConversation(convoID, "TEXT_QST_032_FISHERWIFE_CRY", me, hero, false)
            quest:SetTimer(wifeTimer, 15)
        end

        -- Talked to by Hero
        if me:IsTalkedToByHero() then
            if quest:GetStateBool("Helped") then
                me:SpeakAndWait("TEXT_QST_032_FISHERWIFE_THANKS")
                quest:EntitySetFacingAngle(me, initial_angle, true)
                quest:EntitySetTargetingType(me, 2)
            -- Condition: Helping is true, treasure is DUG UP (spot is disabled), and hero has >= 500 gold
            elseif quest:GetStateBool("Helping") and (not bootyMarker or not quest:IsDiggingSpotEnabled(bootyMarker)) and quest:GetHeroGold() >= 500 then
                me:ReleaseControl()
                quest:StartCutscene({ HERO = hero, WIFE = me })

                local ans = quest:GiveHeroYesNoQuestion("TEXT_QST_032_FISHERWIFE_GOLD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                if ans == 1 then
                    quest:SetStateBool("Helped", true)
                    quest:RunCutscene("CS_GHOSTFISH_WIFE_SUCCESS", true, false)
                    quest:EndCutscene()
                    me:AcquireControl()

                    quest:GiveHeroMorality(0.02)
                    quest:AddItemToContainer(me, "OBJECT_GOLDBAG_MEDIUM_WITH_COINS_500")
                    quest:GiveHeroGold(-500)

                    local activeQuestName = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_03", "", "OakBay")

                    quest:ClearThingHasInformation(me)
                    quest:MiniMapRemoveMarker(me)

                    local ghost = quest:GetThingWithScriptName("GhostFisherman")
                    if ghost then
                        quest:MiniMapAddMarker(ghost, "HUD_ORB_QUEST_VIGNETTE")
                    end

                    quest:FadeScreenIn()
                    leavingHappy = true
                else
                    if talkedTo then
                        quest:EndCutscene()
                        me:AcquireControl()
                        me:SpeakAndWait("TEXT_QST_032_FISHERWIFE_REPEAT_GREET")
                    else
                        quest:RunCutscene("CS_GHOSTFISH_WIFE_INTRO", true, false)
                        quest:EndCutscene()
                        me:AcquireControl()
                        talkedTo = true
                    end
                    quest:EntitySetFacingAngle(me, initial_angle, true)
                    quest:EntitySetTargetingType(me, 2)
                end
            else
                if not talkedTo then
                    me:ReleaseControl()
                    quest:StartCutscene({ HERO = hero, WIFE = me })
                    quest:RunCutscene("CS_GHOSTFISH_WIFE_INTRO", true, false)
                    quest:EndCutscene()
                    me:AcquireControl()
                    talkedTo = true
                else
                    me:SpeakAndWait("TEXT_QST_032_FISHERWIFE_REPEAT_GREET")
                end
                quest:EntitySetFacingAngle(me, initial_angle, true)
                quest:EntitySetTargetingType(me, 2)
            end
        end

        -- Hit by Hero
        local wasHit = me:MsgIsHitByHero()
            or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

        if wasHit then
            me:SpeakAndWait("TEXT_QST_032_FISHERWIFE_ATTACKED")
            quest:EntitySetAsKillable(me, true)
            quest:SetStateBool("WifeAttacked", true)
            if leaveMarker then
                me:MoveToPosition(leaveMarker:GetPos(), 3.0, 1) -- ENTITY_MOVE_RUN
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then
                        quest:DeregisterTimer(wifeTimer)
                        me:ReleaseControl()
                        return
                    end
                end
            end
            quest:SetStateBool("WifeAttackedAndLeft", true)
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
            quest:DeregisterTimer(wifeTimer)
            me:ReleaseControl()
            return
        end

        if not quest:NewScriptFrame(me) then
            quest:DeregisterTimer(wifeTimer)
            me:ReleaseControl()
            return
        end
    end
end
