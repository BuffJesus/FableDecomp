#include "engine/CStaticFontBank.h"

float __fastcall GetFontMaxHeight(CStaticFontBank* self)
{
    return (float)(unsigned long)self->m_maxHeight;
}
