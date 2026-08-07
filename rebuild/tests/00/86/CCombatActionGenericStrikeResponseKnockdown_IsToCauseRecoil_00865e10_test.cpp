#include <cstdio>
static void* g1=0; static bool r_h2(void* p){ (void)p; return true; }
static void* r_h1(void* p){ g1=p; return (void*)0x99; }
static bool f(void* self){ return r_h2(r_h1(reinterpret_cast<char*>(self)+8)); }
int main(){ char blk[16]; if(!f(blk)||g1!=blk+8){ std::printf("00865e10_TEST FAIL\n"); return 1;}
 std::printf("00865e10_TEST PASS\n"); return 0;}