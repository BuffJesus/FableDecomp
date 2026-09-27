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
    local function parseGameInteger(text)
        local value, negative = 0, false
        for position = 1, #text do
            local character = text:sub(position, position)
            if character == "." then break end
            if character == "-" then
                negative = true
            elseif character >= "0" and character <= "9" then
                value = (value * 10 + tonumber(character)) % 4294967296
            end
        end
        if negative then value = (-value) % 4294967296 end
        -- Match the game's signed 32-bit result, including overflow.
        if value >= 2147483648 then value = value - 4294967296 end
        return value
    end
    local bVar3, cVar1, pPosition, r1
    local alive = true
    local p0 = v_stk_14
    local this_00 = me:GetDataString()
    local iVar4 = parseGameInteger(this_00)
    local timerId = quest:RegisterTimer()
    local v_stk_14 = timerId
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    while not bVar3 do
        cVar1 = quest:GetStateBool(("ArenaSpawnNeeded_" .. iVar4))
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(timerId)
                return
            end
            cVar1 = quest:GetStateBool(("ArenaSpawnNeeded_" .. iVar4))
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
        quest:SetStateBool(("ArenaSpawnNeeded_" .. iVar4), false)
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

