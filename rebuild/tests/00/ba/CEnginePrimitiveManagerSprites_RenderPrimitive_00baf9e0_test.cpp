#include "engine/CEnginePrimitiveManagerSprites.h"
#include <cstdio>

typedef int ERendered;
typedef int EEngineRenderLayer;

struct CEngineInternalPrimitiveBase { int tag; };
struct CEngineSubPrimitiveChain { int dummy; };


static int g_helperCalls = 0;
static int g_lastTag = -1;

void __stdcall SpriteHelper(CEngineInternalPrimitiveBase &prim) {
    g_helperCalls++;
    g_lastTag = prim.tag;
}

ERendered __fastcall RenderPrimitive(
    CEnginePrimitiveManagerSprites *self,
    int edx_dummy,
    CEngineInternalPrimitiveBase &prim,
    EEngineRenderLayer layer,
    CEngineSubPrimitiveChain &chain)
{
    (void)edx_dummy; (void)layer; (void)chain;
    if (self->enabled == 0)
        return 0;
    SpriteHelper(prim);
    return 2;
}

int main() {
    CEngineInternalPrimitiveBase prim; prim.tag = 77;
    CEngineSubPrimitiveChain chain;

    // disabled -> returns 0, no helper call
    CEnginePrimitiveManagerSprites off; off.enabled = 0;
    g_helperCalls = 0;
    ERendered r0 = RenderPrimitive(&off, 0, prim, 3, chain);

    // enabled -> returns 2, helper called once with prim
    CEnginePrimitiveManagerSprites on; on.enabled = 1;
    ERendered r1 = RenderPrimitive(&on, 0, prim, 3, chain);

    if (r0 == 0 && r1 == 2 && g_helperCalls == 1 && g_lastTag == 77)
        printf("PARITY_OK\n");
    else
        printf("FAIL r0=%d r1=%d calls=%d tag=%d\n", r0, r1, g_helperCalls, g_lastTag);
    return 0;
}