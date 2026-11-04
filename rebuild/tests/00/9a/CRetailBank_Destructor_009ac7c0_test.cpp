#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

static int order[10];
static int calls;
static int mapCalls;
static void* mapRoot;
static void record(int n) { order[calls++] = n; }
static void FABLE_FASTCALL releaseThread(void*) { record(3); }
static void FABLE_FASTCALL releaseDisk(void*) { record(4); }

CCharString::~CCharString() { record(1); }
void NBankFileManager::CContainedBankMap::Erase(CNode* root)
{
    record(2);
    mapRoot = root;
    ++mapCalls;
}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { record(5); }

static CCPPointerInfo* info(void (FABLE_FASTCALL* callback)(void*), unsigned long refs)
{
    CCPPointerInfo* result = new CCPPointerInfo(callback, 0);
    result->RefCount = refs;
    return result;
}

int main()
{
    unsigned char raw[sizeof(NBankFileManager::CRetailBank)] = {0};
    NBankFileManager::CRetailBank& bank =
        *reinterpret_cast<NBankFileManager::CRetailBank*>(raw);
    bank.containedBanks_.head_ = static_cast<NBankFileManager::CContainedBankMap::CNode*>(malloc(0x28));
    bank.containedBanks_.head_->parent_ = reinterpret_cast<NBankFileManager::CContainedBankMap::CNode*>(0x1234);
    bank.containedBanks_.size_ = 1;
    bank.threadedFile_.control_ = info(releaseThread, 1);
    bank.diskFile_.Info = info(releaseDisk, 1);
    bank.NBankFileManager::CRetailBank::~CRetailBank();
    if (calls != 5 || mapCalls != 1 || mapRoot != reinterpret_cast<void*>(0x1234))
        return 1;
    for (int i = 0; i < 5; ++i) if (order[i] != i + 1) return 2;

    calls = 0;
    mapCalls = 0;
    memset(raw, 0, sizeof(raw));
    bank.containedBanks_.head_ = static_cast<NBankFileManager::CContainedBankMap::CNode*>(malloc(0x28));
    CCPPointerInfo* sharedThread = info(releaseThread, 2);
    CCPPointerInfo* sharedDisk = info(releaseDisk, 2);
    bank.threadedFile_.control_ = sharedThread;
    bank.diskFile_.Info = sharedDisk;
    bank.NBankFileManager::CRetailBank::~CRetailBank();
    if (calls != 2 || order[0] != 1 || order[1] != 5 || mapCalls != 0)
        return 3;
    if (sharedThread->RefCount != 1 || sharedDisk->RefCount != 1)
        return 4;
    delete sharedThread;
    delete sharedDisk;
    puts("RETAIL_BANK_DESTRUCTOR_PASS");
    return 0;
}
