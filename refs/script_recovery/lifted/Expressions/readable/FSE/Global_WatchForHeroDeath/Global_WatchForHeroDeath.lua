-- Readable native conversion: Global_WatchForHeroDeath. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_WatchForHeroDeath.Main (retail 0x00ee8f50)
function Main(quest)
    quest:CreateThread("WatchForHeroDeath")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForHeroDeath(quest)
end

-- Global_WatchForHeroDeath.Init (retail 0x00ee8ea0)
function Init(quest)
end

-- Global_WatchForHeroDeath.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- Global_WatchForHeroDeath.WatchForHeroDeath (retail 0x00ee8fe0)
function WatchForHeroDeath(quest)
    local hero = quest:GetHero()
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if not (hero ~= nil and hero:IsAlive()) then
            quest:NewScriptFrame()
        else
            repeat
                if hero ~= nil and hero:IsUnconscious() then break end
                if not quest:NewScriptFrame() then return end
            until not (hero ~= nil and hero:IsAlive())
            quest:NewScriptFrame()
        end
    until false
end

