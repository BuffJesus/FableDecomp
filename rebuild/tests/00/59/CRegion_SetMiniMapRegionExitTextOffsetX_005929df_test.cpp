#include <cstdio>
static void* g_in=0;
extern "C" void* __cdecl sub(void* x){ g_in=x; return (void*)0x99; }
struct S { void* f0; S* M(); };
S* S::M(){ this->f0 = sub(this->f0); return this; }
int main(){ S o; o.f0=(void*)0x55; S* r=o.M();
 if(r!=&o||g_in!=(void*)0x55||o.f0!=(void*)0x99){ std::printf("005929df_TEST FAIL\n"); return 1;}
 std::printf("005929df_TEST PASS\n"); return 0;}