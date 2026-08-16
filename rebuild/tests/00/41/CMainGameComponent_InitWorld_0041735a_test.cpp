#include <stdio.h>
#include <stdlib.h>
static int g_ctor=0,g_del=0,g_load=0,g_save=0,g_prog=0,g_vtbl=0;
struct Ctx { void* comp; int z4; void* f1c; void* dm; int z10; };
struct World { int tag; void vtbl9(){ ++g_vtbl; } };
static World* ctor(World* w, Ctx* c){ (void)c; w->tag=0x1234; ++g_ctor; return w; }
struct CountedPtr { World* ptr; void DeleteData(World* w){ ptr=w; ++g_del; } };
struct Comp {
    void* f1c;
    CountedPtr f24;
    void InitWorld(unsigned char flag1, unsigned char flag2);
};
static void* dmget(){ static int x; return &x; }
static void* gptr(){ static int y; return &y; }
void Comp::InitWorld(unsigned char flag1, unsigned char flag2){
    Ctx ctx; ctx.z4=0; ctx.dm=0; ctx.z10=0;
    ctx.comp=this; ctx.f1c=this->f1c; ctx.dm=dmget();
    World* w=(World*)malloc(0x198);
    if (w) w=ctor(w,&ctx); else w=0;
    this->f24.DeleteData(w);
    if (flag1){ (void)gptr(); ++g_load; }
    if (flag2){ (void)gptr(); ++g_save; }
    { char s[4]; (void)s; ++g_prog; }
    this->f24.ptr->vtbl9();
}
int main(){
    Comp c; c.f1c=(void*)0x11;
    c.InitWorld(1,1);
    int ok = g_ctor==1 && g_del==1 && g_load==1 && g_save==1 && g_prog==1 && g_vtbl==1
          && c.f24.ptr!=0 && c.f24.ptr->tag==0x1234;
    printf(ok?"INITWORLD_OK\n":"FAIL\n");
    return 0;
}
