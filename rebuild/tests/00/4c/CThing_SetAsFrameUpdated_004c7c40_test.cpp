#include <cstdio>
struct CThing { unsigned char pad[0x91]; unsigned char flags; };
static int g_calls = 0;
static CThing* g_last = 0;
struct GlobalMgr { void Notify(CThing* t){ g_calls++; g_last=t; } };
static GlobalMgr mgr;
static GlobalMgr* g_mgr = &mgr;
void model_SetAsFrameUpdated(CThing* self){
    if(!(self->flags & 1)) g_mgr->Notify(self);
}
int main(){
    CThing a; a.flags=0;
    model_SetAsFrameUpdated(&a);
    if(g_calls!=1 || g_last!=&a){ printf("FAIL1\n"); return 1; }
    CThing b; b.flags=1;
    model_SetAsFrameUpdated(&b);
    if(g_calls!=1){ printf("FAIL2\n"); return 2; }
    CThing c; c.flags=3;
    model_SetAsFrameUpdated(&c);
    if(g_calls!=1){ printf("FAIL3\n"); return 3; }
    CThing d; d.flags=2;
    model_SetAsFrameUpdated(&d);
    if(g_calls!=2 || g_last!=&d){ printf("FAIL4\n"); return 4; }
    printf("OK_004c7c40\n");
    return 0;
}