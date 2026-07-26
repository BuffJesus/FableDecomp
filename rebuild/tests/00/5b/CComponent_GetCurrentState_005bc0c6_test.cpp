#include <cstdio>

struct CUIState { int v; };

static CUIState g_state;
static char* g_seen;

CUIState* __stdcall NUISystem_GetState(char* p)
{
    g_seen = p;
    return &g_state;
}

CUIState* __fastcall CComponent_GetCurrentState(void* self)
{
    char local;
    return NUISystem_GetState(&local);
}

int main()
{
    g_state.v = 42;
    CUIState* r = CComponent_GetCurrentState((void*)0);
    if (r == &g_state && r->v == 42 && g_seen != 0) {
        std::printf("CComponent_005bc0c6_TEST PASS\n");
        return 0;
    }
    std::printf("CComponent_005bc0c6_TEST FAIL\n");
    return 1;
}