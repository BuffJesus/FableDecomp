#include <cstdio>
static unsigned long g_actionid=100;
struct S { char pad[0x28]; unsigned long f28; };
static void f(S* self){ self->f28=g_actionid; ++g_actionid; }
int main(){ S o; f(&o); if(o.f28!=100||g_actionid!=101){ std::printf("00692450_TEST FAIL\n"); return 1;}
 std::printf("00692450_TEST PASS\n"); return 0;}