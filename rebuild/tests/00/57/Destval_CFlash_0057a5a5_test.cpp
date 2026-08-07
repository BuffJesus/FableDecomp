#include <cstdio>
int g_glob=0; static char* g_this=0; static int* g_p=0;
struct Sub { void M(int* p); };
struct S { char pad[0x170]; void Dtor(); };
void Sub::M(int* p){ g_this=(char*)this; g_p=p; }
void S::Dtor(){ ((Sub*)((char*)this+0x170))->M(&g_glob); }
int main(){ static S o; o.Dtor();
 if(g_this!=(char*)&o+0x170||g_p!=&g_glob){ std::printf("0057a5a5_TEST FAIL\n"); return 1;}
 std::printf("0057a5a5_TEST PASS\n"); return 0;}