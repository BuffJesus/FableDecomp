#include "fable_system_manager.h"
#include "engine/CStreamingFontDataBank.h"
#include "engine/CStreamingFontDataBankEntry.h"
template<> inline CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
CCountedPointer<CFontBank> CFontManager::GetStreamingFont(unsigned long index)
{
    CCountedPointer<CFontBank> font = PStreamingFontDataBank->PeekFontInfo(index)->AcquireFont();
    font->PManager = this;
    return font;
}
