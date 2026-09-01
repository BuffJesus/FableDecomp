-- Retail helper 0x004AA840 (C3DClothPrimitive::~C3DClothPrimitive)
-- Preserve pre-release identity, all owner branches, clearing, and base initialization.
return function(read_owner, decrement_ref, destroy_pointee, free_owner, write_u32, initialize_base)
    write_u32(0, 19106956)
    local owner = read_owner(8)
    if owner ~= nil then
        local remaining = decrement_ref(owner)
        if remaining == 0 then
            destroy_pointee(owner)
            free_owner(owner, 12577212)
        end
    end
    write_u32(4, 0)
    write_u32(8, 0)
    initialize_base(10068704)
end
