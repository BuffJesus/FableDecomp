#include "fable_boot.h"

CProgressDisplay* g_FableProgressDisplayObject_013CAA38 = 0;
FableReferenceCount* g_FableProgressDisplayReference_013CAA3C = 0;

void FABLE_FASTCALL SetProgressDisplay(
    CCountedProgressDisplay* display)
{
    if (g_FableProgressDisplayObject_013CAA38 != 0)
    {
        if (g_FableProgressDisplayReference_013CAA3C != 0)
        {
            --g_FableProgressDisplayReference_013CAA3C->owners;
            if (g_FableProgressDisplayReference_013CAA3C->owners == 0)
            {
                g_FableProgressDisplayReference_013CAA3C->destroy(
                    g_FableProgressDisplayReference_013CAA3C->object);
                FableOperatorDelete(
                    g_FableProgressDisplayReference_013CAA3C);
            }
        }

        g_FableProgressDisplayReference_013CAA3C = 0;
        g_FableProgressDisplayObject_013CAA38 = 0;
    }

    FableReferenceCount* incomingReference = display->reference;
    CProgressDisplay* incomingObject = display->object;
    if (g_FableProgressDisplayReference_013CAA3C != incomingReference)
    {
        if (g_FableProgressDisplayReference_013CAA3C != 0)
        {
            --g_FableProgressDisplayReference_013CAA3C->owners;
            if (g_FableProgressDisplayReference_013CAA3C->owners == 0)
            {
                g_FableProgressDisplayReference_013CAA3C->destroy(
                    g_FableProgressDisplayReference_013CAA3C->object);
                FableOperatorDelete(
                    g_FableProgressDisplayReference_013CAA3C);
            }
        }

        g_FableProgressDisplayObject_013CAA38 = incomingObject;
        g_FableProgressDisplayReference_013CAA3C = incomingReference;
        if (g_FableProgressDisplayReference_013CAA3C != 0)
            ++g_FableProgressDisplayReference_013CAA3C->owners;
    }
}
