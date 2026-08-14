#include <cstdio>
struct Handle { int id; };
struct Prim { int v; };
struct Target { int calls; unsigned long lastx; int lastflag; Handle* lh; Prim* lp; };
static Target g_obj = {0,0,0,0,0};
static Target* g_target = &g_obj;

void real_set(Target* self, Handle* h, Prim* p, unsigned long x, int flag){
    self->calls++; self->lastx = x; self->lastflag = flag; self->lh = h; self->lp = p;
}
void SetPersistentPrimitive(Handle& h, Prim const& p, unsigned long x){
    real_set(g_target, &h, (Prim*)&p, x, 0);
}
int main(){
    Handle h={7}; Prim p={9};
    SetPersistentPrimitive(h, p, 12345);
    if(g_obj.calls==1 && g_obj.lastx==12345 && g_obj.lastflag==0 && g_obj.lh==&h && g_obj.lp==&p)
        printf("PARITY_OK\n");
    return 0;
}