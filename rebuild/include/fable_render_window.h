#pragma once

#include "fable_display_viewport.h"

typedef CDisplayManager CDisplayManagerWindowView;

struct CRenderManagerCoreWindowView
{
    fable_u8 unknown0000[0x3A3C];
    CDisplayManagerWindowView* displayManager3A3C;

    void SetAWindow(const C2DBoxF& window);
};

FABLE_STATIC_ASSERT(sizeof(CRenderManagerCoreWindowView) == 0x3A40);
