#include "fable_font_instances.h"
C2DExtentsF CStaticFontBank::GetStringExtents(const wchar_t* text, bool firstLineOnly) const
{
    float maxY = 0.0f;
    float maxX = 0.0f;
    const float lineHeight = static_cast<float>(FontHeight + 1);
    float currentY = lineHeight;
    // Native stores the accumulated width as float after every glyph.
    volatile float currentX = 0.0f;
    wchar_t character;
    while ((character = *text++) != 0) {
        if (character == 10) {
            if (firstLineOnly) return C2DExtentsF(maxX, maxY);
            currentX = 0.0f;
            currentY += lineHeight;
        }
        if (static_cast<long>(character) < MinChar || static_cast<long>(character) > MaxChar) continue;
        const CStaticFontGlyphBank& bank = GlyphBanks[character >> 6];
        if (bank.Glyphs) {
            unsigned index = (character & 63) - bank.FirstGlyph;
            if (index < bank.NoGlyphs) {
                const CGlyph* glyph = &bank.Glyphs[index];
                if (glyph) currentX = glyph->Offset + currentX + glyph->Advance;
            }
        }
        if (currentX > maxX) maxX = currentX;
        if (currentY > maxY) maxY = currentY;
    }
    return C2DExtentsF(maxX, maxY);
}
