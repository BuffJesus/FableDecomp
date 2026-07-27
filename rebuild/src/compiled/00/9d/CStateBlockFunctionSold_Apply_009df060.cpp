#include "fable_state_block_sold.h"

template <typename T>
static __forceinline T& SoldStateAt(
    fable_u8* base,
    fable_u32 offset)
{
    return *reinterpret_cast<T*>(base + offset);
}

static __forceinline void QueueSoldTrackedDwordState(
    fable_u8* manager,
    fable_u32 stateOffset,
    fable_u32 requestedValue)
{
    CTrackedDwordStateSoldView* state =
        reinterpret_cast<CTrackedDwordStateSoldView*>(
            manager + stateOffset);
    if (state->requestedValue04 == requestedValue)
    {
        return;
    }

    const fable_u32 activeCaptureMask =
        SoldStateAt<fable_u32>(manager, 0x2814);
    const fable_u32 stateCaptureMask = state->captureMask08;
    if ((activeCaptureMask & stateCaptureMask) == 0)
    {
        state->captureMask08 =
            stateCaptureMask | activeCaptureMask;

        fable_u32& captureCount =
            SoldStateAt<fable_u32>(manager, 0x2808);
        CCapturedDwordStateSoldView* captureStack =
            reinterpret_cast<CCapturedDwordStateSoldView*>(
                manager + 0x0008);
        captureStack[captureCount].state00 = state;
        captureStack[captureCount].priorRequestedValue04 =
            state->requestedValue04;
        ++captureCount;
    }

    state->requestedValue04 = requestedValue;
    if (state->queuedForRealise10 == 0)
    {
        state->queuedForRealise10 = 1;

        fable_u32& dirtyCount =
            SoldStateAt<fable_u32>(manager, 0x280C);
        CTrackedDwordStateSoldView** dirtyStates =
            reinterpret_cast<CTrackedDwordStateSoldView**>(
                manager + 0x2008);
        dirtyStates[dirtyCount] = state;
        ++dirtyCount;
    }
}

void CStateBlockFunctionSoldView::Apply()
{
    fable_u8* systemManager =
        static_cast<fable_u8*>(GFGetSystemManager());
    fable_u8* renderSystem =
        SoldStateAt<fable_u8*>(systemManager, 0x60);
    fable_u8* manager =
        SoldStateAt<fable_u8*>(renderSystem, 0x08);

    QueueSoldTrackedDwordState(
        manager, 0x291C, DAT_01396f6c);
    QueueSoldTrackedDwordState(
        manager, 0x2930, DAT_01396f68);
    QueueSoldTrackedDwordState(manager, 0x2868, 1);
    QueueSoldTrackedDwordState(manager, 0x28B8, 0);
    QueueSoldTrackedDwordState(manager, 0x28CC, 0);
    QueueSoldTrackedDwordState(manager, 0x2854, 1);
    QueueSoldTrackedDwordState(
        manager, 0x287C, DAT_01396e04);

    QueueSoldTrackedDwordState(
        manager, 0x3344, DAT_01396ef0);
    QueueSoldTrackedDwordState(
        manager, 0x33E4, DAT_01396eec);
    QueueSoldTrackedDwordState(
        manager, 0x3484, DAT_01396ef0);
    QueueSoldTrackedDwordState(
        manager, 0x3524, DAT_01396eec);
    QueueSoldTrackedDwordState(
        manager, 0x3204, DAT_01396e7c);
    QueueSoldTrackedDwordState(
        manager, 0x32A4, DAT_01396e7c);
    QueueSoldTrackedDwordState(
        manager, 0x3218, DAT_01396e70);
    QueueSoldTrackedDwordState(
        manager, 0x32B8, DAT_01396e70);

    QueueSoldTrackedDwordState(
        manager, 0x30C4, DAT_01396e5c);
    QueueSoldTrackedDwordState(
        manager, 0x3164, DAT_01396e5c);
}
