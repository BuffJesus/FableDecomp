#include <cstdio>
static int g_setup=0, g_a=0, g_b=0, g_local=-1;
static void* g_a_arg=0; static void* g_b_a=0; static void** g_b_b=0;

void __stdcall Setup(const void* p){ g_setup=1; }

struct CObjA { void DoA(void* arg){ g_a=1; g_a_arg=arg; } };
struct TLocal { int v; void DoB(void* a, void** b){ g_b=1; g_local=v; g_b_a=a; g_b_b=b; } };

struct CEngineLightingManager {
    char pad[0x18];
    int field_18;
    char pad2[0x24-0x1c];
    CObjA* field_24;
    CObjA* field_28;
    void UpdateShadowScene(void** pArg);
};

void CEngineLightingManager::UpdateShadowScene(void** pArg)
{
    TLocal local;
    Setup((const void*)0x122d70e);
    local.v = 0;
    switch (field_18) {
    case 2:
        local.DoB(field_24, pArg);
        break;
    case 3:
        field_28->DoA(*pArg);
        break;
    }
}

int main(){
    CEngineLightingManager m; for(int i=0;i<(int)sizeof(m);i++) ((char*)&m)[i]=0;
    CObjA a24, a28; m.field_24=&a24; m.field_28=&a28;
    void* dummy=(void*)0x1234; void* p=&dummy;
    m.field_18=2; g_setup=g_a=g_b=0; g_local=-1;
    m.UpdateShadowScene((void**)&p);
    if(!(g_setup&&g_b&&!g_a&&g_local==0&&g_b_a==(void*)&a24&&g_b_b==(void**)&p)){ std::printf("FAIL case2\n"); return 1; }
    m.field_18=3; g_setup=g_a=g_b=0;
    m.UpdateShadowScene((void**)&p);
    if(!(g_setup&&g_a&&!g_b&&g_a_arg==(void*)&dummy)){ std::printf("FAIL case3\n"); return 1; }
    m.field_18=5; g_setup=g_a=g_b=0;
    m.UpdateShadowScene((void**)&p);
    if(!(g_setup&&!g_a&&!g_b)){ std::printf("FAIL default\n"); return 1; }
    std::printf("CEngineLightingManager_004526f7_TEST PASS\n");
    return 0;
}