#include <cstdio>
static unsigned int g_n=0;
extern "C" void* __cdecl helper(unsigned int n){ g_n=n; return (void*)0x1234; }
void* __stdcall operator_new(unsigned int n){ return helper(n); }
int main(){ void* r=operator_new(64);
 if(g_n!=64||r!=(void*)0x1234){ std::printf("00c36080_TEST FAIL\n"); return 1;}
 std::printf("00c36080_TEST PASS\n"); return 0;}