-- TrophyDealerInCave.lua
-- Entity script for TrophyDealerInCave in Witchwood Cavern
-- Functional parity port of NScript::CV_TrophyDealerScript::CTrophyDealerInCave

local MY_SCRIPT_NAME = "TrophyDealerInCave"

local STATE_PLAY_CUTSCENE = 1
local STATE_DONE = 2

function Init(quest, me)
    quest:Log("TrophyDealerInCave: Init() started.")

    -- Clear information icon (wander=false, wave=false, continue_ai=false)
    quest:SetThingHasInformation(me, false, false, false)

    quest:SetStateInt("TrophyDealerState", STATE_PLAY_CUTSCENE)
end

function Main(quest, me)
    quest:Log("TrophyDealerInCave: Main() started.")

    -- Get Hero reference once outside loops (prevents memory leaks)
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: TrophyDealerInCave Main - could not get Hero.")
        return
    end

    -- Acquire script control
    if not me:AcquireControl() then
        quest:Log("!!! ERROR: TrophyDealerInCave Main - failed to acquire control.")
        return
    end

    while true do
        local currentState = quest:GetStateInt("TrophyDealerState") or STATE_PLAY_CUTSCENE

        if currentState == STATE_PLAY_CUTSCENE then
            -- Set entity as unkillable and undamageable
            quest:EntitySetAsKillable(me, false)
            quest:EntitySetAsDamageable(me, false)

            -- Release control before cutscene so the cutscene system can bind Hero & Trophy
            me:ReleaseControl()

            quest:Log("TrophyDealerInCave: Playing CS_TROPHY_DEALER cutscene...")
            quest:PlayCutscene("CS_TROPHY_DEALER", { Hero = hero, Trophy = me })
            quest:Log("TrophyDealerInCave: CS_TROPHY_DEALER cutscene completed.")

            -- Re-enable creature generators in Witchwood2
            quest:SetCreatureGeneratorsEnabled("Witchwood2", true)

            -- Mark quest as completed, fade screen in, and deactivate
            local activeQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(activeQuestName, true, false, false)
            quest:FadeScreenIn()
            quest:DeactivateQuestLater(activeQuestName, 0)
            quest:Log("TrophyDealer: Quest marked as completed and scheduled for deactivation.")

            quest:SetStateInt("TrophyDealerState", STATE_DONE)
            return
        end

        if currentState == STATE_DONE then
            return
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end
end

function OnPersist(quest, me, context)
    local state = quest:GetStateInt("TrophyDealerState") or STATE_PLAY_CUTSCENE
    state = quest:PersistTransferInt(context, "TrophyDealerState", state)
    quest:SetStateInt("TrophyDealerState", state)
end
