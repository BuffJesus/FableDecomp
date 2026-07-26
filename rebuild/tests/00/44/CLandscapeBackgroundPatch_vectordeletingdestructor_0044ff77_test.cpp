#include <cstdio>
static int g_dtor=0, g_del=0, g_vt1;
struct Obj { void* vptr; };
void* vecdd(Obj* self, unsigned int flags){
    self->vptr = (void*)&g_vt1;
    g_dtor++;
    if (flags & 1) g_del++;
    return self;
}
int main(){
    Obj o; o.vptr=0;
    void* r = vecdd(&o, 0);
    if (r != &o) { std::printf("FAIL ret\n"); return 1; }
    if (o.vptr != (void*)&g_vt1) { std::printf("FAIL vptr\n"); return 1; }
    if (g_dtor != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_del != 0) { std::printf("FAIL nodel\n"); return 1; }
    vecdd(&o, 1);
    if (g_del != 1) { std::printf("FAIL del\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044ff77_TEST PASS\n");
    return 0;
}