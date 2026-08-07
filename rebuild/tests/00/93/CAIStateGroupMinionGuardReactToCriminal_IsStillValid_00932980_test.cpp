#include <cstdio>
struct S { char pad[0x14]; char sub[4]; };
static int isv_probe(void* p){ (void)p; return 3; }
static bool f(S* self){ return isv_probe(reinterpret_cast<char*>(self)+0x14) ? true : false; }
int main(){ S o; if(!f(&o)){ std::printf("00932980_TEST FAIL\n"); return 1;}
 std::printf("00932980_TEST PASS\n"); return 0;}