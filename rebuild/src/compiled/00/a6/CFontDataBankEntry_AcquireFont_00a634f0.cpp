#include "engine/CFontDataBankEntry.h"
#include "engine/CStaticFontBank.h"
template<> inline CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
CCountedPointer<CFontBank> CFontDataBankEntry::AcquireFont() const
{
    if (Font.IsNull())
    {
        unsigned long index = Index;
        CFontDataBank* bank = Bank;
        Font.Reset(reinterpret_cast<CFontBank*>(new CStaticFontBank(bank, index)));
    }
    return Font;
}
