#include <cstdio>
struct S { char pad[4]; long long* f4; long long M(int a0); };
long long S::M(int a0){ return this->f4[a0]; }
int main(){ long long arr[3]={0,0,0x1122334455667788LL}; S o; o.f4=arr;
 if(o.M(2)!=0x1122334455667788LL){ std::printf("00c442d0_TEST FAIL\n"); return 1;}
 std::printf("00c442d0_TEST PASS\n"); return 0;}