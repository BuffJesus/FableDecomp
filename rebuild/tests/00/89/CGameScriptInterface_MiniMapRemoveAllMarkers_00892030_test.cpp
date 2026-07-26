#include <cstdio>
struct CObj; struct CSub;
static int g_flag = 0;
CObj* g_obj = (CObj*)0x1;
extern "C" CSub* __fastcall get_sub(CObj* self){ (void)self; return (CSub*)0x2; }
extern "C" void __fastcall sub_method(CSub* self){ if(self==(CSub*)0x2) g_flag=1; }

void __fastcall CGameScriptInterface_MiniMapRemoveAllMarkers(void* self)
{
    (void)self;
    CSub* s = get_sub(g_obj);
    sub_method(s);
}

int main(){
    CGameScriptInterface_MiniMapRemoveAllMarkers((void*)0);
    if(g_flag==1){ std::printf("CGameScriptInterface_00892030_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}