#include <cstdio>

struct CDisplayEngine {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30();
    virtual bool v34();
};

struct CViewBase {
    CDisplayEngine *GetDisplayEngine();
};

struct CThingTrackNode : CViewBase {
    bool DrawIsDrawable();
};

// -- test doubles --

static bool g_v34_called = false;
static bool g_v34_result = true;

struct FakeDisplayEngine : CDisplayEngine {
};

// Provide the out-of-line bodies the linker needs for this translation unit.
void CDisplayEngine::v00() {}
void CDisplayEngine::v04() {}
void CDisplayEngine::v08() {}
void CDisplayEngine::v0c() {}
void CDisplayEngine::v10() {}
void CDisplayEngine::v14() {}
void CDisplayEngine::v18() {}
void CDisplayEngine::v1c() {}
void CDisplayEngine::v20() {}
void CDisplayEngine::v24() {}
void CDisplayEngine::v28() {}
void CDisplayEngine::v2c() {}
void CDisplayEngine::v30() {}
bool CDisplayEngine::v34() { g_v34_called = true; return g_v34_result; }

static FakeDisplayEngine g_engine;

CDisplayEngine *CViewBase::GetDisplayEngine()
{
    return &g_engine;
}

bool CThingTrackNode::DrawIsDrawable()
{
    return GetDisplayEngine()->v34();
}

int main()
{
    CThingTrackNode node;

    g_v34_called = false;
    g_v34_result = true;
    bool r1 = node.DrawIsDrawable();

    g_v34_called = false;
    g_v34_result = false;
    bool r2 = node.DrawIsDrawable();

    if (r1 == true && r2 == false && g_v34_called) {
        std::printf("DRAWISDRAWABLE_THUNK_OK\n");
    } else {
        std::printf("DRAWISDRAWABLE_THUNK_FAIL %d %d %d\n", (int)r1, (int)r2, (int)g_v34_called);
    }
    return 0;
}