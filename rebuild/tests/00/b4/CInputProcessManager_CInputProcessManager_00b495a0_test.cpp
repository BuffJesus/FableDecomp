
#include <cstdio>
struct CInputProcessManager { void* vtbl; };
static int g_helper=0, g_base=0;
int helper(CInputProcessManager* s){ g_helper++; return 0; }
void basedtor(CInputProcessManager* s){ g_base++; }
void model(CInputProcessManager* s){
    s->vtbl = (void*)0x012a20e4;
    helper(s);
    basedtor(s);
}
int main(){
    CInputProcessManager o; o.vtbl=0;
    model(&o);
    if(o.vtbl==(void*)0x012a20e4 && g_helper==1 && g_base==1)
        printf("OK_00b495a0\n");
    else { printf("FAIL\n"); return 1; }
    return 0;
}