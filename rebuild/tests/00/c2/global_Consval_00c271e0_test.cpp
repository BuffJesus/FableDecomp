#include <cstdio>
struct Pair; struct Alloc;
static void* g_v=0; static double cons_ctor(Pair* val){ g_v=val; return 1.0; }
static void Cons_val(Alloc* self, Pair* dst, Pair* val){ (void)self; (void)dst; cons_ctor(val); }
int main(){ Pair* v=(Pair*)0x99; Cons_val((Alloc*)0x11,(Pair*)0x22,v);
 if(g_v!=v){ std::printf("00c271e0_TEST FAIL\n"); return 1;}
 std::printf("00c271e0_TEST PASS\n"); return 0;}