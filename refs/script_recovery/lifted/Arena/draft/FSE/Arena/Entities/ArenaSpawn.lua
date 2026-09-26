-- Generated native draft: ArenaSpawn. Review coverage report before use.
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
    local bVar3, cVar1, pPosition, r1
    local alive = true
    local p0 = v_stk_14
    local this_00 = me:GetDataString()
    local iVar4 = tonumber(this_00)
    local timerId = quest:RegisterTimer()
    local v_stk_14 = timerId
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    while not bVar3 do
        -- TODO(native): cVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0xe2 + iVar4)
        cVar1 = nil --[[unresolved native value]]
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(timerId)
                return
            end
            -- TODO(native): cVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0xe2 + iVar4)
            cVar1 = nil --[[unresolved native value]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        if quest:GetStateInt("ArenaRound") ~= 3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00f1b7e2: (native jump target)
                quest:DeregisterTimer(timerId)
                return
            end
            bVar3 = false
            pPosition = me:GetPos()
            r1 = quest:CreateEffectAtPos("SUMMON_ARENA", pPosition, (v_stk_14 + 4), 0.0)
            timerId = v_stk_14
        end
        -- TODO(native): *(undefined1 *)(*(int *)(this + 0x14) + 0xe2 + iVar4) = 0;
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    end
    quest:DeregisterTimer(timerId)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

