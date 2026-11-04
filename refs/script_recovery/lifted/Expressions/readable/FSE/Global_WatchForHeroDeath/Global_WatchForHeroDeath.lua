-- Readable native conversion: Global_WatchForHeroDeath. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_WatchForHeroDeath.Main (retail 0x00ee8f50)
function Main(quest)
    local scratchValue
    quest:CreateThread("WatchForHeroDeath")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForHeroDeath(quest)
    if not scratchValue then
    end
end

-- Global_WatchForHeroDeath.Init (retail 0x00ee8ea0)
function Init(quest)
end

-- Global_WatchForHeroDeath.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Global_WatchForHeroDeath.WatchForHeroDeath (retail 0x00ee8fe0)
function WatchForHeroDeath(quest)
    local hero3
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local hero = quest:GetHero()
        if hero ~= nil and hero:IsAlive() then
            repeat
                local hero2 = quest:GetHero()
                if hero2 ~= nil and hero2:IsUnconscious() then break end
                if not quest:NewScriptFrame() then return end
                hero3 = quest:GetHero()
            until not (hero3 ~= nil and hero3:IsAlive())
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:NewScriptFrame()
    until false
end

