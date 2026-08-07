#include <cstdio>
static int g_v=0; static char* g_p=0;
struct Sub { void M(int* pv); };
struct S { char pad[72]; void Add(int v); };
void Sub::M(int* pv){ g_p=(char*)this; g_v=*pv; }
void S::Add(int v){ ((Sub*)((char*)this+72))->M(&v); }
int main(){ S o; o.Add(55);
 if(g_v!=55||g_p!=(char*)&o+72){ std::printf("00a2e960_TEST FAIL\n"); return 1;}
 std::printf("00a2e960_TEST PASS\n"); return 0;}