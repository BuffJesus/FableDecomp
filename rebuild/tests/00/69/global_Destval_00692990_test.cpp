#include <cstdio>
static void* g1=0,*g2=0;
static void* dv_h1(void* p){ g1=p; return (void*)0x99; }
static void dv_h2(void* p){ g2=p; }
static void Dest_val(void* self){ dv_h2(dv_h1(reinterpret_cast<char*>(self)+8)); }
int main(){ char blk[16]; Dest_val(blk); if(g1!=blk+8||g2!=(void*)0x99){ std::printf("00692990_TEST FAIL\n"); return 1;}
 std::printf("00692990_TEST PASS\n"); return 0;}