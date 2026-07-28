namespace
{
    struct SaveMetadataLoadContext
    {
        void** vftable;
        CMemoryFile* memoryFile;
    };
    static_assert(offsetof(SaveMetadataLoadContext, vftable) == 0x00);
    static_assert(offsetof(SaveMetadataLoadContext, memoryFile) == 0x04);

    struct ChildPrimitiveHandlerOverlay
    {
        std::byte pad_00[0x7C];
        void* handlerObject;
    };
    static_assert(offsetof(ChildPrimitiveHandlerOverlay, handlerObject) == 0x7C);

    using MetadataLoadThunk =
        void(__thiscall*)(void* handlerObject, CCharString* metadataName, SaveMetadataLoadContext* context);

    extern CEngineInternalPrimitiveBase* unaff_this;
    extern CEnginePrimitive* unaff_EDI;
    extern unsigned long unaff_ESI;
}

void GFMain_ProcessSaveFileMetadata()
{
    std::byte findState[524];
    CMemoryFile metadataFile;
    CCharString fileBaseName;
    CCharString fileExtension;
    SaveMetadataLoadContext loadContext{};

    CCharString initialAutoSaveTemp;
    CCharString findPatternTemp;

    CUserProfileManager::GetAutoSaveFileName(&initialAutoSaveTemp);
    CCharString__AssignFromValue(&findPatternTemp, L"*.lug");
    initialAutoSaveTemp.~CCharString();

    char found = CAFile_FindFirstFile(findState);
    while (found != '\0')
    {
        CCharString splitPathTemp;
        CCharString::CCharString(&fileBaseName);
        CCharString::CCharString(&fileExtension);

        CCharString__AssignFromWide(&splitPathTemp, findState);
        CAFile::SplitPath(&splitPathTemp, nullptr, nullptr, &fileBaseName, &fileExtension);
        splitPathTemp.~CCharString();

        CCharString metadataOpenAutoSaveTemp;
        CCharString metadataOpenBaseTemp;
        CCharString metadataOpenExtensionTemp;

        CUserProfileManager::GetAutoSaveFileName(&metadataOpenAutoSaveTemp);
        CCharString__AssignFromData(&metadataOpenBaseTemp, &fileBaseName);
        CWideString* const metadataExtensionWide =
            reinterpret_cast<CWideString*>(CCharString__AssignFromValue(&metadataOpenExtensionTemp, L".met"));
        CMemoryFile::CMemoryFile(&metadataFile, metadataExtensionWide, static_cast<FileAccessType>(3), 2UL);
        metadataOpenExtensionTemp.~CCharString();
        metadataOpenBaseTemp.~CCharString();
        metadataOpenAutoSaveTemp.~CCharString();

        loadContext.vftable = PTR_LAB_0122ed7c;
        loadContext.memoryFile = &metadataFile;

        auto* const childPrimitive = reinterpret_cast<ChildPrimitiveHandlerOverlay*>(
            CEngineInternalPrimitiveBase::AddChildPrimitive(unaff_this, unaff_EDI, unaff_ESI));
        auto* const handlerObject = childPrimitive->handlerObject;
        auto* const handlerVftable = *reinterpret_cast<void***>(handlerObject);

        CCharString metadataCallAutoSaveTemp;
        CCharString metadataCallBaseTemp;
        CCharString metadataCallExtensionTemp;

        CUserProfileManager::GetAutoSaveFileName(&metadataCallAutoSaveTemp);
        CCharString__AssignFromData(&metadataCallBaseTemp, &fileBaseName);
        CCharString* const metadataName =
            CCharString__AssignFromData(&metadataCallExtensionTemp, &fileExtension);

        reinterpret_cast<MetadataLoadThunk>(handlerVftable[0xCC / sizeof(void*)])(
            handlerObject,
            metadataName,
            &loadContext);

        metadataCallExtensionTemp.~CCharString();
        metadataCallBaseTemp.~CCharString();
        metadataCallAutoSaveTemp.~CCharString();

        CMemoryFile::DumpToFile(&metadataFile);

        found = CAFile__FindNextFileSequential(findState);

        loadContext.vftable = PTR_LAB_0122ed7c;
        CTCInventoryItem::OnDie(reinterpret_cast<CTCInventoryItem*>(&metadataFile));

        fileExtension.~CCharString();
        fileBaseName.~CCharString();
    }

    findPatternTemp.~CCharString();
}