-- M_Experience.lua
-- Script for experience orb marker in KrakenChamber
-- Experience orbs are spawned directly by the quest script at all M_Experience markers upon Kraken defeat.

function Init(quest, me)
end

function OnPredicateFail(quest, me)
end

function Main(quest, me)
    -- Simply yield once and exit. The quest script spawns all experience orbs when Kraken is defeated.
    if not quest:NewScriptFrame(me) then return end
end
