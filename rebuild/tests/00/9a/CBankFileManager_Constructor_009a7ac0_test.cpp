#include <stdio.h>
#include <stdlib.h>
#include "fable_retail_banks.h"

static int baseConstructs;
static int baseDestructs;
static int wideConstructs;
static int wideDestructs;
CBaseClass::CBaseClass() { ++baseConstructs; }
CBaseClass::~CBaseClass() { ++baseDestructs; }
CWideString::CWideString() : storage_(0) { ++wideConstructs; }
CWideString::~CWideString() { ++wideDestructs; }
CBankFileManager::~CBankFileManager()
{
    free(Banks.head_);
    free(RetailBanks.node_);
    free(Aliases.head_);
    free(WidePaths.head_);
    BasePath().~CWideString();
}

int main()
{
    {
        CBankFileManager manager;
        if (sizeof(manager) != 0x38 || baseConstructs != 1 || wideConstructs != 1)
            return 1;
        if (!manager.Banks.head_ || manager.Banks.size_ ||
            manager.Banks.head_->color_ || manager.Banks.head_->parent_ ||
            manager.Banks.head_->left_ != manager.Banks.head_ ||
            manager.Banks.head_->right_ != manager.Banks.head_)
            return 2;
        if (!manager.RetailBanks.node_ ||
            manager.RetailBanks.node_->next_ != manager.RetailBanks.node_ ||
            manager.RetailBanks.node_->prev_ != manager.RetailBanks.node_)
            return 3;
        if (!manager.Aliases.head_ || manager.Aliases.size_ ||
            manager.Aliases.head_->color_ || manager.Aliases.head_->parent_ ||
            manager.Aliases.head_->left_ != manager.Aliases.head_ ||
            manager.Aliases.head_->right_ != manager.Aliases.head_)
            return 4;
        CBankManagerWidePathSet::CNode* extra = manager.WidePaths.head_;
        if (!extra || manager.WidePaths.size_ || extra->color_ ||
            extra->parent_ || extra->left_ != extra || extra->right_ != extra)
            return 5;
        if (manager.UseRetailBanks || manager.retailBankVersion_ != 100)
            return 6;
    }
    if (baseDestructs != 1 || wideDestructs != 1)
        return 7;
    puts("BANK_MANAGER_CONSTRUCTOR_PASS");
    return 0;
}
