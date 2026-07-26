#include <stdio.h>

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

int main()
{
    CRenderStateEntry sentinel = {};
    sentinel.captureType11 = 4;
    CRenderStateEntry alreadyQueued = {};
    alreadyQueued.captureType11 = 2;
    alreadyQueued.captureOffset08 = 28;
    alreadyQueued.queuedForRestore10 = 1;
    CRenderStateEntry newlyQueued = {};
    newlyQueued.captureType11 = 1;
    newlyQueued.captureOffset08 = 20;

    CRenderStateManagerCaptureView manager = {};
    manager.captures0008[0].entry00 = &sentinel;
    manager.captures0008[0].value04 = 100;
    manager.captures0008[1].entry00 = &alreadyQueued;
    manager.captures0008[1].value04 = 200;
    manager.captures0008[2].entry00 = &newlyQueued;
    manager.captures0008[2].value04 = 300;
    manager.captureCount2808 = 3;
    manager.captureOffset2814 = 8;

    manager.RestoreCaptureBlock();

    if (manager.captureCount2808 != 0 ||
        manager.captureOffset2814 != 4 ||
        newlyQueued.captureOffset08 != 12 ||
        newlyQueued.capturedValue04 != 300 ||
        newlyQueued.queuedForRestore10 != 1 ||
        alreadyQueued.captureOffset08 != 20 ||
        alreadyQueued.capturedValue04 != 200 ||
        manager.pendingRestoreCount280C != 1 ||
        manager.pendingRestores2008[0] != &newlyQueued)
    {
        printf("FABLETLC_RESTORE_CAPTURE_BLOCK_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    CRenderStateManagerCaptureView emptyBlock = {};
    emptyBlock.captures0008[0].entry00 = &sentinel;
    emptyBlock.captureCount2808 = 1;
    emptyBlock.captureOffset2814 = 2;
    emptyBlock.RestoreCaptureBlock();
    if (emptyBlock.captureCount2808 != 0 ||
        emptyBlock.captureOffset2814 != 1 ||
        emptyBlock.pendingRestoreCount280C != 0)
    {
        printf("FABLETLC_RESTORE_CAPTURE_BLOCK_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_RESTORE_CAPTURE_BLOCK_BEHAVIOR PASS\n");
    return 0;
}
