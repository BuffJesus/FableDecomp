#include <cstdio>

struct CThingTrackNode;

struct IDrawable {
    void* slots[0x12];
    int (__fastcall *slot12)(void* self, int edx, CThingTrackNode* node);
};

struct IDrawableObj {
    IDrawable* vtbl;
};

struct CThingTrackNode {
    IDrawableObj* DrawIsDrawable();
};

static IDrawableObj g_obj;
static CThingTrackNode* g_seen_node = 0;
static void* g_seen_self = 0;

IDrawableObj* __fastcall GetDrawableObject(CThingTrackNode* self)
{
    return &g_obj;
}

static int __fastcall my_slot12(void* self, int edx, CThingTrackNode* node)
{
    g_seen_self = self;
    g_seen_node = node;
    return 7;
}

IDrawableObj* CThingTrackNode::DrawIsDrawable()
{
    IDrawableObj* obj = GetDrawableObject(this);
    obj->vtbl->slot12(obj, 0, this);
    return obj;
}

int main()
{
    IDrawable vt;
    vt.slot12 = my_slot12;
    g_obj.vtbl = &vt;

    CThingTrackNode node;
    IDrawableObj* r = node.DrawIsDrawable();

    if (r == &g_obj && g_seen_self == &g_obj && g_seen_node == &node)
        printf("00afe580_TEST PASS\n");
    else
        printf("FAIL r=%p self=%p node=%p\n", (void*)r, g_seen_self, (void*)g_seen_node);
    return 0;
}