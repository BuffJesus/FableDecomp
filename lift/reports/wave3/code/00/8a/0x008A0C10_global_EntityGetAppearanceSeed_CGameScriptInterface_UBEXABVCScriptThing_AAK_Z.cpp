void __thiscall CGameScriptInterface::EntityGetAppearanceSeed(
    const CScriptThing& thing,
    unsigned long& appearanceSeed
) const
{
    struct CTCBaseOverlay
    {
        std::byte pad0[0x18];
        unsigned long mAppearanceSeed;
    };
    static_assert(offsetof(CTCBaseOverlay, mAppearanceSeed) == 0x18);

    struct InterfaceOwnerOverlay
    {
        std::byte pad0[0x20];
        std::uint8_t mFlags;
        std::byte pad1[0x23];
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> mInterfaces;
    };
    static_assert(offsetof(InterfaceOwnerOverlay, mFlags) == 0x20);
    static_assert(offsetof(InterfaceOwnerOverlay, mInterfaces) == 0x44);

    auto* const owner = thing.GetObject();
    if ((owner != nullptr) && ((reinterpret_cast<InterfaceOwnerOverlay*>(owner)->mFlags & 0x08) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(3);
        auto* it =
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
                &reinterpret_cast<InterfaceOwnerOverlay*>(owner)->mInterfaces,
                &interfaceType);

        auto* const end = reinterpret_cast<InterfaceOwnerOverlay*>(owner)->mInterfaces.mEnd;
        if ((it == end) || (static_cast<unsigned int>(it->first) > 3))
        {
            it = end;
        }

        appearanceSeed = reinterpret_cast<CTCBaseOverlay*>(it->second)->mAppearanceSeed;
    }
}