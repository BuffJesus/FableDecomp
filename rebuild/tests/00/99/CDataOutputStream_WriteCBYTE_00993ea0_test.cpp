#include <cstdio>
static int g_n=0; static void* g_p=0;
struct S { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){}
           virtual void Write(void* p, int n){ g_p=p; g_n=n; } void WriteCBYTE(char c); };
void S::WriteCBYTE(char c){ this->Write(&c, 4); }
int main(){ S o; o.WriteCBYTE((char)0x5a); if(g_n!=4){ std::printf("00993ea0_TEST FAIL\n"); return 1;}
 std::printf("00993ea0_TEST PASS\n"); return 0;}