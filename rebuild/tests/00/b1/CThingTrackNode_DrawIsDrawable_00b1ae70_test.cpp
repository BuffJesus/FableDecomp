#include <cstdio>

struct CThingTrackNode;

struct CPrim
{
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual void v08() {}
    virtual void v09() {}
    virtual void v10() {}
    virtual void v11() {}
    virtual void v12() {}
    virtual void v13() {}
    virtual void v14() {}
    virtual void v15() {}
    virtual void v16() {}
    virtual void v17() {}
    virtual void Draw(CThingTrackNode* node) {}
};

struct CThingTrackNode
{
    CPrim* __fastcall DrawIsDrawable();
};

static CThingTrackNode* g_drawnWith = 0;
static CPrim* g_drawnSelf = 0;

struct CPrimImpl : CPrim
{
    void v00() {} void v01() {} void v02() {} void v03() {}
    void v04() {} void v05() {} void v06() {} void v07() {}
    void v08() {} void v09() {} void v10() {} void v11() {}
    void v12() {} void v13() {} void v14() {} void v15() {}
    void v16() {} void v17() {}
    void Draw(CThingTrackNode* node) { g_drawnSelf = this; g_drawnWith = node; }
};

static CPrimImpl g_prim;
static CThingTrackNode* g_createArg = 0;

CPrim* __fastcall CreatePrim(CThingTrackNode* self)
{
    g_createArg = self;
    return &g_prim;
}

CPrim* __fastcall CThingTrackNode::DrawIsDrawable()
{
    CPrim* p = CreatePrim(this);
    p->Draw(this);
    return p;
}

int main()
{
    CThingTrackNode node;
    CPrim* r = node.DrawIsDrawable();
    if (r == &g_prim && g_createArg == &node &&
        g_drawnSelf == &g_prim && g_drawnWith == &node)
        printf("00b1ae70_TEST PASS\n");
    else
        printf("FAIL r=%p create=%p self=%p with=%p\n",
               (void*)r, (void*)g_createArg, (void*)g_drawnSelf, (void*)g_drawnWith);
    return 0;
}