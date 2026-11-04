#include "fable_ime.h"

extern NUISystem::CIME g_FableIME_013B7CD8;

void FABLE_FASTCALL NUISystem::CIME::ResetToDefaultSettings()
{
    IMEUI_APPEARANCE& appearance = g_FableIME_013B7CD8.Appearance;
    appearance.symbolColor = 0;
    appearance.symbolColorOff = 0x00404040;
    appearance.symbolColorText = 0xff000000;
    appearance.symbolHeight = 24;
    appearance.symbolTranslucence = 160;
    appearance.symbolPlacement = 0;
    appearance.symbolFont = 0;
    appearance.candColorBase = 0xffffffff;
    appearance.candColorBorder = 0xff000000;
    appearance.candColorText = 0;
    appearance.compColorInput = 0x00ffff00;
    appearance.compColorTargetConv = 0x000000ff;
    appearance.compColorConverted = 0x0000ff00;
    appearance.compColorTargetNotConv = 0x00ff0000;
    appearance.compColorInputErr = 0x00ff0000;
    appearance.compTranslucence = 128;
    appearance.compColorText = 0;
    appearance.caretWidth = 2;
    appearance.caretYMargin = 1;
}
