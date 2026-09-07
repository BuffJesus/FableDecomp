local MY_SCRIPT_NAME = "BowerstoneIntroGuardToPosh"

local POSH_GUARD_PROXIMITY = 5.0

local STATE_MONITORING   = 1
local STATE_TURN_AWAY    = 2
local STATE_ALLOW_ENTER  = 3
local STATE_GATE_OPEN    = 4

function Init(quest, me)
    local arenaFinished = quest:GetMasterGameState("ArenaFinished")
    if not arenaFinished then
        arenaFinished = quest:IsQuestCompleted("Q_Arena")
    end
    quest:SetStateBool("ArenaFinished", arenaFinished or false)

    local hero = quest:GetHero()
    if hero then
        quest:EntitySetThingAsAllyOfThing(me, hero)
        quest:EntitySetThingAsAllyOfThing(hero, me)
    end

    quest:SetThingHasInformation(me, false, false, false)
end

function Main(quest, me)
    quest:Log("BowerstoneIntroGuardToPosh: Main() started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: BowerstoneIntroGuardToPosh Main - could not get Hero.")
        return
    end

    if not me:AcquireControl() then
        quest:Log("!!! ERROR: BowerstoneIntroGuardToPosh Main - failed to acquire control.")
        return
    end

    -- Wait until the Bowerstone South entrance speech has finished
    while not quest:GetStateBool("DoneGuardSpeech") do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    -- Face towards the designated marker with instant snap
    local faceMe = quest:GetThingWithScriptName("M_BTLI_GuardPoshFaceMe")
    if faceMe and faceMe:IsAlive() then
        if not quest:NewScriptFrame(me) then me:ReleaseControl() return end
        if not quest:NewScriptFrame(me) then me:ReleaseControl() return end
        quest:EntitySetFacingAngleTowardsThing(me, faceMe, true)
        if not quest:NewScriptFrame(me) then me:ReleaseControl() return end
        if not quest:NewScriptFrame(me) then me:ReleaseControl() return end
    end

    local currentState = STATE_MONITORING
    local wasNear = false

    while true do
        -- Sync ArenaFinished if completed in the master game state
        if not quest:GetStateBool("ArenaFinished") then
            if quest:GetMasterGameState("ArenaFinished") or quest:IsQuestCompleted("Q_Arena") then
                quest:SetStateBool("ArenaFinished", true)
            end
        end

        local poshComplete = quest:GetStateBool("PoshGuardComplete")

        if poshComplete then
            currentState = STATE_GATE_OPEN
        end

        if currentState == STATE_GATE_OPEN then
            -- Gate is open: Guard speaks reminder if Hero talks to him
            if me:IsTalkedToByHero() then
                if quest:GetHealth(me) > 0.0 then
                    me:SpeakAndWait("TEXT_QST_037_GUARD_POSH_REMINDER")
                end
            end
        else
            -- Check proximity to Hero
            local isNear = quest:IsDistanceBetweenThingsUnder(hero, me, POSH_GUARD_PROXIMITY)

            if isNear and not wasNear then
                wasNear = true

                if not quest:GetStateBool("ArenaFinished") then
                    currentState = STATE_TURN_AWAY
                    me:ReleaseControl()

                    quest:StartCutscene({ HERO = hero, Guard = me })
                    quest:RunCutscene("CS_BOWERSTONE_INTRO_POSH_LEAVE", true, false)
                    quest:EndCutscene()

                    me:AcquireControl()
                    currentState = STATE_MONITORING
                else
                    currentState = STATE_ALLOW_ENTER
                    me:ReleaseControl()

                    quest:StartCutscene({ HERO = hero, Guard = me })
                    quest:RunCutscene("CS_BOWERSTONE_INTRO_POSH_ENTER", true, false)
                    quest:EndCutscene()

                    quest:SetStateBool("PoshGuardComplete", true)
                    quest:EntityUnsetThingAsAllyOfThing(me, hero)
                    quest:EntityUnsetThingAsAllyOfThing(hero, me)
                    quest:ClearThingHasInformation(me)

                    currentState = STATE_GATE_OPEN
                end
            elseif not isNear then
                wasNear = false
            end
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    me:ReleaseControl()
    quest:Log("BowerstoneIntroGuardToPosh: Main() completed.")
end
