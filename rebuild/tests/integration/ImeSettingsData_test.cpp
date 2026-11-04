// Fixture for the IME settings data: the real provider's zero initial values,
// then the real CIME::LoadIMESettings filling every setting with its default.
// The fixture defines none of the settings; the transfer calls are stand-ins
// that keep the default (no settings file), recording each name once.
#include <stdio.h>
#include <string.h>
#include "fable_ime.h"
#include "fable_persist_context.h"
#include "fable_string.h"

// Fixture aliases inspect the original retail offsets in the actual singleton.
#define g_FableImeEnabled_013B7CD8 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x0))
#define g_FableImeSymbolHeight_013B7CF0 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x18))
#define g_FableImeSymbolTranslucence_013B7CF1 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x19))
#define g_FableImeSymbolPlacement_013B7CF2 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x1a))
#define g_FableImeCompositionTranslucence_013B7D18 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x40))
#define g_FableImeCaretWidth_013B7D20 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x48))
#define g_FableImeCaretYMargin_013B7D21 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x49))
#define g_FableImeCancelOnOkClick_013B7D28 (*reinterpret_cast<signed char*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x50))
#define g_FableImeSymbolColour_013B7CE4 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0xc))
#define g_FableImeSymbolColourOff_013B7CE8 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x10))
#define g_FableImeSymbolColourText_013B7CEC (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x14))
#define g_FableImeCandidateColourBase_013B7CF8 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x20))
#define g_FableImeCandidateColourBorder_013B7CFC (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x24))
#define g_FableImeCandidateColourText_013B7D00 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x28))
#define g_FableImeCandidateColourInput_013B7D04 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x2c))
#define g_FableImeCandidateColourTargetConverted_013B7D08 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x30))
#define g_FableImeCandidateColourConverted_013B7D0C (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x34))
#define g_FableImeCandidateColourTargetNotConverted_013B7D10 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x38))
#define g_FableImeCandidateColourInputError_013B7D14 (*reinterpret_cast<fable_u32*>(reinterpret_cast<unsigned char*>(&g_FableIME_013B7CD8)+0x3c))


static int g_bytes, g_bools, g_colours, g_strings;
void CPersistContext::TransferSignedChar(const char*, signed char& value, const signed char& defaultValue) { value = defaultValue; ++g_bytes; }
template<> void CPersistContext::Transfer<bool>(const char*, bool& value, const bool& defaultValue) { value = defaultValue; ++g_bools; }
fable_u32 FABLE_FASTCALL TransferIMEColour(CPersistContext&, const CCharString&, fable_u32 defaultColour) { ++g_colours; return defaultColour; }
CCharString::CCharString(const char*, long) { ++g_strings; }
CCharString::~CCharString() { --g_strings; }

int main()
{
    signed char* bytes[] = { &g_FableImeEnabled_013B7CD8, &g_FableImeSymbolHeight_013B7CF0, &g_FableImeSymbolTranslucence_013B7CF1,
        &g_FableImeSymbolPlacement_013B7CF2, &g_FableImeCompositionTranslucence_013B7D18, &g_FableImeCaretWidth_013B7D20,
        &g_FableImeCaretYMargin_013B7D21, &g_FableImeCancelOnOkClick_013B7D28 };
    fable_u32* colours[] = { &g_FableImeSymbolColour_013B7CE4, &g_FableImeSymbolColourOff_013B7CE8, &g_FableImeSymbolColourText_013B7CEC,
        &g_FableImeCandidateColourBase_013B7CF8, &g_FableImeCandidateColourBorder_013B7CFC, &g_FableImeCandidateColourText_013B7D00,
        &g_FableImeCandidateColourInput_013B7D04, &g_FableImeCandidateColourTargetConverted_013B7D08,
        &g_FableImeCandidateColourConverted_013B7D0C, &g_FableImeCandidateColourTargetNotConverted_013B7D10,
        &g_FableImeCandidateColourInputError_013B7D14 };
    int cases = 0;
    // Retail initial values: all zero.
    for (int i = 0; i < 8; ++i) { if (*bytes[i] != 0) return 1; ++cases; }
    for (int i = 0; i < 11; ++i) { if (*colours[i] != 0) return 2; ++cases; }
    // The defaults LoadIMESettings writes when nothing overrides them.
    static const signed char byteDefaults[] = { 0, 0x18, -0x60, 0, -0x80, 2, 1, 1 };
    static const fable_u32 colourDefaults[] = { 0, 0x00404040, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0, 0x00FFFF00, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00FF0000 };
    for (int round = 0; round < 3; ++round) {
        for (int i = 0; i < 8; ++i) *bytes[i] = (signed char)(0x55 + round);
        for (int i = 0; i < 11; ++i) *colours[i] = 0xA5A50000 + round;
        g_bytes = g_bools = g_colours = g_strings = 0;
        union { double align; char storage[64]; } context;
        NUISystem::CIME::LoadIMESettings(*reinterpret_cast<CPersistContext*>(&context));
        if (g_bytes != 6 || g_bools != 2 || g_colours != 12 || g_strings != 0) return 3;
        for (int i = 0; i < 8; ++i) { if (*bytes[i] != byteDefaults[i]) return 4; ++cases; }
        for (int i = 0; i < 11; ++i) { if (*colours[i] != colourDefaults[i]) return 5; ++cases; }
    }
    printf("IME_SETTINGS_DATA_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
