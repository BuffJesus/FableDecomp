#include <cstdio>

int g_ret;
extern "C" int __fastcall sub_a01b50(void* p){ (void)p; return g_ret; }

// behavioural model of IsTaskAvailable: bool = (sub_a01b50(this+0x10) != 0)
bool model(int r){ g_ret=r; return r != 0; }

int main(){
    if(model(0)!=false) { printf("FAIL zero\n"); return 1; }
    if(model(5)!=true)  { printf("FAIL nonzero\n"); return 1; }
    if(model(-3)!=true) { printf("FAIL neg\n"); return 1; }
    printf("OK_0065ee50\n");
    return 0;
}