#pragma once

#include <stddef.h>

#include "fable_string.h"

class CBasicRetailBankAliasMap
{
public:
    CCharString& operator[](const CCharString& key);
};

class CBasicRetailBankManager
{
public:
    void SetNavigatorEnabled(bool enabled);
    void AddBankAlias(
        const CCharString& genericName,
        const CCharString& platformName);

    fable_u8 unknown00_[0x14];
    bool navigatorEnabled_;
    fable_u8 unknown15_[3];
    CBasicRetailBankAliasMap aliases_;
};

FABLE_STATIC_ASSERT(
    offsetof(CBasicRetailBankManager, navigatorEnabled_) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(CBasicRetailBankManager, aliases_) == 0x18);

namespace NBankFileManager
{
    struct CContainedBank
    {
        fable_u32 dataVersion_;
        fable_u32 entryCount_;
        fable_u32 infoOffset_;
        fable_u32 infoSize_;
        fable_u32 alignment_;
    };

    class CContainedBankMap
    {
    public:
        CContainedBank& operator[](const CCharString& key);

        void* head_;
        fable_u32 size_;
        fable_u32 allocator_;
    };

    class CRetailBank
    {
    public:
        CRetailBank();

        fable_u32 vtable_;
        void* diskFile_;
        void* diskFileControl_;
        void* threadedFile_;
        void* threadedFileControl_;
        bool useDma_;
        fable_u8 padding15_[3];
        void* containedBanksHead_;
        fable_u32 containedBankCount_;
        fable_u32 containedBanksAllocator_;
        void* filePathStorage_;
    };
}

FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBank) == 0x14);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CContainedBankMap) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(NBankFileManager::CRetailBank) == 0x28);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, diskFile_) == 0x04);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, threadedFile_) == 0x0C);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, useDma_) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, containedBanksHead_) == 0x18);
FABLE_STATIC_ASSERT(
    offsetof(NBankFileManager::CRetailBank, filePathStorage_) == 0x24);

class CBankFileManager
{
public:
    class CBankInfo
    {
    public:
        void* devBankFilePath_;
        void* retailBankFilePath_;
        void* devHeaderFilePath_;
        void* retailHeaderFilePath_;
        void* registeredBank_;
    };

    void OpenRetailBank(
        const CWideString& pathname,
        bool startupPriority);
    void OpenIniFile(const CWideString& pathname);
    void Close();

    fable_u8 unknown00_[0x24];
    void* basePathStorage_;
    fable_u8 unknown28_[0x0C];
    fable_u32 retailBankVersion_;
};

FABLE_STATIC_ASSERT(sizeof(CBankFileManager::CBankInfo) == 0x14);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, basePathStorage_) == 0x24);
FABLE_STATIC_ASSERT(
    offsetof(CBankFileManager, retailBankVersion_) == 0x34);
