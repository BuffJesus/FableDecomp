-- ScytheNearOracle.lua
-- Entity script for ScytheNearOracle (QS_ScytheInfo.c)
-- Functional parity port of NScript::CQS_ScytheInfoScript::CScytheNearOracle

local STATE_WAITING  = 1
local STATE_HIT      = 2
local STATE_CUTSCENE = 3
local STATE_DONE     = 4

function Init(quest, me)
    quest:SetIsThingForcePushable(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, false, false)
end

function Main(quest, me)
    -- Cache Hero and Marker references outside the loop to avoid memory leaks
    local hero = quest:GetHero()
    if not hero then
        quest:Log("ScytheNearOracle: Could not get Hero. Aborting.")
        return
    end

    local oracleMarker = quest:GetThingWithScriptName("MK_OW_SCYTHE3")

    -- Acquire script control and take exclusive control
    me:AcquireControl()
    me:TakeExclusiveControl()

    -- Add quest orb marker to minimap
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")

    -- Initial orientation: face the Oracle marker
    if oracleMarker then
        quest:EntitySetFacingAngleTowardsThing(me, oracleMarker, false)
    end

    -- Cooldown timer for turning to face the Hero
    local turningTimer = quest:RegisterTimer()

    local state = STATE_WAITING

    while state ~= STATE_DONE do
        if state == STATE_WAITING then
            -- Face Hero if within 20m and cooldown timer has elapsed
            if quest:IsDistanceBetweenThingsUnder(me, hero, 20.0) and quest:GetTimer(turningTimer) <= 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero)
                quest:SetTimer(turningTimer, 2)
            end

            -- Proximity cutscene trigger if within 12m
            if quest:IsDistanceBetweenThingsUnder(me, hero, 12.0) then
                state = STATE_CUTSCENE
            else
                -- Hit check: reacted to damage or offensive abilities, excluding Heal Life
                local wasHit = me:MsgIsHitByHero()
                    or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())
                if wasHit then
                    state = STATE_HIT
                end
            end

        elseif state == STATE_HIT then
            -- Speak on-hit dialogue
            if quest:GetHealth(me) > 0.0 then
                me:SpeakAndWait("TEXT_QST_B03_SCYTHE_ON_HIT_RETURNED")
            end

            -- Ensure mutual alliance between Scythe and Hero
            quest:EntitySetThingAsAllyOfThing(me, hero)
            quest:EntitySetThingAsAllyOfThing(hero, me)

            state = STATE_WAITING

        elseif state == STATE_CUTSCENE then
            quest:MiniMapRemoveMarker(me)

            -- Release control handle before launching cutscene so PlayCutscene can acquire actors cleanly
            me:ReleaseControl()

            quest:PlayCutscene("CS_ORACLE_AWAKENS", { HERO = hero, SCYTHE = me })

            quest:SetStateBool("MissionSucceeded", true)
            quest:RemoveThing(me)

            state = STATE_DONE
            break
        end

        if not quest:NewScriptFrame(me) then
            break
        end
    end

    -- Clean up handle if loop terminated abnormally before cutscene
    if state ~= STATE_DONE then
        me:ReleaseControl()
    end
    quest:DeregisterTimer(turningTimer)
end
