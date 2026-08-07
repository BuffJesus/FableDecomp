#include <cstdio>
struct P { char pad[0x2c]; int f2c; char pad3[0x34-0x30]; int f34; };
struct S { char pad[8]; P* f8; char pad2[0x18-0xc]; int f18; void M(); };
void S::M(){ P* p=this->f8; if(p) this->f18 = p->f2c - p->f34; }
int main(){ P p; p.f2c=30; p.f34=8; S o; o.f8=&p; o.f18=0; o.M();
 if(o.f18!=22){ std::printf("009fbf50_TEST FAIL\n"); return 1;}
 o.f8=0; o.f18=5; o.M(); if(o.f18!=5){ std::printf("009fbf50_TEST FAIL\n"); return 1;}
 std::printf("009fbf50_TEST PASS\n"); return 0;}