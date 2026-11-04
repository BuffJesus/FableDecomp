#include "fable_ime.h"

extern unsigned char g_IMEAlphaDefault_013750B0;

// The PDB-named conversion is inlined by SetColor. Its separate retail body
// at 00404CF0 is not claimed as reconstructed by this translation unit.
static inline CRGBColour FABLE_FASTCALL IMEColour2RGB(unsigned long colour)
{
    const unsigned char blue = static_cast<unsigned char>(colour);
    colour >>= 8;
    const unsigned char green = static_cast<unsigned char>(colour);
    colour >>= 8;
    return CRGBColour(static_cast<unsigned char>(colour), green, blue,
        g_IMEAlphaDefault_013750B0);
}

void NUISystem::CIMEFont::SetColor(unsigned long colour)
{
    Colour = IMEColour2RGB(colour);
}
