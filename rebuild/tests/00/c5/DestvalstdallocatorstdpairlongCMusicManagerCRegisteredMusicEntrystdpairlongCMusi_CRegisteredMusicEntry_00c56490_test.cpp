#include <cstdio>
static int g_clean = 0;
static int g_base = 0;
static void* g_cleanArg = 0;
static void* g_baseArg = 0;
struct Sub { int q; };
struct T { char pad[0x110]; Sub sub; };
static void Sub_Cleanup(Sub* s){ g_clean++; g_cleanArg = s; }
static void T_Base(T* t){ g_base++; g_baseArg = t; }
static void DestVal_110(T* self){ Sub_Cleanup(&self->sub); T_Base(self); }
int main(){
    T obj; 
    DestVal_110(&obj);
    if(g_clean!=1){ printf("FAIL clean\n"); return 1; }
    if(g_base!=1){ printf("FAIL base\n"); return 1; }
    if(g_cleanArg != (void*)&obj.sub){ printf("FAIL cleanArg\n"); return 1; }
    if(g_baseArg != (void*)&obj){ printf("FAIL baseArg\n"); return 1; }
    // offset check: sub must be at +0x110
    if((char*)&obj.sub - (char*)&obj != 0x110){ printf("FAIL offset\n"); return 1; }
    printf("OK_00c56490\n");
    return 0;
}