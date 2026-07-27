#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/9d/CStateBlockFunctionSold_Apply_009df060.cpp"

fable_u32 DAT_01396f6c = 2;
fable_u32 DAT_01396f68 = 1;
fable_u32 DAT_01396e04 = 4;
fable_u32 DAT_01396ef0 = 2;
fable_u32 DAT_01396eec = 0;
fable_u32 DAT_01396e7c = 4;
fable_u32 DAT_01396e70 = 1;
fable_u32 DAT_01396e5c = 2;

static fable_u8 g_SystemManager[0x64];
static fable_u8 g_RenderSystem[0x0C];
static fable_u8 g_StateManager[0x3600];

void* FABLE_FASTCALL GFGetSystemManager()
{
    return g_SystemManager;
}

static const fable_u32 g_StateOffsets[17] = {
    0x291C, 0x2930, 0x2868, 0x28B8, 0x28CC, 0x2854,
    0x287C, 0x3344, 0x33E4, 0x3484, 0x3524, 0x3204,
    0x32A4, 0x3218, 0x32B8, 0x30C4, 0x3164
};

static fable_u32 RequestedValue(fable_u32 index)
{
    static const fable_u32 values[17] = {
        2, 1, 1, 0, 0, 1, 4, 2, 0, 2, 0, 4, 4, 1,
        1, 2, 2
    };
    return values[index];
}

static CTrackedDwordStateSoldView* State(fable_u32 index)
{
    return reinterpret_cast<CTrackedDwordStateSoldView*>(
        g_StateManager + g_StateOffsets[index]);
}

static void ResetStateManager()
{
    memset(g_SystemManager, 0, sizeof(g_SystemManager));
    memset(g_RenderSystem, 0, sizeof(g_RenderSystem));
    memset(g_StateManager, 0, sizeof(g_StateManager));
    *reinterpret_cast<fable_u8**>(
        g_SystemManager + 0x60) = g_RenderSystem;
    *reinterpret_cast<fable_u8**>(
        g_RenderSystem + 0x08) = g_StateManager;
    *reinterpret_cast<fable_u32*>(
        g_StateManager + 0x2814) = 4;

    for (fable_u32 index = 0; index != 17; ++index)
    {
        State(index)->requestedValue04 =
            0x1000 + index;
    }
}

static bool VerifyQueuedState(
    fable_u32 index,
    fable_u32 captureIndex,
    fable_u32 dirtyIndex)
{
    CTrackedDwordStateSoldView* state = State(index);
    CCapturedDwordStateSoldView* captures =
        reinterpret_cast<CCapturedDwordStateSoldView*>(
            g_StateManager + 0x0008);
    CTrackedDwordStateSoldView** dirty =
        reinterpret_cast<CTrackedDwordStateSoldView**>(
            g_StateManager + 0x2008);
    return
        state->requestedValue04 == RequestedValue(index) &&
        state->captureMask08 == 4 &&
        state->queuedForRealise10 == 1 &&
        captures[captureIndex].state00 == state &&
        captures[captureIndex].priorRequestedValue04 ==
            0x1000 + index &&
        dirty[dirtyIndex] == state;
}

int main()
{
    CStateBlockFunctionSoldView sold;
    ResetStateManager();
    sold.Apply();

    if (*reinterpret_cast<fable_u32*>(
            g_StateManager + 0x2808) != 17 ||
        *reinterpret_cast<fable_u32*>(
            g_StateManager + 0x280C) != 17)
    {
        printf("FABLETLC_STATE_BLOCK_SOLD_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    for (fable_u32 index = 0; index != 17; ++index)
    {
        if (!VerifyQueuedState(index, index, index))
        {
            printf(
                "FABLETLC_STATE_BLOCK_SOLD_BEHAVIOR FAIL code=2 index=%lu\n",
                index);
            return 2;
        }
    }

    sold.Apply();
    if (*reinterpret_cast<fable_u32*>(
            g_StateManager + 0x2808) != 17 ||
        *reinterpret_cast<fable_u32*>(
            g_StateManager + 0x280C) != 17)
    {
        printf("FABLETLC_STATE_BLOCK_SOLD_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    ResetStateManager();
    State(0)->captureMask08 = 4;
    State(0)->queuedForRealise10 = 1;
    sold.Apply();
    if (*reinterpret_cast<fable_u32*>(
            g_StateManager + 0x2808) != 16 ||
        *reinterpret_cast<fable_u32*>(
            g_StateManager + 0x280C) != 16 ||
        State(0)->requestedValue04 != RequestedValue(0))
    {
        printf("FABLETLC_STATE_BLOCK_SOLD_BEHAVIOR FAIL code=4\n");
        return 4;
    }

    printf("FABLETLC_STATE_BLOCK_SOLD_BEHAVIOR PASS\n");
    return 0;
}
