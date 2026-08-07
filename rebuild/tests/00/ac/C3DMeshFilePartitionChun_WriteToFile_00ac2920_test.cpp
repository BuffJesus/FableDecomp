#include <cstdio>
static void* g_a=0; static char* g_this=0;
struct Sub { void N(void* a); };
struct S { char pad[0x10]; Sub* f10; void WriteToFile(void* a, int b); };
void Sub::N(void* a){ g_this=(char*)this; g_a=a; }
void S::WriteToFile(void* a, int b){ this->f10->N(a); }
int main(){ Sub sb; S o; o.f10=&sb; o.WriteToFile((void*)88,0);
 if(g_this!=(char*)&sb||g_a!=(void*)88){ std::printf("00ac2920_TEST FAIL\n"); return 1;}
 std::printf("00ac2920_TEST PASS\n"); return 0;}