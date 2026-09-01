-- Retail helper 0x006E7B80 (CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase)
-- Preserve all reference-count branches and final base teardown in native order.
return function(read_owner, decrement_ref, destroy_pointee, free_owner, write_u32, destroy_base)
    local owner = read_owner(12)
    if owner ~= nil then
        local remaining = decrement_ref(owner)
        if remaining == 0 then
            destroy_pointee(owner)
            free_owner(owner, 12577212)
        end
    end
    write_u32(8, 0)
    write_u32(12, 0)
    write_u32(0, 19267724)
    destroy_base(10069040)
end
