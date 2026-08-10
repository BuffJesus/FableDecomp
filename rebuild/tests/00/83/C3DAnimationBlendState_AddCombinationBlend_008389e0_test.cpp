#include <cstdio>

struct C3DAnimationBlendState { int tag; };

static int g_helperCalls = 0;
static C3DAnimationBlendState* g_lastAdded = 0;

struct BlendMgr {
    int addCount;
    void Add(C3DAnimationBlendState* s) { addCount++; g_lastAdded = s; }
};

static BlendMgr realMgr = {0};
BlendMgr* g_theMgr = &realMgr;

extern "C" void __fastcall helper_6a5230(C3DAnimationBlendState* self) {
    g_helperCalls++;
    (void)self;
}

void __fastcall AddCombinationBlend(C3DAnimationBlendState* self)
{
    helper_6a5230(self);
    g_theMgr->Add(self);
}

int main() {
    C3DAnimationBlendState s; s.tag = 7;
    AddCombinationBlend(&s);
    if (g_helperCalls != 1) { printf("FAIL helper\n"); return 1; }
    if (realMgr.addCount != 1) { printf("FAIL add\n"); return 1; }
    if (g_lastAdded != &s) { printf("FAIL arg\n"); return 1; }

    // second call, different self
    C3DAnimationBlendState s2; s2.tag = 9;
    AddCombinationBlend(&s2);
    if (g_helperCalls != 2) { printf("FAIL helper2\n"); return 1; }
    if (realMgr.addCount != 2) { printf("FAIL add2\n"); return 1; }
    if (g_lastAdded != &s2) { printf("FAIL arg2\n"); return 1; }

    printf("OK_008389e0\n");
    return 0;
}