-- Generated native draft: SummonerMinion. Review coverage report before use.
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
    local iVar1, pTarget
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar2 = not alive
    if not bVar2 then
        quest:EntitySetAlpha(me, 0.0, true)
        quest:EntitySetInLimbo(me, true, true)
        while (not quest:GetStateBool("SummonerAttacksStarted") or (quest:GetStateInt("CurrentAttackWave") ~= __native_entity_state:GetStateInt("WaveID"))) do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            if __native_entity_state:GetStateInt("WaveID") == 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                pTarget = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, pTarget)
            end
            quest:EntitySetInLimbo(me, false, true)
            -- TODO(native): FadeInThing(p0,0x40000000);
            iVar1 = quest:GetStateInt("SummonersAlive")
            while iVar1 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                iVar1 = quest:GetStateInt("SummonersAlive")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:Pause(1.0)
                quest:RemoveThing(me, true, true)
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                until not (not bVar2)
            end
        end
    end
end

function Init(quest, me)
    local piVar1 = me:GetDataString()
    local iVar2 = ((piVar1 == "WAVE2") and 0 or 1)
    local cVar5 = not (iVar2 ~= 0)
    __native_entity_state:SetStateInt("WaveID", ((cVar5 ~= false and cVar5 ~= nil and cVar5 ~= 0) and 1 or 0) + 1)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

