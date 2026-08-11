#include <cstdio>

struct CThingTrackNode;

static CThingTrackNode* g_seen_node = 0;
static void* g_seen_self = 0;

struct IDrawableBase {
    virtual int pad00(){return 0;}
    virtual int pad01(){return 0;}
    virtual int pad02(){return 0;}
    virtual int pad03(){return 0;}
    virtual int pad04(){return 0;}
    virtual int pad05(){return 0;}
    virtual int pad06(){return 0;}
    virtual int pad07(){return 0;}
    virtual int pad08(){return 0;}
    virtual int pad09(){return 0;}
    virtual int pad10(){return 0;}
    virtual int pad11(){return 0;}
    virtual int pad12(){return 0;}
    virtual int pad13(){return 0;}
    virtual int pad14(){return 0;}
    virtual int pad15(){return 0;}
    virtual int pad16(){return 0;}
    virtual int pad17(){return 0;}
    virtual int slot18(CThingTrackNode* node) {
        g_seen_self = this;
        g_seen_node = node;
        return 7;
    }
};

static IDrawableBase g_obj;

IDrawableBase* __fastcall GetDrawableObject(CThingTrackNode* self)
{
    return &g_obj;
}

struct CThingTrackNode {
    IDrawableBase* DrawIsDrawable();
};

IDrawableBase* CThingTrackNode::DrawIsDrawable()
{
    IDrawableBase* obj = GetDrawableObject(this);
    obj->slot18(this);
    return obj;
}

int main()
{
    CThingTrackNode node;
    IDrawableBase* r = node.DrawIsDrawable();

    if (r == &g_obj && g_seen_self == &g_obj && g_seen_node == &node)
        printf("00b15b00_TEST PASS\n");
    else
        printf("FAIL r=%p self=%p node=%p\n", (void*)r, g_seen_self, (void*)g_seen_node);
    return 0;
}