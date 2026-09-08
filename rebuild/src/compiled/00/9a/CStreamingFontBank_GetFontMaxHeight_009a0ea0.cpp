#include "engine/CStreamingFontBank.h"

float __fastcall GetFontMaxHeight(CStreamingFontBank* self)
{
    return (float)(unsigned long)self->m_maxHeight;
}
