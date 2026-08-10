#include <cstdio>
struct CInputProcessManager { void* vptr; };
static int g_calls=0; static void* g_vwrite=0;
void model_sub(void* s){ g_calls++; }
void model_base(void* s){ g_calls++; }
void DtorModel(CInputProcessManager* self){
    self->vptr=(void*)0x129ba20;
    g_vwrite=self->vptr;
    model_sub(self);
    model_base(self);
}
int main(){
    CInputProcessManager o; o.vptr=0;
    DtorModel(&o);
    if(g_vwrite==(void*)0x129ba20 && g_calls==2 && o.vptr==(void*)0x129ba20)
        printf("OK_00b564c0\n");
    else { printf("FAIL\n"); return 1; }
    return 0;
}