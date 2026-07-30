#include "fable_ime.h"
#include "fable_persist_context.h"
#include "fable_string.h"

extern "C" signed char g_FableImeSymbolHeight_013B7CF0;
extern "C" signed char g_FableImeSymbolTranslucence_013B7CF1;
extern "C" signed char g_FableImeSymbolPlacement_013B7CF2;
extern "C" signed char g_FableImeCompositionTranslucence_013B7D18;
extern "C" signed char g_FableImeCaretWidth_013B7D20;
extern "C" signed char g_FableImeCaretYMargin_013B7D21;
extern "C" signed char g_FableImeCancelOnOkClick_013B7D28;
extern "C" signed char g_FableImeEnabled_013B7CD8;

extern "C" fable_u32 g_FableImeSymbolColour_013B7CE4;
extern "C" fable_u32 g_FableImeSymbolColourOff_013B7CE8;
extern "C" fable_u32 g_FableImeSymbolColourText_013B7CEC;
extern "C" fable_u32 g_FableImeCandidateColourBase_013B7CF8;
extern "C" fable_u32 g_FableImeCandidateColourBorder_013B7CFC;
extern "C" fable_u32 g_FableImeCandidateColourText_013B7D00;
extern "C" fable_u32 g_FableImeCandidateColourInput_013B7D04;
extern "C" fable_u32 g_FableImeCandidateColourTargetConverted_013B7D08;
extern "C" fable_u32 g_FableImeCandidateColourConverted_013B7D0C;
extern "C" fable_u32 g_FableImeCandidateColourTargetNotConverted_013B7D10;
extern "C" fable_u32 g_FableImeCandidateColourInputError_013B7D14;

fable_u32 FABLE_FASTCALL TransferIMEColour(
    CPersistContext& context,
    const CCharString& token,
    fable_u32 defaultColour);

void FABLE_FASTCALL NUISystem::CIME::LoadIMESettings(
    CPersistContext& context)
{
    signed char defaultValue = 0x18;
    context.TransferSignedChar(
        "SymbolHeight",
        g_FableImeSymbolHeight_013B7CF0,
        defaultValue);
    defaultValue = -0x60;
    context.TransferSignedChar(
        "SymbolTranslucence",
        g_FableImeSymbolTranslucence_013B7CF1,
        defaultValue);
    defaultValue = 0;
    context.TransferSignedChar(
        "SymbolPlacement",
        g_FableImeSymbolPlacement_013B7CF2,
        defaultValue);
    defaultValue = -0x80;
    context.TransferSignedChar(
        "CompTranslucence",
        g_FableImeCompositionTranslucence_013B7D18,
        defaultValue);
    defaultValue = 2;
    context.TransferSignedChar(
        "CaretWidth",
        g_FableImeCaretWidth_013B7D20,
        defaultValue);
    defaultValue = 1;
    context.TransferSignedChar(
        "CaretYMargin",
        g_FableImeCaretYMargin_013B7D21,
        defaultValue);

#define FABLE_LOAD_IME_COLOUR(text, destination, fallback) \
    { \
        CCharString token(text, -1); \
        destination = TransferIMEColour(context, token, fallback); \
    }

    FABLE_LOAD_IME_COLOUR(
        "SymbolColor",
        g_FableImeSymbolColour_013B7CE4,
        0);
    FABLE_LOAD_IME_COLOUR(
        "SymbolColorOff",
        g_FableImeSymbolColourOff_013B7CE8,
        0x00404040);
    FABLE_LOAD_IME_COLOUR(
        "SymbolColorText",
        g_FableImeSymbolColourText_013B7CEC,
        0xFF000000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorBase",
        g_FableImeCandidateColourBase_013B7CF8,
        0xFFFFFFFF);
    FABLE_LOAD_IME_COLOUR(
        "CandColorBorder",
        g_FableImeCandidateColourBorder_013B7CFC,
        0xFF000000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorText",
        g_FableImeCandidateColourText_013B7D00,
        0);
    FABLE_LOAD_IME_COLOUR(
        "CandColorInput",
        g_FableImeCandidateColourInput_013B7D04,
        0x00FFFF00);
    FABLE_LOAD_IME_COLOUR(
        "CandColorTargetConv",
        g_FableImeCandidateColourTargetConverted_013B7D08,
        0x000000FF);
    FABLE_LOAD_IME_COLOUR(
        "CandColorConverted",
        g_FableImeCandidateColourConverted_013B7D0C,
        0x0000FF00);
    FABLE_LOAD_IME_COLOUR(
        "CandColorTargetNotConv",
        g_FableImeCandidateColourTargetNotConverted_013B7D10,
        0x00FF0000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorInputErr",
        g_FableImeCandidateColourInputError_013B7D14,
        0x00FF0000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorText",
        g_FableImeCandidateColourText_013B7D00,
        0);

#undef FABLE_LOAD_IME_COLOUR

    defaultValue = 1;
    context.TransferSignedChar(
        "CancelOnOKClick",
        g_FableImeCancelOnOkClick_013B7D28,
        defaultValue);
    defaultValue = 0;
    context.TransferSignedChar(
        "EnableIME",
        g_FableImeEnabled_013B7CD8,
        defaultValue);
}
