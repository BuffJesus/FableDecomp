-- Generated native draft: Global_WatchForHeroDeath. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar1
    quest:CreateThread("WatchForHeroDeath")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForHeroDeath(quest)
    if not bVar1 then
    end
end

function Init(quest)
end

function OnPersist(quest, context)
end

function WatchForHeroDeath(quest)
    local pCVar2
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    repeat
        if bVar1 then
            return
        end
        pCVar2 = quest:GetHero()
        bVar1 = (pCVar2 ~= nil and pCVar2:IsAlive())
        if bVar1 then
            repeat
                pCVar2 = quest:GetHero()
                bVar1 = (pCVar2 ~= nil and pCVar2:IsUnconscious())
                if bVar1 then break end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                pCVar2 = quest:GetHero()
                bVar1 = (pCVar2 ~= nil and pCVar2:IsAlive())
            until not (bVar1)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

