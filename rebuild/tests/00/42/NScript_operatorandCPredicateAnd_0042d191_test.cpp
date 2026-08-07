#include <cstdio>
static void* g_self=0; static void* g_arg=0;
struct S { int x; void pa_h(void* arg); S* M(void* arg); };
void S::pa_h(void* arg){ g_self=this; g_arg=arg; }
S* S::M(void* arg){ this->pa_h(arg); return this; }
int main(){ S o; int z=1; S* r=o.M(&z);
 if(r!=&o||g_self!=&o||g_arg!=&z){ std::printf("0042d191_TEST FAIL\n"); return 1;}
 std::printf("0042d191_TEST PASS\n"); return 0;}