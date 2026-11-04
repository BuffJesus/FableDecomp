#include "fable_ime.h"
#include "fable_persist_context.h"
#include "fable_string.h"

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
        reinterpret_cast<signed char&>(g_FableIME_013B7CD8.Appearance.symbolHeight),
        defaultValue);
    defaultValue = -0x60;
    context.TransferSignedChar(
        "SymbolTranslucence",
        reinterpret_cast<signed char&>(g_FableIME_013B7CD8.Appearance.symbolTranslucence),
        defaultValue);
    defaultValue = 0;
    context.TransferSignedChar(
        "SymbolPlacement",
        reinterpret_cast<signed char&>(g_FableIME_013B7CD8.Appearance.symbolPlacement),
        defaultValue);
    defaultValue = -0x80;
    context.TransferSignedChar(
        "CompTranslucence",
        reinterpret_cast<signed char&>(g_FableIME_013B7CD8.Appearance.compTranslucence),
        defaultValue);
    defaultValue = 2;
    context.TransferSignedChar(
        "CaretWidth",
        reinterpret_cast<signed char&>(g_FableIME_013B7CD8.Appearance.caretWidth),
        defaultValue);
    defaultValue = 1;
    context.TransferSignedChar(
        "CaretYMargin",
        reinterpret_cast<signed char&>(g_FableIME_013B7CD8.Appearance.caretYMargin),
        defaultValue);

#define FABLE_LOAD_IME_COLOUR(text, destination, fallback) \
    { \
        CCharString token(text, -1); \
        destination = TransferIMEColour(context, token, fallback); \
    }

    FABLE_LOAD_IME_COLOUR(
        "SymbolColor",
        g_FableIME_013B7CD8.Appearance.symbolColor,
        0);
    FABLE_LOAD_IME_COLOUR(
        "SymbolColorOff",
        g_FableIME_013B7CD8.Appearance.symbolColorOff,
        0x00404040);
    FABLE_LOAD_IME_COLOUR(
        "SymbolColorText",
        g_FableIME_013B7CD8.Appearance.symbolColorText,
        0xFF000000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorBase",
        g_FableIME_013B7CD8.Appearance.candColorBase,
        0xFFFFFFFF);
    FABLE_LOAD_IME_COLOUR(
        "CandColorBorder",
        g_FableIME_013B7CD8.Appearance.candColorBorder,
        0xFF000000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorText",
        g_FableIME_013B7CD8.Appearance.candColorText,
        0);
    FABLE_LOAD_IME_COLOUR(
        "CandColorInput",
        g_FableIME_013B7CD8.Appearance.compColorInput,
        0x00FFFF00);
    FABLE_LOAD_IME_COLOUR(
        "CandColorTargetConv",
        g_FableIME_013B7CD8.Appearance.compColorTargetConv,
        0x000000FF);
    FABLE_LOAD_IME_COLOUR(
        "CandColorConverted",
        g_FableIME_013B7CD8.Appearance.compColorConverted,
        0x0000FF00);
    FABLE_LOAD_IME_COLOUR(
        "CandColorTargetNotConv",
        g_FableIME_013B7CD8.Appearance.compColorTargetNotConv,
        0x00FF0000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorInputErr",
        g_FableIME_013B7CD8.Appearance.compColorInputErr,
        0x00FF0000);
    FABLE_LOAD_IME_COLOUR(
        "CandColorText",
        g_FableIME_013B7CD8.Appearance.candColorText,
        0);

#undef FABLE_LOAD_IME_COLOUR

    bool defaultBool = true;
    context.Transfer<bool>(
        "CancelOnOKClick",
        g_FableIME_013B7CD8.CancelOnOKClick,
        defaultBool);
    defaultBool = false;
    context.Transfer<bool>(
        "EnableIME",
        g_FableIME_013B7CD8.EnableIME,
        defaultBool);
}
