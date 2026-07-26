
#include <cstdio>
extern "C" void __stdcall LogShadowScene(int tag);

struct Local4 { int v; void Consume(void* obj24, int* param); };
struct Callee28 { void Fire(int a); };

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;      // +0x18
    char pad1c[0x24 - 0x1c];
    void* obj24;   // +0x24
    Callee28* obj28;   // +0x28
};

static int g_log=0, g_a=0, g_b=0;
extern "C" void __stdcall LogShadowScene(int tag){ g_log=tag; }
void Local4::Consume(void* obj24, int* param){ (void)obj24; g_b=*param; }
void Callee28::Fire(int a){ g_a=a; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* param)
{
    Local4 local;
    LogShadowScene(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Consume(self->obj24, param);
        break;
    case 3:
        self->obj28->Fire(*param);
        break;
    }
}

int main(){
    CEngineLightingManager m;
    for(int i=0;i<(int)sizeof(m);i++) ((char*)&m)[i]=0;
    Callee28 c; m.obj28=&c;
    int p=77;
    m.mode=3;
    CEngineLightingManager_UpdateShadowScene(&m,0,&p);
    if(g_log!=0x122d70e){std::printf("FAIL log\n");return 1;}
    if(g_a!=77){std::printf("FAIL a\n");return 1;}
    m.mode=2; g_b=0;
    CEngineLightingManager_UpdateShadowScene(&m,0,&p);
    if(g_b!=77){std::printf("FAIL b\n");return 1;}
    m.mode=1; g_a=g_b=0;
    CEngineLightingManager_UpdateShadowScene(&m,0,&p);
    if(g_a||g_b){std::printf("FAIL default\n");return 1;}
    std::printf("CEngineLightingManager_00452045_TEST PASS\n");
    return 0;
}