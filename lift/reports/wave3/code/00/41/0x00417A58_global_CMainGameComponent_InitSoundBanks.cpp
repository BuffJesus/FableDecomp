void __fastcall CMainGameComponent_InitSoundBanks(void* self)
{
    struct CMainGameComponent_Overlay
    {
        std::byte pad_00[0x10];
        void* currentSoundBankMessage; // 0x10
    };
    static_assert(offsetof(CMainGameComponent_Overlay, currentSoundBankMessage) == 0x10);

    struct SoundBankEntryOverlay
    {
        int messageId;          // 0x00
        CDefString bankFile;    // 0x04
        CDefString fullPathA;   // 0x08
        CDefString nameArg;     // 0x0C
        char useAltBaseDir;     // 0x10
        std::byte pad_11[3];
    };
    static_assert(offsetof(SoundBankEntryOverlay, messageId) == 0x00);
    static_assert(offsetof(SoundBankEntryOverlay, bankFile) == 0x04);
    static_assert(offsetof(SoundBankEntryOverlay, fullPathA) == 0x08);
    static_assert(offsetof(SoundBankEntryOverlay, nameArg) == 0x0C);
    static_assert(offsetof(SoundBankEntryOverlay, useAltBaseDir) == 0x10);
    static_assert(sizeof(SoundBankEntryOverlay) == 0x14);

    struct AtmosSoundBankEntryOverlay
    {
        int messageId;       // 0x00
        CDefString bankFile; // 0x04
    };
    static_assert(offsetof(AtmosSoundBankEntryOverlay, messageId) == 0x00);
    static_assert(offsetof(AtmosSoundBankEntryOverlay, bankFile) == 0x04);
    static_assert(sizeof(AtmosSoundBankEntryOverlay) == 0x08);

    struct SoundSetupDefOverlay
    {
        void* vfptr;                            // 0x00
        int refCount;                           // 0x04
        std::byte pad_08[0x34];
        SoundBankEntryOverlay* entriesBegin;    // 0x3C
        SoundBankEntryOverlay* entriesEnd;      // 0x40
        std::byte pad_44[0x04];
        AtmosSoundBankEntryOverlay* atmosBegin; // 0x48
        AtmosSoundBankEntryOverlay* atmosEnd;   // 0x4C
    };
    static_assert(offsetof(SoundSetupDefOverlay, entriesBegin) == 0x3C);
    static_assert(offsetof(SoundSetupDefOverlay, entriesEnd) == 0x40);
    static_assert(offsetof(SoundSetupDefOverlay, atmosBegin) == 0x48);
    static_assert(offsetof(SoundSetupDefOverlay, atmosEnd) == 0x4C);

    using ReleaseDefFn = void(__thiscall*)(void*);
    using MessageSlot0CFn = void(__thiscall*)(void*, void*);

    auto releaseDef = [](void* p) {
        if (p != nullptr)
        {
            auto* words = static_cast<int*>(p);
            int* const refCount = words + 1;
            *refCount = *refCount - 1;
            if (*refCount == 0)
            {
                auto** const vtbl = *reinterpret_cast<void***>(p);
                reinterpret_cast<ReleaseDefFn>(vtbl[1])(p);
            }
        }
    };

    auto* const game = reinterpret_cast<CMainGameComponent_Overlay*>(self);
    game->currentSoundBankMessage = nullptr;

    if (DAT_013b8394 == 0)
    {
        return;
    }

    CCharString local_24_text("Lut register", -1);
    _E2();

    void* localisedDefRaw = nullptr;
    {
        CCharString* const fontBankName = static_cast<CCharString*>(NLocalisation::GetFontBankName());
        CDefinitionManager* const defMgr = static_cast<CDefinitionManager*>(GFGetPreMainMemoryUsed());
        CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>(
            defMgr,
            fontBankName,
            reinterpret_cast<CDefPointer<CHeroPostcardGeneratorDef const>*>(&localisedDefRaw));
    }

    auto* const localisedDef = reinterpret_cast<SoundSetupDefOverlay*>(localisedDefRaw);
    unsigned local_c = 0;
    if (((reinterpret_cast<std::uintptr_t>(localisedDef->entriesEnd) -
          reinterpret_cast<std::uintptr_t>(localisedDef->entriesBegin)) / 0x14) != 0)
    {
        int local_14 = 0;
        do
        {
            CCharString local_24("Init Localised Sound Bank Entries", -1);
            NProgressDisplay::DisplayProgress(&local_24, (float)(extraout_EDX_01 & 0xffffff00), SUB41(_DAT_0122dee0, 0), false);

            auto* const entry = reinterpret_cast<SoundBankEntryOverlay*>(
                reinterpret_cast<std::byte*>(localisedDef->entriesBegin) + local_14);

            CWideString local_10;
            {
                auto* const wideBank = reinterpret_cast<CWideString*>(CDefString::ToWideString(&entry->bankFile));
                CWideString::operator=(&local_10, wideBank);
            }

            CCharString local_18;
            CCharString local_8;
            CAFile::SplitPath(reinterpret_cast<CCharString*>(&local_10), nullptr, nullptr, &local_18, &local_8);

            if (*reinterpret_cast<void**>(&local_8) == nullptr)
            {
                if (wcscmp(L".lut", L"") == 0)
                {
                    CCharString__AssignFromWide(L".lug");
                }
            }
            else if (WideString_EqualsLen(*reinterpret_cast<wchar_t**>(&local_8), L".lut") != '\0')
            {
                CCharString__AssignFromWide(L".lug");
            }

            {
                auto* const assignedExt = static_cast<CWideString*>(CCharString__AssignFromData(&local_8));
                CWideString::operator=(&local_10, assignedExt);
            }

            void* uVar8 = CDefString::ToWideString(&entry->bankFile);
            auto* const local_34 = NResourceDirectoryNames::GetDVDDialogueDir();
            (void)local_34;
            CCharString__AssignFromData(uVar8);
            CWideString local_30;
            CAFile::ConvertPathToFullPath(&local_30);

            uVar8 = CDefString::ToWideString(&entry->fullPathA);
            CUserProfileManager local_48[4];
            CUserProfileManager::GetAutoSaveFileName(local_48);
            CCharString__AssignFromData(uVar8);
            CWideString local_2c;
            CAFile::ConvertPathToFullPath(reinterpret_cast<CWideString*>(&local_2c));

            CCharString local_20("Registering Localised Sound Bank", -1);
            NProgressDisplay::DisplayProgress(&local_20, (float)(extraout_EDX_03 & 0xffffff00), SUB41(_DAT_0122dee0, 0), false);

            CDefString::operator_class_CCharString(&entry->nameArg);
            CCharString local_28;
            RegisterSoundBanks(entry->messageId, &local_10, &local_30, reinterpret_cast<CWideString*>(&local_2c), &local_28);

            _E2();

            ++local_c;
            local_14 += 0x14;
        } while (local_c < static_cast<unsigned>(
                               (reinterpret_cast<std::uintptr_t>(localisedDef->entriesEnd) -
                                reinterpret_cast<std::uintptr_t>(localisedDef->entriesBegin)) / 0x14));
    }

    _E2();

    void* mainDefRaw = nullptr;
    {
        CCharString local_30_name("MAIN_SOUND_SETUP", -1);
        CDefinitionManager* const defMgr = static_cast<CDefinitionManager*>(GFGetPreMainMemoryUsed());
        CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>(
            defMgr,
            &local_30_name,
            reinterpret_cast<CDefPointer<CHeroPostcardGeneratorDef const>*>(&mainDefRaw));
    }

    auto* const mainDef = reinterpret_cast<SoundSetupDefOverlay*>(mainDefRaw);
    unsigned local_24 = 0;
    if (((reinterpret_cast<std::uintptr_t>(mainDef->entriesEnd) -
          reinterpret_cast<std::uintptr_t>(mainDef->entriesBegin)) / 0x14) != 0)
    {
        int local_20 = 0;
        do
        {
            CCharString local_30("Init Sound Bank Entries", -1);
            NProgressDisplay::DisplayProgress(&local_30, (float)(extraout_EDX_07 & 0xffffff00), SUB41(_DAT_0122dee0, 0), false);

            auto* const entry = reinterpret_cast<SoundBankEntryOverlay*>(
                reinterpret_cast<std::byte*>(mainDef->entriesBegin) + local_20);

            CDefString* const local_4c = &entry->bankFile;
            CDefString::ToWideString(local_4c);
            CDefString::operator_class_CCharString(local_4c);

            CCharString local_2c_concat;
            CCharString_OperatorPlus_API(&local_2c_concat);
            _E2();

            CCharString local_18;
            CCharString local_10;
            CCharString local_8;
            CAFile::SplitPath(&local_2c_concat, nullptr, nullptr, &local_10, &local_8);

            CWideString local_c(reinterpret_cast<CWideString*>(&local_8));

            if (*reinterpret_cast<void**>(&local_8) == nullptr)
            {
                if (wcscmp(L".lug", L"") == 0)
                {
                    CCharString__AssignFromWide(L".met");
                }
                else
                {
                    if (wcscmp(L".lut", L"") == 0)
                    {
                        CCharString__AssignFromWide(L".lug");
                    }
                }
            }
            else if (WideString_EqualsLen(*reinterpret_cast<wchar_t**>(&local_8), L".lug") != '\0')
            {
                CCharString__AssignFromWide(L".met");
            }
            else if (WideString_EqualsLen(*reinterpret_cast<wchar_t**>(&local_8), L".lut") != '\0')
            {
                CCharString__AssignFromWide(L".lug");
            }

            {
                auto* const assignedExt = static_cast<CWideString*>(CCharString__AssignFromData(&local_8));
                CWideString::operator=(reinterpret_cast<CWideString*>(&local_18), assignedExt);
            }

            CWideString local_14;
            if (entry->useAltBaseDir == '\0')
            {
                CUserProfileManager local_64[4];
                CUserProfileManager::GetAutoSaveFileName(local_64);
                CCharString__AssignFromData(&local_10);
                CCharString__AssignFromData(reinterpret_cast<unsigned*>(&local_c));
                CWideString local_58;
                auto* const fullPath = static_cast<CWideString*>(CAFile::ConvertPathToFullPath(&local_58));
                CWideString::operator=(&local_14, fullPath);
            }
            else
            {
                CUserProfileManager local_54[4];
                CUserProfileManager::GetAutoSaveFileName(local_54);
                CCharString__AssignFromData(&local_10);
                CCharString__AssignFromData(reinterpret_cast<unsigned*>(&local_c));
                CWideString local_40;
                auto* const fullPath = static_cast<CWideString*>(CAFile::ConvertPathToFullPath(&local_40));
                CWideString::operator=(&local_14, fullPath);
            }

            void* uVar8 = CDefString::ToWideString(&entry->fullPathA);
            CUserProfileManager local_6c[4];
            CUserProfileManager::GetAutoSaveFileName(local_6c);
            CCharString__AssignFromData(uVar8);
            CWideString local_38;
            CAFile::ConvertPathToFullPath(reinterpret_cast<CWideString*>(&local_38));

            CCharString local_28("Registering Sound Bank", -1);
            NProgressDisplay::DisplayProgress(&local_28, (float)(extraout_EDX_11 & 0xffffff00), SUB41(_DAT_0122dee0, 0), false);

            CDefString::operator_class_CCharString(&entry->nameArg);
            CCharString local_34;
            RegisterSoundBanks(entry->messageId, reinterpret_cast<CWideString*>(&local_18), &local_14, reinterpret_cast<CWideString*>(&local_38), &local_34);

            bool doMetUpdate = false;
            if (*reinterpret_cast<void**>(&local_c) == nullptr)
            {
                if (wcscmp(L".met", L"") == 0)
                {
                    doMetUpdate = true;
                }
            }
            else if (WideString_EqualsLen(*reinterpret_cast<wchar_t**>(&local_c), L".met") != '\0')
            {
                doMetUpdate = true;
            }

            if (doMetUpdate)
            {
                void* const message = MessageMap__FindMessageByID(entry->messageId);
                auto** const vtbl = *reinterpret_cast<void***>(message);

                CUserProfileManager local_7c[4];
                CUserProfileManager::GetAutoSaveFileName(local_7c);
                CCharString__AssignFromData(&local_10);
                void* const lugValue = CCharString__AssignFromValue(L".lug");
                reinterpret_cast<MessageSlot0CFn>(vtbl[3])(message, lugValue);
            }

            _E2();

            ++local_24;
            local_20 += 0x14;
        } while (local_24 < static_cast<unsigned>(
                                (reinterpret_cast<std::uintptr_t>(mainDef->entriesEnd) -
                                 reinterpret_cast<std::uintptr_t>(mainDef->entriesBegin)) / 0x14));
    }

    unsigned local_18_count = 0;
    if (((reinterpret_cast<std::uintptr_t>(mainDef->atmosEnd) -
          reinterpret_cast<std::uintptr_t>(mainDef->atmosBegin)) >> 3) != 0)
    {
        do
        {
            CCharString local_38_text("Registering Atmos Sound Bank", -1);
            NProgressDisplay::DisplayProgress(&local_38_text, (float)(extraout_EDX_14 & 0xffffff00), SUB41(_DAT_0122dee0, 0), false);

            auto* const entry = reinterpret_cast<AtmosSoundBankEntryOverlay*>(
                reinterpret_cast<std::byte*>(mainDef->atmosBegin) + local_18_count * 8);

            CDefString::ToWideString(&entry->bankFile);
            CDefString::operator_class_CCharString(&entry->bankFile);

            CCharString local_34;
            CCharString_OperatorPlus_API(&local_34);
            _E2();

            CCharString local_c_path;
            CCharString local_10;
            CCharString local_2c;
            CAFile::SplitPath(&local_34, nullptr, nullptr, &local_10, &local_2c);

            {
                auto* const metExt = static_cast<CWideString*>(CCharString__AssignFromValue(L".met"));
                CWideString::operator=(reinterpret_cast<CWideString*>(&local_c_path), metExt);
            }

            CUserProfileManager local_6c[4];
            CUserProfileManager::GetAutoSaveFileName(local_6c);
            CCharString__AssignFromData(reinterpret_cast<unsigned*>(&local_c_path));
            CWideString local_74;
            void* const fullMet = CAFile::ConvertPathToFullPath(reinterpret_cast<CWideString*>(&local_74));
            CMainGameComponent_RegisterSoundBank(entry->messageId, reinterpret_cast<CWideString*>(&local_c_path), fullMet);

            CUserProfileManager local_64[4];
            CUserProfileManager::GetAutoSaveFileName(local_64);
            CCharString__AssignFromData(&local_10);
            CCharString__AssignFromValue(L".lug");

            CUserProfileManager local_5c[4];
            CUserProfileManager::GetAutoSaveFileName(reinterpret_cast<CUserProfileManager*>(local_5c));
            CCharString__AssignFromData(&local_10);
            CCharString local_30;
            {
                void* const lugValue = CCharString__AssignFromValue(L".lug");
                local_30 = *reinterpret_cast<CCharString*>(lugValue);
            }

            void* const message = MessageMap__FindMessageByID(entry->messageId);
            auto** const vtbl = *reinterpret_cast<void***>(message);
            reinterpret_cast<MessageSlot0CFn>(vtbl[3])(message, &local_30);

            _E2();

            ++local_18_count;
        } while (local_18_count < static_cast<unsigned>(
                                        (reinterpret_cast<std::uintptr_t>(mainDef->atmosEnd) -
                                         reinterpret_cast<std::uintptr_t>(mainDef->atmosBegin)) >> 3));
    }

    game->currentSoundBankMessage = MessageMap__FindMessageByID(1);

    releaseDef(mainDefRaw);
    releaseDef(localisedDefRaw);
}