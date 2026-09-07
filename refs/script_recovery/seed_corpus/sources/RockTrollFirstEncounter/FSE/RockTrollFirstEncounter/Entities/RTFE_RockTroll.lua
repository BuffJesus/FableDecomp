-- RTFE_RockTroll.lua
-- Entity script for the Rock Troll boss in RockTrollFirstEncounter
-- Functional parity port of NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll

local SCRIPT_AI_PRIORITY_HIGHEST = 5

local REWARD_ITEM_1 = "OBJECT_GEMSTONE_RUBY"
local REWARD_ITEM_2 = "OBJECT_GEMSTONE_SAPPHIRE"

function Init(quest, me)
    quest:Log("RTFE_RockTroll: Init() started.")

    -- Ensure Rock Troll persists across region streaming
    quest:SetThingPersistent(me, true)

    -- Add red blip to mini-map
    quest:MiniMapAddMarker(me, "HUD_ORB_RED_SMALL")

    quest:Log("RTFE_RockTroll: Init() completed.")
end

function Main(quest, me)
    quest:Log("RTFE_RockTroll: Main() started.")

    -- Cache Hero reference outside loop to prevent memory leak
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: RTFE_RockTroll could not get Hero reference.")
        return
    end

    -- Spawn death tracking thread on Quest
    quest:CreateThread("WatchForRockTrollKilled")

    -- Attach to quest script and acquire highest priority control
    me:MakeBehavioral()
    if not me:AcquireControl(SCRIPT_AI_PRIORITY_HIGHEST) then
        quest:Log("!!! ERROR: RTFE_RockTroll could not acquire script control.")
        return
    end

    -- Add reward items to Rock Troll container once
    if not quest:GetStateBool("AddedItemsToRockTroll") then
        quest:Log("RTFE_RockTroll: Adding reward items to Rock Troll inventory...")
        quest:AddItemToContainer(me, REWARD_ITEM_1)
        quest:AddItemToContainer(me, REWARD_ITEM_2)
        quest:SetStateBool("AddedItemsToRockTroll", true)
    end

    -- Play the cinematic exhume cutscene if not already played
    if not quest:GetStateBool("PlayedExhumeCutScene") then
        quest:Log("RTFE_RockTroll: Playing 'CS_ROCKTROLL_EXHUME' cutscene...")

        -- Release entity control so StartCutscene can acquire it cleanly
        me:ReleaseControl()

        -- Manual cutscene sequence setup
        quest:StartCutscene({ HERO = hero, TROLL = me }, nil, false)
        quest:RunCutscene("CS_ROCKTROLL_EXHUME", true, false)
        quest:EndCutscene()

        -- Spawn hit monitoring thread for HUD health bar display
        quest:CreateThread("WatchForRockTrollHit")
        quest:SetStateBool("PlayedExhumeCutScene", true)
        quest:Log("RTFE_RockTroll: Exhume cutscene finished.")

        -- Re-acquire control after cutscene for post-cutscene boasting animations
        me:AcquireControl(SCRIPT_AI_PRIORITY_HIGHEST)
    end

    quest:Log("RTFE_RockTroll: Playing SPECIAL_BOAST animation...")
    me:PlayAnimation("SPECIAL_BOAST")
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then return end
    end

    quest:Log("RTFE_RockTroll: Playing SPECIAL_IDLE animation...")
    me:PlayAnimation("SPECIAL_IDLE")
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then return end
    end

    -- Release script control to allow default combat AI
    me:ReleaseControl()
    quest:Log("RTFE_RockTroll: Script control released. Setting combat targets...")

    -- Force troll to face and target the Hero
    quest:EntityForceToLookAtThing(me, hero)
    quest:GiveThingBestEnemyTarget(me, hero)

    -- Combat loop: maintain execution while alive
    while true do
        if not quest:NewScriptFrame(me) then break end
    end

    quest:Log("RTFE_RockTroll: Main() ended.")
end
