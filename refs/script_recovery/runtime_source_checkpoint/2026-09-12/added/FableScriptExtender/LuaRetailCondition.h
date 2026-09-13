#pragma once
#include "FableAPI.h"
#include <cstring>
#include <stdexcept>

// Guardian E27EA2..E27EC7: native registration clones this local condition.
// The active thread owns the clone; destruction here releases only our copy.
inline void RegisterRetailAliveCondition(void* entity, const void* parent, const CScriptThing* actor) {
    if (!entity || !parent || !actor || !RetailThing_Copy_API || !RetailThing_Destroy_API ||
        !RetailEntity_SetCondition_API || !g_pRetailAliveConditionVTable)
        throw std::runtime_error("Retail alive-condition APIs or entity host are unavailable");
    // CB7920/CB7940 use +2C. Do not use the legacy PActiveThread member at +38.
    void* active = nullptr;
    std::memcpy(&active, static_cast<const char*>(parent) + 0x2c, sizeof(active));
    if (!active) throw std::runtime_error("Retail alive condition requires an active entity thread");
    struct Condition {
        void** table;
        CScriptThing thing{};
        ~Condition() { RetailThing_Destroy_API(&thing); }
    } condition{g_pRetailAliveConditionVTable};
    static_assert(offsetof(Condition, thing) == 4 && sizeof(Condition) == 16, "Retail x86 condition layout");
    RetailThing_Copy_API(&condition.thing, actor);
    RetailEntity_SetCondition_API(entity, &condition);
}
