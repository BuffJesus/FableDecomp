// CBankFileManager::CloseRetailBank @ 0x009A78D0
// (FableWin ?CloseRetailBank@CBankFileManager@@QAEXABVCWideString@@@Z).
//
// Finds the retail bank opened from this path (case-insensitive match on the
// narrow path OpenRetailBank recorded) and drops it from the list, releasing
// the list's reference; the first match only.

#include "fable_retail_banks.h"

void CBankFileManager::CloseRetailBank(const CWideString& pathname)
{
    CCharString name = pathname.ToCharString();

    for (CRetailBankList::CNode* node = RetailBanks.node_->next_;
         node != RetailBanks.node_;
         node = node->next_)
    {
        if (name.EqualsCaseInsensitive(node->value_->filePath_))
        {
            RetailBanks.erase(node);
            return;
        }
    }
}