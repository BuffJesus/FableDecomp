#include <cstdio>

struct CThingTrackNode;

static CThingTrackNode* g_seen_node = 0;
static void* g_seen_self = 0;

struct IDrawableObj {
    virtual void pad00() {}
    virtual void pad01() {}
    virtual void pad02() {}
    virtual void pad03() {}
    virtual void pad04() {}
    virtual void pad05() {}
    virtual void pad06() {}
    virtual void pad07() {}
    virtual void pad08() {}
    virtual void pad09() {}
    virtual void pad10() {}
    virtual void pad11() {}
    virtual void pad12() {}
    virtual void pad13() {}
    virtual void pad14() {}
    virtual void pad15() {}
    virtual void pad16() {}
    virtual void pad17() {}
    virtual int slot12(CThingTrackNode* node) {
        g_seen_self = this;
        g_seen_node = node;
        return 7;
    }
};

struct CThingTrackNode {
    IDrawableObj* DrawIsDrawable();
};

static IDrawableObj g_obj;

IDrawableObj* __fastcall GetDrawableObject(CThingTrackNode* self)
{
    return &g_obj;
}

IDrawableObj* CThingTrackNode::DrawIsDrawable()
{
    IDrawableObj* obj = GetDrawableObject(this);
    obj->slot12(this);
    return obj;
}

int main()
{
    CThingTrackNode node;
    IDrawableObj* r = node.DrawIsDrawable();

    if (r == &g_obj && g_seen_self == &g_obj && g_seen_node == &node)
        printf("00b1ff80_TEST PASS\n");
    else
        printf("FAIL r=%p self=%p node=%p\n", (void*)r, g_seen_self, (void*)g_seen_node);
    return 0;
}