#include <cstdio>

struct CDisplayEngine;

struct CDisplayEngineVtbl
{
    void* slot00; void* slot04; void* slot08; void* slot0C;
    void* slot10; void* slot14; void* slot18; void* slot1C;
    void* slot20; void* slot24; void* slot28; void* slot2C;
    bool (__fastcall *DrawIsDrawable)(CDisplayEngine*);
};

struct CDisplayEngine
{
    CDisplayEngineVtbl* vtbl;
};

struct CViewBase {};

struct CThingTrackNode : CViewBase
{
    bool __fastcall DrawIsDrawable();
};

static CDisplayEngine g_engine;
static CDisplayEngineVtbl g_vtbl;
static CThingTrackNode g_node;
static bool g_called = false;

static bool __fastcall FakeDrawIsDrawable(CDisplayEngine* self)
{
    g_called = true;
    return self == &g_engine;
}

CDisplayEngine* __fastcall GetDisplayEngine(CViewBase* self)
{
    (void)self;
    return &g_engine;
}

bool __fastcall CThingTrackNode::DrawIsDrawable()
{
    CDisplayEngine* eng = GetDisplayEngine(this);
    return eng->vtbl->DrawIsDrawable(eng);
}

int main()
{
    g_vtbl.DrawIsDrawable = FakeDrawIsDrawable;
    g_engine.vtbl = &g_vtbl;

    bool result = g_node.DrawIsDrawable();

    if (g_called && result)
        printf("DRAWISDRAWABLE_FORWARD_OK\n");
    else
        printf("DRAWISDRAWABLE_FORWARD_FAIL\n");
    return 0;
}