#include <cstdio>
struct CLBP { int vt0; int pad[9]; int vt28; int rest[10]; };
static int g_deleted=0, g_dtored=0;
static void dtor(CLBP* s){ g_dtored=1; }
static void del(void* p){ g_deleted=1; }
static void* vdd(CLBP* self, int flags){
    *(unsigned int*)((char*)self+0x28)=0x1232484u;
    *(unsigned int*)((char*)self)=0x1230ba0u;
    dtor(self);
    if (flags & 1) del(self);
    return self;
}
int main(){
    CLBP obj;
    void* r0=vdd(&obj,0);
    int d0=g_deleted;
    g_deleted=0; g_dtored=0;
    void* r1=vdd(&obj,1);
    if (r0==&obj && r1==&obj && d0==0 && g_deleted==1 && g_dtored==1){
        std::printf("CLandscapeBackgroundPatch_0044fd16_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}