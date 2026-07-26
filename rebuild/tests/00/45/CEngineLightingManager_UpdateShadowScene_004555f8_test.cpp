
#include <cstdio>
struct CShadowTarget28 { void Do(int v); };
struct CLocalScene { int state; void Run(void* obj24, int* arg); };
struct CEngineLightingManager { char pad0[0x18]; int mode; char pad1c[0x24-0x1c]; void* obj24; CShadowTarget28* obj28; };
static int g_called=0, g_v=0, g_state=-1;
void __stdcall EnterGuard(void* p){ (void)p; g_called|=1; }
void CShadowTarget28::Do(int v){ g_v=v; g_called|=2; }
void CLocalScene::Run(void* obj24, int* arg){ (void)obj24;(void)arg; g_state=this->state; g_called|=4; }
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int, int* arg){
    CLocalScene local; EnterGuard((void*)0x122d70e); local.state=0;
    switch(self->mode){ case 2: local.Run(self->obj24,arg); break; case 3: self->obj28->Do(*arg); break; } }
int main(){
    CShadowTarget28 t; CEngineLightingManager o; o.obj28=&t; o.obj24=(void*)7; int a=99;
    o.mode=3; g_called=0;
    CEngineLightingManager_UpdateShadowScene(&o,0,&a);
    if(!(g_called&1)){ std::printf("FAIL guard\n"); return 1; }
    if(!(g_called&2)||g_v!=99){ std::printf("FAIL case3\n"); return 1; }
    o.mode=2; g_called=0;
    CEngineLightingManager_UpdateShadowScene(&o,0,&a);
    if(!(g_called&4)||g_state!=0){ std::printf("FAIL case2\n"); return 1; }
    o.mode=9; g_called=0;
    CEngineLightingManager_UpdateShadowScene(&o,0,&a);
    if(g_called&6){ std::printf("FAIL default\n"); return 1; }
    std::printf("CEngineLightingManager_004555f8_TEST PASS\n"); return 0; }