#include "fable_boot.h"

void FABLE_FASTCALL SetProgressDisplay(
    CCountedProgressDisplay* display)
{
    if (g_FableProgressDisplay_013CAA38.object != 0)
    {
        if (g_FableProgressDisplay_013CAA38.reference != 0)
        {
            --g_FableProgressDisplay_013CAA38.reference->owners;
            if (g_FableProgressDisplay_013CAA38.reference->owners == 0)
            {
                g_FableProgressDisplay_013CAA38.reference->destroy(
                    g_FableProgressDisplay_013CAA38.reference->object);
                FableOperatorDelete(
                    g_FableProgressDisplay_013CAA38.reference);
            }
        }

        g_FableProgressDisplay_013CAA38.reference = 0;
        g_FableProgressDisplay_013CAA38.object = 0;
    }

    FableReferenceCount* incomingReference = display->reference;
    CProgressDisplay* incomingObject = display->object;
    if (g_FableProgressDisplay_013CAA38.reference != incomingReference)
    {
        if (g_FableProgressDisplay_013CAA38.reference != 0)
        {
            --g_FableProgressDisplay_013CAA38.reference->owners;
            if (g_FableProgressDisplay_013CAA38.reference->owners == 0)
            {
                g_FableProgressDisplay_013CAA38.reference->destroy(
                    g_FableProgressDisplay_013CAA38.reference->object);
                FableOperatorDelete(
                    g_FableProgressDisplay_013CAA38.reference);
            }
        }

        g_FableProgressDisplay_013CAA38.object = incomingObject;
        g_FableProgressDisplay_013CAA38.reference = incomingReference;
        if (g_FableProgressDisplay_013CAA38.reference != 0)
            ++g_FableProgressDisplay_013CAA38.reference->owners;
    }
}
