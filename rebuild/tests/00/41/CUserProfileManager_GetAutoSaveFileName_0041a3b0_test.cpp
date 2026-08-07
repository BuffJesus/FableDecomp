#include <cstdio>
const char g_str[]="x";
static char* g_p=0; static const char* g_s=0;
struct S { int x; void N(const char* s); S* M(); };
void S::N(const char* s){ g_p=(char*)this; g_s=s; }
S* S::M(){ this->N(g_str); return this; }
int main(){ S o; S* r=o.M();
 if(r!=&o||g_p!=(char*)&o||g_s!=g_str){ std::printf("0041a3b0_TEST FAIL\n"); return 1;}
 std::printf("0041a3b0_TEST PASS\n"); return 0;}