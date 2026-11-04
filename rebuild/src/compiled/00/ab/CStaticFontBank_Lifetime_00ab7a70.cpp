#include <new>
#include "fable_font_instances.h"
#include "fable_gfuninitialise.h"
#include "fable_surface_types.h"
CStaticFontBank::~CStaticFontBank()
{
    for (unsigned i=0; i<1024; ++i) {
        if (GlyphBanks[i].Glyphs) {
            delete[] GlyphBanks[i].Glyphs;
            GlyphBanks[i].Glyphs=0;
        }
    }
    if (GFGetSystemManager() && GFGetSystemManager()->IsValid() && GFGetSystemManager()->GetDisplayManager()) {
        delete PTexture;
        PTexture=0;
    }
}
