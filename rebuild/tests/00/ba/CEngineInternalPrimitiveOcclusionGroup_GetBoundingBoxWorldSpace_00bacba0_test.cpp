#include <stdio.h>

struct C3DBoundingBox { float mn[3]; float mx[3]; };

struct COcclusionInner {
    int tag;
    bool occ(C3DBoundingBox& bbox, int f58, int f5c);
};

static COcclusionInner* g_self;
static C3DBoundingBox* g_bbox;
static int g_f58, g_f5c;

bool COcclusionInner::occ(C3DBoundingBox& bbox, int f58, int f5c)
{
    g_self = this;
    g_bbox = &bbox;
    g_f58 = f58;
    g_f5c = f5c;
    return true;
}

struct CEngineInternalPrimitiveOcclusionGroup {
    char pad[0x54];
    COcclusionInner* f54;
    int f58;
    int f5c;
};

bool __fastcall CEngineInternalPrimitiveOcclusionGroup_GetBoundingBoxWorldSpace(
        CEngineInternalPrimitiveOcclusionGroup* self, int /*edx*/, C3DBoundingBox& bbox);

int main()
{
    CEngineInternalPrimitiveOcclusionGroup obj;
    COcclusionInner inner; inner.tag = 0x1234;
    obj.f54 = &inner;
    obj.f58 = 0x58585858;
    obj.f5c = 0x5c5c5c5c;
    C3DBoundingBox bb;

    bool r = CEngineInternalPrimitiveOcclusionGroup_GetBoundingBoxWorldSpace(&obj, 0, bb);

    if (r && g_self == &inner && g_bbox == &bb && g_f58 == 0x58585858 && g_f5c == 0x5c5c5c5c)
        printf("00bacba0_TEST PASS\n");
    else
        printf("FAIL r=%d self=%p bbox=%p f58=%x f5c=%x\n", r, g_self, g_bbox, g_f58, g_f5c);
    return 0;
}