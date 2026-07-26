#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/outline_glow_compiled.h"

fable_u8* DAT_01436ea8;
static int g_insertCalls;
static int g_insertedNode;
static LandscapeLayerList* g_insertList;

void LandscapeLayerList::InsertNode(int node)
{
    ++g_insertCalls;
    g_insertedNode = node;
    g_insertList = this;
}

static int TestPopulated()
{
    fable_u8 effectStorage[sizeof(CEngineScreenEffectOutlineGlow)];
    CEngineScreenEffectOutlineGlow* effect =
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(effectStorage);
    LandscapeLayerList layerList;
    fable_u8 globals[0x6B4];
    memset(effect, 0, sizeof(*effect));
    memset(globals, 0, sizeof(globals));
    *reinterpret_cast<LandscapeLayerList**>(globals + 0x6B0) = &layerList;
    DAT_01436ea8 = globals;
    effect->Blur2Texture = operator new(8);
    effect->field_10 = 0x12345678;
    g_insertCalls = 0;
    g_insertedNode = 0;
    g_insertList = 0;

    effect->~CEngineScreenEffectOutlineGlow();

    if (effect->vfptr != reinterpret_cast<void**>(0x0129B860)) return 1;
    if (effect->Blur2Texture != 0) return 2;
    if (effect->field_10 != 0) return 3;
    if (g_insertCalls != 1 || g_insertedNode != 0x12345678) return 4;
    if (g_insertList != &layerList) return 5;
    return 0;
}

static int TestEmpty()
{
    fable_u8 effectStorage[sizeof(CEngineScreenEffectOutlineGlow)];
    CEngineScreenEffectOutlineGlow* effect =
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(effectStorage);
    LandscapeLayerList layerList;
    fable_u8 globals[0x6B4];
    memset(effect, 0, sizeof(*effect));
    memset(globals, 0, sizeof(globals));
    *reinterpret_cast<LandscapeLayerList**>(globals + 0x6B0) = &layerList;
    DAT_01436ea8 = globals;
    g_insertCalls = 0;

    effect->~CEngineScreenEffectOutlineGlow();

    if (effect->vfptr != reinterpret_cast<void**>(0x0129B860)) return 1;
    if (g_insertCalls != 0) return 2;
    return 0;
}

int main()
{
    const int populated = TestPopulated();
    if (populated != 0)
    {
        printf("OUTLINEGLOW_TEST FAIL populated=%d\n", populated);
        return 100 + populated;
    }
    const int empty = TestEmpty();
    if (empty != 0)
    {
        printf("OUTLINEGLOW_TEST FAIL empty=%d\n", empty);
        return 200 + empty;
    }
    printf("OUTLINEGLOW_TEST PASS\n");
    return 0;
}
