#include <cstdio>
struct Cat {
    int x;
    void ctor(int arg);
    void dtor();
};
struct Mgr {
    void remove(Cat* c);
};
static int g_step=0;
static int g_ctorArg=-1;
static Cat* g_removed=0;
static Cat* g_dtored=0;
static void* g_mgr=0;
void Cat::ctor(int arg){ x=arg; g_ctorArg=arg; g_step=1; }
void Cat::dtor(){ g_dtored=this; g_step=3; }
void Mgr::remove(Cat* c){ g_mgr=this; g_removed=c; g_step=2; }

void __fastcall CGameScriptInterface_RemoveRumourCategory(void* self, int edx, int arg)
{
    Cat local;
    local.ctor(arg);
    ((Mgr*)0x13bae44)->remove(&local);
    local.dtor();
}

int main(){
    CGameScriptInterface_RemoveRumourCategory((void*)0, 0, 77);
    if(g_ctorArg==77 && g_removed!=0 && g_removed==g_dtored && g_mgr==(void*)0x13bae44 && g_step==3){
        std::printf("CGameScriptInterface_008a9e00_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL step=%d arg=%d\n", g_step, g_ctorArg);
    return 1;
}