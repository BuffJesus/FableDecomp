#include <cstdio>
static void* g_a=0;
struct S { char pad[0xC]; char* fC; void OnShowTrophyInitialise(void* arg); void Helper(void* a); };
void S::Helper(void* a){ g_a=a; }
void S::OnShowTrophyInitialise(void* arg){ (void)arg; this->Helper(this->fC + 0x4c); }
int main(){ char buf[0x60]; S o; o.fC=buf; o.OnShowTrophyInitialise((void*)0x1);
 if(g_a!=buf+0x4c){ std::printf("006de8b0_TEST FAIL\n"); return 1;}
 std::printf("006de8b0_TEST PASS\n"); return 0;}