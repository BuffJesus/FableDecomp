#include <cstdio>
unsigned short g_frame=0;
struct S { char pad[0x36]; unsigned short w36; int M(); };
int S::M(){ unsigned short v=this->w36; return v == g_frame; }
int main(){ S o; o.w36=7; g_frame=7; if(o.M()!=1){ std::printf("00b3aaa0_TEST FAIL\n"); return 1;}
 g_frame=8; if(o.M()!=0){ std::printf("00b3aaa0_TEST FAIL\n"); return 1;}
 std::printf("00b3aaa0_TEST PASS\n"); return 0;}