#include "fable_ime.h"
#include "fable_system_manager.h"
#include "fable_system_manager_state.h"
#include "fable_gfuninitialise.h"
#include "fable_font_bank.h"

void NUISystem::CIMEFont::GetTextExtent(const wchar_t* text,
    unsigned long* width, unsigned long* height)
{
    CCharString name;
    float scale;
    GetFontNameAndScale(name, scale);
    CCountedPointer<CFontBank> font = GFGetSystemManager()->PFontManager.Data->GetFont(name);
    const C2DExtentsF extents = font.Data->GetStringExtents(text, false);
    *width = static_cast<unsigned long>(extents.X * scale);
    *height = static_cast<unsigned long>(extents.Y * scale);
}
