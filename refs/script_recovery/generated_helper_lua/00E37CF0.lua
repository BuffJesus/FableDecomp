-- Retail helper 0x00E37CF0 (NScript::CV_AssassinAttacksScript::InitialiseRegionSpecificInfo)
-- Preserve both fixed-loop branches, strides, and per-element termination checks.
return function(read_u8, is_terminating, read_indirect_i32, write_indirect_i32, write_i32)
    for index = 0, 4 do
        local condition = read_u8(80)
        if is_terminating() then return end
        local source_offset = index * 4
        if condition == 0 then
            write_indirect_i32(168, source_offset, 0)
        else
            local value = read_indirect_i32(168, source_offset)
            write_i32(92 + index * 16, value)
        end
    end
end
