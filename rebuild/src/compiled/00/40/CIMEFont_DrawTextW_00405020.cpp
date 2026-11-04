#include "fable_ime.h"
#include "fable_system_manager.h"
#include "fable_system_manager_state.h"
#include "fable_gfuninitialise.h"
#include "fable_font_bank.h"
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
extern unsigned char g_IMEAlphaDefault_013750B0;

void NUISystem::CIMEFont::DrawTextW(const wchar_t* text)
{
    CWideString string(text);
    CCharString name;
    float scale;
    GetFontNameAndScale(name, scale);
    CCountedPointer<CFontBank> font = GFGetSystemManager()->PFontManager.Data->GetFont(name);
    C2DVector position = Position;
    GFACoordToVCoord(position, position);
    Colour.A = g_IMEAlphaDefault_013750B0;
    if (Colour.R > 96 || Colour.G > 96 || Colour.B > 96)
    {
        C2DVector shadowPosition(Position.X - 2.0f, Position.Y + 2.0f);
        GFACoordToVCoord(shadowPosition, shadowPosition);
        CRGBColour shadow(0, 0, 0, Colour.A);
        C3DVector drawPosition(shadowPosition.X, shadowPosition.Y, 0.0f);
        GFGetSystemManager()->PDisplayManager.Data->GetRenderManager().Draw3DScaledText(
            drawPosition, scale, string, *font.Data, shadow, 0);
    }
    C3DVector drawPosition(position.X, position.Y, 0.0f);
    GFGetSystemManager()->PDisplayManager.Data->GetRenderManager().Draw3DScaledText(
        drawPosition, scale, string, *font.Data, Colour, 0);
}
