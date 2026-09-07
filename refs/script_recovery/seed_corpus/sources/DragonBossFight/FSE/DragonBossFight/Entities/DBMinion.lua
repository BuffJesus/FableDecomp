-- DBMinion.lua
-- Script for Minion Wardogs spawned during Dragon Boss Fight

local DS_DEAD = 4

function Init(quest, me)
    quest:Log("DBMinion: Init phase started.")
    local count = (quest:GetStateInt("NumMinions") or 0) + 1
    quest:SetStateInt("NumMinions", count)
    quest:Log("DBMinion: NumMinions incremented to " .. tostring(count))
end

function OnPredicateFail(quest, me)
    quest:Log("DBMinion: OnPredicateFail triggered.")
    local count = math.max(0, (quest:GetStateInt("NumMinions") or 1) - 1)
    quest:SetStateInt("NumMinions", count)

    local timer = quest:GetStateInt("MinionSpawnDelayTimer")
    if timer and timer >= 0 then
        local cur = quest:GetTimer(timer)
        quest:SetTimer(timer, cur + 20)
        quest:Log("DBMinion: Spawn timer delayed by 20s (new duration=" .. tostring(cur + 20) .. ")")
    end
end

function Main(quest, me)
    quest:Log("DBMinion: Main() started.")

    -- Initial yield to ensure engine has linked the newly spawned creature
    if not quest:NewScriptFrame(me) then return end

    local hero = quest:GetHero()
    if hero then
        quest:GiveThingBestEnemyTarget(me, hero)
    else
        quest:Log("!!! WARNING: DBMinion - Hero is nil.")
    end

    while true do
        if not quest:NewScriptFrame(me) then
            quest:Log("DBMinion: Script frame terminated.")
            break
        end

        if me:IsDead() then
            quest:Log("DBMinion: Minion died.")
            break
        end

        local dragonState = quest:GetStateInt("DragonState")
        if dragonState == DS_DEAD then
            quest:Log("DBMinion: Dragon dead. Removing minion.")
            quest:RemoveThing(me)
            break
        end
    end
end
