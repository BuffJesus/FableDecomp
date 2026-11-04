#include "fable_retail_banks.h"

CBankFileManager::~CBankFileManager()
{
    Close();
    WidePaths.clear();
    if (WidePaths.head_ != 0)
        free(WidePaths.head_);
    BasePath().~CWideString();
    Aliases.clear();
    if (Aliases.head_ != 0)
        free(Aliases.head_);
    RetailBanks.clear();
    if (RetailBanks.node_ != 0)
        free(RetailBanks.node_);
    Banks.clear();
    if (Banks.head_ != 0)
        free(Banks.head_);
}
