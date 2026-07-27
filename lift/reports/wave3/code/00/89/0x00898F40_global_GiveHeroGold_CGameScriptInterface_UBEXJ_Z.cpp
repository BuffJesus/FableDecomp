namespace
{
    using InterfaceMap = CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

    struct CGameScriptInterface_GiveHeroGold_Overlay
    {
        std::byte pad_00[0x14];
        void* field_14;
    };
    static_assert(offsetof(CGameScriptInterface_GiveHeroGold_Overlay, field_14) == 0x14);

    struct InterfaceMap_LowerBoundOverlay
    {
        std::byte pad_00[0x04];
        int* end;
    };
    static_assert(offsetof(InterfaceMap_LowerBoundOverlay, end) == 0x04);

    struct GiveHeroGold_ContextOverlay
    {
        std::byte pad_00[0x20];
        std::uint8_t flags_20;
        std::byte pad_21[0x44 - 0x21];
        InterfaceMap_LowerBoundOverlay interfaces;
        std::byte pad_4c[0x91 - 0x4c];
        std::uint8_t flags_91;
    };
    static_assert(offsetof(GiveHeroGold_ContextOverlay, flags_20) == 0x20);
    static_assert(offsetof(GiveHeroGold_ContextOverlay, interfaces) == 0x44);
    static_assert(offsetof(GiveHeroGold_ContextOverlay, flags_91) == 0x91);

    [[nodiscard]] void* __thiscall Catch_0040D959(void* value);
    [[nodiscard]] void* __thiscall ResolveGiveHeroGoldContext(void* value);
}

void CGameScriptInterface::GiveHeroGold(long amount) const
{
    auto* const self = reinterpret_cast<const CGameScriptInterface_GiveHeroGold_Overlay*>(this);
    auto* const context = reinterpret_cast<GiveHeroGold_ContextOverlay*>(
        ResolveGiveHeroGoldContext(Catch_0040D959(self->field_14)));

    if ((context != nullptr) &&
        ((context->flags_91 & 0x01) == 0) &&
        ((context->flags_20 & 0x10) != 0))
    {
        const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(4);
        auto* entry = reinterpret_cast<int*>(
            reinterpret_cast<InterfaceMap*>(std::addressof(context->interfaces))
                ->LowerBound(const_cast<ETCInterfaceType*>(std::addressof(interfaceType))));
        int* const end = context->interfaces.end;

        if ((entry == end) || (4 < *entry))
        {
            entry = end;
        }

        CTCHeroStats::AddMoney(reinterpret_cast<CTCHeroStats*>(entry[1]), amount, true);
    }
}