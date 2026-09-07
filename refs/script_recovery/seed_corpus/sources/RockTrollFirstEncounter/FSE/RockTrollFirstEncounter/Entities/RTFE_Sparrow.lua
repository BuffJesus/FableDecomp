-- RTFE_Sparrow.lua
-- Entity script for the perched sparrow in RockTrollFirstEncounter
-- Functional parity port of NScript::CV_RockTrollFirstEncounterScript::CRTFE_Sparrow

local SCRIPT_AI_PRIORITY_HIGHEST = 5

function Init(quest, me)
    quest:Log("RTFE_Sparrow: Init() started.")
end

function Main(quest, me)
    quest:Log("RTFE_Sparrow: Main() started.")

    -- Initial frame yield matching C++
    if not quest:NewScriptFrame(me) then return end

    -- Acquire highest priority script control to hold the sparrow in place
    if not me:AcquireControl(SCRIPT_AI_PRIORITY_HIGHEST) then
        quest:Log("RTFE_Sparrow: Failed to acquire script control.")
        return
    end

    quest:Log("RTFE_Sparrow: Control acquired. Holding position until Rock Troll awakens...")

    -- State machine loop: Wait until the Rock Troll unearths
    while true do
        if not quest:NewScriptFrame(me) then break end

        -- Check if Rock Troll exhume cutscene has played (Troll is awake)
        if quest:GetStateBool("PlayedExhumeCutScene") then
            quest:Log("RTFE_Sparrow: Troll is awake! Releasing control so sparrow can fly away.")
            break
        end
    end

    -- Release control allowing ambient flight/flee behavior
    me:ReleaseControl()
    quest:Log("RTFE_Sparrow: Control released, Main() finished.")
end
