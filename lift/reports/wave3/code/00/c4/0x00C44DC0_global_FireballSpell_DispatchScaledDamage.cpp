#include <cstddef>
#include <cstdint>

struct FireballSpell_DispatchScaledDamage_This {
    void* field_00;
    std::byte pad_04[0x4C];
    void** dispatch_vftable;
};

static_assert(offsetof(FireballSpell_DispatchScaledDamage_This, field_00) == 0x00);
static_assert(offsetof(FireballSpell_DispatchScaledDamage_This, dispatch_vftable) == 0x50);

void __fastcall FireballSpell_DispatchScaledDamage(void* self_void)
{
    auto* const self = static_cast<FireballSpell_DispatchScaledDamage_This*>(self_void);

    const auto* const damage_source =
        reinterpret_cast<void*>(static_cast<std::byte*>(self->field_00) + 0x4C);

    const float damage_value =
        reinterpret_cast<float(__thiscall*)(const void*)>(&CTCWeapon_GetDamageValue)(damage_source);

    const long double scale_value =
        static_cast<long double>(
            reinterpret_cast<float(__thiscall*)(void*)>(&CMouseCursor__Initialize)(self));

    CSBAgents agents(
        static_cast<float>(scale_value * static_cast<long double>(damage_value)));

    auto* const dispatch_subobject =
        reinterpret_cast<void*>(reinterpret_cast<std::byte*>(self) + 0x50);

    using DispatchFn = void(__thiscall*)(void*, CSBAgents*);
    reinterpret_cast<DispatchFn>(self->dispatch_vftable[0x28 / sizeof(void*)])(
        dispatch_subobject,
        &agents);
}