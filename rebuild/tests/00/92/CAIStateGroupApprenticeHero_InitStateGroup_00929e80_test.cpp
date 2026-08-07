#include <cstdio>
struct S { char pad[0x14]; int f14; int f18; char pad2[0x20-0x1c]; int f20; char pad3[0x2d-0x24]; unsigned char b2d; void M(int a, int b); };
void S::M(int a, int b){ this->f14=0; this->f18=0; this->f20=0; this->b2d=0; }
int main(){ S o; o.f14=1;o.f18=1;o.f20=1;o.b2d=1; o.M(0,0);
 if(o.f14||o.f18||o.f20||o.b2d){ std::printf("00929e80_TEST FAIL\n"); return 1;}
 std::printf("00929e80_TEST PASS\n"); return 0;}