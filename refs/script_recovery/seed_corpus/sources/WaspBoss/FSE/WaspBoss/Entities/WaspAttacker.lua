function Main(quest, me)
    quest:Log("WaspAttacker: Main() started.")

    local victim = quest:GetThingWithScriptName("WaspVictim")
    if victim then
        quest:Log("WaspAttacker: Targeting WaspVictim.")
        quest:GiveThingBestEnemyTarget(me, victim)
    else
        quest:Log("!!! WARNING: WaspAttacker - WaspVictim not found!")
    end

    while victim and victim:IsAlive() and quest:GetHealth(victim) > 0 do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    quest:Log("WaspAttacker: WaspVictim is dead or removed. Switching target to Hero.")
    local hero = quest:GetHero()
    if hero then
        quest:GiveThingBestEnemyTarget(me, hero)
    end
end
