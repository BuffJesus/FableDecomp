#include <cstdio>
struct S { char pad[0xc]; int f0c; int f10; char pad2[0x24-0x14]; int f24; int M(); };
int S::M(){ return this->f24 - ((this->f10 - this->f0c) >> 2); }
int main(){ S o; o.f0c=4; o.f10=20; o.f24=100;
 if(o.M()!=100-4){ std::printf("007fec10_TEST FAIL %d\n",o.M()); return 1;}
 std::printf("007fec10_TEST PASS\n"); return 0;}