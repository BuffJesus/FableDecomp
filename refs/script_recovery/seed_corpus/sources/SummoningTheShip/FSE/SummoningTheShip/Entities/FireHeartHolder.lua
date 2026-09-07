-- FireHeartHolder.lua
-- Entity script for the Fire Heart Holder receptacle in Hook Coast Lighthouse
-- Functional parity port of NScript::CQ_SummoningTheShipScript::CFireHeartHolder

function Init(quest, me)
    quest:Log("FireHeartHolder: Init() started.")
    me:MakeBehavioral()
    quest:Log("FireHeartHolder: Init() completed.")
end

function Main(quest, me)
    quest:Log("FireHeartHolder: Main() started.")

    -- If already started (e.g. after reload), idle
    if quest:GetStateBool("LighthouseStarted") then
        while true do
            if not quest:NewScriptFrame(me) then break end
        end
        return
    end

    -- Wait for player interaction
    while true do
        if not quest:NewScriptFrame(me) then return end

        if me:MsgIsUsedByHero() then
            quest:Log("FireHeartHolder: Used by Hero! Placing Fire Heart into lighthouse...")

            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)

            quest:TakeObjectFromHero("OBJECT_LPDD_FIREHEART_01")
            quest:MiniMapRemoveMarker(me)

            quest:PauseAllNonScriptedEntities(false)
            quest:EndMovieSequence()

            quest:SetStateBool("LighthouseStarted", true)
            break
        end

        if quest:GetStateBool("LighthouseStarted") then
            break
        end
    end

    -- Idle until end of quest
    while true do
        if not quest:NewScriptFrame(me) then break end
    end

    quest:Log("FireHeartHolder: Main() finished.")
end
