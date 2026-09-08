#include "engine/CDisplayViewManager.h"
#include <cstdio>

struct CInterpolationInfo;
struct CViewBase { virtual void Draw(CInterpolationInfo* info, bool b) = 0; };
struct CDisplayViewManager_Methods : CDisplayViewManager {
    void Render(CInterpolationInfo* info, bool b);
};

extern CViewBase* __fastcall GetFallbackView_c31a70(CDisplayViewManager* self);
static int g_drawn = 0;
static bool g_b = false;
static CInterpolationInfo* g_info = 0;
struct View : CViewBase {
    void Draw(CInterpolationInfo* info, bool b) { g_drawn++; g_info = info; g_b = b; }
};
static View g_fallback;
CViewBase* __fastcall GetFallbackView_c31a70(CDisplayViewManager*) { return &g_fallback; }

int main()
{
    CInterpolationInfo* info = (CInterpolationInfo*)0x1234;
    View v; CDisplayViewManager_Methods m; m.PCurrentView_Data = &v;
    m.Render(info, true);
    bool ok1 = g_drawn == 1 && g_b == true && g_info == info &&
               &v == (CViewBase*)m.PCurrentView_Data;
    g_drawn = 0; CDisplayViewManager_Methods m2; m2.PCurrentView_Data = 0;
    m2.Render(info, false);
    bool ok2 = g_drawn == 1 && g_b == false && g_info == info;
    if (ok1 && ok2) printf("00c31b30_TEST PASS\n");
    else printf("FAIL d=%d b=%d\n", g_drawn, (int)g_b);
    return 0;
}
