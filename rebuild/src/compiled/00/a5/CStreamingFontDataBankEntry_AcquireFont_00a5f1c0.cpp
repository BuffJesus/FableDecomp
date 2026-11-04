#include "engine/CStreamingFontDataBankEntry.h"
#include "engine/CStreamingFontBank.h"
template<> inline CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
CCountedPointer<CFontBank> CStreamingFontDataBankEntry::AcquireFont() const
{
    if (Font.IsNull())
    {
        unsigned long index = Index;
        CStreamingFontDataBank* bank = Bank;
        Font.Reset(reinterpret_cast<CFontBank*>(new CStreamingFontBank(bank, index)));
    }
    return Font;
}
