#include "fable_retail_banks.h"

bool CBankFileManager::BankExists(const CCharString& bankHandle) const
{
    CCharString resolved = Aliases.Resolve(bankHandle);
    if (UseRetailBanks)
    {
        CRetailBankList::CNode* node = RetailBanks.node_->next_;
        CRetailBankList::CNode* end = RetailBanks.node_;
        for (; node != end; node = node->next_)
        {
            if (node->value_.Data->ContainsBank(resolved))
                return true;
        }
    }
    else if (Banks.Find(resolved) != Banks.head_)
        return true;
    return false;
}
