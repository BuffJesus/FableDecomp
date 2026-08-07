#include <cstdio>
struct E { char pad[8]; void* fld; char pad2[0x2c-(8+4)]; };
struct S { char pad[4]; E* f4; void* M(int a0); };
void* S::M(int a0){ return this->f4[a0].fld; }
int main(){ E arr[3]; int z=1; arr[2].fld=&z; S o; o.f4=arr;
 if(o.M(2)!=&z){ std::printf("00f36f10_TEST FAIL\n"); return 1;}
 std::printf("00f36f10_TEST PASS\n"); return 0;}