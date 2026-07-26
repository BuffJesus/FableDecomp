#include <cstdio>
struct CDisplayEngine { int tag; };
struct CViewBase;
struct SomeGlobal { char pad[0x28]; CDisplayEngine* engine; };
SomeGlobal g_inst;
SomeGlobal* g_someGlobal = &g_inst;

CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(const CViewBase* self)
{
    (void)self;
    return g_someGlobal->engine;
}

int main()
{
    CDisplayEngine e; e.tag = 0x1234;
    g_inst.engine = &e;
    CDisplayEngine* r = CViewBase_GetDisplayEngine((const CViewBase*)0);
    if (r == &e && r->tag == 0x1234) {
        std::printf("CViewBase_004c79e0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}