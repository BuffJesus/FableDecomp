#include <cstdio>

struct CTeleportState { char pad[0xd5]; bool active; };
struct CGameScriptInterface { void* vtbl; CTeleportState* state; };

bool __fastcall CGameScriptInterface_IsTeleportingActive(const CGameScriptInterface* self)
{
    return self->state->active;
}

int main()
{
    CTeleportState st;
    st.active = true;
    CGameScriptInterface gsi;
    gsi.vtbl = 0;
    gsi.state = &st;
    if (CGameScriptInterface_IsTeleportingActive(&gsi) != true) { std::printf("FAIL true\n"); return 1; }
    st.active = false;
    if (CGameScriptInterface_IsTeleportingActive(&gsi) != false) { std::printf("FAIL false\n"); return 1; }
    std::printf("CGameScriptInterface_0088f420_TEST PASS\n");
    return 0;
}