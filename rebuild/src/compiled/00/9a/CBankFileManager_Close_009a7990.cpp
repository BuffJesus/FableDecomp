#include "fable_retail_banks.h"

void CBankFileManager::Close()
{
    while (RetailBanks.node_->next_ != RetailBanks.node_)
        RetailBanks.erase(RetailBanks.node_->prev_);
    Banks.clear();
}
