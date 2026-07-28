namespace
{
    struct CChangingStateComponent_SubobjectOverlay
    {
        void* vfptr;
    };
    static_assert(offsetof(CChangingStateComponent_SubobjectOverlay, vfptr) == 0x0);

    struct CChangingStateComponent_LayoutOverlay
    {
        std::byte pad_0[0x4];
        CChangingStateComponent_SubobjectOverlay subobjectAt4;
    };
    static_assert(offsetof(CChangingStateComponent_LayoutOverlay, subobjectAt4) == 0x4);
}

void __thiscall CChangingStateComponent_StateChangeHandler_2(CChangingStateComponent* self, unsigned long state)
{
    using FrontEndManagerSlot0C = void(__thiscall*)(NUISystem::CFrontEndManager*, void*);
    using FrontEndManagerSlot10 = void(__thiscall*)(NUISystem::CFrontEndManager*, void*);
    using SubobjectSlot18 = void(__thiscall*)(CChangingStateComponent_SubobjectOverlay*);

    if (state == 1) {
        NUISystem::CFrontEndManager* frontEndManager = NUISystem::CFrontEndManager::GetInstance();

        auto* const selfLayout = reinterpret_cast<CChangingStateComponent_LayoutOverlay*>(self);
        void* const stateTarget =
            reinterpret_cast<void*>((-(static_cast<unsigned int>(self != nullptr))) &
                                    static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(&selfLayout->subobjectAt4)));

        auto** const frontEndManagerVftable = reinterpret_cast<void***>(frontEndManager);
        reinterpret_cast<FrontEndManagerSlot0C>((*frontEndManagerVftable)[3])(frontEndManager, stateTarget);

        auto* const subobject = &selfLayout->subobjectAt4;
        auto** const subobjectVftable = reinterpret_cast<void***>(subobject);
        reinterpret_cast<SubobjectSlot18>((*subobjectVftable)[6])(subobject);
    }

    if (state == 0) {
        NUISystem::CFrontEndManager* frontEndManager = NUISystem::CFrontEndManager::GetInstance();

        auto* const selfLayout = reinterpret_cast<CChangingStateComponent_LayoutOverlay*>(self);
        void* const stateTarget =
            reinterpret_cast<void*>((-(static_cast<unsigned int>(self != nullptr))) &
                                    static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(&selfLayout->subobjectAt4)));

        auto** const frontEndManagerVftable = reinterpret_cast<void***>(frontEndManager);
        reinterpret_cast<FrontEndManagerSlot10>((*frontEndManagerVftable)[4])(frontEndManager, stateTarget);
    }

    NUISystem::CChangingStateComponent::ChangeState(self, state);
}