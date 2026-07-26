#include <stdio.h>

#include "fable_boot.h"

namespace
{
    struct CountedDisplayView
    {
        CProgressDisplay* object;
        FableReferenceCount* reference;
    };

    int g_destroyCalls = 0;
    int g_deleteCalls = 0;
    void* g_destroyedObject = 0;
    void* g_deletedReference = 0;

    void FABLE_FASTCALL DestroyDisplay(void* object)
    {
        ++g_destroyCalls;
        g_destroyedObject = object;
    }

    void ResetObservations()
    {
        g_destroyCalls = 0;
        g_deleteCalls = 0;
        g_destroyedObject = 0;
        g_deletedReference = 0;
    }
}

void* FABLE_CDECL FableOperatorNew(fable_u32 /* size */)
{
    return 0;
}

void FABLE_CDECL FableOperatorDelete(void* allocation)
{
    ++g_deleteCalls;
    g_deletedReference = allocation;
}

int main()
{
    fable_u8 oldDisplayStorage[sizeof(CProgressDisplay)] = {};
    fable_u8 incomingDisplayStorage[sizeof(CProgressDisplay)] = {};
    CProgressDisplay* oldDisplay =
        reinterpret_cast<CProgressDisplay*>(oldDisplayStorage);
    CProgressDisplay* incomingDisplay =
        reinterpret_cast<CProgressDisplay*>(incomingDisplayStorage);
    FableReferenceCount oldReference = {
        1,
        DestroyDisplay,
        oldDisplay
    };
    FableReferenceCount incomingReference = {
        2,
        DestroyDisplay,
        incomingDisplay
    };
    CountedDisplayView incoming = {
        incomingDisplay,
        &incomingReference
    };

    ResetObservations();
    g_FableProgressDisplayObject_013CAA38 = oldDisplay;
    g_FableProgressDisplayReference_013CAA3C = &oldReference;
    SetProgressDisplay(
        reinterpret_cast<CCountedProgressDisplay*>(&incoming));

    if (
        g_FableProgressDisplayObject_013CAA38 != incomingDisplay ||
        g_FableProgressDisplayReference_013CAA3C != &incomingReference ||
        oldReference.owners != 0 ||
        incomingReference.owners != 3 ||
        g_destroyCalls != 1 ||
        g_destroyedObject != oldDisplay ||
        g_deleteCalls != 1 ||
        g_deletedReference != &oldReference)
    {
        return 1;
    }

    ResetObservations();
    incomingReference.owners = 1;
    CountedDisplayView empty = {0, 0};
    SetProgressDisplay(
        reinterpret_cast<CCountedProgressDisplay*>(&empty));
    if (
        g_FableProgressDisplayObject_013CAA38 != 0 ||
        g_FableProgressDisplayReference_013CAA3C != 0 ||
        incomingReference.owners != 0 ||
        g_destroyCalls != 1 ||
        g_destroyedObject != incomingDisplay ||
        g_deleteCalls != 1 ||
        g_deletedReference != &incomingReference)
    {
        return 2;
    }

    ResetObservations();
    FableReferenceCount orphanReference = {
        1,
        DestroyDisplay,
        oldDisplay
    };
    g_FableProgressDisplayObject_013CAA38 = 0;
    g_FableProgressDisplayReference_013CAA3C = &orphanReference;
    SetProgressDisplay(
        reinterpret_cast<CCountedProgressDisplay*>(&empty));
    if (
        g_FableProgressDisplayObject_013CAA38 != 0 ||
        g_FableProgressDisplayReference_013CAA3C != 0 ||
        orphanReference.owners != 0 ||
        g_destroyCalls != 1 ||
        g_destroyedObject != oldDisplay ||
        g_deleteCalls != 1 ||
        g_deletedReference != &orphanReference)
    {
        return 3;
    }

    printf("FABLETLC_SET_PROGRESS_DISPLAY_BEHAVIOR PASS\n");
    return 0;
}
