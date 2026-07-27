void CGameScriptInterface::SetQuestInfoName(const char* name) const
{
    struct DataBankRootOverlay
    {
        std::byte pad[0x14];
        NGameText::CDataBank* dataBank;
    };
    static_assert(offsetof(DataBankRootOverlay, dataBank) == 0x14);

    struct QuestInfoNameOverlay
    {
        std::byte pad[0x50];
        CWideString questInfoName;
    };
    static_assert(offsetof(QuestInfoNameOverlay, questInfoName) == 0x50);

    struct QuestInfoRootOverlay
    {
        std::byte pad[0x1C8];
        QuestInfoNameOverlay* questInfo;
    };
    static_assert(offsetof(QuestInfoRootOverlay, questInfo) == 0x1C8);

    struct CCharStringOverlay
    {
        struct Rep
        {
            const char* data;
            std::uint32_t length;
        };

        Rep* rep;
    };
    static_assert(offsetof(CCharStringOverlay, rep) == 0x0);

    struct RefCountedHandleOverlay
    {
        void** vftable;
        std::int32_t refCount;
    };
    static_assert(offsetof(RefCountedHandleOverlay, refCount) == 0x4);

    auto* const dataBankRoot = *reinterpret_cast<DataBankRootOverlay**>(0x013B86A0);
    NGameText::CDataBank* const dataBank = dataBankRoot->dataBank;

    alignas(CCharString) std::byte charStringStorage[sizeof(CCharString)];
    auto* const text = new (charStringStorage) CCharString(name, -1);

    const auto* const rep = reinterpret_cast<const CCharStringOverlay*>(text)->rep;

    std::uint32_t crcLength;
    const char* crcData;
    if (rep == nullptr)
    {
        crcLength = 0;
        crcData = reinterpret_cast<const char*>(0x0129AAF4);
    }
    else
    {
        crcLength = rep->length;
        crcData = rep->data;
    }

    const std::uint32_t crc = CCRC::Calc(0, crcData, crcLength);
    const std::uint64_t lookup = CVectorMap__LookupValue(crc);

    const std::pair<EHeroMorphType, CParticleMorphs::CEntry> entry =
        *reinterpret_cast<const std::pair<EHeroMorphType, CParticleMorphs::CEntry>*>(
            static_cast<std::uintptr_t>(lookup >> 32));

    if (static_cast<std::uint32_t>(lookup) != 0)
    {
        CBankFile* const bankFile =
            dataBank->GetPGroupEntry(entry, static_cast<unsigned char>(lookup));

        auto* const questInfoRoot = *reinterpret_cast<QuestInfoRootOverlay**>(0x013B8790);
        auto* const questInfo = questInfoRoot->questInfo;

        CWideString::operator=(&questInfo->questInfoName, bankFile->GetBankHandle());

        text->~CCharString();

        if (bankFile != nullptr)
        {
            auto* const handle = reinterpret_cast<RefCountedHandleOverlay*>(bankFile);
            const std::int32_t oldRefCount = handle->refCount;
            handle->refCount = oldRefCount - 1;
            if (oldRefCount - 1 == 0)
            {
                reinterpret_cast<void(__thiscall*)(void*)>(handle->vftable[1])(handle);
            }
        }
    }
}