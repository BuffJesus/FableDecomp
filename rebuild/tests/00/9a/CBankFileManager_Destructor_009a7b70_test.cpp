#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

static int order[8];
static int calls;
static void* wideRoot;
static void* aliasRoot;
static void* bankRoot;
static int baseDestructs;
static void record(int step) { order[calls++] = step; }

void CBankFileManager::Close() { record(1); }
void CBankManagerWidePathSet::Erase(CNode* node)
{
    record(2);
    wideRoot = node;
}
CWideString::~CWideString() { record(3); }
void CBasicRetailBankAliasMap::Erase(CNode* node)
{
    record(4);
    aliasRoot = node;
}
void CBankFileManager::CRetailBankList::clear() { record(5); }
void CBankFileManager::CBankInfoMap::Erase(CNode* node)
{
    record(6);
    bankRoot = node;
}
CBaseClass::~CBaseClass()
{
    record(7);
    ++baseDestructs;
}

int main()
{
    unsigned char raw[sizeof(CBankFileManager)] = {0};
    CBankFileManager& manager = *reinterpret_cast<CBankFileManager*>(raw);
    manager.WidePaths.head_ = static_cast<CBankManagerWidePathSet::CNode*>(malloc(0x14));
    manager.WidePaths.size_ = 1;
    manager.WidePaths.head_->parent_ = reinterpret_cast<CBankManagerWidePathSet::CNode*>(1);
    manager.Aliases.head_ = static_cast<CBasicRetailBankAliasMap::CNode*>(malloc(0x18));
    manager.Aliases.size_ = 1;
    manager.Aliases.head_->parent_ = reinterpret_cast<CBasicRetailBankAliasMap::CNode*>(2);
    manager.RetailBanks.node_ = static_cast<CBankFileManager::CRetailBankList::CNode*>(malloc(0x10));
    manager.Banks.head_ = static_cast<CBankFileManager::CBankInfoMap::CNode*>(malloc(0x28));
    manager.Banks.size_ = 1;
    manager.Banks.head_->parent_ = reinterpret_cast<CBankFileManager::CBankInfoMap::CNode*>(3);
    manager.CBankFileManager::~CBankFileManager();
    if (calls != 7 || baseDestructs != 1) return 1;
    for (int i = 0; i < 7; ++i)
        if (order[i] != i + 1) return 2;
    if (wideRoot != reinterpret_cast<void*>(1) ||
        aliasRoot != reinterpret_cast<void*>(2) ||
        bankRoot != reinterpret_cast<void*>(3)) return 3;
    if (manager.WidePaths.size_ || manager.Aliases.size_ || manager.Banks.size_)
        return 4;
    puts("BANK_MANAGER_DESTRUCTOR_PASS");
    return 0;
}
