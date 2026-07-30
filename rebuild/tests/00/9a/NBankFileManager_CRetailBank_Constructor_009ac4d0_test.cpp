#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_retail_banks.h"

extern "C" fable_u8 g_FableRetailBankVTable_0129B010 = 0;

namespace
{
    fable_u8 g_node[0x28];
    void* g_bank;
    bool g_baseCalled;
    bool g_pathCalled;
    bool g_argumentsValid;
}

extern "C" void FABLE_FASTCALL
FableRetailBankBaseConstruct_009AC4D0(
    void* bank,
    void*)
{
    g_baseCalled = true;
    g_argumentsValid =
        g_argumentsValid &&
        bank == g_bank;
}

extern "C" void* FABLE_CDECL
FableRetailBankAllocateNode_009AC4D0(fable_u32 size)
{
    g_argumentsValid =
        g_argumentsValid &&
        size == 0x28;
    memset(g_node, 0xA5, sizeof(g_node));
    return g_node;
}

extern "C" void FABLE_FASTCALL
FableRetailBankPathConstruct_009AC4D0(
    void* path,
    void*)
{
    g_pathCalled = true;
    g_argumentsValid =
        g_argumentsValid &&
        path == reinterpret_cast<fable_u8*>(g_bank) + 0x24;
    *reinterpret_cast<fable_u32*>(path) = 0x13572468;
}

int main()
{
    fable_u8 storage[sizeof(NBankFileManager::CRetailBank)];
    memset(storage, 0xA5, sizeof(storage));
    g_bank = storage;
    g_baseCalled = false;
    g_pathCalled = false;
    g_argumentsValid = true;

    NBankFileManager::CRetailBank* bank =
        new (storage) NBankFileManager::CRetailBank;

    const bool passed =
        bank ==
            reinterpret_cast<NBankFileManager::CRetailBank*>(storage) &&
        g_baseCalled &&
        g_pathCalled &&
        g_argumentsValid &&
        bank->vtable_ ==
            reinterpret_cast<fable_u32>(
                &g_FableRetailBankVTable_0129B010) &&
        bank->diskFile_ == 0 &&
        bank->diskFileControl_ == 0 &&
        bank->threadedFile_ == 0 &&
        bank->threadedFileControl_ == 0 &&
        !bank->useDma_ &&
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
