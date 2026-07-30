#include "fable_retail_banks.h"

void CBasicRetailBankManager::AddBankAlias(
    const CCharString& genericName,
    const CCharString& platformName)
{
    aliases_[platformName] = genericName;
}
