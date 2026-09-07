-- DBSummoner.lua
-- Script for Summoners spawned during Dragon Boss Fight

local DS_DEAD = 4

function Init(quest, me)
    quest:Log("DBSummoner: Init phase started.")
    local count = (quest:GetStateInt("NumSummoners") or 0) + 1
    quest:SetStateInt("NumSummoners", count)
    quest:Log("DBSummoner: NumSummoners incremented to " .. tostring(count))
end

function OnPredicateFail(quest, me)
    quest:Log("DBSummoner: OnPredicateFail triggered.")
    local count = math.max(0, (quest:GetStateInt("NumSummoners") or 1) - 1)
    quest:SetStateInt("NumSummoners", count)

    local timer = quest:GetStateInt("SummonerSpawnDelayTimer")
    if timer and timer >= 0 then
        local cur = quest:GetTimer(timer)
        quest:SetTimer(timer, cur + 20)
        quest:Log("DBSummoner: Spawn timer delayed by 20s (new duration=" .. tostring(cur + 20) .. ")")
    end
end

function Main(quest, me)
    quest:Log("DBSummoner: Main() started.")

    -- Initial yield to ensure engine has linked the newly spawned creature
    if not quest:NewScriptFrame(me) then return end

    local hero = quest:GetHero()
    if hero then
        quest:GiveThingBestEnemyTarget(me, hero)
    else
        quest:Log("!!! WARNING: DBSummoner - Hero is nil.")
    end

    while true do
        if not quest:NewScriptFrame(me) then
            quest:Log("DBSummoner: Script frame terminated.")
            break
        end

        if me:IsDead() then
            quest:Log("DBSummoner: Summoner died.")
            break
        end

        local dragonState = quest:GetStateInt("DragonState")
        if dragonState == DS_DEAD then
            quest:Log("DBSummoner: Dragon dead. Removing summoner.")
            quest:RemoveThing(me)
            break
        end
    end
end
