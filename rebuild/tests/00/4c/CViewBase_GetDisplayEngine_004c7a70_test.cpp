#include <cstdio>
struct CDisplayEngine { int marker; };
struct GlobalRoot { char pad[0x1c]; CDisplayEngine* engine; };
GlobalRoot* g_root;
struct CViewBase {};
CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(CViewBase* self)
{
    (void)self;
    return g_root->engine;
}
int main()
{
    static CDisplayEngine theEngine;
    theEngine.marker = 0x1234;
    static GlobalRoot theRoot;
    CDisplayEngine* pe = &theEngine;
    theRoot.engine = pe;
    GlobalRoot* pr = &theRoot;
    g_root = pr;
    CViewBase vb;
    CViewBase* pv = &vb;
    CDisplayEngine* r = CViewBase_GetDisplayEngine(pv);
    if (r == pe && r->marker == 0x1234) {
        std::printf("CViewBase_004c7a70_TEST PASS\n");
        return 0;
    }
    std::printf("CViewBase_004c7a70_TEST FAIL\n");
    return 1;
}