-- KrakenMinion.lua
-- Script for Kraken Minion tentacles spawned during Kraken Boss Fight
-- Functional parity port of NScript::CQ_TentacleKrakenBossFightScript::CKrakenMinion

function Init(quest, me)
    quest:Log("KrakenMinion: Init phase started.")
end

function OnPredicateFail(quest, me)
    quest:Log("KrakenMinion: OnPredicateFail triggered.")
end

function Main(quest, me)
    quest:Log("KrakenMinion: Main() started.")

    -- Initial yield to ensure engine has linked the entity
    if not quest:NewScriptFrame(me) then return end

    -- SCRIPT_AI_PRIORITY_HIGH = 3
    if me:AcquireControl(3) then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")

        local hero = quest:GetHero()
        if hero then
            quest:GiveThingBestEnemyTarget(me, hero)
        end

        me:ReleaseControl()
    end

    quest:Log("KrakenMinion: Main() finished.")
end
