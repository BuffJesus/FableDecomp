#pragma once

#include "rebuild_abi.h"

struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

struct CDisplayManagerWindowView
{
    void SetViewport(const C2DBoxF& viewport);
};

struct CRenderManagerCoreWindowView
{
    fable_u8 unknown0000[0x3A3C];
    CDisplayManagerWindowView* displayManager3A3C;

    void SetAWindow(const C2DBoxF& window);
};

FABLE_STATIC_ASSERT(sizeof(C2DBoxF) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CRenderManagerCoreWindowView) == 0x3A40);
