#include <cstdio>
struct Sub { int x; };
struct Self { char pad[0x114]; Sub m114; };
static int g_sub=0, g_other=0; static Sub* g_subarg=0; static Self* g_otherarg=0;
extern "C" void __fastcall SubClean(Sub* s){ g_sub++; g_subarg=s; }
extern "C" void __fastcall OtherClean(Self* s){ g_other++; g_otherarg=s; }
extern "C" void __fastcall DestVal_114_A(Self* self, void* edx);
int main(){
    Self o; 
    DestVal_114_A(&o, 0);
    if(g_sub!=1) { printf("FAIL sub\n"); return 1; }
    if(g_other!=1) { printf("FAIL other\n"); return 1; }
    if(g_subarg != &o.m114) { printf("FAIL subarg\n"); return 1; }
    if(g_otherarg != &o) { printf("FAIL otherarg\n"); return 1; }
    printf("OK_008c3a50\n");
    return 0;
}