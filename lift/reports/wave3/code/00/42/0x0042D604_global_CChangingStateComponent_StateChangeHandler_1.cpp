#include <cstddef>
#include <cstdint>

struct CChangingStateComponent_StateOverlay {
    void** vftable;
};

struct CChangingStateComponent_Overlay {
    std::byte padding_0000[0x4];
    CChangingStateComponent_StateOverlay state;
};

static_assert(offsetof(CChangingStateComponent_Overlay, state) == 0x4);

void __thiscall CChangingStateComponent_StateChangeHandler_1(
    CChangingStateComponent* const self,
    unsigned long const state)
{
    using FrontEndManagerCall = void(__thiscall*)(NUISystem::CFrontEndManager*, void*);
    using EmbeddedStateCall = void(__thiscall*)(CChangingStateComponent_StateOverlay*);

    if (state == 1) {
        NUISystem::CFrontEndManager* const front_end_manager = NUISystem::CFrontEndManager::GetInstance();
        void* const state_ptr = reinterpret_cast<void*>(
            (0u - static_cast<std::uint32_t>(self != nullptr)) &
            static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(
                    reinterpret_cast<std::byte*>(self) + 0x4)));

        reinterpret_cast<FrontEndManagerCall>((*reinterpret_cast<void***>(front_end_manager))[2])(
            front_end_manager,
            state_ptr);

        auto* const overlay =
            reinterpret_cast<CChangingStateComponent_Overlay*>(self);
        reinterpret_cast<EmbeddedStateCall>(overlay->state.vftable[6])(&overlay->state);
    }

    if (state == 0) {
        NUISystem::CFrontEndManager* const front_end_manager = NUISystem::CFrontEndManager::GetInstance();
        void* const state_ptr = reinterpret_cast<void*>(
            (0u - static_cast<std::uint32_t>(self != nullptr)) &
            static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(
                    reinterpret_cast<std::byte*>(self) + 0x4)));

        reinterpret_cast<FrontEndManagerCall>((*reinterpret_cast<void***>(front_end_manager))[5])(
            front_end_manager,
            state_ptr);
    }

    NUISystem::CChangingStateComponent::ChangeState(self, state);
}