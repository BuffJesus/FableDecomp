#include "fable_retail_banks.h"
#include <new>

CBankFileManager::CBankFileManager()
    : CBaseClass()
{
    Banks.head_ = 0;
    Banks.head_ = static_cast<CBankInfoMap::CNode*>(malloc(0x28));
    Banks.size_ = 0;
    Banks.head_->color_ = false;
    Banks.head_->parent_ = 0;
    Banks.head_->left_ = Banks.head_;
    Banks.head_->right_ = Banks.head_;

    RetailBanks.node_ = 0;
    CRetailBankList::CNode* listHead =
        static_cast<CRetailBankList::CNode*>(malloc(0x10));
    listHead->next_ = listHead;
    listHead->prev_ = listHead;
    RetailBanks.node_ = listHead;

    Aliases.head_ = 0;
    Aliases.head_ = static_cast<CBasicRetailBankAliasMap::CNode*>(malloc(0x18));
    Aliases.size_ = 0;
    Aliases.head_->color_ = false;
    Aliases.head_->parent_ = 0;
    Aliases.head_->left_ = Aliases.head_;
    Aliases.head_->right_ = Aliases.head_;

    CWideString* path = &BasePath();
    __assume(path != 0);
    new (path) CWideString;

    WidePaths.head_ = 0;
    WidePaths.head_ = static_cast<CBankManagerWidePathSet::CNode*>(malloc(0x14));
    WidePaths.size_ = 0;
    WidePaths.head_->color_ = false;
    WidePaths.head_->parent_ = 0;
    WidePaths.head_->left_ = WidePaths.head_;
    WidePaths.head_->right_ = WidePaths.head_;

    UseRetailBanks = false;
    retailBankVersion_ = 100;
}
