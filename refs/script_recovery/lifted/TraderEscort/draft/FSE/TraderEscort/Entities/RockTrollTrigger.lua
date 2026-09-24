-- Generated native draft: RockTrollTrigger. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local CVar2, CVar6, f_CVar2, pCVar4, pCVar5, xStack_10, xStack_20, xStack_30
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar3 = not alive
    if not bVar3 then
        f_CVar2 = quest:ReadGlobalGameData(0xe1c)
        CVar6 = CVar2
        -- TODO(native): xStack_34 = CVar2;
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, CVar6)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            CVar6 = CVar2
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, CVar6)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            if not __native_entity_state:GetStateBool("RockTrollSpawned") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                __native_entity_state:SetStateBool("RockTrollSpawned", true)
                if not quest:GetStateBool("ShownEarthTroll") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    xStack_20 = resources:NewResource()
                    pCVar5 = xStack_20
                    pCVar4 = quest:GetHero()
                    resources:TryAcquire(pCVar5, pCVar4, 4)
                    xStack_30 = resources:NewActorMap()
                    resources:SetActor(xStack_30, "HERO", xStack_20)
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_DARKWOOD_TRADER_TROLL", xStack_30, false, true)
                    quest:SetStateBool("ShownEarthTroll", true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                    resources:DestroyActorMap(xStack_30)
                    resources:ReleaseResource(xStack_20)
                end
            end
            quest:RemoveThing(me, false, true)
        end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("RockTrollSpawned", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

