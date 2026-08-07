#include <cstdio>
struct S { char pad[896]; int* arr; long GetAbilityLevel(int ab); };
long S::GetAbilityLevel(int ab){ return this->arr[ab]; }
int main(){ int a[4]={0,0,9,0}; S o; o.arr=a;
 if(o.GetAbilityLevel(2)!=9){ std::printf("00a24290_TEST FAIL\n"); return 1;}
 std::printf("00a24290_TEST PASS\n"); return 0;}