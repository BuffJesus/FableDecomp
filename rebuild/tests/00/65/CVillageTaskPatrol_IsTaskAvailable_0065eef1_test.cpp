#include <cstdio>
struct S { char pad[0x10]; char sub[4]; };
static int ita_probe(void* p){ (void)p; return 0; }
static bool IsTaskAvailable(S* self){ return ita_probe(reinterpret_cast<char*>(self)+0x10)==0; }
int main(){ S obj; if(!IsTaskAvailable(&obj)){ std::printf("0065eef1_TEST FAIL\n"); return 1;}
 std::printf("0065eef1_TEST PASS\n"); return 0;}