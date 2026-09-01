-- Retail helper 0x00CBD510 (NParticleEngine::CParticleEmitter::DeleteAllParticles)
-- Preserve four owned-container teardowns and base initialization in native order.
return function(read_range, destroy_element_field, read_pointer, free_pointer, destroy_range, destroy_list, initialize_base)
    for _, element in ipairs(read_range(48, 52, 24)) do
        destroy_element_field(element, 16, 10087136)
    end
    local pointer_1 = read_pointer(48)
    if pointer_1 ~= nil then
        free_pointer(pointer_1, 12577300)
    end
    destroy_range(24, 28, 13349376)
    local pointer_2 = read_pointer(24)
    if pointer_2 ~= nil then
        free_pointer(pointer_2, 12577300)
    end
    destroy_range(8, 12, 13349296)
    local pointer_3 = read_pointer(8)
    if pointer_3 ~= nil then
        free_pointer(pointer_3, 12577300)
    end
    destroy_list(4, 13349008)
    local pointer_4 = read_pointer(4)
    if pointer_4 ~= nil then
        free_pointer(pointer_4, 12577300)
    end
    initialize_base(10068736)
end
