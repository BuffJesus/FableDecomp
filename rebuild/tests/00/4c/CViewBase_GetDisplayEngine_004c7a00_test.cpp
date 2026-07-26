#include <cstdio>
struct CDisplayEngine { int tag; };
struct CViewBase {};
struct GlobalHolder { char pad[0x30]; CDisplayEngine* engine; };
GlobalHolder g_holder;
GlobalHolder* g_displayHolder = &g_holder;
CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(CViewBase* self)
{
    return g_displayHolder->engine;
}
int main()
{
    CDisplayEngine de_obj;
    de_obj.tag = 43981;
    CDisplayEngine* p = &de_obj;
    g_holder.engine = p;
    CViewBase vb_obj;
    CViewBase* pvb = &vb_obj;
    CDisplayEngine* r = CViewBase_GetDisplayEngine(pvb);
    if (r == p && r->tag == 43981) {
        std::printf("CViewBase_004c7a00_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}