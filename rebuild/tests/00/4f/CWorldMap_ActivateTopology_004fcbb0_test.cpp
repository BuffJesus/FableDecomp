#include <cstdio>
struct E { char pad[0x26]; unsigned char b26; char pad2[72-0x27]; };
struct S { char pad[0x20]; E* f20; void M(int a0); };
void S::M(int a0){ this->f20[a0].b26 = 1; }
int main(){ E arr[3]; arr[2].b26=0; S o; o.f20=arr; o.M(2);
 if(arr[2].b26!=1){ std::printf("004fcbb0_TEST FAIL\n"); return 1;}
 std::printf("004fcbb0_TEST PASS\n"); return 0;}