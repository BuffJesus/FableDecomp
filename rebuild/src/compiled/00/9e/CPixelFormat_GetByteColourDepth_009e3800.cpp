#include "engine/CPixelFormat.h"  // retyped onto the PDB layout; byte parity re-verified
struct PixFmtEntry { int bits; int pad[8]; };
extern PixFmtEntry g_pixFmtTable[];
unsigned long __fastcall GetByteColourDepth(CPixelFormat* self)
{
    int bits = g_pixFmtTable[self->FormatIndex].bits;
    return (unsigned long)(bits / 8);
}