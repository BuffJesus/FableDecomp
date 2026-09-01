-- Retail helper 0x00F14270 (NScript::CQ_ArenaScript::GetFanfareMusic)
-- Generated only from a complete native constant-return switch.
local cases = {
    [1] = 36,
    [2] = 37,
    [3] = 38,
    [4] = 39,
    [5] = 40,
    [6] = 41,
    [7] = 42,
    [8] = 43,
    [9] = 44,
}

return function(value)
    return cases[value] or 35
end
