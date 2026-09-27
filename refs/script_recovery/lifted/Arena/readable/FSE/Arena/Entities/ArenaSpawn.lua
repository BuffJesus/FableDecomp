-- Readable native conversion: ArenaSpawn. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- ArenaSpawn.Main (retail 0x00f1b690)
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
    local scratchValue = parseGameInteger(me:GetDataString())
    local timerId = quest:RegisterTimer()
    while not quest:IsActiveThreadTerminating() do
        while not quest:GetStateBool("ArenaSpawnNeeded_" .. scratchValue) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then break end
        if quest:GetStateInt("ArenaRound") == 3 then quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue, false); quest:NewScriptFrame(me); goto continue_2 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:CreateEffectAtPos("SUMMON_ARENA", me:GetPos(), timerId + 4, 0.0)
        quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue, false)
        quest:NewScriptFrame(me)
        ::continue_2::
    end
    quest:DeregisterTimer(timerId)
end

-- ArenaSpawn.Init (retail 0x00f1b660)
function Init(quest, me)
end

-- ArenaSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ArenaSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

