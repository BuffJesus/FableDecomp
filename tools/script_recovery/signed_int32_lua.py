"""Readable Lua arithmetic for counters stored as native signed 32-bit integers."""

SOURCE = '''-- Keep native integer overflow behavior at arithmetic boundaries.
local SIGNED_INT32_MAX = 2147483647
local UINT32_RANGE = 4294967296

local function wrapSignedInt32(value)
    local wrapped = value % UINT32_RANGE
    if wrapped > SIGNED_INT32_MAX then
        return wrapped - UINT32_RANGE
    end
    return wrapped
end

'''
