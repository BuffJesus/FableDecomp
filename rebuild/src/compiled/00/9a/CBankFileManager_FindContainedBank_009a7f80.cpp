// Retail 009A7F80: original-exact 270-byte contained-bank lookup and sharing.
#include "fable_retail_banks.h"
bool CBankFileManager::FindContainedBank(const CCharString& handle,NBankFileManager::CContainedBank& info,CCountedPointer<NBankFileManager::CRetailBank>& result)
{
    CCharString resolved=Aliases.Resolve(handle);
    for(CRetailBankList::CNode* node=RetailBanks.node_->next_;node!=RetailBanks.node_;node=node->next_) {
        CCountedPointer<NBankFileManager::CRetailBank> bank=node->value_;
        NBankFileManager::CContainedBankMap::CNode* found=bank->containedBanks_.Find(resolved);
        if(found!=bank->containedBanks_.head_) {
            info=found->value_.second;
            result=bank;
            return true;
        }
    }
    return false;
}
