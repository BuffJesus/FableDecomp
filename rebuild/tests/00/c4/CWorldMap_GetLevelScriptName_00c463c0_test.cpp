#include <cstdio>
struct E { char pad[4]; int f4; char pad2[12-8]; };
struct S { char pad[8]; E* f8; int* M(int a0); };
int* S::M(int a0){ return &this->f8[a0].f4; }
int main(){ E arr[3]; S o; o.f8=arr;
 if(o.M(2)!=&arr[2].f4){ std::printf("00c463c0_TEST FAIL\n"); return 1;}
 std::printf("00c463c0_TEST PASS\n"); return 0;}