#include "fable_boot.h"

#include <stdlib.h>
#include <string.h>

GFInitialiseProgressPhaseState g_GFInitialiseProgressPhaseState = {};

bool g_FableProgressBeginEnabled = true;
fable_u32 g_FableProgressBeginCalls = 0;
fable_u32 g_FableProgressConstructCalls = 0;
fable_u32 g_FableProgressInstallCalls = 0;
fable_u32 g_FableProgressDestroyCalls = 0;

namespace
{
    void FABLE_FASTCALL DestroyProgressDisplay(void* object)
    {
        if (object != 0)
        {
            ++g_FableProgressDestroyCalls;
            FableOperatorDelete(object);
        }
    }
}

void* FABLE_CDECL FableOperatorNew(fable_u32 size)
{
    return malloc(size);
}

void FABLE_CDECL FableOperatorDelete(void* allocation)
{
    free(allocation);
}

CProgressDisplay::CProgressDisplay()
{
    memset(this, 0, sizeof(*this));
    ++g_FableProgressConstructCalls;
}

CCountedProgressDisplay::CCountedProgressDisplay(CProgressDisplay* value)
    : object(value),
      reference(0)
{
    if (value == 0)
        return;

    reference = static_cast<FableReferenceCount*>(
        FableOperatorNew(sizeof(FableReferenceCount)));
    if (reference == 0)
    {
        DestroyProgressDisplay(value);
        object = 0;
        return;
    }

    reference->owners = 1;
    reference->destroy = DestroyProgressDisplay;
    reference->object = value;
}

fable_u8 FABLE_FASTCALL GFInitialiseState_Begin(
    GFInitialiseState* /* state */)
{
    ++g_FableProgressBeginCalls;
    return g_FableProgressBeginEnabled ? 1 : 0;
}

void FABLE_FASTCALL SetProgressDisplay(
    CCountedProgressDisplay* /* display */)
{
    ++g_FableProgressInstallCalls;
}
