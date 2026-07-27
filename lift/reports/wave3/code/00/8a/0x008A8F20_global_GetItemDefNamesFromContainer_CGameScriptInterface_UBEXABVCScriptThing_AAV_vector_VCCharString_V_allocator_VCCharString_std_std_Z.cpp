struct CGameScriptInterface_Overlay {
    std::byte pad_0000[0x10];
    CDefinitionManager* m_pDefinitionManager;
};
static_assert(offsetof(CGameScriptInterface_Overlay, m_pDefinitionManager) == 0x10);

struct ContainerInterfaceOwner_Overlay {
    std::byte pad_0000[0x24];
    std::uint32_t flags;
    std::byte pad_0028[0x1c];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> interfaces;
};
static_assert(offsetof(ContainerInterfaceOwner_Overlay, flags) == 0x24);
static_assert(offsetof(ContainerInterfaceOwner_Overlay, interfaces) == 0x44);

struct ContainerItemRef_Overlay {
    int globalIndex;
    std::uint32_t unknown_04;
};
static_assert(sizeof(ContainerItemRef_Overlay) == 0x8);

struct ContainerItems_Overlay {
    std::byte pad_0000[0x0c];
    const ContainerItemRef_Overlay* begin;
    const ContainerItemRef_Overlay* end;
};
static_assert(offsetof(ContainerItems_Overlay, begin) == 0x0c);
static_assert(offsetof(ContainerItems_Overlay, end) == 0x10);

struct DefNameEntry_Overlay {
    int stringId;
};
static_assert(offsetof(DefNameEntry_Overlay, stringId) == 0x0);

extern CDefStringTable DAT_013ca828;

void CGameScriptInterface::GetItemDefNamesFromContainer(
    const CScriptThing& thing,
    std::vector<CCharString>& outDefNames) const
{
    outDefNames.erase(outDefNames.begin(), outDefNames.end());

    using HasContainerFn = char(__thiscall*)(const CScriptThing*);
    const auto hasContainer = reinterpret_cast<HasContainerFn>(
        (*reinterpret_cast<void* const* const*>(&thing))[0x300 / sizeof(void*)]
    )(&thing);

    if (hasContainer != 0) {
        using GetOwnerFn = int*(__thiscall*)(const CScriptThing*);
        auto* const interfaceOwner = reinterpret_cast<ContainerInterfaceOwner_Overlay*>(
            reinterpret_cast<GetOwnerFn>(
                (*reinterpret_cast<void* const* const*>(&thing))[0x2c / sizeof(void*)]
            )(&thing)
        );

        if ((interfaceOwner->flags & 0x10000000u) != 0) {
            ETCInterfaceType key = static_cast<ETCInterfaceType>(0x3c);
            auto interfaceIt =
                CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
                    &interfaceOwner->interfaces,
                    &key);

            if (interfaceIt == interfaceOwner->interfaces.end() ||
                static_cast<ETCInterfaceType>(0x3c) < interfaceIt->first) {
                interfaceIt = interfaceOwner->interfaces.end();
            }

            auto* const items = reinterpret_cast<const ContainerItems_Overlay*>(interfaceIt->second);
            std::uint32_t index = 0;
            if (((reinterpret_cast<int>(items->end) - reinterpret_cast<int>(items->begin)) >> 3) != 0) {
                do {
                    const auto* const defNameEntry =
                        reinterpret_cast<const DefNameEntry_Overlay*>(
                            reinterpret_cast<const CGameScriptInterface_Overlay*>(this)
                                ->m_pDefinitionManager->GetDefNameFromGlobalIndex(
                                    items->begin[index].globalIndex));

                    CCharString defName;
                    DAT_013ca828.GetString(&defName, defNameEntry->stringId);
                    outDefNames.push_back(defName);
                    ++index;
                } while (index < static_cast<std::uint32_t>(
                                     (reinterpret_cast<int>(items->end) - reinterpret_cast<int>(items->begin)) >> 3));
            }
        }
    }
}