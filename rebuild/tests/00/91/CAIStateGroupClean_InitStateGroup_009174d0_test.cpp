#include <cstdio>
struct S { char pad[0x14]; unsigned char b14; unsigned char b15; char pad2[0x20-0x16]; int f20; unsigned char b24; void M(int a, int b); };
void S::M(int a, int b){ this->b24=0; this->f20=0; this->b14=0; this->b15=0; }
int main(){ S o; o.b24=1;o.f20=1;o.b14=1;o.b15=1; o.M(0,0);
 if(o.b24||o.f20||o.b14||o.b15){ std::printf("009174d0_TEST FAIL\n"); return 1;}
 std::printf("009174d0_TEST PASS\n"); return 0;}