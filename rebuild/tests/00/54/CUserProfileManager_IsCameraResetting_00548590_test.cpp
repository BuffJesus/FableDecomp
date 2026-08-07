#include <cstdio>
struct S { char pad[0xc]; unsigned char b0c; char pad_lo[15-0xd]; unsigned char blo; char pad_hi[207-(15+1)]; unsigned char bhi; char M(); };
char S::M(){ if(this->b0c) return this->bhi; return this->blo; }
int main(){ S o; o.blo=3; o.bhi=7;
 o.b0c=1; if(o.M()!=7){ std::printf("00548590_TEST FAIL\n"); return 1;}
 o.b0c=0; if(o.M()!=3){ std::printf("00548590_TEST FAIL\n"); return 1;}
 std::printf("00548590_TEST PASS\n"); return 0;}