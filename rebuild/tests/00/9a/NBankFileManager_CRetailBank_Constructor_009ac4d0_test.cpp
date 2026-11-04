#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_retail_banks.h"
#include "test_support/base_class_stubs.h"

namespace
{
    fable_u8* g_bank;
    int g_step;
    int g_baseStep;
    int g_pathStep;
    int g_bankDestroyStep;
    int g_baseDestroyStep;
    void* g_headAtPath;
    bool g_argumentsValid;
}

CBaseClassNonCopyable::CBaseClassNonCopyable()
{
    g_baseStep = ++g_step;
    g_argumentsValid =
        g_argumentsValid &&
        reinterpret_cast<fable_u8*>(this) == g_bank;
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
    g_baseDestroyStep = ++g_step;
}

NBankFileManager::CRetailBank::~CRetailBank()
{
    g_bankDestroyStep = ++g_step;
}

void NBankFileManager::CContainedBankMap::Erase(CNode*) {}

CCharString::CCharString()
{
    g_pathStep = ++g_step;
    g_argumentsValid =
        g_argumentsValid &&
        reinterpret_cast<fable_u8*>(this) == g_bank + 0x24;
    g_headAtPath = *reinterpret_cast<void**>(g_bank + 0x18);
    storage_ = 0;
}

CCharString::~CCharString()
{
}

int main()
{
    fable_u8 storage[sizeof(NBankFileManager::CRetailBank)];
    memset(storage, 0xA5, sizeof(storage));
    g_bank = storage;
    g_step = 0;
    g_argumentsValid = true;

    NBankFileManager::CRetailBank* bank =
        new (storage) NBankFileManager::CRetailBank;
    NBankFileManager::CContainedBankMap::CNode* head =
        bank->containedBanks_.head_;

    const bool constructed =
        bank ==
            reinterpret_cast<NBankFileManager::CRetailBank*>(storage) &&
        g_argumentsValid &&
        g_baseStep == 1 &&
        g_pathStep == 2 &&
        bank->diskFile_.Data == 0 &&
        bank->diskFile_.Info == 0 &&
        bank->threadedFile_.data_ == 0 &&
        bank->threadedFile_.control_ == 0 &&
        !bank->useDma_ &&
        head != 0 &&
        g_headAtPath == head &&
        bank->containedBanks_.size_ == 0 &&
        !head->color_ &&
        head->parent_ == 0 &&
        head->left_ == head &&
        head->right_ == head;

    // The derived vtable is installed: destroying through the base reaches
    // the bank's own destructor first.
    static_cast<CBaseClass*>(bank)->~CBaseClass();
    const bool destroyed =
        g_bankDestroyStep == 3 &&
        g_baseDestroyStep == 4;

    free(head);

    if (!constructed || !destroyed)
    {
        printf("FABLETLC_RETAIL_BANK_CONSTRUCTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_RETAIL_BANK_CONSTRUCTOR PASS\n");
    return 0;
}
