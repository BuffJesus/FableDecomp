#include <cstddef>
#include <cstdint>

namespace
{
struct ConstraintGlobalManager_Overlay
{
    std::uint8_t pad00[0x58];
    void* object58;                // +0x58
    std::uint8_t pad5C[0x180 - 0x5C];
    std::int32_t field180;         // +0x180
    std::uint8_t pad184[0x1D4 - 0x184];
    void* handler1D4;              // +0x1D4
};

struct ConstraintManagerObject_Overlay
{
    void* vftable;                 // +0x00
    std::uint8_t pad04[0x1C - 0x04];
    std::int32_t constraint;       // +0x1C
};

static_assert(offsetof(ConstraintGlobalManager_Overlay, object58) == 0x58);
static_assert(offsetof(ConstraintGlobalManager_Overlay, field180) == 0x180);
static_assert(offsetof(ConstraintGlobalManager_Overlay, handler1D4) == 0x1D4);
static_assert(offsetof(ConstraintManagerObject_Overlay, constraint) == 0x1C);
}

extern std::uintptr_t DAT_0143bb5c;

void __cdecl Constraint_NotifyGlobalManager(std::int32_t constraint)
{
    auto* const globalManager =
        reinterpret_cast<ConstraintGlobalManager_Overlay*>(DAT_0143bb5c);

    if (globalManager->handler1D4 != nullptr)
    {
        auto* const handler = globalManager->handler1D4;
        auto* const vftable = *reinterpret_cast<void***>(handler);
        const auto notify =
            reinterpret_cast<void(__thiscall*)(void*, std::int32_t)>(vftable[7]);
        notify(handler, constraint);
    }

    if (globalManager->field180 != 0)
    {
        auto* const object =
            reinterpret_cast<ConstraintManagerObject_Overlay*>(globalManager->object58);
        object->constraint = constraint;

        auto* const vftable = *reinterpret_cast<void***>(object);
        const auto invoke =
            reinterpret_cast<void(__thiscall*)(void*)>(vftable[1]);
        invoke(object);
    }
}