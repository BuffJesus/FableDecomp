#include "fable_system_manager.h"
#include "engine/CFontDataBank.h"
#include "engine/CFontDataBankEntry.h"
template<> inline CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
CCountedPointer<CFontBank> CFontManager::GetStaticFont(unsigned long index)
{
    CCountedPointer<CFontBank> font = PFontDataBank->PeekFontInfo(index)->AcquireFont();
    font->PManager = this;
    return font;
}
