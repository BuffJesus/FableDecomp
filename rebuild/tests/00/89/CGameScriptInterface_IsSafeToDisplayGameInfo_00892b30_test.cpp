#include <cstdio>
struct CSub { int val; bool Query(bool flag){ return val != 0 && !flag; } };
struct CObj { char pad[0x18]; CSub sub; };
static CObj g_storage;
CObj* g_obj = &g_storage;

bool __fastcall CGameScriptInterface_IsSafeToDisplayGameInfo(void* self)
{
    return g_obj->sub.Query(false);
}

int main(){
    g_storage.sub.val = 5;
    bool r = CGameScriptInterface_IsSafeToDisplayGameInfo(0);
    if(r){ std::printf("CGameScriptInterface_00892b30_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}