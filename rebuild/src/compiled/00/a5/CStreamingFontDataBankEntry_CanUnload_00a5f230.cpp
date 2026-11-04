#include "engine/CStreamingFontDataBankEntry.h"
bool CStreamingFontDataBankEntry::CanUnload() const
{
    if (Font.Data && Font.Info)
    {
        unsigned long count = Font.Info->RefCount;
        if (count == 1) return count == 1;
    }
    return false;
}
