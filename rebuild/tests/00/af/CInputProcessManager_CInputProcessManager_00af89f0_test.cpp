#include <cstdio>
struct CInputProcessManager { void* vptr; };
static int g_sub=0, g_base=0;
void SubCleanup(CInputProcessManager* s){ g_sub++; }
void BaseDtor(CInputProcessManager* s){ g_base++; }
void DtorModel(CInputProcessManager* self){
    self->vptr=(void*)0x0129f644;
    SubCleanup(self);
    BaseDtor(self);
}
int main(){
    CInputProcessManager o; o.vptr=0;
    DtorModel(&o);
    if(o.vptr==(void*)0x0129f644 && g_sub==1 && g_base==1) printf("OK_00af89f0\n");
    else { printf("FAIL\n"); return 1; }
    return 0;
}