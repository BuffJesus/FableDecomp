-- TheKraken.lua
-- Script for The Kraken boss in KrakenChamber
-- Functional parity port of NScript::CQ_TentacleKrakenBossFightScript::CTheKraken

local KRAKEN_ALIVE = 0
local KRAKEN_DEAD  = 1

function Init(quest, me)
    quest:Log("TheKraken: Init phase started.")
end

function OnPredicateFail(quest, me)
    quest:Log("TheKraken: OnPredicateFail triggered - Kraken killed.")
    quest:SetStateInt("KrakenState", KRAKEN_DEAD)
end

function Main(quest, me)
    quest:Log("TheKraken: Main() started.")

    -- Initial yield to ensure engine has linked the entity
    if not quest:NewScriptFrame(me) then return end

    quest:SetStateInt("KrakenState", KRAKEN_ALIVE)

    while quest:GetStateInt("KrakenState") == KRAKEN_ALIVE do
        if not quest:NewScriptFrame(me) then
            quest:Log("TheKraken: Script frame terminated.")
            return
        end

        local hp = quest:GetHealth(me)
        if hp <= 0.0 or me:IsDead() then
            quest:Log("TheKraken: Health depleted (HP=" .. tostring(hp) .. "). Transitioning to KRAKEN_DEAD.")
            quest:SetStateInt("KrakenState", KRAKEN_DEAD)
            break
        end
    end

    quest:Log("TheKraken: Main() finished.")
end
