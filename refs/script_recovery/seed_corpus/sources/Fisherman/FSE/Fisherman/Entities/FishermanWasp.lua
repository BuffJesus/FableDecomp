local MY_SCRIPT_NAME = "FishermanWasp"

function Init(quest, me)
    local alive = quest:GetStateInt("WaspsAlive") or 0
    quest:SetStateInt("WaspsAlive", alive + 1)
    quest:Log("FishermanWasp: Init - WaspsAlive = " .. tostring(alive + 1))
end

function Main(quest, me)
    quest:Log("FishermanWasp: Main() started.")

    local hero = quest:GetHero()
    if hero then
        quest:GiveThingBestEnemyTarget(me, hero)
    end

    while me:IsAlive() do
        if not quest:NewScriptFrame(me) then
            break
        end
    end

    local alive = quest:GetStateInt("WaspsAlive") or 1
    alive = math.max(0, alive - 1)
    quest:SetStateInt("WaspsAlive", alive)
    quest:Log("FishermanWasp: Dead/Exited - WaspsAlive = " .. tostring(alive))
end
