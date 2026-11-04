// Controlled failure-handler boundary. The callback releases queued handles
// through the real pool Free provider; it is not an engine reconstruction.
#include "fable_handle_memory_pool.h"
struct PoolFailureState {
    void* Vtable;
    CHandleBasedMemoryPool* Pool;
    long Pending[8];
    unsigned long Count, Next, Calls, LastSize;
};
FABLE_STATIC_ASSERT(sizeof(PoolFailureState)==56);
FABLE_STATIC_ASSERT(offsetof(PoolFailureState,Next)==44);
FABLE_STATIC_ASSERT(offsetof(PoolFailureState,Calls)==48);
extern "C" bool __fastcall PoolFailureCallback(PoolFailureState* state, void*, unsigned long size)
{
    ++state->Calls;
    state->LastSize = size;
    if (state->Next < state->Count)
    {
        state->Pool->Free(state->Pending[state->Next++]);
        return true;
    }
    return false;
}
