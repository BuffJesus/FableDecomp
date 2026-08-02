#include <new>
#include <stdio.h>
#include <string.h>

typedef unsigned char fable_u8;
typedef unsigned long fable_u32;

extern "C" fable_u8 g_CBaseVTable_009AC4D0 = 0;
extern "C" fable_u8 g_FableRetailBankVTable_0129B010 = 0;

namespace
{
    fable_u8 g_node[0x28];
    bool g_mallocCalled;
    unsigned int g_mallocSize;
}

extern "C" void* __cdecl malloc_009AC4D0(unsigned int size)
{
    g_mallocCalled = true;
    g_mallocSize = size;
    memset(g_node, 0xA5, sizeof(g_node));
    return g_node;
}

class CBase
{
public:
    CBase();

    fable_u32 vtable_;
};

CBase::CBase()
{
    vtable_ = reinterpret_cast<fable_u32>(&g_CBaseVTable_009AC4D0);
}

class CCharString
{
public:
    CCharString();

private:
    void* storage_;
};

CCharString::CCharString()
{
    storage_ = 0;
}

namespace NBankFileManager
{
    class CRetailBank : public CBase
    {
    public:
        CRetailBank();

        void* diskFile_;
        void* diskFileControl_;
        void* threadedFile_;
        void* threadedFileControl_;
        bool useDma_;
        fable_u8 padding15_[3];
        void* containedBanksHead_;
        fable_u32 containedBankCount_;
        fable_u32 containedBanksAllocator_;
        fable_u8 filePathStorageRaw_[sizeof(void*)];
    };

    CRetailBank::CRetailBank()
    {
        vtable_ = reinterpret_cast<fable_u32>(&g_FableRetailBankVTable_0129B010);
        diskFile_ = 0;
        diskFileControl_ = 0;
        threadedFile_ = 0;
        threadedFileControl_ = 0;
        useDma_ = false;

        fable_u8* node =
            reinterpret_cast<fable_u8*>(malloc_009AC4D0(0x28));
        containedBanksHead_ = node;
        containedBankCount_ = 0;

        node[0] = 0;
        *reinterpret_cast<fable_u32*>(node + 4) = 0;
        *reinterpret_cast<fable_u8**>(node + 8) = node;
        *reinterpret_cast<fable_u8**>(node + 0x0C) = node;

        new (filePathStorageRaw_) CCharString();
    }
}

int main()
{
    fable_u8 storage[sizeof(NBankFileManager::CRetailBank)];
    memset(storage, 0xA5, sizeof(storage));
    g_mallocCalled = false;
    g_mallocSize = 0;

    NBankFileManager::CRetailBank* bank =
        new (storage) NBankFileManager::CRetailBank;

    void* expectedVTable =
        reinterpret_cast<void*>(&g_FableRetailBankVTable_0129B010);

    const bool passed =
        bank == reinterpret_cast<NBankFileManager::CRetailBank*>(storage) &&
        bank->vtable_ == reinterpret_cast<fable_u32>(expectedVTable) &&
        bank->diskFile_ == 0 &&
        bank->diskFileControl_ == 0 &&
        bank->threadedFile_ == 0 &&
        bank->threadedFileControl_ == 0 &&
        !bank->useDma_ &&
        g_mallocCalled &&
        g_mallocSize == 0x28 &&
        bank->containedBanksHead_ == g_node &&
        bank->containedBankCount_ == 0 &&
        g_node[0] == 0 &&
        *reinterpret_cast<fable_u32*>(g_node + 4) == 0 &&
        *reinterpret_cast<void**>(g_node + 8) == g_node &&
        *reinterpret_cast<void**>(g_node + 0x0C) == g_node;

    if (!passed)
    {
        printf("FABLETLC_RETAIL_BANK_CONSTRUCTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_RETAIL_BANK_CONSTRUCTOR PASS\n");
    return 0;
}
