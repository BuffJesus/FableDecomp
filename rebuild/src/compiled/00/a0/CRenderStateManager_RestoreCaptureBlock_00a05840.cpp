#include "fable_render_capture.h"

void CRenderStateManagerCaptureView::RestoreCaptureBlock()
{
    --captureCount2808;
    CRenderStateEntry* entry =
        captures0008[captureCount2808].entry00;
    while (entry->captureType11 != 4)
    {
        entry->captureOffset08 -= captureOffset2814;
        entry->capturedValue04 =
            captures0008[captureCount2808].value04;
        if (entry->queuedForRestore10 == 0)
        {
            entry->queuedForRestore10 = 1;
            pendingRestores2008[pendingRestoreCount280C] = entry;
            ++pendingRestoreCount280C;
        }
        --captureCount2808;
        entry = captures0008[captureCount2808].entry00;
    }
    captureOffset2814 >>= 1;
}
