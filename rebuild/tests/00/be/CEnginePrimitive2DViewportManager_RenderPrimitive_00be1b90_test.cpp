
#include <stdio.h>
typedef int ERendered;
static ERendered RenderPrimitive(void* self, void* prim, int layer, void* chain){ return 0; }
int main(){
    ERendered r = RenderPrimitive((void*)1,(void*)2,3,(void*)4);
    printf(r==0 ? "RENDERPRIM_STUB_OK\n" : "FAIL\n");
    return 0;
}