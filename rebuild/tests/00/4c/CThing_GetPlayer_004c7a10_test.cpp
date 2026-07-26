#include <cstdio>
struct CPlayer;
struct CThing { char pad[0x90]; signed char idx; };
static int g_last_idx;
struct CInner { CPlayer* Get(int idx){ g_last_idx = idx; return (CPlayer*)0; } };
struct COuter { char pad[0x30]; CInner* inner; };
static CInner g_inner_obj;
static COuter g_outer_obj;
COuter* g_outer = &g_outer_obj;

CPlayer* CThing_GetPlayer(CThing* self)
{
    return g_outer->inner->Get(self->idx);
}

int main(){
    CThing t; for(int k=0;k<(int)sizeof(t);++k) ((char*)&t)[k]=0;
    g_outer_obj.inner = &g_inner_obj;
    t.idx = -5;
    CThing_GetPlayer(&t);
    if(g_last_idx != -5){ std::printf("FAIL idx=%d\n", g_last_idx); return 1; }
    t.idx = 7;
    CThing_GetPlayer(&t);
    if(g_last_idx != 7){ std::printf("FAIL idx=%d\n", g_last_idx); return 1; }
    std::printf("CThing_004c7a10_TEST PASS\n");
    return 0;
}