#pragma once

#include "rebuild_abi.h"

struct Render2DDrawListBlockView
{
    Render2DDrawListBlockView* begin00;
    Render2DDrawListBlockView* end04;
    fable_u8 unknown08[0x18];

    Render2DDrawListBlockView* CopyBlock(
        Render2DDrawListBlockView* destination,
        const Render2DDrawListBlockView* source);
};

FABLE_STATIC_ASSERT(sizeof(Render2DDrawListBlockView) == 0x20);
