-- OakValeRevisited.lua
-- Main quest script for OakValeRevisited
-- Functional parity port of NScript::CCS_OakValeRevisitedScript (CS_OakValeRevisited.c)

Quest = nil

local QUEST_NAME = "OakValeRevisited"
local fires = {}
local cutsceneFinished = false

local function CleanupFires(quest)
    for _, fire in ipairs(fires) do
        if fire and fire:IsAlive() then
            quest:RemoveThing(fire)
        end
    end
    fires = {}
end

function Init(questObject)
    Quest = questObject
    Quest:Log("OakValeRevisited: Init phase started.")
end

function OnPersist(questObject, context)
    Quest = questObject
end

function OakValeFire(quest)
    quest:Log("OakValeRevisited: OakValeFire microthread started.")

    -- Loop 1: Wait until the "fire" cutscene flag is raised by CS_OAKVALE_REVISITED
    while not quest:GetCutsceneFlag("fire") do
        if cutsceneFinished or not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("OakValeRevisited: 'fire' cutscene flag detected. Spawning burning patch effects...")

    -- Spawn fire effects at all fire point markers
    local firePoints = quest:GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT")
    quest:Log("OakValeRevisited: firePoints count = " .. tostring(#firePoints))
    for i, fp in ipairs(firePoints) do
        quest:Log("OakValeRevisited: Processing fire point #" .. tostring(i))
        if fp == nil then
            quest:Log("OakValeRevisited: fp #" .. tostring(i) .. " is nil!")
        else
            local pos = fp:GetPos()
            quest:Log("OakValeRevisited: fp #" .. tostring(i) .. " pos: x=" .. tostring(pos and pos.x) .. ", y=" .. tostring(pos and pos.y) .. ", z=" .. tostring(pos and pos.z))
            if pos then
                quest:Log("OakValeRevisited: Calling CreateEffectAtPos for fp #" .. tostring(i) .. "...")
                local fire = quest:CreateEffectAtPos("OAKVALE_BURNING_PATCH", pos)
                quest:Log("OakValeRevisited: CreateEffectAtPos returned: " .. tostring(fire ~= nil))
                if fire then
                    table.insert(fires, fire)
                end
            end
        end
    end

    quest:Log("OakValeRevisited: Spawned " .. tostring(#fires) .. " fire effects. Waiting for fire flag to clear...")

    -- Loop 2: Wait until the "fire" cutscene flag is cleared
    while quest:GetCutsceneFlag("fire") do
        if cutsceneFinished or not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("OakValeRevisited: 'fire' cutscene flag cleared. Removing fire effects...")
    CleanupFires(quest)
    quest:Log("OakValeRevisited: OakValeFire microthread finished.")
end

local function OakVale(quest)
    quest:Log("OakValeRevisited: OakVale() started.")

    local hero = quest:GetHero()
    local marker = quest:GetThingWithScriptName("OV_HERO_START_MARKER")

    if hero and marker then
        quest:EntityTeleportToThing(hero, marker)
        quest:Log("OakValeRevisited: Hero teleported to OV_HERO_START_MARKER.")
    else
        quest:Log("!!! WARNING: OakValeRevisited - Hero or OV_HERO_START_MARKER was nil.")
    end

    -- Setup cutscene: actor 'Hero', initial flag 'fire = false', fixCamera = false
    quest:StartCutscene({ Hero = hero }, { fire = false }, false)

    -- Spawn concurrent fire monitoring thread
    quest:CreateThread("OakValeFire")

    -- Run cutscene macro (skippable = true, setupcond = false)
    quest:RunCutscene("CS_OAKVALE_REVISITED", true, false)

    -- Cutscene finished or skipped
    cutsceneFinished = true
    quest:EndCutscene()
    quest:Log("OakValeRevisited: Cutscene finished. Running cleanup...")

    -- Cleanup any remaining fires (failsafe if cutscene was skipped or finished before OakValeFire completed)
    CleanupFires(quest)

    -- Deactivate quest
    local activeQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(activeQuestName ~= "" and activeQuestName or "CS_OakValeRevisited", 0)
    quest:Log("OakValeRevisited: Quest deactivated.")

    -- Remove fence objects
    local fences = quest:GetAllThingsWithScriptName("Fence")
    local removedFenceCount = 0
    for _, fence in ipairs(fences) do
        if fence:IsAlive() then
            quest:RemoveThing(fence)
            removedFenceCount = removedFenceCount + 1
        end
    end
    quest:Log("OakValeRevisited: Removed " .. tostring(removedFenceCount) .. " fence object(s).")
end

function Main(questObject)
    Quest = questObject
    Quest:Log("OakValeRevisited: Main() started.")

    Quest:FinalizeEntityBindings()

    -- Wait until OakBay region is loaded
    while not Quest:IsRegionLoaded("OakBay") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    Quest:Log("OakValeRevisited: OakBay region loaded. Adding logbook story entry 100...")
    Quest:AddLogbookStoryEntry(100)

    OakVale(Quest)
    Quest:Log("OakValeRevisited: Main() completed.")
end
