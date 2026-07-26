#include "fable_texture_lifecycle.h"
#include "fable_pixel_format_table.h"

fable_u32 CPixelFormatByteLengthView::GetColourDepth() const
{
    return static_cast<fable_u32>(
        g_FablePixelFormatTable[value00].colourDepth08);
}
