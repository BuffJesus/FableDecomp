#pragma once

#include "rebuild_abi.h"

struct CRenderStateEntry
{
    fable_u32 unknown00;
    fable_u32 capturedValue04;
    fable_i32 captureOffset08;
    fable_u8 unknown0C[4];
    fable_u8 queuedForRestore10;
    fable_u8 captureType11;
};

struct CRenderStateCapture
{
    CRenderStateEntry* entry00;
    fable_u32 value04;
};

struct CRenderStateManagerCaptureView
{
    fable_u8 unknown0000[8];
    CRenderStateCapture captures0008[1024];
    CRenderStateEntry* pendingRestores2008[512];
    fable_i32 captureCount2808;
    fable_i32 pendingRestoreCount280C;
    fable_u8 unknown2810[4];
    fable_u32 captureOffset2814;

    void RestoreCaptureBlock();
};

FABLE_STATIC_ASSERT(sizeof(CRenderStateEntry) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CRenderStateCapture) == 8);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManagerCaptureView) == 0x2818);
