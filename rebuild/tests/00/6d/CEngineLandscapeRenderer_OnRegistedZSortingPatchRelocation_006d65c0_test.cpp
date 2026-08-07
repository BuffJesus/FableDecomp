#include <cstdio>
struct S { char pad[0x44]; int* f44; void M(int a0, int a1); };
void S::M(int a0, int a1){ this->f44[a0] = a1; }
int main(){ int arr[4]={0}; S o; o.f44=arr; o.M(2,99);
 if(arr[2]!=99){ std::printf("006d65c0_TEST FAIL\n"); return 1;}
 std::printf("006d65c0_TEST PASS\n"); return 0;}