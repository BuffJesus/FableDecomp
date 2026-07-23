#include <cstddef>
#include <cstdint>

namespace
{
struct ConditionalVirtualDispatch_OnLevelUnload_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(ConditionalVirtualDispatch_OnLevelUnload_DispatchTargetOverlay, vftable) == 0x00);

struct ConditionalVirtualDispatch_OnLevelUnload_SelfOverlay
{
    std::byte pad00[0x91];
    std::uint8_t flags91; // +0x91
    std::byte pad92[0x06];
    ConditionalVirtualDispatch_OnLevelUnload_DispatchTargetOverlay* field98; // +0x98
};
static_assert(offsetof(ConditionalVirtualDispatch_OnLevelUnload_SelfOverlay, flags91) == 0x91);
static_assert(offsetof(ConditionalVirtualDispatch_OnLevelUnload_SelfOverlay, field98) == 0x98);

extern "C" std::int32_t DAT_013b8394;

using ConditionalVirtualDispatch_OnLevelUnload_VFunc =
    void(__thiscall*)(ConditionalVirtualDispatch_OnLevelUnload_DispatchTargetOverlay*);
}

void __thiscall ConditionalVirtualDispatch_OnLevelUnload(
    ConditionalVirtualDispatch_OnLevelUnload_SelfOverlay* self,
    std::uint32_t /*unused_stack_arg*/)
{
    if ((DAT_013b8394 != 0) && ((self->flags91 & 1u) == 0)) {
        const auto target = self->field98;
        const auto fn = reinterpret_cast<ConditionalVirtualDispatch_OnLevelUnload_VFunc>(
            reinterpret_cast<std::uintptr_t>(target->vftable[0x34 / sizeof(void*)]));
        fn(target);
        return;
    }

    return;
}