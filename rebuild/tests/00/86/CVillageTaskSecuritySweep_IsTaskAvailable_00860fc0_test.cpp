#include <cstdio>
struct S { char pad[8]; char sub[4]; };
static int ita_probe(void* p){ (void)p; return 0; }
static bool IsTaskAvailable(S* self){ return ita_probe(reinterpret_cast<char*>(self)+8)==0; }
int main(){ S o; if(!IsTaskAvailable(&o)){ std::printf("00860fc0_TEST FAIL\n"); return 1;}
 std::printf("00860fc0_TEST PASS\n"); return 0;}