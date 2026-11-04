#include "fable_string.h"
#include "fable_persist_context.h"

extern unsigned char g_IMEAlphaDefault_013750B0;

unsigned long FABLE_FASTCALL TransferIMEColour(CPersistContext& context, const CCharString& name, unsigned long colour)
{
    signed char red = static_cast<signed char>(colour >> 16);
    signed char green = static_cast<signed char>(colour >> 8);
    signed char blue = static_cast<signed char>(colour);
    signed char alpha = static_cast<signed char>(g_IMEAlphaDefault_013750B0);
    signed char redDefault = red, greenDefault = green, blueDefault = blue, alphaDefault = alpha;
    context.TransferSignedChar(name + "R", red, redDefault);
    context.TransferSignedChar(name + "G", green, greenDefault);
    context.TransferSignedChar(name + "B", blue, blueDefault);
    context.TransferSignedChar(name + "A", alpha, alphaDefault);
    return 0xff000000UL | (static_cast<unsigned long>(static_cast<unsigned char>(red)) << 16)
        | (static_cast<unsigned long>(static_cast<unsigned char>(green)) << 8)
        | static_cast<unsigned char>(blue);
}
