#pragma once
#include "FableAPI.h"
#include <cstring>
#include <stdexcept>

// NewOak DB3FCF and fourteen reviewed entity entries use this native condition.
// Its predicate checks Thing.IsAlive, then !Thing.IsUnconscious. Registration
// clones the counted Thing; destroying our local does not destroy the clone.
inline void RegisterRetailConsciousCondition(void* entity, const void* parent, const CScriptThing* actor) {
    if (!entity || !parent || !actor || !RetailThing_Copy_API || !RetailThing_Destroy_API ||
        !RetailEntity_SetCondition_API)
        throw std::runtime_error("Retail conscious-condition APIs or entity host are unavailable");
    void* active = nullptr;
    std::memcpy(&active, static_cast<const char*>(parent) + 0x2c, sizeof(active));
    if (!active) throw std::runtime_error("Retail conscious condition requires an active entity thread");
    struct Condition {
        void** table;
        CScriptThing thing{};
        ~Condition() { RetailThing_Destroy_API(&thing); }
    } condition{ASLR<void**>(0x12c2fe8)};
    static_assert(offsetof(Condition, thing) == 4 && sizeof(Condition) == 16, "Retail x86 condition layout");
    RetailThing_Copy_API(&condition.thing, actor);
    RetailEntity_SetCondition_API(entity, &condition);
}
