#include <cstdio>

static int g_sub = 0;
static int g_base = 0;
static void* g_sub_self = 0;
static void* g_base_self = 0;

struct Sub { char pad[4]; };
struct Outer { char pad[0x114]; Sub member; };

extern "C" void __fastcall sub_dtor(void* self){ g_sub++; g_sub_self=self; }
extern "C" void __fastcall base_dtor(void* self){ g_base++; g_base_self=self; }

void __fastcall DestVal_114_D(void* self, void* /*edx*/)
{
    Outer* p = (Outer*)self;
    sub_dtor(&p->member);
    base_dtor(self);
}

int main(){
    Outer o;
    DestVal_114_D(&o, 0);
    if(g_sub!=1||g_base!=1) { printf("FAIL counts\n"); return 1; }
    if(g_sub_self != (void*)((char*)&o + 0x114)) { printf("FAIL sub offset\n"); return 1; }
    if(g_base_self != (void*)&o) { printf("FAIL base self\n"); return 1; }
    printf("OK_009531a0\n");
    return 0;
}