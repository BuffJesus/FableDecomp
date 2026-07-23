#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/outline_glow_compiled.h"

fable_u8* DAT_01436ea8;
static int g_insertCalls;
static int g_freeCalls;
static int g_insertedNode;
static void* g_freedAllocation;

void LandscapeLayerList::InsertNode(int node)
{
    ++g_insertCalls;
    g_insertedNode = node;
}

void CEngineLandscapeMemoryManager::FreeVirtual(void* allocation)
{
    ++g_freeCalls;
    g_freedAllocation = allocation;
}

static int RunCase(unsigned int freeFlags, int expectedFreeCalls)
{
    fable_u8 effectStorage[sizeof(CEngineScreenEffectOutlineGlow)];
    CEngineScreenEffectOutlineGlow* effect =
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(effectStorage);
    CEngineLandscapeMemoryManager memoryManager;
    fable_u8 globals[0x6B4];
    memset(effect, 0, sizeof(*effect));
    memset(globals, 0, sizeof(globals));
    *reinterpret_cast<CEngineLandscapeMemoryManager**>(globals + 0x6B0) = &memoryManager;
    DAT_01436ea8 = globals;
    effect->Blur2Texture = operator new(8);
    effect->field_10 = 0x2468;
    g_insertCalls = 0;
    g_freeCalls = 0;
    g_insertedNode = 0;
    g_freedAllocation = 0;

    CEngineScreenEffectOutlineGlow* result = effect->ScalarDeletingDestructor(freeFlags);

    if (result != effect) return 1;
    if (effect->vfptr != reinterpret_cast<void**>(0x0129B860)) return 2;
    if (effect->Blur2Texture != 0 || effect->field_10 != 0) return 3;
    if (g_insertCalls != 1 || g_insertedNode != 0x2468) return 4;
    if (g_freeCalls != expectedFreeCalls) return 5;
    if (expectedFreeCalls && g_freedAllocation != effect) return 6;
    return 0;
}

int main()
{
    const int retained = RunCase(0, 0);
    if (retained != 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL retained=%d\n", retained);
        return 100 + retained;
    }
    const int freed = RunCase(1, 1);
    if (freed != 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL freed=%d\n", freed);
        return 200 + freed;
    }
    printf("OUTLINEGLOW_SCALAR_TEST PASS\n");
    return 0;
}
